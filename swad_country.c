// swad_country.c: countries

/*
    SWAD (Shared Workspace At a Distance),
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
/********************************* Headers ***********************************/
/*****************************************************************************/

#define _GNU_SOURCE 		// For asprintf
#include <linux/stddef.h>	// For NULL
#include <math.h>		// For log10, ceil, pow...
#include <stdio.h>		// For asprintf
#include <stdlib.h>		// For calloc
#include <string.h>		// For string functions

#include "swad_box.h"
#include "swad_constant.h"
#include "swad_country.h"
#include "swad_database.h"
#include "swad_form.h"
#include "swad_global.h"
#include "swad_help.h"
#include "swad_institution.h"
#include "swad_language.h"
#include "swad_QR.h"
#include "swad_setting.h"
#include "swad_table.h"

/*****************************************************************************/
/************** External global variables from others modules ****************/
/*****************************************************************************/

extern struct Globals Gbl;

/*****************************************************************************/
/***************************** Private constants *****************************/
/*****************************************************************************/

/*****************************************************************************/
/******************************* Private types *******************************/
/*****************************************************************************/

/*****************************************************************************/
/***************************** Private variables *****************************/
/*****************************************************************************/

static struct Country *Cty_EditingCty = NULL;	// Static variable to keep the country being edited

/*****************************************************************************/
/***************************** Private prototypes ****************************/
/*****************************************************************************/

static void Cty_Configuration (bool PrintView);
static void Cty_PutIconToPrint (void);
static void Cty_ShowNumUsrsInCrssOfCty (Rol_Role_t Role);

static void Cty_PutHeadCountriesForSeeing (bool OrderSelectable);
static void Cty_ListOneCountryForSeeing (struct Country *Cty,unsigned NumCty);

static bool Cty_CheckIfICanEditCountries (void);

static void Cty_PutIconsListingCountries (void);
static void Cty_PutIconToEditCountries (void);

static unsigned Cty_GetNumUsrsWhoClaimToBelongToCty (long CtyCod);
static void Cty_GetParamCtyOrder (void);

static void Cty_EditCountriesInternal (void);
static void Cty_PutIconsEditingCountries (void);
static void Cty_PutIconToViewCountries (void);

static void Cty_GetMapAttribution (long CtyCod,char **MapAttribution);
static void Cty_FreeMapAttribution (char **MapAttribution);
static void Cty_ListCountriesForEdition (void);
static void Cty_PutParamOtherCtyCod (long CtyCod);
static long Cty_GetParamOtherCtyCod (void);

static bool Cty_CheckIfNumericCountryCodeExists (long CtyCod);
static bool Cty_CheckIfAlpha2CountryCodeExists (const char Alpha2[2 + 1]);
static bool Cty_CheckIfCountryNameExists (Lan_Language_t Language,const char *Name,long CtyCod);
static void Cty_UpdateCtyNameDB (long CtyCod,const char *FieldName,const char *NewCtyName);

static void Cty_ShowAlertAndButtonToGoToCty (void);
static void Cty_PutParamGoToCty (void);

static void Cty_PutFormToCreateCountry (void);
static void Cty_PutHeadCountriesForEdition (void);
static void Cty_CreateCountry (void);

static void Cty_EditingCountryConstructor (void);
static void Cty_EditingCountryDestructor (void);

/*****************************************************************************/
/***************** List countries with pending institutions ******************/
/*****************************************************************************/

void Cty_SeeCtyWithPendingInss (void)
  {
   extern const char *Hlp_SYSTEM_Hierarchy_pending;
   extern const char *Lan_STR_LANG_ID[1 + Lan_NUM_LANGUAGES];
   extern const char *Txt_Countries_with_pending_institutions;
   extern const char *Txt_Country;
   extern const char *Txt_Institutions_ABBREVIATION;
   extern const char *Txt_There_are_no_countries_with_requests_for_institutions_to_be_confirmed;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumCtys;
   unsigned NumCty;
   struct Country Cty;
   const char *BgColor;

   /***** Get countries with pending institutions *****/
   switch (Gbl.Usrs.Me.Role.Logged)
     {
      case Rol_SYS_ADM:
         NumCtys = (unsigned) DB_QuerySELECT (&mysql_res,"can not get countries"
						         "with pending institutions",
					      "SELECT institutions.CtyCod,COUNT(*)"
					      " FROM institutions,countries"
					      " WHERE (institutions.Status & %u)<>0"
					      " AND institutions.CtyCod=countries.CtyCod"
					      " GROUP BY institutions.CtyCod"
					      " ORDER BY countries.Name_%s",
					      (unsigned) Ins_STATUS_BIT_PENDING,
					      Lan_STR_LANG_ID[Gbl.Prefs.Language]);
         break;
      default:	// Forbidden for other users
	 return;
     }

   /***** Get countries *****/
   if (NumCtys)
     {
      /***** Start box and table *****/
      Box_StartBoxTable (NULL,Txt_Countries_with_pending_institutions,NULL,
                         Hlp_SYSTEM_Hierarchy_pending,Box_NOT_CLOSABLE,2);

      /***** Write heading *****/
      Tbl_TR_Begin (NULL);

      Tbl_TH (1,1,"LM",Txt_Country);
      Tbl_TH (1,1,"RM",Txt_Institutions_ABBREVIATION);

      Tbl_TR_End ();

      /***** List the countries *****/
      for (NumCty = 0;
	   NumCty < NumCtys;
	   NumCty++)
        {
         /* Get next country */
         row = mysql_fetch_row (mysql_res);

         /* Get country code (row[0]) */
         Cty.CtyCod = Str_ConvertStrCodToLongCod (row[0]);
         BgColor = (Cty.CtyCod == Gbl.Hierarchy.Cty.CtyCod) ? "LIGHT_BLUE" :
                                                              Gbl.ColorRows[Gbl.RowEvenOdd];

         /* Get data of country */
         Cty_GetDataOfCountryByCod (&Cty,Cty_GET_BASIC_DATA);

         Tbl_TR_Begin (NULL);

         /* Country map */
         Tbl_TD_Begin ("class=\"LM %s\"",BgColor);
         Cty_DrawCountryMapAndNameWithLink (&Cty,ActSeeIns,
                                            "COUNTRY_SMALL",
                                            "COUNTRY_MAP_SMALL",
                                            "DAT");
         Tbl_TD_End ();

         /* Number of pending institutions (row[1]) */
         Tbl_TD_Begin ("class=\"DAT RM %s\"",BgColor);
	 fprintf (Gbl.F.Out,"%s",row[1]);
         Tbl_TD_End ();

         Tbl_TR_End ();
         Gbl.RowEvenOdd = 1 - Gbl.RowEvenOdd;
        }

      /***** End table and box *****/
      Box_EndBoxTable ();
     }
   else
      Ale_ShowAlert (Ale_INFO,Txt_There_are_no_countries_with_requests_for_institutions_to_be_confirmed);

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/***************** Show information of the current country *******************/
/*****************************************************************************/

void Cty_ShowConfiguration (void)
  {
   Cty_Configuration (false);

   /***** Show help to enrol me *****/
   Hlp_ShowHelpWhatWouldYouLikeToDo ();
  }

/*****************************************************************************/
/***************** Print information of the current country ******************/
/*****************************************************************************/

void Cty_PrintConfiguration (void)
  {
   Cty_Configuration (true);
  }

/*****************************************************************************/
/******************** Information of the current country *********************/
/*****************************************************************************/

static void Cty_Configuration (bool PrintView)
  {
   extern const char *Hlp_COUNTRY_Information;
   extern const char *The_ClassFormInBox[The_NUM_THEMES];
   extern const char *Txt_Country;
   extern const char *Txt_Shortcut;
   extern const char *Lan_STR_LANG_ID[1 + Lan_NUM_LANGUAGES];
   extern const char *Txt_QR_code;
   extern const char *Txt_Institutions;
   extern const char *Txt_Institutions_of_COUNTRY_X;
   extern const char *Txt_Centres;
   extern const char *Txt_Degrees;
   extern const char *Txt_Courses;
   extern const char *Txt_Users_of_the_country;
   char *MapAttribution = NULL;
   bool PutLink;

   /***** Trivial check *****/
   if (Gbl.Hierarchy.Cty.CtyCod <= 0)		// No country selected
      return;

   /***** Start box *****/
   if (PrintView)
      Box_StartBox (NULL,NULL,NULL,
		    NULL,Box_NOT_CLOSABLE);
   else
      Box_StartBox (NULL,NULL,Cty_PutIconToPrint,
		    Hlp_COUNTRY_Information,Box_NOT_CLOSABLE);

   /***** Title *****/
   PutLink = !PrintView && Gbl.Hierarchy.Cty.WWW[Gbl.Prefs.Language][0];
   fprintf (Gbl.F.Out,"<div class=\"FRAME_TITLE FRAME_TITLE_BIG\">");
   if (PutLink)
      fprintf (Gbl.F.Out,"<a href=\"%s\" target=\"_blank\""
			 " class=\"FRAME_TITLE_BIG\" title=\"%s\">",
	       Gbl.Hierarchy.Cty.WWW[Gbl.Prefs.Language],
	       Gbl.Hierarchy.Cty.Name[Gbl.Prefs.Language]);
   fprintf (Gbl.F.Out,"%s",Gbl.Hierarchy.Cty.Name[Gbl.Prefs.Language]);
   if (PutLink)
      fprintf (Gbl.F.Out,"</a>");
   fprintf (Gbl.F.Out,"</div>");

   /***** Country map (and link to WWW if exists) *****/
   if (Cty_CheckIfCountryMapExists (&Gbl.Hierarchy.Cty))
     {
      /* Get map attribution */
      Cty_GetMapAttribution (Gbl.Hierarchy.Cty.CtyCod,&MapAttribution);

      /* Map image */
      fprintf (Gbl.F.Out,"<div class=\"DAT_SMALL CM\">");
      if (PutLink)
	 fprintf (Gbl.F.Out,"<a href=\"%s\" target=\"_blank\">",
		  Gbl.Hierarchy.Cty.WWW[Gbl.Prefs.Language]);
      Cty_DrawCountryMap (&Gbl.Hierarchy.Cty,PrintView ? "COUNTRY_MAP_PRINT" :
							  "COUNTRY_MAP_SHOW");
      if (PutLink)
	 fprintf (Gbl.F.Out,"</a>");
      fprintf (Gbl.F.Out,"</div>");

      /* Map attribution */
      if (!PrintView && Cty_CheckIfICanEditCountries ())
	{
	 fprintf (Gbl.F.Out,"<div class=\"CM\">");
	 Frm_StartForm (ActChgCtyMapAtt);
	 fprintf (Gbl.F.Out,"<textarea name=\"Attribution\""
			    " cols=\"50\" rows=\"2\""
			    " onchange=\"document.getElementById('%s').submit();\">",
		  Gbl.Form.Id);
	 if (MapAttribution)
	    fprintf (Gbl.F.Out,"%s",MapAttribution);
	 fprintf (Gbl.F.Out,"</textarea>");
	 Frm_EndForm ();
	 fprintf (Gbl.F.Out,"</div>");
	}
      else if (MapAttribution)
	 fprintf (Gbl.F.Out,"<div class=\"ATTRIBUTION\">"
			    "%s"
			    "</div>",
		  MapAttribution);

      /* Free memory used for map attribution */
      Cty_FreeMapAttribution (&MapAttribution);
     }

   /***** Start table *****/
   Tbl_TABLE_BeginWidePadding (2);

   /***** Country name (an link to WWW if exists) *****/
   Tbl_TR_Begin (NULL);

   Tbl_TD_Begin ("class=\"%s RM\"",The_ClassFormInBox[Gbl.Prefs.Theme]);
   fprintf (Gbl.F.Out,"%s:",Txt_Country);
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT_N LM\"");
   if (!PrintView && Gbl.Hierarchy.Cty.WWW[Gbl.Prefs.Language][0])
      fprintf (Gbl.F.Out,"<a href=\"%s\" target=\"_blank\" class=\"DAT_N\">",
	       Gbl.Hierarchy.Cty.WWW[Gbl.Prefs.Language]);
   fprintf (Gbl.F.Out,"%s",Gbl.Hierarchy.Cty.Name[Gbl.Prefs.Language]);
   if (!PrintView && Gbl.Hierarchy.Cty.WWW[Gbl.Prefs.Language][0])
      fprintf (Gbl.F.Out,"</a>");
   Tbl_TD_End ();

   Tbl_TR_End ();

   /***** Link to the country inside platform *****/
   Tbl_TR_Begin (NULL);

   Tbl_TD_Begin ("class=\"%s RM\"",The_ClassFormInBox[Gbl.Prefs.Theme]);
   fprintf (Gbl.F.Out,"%s:",Txt_Shortcut);
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT LM\"");
   fprintf (Gbl.F.Out,"<a href=\"%s/%s?cty=%ld\" class=\"DAT\" target=\"_blank\">"
		      "%s/%s?cty=%ld</a>",
	    Cfg_URL_SWAD_CGI,
	    Lan_STR_LANG_ID[Gbl.Prefs.Language],
	    Gbl.Hierarchy.Cty.CtyCod,
	    Cfg_URL_SWAD_CGI,
	    Lan_STR_LANG_ID[Gbl.Prefs.Language],
	    Gbl.Hierarchy.Cty.CtyCod);
   Tbl_TD_End ();

   Tbl_TR_End ();

   if (PrintView)
     {
      /***** QR code with link to the country *****/
      Tbl_TR_Begin (NULL);

      Tbl_TD_Begin ("class=\"%s RM\"",The_ClassFormInBox[Gbl.Prefs.Theme]);
      fprintf (Gbl.F.Out,"%s:",Txt_QR_code);
      Tbl_TD_End ();

      Tbl_TD_Begin ("class=\"DAT LM\"");
      QR_LinkTo (250,"cty",Gbl.Hierarchy.Cty.CtyCod);
      Tbl_TD_End ();

      Tbl_TR_End ();
     }
   else
     {
      /***** Number of users who claim to belong to this country *****/
      Tbl_TR_Begin (NULL);

      Tbl_TD_Begin ("class=\"%s RM\"",The_ClassFormInBox[Gbl.Prefs.Theme]);
      fprintf (Gbl.F.Out,"%s:",Txt_Users_of_the_country);
      Tbl_TD_End ();

      Tbl_TD_Begin ("class=\"DAT LM\"");
      fprintf (Gbl.F.Out,"%u",
	       Usr_GetNumUsrsWhoClaimToBelongToCty (Gbl.Hierarchy.Cty.CtyCod));
      Tbl_TD_End ();

      Tbl_TR_End ();

      /***** Number of institutions *****/
      Tbl_TR_Begin (NULL);

      Tbl_TD_Begin ("class=\"%s RM\"",The_ClassFormInBox[Gbl.Prefs.Theme]);
      fprintf (Gbl.F.Out,"%s:",Txt_Institutions);
      Tbl_TD_End ();

      /* Form to go to see institutions of this country */
      Tbl_TD_Begin ("class=\"LM\"");
      Frm_StartFormGoTo (ActSeeIns);
      Cty_PutParamCtyCod (Gbl.Hierarchy.Cty.CtyCod);
      snprintf (Gbl.Title,sizeof (Gbl.Title),
		Txt_Institutions_of_COUNTRY_X,
		Gbl.Hierarchy.Cty.Name[Gbl.Prefs.Language]);
      Frm_LinkFormSubmit (Gbl.Title,"DAT",NULL);
      fprintf (Gbl.F.Out,"%u</a>",
	       Ins_GetNumInssInCty (Gbl.Hierarchy.Cty.CtyCod));
      Frm_EndForm ();
      Tbl_TD_End ();

      Tbl_TR_End ();

      /***** Number of centres *****/
      Tbl_TR_Begin (NULL);

      Tbl_TD_Begin ("class=\"%s RM\"",The_ClassFormInBox[Gbl.Prefs.Theme]);
      fprintf (Gbl.F.Out,"%s:",Txt_Centres);
      Tbl_TD_End ();

      Tbl_TD_Begin ("class=\"DAT LM\"");
      fprintf (Gbl.F.Out,"%u",Ctr_GetNumCtrsInCty (Gbl.Hierarchy.Cty.CtyCod));
      Tbl_TD_End ();

      Tbl_TR_End ();

      /***** Number of degrees *****/
      Tbl_TR_Begin (NULL);

      Tbl_TD_Begin ("class=\"%s RM\"",The_ClassFormInBox[Gbl.Prefs.Theme]);
      fprintf (Gbl.F.Out,"%s:",
	       Txt_Degrees);
      Tbl_TD_End ();

      Tbl_TD_Begin ("class=\"DAT LM\"");
      fprintf (Gbl.F.Out,"%u",Deg_GetNumDegsInCty (Gbl.Hierarchy.Cty.CtyCod));
      Tbl_TD_End ();

      Tbl_TR_End ();

      /***** Number of courses *****/
      Tbl_TR_Begin (NULL);

      Tbl_TD_Begin ("class=\"%s RM\"",The_ClassFormInBox[Gbl.Prefs.Theme]);
      fprintf (Gbl.F.Out,"%s:",Txt_Courses);
      Tbl_TD_End ();

      Tbl_TD_Begin ("class=\"DAT LM\"");
      fprintf (Gbl.F.Out,"%u",Crs_GetNumCrssInCty (Gbl.Hierarchy.Cty.CtyCod));
      Tbl_TD_End ();

      Tbl_TR_End ();

      /***** Number of users in courses of this country *****/
      Cty_ShowNumUsrsInCrssOfCty (Rol_TCH);
      Cty_ShowNumUsrsInCrssOfCty (Rol_NET);
      Cty_ShowNumUsrsInCrssOfCty (Rol_STD);
      Cty_ShowNumUsrsInCrssOfCty (Rol_UNK);
     }

   /***** End table *****/
   Tbl_TABLE_End ();

   /***** End box *****/
   Box_EndBox ();
  }

/*****************************************************************************/
/************* Put icon to print the configuration of a country **************/
/*****************************************************************************/

static void Cty_PutIconToPrint (void)
  {
   Ico_PutContextualIconToPrint (ActPrnCtyInf,NULL);
  }

/*****************************************************************************/
/**************** Number of users in courses of this country *****************/
/*****************************************************************************/

static void Cty_ShowNumUsrsInCrssOfCty (Rol_Role_t Role)
  {
   extern const char *The_ClassFormInBox[The_NUM_THEMES];
   extern const char *Txt_Users_in_courses;
   extern const char *Txt_ROLES_PLURAL_Abc[Rol_NUM_ROLES][Usr_NUM_SEXS];

   Tbl_TR_Begin (NULL);

   Tbl_TD_Begin ("class=\"%s RM\"",The_ClassFormInBox[Gbl.Prefs.Theme]);
   fprintf (Gbl.F.Out,"%s:",
	    (Role == Rol_UNK) ? Txt_Users_in_courses :
		                Txt_ROLES_PLURAL_Abc[Role][Usr_SEX_UNKNOWN]);
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT LM\"");
   fprintf (Gbl.F.Out,"%u",
            Usr_GetNumUsrsInCrssOfCty (Role,Gbl.Hierarchy.Cty.CtyCod));
   Tbl_TD_End ();

   Tbl_TR_End ();
  }

/*****************************************************************************/
/*************************** List all the countries **************************/
/*****************************************************************************/

void Cty_ListCountries (void)
  {
   Cty_ListCountries1 ();
   Cty_ListCountries2 ();
  }

/*****************************************************************************/
/*************************** List all the countries **************************/
/*****************************************************************************/

void Cty_ListCountries1 (void)
  {
   /***** Get parameter with the type of order in the list of countries *****/
   Cty_GetParamCtyOrder ();

   /***** Get list of countries *****/
   Cty_GetListCountries (Cty_GET_EXTRA_DATA);
  }

void Cty_ListCountries2 (void)
  {
   extern const char *Hlp_SYSTEM_Countries;
   extern const char *Txt_Countries;
   extern const char *Txt_Other_countries;
   extern const char *Txt_Country_unspecified;
   unsigned NumCty;

   /***** Write menu to select country *****/
   Hie_WriteMenuHierarchy ();

   /***** Start box and table *****/
   Box_StartBoxTable (NULL,Txt_Countries,Cty_PutIconsListingCountries,
                      Hlp_SYSTEM_Countries,Box_NOT_CLOSABLE,2);

   /***** Write heading *****/
   Cty_PutHeadCountriesForSeeing (true);	// Order selectable

   /***** Write all the countries and their number of users and institutions *****/
   for (NumCty = 0;
	NumCty < Gbl.Hierarchy.Sys.Ctys.Num;
	NumCty++)
      Cty_ListOneCountryForSeeing (&Gbl.Hierarchy.Sys.Ctys.Lst[NumCty],NumCty + 1);

   /***** Separation row *****/
   Tbl_TR_Begin (NULL);
   Tbl_TD_Begin ("colspan=\"8\" class=\"DAT CM\"");
   fprintf (Gbl.F.Out,"&nbsp;");
   Tbl_TD_End ();
   Tbl_TR_End ();

   /***** Write users and institutions in other countries *****/
   Tbl_TR_Begin (NULL);

   Tbl_TD_Begin ("class=\"DAT RM\"");
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT LM\"");
   fprintf (Gbl.F.Out,"%s",Txt_Other_countries);
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT RM\"");
   fprintf (Gbl.F.Out,"%u",Cty_GetNumUsrsWhoClaimToBelongToCty (0));
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT RM\"");
   fprintf (Gbl.F.Out,"%u",Ins_GetNumInssInCty (0));
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT RM\"");
   fprintf (Gbl.F.Out,"%u",Ctr_GetNumCtrsInCty (0));
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT RM\"");
   fprintf (Gbl.F.Out,"%u",Deg_GetNumDegsInCty (0));
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT RM\"");
   fprintf (Gbl.F.Out,"%u",Crs_GetNumCrssInCty (0));
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT RM\"");
   fprintf (Gbl.F.Out,"%u",Usr_GetNumUsrsInCrssOfCty (Rol_TCH,0));
   Tbl_TD_End ();

   Tbl_TR_End ();

   /***** Write users and institutions with unknown country *****/
   Tbl_TR_Begin (NULL);

   Tbl_TD_Begin ("class=\"DAT RM\"");
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT LM\"");
   fprintf (Gbl.F.Out,"%s",Txt_Country_unspecified);
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT RM\"");
   fprintf (Gbl.F.Out,"%u",Cty_GetNumUsrsWhoClaimToBelongToCty (-1L));
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT RM\"");
   fprintf (Gbl.F.Out,"%u",Ins_GetNumInssInCty (-1L));
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT RM\"");
   fprintf (Gbl.F.Out,"%u",Ctr_GetNumCtrsInCty (-1L));
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT RM\"");
   fprintf (Gbl.F.Out,"%u",Deg_GetNumDegsInCty (-1L));
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT RM\"");
   fprintf (Gbl.F.Out,"%u",Crs_GetNumCrssInCty (-1L));
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT RM\"");
   fprintf (Gbl.F.Out,"0");
   Tbl_TD_End ();

   Tbl_TR_End ();

   /***** End table and box *****/
   Box_EndBoxTable ();

   /***** Div for Google Geochart *****/
   if (Gbl.Action.Act == ActSeeCty)
     {
      fprintf (Gbl.F.Out,"<div id=\"chart_div\""
	                 " style=\"width:600px; margin:12px auto;\">"
                         "</div>");
     }

   /***** Free list of countries *****/
   Cty_FreeListCountries ();
  }

/*****************************************************************************/
/******************* Write header with fields of a country *******************/
/*****************************************************************************/

static void Cty_PutHeadCountriesForSeeing (bool OrderSelectable)
  {
   extern const char *Txt_COUNTRIES_HELP_ORDER[2];
   extern const char *Txt_COUNTRIES_ORDER[2];
   extern const char *Txt_Institutions_ABBREVIATION;
   extern const char *Txt_Centres_ABBREVIATION;
   extern const char *Txt_Degrees_ABBREVIATION;
   extern const char *Txt_Courses_ABBREVIATION;
   extern const char *Txt_ROLES_PLURAL_BRIEF_Abc[Rol_NUM_ROLES];
   Cty_Order_t Order;

   Tbl_TR_Begin (NULL);

   Tbl_TH_Empty (1);
   for (Order = Cty_ORDER_BY_COUNTRY;
	Order <= Cty_ORDER_BY_NUM_USRS;
	Order++)
     {
      Tbl_TH_Begin (1,1,Order == Cty_ORDER_BY_COUNTRY ? "LM" :
						        "RM");
      if (OrderSelectable)
	{
	 Frm_StartForm (ActSeeCty);
	 Par_PutHiddenParamUnsigned ("Order",(unsigned) Order);
	 Frm_LinkFormSubmit (Txt_COUNTRIES_HELP_ORDER[Order],"TIT_TBL",NULL);
	 if (Order == Gbl.Hierarchy.Sys.Ctys.SelectedOrder)
	    fprintf (Gbl.F.Out,"<u>");
	}
      fprintf (Gbl.F.Out,"%s",Txt_COUNTRIES_ORDER[Order]);
      if (OrderSelectable)
	{
	 if (Order == Gbl.Hierarchy.Sys.Ctys.SelectedOrder)
	    fprintf (Gbl.F.Out,"</u>");
	 fprintf (Gbl.F.Out,"</a>");
	 Frm_EndForm ();
	}
      Tbl_TH_End ();
     }

   Tbl_TH (1,1,"RM",Txt_Institutions_ABBREVIATION);
   Tbl_TH (1,1,"RM",Txt_Centres_ABBREVIATION);
   Tbl_TH (1,1,"RM",Txt_Degrees_ABBREVIATION);
   Tbl_TH (1,1,"RM",Txt_Courses_ABBREVIATION);
   Tbl_TH_Begin (1,1,"RM");
   fprintf (Gbl.F.Out,"%s+<br />%s",
	    Txt_ROLES_PLURAL_BRIEF_Abc[Rol_TCH],
            Txt_ROLES_PLURAL_BRIEF_Abc[Rol_STD]);
   Tbl_TH_End ();

   Tbl_TR_End ();
  }

/*****************************************************************************/
/************************ List one country for seeing ************************/
/*****************************************************************************/

static void Cty_ListOneCountryForSeeing (struct Country *Cty,unsigned NumCty)
  {
   const char *BgColor;

   BgColor = (Cty->CtyCod == Gbl.Hierarchy.Cty.CtyCod) ? "LIGHT_BLUE" :
							 Gbl.ColorRows[Gbl.RowEvenOdd];

   Tbl_TR_Begin (NULL);

   /***** Number of country in this list *****/
   Tbl_TD_Begin ("class=\"DAT RM %s\"",BgColor);
   fprintf (Gbl.F.Out,"%u",NumCty);
   Tbl_TD_End ();

   /***** Country map (and link to WWW if exists) *****/
   Tbl_TD_Begin ("class=\"LM %s\"",BgColor);
   Cty_DrawCountryMapAndNameWithLink (Cty,ActSeeIns,
				      "COUNTRY_SMALL",
				      "COUNTRY_MAP_SMALL",
				      "DAT_N");
   Tbl_TD_End ();

   /* Write stats of this country */
   Tbl_TD_Begin ("class=\"DAT RM %s\"",BgColor);
   fprintf (Gbl.F.Out,"%u",Cty->NumUsrsWhoClaimToBelongToCty);
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT RM %s\"",BgColor);
   fprintf (Gbl.F.Out,"%u",Cty->Inss.Num);
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT RM %s\"",BgColor);
   fprintf (Gbl.F.Out,"%u",Cty->NumCtrs);
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT RM %s\"",BgColor);
   fprintf (Gbl.F.Out,"%u",Cty->NumDegs);
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT RM %s\"",BgColor);
   fprintf (Gbl.F.Out,"%u",Cty->NumCrss);
   Tbl_TD_End ();

   Tbl_TD_Begin ("class=\"DAT RM %s\"",BgColor);
   fprintf (Gbl.F.Out,"%u",Cty->NumUsrs);
   Tbl_TD_End ();

   Tbl_TR_End ();

   Gbl.RowEvenOdd = 1 - Gbl.RowEvenOdd;
  }

/*****************************************************************************/
/********************** Check if I can edit countries ************************/
/*****************************************************************************/

static bool Cty_CheckIfICanEditCountries (void)
  {
   return (bool) (Gbl.Usrs.Me.Role.Logged == Rol_SYS_ADM);
  }

/*****************************************************************************/
/***************** Put contextual icons in list of countries *****************/
/*****************************************************************************/

static void Cty_PutIconsListingCountries (void)
  {
   /***** Put icon to edit countries *****/
   if (Cty_CheckIfICanEditCountries ())
      Cty_PutIconToEditCountries ();

   /***** Put icon to show a figure *****/
   Gbl.Figures.FigureType = Fig_HIERARCHY;
   Fig_PutIconToShowFigure ();
  }

/*****************************************************************************/
/************************ Put icon to edit countries *************************/
/*****************************************************************************/

static void Cty_PutIconToEditCountries (void)
  {
   Ico_PutContextualIconToEdit (ActEdiCty,NULL);
  }

/*****************************************************************************/
/******** Get number of users who claim to belong to other countries *********/
/*****************************************************************************/

static unsigned Cty_GetNumUsrsWhoClaimToBelongToCty (long CtyCod)
  {
   /***** Get number of users from database *****/
   return
   (unsigned) DB_QueryCOUNT ("can not get number of users"
			     " who claim to belong to other countries",
			     "SELECT COUNT(*) FROM usr_data"
			     " WHERE CtyCod=%ld",
			     CtyCod);
  }

/*****************************************************************************/
/********************* Draw country map and name with link *******************/
/*****************************************************************************/

void Cty_DrawCountryMapAndNameWithLink (struct Country *Cty,Act_Action_t Action,
                                        const char *ClassContainer,
                                        const char *ClassMap,
                                        const char *ClassLink)
  {
   extern const char *Txt_Go_to_X;
   char CountryName[Cty_MAX_BYTES_NAME + 1];

   /***** Start form *****/
   Frm_StartFormGoTo (Action);
   Cty_PutParamCtyCod (Cty->CtyCod);
   fprintf (Gbl.F.Out,"<div class=\"%s\">",ClassContainer);

   /***** Link to action *****/
   snprintf (Gbl.Title,sizeof (Gbl.Title),
	     Txt_Go_to_X,
	     Cty->Name[Gbl.Prefs.Language]);
   Frm_LinkFormSubmit (Gbl.Title,ClassLink,NULL);

   /***** Draw country map *****/
   Cty_DrawCountryMap (Cty,ClassMap);

   /***** Write country name and end link *****/
   Str_Copy (CountryName,Cty->Name[Gbl.Prefs.Language],
             Cty_MAX_BYTES_NAME);
   fprintf (Gbl.F.Out,"&nbsp;%s&nbsp;(%s)"
	              "</a>"
	              "</div>",
	    CountryName,
	    Cty->Alpha2);

   /***** End form *****/
   Frm_EndForm ();
  }

/*****************************************************************************/
/***************************** Draw country map ******************************/
/*****************************************************************************/

void Cty_DrawCountryMap (struct Country *Cty,const char *Class)
  {
   /***** Draw country map *****/
   fprintf (Gbl.F.Out,"<img src=\"");
   if (Cty_CheckIfCountryMapExists (Cty))
      fprintf (Gbl.F.Out,"%s/%s/%s.png",
	       Cfg_URL_ICON_COUNTRIES_PUBLIC,
	       Cty->Alpha2,
	       Cty->Alpha2);
   else
      fprintf (Gbl.F.Out,"%s/tr16x16.gif",	// TODO: Change for a 1x1 image or a generic image
	       Cfg_URL_ICON_PUBLIC);
   fprintf (Gbl.F.Out,"\" alt=\"%s\" title=\"%s\" class=\"%s\" />",
	    Cty->Alpha2,
	    Cty->Name[Gbl.Prefs.Language],
	    Class);
  }

/*****************************************************************************/
/*********************** Check if country map exists *************************/
/*****************************************************************************/

bool Cty_CheckIfCountryMapExists (struct Country *Cty)
  {
   char PathMap[PATH_MAX + 1];

   snprintf (PathMap,sizeof (PathMap),
	     "%s/%s/%s.png",
	     Cfg_PATH_ICON_COUNTRIES_PUBLIC,
	     Cty->Alpha2,
	     Cty->Alpha2);
   return Fil_CheckIfPathExists (PathMap);
  }

/*****************************************************************************/
/********************** Write script for Google Geochart *********************/
/*****************************************************************************/

void Cty_WriteScriptGoogleGeochart (void)
  {
   extern const char *Txt_Country_NO_HTML;
   extern const char *Txt_Users_NO_HTML;
   extern const char *Txt_Institutions_NO_HTML;
   unsigned NumCty;
   unsigned MaxUsrsInCountry = 0;
   unsigned NumCtysWithUsrs = 0;

   /***** Write start of the script *****/
   fprintf (Gbl.F.Out,"<script type=\"text/javascript\" src=\"https://www.google.com/jsapi\"></script>\n"
                      "<script type=\"text/javascript\">\n"
                      "	google.load('visualization', '1', {'packages': ['geochart']});\n"
                      "	google.setOnLoadCallback(drawRegionsMap);\n"
                      "	function drawRegionsMap() {\n"
                      "	var data = new google.visualization.DataTable();\n"
                      "	data.addColumn('string', '%s');\n"
                      "	data.addColumn('number', '%s');\n"
                      "	data.addColumn('number', '%s');\n"
                      "	data.addRows([\n",
            Txt_Country_NO_HTML,
            Txt_Users_NO_HTML,
            Txt_Institutions_NO_HTML);

   /***** Write all the countries and their number of users and institutions *****/
   for (NumCty = 0;
	NumCty < Gbl.Hierarchy.Sys.Ctys.Num;
	NumCty++)
      if (Gbl.Hierarchy.Sys.Ctys.Lst[NumCty].NumUsrsWhoClaimToBelongToCty)
        {
         /* Write data of this country */
         fprintf (Gbl.F.Out,"	['%s', %u, %u],\n",
                  Gbl.Hierarchy.Sys.Ctys.Lst[NumCty].Alpha2,
                  Gbl.Hierarchy.Sys.Ctys.Lst[NumCty].NumUsrsWhoClaimToBelongToCty,
                  Gbl.Hierarchy.Sys.Ctys.Lst[NumCty].Inss.Num);
         if (Gbl.Hierarchy.Sys.Ctys.Lst[NumCty].NumUsrsWhoClaimToBelongToCty > MaxUsrsInCountry)
            MaxUsrsInCountry = Gbl.Hierarchy.Sys.Ctys.Lst[NumCty].NumUsrsWhoClaimToBelongToCty;
         NumCtysWithUsrs++;
        }

   /***** Write end of the script *****/
   fprintf (Gbl.F.Out,"	]);\n"
                      "	var options = {\n"
                      "		width:600,\n"
                      "		height:360,\n"
                      "		backgroundColor:'white',\n"
                      "		datalessRegionColor:'white',\n"
                      "		colorAxis:{colors:['#EAF1F4','#4D88A1'],minValue:0,maxValue:%u}};\n"
                      "	var chart = new google.visualization.GeoChart(document.getElementById('chart_div'));\n"
                      "	chart.draw(data, options);\n"
                      "	};\n"
                      "</script>\n",
            NumCtysWithUsrs ? MaxUsrsInCountry :
        	              0);
  }

/*****************************************************************************/
/******** Get parameter with the type or order in list of countries **********/
/*****************************************************************************/

static void Cty_GetParamCtyOrder (void)
  {
   Gbl.Hierarchy.Sys.Ctys.SelectedOrder = (Cty_Order_t)
	                    Par_GetParToUnsignedLong ("Order",
                                                      0,
                                                      Cty_NUM_ORDERS - 1,
                                                      (unsigned long) Cty_ORDER_DEFAULT);
  }

/*****************************************************************************/
/******************** Put forms to edit institution types ********************/
/*****************************************************************************/

void Cty_EditCountries (void)
  {
   /***** Country constructor *****/
   Cty_EditingCountryConstructor ();

   /***** Edit countries *****/
   Cty_EditCountriesInternal ();

   /***** Country destructor *****/
   Cty_EditingCountryDestructor ();
  }

static void Cty_EditCountriesInternal (void)
  {
   extern const char *Hlp_SYSTEM_Countries;
   extern const char *Txt_Countries;

   /***** Get list of countries *****/
   Gbl.Hierarchy.Sys.Ctys.SelectedOrder = Cty_ORDER_BY_COUNTRY;
   Cty_GetListCountries (Cty_GET_EXTRA_DATA);

   /***** Write menu to select country *****/
   Hie_WriteMenuHierarchy ();

   /***** Start box *****/
   Box_StartBox (NULL,Txt_Countries,Cty_PutIconsEditingCountries,
                 Hlp_SYSTEM_Countries,Box_NOT_CLOSABLE);

   /***** Put a form to create a new country *****/
   Cty_PutFormToCreateCountry ();

   /***** Forms to edit current countries *****/
   if (Gbl.Hierarchy.Sys.Ctys.Num)
      Cty_ListCountriesForEdition ();

   /***** End box *****/
   Box_EndBox ();

   /***** Free list of countries *****/
   Cty_FreeListCountries ();
  }

/*****************************************************************************/
/*************** Put contextual icons in edition of countries ****************/
/*****************************************************************************/

static void Cty_PutIconsEditingCountries (void)
  {
   /***** Put icon to view countries *****/
   Cty_PutIconToViewCountries ();

   /***** Put icon to show a figure *****/
   Gbl.Figures.FigureType = Fig_HIERARCHY;
   Fig_PutIconToShowFigure ();
  }

/*****************************************************************************/
/************************ Put icon to view countries *************************/
/*****************************************************************************/

static void Cty_PutIconToViewCountries (void)
  {
   extern const char *Txt_Countries;

   Lay_PutContextualLinkOnlyIcon (ActSeeCty,NULL,NULL,
                                  "globe.svg",
                                  Txt_Countries);
  }

/*****************************************************************************/
/************************** List all the countries ***************************/
/*****************************************************************************/

#define Cty_MAX_BYTES_SUBQUERY_CTYS	((1 + Lan_NUM_LANGUAGES) * 32)

void Cty_GetListCountries (Cty_GetExtraData_t GetExtraData)
  {
   extern const char *Lan_STR_LANG_ID[1 + Lan_NUM_LANGUAGES];
   char StrField[32];
   char SubQueryNam1[Cty_MAX_BYTES_SUBQUERY_CTYS + 1];
   char SubQueryNam2[Cty_MAX_BYTES_SUBQUERY_CTYS + 1];
   char SubQueryWWW1[Cty_MAX_BYTES_SUBQUERY_CTYS + 1];
   char SubQueryWWW2[Cty_MAX_BYTES_SUBQUERY_CTYS + 1];
   char *OrderBySubQuery = NULL;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows = 0;
   unsigned NumCty;
   struct Country *Cty;
   Lan_Language_t Lan;

   /***** Get countries from database *****/
   switch (GetExtraData)
     {
      case Cty_GET_BASIC_DATA:
         NumRows = DB_QuerySELECT (&mysql_res,"can not get countries",
				   "SELECT CtyCod,Alpha2,Name_%s"
				   " FROM countries ORDER BY Name_%s",
				   Lan_STR_LANG_ID[Gbl.Prefs.Language],
				   Lan_STR_LANG_ID[Gbl.Prefs.Language]);
         break;
      case Cty_GET_EXTRA_DATA:
         SubQueryNam1[0] = '\0';
         SubQueryNam2[0] = '\0';
         SubQueryWWW1[0] = '\0';
         SubQueryWWW2[0] = '\0';
         for (Lan = (Lan_Language_t) 1;
              Lan <= Lan_NUM_LANGUAGES;
              Lan++)
           {
            snprintf (StrField,sizeof (StrField),
        	      "countries.Name_%s,",
        	      Lan_STR_LANG_ID[Lan]);
            Str_Concat (SubQueryNam1,StrField,
                        Cty_MAX_BYTES_SUBQUERY_CTYS);
            snprintf (StrField,sizeof (StrField),
        	      "Name_%s,",
        	      Lan_STR_LANG_ID[Lan]);
            Str_Concat (SubQueryNam2,StrField,
                        Cty_MAX_BYTES_SUBQUERY_CTYS);

            snprintf (StrField,sizeof (StrField),
        	      "countries.WWW_%s,",
        	      Lan_STR_LANG_ID[Lan]);
            Str_Concat (SubQueryWWW1,StrField,
                        Cty_MAX_BYTES_SUBQUERY_CTYS);
            snprintf (StrField,sizeof (StrField),
        	      "WWW_%s,",
        	      Lan_STR_LANG_ID[Lan]);
            Str_Concat (SubQueryWWW2,StrField,
                        Cty_MAX_BYTES_SUBQUERY_CTYS);
           }

         switch (Gbl.Hierarchy.Sys.Ctys.SelectedOrder)
           {
            case Cty_ORDER_BY_COUNTRY:
               if (asprintf (&OrderBySubQuery,"Name_%s",
        	             Lan_STR_LANG_ID[Gbl.Prefs.Language]) < 0)
	          Lay_NotEnoughMemoryExit ();
               break;
            case Cty_ORDER_BY_NUM_USRS:
               if (asprintf (&OrderBySubQuery,"NumUsrs DESC,Name_%s",
        	             Lan_STR_LANG_ID[Gbl.Prefs.Language]) < 0)
	          Lay_NotEnoughMemoryExit ();
               break;
           }

         NumRows = DB_QuerySELECT (&mysql_res,"can not get countries",
				   "(SELECT countries.CtyCod,countries.Alpha2,"
				   "%s%sCOUNT(*) AS NumUsrs"
				   " FROM countries,usr_data"
				   " WHERE countries.CtyCod=usr_data.CtyCod"
				   " GROUP BY countries.CtyCod)"
				   " UNION "
				   "(SELECT CtyCod,Alpha2,%s%s0 AS NumUsrs"
				   " FROM countries"
				   " WHERE CtyCod NOT IN"
				   " (SELECT DISTINCT CtyCod FROM usr_data))"
				   " ORDER BY %s",
				   SubQueryNam1,SubQueryWWW1,
				   SubQueryNam2,SubQueryWWW2,OrderBySubQuery);
         break;
     }

   /***** Free memory for subquery *****/
   if (OrderBySubQuery)
      free ((void *) OrderBySubQuery);

   if (NumRows) // Countries found...
     {
      Gbl.Hierarchy.Sys.Ctys.Num = (unsigned) NumRows;

      /***** Create list with countries *****/
      if ((Gbl.Hierarchy.Sys.Ctys.Lst = (struct Country *) calloc (NumRows,sizeof (struct Country))) == NULL)
         Lay_NotEnoughMemoryExit ();

      /***** Get the countries *****/
      for (NumCty = 0;
	   NumCty < Gbl.Hierarchy.Sys.Ctys.Num;
	   NumCty++)
        {
         Cty = &(Gbl.Hierarchy.Sys.Ctys.Lst[NumCty]);

         /* Get next country */
         row = mysql_fetch_row (mysql_res);

         /* Get numerical country code (row[0]) */
         if ((Cty->CtyCod = Str_ConvertStrCodToLongCod (row[0])) < 0)
            Lay_ShowErrorAndExit ("Wrong code of country.");

         /* Get Alpha-2 country code (row[1]) */
         Str_Copy (Cty->Alpha2,row[1],
                   2);

         switch (GetExtraData)
           {
            case Cty_GET_BASIC_DATA:
               for (Lan = (Lan_Language_t) 1;
        	    Lan <= Lan_NUM_LANGUAGES;
        	    Lan++)
        	 {
                  Cty->Name[Lan][0] = '\0';
                  Cty->WWW[Lan][0] = '\0';
        	 }
               Cty->NumUsrsWhoClaimToBelongToCty = 0;
               Cty->Inss.Num = Cty->NumCtrs = Cty->NumDegs = Cty->NumCrss = 0;
               Cty->NumUsrs = 0;

               /* Get the name of the country in current language */
               Str_Copy (Cty->Name[Gbl.Prefs.Language],row[2],
                         Cty_MAX_BYTES_NAME);
               break;
            case Cty_GET_EXTRA_DATA:
               /* Get the name of the country in several languages */
               for (Lan = (Lan_Language_t) 1;
        	    Lan <= Lan_NUM_LANGUAGES;
        	    Lan++)
        	 {
                  Str_Copy (Cty->Name[Lan],row[1 + Lan],
                            Cty_MAX_BYTES_NAME);
                  Str_Copy (Cty->WWW[Lan],row[1 + Lan_NUM_LANGUAGES + Lan],
                            Cns_MAX_BYTES_WWW);
        	 }

               /* Get number of users who claim to belong to this country */
               if (sscanf (row[1 + Lan_NUM_LANGUAGES * 2 + 1],"%u",
                           &Cty->NumUsrsWhoClaimToBelongToCty) != 1)
                  Cty->NumUsrsWhoClaimToBelongToCty = 0;

               /* Get number of institutions in this country */
               Cty->Inss.Num = Ins_GetNumInssInCty (Cty->CtyCod);

               /* Get number of centres in this country */
               Cty->NumCtrs = Ctr_GetNumCtrsInCty (Cty->CtyCod);

               /* Get number of degrees in this country */
               Cty->NumDegs = Deg_GetNumDegsInCty (Cty->CtyCod);

               /* Get number of courses in this country */
               Cty->NumCrss = Crs_GetNumCrssInCty (Cty->CtyCod);

               /* Get number of users in courses of this country */
               Cty->NumUsrs = Usr_GetNumUsrsInCrssOfCty (Rol_UNK,Cty->CtyCod);	// Here Rol_UNK means "all users"
               break;
           }
        }
     }
   else
      Gbl.Hierarchy.Sys.Ctys.Num = 0;

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/************************** Write selector of country ************************/
/*****************************************************************************/

void Cty_WriteSelectorOfCountry (void)
  {
   extern const char *Txt_Country;
   extern const char *Lan_STR_LANG_ID[1 + Lan_NUM_LANGUAGES];
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumCtys;
   unsigned NumCty;
   long CtyCod;

   /***** Start form *****/
   Frm_StartFormGoTo (ActSeeIns);
   fprintf (Gbl.F.Out,"<select id=\"cty\" name=\"cty\" style=\"width:175px;\""
                      " onchange=\"document.getElementById('%s').submit();\">"
                      "<option value=\"\"",
	    Gbl.Form.Id);
   if (Gbl.Hierarchy.Cty.CtyCod < 0)
      fprintf (Gbl.F.Out," selected=\"selected\"");
   fprintf (Gbl.F.Out," disabled=\"disabled\">[%s]</option>",
            Txt_Country);

   /***** Get countries from database *****/
   NumCtys = (unsigned) DB_QuerySELECT (&mysql_res,"can not get countries",
				        "SELECT DISTINCT CtyCod,Name_%s"
					" FROM countries"
					" ORDER BY countries.Name_%s",
					Lan_STR_LANG_ID[Gbl.Prefs.Language],
					Lan_STR_LANG_ID[Gbl.Prefs.Language]);

   /***** List countries *****/
   for (NumCty = 0;
	NumCty < NumCtys;
	NumCty++)
     {
      /* Get next country */
      row = mysql_fetch_row (mysql_res);

      /* Get country code (row[0]) */
      if ((CtyCod = Str_ConvertStrCodToLongCod (row[0])) < 0)
         Lay_ShowErrorAndExit ("Wrong code of country.");

      /* Write option */
      fprintf (Gbl.F.Out,"<option value=\"%ld\"",CtyCod);
      if (CtyCod == Gbl.Hierarchy.Cty.CtyCod)
	 fprintf (Gbl.F.Out," selected=\"selected\"");
      fprintf (Gbl.F.Out,">%s</option>",row[1]);
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   /***** End form *****/
   fprintf (Gbl.F.Out,"</select>");
   Frm_EndForm ();
  }

/*****************************************************************************/
/***************************** Write country name ****************************/
/*****************************************************************************/
// If ClassLink == NULL ==> do not put link

void Cty_WriteCountryName (long CtyCod,const char *ClassLink)
  {
   char CtyName[Cty_MAX_BYTES_NAME + 1];
   char ActTxt[Act_MAX_BYTES_ACTION_TXT + 1];
   bool PutForm = ClassLink &&
	          !Gbl.Form.Inside &&						// Only if not inside another form
                  Act_GetBrowserTab (Gbl.Action.Act) == Act_BRW_1ST_TAB;	// Only in main browser tab

   /***** Get country name *****/
   Cty_GetCountryName (CtyCod,CtyName);

   if (PutForm)
     {
      /***** Write country name with link to country information *****/
      Frm_StartForm (ActSeeCtyInf);
      Cty_PutParamCtyCod (CtyCod);
      Frm_LinkFormSubmit (Act_GetActionTextFromDB (Act_GetActCod (ActSeeCtyInf),ActTxt),
		          ClassLink,NULL);
      fprintf (Gbl.F.Out,"%s</a>",CtyName);
      Frm_EndForm ();
     }
   else
      /***** Write country name without link *****/
      fprintf (Gbl.F.Out,"%s",CtyName);
  }

/*****************************************************************************/
/***************** Get basic data of country given its code ******************/
/*****************************************************************************/

bool Cty_GetDataOfCountryByCod (struct Country *Cty,Cty_GetExtraData_t GetExtraData)
  {
   extern const char *Txt_Another_country;
   extern const char *Lan_STR_LANG_ID[1 + Lan_NUM_LANGUAGES];
   char StrField[32];
   char SubQueryNam1[Cty_MAX_BYTES_SUBQUERY_CTYS + 1];
   char SubQueryNam2[Cty_MAX_BYTES_SUBQUERY_CTYS + 1];
   char SubQueryWWW1[Cty_MAX_BYTES_SUBQUERY_CTYS + 1];
   char SubQueryWWW2[Cty_MAX_BYTES_SUBQUERY_CTYS + 1];
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows = 0;
   Lan_Language_t Lan;
   bool CtyFound;

   if (Cty->CtyCod < 0)
      return false;

   /***** Clear data *****/
   for (Lan = (Lan_Language_t) 1;
	Lan <= Lan_NUM_LANGUAGES;
	Lan++)
     {
      Cty->Name[Lan][0] = '\0';
      Cty->WWW[Lan][0] = '\0';
     }
   Cty->NumUsrsWhoClaimToBelongToCty = 0;
   Cty->Inss.Num = Cty->NumCtrs = Cty->NumDegs = Cty->NumCrss = 0;
   Cty->NumUsrs = 0;

   /***** Check if country code is correct *****/
   if (Cty->CtyCod == 0)
     {
      for (Lan = (Lan_Language_t) 1;
	   Lan <= Lan_NUM_LANGUAGES;
	   Lan++)
         if (Lan == Gbl.Prefs.Language)
            Str_Copy (Cty->Name[Lan],Txt_Another_country,
                      Cty_MAX_BYTES_NAME);
         else
            Cty->Name[Lan][0] = '\0';
      return false;
     }

   // Here Cty->CtyCod > 0

   /***** Get data of a country from database *****/
   switch (GetExtraData)
     {
      case Cty_GET_BASIC_DATA:
         NumRows = DB_QuerySELECT (&mysql_res,"can not get data of a country",
				   "SELECT Alpha2,Name_%s,WWW_%s"
				   " FROM countries"
				   " WHERE CtyCod='%03ld'",
				   Lan_STR_LANG_ID[Gbl.Prefs.Language],
				   Lan_STR_LANG_ID[Gbl.Prefs.Language],
				   Cty->CtyCod);
         break;
      case Cty_GET_EXTRA_DATA:
	 SubQueryNam1[0] = '\0';
	 SubQueryNam2[0] = '\0';
	 SubQueryWWW1[0] = '\0';
	 SubQueryWWW2[0] = '\0';
	 for (Lan = (Lan_Language_t) 1;
	      Lan <= Lan_NUM_LANGUAGES;
	      Lan++)
	   {
	    snprintf (StrField,sizeof (StrField),
		      "countries.Name_%s,",
		      Lan_STR_LANG_ID[Lan]);
	    Str_Concat (SubQueryNam1,StrField,
	                Cty_MAX_BYTES_SUBQUERY_CTYS);
	    snprintf (StrField,sizeof (StrField),
		      "Name_%s,",
		      Lan_STR_LANG_ID[Lan]);
	    Str_Concat (SubQueryNam2,StrField,
	                Cty_MAX_BYTES_SUBQUERY_CTYS);

	    snprintf (StrField,sizeof (StrField),
		      "countries.WWW_%s,",
		      Lan_STR_LANG_ID[Lan]);
	    Str_Concat (SubQueryWWW1,StrField,
	                Cty_MAX_BYTES_SUBQUERY_CTYS);
	    snprintf (StrField,sizeof (StrField),
		      "WWW_%s,",
		      Lan_STR_LANG_ID[Lan]);
	    Str_Concat (SubQueryWWW2,StrField,
	                Cty_MAX_BYTES_SUBQUERY_CTYS);
	   }
	 NumRows = DB_QuerySELECT (&mysql_res,"can not get data of a country",
				   "(SELECT countries.Alpha2,%s%sCOUNT(*) AS NumUsrs"
				   " FROM countries,usr_data"
				   " WHERE countries.CtyCod='%03ld'"
				   " AND countries.CtyCod=usr_data.CtyCod)"
				   " UNION "
				   "(SELECT Alpha2,%s%s0 AS NumUsrs"
				   " FROM countries"
				   " WHERE CtyCod='%03ld'"
				   " AND CtyCod NOT IN"
				   " (SELECT DISTINCT CtyCod FROM usr_data))",
				   SubQueryNam1,SubQueryWWW1,Cty->CtyCod,
				   SubQueryNam2,SubQueryWWW2,Cty->CtyCod);
	 break;
     }

   /***** Count number of rows in result *****/
   if (NumRows) // Country found...
     {
      CtyFound = true;

      /* Get row */
      row = mysql_fetch_row (mysql_res);

      /* Get Alpha-2 country code (row[0]) */
      Str_Copy (Cty->Alpha2,row[0],
                2);

      switch (GetExtraData)
	{
	 case Cty_GET_BASIC_DATA:
	    /* Get name and WWW of the country in current language */
	    Str_Copy (Cty->Name[Gbl.Prefs.Language],row[1],
	              Cty_MAX_BYTES_NAME);
	    Str_Copy (Cty->WWW[Gbl.Prefs.Language],row[2],
	              Cns_MAX_BYTES_WWW);
	    break;
	 case Cty_GET_EXTRA_DATA:
	    /* Get name and WWW of the country in several languages */
	    for (Lan = (Lan_Language_t) 1;
		 Lan <= Lan_NUM_LANGUAGES;
		 Lan++)
	      {
	       Str_Copy (Cty->Name[Lan],row[Lan],
	                 Cty_MAX_BYTES_NAME);
	       Str_Copy (Cty->WWW[Lan],row[Lan_NUM_LANGUAGES + Lan],
	                 Cns_MAX_BYTES_WWW);
	      }

	    /* Get number of users who claim to belong to this country */
	    if (sscanf (row[Lan_NUM_LANGUAGES * 2 + 1],"%u",
	                &Cty->NumUsrsWhoClaimToBelongToCty) != 1)
	       Cty->NumUsrsWhoClaimToBelongToCty = 0;

	    /* Get number of user in courses of this institution */
	    Cty->NumUsrs = Usr_GetNumUsrsInCrssOfCty (Rol_UNK,Cty->CtyCod);	// Here Rol_UNK means "all users"

	    /* Get number of institutions in this country */
	    Cty->Inss.Num = Ins_GetNumInssInCty (Cty->CtyCod);

	    break;
	}
     }
   else
      CtyFound = false;

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   return CtyFound;
  }

/*****************************************************************************/
/***************************** Get country name ******************************/
/*****************************************************************************/

void Cty_FlushCacheCountryName (void)
  {
   Gbl.Cache.CountryName.CtyCod = -1L;
   Gbl.Cache.CountryName.CtyName[0] = '\0';
  }

void Cty_GetCountryName (long CtyCod,char CtyName[Cty_MAX_BYTES_NAME + 1])
  {
   extern const char *Lan_STR_LANG_ID[1 + Lan_NUM_LANGUAGES];
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;

   /***** 1. Fast check: Trivial case *****/
   if (CtyCod <= 0)
     {
      CtyName[0] = '\0';	// Empty name
      return;
     }

   /***** 2. Fast check: If cached... *****/
   if (CtyCod == Gbl.Cache.CountryName.CtyCod)
     {
      Str_Copy (CtyName,Gbl.Cache.CountryName.CtyName,
		Cty_MAX_BYTES_NAME);
      return;
     }

   /***** 3. Slow: get country name from database *****/
   Gbl.Cache.CountryName.CtyCod = CtyCod;

   if (DB_QuerySELECT (&mysql_res,"can not get the name of a country",
		       "SELECT Name_%s FROM countries WHERE CtyCod='%03ld'",
	               Lan_STR_LANG_ID[Gbl.Prefs.Language],CtyCod)) // Country found...
     {
      /* Get row */
      row = mysql_fetch_row (mysql_res);

      /* Get the name of the country */
      Str_Copy (Gbl.Cache.CountryName.CtyName,row[0],
		Cty_MAX_BYTES_NAME);
     }
   else
      Gbl.Cache.CountryName.CtyName[0] = '\0';

   /* Free structure that stores the query result */
   DB_FreeMySQLResult (&mysql_res);

   Str_Copy (CtyName,Gbl.Cache.CountryName.CtyName,
	     Cty_MAX_BYTES_NAME);
  }

/*****************************************************************************/
/******************** Get map attribution from database **********************/
/*****************************************************************************/

static void Cty_GetMapAttribution (long CtyCod,char **MapAttribution)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   size_t Length;

   /***** Free possible former map attribution *****/
   Cty_FreeMapAttribution (MapAttribution);

   /***** Get photo attribution from database *****/
   if (DB_QuerySELECT (&mysql_res,"can not get photo attribution",
		       "SELECT MapAttribution FROM countries WHERE CtyCod=%ld",
	               CtyCod))
     {
      /* Get row */
      row = mysql_fetch_row (mysql_res);

      /* Get the attribution of the map of the country (row[0]) */
      if (row[0])
	 if (row[0][0])
	   {
	    Length = strlen (row[0]);
	    if ((*MapAttribution = (char *) malloc (Length + 1)) == NULL)
	       Lay_ShowErrorAndExit ("Error allocating memory for map attribution.");
	    Str_Copy (*MapAttribution,row[0],
	              Length);
	   }
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/******************* Free memory used for map attribution ********************/
/*****************************************************************************/

static void Cty_FreeMapAttribution (char **MapAttribution)
  {
   if (*MapAttribution)
     {
      free ((void *) *MapAttribution);
      *MapAttribution = NULL;
     }
  }

/*****************************************************************************/
/*************************** Free list of countries **************************/
/*****************************************************************************/

void Cty_FreeListCountries (void)
  {
   if (Gbl.Hierarchy.Sys.Ctys.Lst)
     {
      /***** Free memory used by the list of courses in institution *****/
      free ((void *) Gbl.Hierarchy.Sys.Ctys.Lst);
      Gbl.Hierarchy.Sys.Ctys.Lst = NULL;
      Gbl.Hierarchy.Sys.Ctys.Num = 0;
     }
  }

/*****************************************************************************/
/*************************** List all the countries **************************/
/*****************************************************************************/

static void Cty_ListCountriesForEdition (void)
  {
   extern const char *Txt_STR_LANG_NAME[1 + Lan_NUM_LANGUAGES];
   unsigned NumCty;
   struct Country *Cty;
   Lan_Language_t Lan;

   /***** Write heading *****/
   Tbl_TABLE_BeginWidePadding (2);
   Cty_PutHeadCountriesForEdition ();

   /***** Write all the countries *****/
   for (NumCty = 0;
	NumCty < Gbl.Hierarchy.Sys.Ctys.Num;
	NumCty++)
     {
      Cty = &Gbl.Hierarchy.Sys.Ctys.Lst[NumCty];

      Tbl_TR_Begin (NULL);

      /* Put icon to remove country */
      Tbl_TD_Begin ("rowspan=\"%u\" class=\"BT\"",1 + Lan_NUM_LANGUAGES);
      if (Cty->Inss.Num ||
	  Cty->NumUsrsWhoClaimToBelongToCty ||
	  Cty->NumUsrs)	// Country has institutions or users ==> deletion forbidden
	 Ico_PutIconRemovalNotAllowed ();
      else
        {
         Frm_StartForm (ActRemCty);
         Cty_PutParamOtherCtyCod (Cty->CtyCod);
         Ico_PutIconRemove ();
         Frm_EndForm ();
        }
      Tbl_TD_End ();

      /* Numerical country code (ISO 3166-1) */
      Tbl_TD_Begin ("rowspan=\"%u\" class=\"DAT RT\"",1 + Lan_NUM_LANGUAGES);
      fprintf (Gbl.F.Out,"%03ld",Cty->CtyCod);
      Tbl_TD_End ();

      /* Alphabetic country code with 2 letters (ISO 3166-1) */
      Tbl_TD_Begin ("rowspan=\"%u\" class=\"DAT RT\"",1 + Lan_NUM_LANGUAGES);
      fprintf (Gbl.F.Out,"%s",Cty->Alpha2);
      Tbl_TD_End ();

      Tbl_TD_Empty (3);

      /* Number of users */
      Tbl_TD_Begin ("rowspan=\"%u\" class=\"DAT RT\"",1 + Lan_NUM_LANGUAGES);
      fprintf (Gbl.F.Out,"%u",Cty->NumUsrsWhoClaimToBelongToCty);
      Tbl_TD_End ();

      /* Number of institutions */
      Tbl_TD_Begin ("rowspan=\"%u\" class=\"DAT RT\"",1 + Lan_NUM_LANGUAGES);
      fprintf (Gbl.F.Out,"%u",Cty->Inss.Num);
      Tbl_TD_End ();

      Tbl_TR_End ();

      /* Country name in several languages */
      for (Lan = (Lan_Language_t) 1;
	   Lan <= Lan_NUM_LANGUAGES;
	   Lan++)
        {
         Tbl_TR_Begin (NULL);

	 /* Language */
         Tbl_TD_Begin ("class=\"DAT RM\"");
         fprintf (Gbl.F.Out,"%s:",Txt_STR_LANG_NAME[Lan]);
         Tbl_TD_End ();

         /* Name */
         Tbl_TD_Begin ("class=\"LT\"");
         Frm_StartForm (ActRenCty);
         Cty_PutParamOtherCtyCod (Cty->CtyCod);
         Par_PutHiddenParamUnsigned ("Lan",(unsigned) Lan);
         fprintf (Gbl.F.Out,"<input type=\"text\" name=\"Name\""
                            " size=\"15\" maxlength=\"%u\" value=\"%s\""
                            " onchange=\"document.getElementById('%s').submit();\" />",
                  Cty_MAX_CHARS_NAME,
                  Cty->Name[Lan],Gbl.Form.Id);
         Frm_EndForm ();
         Tbl_TD_End ();

         /* WWW */
         Tbl_TD_Begin ("class=\"LT\"");
         Frm_StartForm (ActChgCtyWWW);
         Cty_PutParamOtherCtyCod (Cty->CtyCod);
         Par_PutHiddenParamUnsigned ("Lan",(unsigned) Lan);
         fprintf (Gbl.F.Out,"<input type=\"url\" name=\"WWW\""
                            " maxlength=\"%u\" value=\"%s\""
                            " class=\"INPUT_WWW\""
                            " onchange=\"document.getElementById('%s').submit();\" />",
                  Cns_MAX_CHARS_WWW,
                  Cty->WWW[Lan],Gbl.Form.Id);
         Frm_EndForm ();
         Tbl_TD_End ();

         Tbl_TR_End ();
        }
     }

   /***** End table *****/
   Tbl_TABLE_End ();
  }

/*****************************************************************************/
/******************** Write parameter with code of country *******************/
/*****************************************************************************/

void Cty_PutParamCtyCod (long CtyCod)
  {
   Par_PutHiddenParamLong ("cty",CtyCod);
  }

/*****************************************************************************/
/******************** Write parameter with code of country *******************/
/*****************************************************************************/

static void Cty_PutParamOtherCtyCod (long CtyCod)
  {
   Par_PutHiddenParamLong ("OthCtyCod",CtyCod);
  }

/*****************************************************************************/
/******************* Get parameter with code of country **********************/
/*****************************************************************************/

long Cty_GetAndCheckParamOtherCtyCod (long MinCodAllowed)
  {
   long CtyCod;

   /***** Get and check parameter with code of country *****/
   if ((CtyCod = Cty_GetParamOtherCtyCod ()) < MinCodAllowed)
      Lay_ShowErrorAndExit ("Code of country is missing or invalid.");

   return CtyCod;
  }

static long Cty_GetParamOtherCtyCod (void)
  {
   /***** Get code of country *****/
   return Par_GetParToLong ("OthCtyCod");
  }

/*****************************************************************************/
/****************************** Remove a country *****************************/
/*****************************************************************************/

void Cty_RemoveCountry (void)
  {
   extern const char *Txt_You_can_not_remove_a_country_with_institutions_or_users;
   extern const char *Txt_Country_X_removed;

   /***** Country constructor *****/
   Cty_EditingCountryConstructor ();

   /***** Get country code *****/
   Cty_EditingCty->CtyCod = Cty_GetAndCheckParamOtherCtyCod (0);

   /***** Get data of the country from database *****/
   Cty_GetDataOfCountryByCod (Cty_EditingCty,Cty_GET_EXTRA_DATA);

   /***** Check if this country has users *****/
   if (Cty_EditingCty->Inss.Num ||
       Cty_EditingCty->NumUsrsWhoClaimToBelongToCty ||
       Cty_EditingCty->NumUsrs)	// Country has institutions or users ==> don't remove
      Ale_CreateAlert (Ale_WARNING,NULL,
	               Txt_You_can_not_remove_a_country_with_institutions_or_users);
   else	// Country has no users ==> remove it
     {
      /***** Remove surveys of the country *****/
      Svy_RemoveSurveys (Hie_CTY,Cty_EditingCty->CtyCod);

      /***** Remove country *****/
      DB_QueryDELETE ("can not remove a country",
		      "DELETE FROM countries WHERE CtyCod='%03ld'",
		      Cty_EditingCty->CtyCod);

      /***** Flush cache *****/
      Cty_FlushCacheCountryName ();

      /***** Write message to show the change made *****/
      Ale_CreateAlert (Ale_SUCCESS,NULL,
	               Txt_Country_X_removed,
	               Cty_EditingCty->Name[Gbl.Prefs.Language]);

      Cty_EditingCty->CtyCod = -1L;	// To not showing button to go to country
     }
  }

/*****************************************************************************/
/************************ Change the name of a country ***********************/
/*****************************************************************************/

void Cty_RenameCountry (void)
  {
   extern const char *Txt_You_can_not_leave_the_name_of_the_country_X_empty;
   extern const char *Txt_The_country_X_already_exists;
   extern const char *Txt_The_country_X_has_been_renamed_as_Y;
   extern const char *Lan_STR_LANG_ID[1 + Lan_NUM_LANGUAGES];
   extern const char *Txt_The_name_of_the_country_X_has_not_changed;
   char NewCtyName[Cty_MAX_BYTES_NAME + 1];
   Lan_Language_t Language;
   char FieldName[4 + 1 + 2 + 1];	// Example: "Name_en"

   /***** Country constructor *****/
   Cty_EditingCountryConstructor ();

   /***** Get the code of the country *****/
   Cty_EditingCty->CtyCod = Cty_GetAndCheckParamOtherCtyCod (0);

   /***** Get the lenguage *****/
   Language = Lan_GetParamLanguage ();

   /***** Get the new name for the country *****/
   Par_GetParToText ("Name",NewCtyName,Cty_MAX_BYTES_NAME);

   /***** Get from the database the data of the country *****/
   Cty_GetDataOfCountryByCod (Cty_EditingCty,Cty_GET_EXTRA_DATA);

   /***** Check if new name is empty *****/
   if (!NewCtyName[0])
      Ale_CreateAlert (Ale_WARNING,NULL,
	               Txt_You_can_not_leave_the_name_of_the_country_X_empty,
	               Cty_EditingCty->Name[Language]);
   else
     {
      /***** Check if old and new names are the same
             (this happens when return is pressed without changes) *****/
      if (strcmp (Cty_EditingCty->Name[Language],NewCtyName))	// Different names
	{
	 /***** If country was in database... *****/
	 if (Cty_CheckIfCountryNameExists (Language,NewCtyName,Cty_EditingCty->CtyCod))
	    Ale_CreateAlert (Ale_WARNING,NULL,
		             Txt_The_country_X_already_exists,
		             NewCtyName);
	 else
	   {
	    /* Update the table changing old name by new name */
	    snprintf (FieldName,sizeof (FieldName),
		      "Name_%s",
		      Lan_STR_LANG_ID[Language]);
	    Cty_UpdateCtyNameDB (Cty_EditingCty->CtyCod,FieldName,NewCtyName);

	    /* Write message to show the change made */
	    Ale_CreateAlert (Ale_SUCCESS,NULL,
		             Txt_The_country_X_has_been_renamed_as_Y,
		             Cty_EditingCty->Name[Language],NewCtyName);

	    /* Update country name */
	    Str_Copy (Cty_EditingCty->Name[Language],NewCtyName,
		      Cty_MAX_BYTES_NAME);
	   }
	}
      else	// The same name
	 Ale_CreateAlert (Ale_INFO,NULL,
	                  Txt_The_name_of_the_country_X_has_not_changed,
		          Cty_EditingCty->Name[Language]);
     }
  }

/*****************************************************************************/
/******************* Check if a numeric country code exists ******************/
/*****************************************************************************/

static bool Cty_CheckIfNumericCountryCodeExists (long CtyCod)
  {
   /***** Get number of countries with a name from database *****/
   return (DB_QueryCOUNT ("can not check if the numeric code"
	                  " of a country already existed",
			  "SELECT COUNT(*) FROM countries"
			  " WHERE CtyCod='%03ld'",
			  CtyCod) != 0);
  }

/*****************************************************************************/
/*************** Check if an alphabetic country code exists ******************/
/*****************************************************************************/

static bool Cty_CheckIfAlpha2CountryCodeExists (const char Alpha2[2 + 1])
  {
   /***** Get number of countries with a name from database *****/
   return (DB_QueryCOUNT ("can not check if the alphabetic code"
	                  " of a country already existed",
			  "SELECT COUNT(*) FROM countries"
			  " WHERE Alpha2='%s'",
			  Alpha2) != 0);
  }

/*****************************************************************************/
/******************** Check if the name of country exists ********************/
/*****************************************************************************/

static bool Cty_CheckIfCountryNameExists (Lan_Language_t Language,const char *Name,long CtyCod)
  {
   extern const char *Lan_STR_LANG_ID[1 + Lan_NUM_LANGUAGES];

   /***** Get number of countries with a name from database *****/
   return (DB_QueryCOUNT ("can not check if the name"
	                  " of a country already existed",
			  "SELECT COUNT(*) FROM countries"
			  " WHERE Name_%s='%s' AND CtyCod<>'%03ld'",
			  Lan_STR_LANG_ID[Language],Name,CtyCod) != 0);
  }

/*****************************************************************************/
/************ Update institution name in table of institutions ***************/
/*****************************************************************************/

static void Cty_UpdateCtyNameDB (long CtyCod,const char *FieldName,const char *NewCtyName)
  {
   /***** Update country changing old name by new name */
   DB_QueryUPDATE ("can not update the name of a country",
		   "UPDATE countries SET %s='%s' WHERE CtyCod='%03ld'",
	           FieldName,NewCtyName,CtyCod);

   /***** Flush cache *****/
   Cty_FlushCacheCountryName ();
  }

/*****************************************************************************/
/************************ Change the URL of a country ************************/
/*****************************************************************************/

void Cty_ChangeCtyWWW (void)
  {
   extern const char *Txt_The_new_web_address_is_X;
   extern const char *Lan_STR_LANG_ID[1 + Lan_NUM_LANGUAGES];
   char NewWWW[Cns_MAX_BYTES_WWW + 1];
   Lan_Language_t Language;

   /***** Country constructor *****/
   Cty_EditingCountryConstructor ();

   /***** Get the code of the country *****/
   Cty_EditingCty->CtyCod = Cty_GetAndCheckParamOtherCtyCod (0);

   /***** Get the lenguage *****/
   Language = Lan_GetParamLanguage ();

   /***** Get the new WWW for the country *****/
   Par_GetParToText ("WWW",NewWWW,Cns_MAX_BYTES_WWW);

   /***** Get from the database the data of the country *****/
   Cty_GetDataOfCountryByCod (Cty_EditingCty,Cty_GET_EXTRA_DATA);

   /***** Update the table changing old WWW by new WWW *****/
   DB_QueryUPDATE ("can not update the web of a country",
		   "UPDATE countries SET WWW_%s='%s'"
		   " WHERE CtyCod='%03ld'",
	           Lan_STR_LANG_ID[Language],NewWWW,Cty_EditingCty->CtyCod);
   Str_Copy (Cty_EditingCty->WWW[Language],NewWWW,
	     Cns_MAX_BYTES_WWW);

   /***** Write message to show the change made *****/
   Ale_CreateAlert (Ale_SUCCESS,NULL,
	            Txt_The_new_web_address_is_X,
	            NewWWW);
  }

/*****************************************************************************/
/*********** Change the attribution of the map of current country ************/
/*****************************************************************************/

void Cty_ChangeCtyMapAttribution (void)
  {
   char NewMapAttribution[Med_MAX_BYTES_ATTRIBUTION + 1];

   /***** Get parameters from form *****/
   /* Get the new map attribution for the country */
   Par_GetParToText ("Attribution",NewMapAttribution,Med_MAX_BYTES_ATTRIBUTION);

   /***** Update the table changing old attribution by new attribution *****/
   DB_QueryUPDATE ("can not update the map attribution of a country",
		   "UPDATE countries SET MapAttribution='%s'"
		   " WHERE CtyCod='%03ld'",
	           NewMapAttribution,Gbl.Hierarchy.Cty.CtyCod);

   /***** Show the country information again *****/
   Cty_ShowConfiguration ();
  }

/*****************************************************************************/
/********* Show alerts after changing a country and continue editing *********/
/*****************************************************************************/

void Cty_ContEditAfterChgCty (void)
  {
   /***** Write message to show the change made
	  and put button to go to country changed *****/
   Cty_ShowAlertAndButtonToGoToCty ();

   /***** Show the form again *****/
   Cty_EditCountriesInternal ();

   /***** Country destructor *****/
   Cty_EditingCountryDestructor ();
  }

/*****************************************************************************/
/***************** Write message to show the change made   *******************/
/***************** and put button to go to country changed *******************/
/*****************************************************************************/

static void Cty_ShowAlertAndButtonToGoToCty (void)
  {
   extern const char *Txt_Go_to_X;

   // If the country being edited is different to the current one...
   if (Cty_EditingCty->CtyCod != Gbl.Hierarchy.Cty.CtyCod)
     {
      /***** Alert with button to go to couuntry *****/
      snprintf (Gbl.Title,sizeof (Gbl.Title),
	        Txt_Go_to_X,
		Cty_EditingCty->Name[Gbl.Prefs.Language]);
      Ale_ShowLastAlertAndButton (ActSeeIns,NULL,NULL,Cty_PutParamGoToCty,
                                  Btn_CONFIRM_BUTTON,Gbl.Title);
     }
   else
      /***** Alert *****/
      Ale_ShowAlerts (NULL);
  }

static void Cty_PutParamGoToCty (void)
  {
   /***** Put parameter *****/
   Cty_PutParamCtyCod (Cty_EditingCty->CtyCod);
  }

/*****************************************************************************/
/********************* Put a form to create a new country ********************/
/*****************************************************************************/

static void Cty_PutFormToCreateCountry (void)
  {
   extern const char *Txt_New_country;
   extern const char *Txt_STR_LANG_NAME[1 + Lan_NUM_LANGUAGES];
   extern const char *Lan_STR_LANG_ID[1 + Lan_NUM_LANGUAGES];
   extern const char *Txt_Create_country;
   Lan_Language_t Lan;

   /***** Start form *****/
   Frm_StartForm (ActNewCty);

   /***** Start box and table *****/
   Box_StartBoxTable (NULL,Txt_New_country,NULL,
                      NULL,Box_NOT_CLOSABLE,2);

   /***** Write heading *****/
   Cty_PutHeadCountriesForEdition ();

   Tbl_TR_Begin (NULL);

   /***** Column to remove country, disabled here *****/
   Tbl_TD_Begin ("rowspan=\"%u\" class=\"BT\"",1 + Lan_NUM_LANGUAGES);
   Tbl_TD_End ();

   /***** Numerical country code (ISO 3166-1) *****/
   Tbl_TD_Begin ("rowspan=\"%u\" class=\"RT\"",1 + Lan_NUM_LANGUAGES);
   fprintf (Gbl.F.Out,"<input type=\"text\" name=\"OthCtyCod\""
                      " size=\"3\" maxlength=\"10\" value=\"");
   if (Cty_EditingCty->CtyCod > 0)
      fprintf (Gbl.F.Out,"%03ld",Cty_EditingCty->CtyCod);
   fprintf (Gbl.F.Out,"\" required=\"required\" />");
   Tbl_TD_End ();

   /***** Alphabetic country code with 2 letters (ISO 3166-1) *****/
   Tbl_TD_Begin ("rowspan=\"%u\" class=\"RT\"",1 + Lan_NUM_LANGUAGES);
   fprintf (Gbl.F.Out,"<input type=\"text\" name=\"Alpha2\""
                      " size=\"2\" maxlength=\"2\" value=\"%s\""
                      " required=\"required\" />",Cty_EditingCty->Alpha2);
   Tbl_TD_End ();

   Tbl_TD_Empty (3);

   /***** Number of users *****/
   Tbl_TD_Begin ("rowspan=\"%u\" class=\"DAT RT\"",1 + Lan_NUM_LANGUAGES);
   fprintf (Gbl.F.Out,"0");
   Tbl_TD_End ();

   /***** Number of institutions *****/
   Tbl_TD_Begin ("rowspan=\"%u\" class=\"DAT RT\"",1 + Lan_NUM_LANGUAGES);
   fprintf (Gbl.F.Out,"0");
   Tbl_TD_End ();

   Tbl_TR_End ();

   /***** Country name in several languages *****/
   for (Lan = (Lan_Language_t) 1;
	Lan <= Lan_NUM_LANGUAGES;
	Lan++)
     {
      Tbl_TR_Begin (NULL);

      /* Language */
      Tbl_TD_Begin ("class=\"DAT RT\"");
      fprintf (Gbl.F.Out,"%s",Txt_STR_LANG_NAME[Lan]);
      Tbl_TD_End ();

      /* Name */
      Tbl_TD_Begin ("class=\"LM\"");
      fprintf (Gbl.F.Out,"<input type=\"text\" name=\"Name_%s\""
                         " size=\"15\" maxlength=\"%u\" value=\"%s\""
                         " required=\"required\" />",
               Lan_STR_LANG_ID[Lan],
               Cty_MAX_CHARS_NAME,
               Cty_EditingCty->Name[Lan]);
      Tbl_TD_End ();

      /* WWW */
      Tbl_TD_Begin ("class=\"LM\"");
      fprintf (Gbl.F.Out,"<input type=\"url\" name=\"WWW_%s\""
                         " maxlength=\"%u\" value=\"%s\""
                         " class=\"INPUT_WWW\" />",
	       Lan_STR_LANG_ID[Lan],
	       Cns_MAX_CHARS_WWW,
	       Cty_EditingCty->WWW[Lan]);
      Tbl_TD_End ();

      Tbl_TR_End ();
     }

   /***** End table, send button and end box *****/
   Box_EndBoxTableWithButton (Btn_CREATE_BUTTON,Txt_Create_country);

   /***** End form *****/
   Frm_EndForm ();
  }

/*****************************************************************************/
/******************* Write header with fields of a country *******************/
/*****************************************************************************/

static void Cty_PutHeadCountriesForEdition (void)
  {
   extern const char *Txt_Numeric_BR_code_BR_ISO_3166_1;
   extern const char *Txt_Alphabetic_BR_code_BR_ISO_3166_1;
   extern const char *Txt_Name;
   extern const char *Txt_WWW;
   extern const char *Txt_Users;
   extern const char *Txt_Institutions_ABBREVIATION;

   Tbl_TR_Begin (NULL);

   Tbl_TH (1,1,"BM",NULL);
   Tbl_TH (1,1,"RM",Txt_Numeric_BR_code_BR_ISO_3166_1);
   Tbl_TH (1,1,"RM",Txt_Alphabetic_BR_code_BR_ISO_3166_1);
   Tbl_TH_Empty (1);
   Tbl_TH (1,1,"LM",Txt_Name);
   Tbl_TH (1,1,"LM",Txt_WWW);
   Tbl_TH (1,1,"RM",Txt_Users);
   Tbl_TH (1,1,"RM",Txt_Institutions_ABBREVIATION);

   Tbl_TR_End ();
  }

/*****************************************************************************/
/******************* Receive form to create a new country ********************/
/*****************************************************************************/

void Cty_RecFormNewCountry (void)
  {
   extern const char *Txt_You_must_specify_the_numerical_code_of_the_new_country;
   extern const char *Txt_The_numerical_code_X_already_exists;
   extern const char *Txt_The_alphabetical_code_X_is_not_correct;
   extern const char *Txt_The_alphabetical_code_X_already_exists;
   extern const char *Lan_STR_LANG_ID[1 + Lan_NUM_LANGUAGES];
   extern const char *Txt_The_country_X_already_exists;
   extern const char *Txt_You_must_specify_the_name_of_the_new_country_in_all_languages;
   extern const char *Txt_Created_new_country_X;
   char ParamName[32];
   bool CreateCountry = true;
   Lan_Language_t Lan;
   unsigned i;

   /***** Country constructoor *****/
   Cty_EditingCountryConstructor ();

   /***** Get parameters from form *****/
   /* Get numeric country code */
   if ((Cty_EditingCty->CtyCod = Cty_GetParamOtherCtyCod ()) < 0)
     {
      Ale_CreateAlert (Ale_WARNING,NULL,
	               Txt_You_must_specify_the_numerical_code_of_the_new_country);
      CreateCountry = false;
     }
   else if (Cty_CheckIfNumericCountryCodeExists (Cty_EditingCty->CtyCod))
     {
      Ale_CreateAlert (Ale_WARNING,NULL,
	               Txt_The_numerical_code_X_already_exists,
                       Cty_EditingCty->CtyCod);
      CreateCountry = false;
     }
   else	// Numeric code correct
     {
      /* Get alphabetic-2 country code */
      Par_GetParToText ("Alpha2",Cty_EditingCty->Alpha2,2);
      Str_ConvertToUpperText (Cty_EditingCty->Alpha2);
      for (i = 0;
	   i < 2 && CreateCountry;
	   i++)
         if (Cty_EditingCty->Alpha2[i] < 'A' ||
             Cty_EditingCty->Alpha2[i] > 'Z')
           {
            Ale_CreateAlert (Ale_WARNING,NULL,
        	             Txt_The_alphabetical_code_X_is_not_correct,
                             Cty_EditingCty->Alpha2);
            CreateCountry = false;
           }
      if (CreateCountry)
        {
         if (Cty_CheckIfAlpha2CountryCodeExists (Cty_EditingCty->Alpha2))
           {
            Ale_CreateAlert (Ale_WARNING,NULL,
        	             Txt_The_alphabetical_code_X_already_exists,
                             Cty_EditingCty->Alpha2);
            CreateCountry = false;
           }
         else	// Alphabetic code correct
           {
            /* Get country name and WWW in different languages */
            for (Lan = (Lan_Language_t) 1;
        	 Lan <= Lan_NUM_LANGUAGES;
        	 Lan++)
              {
               snprintf (ParamName,sizeof (ParamName),
        	         "Name_%s",
			 Lan_STR_LANG_ID[Lan]);
               Par_GetParToText (ParamName,Cty_EditingCty->Name[Lan],Cty_MAX_BYTES_NAME);

               if (Cty_EditingCty->Name[Lan][0])	// If there's a country name
                 {
                  /***** If name of country was in database... *****/
                  if (Cty_CheckIfCountryNameExists (Lan,Cty_EditingCty->Name[Lan],-1L))
                    {
                     Ale_CreateAlert (Ale_WARNING,NULL,
                	              Txt_The_country_X_already_exists,
                                      Cty_EditingCty->Name[Lan]);
                     CreateCountry = false;
                     break;
                    }
                 }
               else	// If there is not a country name
                 {
                  Ale_CreateAlert (Ale_WARNING,NULL,
                	           Txt_You_must_specify_the_name_of_the_new_country_in_all_languages);
                  CreateCountry = false;
                  break;
                 }

               snprintf (ParamName,sizeof (ParamName),
        	         "WWW_%s",
			 Lan_STR_LANG_ID[Lan]);
               Par_GetParToText (ParamName,Cty_EditingCty->WWW[Lan],Cns_MAX_BYTES_WWW);
              }
           }
        }
     }

   if (CreateCountry)
     {
      Cty_CreateCountry ();	// Add new country to database
      Ale_ShowAlert (Ale_SUCCESS,Txt_Created_new_country_X,
		     Cty_EditingCty->Name);
     }
  }

/*****************************************************************************/
/**************************** Create a new country ***************************/
/*****************************************************************************/

#define Cty_MAX_BYTES_SUBQUERY_CTYS_NAME	((1 + Lan_NUM_LANGUAGES) * Cty_MAX_BYTES_NAME)
#define Cty_MAX_BYTES_SUBQUERY_CTYS_WWW		((1 + Lan_NUM_LANGUAGES) * Cns_MAX_BYTES_WWW)

static void Cty_CreateCountry (void)
  {
   extern const char *Lan_STR_LANG_ID[1 + Lan_NUM_LANGUAGES];
   Lan_Language_t Lan;
   char StrField[32];
   char SubQueryNam1[Cty_MAX_BYTES_SUBQUERY_CTYS + 1];
   char SubQueryNam2[Cty_MAX_BYTES_SUBQUERY_CTYS_NAME + 1];
   char SubQueryWWW1[Cty_MAX_BYTES_SUBQUERY_CTYS + 1];
   char SubQueryWWW2[Cty_MAX_BYTES_SUBQUERY_CTYS_WWW + 1];

   /***** Create a new country *****/
   SubQueryNam1[0] = '\0';
   SubQueryNam2[0] = '\0';
   SubQueryWWW1[0] = '\0';
   SubQueryWWW2[0] = '\0';
   for (Lan = (Lan_Language_t) 1;
	Lan <= Lan_NUM_LANGUAGES;
	Lan++)
     {
      snprintf (StrField,sizeof (StrField),
	        ",Name_%s",
		Lan_STR_LANG_ID[Lan]);
      Str_Concat (SubQueryNam1,StrField,
                  Cty_MAX_BYTES_SUBQUERY_CTYS);

      Str_Concat (SubQueryNam2,",'",
                  Cty_MAX_BYTES_SUBQUERY_CTYS_NAME);
      Str_Concat (SubQueryNam2,Cty_EditingCty->Name[Lan],
                  Cty_MAX_BYTES_SUBQUERY_CTYS_NAME);
      Str_Concat (SubQueryNam2,"'",
                  Cty_MAX_BYTES_SUBQUERY_CTYS_NAME);

      snprintf (StrField,sizeof (StrField),
	        ",WWW_%s",
		Lan_STR_LANG_ID[Lan]);
      Str_Concat (SubQueryWWW1,StrField,
                  Cty_MAX_BYTES_SUBQUERY_CTYS);

      Str_Concat (SubQueryWWW2,",'",
                  Cty_MAX_BYTES_SUBQUERY_CTYS_WWW);
      Str_Concat (SubQueryWWW2,Cty_EditingCty->WWW[Lan],
                  Cty_MAX_BYTES_SUBQUERY_CTYS_WWW);
      Str_Concat (SubQueryWWW2,"'",
                  Cty_MAX_BYTES_SUBQUERY_CTYS_WWW);
     }
   DB_QueryINSERT ("can not create country",
		   "INSERT INTO countries"
		   " (CtyCod,Alpha2,MapAttribution%s%s)"
		   " VALUES"
		   " ('%03ld','%s',''%s%s)",
                   SubQueryNam1,SubQueryWWW1,
                   Cty_EditingCty->CtyCod,Cty_EditingCty->Alpha2,
		   SubQueryNam2,SubQueryWWW2);
  }

/*****************************************************************************/
/*********************** Get total number of countries ***********************/
/*****************************************************************************/

unsigned Cty_GetNumCtysTotal (void)
  {
   /***** Get total number of countries from database *****/
   return (unsigned) DB_GetNumRowsTable ("countries");
  }

/*****************************************************************************/
/***************** Get number of countries with institutions *****************/
/*****************************************************************************/

unsigned Cty_GetNumCtysWithInss (const char *SubQuery)
  {
   /***** Get number of countries with institutions from database *****/
   return
   (unsigned) DB_QueryCOUNT ("can not get number of countries"
			     " with institutions",
			     "SELECT COUNT(DISTINCT countries.CtyCod)"
			     " FROM countries,institutions"
			     " WHERE %scountries.CtyCod=institutions.CtyCod",
			     SubQuery);
  }

/*****************************************************************************/
/******************* Get number of countries with centres ********************/
/*****************************************************************************/

unsigned Cty_GetNumCtysWithCtrs (const char *SubQuery)
  {
   /***** Get number of countries with centres from database *****/
   return
   (unsigned) DB_QueryCOUNT ("can not get number of countries with centres",
			     "SELECT COUNT(DISTINCT countries.CtyCod)"
			     " FROM countries,institutions,centres"
			     " WHERE %scountries.CtyCod=institutions.CtyCod"
			     " AND institutions.InsCod=centres.InsCod",
			     SubQuery);
  }

/*****************************************************************************/
/******************* Get number of countries with degrees ********************/
/*****************************************************************************/

unsigned Cty_GetNumCtysWithDegs (const char *SubQuery)
  {
   /***** Get number of countries with degrees from database *****/
   return
   (unsigned) DB_QueryCOUNT ("can not get number of countries with degrees",
			     "SELECT COUNT(DISTINCT countries.CtyCod)"
			     " FROM countries,institutions,centres,degrees"
			     " WHERE %scountries.CtyCod=institutions.CtyCod"
			     " AND institutions.InsCod=centres.InsCod"
			     " AND centres.CtrCod=degrees.CtrCod",
			     SubQuery);
  }

/*****************************************************************************/
/******************* Get number of countries with courses ********************/
/*****************************************************************************/

unsigned Cty_GetNumCtysWithCrss (const char *SubQuery)
  {
   /***** Get number of countries with courses from database *****/
   return
   (unsigned) DB_QueryCOUNT ("can not get number of countries with courses",
			     "SELECT COUNT(DISTINCT countries.CtyCod)"
			     " FROM countries,institutions,centres,degrees,courses"
			     " WHERE %scountries.CtyCod=institutions.CtyCod"
			     " AND institutions.InsCod=centres.InsCod"
			     " AND centres.CtrCod=degrees.CtrCod"
			     " AND degrees.DegCod=courses.DegCod",
			     SubQuery);
  }

/*****************************************************************************/
/******************* Get number of countries with users **********************/
/*****************************************************************************/

unsigned Cty_GetNumCtysWithUsrs (Rol_Role_t Role,const char *SubQuery)
  {
   /***** Get number of countries with users from database *****/
   return
   (unsigned) DB_QueryCOUNT ("can not get number of countries with users",
			     "SELECT COUNT(DISTINCT countries.CtyCod)"
			     " FROM countries,institutions,centres,degrees,courses,crs_usr"
			     " WHERE %scountries.CtyCod=institutions.CtyCod"
			     " AND institutions.InsCod=centres.InsCod"
			     " AND centres.CtrCod=degrees.CtrCod"
			     " AND degrees.DegCod=courses.DegCod"
			     " AND courses.CrsCod=crs_usr.CrsCod"
			     " AND crs_usr.Role=%u",
			     SubQuery,(unsigned) Role);
  }

/*****************************************************************************/
/***************************** List countries found **************************/
/*****************************************************************************/

void Cty_ListCtysFound (MYSQL_RES **mysql_res,unsigned NumCtys)
  {
   extern const char *Txt_country;
   extern const char *Txt_countries;
   MYSQL_ROW row;
   unsigned NumCty;
   struct Country Cty;

   /***** Query database *****/
   if (NumCtys)
     {
      /***** Start box and table *****/
      /* Number of countries found */
      snprintf (Gbl.Title,sizeof (Gbl.Title),
	        "%u %s",
                NumCtys,NumCtys == 1 ? Txt_country :
				       Txt_countries);
      Box_StartBoxTable (NULL,Gbl.Title,NULL,
                         NULL,Box_NOT_CLOSABLE,2);

      /***** Write heading *****/
      Cty_PutHeadCountriesForSeeing (false);	// Order not selectable

      /***** List the countries (one row per country) *****/
      for (NumCty = 1;
	   NumCty <= NumCtys;
	   NumCty++)
	{
	 /* Get next country */
	 row = mysql_fetch_row (*mysql_res);

	 /* Get country code (row[0]) */
	 Cty.CtyCod = Str_ConvertStrCodToLongCod (row[0]);

	 /* Get data of country */
	 Cty_GetDataOfCountryByCod (&Cty,Cty_GET_EXTRA_DATA);

	 /* Write data of this country */
	 Cty_ListOneCountryForSeeing (&Cty,NumCty);
	}

      /***** End table and box *****/
      Box_EndBoxTable ();
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (mysql_res);
  }

/*****************************************************************************/
/*********************** Country constructor/destructor **********************/
/*****************************************************************************/

static void Cty_EditingCountryConstructor (void)
  {
   Lan_Language_t Lan;

   /***** Pointer must be NULL *****/
   if (Cty_EditingCty != NULL)
      Lay_ShowErrorAndExit ("Error initializing country.");

   /***** Allocate memory for country *****/
   if ((Cty_EditingCty = (struct Country *) malloc (sizeof (struct Country))) == NULL)
      Lay_ShowErrorAndExit ("Error allocating memory for country.");

   /***** Reset country *****/
   Cty_EditingCty->CtyCod = -1L;
   Cty_EditingCty->Alpha2[0] = '\0';
   for (Lan = (Lan_Language_t) 1;
	Lan <= Lan_NUM_LANGUAGES;
	Lan++)
     {
      Cty_EditingCty->Name[Lan][0] = '\0';
      Cty_EditingCty->WWW [Lan][0] = '\0';
     }
   Cty_EditingCty->Inss.Num = 0;
   Cty_EditingCty->Inss.Lst = NULL;
   Cty_EditingCty->Inss.SelectedOrder = Ins_ORDER_DEFAULT;
   Cty_EditingCty->NumCtrs = 0;
   Cty_EditingCty->NumDegs = 0;
   Cty_EditingCty->NumCrss = 0;
   Cty_EditingCty->NumUsrs = 0;
   Cty_EditingCty->NumUsrsWhoClaimToBelongToCty = 0;
  }

static void Cty_EditingCountryDestructor (void)
  {
   /***** Free memory used for country *****/
   if (Cty_EditingCty != NULL)
     {
      free ((void *) Cty_EditingCty);
      Cty_EditingCty = NULL;
     }
  }
