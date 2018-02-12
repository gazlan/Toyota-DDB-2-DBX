/* ******************************************************************** **
** @@ Text
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

#ifndef _TEXT_HPP_
#define _TEXT_HPP_

#if _MSC_VER > 1000
#pragma once
#endif

#define MAP_WORD_SIGNATURE_RU_SIZE     (26)
#define MAX_VOWELS_RU                  (9)      

/* ******************************************************************** **
** @@                   internal defines
** ******************************************************************** */

const char pXLAT_PRINTABLE_MAP[256] =
{
   '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E',
   '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E', '\x2E',
   '\x20', '\x21', '\x22', '\x23', '\x24', '\x25', '\x26', '\x27', '\x28', '\x29', '\x2A', '\x2B', '\x2C', '\x2D', '\x2E', '\x2F',
   '\x30', '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37', '\x38', '\x39', '\x3A', '\x3B', '\x3C', '\x3D', '\x3E', '\x3F',
   '\x40', '\x41', '\x42', '\x43', '\x44', '\x45', '\x46', '\x47', '\x48', '\x49', '\x4A', '\x4B', '\x4C', '\x4D', '\x4E', '\x4F',
   '\x50', '\x51', '\x52', '\x53', '\x54', '\x55', '\x56', '\x57', '\x58', '\x59', '\x5A', '\x5B', '\x5C', '\x5D', '\x5E', '\x5F',
   '\x60', '\x61', '\x62', '\x63', '\x64', '\x65', '\x66', '\x67', '\x68', '\x69', '\x6A', '\x6B', '\x6C', '\x6D', '\x6E', '\x6F',
   '\x70', '\x71', '\x72', '\x73', '\x74', '\x75', '\x76', '\x77', '\x78', '\x79', '\x7A', '\x7B', '\x7C', '\x7D', '\x7E', '\x2E',
   '\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87', '\x88', '\x89', '\x8A', '\x8B', '\x8C', '\x8D', '\x8E', '\x8F',
   '\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97', '\x98', '\x99', '\x9A', '\x9B', '\x9C', '\x9D', '\x9E', '\x9F',
   '\xA0', '\xA1', '\xA2', '\xA3', '\xA4', '\xA5', '\xA6', '\xA7', '\xA8', '\xA9', '\xAA', '\xAB', '\xAC', '\xAD', '\xAE', '\xAF',
   '\xB0', '\xB1', '\xB2', '\xB3', '\xB4', '\xB5', '\xB6', '\xB7', '\xB8', '\xB9', '\xBA', '\xBB', '\xBC', '\xBD', '\xBE', '\xBF',
   '\xC0', '\xC1', '\xC2', '\xC3', '\xC4', '\xC5', '\xC6', '\xC7', '\xC8', '\xC9', '\xCA', '\xCB', '\xCC', '\xCD', '\xCE', '\xCF',
   '\xD0', '\xD1', '\xD2', '\xD3', '\xD4', '\xD5', '\xD6', '\xD7', '\xD8', '\xD9', '\xDA', '\xDB', '\xDC', '\xDD', '\xDE', '\xDF',
   '\xE0', '\xE1', '\xE2', '\xE3', '\xE4', '\xE5', '\xE6', '\xE7', '\xE8', '\xE9', '\xEA', '\xEB', '\xEC', '\xED', '\xEE', '\xEF',
   '\xF0', '\xF1', '\xF2', '\xF3', '\xF4', '\xF5', '\xF6', '\xF7', '\xF8', '\xF9', '\xFA', '\xFB', '\xFC', '\xFD', '\xFE', '\xFF'
};

//////////////////////////////////////////////////////////////////////////

typedef bool (__cdecl * COMPARER)(const void* const pKey1,const void* const pKey2);

bool __fastcall  IsPrintable(BYTE byChr);
BYTE __fastcall  MakePrintable(BYTE byChr);
void __fastcall  EnsurePrintable(char* pStr);
bool __fastcall  IsDigit(int iChr);
bool __fastcall  IsHexDigit(int iChr);
bool __fastcall  IsHexStr(const char* const pStr);
bool __fastcall  IsJokeredHexStr(const char* const pStr);
bool __fastcall  IsDecStr(const char* const pStr);
bool __fastcall  IsOctStr(const char* const pStr);
bool __fastcall  IsBinStr(const char* const pStr);
         
char*    TrimInPlace(char* pszSrc);
void     Unspace(const char* const pszIn,char* pszOut);
void     Normalize(const char* const pszIn,char* pszOut);
char*    DuplicateString(const char* const pszString);
int      CountChars(const BYTE* const pBuf,DWORD dwSize,const BYTE* const pMap);
int      CountDups(const BYTE* const pBuf,DWORD dwSize,COMPARER pIsEqual,int iMin = 0,int iMax = 255);
bool     IsValidDupsCnt(const char* const pStr);
void     Equalize(BYTE* pBuf,DWORD dwSize,const BYTE* const pMap);
bool     Jammer(char* pSrc,int& riSize);
void     NormalizeTokenCSV(char* pszText);

bool     IsRussianVovel(int iChr);
bool     IsRussianVoicedConsonant(int iChr);
bool     IsRussianVoicelessConsonant(int iChr);
bool     IsText(const BYTE* const pBuf,DWORD dwSize);

const char* const AcquireToken(const char* const pText,char pcDelimiter,char* pszValue,int iMaxTokenSize,bool bCSV = false);
const char* const AcquireToken(const char* const pText,char pcDelimiter,CString& rsValue,bool bCSV = false);

void  ShrinkString(CString& rShow,const char* const pszFull,int iDisplaySize);
void  ShrinkWord(char* pszDest,const char* const pszSrc,int iSize);

void  PrintWordBits(WORD wValue);

void  MakeWordSignatureS_RU(char* pszSignature,const BYTE* const pszWord,DWORD dwLen);
void  MakeWordSignatureI_RU(char* pszSignature,const BYTE* const pszWord,DWORD dwLen);

const char* const RussianVowels(const BYTE* const pBuf,DWORD dwSize);

#endif

/* ******************************************************************** **
** End of File
** ******************************************************************** */
