#ifndef __XLAT_HPP__
#define __XLAT_HPP__

#if _MSC_VER > 1000
#pragma once
#endif

void     XLAT(char* pszText,DWORD dwSize,const BYTE* const pTable);
bool     IsRussianLetter(int iChar);
bool     IsRussianMetatextVowels(int iChar);
int      PackRusText20(const char* const pszLastName,__int64& iPart1,__int64& iPart2);

#endif

/* ******************************************************************** **
** End of File
** ******************************************************************** */
