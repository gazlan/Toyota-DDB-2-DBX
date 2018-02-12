/* ******************************************************************** **
** @@ Memory Mapped File
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

#include "mmf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char    THIS_FILE[] = __FILE__;
#endif

/* ******************************************************************** **
** @@                   internal defines
** ******************************************************************** */

/* ******************************************************************** **
** @@                   internal prototypes
** ******************************************************************** */

/* ******************************************************************** **
** @@                   external global variables
** ******************************************************************** */

extern DWORD      dwKeepError;

/* ******************************************************************** **
** @@                   static global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@                   real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ MMF::MMF()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Constructor
** ******************************************************************** */

MMF::MMF()
{
   _hFile = INVALID_HANDLE_VALUE;
   _hMap  = INVALID_HANDLE_VALUE;

   _pView = NULL;

   _dwSizeHi = 0;
   _dwSizeLo = 0;

   _bReadOnly = TRUE;
   _bOpen     = FALSE;
}

/* ******************************************************************** **
** @@ MMF::~MMF()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Destructor
** ******************************************************************** */

MMF::~MMF()
{
   Close();
}

/* ******************************************************************** **
** @@ MMF::OpenReadOnly()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

BOOL MMF::OpenReadOnly(const char* const pszFileName)
{
   if (_bOpen)
   {
      Close();
   }

   if (!pszFileName || !*pszFileName)
   {
      // Error !
      ASSERT(0);
      return FALSE;
   }
   
   // Open File
   if ((_hFile = CreateFile
      (
      pszFileName,                  // Pointer to Name of the File
      GENERIC_READ,                 // Access (Read-Write) Mode
      FILE_SHARE_READ,              // Share Mode
      NULL,                         // Pointer to Security Attributes
      OPEN_EXISTING,                // How to Create
      FILE_ATTRIBUTE_NORMAL,        // File Attributes
      NULL                          // Handle to File with Attributes to Copy
      )) == INVALID_HANDLE_VALUE)
   {
      // Error !
      dwKeepError = GetLastError();
      ASSERT(0);
      return FALSE;
   }

   // Get File Size
   // Really FileSizeHi always ignored !

   _dwSizeHi = 0;
   _dwSizeLo = GetFileSize(_hFile,&_dwSizeHi);

   if (!_dwSizeLo)
   {
      // Error !
      dwKeepError = 0;
      ASSERT(0);
      return FALSE;
   }

   // Map to Memory
   _hMap = CreateFileMapping
   (
      _hFile,           // Handle to File to Map
      NULL,             // Optional Security Attributes
      PAGE_READONLY,    // Protection for Mapping Object
      _dwSizeHi,        // High-order 32 bits of Object Size
      _dwSizeLo,        // Low-order 32 bits of Object Size
      NULL              // Name of File-Mapping Object
   );

   if (!_hMap)
   {
      // Error !
      dwKeepError = GetLastError();
      ASSERT(0);
      return FALSE;
   }

   // Create View
   _pView = MapViewOfFile
   (
      _hMap,            // File-mapping Object to Map into Address Space
      FILE_MAP_READ,    // Access mode
      0,                // High-order 32 bits of File Offset
      0,                // Low-order 32 bits of File Offset
      0                 // Number of Bytes to Map
   );

   if (!_pView)
   {
      // Error !
      dwKeepError = GetLastError();
      ASSERT(0);
      return FALSE;
   }

   _bOpen = TRUE;

   return TRUE;
}

/* ******************************************************************** **
** @@ MMF::OpenReadWrite()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

BOOL MMF::OpenReadWrite(const char* const pszFileName)
{
   if (_bOpen)
   {
      Close();
   }

   if (!pszFileName || !*pszFileName)
   {
      // Error !
      ASSERT(0);
      return FALSE;
   }
   
   // Open File
   if ((_hFile = CreateFile
      (
         pszFileName,                  // Pointer to Name of the File
         GENERIC_READ | GENERIC_WRITE, // Access (Read-Write) Mode
         FILE_SHARE_READ,              // Share Mode
         NULL,                         // Pointer to Security Attributes
         OPEN_ALWAYS,                  // How to Create
         FILE_ATTRIBUTE_NORMAL,        // File Attributes
         NULL                          // Handle to File with Attributes to Copy
      )) == INVALID_HANDLE_VALUE)
   {
      // Error !
      dwKeepError = GetLastError();
      ASSERT(0);
      return FALSE;
   }

   // Get File Size
   // Really FileSizeHi always ignored !
   _dwSizeHi = 0;
   _dwSizeLo = GetFileSize(_hFile,&_dwSizeHi);

   // Map to Memory
   _hMap = CreateFileMapping
   (
      _hFile,              // Handle to File to Map
      NULL,                // Optional Security Attributes
      PAGE_READWRITE,      // Protection for Mapping Object
      _dwSizeHi,           // High-order 32 bits of Object Size
      _dwSizeLo,           // Low-order 32 bits of Object Size
      NULL                 // Name of File-Mapping Object
   );

   if (!_hMap)
   {
      // Error !
      dwKeepError = GetLastError();
      ASSERT(0);
      return FALSE;
   }

   // Create View
   _pView = MapViewOfFile
   (
      _hMap,               // File-mapping Object to Map into Address Space
      FILE_MAP_WRITE,      // Access mode
      0,                   // High-order 32 bits of File Offset
      0,                   // Low-order 32 bits of File Offset
      0                    // Number of Bytes to Map
   );

   if (!_pView)
   {
      // Error !
      dwKeepError = GetLastError();
      ASSERT(0);
      return FALSE;
   }

   _bReadOnly = FALSE;
   _bOpen     = TRUE;

   return TRUE;
}

/* ******************************************************************** **
** @@ MMF::IsOpen()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

BOOL MMF::IsOpen()
{
   return _bOpen;
}

/* ******************************************************************** **
** @@ MMF::Close()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

BOOL MMF::Close()
{
   if (!_bOpen)
   {
      return TRUE;
   }

   if (_pView)
   {
      if (!UnmapViewOfFile(_pView))
      {
         // Error !
         dwKeepError = GetLastError();
         ASSERT(0);
         return FALSE;
      }

      _pView = NULL;
   }

   if (_hMap)
   {
      if (!CloseHandle(_hMap))
      {
         // Error !
         dwKeepError = GetLastError();
         ASSERT(0);
         return FALSE;
      }

      _hMap = NULL;
   }

   if (_hFile != INVALID_HANDLE_VALUE)
   {
      if (!CloseHandle(_hFile))
      {
         // Error !
         dwKeepError = GetLastError();
         ASSERT(0);
         return FALSE;
      }

      _hFile = INVALID_HANDLE_VALUE;
   }

   _bOpen = FALSE;

   return TRUE;
}

/* ******************************************************************** **
** @@ MMF::Flush()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

BOOL MMF::Flush()
{
   if (!_bOpen)
   {
      return TRUE;
   }

   return FlushViewOfFile(_pView,0)  ?  TRUE  :  FALSE;
}

/* ******************************************************************** **
** @@ MMF::Buffer()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

BYTE* MMF::Buffer()
{
   if (!_bOpen)
   {
      // Error !
      ASSERT(0);
      return NULL;
   }

   return (BYTE *) _pView;
}

/* ******************************************************************** **
** @@ MMF::Size()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

DWORD MMF::Size()
{
   if (!_bOpen)
   {
      // Error !
      ASSERT(0);
      return 0;
   }

   return _dwSizeLo;
}

/* ******************************************************************** **
** @@                   End of File
** ******************************************************************** */
