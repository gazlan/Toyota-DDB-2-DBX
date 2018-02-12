/* ******************************************************************** **
** @@ BMH Fast Search
** @  Copyrt : 2003-2004 Herbert Bos & Willem de Bruijn * wdebruij@users.sourceforge.net
** @  Author : Fairly Fast Packet Filter * http://ffpf.sourceforge.net/
** @  Modify : 
** @  Update : 
** @  Notes  : The the Boyer-Moore-Horspool algorithm
** ******************************************************************** */

/*
   Original code courtesy of

   "EXACT STRING MATCHING ALGORITHMS
   Animation in Java"
   http://www-igm.univ-mlv.fr/~lecroq/string/

   Christian Charras - Thierry Lecroq
   Laboratoire d'Informatique de Rouen
   Universit�de Rouen
   Facult�des Sciences et des Techniques
   76821 Mont-Saint-Aignan Cedex
   FRANCE
*/

/* ******************************************************************** **
**                uses pre-compiled headers
** ******************************************************************** */

#include "stdafx.h"

#include "search_bmh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
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

/* ******************************************************************** **
** @@                   static global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@                   real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ BMH_Search()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Search pat[1..m] in text[1..n]
** ******************************************************************** */

int BMH_Search
( 
   const BYTE* const    pBuf, 
   int                  iBufSize,
   const BYTE* const    pPattern, 
   int                  iPatSize
)
{
   int   pBMH_Badchars[256];

   // Preprocessing
   for (int ii = 0; ii < 256; ++ii)
   {
      pBMH_Badchars[ii] = iPatSize;
   }

   for (ii = 0; ii < (iPatSize - 1); ++ii)
   {
      pBMH_Badchars[(BYTE)pPattern[ii]] = iPatSize - ii - 1;
   }

   // Searching
   for (int jj = 0; jj <= iBufSize - iPatSize; ) 
   {
      BYTE     byChr = pBuf[jj + iPatSize - 1];

      if ((pPattern[iPatSize - 1] == byChr) && !memcmp(pPattern,pBuf + jj,iPatSize - 1))
      {
         return jj;
      }

      jj += pBMH_Badchars[byChr];
   }

   return -1;
}

/* ******************************************************************** **
** @@ BMH_ISearch()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Search pat[1..m] in text[1..n]
** ******************************************************************** */

int BMH_ISearch
( 
   const BYTE* const    pBuf, 
   int                  iBufSize,
   const BYTE* const    pPattern, 
   int                  iPatSize
)
{
   int   pBMH_Badchars[256];

   // Preprocessing
   for (int ii = 0; ii < 256; ++ii)
   {
      pBMH_Badchars[ii] = iPatSize;
   }

   for (ii = 0; ii < iPatSize - 1; ++ii)
   {
      pBMH_Badchars[(BYTE)tolower(pPattern[ii])] = iPatSize - ii - 1;
      pBMH_Badchars[(BYTE)toupper(pPattern[ii])] = iPatSize - ii - 1;
   }

   // Searching
   for (int jj = 0; jj <= (iBufSize - iPatSize); ) 
   {
      BYTE     byChr = pBuf[jj + iPatSize - 1];

      if ((pPattern[iPatSize - 1] == byChr) && !_memicmp(pPattern,pBuf + jj,iPatSize - 1))
      {
         return jj;
      }

      jj += pBMH_Badchars[byChr];
   }

   return -1;
}

/* ******************************************************************** **
** @@                   End of File
** ******************************************************************** */
