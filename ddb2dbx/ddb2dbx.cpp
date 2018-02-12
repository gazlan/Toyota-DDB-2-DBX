/* ******************************************************************** **
** @@ DDB-2-DBX
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Note   :
** ******************************************************************** */

/* ******************************************************************** **
**                uses pre-compiled headers
** ******************************************************************** */

#include "stdafx.h"

#include <stdio.h>
#include <limits.h>
#include <time.h>

#include "..\shared\xlat_tables.h"
#include "..\shared\xlat.h"
#include "..\shared\text.h"
#include "..\shared\file.h"
#include "..\shared\file_walker.h"
#include "..\shared\mmf.h"
#include "..\shared\timestamp.h"
#include "..\shared\vector.h"
#include "..\shared\vector_sorted.h"
#include "..\shared\db_dbx.h"
#include "..\shared\search_bmh.h"
#include "..\shared\hash_murmur3.h"

#ifdef NDEBUG
#pragma optimize("gsy",on)
#pragma comment(linker,"/FILEALIGN:512 /MERGE:.rdata=.text /MERGE:.data=.text /SECTION:.text,EWR /IGNORE:4078")
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ******************************************************************** **
** @@                   internal defines
** ******************************************************************** */

#ifndef QWORD
typedef unsigned __int64   QWORD;
#endif

#define  MAX_RECORD_SIZE                        (128)

#define  MAX_DDB_FILE_NUM                       (2)

#define  DDB_PLAIN_REC_SIZE                     (164)

#define DATA_COMPRESS_DT_DLL                    "DataCompress_DT.dll"
#define GET_COMPRESS_VERSION                    "_GetCompressVersion@0"
#define DATA_DECODE                             "_DataDecode@12"

#define JUMBO_PATTERN                           "\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD\xCD"

typedef void (__stdcall * pDATA_DECODE)(BYTE*,DWORD,BYTE*);

enum  DDB_FILE_FORMAT
{
   FF_NONE        = 0,   
   FF_HASH_TABLE  = 0x0100,
   FF_PLAIN_TABLE = 0x0101
};

/* ******************************************************************** **
** @@ struct DDB_HEADER
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

#pragma pack(push,1)
struct DDB_HEADER
{
   DWORD             _dw1;
   DWORD             _dw2;
   WORD              _w1;
   char              _pszSignature[18];
   WORD              _w3;
   WORD              _w4;
   BYTE              _by1;
   BYTE              _by2;
   BYTE              _by3;
   BYTE              _by4;
};
#pragma pack(pop)

/* ******************************************************************** **
** @@ struct DDB_SINGLE
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

#pragma pack(push,1)
struct DDB_SINGLE
{
   WORD           _wFileType;
   DWORD          _dwRecCnt;
   DWORD          _dwPakSize;
};
#pragma pack(pop)

/* ******************************************************************** **
** @@ struct DDB_LIST
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

#pragma pack(push,1)
struct DDB_LIST
{
   DWORD          _dwNext;
   DWORD          _dw1;
   DWORD          _dw2;
   WORD           _wFileType;
   DWORD          _dwRecCnt;
   DWORD          _dwPakSize;
};
#pragma pack(pop)

/* ******************************************************************** **
** @@ struct DDB_RECORD
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

#pragma pack(push,1)
struct DDB_RECORD
{
   DWORD       _dwOfs;
   WORD        _wSize;
};
#pragma pack(pop)

/* ******************************************************************** **
** @@                   internal prototypes
** ******************************************************************** */

/* ******************************************************************** **
** @@                   external global variables
** ******************************************************************** */

extern DWORD   dwKeepError = 0;

/* ******************************************************************** **
** @@                   static global variables
** ******************************************************************** */

static const char pXLAT_UNICODE_2_WIN1251[256] =
{
   '\x00', '\xA8', '\x02', '\x03', '\xAA', '\x05', '\x06', '\xB4', 
   '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\xB6', '\x0F', 
   '\xC0', '\xC1', '\xC2', '\xC3', '\xC4', '\xC5', '\xC6', '\xC7', 
   '\xC8', '\xC9', '\xCA', '\xCB', '\xCC', '\xCD', '\xCE', '\xCF', 
   '\xD0', '\xD1', '\xD2', '\xD3', '\xD4', '\xD5', '\xD6', '\xD7', 
   '\xD8', '\xD9', '\xDA', '\xDB', '\xDC', '\xDD', '\xDE', '\xDF', 
   '\xE0', '\xE1', '\xE2', '\xE3', '\xE4', '\xE5', '\xE6', '\xE7', 
   '\xE8', '\xE9', '\xEA', '\xEB', '\xEC', '\xED', '\xEE', '\xEF', 
   '\xF0', '\xF1', '\xF2', '\xF3', '\xF4', '\xF5', '\xF6', '\xF7', 
   '\xF8', '\xF9', '\xFA', '\xFB', '\xFC', '\xFD', '\xFE', '\xFF', 
   '\x50', '\xB8', '\x52', '\x53', '\xB3', '\x55', '\x56', '\xB5', 
   '\x58', '\x59', '\x5A', '\x5B', '\x5C', '\x5D', '\xB7', '\x5F', 
   '\x60', '\x61', '\x62', '\x63', '\x64', '\x65', '\x66', '\x67', 
   '\x68', '\x69', '\x6A', '\x6B', '\x6C', '\x6D', '\x6E', '\x6F', 
   '\x70', '\x71', '\x72', '\x73', '\x74', '\x75', '\x76', '\x77', 
   '\x78', '\x79', '\x7A', '\x7B', '\x7C', '\x7D', '\x7E', '\x2E', 
   '\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x2E', 
   '\x88', '\x89', '\x8A', '\x8B', '\x8C', '\x8D', '\x8E', '\x8F', 
   '\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97', 
   '\x98', '\x99', '\x9A', '\x9B', '\x9C', '\x9D', '\x9E', '\x9F', 
   '\xA0', '\xA1', '\xA2', '\xA3', '\xA4', '\xA5', '\xA6', '\xA7', 
   '\xA8', '\xA9', '\xAA', '\xAB', '\xAC', '\xAD', '\xAE', '\xAF', 
   '\xB0', '\xB1', '\xB2', '\xB3', '\xB4', '\xB5', '\xB6', '\xB7', 
   '\xB8', '\xB9', '\xBA', '\xBB', '\xBC', '\xBD', '\xBE', '\xBF', 
   '\xC0', '\xC1', '\xC2', '\xC3', '\xC4', '\xC5', '\xC6', '\xC7', 
   '\xC8', '\xC9', '\xCA', '\xCB', '\xCC', '\xCD', '\xCE', '\x2E', 
   '\xD0', '\xD1', '\xD2', '\xD3', '\xD4', '\xD5', '\xD6', '\xD7', 
   '\xD8', '\xD9', '\xDA', '\xDB', '\xDC', '\xDD', '\xDE', '\xDF', 
   '\xE0', '\xE1', '\xE2', '\xE3', '\xE4', '\xE5', '\xE6', '\xE7', 
   '\xE8', '\xE9', '\xEA', '\xEB', '\xEC', '\xED', '\xEE', '\xEF', 
   '\xF0', '\xF1', '\xF2', '\xF3', '\xF4', '\xF5', '\x2E', '\x2E', 
   '\xF8', '\xF9', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E' 
};

static DBX           _DBF;
static DBX_TABLE*    _pTable = NULL;

static DWORD      _dwStart   = 0;
static DWORD      _dwPacked  = 0;
static DWORD      _dwNext    = 0;
static DWORD      _dwRecCnt  = 0;
static DWORD      _dwPakSize = 0;
static BYTE       _byBlock   = 0;

static DWORD      _dwSizeOrg = 0;
static DWORD      _pHashOrg[4];

static BYTE*      _pJumbo = NULL;

static DWORD      _dwGranulation = 3; // 8 bytes granulation // 2 Power: 0, 2, 3, 4

// static HANDLE     _hDBF  = INVALID_HANDLE_VALUE;
static HANDLE     _hMemo = INVALID_HANDLE_VALUE;

static DDB_FILE_FORMAT     _pFileType[MAX_DDB_FILE_NUM];

static BYTE       _Dummy[MAX_PATH];

/* ******************************************************************** **
** @@                   real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ Error()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Note   :
** ******************************************************************** */

static void Error()
{
   void*    pMsgBuf;

   FormatMessage
   (
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,   // Source and Processing Options
      NULL,                                                          // Pointer to Message Source
      GetLastError(),                                                // Requested Message Identifier
      MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),                      // Language Identifier for Requested Message
      (LPTSTR)&pMsgBuf,                                              // Pointer to Message Buffer
      0,                                                             // Maximum Size of Message Buffer
      NULL                                                           // Address of Array of Message Inserts
   );

   perror((const char*)pMsgBuf);

   LocalFree(pMsgBuf);
}

/* ******************************************************************** **
** @@ GetInfo()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Note   :
** ******************************************************************** */

static DWORD GetInfo(const BYTE* const pBuf)
{
   _dwStart   = 0;
   _dwPacked  = 0;
   _dwNext    = 0;
   _dwRecCnt  = 0;
   _dwPakSize = 0;

   ASSERT(pBuf);

   BYTE*    pDataEx  = NULL;
   DWORD    dwSizeEx = 0;

   if (!_byBlock)
   {
      _dwStart = *(DWORD*)(pBuf + sizeof(DDB_HEADER));

      if (!_dwStart)
      {
         // Smth special !
         return 0;
      }
   
      if (IsBadReadPtr(pBuf,_dwStart))
      {
         // Error !
         ASSERT(0);
         return 0;
      }

      dwSizeEx = _dwStart - sizeof(DDB_HEADER) - sizeof(DWORD);

      pDataEx = (BYTE*)(pBuf + sizeof(DDB_HEADER) + sizeof(DWORD));

      if (IsBadReadPtr(pDataEx,dwSizeEx))
      {
         // Error !
         ASSERT(0);
         return 0;
      }
   }
   else
   {
      pDataEx  = (BYTE*)pBuf;
      dwSizeEx = 0;
   }

   if (dwSizeEx)
   {
      DDB_LIST*      pInfo = (DDB_LIST*)pDataEx;

      _dwNext    = pInfo->_dwNext;
      _dwRecCnt  = pInfo->_dwRecCnt;
      _dwPakSize = pInfo->_dwPakSize;
   }
   else
   {
      DDB_SINGLE*    pInfo = (DDB_SINGLE*)pDataEx;

      _dwNext    = 0;
      _dwRecCnt  = pInfo->_dwRecCnt;
      _dwPakSize = pInfo->_dwPakSize;
   }
   
   _dwPacked = _dwStart + sizeof(DDB_SINGLE);

   return dwSizeEx;
}

/* ******************************************************************** **
** @@ Unpack()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Note   :
** ******************************************************************** */

static int Unpack(const BYTE* const pBuf)
{
   if (_pJumbo)
   {
      delete[] _pJumbo;
      _pJumbo = NULL;
   }   

   HINSTANCE      pDLL = LoadLibrary(DATA_COMPRESS_DT_DLL);

   if (!pDLL)      
   {
      // Error !
      ASSERT(0);
      Error();
      return -2;
   }

   FARPROC     pVersion = GetProcAddress(pDLL,GET_COMPRESS_VERSION);

   if (!pVersion)
   {
      // Error !
      ASSERT(0);
      Error();
      return -3;
   }

   // VIEWERDB.DLL
   // 003C2950 ?MakeTable@CDbTableRead@@SAPAV1@GF@Z proc near
   // const DWORD       JUMBO_BUF_SIZE = 0x07E90003; // 132,710,403
   const DWORD       JUMBO_BUF_SIZE = (1 << 27);   // 08000000 ; 134,217,728

   _pJumbo = new BYTE[JUMBO_BUF_SIZE];

   memset(_pJumbo,0xCD,JUMBO_BUF_SIZE);

   FARPROC     pDataDecode = GetProcAddress(pDLL,DATA_DECODE);

   if (!pDataDecode)
   {
      // Error !
      ASSERT(0);
      Error();
      return -4;
   }

   ((pDATA_DECODE)pDataDecode)((BYTE*)(pBuf + _dwPacked),_dwPakSize,_pJumbo);

  if (!FreeLibrary(pDLL)) 
   {
      // Error !
      ASSERT(0);
      Error();
      return -5;
   }

   int   iFound = BMH_Search(_pJumbo,JUMBO_BUF_SIZE,(BYTE*)JUMBO_PATTERN,sizeof(JUMBO_PATTERN) - 1); 

   #ifdef _DEBUG   
   HANDLE hFile = CreateFile("dump.bin");
   WriteBuffer(hFile,_pJumbo,JUMBO_BUF_SIZE);
   CloseHandle(hFile);
   #endif

   return iFound;
}

/* ******************************************************************** **
** @@ AppendFix()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void AppendFix
(
   DWORD       dwIndex,
   DWORD       dwText
)
{
   BYTE     pRecord[MAX_RECORD_SIZE];

   memset(pRecord,0x20,MAX_RECORD_SIZE);

   DBX_COLUMN*    pIndex = _pTable->GetColumn("INDEX");
   DBX_COLUMN*    pGuid  = _pTable->GetColumn("GUID");
   DBX_COLUMN*    pText  = _pTable->GetColumn("TEXT");

   ASSERT(pIndex);
   ASSERT(pGuid);
   ASSERT(pText);

   pIndex->Set(pRecord,&dwIndex);
   pGuid-> Set(pRecord,_pHashOrg);
   pText-> Set(pRecord,&dwText);

   _pTable->InsertRecord((BYTE*)pRecord);
}

/* ******************************************************************** **
** @@ AppendVar()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static DWORD AppendVar(const BYTE* const pData,DWORD dwSize)
{
   ASSERT(pData);
   ASSERT(dwSize);
   ASSERT(_hMemo != INVALID_HANDLE_VALUE);

   DWORD    dwFileSize = GetFileSize(_hMemo,NULL);

   DWORD    dwBound = 0;

   if (_dwGranulation)
   {
      dwBound = ((dwFileSize + ((1 << _dwGranulation) - 1)) >> _dwGranulation) << _dwGranulation;
   }
   else
   {
      dwBound = dwFileSize;
   }

   DWORD    dwDelta = dwBound - dwFileSize;

   ASSERT(dwDelta < (WORD)(1 << _dwGranulation));

   if (dwDelta)
   {
      WriteBuffer(_hMemo,&_Dummy,dwDelta);
   }

   WriteBuffer(_hMemo,&dwSize,sizeof(DWORD));
   WriteBuffer(_hMemo,(void*)pData,dwSize);

   return dwBound;
}

/* ******************************************************************** **
** @@ Append()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void Append
(
   DWORD                   dwIndex,
   const BYTE* const       pText,
   DWORD                   dwSize
)
{
   ASSERT(pText);
   ASSERT(dwSize);

   DWORD    dwText = AppendVar(pText,dwSize);

   AppendFix(dwIndex,dwText);
}

/* ******************************************************************** **
** @@ InsertHashTable()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void InsertHashTable()
{
   ASSERT(_dwRecCnt);

   BYTE*    pASCII = new BYTE[USHRT_MAX];

   DWORD    dwDataOfs = _dwRecCnt * sizeof(DDB_RECORD);

   BYTE*    pData = _pJumbo + dwDataOfs;

   for (DWORD ii = 0; ii < _dwRecCnt; ++ii)
   {
      DDB_RECORD*    pRec = (DDB_RECORD*)(_pJumbo + sizeof(DDB_RECORD) * ii);

      if (pRec->_wSize)
      {
         BYTE*    pItem = pData + pRec->_dwOfs;

         ASSERT(pItem);

         _dwSizeOrg = pRec->_wSize;

         MurmurHash3_x86_128(pItem,_dwSizeOrg,MM3_DEFAULT_HASH_SEED,_pHashOrg);

         int   kk = 0;

         for (DWORD jj = 0; jj < pRec->_wSize; jj += 2)
         {
            char     cValue = 0;

            switch (pItem[jj + 1])
            {
               case 0:  // English
               default:
               {
                  cValue = pItem[jj];
                  break;
               }
               case 4:  // Russian
               {
                  cValue = pXLAT_UNICODE_2_WIN1251[pItem[jj]];
                  break;
               }
            }
     
            pASCII[kk++] = cValue;
         }

         pASCII[kk] = 0;    // Ensure ASCIIZ

         Append(ii,pASCII,kk);  // FF_HASH_TABLE
      }
   }

   if (pASCII)
   {
      delete[] pASCII;
      pASCII = NULL;
   }
}

/* ******************************************************************** **
** @@ InsertPlainTable()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */
/*
static void InsertPlainTable()
{
   ASSERT(_dwRecCnt);

   BYTE*    pASCII = new BYTE[USHRT_MAX];

   for (DWORD ii = 0; ii < _dwRecCnt; ++ii)
   {
      BYTE*       pRec = _pJumbo + DDB_PLAIN_REC_SIZE * ii;

      if (pRec)
      {
         BYTE*    pItem = pRec;

         ASSERT(pItem);

         _dwSizeOrg = DDB_PLAIN_REC_SIZE;

         MurmurHash3_x86_128(pItem,_dwSizeOrg,MM3_DEFAULT_HASH_SEED,_pHashOrg);

         int   kk = 0;

         for (DWORD jj = 0; jj < DDB_PLAIN_REC_SIZE; jj += 2)
         {
            char     cValue = 0;

            switch (pItem[jj + 1])
            {
               case 0:  // English
               default:
               {
                  cValue = pItem[jj];
                  break;
               }
               case 4:  // Russian
               {
                  cValue = pXLAT_UNICODE_2_WIN1251[pItem[jj]];
                  break;
               }
            }
     
            pASCII[kk++] = cValue;
         }

         pASCII[kk] = 0;    // Ensure ASCIIZ

         Append(ii,pASCII,kk);   // FF_PLAIN_TABLE
      }
   }

   if (pASCII)
   {
      delete[] pASCII;
      pASCII = NULL;
   }
}
*/

/* ******************************************************************** **
** @@ GetFileInfo()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void GetFileInfo(const BYTE* const pBuf,DWORD dwSize)
{
   if (IsBadReadPtr(pBuf,dwSize))
   {
      // Error !
      ASSERT(0);
      return;
   }
               
   memset(_pFileType,0,sizeof(DWORD) * MAX_DDB_FILE_NUM);

   ASSERT(pBuf);

   DWORD    dwStart = *(DWORD*)(pBuf + sizeof(DDB_HEADER));

   if (!dwStart)
   {
      // Smth special !
      return;
   }
   
   if (IsBadReadPtr(pBuf,dwStart))
   {
      // Error !
      ASSERT(0);
      return;
   }

   DWORD    dwSizeEx = dwStart - sizeof(DDB_HEADER) - sizeof(DWORD);

   BYTE*    pDataEx = (BYTE*)(pBuf + sizeof(DDB_HEADER) + sizeof(DWORD));

   if (IsBadReadPtr(pDataEx,dwSizeEx))
   {
      // Error !
      ASSERT(0);
      return;
   }

   if (dwSizeEx)
   {
      // List
      DDB_LIST*      pLBlock = (DDB_LIST*)pDataEx;

      if (IsBadReadPtr(pLBlock,sizeof(DDB_LIST)))
      {
         // Error !
         ASSERT(0);
         return;
      }

      _pFileType[0] = (DDB_FILE_FORMAT)pLBlock->_wFileType;

      if (pLBlock->_dwNext)
      {
         DDB_SINGLE*    pSBlock = (DDB_SINGLE*)(pBuf + pLBlock->_dwNext);

         if (IsBadReadPtr(pSBlock,sizeof(DDB_SINGLE)))
         {
            // Error !
            ASSERT(0);
            return;
         }
               
         _pFileType[1] = (DDB_FILE_FORMAT)pSBlock->_wFileType;
      }
   }
   else
   {
      // Single
      DDB_SINGLE*    pSBlock = (DDB_SINGLE*)pDataEx;

      if (IsBadReadPtr(pSBlock,sizeof(DDB_SINGLE)))
      {
         // Error !
         ASSERT(0);
         return;
      }
               
      _pFileType[0] = (DDB_FILE_FORMAT)pSBlock->_wFileType;
   }
}

/* ******************************************************************** **
** @@ Import()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void Import(const char* const pszFile)
{
   MMF*     pDDB = new MMF;

   if (!pDDB->OpenReadOnly(pszFile))
   {
      // Error !
      ASSERT(0);
      Error();
      return;
   }

   BYTE*       pBuf   = pDDB->Buffer();
   DWORD       dwSize = pDDB->Size();

   ASSERT(pBuf && dwSize && (dwSize > sizeof(DDB_HEADER)));

   if (IsBadReadPtr(pBuf,sizeof(DDB_HEADER)))
   {
      // Error !
      ASSERT(0);
      return;
   }

   if (memcmp(pBuf + 0x0A,"DiagTool DataCtrl",0x12))
   {
      // Error !
      ASSERT(0);
      return;
   }

   _byBlock = 0;

   GetFileInfo(pBuf,dwSize);

   do 
   {
      /*DWORD    dwRes = */ GetInfo(pBuf);

      int   iUnpackedSize = Unpack(pBuf);

      ASSERT(iUnpackedSize > 0);
      
      switch (_pFileType[_byBlock])
      {
         case FF_HASH_TABLE: // 0x0100
         {
            InsertHashTable();
            break;
         }
         case FF_PLAIN_TABLE: // 0x0101
         {
//            InsertPlainTable();
            break;
         }
         default:
         {
            // Error !
            // Unsupported type
            ASSERT(0);
            break;
         }
      }

      pBuf += _dwNext; 

      ++_byBlock;
   }
   while (_dwNext);

   pDDB->Close();

   delete pDDB;
   pDDB = NULL;

   if (_pJumbo)
   {
      delete[] _pJumbo;
      _pJumbo = NULL;
   }   
}

/* ******************************************************************** **
** @@ Creator()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void Creator(const DBX_TABLE_INFO& rInfo)
{
   if (!_DBF.CreateEmptyTable(&rInfo))
   {
      // Error !
      return;
   }

   DBX_TABLE*     pTable = _DBF.OpenTable((char*)&rInfo._pszName,(char*)&rInfo._pszName,NULL,DBX_OM_READ_WRITE,DBX_OM_READ_WRITE);
   
   if (!pTable)
   {
      // Error !
      ASSERT(0);
      return;
   }

   _DBF.CloseTable(pTable);
}

/* ******************************************************************** **
** @@ ForEach()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void ForEach(const char* const pszFilename)
{
   const int      FIELD_CNT = 3;

   DBX_COLUMN_DESCRIPTOR    pFieldArr[FIELD_CNT];

   memset(pFieldArr,0,sizeof(DBX_COLUMN_DESCRIPTOR) * FIELD_CNT);

   // 1. IDX
   DefineField(pFieldArr,0,"INDEX",DBX_FLT_CHARACTER,DBX_FF_BINARY,sizeof(DWORD));
   // 2. Hash
   DefineField(pFieldArr,1,"GUID", DBX_FLT_CHARACTER,DBX_FF_BINARY,sizeof(DWORD) * 4);
   // 3. Memo Position
   DefineField(pFieldArr,2,"TEXT", DBX_FLT_CHARACTER,DBX_FF_BINARY,sizeof(DWORD));

   DBX_TABLE_INFO      Info;

   Info._FileType  = DBX_FT_FOX_BASE_PLUS_NO_MEMO;
   Info._iCnt      = FIELD_CNT;
   Info._pFieldArr = pFieldArr;

   char     pszDBFName [_MAX_PATH];
   char     pszMemoName[_MAX_PATH];
   char     pszDrive   [_MAX_DRIVE];
   char     pszDir     [_MAX_DIR];
   char     pszFName   [_MAX_FNAME];
   char     pszExt     [_MAX_EXT];

   _splitpath(pszFilename,pszDrive,pszDir,pszFName,pszExt);
   _makepath( pszDBFName, pszDrive,pszDir,pszFName,"dbf");
   _makepath( pszMemoName,pszDrive,pszDir,pszFName,"dbv");

   strcpy((char*)&Info._pszName,pszDBFName);

   Creator(Info);

   _pTable = _DBF.OpenTable(pszFName,pszDBFName,NULL,DBX_OM_READ_WRITE,DBX_OM_READ_WRITE);

   if (!_pTable)
   {
      // Error !
      ASSERT(0);
      return;
   }

   _hMemo = CreateFile(pszMemoName);

   if (_hMemo == INVALID_HANDLE_VALUE)
   {
      // Error !
      ASSERT(0);
      return;
   }
   
   DWORD    dwFreeListHead = 0;

   WriteBuffer(_hMemo,&_dwGranulation,sizeof(DWORD));
   WriteBuffer(_hMemo,&dwFreeListHead,sizeof(DWORD));

   Import(pszFilename);

   CloseHandle(_hMemo);
   _hMemo = INVALID_HANDLE_VALUE;

   _DBF.CloseTable(_pTable);
}

/* ******************************************************************** **
** @@ ShowHelp()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void ShowHelp()
{
   const char  pszCopyright[] = "-*-   DDB-2-DBX  1.0   *   Copyright (c) Gazlan, 2015   -*-";
   const char  pszDescript [] = "Toyota DDB to DBX converter";
   const char  pszE_Mail   [] = "complains_n_suggestions direct to gazlan@yandex.ru";

   printf("%s\n\n",pszCopyright);
   printf("%s\n\n",pszDescript);
   printf("Usage: ddb2dbx.com wildcards\n\n");
   printf("%s\n",pszE_Mail);
}

/* ******************************************************************** **
** @@ main()
** @ Copyrt:
** @ Author:
** @ Modify:
** @ Update:
** @ Notes :
** ******************************************************************** */

int main(int argc,char** argv)
{
   if (argc != 2)
   {
      ShowHelp();
      return 0;
   }

   if (argc == 2 && ((!strcmp(argv[1],"?")) || (!strcmp(argv[1],"/?")) || (!strcmp(argv[1],"-?")) || (!stricmp(argv[1],"/h")) || (!stricmp(argv[1],"-h"))))
   {
      ShowHelp();
      return 0;
   }

   char     pszMask[MAX_PATH + 1];
   
   memset(pszMask,0,sizeof(pszMask));
   
   strncpy(pszMask,argv[1],MAX_PATH);
   pszMask[MAX_PATH] = 0; // Ensure ASCIIZ
   
   char     pszDrive   [_MAX_DRIVE];
   char     pszDir     [_MAX_DIR];
   char     pszFName   [_MAX_FNAME];
   char     pszExt     [_MAX_EXT];
   
   _splitpath(pszMask,pszDrive,pszDir,pszFName,pszExt);
   
   char     pszSrchMask[MAX_PATH + 1];
   char     pszSrchPath[MAX_PATH + 1];
   
   strcpy(pszSrchMask,pszFName);
   strcat(pszSrchMask,pszExt);
   
   Walker      Visitor;

   Visitor.Init(ForEach,pszSrchMask,false);

   strcpy(pszSrchPath,pszDrive);
   strcat(pszSrchPath,pszDir);

   Visitor.Run(*pszSrchPath  ?  pszSrchPath  :  ".");

   return 0;
}

/* ******************************************************************** **
**                That's All
** ******************************************************************** */
