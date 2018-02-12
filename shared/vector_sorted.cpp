/* ******************************************************************** **
** @@ Sorted Vector
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

/* ******************************************************************** **
**                uses pre-compiled headers
** ******************************************************************** */

#include "stdafx.h"

#include "search.h"
#include "vector.h"
#include "vector_sorted.h"

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

static const int  MIN_INDEX_OF = 64;      // Search Optimization

/* ******************************************************************** **
** @@                   real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ SortedVector()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : Constructor
** ******************************************************************** */

SortedVector::SortedVector()
:  Vector()
{
   _bUnique = true;
}

/* ******************************************************************** **
** @@ SortedVector::SortedVector()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : Constructor
** ******************************************************************** */

SortedVector::SortedVector
(
   DWORD    dwSize,
   WORD     wDelta,
   bool     bGrow,
   bool     bShrink,
   bool     bUnique
)
:  Vector(dwSize,wDelta,bGrow,bShrink)
{
   _bUnique = bUnique;
}

/* ******************************************************************** **
** @@ SortedVector::SortedVector()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : Copy Constructor
** ******************************************************************** */

SortedVector::SortedVector(const SortedVector* const pSortedVector)
:  Vector((const Vector* const)pSortedVector)
{
   _bUnique = true;
}

/* ******************************************************************** **
** @@ SortedVector::~SortedVector()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : Destructor
** ******************************************************************** */

SortedVector::~SortedVector()
{
}

/* ******************************************************************** **
** @@ SortedVector::Indexof()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

int SortedVector::Indexof(const void* const pItem)
{
   if (!_pSorter)
   {
      return -1;
   }

   if (_dwCount < MIN_INDEX_OF)
   {
      // Linear Search
      return Vector::Indexof(pItem);      
   }

   // Binary Search
   int   iIdx = 0;

   void*    pFound = Search(pItem,&iIdx);

   if (pFound == pItem)
   {
      return iIdx;
   }

   if (!pFound)
   {
      return -1;
   }

   if (!_bUnique)
   {
      int   Lo = iIdx;
      int   Hi = (int)(_dwCount - 1);

      while (Lo < (Hi - MIN_INDEX_OF))
      {
         int   Mid = (Lo + Hi) >> 1;

         if (_pSorter((const void** const)&_pItems[Mid],(const void** const)&pItem) <= 0)
         {
            Lo = Mid + 1;
         }
         else
         {
            Hi = Mid - 1;
         }
      }

      for (int ii = iIdx + 1; ii <= Hi; ++ii)
      {
         if (pItem == _pItems[ii])
         {
            return ii;
         }
      }
   }

   return -1;
}

/* ******************************************************************** **
** @@ SortedVector::Insert()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

int SortedVector::Insert(const void* const pItem)
{
   if (!_pSorter)
   {
      return -1;
   }

   int   iIdx = 0;

   void*    pFound = Search(pItem,&iIdx);

   if (pFound && _bUnique)
   {
      return -1;
   }

   return InsertAt(iIdx,pItem)  ?  iIdx  :  -1;
}

/* ******************************************************************** **
** @@ SortedVector::Search()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void* SortedVector::Search
(
   const void* const    pKey,
   int*                 pIndex,
   SEARCHER             pSearch
)
{
   if (!_pSorter)
   {
      return NULL;
   }

   if (!_dwCount)
   {
      return NULL;
   }

   int   Lo = 0;
   int   Hi = (int)(_dwCount - 1);

   bool  bFound = false;

   while (Lo <= Hi)
   {
      int   Mid = (Lo + Hi) >> 1;

      int   iResult = 0;
      
      if (pSearch)
      {
         iResult = pSearch((const void** const)&_pItems[Mid],(const void** const)&pKey);
      }
      else
      {
         iResult = _pSorter((const void** const)&_pItems[Mid],(const void** const)&pKey);
      }

      if (iResult < 0)
      {
         Lo = Mid + 1;
      }
      else
      {
         Hi = Mid - 1;

         if (!iResult)     // Found
         {
            bFound = true;

            if (_bUnique)
            {
               Lo = Mid;
            }
         }
      }
   }

   if (pIndex)
   {
      *pIndex = Lo;
   }

   return bFound  ?  _pItems[Lo]  :  NULL;
}

/* ******************************************************************** **
**                End of File
** ******************************************************************** */
