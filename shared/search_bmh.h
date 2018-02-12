/* ******************************************************************** **
** @@ BMH Fast Search
** @  Copyrt : 2003-2004 Herbert Bos & Willem de Bruijn * wdebruij@users.sourceforge.net
** @  Author : Fairly Fast Packet Filter * http://ffpf.sourceforge.net/
** @  Modify :
** @  Update :
** @  Notes  : The the Boyer-Moore-Horspool algorithm
** ******************************************************************** */

/*
   Original code courtesy of

   "EXACT STRING MATCHING ALGORITHMS
   Animation in Java"
   http://www-igm.univ-mlv.fr/~lecroq/string/

   Christian Charras - Thierry Lecroq
   Laboratoire d'Informatique de Rouen
   Universit�de Rouen
   Facult�des Sciences et des Techniques
   76821 Mont-Saint-Aignan Cedex
   FRANCE
*/

#ifndef _BMH_HPP_
#define _BMH_HPP_

#if _MSC_VER > 1000
#pragma once
#endif

/* ******************************************************************** **
** @@ BMH_Search
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  : 
** ******************************************************************** */

int BMH_Search (const BYTE* const pBuf,int iBufSize,const BYTE* const pPattern,int iPatternSize); 
int BMH_ISearch(const BYTE* const pBuf,int iBufSize,const BYTE* const pPattern,int iPatternSize); 

/* ******************************************************************** **
** @@                   The End
** ******************************************************************** */

#endif
