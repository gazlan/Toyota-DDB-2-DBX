/* ******************************************************************** **
** @@ Vector Header File
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Dscr   :
** ******************************************************************** */

#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#if _MSC_VER > 1000
#pragma once
#endif

/* ******************************************************************** **
** @@                   internal defines
** ******************************************************************** */

typedef  int (__cdecl *SORTER)  (const void** const pKey1,const void** const pKey2);
typedef  int (__cdecl *SEARCHER)(const void** const pKey1,const void** const pKey2);

typedef bool (*TEST)(const void* const pKey1,const void* const pKey2);
typedef void (*APP) (const void* const pKey1,const void* const pKey2);

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
** @@ class Vector
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

class Vector
{
   protected:

      void**         _pItems;
      DWORD          _dwCount;
      DWORD          _dwSize;
      SORTER         _pSorter;
      WORD           _wDelta;
      bool           _bGrow;
      bool           _bShrink;

   public:

               Vector();
               Vector(DWORD dwSize,WORD wDelta,bool bGrow = true,bool bShrink = false);
               Vector(const Vector* const pVector);
      virtual ~Vector();

      void     Attach(void** pBuf,DWORD dwCount,DWORD dwSize);
      void**   Detach(DWORD& rCount,DWORD& rSize);
      void**   GetData();

      DWORD    Count();
      void     Reset();

      DWORD    Size ();
      bool     Resize(DWORD dwSize);
      void     Delta (WORD wDelta);
      void     SetGrowMode  (bool bGrow);
      void     SetShrinkMode(bool bShrink);
                        
      void* __fastcall  At       (DWORD dwIndex);
      bool  __fastcall  PutAt    (DWORD dwIndex,const void* const pItem);
      bool  __fastcall  InsertAt (DWORD dwIndex,const void* const pItem);
      bool  __fastcall  RemoveAt (DWORD dwIndex);
      void  __fastcall  Exchange (DWORD dwFirst,DWORD dwSecond);

      void* __fastcall  FirstThat(TEST pTest,  const void* const pArg);
      void* __fastcall  LastThat (TEST pTest,  const void* const pArg);
      void  __fastcall  ForEach  (APP  pAction,const void* const pArg);

      void* __fastcall  LSearch(const void* const pKey,int* pIndex = NULL,SEARCHER pSearch = NULL);
      void* __fastcall  BSearch(const void* const pKey,int* pIndex = NULL,SEARCHER pSearch = NULL);

      void     SetSorter(SORTER pSorter);
      SORTER   GetSorter() const;

      void     QSort();
                        
      bool  __fastcall  Remove(const void* const pItem);
      bool              RemoveLast();

      virtual int __fastcall  Indexof(const void* const pItem);
      virtual int __fastcall  Insert (const void* const pItem);

      virtual void* __fastcall   Search(const void* const pKey,int* pIndex = NULL,SEARCHER pSearch = NULL);
               
      //  Linked list functions
      void*    First();
      void*    Last ();

      void* __fastcall  Prev (const void* const pItem);
      void* __fastcall  Next (const void* const pItem);
            
      //  Stack functions
      void  __fastcall  Push (const void* const pItem);
      bool  __fastcall  Drop(int iItems);

      void*    Pop ();
      void*    Peek();
            
      // Queue functions
      int   __fastcall qEnqueue(const void* const pItem);
      void*            qDequeue();
};

/* ******************************************************************** **
** @@                   Prototypes
** ******************************************************************** */

#endif

/* ******************************************************************** **
**                End of File
** ******************************************************************** */
