/* ******************************************************************** **
** @@ Timestamp Source File
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

/* ******************************************************************** **
** uses pre-compiled headers
** ******************************************************************** */

#include "stdafx.h"

#include "math.h"
#include "stdlib.h"
#include "time.h"

#include "prng_int.h"
#include "timestamp.h"
#include "file.h"

/* ******************************************************************** **
** @@ internal defines
** ******************************************************************** */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
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
                  
static const char*   pszDay[7]     = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sanday" };
static const char*   pszAbbrDay[7] = { "Mon",    "Tue",     "Wed",       "Thu",      "Fri",    "Sat",      "San"    };

#define  TS_BASE_YEAR   (1900)
#define  YEAR_SIZE      (365.25)
#define  MONTH_SIZE     (30.6001)

static const int MAX_DAY[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static const int  MONTH_DAY_SUM[13] =
{
   0x0000, 0x001F, 0x003B, 0x005A, 0x0078, 0x0097, 0x00B5, 0x00D4, 0x00F3, 0x0111, 0x0130, 0x014E, 0x016D
};

static const int  YEAR_DAY_SUM[256] =
{
   0x00000000, 0x0000016D, 0x000002DA, 0x00000447, 0x000005B5, 0x00000722, 0x0000088F, 0x000009FC, 0x00000B6A, 0x00000CD7, 0x00000E44, 0x00000FB1, 0x0000111F, 0x0000128C, 0x000013F9, 0x00001566, 
   0x000016D4, 0x00001841, 0x000019AE, 0x00001B1B, 0x00001C89, 0x00001DF6, 0x00001F63, 0x000020D0, 0x0000223E, 0x000023AB, 0x00002518, 0x00002685, 0x000027F3, 0x00002960, 0x00002ACD, 0x00002C3A, 
   0x00002DA8, 0x00002F15, 0x00003082, 0x000031EF, 0x0000335D, 0x000034CA, 0x00003637, 0x000037A4, 0x00003912, 0x00003A7F, 0x00003BEC, 0x00003D59, 0x00003EC7, 0x00004034, 0x000041A1, 0x0000430E, 
   0x0000447C, 0x000045E9, 0x00004756, 0x000048C3, 0x00004A31, 0x00004B9E, 0x00004D0B, 0x00004E78, 0x00004FE6, 0x00005153, 0x000052C0, 0x0000542D, 0x0000559B, 0x00005708, 0x00005875, 0x000059E2, 
   0x00005B50, 0x00005CBD, 0x00005E2A, 0x00005F97, 0x00006105, 0x00006272, 0x000063DF, 0x0000654C, 0x000066BA, 0x00006827, 0x00006994, 0x00006B01, 0x00006C6F, 0x00006DDC, 0x00006F49, 0x000070B6, 
   0x00007224, 0x00007391, 0x000074FE, 0x0000766B, 0x000077D9, 0x00007946, 0x00007AB3, 0x00007C20, 0x00007D8E, 0x00007EFB, 0x00008068, 0x000081D5, 0x00008343, 0x000084B0, 0x0000861D, 0x0000878A, 
   0x000088F8, 0x00008A65, 0x00008BD2, 0x00008D3F, 0x00008EAD, 0x0000901A, 0x00009187, 0x000092F4, 0x00009462, 0x000095CF, 0x0000973C, 0x000098A9, 0x00009A17, 0x00009B84, 0x00009CF1, 0x00009E5E, 
   0x00009FCC, 0x0000A139, 0x0000A2A6, 0x0000A413, 0x0000A581, 0x0000A6EE, 0x0000A85B, 0x0000A9C8, 0x0000AB36, 0x0000ACA3, 0x0000AE10, 0x0000AF7D, 0x0000B0EB, 0x0000B258, 0x0000B3C5, 0x0000B532, 
   0x0000B6A0, 0x0000B80D, 0x0000B97A, 0x0000BAE7, 0x0000BC55, 0x0000BDC2, 0x0000BF2F, 0x0000C09C, 0x0000C20A, 0x0000C377, 0x0000C4E4, 0x0000C651, 0x0000C7BF, 0x0000C92C, 0x0000CA99, 0x0000CC06, 
   0x0000CD74, 0x0000CEE1, 0x0000D04E, 0x0000D1BB, 0x0000D329, 0x0000D496, 0x0000D603, 0x0000D770, 0x0000D8DE, 0x0000DA4B, 0x0000DBB8, 0x0000DD25, 0x0000DE93, 0x0000E000, 0x0000E16D, 0x0000E2DA, 
   0x0000E448, 0x0000E5B5, 0x0000E722, 0x0000E88F, 0x0000E9FD, 0x0000EB6A, 0x0000ECD7, 0x0000EE44, 0x0000EFB2, 0x0000F11F, 0x0000F28C, 0x0000F3F9, 0x0000F567, 0x0000F6D4, 0x0000F841, 0x0000F9AE, 
   0x0000FB1C, 0x0000FC89, 0x0000FDF6, 0x0000FF63, 0x000100D1, 0x0001023E, 0x000103AB, 0x00010518, 0x00010686, 0x000107F3, 0x00010960, 0x00010ACD, 0x00010C3B, 0x00010DA8, 0x00010F15, 0x00011082, 
   0x000111F0, 0x0001135D, 0x000114CA, 0x00011637, 0x000117A5, 0x00011912, 0x00011A7F, 0x00011BEC, 0x00011D59, 0x00011EC6, 0x00012033, 0x000121A0, 0x0001230E, 0x0001247B, 0x000125E8, 0x00012755, 
   0x000128C3, 0x00012A30, 0x00012B9D, 0x00012D0A, 0x00012E78, 0x00012FE5, 0x00013152, 0x000132BF, 0x0001342D, 0x0001359A, 0x00013707, 0x00013874, 0x000139E2, 0x00013B4F, 0x00013CBC, 0x00013E29, 
   0x00013F97, 0x00014104, 0x00014271, 0x000143DE, 0x0001454C, 0x000146B9, 0x00014826, 0x00014993, 0x00014B01, 0x00014C6E, 0x00014DDB, 0x00014F48, 0x000150B6, 0x00015223, 0x00015390, 0x000154FD, 
   0x0001566B, 0x000157D8, 0x00015945, 0x00015AB2, 0x00015C20, 0x00015D8D, 0x00015EFA, 0x00016067, 0x000161D5, 0x00016342, 0x000164AF, 0x0001661C, 0x0001678A, 0x000168F7, 0x00016A64, 0x00016BD1 
};

/* ******************************************************************** **
** @@ real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ Timestamp::Timestamp()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update : 25 Dec 2003
** @  Notes  : Constructor
** ******************************************************************** */

Timestamp::Timestamp()
{
   memset(&_TS,0,sizeof(_TS));
}

/* ******************************************************************** **
** @@ Timestamp::~Timestamp()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update : 25 Dec 2003
** @  Notes  : Destructor
** ******************************************************************** */

Timestamp::~Timestamp()
{
}

/* ******************************************************************** **
** @@ Timestamp::SplitDateString()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

#pragma warning (disable: 4100)  // unreferenced formal parameter

void Timestamp::SplitDateString(const char* const pszDate,const char* const pszFormat)
{
   if (!pszDate || !*pszDate)
   {
      return;
   }
          
   if (!pszFormat || !*pszFormat)
   {
      return;
   }
     
   int   iDayPos   = 0;
   int   iMonthPos = 0;
   int   iYearPos  = 0;

   char*    pDate   = (char*)pszDate;
   char*    pFormat = (char*)pszFormat;

   while (*pFormat && *pDate)
   {
      switch (*pFormat)
      {
         case 'D':
         case 'd':
         {
            if (iDayPos < 2)
            {
               _TS._pszDay[iDayPos++] = *pDate;
            }
            
            break;
         }
         case 'M':
         case 'm':
         {
            if (iMonthPos < 2)
            {
               _TS._pszMonth[iMonthPos++] = *pDate;
            }
            
            break;
         }
         case 'Y':
         case 'y':
         {
            if (iYearPos < 4)
            {
               _TS._pszYear[iYearPos++] = *pDate;
            }
            break;
         }
         case '.':
         case '-':
         case '/':
         default:
         {
            break;
         }
      }

      ++pDate;
      ++pFormat;
   }

   _TS._byDay   = (BYTE)atoi(_TS._pszDay);
   _TS._byMonth = (BYTE)atoi(_TS._pszMonth);
   _TS._iYear   = atoi(_TS._pszYear);
}

/* ******************************************************************** **
** @@ Timestamp::MergeDateString()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void Timestamp::MergeDateString(char* pszDate,const char* const pszFormat)
{
   if (!pszDate || !*pszDate)
   {
      return;
   }
          
   if (!pszFormat || !*pszFormat)
   {
      return;
   }
     
   int   iDayPos   = 0;
   int   iMonthPos = 0;
   int   iYearPos  = 0;

   char*    pDate   = (char*)pszDate;
   char*    pFormat = (char*)pszFormat;

   while (*pFormat)
   {
      switch (*pFormat)
      {
         case 'D':
         {
            *pDate = _TS._pszDay[iDayPos++];
            break;
         }
         case 'M':
         {
            *pDate = _TS._pszMonth[iMonthPos++];
            break;
         }
         case 'Y':
         {
            *pDate = _TS._pszYear[iYearPos++];
            break;
         }
         case '.':
         {
            *pDate = '.';
            break;
         }
         case '-':
         {
            *pDate = '-';
            break;
         }
         case '/':
         {
            *pDate = '/';
            break;
         }
         default:
         {
            break;
         }
      }

      ++pDate;
      ++pFormat;

      *pDate = 0; // ASCIIZ
   }
}

/* ******************************************************************** **
** @@ Timestamp::ConvertDateDelimiter()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void Timestamp::ConvertDateDelimiter
(
   char*       pszDate,
   char        cOld,
   char        cNew
)
{
   if (!pszDate)
   {
      return;
   }

   while(*pszDate)
   {
      if (*pszDate == cOld)
      {
         *pszDate = cNew;
      }

      ++pszDate;
   }
}

/* ******************************************************************** **
** @@ Timestamp::WeekDay()
** @  Copyrt : http://mathforum.org/dr.math/faq/faq.calendar.html
** @  Author : Zeller's Rule
** @  Modify :
** @  Update :
** @  Notes  : It returns 1 for SunDay, 2 for MonDay, etc.
** ******************************************************************** */

int Timestamp::WeekDay(DWORD dwDate)
{
   int   iYear  = 0;
   int   iMonth = 0;
   int   iDay   = 0;

   TS_Date(dwDate,iYear,iMonth,iDay);

   return WeekDay(iYear,iMonth,iDay);
}

/* ******************************************************************** **
** @@ Timestamp::WeekDay()
** @  Copyrt : http://mathforum.org/dr.math/faq/faq.calendar.html
** @  Author : Zeller's Rule
** @  Modify :
** @  Update :
** @  Notes  : It returns 1 for SunDay, 2 for MonDay, etc.
** ******************************************************************** */

int Timestamp::WeekDay()
{
   return WeekDay(_TS._iYear,_TS._byMonth,_TS._byDay);
}

/* ******************************************************************** **
** @@ Timestamp::WeekDay()
** @  Copyrt : http://mathforum.org/dr.math/faq/faq.calendar.html
** @  Author : Zeller's Rule
** @  Modify :
** @  Update :
** @  Notes  : WeekDay() computes the Day of the Week based upon the given Date,
** @  Notes  : Month and Year. It uses the Zeller Congruence Algorithim.
** @  Notes  : (https://en.wikipedia.org/wiki/Zeller's_congruence)
** @  Notes  : iDate  is the Day of the Month,   1 - 31
** @  Notes  : iMonth is the Month of the Year,  1 - 12
** @  Notes  : iYear  is the Year, e.g.,         2000
** @  Notes  : It returns 0 for SunDay, 1 for MonDay, etc.
** ******************************************************************** */

int Timestamp::WeekDay(int iYear,int iMonth,int iDay)
{
   // Month:  March   -  3 ... December - 12 of Current  Year
   //         January - 13,    February - 14 of Previous Year
   if (iMonth < 3) 
   { 
      // If January or February, adjust Month and Year
      iMonth += 12;
      --iYear;
   }

   int   N1 = (26 * (iMonth + 1)) / 10;    // Month Shift
   int   N2 = (125 * iYear) / 100;         // Leap Correction

   int   N3 = iDay + N1 + N2 - (iYear / 100) + (iYear / 400) - 1;

   return N3 % 7;
}

/* ******************************************************************** **
** @@ Timestamp::IsValidDate()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool Timestamp::IsValidDate()
{
   return IsValidDate(_TS._iYear,_TS._byMonth,_TS._byDay);
}

/* ******************************************************************** **
** @@ Timestamp::IsValidDate()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool Timestamp::IsValidDate(int iYear,int iMonth,int iDay) 
{
   if ((iDay < 1)   || (iDay > 31)   || 
       (iMonth < 1) || (iMonth > 12) || 
       (iYear < TS_BASE_YEAR))
   {
      return false;
   }
         
   int   iAdd = ((iMonth > 2) && IsLeapYear(iYear))  ?  1  :  0;

   if (iDay > (MAX_DAY[iMonth - 1] + iAdd)) 
   {
      return false;
   }

   return true;
}

/* ******************************************************************** **
** @@ Timestamp::IsLeapYear()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool Timestamp::IsLeapYear(int iYear) 
{
   return ((!(iYear % 4) && (iYear % 100)) || (!(iYear % 400)))  ?  true  :  false;
}

/* ******************************************************************** **
** @@ Timestamp::JulyanDate()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : Convert Date to Julian's Date
** ******************************************************************** */

double Timestamp::JulyanDate(int iYear,int iMonth,int iDay)
{
   const double START_GREGORY = 1582.1015;   // 15 October 1582
   const double START_JULY    = 1720994.5;

   double    dCurrDate  =  (double)iYear;

   dCurrDate += ((double)iMonth) / 100.0;
   dCurrDate += ((double)iDay)   / 10000.0;

   // Month :  March   -  3 ... December - 12 of Current  Year
   //          January - 13,    February - 14 of Previous Year
   if (iMonth < 3) 
   { 
      // January & February
      iMonth += 12;
      --iYear;
   }

   int iLeap = 0;

   if (dCurrDate >= START_GREGORY)
   {
      iLeap = 2 - (iYear / 100) + (iYear / 400);
   }

   double dPart1 = YEAR_SIZE  * (double)iYear;
   double dPart2 = MONTH_SIZE * (double)(iMonth + 1);

   // Start New Day - 12:00 GMT
   return iLeap + dPart1 + dPart2 + (double)iDay + START_JULY;
}

/* ******************************************************************** **
** @@ Timestamp::GregorianDate()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : Convert Julian's Date to Gregorian's Date
** ******************************************************************** */

void Timestamp::GregorianDate(double fJulyanDate,int& riYear,int& riMonth,int& riDay)
{
   const double START_GREGORY = 2299161.0;   // 15 October 1582
   const double START_JULY    = 1720994.5;

   const double    CIVIL_YEAR = 365.2425;
   const double    NUMBER1    = 1867216.25;
   const double    NUMBER3    = 1524.0;
   const double    NUMBER4    = 122.1;

   double      fPart1    = 0.0;
   double      fPart2    = 0.0; 
   double      fPart3    = 0.0; 
   double      fMonthNum = 0.0;
   double      fCentury  = 0.0;
   double      fYear     = 0.0;
   double      fTmp      = 0.0;

   double    fLeap = 0.0;

// if (dJulyDate >= START_GREGORY)
//    dLeap = (double)(2 - (iYear / 100) + (iYear / 400));

   double fFractPart = modf((double)fJulyanDate,&fPart1);

   if (fPart1 >= START_GREGORY) 
   {
      modf((fPart1 - NUMBER1) / (CIVIL_YEAR * 100.0),&fCentury);
      modf(fCentury / 4.0,&fTmp);
      fLeap = fCentury - fTmp + 1.0;
   }

   double   fX = fJulyanDate - START_JULY - fLeap;

   riDay   = 0;
   riMonth = 0;
   riYear  = 0;

   modf(fX / YEAR_SIZE,&fYear);

   riYear = (int)fYear;
}

/* ******************************************************************** **
** @@ TS_Date()
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DWORD Timestamp::TS_Date(int iYear,int iMonth,int iDay) 
{
   if (iYear < TS_BASE_YEAR)
   {
      return 0;
   }

   if (!IsValidDate(iYear,iMonth,iDay))
   {
      return 0;
   }

   DWORD    dwDaySum = YEAR_DAY_SUM[iYear - TS_BASE_YEAR];

   dwDaySum += MONTH_DAY_SUM[iMonth - 1];

   if ((iMonth > 2) && IsLeapYear(iYear))
   {
      ++dwDaySum;
   }

   return dwDaySum + iDay;
}

/* ******************************************************************** **
** @@ Timestamp::TS_DateNow()
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DWORD Timestamp::TS_DateNow()
{
   time_t         TimeNow     = time(&TimeNow);
   struct tm*     pPackedTime = localtime(&TimeNow);

   return TS_Date(pPackedTime->tm_year + 1900,pPackedTime->tm_mon + 1,pPackedTime->tm_mday);
}

/* ******************************************************************** **
** @@ Timestamp::TS_Date()
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

DWORD Timestamp::TS_Date() 
{
   return TS_Date(_TS._iYear,_TS._byMonth,_TS._byDay);
}

/* ******************************************************************** **
** @@ Timestamp::TS_Date()
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void Timestamp::TS_Date(DWORD dwDate,int& iYear,int& iMonth,int& iDay)
{
   iYear  = 0;
   iMonth = 0;
   iDay   = 0;

   int   iRest = 0;

   for (int ii = 0; ii < 256; ++ii)
   {
      if ((DWORD)YEAR_DAY_SUM[ii] > dwDate)
      {
         iYear = TS_BASE_YEAR + (ii - 1);
         iRest = dwDate - YEAR_DAY_SUM[(ii - 1)];
         break;
      }
   }

   for (ii = 0; ii < 13; ++ii)
   {
      int   iAdd = ((ii > 2) && IsLeapYear(iYear))  ?  1  :  0;
       
      int   iMonthDaySum = MONTH_DAY_SUM[ii] + iAdd;

      if (iMonthDaySum > iRest)
      {
         iMonth = ii;
         iRest -= (MONTH_DAY_SUM[(ii - 1)] + iAdd);
         break;
      }
   }

   iDay = iRest  ?  iRest  :  1; 
}
 
/* ******************************************************************** **
** @@ Timestamp::TS_Date()
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void Timestamp::TS_Date(DWORD dwDate)
{
   int   iYear  = 0;
   int   iMonth = 0;
   int   iDay   = 0;

   TS_Date(dwDate,iYear,iMonth,iDay);

   _TS._iYear   = iYear;
   _TS._byMonth = (BYTE)iMonth;
   _TS._byDay   = (BYTE)iDay;
}

/* ******************************************************************** **
** @@ DateDifference()
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

#if 0
long DateDifference(const struct Date * const pdt1stDate,
                    const struct Date * const pdt2ndDate,
                    const char * const pszMetre) {

   if ((pdt1stDate->wYear < (WORD)iMCP_BaseYear) || 
       (pdt2ndDate->wYear < (WORD)iMCP_BaseYear))
      return (ERR_MCP_DATE_OUT_OF_RANGE);

   switch (*pszMetre) {
      case 'Y' :
      case 'y' :
         return (YearDifference(pdt1stDate,pdt2ndDate));
      case 'Q' :
      case 'q' :
         return (QuarterDifference(pdt1stDate,pdt2ndDate));
      case 'M' :
      case 'm' :
         return (MonthDifference(pdt1stDate,pdt2ndDate));
      case 'W' :
      case 'w' :
         return (WeekDifference(pdt1stDate,pdt2ndDate));
      case 'D' :
      case 'd' :
         return (DayDifference(pdt1stDate,pdt2ndDate));
      default :
         return (ERR_MCP_INVALID_METRE);
   }
}
#endif

/* ******************************************************************** **
** @@ YearDifference()
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

#if 0
long YearDifference(const struct Date * const pdt1stDate,const struct Date * const pdt2ndDate) 
{
   long lSumDays = 0L;

   int i1stYear  = pdt1stDate->wYear;
   int i1stMonth = pdt1stDate->byMonth;
   int i1stDay   = pdt1stDate->byDay;

   int i2ndYear  = pdt2ndDate->wYear;
   int i2ndMonth = pdt2ndDate->byMonth;
   int i2ndDay   = pdt2ndDate->byDay;

   int iYearDiff  = i1stYear  - i2ndYear;
   int iMonthDiff = i1stMonth - i2ndMonth;
   int iDayDiff   = i1stDay   - i2ndDay;

   if (!iYearDiff)
      return (0L);

/*
   if (iMonthDiff > 0) {
   }
   else if (iMonthDiff < 0) {
   }
   else {
      if (iDayDiff > 0) {
      }
      else if (iDayDiff < 0) {
      }
      else {
      }
   }
*/

   return (ERR_MCP_DATE_OUT_OF_RANGE);
}
#endif

/* ******************************************************************** **
** @@ QuarterDifference()
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

#if 0
long QuarterDifference(const struct Date * const pdt1stDate,const struct Date * const pdt2ndDate) 
{
   return (ERR_MCP_DATE_OUT_OF_RANGE);
}
#endif

/* ******************************************************************** **
** @@ MonthDifference()
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

#if 0
long MonthDifference(const struct Date * const pdt1stDate,const struct Date * const pdt2ndDate) 
{
   return (ERR_MCP_DATE_OUT_OF_RANGE);
}
#endif

/* ******************************************************************** **
** @@ WeekDifference()
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

#if 0
long WeekDifference(const struct Date * const pdt1stDate,const struct Date * const pdt2ndDate) 
{
   return (DayDifference(pdt1stDate,pdt2ndDate) / 7);
}
#endif

/* ******************************************************************** **
** @@ DayDifference()
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

#if 0
long DayDifference(const struct Date * const pdt1stDate,const struct Date * const pdt2ndDate) 
{
   long l1stDays = DaysFromBase(pdt1stDate);
   long l2ndDays = DaysFromBase(pdt2ndDate);

   return (l1stDays - l2ndDays);
}
#endif

/* ******************************************************************** **
** @@ Date_Add()
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : Date + Something !
** ******************************************************************** */

#if 0
int Date_Add(const struct Date * const pdtBaseDate,struct Date * pdtResultDate,const char * const pszMetre) 
{
   int iErrCode = 0;

   int iPeriod = atoi((const char *)&pszMetre[1]);

   if (iPeriod <= 0)
      return (ERR_MCP_INVALID_PERIODE_CODE);

   switch (pszMetre[0]) {
      case 'Y':
      case 'y':
         pdtResultDate = (struct Date * )pdtBaseDate;
         pdtResultDate->wYear += (WORD)iPeriod;

         if ((pdtResultDate->byMonth == 2) && (pdtResultDate->byDay == 29) &&
             (!CheckLeapYear(pdtResultDate->wYear)))
            pdtResultDate->byDay = 28;

         break;
      case 'Q':
      case 'q':
         iPeriod *= 3;
      case 'M':
      case 'm':
         {
            int iYear  = iPeriod % 12;
            int iMonth = iPeriod - (iYear * 12);

            pdtResultDate = (struct Date * )pdtBaseDate;
            pdtResultDate->wYear   += (WORD)iYear;
            pdtResultDate->byMonth += (BYTE)iMonth;
         }
         break;
      case 'W':
      case 'w':
         iPeriod *= 7;
      case 'D':
      case 'd':
         pdtResultDate = (struct Date * )pdtBaseDate;
         break;
      default :
         iErrCode = ERR_MCP_INVALID_PERIODE_CODE;
         break;
   }

   return (iErrCode);
}
#endif

/* ******************************************************************** **
** @@ Timestamp::TS_Time()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

DWORD Timestamp::TS_Time(int iHour,int iMinute,int iSecond,int iMillisec)
{
   iMillisec %= 1000;
   iSecond   %= 60;
   iMinute   %= 60;
   iHour     %= 24;

   return iMillisec + iSecond * 1000 + iMinute * 60000 + iHour * 3600000;
}
 
/* ******************************************************************** **
** @@ Timestamp::TS_Time()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void Timestamp::TS_Time(DWORD dwTime,int& iHour,int& iMinute,int& iSecond,int& iMillisec)
{
   iHour = dwTime / 3600000; 

   dwTime -= iHour * 3600000;

   iMinute = dwTime / 60000;

   dwTime -= iMinute * 60000;

   iSecond = dwTime / 1000;

   dwTime -= iSecond * 1000;

   iMillisec = dwTime;
}
 
/* ******************************************************************** **
** @@ Timestamp::TS_TimeNow()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

DWORD Timestamp::TS_TimeNow()
{
   time_t         TimeNow     = time(&TimeNow);
   struct tm*     pPackedTime = localtime(&TimeNow);

   return TS_Time(pPackedTime->tm_hour,pPackedTime->tm_min,pPackedTime->tm_sec,0);
}
 
/* ******************************************************************** **
** @@ Timestamp::TS_Time()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

DWORD Timestamp::TS_Time() 
{
   return TS_Time(_TS._byHour,_TS._byMinute,_TS._bySecond,_TS._wMillisec);
}
 
/* ******************************************************************** **
** @@ Timestamp::TS_Time()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

void Timestamp::TS_Time(DWORD dwTime) 
{
   int   iHour     = 0;
   int   iMinute   = 0;
   int   iSecond   = 0;
   int   iMillisec = 0;

   TS_Time(dwTime,iHour,iMinute,iSecond,iMillisec);

   _TS._byHour    = (BYTE)iHour;
   _TS._byMinute  = (BYTE)iMinute;
   _TS._bySecond  = (BYTE)iSecond;
   _TS._wMillisec = (WORD)iMillisec;
}

/* ******************************************************************** **
** @@ Timestamp::LinearTime()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

__int64 Timestamp::LinearTime(DWORD dwDate,DWORD dwTime)
{
   return (__int64)dwDate * (__int64)86400000 + (__int64)dwTime;
}

/* ******************************************************************** **
** @@ Timestamp::GetFileTime()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

__int64 Timestamp::GetFileTime(HANDLE hFile)
{
   FILETIME       FileTime;

   memset(&FileTime,0,sizeof(FILETIME));
   
   GetLatestFileTime(hFile,FileTime); 

// Haven't yet implemented !
   return 0;
}

/* ******************************************************************** **
** @@ Time_Local_2_UTC()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void Time_Local_2_UTC(const FILETIME& rLocal,FILETIME& rUTC)
{
   #ifdef _DEBUG
   SYSTEMTIME    SysTimeDebug;
   memset(&SysTimeDebug,0,sizeof(SYSTEMTIME));
   #endif

   unsigned __int64     FileTime64 = 0ui64;

   memcpy(&FileTime64,&rLocal,sizeof(__int64));

   TIME_ZONE_INFORMATION      TZInfo;

   ::GetTimeZoneInformation(&TZInfo);

   __int64     iBias  = TZInfo.Bias;
   __int64     iShift = 10000000i64 * 60i64 * iBias;

   #ifdef _DEBUG
   if (!::FileTimeToSystemTime((FILETIME*)&FileTime64,&SysTimeDebug))
   {
      // Error !
      ASSERT(0);
   }
   #endif

   FileTime64 += iShift;  // Inverted Sign !

   #ifdef _DEBUG
   if (!::FileTimeToSystemTime((FILETIME*)&FileTime64,&SysTimeDebug))
   {
      // Error !
      ASSERT(0);
   }
   #endif

   memcpy(&rUTC,&FileTime64,sizeof(__int64));
}

/* ******************************************************************** **
** @@ Time_Local_2_Local()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void Time_Local_2_Local(const FILETIME& rLocalFrom,FILETIME& rLocalTo,int iTZShiftFrom,int iTZShiftTo)
{
   if (iTZShiftFrom == iTZShiftTo)
   {
      memcpy(&rLocalTo,&rLocalFrom,sizeof(__int64));
      return;
   }

   #ifdef _DEBUG
   SYSTEMTIME    SysTimeDebug;
   memset(&SysTimeDebug,0,sizeof(SYSTEMTIME));
   #endif

   unsigned __int64     FileTime64 = 0ui64;

   memcpy(&FileTime64,&rLocalFrom,sizeof(__int64));

   TIME_ZONE_INFORMATION      TZInfo;

   ::GetTimeZoneInformation(&TZInfo);

//   __int64     iBias  = TZInfo.Bias;
   __int64     iShift = 10000000i64 * 60i64 * 60i64 * (iTZShiftTo - iTZShiftFrom);

   #ifdef _DEBUG
   if (!::FileTimeToSystemTime((FILETIME*)&FileTime64,&SysTimeDebug))
   {
      // Error !
      ASSERT(0);
   }
   #endif

   FileTime64 += iShift;

   #ifdef _DEBUG
   if (!::FileTimeToSystemTime((FILETIME*)&FileTime64,&SysTimeDebug))
   {
      // Error !
      ASSERT(0);
   }
   #endif

   memcpy(&rLocalTo,&FileTime64,sizeof(__int64));
}

/* ******************************************************************** **
** @@ SetFileTimeStampUTC()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool SetFileTimeStampUTC
(
   const char* const       pszFile,
   const FILETIME&         rBaseUTC,
   int                     iDelayMin,
   int                     iDelaySec,
   bool                    bDelayRandom
)
{
   HANDLE      hFile = OpenFileReadWrite(pszFile);

   if (hFile == INVALID_HANDLE_VALUE)
   {
      // Error !
      return false;
   }

   unsigned __int64     FileTime64 = 0ui64;

   memcpy(&FileTime64,&rBaseUTC,sizeof(__int64));

   int   iApxSec = 60 * iDelayMin + iDelaySec;

   if (bDelayRandom)
   {
      iApxSec += (int)IntRand() % 30;

      int   iApxMSec = (int)IntRand() % 10000; // random milliseconds

      __int64     iShift = 10000000i64 * iApxSec + 10000i64 * iApxMSec;

      FileTime64 += iShift;
   }

   FILETIME    CreationTime;
   FILETIME    LastAccessTime;
   FILETIME    LastWriteTime;

   memcpy(&CreationTime,  &FileTime64,sizeof(__int64));
   memcpy(&LastAccessTime,&FileTime64,sizeof(__int64));
   memcpy(&LastWriteTime, &FileTime64,sizeof(__int64));

   SetFileTime(hFile,&CreationTime,&LastAccessTime,&LastWriteTime);

   CloseHandle(hFile);
   hFile = INVALID_HANDLE_VALUE;

   return true;
}

/* ******************************************************************** **
** End of File
** ******************************************************************** */
