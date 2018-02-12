/* ******************************************************************** **
** uses pre-compiled headers
** ******************************************************************** */

#include "stdafx.h"

#include "text.h"

#ifdef _MFC_VER
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/* ******************************************************************** **
** @@ internal defines
** ******************************************************************** */

#define ASCII_SIZE                  (256)

// Without Yo !
#define ALPHABET_SIZE_RU            (32)

const char RUSSIAN_SYMBOLS[ASCII_SIZE] =
{
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x01', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x01', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x01', '\x02', '\x02', '\x02', '\x02', '\x01', '\x02', '\x02', '\x01', '\x02', '\x03', '\x02', '\x02', '\x02', '\x01', '\x03', 
   '\x02', '\x03', '\x03', '\x01', '\x03', '\x03', '\x03', '\x03', '\x03', '\x03', '\xDA', '\x01', '\xDC', '\x01', '\x01', '\x01', 
   '\x01', '\x02', '\x02', '\x02', '\x02', '\x01', '\x02', '\x02', '\x01', '\x02', '\x03', '\x02', '\x02', '\x02', '\x01', '\x03', 
   '\x02', '\x03', '\x03', '\x01', '\x03', '\x03', '\x03', '\x03', '\x03', '\x03', '\xFA', '\x01', '\xFC', '\x01', '\x01', '\x01'
};

enum RUSSIAN_PHONE
{
   RUSSIAN_PHONE_NONE,
   RUSSIAN_PHONE_VOVEL,   
   RUSSIAN_PHONE_VOICED_CONSONANT,
   RUSSIAN_PHONE_VOICELESS_CONSONANT
};
            
// Гласные: А, Е, Ё, И, О, У, Ы, Э, Ю, Я
// Звонкие: Б, В, Д, З, Ж, Г
// только звонкие (сонорные): Л, М, Н, Р, Й
// Глухие: П, Ф, Т, С, Ш, К
// только глухие: Ч, Щ, Ц, Х

const char pXLAT_WORD_SIGNATURE_RU[ASCII_SIZE] =
{
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x56', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x56', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', 
   '\x56', '\x43', '\x43', '\x43', '\x43', '\x56', '\x43', '\x43', '\x56', '\x43', '\x43', '\x43', '\x43', '\x43', '\x56', '\x43', 
   '\x43', '\x43', '\x43', '\x56', '\x43', '\x43', '\x43', '\x43', '\x43', '\x43', '\x43', '\x56', '\x43', '\x56', '\x56', '\x56', 
   '\x56', '\x43', '\x43', '\x43', '\x43', '\x56', '\x43', '\x43', '\x56', '\x43', '\x43', '\x43', '\x43', '\x43', '\x56', '\x43', 
   '\x43', '\x43', '\x43', '\x56', '\x43', '\x43', '\x43', '\x43', '\x43', '\x43', '\x43', '\x56', '\x43', '\x56', '\x56', '\x56' 
};

static const char pLOWER_2_UPPER[ASCII_SIZE] =
{
   '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07', 
   '\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F', 
   '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17', 
   '\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F', 
   '\x20', '\x21', '\x22', '\x23', '\x24', '\x25', '\x26', '\x27', 
   '\x28', '\x29', '\x2A', '\x2B', '\x2C', '\x2D', '\x2E', '\x2F', 
   '\x30', '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37', 
   '\x38', '\x39', '\x3A', '\x3B', '\x3C', '\x3D', '\x3E', '\x3F', 
   '\x40', '\x41', '\x42', '\x43', '\x44', '\x45', '\x46', '\x47', 
   '\x48', '\x49', '\x4A', '\x4B', '\x4C', '\x4D', '\x4E', '\x4F', 
   '\x50', '\x51', '\x52', '\x53', '\x54', '\x55', '\x56', '\x57', 
   '\x58', '\x59', '\x5A', '\x5B', '\x5C', '\x5D', '\x5E', '\x5F', 
   '\x60', '\x41', '\x42', '\x43', '\x44', '\x45', '\x46', '\x47', 
   '\x48', '\x49', '\x4A', '\x4B', '\x4C', '\x4D', '\x4E', '\x4F', 
   '\x50', '\x51', '\x52', '\x53', '\x54', '\x55', '\x56', '\x57', 
   '\x58', '\x59', '\x5A', '\x7B', '\x7C', '\x7D', '\x7E', '\x7F', 
   '\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87', 
   '\x88', '\x89', '\x8A', '\x8B', '\x8C', '\x8D', '\x8E', '\x8F', 
   '\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97', 
   '\x98', '\x99', '\x9A', '\x9B', '\x9C', '\x9D', '\x9E', '\x9F', 
   '\xA0', '\xA1', '\xA2', '\xA3', '\xA4', '\xA5', '\xA6', '\xA7', 
   '\xA8', '\xA9', '\xAA', '\xAB', '\xAC', '\xAD', '\xAE', '\xAF', 
   '\xB0', '\xB1', '\xB2', '\xB3', '\xB4', '\xB5', '\xB6', '\xB7', 
   '\xB8', '\xB9', '\xBA', '\xBB', '\xBC', '\xBD', '\xBE', '\xBF', 
   '\xC0', '\xC1', '\xC2', '\xC3', '\xC4', '\xC5', '\xC6', '\xC7', 
   '\xC8', '\xC9', '\xCA', '\xCB', '\xCC', '\xCD', '\xCE', '\xCF', 
   '\xD0', '\xD1', '\xD2', '\xD3', '\xD4', '\xD5', '\xD6', '\xD7', 
   '\xD8', '\xD9', '\xDA', '\xDB', '\xDC', '\xDD', '\xDE', '\xDF', 
   '\xC0', '\xC1', '\xC2', '\xC3', '\xC4', '\xC5', '\xC6', '\xC7', 
   '\xC8', '\xC9', '\xCA', '\xCB', '\xCC', '\xCD', '\xCE', '\xCF', 
   '\xD0', '\xD1', '\xD2', '\xD3', '\xD4', '\xD5', '\xD6', '\xD7', 
   '\xD8', '\xD9', '\xDA', '\xDB', '\xDC', '\xDD', '\xDE', '\xDF' 
};

const char pMAP_WORD_SIGNATURE_RU[MAP_WORD_SIGNATURE_RU_SIZE + 1] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* ******************************************************************** **
** @@ internal prototypes
** ******************************************************************** */

/* ******************************************************************** **
** @@ external global variables
** ******************************************************************** */

/* ******************************************************************** **
** @@ static global variables
** ******************************************************************** */
                  
static char    _pVowelsArr[MAX_VOWELS_RU];

/* ******************************************************************** **
** @@ real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ IsPrintable()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update : 
** @  Notes  : 
** ******************************************************************** */

bool __fastcall IsPrintable(BYTE byChr)
{
   if (byChr < 0x20)
   {
      return false;
   }
   
   if (byChr == 0x7F)
   {
      return false;
   }
   
   return true;
}

/* ******************************************************************** **
** @@ MakePrintable()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

BYTE __fastcall MakePrintable(BYTE byChr)
{
   if (byChr < 0x20)
   {
      return 0x2E;
   }

   if (byChr == 0x7F)
   {
      return 0x2E;
   }

   return byChr;
}

/* ******************************************************************** **
** @@ IsDigit()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Due BUG in MSVC routine !
** ******************************************************************** */

bool __fastcall IsDigit(int iChr)
{
   return ((BYTE)iChr >= 0x30) && ((BYTE)iChr <= 0x39); 
}

/* ******************************************************************** **
** @@ IsHexDigit()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool __fastcall IsHexDigit(int iChr)
{
   return ((BYTE)iChr >= 0x30) && ((BYTE)iChr <= 0x39) ||
          ((BYTE)iChr >= 'a') && ((BYTE)iChr <= 'z')   ||
          ((BYTE)iChr >= 'A') && ((BYTE)iChr <= 'Z');
}

/* ******************************************************************** **
** @@ IsHexStr()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool __fastcall IsHexStr(const char* const pStr)
{
   int      iSize = strlen(pStr);

   for (int ii = 0; ii < iSize; ++ii)
   {
      if ((!IsDigit(pStr[ii])) && !((pStr[ii] >= 'a') && (pStr[ii] <= 'f')) && !((pStr[ii] >= 'A') && (pStr[ii] <= 'F'))) 
      {
         return false;
      }
   }

   return true;
}

/* ******************************************************************** **
** @@ IsJokeredHexStr()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool __fastcall IsJokeredHexStr(const char* const pStr)
{
   int      iSize = strlen(pStr);

   for (int ii = 0; ii < iSize; ++ii)
   {
      if ((!IsDigit(pStr[ii])) && !((pStr[ii] >= 'a') && (pStr[ii] <= 'f')) && !((pStr[ii] >= 'A') && (pStr[ii] <= 'F'))) 
      {
         if (pStr[ii] != '.')
         {
            return false;
         }
      }
   }

   return true;
}

/* ******************************************************************** **
** @@ IsDecStr()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool __fastcall IsDecStr(const char* const pStr)
{ 
   int      iSize = strlen(pStr);

   for (int ii = 0; ii < iSize; ++ii)
   {
      if (!IsDigit(pStr[ii]))
      {
         return false;
      }         
   }

   return true;
}

/* ******************************************************************** **
** @@ IsOctStr()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool __fastcall IsOctStr(const char* const pStr)
{ 
   int      iSize = strlen(pStr);

   for (int ii = 0; ii < iSize; ++ii)
   {
      if (!IsDigit(pStr[ii]) || (pStr[ii] > '7'))
      {
         return false;
      }
   }

   return true;
}

/* ******************************************************************** **
** @@ IsBinStr()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool __fastcall IsBinStr(const char* const pStr)
{ 
   int      iSize = strlen(pStr);

   for (int ii = 0; ii < iSize; ++ii)
   {
      if ((pStr[ii] != '0') && (pStr[ii] != '1'))
      {
         return false;
      }
   }

   return true;
}

/* ******************************************************************** **
** @@ TrimInPlace()
** @  Copyrt :
** @  Author :
** @  Modify : 
** @  Update :
** @  Notes  : 
** ******************************************************************** */

char* TrimInPlace(char* pszSrc)
{
   if (!pszSrc || !*pszSrc)
   {
      // Nothing to do !
      return NULL;
   }

   int   iSkip = 0;

   while (isspace((BYTE)pszSrc[iSkip]))
   {
      ++iSkip;
   }

   int   iSize = strlen(pszSrc) - iSkip;

   memmove(pszSrc,&pszSrc[iSkip],iSize);
   pszSrc[iSize] = 0; // Ensure ASCIIZ

   iSize = strlen(pszSrc);

   while (iSize && (isspace((BYTE)pszSrc[iSize - 1])))
   {
      --iSize;
   }

   pszSrc[iSize] = 0; // Ensure ASCIIZ

   return pszSrc;
}

/* ******************************************************************** **
** @@ CountDups()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

int CountDups
(
   const BYTE* const       pBuf,
   DWORD                   dwSize,
   COMPARER                pIsEqual,
   int                     iMin,
   int                     iMax
)
{
   if (IsBadReadPtr(pBuf,dwSize))
   {
      return 0;
   }

   if (dwSize < 2)
   {
      return 0;
   }

   iMin = max(  0,iMin);
   iMax = min(255,iMax);

   BYTE  pMap[ASCII_SIZE * ASCII_SIZE];

   memset(pMap,0,sizeof(pMap));

   BYTE     pCnt[ASCII_SIZE];

   memset(pCnt,0,sizeof(pCnt));

   int   iSerial = 0;

   DWORD    ii = 0;
   
   for (ii = 1; ii < dwSize; ++ii)
   {
      if (pIsEqual(&pBuf[ii],&pBuf[ii - 1]))
      {
         ++pMap[(BYTE)pBuf[ii] + ASCII_SIZE * iSerial];
      }
      else
      {
         ++iSerial;
      }
   }

   int   iMaxDups = 0;

   for (ii = 0; ii < (dwSize - 1); ++ii)
   {
      for (int jj = iMin; jj <= iMax; ++jj)
      {
         int   iValue = pMap[jj + ASCII_SIZE * ii];

         if (iValue)
         {
            pCnt[jj] = (BYTE)max(pCnt[jj],iValue);
            iMaxDups = max(iMaxDups,pCnt[jj]);
         }
      }
   }

   return iMaxDups;
}

/* ******************************************************************** **
** @@ CountChars()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

int CountChars
(
   const BYTE* const       pBuf,
   DWORD                   dwSize,
   const BYTE* const       pMap
)
{
   if (!dwSize)
   {
      return 0;
   }

   if (IsBadReadPtr(pBuf,dwSize))
   {
      return 0;
   }

   if (IsBadReadPtr(pMap,ASCII_SIZE))
   {
      return 0;
   }

   int   iCnt = 0;

   for (DWORD ii = 0; ii < dwSize; ++ii)
   {
      if (pMap[pBuf[ii]])
      {
         ++iCnt;
      }
   }

   return iCnt;
}

/* ******************************************************************** **
** @@ Equalize()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void Equalize
(
   BYTE*                   pBuf,
   DWORD                   dwSize,
   const BYTE* const       pMap
)
{
   if (!dwSize)
   {
      return;
   }

   if (IsBadReadPtr(pBuf,dwSize))
   {
      return;
   }

   if (IsBadReadPtr(pMap,ASCII_SIZE))
   {
      return;
   }

   for (DWORD ii = 0; ii < dwSize; ++ii)
   {
      pBuf[ii] = pMap[pBuf[ii]];
   }
}
/* ******************************************************************** **
** @@ Unspace()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void Unspace(const char* const pszIn,char* pszOut)
{      
   char*    pTemp = (char*)pszIn;

   int   iSize = 0;
   
   for (int ii = 0; *pTemp; ++ii)
   {
      if (*pTemp != ' ' && *pTemp != '\t' && *pTemp != '\n' && *pTemp != '\r')
      {
         pszOut[ii] = *pTemp;
         ++pTemp;
         ++iSize;
      }
      else
      {
         pszOut[ii] = 0x20;
         ++pTemp;
         ++iSize;

         while (*pTemp == ' ' || *pTemp == '\t' || *pTemp == '\n' || *pTemp == '\r')
         {
            ++pTemp;
         }
      }
   }

   pszOut[iSize] = 0;       // ASCIIZ

   // Remove Trailing Spaces
   if (iSize)
   {
      char*    pEnd = pszOut + (iSize - 1);

      while (pEnd && (pEnd > pszOut) && *pEnd == ' ')
      {
         *pEnd = 0;
         --pEnd;
      }
   }
}

/* ******************************************************************** **
** @@ Normalize()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : Eat whitespace, but untouch quoted string (& remove qoutes itself)
** ******************************************************************** */

void Normalize(const char* const pszIn,char* pszOut)
{
   char*    pTemp = (char*)pszIn;
    
   bool     bQuote = false;

   int   iSize = 0;
   
   for (int ii = 0; *pTemp; ++ii)
   {
      if (*pTemp == '"')
      {
         bQuote = !bQuote;
         ++pTemp;
      }

      if ((*pTemp == '\\') && ((*(pTemp + 1) == 'x') || (*(pTemp + 1) == 'X')))
      {
         DWORD    dwData = 0;

         memcpy(&dwData,pTemp + 2,2);

         pszOut[ii++] = (char)strtoul((char*)&dwData,NULL,0x10);
         pTemp += 4;
         ++iSize;
      }

      if ((*pTemp != ' ' && *pTemp != '\t' && *pTemp != '\n' && *pTemp != '\r') || bQuote)
      {
         pszOut[ii] = *pTemp;
         ++pTemp;
         ++iSize;
      }
      else if (*pTemp == ' ' || *pTemp == '\t')
      {
         pszOut[ii] = *pTemp;
         ++pTemp;
         ++iSize;

         while (*pTemp == ' ' || *pTemp == '\t' || *pTemp == '\n' || *pTemp == '\r')
         {
            ++pTemp;
         }
      }
      else
      {
         ++pTemp;
      }
   }

   pszOut[iSize] = 0;       // ASCIIZ
}

/* ******************************************************************** **
** @@ DuplicateString()
** @  Copyrt :
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

char* DuplicateString(const char* const pszString)
{
   ASSERT(pszString);

   if (!pszString)
   {
      return NULL;
   }

   int   iSize = strlen(pszString);

   ASSERT(iSize);
   ASSERT(iSize <= MAX_PATH);

   if (!iSize)
   {
      return NULL;
   }

   iSize = min(iSize,MAX_PATH);

   char*    pNewStr = new char[iSize + 1];

   strncpy(pNewStr,pszString,iSize);
   pNewStr[iSize] = 0;       // ASCIIZ

   return pNewStr;
}

/* ******************************************************************** **
** @@ AcquireToken()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

const char* const AcquireToken
(  
   const char* const    pText,
   char                 pcDelimiter,
   char*                pszValue,
   int                  iMaxTokenSize,
   bool                 bCSV
)
{
   if (!pText || !pszValue)
   {
      return NULL;
   }
          
   *pszValue = 0;    // Clear !

   char*    pDelimiter = (char*)pText;   // Start

   NEXT:
   
   pDelimiter = strchr(pDelimiter,pcDelimiter);

   if (!pDelimiter)
   {
      // May be the LAST token ?
      strncpy(pszValue,pText,iMaxTokenSize);
      pszValue[iMaxTokenSize - 1] = 0;    // ASCIIZ
      return NULL;
   }

   if (bCSV)
   {
      if (*(pDelimiter - 1) == '\"')
      {
         int      iSize = pDelimiter - pText;

         strncpy(pszValue,pText,iSize);
         pszValue[iSize] = 0;    // ASCIIZ

         return (const char* const)++pDelimiter;
      }
      else
      {
         ++pDelimiter;  // Search Next
         goto NEXT;
      }
   }

   int      iSize = pDelimiter - pText;

   strncpy(pszValue,pText,iSize);
   pszValue[iSize] = 0;    // ASCIIZ

   return (const char* const)++pDelimiter;
}

/* ******************************************************************** **
** @@ AcquireToken()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

const char* const AcquireToken
(  
   const char* const    pText,
   char                 pcDelimiter,
   CString&             rsValue,
   bool                 bCSV
)
{
   if (!pText)
   {
      // Error !
      return NULL;
   }
          
   rsValue = _T("");    // Clear !

   char*    pDelimiter = (char*)pText;   // Start

   NEXT:
   
   pDelimiter = strchr(pDelimiter,pcDelimiter);

   if (!pDelimiter)
   {
      // May be the LAST token ?
      rsValue = pText;
      return NULL;
   }

   if (bCSV)
   {
      if (*(pDelimiter - 1) == '\"')
      {
         int      iSize = pDelimiter - pText;

         rsValue = pText;
         rsValue.SetAt(iSize,0);    // Truncate

         return (const char* const)++pDelimiter;
      }
      else
      {
         ++pDelimiter;  // Search Next
         goto NEXT;
      }
   }

   int      iSize = pDelimiter - pText;

   rsValue = pText;
   rsValue.SetAt(iSize,0);    // Truncate

   return (const char* const)++pDelimiter;
}

/* ******************************************************************** **
** @@ Jammer()
** @  Copyrt :
** @  Author :
** @  Modify : 
** @  Update :
** @  Notes  : 
** ******************************************************************** */

bool Jammer(char* pSrc,int& riSize)
{
   if (IsBadReadPtr(pSrc,riSize))
   {
      // Error !
      return false; 
   }

   int   ii = 0;
   
   for (ii = 0; ii < riSize; ++ii)
   {  
      switch (pSrc[ii])
      {
         case '\t':  // Replace Tabs with Space
         {
            pSrc[ii] = ' ';
            break;
         }
         case '\r':  // Replace CR with Space
         {
            pSrc[ii] = ' ';
            break;
         }
         case '\n':  // Replace LF with Space
         {
            pSrc[ii] = ' ';
            break;
         }
         default:
         {
            // Do nothing !
            break;
         }
      }
   }
   
   // Remove Leading Spaces
   char*    pBegin = pSrc;

   while (pBegin && *pBegin == ' ' && riSize)
   {
      ++pBegin;
      --riSize;
   }

   char*    pEnd = pBegin + riSize - 1; 

   // Proceed trailing zeros if any
   while (!*pEnd && riSize)
   {
      --pEnd;
      --riSize;
   }

   // Remove Trailing Spaces
   while (pEnd && *pEnd == ' ' && riSize)
   {
      --pEnd;
      --riSize;
   }

   char*    pTemp = new char[riSize + 1];

   if (!pTemp)
   {
      // Error !
      return false; 
   }

   memset(pTemp,0,riSize);

   int      jj = 0;

   for (ii = 0; ii < riSize; ++ii)
   {
      // Remove Duplicated Spaces
      if (pBegin[ii] == ' ')
      {
         // Very first - just copy
         pTemp[jj++] = pBegin[ii++];                   

         while (pBegin[ii] == ' ')
         {
            ++ii;  // Skip !
         }
      }

      // Just Copy
      pTemp[jj++] = pBegin[ii]; 
      pTemp[jj]   = 0;    // ASCIIZ
   }

   riSize    = jj;
   pTemp[jj] = 0;    // ASCIIZ

   memcpy(pSrc,pTemp,riSize + 1);   // With trailing ZERO !

   delete pTemp;
   pTemp = NULL;

   return true;
}

/* ******************************************************************** **
** @@ NormalizeTokenCSV()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void NormalizeTokenCSV(char* pszText)
{
   int   iIn  = 0;
   int   iOut = 0;

   while (pszText[iIn])
   {
      while ((pszText[iIn] == ' ') || (pszText[iIn] == '\"'))
      {
         ++iIn;
      }

      pszText[iOut++] = pszText[iIn++];
   }

   pszText[iOut] = 0;   // ASCIIZ
}

/* ******************************************************************** **
** @@ ShrinkString()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void ShrinkString(CString& rShow,const char* const pszFull,int iDisplaySize) 
{
   rShow = _T("");

   if (!pszFull || !*pszFull)
   {  
      // Empty string
      ASSERT(0);
      return;
   }

   int   iSize   = strlen(pszFull);
   int   iSize2  = iSize / 2;
   int   iExtra  = iSize - iDisplaySize + 5;
   int   iExtra2 = (iExtra + 1) / 2;

   if (iSize <= iDisplaySize)
   {
      rShow = pszFull;
      return;
   }

   int   iLeft  = max(0,iSize2 - iExtra2);
   int   iRight = min(iSize2 + iExtra2,iSize);

   for (int ii = 0; ii < iLeft; ++ii)   
   {
      rShow += pszFull[ii];
   }

   rShow += "\x20...\x20";

   rShow += &pszFull[iRight];
}  

/* ******************************************************************** **
** @@ IsRussianVovel()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool IsRussianVovel(int iChr)
{
   return RUSSIAN_SYMBOLS[(BYTE)iChr] == RUSSIAN_PHONE_VOVEL;
}

/* ******************************************************************** **
** @@ IsRussianVoicedConsonant()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool IsRussianVoicedConsonant(int iChr)
{
   return RUSSIAN_SYMBOLS[(BYTE)iChr] == RUSSIAN_PHONE_VOICED_CONSONANT;
}

/* ******************************************************************** **
** @@ IsRussianVoicelessConsonant()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool IsRussianVoicelessConsonant(int iChr)
{
   return RUSSIAN_SYMBOLS[(BYTE)iChr] == RUSSIAN_PHONE_VOICELESS_CONSONANT;
}

/* ******************************************************************** **
** @@ IsText()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

bool IsText(const BYTE* const pBuf,DWORD dwSize)
{
   if (IsBadReadPtr(pBuf,dwSize))
   {
      // Error !
      ASSERT(0);
      return false;
   }

   DWORD    pFrq[ASCII_SIZE];

   memset(pFrq,0,sizeof(DWORD) * ASCII_SIZE);

   for (DWORD ii = 0; ii < dwSize; ++ii)
   {
      ++pFrq[pBuf[ii]];
   }

   DWORD    dwScore = 0;

   for (int jj = 0; jj < 0x20; ++jj)
   {
      if ((jj >= 0x08) && (jj <= 0x0D))
      {
         // Skip Formatting & Whitespace
         continue;
      }
      
      dwScore += pFrq[jj];
   }

   return dwScore == 0;
}

/* ******************************************************************** **
** @@ ShrinkWord()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void ShrinkWord(char* pszDest,const char* const pszSrc,int iSize) 
{
   int   iSrcSize = strlen(pszSrc);
   int   iLead    = (iSize + 1) / 2 - 1;

   strncpy(pszDest,pszSrc,iLead);
   pszDest[iLead] = 0;   // Ensure ASCIIZ

   strcat(pszDest,"-");
   strcpy(&pszDest[iLead + 1],&pszSrc[iSrcSize - iLead]);
}

/* ******************************************************************** **
** @@ PrintWordBits()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void PrintWordBits(WORD wValue)
{
   printf("%5d  %04X  |  ",wValue,wValue);

   int   iStrob = 0x01;

   for (int jj = 15; jj >= 0; --jj)
   {
      printf("%c",wValue & (iStrob << jj)  ?  '1'  :  '0');

      if (jj == 8)
      {
         printf(" ");
      }
   }

   printf("\n");
}

/* ******************************************************************** **
** @@ MakeWordSignatureS_RU()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : case sensitive
** ******************************************************************** */

void MakeWordSignatureS_RU(char* pszSignature,const BYTE* const pszWord,DWORD dwLen)
{
   if (IsBadWritePtr(pszSignature,MAP_WORD_SIGNATURE_RU_SIZE + 1))
   {
      // Error !
      return;
   }

   memset(pszSignature,0,MAP_WORD_SIGNATURE_RU_SIZE);

   DWORD    dwSigLen = min(dwLen,MAP_WORD_SIGNATURE_RU_SIZE);

   if (!dwSigLen)
   {
      return;
   }

   bool     bLong = (dwLen > dwSigLen)  ?  true  :  false;

   BYTE     pSet[ASCII_SIZE];

   memset(pSet,0xFF,ASCII_SIZE);

   bool     bStop = false;

   DWORD    ii = 0;
   
   for (ii = 0; ii < dwSigLen; ++ii) 
   {           
      if (bStop)
      {
         break;
      }

      BYTE     Type = pXLAT_WORD_SIGNATURE_RU[(BYTE)pszWord[ii]];
      BYTE     Idx  = (BYTE)pszWord[ii];

      if (pSet[Idx] == 0xFF)
      {
         // First time !
         pSet[Idx] = (BYTE)ii;
      }

      switch (Type)
      {
         case 'V':
         {
            // Vowel
            pszSignature[ii] = (char)(pMAP_WORD_SIGNATURE_RU[pSet[Idx]] & ~0x20);
            break;
         }
         case 'C':
         {
            // Consonant
            pszSignature[ii] = (char)(pMAP_WORD_SIGNATURE_RU[pSet[Idx]] | 0x20);
            break;
         }
         default:
         {
            // Error !
//            pszSignature[ii] = '#';
//            bStop = true;
            pszSignature[ii] = pMAP_WORD_SIGNATURE_RU[pSet[Idx]];
            break;
         }
      }
   }

   if (bLong)
   {
      pszSignature[ii] = '*';
   }

   pszSignature[ii + 1] = 0;  // Ensure ASCIIZ
}  

/* ******************************************************************** **
** @@ MakeWordSignatureI_RU()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  : case insensitive
** ******************************************************************** */

void MakeWordSignatureI_RU(char* pszSignature,const BYTE* const pszWord,DWORD dwLen)
{
   if (IsBadWritePtr(pszSignature,MAP_WORD_SIGNATURE_RU_SIZE + 1))
   {
      // Error !
      return;
   }

   memset(pszSignature,0,MAP_WORD_SIGNATURE_RU_SIZE);

   DWORD    dwSigLen = min(dwLen,MAP_WORD_SIGNATURE_RU_SIZE);

   if (!dwSigLen)
   {
      return;
   }

   bool     bLong = (dwLen > dwSigLen)  ?  true  :  false;

   BYTE     pSet[ASCII_SIZE];

   memset(pSet,0xFF,ASCII_SIZE);

   bool     bStop = false;

   DWORD    ii = 0;
   
   for (ii = 0; ii < dwSigLen; ++ii) 
   {           
      if (bStop)
      {
         break;
      }

      BYTE     Type = pXLAT_WORD_SIGNATURE_RU[(BYTE)pszWord[ii]];
      BYTE     Idx  = (BYTE)pszWord[ii];

      if (pSet[Idx] == 0xFF)
      {
         // First time !
         pSet[Idx] = (BYTE)ii;
      }

      switch (Type)
      {
         case 'V':
         {
            // Vowel
            pszSignature[ii] = (char)(pMAP_WORD_SIGNATURE_RU[pSet[Idx]] & ~0x20);
            break;
         }
         case 'C':
         {
            // Consonant
            pszSignature[ii] = (char)(pMAP_WORD_SIGNATURE_RU[pSet[Idx]] & ~0x20);
            break;
         }
         default:
         {
            // Error !
//            pszSignature[ii] = '#';
//            bStop = true;
            pszSignature[ii] = pMAP_WORD_SIGNATURE_RU[pSet[Idx]];
            break;
         }
      }
   }

   if (bLong)
   {
      pszSignature[ii - 1] = '*';
   }

   pszSignature[ii] = 0;  // Ensure ASCIIZ
}  

/* ******************************************************************** **
** @@ EnsurePrintable()
** @  Copyrt : 
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

void __fastcall EnsurePrintable(char* pszStr)
{
   if (!pszStr || !*pszStr)
   {
      // Empty !
      return;
   }

   DWORD    dwLen = strlen(pszStr);

   for (DWORD ii = 0; ii < dwLen; ++ii)
   {
      pszStr[ii] = pXLAT_PRINTABLE_MAP[(BYTE)pszStr[ii]];
   }
}

/* ******************************************************************** **
** @@ RussianVowels()
** @ Copyrt:
** @ Author:
** @ Modify:
** @ Update:
** @ Notes : Identify which ciphertext characters most likely represent vowels 
** ******************************************************************** */

const char* const RussianVowels(const BYTE* const pBuf,DWORD dwSize)
{
   DWORD    pMatrix[ALPHABET_SIZE_RU][ALPHABET_SIZE_RU];
   int      pSum[ALPHABET_SIZE_RU];

   memset(pMatrix,0,ALPHABET_SIZE_RU * ALPHABET_SIZE_RU * sizeof(DWORD));
   memset(pSum,0,ALPHABET_SIZE_RU * sizeof(int));

   bool    pVowelFlag[ALPHABET_SIZE_RU];
   
   memset(pVowelFlag,0,ALPHABET_SIZE_RU);

   DWORD    ii = 0;
   
   // Initialize adjacency matrix
   for (ii = 1; ii < dwSize; ++ii)
   {
      BYTE     byChrPrev = (BYTE)(pLOWER_2_UPPER[pBuf[ii - 1]]);
      BYTE     byChrNext = (BYTE)(pLOWER_2_UPPER[pBuf[ii]]);

      if ((byChrPrev >= 0xC0) && (byChrPrev <= 0xDF) && (byChrNext >= 0xC0) && (byChrNext <= 0xDF))
      {                       
         byChrPrev -= 0xC0;   // 'А'
         byChrNext -= 0xC0;   // 'А'

         ++pMatrix[byChrNext][byChrPrev];
         ++pMatrix[byChrPrev][byChrNext];
      }
   }

   for (int jj = 0; jj < ALPHABET_SIZE_RU; ++jj)
   {
      pMatrix[jj][jj] = 0; // Diagonal

      for (int kk = 0; kk < ALPHABET_SIZE_RU; ++kk)
      {
         pSum[jj] += pMatrix[jj][kk];
      }
   }

   int   iCnt = 0;

   while (true)
   {
      int      iMaxSum = 0;
      int      iIndex  = 0;

      int   ii = 0;
      
      for (ii = 0; ii < ALPHABET_SIZE_RU; ++ii)
      {
         if (!pVowelFlag[ii] && (pSum[ii] > iMaxSum))
         {
            iMaxSum = pSum[ii];
            iIndex  = ii;
         }
      }

      if ((iMaxSum > 0) && (iCnt < MAX_VOWELS_RU))
      {
         pVowelFlag[iIndex] = true;

         _pVowelsArr[iCnt++] = (char)(0xC0 + iIndex); // 'А'
      }
      else
      {
         break;
      }

      for (ii = 0; ii < ALPHABET_SIZE_RU; ++ii)
      {
         if (!pVowelFlag[ii])
         {
            pSum[ii] -= pMatrix[ii][iIndex] * 2;
         }
      }
   }

   return _pVowelsArr;
}

/* ******************************************************************** **
**  End of File
** ******************************************************************** */
