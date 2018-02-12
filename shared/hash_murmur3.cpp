#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

// Note - The x86 and x64 versions do _not_ produce the same results, as the
// algorithms are optimized for their respective platforms. You can still
// compile and run any of them on any platform, but your performance with the
// non-native version will be less than optimal.
//-----------------------------------------------------------------------------

#include "hash_murmur3.h"

#ifndef QWORD
typedef unsigned __int64   QWORD;
#endif

//////////////////////////////////////////////////////////////////////////
// Platform-specific functions and macros
#if defined(_MSC_VER)
   // Microsoft Visual Studio
   #define FORCE_INLINE       __forceinline
   #include <stdlib.h>
   #define ROTL32(x,y)        _rotl(x,y)
   #define BIG_CONSTANT(x)    (x)
#else
   // Other compilers
   #define FORCE_INLINE       __attribute__((always_inline))

   inline DWORD rotl32(DWORD x,char r)
   {
      return (x << r) | (x >> (32 - r));
   }

   inline QWORD rotl64(QWORD x,char r)
   {
      return (x << r) | (x >> (64 - r));
   }

   #define ROTL32(x,y)     rotl32(x,y)
   #define ROTL64(x,y)     rotl64(x,y)

   #define BIG_CONSTANT(x) (x##LLU)
#endif
//////////////////////////////////////////////////////////////////////////

/* ******************************************************************** **
** @@ inline QWORD rotl64()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

inline QWORD rotl64(QWORD x,char r)
{
   return (x << r) | (x >> (64 - r));
}

#define ROTL64(x,y)     rotl64(x,y)

//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here

/* ******************************************************************** **
** @@ FORCE_INLINE DWORD getblock()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

FORCE_INLINE DWORD getblock(const DWORD* p,int i)
{
   return p[i];
}

/* ******************************************************************** **
** @@ FORCE_INLINE QWORD getblock()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

FORCE_INLINE QWORD getblock(const QWORD* p,int i)
{
   return p[i];
}

//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche

/* ******************************************************************** **
** @@ FORCE_INLINE DWORD fmix()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

FORCE_INLINE DWORD fmix(DWORD h)
{
   h ^= h >> 16;
   h *= 0x85EBCA6B;
   h ^= h >> 13;
   h *= 0xC2B2AE35;
   h ^= h >> 16;

  return h;
}

/* ******************************************************************** **
** @@ FORCE_INLINE QWORD fmix()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

FORCE_INLINE QWORD fmix(QWORD k)
{
   k ^= k >> 33;
   k *= BIG_CONSTANT(0xFF51AFD7ED558CCD);
   k ^= k >> 33;
   k *= BIG_CONSTANT(0xC4CEB9FE1A85EC53);
   k ^= k >> 33;

  return k;
}

/* ******************************************************************** **
** @@ DWORD __fastcall MurmurHash3_x86_32()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DWORD __fastcall MurmurHash3_x86_32(const void* const pBuf,DWORD dwSize,DWORD seed)
{
   if (!dwSize || !pBuf)
   {
      // Error !
      ASSERT(0);
      return 0;
   }

   if (IsBadReadPtr(pBuf,dwSize))
   {
      // Error !
      ASSERT(0);
      return 0;
   }

   const BYTE*    data = (const BYTE*)pBuf;

   const int   nblocks = dwSize / 4;

   DWORD    h1 = seed;

   DWORD    c1 = 0xCC9E2D51;
   DWORD    c2 = 0x1B873593;

   // body
   const DWORD*   blocks = (const DWORD*)(data + nblocks * 4);

   for (int ii = -nblocks; ii; ++ii)
   {
     DWORD  k1 = getblock(blocks,ii);

     k1 *= c1;
     k1  = ROTL32(k1,15);
     k1 *= c2;

     h1 ^= k1;
     h1  = ROTL32(h1,13);
     h1  = h1 * 5 + 0xE6546B64;
   }

   // tail
   const BYTE*  tail = (const BYTE*)(data + nblocks * 4);

   DWORD    k1 = 0;

   switch (dwSize & 3)
   {  
      case 3: 
      {
         k1 ^= tail[2] << 16;
      }
      case 2: 
      {
         k1 ^= tail[1] << 8;
      }
      case 1: 
      {
         k1 ^= tail[0];
         k1 *= c1; 
         k1  = ROTL32(k1,15); 
         k1 *= c2; 

         h1 ^= k1;
      }
   }
   
   // finalization
   h1 ^= dwSize;
   h1  = fmix(h1);
   
   return h1;
}

/* ******************************************************************** **
** @@ void MurmurHash3_x86_128()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void MurmurHash3_x86_128(const void* const pBuf,DWORD dwSize,DWORD dwSeed,void* pOut)
{
   if (!dwSize || !pBuf)
   {
      // Error !
      ASSERT(0);
      return;
   }

   if (IsBadReadPtr(pBuf,dwSize))
   {
      // Error !
      ASSERT(0);
      return;
   }

   const BYTE*    data = (const BYTE*)pBuf;

   const int   nblocks = dwSize / 16;
   
   DWORD h1 = dwSeed;
   DWORD h2 = dwSeed;
   DWORD h3 = dwSeed;
   DWORD h4 = dwSeed;
   
   DWORD    c1 = 0x239B961B;
   DWORD    c2 = 0xAB0E9789;
   DWORD    c3 = 0x38B34AE5;
   DWORD    c4 = 0xA1E38B93;
   
   //----------
   // body
   
   const DWORD*   blocks = (const DWORD*)(data + nblocks*16);
         
   for(int i = -nblocks; i; i++)
   {
      DWORD    k1 = getblock(blocks,i*4+0);
      DWORD    k2 = getblock(blocks,i*4+1);
      DWORD    k3 = getblock(blocks,i*4+2);
      DWORD    k4 = getblock(blocks,i*4+3);
   
      k1 *= c1; 
      k1  = ROTL32(k1,15); 
      k1 *= c2; 
      
      h1 ^= k1;
      
      h1 = ROTL32(h1,19); 
      h1 += h2; 
      h1 = h1 * 5 + 0x561CCD1B;
      
      k2 *= c2; 
      k2  = ROTL32(k2,16); 
      k2 *= c3; h2 ^= k2;
      
      h2 = ROTL32(h2,17); 
      h2 += h3; 
      h2 = h2 * 5 + 0x0BCAA747;
      
      k3 *= c3; 
      k3  = ROTL32(k3,17); 
      k3 *= c4; 
      
      h3 ^= k3;
      
      h3 = ROTL32(h3,15); 
      h3 += h4; 
      h3 = h3 * 5 + 0x96CD1C35;
      
      k4 *= c4; 
      k4  = ROTL32(k4,18); 
      k4 *= c1; 
      
      h4 ^= k4;
      
      h4 = ROTL32(h4,13); 
      h4 += h1; 
      h4 = h4 * 5 + 0x32AC3B17;
   }
   
   //----------
   // tail
   
   const BYTE*    tail = (const BYTE*)(data + nblocks*16);
   
   DWORD k1 = 0;
   DWORD k2 = 0;
   DWORD k3 = 0;
   DWORD k4 = 0;
   
   switch (dwSize & 15)
   {  
      case 15: k4 ^= tail[14] << 16;
      case 14: k4 ^= tail[13] << 8;
      case 13: k4 ^= tail[12] << 0;
               k4 *= c4; 
               k4  = ROTL32(k4,18); 
               k4 *= c1; 
               h4 ^= k4;
      
      case 12: k3 ^= tail[11] << 24;
      case 11: k3 ^= tail[10] << 16;
      case 10: k3 ^= tail[ 9] << 8;
      case  9: k3 ^= tail[ 8] << 0;
               k3 *= c3; 
               k3  = ROTL32(k3,17); 
               k3 *= c4; 
               h3 ^= k3;
      
      case  8: k2 ^= tail[ 7] << 24;
      case  7: k2 ^= tail[ 6] << 16;
      case  6: k2 ^= tail[ 5] << 8;
      case  5: k2 ^= tail[ 4] << 0;
               k2 *= c2; 
               k2  = ROTL32(k2,16); 
               k2 *= c3; 
               h2 ^= k2;
      
      case  4: k1 ^= tail[ 3] << 24;
      case  3: k1 ^= tail[ 2] << 16;
      case  2: k1 ^= tail[ 1] << 8;
      case  1: k1 ^= tail[ 0] << 0;
               k1 *= c1; 
               k1  = ROTL32(k1,15); 
               k1 *= c2; 
               h1 ^= k1;
   }
      
   // finalization
   h1 ^= dwSize; 
   h2 ^= dwSize; 
   h3 ^= dwSize; 
   h4 ^= dwSize;
   
   h1 += h2; 
   h1 += h3; 
   h1 += h4;
   
   h2 += h1; 
   h3 += h1; 
   h4 += h1;
   
   h1 = fmix(h1);
   h2 = fmix(h2);
   h3 = fmix(h3);
   h4 = fmix(h4);
   
   h1 += h2; 
   h1 += h3; 
   h1 += h4;

   h2 += h1; 
   h3 += h1; 
   h4 += h1;
   
   ((DWORD*)pOut)[0] = h1;
   ((DWORD*)pOut)[1] = h2;
   ((DWORD*)pOut)[2] = h3;
   ((DWORD*)pOut)[3] = h4;
}

/* ******************************************************************** **
** @@ void MurmurHash3_x64_128()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void MurmurHash3_x64_128(const void* pBuf,DWORD dwSize,DWORD dwSeed,void* pOut)
{
   if (!dwSize || !pBuf)
   {
      // Error !
      ASSERT(0);
      return;
   }

   if (IsBadReadPtr(pBuf,dwSize))
   {
      // Error !
      ASSERT(0);
      return;
   }

   const BYTE* data = (const BYTE*)pBuf;

   const int nblocks = dwSize / 16;

   QWORD h1 = dwSeed;
   QWORD h2 = dwSeed;
   
   QWORD c1 = BIG_CONSTANT(0x87C37B91114253D5);
   QWORD c2 = BIG_CONSTANT(0x4CF5AD432745937F);
   
   // body
   const QWORD*    blocks = (const QWORD*)(data);
   
   for(int ii = 0; ii < nblocks; ++ii)
   {
      QWORD   k1 = getblock(blocks,ii * 2 + 0);
      QWORD   k2 = getblock(blocks,ii * 2 + 1);
      
      k1 *= c1; 
      k1  = ROTL64(k1,31); 
      k1 *= c2; 
      
      h1 ^= k1;
      
      h1 = ROTL64(h1,27); 
      h1 += h2; 
      h1  = h1 * 5 + 0x52DCE729;
      
      k2 *= c2; 
      k2  = ROTL64(k2,33); 
      k2 *= c1; 
      
      h2 ^= k2;
      
      h2 = ROTL64(h2,31); 
      h2 += h1; 
      h2  = h2 * 5 + 0x38495AB5;
   }

   // tail
   const BYTE* tail = (const BYTE*)(data + nblocks * 16);
   
   QWORD k1 = 0;
   QWORD k2 = 0;
   
   switch (dwSize & 15)
   {
      case 15: k2 ^= QWORD(tail[14]) << 48;
      case 14: k2 ^= QWORD(tail[13]) << 40;
      case 13: k2 ^= QWORD(tail[12]) << 32;
      case 12: k2 ^= QWORD(tail[11]) << 24;
      case 11: k2 ^= QWORD(tail[10]) << 16;
      case 10: k2 ^= QWORD(tail[ 9]) << 8;
      case  9: k2 ^= QWORD(tail[ 8]) << 0;
               k2 *= c2; 
               k2  = ROTL64(k2,33); 
               k2 *= c1; 
               h2 ^= k2;
      
      case  8: k1 ^= QWORD(tail[ 7]) << 56;
      case  7: k1 ^= QWORD(tail[ 6]) << 48;
      case  6: k1 ^= QWORD(tail[ 5]) << 40;
      case  5: k1 ^= QWORD(tail[ 4]) << 32;
      case  4: k1 ^= QWORD(tail[ 3]) << 24;
      case  3: k1 ^= QWORD(tail[ 2]) << 16;
      case  2: k1 ^= QWORD(tail[ 1]) << 8;
      case  1: k1 ^= QWORD(tail[ 0]) << 0;
               k1 *= c1; 
               k1  = ROTL64(k1,31); 
               k1 *= c2; 
               h1 ^= k1;
   }
   
   // finalization
   h1 ^= dwSize; 
   h2 ^= dwSize;
   
   h1 += h2;
   h2 += h1;
   
   h1 = fmix(h1);
   h2 = fmix(h2);
   
   h1 += h2;
   h2 += h1;
   
   ((QWORD*)pOut)[0] = h1;
   ((QWORD*)pOut)[1] = h2;
}

/* ******************************************************************** **
** @@                   End of File
** ******************************************************************** */
