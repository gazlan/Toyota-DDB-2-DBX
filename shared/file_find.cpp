/* ******************************************************************** **
** @@ FileFind 
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

#include "file_find.h"

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
** @@ FindFile::FileFind()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Constructor
** ******************************************************************** */

FindFile::FindFile()
{
   Init();
}

/* ******************************************************************** **
** @@ FindFile::~FileFind()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 22 Jun 1999
** @  Notes  : Destructor
** ******************************************************************** */

FindFile::~FindFile()
{
   FindClose(_hFile);
}

/* ******************************************************************** **
** @@ FindFile::Init()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void FindFile::Init()
{
   memset(&_w32fd,0,sizeof(WIN32_FIND_DATA));
   memset(_pszFileMask,0,sizeof(_pszFileMask));
   memset(_pszPrefix,0,sizeof(_pszPrefix));

   _hFile = INVALID_HANDLE_VALUE;

   _bFirstCall = true;
}

/* ******************************************************************** **
** @@ FindFile::SetPrefix()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void FindFile::SetPrefix(const char* const pszPrefix)
{
   strncpy(_pszPrefix,pszPrefix,MAX_PATH);
   _pszPrefix[MAX_PATH] = 0;  // ASCIIZ
}

/* ******************************************************************** **
** @@ FindFile::SetMask()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void FindFile::SetMask(const char* const pszFileMask)
{
   strncpy(_pszFileMask,pszFileMask,MAX_PATH);
   _pszFileMask[MAX_PATH] = 0;  // ASCIIZ
}

/* ******************************************************************** **
** @@ FindFile::FindFirst()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool FindFile::FindFirst()
{
   char  pszSearchMask[MAX_PATH + 1];

   memset(pszSearchMask,0,sizeof(pszSearchMask));

   int   iLen  = strlen(_pszPrefix);

   if (iLen > 255)
   {
      // Err: Path too long!
      return false;
   }

   if (iLen && (_pszPrefix[iLen - 1] == '\\'))
   {
      _pszPrefix[iLen - 1] = 0;
   }

   if (iLen)
   {
      strcat(pszSearchMask,_pszPrefix);
      strcat(pszSearchMask,"\\");
   }

   strcat(pszSearchMask,_pszFileMask);

   _hFile = FindFirstFile
   (
      pszSearchMask, // pointer to name of file to search for
      &_w32fd        // pointer to returned information
   );

   if (_hFile == INVALID_HANDLE_VALUE)
   {
      return false;
   }

   return true;
}

/* ******************************************************************** **
** @@ FindFile::FindNext()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool FindFile::FindNext()
{
   return FindNextFile(_hFile,&_w32fd) != 0;
}

/* ******************************************************************** **
** @@ FindFile::Fetch()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool FindFile::Fetch()
{
   if (_bFirstCall)
   {
      _bFirstCall = false;
      return FindFirst();
   }
   else
   {
      return FindNext();
   }
}

/* ******************************************************************** **
** @@ FindFile::Walk()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Recursive Search
** ******************************************************************** */

void FindFile::Walk()
{
   if (_bFirstCall)
   {
      _bFirstCall = false;

      if (!FindFirst())
      {
         return;
      }
   }

   if (!((_w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (!strcmp(_w32fd.cFileName,".") || (!strcmp(_w32fd.cFileName,"..")))))
   {
      AppendFile();

      if (_w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
         Loop();
      }
   }

   while (FindNextFile(_hFile,&_w32fd))
   {
      if ((_w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (!strcmp(_w32fd.cFileName,".") || (!strcmp(_w32fd.cFileName,".."))))
      {
         continue;
      }

      AppendFile();

      if (_w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
         Loop();
      }
   }
}

/* ******************************************************************** **
** @@ FindFile::Loop()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Process SubDirectory
** ******************************************************************** */

void FindFile::Loop()
{
   WIN32_FIND_DATA   w32fd;
   HANDLE            hKeepFile;

   char              pszKeepPrefix[MAX_PATH];

   // Keep All
   strcpy(pszKeepPrefix,_pszPrefix);

   strcat(_pszPrefix,_w32fd.cFileName);
   strcat(_pszPrefix,"\\");

   memcpy(&w32fd,&_w32fd,sizeof(WIN32_FIND_DATA));

   hKeepFile = _hFile;

   _bFirstCall = true;

   Walk();

   // Restore All
   memcpy(&_w32fd,&w32fd,sizeof(WIN32_FIND_DATA));

   strcpy(_pszPrefix,pszKeepPrefix);

   _hFile = hKeepFile;

   _bFirstCall = false;
}

/* ******************************************************************** **
** @@ FindFile::AppendFile()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void FindFile::AppendFile()
{
   char  pAttr[15];

   memset(pAttr,'.',sizeof(pAttr));
   pAttr[14] = 0;

   if (_w32fd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
   {
      pAttr[0] = 'A';
   }

   if (_w32fd.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)
   {
      pAttr[1] = 'C';
   }

   if (_w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
   {
      pAttr[2] = 'D';
   }

   //   if (_w32fd.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
   //      pAttr[3] = 'E';

   if (_w32fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
   {
      pAttr[4] = 'H';
   }

   //   if (_w32fd.dwFileAttributes & FILE_ATTRIBUTE_INROM)
   //      pAttr[5] = 'I';

   if (_w32fd.dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
   {
      pAttr[6] = 'N';
   }

   if (_w32fd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
   {
      pAttr[7] = 'R';
   }

   //   if (_w32fd.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
   //      pAttr[8] = 'P';

   //   if (_w32fd.dwFileAttributes & FILE_ATTRIBUTE_ROMMODULE)
   //      pAttr[9] = 'M';

   //   if (_w32fd.dwFileAttributes & FILE_ATTRIBUTE_ROMSTATICREF)
   //      pAttr[10] = 'R';

   //   if (_w32fd.dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)
   //      pAttr[11] = 'F';

   if (_w32fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
   {
      pAttr[12] = 'S';
   }

   if (_w32fd.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)
   {
      pAttr[13] = 'T';
   }

   FILETIME    LocalTime;
   SYSTEMTIME  TimeStamp;

   FileTimeToLocalFileTime(&_w32fd.ftLastWriteTime,&LocalTime);
   FileTimeToSystemTime(&LocalTime,&TimeStamp);

   if (*_pszPrefix)
   {
      printf("%12lu  %04d-%02d-%02d  %02d:%02d:%02d  %s  %s%s\n",_w32fd.nFileSizeLow,TimeStamp.wYear,TimeStamp.wMonth,TimeStamp.wDay,TimeStamp.wHour,TimeStamp.wMinute,TimeStamp.wSecond,pAttr,_pszPrefix,_w32fd.cFileName);
   }
   else
   {
      printf("%12lu  %04d-%02d-%02d  %02d:%02d:%02d  %s  %s\n",_w32fd.nFileSizeLow,TimeStamp.wYear,TimeStamp.wMonth,TimeStamp.wDay,TimeStamp.wHour,TimeStamp.wMinute,TimeStamp.wSecond,pAttr,_w32fd.cFileName);
   }
}

/* ******************************************************************** **
** @@                   End of File
** ******************************************************************** */
