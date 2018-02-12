/* ******************************************************************** **
** @@ File
** @  Copyrt : 
** @  Author : 
** @  Update :
** @  Update :
** @  Notes  :
** ******************************************************************** */

/* ******************************************************************** **
** uses pre-compiled headers
** ******************************************************************** */

#include "stdafx.h"

#include "file.h"

/* ******************************************************************** **
** @@ internal defines
** ******************************************************************** */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* ******************************************************************** **
** @@ internal prototypes
** ******************************************************************** */

/* ******************************************************************** **
** @@ external global variables
** ******************************************************************** */

extern DWORD   dwKeepError;

/* ******************************************************************** **
** @@ static global variables
** ******************************************************************** */
                  
/* ******************************************************************** **
** @@ real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ GetLatestFileTime()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool GetLatestFileTime(HANDLE hFile,FILETIME& rFileTime)
{
   memset(&rFileTime,0,sizeof(FILETIME));

   if (hFile == INVALID_HANDLE_VALUE)
   {
      // Error !
      dwKeepError = GetLastError();
      return false;
   }

   FILETIME       CreationTime; 
   FILETIME       LastAccessTime; 
   FILETIME       LastWriteTime; 

   memset(&CreationTime,  0,sizeof(CreationTime));
   memset(&LastAccessTime,0,sizeof(LastAccessTime));
   memset(&LastWriteTime, 0,sizeof(LastWriteTime));

   if (!::GetFileTime(hFile,&CreationTime,&LastAccessTime,&LastWriteTime))
   {
      // Error !
      dwKeepError = GetLastError();
      return false;
   }

   if (::CompareFileTime(&CreationTime,&LastWriteTime) >= 0)
   {
      memcpy(&rFileTime,&CreationTime,sizeof(FILETIME));
   }
   else      
   {
      memcpy(&rFileTime,&LastWriteTime,sizeof(FILETIME));
   }

   return true;
}

/* ******************************************************************** **
** @@ GetLatestFileTime()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool GetLatestFileTime(const char* const pszFile,FILETIME& rFileTime)
{
   memset(&rFileTime,0,sizeof(FILETIME));

   if (!pszFile || !*pszFile)
   {
      // Error !
      return false;
   }

   HANDLE      hFile = OpenFileReadOnly(pszFile);

   if (hFile == INVALID_HANDLE_VALUE)
   {
      dwKeepError = GetLastError();
      return false;
   }

   if (!GetLatestFileTime(hFile,rFileTime))
   {
      // Error !
      CloseHandle(hFile);
      hFile = INVALID_HANDLE_VALUE;
      return false;
   }

   CloseHandle(hFile);
   hFile = INVALID_HANDLE_VALUE;

   return true;
}

/* ******************************************************************** **
** @@ CreateFile()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

HANDLE CreateFile
(
   const char* const       pszFileName,
   DWORD                   dwCreationDisposition,
   DWORD                   dwShareMode
)
{
   HANDLE      hFile = CreateFile
   (
      pszFileName,                     // pointer to name of the file
      GENERIC_READ | GENERIC_WRITE,    // access (read-write) mode
      dwShareMode,                     // share mode
      NULL,                            // pointer to security attributes
      dwCreationDisposition,           // how to create
      FILE_ATTRIBUTE_NORMAL,           // file attributes
      NULL                             // handle to file with attributes to copy
   );

   dwKeepError = GetLastError();

   return hFile;
}

/* ******************************************************************** **
** @@ OpenFileReadOnly()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

HANDLE OpenFileReadOnly(const char* const pszFileName)
{
   HANDLE   hFile = CreateFile            // Open File
   (
      pszFileName,                        // pointer to name of the file
      GENERIC_READ,                       // access (read-write) mode
      FILE_SHARE_READ,                    // share mode
      NULL,                               // pointer to security attributes
      OPEN_EXISTING,                      // how to create
      FILE_ATTRIBUTE_NORMAL,              // file attributes
      NULL                                // handle to file with attributes to copy
   );

   dwKeepError = GetLastError();

   return hFile;
}

/* ******************************************************************** **
** @@ OpenFileReadWrite()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

HANDLE OpenFileReadWrite
(
   const char* const       pszFileName,
   DWORD                   dwShareMode
)
{
   HANDLE   hFile = CreateFile               // Open File
   (
      pszFileName,                           // pointer to name of the file
      GENERIC_READ | GENERIC_WRITE,          // access (read-write) mode
      dwShareMode,                           // share mode
      NULL,                                  // pointer to security attributes
      OPEN_EXISTING,                         // how to create
      FILE_ATTRIBUTE_NORMAL,                 // file attributes
      NULL                                   // handle to file with attributes to copy
   );

   dwKeepError = GetLastError();

   return hFile;
}

/* ******************************************************************** **
** @@ Open_or_CreateFile()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

HANDLE Open_or_CreateFile(const char* const pszFileName)
{  
   HANDLE      hFile = CreateFile(pszFileName,OPEN_EXISTING);

   if (hFile != INVALID_HANDLE_VALUE)
   {
      dwKeepError = GetLastError();
   }
   else 
   {
      hFile = CreateFile(pszFileName,CREATE_NEW);

      dwKeepError = GetLastError();
   }

   return hFile;
}

/* ******************************************************************** **
** @@ ReadBuffer()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool ReadBuffer
(
   HANDLE               hFile,
   void*                pBuf,
   DWORD                dwBufSize
)
{
   DWORD    dwRead = 0;

   if (!ReadFile
   (
      hFile,                           // handle to file to read
      pBuf,                            // address of buffer that receives data
      dwBufSize,                       // number of bytes to read
      &dwRead,                         // address of number of bytes read
      NULL                             // address of structure for data
   ))
   {
      // Error !
      dwKeepError = GetLastError();
      return false;
   }

   if (dwRead != dwBufSize)
   {
      // Error !
      dwKeepError = GetLastError();
      return false;
   }

   return true;
}

/* ******************************************************************** **
** @@ WriteBuffer()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool WriteBuffer
(
   HANDLE               hFile,
   const void* const    pBuf,
   DWORD                dwBufSize
)
{
   DWORD    dwWritten = 0;

   if (!WriteFile
   (
      hFile,                           // handle to file to write to
      pBuf,                            // pointer to data to write to file
      dwBufSize,                       // number of bytes to write
      &dwWritten,                      // pointer to number of bytes written
      NULL                             // pointer to structure needed for overlapped I/O
   ))
   {
      // Error !
      dwKeepError = GetLastError();
      return false;
   }

   if (dwWritten != dwBufSize)
   {
      // Error !
      dwKeepError = GetLastError();
      return false;
   }

   return true;
}

/* ******************************************************************** **
** @@ GetFileSizeLo()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DWORD GetFileSizeLo(HANDLE hFile)
{
   if (hFile == INVALID_HANDLE_VALUE)
   {
      dwKeepError = GetLastError();
      return 0;         
   }

   DWORD    dwSizeHi = 0;

   return GetFileSize(hFile,&dwSizeHi);
}

/* ******************************************************************** **
** @@ GetFilePointer()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DWORD GetFilePointer(HANDLE hFile)
{
   DWORD    dwOffset = SetFilePointer
   (
      hFile,                                 // handle of file
      0,                                     // number of bytes to move file pointer
      NULL,                                  // address of high-order word of distance to move
      FILE_CURRENT                           // how to move
   );

   dwKeepError = GetLastError();

   return dwOffset;
}

/* ******************************************************************** **
** @@ SetFilePointerBOF()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DWORD SetFilePointerBOF(HANDLE hFile)
{
   DWORD    dwOffset = SetFilePointer
   (
      hFile,                                 // handle of file
      0,                                     // number of bytes to move file pointer
      NULL,                                  // address of high-order word of distance to move
      SEEK_SET                               // how to move
   );

   dwKeepError = GetLastError();

   return dwOffset;
}

/* ******************************************************************** **
** @@ SetFilePointerEOF()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DWORD SetFilePointerEOF(HANDLE hFile)
{
   DWORD    dwOffset = SetFilePointer
   (
      hFile,                                 // handle of file
      0,                                     // number of bytes to move file pointer
      NULL,                                  // address of high-order word of distance to move
      SEEK_END                               // how to move
   );

   dwKeepError = GetLastError();

   return dwOffset;
}

/* ******************************************************************** **
** @@ SetFilePointerCUR()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DWORD SetFilePointerCUR
(
   HANDLE      hFile,
   int         iOfs,
   DWORD       dwMethod
)
{
   // SEEK_SET - The starting point is zero or the beginning of the file. 
   // SEEK_CUR - The starting point is the current value of the file pointer. 
   // SEEK_END - The starting point is the current end-of-file position. 

   DWORD    dwOffset = SetFilePointer
   (
      hFile,                                 // handle of file
      iOfs,                                  // number of bytes to move file pointer
      NULL,                                  // address of high-order word of distance to move
      dwMethod                               // how to move
   );

   dwKeepError = GetLastError();

   return dwOffset;
}

/* ******************************************************************** **
** @@ EnsureDirectory()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void EnsureDirectory(const char* const pszPath)
{
   if (!pszPath || !*pszPath) 
   {  
      // Error !
      ASSERT(0);
      return;
   }
   
   char*    pBackSlash = (char*)strrchr(pszPath,'\\');

   if (!pBackSlash)
   {  
      if (GetFileAttributes(pszPath) == 0xFFFFFFFF) 
      {
         CreateDirectory(pszPath,NULL);
      }
      
      return;
   }

   char     pszTemp[MAX_PATH + 1];

   memset(pszTemp,0,MAX_PATH);

   strncpy(pszTemp,pszPath,min(MAX_PATH,pBackSlash - pszPath));
   pszTemp[MAX_PATH] = 0;  // Ensure ASCIIZ

   EnsureDirectory(pszTemp);
   
   if (GetFileAttributes(pszPath) == 0xFFFFFFFF) 
   {
      CreateDirectory(pszPath,NULL);
   }
}

/* ******************************************************************** **
** @@ IsValidFileName()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool IsValidFileName(const char* const pszFileName)
{
   if (!pszFileName || !*pszFileName)
   {
      // Error !
      ASSERT(0);
      return false;
   }

   char     pszDrive   [_MAX_DRIVE];
   char     pszDir     [_MAX_DIR];
   char     pszFName   [_MAX_FNAME];
   char     pszExt     [_MAX_EXT];
   
   _splitpath(pszFileName,pszDrive,pszDir,pszFName,pszExt);

   if (strchr(pszDir,':') || strchr(pszFName,':') || strchr(pszExt,':'))
   {
      // Error !
      ASSERT(0);
      return false;
   }

   if (strchr(pszFileName,'*'))
   {
      // Error !
      ASSERT(0);
      return false;
   }

   if (strchr(pszFileName,'?'))
   {
      // Error !
      ASSERT(0);
      return false;
   }

   return true;
}

/* ******************************************************************** **
** End of File
** ******************************************************************** */
