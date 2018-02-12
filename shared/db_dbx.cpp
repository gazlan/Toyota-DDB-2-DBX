/* ******************************************************************** **
** @@ DBF Manager
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Dscr   :
** ******************************************************************** */

/* ******************************************************************** **
**                uses pre-compiled headers
** ******************************************************************** */

#include "stdafx.h"

#include <time.h>
#include <ctype.h>
#include <stdio.h>

#include "xlat.h"
#include "text.h"
#include "file.h"
#include "queue.h"
#include "mmf.h"
#include "timestamp.h"
#include "vector.h"
#include "vector_sorted.h"
#include "db_dbx.h"
#include "xlat_tables.h"
#include "xlat.h"

/* ******************************************************************** **
** @@                   internal defines
** ******************************************************************** */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ******************************************************************** **
** @@                   internal prototypes
** ******************************************************************** */

/* ******************************************************************** **
** @@                   external global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@                   static global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@                   real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ CompareTableNames()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static int __cdecl CompareTableNames
(
   const void** const    pKey1,
   const void** const    pKey2
)
{
   DBX_TABLE*  pTable1 = *(DBX_TABLE**)pKey1;
   DBX_TABLE*  pTable2 = *(DBX_TABLE**)pKey2;

   char*       pName1 = (char*)(pTable1->GetName());
   char*       pName2 = (char*)(pTable2->GetName());

   return stricmp(pName1,pName2);
}

/* ******************************************************************** **
** @@ DefineField()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void DefineField
(
   DBX_COLUMN_DESCRIPTOR*     pFieldArr,
   int                        iFieldNum,
   const char* const          pszTitle,
   DBX_FIELD_TYPE             FieldType,
   DBX_FIELD_FLAGS            FieldFlags,
   DWORD                      dwSize
)
{
   strncpy(pFieldArr[iFieldNum]._pszName,pszTitle,DBF_NAME_MAX_SIZE);
   pFieldArr[iFieldNum]._pszName[DBF_NAME_MAX_SIZE] = 0; // Ensure ASCIIZ

   pFieldArr[iFieldNum]._FieldType  = FieldType;
   pFieldArr[iFieldNum]._FieldFlags = FieldFlags;
   pFieldArr[iFieldNum]._dwSize     = dwSize;
}

/* ******************************************************************** **
** @@ DBX::DBX()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Constructor
** ******************************************************************** */

DBX::DBX()
{
}

/* ******************************************************************** **
** @@ DBX::DBX()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Constructor
** ******************************************************************** */

DBX::DBX(DWORD dwSize,WORD wDelta,bool bDelete)
:  Vector(dwSize,wDelta,bDelete)
{
}

/* ******************************************************************** **
** @@ DBX::~DBX()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Destructor
** ******************************************************************** */

DBX::~DBX()
{
   CloseAll();
}

/* ******************************************************************** **
** @@ DBX::CloseAll()
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */

void DBX::CloseAll()
{
   int   iCnt = Count();

   // Should be int !
   for (int ii = iCnt - 1; ii >= 0; --ii)
   {
      DBX_TABLE*     pTable = GetTable(ii);

      Remove(pTable);

      if (pTable)
      {
         CloseTable(pTable);
      }
   }
}

/* ******************************************************************** **
** @@ DBX::GetLastErr()
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */

int DBX::GetLastErr()
{
   return _iErr;
}

/* ******************************************************************** **
** @@ DBX::CreateEmptyTable()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool DBX::CreateEmptyTable(const DBX_TABLE_INFO* const pInfo)
{
   if (!pInfo)
   {
      // Error !
      ASSERT(0);
      return false;
   }

   if (!pInfo->_pszName || !*pInfo->_pszName)
   {
      // Error !
      ASSERT(0);
      return false;
   }

   char     pszName[MAX_PATH];

   strcpy(pszName,pInfo->_pszName);

   int   iCnt = pInfo->_iCnt;

   int      iHdrSize = sizeof(DBX_HEADER) + sizeof(DBX_FIELD) * iCnt + 1 + 1;   // For EOH Terminator + EOF Marker

   BYTE*    pHeader = new BYTE[iHdrSize]; 

   if (!pHeader)
   {
      // Error !
      ASSERT(0);
      return false;
   }

   memset(pHeader,0,iHdrSize);

   DBX_HEADER*      pDBF_Header = (DBX_HEADER*)pHeader;


   pDBF_Header->_byFileType = (BYTE)pInfo->_FileType;

   time_t         TimeNow     = time(&TimeNow);
   struct tm*     pPackedTime = localtime(&TimeNow);

   pDBF_Header->_byDay   = (BYTE)pPackedTime->tm_mday;
   pDBF_Header->_byMonth = (BYTE)(pPackedTime->tm_mon + 1);
   pDBF_Header->_byYear  = (BYTE)pPackedTime->tm_year;

   pDBF_Header->_dwRecCnt = 0;   
   pDBF_Header->_wRecSize = 1;   // For 'Deleted' marker

   DBX_FIELD*    pDBF_Field = (DBX_FIELD*)(pHeader + sizeof(DBX_HEADER));

   pDBF_Header->_wDataStart = (WORD)(sizeof(DBX_HEADER) + sizeof(DBX_FIELD) * iCnt + 1); // For EOH Terminator

   DWORD    dwDisplacement = 1;  // For 'Deleted' Marker

   for (int ii = 0; ii < iCnt; ++ii)
   {
      strncpy(pDBF_Field->_pszName,pInfo->_pFieldArr[ii]._pszName,DBF_NAME_MAX_SIZE);
      pDBF_Field->_pszName[DBF_NAME_MAX_SIZE - 1] = 0; // ASCIIZ

      pDBF_Field->_byType         = (BYTE)pInfo->_pFieldArr[ii]._FieldType;
      pDBF_Field->_dwDisplacement = dwDisplacement;
      pDBF_Field->_bySize         = (BYTE)pInfo->_pFieldArr[ii]._dwSize;
      pDBF_Field->_byFlags        = (BYTE)pInfo->_pFieldArr[ii]._FieldFlags;

      dwDisplacement += pDBF_Field->_bySize;

      ++pDBF_Field;

      #pragma warning(disable: 4244)   // '+=' : conversion from 'const unsigned long' to 'unsigned short', possible loss of data
      pDBF_Header->_wRecSize += pInfo->_pFieldArr[ii]._dwSize;
   }

   pHeader[iHdrSize - 2] = DBF_HEADER_TERMINATOR;
   pHeader[iHdrSize - 1] = DBF_EOF_MARKER;

   HANDLE      hFile = CreateFile(pszName);

   if (hFile == INVALID_HANDLE_VALUE)
   {
      delete pHeader;
      pHeader = NULL;
      return false;
   }

   if (!WriteBuffer(hFile,pHeader,iHdrSize))
   {
      delete pHeader;
      pHeader = NULL;
      CloseHandle(hFile);
      hFile = INVALID_HANDLE_VALUE;
      return false;
   }

   delete pHeader;
   pHeader = NULL;

   CloseHandle(hFile);
   hFile = INVALID_HANDLE_VALUE;

   return true;
}

/* ******************************************************************** **
** @@ DBX::CreateEmptyMemo()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool DBX::CreateEmptyMemo(const char* const pszName,DWORD dwGranulation)
{
   HANDLE   hMemo = CreateFile(pszName);

   if (hMemo == INVALID_HANDLE_VALUE)
   {
      // Error !
      ASSERT(0);
      return false;
   }

   if (!WriteBuffer(hMemo,&dwGranulation,sizeof(DWORD)))
   {
      CloseHandle(hMemo);
      hMemo = INVALID_HANDLE_VALUE;
      return false;
   }

   DWORD    dwFreeListHead = 0;

   if (!WriteBuffer(hMemo,&dwFreeListHead,sizeof(DWORD)))
   {
      CloseHandle(hMemo);
      hMemo = INVALID_HANDLE_VALUE;
      return false;
   }

   CloseHandle(hMemo);
   hMemo = INVALID_HANDLE_VALUE;

   return true;
}

/* ******************************************************************** **
** @@ DBX::OpenTable()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DBX_TABLE* DBX::OpenTable
(
   const char* const       pszTableName,
   const char* const       pszFixName,
   const char* const       pszVarName,
   DBX_OPEN_MODE           FixOpenMode,
   DBX_OPEN_MODE           VarOpenMode,
   DBX_FLUSH_MODE          FixFlushMode,
   DBX_FLUSH_MODE          VarFlushMode
)
{  
   if (FixOpenMode != DBX_OM_NONE)
   {
      if (pszFixName && *pszFixName)
      {
         DBX_TABLE*     pTable = GetTableIfOpen(pszTableName);

         if (pTable)
         {
            return pTable;
         }
      
         pTable = new DBX_TABLE;

         if (!pTable)
         {
            // Error !
            ASSERT(0);
            return NULL;
         }

         pTable->SetName(pszTableName);

         if (!pTable->Open(pszTableName,pszFixName,pszVarName,FixOpenMode,VarOpenMode,FixFlushMode,VarFlushMode))
         {
            // Error !
            ASSERT(0);
            return NULL;
         }

         int   iIndex = Insert(pTable);

         if (iIndex == -1)
         {
            // Error !
            ASSERT(0);
            CloseTable(pTable);
            return NULL;
         }

         pTable->SetIndex(iIndex);

         return (DBX_TABLE*)pTable;
      }
   }

   // Error !
   ASSERT(0);
   return NULL;
}

/* ******************************************************************** **
** @@ DBX::RenameTable()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool DBX::RenameTable(const char* const pszOldName,const char* const pszNewName)
{
   return false;
}

/* ******************************************************************** **
** @@ DBX::ReIndexTable()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool DBX::ReIndexTable(const char* const pszName)
{
   return false;
}

/* ******************************************************************** **
** @@ DBX::ReduceTable()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 27 Jan 2004
** @  Notes  :
** ******************************************************************** */

bool DBX::ReduceTable(const char* const pszName)
{
   return false;
}

/* ******************************************************************** **
** @@ DBX::CompactTable()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool DBX::CompactTable(const char* const pszName)
{
   return false;
}

/* ******************************************************************** **
** @@ DBX::CloseTable()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool DBX::CloseTable(const char* const pszName)
{
   int      iIndex = 0;

   DBX_TABLE*    pTable = (DBX_TABLE*)Search(pszName,&iIndex);
   
   if (!pTable || IsBadWritePtr(pTable,sizeof(DBX_TABLE)))
   {
      return false;
   }
   
   bool     bDone = CloseTable(pTable);

   pTable = NULL;

   return bDone;
}

/* ******************************************************************** **
** @@ DBX::CloseTable()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool DBX::CloseTable(DBX_TABLE*& rpTable)
{
   if (rpTable && !IsBadWritePtr(rpTable,sizeof(DBX_TABLE)))
   {
      Remove(rpTable);

      rpTable->Close();
      
      delete rpTable;
      rpTable = NULL;
      
      return true;
   }

   return false;
}

/* ******************************************************************** **
** @@ DBX::GetTable()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DBX_TABLE* DBX::GetTable(const char* const pszName)
{
   DBX_TABLE     Table;

   Table.SetName(pszName);

   return (DBX_TABLE*)Search(&Table);
}

/* ******************************************************************** **
** @@ DBX::GetTable()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DBX_TABLE* DBX::GetTable(DWORD dwTblID)
{
   DBX_TABLE*     pTable = (DBX_TABLE*)At(dwTblID);

   return pTable;
}

/* ******************************************************************** **
** @@ DBX_TSS::GetTableIfOpen()
** @  Copyrt  : 
** @  Author  :
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */

DBX_TABLE* DBX::GetTableIfOpen(const char* const pszName)
{
   SetSorter(CompareTableNames);

   int      iIndex = 0;

   DBX_TABLE  Table;

   Table.SetName(pszName);

   DBX_TABLE*   pTable = (DBX_TABLE*)Search(&Table,&iIndex);

   return pTable;
}

/* ******************************************************************** **
** @@ DBX_TABLE::DBX_TABLE()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Constructor
** ******************************************************************** */

DBX_TABLE::DBX_TABLE()
{
/*
      DWORD                _dwCurrent;
*/
   _pMemFixFile  = NULL;
   _pMemBufFix   = NULL;
   _dwMemSizeFix = 0;
   
   _dwDiskSizeFix = 0;
   _dwDiskSizeVar = 0;

   _pMemVarFile  = NULL;
   _pMemBufVar   = NULL;
   _dwMemSizeVar = 0;
   
   _hFixFile = INVALID_HANDLE_VALUE;
   _hVarFile = INVALID_HANDLE_VALUE;

   _FixOpenMode  = DBX_OM_NONE;
   _FixFlushMode = DBX_FM_FLUSH_NEVER;

   _VarOpenMode  = DBX_OM_NONE;
   _VarFlushMode = DBX_FM_FLUSH_NEVER;

   memset(&_FixHeader,0,sizeof(DBX_HEADER));
   memset(&_VarHeader,0,sizeof(DBV_HEADER));

   memset(_pszTblName,0,sizeof(_pszTblName));
   memset(_pszFixName,0,sizeof(_pszFixName));
   memset(_pszVarName,0,sizeof(_pszVarName));

   memset(&_TStamp,0,sizeof(Timestamp));

   _bOpen = false;

   _iIndex = 0;
}

/* ******************************************************************** **
** @@ DBX_TABLE::~DBX_TABLE()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Destructor
** ******************************************************************** */

DBX_TABLE::~DBX_TABLE()
{
   Close();
}

/* ******************************************************************** **
** @@ DBX_TABLE::InsertEmptyRecord()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

DWORD DBX_TABLE::InsertEmptyRecord()
{
   if (_FixOpenMode != DBX_OM_READ_WRITE)
   {
      return 0;
   }

   BYTE*    pRecord = new BYTE[_FixHeader._wRecSize];

   if (!pRecord)
   {
      return 0;
   }

   memset(pRecord,0,_FixHeader._wRecSize);

   if (!InsertRecord(pRecord))
   {
      delete pRecord;
      pRecord = NULL;
      return 0;
   }

   delete pRecord;
   pRecord = NULL;

   ++_FixHeader._dwRecCnt;
   ++_FixHeader._dwLastID;

   Flush();

   return _FixHeader._dwLastID;
}

/* ******************************************************************** **
** @@ DBX_TABLE::InsertRecord()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

DWORD DBX_TABLE::InsertRecord(const BYTE* const pRecord)
{
   if (!_bOpen)
   {
      // Error
      ASSERT(0);
      return 0;
   }

   if (_FixOpenMode != DBX_OM_READ_WRITE)
   {
      // Error
      ASSERT(0);
      return 0;
   }

   if (_hFixFile == INVALID_HANDLE_VALUE)
   {
      // Error
      ASSERT(0);
      return 0;
   }

   // FixSize
   int   iOfs = _FixHeader._wDataStart + _FixHeader._dwRecCnt * _FixHeader._wRecSize;

   if (SetFilePointerCUR(_hFixFile,iOfs,FILE_BEGIN) != (DWORD)iOfs)
   {
      // Error
      ASSERT(0);
      return 0;
   }

   if (!WriteBuffer(_hFixFile,pRecord,_FixHeader._wRecSize))
   {
      // Error
      ASSERT(0);
      return 0;
   }

   char     cEOF = DBF_EOF_MARKER;

   if (!WriteBuffer(_hFixFile,&cEOF,1))
   {
      // Error
      ASSERT(0);
      return 0;
   }

   _dwDiskSizeFix += (_FixHeader._wRecSize + 1);

   ++_FixHeader._dwRecCnt;
   ++_FixHeader._dwLastID;

   return _FixHeader._dwLastID;
}

/* ******************************************************************** **
** @@ DBX_TABLE::DuplicateRecord()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::DuplicateRecord(DWORD dwRecID)
{
   // Haven't yet implemented !
   return false;
}

/* ******************************************************************** **
** @@ DBX_TABLE::UpdateRecord()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::UpdateRecord
(
   DWORD                      dwRecID,
   const BYTE* const          pRecord
)
{
   switch (_FixOpenMode)
   {
      case DBX_OM_IN_PLACE:
      {
         if (!_pMemFixFile || !_pMemFixFile->Buffer())
         {
            // Error !
            ASSERT(0);
            return false;
         }

         DWORD    dwPos = _FixHeader._wDataStart + (dwRecID - 1) * _FixHeader._wRecSize;
         BYTE*    pRec  = _pMemFixFile->Buffer() + dwPos;   

         if (IsBadWritePtr(pRec,_FixHeader._wRecSize))
         {
            // Error !
            ASSERT(0);
            return false;
         }
         
         memcpy(pRec,pRecord,_FixHeader._wRecSize);

         return true;
      }
      case DBX_OM_READ_WRITE:
      {
         // FixMe!
         // Haven't yet implemented
         ASSERT(0);
         return false;
      }
      case DBX_OM_READ_ONLY:
      default:
      {
         // Error !
         ASSERT(0);
         return false;
      }
   }
}

/* ******************************************************************** **
** @@ DBX_TABLE::DeleteRecord()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::DeleteRecord(DWORD dwRecID)
{
   // Haven't yet implemented !
   return false;
}

/* ******************************************************************** **
** @@ DBX_TABLE::DeleteAll()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::DeleteAll()
{
   // Haven't yet implemented !
   return false;
}

/* ******************************************************************** **
** @@ DBX_TABLE::Unerase()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void DBX_TABLE::Unerase()
{
}

/* ******************************************************************** **
** @@ DBX_TABLE::SwapRecords()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::SwapRecords(DWORD dwSrcID,DWORD dwDstID)
{
   return false;
}

/* ******************************************************************** **
** @@ DBX_TABLE::Current()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

DWORD DBX_TABLE::Current()
{
   return 1;
}

/* ******************************************************************** **
** @@ DBX_TABLE::First()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : 
** ******************************************************************** */

DWORD DBX_TABLE::First()
{
   // Haven't yet implemented !
   return NULL;
}

/* ******************************************************************** **
** @@ DBX_TABLE::Last()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

DWORD DBX_TABLE::Last()
{
   return _FixHeader._dwRecCnt;
}

/* ******************************************************************** **
** @@ DBX_TABLE::Prev()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : 
** ******************************************************************** */

DWORD DBX_TABLE::Prev()
{
   if (_FixHeader._dwCurRec <= 1)
   {
      return 0;
   }

   return --_FixHeader._dwCurRec;
}

/* ******************************************************************** **
** @@ DBX_TABLE::Next()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : 
** ******************************************************************** */

DWORD DBX_TABLE::Next()
{
   if (_FixHeader._dwCurRec >= _FixHeader._dwRecCnt)
   {
      return 0;
   }

   return ++_FixHeader._dwCurRec;
}

/* ******************************************************************** **
** @@ DBX_TABLE::Go()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

const BYTE* const DBX_TABLE::Go(DWORD dwRecNum)
{
   if ((dwRecNum < 1) || (dwRecNum > _FixHeader._dwRecCnt))
   {
      // Error !
      ASSERT(0);
      return NULL;
   }

   _FixHeader._dwCurRec = dwRecNum;

   switch (_FixOpenMode)
   {
      case DBX_OM_READ_ONLY:
      case DBX_OM_IN_PLACE:
      {
         BYTE*    pRec = _pMemFixFile->Buffer() + _FixHeader._wDataStart + (_FixHeader._dwCurRec - 1) * _FixHeader._wRecSize;

         if (IsBadReadPtr(pRec,_FixHeader._wRecSize))
         {
            // Error !
            ASSERT(0);
            return NULL;
         }

         return pRec;    
      }
      case DBX_OM_READ_WRITE:
      {
         // FixMe!
         // Haven't yet implemented
         ASSERT(0);
         return NULL;
      }
      default:
      {
         // Error !
         ASSERT(0);
         return NULL;
      }
   }

   return NULL;
}

/* ******************************************************************** **
** @@ DBX_TABLE::FindFixColumnByOffset()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

int DBX_TABLE::FindFixColumnByOffset(DWORD dwOfs)
{
   if (!_bOpen)
   {
      // Error !
      ASSERT(0);
      return -1;
   }

   DWORD    dwCnt = Count();

   if (!dwCnt)
   {
      // Error !
      ASSERT(0);
      return -1;
   }

   for (DWORD ii = 0; ii < dwCnt; ++ii)
   {
      DBX_COLUMN*      pField = (DBX_COLUMN*)At(ii);

      if ((dwOfs >= (int)pField->GetOffset()) && (dwOfs < (pField->GetOffset() + pField->GetSize())))
      {
         return (int)ii;
      }
   }

   // Error !
   ASSERT(0);
   return -1;
}

/* ******************************************************************** **
** @@ DBX_TABLE::Open()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool DBX_TABLE::Open
(
   const char* const    pszTableName,
   const char* const    pszFixName,
   const char* const    pszVarName,
   DBX_OPEN_MODE        FixOpenMode,
   DBX_OPEN_MODE        VarOpenMode,
   DBX_FLUSH_MODE       FixFlushMode,
   DBX_FLUSH_MODE       VarFlushMode
)
{
   if (_bOpen)
   {
      return true;
   }

   switch (FixOpenMode)
   {
      case DBX_OM_READ_ONLY:
      {
         return _Open_Fix_RO(pszFixName,pszVarName,VarOpenMode,FixFlushMode,VarFlushMode);
      }
      case DBX_OM_IN_PLACE:
      {
         return _Open_Fix_IP(pszFixName,pszVarName,VarOpenMode,FixFlushMode,VarFlushMode);
      }
      case DBX_OM_READ_WRITE:
      {
          return _Open_Fix_RW(pszFixName,pszVarName,VarOpenMode,FixFlushMode,VarFlushMode);
      }
   }

   return false;
}

/* ******************************************************************** **
** @@ DBX_TABLE::_Open_Fix_RO()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::_Open_Fix_RO
(
   const char* const    pszFixName,
   const char* const    pszVarName,
   DBX_OPEN_MODE        VarOpenMode,
   DBX_FLUSH_MODE       FixFlushMode,
   DBX_FLUSH_MODE       VarFlushMode
)
{
   if (_pMemFixFile && _pMemFixFile->IsOpen())
   {
      if (stricmp(_pszFixName,pszFixName))
      {
         // Other open
         ASSERT(0);
         return false;
      }

      // Already open
      return true;
   }

   if (!_pMemFixFile)
   {
      strcpy(_pszFixName,pszFixName);

      _pMemFixFile = new MMF;

      if (!_pMemFixFile)
      {
         return false;
      }
   }

   if (!_pMemFixFile->OpenReadOnly(_pszFixName))
   {
      _pMemFixFile->Close();
      delete _pMemFixFile;
      _pMemFixFile = NULL;
      return false;
   }

   _pMemBufFix   = _pMemFixFile->Buffer();
   _dwMemSizeFix = _pMemFixFile->Size();

   memcpy(&_FixHeader,_pMemBufFix,sizeof(DBX_HEADER));

   DWORD    dwDisplacement = 1;     // For DELETE Marker
   int      iFieldNum      = 0;

   DWORD    dwRecCntFile = (_pMemFixFile->Size() - _FixHeader._wDataStart) / _FixHeader._wRecSize;

   if (dwRecCntFile != _FixHeader._dwRecCnt)
   {
      _FixHeader._dwRecCnt = min(_FixHeader._dwRecCnt,dwRecCntFile);
   }

   DBX_FIELD*    pField = (DBX_FIELD*)(_pMemFixFile->Buffer() + sizeof(DBX_HEADER));
   
   while (*pField->_pszName != DBF_HEADER_TERMINATOR) 
   {
      DBX_COLUMN*    pColumn = new DBX_COLUMN;

      if (!pColumn)
      {
         int   iCnt = Count();

         for (int ii = iCnt - 1; ii >= 0; --ii)
         {
            DBX_COLUMN*   pItem = (DBX_COLUMN*)At(ii);
            
            RemoveAt(ii);

            delete pItem;
            pItem = NULL;
         }

         return false;
      }

      pColumn->SetNum(iFieldNum++);
      pColumn->SetDisplacement(dwDisplacement);
      pColumn->SetParams((DBX_FILE_TYPE)_FixHeader._byFileType,pField);

      ++pField;

      dwDisplacement += pColumn->GetSize();

      if (Insert(pColumn) == -1)
      {
         int   iCnt = Count();

         for (int ii = iCnt - 1; ii >= 0; --ii)
         {
            DBX_COLUMN*   pItem = (DBX_COLUMN*)At(ii);
            
            RemoveAt(ii);
            delete pItem;
            pItem = NULL;
         }

         return false;
      }
   }

   _FixFlushMode = FixFlushMode;
   _FixOpenMode  = DBX_OM_READ_ONLY;

   if (!pszVarName || !*pszVarName)
   {
      // Is NOT Error !
      // Nothing to do !
      _bOpen = true;

      return _bOpen;
   }
   
   switch (VarOpenMode)
   {
      case DBX_OM_READ_ONLY:
      {
         return _Open_Var_RO(pszVarName,VarFlushMode);
      }
      case DBX_OM_IN_PLACE:
      {
         return _Open_Var_IP(pszVarName,VarFlushMode);
      }
      case DBX_OM_READ_WRITE:
      {
          return _Open_Var_RW(pszVarName,VarFlushMode);
      }
   }

   return false;
}

/* ******************************************************************** **
** @@ DBX_TABLE::_Open_Fix_RW()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::_Open_Fix_RW
(
   const char* const    pszFixName,
   const char* const    pszVarName,
   DBX_OPEN_MODE        VarOpenMode,
   DBX_FLUSH_MODE       FixFlushMode,
   DBX_FLUSH_MODE       VarFlushMode
)
{
   strcpy(_pszFixName,pszFixName);

   if (_hFixFile != INVALID_HANDLE_VALUE)
   {
      // Error !
      ASSERT(0);
      return false;
   }

   _hFixFile = OpenFileReadWrite(pszFixName,FILE_SHARE_READ | FILE_SHARE_WRITE);

   if (_hFixFile == INVALID_HANDLE_VALUE)
   {
      // Error !
      ASSERT(0);
      return false;
   }

   _dwDiskSizeFix = GetFileSizeLo(_hFixFile);

   if (!ReadBuffer(_hFixFile,&_FixHeader,sizeof(_FixHeader)))
   {
      // Error !
      ASSERT(0);
      CloseHandle(_hFixFile);
      _hFixFile = INVALID_HANDLE_VALUE;
      return false;
   }

   ASSERT(_FixHeader._byFileType);

   switch (_FixHeader._byFileType)
   {
      case DBX_FT_FOX_BASE:
      {
         break;
      }
      case DBX_FT_FOX_BASE_PLUS_NO_MEMO:
      {
         break;
      }
      case DBX_FT_VISUAL_FOX_PRO:
      {
         break;
      }
      case DBX_FT_DBASE_IV_SQL_TABLE_FILES_NO_MEMO:
      {
         break;
      }
      case DBX_FT_DBASE_IV_SQL_SYSTEM_FILES_NO_MEMO:
      {
         break;
      }
      case DBX_FT_FOX_BASE_PLUS_WITH_MEMO:
      {
         break;
      }
      case DBX_FT_DBASE_IV_WITH_MEMO:
      {
         break;
      }
      case DBX_FT_DBASE_IV_SQL_TABLE_FILES_WITH_MEMO:
      {
         break;
      }
      case DBX_FT_FOX_2X_WITH_MEMO:
      {
         break;
      }
      case DBX_FT_FOX_BASE_OTHER:
      {
         break;
      }
      default:
      {
         // Unknown DB type
         CloseHandle(_hFixFile);
         _hFixFile = INVALID_HANDLE_VALUE;
         return false;
      }
   }   

   DWORD    dwDisplacement = 1;     // For DELETE Marker
   int      iFieldNum      = 0;

   DWORD    dwRecCntFile = (GetFileSizeLo(_hFixFile) - _FixHeader._wDataStart) / _FixHeader._wRecSize;

   if (dwRecCntFile != _FixHeader._dwRecCnt)
   {
      _FixHeader._dwRecCnt = min(_FixHeader._dwRecCnt,dwRecCntFile);
   }

   // Fill Fields Info Array
   DBX_FIELD     DBF_Field;

   memset(&DBF_Field,0,sizeof(DBF_Field));

   while (true)  
   {
      bool  bDone = ReadBuffer(_hFixFile,&DBF_Field,sizeof(DBF_Field));

      if (*DBF_Field._pszName == DBF_HEADER_TERMINATOR)  // The First Field
      {
         break;
      }

      if (!bDone)
      {
         CloseHandle(_hFixFile);
         _hFixFile = INVALID_HANDLE_VALUE;

         int   iCnt = Count();

         for (int ii = iCnt - 1; ii >= 0; --ii)
         {
            DBX_COLUMN*   pItem = (DBX_COLUMN*)At(ii);
            
            RemoveAt(ii);
            delete pItem;
            pItem = NULL;
         }

         return false;
      }

      DBX_COLUMN*    pColumn = new DBX_COLUMN;

      if (!pColumn)
      {
         int   iCnt = Count();

         for (int ii = iCnt - 1; ii >= 0; --ii)
         {
            DBX_COLUMN*   pItem = (DBX_COLUMN*)At(ii);
            
            RemoveAt(ii);
            delete pItem;
            pItem = NULL;
         }

         return false;
      }

      pColumn->SetNum(iFieldNum++);
      pColumn->SetDisplacement(dwDisplacement);
      pColumn->SetParams((DBX_FILE_TYPE)_FixHeader._byFileType,&DBF_Field);

      dwDisplacement += pColumn->GetSize();

      if (Insert(pColumn) == -1)
      {
         int   iCnt = Count();

         for (int ii = iCnt - 1; ii >= 0; --ii)
         {
            DBX_COLUMN*   pItem = (DBX_COLUMN*)At(ii);
            
            RemoveAt(ii);
            delete pItem;
            pItem = NULL;
         }

         return false;
      }
   }

   _FixFlushMode = FixFlushMode;
   _FixOpenMode  = DBX_OM_READ_WRITE;

   if (!pszVarName || !*pszVarName)
   {
      // Is NOT Error !
      // Nothing to do !
      _bOpen = true;

      return _bOpen;
   }
   
   switch (VarOpenMode)
   {
      case DBX_OM_NONE:
      {
         // Is NOT Error !
         // Nothing to do !
         _bOpen = true;

         return _bOpen;
      }
      case DBX_OM_READ_ONLY:
      {
         return _Open_Var_RO(pszVarName,VarFlushMode);
      }
      case DBX_OM_IN_PLACE:
      {
         return _Open_Var_IP(pszVarName,VarFlushMode);
      }
      case DBX_OM_READ_WRITE:
      {
          return _Open_Var_RW(pszVarName,VarFlushMode);
      }
   }

   // Error !
   ASSERT(0);
   return false;
}

/* ******************************************************************** **
** @@ DBX_TABLE::_Open_Fix_IP()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::_Open_Fix_IP
(
   const char* const    pszFixName,
   const char* const    pszVarName,
   DBX_OPEN_MODE        VarOpenMode,
   DBX_FLUSH_MODE       FixFlushMode,
   DBX_FLUSH_MODE       VarFlushMode
)
{
   if (_pMemFixFile && _pMemFixFile->IsOpen())
   {
      if (stricmp(_pszFixName,pszFixName))
      {
         // Other open
         return false;
      }

      // Already open
      return true;
   }

   if (!_pMemFixFile)
   {
      strcpy(_pszFixName,pszFixName);

      _pMemFixFile = new MMF;

      if (!_pMemFixFile)
      {
         // Error !
         ASSERT(0);
         return false;
      }
   }

   if (!_pMemFixFile->OpenReadWrite(_pszFixName))
   {
      // Error !
      ASSERT(0);
      _pMemFixFile->Close();
      delete _pMemFixFile;
      _pMemFixFile = NULL;
      return false;
   }

   _pMemBufFix   = _pMemFixFile->Buffer();
   _dwMemSizeFix = _pMemFixFile->Size();

   memcpy(&_FixHeader,_pMemBufFix,sizeof(DBX_HEADER));

   DWORD    dwDisplacement = 1;     // For DELETE Marker
   int      iFieldNum      = 0;

   DWORD    dwRecCntFile = (_pMemFixFile->Size() - _FixHeader._wDataStart) / _FixHeader._wRecSize;

   if (dwRecCntFile != _FixHeader._dwRecCnt)
   {
      _FixHeader._dwRecCnt = min(_FixHeader._dwRecCnt,dwRecCntFile);
   }

   DBX_FIELD*    pField = (DBX_FIELD*)(_pMemFixFile->Buffer() + sizeof(DBX_HEADER));
   
   while (*pField->_pszName != DBF_HEADER_TERMINATOR) 
   {
      DBX_COLUMN*    pColumn = new DBX_COLUMN;

      if (!pColumn)
      {
         int   iCnt = Count();

         for (int ii = iCnt - 1; ii >= 0; --ii)
         {
            DBX_COLUMN*   pItem = (DBX_COLUMN*)At(ii);
            
            RemoveAt(ii);
            delete pItem;
            pItem = NULL;
         }

         // Error !
         ASSERT(0);
         return false;
      }

      pColumn->SetNum(iFieldNum++);
      pColumn->SetDisplacement(dwDisplacement);
      pColumn->SetParams((DBX_FILE_TYPE)_FixHeader._byFileType,pField);

      ++pField;

      dwDisplacement += pColumn->GetSize();

      if (Insert(pColumn) == -1)
      {
         int   iCnt = Count();

         for (int ii = iCnt - 1; ii >= 0; --ii)
         {
            DBX_COLUMN*   pItem = (DBX_COLUMN*)At(ii);
            
            RemoveAt(ii);
            delete pItem;
            pItem = NULL;
         }

         // Error !
         ASSERT(0);
         return false;
      }
   }

   _FixFlushMode = FixFlushMode;
   _FixOpenMode  = DBX_OM_IN_PLACE;

   if (!pszVarName || !*pszVarName)
   {
      // Is NOT Error !
      // Nothing to do !
      _bOpen = true;

      return _bOpen;
   }
   
   switch (VarOpenMode)
   {
      case DBX_OM_NONE:
      {
         // Is NOT Error !
         // Nothing to do !
         _bOpen = true;

         return _bOpen;
      }
      case DBX_OM_READ_ONLY:
      {
         return _Open_Var_RO(pszVarName,VarFlushMode);
      }
      case DBX_OM_IN_PLACE:
      {
         return _Open_Var_IP(pszVarName,VarFlushMode);
      }
      case DBX_OM_READ_WRITE:
      {
          return _Open_Var_RW(pszVarName,VarFlushMode);
      }
   }

   // Error !
   ASSERT(0);
   return false;
}

/* ******************************************************************** **
** @@ DBX_TABLE::_Open_Var_RO()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::_Open_Var_RO(const char* const pszVarName,DBX_FLUSH_MODE VarFlushMode)
{
   if (_pMemVarFile && _pMemVarFile->IsOpen())
   {
      if (stricmp(_pszVarName,pszVarName))
      {
         // Other open
         return false;
      }

      // Already open
      return true;
   }

   if (!_pMemVarFile)
   {
      strcpy(_pszVarName,pszVarName);

      _pMemVarFile = new MMF;

      if (!_pMemVarFile)
      {
         // Error !
         ASSERT(0);
         return false;
      }
   }

   _pMemVarFile->OpenReadOnly(_pszVarName);

   _pMemBufVar   = _pMemVarFile->Buffer();
   _dwMemSizeVar = _pMemVarFile->Size();

   _VarOpenMode  = DBX_OM_READ_ONLY;
   _VarFlushMode = VarFlushMode;
      
   memcpy(&_VarHeader,_pMemBufVar,sizeof(DBV_HEADER));

   _bOpen = true;

   return _bOpen;
}

/* ******************************************************************** **
** @@ DBX_TABLE::_Open_Var_IP()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::_Open_Var_IP(const char* const pszVarName,DBX_FLUSH_MODE VarFlushMode)
{
   if (_pMemVarFile && _pMemVarFile->IsOpen())
   {
      if (stricmp(_pszVarName,pszVarName))
      {
         // Other open
         return false;
      }

      // Already open
      return true;
   }

   if (!_pMemVarFile)
   {
      strcpy(_pszVarName,pszVarName);

      _pMemVarFile = new MMF;

      if (!_pMemVarFile)
      {
         // Error !
         ASSERT(0);
         return false;
      }
   }

   _pMemVarFile->OpenReadWrite(_pszVarName);

   _pMemBufVar   = _pMemVarFile->Buffer();
   _dwMemSizeVar = _pMemVarFile->Size();

   _VarOpenMode  = DBX_OM_READ_WRITE;
   _VarFlushMode = VarFlushMode;
      
   memcpy(&_VarHeader,_pMemBufVar,sizeof(DBV_HEADER));

   _bOpen = true;

   return _bOpen;
}

/* ******************************************************************** **
** @@ DBX_TABLE::_Open_Var_RW()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::_Open_Var_RW(const char* const pszVarName,DBX_FLUSH_MODE VarFlushMode)
{
   strcpy(_pszVarName,pszVarName);

   if (_hVarFile != INVALID_HANDLE_VALUE)
   {
      // Error !
      ASSERT(0);
      return false;
   }

   _hVarFile = OpenFileReadWrite(pszVarName,FILE_SHARE_READ | FILE_SHARE_WRITE);

   if (_hVarFile == INVALID_HANDLE_VALUE)
   {
      // Error !
      ASSERT(0);
      return false;
   }

   _dwDiskSizeVar = GetFileSizeLo(_hVarFile);

   if (!ReadBuffer(_hVarFile,&_VarHeader,sizeof(_VarHeader)))
   {
      // Error !
      ASSERT(0);
      CloseHandle(_hVarFile);
      _hVarFile = INVALID_HANDLE_VALUE;
      return false;
   }

   _VarOpenMode = DBX_OM_READ_WRITE;

   _bOpen = true;

   return _bOpen;
}

/* ******************************************************************** **
** @@ DBX_TABLE::Close()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::Close()
{
   if (!_bOpen)
   {
      return true;
   }

   // Update Fix Header
   switch (_FixOpenMode)
   {
      case DBX_OM_READ_ONLY:
      {
         break;
      }
      case DBX_OM_IN_PLACE:
      {
         UpdateFixHeader_IP();
         break;
      }
      case DBX_OM_READ_WRITE:
      {
         UpdateFixHeader_RW();
         break;
      }
   }

   // Update Var Header
   switch (_VarOpenMode)
   {
      case DBX_OM_READ_ONLY:
      {
         break;
      }
      case DBX_OM_IN_PLACE:
      {
         UpdateVarHeader_IP();
         break;
      }
      case DBX_OM_READ_WRITE:
      {
         UpdateVarHeader_RW();
         break;
      }
   }

   // Update Fix
   if (_hFixFile != INVALID_HANDLE_VALUE)
   {
      if (!UpdateFixHeader_RW())
      {
         // Error !
         ASSERT(0);
      }

      CloseHandle(_hFixFile);
      _hFixFile = INVALID_HANDLE_VALUE;
   }

   if (_pMemFixFile)
   {
      _pMemFixFile->Close();
      delete _pMemFixFile;
      _pMemFixFile = NULL;
   }

   if (_pMemVarFile)
   {
      _pMemVarFile->Close();
      delete _pMemVarFile;
      _pMemVarFile = NULL;
   }

   if (_hVarFile != INVALID_HANDLE_VALUE)
   {
      CloseHandle(_hVarFile);
   }

   int   iCnt = Count();

   // Should be int !
   for (int ii = iCnt - 1; ii >= 0; --ii)
   {
      DBX_COLUMN*    pItem = (DBX_COLUMN*)At(ii);
   
      if (pItem)
      {
         RemoveAt(ii);
         delete pItem;
         pItem = NULL;
      }
   }

   _bOpen = false;

   return true;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetColumn()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : 
** ******************************************************************** */

DBX_COLUMN* DBX_TABLE::GetColumn(const char* const pszName)
{
   if (!_bOpen)
   {
      return NULL;
   }

   int   iCnt = Count();

   for (int ii = 0; ii < iCnt; ++ii)
   {
      DBX_COLUMN*    pField = (DBX_COLUMN*)At(ii);

      if (!stricmp(pszName,pField->GetName()))
      {
         return pField;
      }
   }

   return NULL;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetColumn()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : 
** ******************************************************************** */

DBX_COLUMN* DBX_TABLE::GetColumn(DWORD dwNum)
{
   if (!_bOpen)
   {
      return NULL;
   }

   if (dwNum >= Count())
   {
      return NULL;
   }

   return (DBX_COLUMN*)At(dwNum);
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetMemBufFix()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

BYTE* DBX_TABLE::GetMemBufFix()
{
   return _pMemBufFix;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetMemBufVar()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : 
** ******************************************************************** */

BYTE* DBX_TABLE::GetMemBufVar()
{
   return _pMemBufVar;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetMemSizeFix()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : 
** ******************************************************************** */

DWORD DBX_TABLE::GetMemSizeFix()
{
   return _dwMemSizeFix;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetMemSizeVar()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

DWORD DBX_TABLE::GetMemSizeVar()
{
   return _dwMemSizeVar;
}

/* ******************************************************************** **
** @@ DBX_TABLE::IsErased()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::IsErased(DWORD dwRecID)
{
   return false;
}

/* ******************************************************************** **
** @@ DBX_TABLE::InsertColumn()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */
                                                 
int DBX_TABLE::InsertEmptyColumn()
{
   return -1;
}

/* ******************************************************************** **
** @@ DBX_TABLE::InsertColumn()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */
                                                 
int DBX_TABLE::InsertColumn(const DBX_COLUMN_DESCRIPTOR* const pDescr)
{
   return -1;
}

/* ******************************************************************** **
** @@ DBX_TABLE::InsertColumn()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */
                                                 
int DBX_TABLE::InsertColumn(const DBX_COLUMN_DESCRIPTOR& rDescr)
{
   return -1;
}

/* ******************************************************************** **
** @@ DBX_TABLE::InsertColumn()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::InsertColumn
(const DBX_COLUMN* const pColumn)
{
   return Insert(pColumn) != -1;
}                     

/* ******************************************************************** **
** @@ DBX_TABLE::DeleteColumn()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::DeleteColumn(const char* const pszName)
{
   return false;
}                     

/* ******************************************************************** **
** @@ DBX_TABLE::DeleteColumn()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::DeleteColumn(DWORD dwNum)
{
   return false;
}                     

/* ******************************************************************** **
** @@ DBX_TABLE::RenameColumn()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::RenameColumn(const char* const pszName,const char* const pszNewName)
{
   return false;
}                     

/* ******************************************************************** **
** @@ DBX_TABLE::RenameColumn()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::RenameColumn(DWORD dwNum,const char* const pszNewName)
{
   return false;
}                     

/* ******************************************************************** **
** @@ DBX_TABLE::ChangeColumnType()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::ChangeColumnType
(
   const char* const       pszName,
   DBX_FIELD_TYPE          NewType
)
{
   ASSERT(0);
   return false;
}                     

/* ******************************************************************** **
** @@ DBX_TABLE::ChangeColumnType()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : 
** ******************************************************************** */

bool DBX_TABLE::ChangeColumnType
(
   DWORD                dwNum,
   DBX_FIELD_TYPE       NewType
)
{
   ASSERT(0);
   return false;
}                     

/* ******************************************************************** **
** @@ DBX_TABLE::UpdateFixHeader_IP()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

bool DBX_TABLE::UpdateFixHeader_IP()
{
   if (!_bOpen)
   {
      return false;
   }

   if (_FixOpenMode != DBX_OM_IN_PLACE)
   {
      return false;
   }

   DWORD    dwDateNow = _TStamp.TS_DateNow();

   int   iYear  = 0;
   int   iMonth = 0;
   int   iDay   = 0; 

   _TStamp.TS_Date(dwDateNow,iYear,iMonth,iDay); 

   _FixHeader._byYear  = (BYTE)(iYear - 1900);
   _FixHeader._byMonth = (BYTE)iMonth;
   _FixHeader._byDay   = (BYTE)iDay;

   memcpy(_pMemFixFile->Buffer(),&_FixHeader,sizeof(_FixHeader));

   return true;
}

/* ******************************************************************** **
** @@ DBX_TABLE::UpdateFixHeader_RW()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

bool DBX_TABLE::UpdateFixHeader_RW()
{
   if (!_bOpen)
   {
      return false;
   }

   if (_FixOpenMode != DBX_OM_READ_WRITE)
   {
      return false;
   }

   if (_hFixFile == INVALID_HANDLE_VALUE)
   {
      return false;
   }

   if (SetFilePointerBOF(_hFixFile))
   {
      // Error
      return false;
   }

   DWORD    dwDateNow = _TStamp.TS_DateNow();

   int   iYear  = 0;
   int   iMonth = 0;
   int   iDay   = 0; 

   _TStamp.TS_Date(dwDateNow,iYear,iMonth,iDay); 

   _FixHeader._byYear  = (BYTE)(iYear - 1900);
   _FixHeader._byMonth = (BYTE)iMonth;
   _FixHeader._byDay   = (BYTE)iDay;

   if (!WriteBuffer(_hFixFile,&_FixHeader,sizeof(_FixHeader)))
   {
      // Error
      return false;
   }

   return true;
}

/* ******************************************************************** **
** @@ DBX_TABLE::UpdateVarHeader_IP()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool DBX_TABLE::UpdateVarHeader_IP()
{
   memcpy(_pMemBufVar,&_VarHeader,sizeof(DBV_HEADER));

   return true;
}

/* ******************************************************************** **
** @@ DBX_TABLE::UpdateVarHeader_RW()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool DBX_TABLE::UpdateVarHeader_RW()
{
   return false;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetRecID()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DWORD DBX_TABLE::GetRecID(const BYTE* const pRec)
{
   return 0;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetCurRecID()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

DWORD DBX_TABLE::GetCurRecID()
{
   return 0;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetCurRec()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

DWORD DBX_TABLE::GetCurRec()
{
   return _FixHeader._dwCurRec;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetCurRec()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void DBX_TABLE::SetCurRec(DWORD dwCurRec)
{
   _FixHeader._dwCurRec = dwCurRec;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetColumnName()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

const char* const DBX_TABLE::GetColumnName(DWORD dwNum)
{
   return ((DBX_COLUMN*)GetColumn(dwNum))->GetName();
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetColumnNum()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

int DBX_TABLE::GetColumnNum(const char* const pszName)
{
   return ((DBX_COLUMN*)GetColumn(pszName))->GetNum();
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetColumnOfs()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

DWORD DBX_TABLE::GetColumnOfs(DWORD dwNum)
{
   return ((DBX_COLUMN*)GetColumn(dwNum))->GetOffset();
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetColumnSize()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

DWORD DBX_TABLE::GetColumnSize(DWORD dwNum)
{
   return ((DBX_COLUMN*)GetColumn(dwNum))->GetSize();
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetFlushModeFix()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

DBX_FLUSH_MODE DBX_TABLE::GetFlushModeFix()
{
   return _FixFlushMode;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetFlushModeVar()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

DBX_FLUSH_MODE DBX_TABLE::GetFlushModeVar()
{
   return _VarFlushMode;
}

/* ******************************************************************** **
** @@ DBX_TABLE::SetFlushModeFix()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void DBX_TABLE::SetFlushModeFix(DBX_FLUSH_MODE FlushMode)
{
   _FixFlushMode = FlushMode;
}
          
/* ******************************************************************** **
** @@ DBX_TABLE::SetFlushModeVar()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void DBX_TABLE::SetFlushModeVar(DBX_FLUSH_MODE FlushMode)
{
   _VarFlushMode = FlushMode;
}
          
/* ******************************************************************** **
** @@ DBX_TABLE::Flush()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void DBX_TABLE::Flush()
{
   // Haven't yet implemented !
}
                        
/* ******************************************************************** **
** @@ DBX_TABLE::SetIndex()
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */

void DBX_TABLE::SetIndex(int iIndex)
{
   _iIndex = iIndex;
}

/* ******************************************************************** **
** @@ DBX_TABLE::SetName()
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  : 
** @  Notes   : 
** ******************************************************************** */

void DBX_TABLE::SetName(const char* const pszName)
{
   strncpy(_pszTblName,pszName,MAX_PATH);
   _pszTblName[MAX_PATH - 1] = 0;   // ASCIIZ
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetRecCnt()
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */

DWORD DBX_TABLE::GetRecCnt()
{
   if (!_bOpen)
   {
      return 0;
   }

   return _FixHeader._dwRecCnt;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetLastID()
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */

DWORD DBX_TABLE::GetLastID()
{
   if (!_bOpen)
   {
      return 0;
   }

   return _FixHeader._dwLastID;
}

/* ******************************************************************** **
** @@ DBX_TABLE::IsOpen()
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */

bool DBX_TABLE::IsOpen()
{
   return _bOpen;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetIndex()
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */

int DBX_TABLE::GetIndex()
{
   return _iIndex;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetFieldCnt()
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  : 
** @  Notes   : 
** ******************************************************************** */

int DBX_TABLE::GetFieldCnt()
{
   if (!_bOpen)
   {
      return 0;
   }

   return Count();
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetFixName()
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */

const char* const DBX_TABLE::GetFixName()
{
   return _pszFixName;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetVarName()
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */

const char* const DBX_TABLE::GetVarName()
{
   return _pszVarName;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetOpenModeFix()
** @  Copyrt  : 
** @  Author  :
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */

DBX_OPEN_MODE DBX_TABLE::GetOpenModeFix()
{
   return _FixOpenMode;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetOpenModeVar()
** @  Copyrt  : 
** @  Author  :
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */

DBX_OPEN_MODE DBX_TABLE::GetOpenModeVar()
{
   return _VarOpenMode;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetName()
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */

const char* const DBX_TABLE::GetName()
{
   return _pszTblName;
}

/* ******************************************************************** **
** @@ DBX_TABLE::GetMemo()
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */

BYTE* DBX_TABLE::GetMemo(DWORD dwPos,DWORD& rSize)
{
   rSize = 0;

   if (_VarOpenMode != DBX_OM_READ_WRITE)
   {
      // Error !
      ASSERT(0);
      return NULL;
   }

   if (!dwPos || (dwPos & 0x07) || (dwPos >= _dwDiskSizeVar))
   {
      // Error !
      ASSERT(0);
      return NULL;
   }

   if (SetFilePointerCUR(_hVarFile,dwPos,SEEK_SET  ) != dwPos)
   {
      // Error
      ASSERT(0);
      return NULL;
   }

   if (!ReadBuffer(_hVarFile,&rSize,sizeof(DWORD)))
   {
      // Error
      ASSERT(0);
      return NULL;
   }
   
   BYTE*    pMemo = new BYTE[rSize];

   if (SetFilePointerCUR(_hVarFile,dwPos + sizeof(DWORD),SEEK_SET  ) != (dwPos + sizeof(DWORD)))
   {
      // Error
      ASSERT(0);
      return NULL;
   }

   if (!ReadBuffer(_hVarFile,pMemo,rSize))
   {
      // Error
      ASSERT(0);
      return NULL;
   }
   
   if (IsBadReadPtr(pMemo,rSize))
   {
      // Error !
      ASSERT(0);
      delete[] pMemo;
      pMemo = NULL;
      return NULL;
   }

   return pMemo;
}

/* ******************************************************************** **
** @@ DBX_COLUMN::DBX_COLUMN()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Constructor
** ******************************************************************** */

DBX_COLUMN::DBX_COLUMN()
{
   _FieldType      = DBX_FLT_UNKNOWN;
   _dwReadStyle    = 0;
   _dwWriteStyle   = 0;
   _cFiller        = 0;
   _iNum           = 0;
   _dwSize         = 0;
   _dwOffset       = 0;
   _FieldFlags     = DBX_FF_NONE;
   _byDecimalPoint = 0;

   memset(_pszName,            0,sizeof(_pszName));
   memset(_pDateFormatTemplate,0,sizeof(_pDateFormatTemplate));

   _cFiller  = 0x20;

   memset(_pVariant,0,sizeof(_pVariant));

   SetDateFormatTemplate("YYYYMMDD");
}

/* ******************************************************************** **
** @@ DBX_COLUMN::DBX_COLUMN()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Constructor
** ******************************************************************** */

DBX_COLUMN::DBX_COLUMN(const DBX_COLUMN_DESCRIPTOR& rDescriptor)
{
   _FieldType      = rDescriptor._FieldType;
   _dwReadStyle    = rDescriptor._dwReadStyle;
   _dwWriteStyle   = rDescriptor._dwWriteStyle;
   _cFiller        = rDescriptor._cFiller;
   _iNum           = rDescriptor._iNum;
   _dwSize         = rDescriptor._dwSize;
   _dwOffset       = rDescriptor._dwOffset;
   _FieldFlags     = rDescriptor._FieldFlags;
   _byDecimalPoint = rDescriptor._byDecimalPoint;

   memset(_pszName,            0,sizeof(_pszName));
   memset(_pDateFormatTemplate,0,sizeof(_pDateFormatTemplate));

   strncpy(_pszName,rDescriptor._pszName,MAX_PATH + 1);
   _pszName[MAX_PATH + 1] = 0;   // Ensure ASCIIZ

   strncpy(_pDateFormatTemplate,rDescriptor._pDateFormatTemplate,MAX_DATE_FORMAT_SIZE);
   _pDateFormatTemplate[MAX_DATE_FORMAT_SIZE] = 0;   // Ensure ASCIIZ

   _cFiller = 0x20;

   memset(_pVariant,0,sizeof(_pVariant));

   SetDateFormatTemplate("YYYYMMDD");
}

/* ******************************************************************** **
** @@ DBX_COLUMN::~DBX_COLUMN()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Destructor
** ******************************************************************** */

DBX_COLUMN::~DBX_COLUMN()
{
}

/* ******************************************************************** **
** @@ DBX_COLUMN::SetType()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void DBX_COLUMN::SetType(DBX_FIELD_TYPE FieldType)
{
   _FieldType = FieldType;
}

/* ******************************************************************** **
** @@ DBX_COLUMN::Get_DBF_String()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

const void* const DBX_COLUMN::Get_DBF_String(const void* const pBuf)
{
   strncpy(_pVariant,(char*)pBuf + _dwOffset,_dwSize);
   _pVariant[_dwSize - 1] = 0; // ASCIIZ

   int   iSize = strlen(_pVariant);

   if (_dwReadStyle & DBX_CS_TRIM)
   {
      TrimInPlace(_pVariant);
      iSize = strlen(_pVariant);   
   }

   if (_dwReadStyle & DBX_CS_RUSSIAN_OEM_TO_ANSI)
   {
      XLAT(_pVariant,strlen(_pVariant),(BYTE*)XLAT_DOS_WIN);
   }

   if (_dwReadStyle & DBX_CS_CORRECT_INPROPER_RUSSIAN_ANSI)
   {
      XLAT(_pVariant,strlen(_pVariant),(BYTE*)XLAT_CORRECT_INPROPER_RUSSIAN_ANSI);
   }

   if (_dwReadStyle & DBX_CS_SET_CASE_UPPER_RUSSIAN_ANSI)
   {
      XLAT(_pVariant,strlen(_pVariant),(BYTE*)XLAT_UPPERCASE_RUS_ANSI);
   }

   return _pVariant; 
}

/* ******************************************************************** **
** @@ DBX_COLUMN::Set_DBF_String()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void DBX_COLUMN::Set_DBF_String
(
   void*                   pBuf,
   const void* const       pValue
)
{
   if (_dwWriteStyle & DBX_CS_EXPAND)
   {
      Expand();
   }

   int   iLen = min(strlen((char*)pValue),_dwSize);

   memcpy(_pVariant,(char*)pValue,iLen);

   if (_dwWriteStyle & DBX_CS_RUSSIAN_ANSI_TO_OEM)
   {
      XLAT(_pVariant,iLen,(BYTE*)XLAT_WIN_DOS);
   }

   memcpy((char*)pBuf + _dwOffset,_pVariant,_dwSize);
}

/* ******************************************************************** **
** @@ DBX_COLUMN::SetParams()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void DBX_COLUMN::SetParams
(
   DBX_FILE_TYPE                FileType,
   const DBX_FIELD* const       pField
)
{
   strncpy((char*)_pszName,pField->_pszName,11);
   _pszName[10] = 0;    // ASCIIZ

   _FieldFlags = (DBX_FIELD_FLAGS)pField->_byFlags;
   
   SetType((DBX_FIELD_TYPE)pField->_byType);

// Commented due Fucking DBF-Editors, wrote JUNK to this field !   
//   _dwOffset = rField._dwDisplacement;
   _dwSize   = pField->_bySize;

   _byDecimalPoint = pField->_byDecimalPoint;
}

/* ******************************************************************** **
** @@ DBX_COLUMN::Get()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

const void* const DBX_COLUMN::Get(const void* const pBuf)
{
   switch (_FieldType)
   {
      // DBX_FLT_CHARACTER        = 'C',
      case DBX_FLT_CHARACTER:
      {
         if (_FieldFlags == DBX_FF_BINARY)
         {
            memset(_pVariant,0,sizeof(_pVariant));
            
            memcpy(_pVariant,(char*)pBuf + _dwOffset,_dwSize);
            
            return _pVariant;
         }
         
         return Get_DBF_String(pBuf);
      }
      // DBX_FLT_CURRENCY         = 'Y',
      case DBX_FLT_CURRENCY:
      {
         return NULL; 
      }
      // DBX_FLT_NUMERIC          = 'N',
      case DBX_FLT_NUMERIC:
      {
         // INT64 Precision !
         if (!_byDecimalPoint)
         {
            strncpy(_pVariant,(char*)pBuf + _dwOffset,_dwSize);
            _pVariant[_dwSize - 1] = 0; // ASCIIZ

            __int64     iNum = _atoi64(_pVariant);
            
            memset(_pVariant,0,sizeof(_pVariant));
            memcpy(_pVariant,&iNum,sizeof(__int64));
         }
         else
         {
            // Rational - Insert Decimal Point
         }

         return _pVariant; 
      }
      // DBX_FLT_FLOAT            = 'F',
      case DBX_FLT_FLOAT:
      {
         return NULL; 
      }
      // DBX_FLT_DATE             = 'D',
      case DBX_FLT_DATE:
      {
         return Get_DBF_Date(pBuf);
      }
      // DBX_FLT_DATETIME         = 'T',
      case DBX_FLT_DATETIME:
      {  
//         return Get_DBF_Time(pBuf);
         return NULL;
      }
      // DBX_FLT_DOUBLE           = 'B',
      case DBX_FLT_DOUBLE:
      {
         return NULL; 
      }
      // DBX_FLT_INTEGER          = 'I',
      case DBX_FLT_INTEGER:
      {
         return NULL; 
      }
      // DBX_FLT_LOGICAL          = 'L',
      case DBX_FLT_LOGICAL:
      {
         memset(_pVariant,0,sizeof(_pVariant));
         *_pVariant = (char)(*((char*)pBuf + _dwOffset) == 'T'  ?  1  :  0); 
         return _pVariant; 
      }
      // DBX_FLT_MEMO             = 'M',
      case DBX_FLT_MEMO:
      {
         return NULL; 
      }
      // DBX_FLT_GENERAL          = 'G',
      case DBX_FLT_GENERAL:
      {
         return NULL; 
      }
      // DBX_FLT_CHARACTER_BINARY = 'c',
      case DBX_FLT_CHARACTER_BINARY:
      {
         if (IsBadReadPtr((char*)pBuf + _dwOffset,_dwSize))
         {
            // Error !
            ASSERT(0);
            return NULL;
         }
         
         memset(_pVariant,0,sizeof(_pVariant));
         memcpy(_pVariant,(char*)pBuf + _dwOffset,_dwSize);
         return _pVariant; 
      }
      // DBX_FLT_MEMO_BINARY      = 'm',
      case DBX_FLT_MEMO_BINARY:
      {
         return NULL; 
      }
      // DBX_FLT_PICTURE          = 'P',
      case DBX_FLT_PICTURE:
      {
         return NULL; 
      }
      // DBX_FLT_VAR_BINARY       = 'Q',
      case DBX_FLT_VAR_BINARY:
      {
         return NULL; 
      }
      // DBX_FLT_VARCHAR_BINARY   = 'V'
      case DBX_FLT_VARCHAR_BINARY:
      {
         return NULL; 
      }
      // DBX_FLT_UNKNOWN          = 'X',
      case DBX_FLT_UNKNOWN:
      default:
      {
         return NULL;
      }
   }
}

/* ******************************************************************** **
** @@ DBX_COLUMN::Set()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void DBX_COLUMN::Set
(
   void*                   pBuf,
   const void* const       pValue
)
{
   ASSERT(pBuf);
   ASSERT(pValue);

   switch (_FieldType)
   {
      // DBX_FLT_CHARACTER        = 'C',
      case DBX_FLT_CHARACTER:
      {
         if ((_FieldFlags & DBX_FF_BINARY) == DBX_FF_BINARY)
         {
            if (!IsBadReadPtr(pValue,_dwSize))
            {
               memcpy((char*)pBuf + _dwOffset,pValue,_dwSize);
            }
         }
         else
         {
            Set_DBF_String(pBuf,pValue);
         }

         return;
      }
      // DBX_FLT_CURRENCY         = 'Y',
      case DBX_FLT_CURRENCY:
      {
         return;
      }
      // DBX_FLT_NUMERIC          = 'N',
      case DBX_FLT_NUMERIC:
      {
         // INT64 Precision !
         if (!_byDecimalPoint)
         {  
            if (!IsBadReadPtr(pValue,sizeof(__int64)))
            {
               wsprintf(_pVariant,"%I64d",*(__int64*)pValue);

               memset((char*)pBuf + _dwOffset,0x20,_dwSize);

               int   iLen = min(strlen(_pVariant),_dwSize);

               memcpy((char*)pBuf + _dwOffset + (_dwSize - iLen),_pVariant,iLen);
            }
         }
         else
         {
            // Rational - Insert Decimal Point
         }

         return; 
      }
      // DBX_FLT_FLOAT            = 'F',
      case DBX_FLT_FLOAT:
      {
         return;
      }
      // DBX_FLT_DATE             = 'D',
      case DBX_FLT_DATE:
      {
         Set_DBF_Date(pBuf,pValue);
         return;
      }
      // DBX_FLT_DATETIME         = 'T',
      case DBX_FLT_DATETIME:
      {
         return;
      }
      // DBX_FLT_DOUBLE           = 'B',
      case DBX_FLT_DOUBLE:
      {
         return;
      }
      // DBX_FLT_INTEGER          = 'I',
      case DBX_FLT_INTEGER:
      {
         return;
      }
      // DBX_FLT_LOGICAL          = 'L',
      case DBX_FLT_LOGICAL:
      {
         if (!IsBadReadPtr(pBuf,sizeof(bool)))
         {
//            *((char*)pBuf + _dwOffset) = *(char*)pValue;
         }
   
         return; 
      }
      // DBX_FLT_MEMO             = 'M',
      case DBX_FLT_MEMO:
      {
         return;
      }
      // DBX_FLT_GENERAL          = 'G',
      case DBX_FLT_GENERAL:
      {
         return;
      }
      // DBX_FLT_CHARACTER_BINARY = 'c',
      case DBX_FLT_CHARACTER_BINARY:
      {
         return;
      }
      // DBX_FLT_MEMO_BINARY      = 'm',
      case DBX_FLT_MEMO_BINARY:
      {
         return;
      }
      // DBX_FLT_PICTURE          = 'P',
      case DBX_FLT_PICTURE:
      {
         return;
      }
      // DBX_FLT_VAR_BINARY       = 'q',
      case DBX_FLT_VAR_BINARY:
      {
         return;
      }
      // DBX_FLT_VARCHAR_BINARY   = 'v'
      case DBX_FLT_VARCHAR_BINARY:
      {
         return;
      }
      // DBX_FLT_UNKNOWN          = 'X',
      case DBX_FLT_UNKNOWN:
      default:
      {
         return; 
      }
   }
}

/* ******************************************************************** **
** @@ DBX_COLUMN::SetDisplacement()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void DBX_COLUMN::SetDisplacement(DWORD dwDisplacement)
{
   _dwOffset = dwDisplacement;
}

/* ******************************************************************** **
** @@ GetFieldType()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

DBX_FIELD_TYPE GetFieldType(char cType,BYTE byFlag)
{
   switch (cType)
   {
      case 'C':
      {
         return ((byFlag & DBX_FF_BINARY) == DBX_FF_BINARY)  ?  DBX_FLT_CHARACTER_BINARY  :  DBX_FLT_CHARACTER;
      }
      case 'Y':
      {
         return DBX_FLT_CURRENCY;
      }
      case 'N':
      {
         return DBX_FLT_NUMERIC;
      }
      case 'F':
      {
         return DBX_FLT_FLOAT;
      }
      case 'D':
      {
         return DBX_FLT_DATE;
      }
      case 'T':
      {
         return DBX_FLT_DATETIME;
      }
      case 'B':
      {
         return DBX_FLT_DOUBLE;
      }
      case 'I':
      {
         return DBX_FLT_INTEGER;
      }
      case 'L':
      {
         return DBX_FLT_LOGICAL;
      }
      case 'M':
      {
         return ((byFlag & DBX_FF_BINARY) == DBX_FF_BINARY)  ?  DBX_FLT_MEMO_BINARY  :  DBX_FLT_MEMO;
      }
      case 'G':
      {
         return DBX_FLT_GENERAL;
      }
      case 'P':
      {
         return DBX_FLT_PICTURE;
      }
      case 'Q':
      {
         return DBX_FLT_VAR_BINARY;
      }
      case 'V':
      {
         return DBX_FLT_VARCHAR_BINARY;
      }
      default:
      {
         return DBX_FLT_UNKNOWN;
      }
   }
}

/* ******************************************************************** **
** @@ DBX_COLUMN::Get_DBF_Date()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

const void* const DBX_COLUMN::Get_DBF_Date(const void* const pBuf)
{
   _TStamp.SplitDateString((char*)pBuf + _dwOffset,GetDateFormatTemplate());

   DWORD    dwDate = _TStamp.TS_Date();

   memset(_pVariant,0,sizeof(_pVariant));
   memcpy(_pVariant,&dwDate,sizeof(DWORD));

   return _pVariant; 
}

/* ******************************************************************** **
** @@ DBX_COLUMN::Get_DBF_Time()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

const void* const DBX_COLUMN::Get_DBF_Time(const void* const pBuf)
{
/*
   TIMESTAMP*     pTS = (TIMESTAMP*)_pVariant;
   memset(pTS,0,sizeof(TIMESTAMP));
   void     SplitDateString(const char* const pszDate,const char* const pszFormat);
   strncpy(_pTempBuf,(char*)pBuf + _dwOffset,_dwSize);
   _pTempBuf[_dwSize - 1] = 0; // ASCIIZ
*/

   return NULL; 
}

/* ******************************************************************** **
** @@ DBX_COLUMN::Set_DBF_Date()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void DBX_COLUMN::Set_DBF_Date
(
   void*                   pBuf,
   const void* const       pValue
)
{
   int   iYear  = 0;
   int   iMonth = 0;
   int   iDay   = 0;

   _TStamp.TS_Date(*(DWORD*)pValue,iYear,iMonth,iDay);

   wsprintf(_pVariant,"%04d%02d%02d",iYear,iMonth,iDay);

   memcpy((char*)pBuf + _dwOffset,_pVariant,_dwSize);
}

/* ******************************************************************** **
** @@ DBX_COLUMN::Expand()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void DBX_COLUMN::Expand()
{
   memset(_pVariant,0x20,_dwSize);
   
   _pVariant[_dwSize] = 0;    // Ensure ASCIIZ
}

/* ******************************************************************** **
** @@ DBX_COLUMN::GetSize()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

DWORD DBX_COLUMN::GetSize()
{
   return _dwSize;
}

/* ******************************************************************** **
** @@ DBX_COLUMN::GetOffset()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DWORD DBX_COLUMN::GetOffset()
{
   return _dwOffset;
}

/* ******************************************************************** **
** @@ DBX_COLUMN::SetNum()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void DBX_COLUMN::SetNum(DWORD dwNum)
{
   _iNum = dwNum;
}

/* ******************************************************************** **
** @@ DBX_COLUMN::GetNum()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

int DBX_COLUMN::GetNum()
{
   return _iNum;
}

/* ******************************************************************** **
** @@ DBX_COLUMN::GetName()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

const char* const DBX_COLUMN::GetName()
{
   return _pszName;
}

/* ******************************************************************** **
** @@ DBX_COLUMN::SetName()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void DBX_COLUMN::SetName
(
   const char* const pszName
)
{
   strncpy(_pszName,pszName,MAX_PATH);
   _pszName[MAX_PATH - 1] = 0;
}

/* ******************************************************************** **
** @@ DBX_COLUMN::SetDateFormatTemplate()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void DBX_COLUMN::SetDateFormatTemplate(const char* const pTemplate)
{
   strncpy(_pDateFormatTemplate,pTemplate,sizeof(_pDateFormatTemplate));
   _pDateFormatTemplate[sizeof(_pDateFormatTemplate) - 1] = 0;
}

/* ******************************************************************** **
** @@ DBX_COLUMN::GetDateFormatTemplate()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

const char* const DBX_COLUMN::GetDateFormatTemplate()
{
   return _pDateFormatTemplate;
}

/* ******************************************************************** **
**                End of File
** ******************************************************************** */
