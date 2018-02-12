/* ******************************************************************** **
** @@ Wallker
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Dscr   : Directory Walker
** ******************************************************************** */

/* ******************************************************************** **
**                uses pre-compiled headers
** ******************************************************************** */

#include "stdafx.h"

#include "file_walker.h"

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
** @@ Match()
** @  Copyrt : Jack Handy | 15 Feb 2005 
** @  Author : Jack Handy  * jakkhandy@hotmail.com
** @  Modify :
** @  Update :
** @  Notes  : http://www.codeproject.com/KB/string/wildcmp.aspx
** ******************************************************************** */

static bool Match(const char* pszMask,const char* pszFilename) 
{
   while ((*pszFilename) && (*pszMask != '*')) 
   {
      if ((toupper(*pszMask) != toupper(*pszFilename)) && (*pszMask != '?')) 
      {
         return false;
      }

      ++pszMask;
      ++pszFilename;
   }

   const char*    pFile = NULL; 
   const char*    pMask = NULL;

   while (*pszFilename) 
   {
      if (*pszMask == '*') 
      {
         if (!*++pszMask) 
         {
            return true;
         }

         pMask = pszMask;
         pFile = pszFilename + 1;
      } 
      else if ((toupper(*pszMask) == toupper(*pszFilename)) || (*pszMask == '?')) 
      {
         ++pszMask;
         ++pszFilename;
      } 
      else 
      {
         pszMask     = pMask;
         pszFilename = pFile++;
      }
   }

   while (*pszMask == '*') 
   {
      ++pszMask;
   }

   return *pszMask == 0;
}

/* ******************************************************************** **
** @@ Walker::Walker()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Constructor
** ******************************************************************** */

Walker::Walker()
{
   Reset();
}

/* ******************************************************************** **
** @@ Walker::~Walker()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Destructor
** ******************************************************************** */

Walker::~Walker()
{
   Reset();
}

/* ******************************************************************** **
** @@ Walker::Reset()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void Walker::Reset()
{
   _pHandler = NULL;
   
   memset(_pszMask,0,MAX_PATH + 1);

   _bRecursive = false;
}

/* ******************************************************************** **
** @@ Walker::Init()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void Walker::Init(USER_FUNC pHandler,const char* const pszMask,bool bRecursive)
{ 
   _pHandler = pHandler;

   _bRecursive = bRecursive;

   if (pszMask && *pszMask)
   {
      strncpy(_pszMask,pszMask,MAX_PATH);
      _pszMask[MAX_PATH] = 0; // Ensure ASCIIZ
   }
   else
   {
      strcpy(_pszMask,"*.*");
   }
}

/* ******************************************************************** **
** @@ Walker::Run()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

bool Walker::Run(const char* const pszRoot)
{
   if (!_pHandler)
   {
      // Error !
      return false;
   }

   if (!pszRoot || !*pszRoot)
   {
      // Error !
      return false;
   }

   char     pszPath[MAX_PATH + 1];

   strncpy(pszPath,pszRoot,MAX_PATH);
   pszPath[MAX_PATH] = 0;  // ASCIIZ
      
   strcat(pszPath,"\\");

   if (_pszMask && *_pszMask)
   {
      strcat(pszPath,_pszMask);
   }
   else
   {
      strcat(pszPath,"*.*");
   }

   WIN32_FIND_DATA      FindData;

   memset(&FindData,0,sizeof(FindData));

   HANDLE      hFile = FindFirstFile(pszPath,&FindData);
      
   if (hFile != INVALID_HANDLE_VALUE)
   {
      do
      {
         if (FindData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) 
         {
            // Stage 1: Search for Files
            #if _DEBUG
            // printf("0x%02X  %s\n",FindData.dwFileAttributes,FindData.cFileName);
            #endif

            if (Match(_pszMask,FindData.cFileName))
            {
               char     pszFilename[MAX_PATH + 1];

               strcpy(pszFilename,pszRoot);

               DWORD    dwLastPos = strlen(pszFilename) - 1;
                                    
               if (pszFilename[dwLastPos] != '\\')
               {
                  strcat(pszFilename,"\\");
               }
      
               strcat(pszFilename,FindData.cFileName);

               _pHandler(pszFilename);
            }
         }
         else  
         {
            if (_bRecursive)
            {
               // Stage 2: Search for Folders
               #if _DEBUG
               // printf("0x%02X  %s\n",FindData.dwFileAttributes,FindData.cFileName);
               #endif
               
               if ((FindData.cFileName[0] == '.') && !FindData.cFileName[1])
               {
                  // Skip !
                  continue;
               }
               
               if ((FindData.cFileName[0] == '.') && (FindData.cFileName[1] == '.') && !FindData.cFileName[2])
               {
                  // Skip !
                  continue;
               }
               
               char     pszNewPath[MAX_PATH + 1];
               
               strncpy(pszNewPath,pszRoot,MAX_PATH);
               pszNewPath[MAX_PATH] = 0;  // ASCIIZ
               
               strcat(pszNewPath,"\\");
               strcat(pszNewPath,FindData.cFileName);
               
               Run(pszNewPath);
            }
         }   
      }
      while (FindNextFile(hFile,&FindData) == TRUE);
   }

   FindClose(hFile);
   hFile = INVALID_HANDLE_VALUE;

   return true;
}

/* ******************************************************************** **
** @@                   End of File
** ******************************************************************** */
