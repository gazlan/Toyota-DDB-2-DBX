/* ******************************************************************** **
** @@ PRNG
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

#include "prng_int.h"

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

static int  iRandomNum = 1;

/* ******************************************************************** **
** @@                   real code
** ******************************************************************** */

/* +++Date last modified: 05-Jul-1997 */

/*
**  IntRand() - generate 2**31-2 random numbers
**
**  public domain by Ray Gardner
** 
**  based on "Random Number Generators: Good Ones Are Hard to Find",
**  S.K. Park and K.W. Miller, Communications of the ACM 31:10 (Oct 1988),
**  and "Two Fast Implementations of the 'Minimal Standard' Random
**  Number Generator", David G. Carta, Comm. ACM 33, 1 (Jan 1990), p. 87-88
**
**  linear congruential generator f(z) = 16807 z mod (2 ** 31 - 1)
**
**  uses L. Schrage's method to avoid overflow problems
*/

#define a      16807         /* multiplier */
#define m      2147483647L   /* 2**31 - 1 */
#define q      127773L       /* m div a */
#define r      2836          /* m mod a */

/* ******************************************************************** **
** @@ NextIntRand()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

static int NextIntRand(int iSeed)
{  
   DWORD    Lo = a * (int)(iSeed & 0xFFFF);
   DWORD    Hi = a * (int)((DWORD)iSeed >> 16);

   Lo += (Hi & 0x7FFF) << 16;
      
   if (Lo > m)
   {
      Lo &= m;
      ++Lo;
   }

   Lo += Hi >> 15;

   if (Lo > m)
   {  
      Lo &= m;
      ++Lo;
   }

   return (int)Lo;
}

/* ******************************************************************** **
** @@ SeedIntRand()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : To seed it 
** ******************************************************************** */

void SeedIntRand(DWORD dwSeed)
{    
   iRandomNum = dwSeed  ?  (int)(dwSeed & m)  :  1;  // nonzero seed
}

/* ******************************************************************** **
** @@ IntRand()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Return next random int
** ******************************************************************** */

int IntRand()
{
   iRandomNum = NextIntRand(iRandomNum);

   return iRandomNum;
}





#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{  
   int reps, k, num;
   DWORD seed;

   reps = 10000;
   seed = 1;

   /*
   ** correctness test: after 10000 reps starting with seed 1,
   ** result should be 1043618065
   */
   
   if (argc > 1)
   {  
      reps = atol(argv[1]);
   }

   if (argc > 2)
   {  
      seed = atol(argv[2]);
   }

   printf("seed %ld for %ld reps...\n", seed, reps);

   SeedIntRand(seed);

   for (k = 0; k < reps; ++k)
   {  
      num = IntRand();
   }

   printf("%ld\n", num);

   return 0;
}

#endif
