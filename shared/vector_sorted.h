/* ******************************************************************** **
** @@ Sorted Vector
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Dscr   :
** ******************************************************************** */

#ifndef _SORTED_VECTOR_HPP_
#define _SORTED_VECTOR_HPP_

#if _MSC_VER > 1000
#pragma once
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
** @@                   Classes
** ******************************************************************** */

/* ******************************************************************** **
** @@ class SortedVector : public Vector
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

class SortedVector : public Vector
{
   protected:

      bool   _bUnique;

   public:

                SortedVector();
                SortedVector(DWORD dwSize,WORD wDelta,bool bGrow = true,bool bShrink = false,bool bUnique = true);
                SortedVector(const SortedVector* const pSortedVector);
      virtual  ~SortedVector();

      bool IsUnique() { return  _bUnique; }
                                 
      virtual int    __fastcall  Indexof(const void* const pItem);
      virtual int    __fastcall  Insert (const void* const pItem);
      virtual void*  __fastcall  Search (const void* const pKey,int* pIndex = NULL,SEARCHER pSearch = NULL);
};

/* ******************************************************************** **
** @@                   Prototypes
** ******************************************************************** */

#endif

/* ******************************************************************** **
**                End of File
** ******************************************************************** */
