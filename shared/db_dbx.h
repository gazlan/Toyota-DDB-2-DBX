/* ******************************************************************** **
** @@ DB_DBX
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

#ifndef __DB_DBX_HPP__
#define __DB_DBX_HPP__

#if _MSC_VER > 1000
#pragma once
#endif

/* ******************************************************************** **
** @@                   internal defines
** ******************************************************************** */

#ifndef QWORD
typedef unsigned __int64         QWORD;
#endif

#define  MAX_DATE_FORMAT_SIZE       (20)

#define  DBX_CS_TRIM                            (0x00000001)
#define  DBX_CS_EXPAND                          (0x00000002)
#define  DBX_CS_RUSSIAN_OEM_TO_ANSI             (0x00000004)
#define  DBX_CS_RUSSIAN_ANSI_TO_OEM             (0x00000008)
#define  DBX_CS_SET_CASE_LOWER_ENGLISH          (0x00000010)
#define  DBX_CS_SET_CASE_UPPER_ENGLISH          (0x00000020)
#define  DBX_CS_SET_CASE_CAPTION_ENGLISH        (0x00000040)
#define  DBX_CS_SET_CASE_INVERSE_ENGLISH        (0x00000080)
#define  DBX_CS_SET_CASE_LOWER_RUSSIAN_OEM      (0x00000100)
#define  DBX_CS_SET_CASE_UPPER_RUSSIAN_OEM      (0x00000200)
#define  DBX_CS_SET_CASE_CAPTION_RUSSIAN_OEM    (0x00000400)
#define  DBX_CS_SET_CASE_INVERSE_RUSSIAN_OEM    (0x00000800)
#define  DBX_CS_SET_CASE_LOWER_RUSSIAN_ANSI     (0x00001000)
#define  DBX_CS_SET_CASE_UPPER_RUSSIAN_ANSI     (0x00002000)
#define  DBX_CS_SET_CASE_CAPTION_RUSSIAN_ANSI   (0x00004000)
#define  DBX_CS_SET_CASE_INVERSE_RUSSIAN_ANSI   (0x00008000)
#define  DBX_CS_CORRECT_INPROPER_RUSSIAN_OEM    (0x00010000)
#define  DBX_CS_CORRECT_INPROPER_ENGLISH_OEM    (0x00020000)
#define  DBX_CS_CORRECT_INPROPER_RUSSIAN_ANSI   (0x00040000)
#define  DBX_CS_CORRECT_INPROPER_ENGLISH_ANSI   (0x00080000)

/* ******************************************************************** **
** @@ enum DBX_OPEN_MODE
** @ Copyrt  : 
** @ Author  : 
** @ Modify  :
** @ Update  :
** @ Notes   : 
** ******************************************************************** */

enum DBX_OPEN_MODE
{
   DBX_OM_NONE,
   DBX_OM_READ_ONLY,
   DBX_OM_IN_PLACE,
   DBX_OM_READ_WRITE
};
                                      
/* ******************************************************************** **
** @@ enum DBX_FLUSH_MODE
** @ Copyrt  : 
** @ Author  : 
** @ Modify  :
** @ Update  :
** @ Notes   : 
** ******************************************************************** */

enum DBX_FLUSH_MODE
{
   DBX_FM_FLUSH_NEVER       = 0,
   DBX_FM_FLUSH_IMMEDIATELY = 1,
   DBX_FM_FLUSH_ON_TIMER    = 2,
   DBX_FM_FLUSH_ON_CLOSE    = 3,
   DBX_FM_BARRIER_FLUSH     = 4
};

/* ******************************************************************** **
** @@ enum DBX_ERROR_TYPE
** @ Copyrt  : 
** @ Author  : 
** @ Modify  :
** @ Update  :
** @ Notes   : 
** ******************************************************************** */

enum DBX_ERROR
{
   DBX_ERR_NOERR            = 0,
   DBX_ERR_MEMORY           = -1,
   DBX_ERR_RECORD_NOT_FOUND = -2,
   DBX_ERR_SECTOR_SIZE      = -3
};

/* ******************************************************************** **
** @@ enum DBX_ERASE_MODE
** @ Copyrt  : 
** @ Author  : 
** @ Modify  :
** @ Update  :
** @ Notes   : 
** ******************************************************************** */

enum DBX_ERASE_MODE
{
   DBX_EM_ERASE_NONE        = 0,
   DBX_EM_ERASE_IMMEDIATELY = 1,
   DBX_EM_ERASE_KEEP_ERASED = 2
};

/* ******************************************************************** **
** @@ enum DBX_HIDE_MODE
** @ Copyrt  : 
** @ Author  : 
** @ Modify  :
** @ Update  :
** @ Notes   : 
** ******************************************************************** */

enum DBX_HIDE_MODE
{
   DBX_EH_HIDE_NONE   = 0,
   DBX_EH_HIDE_ERASED = 1,
   DBX_EH_SHOW_ERASED = 2
};

enum DBX_FILE_TYPE
{
   DBX_FT_UNKNOWN                            = 0,
   DBX_FT_FOX_BASE                           = 0x02,
   DBX_FT_FOX_BASE_PLUS_NO_MEMO              = 0x03,
   DBX_FT_VISUAL_FOX_PRO                     = 0x30,   
   DBX_FT_DBASE_IV_SQL_TABLE_FILES_NO_MEMO   = 0x43,
   DBX_FT_DBASE_IV_SQL_SYSTEM_FILES_NO_MEMO  = 0x63,
   DBX_FT_FOX_BASE_PLUS_WITH_MEMO            = 0x83,
   DBX_FT_DBASE_IV_WITH_MEMO                 = 0x8B,
   DBX_FT_DBASE_IV_SQL_TABLE_FILES_WITH_MEMO = 0xCB,
   DBX_FT_FOX_2X_WITH_MEMO                   = 0xF5,
   DBX_FT_FOX_BASE_OTHER                     = 0xFB
};

enum DBX_FIELD_TYPE
{
   DBX_FLT_UNKNOWN          = 'X',
   DBX_FLT_BINARY           = 'B', // 10 digits representing a .DBT block number. The number is stored as a string, right justified and padded with blanks.
   DBX_FLT_CHARACTER        = 'C', // All OEM code page characters - padded with blanks to the width of the field
   DBX_FLT_CURRENCY         = 'Y',
   DBX_FLT_NUMERIC          = 'N', // Number stored as a string, right justified, and padded with blanks to the width of the field.
   DBX_FLT_FLOAT            = 'F', // Number stored as a string, right justified, and padded with blanks to the width of the field.
   DBX_FLT_DATE             = 'D', // 8 bytes - date stored as a string in the format YYYYMMDD.
   DBX_FLT_DATETIME         = 'T',
   DBX_FLT_DOUBLE           = 'O', // 8 bytes - no conversions, stored as a double.
   DBX_FLT_INTEGER          = 'I',
   DBX_FLT_LOGICAL          = 'L', // 1 byte - initialized to 0x20 (space) otherwise T or F
   DBX_FLT_MEMO             = 'M', // 10 digits (bytes) representing a .DBT block number. The number is stored as a string, right justified and padded with blanks.
   DBX_FLT_GENERAL          = 'G', // OLE, 10 digits (bytes) representing a .DBT block number. The number is stored as a string, right justified and padded with blanks.
   DBX_FLT_CHARACTER_BINARY = 'c',
   DBX_FLT_MEMO_BINARY      = 'm',
   DBX_FLT_PICTURE          = 'P',
   DBX_FLT_VAR_BINARY       = 'Q',
   DBX_FLT_VARCHAR_BINARY   = 'V',
   DBX_FLT_TIMESTAMP        = '@', // 8 bytes - two longs, first for date, second for time.  The date is the number of days since 01/01/4713 BC. Time is hours * 3600000L + minutes * 60000L + Seconds * 1000L
   DBX_FLT_LONG             = '|', // 4 bytes. Leftmost bit used to indicate sign, 0 negative.
   DBX_FLT_AUTOINCREMENT    = '+'  // Same as a Long
};

enum DBX_FIELD_FLAGS
{
   DBX_FF_NONE                 = 0,
   DBX_FF_SYSTEM_COLUMN        = 0x01,
   DBX_FF_CAN_STORE_NULL_VALUE = 0x02, 
   DBX_FF_BINARY               = 0x04
};

/* ******************************************************************** **
** @@ struct DBX_COLUMN_DESCRIPTOR
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */
  
struct DBX_COLUMN_DESCRIPTOR
{
   DBX_FIELD_TYPE       _FieldType;
   DWORD                _dwReadStyle;
   DWORD                _dwWriteStyle;
   char                 _cFiller;
   char                 _pDateFormatTemplate[MAX_DATE_FORMAT_SIZE];
   char                 _pszName[MAX_PATH + 1];
   int                  _iNum;
   DWORD                _dwSize;
   DWORD                _dwOffset;
   DBX_FIELD_FLAGS      _FieldFlags;
   BYTE                 _byDecimalPoint;
};

/* ******************************************************************** **
** @@ struct DBX_TABLE_INFO
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */
  
struct DBX_TABLE_INFO
{
   DBX_FILE_TYPE                 _FileType;
   char                          _pszName[MAX_PATH + 1];
   int                           _iCnt;
   DBX_COLUMN_DESCRIPTOR*        _pFieldArr;
};

#define  DBF_HEADER_TERMINATOR      (0x0D)
#define  DBF_EOF_MARKER             (0x1A)
#define  DBF_NAME_MAX_SIZE          (10 + 1)

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
** @@                   Classes
** ******************************************************************** */

/* ******************************************************************** **
** @@ DBV_HEADER
** @ Copyrt  : 
** @ Author  : 
** @ Modify  :
** @ Update  :
** @ Notes   : 
** ******************************************************************** */

#pragma pack(push,1)
struct DBV_HEADER
{
   DWORD       _dwGranulation;
   DWORD       _dwFreeListHead;
};
#pragma pack(pop)

/* ******************************************************************** **
** @@ DBV_DELETED
** @ Copyrt  : 
** @ Author  : 
** @ Modify  :
** @ Update  :
** @ Notes   : 
** ******************************************************************** */

#pragma pack(push,1)
struct DBV_DELETED
{
   DWORD       _dwSize;
   DWORD       _dwNext;
};
#pragma pack(pop)

/* ******************************************************************** **
** @@ DBX_HEADER
** @ Copyrt  : 
** @ Author  : 
** @ Modify  :
** @ Update  :
** @ Notes   : 
** ******************************************************************** */

#pragma pack(push,1)
struct DBX_HEADER
{
   BYTE        _byFileType;
   BYTE        _byYear;
   BYTE        _byMonth;
   BYTE        _byDay;
   DWORD       _dwRecCnt;
   WORD        _wDataStart;
   WORD        _wRecSize;
   WORD        _wReserved1;
   BYTE        _byTransaction;
   BYTE        _byEncryption;
   DWORD       _dwFreeListHead;
   DWORD       _dwLastID;
   DWORD       _dwCurRec;
   BYTE        _byTableType;
   BYTE        _byCodePage;
   WORD        _wReserved2;
};
#pragma pack(pop)

/* ******************************************************************** **
** @@ DBX_FIELD
** @ Copyrt  : 
** @ Author  : 
** @ Modify  :
** @ Update  :
** @ Notes   : 
** ******************************************************************** */

#pragma pack(push,1)
struct DBX_FIELD
{
   char        _pszName[DBF_NAME_MAX_SIZE];
   BYTE        _byType;
   DWORD       _dwDisplacement;
   BYTE        _bySize;
   BYTE        _byDecimalPoint;
   BYTE        _byFlags;
   DWORD       _dwAutoNext;
   BYTE        _byAutoStep;
   char        _pReserved[8];
};
#pragma pack(pop)

/* ******************************************************************** **
** @@ class DBX_COLUMN : protected DBX_COLUMN_DESCRIPTOR
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */
/*  
class DBX_COLUMN : protected DBX_COLUMN_DESCRIPTOR
{
   protected:

      int                     _iErr;

   protected:

      DBX_COLUMN();

   public:     

                DBX_COLUMN(const DBX_COLUMN_DESCRIPTOR& rDescriptor);
      virtual  ~DBX_COLUMN();

      virtual const void* const  Get(const void* const pBuf) = 0;
      virtual void               Set(void* pBuf,const void* const pValue) = 0;

      const char* const GetName();
      void              SetName(const char* const pszName);

      DBX_FIELD_TYPE    GetType();
      void              SetType(DBX_FIELD_TYPE FieldType);

      DWORD             GetReadStyle();
      void              SetReadStyle(DWORD dwStyle);

      DWORD             GetWriteStyle();
      void              SetWriteStyle(DWORD dwStyle);

      int               GetFiller();
      void              SetFiller(int iFiller);

      void              GetDescriptor(DBX_COLUMN_DESCRIPTOR& rDescriptor);
      void              SetDescriptor(const DBX_COLUMN_DESCRIPTOR& rDescriptor);

      int               GetLastErr();
};
*/

/* ******************************************************************** **
** @@ class DBX_COLUMN : private DBX_COLUMN_DESCRIPTOR
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */

class DBX_COLUMN : private DBX_COLUMN_DESCRIPTOR
{
   private:

      char              _pVariant[MAX_PATH + 1];
      Timestamp         _TStamp;

   public:

                DBX_COLUMN();
                DBX_COLUMN(const DBX_COLUMN_DESCRIPTOR& rDescriptor);
      virtual  ~DBX_COLUMN();
               
      DWORD    GetOffset();
      DWORD    GetSize();

      void     SetNum(DWORD dwNum);
      int      GetNum();

      const char* const    GetName();
      void                 SetName(const char* const pszName);

      const void* const    Get(const void* const pBuf);
      void                 Set(void* pBuf,const void* const pValue);

      void  SetType(DBX_FIELD_TYPE FieldType);
      void  SetDisplacement(DWORD dwDisplacement);
      void  SetParams(DBX_FILE_TYPE FileType,const DBX_FIELD* const pField);

   private:

      void  Expand();

      const void* const    Get_DBF_Time  (const void* const pBuf);
      const void* const    Get_DBF_Date  (const void* const pBuf);
      const void* const    Get_DBF_String(const void* const pBuf);

      void  Set_DBF_Date  (void* pBuf,const void* const pValue);
      void  Set_DBF_String(void* pBuf,const void* const pValue);

      const char* const GetDateFormatTemplate();
      void              SetDateFormatTemplate(const char* const pTemplate);
};

/* ******************************************************************** **
** @@ DBX_TABLE
** @ Copyrt  : 
** @ Author  : 
** @ Modify  :
** @ Update  :
** @ Notes   : 
** ******************************************************************** */
/*
class DBX_TABLE : public Vector
{
   protected: 
                                    
      char                          _pszHdrName[MAX_PATH];
      DWORD                         _dwSizeVar;
//      MappedFile*                 _pVarFile;  // Temporary !
//      SortedVector                  _TContext;
//      CRITICAL_SECTION              _AccessSync;
      int                           _iErr;
//      DWORD                         _dwOperationThreshold;
//      DWORD                         _dwOperationCounter;

   public:

                DBX_TABLE();
      virtual  ~DBX_TABLE();

      virtual DWORD  GetOperationThreshold();
      virtual void   SetOperationThreshold(DWORD dwNewThreshold);

      virtual DBX_FLUSH_MODE  GetFlushMode() = 0;
      virtual void            SetFlushMode(DBX_FLUSH_MODE FlushMode) = 0;
                     
      virtual DWORD  InsertEmptyRecord() = 0;
      virtual DWORD  InsertRecord     (const BYTE* const pRecord) = 0;
      virtual bool   DuplicateRecord  (DWORD dwRecID) = 0;
      virtual bool   UpdateRecord     (DWORD dwRecID,const BYTE* const pRecord) = 0;
      virtual bool   DeleteRecord     (DWORD dwRecID) = 0;

      virtual bool   DeleteAll() = 0;
      virtual void   Unerase()   = 0;

      virtual DWORD  Current() = 0;
      virtual DWORD  First  () = 0;
      virtual DWORD  Last   () = 0;
      virtual DWORD  Prev   () = 0;
      virtual DWORD  Next   () = 0;

      virtual const BYTE* const Go(DWORD dwRecID) = 0;

      virtual bool   Open(const char* const pszName,DBX_OPEN_MODE Mode = DBX_OM_READ_ONLY,DBX_FLUSH_MODE FlushMode = DBX_FM_FLUSH_ON_CLOSE) = 0;
      virtual bool   Close() = 0;
            
      virtual BYTE*  GetFixBuf() = 0;
      virtual BYTE*  GetVarBuf() = 0;

      virtual DWORD  GetFixSize() = 0;
      virtual DWORD  GetVarSize() = 0;

      virtual int    FindFixColumnByOffset(DWORD dwOfs) = 0;
      virtual DWORD  GetFutureRecID() = 0;
      virtual DWORD  GetRecID(const BYTE* const pRec) = 0;

      virtual int    InsertEmptyColumn() = 0;
      virtual int    InsertColumn(const DBX_COLUMN_DESCRIPTOR* const pDescr) = 0;
      virtual int    InsertColumn(const DBX_COLUMN_DESCRIPTOR& rDescr) = 0;
      virtual bool   InsertColumn(const DBX_COLUMN* const pColumn) = 0;
      virtual bool   DeleteColumn(const char* const pszName) = 0;
      virtual bool   DeleteColumn(DWORD dwNum) = 0;
      virtual bool   RenameColumn(const char* const pszName,const char* const pszNewName) = 0;
      virtual bool   RenameColumn(DWORD dwNum,const char* const pszNewName) = 0;
      virtual bool   ChangeColumnType(const char* const pszName,DBX_DATATYPE NewType) = 0;
      virtual bool   ChangeColumnType(DWORD dwNum,DBX_DATATYPE NewType) = 0;
                     
      virtual DBX_COLUMN*        GetColumn(const char* const pszName) = 0;
      virtual DBX_COLUMN*        GetColumn(DWORD dwNum) = 0;
      virtual const char* const  GetColumnName(DWORD dwNum) = 0;
      virtual int                GetColumnNum(const char* const pszName) = 0;
      virtual DWORD              GetColumnOfs (DWORD dwNum) = 0;
      virtual DWORD              GetColumnSize(DWORD dwNum) = 0;

      virtual bool   IsErased(DWORD dwRecID) = 0;
};
*/

/* ******************************************************************** **
** @@ class DBX_TABLE : public Vector
** @  Copyrt  : 
** @  Author  : 
** @  Modify  :
** @  Update  :
** @  Notes   : 
** ******************************************************************** */

class DBX_TABLE : public Vector
{
   private:

      char                 _pszTblName[MAX_PATH + 1];
      char                 _pszFixName[MAX_PATH + 1];
      char                 _pszVarName[MAX_PATH + 1];
      int                  _iIndex;
      MMF*                 _pMemFixFile;  
      DWORD                _dwDiskSizeFix;
      DBX_OPEN_MODE        _FixOpenMode;
      DBX_FLUSH_MODE       _FixFlushMode;
      Timestamp            _TStamp;
      MMF*                 _pMemVarFile;  
      DBX_OPEN_MODE        _VarOpenMode;
      DBX_FLUSH_MODE       _VarFlushMode;
      DBV_HEADER           _VarHeader;
      bool                 _bOpen;

   public:

      DBX_HEADER           _FixHeader;
      BYTE*                _pMemBufFix;
      DWORD                _dwMemSizeFix;
      HANDLE               _hFixFile;
      BYTE*                _pMemBufVar;
      DWORD                _dwMemSizeVar;
      DWORD                _dwDiskSizeVar;
      HANDLE               _hVarFile;

   public:

                DBX_TABLE();
      virtual  ~DBX_TABLE();
                              
      DBX_FLUSH_MODE GetFlushModeFix();
      DBX_FLUSH_MODE GetFlushModeVar();

      void  SetFlushModeFix(DBX_FLUSH_MODE FlushMode);
      void  SetFlushModeVar(DBX_FLUSH_MODE FlushMode);
                          
      void  SetName(const char* const pszName);

      const char* const GetName();
      const char* const GetFixName();
      const char* const GetVarName();

      DBX_OPEN_MODE  GetOpenModeFix();
      DBX_OPEN_MODE  GetOpenModeVar();

      int   GetIndex();
      void  SetIndex(int iIndex);
      
      DWORD InsertEmptyRecord();
      bool  SwapRecords(DWORD dwSrcID,DWORD dwDstID);

      DWORD GetRecCnt();

      DWORD GetCurRec();
      void  SetCurRec(DWORD dwRecCur);
      
      DWORD GetLastID();
      int   GetFieldCnt();
      bool  IsOpen();
      int   GetLastErr();

      DWORD InsertRecord   (const BYTE* const pRecord);
      bool  DuplicateRecord(DWORD dwRecID);
      bool  UpdateRecord   (DWORD dwRecID,const BYTE* const pRecord);
      bool  DeleteRecord   (DWORD dwRecID);
      bool  DeleteAll();
      void  Unerase();
            
      DWORD Current();
      DWORD First  ();
      DWORD Last   ();
      DWORD Prev   ();
      DWORD Next   ();

      const BYTE* const Go(DWORD dwRecID);
            
      bool  Open
      (
         const char* const    pszTableName,
         const char* const    pszFixName,
         const char* const    pszVarName,
         DBX_OPEN_MODE        FixOpenMode,
         DBX_OPEN_MODE        VarOpenMode,
         DBX_FLUSH_MODE       FixFlushMode = DBX_FM_FLUSH_ON_CLOSE,
         DBX_FLUSH_MODE       VarFlushMode = DBX_FM_FLUSH_ON_CLOSE
      );
      
      bool  Close();

      BYTE* GetMemBufFix();
      BYTE* GetMemBufVar();

      DWORD GetMemSizeFix();
      DWORD GetMemSizeVar();

      int   FindFixColumnByOffset(DWORD dwOfs);
//      DWORD GetFutureRecID();
      DWORD GetRecID(const BYTE* const pRec);
      DWORD GetCurRecID();

      int   InsertEmptyColumn();
      int   InsertColumn(const DBX_COLUMN_DESCRIPTOR* const pDescr);
      int   InsertColumn(const DBX_COLUMN_DESCRIPTOR& rDescr);
      bool  InsertColumn(const DBX_COLUMN* const pColumn);
      bool  DeleteColumn(const char* const pszName);
      bool  DeleteColumn(DWORD dwNum);
      bool  RenameColumn(const char* const pszName,const char* const pszNewName);
      bool  RenameColumn(DWORD dwNum,const char* const pszNewName);
      bool  ChangeColumnType(const char* const pszName,DBX_FIELD_TYPE NewType);
      bool  ChangeColumnType(DWORD dwNum,DBX_FIELD_TYPE NewType);
       
      DBX_COLUMN*       GetColumn(const char* const pszName);
      DBX_COLUMN*       GetColumn(DWORD dwNum);
      const char* const GetColumnName(DWORD dwNum);
      int               GetColumnNum(const char* const pszName);
      DWORD             GetColumnOfs (DWORD dwNum);
      DWORD             GetColumnSize(DWORD dwNum);

      BYTE* GetMemo(DWORD dwPos,DWORD& rSize);    

      bool   IsErased(DWORD dwRecID);

   private:
            
      virtual void   Flush();

      bool  UpdateFixHeader_IP();
      bool  UpdateVarHeader_IP();

      bool  UpdateFixHeader_RW();
      bool  UpdateVarHeader_RW();

      bool   _Open_Fix_RO
      (
         const char* const    pszFixName,
         const char* const    pszVarName,
         DBX_OPEN_MODE        VarOpenMode,
         DBX_FLUSH_MODE       FixFlushMode,
         DBX_FLUSH_MODE       VarFlushMode
      );

      bool   _Open_Fix_IP
      (
         const char* const    pszFixName,
         const char* const    pszVarName,
         DBX_OPEN_MODE        VarOpenMode,
         DBX_FLUSH_MODE       FixFlushMode,
         DBX_FLUSH_MODE       VarFlushMode
      );

      bool   _Open_Fix_RW
      (
         const char* const    pszFixName,
         const char* const    pszVarName,
         DBX_OPEN_MODE        VarOpenMode,
         DBX_FLUSH_MODE       FixFlushMode,
         DBX_FLUSH_MODE       VarFlushMode
      );

      bool  _Open_Var_RO(const char* const pszVarName,DBX_FLUSH_MODE VarFlushMode);
      bool  _Open_Var_IP(const char* const pszVarName,DBX_FLUSH_MODE VarFlushMode);
      bool  _Open_Var_RW(const char* const pszVarName,DBX_FLUSH_MODE VarFlushMode);
};

/* ******************************************************************** **
** @@ class DBX : public Vector
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

struct DBX_TABLE_INFO;

class DBX : public Vector
{
   private:
                           
      int                  _iErr;

   public:

                DBX();
                DBX(DWORD dwSize,WORD wDelta,bool bDelete = true);
      virtual  ~DBX();
               
      bool  CreateEmptyTable(const DBX_TABLE_INFO* const pInfo);
      bool  CreateEmptyMemo (const char* const pszName,DWORD dwGranulation);
      
      DBX_TABLE*  OpenTable
      (
         const char* const    pszTableName,
         const char* const    pszFixName,
         const char* const    pszVarName,
         DBX_OPEN_MODE        FixOpenMode = DBX_OM_READ_ONLY,
         DBX_OPEN_MODE        VarOpenMode = DBX_OM_READ_ONLY,
         DBX_FLUSH_MODE       FixFlushMode = DBX_FM_FLUSH_ON_CLOSE,
         DBX_FLUSH_MODE       VarFlushMode = DBX_FM_FLUSH_ON_CLOSE
      );

      void        CloseAll();

      bool        CloseTable(const char* const pszName);
      bool        CloseTable(DBX_TABLE*& rpTable);
      bool        RenameTable(const char* const pszOldName,const char* const pszNewName);
      bool        ReIndexTable(const char* const pszName);
      bool        ReduceTable(const char* const pszName);
      bool        CompactTable(const char* const pszName);
      DBX_TABLE*  GetTable(const char* const pszName);
      DBX_TABLE*  GetTable(DWORD dwTblID);

      int   GetLastErr();

   private:
            
      DBX_TABLE*  GetTableIfOpen(const char* const pszName);
};

/* ******************************************************************** **
** @@ DBX_DBF
** @ Copyrt  : 
** @ Author  : 
** @ Modify  :
** @ Update  :
** @ Notes   : 
** ******************************************************************** */
/*
class DBX_DBF : public DBX
{
   public:

                DBX_DBF();
                DBX_DBF(DWORD dwSize,WORD wDelta,bool bDelete = true);
      virtual  ~DBX_DBF();

      virtual bool         CreateEmptyTable(const DBX_TABLE_INFO* const pInfo);
      virtual DBX_TABLE*   OpenTable(const char* const pszName,DBX_OPEN_MODE Mode = DBX_OM_READ_ONLY);
      virtual bool         CloseTable(const char* const pszName);
      virtual bool         CloseTable(DBX_TABLE* pTable);
      virtual bool         DeleteTable(const char* const pszName,bool bErase = false);
      virtual bool         DeleteTable(DBX_TABLE* pTable,bool bErase = false);
      virtual bool         RenameTable(const char* const pszOldName,const char* const pszNewName);
      virtual bool         ReIndexTable(const char* const pszName);
      virtual bool         ReduceTable(const char* const pszName);
      virtual bool         CompactTable(const char* const pszName);
      virtual DBX_TABLE*   GetTable(const char* const pszName);
      virtual DBX_TABLE*   GetTable(DWORD dwTblID);
};
*/
//////////////////////////////////////////////////////////////////////////

void  DefineField(DBX_COLUMN_DESCRIPTOR* pFieldArr,int iFieldNum,const char* const pszTitle,DBX_FIELD_TYPE FieldType,DBX_FIELD_FLAGS FieldFlags,DWORD dwSize);

DBX_FIELD_TYPE GetFieldType(char cType,BYTE byFlag);

#endif

/* ******************************************************************** **
**                End of File
** ******************************************************************** */
