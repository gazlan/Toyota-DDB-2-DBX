/* ******************************************************************** **
** @@ Vector
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

static const int  MIN_INDEX_OF = 64;      // Search Optimization

/* ******************************************************************** **
** @@                   real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ Vector::Vector()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : Constructor
** ******************************************************************** */

Vector::Vector()
{
   _pSorter  = NULL;
   _pItems   = NULL;
   _dwCount  = 0;
   _dwSize   = 0;
   _wDelta   = 1;
   _bGrow    = true;
   _bShrink  = false;
}

/* ******************************************************************** **
** @@ Vector::Vector()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : Constructor
** ******************************************************************** */

Vector::Vector
(
   DWORD       dwSize,
   WORD        wDelta,
   bool        bGrow,
   bool        bShrink
)
{
   _pSorter = NULL;
   _pItems  = NULL;
   _dwCount = 0;
   _dwSize  = 0;
   _wDelta  = (WORD)max(0,wDelta);  // Always Positive !
   _bGrow   = bGrow;
   _bShrink = bShrink;

   if (max(0,dwSize))
   {  
      Resize(dwSize);
   }
}

/* ******************************************************************** **
** @@ Vector::Vector()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : Copy Constructor
** ******************************************************************** */

Vector::Vector(const Vector* const pVector)
{
   _pSorter = NULL;
   _pItems  = NULL;

   _dwCount = 0;
   _dwSize  = 0;

   _wDelta  = pVector->_wDelta;
   _bGrow   = pVector->_bGrow;
   _bShrink = pVector->_bShrink;

   if (max(0,pVector->_dwSize))
   {  
      Resize(pVector->_dwSize);
   }
}

/* ******************************************************************** **
** @@ Vector::~Vector()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : Destructor
** ******************************************************************** */

Vector::~Vector()
{
   delete[] _pItems;
   _pItems = NULL;
}

/* ******************************************************************** **
** @@ Vector::Resize()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

bool Vector::Resize(DWORD dwSize)
{
   void**   pItems = NULL;

   dwSize = max(0,dwSize);

   if (dwSize != _dwSize)
   {
      if (dwSize)
      {
         pItems = (void**)new char[dwSize * sizeof(void*)];

         if (!pItems)
         {
            return false;
         }

         if (_dwSize)
         {
            memcpy(pItems,_pItems,min(dwSize,_dwSize) * sizeof(void*));
         }
      }

      delete[] _pItems;
      _pItems = NULL;

      _pItems = pItems;
      _dwSize = dwSize;
   }

   return true;
}

/* ******************************************************************** **
** @@ Vector::Delta()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void Vector::Delta(WORD wDelta)
{
   _wDelta = wDelta;
}

/* ******************************************************************** **
** @@ Vector::At()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void* Vector::At(DWORD dwIndex)
{
   return (dwIndex >= _dwCount)  ?  NULL  :  _pItems[dwIndex];
}

/* ******************************************************************** **
** @@ Vector::PutAt()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

bool Vector::PutAt
(
   DWORD                dwIndex,
   const void* const    pItem
)
{
   if (dwIndex >= _dwSize)
   {
      return false;
   }

   _pItems[dwIndex] = (void**)pItem;

   return true;
}

/* ******************************************************************** **
** @@ Vector::InsertAt()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

bool Vector::InsertAt
(
   DWORD                dwIndex,
   const void* const    pItem
)
{
   if (dwIndex > _dwSize)
   {
      return false;
   }

   if (_dwCount == _dwSize)
   {
      if (!_bGrow)
      {
         return false;
      }
               
      if (!Resize(_dwSize + _wDelta))
      {
         return false;
      }
   }

   if (_dwCount > dwIndex)
   {
      int   iLen = (_dwCount - dwIndex) * (int)sizeof(void*);

      memmove(&_pItems[dwIndex + 1],&_pItems[dwIndex],(DWORD)iLen);
   }

   ++_dwCount;

   _pItems[dwIndex] = (void**)pItem;

   return true;
}

/* ******************************************************************** **
** @@ Vector::RemoveAt()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

bool Vector::RemoveAt
(
   DWORD    dwIndex
)
{
   if (dwIndex >= _dwCount)
   {
      return false;
   }

   --_dwCount;

   memmove(&_pItems[dwIndex],&_pItems[dwIndex + 1],(_dwCount - dwIndex) * sizeof(void*));

   if (_bShrink && (_dwCount < (_dwSize - _wDelta)))
   {
      if (!Resize(_dwSize - _wDelta))
      {
         return false;
      }
   }

   return true;
}

/* ******************************************************************** **
** @@ Vector::Remove()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

bool Vector::Remove(const void* const pItem)
{
   int   iIndex = Indexof(pItem);

   if (iIndex == -1)
   {
      return false;
   }

   RemoveAt(iIndex);

   return true;
}

/* ******************************************************************** **
** @@ Vector::Indexof()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : Linear Search
** ******************************************************************** */

int Vector::Indexof(const void* const pItem)
{
   for (DWORD ii = 0; ii < _dwCount; ++ii)
   {
      if (pItem == _pItems[ii])
      {
         return ii;
      }
   }

   return -1;
}

/* ******************************************************************** **
** @@ Vector::Insert()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

int Vector::Insert(const void* const pItem)
{
   return InsertAt(_dwCount,pItem)  ?  _dwCount - 1  :  -1;
}

/* ******************************************************************** **
** @@ Vector::Search()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void* Vector::Search
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

   for (DWORD ii = 0; ii < _dwCount; ++ii)
   {
      int   iResult = 0;

      if (pSearch)
      {
         iResult = pSearch((const void** const)&_pItems[ii],(const void** const)&pKey);
      }
      else
      {
         iResult = _pSorter((const void** const)&_pItems[ii],(const void** const)&pKey);
      }

      if (!iResult)     // Found
      {
         if (pIndex)
         {
            *pIndex = ii;
         }

         return _pItems[ii];
      }
   }

   if (pIndex)
   {
      *pIndex = -1;
   }

   return NULL;
}

/* ******************************************************************** **
** @@ Vector::First()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void* Vector::First()
{
   return At(0);
}

/* ******************************************************************** **
** @@ Vector::Last()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void* Vector::Last()
{
   return At(_dwCount - 1);
}

/* ******************************************************************** **
** @@ Vector::RemoveLast()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

bool Vector::RemoveLast()
{
   return RemoveAt(_dwCount - 1);
}

/* ******************************************************************** **
** @@ Vector::Prev()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void* Vector::Prev(const void* const pItem)
{
   int   iIndex = Indexof(pItem);

   return iIndex == -1  ?  NULL  :  At(iIndex - 1);
}

/* ******************************************************************** **
** @@ Vector::Next()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void* Vector::Next(const void* const pItem)
{
   int   iIndex = Indexof(pItem);

   return iIndex == -1  ?  NULL  :  At(iIndex + 1);
}

/* ******************************************************************** **
** @@ Vector::Push()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void Vector::Push(const void* const pItem)
{
   Insert(pItem);
}

/* ******************************************************************** **
** @@ Vector::Pop()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void* Vector::Pop()
{
   void*    pItem = At(_dwCount - 1);

   RemoveAt(_dwCount - 1);

   return pItem;
}

/* ******************************************************************** **
** @@ Vector::Peek()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void* Vector::Peek()
{
   return At(_dwCount - 1);
}

/* ******************************************************************** **
** @@ Vector::Drop()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

bool Vector::Drop(int iItems)
{
   for (DWORD ii = (_dwCount - 1); ((ii > 0) && (iItems > 0)); --iItems, --ii)
   {
      if (!RemoveAt(ii))
      {
         return false;
      }
   }

   return true;
}

/* ******************************************************************** **
** @@ Vector::LSearch()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void* Vector::LSearch
(
   const void* const    pKey,
   int*                 pIndex,
   SEARCHER             pSearch
)
{
   if (pIndex)
   {
      *pIndex = -1;
   }

   if (!_pSorter && !pSearch)
   {
      return NULL;
   }
      
   void*    pItem = NULL;

   if (pSearch)
   {
      pItem = _lfind(pKey,_pItems,(unsigned int*)&_dwCount,sizeof(void*),(int (__cdecl *)(const void*,const void*))pSearch);
   }
   else
   {
      pItem = _lfind(pKey,_pItems,(unsigned int*)&_dwCount,sizeof(void*),(int (__cdecl *)(const void*,const void*))_pSorter);
   }

   if (pItem && pIndex)
   {
      for (DWORD ii = 0; ii < _dwCount; ++ii)
      {
         if (pItem == _pItems[ii])
         {
            *pIndex = ii;
         }
      }
   }

   return pItem;
}

/* ******************************************************************** **
** @@ Vector::BSearch()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void* Vector::BSearch
(
   const void* const    pKey,
   int*                 pIndex,
   SEARCHER             pSearch
)
{
   if (pIndex)
   {
      *pIndex = -1;
   }

   if (!_pSorter && !pSearch)
   {
      return NULL;
   }

   void*    pItem = NULL;

   if (pSearch)
   {
      pItem = bsearch(pKey,_pItems,_dwCount,sizeof(void*),(int (__cdecl *)(const void*,const void*))pSearch);
   }
   else
   {
      pItem = bsearch(pKey,_pItems,_dwCount,sizeof(void*),(int (__cdecl *)(const void*,const void*))_pSorter);
   }

   if (pItem && pIndex)
   {
      for (DWORD ii = 0; ii < _dwCount; ++ii)
      {
         if (pItem == _pItems[ii])
         {
            *pIndex = ii;
         }
      }
   }

   return pItem;
}

/* ******************************************************************** **
** @@ Vector::SetSorter()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void Vector::SetSorter(SORTER pSorter)
{
   _pSorter = pSorter;
}

/* ******************************************************************** **
** @@ Vector::GetSorter()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

SORTER Vector::GetSorter() const
{
   return _pSorter;
}

/* ******************************************************************** **
** @@ Vector::QSort()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void Vector::QSort()
{
   if (!_pSorter)
   {
      return;
   }

   qsort(_pItems,_dwCount,sizeof(void*),(int (__cdecl *)(const void*,const void*))_pSorter);
}

/* ******************************************************************** **
** @@ Vector::Count()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

DWORD Vector::Count() 
{ 
   return _dwCount; 
}

/* ******************************************************************** **
** @@ Vector::Reset()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void Vector::Reset()
{
   _dwCount = 0;  // Reset Counter on Load only !!   
}

/* ******************************************************************** **
** @@ Vector::Size()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

DWORD Vector::Size()
{ 
   return _dwSize; 
}

/* ******************************************************************** **
** @@ Vector::FirstThat()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

#pragma warning(disable: 4100)   // unreferenced formal parameter
void* Vector::FirstThat(TEST pTest,const void* const pArg)
{
   // TODO !
   return NULL;
}

/* ******************************************************************** **
** @@ Vector::LastThat()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void* Vector::LastThat(TEST pTest,const void* const pArg)
{
   // TODO !
   return NULL;
}

/* ******************************************************************** **
** @@ Vector::ForEach()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void Vector::ForEach(APP pAction,const void* const pArg)
{
   // TODO !
}

/* ******************************************************************** **
** @@ Vector::Exchange()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void Vector::Exchange(DWORD dwFirst,DWORD dwSecond)
{
   if ((dwFirst >= _dwCount) || (dwSecond >= _dwCount))
   {
      return;
   }

   void*    p1 = _pItems[dwFirst];
   void*    p2 = _pItems[dwSecond];

   _pItems[dwFirst]  = p2;
   _pItems[dwSecond] = p1;
}

/* ******************************************************************** **
** @@ Vector::Attach()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void Vector::Attach(void** pBuf,DWORD dwCount,DWORD dwSize)
{
   delete[] _pItems;
   _pItems = NULL;

   _pItems  = pBuf;

   _dwCount = dwCount;
   _dwSize  = dwSize;
}

/* ******************************************************************** **
** @@ Vector::Detach()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void** Vector::Detach(DWORD& rCount,DWORD& rSize)
{
   rCount = _dwCount;
   rSize  = _dwSize;

   void**   pItems = _pItems;

   _pItems = NULL;

   _dwCount = 0; 
   _dwSize  = 0;
    
   return pItems;
}

/* ******************************************************************** **
** @@ Vector::GetData()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void** Vector::GetData()
{
   return _pItems;
}

/* ******************************************************************** **
** @@ Vector::qEnqueue()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

int Vector::qEnqueue(const void* const pItem)
{
   return InsertAt(_dwCount,pItem)  ?  (int)_dwCount  :  -1;
}

/* ******************************************************************** **
** @@ Vector::qDequeue()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update : 
** @  Notes  :
** ******************************************************************** */

void* Vector::qDequeue()
{
   void*    pItem = At(0);

   RemoveAt(0);

   return pItem;
}

/* ******************************************************************** **
**                End of File
** ******************************************************************** */
