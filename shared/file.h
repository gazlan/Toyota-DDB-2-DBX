/* ******************************************************************** **
** @@ File
** @ Copyrt : 
** @ Author : 
** @ Modify :
** @ Update :
** @ Notes  :
** ******************************************************************** */

#ifndef __FILE_HPP__
#define __FILE_HPP__

/* ******************************************************************** **
** @@ internal defines
** ******************************************************************** */

#if _MSC_VER > 1000
#pragma once
#endif

/* ******************************************************************** **
** @@ internal prototypes
** ******************************************************************** */

/* ******************************************************************** **
** @@ external global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@ static global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@ Classes
** ******************************************************************** */

/* ******************************************************************** **
** @@ prototypes
** ******************************************************************** */

bool     ReadBuffer (HANDLE hFile,void* pBuf,DWORD dwBufSize);
bool     WriteBuffer(HANDLE hFile,const void* const pBuf,DWORD dwBufSize);
HANDLE   CreateFile(const char* const pszFileName,DWORD dwCreationDisposition = CREATE_ALWAYS,DWORD dwShareMode = FILE_SHARE_READ);
HANDLE   OpenFileReadWrite(const char* const pszFileName,DWORD dwShareMode = FILE_SHARE_READ);
HANDLE   OpenFileReadOnly(const char* const pszFileName);
HANDLE   Open_or_CreateFile(const char* const pszFileName);
DWORD    GetFilePointer(HANDLE hFile);
DWORD    GetFileSizeLo(HANDLE hFile);
DWORD    SetFilePointerBOF(HANDLE hFile);
DWORD    SetFilePointerEOF(HANDLE hFile);
// SEEK_SET - The starting point is zero or the beginning of the file. 
// SEEK_CUR - The starting point is the current value of the file pointer. 
// SEEK_END - The starting point is the current end-of-file position. 
DWORD    SetFilePointerCUR(HANDLE hFile,int iOfs,DWORD dwMethod);
bool     GetLatestFileTime(HANDLE hFile,FILETIME& rFileTime);
bool     GetLatestFileTime(const char* const pszFile,FILETIME& rFileTime);
bool     IsValidFileName(const char* const pszFileName);
void     EnsureDirectory(const char* const pszPath);

#endif

/* ******************************************************************** **
** End of File
** ******************************************************************** */
