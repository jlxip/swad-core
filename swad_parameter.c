// swad_parameter.c: CGI parameters

/*
    SWAD (Shared Workspace At a Distance),
    is a web platform developed at the University of Granada (Spain),
    and used to support university teaching.

    This file is part of SWAD core.
    Copyright (C) 1999-2020 Antonio Ca�as Vargas

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
/********************************** Headers **********************************/
/*****************************************************************************/

#include <ctype.h>		// For isprint, isspace, etc.
#include <stddef.h>		// For NULL
#include <stdlib.h>		// For calloc
#include <string.h>		// For string functions

#include "swad_action.h"
#include "swad_config.h"
#include "swad_global.h"
#include "swad_HTML.h"
#include "swad_parameter.h"
#include "swad_password.h"
#include "swad_setting.h"
#include "swad_tab.h"

/*****************************************************************************/
/*************** External global variables from others modules ***************/
/*****************************************************************************/

extern struct Globals Gbl;

/*****************************************************************************/
/***************************** Public constants ******************************/
/*****************************************************************************/

const char *Par_SEPARATOR_PARAM_MULTIPLE = "\x0a";	// Must be 1 <= character <= 31

/*****************************************************************************/
/*********************** Private types and constants *************************/
/*****************************************************************************/

/*****************************************************************************/
/****************************** Private variables ****************************/
/*****************************************************************************/

/*****************************************************************************/
/***************************** Private prototypes ****************************/
/*****************************************************************************/

static void Par_GetBoundary (void);

static void Par_CreateListOfParamsFromQueryString (void);
static void Par_CreateListOfParamsFromTmpFile (void);
static int Par_ReadTmpFileUntilQuote (void);
static int Par_ReadTmpFileUntilReturn (void);

static bool Par_CheckIsParamCanBeUsedInGETMethod (const char *ParamName);

/*****************************************************************************/
/*** Read all parameters passed to this CGI and store for later processing ***/
/*****************************************************************************/

#define Par_MAX_BYTES_METHOD		(128 - 1)
#define Par_MAX_BYTES_CONTENT_TYPE	(128 - 1)

bool Par_GetQueryString (void)
  {
   char Method[Par_MAX_BYTES_METHOD + 1];
   char ContentType[Par_MAX_BYTES_CONTENT_TYPE + 1];
   char UnsignedLongStr[Cns_MAX_DECIMAL_DIGITS_ULONG + 1];
   unsigned long UnsignedLong;

   Str_Copy (Method,getenv ("REQUEST_METHOD"),
             Par_MAX_BYTES_METHOD);

   if (!strcmp (Method,"GET"))
     {
      /***** GET method *****/
      Gbl.Params.GetMethod = true;
      Gbl.ContentReceivedByCGI = Act_CONT_NORM;

      /* Get content length */
      Gbl.Params.ContentLength = strlen (getenv ("QUERY_STRING"));

      /* Allocate memory for query string */
      if ((Gbl.Params.QueryString = (char *) malloc (Gbl.Params.ContentLength + 1)) == NULL)
	 return false;

      /* Copy query string from environment variable */
      Str_Copy (Gbl.Params.QueryString,getenv ("QUERY_STRING"),
                Gbl.Params.ContentLength);
     }
   else
     {
      /***** PUSH method *****/
      /* Get content length */
      if (getenv ("CONTENT_LENGTH"))
	{
         Str_Copy (UnsignedLongStr,getenv ("CONTENT_LENGTH"),
                   Cns_MAX_DECIMAL_DIGITS_ULONG);
         if (sscanf (UnsignedLongStr,"%lu",&UnsignedLong) != 1)
            return false;
         Gbl.Params.ContentLength = (size_t) UnsignedLong;
	}
      else
         return false;

      /* If data are received ==> the environment variable CONTENT_TYPE will hold:
      multipart/form-data; boundary=---------------------------7d13ca2e948
      */
      if (getenv ("CONTENT_TYPE") == NULL)
         return false;

      Str_Copy (ContentType,getenv ("CONTENT_TYPE"),
                Par_MAX_BYTES_CONTENT_TYPE);

      if (!strncmp (ContentType,"multipart/form-data",strlen ("multipart/form-data")))
        {
         Gbl.ContentReceivedByCGI = Act_CONT_DATA;
         Par_GetBoundary ();
         return Fil_ReadStdinIntoTmpFile ();
        }
      else if (!strncmp (ContentType,"text/xml",strlen ("text/xml")))
        {
         Gbl.WebService.IsWebService = true;
        }
      else
        {
         Gbl.ContentReceivedByCGI = Act_CONT_NORM;

	 /* Allocate memory for query string */
	 if ((Gbl.Params.QueryString = (char *) malloc (Gbl.Params.ContentLength + 1)) == NULL)
	    return false;

	 /* Copy query string from stdin */
         if (fread (Gbl.Params.QueryString,sizeof (char),Gbl.Params.ContentLength,stdin) != Gbl.Params.ContentLength)
           {
            Gbl.Params.QueryString[0] = '\0';
            return false;
           }
	 Gbl.Params.QueryString[Gbl.Params.ContentLength] = '\0';
        }
     }

   return true;
  }

/*****************************************************************************/
/**************************** Get boundary string ****************************/
/*****************************************************************************/

static void Par_GetBoundary (void)
  {
   const char *PtrToBoundary;

   /*
   If data are received ==> the environment variable CONTENT_TYPE will hold:
   multipart/form-data; boundary=---------------------------7d13ca2e948
   Gbl.Boundary.StrWithCRLF will be set to:
			  "\r\n-----------------------------7d13ca2e948"
   I.e. 0x0D, 0x0A, '-', '-', and boundary.
   */
   /***** Get pointer to boundary string *****/
   PtrToBoundary = strstr (getenv ("CONTENT_TYPE"),"boundary=") + strlen ("boundary=");

   /***** Check that boundary string is not too long *****/
   if (2 + 2 + strlen (PtrToBoundary) > Par_MAX_BYTES_BOUNDARY_WITH_CR_LF)
      Lay_ShowErrorAndExit ("Delimiter string too long.");

   /***** Create boundary strings *****/
   snprintf (Gbl.Boundary.StrWithoutCRLF,sizeof (Gbl.Boundary.StrWithoutCRLF),
	     "--%s",
	     PtrToBoundary);
   snprintf (Gbl.Boundary.StrWithCRLF,sizeof (Gbl.Boundary.StrWithCRLF),
	     "%c%c%s",
	     0x0D,0x0A,Gbl.Boundary.StrWithoutCRLF);

   /***** Compute lengths *****/
   Gbl.Boundary.LengthWithoutCRLF = strlen (Gbl.Boundary.StrWithoutCRLF);
   Gbl.Boundary.LengthWithCRLF    = 2 + Gbl.Boundary.LengthWithoutCRLF;
  }

/*****************************************************************************/
/************************ Create list of parameters **************************/
/*****************************************************************************/
/*
         Parameter #1               Parameter #n
         +------------------+       +------------------+
List --> |Name.Start        |    -> |Name.Start        |
         +------------------+   /   +------------------+
         |Name.Length       |  |    |Name.Length       |
         +------------------+  |    +------------------+
         |FileName.Start    |  |    |FileName.Start    |
         +------------------+  |    +------------------+
         |FileName.Lenght   |  |    |FileName.Lenght   |
         +------------------+  .    +------------------+
         |ContentType.Start |  .    |ContentType.Start |
         +------------------+  .    +------------------+
         |ContentType.Lenght|  |    |ContentType.Lenght|
         +------------------+  |    +------------------+
         |Value.Start       |  |    |Value.Start       |
         +------------------+  |    +------------------+
         |Value.Lengh       |  |    |Value.Lengh       |
         +------------------+ /     +------------------+
         |       Next --------      |       NULL       |
         +------------------+       +------------------+
*/

void Par_CreateListOfParams (void)
  {
   /***** Initialize empty list of parameters *****/
   Gbl.Params.List = NULL;

   /***** Get list *****/
   if (Gbl.Params.ContentLength)
      switch (Gbl.ContentReceivedByCGI)
	{
	 case Act_CONT_NORM:
	    Par_CreateListOfParamsFromQueryString ();
	    break;
	 case Act_CONT_DATA:
	    Par_CreateListOfParamsFromTmpFile ();
	    break;
	}
  }

/*****************************************************************************/
/**************** Create list of parameters from query string ****************/
/*****************************************************************************/

static void Par_CreateListOfParamsFromQueryString (void)
  {
   unsigned long CurPos;	// Current position in query string
   struct Param *Param = NULL;	// Initialized to avoid warning
   struct Param *NewParam;

   /***** Check if query string is empty *****/
   if (!Gbl.Params.QueryString)    return;
   if (!Gbl.Params.QueryString[0]) return;

   /***** Go over the query string
          getting start positions and lengths of parameters *****/
   for (CurPos = 0;
	CurPos < Gbl.Params.ContentLength;
	)
     {
      /* Allocate space for a new parameter initialized to 0 */
      if ((NewParam = (struct Param *) calloc (1,sizeof (struct Param))) == NULL)
	 Lay_ShowErrorAndExit ("Error allocating memory for parameter");

      /* Link the previous element in list with the current element */
      if (CurPos == 0)
	 Gbl.Params.List = NewParam;	// Pointer to first param
      else
	 Param->Next = NewParam;	// Pointer from former param to new param

      /* Make the current element to be the just created */
      Param = NewParam;

      /* Get parameter name */
      Param->Name.Start = CurPos;
      Param->Name.Length = strcspn (&Gbl.Params.QueryString[CurPos],"=");
      CurPos += Param->Name.Length;

      /* Get parameter value */
      if (CurPos < Gbl.Params.ContentLength)
	 if (Gbl.Params.QueryString[CurPos] == '=')
	   {
	    CurPos++;	// Skip '='
	    if (CurPos < Gbl.Params.ContentLength)
	      {
	       Param->Value.Start = CurPos;
	       Param->Value.Length = strcspn (&Gbl.Params.QueryString[CurPos],"&");
	       CurPos += Param->Value.Length;
	       if (CurPos < Gbl.Params.ContentLength)
		  if (Gbl.Params.QueryString[CurPos] == '&')
		     CurPos++;	// Skip '&'
	      }
	   }
     }
  }

/*****************************************************************************/
/*************** Create list of parameters from temporary file ***************/
/*****************************************************************************/
// TODO: Rename Gbl.F.Tmp to Gbl.F.In (InFile, QueryFile)?

#define Par_LENGTH_OF_STR_BEFORE_PARAM	38		// Length of "Content-Disposition: form-data; name=\""
#define Par_LENGTH_OF_STR_FILENAME	12		// Length of "; filename=\""
#define Par_LENGTH_OF_STR_CONTENT_TYPE	14		// Length of "Content-Type: "
#define Par_MAX_BYTES_STR_AUX		(38 + 1)	// Space to read any of the three preceding strings

static void Par_CreateListOfParamsFromTmpFile (void)
  {
   static const char *StringBeforeParam = "Content-Disposition: form-data; name=\"";
   static const char *StringFilename = "; filename=\"";
   static const char *StringContentType = "Content-Type: ";
   unsigned long CurPos;	// Current position in temporal file
   struct Param *Param = NULL;	// Initialized to avoid warning
   struct Param *NewParam;
   int Ch;
   char StrAux[Par_MAX_BYTES_STR_AUX + 1];

   /***** Go over the file
          getting start positions and lengths of parameters *****/
   if (Str_ReadFileUntilBoundaryStr (Gbl.F.Tmp,NULL,
                                     Gbl.Boundary.StrWithoutCRLF,
                                     Gbl.Boundary.LengthWithoutCRLF,
                                     Fil_MAX_FILE_SIZE) == 1)	// Delimiter string found

      for (CurPos = 0;
	   CurPos < Gbl.Params.ContentLength;
	   )
	{
	 /***** Skip \r\n after delimiter string *****/
	 if (fgetc (Gbl.F.Tmp) != 0x0D) break;	// '\r'
	 if (fgetc (Gbl.F.Tmp) != 0x0A) break;	// '\n'

	 Str_GetNextStrFromFileConvertingToLower (Gbl.F.Tmp,StrAux,
	                                          Par_LENGTH_OF_STR_BEFORE_PARAM);
	 if (!strcasecmp (StrAux,StringBeforeParam)) // Start of a parameter
	   {
	    /* Allocate space for a new parameter initialized to 0 */
	    if ((NewParam = (struct Param *) calloc (1,sizeof (struct Param))) == NULL)
	       Lay_ShowErrorAndExit ("Error allocating memory for parameter");

	    /* Link the previous element in list with the current element */
	    if (CurPos == 0)
	       Gbl.Params.List = NewParam;	// Pointer to first param
	    else
	       Param->Next = NewParam;	// Pointer from former param to new param

	    /* Make the current element to be the just created */
	    Param = NewParam;

	    /***** Get parameter name *****/
	    CurPos = (unsigned long) ftell (Gbl.F.Tmp);	// At start of parameter name
	    Param->Name.Start = CurPos;
	    Ch = Par_ReadTmpFileUntilQuote ();
	    CurPos = (unsigned long) ftell (Gbl.F.Tmp);	// Just after quote
	    Param->Name.Length = CurPos - 1 - Param->Name.Start;

	    /* Check if last character read after parameter name is a quote */
	    if (Ch != (int) '\"') break;		// '\"'

	    /* Get next char after parameter name */
	    Ch = fgetc (Gbl.F.Tmp);

	    /***** Check if filename is present *****/
	    if (Ch == (int) StringFilename[0])
	      {
	       Str_GetNextStrFromFileConvertingToLower (Gbl.F.Tmp,StrAux,
	                                                Par_LENGTH_OF_STR_FILENAME-1);
	       if (!strcasecmp (StrAux,StringFilename + 1))	// Start of filename
		 {
		  /* Get filename */
		  CurPos = (unsigned long) ftell (Gbl.F.Tmp);	// At start of filename
		  Param->FileName.Start = CurPos;
		  Ch = Par_ReadTmpFileUntilQuote ();
		  CurPos = (unsigned long) ftell (Gbl.F.Tmp);	// Just after quote
		  Param->FileName.Length = CurPos - 1 - Param->FileName.Start;

		  /* Check if last character read after filename is a quote */
		  if (Ch != (int) '\"') break;		// '\"'

		  /* Skip \r\n */
		  if (fgetc (Gbl.F.Tmp) != 0x0D) break;	// '\r'
		  if (fgetc (Gbl.F.Tmp) != 0x0A) break;	// '\n'

		  /* Check if Content-Type is present */
		  Str_GetNextStrFromFileConvertingToLower (Gbl.F.Tmp,StrAux,
		                                           Par_LENGTH_OF_STR_CONTENT_TYPE);
		  if (!strcasecmp (StrAux,StringContentType)) // Start of Content-Type
		    {
		     /* Get content type */
		     CurPos = (unsigned long) ftell (Gbl.F.Tmp);	// At start of content type
		     Param->ContentType.Start = CurPos;
		     Ch = Par_ReadTmpFileUntilReturn ();
		     CurPos = (unsigned long) ftell (Gbl.F.Tmp);	// Just after return
		     Param->ContentType.Length = CurPos - 1 - Param->ContentType.Start;
		    }
		 }
	      }

	    /***** Now \r\n\r\n is expected just before parameter value or file content *****/
	    /* Check if last character read is '\r' */
	    if (Ch != 0x0D) break;			// '\r'

	    /* Skip \n\r\n */
	    if (fgetc (Gbl.F.Tmp) != 0x0A) break;	// '\n'
	    if (fgetc (Gbl.F.Tmp) != 0x0D) break;	// '\r'
	    if (fgetc (Gbl.F.Tmp) != 0x0A) break;	// '\n'

	    /***** Get parameter value or file content *****/
	    CurPos = (unsigned long) ftell (Gbl.F.Tmp);	// At start of value or file content
	    if (Str_ReadFileUntilBoundaryStr (Gbl.F.Tmp,NULL,
					      Gbl.Boundary.StrWithCRLF,
					      Gbl.Boundary.LengthWithCRLF,
					      Fil_MAX_FILE_SIZE) != 1) break;	// Boundary string not found

	    // Delimiter string found
	    Param->Value.Start = CurPos;
	    CurPos = (unsigned long) ftell (Gbl.F.Tmp);	// Just after delimiter string
	    Param->Value.Length = CurPos - Gbl.Boundary.LengthWithCRLF -
		                  Param->Value.Start;
	   }
        }
  }

/*****************************************************************************/
/******************** Read from file until quote '\"' ************************/
/*****************************************************************************/
// Return last char read

static int Par_ReadTmpFileUntilQuote (void)
  {
   int Ch;

   /***** Read until quote if found *****/
   do
      Ch = fgetc (Gbl.F.Tmp);
   while (Ch != EOF && Ch != (int) '\"');

   return Ch;
  }

/*****************************************************************************/
/*************************** Read from file until \r *************************/
/*****************************************************************************/
// Return last char read

static int Par_ReadTmpFileUntilReturn (void)
  {
   int Ch;

   /***** Read until \r if found *****/
   do
      Ch = fgetc (Gbl.F.Tmp);
   while (Ch != EOF && Ch != 0x0D);	// '\r'

   return Ch;
  }

/*****************************************************************************/
/***************** Free memory allocated for query string ********************/
/*****************************************************************************/

void Par_FreeParams (void)
  {
   struct Param *Param;
   struct Param *NextParam;

   /***** Free list of parameters *****/
   for (Param = Gbl.Params.List;
	Param != NULL;
	Param = NextParam)
     {
      NextParam = Param->Next;
      free (Param);
     }

   /***** Free query string *****/
   if (Gbl.Params.QueryString)
      free (Gbl.Params.QueryString);
  }

/*****************************************************************************/
/************************* Get the value of a parameter **********************/
/*****************************************************************************/
// Return the number of parameters found
// If ParamPtr is not NULL, on return it will point to the first ocurrence in list of parameters
// ParamValue can be NULL (if so, no value is copied)

unsigned Par_GetParameter (tParamType ParamType,const char *ParamName,
                           char *ParamValue,size_t MaxBytes,
                           struct Param **ParamPtr)	// NULL if not used
  {
   extern const char *Par_SEPARATOR_PARAM_MULTIPLE;
   size_t BytesAlreadyCopied = 0;
   unsigned i;
   struct Param *Param;
   char *PtrDst;
   unsigned NumTimes;
   bool ParamFound = false;
   unsigned ParamNameLength;
   bool FindMoreThanOneOcurrence;
   char ErrorTxt[256];

   /***** Default values returned *****/
   if (ParamValue)
      ParamValue[0] = '\0'; // By default, the value of the parameter will be an empty string

   /***** Only some selected parameters can be passed by GET method *****/
   if (Gbl.Params.GetMethod)
      if (!Par_CheckIsParamCanBeUsedInGETMethod (ParamName))
	 return 0;	// Return no-parameters-found

   /***** Initializations *****/
   ParamNameLength = strlen (ParamName);
   PtrDst = ParamValue;
   FindMoreThanOneOcurrence = (ParamType == Par_PARAM_MULTIPLE);

   /***** For multiple parameters, loop for any ocurrence of the parameter
          For unique parameter, find only the first ocurrence *****/
   for (Param = Gbl.Params.List, NumTimes = 0;
	Param != NULL && (FindMoreThanOneOcurrence || NumTimes == 0);
	)
      /***** Find next ocurrence of parameter in list of parameters *****/
      for (ParamFound = false;
	   Param != NULL && !ParamFound;
	   Param = Param->Next)
	{
	 if (Param->Name.Length == ParamNameLength)
	   {
	    // The current element in the list has the length of the searched parameter
	    // Check if the name of the parameter is the same
	    switch (Gbl.ContentReceivedByCGI)
	      {
	       case Act_CONT_NORM:
		  ParamFound = !strncmp (ParamName,&Gbl.Params.QueryString[Param->Name.Start],
					 Param->Name.Length);
		  break;
	       case Act_CONT_DATA:
		  fseek (Gbl.F.Tmp,Param->Name.Start,SEEK_SET);
		  for (i = 0, ParamFound = true;
		       i < Param->Name.Length && ParamFound;
		       i++)
		     if (ParamName[i] != (char) fgetc (Gbl.F.Tmp))
			ParamFound = false;
		  break;
	      }

	    if (ParamFound)
	      {
	       NumTimes++;
	       if (NumTimes == 1)	// NumTimes == 1 ==> the first ocurrence of this parameter
		 {
		  /***** Get the first ocurrence of this parameter in list *****/
		  if (ParamPtr)
		     *ParamPtr = Param;

		  /***** If this parameter is a file ==> do not find more ocurrences ******/
		  if (Param->FileName.Start != 0)	// It's a file
		     FindMoreThanOneOcurrence = false;
		 }
	       else			// NumTimes > 1 ==> not the first ocurrence of this parameter
		 {
	          /***** Add separator when param multiple *****/
		  /* Check if there is space to copy separator */
		  if (BytesAlreadyCopied + 1 > MaxBytes)
		    {
		     snprintf (ErrorTxt,sizeof (ErrorTxt),
	                       "Multiple parameter <strong>%s</strong> too large,"
			       " it exceed the maximum allowed size (%lu bytes).",
			       ParamName,(unsigned long) MaxBytes);
		     Lay_ShowErrorAndExit (ErrorTxt);
		    }

		  /* Copy separator */
		  if (PtrDst)
		     *PtrDst++ = Par_SEPARATOR_PARAM_MULTIPLE[0];	// Separator in the destination string
		  BytesAlreadyCopied++;
		 }

	       /***** Copy parameter value *****/
	       if (Param->Value.Length)
		 {
		  /* Check if there is space to copy the parameter value */
		  if (BytesAlreadyCopied + Param->Value.Length > MaxBytes)
		    {
		     snprintf (ErrorTxt,sizeof (ErrorTxt),
	                       "Parameter <strong>%s</strong> too large,"
			       " it exceed the maximum allowed size (%lu bytes).",
			       ParamName,(unsigned long) MaxBytes);
		     Lay_ShowErrorAndExit (ErrorTxt);
		    }

		  /* Copy parameter value */
		  switch (Gbl.ContentReceivedByCGI)
		    {
		     case Act_CONT_NORM:
			if (PtrDst)
			   strncpy (PtrDst,&Gbl.Params.QueryString[Param->Value.Start],
				    Param->Value.Length);
			break;
		     case Act_CONT_DATA:
		        if (Param->FileName.Start == 0 &&	// Copy into destination only if it's not a file
		            PtrDst)
		          {
			   fseek (Gbl.F.Tmp,Param->Value.Start,SEEK_SET);
			   if (fread (PtrDst,sizeof (char),Param->Value.Length,Gbl.F.Tmp) !=
			       Param->Value.Length)
			      Lay_ShowErrorAndExit ("Error while getting value of parameter.");
		          }
			break;
		    }
		  BytesAlreadyCopied += Param->Value.Length;
		  if (PtrDst)
		     PtrDst += Param->Value.Length;
		 }
	      }
	   }
	}

   if (PtrDst)
      *PtrDst = '\0'; // Add the final NULL

   return NumTimes;
  }

/*****************************************************************************/
/*************** Check if parameter can be used in GET method ****************/
/*****************************************************************************/

static bool Par_CheckIsParamCanBeUsedInGETMethod (const char *ParamName)
  {
   static const char *ValidParamsInGETMethod[] =
     {
      "cty",	// To enter directly to a country
      "ins",	// To enter directly to an institution
      "ctr",	// To enter directly to a centre
      "deg",	// To enter directly to a degree
      "crs",	// To enter directly to a course
      "usr",	// To enter directly to a user's profile
      "agd",	// To view user's public agenda
      "act",	// To execute directly an action (allowed only for fully public actions)
      "ses",	// To use an open session when redirecting from one language to another
      "key",	// To verify an email address
     };
#define NUM_VALID_PARAMS (sizeof (ValidParamsInGETMethod) / sizeof (ValidParamsInGETMethod[0]))
   unsigned i;

   for (i = 0;
	i < NUM_VALID_PARAMS;
	i++)
      if (!strcmp (ParamName,ValidParamsInGETMethod[i]))
         return true;
   return false;
  }

/*****************************************************************************/
/****************** Get the parameters sent to this CGI **********************/
/*****************************************************************************/

void Par_GetMainParameters (void)
  {
   extern Act_Action_t Act_FromActCodToAction[1 + Act_MAX_ACTION_COD];
   extern const char *The_ThemeId[The_NUM_THEMES];
   extern const char *Ico_IconSetId[Ico_NUM_ICON_SETS];
   long ActCod;
   char Nickname[Nck_MAX_BYTES_NICKNAME_FROM_FORM + 1];
   char URL[PATH_MAX + 1];
   char LongStr[Cns_MAX_DECIMAL_DIGITS_LONG + 1];

   /***** Reset codes of country, institution, centre, degree and course *****/
   Gbl.Hierarchy.Cty.CtyCod =
   Gbl.Hierarchy.Ins.InsCod =
   Gbl.Hierarchy.Ctr.CtrCod =
   Gbl.Hierarchy.Deg.DegCod =
   Gbl.Hierarchy.Crs.CrsCod = -1L;

   // First of all, get action, and session identifier.
   // So, if other parameters have been stored in the database, there will be no problems to get them.

   /***** Get action to perform *****/
   if (Gbl.WebService.IsWebService)
     {
      Gbl.Action.Act = Gbl.Action.Original = ActWebSvc;
      Tab_SetCurrentTab ();
      return;
     }

   /***** Set default action *****/
   Gbl.Action.Act = Gbl.Action.Original = ActUnk;

   /***** Get another user's nickname, if exists
          (this nickname is used to go to another user's profile,
           not to get the logged user) *****/
   if (Par_GetParToText ("usr",Nickname,Nck_MAX_BYTES_NICKNAME_FROM_FORM))
     {
      if (Nickname[0])
	{
	 /* Set another user's nickname */
	 Str_RemoveLeadingArrobas (Nickname);
         Str_Copy (Gbl.Usrs.Other.UsrDat.Nickname,Nickname,	// without arroba
                   Nck_MAX_BYTES_NICKNAME_WITHOUT_ARROBA);

	 // This user's code is used to go to public profile
	 // and to refresh old publishings in user's timeline
	 // If user does not exist ==> UsrCod = -1
	 Gbl.Usrs.Other.UsrDat.UsrCod = Nck_GetUsrCodFromNickname (Gbl.Usrs.Other.UsrDat.Nickname);
         Gbl.Action.Act = Gbl.Action.Original = ActSeeOthPubPrf;	// Set default action if no other is specified
	}
     }
   else if (Par_GetParToText ("agd",Nickname,Nck_MAX_BYTES_NICKNAME_FROM_FORM))
     {
      if (Nickname[0])
	{
	 /* Set another user's nickname */
	 Str_RemoveLeadingArrobas (Nickname);
         Str_Copy (Gbl.Usrs.Other.UsrDat.Nickname,Nickname,	// without arroba
                   Nck_MAX_BYTES_NICKNAME_WITHOUT_ARROBA);

	 // This user's code is used to go to public agenda
	 // If user does not exist ==> UsrCod = -1
	 Gbl.Usrs.Other.UsrDat.UsrCod = Nck_GetUsrCodFromNickname (Gbl.Usrs.Other.UsrDat.Nickname);
	 Gbl.Action.Act = Gbl.Action.Original = ActFrmLogInUsrAgd;	// Set default action if no other is specified
	}
     }

   /***** Get action to perform *****/
   ActCod = Par_GetParToLong ("act");
   if (ActCod >= 0 && ActCod <= Act_MAX_ACTION_COD)
      Gbl.Action.Act = Gbl.Action.Original = Act_FromActCodToAction[ActCod];

   /***** Some preliminary adjusts depending on action *****/
   switch (Act_GetBrowserTab (Gbl.Action.Act))
     {
      case Act_AJAX_NORMAL:
	 Gbl.Action.UsesAJAX          = true;
	 Gbl.Action.IsAJAXAutoRefresh = false;
	 break;
      case Act_AJAX_RFRESH:
         Gbl.Action.UsesAJAX          = true;
         Gbl.Action.IsAJAXAutoRefresh = true;
         break;
      default:
	 Gbl.Action.UsesAJAX          = false;
	 Gbl.Action.IsAJAXAutoRefresh = false;
         break;
     }

   /***** Get session identifier, if exists *****/
   Par_GetParToText ("ses",Gbl.Session.Id,Cns_BYTES_SESSION_ID);
   if (Gbl.Session.Id[0])
     {
      /***** Get user's code, password, current degree and current course from stored session *****/
      if (Ses_GetSessionData ())
	 Gbl.Session.IsOpen = true;
      else
	{
	 Gbl.Session.HasBeenDisconnected = true;
	 Gbl.Session.Id[0] = '\0';
	}
     }
   else
     {
      // Try old parameter "IdSes" (allowed for compatibility, to be removed soon)
      Par_GetParToText ("IdSes",Gbl.Session.Id,Cns_BYTES_SESSION_ID);
      if (Gbl.Session.Id[0])
	{
	 /***** Get user's code, password, current degree and current course from stored session *****/
	 if (Ses_GetSessionData ())
	    Gbl.Session.IsOpen = true;
	 else
	   {
	    Gbl.Session.HasBeenDisconnected = true;
	    Gbl.Session.Id[0] = '\0';
	   }
	}
     }

   /***** Get user password and login *****/
   switch (Gbl.Action.Act)
     {
      case ActLogIn:
      case ActFrmLogInUsrAgd:
      case ActLogInUsrAgd:	// This action is necessary here when log in fails
         Pwd_GetParamUsrPwdLogin ();
	 /* falls through */
	 /* no break */
      case ActReqSndNewPwd:
      case ActSndNewPwd:
         Usr_GetParamUsrIdLogin ();
	 break;
     }

   /***** Try to get settings changed from current IP *****/
   Set_GetSettingsFromIP ();

   if (!Gbl.Session.IsOpen)	// When no session open (no logged user)...
     {
      /***** Try to get settings changed from current IP *****/
      if (Gbl.Prefs.Theme == The_THEME_UNKNOWN)
         Gbl.Prefs.Theme = The_THEME_DEFAULT;

      /***** Set path of theme *****/
      snprintf (URL,sizeof (URL),
	        "%s/%s",
                Cfg_URL_ICON_THEMES_PUBLIC,The_ThemeId[Gbl.Prefs.Theme]);
      Str_Copy (Gbl.Prefs.URLTheme,URL,
                PATH_MAX);

      /***** Set path of icon set *****/
      snprintf (URL,sizeof (URL),
	        "%s/%s",
                Cfg_URL_ICON_SETS_PUBLIC,Ico_IconSetId[Gbl.Prefs.IconSet]);
      Str_Copy (Gbl.Prefs.URLIconSet,URL,
                PATH_MAX);
     }

   /***** Get country if exists (from menu) *****/
   Par_GetParToText ("cty",LongStr,Cns_MAX_DECIMAL_DIGITS_LONG);
   if (LongStr[0])	// Parameter "cty" available
     {
      Gbl.Hierarchy.Cty.CtyCod = Str_ConvertStrCodToLongCod (LongStr);
      Gbl.Hierarchy.Ins.InsCod =
      Gbl.Hierarchy.Ctr.CtrCod =
      Gbl.Hierarchy.Deg.DegCod =
      Gbl.Hierarchy.Crs.CrsCod = -1L;
     }

   /***** Get institution if exists (from menu) *****/
   Par_GetParToText ("ins",LongStr,Cns_MAX_DECIMAL_DIGITS_LONG);
   if (LongStr[0])	// Parameter "ins" available
     {
      Gbl.Hierarchy.Ins.InsCod = Str_ConvertStrCodToLongCod (LongStr);
      Gbl.Hierarchy.Ctr.CtrCod =
      Gbl.Hierarchy.Deg.DegCod =
      Gbl.Hierarchy.Crs.CrsCod = -1L;
     }

   /***** Get centre if exists (from menu) *****/
   Par_GetParToText ("ctr",LongStr,Cns_MAX_DECIMAL_DIGITS_LONG);
   if (LongStr[0])	// Parameter "ctr" available
     {
      Gbl.Hierarchy.Ctr.CtrCod = Str_ConvertStrCodToLongCod (LongStr);
      Gbl.Hierarchy.Deg.DegCod =
      Gbl.Hierarchy.Crs.CrsCod = -1L;
     }

   /***** Get numerical degree code if exists (from menu) *****/
   Par_GetParToText ("deg",LongStr,Cns_MAX_DECIMAL_DIGITS_LONG);
   if (LongStr[0])	// Parameter "deg" available
     {
      Gbl.Hierarchy.Deg.DegCod = Str_ConvertStrCodToLongCod (LongStr);
      Gbl.Hierarchy.Crs.CrsCod = -1L;	// Reset possible course from session
     }

   /***** Get numerical course code if exists (from menu) *****/
   Par_GetParToText ("crs",LongStr,Cns_MAX_DECIMAL_DIGITS_LONG);
   if (LongStr[0])	// Parameter "crs" available
      Gbl.Hierarchy.Crs.CrsCod = Str_ConvertStrCodToLongCod (LongStr);	// Overwrite CrsCod from session

   /***** Get tab to activate *****/
   Gbl.Action.Tab = TabUnk;
   if (Gbl.Action.Act == ActMnu)
     {
      Gbl.Action.Tab = (Tab_Tab_t)
	               Par_GetParToUnsignedLong ("NxtTab",
                                                 (unsigned long) TabUnk,
                                                 Tab_NUM_TABS - 1,
                                                 (unsigned long) TabUnk);
      Tab_DisableIncompatibleTabs ();
     }
   else	// Set tab depending on current action
      Tab_SetCurrentTab ();
  }

/*****************************************************************************/
/******* Get the value of a single parameter and change format to text *******/
/*****************************************************************************/
// Return the number of parameters found

unsigned Par_GetParToText (const char *ParamName,char *ParamValue,size_t MaxBytes)
  {
   return Par_GetParAndChangeFormat (ParamName,ParamValue,MaxBytes,
                                     Str_TO_TEXT,true);
  }

/*****************************************************************************/
/****************** Get the unsigned value of a parameter ********************/
/*****************************************************************************/

unsigned long Par_GetParToUnsignedLong (const char *ParamName,
                                        unsigned long Min,
                                        unsigned long Max,
                                        unsigned long Default)
  {
   char UnsignedLongStr[Cns_MAX_DECIMAL_DIGITS_ULONG + 1];
   unsigned long UnsignedLongNum;

   /***** Get parameter with unsigned number *****/
   Par_GetParToText (ParamName,UnsignedLongStr,Cns_MAX_DECIMAL_DIGITS_ULONG);
   if (sscanf (UnsignedLongStr,"%lu",&UnsignedLongNum) == 1)
      if (UnsignedLongNum >= Min && UnsignedLongNum <= Max)
         return UnsignedLongNum;

   return Default;
  }

/*****************************************************************************/
/******************** Get the long value of a parameter **********************/
/*****************************************************************************/

long Par_GetParToLong (const char *ParamName)
  {
   char LongStr[Cns_MAX_DECIMAL_DIGITS_LONG + 1];

   /***** Get parameter with long number *****/
   Par_GetParToText (ParamName,LongStr,Cns_MAX_DECIMAL_DIGITS_LONG);
   return Str_ConvertStrCodToLongCod (LongStr);
  }

/*****************************************************************************/
/************************** Get a boolean parameter **************************/
/*****************************************************************************/

bool Par_GetParToBool (const char *ParamName)
  {
   char YN[1 + 1];

   /***** Get parameter "Y"/"N" and convert to boolean *****/
   Par_GetParToText (ParamName,YN,1);
   return (Str_ConvertToUpperLetter (YN[0]) == 'Y');
  }

/*****************************************************************************/
/*** Get value of a single parameter and change format to not rigorous HTML **/
/*****************************************************************************/
// Return the number of parameters found

unsigned Par_GetParToHTML (const char *ParamName,char *ParamValue,size_t MaxBytes)
  {
   return Par_GetParAndChangeFormat (ParamName,ParamValue,MaxBytes,
                                     Str_TO_HTML,true);
  }

/*****************************************************************************/
/****** Get the value of a multiple parameter and change format to text ******/
/*****************************************************************************/
// Return the number of parameters found

unsigned Par_GetParMultiToText (const char *ParamName,char *ParamValue,size_t MaxBytes)
  {
   unsigned NumTimes = Par_GetParameter (Par_PARAM_MULTIPLE,ParamName,
                                         ParamValue,MaxBytes,NULL);

   Str_ChangeFormat (Str_FROM_FORM,Str_TO_TEXT,
                     ParamValue,MaxBytes,true);
   return NumTimes;
  }

/*****************************************************************************/
/*********** Get the value of a single parameter and change format ***********/
/*****************************************************************************/
// Return the number of parameters found

unsigned Par_GetParAndChangeFormat (const char *ParamName,char *ParamValue,size_t MaxBytes,
                                    Str_ChangeTo_t ChangeTo,bool RemoveLeadingAndTrailingSpaces)
  {
   unsigned NumTimes = Par_GetParameter (Par_PARAM_SINGLE,ParamName,
                                         ParamValue,MaxBytes,NULL);

   Str_ChangeFormat (Str_FROM_FORM,ChangeTo,
                     ParamValue,MaxBytes,RemoveLeadingAndTrailingSpaces);
   return NumTimes;
  }

/*****************************************************************************/
/***** Search in the string StrSrc the next string until find separator ******/
/*****************************************************************************/
// Modifies *StrSrc
// When StrDst is NULL, nothing is stored
// Return true if characters found

bool Par_GetNextStrUntilSeparParamMult (const char **StrSrc,char *StrDst,size_t LongMax)
  {
   size_t i = 0;
   unsigned char Ch;	// Must be unsigned to work with characters > 127
   bool CharsFound = false;

   do
      if ((Ch = (unsigned char) **StrSrc))
	 (*StrSrc)++;
   while (Ch && Ch < 32);	// Skip special characters
				// (the separator will be a special character
				// less than 32)

   while (Ch >= 32)		// Until special character or end
     {
      CharsFound = true;
      if (i < LongMax)
         if (StrDst)
            StrDst[i++] = (char) Ch;
      if ((Ch = (unsigned char) **StrSrc))
	 (*StrSrc)++;
     }

   if (StrDst)
      StrDst[i] = '\0';

   return CharsFound;
  }

/*****************************************************************************/
/***** Search in the string StrSrc the next string until find separator ******/
/*****************************************************************************/
// Modifies *StrSrc
// When StrDst is NULL, nothing is stored
// Return true if characters found

bool Par_GetNextStrUntilComma (const char **StrSrc,char *StrDst,size_t LongMax)
  {
   size_t i = 0;
   unsigned char Ch;	// Must be unsigned to work with characters > 127
   bool CharsFound = false;

   do
      if ((Ch = (unsigned char) **StrSrc))
	 (*StrSrc)++;
   while (Ch == ',');	// Skip commas

   while (Ch && Ch != ',')		// Until special character or end
     {
      CharsFound = true;
      if (i < LongMax)
         if (StrDst)
            StrDst[i++] = (char) Ch;
      if ((Ch = (unsigned char) **StrSrc))
	 (*StrSrc)++;
     }

   if (StrDst)
      StrDst[i] = '\0';

   return CharsFound;
  }

/*****************************************************************************/
/******** Replace each comma by the separator of multiple parameters *********/
/*****************************************************************************/

void Par_ReplaceSeparatorMultipleByComma (char *Str)
  {
   for (;
	*Str;
	Str++)
      if ((unsigned char) *Str < 32)
	 *Str = ',';
  }

/*****************************************************************************/
/********************** Put an unsigned hidden parameter *********************/
/*****************************************************************************/

void Par_PutHiddenParamUnsigned (const char *Id,const char *ParamName,unsigned Value)
  {
   HTM_Txt ("<input type=\"hidden\"");
   if (Id)
      if (Id[0])
         HTM_TxtF (" id=\"%s\"",Id);
   HTM_TxtF (" name=\"%s\" value=\"%u\" />",ParamName,Value);
  }

void Par_PutHiddenParamUnsignedDisabled (const char *Id,const char *ParamName,unsigned Value)
  {
   HTM_Txt ("<input type=\"hidden\"");
   if (Id)
      if (Id[0])
         HTM_TxtF (" id=\"%s\"",Id);
   HTM_TxtF (" name=\"%s\" value=\"%u\" disabled=\"disabled\" />",
	     ParamName,Value);
  }

/*****************************************************************************/
/************************* Put a long hidden parameter ***********************/
/*****************************************************************************/

void Par_PutHiddenParamLong (const char *Id,const char *ParamName,long Value)
  {
   HTM_Txt ("<input type=\"hidden\"");
   if (Id)
      if (Id[0])
         HTM_TxtF (" id=\"%s\"",Id);
   HTM_TxtF (" name=\"%s\" value=\"%ld\" />",ParamName,Value);
  }

/*****************************************************************************/
/************************* Put a char hidden parameter ***********************/
/*****************************************************************************/

void Par_PutHiddenParamChar (const char *ParamName,char Value)
  {
   HTM_TxtF ("<input type=\"hidden\" name=\"%s\" value=\"%c\" />",
             ParamName,Value);
  }

/*****************************************************************************/
/************************ Put a string hidden parameter **********************/
/*****************************************************************************/

void Par_PutHiddenParamString (const char *Id,const char *ParamName,
                               const char *Value)
  {
   HTM_Txt ("<input type=\"hidden\"");
   if (Id)
      if (Id[0])
         HTM_TxtF (" id=\"%s\"",Id);
   HTM_TxtF (" name=\"%s\" value=\"%s\" />",
             ParamName,Value ? Value :
	                       "");
  }
