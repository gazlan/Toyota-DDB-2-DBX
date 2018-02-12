/* ******************************************************************** **
** @@ Timestamp Header File
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

#ifndef _TIMESTAMP_HPP_
#define _TIMESTAMP_HPP_

#if _MSC_VER > 1000
#pragma once
#endif

/* ******************************************************************** **
** @@                   internal defines
** ******************************************************************** */

struct TIMESTAMP
{
   BYTE                 _byHour;
   BYTE                 _byMinute;
   BYTE                 _bySecond;
   WORD                 _wMillisec;
   BYTE                 _byDay;
   BYTE                 _byMonth;
   int                  _iYear;
   char                 _pszDay  [3];
   char                 _pszMonth[3];
   char                 _pszYear [5];
   bool                 _bLeap;
   BYTE                 _byWeekDay;
   WORD                 _byYearDay;
   __int64              _iLinearTime;
};

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
** @@ class DateTime
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

class Timestamp
{
   private:

      TIMESTAMP      _TS;

   public:

               Timestamp();
      virtual ~Timestamp();

      void     SplitDateString(const char* const pszDate,const char* const pszFormat);
      void     MergeDateString(char* pszDate,const char* const pszFormat);

      DWORD    TS_Time(int iHour,int iMinute,int iSecond,int iMillisec);
      void     TS_Time(DWORD dwTime,int& iHour,int& iMinute,int& iSecond,int& iMillisec);

      DWORD    TS_TimeNow(); 
      DWORD    TS_Time(); 
      void     TS_Time(DWORD dwTime); 

      DWORD    TS_Date(int iYear,int iMonth,int iDay); 
      void     TS_Date(DWORD dwDate,int& iYear,int& iMonth,int& iDay); 

      DWORD    TS_DateNow(); 
      DWORD    TS_Date(); 
      void     TS_Date(DWORD dwDate); 
      
      __int64  LinearTime(DWORD dwDate,DWORD dwTime);
      __int64  GetFileTime(HANDLE hFile);

      bool     IsValidDate(); 
      bool     IsValidDate(int iYear,int iMonth,int iDay); 
      
      int      WeekDay(); 
      int      WeekDay(DWORD dwDate);
      int      WeekDay(int iYear,int iMonth,int iDay); 

      double   JulyanDate(int iYear,int iMonth,int iDay); 
      void     GregorianDate(double fJulyanDate,int& riYear,int& riMonth,int& riDay); 
      bool     IsLeapYear(int iYear); 
      void     ConvertDateDelimiter(char* pszDate,char cOld,char cNew);
};

#endif

void Time_Local_2_UTC(const FILETIME& rLocal,FILETIME& rUTC);
void Time_Local_2_Local(const FILETIME& rLocalFrom,FILETIME& rLocalTo,int iTZShiftFrom,int iTZShiftTo);
bool SetFileTimeStampUTC(const char* const pszFile,const FILETIME& rBaseUTC,int iDelayMin,int iDelaySec,bool bDelayRandom);

/* ******************************************************************** **
** End of File
** ******************************************************************** */
