// swad_string.h: string processing

#ifndef _SWAD_STR
#define _SWAD_STR
/*
    SWAD (Shared Workspace At a Distance in Spanish),
    is a web platform developed at the University of Granada (Spain),
    and used to support university teaching.

    This file is part of SWAD core.
    Copyright (C) 1999-2019 Antonio Ca�as Vargas

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*****************************************************************************/
/********************************* Headers **********************************/
/*****************************************************************************/

#include <limits.h>		// For NAME_MAX and PATH_MAX
#include <stdbool.h>		// For boolean type
#include <stdio.h>		// For FILE

/*****************************************************************************/
/***************************** Public constants *****************************/
/*****************************************************************************/
/*
   Some special characters, like a chinese character,
   are received from a form in a format like this:
   %26%2335753%3B --> %26 %23 3 5 7 5 3 %3B --> &#35753;
		       ^   ^             ^
		       |   |             |
	      SpecialChar SpecialChar SpecialChar
   Here one chinese character is converted
   to 2 special chars + 5 normal chars + 1 special char,
   and finally is stored as the following 8 bytes: &#35753;

   The maximum UTF-8 code is 1114111 or 0x10FFFF.
   So, when read from form, a character may be read temporarily as %26%231114111%3B (16 bytes)
   Then it may be transformed to &#1114111; (10 bytes)
   So, each char from a form may be transformed finally into a sequence of 1 to 10 bytes,
   but temporarily it may need 16 bytes
*/
#define Str_MAX_BYTES_PER_CHAR	16	// Maximum number of bytes of a char.
					// Do not change (or change carefully) because it is used to compute size of database fields

/*****************************************************************************/
/******************************* Public types *******************************/
/*****************************************************************************/

typedef enum
  {
   Str_NO_SKIP_HTML_COMMENTS,
   Str_SKIP_HTML_COMMENTS,
  } Str_SkipHTMLComments_t;
typedef enum
  {
   Str_FROM_FORM,
   Str_FROM_TEXT,
   Str_FROM_HTML,
   } Str_ChangeFrom_t;
typedef enum
  {
   Str_DONT_CHANGE,
   Str_TO_RIGOROUS_HTML,
   Str_TO_HTML,
   Str_TO_TEXT,
   Str_TO_MARKDOWN,
   } Str_ChangeTo_t;

/*****************************************************************************/
/***************************** Public prototypes ****************************/
/*****************************************************************************/

void Str_InsertLinks (char *Txt,unsigned long MaxLength,size_t MaxCharsURLOnScreen);
size_t Str_LimitLengthHTMLStr (char *Str,size_t MaxCharsOnScreen);
// bool Str_URLLooksValid (const char *URL);
void Str_ConvertToTitleType (char *Str);
void Str_ConvertToComparable (char *Str);
char *Str_ConvertToUpperText (char *Str);
char *Str_ConvertToLowerText (char *Str);
char Str_ConvertToUpperLetter (char Ch);
char Str_ConvertToLowerLetter (char Ch);

void Str_WriteDoubleNumToFile (FILE *FileDst,double Number);
void Str_DoubleNumToStr (char **Str,double Number);
void Str_ConvertStrFloatCommaToStrFloatPoint (char *Str);
double Str_GetDoubleNumFromStr (const char *Str);
void Str_SetDecimalPointToUS (void);
void Str_SetDecimalPointToLocal (void);

void Str_AddStrToQuery (char *Query,const char *Str,size_t SizeOfQuery);
void Str_ChangeFormat (Str_ChangeFrom_t ChangeFrom,Str_ChangeTo_t ChangeTo,
                       char *Str,size_t MaxLengthStr,bool RemoveLeadingAndTrailingSpaces);
void Str_RemoveLeadingSpacesHTML (char *Str);
void Str_RemoveTrailingSpacesHTML (char *Str);
void Str_RemoveLeadingZeros (char *Str);
void Str_RemoveLeadingArrobas (char *Str);
bool Str_FindStrInFile (FILE *FileSrc,const char *Str,Str_SkipHTMLComments_t SkipHTMLComments);
bool Str_FindStrInFileBack (FILE *FileSrc,const char *Str,Str_SkipHTMLComments_t SkipHTMLComments);
bool Str_WriteUntilStrFoundInFileIncludingStr (FILE *FileTgt,FILE *FileSrc,const char *Str,Str_SkipHTMLComments_t SkipHTMLComments);
char *Str_GetCellFromHTMLTableSkipComments (FILE *FileSrc,char *Str,int MaxLength);
char *Str_GetNextStrFromFileConvertingToLower (FILE *FileSrc,char *Str, int N);
void Str_GetNextStringUntilSpace (const char **StrSrc,char *StrDst,size_t MaxLength);
void Str_GetNextStringUntilSeparator (const char **StrSrc,char *StrDst,size_t MaxLength);
void Str_GetNextStringUntilComma (const char **StrSrc,char *StrDst,size_t MaxLength);
void Str_ReplaceSeveralSpacesForOne (char *Str);
void Str_CopyStrChangingSpaces (const char *StringWithSpaces,char *StringWithoutSpaces,unsigned MaxLength);
long Str_ConvertStrCodToLongCod (const char *Str);
size_t Str_GetLengthRootFileName (const char *FileName);
void Str_SplitFullPathIntoPathAndFileName (const char FullPath[PATH_MAX + 1],
                                           char PathWithoutFileName[PATH_MAX + 1],
                                           char FileName[NAME_MAX + 1]);
bool Str_FileIs (const char *FileName,const char *Extension);
bool Str_FileIsHTML (const char *FileName);
bool Str_Path1BeginsByPath2 (const char *Path1,const char *Path2);
void Str_SkipSpacesInFile (FILE *FileSrc);
void Str_FilePrintStrChangingBRToRetAndNBSPToSpace (FILE *FileTgt,const char *Str);
int Str_ReadFileUntilBoundaryStr (FILE *FileSrc,char *StrDst,
                                  const char *BoundaryStr,
                                  unsigned LengthBoundaryStr,
                                  unsigned long long MaxLength);
bool Str_ConvertFilFolLnkNameToValid (char *FileName);
void Str_ConvertToValidFileName (char *Str);

void Str_CreateRandomAlphanumStr (char *Str,size_t Length);

void Str_Copy (char *Dst,const char *Src,size_t DstSize);
void Str_Concat (char *Dst,const char *Src,size_t DstSize);

#endif
