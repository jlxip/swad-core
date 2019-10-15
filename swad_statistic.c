// swad_statistic.c: statistics

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

#include <math.h>		// For log10, floor, ceil, modf, sqrt...
#include <stdlib.h>		// For getenv, malloc
#include <string.h>		// For string functions

#include "swad_box.h"
#include "swad_database.h"
#include "swad_form.h"
#include "swad_global.h"
#include "swad_ID.h"
#include "swad_profile.h"
#include "swad_role.h"
#include "swad_statistic.h"
#include "swad_table.h"

/*****************************************************************************/
/************** External global variables from others modules ****************/
/*****************************************************************************/

extern struct Globals Gbl;

/*****************************************************************************/
/***************************** Private constants *****************************/
/*****************************************************************************/

#define Sta_SECONDS_IN_RECENT_LOG ((time_t) (Cfg_DAYS_IN_RECENT_LOG * 24UL * 60UL * 60UL))	// Remove entries in recent log oldest than this time

const unsigned Sta_CellPadding[Sta_NUM_CLICKS_GROUPED_BY] =
  {
   2,	// Sta_CLICKS_CRS_DETAILED_LIST

   1,	// Sta_CLICKS_CRS_PER_USR
   1,	// Sta_CLICKS_CRS_PER_DAY
   0,	// Sta_CLICKS_CRS_PER_DAY_AND_HOUR
   1,	// Sta_CLICKS_CRS_PER_WEEK
   1,	// Sta_CLICKS_CRS_PER_MONTH
   1,	// Sta_CLICKS_CRS_PER_YEAR
   1,	// Sta_CLICKS_CRS_PER_HOUR
   0,	// Sta_CLICKS_CRS_PER_MINUTE
   1,	// Sta_CLICKS_CRS_PER_ACTION

   1,	// Sta_CLICKS_GBL_PER_DAY
   0,	// Sta_CLICKS_GBL_PER_DAY_AND_HOUR
   1,	// Sta_CLICKS_GBL_PER_WEEK
   1,	// Sta_CLICKS_GBL_PER_MONTH
   1,	// Sta_CLICKS_GBL_PER_YEAR
   1,	// Sta_CLICKS_GBL_PER_HOUR
   0,	// Sta_CLICKS_GBL_PER_MINUTE
   1,	// Sta_CLICKS_GBL_PER_ACTION
   1,	// Sta_CLICKS_GBL_PER_PLUGIN
   1,	// Sta_CLICKS_GBL_PER_API_FUNCTION
   1,	// Sta_CLICKS_GBL_PER_BANNER
   1,	// Sta_CLICKS_GBL_PER_COUNTRY
   1,	// Sta_CLICKS_GBL_PER_INSTITUTION
   1,	// Sta_CLICKS_GBL_PER_CENTRE
   1,	// Sta_CLICKS_GBL_PER_DEGREE
   1,	// Sta_CLICKS_GBL_PER_COURSE
  };

#define Sta_STAT_RESULTS_SECTION_ID	"stat_results"

/*****************************************************************************/
/******************************* Private types *******************************/
/*****************************************************************************/

typedef enum
  {
   Sta_SHOW_GLOBAL_ACCESSES,
   Sta_SHOW_COURSE_ACCESSES,
  } Sta_GlobalOrCourseAccesses_t;

/*****************************************************************************/
/***************************** Internal prototypes ***************************/
/*****************************************************************************/

static void Sta_PutLinkToCourseHits (void);
static void Sta_PutLinkToGlobalHits (void);

static void Sta_WriteSelectorCountType (void);
static void Sta_WriteSelectorAction (void);
static void Sta_ShowHits (Sta_GlobalOrCourseAccesses_t GlobalOrCourse);
static void Sta_ShowDetailedAccessesList (unsigned long NumRows,MYSQL_RES *mysql_res);
static void Sta_WriteLogComments (long LogCod);
static void Sta_ShowNumHitsPerUsr (unsigned long NumRows,MYSQL_RES *mysql_res);
static void Sta_ShowNumHitsPerDay (unsigned long NumRows,MYSQL_RES *mysql_res);
static void Sta_ShowDistrAccessesPerDayAndHour (unsigned long NumRows,MYSQL_RES *mysql_res);
static void Sta_PutHiddenParamScopeSta (void);
static Sta_ColorType_t Sta_GetStatColorType (void);
static void Sta_DrawBarColors (Sta_ColorType_t ColorType,float HitsMax);
static void Sta_DrawAccessesPerHourForADay (Sta_ColorType_t ColorType,float HitsNum[24],float HitsMax);
static void Sta_SetColor (Sta_ColorType_t ColorType,float HitsNum,float HitsMax,
                          unsigned *R,unsigned *G,unsigned *B);
static void Sta_ShowNumHitsPerWeek (unsigned long NumRows,
                                     MYSQL_RES *mysql_res);
static void Sta_ShowNumHitsPerMonth (unsigned long NumRows,
                                      MYSQL_RES *mysql_res);
static void Sta_ShowNumHitsPerYear (unsigned long NumRows,
                                    MYSQL_RES *mysql_res);
static void Sta_ShowNumHitsPerHour (unsigned long NumRows,
                                    MYSQL_RES *mysql_res);
static void Sta_WriteAccessHour (unsigned Hour,struct Sta_Hits *Hits,unsigned ColumnWidth);
static void Sta_ShowAverageAccessesPerMinute (unsigned long NumRows,MYSQL_RES *mysql_res);
static void Sta_WriteLabelsXAxisAccMin (float IncX,const char *Format);
static void Sta_WriteAccessMinute (unsigned Minute,float HitsNum,float MaxX);
static void Sta_ShowNumHitsPerAction (unsigned long NumRows,
                                      MYSQL_RES *mysql_res);
static void Sta_ShowNumHitsPerPlugin (unsigned long NumRows,
                                      MYSQL_RES *mysql_res);
static void Sta_ShowNumHitsPerWSFunction (unsigned long NumRows,
                                          MYSQL_RES *mysql_res);
static void Sta_ShowNumHitsPerBanner (unsigned long NumRows,
                                      MYSQL_RES *mysql_res);
static void Sta_ShowNumHitsPerCountry (unsigned long NumRows,
                                       MYSQL_RES *mysql_res);
static void Sta_WriteCountry (long CtyCod);
static void Sta_ShowNumHitsPerInstitution (unsigned long NumRows,
                                           MYSQL_RES *mysql_res);
static void Sta_WriteInstitution (long InsCod);
static void Sta_ShowNumHitsPerCentre (unsigned long NumRows,
                                      MYSQL_RES *mysql_res);
static void Sta_WriteCentre (long CtrCod);
static void Sta_ShowNumHitsPerDegree (unsigned long NumRows,
                                      MYSQL_RES *mysql_res);
static void Sta_WriteDegree (long DegCod);
static void Sta_ShowNumHitsPerCourse (unsigned long NumRows,
                                      MYSQL_RES *mysql_res);

static void Sta_DrawBarNumHits (char Color,
				float HitsNum,float HitsMax,float HitsTotal,
				unsigned MaxBarWidth);

/*****************************************************************************/
/*************** Read CGI environment variable REMOTE_ADDR *******************/
/*****************************************************************************/
/*
CGI Environment Variables:
REMOTE_ADDR
The IP address of the remote host making the request.
*/
void Sta_GetRemoteAddr (void)
  {
   if (getenv ("REMOTE_ADDR"))
      Str_Copy (Gbl.IP,getenv ("REMOTE_ADDR"),
                Cns_MAX_BYTES_IP);
   else
      Gbl.IP[0] = '\0';
  }

/*****************************************************************************/
/**************************** Log access in database *************************/
/*****************************************************************************/

void Sta_LogAccess (const char *Comments)
  {
   long LogCod;
   long ActCod = Act_GetActCod (Gbl.Action.Act);
   Rol_Role_t RoleToStore = (Gbl.Action.Act == ActLogOut) ? Gbl.Usrs.Me.Role.LoggedBeforeCloseSession :
                                                            Gbl.Usrs.Me.Role.Logged;

   /***** Insert access into database *****/
   /* Log access in historical log (log_full) */
   LogCod =
   DB_QueryINSERTandReturnCode ("can not log access (full)",
				"INSERT INTO log_full "
				"(ActCod,CtyCod,InsCod,CtrCod,DegCod,CrsCod,UsrCod,"
				"Role,ClickTime,TimeToGenerate,TimeToSend,IP)"
				" VALUES "
				"(%ld,%ld,%ld,%ld,%ld,%ld,%ld,"
				"%u,NOW(),%ld,%ld,'%s')",
				ActCod,
				Gbl.Hierarchy.Cty.CtyCod,
				Gbl.Hierarchy.Ins.InsCod,
				Gbl.Hierarchy.Ctr.CtrCod,
				Gbl.Hierarchy.Deg.DegCod,
				Gbl.Hierarchy.Crs.CrsCod,
				Gbl.Usrs.Me.UsrDat.UsrCod,
				(unsigned) RoleToStore,
				Gbl.TimeGenerationInMicroseconds,
				Gbl.TimeSendInMicroseconds,
				Gbl.IP);

   /* Log access in recent log (log_recent) */
   DB_QueryINSERT ("can not log access (recent)",
		   "INSERT INTO log_recent "
	           "(LogCod,ActCod,CtyCod,InsCod,CtrCod,DegCod,CrsCod,UsrCod,"
	           "Role,ClickTime,TimeToGenerate,TimeToSend,IP)"
                   " VALUES "
                   "(%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,"
                   "%u,NOW(),%ld,%ld,'%s')",
		   LogCod,ActCod,
		   Gbl.Hierarchy.Cty.CtyCod,
		   Gbl.Hierarchy.Ins.InsCod,
		   Gbl.Hierarchy.Ctr.CtrCod,
		   Gbl.Hierarchy.Deg.DegCod,
		   Gbl.Hierarchy.Crs.CrsCod,
		   Gbl.Usrs.Me.UsrDat.UsrCod,
		   (unsigned) RoleToStore,
		   Gbl.TimeGenerationInMicroseconds,
		   Gbl.TimeSendInMicroseconds,
		   Gbl.IP);

   /* Log comments */
   if (Comments)
      DB_QueryINSERT ("can not log access (comments)",
		      "INSERT INTO log_comments"
		      " (LogCod,Comments)"
		      " VALUES"
		      " (%ld,'%s')",
		      LogCod,Comments);

   /* Log search string */
   if (Gbl.Search.LogSearch && Gbl.Search.Str[0])
      DB_QueryINSERT ("can not log access (search)",
		      "INSERT INTO log_search"
		      " (LogCod,SearchStr)"
		      " VALUES"
		      " (%ld,'%s')",
		      LogCod,Gbl.Search.Str);

   if (Gbl.WebService.IsWebService)
      /* Log web service plugin and function */
      DB_QueryINSERT ("can not log access (comments)",
		      "INSERT INTO log_ws"
	              " (LogCod,PlgCod,FunCod)"
                      " VALUES"
                      " (%ld,%ld,%u)",
	              LogCod,Gbl.WebService.PlgCod,
		      (unsigned) Gbl.WebService.Function);
   else if (Gbl.Banners.BanCodClicked > 0)
      /* Log banner clicked */
      DB_QueryINSERT ("can not log banner clicked",
		      "INSERT INTO log_banners"
	              " (LogCod,BanCod)"
                      " VALUES"
                      " (%ld,%ld)",
	              LogCod,Gbl.Banners.BanCodClicked);

   /***** Increment my number of clicks *****/
   if (Gbl.Usrs.Me.Logged)
      Prf_IncrementNumClicksUsr (Gbl.Usrs.Me.UsrDat.UsrCod);
  }

/*****************************************************************************/
/************ Sometimes, we delete old entries in recent log table ***********/
/*****************************************************************************/

void Sta_RemoveOldEntriesRecentLog (void)
  {
   /***** Remove all expired clipboards *****/
   DB_QueryDELETE ("can not remove old entries from recent log",
		   "DELETE LOW_PRIORITY FROM log_recent"
                   " WHERE ClickTime<FROM_UNIXTIME(UNIX_TIMESTAMP()-%lu)",
		   Sta_SECONDS_IN_RECENT_LOG);
  }

/*****************************************************************************/
/******************** Show a form to make a query of clicks ******************/
/*****************************************************************************/

void Sta_AskShowCrsHits (void)
  {
   extern const char *Hlp_ANALYTICS_Visits_visits_to_course;
   extern const char *The_ClassFormInBox[The_NUM_THEMES];
   extern const char *Txt_Statistics_of_visits_to_the_course_X;
   extern const char *Txt_Users;
   extern const char *Txt_Show;
   extern const char *Txt_distributed_by;
   extern const char *Txt_STAT_CLICKS_GROUPED_BY[Sta_NUM_CLICKS_GROUPED_BY];
   extern const char *Txt_results_per_page;
   extern const char *Txt_Show_hits;
   extern const char *Txt_No_teachers_or_students_found;
   static unsigned long RowsPerPage[] =
     {
      Sta_MIN_ROWS_PER_PAGE * 1,
      Sta_MIN_ROWS_PER_PAGE * 2,
      Sta_MIN_ROWS_PER_PAGE * 3,
      Sta_MIN_ROWS_PER_PAGE * 4,
      Sta_MIN_ROWS_PER_PAGE * 5,
      Sta_MIN_ROWS_PER_PAGE * 10,
      Sta_MIN_ROWS_PER_PAGE * 50,
      Sta_MIN_ROWS_PER_PAGE * 100,
      Sta_MIN_ROWS_PER_PAGE * 500,
      Sta_MIN_ROWS_PER_PAGE * 1000,
      Sta_MIN_ROWS_PER_PAGE * 5000,
      Sta_MAX_ROWS_PER_PAGE,
     };
#define NUM_OPTIONS_ROWS_PER_PAGE (sizeof (RowsPerPage) / sizeof (RowsPerPage[0]))
   unsigned NumTotalUsrs;
   Sta_ClicksGroupedBy_t ClicksGroupedBy;
   unsigned long i;

   /***** Contextual links *****/
   fprintf (Gbl.F.Out,"<div class=\"CONTEXT_MENU\">");

   /* Put form to go to test edition and configuration */
   Sta_PutLinkToGlobalHits ();

   /* Link to show last clicks in real time */
   Sta_PutLinkToLastClicks ();

   fprintf (Gbl.F.Out,"</div>");

   /***** Get and update type of list,
          number of columns in class photo
          and preference about view photos *****/
   Usr_GetAndUpdatePrefsAboutUsrList ();

   /***** Get groups to show ******/
   Grp_GetParCodsSeveralGrpsToShowUsrs ();

   /***** Get and order the lists of users of this course *****/
   Usr_GetListUsrs (Hie_CRS,Rol_STD);
   Usr_GetListUsrs (Hie_CRS,Rol_NET);
   Usr_GetListUsrs (Hie_CRS,Rol_TCH);
   NumTotalUsrs = Gbl.Usrs.LstUsrs[Rol_STD].NumUsrs +
	          Gbl.Usrs.LstUsrs[Rol_NET].NumUsrs +
	          Gbl.Usrs.LstUsrs[Rol_TCH].NumUsrs;

   /***** Start box *****/
   snprintf (Gbl.Title,sizeof (Gbl.Title),
	     Txt_Statistics_of_visits_to_the_course_X,
	     Gbl.Hierarchy.Crs.ShrtName);
   Box_StartBox (NULL,Gbl.Title,NULL,
                 Hlp_ANALYTICS_Visits_visits_to_course,Box_NOT_CLOSABLE);

   /***** Show form to select the groups *****/
   Grp_ShowFormToSelectSeveralGroups (NULL,
	                              Grp_MY_GROUPS);

   /***** Start section with user list *****/
   Lay_StartSection (Usr_USER_LIST_SECTION_ID);

   if (NumTotalUsrs)
     {
      if (Usr_GetIfShowBigList (NumTotalUsrs,NULL,NULL))
        {
	 /***** Form to select type of list used for select several users *****/
	 Usr_ShowFormsToSelectUsrListType (NULL);

	 /***** Put link to register students *****/
         Enr_CheckStdsAndPutButtonToRegisterStdsInCurrentCrs ();

	 /***** Start form *****/
         Frm_StartFormAnchor (ActSeeAccCrs,Sta_STAT_RESULTS_SECTION_ID);

         Grp_PutParamsCodGrps ();
         Par_PutHiddenParamLong ("FirstRow",0);
         Par_PutHiddenParamLong ("LastRow",0);

         /***** Put list of users to select some of them *****/
         Tbl_TABLE_BeginCenterPadding (2);

         Tbl_TR_Begin (NULL);

         Tbl_TD_Begin ("class=\"RT %s\"",The_ClassFormInBox[Gbl.Prefs.Theme]);
         fprintf (Gbl.F.Out,"%s:",Txt_Users);
         Tbl_TD_End ();

	 Tbl_TD_Begin ("colspan=\"2\" class=\"%s LT\"",The_ClassFormInBox[Gbl.Prefs.Theme]);
         Tbl_TABLE_Begin (NULL);
         Usr_ListUsersToSelect (Rol_TCH);
         Usr_ListUsersToSelect (Rol_NET);
         Usr_ListUsersToSelect (Rol_STD);
         Tbl_TABLE_End ();
         Tbl_TD_End ();

         Tbl_TR_End ();

         /***** Initial and final dates of the search *****/
         Dat_PutFormStartEndClientLocalDateTimesWithYesterdayToday (Gbl.Action.Act == ActReqAccCrs);

         /***** Selection of action *****/
         Sta_WriteSelectorAction ();

         /***** Option a) Listing of clicks distributed by some metric *****/
         Tbl_TR_Begin (NULL);

         Tbl_TD_Begin ("class=\"RM %s\"",The_ClassFormInBox[Gbl.Prefs.Theme]);
         fprintf (Gbl.F.Out,"%s:",Txt_Show);
         Tbl_TD_End ();

	 Tbl_TD_Begin ("colspan=\"2\" class=\"LM\"");

         if ((Gbl.Stat.ClicksGroupedBy < Sta_CLICKS_CRS_PER_USR ||
              Gbl.Stat.ClicksGroupedBy > Sta_CLICKS_CRS_PER_ACTION) &&
              Gbl.Stat.ClicksGroupedBy != Sta_CLICKS_CRS_DETAILED_LIST)
            Gbl.Stat.ClicksGroupedBy = Sta_CLICKS_GROUPED_BY_DEFAULT;

         fprintf (Gbl.F.Out,"<input type=\"radio\""
                            " name=\"GroupedOrDetailed\" value=\"%u\"",
                  (unsigned) Sta_CLICKS_GROUPED);
         if (Gbl.Stat.ClicksGroupedBy != Sta_CLICKS_CRS_DETAILED_LIST)
            fprintf (Gbl.F.Out," checked=\"checked\"");
         fprintf (Gbl.F.Out," onclick=\"disableDetailedClicks()\" />");

         /* Selection of count type (number of pages generated, accesses per user, etc.) */
         Sta_WriteSelectorCountType ();

         fprintf (Gbl.F.Out,"<label class=\"%s\">&nbsp;%s&nbsp;"
                            "<select id=\"GroupedBy\" name=\"GroupedBy\">",
                  The_ClassFormInBox[Gbl.Prefs.Theme],Txt_distributed_by);
         for (ClicksGroupedBy = Sta_CLICKS_CRS_PER_USR;
              ClicksGroupedBy <= Sta_CLICKS_CRS_PER_ACTION;
              ClicksGroupedBy++)
           {
            fprintf (Gbl.F.Out,"<option value=\"%u\"",
                     (unsigned) ClicksGroupedBy);
            if (ClicksGroupedBy == Gbl.Stat.ClicksGroupedBy)
	       fprintf (Gbl.F.Out," selected=\"selected\"");
            fprintf (Gbl.F.Out,">%s",Txt_STAT_CLICKS_GROUPED_BY[ClicksGroupedBy]);
           }
         fprintf (Gbl.F.Out,"</select>"
                            "</label><br />");

         /***** Option b) Listing of detailed clicks to this course *****/
         fprintf (Gbl.F.Out,"<label>"
                            "<input type=\"radio\""
                            " name=\"GroupedOrDetailed\" value=\"%u\"",
                  (unsigned) Sta_CLICKS_DETAILED);
         if (Gbl.Stat.ClicksGroupedBy == Sta_CLICKS_CRS_DETAILED_LIST)
            fprintf (Gbl.F.Out," checked=\"checked\"");
         fprintf (Gbl.F.Out," onclick=\"enableDetailedClicks()\" />"
                            "%s"
                            "</label>",
                  Txt_STAT_CLICKS_GROUPED_BY[Sta_CLICKS_CRS_DETAILED_LIST]);

         /* Number of rows per page */
         // To use getElementById in Firefox, it's necessary to have the id attribute
         fprintf (Gbl.F.Out," "
                            "<label>"
                            "(%s: <select id=\"RowsPage\" name=\"RowsPage\"",
                  Txt_results_per_page);
         if (Gbl.Stat.ClicksGroupedBy != Sta_CLICKS_CRS_DETAILED_LIST)
            fprintf (Gbl.F.Out," disabled=\"disabled\"");
         fprintf (Gbl.F.Out,">");
         for (i = 0;
              i < NUM_OPTIONS_ROWS_PER_PAGE;
              i++)
           {
            fprintf (Gbl.F.Out,"<option");
            if (RowsPerPage[i] == Gbl.Stat.RowsPerPage)
	       fprintf (Gbl.F.Out," selected=\"selected\"");
            fprintf (Gbl.F.Out,">%lu",RowsPerPage[i]);
           }
         fprintf (Gbl.F.Out,"</select>)"
                            "</label>");
         Tbl_TD_End ();

         Tbl_TR_End ();
         Tbl_TABLE_End ();

	 /***** Hidden param used to get client time zone *****/
	 Dat_PutHiddenParBrowserTZDiff ();

         /***** Send button *****/
	 Btn_PutConfirmButton (Txt_Show_hits);

         /***** End form *****/
         Frm_EndForm ();
        }
     }
   else	// No teachers nor students found
      Ale_ShowAlert (Ale_WARNING,Txt_No_teachers_or_students_found);

   /***** End section with user list *****/
   Lay_EndSection ();

   /***** End box *****/
   Box_EndBox ();

   /***** Free memory used by the lists *****/
   Usr_FreeUsrsList (Rol_TCH);
   Usr_FreeUsrsList (Rol_NET);
   Usr_FreeUsrsList (Rol_STD);

   /***** Free memory for list of selected groups *****/
   Grp_FreeListCodSelectedGrps ();
  }

/*****************************************************************************/
/********** Show a form to select the type of global stat of clics ***********/
/*****************************************************************************/

void Sta_AskShowGblHits (void)
  {
   extern const char *Hlp_ANALYTICS_Visits_global_visits;
   extern const char *The_ClassFormInBox[The_NUM_THEMES];
   extern const char *Txt_Statistics_of_all_visits;
   extern const char *Txt_Users;
   extern const char *Txt_ROLE_STATS[Sta_NUM_ROLES_STAT];
   extern const char *Txt_Scope;
   extern const char *Txt_Show;
   extern const char *Txt_distributed_by;
   extern const char *Txt_STAT_CLICKS_GROUPED_BY[Sta_NUM_CLICKS_GROUPED_BY];
   extern const char *Txt_Show_hits;
   Sta_Role_t RoleStat;
   Sta_ClicksGroupedBy_t ClicksGroupedBy;

   /***** Contextual links *****/
   fprintf (Gbl.F.Out,"<div class=\"CONTEXT_MENU\">");

   /* Put form to go to test edition and configuration */
   Sta_PutLinkToCourseHits ();

   /* Link to show last clicks in real time */
   Sta_PutLinkToLastClicks ();

   fprintf (Gbl.F.Out,"</div>");

   /***** Start form *****/
   Frm_StartFormAnchor (ActSeeAccGbl,Sta_STAT_RESULTS_SECTION_ID);

   /***** Start box and table *****/
   Box_StartBoxTable (NULL,Txt_Statistics_of_all_visits,NULL,
                      Hlp_ANALYTICS_Visits_global_visits,Box_NOT_CLOSABLE,2);

   /***** Start and end dates for the search *****/
   Dat_PutFormStartEndClientLocalDateTimesWithYesterdayToday (Gbl.Action.Act == ActReqAccGbl);

   /***** Users' roles whose accesses we want to see *****/
   Tbl_TR_Begin (NULL);

   Tbl_TD_Begin ("class=\"RM\"");
   fprintf (Gbl.F.Out,"<label for=\"Role\" class=\"%s\">%s:</label>",
            The_ClassFormInBox[Gbl.Prefs.Theme],Txt_Users);
   Tbl_TD_End ();

   Tbl_TD_Begin ("colspan=\"2\" class=\"LM\"");
   fprintf (Gbl.F.Out,"<select id=\"Role\" name=\"Role\">");
   for (RoleStat = (Sta_Role_t) 0;
	RoleStat < Sta_NUM_ROLES_STAT;
	RoleStat++)
     {
      fprintf (Gbl.F.Out,"<option value=\"%u\"",(unsigned) RoleStat);
      if (RoleStat == Gbl.Stat.Role)
	 fprintf (Gbl.F.Out," selected=\"selected\"");
      fprintf (Gbl.F.Out,">%s",Txt_ROLE_STATS[RoleStat]);
     }
   fprintf (Gbl.F.Out,"</select>");
   Tbl_TD_End ();

   Tbl_TR_End ();

   /***** Selection of action *****/
   Sta_WriteSelectorAction ();

   /***** Clicks made from anywhere, current centre, current degree or current course *****/
   Tbl_TR_Begin (NULL);

   Tbl_TD_Begin ("class=\"RM\"");
   fprintf (Gbl.F.Out,"<label for=\"ScopeSta\" class=\"%s\">%s:</label>",
            The_ClassFormInBox[Gbl.Prefs.Theme],Txt_Scope);
   Tbl_TD_End ();

   Tbl_TD_Begin ("colspan=\"2\" class=\"LM\"");
   Gbl.Scope.Allowed = 1 << Hie_SYS |
	               1 << Hie_CTY |
		       1 << Hie_INS |
		       1 << Hie_CTR |
		       1 << Hie_DEG |
		       1 << Hie_CRS;
   Gbl.Scope.Default = Hie_SYS;
   Sco_GetScope ("ScopeSta");
   Sco_PutSelectorScope ("ScopeSta",false);
   Tbl_TD_End ();

   Tbl_TR_End ();

   /***** Count type for the statistic *****/
   Tbl_TR_Begin (NULL);

   Tbl_TD_Begin ("class=\"RM\"");
   fprintf (Gbl.F.Out,"<label for=\"CountType\" class=\"%s\">%s:</label>",
            The_ClassFormInBox[Gbl.Prefs.Theme],Txt_Show);
   Tbl_TD_End ();

   Tbl_TD_Begin ("colspan=\"2\" class=\"LM\"");
   Sta_WriteSelectorCountType ();

   /***** Type of statistic *****/
   fprintf (Gbl.F.Out,"<label class=\"%s\">&nbsp;%s&nbsp;",
            The_ClassFormInBox[Gbl.Prefs.Theme],Txt_distributed_by);

   if (Gbl.Stat.ClicksGroupedBy < Sta_CLICKS_GBL_PER_DAY ||
       Gbl.Stat.ClicksGroupedBy > Sta_CLICKS_GBL_PER_COURSE)
      Gbl.Stat.ClicksGroupedBy = Sta_CLICKS_GBL_PER_DAY;

   fprintf (Gbl.F.Out,"<select name=\"GroupedBy\">");
   for (ClicksGroupedBy = Sta_CLICKS_GBL_PER_DAY;
	ClicksGroupedBy <= Sta_CLICKS_GBL_PER_COURSE;
	ClicksGroupedBy++)
     {
      fprintf (Gbl.F.Out,"<option value=\"%u\"",
	       (unsigned) ClicksGroupedBy);
      if (ClicksGroupedBy == Gbl.Stat.ClicksGroupedBy)
	 fprintf (Gbl.F.Out," selected=\"selected\"");
      fprintf (Gbl.F.Out,">%s",Txt_STAT_CLICKS_GROUPED_BY[ClicksGroupedBy]);
     }
   fprintf (Gbl.F.Out,"</select>"
	              "</label>");
   Tbl_TD_End ();

   Tbl_TR_End ();

   /***** End table *****/
   Tbl_TABLE_End ();

   /***** Hidden param used to get client time zone *****/
   Dat_PutHiddenParBrowserTZDiff ();

   /***** Send button and end box *****/
   Box_EndBoxWithButton (Btn_CONFIRM_BUTTON,Txt_Show_hits);

   /***** End form *****/
   Frm_EndForm ();
  }

/*****************************************************************************/
/*************** Put a link to show visits to current course *****************/
/*****************************************************************************/

static void Sta_PutLinkToCourseHits (void)
  {
   extern const char *Txt_Visits_to_course;

   if (Gbl.Hierarchy.Level == Hie_CRS)		// Course selected
      switch (Gbl.Usrs.Me.Role.Logged)
        {
	 case Rol_NET:
	 case Rol_TCH:
	 case Rol_SYS_ADM:
	    Lay_PutContextualLinkIconText (ActReqAccCrs,NULL,NULL,
					   "chart-line.svg",
					   Txt_Visits_to_course);
	    break;
	 default:
	    break;
        }
  }

/*****************************************************************************/
/********************* Put a link to show global visits **********************/
/*****************************************************************************/

static void Sta_PutLinkToGlobalHits (void)
  {
   extern const char *Txt_Global_visits;

   Lay_PutContextualLinkIconText (ActReqAccGbl,NULL,NULL,
				  "chart-line.svg",
				  Txt_Global_visits);
  }

/*****************************************************************************/
/****** Put selectors for type of access count and for degree or course ******/
/*****************************************************************************/

static void Sta_WriteSelectorCountType (void)
  {
   extern const char *Txt_STAT_TYPE_COUNT_SMALL[Sta_NUM_COUNT_TYPES];
   Sta_CountType_t StatCountType;

   /**** Count type *****/
   fprintf (Gbl.F.Out,"<select id=\"CountType\" name=\"CountType\">");
   for (StatCountType = (Sta_CountType_t) 0;
	StatCountType < Sta_NUM_COUNT_TYPES;
	StatCountType++)
     {
      fprintf (Gbl.F.Out,"<option value=\"%u\"",(unsigned) StatCountType);
      if (StatCountType == Gbl.Stat.CountType)
	 fprintf (Gbl.F.Out," selected=\"selected\"");
      fprintf (Gbl.F.Out,">%s",Txt_STAT_TYPE_COUNT_SMALL[StatCountType]);
     }
   fprintf (Gbl.F.Out,"</select>");
  }

/*****************************************************************************/
/****** Put selectors for type of access count and for degree or course ******/
/*****************************************************************************/

static void Sta_WriteSelectorAction (void)
  {
   extern const char *The_ClassFormInBox[The_NUM_THEMES];
   extern const char *Txt_Action;
   extern const char *Txt_TABS_TXT[Tab_NUM_TABS];
   Act_Action_t Action;
   Tab_Tab_t Tab;
   char ActTxt[Act_MAX_BYTES_ACTION_TXT + 1];

   Tbl_TR_Begin (NULL);

   Tbl_TD_Begin ("class=\"RM\"");
   fprintf (Gbl.F.Out,"<label for=\"StatAct\" class=\"%s\">%s:</label>",
            The_ClassFormInBox[Gbl.Prefs.Theme],Txt_Action);
   Tbl_TD_End ();

   Tbl_TD_Begin ("colspan=\"2\" class=\"LM\"");
   fprintf (Gbl.F.Out,"<select id=\"StatAct\" name=\"StatAct\""
                      " style=\"width:375px;\">");
   for (Action = (Act_Action_t) 0;
	Action < Act_NUM_ACTIONS;
	Action++)
     {
      fprintf (Gbl.F.Out,"<option value=\"%u\"",(unsigned) Action);
      if (Action == Gbl.Stat.NumAction)
	 fprintf (Gbl.F.Out," selected=\"selected\"");
      fprintf (Gbl.F.Out,">");
      if (Action)
         fprintf (Gbl.F.Out,"%u: ",(unsigned) Action);
      Tab = Act_GetTab (Act_GetSuperAction (Action));
      if (Txt_TABS_TXT[Tab])
         fprintf (Gbl.F.Out,"%s &gt; ",Txt_TABS_TXT[Tab]);
      fprintf (Gbl.F.Out,"%s",
               Act_GetActionTextFromDB (Act_GetActCod (Action),ActTxt));
     }
   fprintf (Gbl.F.Out,"</select>");
   Tbl_TD_End ();

   Tbl_TR_End ();
  }

/*****************************************************************************/
/************ Set end date to current date                        ************/
/************ and set initial date to end date minus several days ************/
/*****************************************************************************/

void Sta_SetIniEndDates (void)
  {
   Gbl.DateRange.TimeUTC[0] = Gbl.StartExecutionTimeUTC - ((Cfg_DAYS_IN_RECENT_LOG - 1) * 24 * 60 * 60);
   Gbl.DateRange.TimeUTC[1] = Gbl.StartExecutionTimeUTC;
  }

/*****************************************************************************/
/******************** Compute and show access statistics *********************/
/*****************************************************************************/

void Sta_SeeGblAccesses (void)
  {
   Sta_ShowHits (Sta_SHOW_GLOBAL_ACCESSES);
  }

void Sta_SeeCrsAccesses (void)
  {
   Sta_ShowHits (Sta_SHOW_COURSE_ACCESSES);
  }

/*****************************************************************************/
/******************** Compute and show access statistics ********************/
/*****************************************************************************/

#define Sta_MAX_BYTES_QUERY_ACCESS (1024 + (10 + ID_MAX_BYTES_USR_ID) * 5000 - 1)

#define Sta_MAX_BYTES_COUNT_TYPE (256 - 1)

static void Sta_ShowHits (Sta_GlobalOrCourseAccesses_t GlobalOrCourse)
  {
   extern const char *Txt_You_must_select_one_ore_more_users;
   extern const char *Txt_There_is_no_knowing_how_many_users_not_logged_have_accessed;
   extern const char *Txt_The_date_range_must_be_less_than_or_equal_to_X_days;
   extern const char *Txt_There_are_no_accesses_with_the_selected_search_criteria;
   extern const char *Txt_List_of_detailed_clicks;
   extern const char *Txt_STAT_TYPE_COUNT_CAPS[Sta_NUM_COUNT_TYPES];
   extern const char *Txt_Time_zone_used_in_the_calculation_of_these_statistics;
   char *Query = NULL;
   char QueryAux[512];
   long LengthQuery;
   MYSQL_RES *mysql_res;
   unsigned long NumRows;
   const char *LogTable;
   Sta_ClicksDetailedOrGrouped_t DetailedOrGrouped = Sta_CLICKS_GROUPED;
   struct UsrData UsrDat;
   char BrowserTimeZone[Dat_MAX_BYTES_TIME_ZONE + 1];
   unsigned NumUsr = 0;
   const char *Ptr;
   char StrRole[256];
   char StrQueryCountType[Sta_MAX_BYTES_COUNT_TYPE + 1];
   unsigned NumDays;
   bool ICanQueryWholeRange;

   /***** Get initial and ending dates *****/
   Dat_GetIniEndDatesFromForm ();

   /***** Get client time zone *****/
   Dat_GetBrowserTimeZone (BrowserTimeZone);

   /***** Set table where to find depending on initial date *****/
   /* If initial day is older than current day minus Cfg_DAYS_IN_RECENT_LOG,
      then use recent log table, else use historic log table */
   LogTable = (Dat_GetNumDaysBetweenDates (&Gbl.DateRange.DateIni.Date,&Gbl.Now.Date)
	       <= Cfg_DAYS_IN_RECENT_LOG) ? "log_recent" :
	                                    "log_full";

   /***** Get the type of stat of clicks ******/
   DetailedOrGrouped = (Sta_ClicksDetailedOrGrouped_t)
	               Par_GetParToUnsignedLong ("GroupedOrDetailed",
	                                         0,
	                                         Sta_NUM_CLICKS_DETAILED_OR_GROUPED - 1,
	                                         (unsigned long) Sta_CLICKS_DETAILED_OR_GROUPED_DEFAULT);

   if (DetailedOrGrouped == Sta_CLICKS_DETAILED)
      Gbl.Stat.ClicksGroupedBy = Sta_CLICKS_CRS_DETAILED_LIST;
   else	// DetailedOrGrouped == Sta_CLICKS_GROUPED
      Gbl.Stat.ClicksGroupedBy = (Sta_ClicksGroupedBy_t)
			         Par_GetParToUnsignedLong ("GroupedBy",
						           0,
						           Sta_NUM_CLICKS_GROUPED_BY - 1,
						           (unsigned long) Sta_CLICKS_GROUPED_BY_DEFAULT);

   /***** Get the type of count of clicks *****/
   if (Gbl.Stat.ClicksGroupedBy != Sta_CLICKS_CRS_DETAILED_LIST)
      Gbl.Stat.CountType = (Sta_CountType_t)
	                   Par_GetParToUnsignedLong ("CountType",
	                                             0,
	                                             Sta_NUM_COUNT_TYPES - 1,
	                                             (unsigned long) Sta_COUNT_TYPE_DEFAULT);

   /***** Get action *****/
   Gbl.Stat.NumAction = (Act_Action_t)
			Par_GetParToUnsignedLong ("StatAct",
					          0,
					          Act_NUM_ACTIONS - 1,
					          (unsigned long) Sta_NUM_ACTION_DEFAULT);

   switch (GlobalOrCourse)
     {
      case Sta_SHOW_GLOBAL_ACCESSES:
	 /***** Get the type of user of clicks *****/
	 Gbl.Stat.Role = (Sta_Role_t)
			 Par_GetParToUnsignedLong ("Role",
				                   0,
					           Sta_NUM_ROLES_STAT - 1,
				                   (unsigned long) Sta_ROLE_DEFAULT);

	 /***** Get users range for access statistics *****/
	 Gbl.Scope.Allowed = 1 << Hie_SYS |
			     1 << Hie_CTY |
			     1 << Hie_INS |
			     1 << Hie_CTR |
			     1 << Hie_DEG |
			     1 << Hie_CRS;
	 Gbl.Scope.Default = Hie_SYS;
	 Sco_GetScope ("ScopeSta");

	 /***** Show form again *****/
	 Sta_AskShowGblHits ();

	 /***** Start results section *****/
	 Lay_StartSection (Sta_STAT_RESULTS_SECTION_ID);

	 /***** Check selection *****/
	 if ((Gbl.Stat.Role == Sta_ROLE_ALL_USRS ||
	      Gbl.Stat.Role == Sta_ROLE_UNKNOWN_USRS) &&
	     (Gbl.Stat.CountType == Sta_DISTINCT_USRS ||
	      Gbl.Stat.CountType == Sta_CLICKS_PER_USR))	// These types of query will never give a valid result
	   {
	    /* Write warning message and abort */
	    Ale_ShowAlert (Ale_WARNING,Txt_There_is_no_knowing_how_many_users_not_logged_have_accessed);
	    return;
	   }
	 break;
      case Sta_SHOW_COURSE_ACCESSES:
	 if (Gbl.Stat.ClicksGroupedBy == Sta_CLICKS_CRS_DETAILED_LIST)
	   {
	    /****** Get the number of the first row to show ******/
	    Gbl.Stat.FirstRow = Par_GetParToUnsignedLong ("FirstRow",
	                                                  1,
	                                                  ULONG_MAX,
	                                                  0);

	    /****** Get the number of the last row to show ******/
	    Gbl.Stat.LastRow = Par_GetParToUnsignedLong ("LastRow",
	                                                 1,
	                                                 ULONG_MAX,
	                                                 0);

	    /****** Get the number of rows per page ******/
	    Gbl.Stat.RowsPerPage = Par_GetParToUnsignedLong ("RowsPage",
	                                                     Sta_MIN_ROWS_PER_PAGE,
	                                                     Sta_MAX_ROWS_PER_PAGE,
	                                                     Sta_DEF_ROWS_PER_PAGE);
	   }

	 /****** Get lists of selected users ******/
	 Usr_GetListsSelectedUsrsCods ();

	 /***** Show the form again *****/
	 Sta_AskShowCrsHits ();

	 /***** Start results section *****/
	 Lay_StartSection (Sta_STAT_RESULTS_SECTION_ID);

	 /***** Check selection *****/
	 if (!Usr_CountNumUsrsInListOfSelectedUsrs ())	// Error: there are no users selected
	   {
	    /* Write warning message, clean and abort */
	    Ale_ShowAlert (Ale_WARNING,Txt_You_must_select_one_ore_more_users);
            Usr_FreeListsSelectedUsrsCods ();
	    return;
	   }
	 break;
     }

   /***** Check if range of dates is forbidden for me *****/
   NumDays = Dat_GetNumDaysBetweenDates (&Gbl.DateRange.DateIni.Date,&Gbl.DateRange.DateEnd.Date);
   ICanQueryWholeRange = (Gbl.Usrs.Me.Role.Logged >= Rol_TCH && GlobalOrCourse == Sta_SHOW_COURSE_ACCESSES) ||
			 (Gbl.Usrs.Me.Role.Logged == Rol_TCH     &&  Gbl.Scope.Current == Hie_CRS)  ||
			 (Gbl.Usrs.Me.Role.Logged == Rol_DEG_ADM && (Gbl.Scope.Current == Hie_DEG   ||
			                                             Gbl.Scope.Current == Hie_CRS)) ||
			 (Gbl.Usrs.Me.Role.Logged == Rol_CTR_ADM && (Gbl.Scope.Current == Hie_CTR   ||
			                                             Gbl.Scope.Current == Hie_DEG   ||
			                                             Gbl.Scope.Current == Hie_CRS)) ||
			 (Gbl.Usrs.Me.Role.Logged == Rol_INS_ADM && (Gbl.Scope.Current == Hie_INS   ||
			                                             Gbl.Scope.Current == Hie_CTR   ||
			                                             Gbl.Scope.Current == Hie_DEG   ||
			                                             Gbl.Scope.Current == Hie_CRS)) ||
			  Gbl.Usrs.Me.Role.Logged == Rol_SYS_ADM;
   if (!ICanQueryWholeRange && NumDays > Cfg_DAYS_IN_RECENT_LOG)
     {
      /* ...write warning message and show the form again */
      Ale_ShowAlert (Ale_WARNING,Txt_The_date_range_must_be_less_than_or_equal_to_X_days,
	             Cfg_DAYS_IN_RECENT_LOG);
      return;
     }

   /***** Query depending on the type of count *****/
   switch (Gbl.Stat.CountType)
     {
      case Sta_TOTAL_CLICKS:
         Str_Copy (StrQueryCountType,"COUNT(*)",
                   Sta_MAX_BYTES_COUNT_TYPE);
	 break;
      case Sta_DISTINCT_USRS:
         sprintf (StrQueryCountType,"COUNT(DISTINCT(%s.UsrCod))",LogTable);
	 break;
      case Sta_CLICKS_PER_USR:
         sprintf (StrQueryCountType,"COUNT(*)/GREATEST(COUNT(DISTINCT(%s.UsrCod)),1)+0.000000",LogTable);
	 break;
      case Sta_GENERATION_TIME:
         sprintf (StrQueryCountType,"(AVG(%s.TimeToGenerate)/1E6)+0.000000",LogTable);
	 break;
      case Sta_SEND_TIME:
         sprintf (StrQueryCountType,"(AVG(%s.TimeToSend)/1E6)+0.000000",LogTable);
	 break;
     }

   /***** Select clicks from the table of log *****/
   /* Allocate memory for the query */
   if ((Query = (char *) malloc (Sta_MAX_BYTES_QUERY_ACCESS + 1)) == NULL)
      Lay_NotEnoughMemoryExit ();

   /* Start the query */
   switch (Gbl.Stat.ClicksGroupedBy)
     {
      case Sta_CLICKS_CRS_DETAILED_LIST:
   	 snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           "SELECT SQL_NO_CACHE LogCod,UsrCod,Role,"
   		   "UNIX_TIMESTAMP(ClickTime) AS F,ActCod FROM %s",
                   LogTable);
	 break;
      case Sta_CLICKS_CRS_PER_USR:
	 snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           "SELECT SQL_NO_CACHE UsrCod,%s AS Num FROM %s",
                   StrQueryCountType,LogTable);
	 break;
      case Sta_CLICKS_CRS_PER_DAY:
      case Sta_CLICKS_GBL_PER_DAY:
         snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           "SELECT SQL_NO_CACHE "
                   "DATE_FORMAT(CONVERT_TZ(ClickTime,@@session.time_zone,'%s'),'%%Y%%m%%d') AS Day,"
                   "%s FROM %s",
                   BrowserTimeZone,
                   StrQueryCountType,LogTable);
	 break;
      case Sta_CLICKS_CRS_PER_DAY_AND_HOUR:
      case Sta_CLICKS_GBL_PER_DAY_AND_HOUR:
         snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           "SELECT SQL_NO_CACHE "
                   "DATE_FORMAT(CONVERT_TZ(ClickTime,@@session.time_zone,'%s'),'%%Y%%m%%d') AS Day,"
                   "DATE_FORMAT(CONVERT_TZ(ClickTime,@@session.time_zone,'%s'),'%%H') AS Hour,"
                   "%s FROM %s",
                   BrowserTimeZone,
                   BrowserTimeZone,
                   StrQueryCountType,LogTable);
	 break;
      case Sta_CLICKS_CRS_PER_WEEK:
      case Sta_CLICKS_GBL_PER_WEEK:
	 /* With %x%v the weeks are counted from monday to sunday.
	    With %X%V the weeks are counted from sunday to saturday. */
	 snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           (Gbl.Prefs.FirstDayOfWeek == 0) ?
	           "SELECT SQL_NO_CACHE "	// Weeks start on monday
		   "DATE_FORMAT(CONVERT_TZ(ClickTime,@@session.time_zone,'%s'),'%%x%%v') AS Week,"
		   "%s FROM %s" :
		   "SELECT SQL_NO_CACHE "	// Weeks start on sunday
		   "DATE_FORMAT(CONVERT_TZ(ClickTime,@@session.time_zone,'%s'),'%%X%%V') AS Week,"
		   "%s FROM %s",
		   BrowserTimeZone,
		   StrQueryCountType,LogTable);
	 break;
      case Sta_CLICKS_CRS_PER_MONTH:
      case Sta_CLICKS_GBL_PER_MONTH:
         snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           "SELECT SQL_NO_CACHE "
                   "DATE_FORMAT(CONVERT_TZ(ClickTime,@@session.time_zone,'%s'),'%%Y%%m') AS Month,"
                   "%s FROM %s",
                   BrowserTimeZone,
                   StrQueryCountType,LogTable);
	 break;
      case Sta_CLICKS_CRS_PER_YEAR:
      case Sta_CLICKS_GBL_PER_YEAR:
         snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           "SELECT SQL_NO_CACHE "
                   "DATE_FORMAT(CONVERT_TZ(ClickTime,@@session.time_zone,'%s'),'%%Y') AS Year,"
                   "%s FROM %s",
                   BrowserTimeZone,
                   StrQueryCountType,LogTable);
	 break;
      case Sta_CLICKS_CRS_PER_HOUR:
      case Sta_CLICKS_GBL_PER_HOUR:
         snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           "SELECT SQL_NO_CACHE "
                   "DATE_FORMAT(CONVERT_TZ(ClickTime,@@session.time_zone,'%s'),'%%H') AS Hour,"
                   "%s FROM %s",
                   BrowserTimeZone,
                   StrQueryCountType,LogTable);
	 break;
      case Sta_CLICKS_CRS_PER_MINUTE:
      case Sta_CLICKS_GBL_PER_MINUTE:
         snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           "SELECT SQL_NO_CACHE "
                   "DATE_FORMAT(CONVERT_TZ(ClickTime,@@session.time_zone,'%s'),'%%H%%i') AS Minute,"
                   "%s FROM %s",
                   BrowserTimeZone,
                   StrQueryCountType,LogTable);
	 break;
      case Sta_CLICKS_CRS_PER_ACTION:
      case Sta_CLICKS_GBL_PER_ACTION:
         snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           "SELECT SQL_NO_CACHE ActCod,%s AS Num FROM %s",
                   StrQueryCountType,LogTable);
	 break;
      case Sta_CLICKS_GBL_PER_PLUGIN:
         snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           "SELECT SQL_NO_CACHE log_ws.PlgCod,%s AS Num FROM %s,log_ws",
                   StrQueryCountType,LogTable);
         break;
      case Sta_CLICKS_GBL_PER_API_FUNCTION:
         snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           "SELECT SQL_NO_CACHE log_ws.FunCod,%s AS Num FROM %s,log_ws",
                   StrQueryCountType,LogTable);
         break;
      case Sta_CLICKS_GBL_PER_BANNER:
         snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           "SELECT SQL_NO_CACHE log_banners.BanCod,%s AS Num FROM %s,log_banners",
                   StrQueryCountType,LogTable);
         break;
      case Sta_CLICKS_GBL_PER_COUNTRY:
         snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           "SELECT SQL_NO_CACHE CtyCod,%s AS Num FROM %s",
                   StrQueryCountType,LogTable);
	 break;
      case Sta_CLICKS_GBL_PER_INSTITUTION:
         snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           "SELECT SQL_NO_CACHE InsCod,%s AS Num FROM %s",
                   StrQueryCountType,LogTable);
	 break;
      case Sta_CLICKS_GBL_PER_CENTRE:
         snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           "SELECT SQL_NO_CACHE CtrCod,%s AS Num FROM %s",
                   StrQueryCountType,LogTable);
	 break;
      case Sta_CLICKS_GBL_PER_DEGREE:
         snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           "SELECT SQL_NO_CACHE DegCod,%s AS Num FROM %s",
                   StrQueryCountType,LogTable);
	 break;
      case Sta_CLICKS_GBL_PER_COURSE:
	 snprintf (Query,Sta_MAX_BYTES_QUERY_ACCESS + 1,
   	           "SELECT SQL_NO_CACHE CrsCod,%s AS Num FROM %s",
                   StrQueryCountType,LogTable);
	 break;
     }
   sprintf (QueryAux," WHERE %s.ClickTime"
	             " BETWEEN FROM_UNIXTIME(%ld) AND FROM_UNIXTIME(%ld)",
            LogTable,
            (long) Gbl.DateRange.TimeUTC[0],
            (long) Gbl.DateRange.TimeUTC[1]);
   Str_Concat (Query,QueryAux,
               Sta_MAX_BYTES_QUERY_ACCESS);

   switch (GlobalOrCourse)
     {
      case Sta_SHOW_GLOBAL_ACCESSES:
	 /* Scope */
	 switch (Gbl.Scope.Current)
	   {
	    case Hie_UNK:
	    case Hie_SYS:
               break;
	    case Hie_CTY:
               if (Gbl.Hierarchy.Cty.CtyCod > 0)
		 {
		  sprintf (QueryAux," AND %s.CtyCod=%ld",
			   LogTable,Gbl.Hierarchy.Cty.CtyCod);
		  Str_Concat (Query,QueryAux,
		              Sta_MAX_BYTES_QUERY_ACCESS);
		 }
               break;
	    case Hie_INS:
	       if (Gbl.Hierarchy.Ins.InsCod > 0)
		 {
		  sprintf (QueryAux," AND %s.InsCod=%ld",
			   LogTable,Gbl.Hierarchy.Ins.InsCod);
		  Str_Concat (Query,QueryAux,
		              Sta_MAX_BYTES_QUERY_ACCESS);
		 }
	       break;
	    case Hie_CTR:
               if (Gbl.Hierarchy.Ctr.CtrCod > 0)
		 {
		  sprintf (QueryAux," AND %s.CtrCod=%ld",
			   LogTable,Gbl.Hierarchy.Ctr.CtrCod);
		  Str_Concat (Query,QueryAux,
		              Sta_MAX_BYTES_QUERY_ACCESS);
		 }
               break;
	    case Hie_DEG:
	       if (Gbl.Hierarchy.Deg.DegCod > 0)
		 {
		  sprintf (QueryAux," AND %s.DegCod=%ld",
			   LogTable,Gbl.Hierarchy.Deg.DegCod);
		  Str_Concat (Query,QueryAux,
		              Sta_MAX_BYTES_QUERY_ACCESS);
		 }
	       break;
	    case Hie_CRS:
	       if (Gbl.Hierarchy.Level == Hie_CRS)
		 {
		  sprintf (QueryAux," AND %s.CrsCod=%ld",
			   LogTable,Gbl.Hierarchy.Crs.CrsCod);
		  Str_Concat (Query,QueryAux,
		              Sta_MAX_BYTES_QUERY_ACCESS);
		 }
	       break;
	   }

         /* Type of users */
	 switch (Gbl.Stat.Role)
	   {
	    case Sta_ROLE_IDENTIFIED_USRS:
               sprintf (StrRole," AND %s.Role<>%u",
                        LogTable,(unsigned) Rol_UNK);
	       break;
	    case Sta_ROLE_ALL_USRS:
               switch (Gbl.Stat.CountType)
                 {
                  case Sta_TOTAL_CLICKS:
                  case Sta_GENERATION_TIME:
                  case Sta_SEND_TIME:
                     StrRole[0] = '\0';
	             break;
                  case Sta_DISTINCT_USRS:
                  case Sta_CLICKS_PER_USR:
                     sprintf (StrRole," AND %s.Role<>%u",
                              LogTable,(unsigned) Rol_UNK);
                     break;
                    }
	       break;
	    case Sta_ROLE_INS_ADMINS:
               sprintf (StrRole," AND %s.Role=%u",
                        LogTable,(unsigned) Rol_INS_ADM);
	       break;
	    case Sta_ROLE_CTR_ADMINS:
               sprintf (StrRole," AND %s.Role=%u",
                        LogTable,(unsigned) Rol_CTR_ADM);
	       break;
	    case Sta_ROLE_DEG_ADMINS:
               sprintf (StrRole," AND %s.Role=%u",
                        LogTable,(unsigned) Rol_DEG_ADM);
	       break;
	    case Sta_ROLE_TEACHERS:
               sprintf (StrRole," AND %s.Role=%u",
                        LogTable,(unsigned) Rol_TCH);
	       break;
	    case Sta_ROLE_NON_EDITING_TEACHERS:
               sprintf (StrRole," AND %s.Role=%u",
                        LogTable,(unsigned) Rol_NET);
	       break;
	    case Sta_ROLE_STUDENTS:
               sprintf (StrRole," AND %s.Role=%u",
                        LogTable,(unsigned) Rol_STD);
	       break;
	    case Sta_ROLE_USERS:
               sprintf (StrRole," AND %s.Role=%u",
                        LogTable,(unsigned) Rol_USR);
               break;
	    case Sta_ROLE_GUESTS:
               sprintf (StrRole," AND %s.Role=%u",
                        LogTable,(unsigned) Rol_GST);
               break;
	    case Sta_ROLE_UNKNOWN_USRS:
               sprintf (StrRole," AND %s.Role=%u",
                        LogTable,(unsigned) Rol_UNK);
               break;
	    case Sta_ROLE_ME:
               sprintf (StrRole," AND %s.UsrCod=%ld",
                        LogTable,Gbl.Usrs.Me.UsrDat.UsrCod);
	       break;
	   }
         Str_Concat (Query,StrRole,
                     Sta_MAX_BYTES_QUERY_ACCESS);

         switch (Gbl.Stat.ClicksGroupedBy)
           {
            case Sta_CLICKS_GBL_PER_PLUGIN:
            case Sta_CLICKS_GBL_PER_API_FUNCTION:
               sprintf (QueryAux," AND %s.LogCod=log_ws.LogCod",
                        LogTable);
               Str_Concat (Query,QueryAux,
                           Sta_MAX_BYTES_QUERY_ACCESS);
               break;
            case Sta_CLICKS_GBL_PER_BANNER:
               sprintf (QueryAux," AND %s.LogCod=log_banners.LogCod",
                        LogTable);
               Str_Concat (Query,QueryAux,
                           Sta_MAX_BYTES_QUERY_ACCESS);
               break;
            default:
               break;
           }
	 break;
      case Sta_SHOW_COURSE_ACCESSES:
         sprintf (QueryAux," AND %s.CrsCod=%ld",
                  LogTable,Gbl.Hierarchy.Crs.CrsCod);
	 Str_Concat (Query,QueryAux,
	             Sta_MAX_BYTES_QUERY_ACCESS);

	 /***** Initialize data structure of the user *****/
         Usr_UsrDataConstructor (&UsrDat);

	 LengthQuery = strlen (Query);
	 NumUsr = 0;
	 Ptr = Gbl.Usrs.Selected.List[Rol_UNK];
	 while (*Ptr)
	   {
	    Par_GetNextStrUntilSeparParamMult (&Ptr,UsrDat.EncryptedUsrCod,
	                                       Cry_BYTES_ENCRYPTED_STR_SHA256_BASE64);
            Usr_GetUsrCodFromEncryptedUsrCod (&UsrDat);
	    if (UsrDat.UsrCod > 0)
	      {
	       LengthQuery = LengthQuery + 25 + 10 + 1;
	       if (LengthQuery > Sta_MAX_BYTES_QUERY_ACCESS - 128)
                  Lay_ShowErrorAndExit ("Query is too large.");
               sprintf (QueryAux,
                        NumUsr ? " OR %s.UsrCod=%ld" :
                                 " AND (%s.UsrCod=%ld",
                        LogTable,UsrDat.UsrCod);
	       Str_Concat (Query,QueryAux,
	                   Sta_MAX_BYTES_QUERY_ACCESS);
	       NumUsr++;
	      }
	   }
	 Str_Concat (Query,")",
	             Sta_MAX_BYTES_QUERY_ACCESS);

	 /***** Free memory used by the data of the user *****/
         Usr_UsrDataDestructor (&UsrDat);
	 break;
     }

   /* Select action */
   if (Gbl.Stat.NumAction != ActAll)
     {
      sprintf (QueryAux," AND %s.ActCod=%ld",
               LogTable,Act_GetActCod (Gbl.Stat.NumAction));
      Str_Concat (Query,QueryAux,
                  Sta_MAX_BYTES_QUERY_ACCESS);
     }

   /* End the query */
   switch (Gbl.Stat.ClicksGroupedBy)
     {
      case Sta_CLICKS_CRS_DETAILED_LIST:
	 Str_Concat (Query," ORDER BY F",
	             Sta_MAX_BYTES_QUERY_ACCESS);
	 break;
      case Sta_CLICKS_CRS_PER_USR:
	 sprintf (QueryAux," GROUP BY %s.UsrCod ORDER BY Num DESC",LogTable);
         Str_Concat (Query,QueryAux,
                     Sta_MAX_BYTES_QUERY_ACCESS);
	 break;
      case Sta_CLICKS_CRS_PER_DAY:
      case Sta_CLICKS_GBL_PER_DAY:
	 Str_Concat (Query," GROUP BY Day DESC",
	             Sta_MAX_BYTES_QUERY_ACCESS);
	 break;
      case Sta_CLICKS_CRS_PER_DAY_AND_HOUR:
      case Sta_CLICKS_GBL_PER_DAY_AND_HOUR:
	 Str_Concat (Query," GROUP BY Day DESC,Hour",
	             Sta_MAX_BYTES_QUERY_ACCESS);
	 break;
      case Sta_CLICKS_CRS_PER_WEEK:
      case Sta_CLICKS_GBL_PER_WEEK:
	 Str_Concat (Query," GROUP BY Week DESC",
	             Sta_MAX_BYTES_QUERY_ACCESS);
	 break;
      case Sta_CLICKS_CRS_PER_MONTH:
      case Sta_CLICKS_GBL_PER_MONTH:
	 Str_Concat (Query," GROUP BY Month DESC",
	             Sta_MAX_BYTES_QUERY_ACCESS);
	 break;
      case Sta_CLICKS_CRS_PER_YEAR:
      case Sta_CLICKS_GBL_PER_YEAR:
	 Str_Concat (Query," GROUP BY Year DESC",
	             Sta_MAX_BYTES_QUERY_ACCESS);
	 break;
      case Sta_CLICKS_CRS_PER_HOUR:
      case Sta_CLICKS_GBL_PER_HOUR:
	 Str_Concat (Query," GROUP BY Hour",
	             Sta_MAX_BYTES_QUERY_ACCESS);
	 break;
      case Sta_CLICKS_CRS_PER_MINUTE:
      case Sta_CLICKS_GBL_PER_MINUTE:
	 Str_Concat (Query," GROUP BY Minute",
	             Sta_MAX_BYTES_QUERY_ACCESS);
	 break;
      case Sta_CLICKS_CRS_PER_ACTION:
      case Sta_CLICKS_GBL_PER_ACTION:
	 sprintf (QueryAux," GROUP BY %s.ActCod ORDER BY Num DESC",LogTable);
         Str_Concat (Query,QueryAux,
                     Sta_MAX_BYTES_QUERY_ACCESS);
	 break;
      case Sta_CLICKS_GBL_PER_PLUGIN:
         Str_Concat (Query," GROUP BY log_ws.PlgCod ORDER BY Num DESC",
                     Sta_MAX_BYTES_QUERY_ACCESS);
         break;
      case Sta_CLICKS_GBL_PER_API_FUNCTION:
         Str_Concat (Query," GROUP BY log_ws.FunCod ORDER BY Num DESC",
                     Sta_MAX_BYTES_QUERY_ACCESS);
         break;
      case Sta_CLICKS_GBL_PER_BANNER:
         Str_Concat (Query," GROUP BY log_banners.BanCod ORDER BY Num DESC",
                     Sta_MAX_BYTES_QUERY_ACCESS);
         break;
      case Sta_CLICKS_GBL_PER_COUNTRY:
	 sprintf (QueryAux," GROUP BY %s.CtyCod ORDER BY Num DESC",LogTable);
         Str_Concat (Query,QueryAux,
                     Sta_MAX_BYTES_QUERY_ACCESS);
	 break;
      case Sta_CLICKS_GBL_PER_INSTITUTION:
	 sprintf (QueryAux," GROUP BY %s.InsCod ORDER BY Num DESC",LogTable);
         Str_Concat (Query,QueryAux,
                     Sta_MAX_BYTES_QUERY_ACCESS);
	 break;
      case Sta_CLICKS_GBL_PER_CENTRE:
	 sprintf (QueryAux," GROUP BY %s.CtrCod ORDER BY Num DESC",LogTable);
         Str_Concat (Query,QueryAux,
                     Sta_MAX_BYTES_QUERY_ACCESS);
	 break;
      case Sta_CLICKS_GBL_PER_DEGREE:
	 sprintf (QueryAux," GROUP BY %s.DegCod ORDER BY Num DESC",LogTable);
         Str_Concat (Query,QueryAux,
                     Sta_MAX_BYTES_QUERY_ACCESS);
	 break;
      case Sta_CLICKS_GBL_PER_COURSE:
	 sprintf (QueryAux," GROUP BY %s.CrsCod ORDER BY Num DESC",LogTable);
         Str_Concat (Query,QueryAux,
                     Sta_MAX_BYTES_QUERY_ACCESS);
	 break;
     }
   /***** Write query for debug *****/
   /*
   if (Gbl.Usrs.Me.Role.Logged == Rol_SYS_ADM)
      Ale_ShowFixedAlert (Ale_INFO,Query);
   */
   /***** Make the query *****/
   NumRows = DB_QuerySELECT (&mysql_res,"can not get clicks",
			     "%s",
			     Query);

   /***** Count the number of rows in result *****/
   if (NumRows == 0)
      Ale_ShowAlert (Ale_INFO,Txt_There_are_no_accesses_with_the_selected_search_criteria);
   else
     {
      /***** Put the table with the clicks *****/
      if (Gbl.Stat.ClicksGroupedBy == Sta_CLICKS_CRS_DETAILED_LIST)
	 Box_StartBox ("100%",Txt_List_of_detailed_clicks,NULL,
	               NULL,Box_NOT_CLOSABLE);
      else
	 Box_StartBox (NULL,Txt_STAT_TYPE_COUNT_CAPS[Gbl.Stat.CountType],NULL,
	               NULL,Box_NOT_CLOSABLE);

      Tbl_TABLE_BeginPadding (Sta_CellPadding[Gbl.Stat.ClicksGroupedBy]);
      switch (Gbl.Stat.ClicksGroupedBy)
	{
	 case Sta_CLICKS_CRS_DETAILED_LIST:
	    Sta_ShowDetailedAccessesList (NumRows,mysql_res);
	    break;
	 case Sta_CLICKS_CRS_PER_USR:
	    Sta_ShowNumHitsPerUsr (NumRows,mysql_res);
	    break;
	 case Sta_CLICKS_CRS_PER_DAY:
	 case Sta_CLICKS_GBL_PER_DAY:
	    Sta_ShowNumHitsPerDay (NumRows,mysql_res);
	    break;
	 case Sta_CLICKS_CRS_PER_DAY_AND_HOUR:
	 case Sta_CLICKS_GBL_PER_DAY_AND_HOUR:
	    Sta_ShowDistrAccessesPerDayAndHour (NumRows,mysql_res);
	    break;
	 case Sta_CLICKS_CRS_PER_WEEK:
	 case Sta_CLICKS_GBL_PER_WEEK:
	    Sta_ShowNumHitsPerWeek (NumRows,mysql_res);
	    break;
	 case Sta_CLICKS_CRS_PER_MONTH:
	 case Sta_CLICKS_GBL_PER_MONTH:
	    Sta_ShowNumHitsPerMonth (NumRows,mysql_res);
	    break;
	 case Sta_CLICKS_CRS_PER_YEAR:
	 case Sta_CLICKS_GBL_PER_YEAR:
	    Sta_ShowNumHitsPerYear (NumRows,mysql_res);
	    break;
	 case Sta_CLICKS_CRS_PER_HOUR:
	 case Sta_CLICKS_GBL_PER_HOUR:
	    Sta_ShowNumHitsPerHour (NumRows,mysql_res);
	    break;
	 case Sta_CLICKS_CRS_PER_MINUTE:
	 case Sta_CLICKS_GBL_PER_MINUTE:
	    Sta_ShowAverageAccessesPerMinute (NumRows,mysql_res);
	    break;
	 case Sta_CLICKS_CRS_PER_ACTION:
	 case Sta_CLICKS_GBL_PER_ACTION:
	    Sta_ShowNumHitsPerAction (NumRows,mysql_res);
	    break;
         case Sta_CLICKS_GBL_PER_PLUGIN:
            Sta_ShowNumHitsPerPlugin (NumRows,mysql_res);
            break;
         case Sta_CLICKS_GBL_PER_API_FUNCTION:
            Sta_ShowNumHitsPerWSFunction (NumRows,mysql_res);
            break;
         case Sta_CLICKS_GBL_PER_BANNER:
            Sta_ShowNumHitsPerBanner (NumRows,mysql_res);
            break;
         case Sta_CLICKS_GBL_PER_COUNTRY:
	    Sta_ShowNumHitsPerCountry (NumRows,mysql_res);
	    break;
         case Sta_CLICKS_GBL_PER_INSTITUTION:
	    Sta_ShowNumHitsPerInstitution (NumRows,mysql_res);
	    break;
         case Sta_CLICKS_GBL_PER_CENTRE:
	    Sta_ShowNumHitsPerCentre (NumRows,mysql_res);
	    break;
	 case Sta_CLICKS_GBL_PER_DEGREE:
	    Sta_ShowNumHitsPerDegree (NumRows,mysql_res);
	    break;
	 case Sta_CLICKS_GBL_PER_COURSE:
	    Sta_ShowNumHitsPerCourse (NumRows,mysql_res);
	    break;
	}
      Tbl_TABLE_End ();

      /* End box and section */
      Box_EndBox ();
      Lay_EndSection ();
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   /***** Free memory used by list of selected users' codes *****/
   if (Gbl.Action.Act == ActSeeAccCrs)
      Usr_FreeListsSelectedUsrsCods ();

   /***** Write time zone used in the calculation of these statistics *****/
   switch (Gbl.Stat.ClicksGroupedBy)
     {
      case Sta_CLICKS_CRS_PER_DAY:
      case Sta_CLICKS_GBL_PER_DAY:
      case Sta_CLICKS_CRS_PER_DAY_AND_HOUR:
      case Sta_CLICKS_GBL_PER_DAY_AND_HOUR:
      case Sta_CLICKS_CRS_PER_WEEK:
      case Sta_CLICKS_GBL_PER_WEEK:
      case Sta_CLICKS_CRS_PER_MONTH:
      case Sta_CLICKS_GBL_PER_MONTH:
      case Sta_CLICKS_CRS_PER_YEAR:
      case Sta_CLICKS_GBL_PER_YEAR:
      case Sta_CLICKS_CRS_PER_HOUR:
      case Sta_CLICKS_GBL_PER_HOUR:
      case Sta_CLICKS_CRS_PER_MINUTE:
      case Sta_CLICKS_GBL_PER_MINUTE:
	 fprintf (Gbl.F.Out,"<p class=\"DAT_SMALL CM\">%s: %s</p>",
		  Txt_Time_zone_used_in_the_calculation_of_these_statistics,
		  BrowserTimeZone);
	 break;
      default:
	 break;
     }
  }

/*****************************************************************************/
/******************* Show a listing of detailed clicks ***********************/
/*****************************************************************************/

static void Sta_ShowDetailedAccessesList (unsigned long NumRows,MYSQL_RES *mysql_res)
  {
   extern const char *Txt_Show_previous_X_clicks;
   extern const char *Txt_PAGES_Previous;
   extern const char *Txt_Clicks;
   extern const char *Txt_of_PART_OF_A_TOTAL;
   extern const char *Txt_page;
   extern const char *Txt_Show_next_X_clicks;
   extern const char *Txt_PAGES_Next;
   extern const char *Txt_No_INDEX;
   extern const char *Txt_User_ID;
   extern const char *Txt_Name;
   extern const char *Txt_Role;
   extern const char *Txt_Date;
   extern const char *Txt_Action;
   extern const char *Txt_LOG_More_info;
   extern const char *Txt_ROLES_SINGUL_Abc[Rol_NUM_ROLES][Usr_NUM_SEXS];
   extern const char *Txt_Today;
   unsigned long NumRow;
   unsigned long FirstRow;	// First row to show
   unsigned long LastRow;	// Last rows to show
   unsigned long NumPagesBefore;
   unsigned long NumPagesAfter;
   unsigned long NumPagsTotal;
   struct UsrData UsrDat;
   MYSQL_ROW row;
   long LogCod;
   Rol_Role_t RoleFromLog;
   unsigned UniqueId;
   long ActCod;
   char ActTxt[Act_MAX_BYTES_ACTION_TXT + 1];

   /***** Initialize estructura of data of the user *****/
   Usr_UsrDataConstructor (&UsrDat);

   /***** Compute the first and the last row to show *****/
   FirstRow = Gbl.Stat.FirstRow;
   LastRow  = Gbl.Stat.LastRow;
   if (FirstRow == 0 && LastRow == 0) // Call from main form
     {
      // Show last clicks
      FirstRow = (NumRows / Gbl.Stat.RowsPerPage - 1) * Gbl.Stat.RowsPerPage + 1;
      if ((FirstRow + Gbl.Stat.RowsPerPage - 1) < NumRows)
	 FirstRow += Gbl.Stat.RowsPerPage;
      LastRow = NumRows;
     }
   if (FirstRow < 1) // For security reasons; really it should never be less than 1
      FirstRow = 1;
   if (LastRow > NumRows)
      LastRow = NumRows;
   if ((LastRow - FirstRow) >= Gbl.Stat.RowsPerPage) // For if there have been clicks that have increased the number of rows
      LastRow = FirstRow + Gbl.Stat.RowsPerPage - 1;

   /***** Compute the number total of pages *****/
   /* Number of pages before the current one */
   NumPagesBefore = (FirstRow-1) / Gbl.Stat.RowsPerPage;
   if (NumPagesBefore * Gbl.Stat.RowsPerPage < (FirstRow-1))
      NumPagesBefore++;
   /* Number of pages after the current one */
   NumPagesAfter = (NumRows - LastRow) / Gbl.Stat.RowsPerPage;
   if (NumPagesAfter * Gbl.Stat.RowsPerPage < (NumRows - LastRow))
      NumPagesAfter++;
   /* Count the total number of pages */
   NumPagsTotal = NumPagesBefore + 1 + NumPagesAfter;

   /***** Put heading with backward and forward buttons *****/
   Tbl_TR_Begin (NULL);

   Tbl_TD_Begin ("colspan=\"7\" class=\"LM\"");
   Tbl_TABLE_BeginWidePadding (2);
   Tbl_TR_Begin (NULL);

   /* Put link to jump to previous page (older clicks) */
   if (FirstRow > 1)
     {
      Frm_StartFormAnchor (ActSeeAccCrs,Sta_STAT_RESULTS_SECTION_ID);
      Dat_WriteParamsIniEndDates ();
      Par_PutHiddenParamUnsigned ("GroupedBy",(unsigned) Sta_CLICKS_CRS_DETAILED_LIST);
      Par_PutHiddenParamUnsigned ("StatAct"  ,(unsigned) Gbl.Stat.NumAction);
      Par_PutHiddenParamLong ("FirstRow",FirstRow - Gbl.Stat.RowsPerPage);
      Par_PutHiddenParamLong ("LastRow" ,FirstRow - 1);
      Par_PutHiddenParamLong ("RowsPage",Gbl.Stat.RowsPerPage);
      Usr_PutHiddenParSelectedUsrsCods ();
     }
   Tbl_TD_Begin ("class=\"LM\" style=\"width:20%%;\"");
   if (FirstRow > 1)
     {
      snprintf (Gbl.Title,sizeof (Gbl.Title),
	        Txt_Show_previous_X_clicks,
                Gbl.Stat.RowsPerPage);
      Frm_LinkFormSubmit (Gbl.Title,"TIT_TBL",NULL);
      fprintf (Gbl.F.Out,"<strong>&lt;%s</strong></a>",
               Txt_PAGES_Previous);
     }
   Tbl_TD_End ();
   if (FirstRow > 1)
      Frm_EndForm ();

   /* Write number of current page */
   Tbl_TD_Begin ("class=\"DAT_N CM\" style=\"width:60%%;\"");
   fprintf (Gbl.F.Out,"<strong>"
                      "%s %lu-%lu %s %lu (%s %ld %s %lu)"
                      "</strong>",
            Txt_Clicks,
            FirstRow,LastRow,Txt_of_PART_OF_A_TOTAL,NumRows,
            Txt_page,NumPagesBefore + 1,Txt_of_PART_OF_A_TOTAL,NumPagsTotal);
   Tbl_TD_End ();

   /* Put link to jump to next page (more recent clicks) */
   if (LastRow < NumRows)
     {
      Frm_StartFormAnchor (ActSeeAccCrs,Sta_STAT_RESULTS_SECTION_ID);
      Dat_WriteParamsIniEndDates ();
      Par_PutHiddenParamUnsigned ("GroupedBy",(unsigned) Sta_CLICKS_CRS_DETAILED_LIST);
      Par_PutHiddenParamUnsigned ("StatAct"  ,(unsigned) Gbl.Stat.NumAction);
      Par_PutHiddenParamUnsigned ("FirstRow" ,(unsigned) (LastRow + 1));
      Par_PutHiddenParamUnsigned ("LastRow"  ,(unsigned) (LastRow + Gbl.Stat.RowsPerPage));
      Par_PutHiddenParamUnsigned ("RowsPage" ,(unsigned) Gbl.Stat.RowsPerPage);
      Usr_PutHiddenParSelectedUsrsCods ();
     }
   Tbl_TD_Begin ("class=\"RM\" style=\"width:20%%;\"");
   if (LastRow < NumRows)
     {
      snprintf (Gbl.Title,sizeof (Gbl.Title),
	        Txt_Show_next_X_clicks,
                Gbl.Stat.RowsPerPage);
      Frm_LinkFormSubmit (Gbl.Title,"TIT_TBL",NULL);
      fprintf (Gbl.F.Out,"<strong>%s&gt;</strong>"
	                 "</a>",
               Txt_PAGES_Next);
     }
   Tbl_TD_End ();
   if (LastRow < NumRows)
      Frm_EndForm ();

   Tbl_TR_End ();
   Tbl_TABLE_End ();
   Tbl_TD_End ();
   Tbl_TR_End ();

   /***** Write heading *****/
   Tbl_TR_Begin (NULL);

   Tbl_TH (1,1,"RT",Txt_No_INDEX);
   Tbl_TH (1,1,"CT",Txt_User_ID);
   Tbl_TH (1,1,"LT",Txt_Name);
   Tbl_TH (1,1,"CT",Txt_Role);
   Tbl_TH (1,1,"CT",Txt_Date);
   Tbl_TH (1,1,"LT",Txt_Action);
   Tbl_TH (1,1,"LT",Txt_LOG_More_info);

   Tbl_TR_End ();

   /***** Write rows back *****/
   for (NumRow = LastRow, UniqueId = 1, Gbl.RowEvenOdd = 0;
	NumRow >= FirstRow;
	NumRow--, UniqueId++, Gbl.RowEvenOdd = 1 - Gbl.RowEvenOdd)
     {
      mysql_data_seek (mysql_res,(my_ulonglong) (NumRow - 1));
      row = mysql_fetch_row (mysql_res);

      /* Get log code */
      LogCod = Str_ConvertStrCodToLongCod (row[0]);

      /* Get user's data of the database */
      UsrDat.UsrCod = Str_ConvertStrCodToLongCod (row[1]);
      Usr_ChkUsrCodAndGetAllUsrDataFromUsrCod (&UsrDat,Usr_DONT_GET_PREFS);

      /* Get logged role */
      if (sscanf (row[2],"%u",&RoleFromLog) != 1)
	 Rol_WrongRoleExit ();

      Tbl_TR_Begin (NULL);

      /* Write the number of row */
      Tbl_TD_Begin ("class=\"LOG RT COLOR%u\"",Gbl.RowEvenOdd);
      fprintf (Gbl.F.Out,"%ld&nbsp;",NumRow);
      Tbl_TD_End ();

      /* Write the user's ID if user is a student */
      Tbl_TD_Begin ("class=\"LOG CT COLOR%u\"",Gbl.RowEvenOdd);
      ID_WriteUsrIDs (&UsrDat,NULL);
      fprintf (Gbl.F.Out,"&nbsp;");
      Tbl_TD_End ();

      /* Write the first name and the surnames */
      Tbl_TD_Begin ("class=\"LOG LT COLOR%u\"",Gbl.RowEvenOdd);
      fprintf (Gbl.F.Out,"%s&nbsp;",UsrDat.FullName);
      Tbl_TD_End ();

      /* Write the user's role */
      Tbl_TD_Begin ("class=\"LOG CT COLOR%u\"",Gbl.RowEvenOdd);
      fprintf (Gbl.F.Out,"%s&nbsp;",
	       RoleFromLog < Rol_NUM_ROLES ? Txt_ROLES_SINGUL_Abc[RoleFromLog][UsrDat.Sex] :
		                             "?");
      Tbl_TD_End ();

      /* Write the date-time (row[3]) */
      Tbl_TD_Begin ("id=\"log_date_%u\" class=\"LOG RT COLOR%u\"",
                         UniqueId,Gbl.RowEvenOdd);
      fprintf (Gbl.F.Out,"<script type=\"text/javascript\">"
			 "writeLocalDateHMSFromUTC('log_date_%u',%ld,"
			 "%u,',&nbsp;','%s',true,false,0x7);"
			 "</script>",
               UniqueId,(long) Dat_GetUNIXTimeFromStr (row[3]),
               (unsigned) Gbl.Prefs.DateFormat,Txt_Today);
      Tbl_TD_End ();

      /* Write the action */
      if (sscanf (row[4],"%ld",&ActCod) != 1)
	 Lay_ShowErrorAndExit ("Wrong action code.");
      if (ActCod >= 0)
	{
         Tbl_TD_Begin ("class=\"LOG LT COLOR%u\"",Gbl.RowEvenOdd);
         fprintf (Gbl.F.Out,"%s&nbsp;",Act_GetActionTextFromDB (ActCod,ActTxt));
	}
      else
	{
         Tbl_TD_Begin ("class=\"LOG LT COLOR%u\"",Gbl.RowEvenOdd);
         fprintf (Gbl.F.Out,"?&nbsp;");
	}
      Tbl_TD_End ();

      /* Write the comments of the access */
      Tbl_TD_Begin ("class=\"LOG LT COLOR%u\"",Gbl.RowEvenOdd);
      Sta_WriteLogComments (LogCod);
      Tbl_TD_End ();

      Tbl_TR_End ();
     }

   /***** Free memory used by the data of the user *****/
   Usr_UsrDataDestructor (&UsrDat);
  }

/*****************************************************************************/
/******** Show a listing of with the number of clicks of each user ***********/
/*****************************************************************************/

static void Sta_WriteLogComments (long LogCod)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;

   /***** Get log comments from database *****/
   if (DB_QuerySELECT (&mysql_res,"can not get log comments",
		       "SELECT Comments FROM log_comments WHERE LogCod=%ld",
		       LogCod))
     {
      /***** Get and write comments *****/
      row = mysql_fetch_row (mysql_res);
      fprintf (Gbl.F.Out,"%s",row[0]);
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/********* Show a listing of with the number of clicks of each user **********/
/*****************************************************************************/

static void Sta_ShowNumHitsPerUsr (unsigned long NumRows,MYSQL_RES *mysql_res)
  {
   extern const char *Txt_No_INDEX;
   extern const char *Txt_Photo;
   extern const char *Txt_ID;
   extern const char *Txt_Name;
   extern const char *Txt_Role;
   extern const char *Txt_STAT_TYPE_COUNT_CAPS[Sta_NUM_COUNT_TYPES];
   extern const char *Txt_ROLES_SINGUL_Abc[Rol_NUM_ROLES][Usr_NUM_SEXS];
   MYSQL_ROW row;
   unsigned long NumRow;
   struct Sta_Hits Hits;
   unsigned BarWidth;
   struct UsrData UsrDat;
   char PhotoURL[PATH_MAX + 1];
   bool ShowPhoto;

   /***** Initialize user's data *****/
   Usr_UsrDataConstructor (&UsrDat);

   /***** Write heading *****/
   Tbl_TR_Begin (NULL);

   Tbl_TH (1,1,"RT",Txt_No_INDEX);
   Tbl_TH (1,1,"CT",Txt_Photo);
   Tbl_TH (1,1,"LT",Txt_ID);
   Tbl_TH (1,1,"LT",Txt_Name);
   Tbl_TH (1,1,"CT",Txt_Role);
   Tbl_TH (1,2,"LT",Txt_STAT_TYPE_COUNT_CAPS[Gbl.Stat.CountType]);

   Tbl_TR_End ();

   /***** Write rows *****/
   for (NumRow = 1, Hits.Max = 0.0, Gbl.RowEvenOdd = 0;
	NumRow <= NumRows;
	NumRow++, Gbl.RowEvenOdd = 1 - Gbl.RowEvenOdd)
     {
      row = mysql_fetch_row (mysql_res);

      /* Get user's data from the database */
      UsrDat.UsrCod = Str_ConvertStrCodToLongCod (row[0]);
      Usr_ChkUsrCodAndGetAllUsrDataFromUsrCod (&UsrDat,Usr_DONT_GET_PREFS);	// Get the data of the user from the database

      Tbl_TR_Begin (NULL);

      /* Write the number of row */
      Tbl_TD_Begin ("class=\"LOG RT COLOR%u\"",Gbl.RowEvenOdd);
      fprintf (Gbl.F.Out,"%ld&nbsp;",NumRow);
      Tbl_TD_End ();

      /* Show the photo */
      Tbl_TD_Begin ("class=\"CT COLOR%u\"",Gbl.RowEvenOdd);
      ShowPhoto = Pho_ShowingUsrPhotoIsAllowed (&UsrDat,PhotoURL);
      Pho_ShowUsrPhoto (&UsrDat,ShowPhoto ? PhotoURL :
                                            NULL,
                        "PHOTO15x20",Pho_ZOOM,false);
      Tbl_TD_End ();

      /* Write the user's ID if user is a student in current course */
      Tbl_TD_Begin ("class=\"LOG LT COLOR%u\"",Gbl.RowEvenOdd);
      ID_WriteUsrIDs (&UsrDat,NULL);
      fprintf (Gbl.F.Out,"&nbsp;");
      Tbl_TD_End ();

      /* Write the name and the surnames */
      Tbl_TD_Begin ("class=\"LOG LT COLOR%u\"",Gbl.RowEvenOdd);
      fprintf (Gbl.F.Out,"%s&nbsp;",UsrDat.FullName);
      Tbl_TD_End ();

      /* Write user's role */
      Tbl_TD_Begin ("class=\"LOG CT COLOR%u\"",Gbl.RowEvenOdd);
      fprintf (Gbl.F.Out,"%s&nbsp;",
	       Txt_ROLES_SINGUL_Abc[UsrDat.Roles.InCurrentCrs.Role][UsrDat.Sex]);
      Tbl_TD_End ();

      /* Write the number of clicks */
      Hits.Num = Str_GetFloatNumFromStr (row[1]);
      if (NumRow == 1)
	 Hits.Max = Hits.Num;
      if (Hits.Max > 0.0)
        {
         BarWidth = (unsigned) (((Hits.Num * 375.0) / Hits.Max) + 0.5);
         if (BarWidth == 0)
            BarWidth = 1;
        }
      else
         BarWidth = 0;

      Tbl_TD_Begin ("class=\"LOG LT COLOR%u\"",Gbl.RowEvenOdd);
      if (BarWidth)
	 fprintf (Gbl.F.Out,"<img src=\"%s/%c1x1.png\""	// Background
	                    " alt=\"\" title=\"\""
                            " class=\"LT\""
	                    " style=\"width:%upx; height:10px; padding-top:4px;\" />"
	                    "&nbsp;",
		  Cfg_URL_ICON_PUBLIC,
		  UsrDat.Roles.InCurrentCrs.Role == Rol_STD ? 'o' :	// Student
			                                      'r',	// Non-editing teacher or teacher
		  BarWidth);
      Str_WriteFloatNumToFile (Gbl.F.Out,Hits.Num);
      fprintf (Gbl.F.Out,"&nbsp;");
      Tbl_TD_End ();

      Tbl_TR_End ();
     }

   /***** Free memory used by the data of the user *****/
   Usr_UsrDataDestructor (&UsrDat);
  }

/*****************************************************************************/
/********** Show a listing of with the number of clicks in each date *********/
/*****************************************************************************/

static void Sta_ShowNumHitsPerDay (unsigned long NumRows,MYSQL_RES *mysql_res)
  {
   extern const char *Txt_Date;
   extern const char *Txt_Day;
   extern const char *Txt_STAT_TYPE_COUNT_CAPS[Sta_NUM_COUNT_TYPES];
   extern const char *Txt_DAYS_SMALL[7];
   unsigned long NumRow;
   struct Date ReadDate;
   struct Date LastDate;
   struct Date Date;
   unsigned D;
   unsigned NumDaysFromLastDateToCurrDate;
   int NumDayWeek;
   struct Sta_Hits Hits;
   MYSQL_ROW row;
   char StrDate[Cns_MAX_BYTES_DATE + 1];

   /***** Initialize LastDate *****/
   Dat_AssignDate (&LastDate,&Gbl.DateRange.DateEnd.Date);

   /***** Write heading *****/
   Tbl_TR_Begin (NULL);

   Tbl_TH (1,1,"CT",Txt_Date);
   Tbl_TH (1,1,"LT",Txt_Day);
   Tbl_TH (1,1,"LT",Txt_STAT_TYPE_COUNT_CAPS[Gbl.Stat.CountType]);

   Tbl_TR_End ();

   /***** Compute maximum number of pages generated per day *****/
   Sta_ComputeMaxAndTotalHits (&Hits,NumRows,mysql_res,1,1);

   /***** Write rows beginning by the most recent day and ending by the oldest *****/
   mysql_data_seek (mysql_res,0);
   for (NumRow = 1;
	NumRow <= NumRows;
	NumRow++)
     {
      row = mysql_fetch_row (mysql_res);

      /* Get year, month and day (row[0] holds the date in YYYYMMDD format) */
      if (!(Dat_GetDateFromYYYYMMDD (&ReadDate,row[0])))
	 Lay_ShowErrorAndExit ("Wrong date.");

      /* Get number of pages generated (in row[1]) */
      Hits.Num = Str_GetFloatNumFromStr (row[1]);

      Dat_AssignDate (&Date,&LastDate);
      NumDaysFromLastDateToCurrDate = Dat_GetNumDaysBetweenDates (&ReadDate,&LastDate);
      /* In the next loop (NumDaysFromLastDateToCurrDate-1) d�as (the more recent) with 0 clicks are shown
         and a last day (the oldest) with Hits.Num */
      for (D = 1;
	   D <= NumDaysFromLastDateToCurrDate;
	   D++)
        {
         NumDayWeek = Dat_GetDayOfWeek (Date.Year,Date.Month,Date.Day);

         Tbl_TR_Begin (NULL);

         /* Write the date */
	 Dat_ConvDateToDateStr (&Date,StrDate);
         Tbl_TD_Begin ("class=\"%s RT\"",NumDayWeek == 6 ? "LOG_R" :
					                          "LOG");
         fprintf (Gbl.F.Out,"%s&nbsp;",StrDate);
         Tbl_TD_End ();

         /* Write the day of the week */
         Tbl_TD_Begin ("class=\"%s LT\"",NumDayWeek == 6 ? "LOG_R" :
					                         "LOG");
         fprintf (Gbl.F.Out,"%s&nbsp;",Txt_DAYS_SMALL[NumDayWeek]);
         Tbl_TD_End ();

         /* Draw bar proportional to number of hits */
         Sta_DrawBarNumHits (NumDayWeek == 6 ? 'r' :	// red background
                                               'o',	// orange background
                             D == NumDaysFromLastDateToCurrDate ? Hits.Num :
                        	                                  0.0,
                             Hits.Max,Hits.Total,500);

         Tbl_TR_End ();

         /* Decrease day */
         Dat_GetDateBefore (&Date,&Date);
        }
      Dat_AssignDate (&LastDate,&Date);
     }
   NumDaysFromLastDateToCurrDate = Dat_GetNumDaysBetweenDates (&Gbl.DateRange.DateIni.Date,&LastDate);

   /***** Finally NumDaysFromLastDateToCurrDate days are shown with 0 clicks
          (the oldest days from the requested initial day until the first with clicks) *****/
   for (D = 1;
	D <= NumDaysFromLastDateToCurrDate;
	D++)
     {
      NumDayWeek = Dat_GetDayOfWeek (Date.Year,Date.Month,Date.Day);

      Tbl_TR_Begin (NULL);

      /* Write the date */
      Dat_ConvDateToDateStr (&Date,StrDate);
      Tbl_TD_Begin ("class=\"%s RT\"",NumDayWeek == 6 ? "LOG_R" :
					                       "LOG");
      fprintf (Gbl.F.Out,"%s&nbsp;",StrDate);
      Tbl_TD_End ();

      /* Write the day of the week */
      Tbl_TD_Begin ("class=\"%s LT\"",NumDayWeek == 6 ? "LOG_R" :
					                      "LOG");
      fprintf (Gbl.F.Out,"%s&nbsp;",Txt_DAYS_SMALL[NumDayWeek]);
      Tbl_TD_End ();

      /* Draw bar proportional to number of hits */
      Sta_DrawBarNumHits (NumDayWeek == 6 ? 'r' :	// red background
	                                    'o',	// orange background
	                  0.0,Hits.Max,Hits.Total,500);

      Tbl_TR_End ();

      /* Decrease day */
      Dat_GetDateBefore (&Date,&Date);
     }
  }

/*****************************************************************************/
/************ Show graphic of number of pages generated per hour *************/
/*****************************************************************************/

#define GRAPH_DISTRIBUTION_PER_HOUR_HOUR_WIDTH 25
#define GRAPH_DISTRIBUTION_PER_HOUR_TOTAL_WIDTH (GRAPH_DISTRIBUTION_PER_HOUR_HOUR_WIDTH * 24)

static void Sta_ShowDistrAccessesPerDayAndHour (unsigned long NumRows,MYSQL_RES *mysql_res)
  {
   extern const char *The_ClassFormInBox[The_NUM_THEMES];
   extern const char *Txt_Color_of_the_graphic;
   extern const char *Txt_STAT_COLOR_TYPES[Sta_NUM_COLOR_TYPES];
   extern const char *Txt_Date;
   extern const char *Txt_Day;
   extern const char *Txt_STAT_TYPE_COUNT_CAPS[Sta_NUM_COUNT_TYPES];
   extern const char *Txt_DAYS_SMALL[7];
   Sta_ColorType_t ColorType;
   Sta_ColorType_t SelectedColorType;
   unsigned long NumRow;
   struct Date PreviousReadDate;
   struct Date CurrentReadDate;
   struct Date LastDate;
   struct Date Date;
   unsigned D;
   unsigned NumDaysFromLastDateToCurrDate = 1;
   unsigned NumDayWeek;
   unsigned Hour;
   unsigned ReadHour = 0;
   struct Sta_Hits Hits;
   float NumAccPerHour[24];
   float NumAccPerHourZero[24];
   MYSQL_ROW row;
   char StrDate[Cns_MAX_BYTES_DATE + 1];

   /***** Get selected color type *****/
   SelectedColorType = Sta_GetStatColorType ();

   /***** Put a selector for the type of color *****/
   Tbl_TR_Begin (NULL);

   Tbl_TD_Begin ("colspan=\"26\" class=\"CM\"");

   Frm_StartFormAnchor (Gbl.Action.Act,Sta_STAT_RESULTS_SECTION_ID);
   Dat_WriteParamsIniEndDates ();
   Par_PutHiddenParamUnsigned ("GroupedBy",(unsigned) Gbl.Stat.ClicksGroupedBy);
   Par_PutHiddenParamUnsigned ("CountType",(unsigned) Gbl.Stat.CountType);
   Par_PutHiddenParamUnsigned ("StatAct"  ,(unsigned) Gbl.Stat.NumAction);
   if (Gbl.Action.Act == ActSeeAccCrs)
      Usr_PutHiddenParSelectedUsrsCods ();
   else // Gbl.Action.Act == ActSeeAccGbl
     {
      Par_PutHiddenParamUnsigned ("Role",(unsigned) Gbl.Stat.Role);
      Sta_PutHiddenParamScopeSta ();
     }

   fprintf (Gbl.F.Out,"<label class=\"%s\">%s:&nbsp;"
                      "<select name=\"ColorType\""
                      " onchange=\"document.getElementById('%s').submit();\">",
            The_ClassFormInBox[Gbl.Prefs.Theme],
            Txt_Color_of_the_graphic,
            Gbl.Form.Id);
   for (ColorType = (Sta_ColorType_t) 0;
	ColorType < Sta_NUM_COLOR_TYPES;
	ColorType++)
     {
      fprintf (Gbl.F.Out,"<option value=\"%u\"",(unsigned) ColorType);
      if (ColorType == SelectedColorType)
         fprintf (Gbl.F.Out," selected=\"selected\"");
      fprintf (Gbl.F.Out,">%s",Txt_STAT_COLOR_TYPES[ColorType]);
     }
   fprintf (Gbl.F.Out,"</select>"
	              "</label>");
   Frm_EndForm ();
   Tbl_TD_End ();
   Tbl_TR_End ();

   /***** Compute maximum number of pages generated per day-hour *****/
   Sta_ComputeMaxAndTotalHits (&Hits,NumRows,mysql_res,2,1);

   /***** Initialize LastDate *****/
   Dat_AssignDate (&LastDate,&Gbl.DateRange.DateEnd.Date);

   /***** Reset number of pages generated per hour *****/
   for (Hour = 0;
	Hour < 24;
	Hour++)
      NumAccPerHour[Hour] = NumAccPerHourZero[Hour] = 0.0;

   /***** Write heading *****/
   Tbl_TR_Begin (NULL);

   Tbl_TH (3,1,"CT",Txt_Date);
   Tbl_TH (3,1,"LT",Txt_Day);
   Tbl_TH (1,24,"LT",Txt_STAT_TYPE_COUNT_CAPS[Gbl.Stat.CountType]);

   Tbl_TR_End ();

   Tbl_TR_Begin (NULL);
   Tbl_TD_Begin ("colspan=\"24\" class=\"LT\"");
   Sta_DrawBarColors (SelectedColorType,Hits.Max);
   Tbl_TD_End ();
   Tbl_TR_End ();

   Tbl_TR_Begin (NULL);
   for (Hour = 0;
	Hour < 24;
	Hour++)
     {
      Tbl_TD_Begin ("class=\"LOG CT\" style=\"width:%upx;\"",
	            GRAPH_DISTRIBUTION_PER_HOUR_HOUR_WIDTH);
      fprintf (Gbl.F.Out,"%02uh",Hour);
      Tbl_TD_End ();
     }
   Tbl_TR_End ();

   /***** Write rows beginning by the most recent day and ending by the oldest one *****/
   mysql_data_seek (mysql_res,0);

   for (NumRow = 1;
	NumRow <= NumRows;
	NumRow++)
     {
      row = mysql_fetch_row (mysql_res);

      /* Get year, month and day (row[0] holds the date in YYYYMMDD format) */
      if (!(Dat_GetDateFromYYYYMMDD (&CurrentReadDate,row[0])))
	 Lay_ShowErrorAndExit ("Wrong date.");

      /* Get the hour (in row[1] is the hour in formato HH) */
      if (sscanf (row[1],"%02u",&ReadHour) != 1)
	 Lay_ShowErrorAndExit ("Wrong hour.");

      /* Get number of pages generated (in row[2]) */
      Hits.Num = Str_GetFloatNumFromStr (row[2]);

      /* If this is the first read date, initialize PreviousReadDate */
      if (NumRow == 1)
         Dat_AssignDate (&PreviousReadDate,&CurrentReadDate);

      /* Update number of hits per hour */
      if (PreviousReadDate.Year  != CurrentReadDate.Year  ||
          PreviousReadDate.Month != CurrentReadDate.Month ||
          PreviousReadDate.Day   != CurrentReadDate.Day)	// Current read date (CurrentReadDate) is older than previous read date (PreviousReadDate) */
        {
         /* In the next loop we show (NumDaysFromLastDateToCurrDate-1) days with 0 clicks
            and a last day (older) with Hits.Num */
         Dat_AssignDate (&Date,&LastDate);
         NumDaysFromLastDateToCurrDate = Dat_GetNumDaysBetweenDates (&PreviousReadDate,&LastDate);
         for (D = 1;
              D <= NumDaysFromLastDateToCurrDate;
              D++)
           {
            NumDayWeek = Dat_GetDayOfWeek (Date.Year,Date.Month,Date.Day);

            Tbl_TR_Begin (NULL);

            /* Write the date */
            Dat_ConvDateToDateStr (&Date,StrDate);
            Tbl_TD_Begin ("class=\"%s RT\"",NumDayWeek == 6 ? "LOG_R" :
						                     "LOG");
            fprintf (Gbl.F.Out,"%s&nbsp;",StrDate);
            Tbl_TD_End ();

            /* Write the day of the week */
            Tbl_TD_Begin ("class=\"%s LT\"",NumDayWeek == 6 ? "LOG_R" :
						                    "LOG");
            fprintf (Gbl.F.Out,"%s&nbsp;",Txt_DAYS_SMALL[NumDayWeek]);
            Tbl_TD_End ();

            /* Draw a cell with the color proportional to the number of clicks */
            if (D == NumDaysFromLastDateToCurrDate)
               Sta_DrawAccessesPerHourForADay (SelectedColorType,NumAccPerHour,Hits.Max);
            else	// D < NumDaysFromLastDateToCurrDate
               Sta_DrawAccessesPerHourForADay (SelectedColorType,NumAccPerHourZero,Hits.Max);
            Tbl_TR_End ();

            /* Decrease day */
            Dat_GetDateBefore (&Date,&Date);
           }
         Dat_AssignDate (&LastDate,&Date);
         Dat_AssignDate (&PreviousReadDate,&CurrentReadDate);

         /* Reset number of pages generated per hour */
         for (Hour = 0;
              Hour < 24;
              Hour++)
            NumAccPerHour[Hour] = 0.0;
        }
      NumAccPerHour[ReadHour] = Hits.Num;
     }

   /***** Show the clicks of the oldest day with clicks *****/
   /* In the next loop we show (NumDaysFromLastDateToCurrDate-1) days (more recent) with 0 clicks
      and a last day (older) with Hits.Num clicks */
   Dat_AssignDate (&Date,&LastDate);
   NumDaysFromLastDateToCurrDate = Dat_GetNumDaysBetweenDates (&PreviousReadDate,&LastDate);
   for (D = 1;
	D <= NumDaysFromLastDateToCurrDate;
	D++)
     {
      NumDayWeek = Dat_GetDayOfWeek (Date.Year,Date.Month,Date.Day);

      Tbl_TR_Begin (NULL);

      /* Write the date */
      Dat_ConvDateToDateStr (&Date,StrDate);
      Tbl_TD_Begin ("class=\"%s RT\"",NumDayWeek == 6 ? "LOG_R" :
					                       "LOG");
      fprintf (Gbl.F.Out,"%s&nbsp;",StrDate);
      Tbl_TD_End ();

      /* Write the day of the week */
      Tbl_TD_Begin ("class=\"%s LT\"",NumDayWeek == 6 ? "LOG_R" :
					                      "LOG");
      fprintf (Gbl.F.Out,"%s&nbsp;",Txt_DAYS_SMALL[NumDayWeek]);
      Tbl_TD_End ();

      /* Draw the color proporcional al number of clicks */
      if (D == NumDaysFromLastDateToCurrDate)
         Sta_DrawAccessesPerHourForADay (SelectedColorType,NumAccPerHour,Hits.Max);
      else	// D < NumDaysFromLastDateToCurrDate
         Sta_DrawAccessesPerHourForADay (SelectedColorType,NumAccPerHourZero,Hits.Max);
      Tbl_TR_End ();

      /* Decrease day */
      Dat_GetDateBefore (&Date,&Date);
     }

   /***** Finally NumDaysFromLastDateToCurrDate days are shown with 0 clicks
          (the oldest days since the initial day requested by the user until the first with clicks) *****/
   Dat_AssignDate (&LastDate,&Date);
   NumDaysFromLastDateToCurrDate = Dat_GetNumDaysBetweenDates (&Gbl.DateRange.DateIni.Date,&LastDate);
   for (D = 1;
	D <= NumDaysFromLastDateToCurrDate;
	D++)
     {
      NumDayWeek = Dat_GetDayOfWeek (Date.Year,Date.Month,Date.Day);

      Tbl_TR_Begin (NULL);

      /* Write the date */
      Dat_ConvDateToDateStr (&Date,StrDate);
      Tbl_TD_Begin ("class=\"%s RT\"",NumDayWeek == 6 ? "LOG_R" :
					                       "LOG");
      fprintf (Gbl.F.Out,"%s&nbsp;",StrDate);
      Tbl_TD_End ();

      /* Write the day of the week */
      Tbl_TD_Begin ("class=\"%s LT\"",NumDayWeek == 6 ? "LOG_R" :
					                      "LOG");
      fprintf (Gbl.F.Out,"%s&nbsp;",Txt_DAYS_SMALL[NumDayWeek]);
      Tbl_TD_End ();

      /* Draw the color proportional to number of clicks */
      Sta_DrawAccessesPerHourForADay (SelectedColorType,NumAccPerHourZero,Hits.Max);

      Tbl_TR_End ();

      /* Decrease day */
      Dat_GetDateBefore (&Date,&Date);
     }
  }

/*****************************************************************************/
/********* Put hidden parameter for the type of figure (statistic) ***********/
/*****************************************************************************/

static void Sta_PutHiddenParamScopeSta (void)
  {
   Sco_PutParamScope ("ScopeSta",Gbl.Scope.Current);
  }

/*****************************************************************************/
/********************** Get type of color for statistics *********************/
/*****************************************************************************/

static Sta_ColorType_t Sta_GetStatColorType (void)
  {
   return (Sta_ColorType_t)
	  Par_GetParToUnsignedLong ("ColorType",
	                            0,
	                            Sta_NUM_COLOR_TYPES - 1,
	                            (unsigned long) Sta_COLOR_TYPE_DEF);
  }

/*****************************************************************************/
/************************* Draw a bar with colors ****************************/
/*****************************************************************************/

static void Sta_DrawBarColors (Sta_ColorType_t ColorType,float HitsMax)
  {
   unsigned Interval;
   unsigned NumColor;
   unsigned R;
   unsigned G;
   unsigned B;

   /***** Write numbers from 0 to Hits.Max *****/
   Tbl_TABLE_BeginWide ();
   Tbl_TR_Begin (NULL);

   Tbl_TD_Begin ("colspan=\"%u\" class=\"LOG LB\" style=\"width:%upx;\"",
		 (GRAPH_DISTRIBUTION_PER_HOUR_TOTAL_WIDTH/5)/2,
		 (GRAPH_DISTRIBUTION_PER_HOUR_TOTAL_WIDTH/5)/2);
   fprintf (Gbl.F.Out,"0");
   Tbl_TD_End ();

   for (Interval = 1;
	Interval <= 4;
	Interval++)
     {
      Tbl_TD_Begin ("colspan=\"%u\" class=\"LOG CB\" style=\"width:%upx;\"",
		    GRAPH_DISTRIBUTION_PER_HOUR_TOTAL_WIDTH/5,
		    GRAPH_DISTRIBUTION_PER_HOUR_TOTAL_WIDTH/5);
      Str_WriteFloatNumToFile (Gbl.F.Out,(float) Interval * HitsMax / 5.0);
      Tbl_TD_End ();
     }

   Tbl_TD_Begin ("colspan=\"%u\" class=\"LOG RB\" style=\"width:%upx;\"",
		 (GRAPH_DISTRIBUTION_PER_HOUR_TOTAL_WIDTH/5)/2,
		 (GRAPH_DISTRIBUTION_PER_HOUR_TOTAL_WIDTH/5)/2);
   Str_WriteFloatNumToFile (Gbl.F.Out,HitsMax);
   Tbl_TD_End ();

   Tbl_TR_End ();

   Tbl_TR_Begin (NULL);

   /***** Draw colors *****/
   for (NumColor = 0;
	NumColor < GRAPH_DISTRIBUTION_PER_HOUR_TOTAL_WIDTH;
	NumColor++)
     {
      Sta_SetColor (ColorType,(float) NumColor,(float) GRAPH_DISTRIBUTION_PER_HOUR_TOTAL_WIDTH,&R,&G,&B);
      Tbl_TD_Begin ("class=\"LM\" style=\"width:1px; background-color:#%02X%02X%02X;\"",
	            R,G,B);
      fprintf (Gbl.F.Out,"<img src=\"%s/tr1x14.gif\" alt=\"\" title=\"\" />",
               Cfg_URL_ICON_PUBLIC);
      Tbl_TD_End ();
     }
   Tbl_TR_End ();
   Tbl_TABLE_End ();
  }

/*****************************************************************************/
/********************* Draw accesses per hour for a day **********************/
/*****************************************************************************/

static void Sta_DrawAccessesPerHourForADay (Sta_ColorType_t ColorType,float HitsNum[24],float HitsMax)
  {
   unsigned Hour;
   unsigned R;
   unsigned G;
   unsigned B;
   char *Str;

   for (Hour = 0;
	Hour < 24;
	Hour++)
     {
      /***** Set color depending on hits *****/
      Sta_SetColor (ColorType,HitsNum[Hour],HitsMax,&R,&G,&B);

      /***** Write from floating point number to string *****/
      Str_FloatNumToStr (&Str,HitsNum[Hour]);

      /***** Write cell *****/
      Tbl_TD_Begin ("class=\"LOG LM\" title=\"%s\""
	            " style=\"width:%upx; background-color:#%02X%02X%02X;\"",
	            Str,GRAPH_DISTRIBUTION_PER_HOUR_HOUR_WIDTH,R,G,B);
      Tbl_TD_End ();

      /***** Free memory allocated for string *****/
      free ((void *) Str);
     }
  }

/*****************************************************************************/
/************************* Set color depending on hits ***********************/
/*****************************************************************************/
// Hits.Max must be > 0
/*
Black         Blue         Cyan        Green        Yellow        Red
  +------------+------------+------------+------------+------------+
  |     0.2    |     0.2    |     0.2    |     0.2    |     0.2    |
  +------------+------------+------------+------------+------------+
 0.0          0.2          0.4          0.6          0.8          1.0
*/

static void Sta_SetColor (Sta_ColorType_t ColorType,float HitsNum,float HitsMax,
                          unsigned *R,unsigned *G,unsigned *B)
  {
   float Result = (HitsNum / HitsMax);

   switch (ColorType)
     {
      case Sta_COLOR:
         if (Result < 0.2)		// Black -> Blue
           {
            *R = 0;
            *G = 0;
            *B = (unsigned) (Result * 256.0 / 0.2 + 0.5);
            if (*B == 256)
               *B = 255;
           }
         else if (Result < 0.4)	// Blue -> Cyan
           {
            *R = 0;
            *G = (unsigned) ((Result-0.2) * 256.0 / 0.2 + 0.5);
            if (*G == 256)
               *G = 255;
            *B = 255;
           }
         else if (Result < 0.6)	// Cyan -> Green
           {
            *R = 0;
            *G = 255;
            *B = 256 - (unsigned) ((Result-0.4) * 256.0 / 0.2 + 0.5);
            if (*B == 256)
               *B = 255;
           }
         else if (Result < 0.8)	// Green -> Yellow
           {
            *R = (unsigned) ((Result-0.6) * 256.0 / 0.2 + 0.5);
            if (*R == 256)
               *R = 255;
            *G = 255;
            *B = 0;
           }
         else			// Yellow -> Red
           {
            *R = 255;
            *G = 256 - (unsigned) ((Result-0.8) * 256.0 / 0.2 + 0.5);
            if (*G == 256)
               *G = 255;
            *B = 0;
           }
         break;
      case Sta_BLACK_TO_WHITE:
         *B = (unsigned) (Result * 256.0 + 0.5);
         if (*B == 256)
            *B = 255;
         *R = *G = *B;
         break;
      case Sta_WHITE_TO_BLACK:
         *B = 256 - (unsigned) (Result * 256.0 + 0.5);
         if (*B == 256)
            *B = 255;
         *R = *G = *B;
         break;
     }
  }

/*****************************************************************************/
/********** Show listing with number of pages generated per week *************/
/*****************************************************************************/

static void Sta_ShowNumHitsPerWeek (unsigned long NumRows,
                                     MYSQL_RES *mysql_res)
  {
   extern const char *Txt_Week;
   extern const char *Txt_STAT_TYPE_COUNT_CAPS[Sta_NUM_COUNT_TYPES];
   unsigned long NumRow;
   struct Date ReadDate;
   struct Date LastDate;
   struct Date Date;
   unsigned W;
   unsigned NumWeeksBetweenLastDateAndCurDate;
   struct Sta_Hits Hits;
   MYSQL_ROW row;

   /***** Initialize LastDate to avoid warning *****/
   Dat_CalculateWeekOfYear (&Gbl.DateRange.DateEnd.Date);	// Changes Week and Year
   Dat_AssignDate (&LastDate,&Gbl.DateRange.DateEnd.Date);

   /***** Write heading *****/
   Tbl_TR_Begin (NULL);

   Tbl_TH (1,1,"LT",Txt_Week);
   Tbl_TH (1,1,"LT",Txt_STAT_TYPE_COUNT_CAPS[Gbl.Stat.CountType]);

   Tbl_TR_End ();

   /***** Compute maximum number of pages generated per week *****/
   Sta_ComputeMaxAndTotalHits (&Hits,NumRows,mysql_res,1,1);

   /***** Write rows *****/
   mysql_data_seek (mysql_res,0);
   for (NumRow = 1;
	NumRow <= NumRows;
	NumRow++)
     {
      row = mysql_fetch_row (mysql_res);

      /* Get year and week (row[0] holds date in YYYYWW format) */
      if (sscanf (row[0],"%04u%02u",&ReadDate.Year,&ReadDate.Week) != 2)
	 Lay_ShowErrorAndExit ("Wrong date.");

      /* Get number of pages generated (in row[1]) */
      Hits.Num = Str_GetFloatNumFromStr (row[1]);

      Dat_AssignDate (&Date,&LastDate);
      NumWeeksBetweenLastDateAndCurDate = Dat_GetNumWeeksBetweenDates (&ReadDate,&LastDate);
      for (W = 1;
	   W <= NumWeeksBetweenLastDateAndCurDate;
	   W++)
        {
         Tbl_TR_Begin (NULL);

         /* Write week */
         Tbl_TD_Begin ("class=\"LOG LT\"");
         fprintf (Gbl.F.Out,"%04u-%02u&nbsp;",Date.Year,Date.Week);
         Tbl_TD_End ();

         /* Draw bar proportional to number of hits */
         Sta_DrawBarNumHits ('o',	// orange background
                             W == NumWeeksBetweenLastDateAndCurDate ? Hits.Num :
                        	                                      0.0,
                             Hits.Max,Hits.Total,500);

         Tbl_TR_End ();

         /* Decrement week */
         Dat_GetWeekBefore (&Date,&Date);
        }
      Dat_AssignDate (&LastDate,&Date);
     }

  /***** Finally, show the old weeks without pages generated *****/
  Dat_CalculateWeekOfYear (&Gbl.DateRange.DateIni.Date);	// Changes Week and Year
  NumWeeksBetweenLastDateAndCurDate = Dat_GetNumWeeksBetweenDates (&Gbl.DateRange.DateIni.Date,
                                                                   &LastDate);
  for (W = 1;
       W <= NumWeeksBetweenLastDateAndCurDate;
       W++)
    {
     Tbl_TR_Begin (NULL);

     /* Write week */
     Tbl_TD_Begin ("class=\"LOG LT\"");
     fprintf (Gbl.F.Out,"%04u-%02u&nbsp;",Date.Year,Date.Week);
     Tbl_TD_End ();

     /* Draw bar proportional to number of hits */
     Sta_DrawBarNumHits ('o',	// orange background
                         0.0,Hits.Max,Hits.Total,500);

     Tbl_TR_End ();

     /* Decrement week */
     Dat_GetWeekBefore (&Date,&Date);
    }
  }

/*****************************************************************************/
/********** Show a graph with the number of clicks in each month *************/
/*****************************************************************************/

static void Sta_ShowNumHitsPerMonth (unsigned long NumRows,
                                      MYSQL_RES *mysql_res)
  {
   extern const char *Txt_Month;
   extern const char *Txt_STAT_TYPE_COUNT_CAPS[Sta_NUM_COUNT_TYPES];
   unsigned long NumRow;
   struct Date ReadDate;
   struct Date LastDate;
   struct Date Date;
   unsigned M;
   unsigned NumMonthsBetweenLastDateAndCurDate;
   struct Sta_Hits Hits;
   MYSQL_ROW row;

   /***** Initialize LastDate *****/
   Dat_AssignDate (&LastDate,&Gbl.DateRange.DateEnd.Date);

   /***** Write heading *****/
   Tbl_TR_Begin (NULL);

   Tbl_TH (1,1,"LT",Txt_Month);
   Tbl_TH (1,1,"LT",Txt_STAT_TYPE_COUNT_CAPS[Gbl.Stat.CountType]);

   Tbl_TR_End ();

   /***** Compute maximum number of pages generated per month *****/
   Sta_ComputeMaxAndTotalHits (&Hits,NumRows,mysql_res,1,1);

   /***** Write rows *****/
   mysql_data_seek (mysql_res,0);
   for (NumRow = 1;
	NumRow <= NumRows;
	NumRow++)
     {
      row = mysql_fetch_row (mysql_res);

      /* Get the year and the month (in row[0] is the date in YYYYMM format) */
      if (sscanf (row[0],"%04u%02u",&ReadDate.Year,&ReadDate.Month) != 2)
	 Lay_ShowErrorAndExit ("Wrong date.");

      /* Get number of pages generated (in row[1]) */
      Hits.Num = Str_GetFloatNumFromStr (row[1]);

      Dat_AssignDate (&Date,&LastDate);
      NumMonthsBetweenLastDateAndCurDate = Dat_GetNumMonthsBetweenDates (&ReadDate,
                                                                         &LastDate);
      for (M = 1;
	   M <= NumMonthsBetweenLastDateAndCurDate;
	   M++)
        {
         Tbl_TR_Begin (NULL);

         /* Write the month */
         Tbl_TD_Begin ("class=\"LOG LT\"");
         fprintf (Gbl.F.Out,"%04u-%02u&nbsp;",Date.Year,Date.Month);
         Tbl_TD_End ();

         /* Draw bar proportional to number of hits */
         Sta_DrawBarNumHits ('o',	// orange background
                             M == NumMonthsBetweenLastDateAndCurDate ? Hits.Num :
                        	                                       0.0,
                             Hits.Max,Hits.Total,500);

         Tbl_TR_End ();

         /* Decrease month */
         Dat_GetMonthBefore (&Date,&Date);
        }
      Dat_AssignDate (&LastDate,&Date);
     }

  /***** Finally, show the oldest months without clicks *****/
  NumMonthsBetweenLastDateAndCurDate = Dat_GetNumMonthsBetweenDates (&Gbl.DateRange.DateIni.Date,
                                                                     &LastDate);
  for (M = 1;
       M <= NumMonthsBetweenLastDateAndCurDate;
       M++)
    {
     Tbl_TR_Begin (NULL);

     /* Write the month */
     Tbl_TD_Begin ("class=\"LOG LT\"");
     fprintf (Gbl.F.Out,"%04u-%02u&nbsp;",Date.Year,Date.Month);
     Tbl_TD_End ();

     /* Draw bar proportional to number of hits */
     Sta_DrawBarNumHits ('o',	// orange background
                         0.0,Hits.Max,Hits.Total,500);

     Tbl_TR_End ();

     /* Decrease month */
     Dat_GetMonthBefore (&Date,&Date);
    }
  }

/*****************************************************************************/
/*********** Show a graph with the number of clicks in each year *************/
/*****************************************************************************/

static void Sta_ShowNumHitsPerYear (unsigned long NumRows,
                                    MYSQL_RES *mysql_res)
  {
   extern const char *Txt_Year;
   extern const char *Txt_STAT_TYPE_COUNT_CAPS[Sta_NUM_COUNT_TYPES];
   unsigned long NumRow;
   struct Date ReadDate;
   struct Date LastDate;
   struct Date Date;
   unsigned Y;
   unsigned NumYearsBetweenLastDateAndCurDate;
   struct Sta_Hits Hits;
   MYSQL_ROW row;

   /***** Initialize LastDate *****/
   Dat_AssignDate (&LastDate,&Gbl.DateRange.DateEnd.Date);

   /***** Write heading *****/
   Tbl_TR_Begin (NULL);

   Tbl_TH (1,1,"LT",Txt_Year);
   Tbl_TH (1,1,"LT",Txt_STAT_TYPE_COUNT_CAPS[Gbl.Stat.CountType]);

   Tbl_TR_End ();

   /***** Compute maximum number of pages generated per year *****/
   Sta_ComputeMaxAndTotalHits (&Hits,NumRows,mysql_res,1,1);

   /***** Write rows *****/
   mysql_data_seek (mysql_res,0);
   for (NumRow = 1;
	NumRow <= NumRows;
	NumRow++)
     {
      row = mysql_fetch_row (mysql_res);

      /* Get the year (in row[0] is the date in YYYY format) */
      if (sscanf (row[0],"%04u",&ReadDate.Year) != 1)
	 Lay_ShowErrorAndExit ("Wrong date.");

      /* Get number of pages generated (in row[1]) */
      Hits.Num = Str_GetFloatNumFromStr (row[1]);

      Dat_AssignDate (&Date,&LastDate);
      NumYearsBetweenLastDateAndCurDate = Dat_GetNumYearsBetweenDates (&ReadDate,
                                                                       &LastDate);
      for (Y = 1;
	   Y <= NumYearsBetweenLastDateAndCurDate;
	   Y++)
        {
         Tbl_TR_Begin (NULL);

         /* Write the year */
         Tbl_TD_Begin ("class=\"LOG LT\"");
         fprintf (Gbl.F.Out,"%04u&nbsp;",Date.Year);
         Tbl_TD_End ();

         /* Draw bar proportional to number of hits */
         Sta_DrawBarNumHits ('o',	// orange background
                             Y == NumYearsBetweenLastDateAndCurDate ? Hits.Num :
                        	                                      0.0,
                             Hits.Max,Hits.Total,500);

         Tbl_TR_End ();

         /* Decrease year */
         Dat_GetYearBefore (&Date,&Date);
        }
      Dat_AssignDate (&LastDate,&Date);
     }

  /***** Finally, show the oldest years without clicks *****/
  NumYearsBetweenLastDateAndCurDate = Dat_GetNumYearsBetweenDates (&Gbl.DateRange.DateIni.Date,
                                                                   &LastDate);
  for (Y = 1;
       Y <= NumYearsBetweenLastDateAndCurDate;
       Y++)
    {
     Tbl_TR_Begin (NULL);

     /* Write the year */
     Tbl_TD_Begin ("class=\"LOG LT\"");
     fprintf (Gbl.F.Out,"%04u&nbsp;",Date.Year);
     Tbl_TD_End ();

     /* Draw bar proportional to number of hits */
     Sta_DrawBarNumHits ('o',	// orange background
                         0.0,Hits.Max,Hits.Total,500);

     Tbl_TR_End ();

     /* Decrease year */
     Dat_GetYearBefore (&Date,&Date);
    }
  }

/*****************************************************************************/
/**************** Show graphic of number of pages generated per hour ***************/
/*****************************************************************************/

#define DIGIT_WIDTH 6

static void Sta_ShowNumHitsPerHour (unsigned long NumRows,
                                    MYSQL_RES *mysql_res)
  {
   unsigned long NumRow;
   struct Sta_Hits Hits;
   unsigned NumDays;
   unsigned Hour = 0;
   unsigned ReadHour = 0;
   unsigned H;
   unsigned NumDigits;
   unsigned ColumnWidth;
   MYSQL_ROW row;

   if ((NumDays = Dat_GetNumDaysBetweenDates (&Gbl.DateRange.DateIni.Date,&Gbl.DateRange.DateEnd.Date)))
     {
      /***** Compute maximum number of pages generated per hour *****/
      Sta_ComputeMaxAndTotalHits (&Hits,NumRows,mysql_res,1,NumDays);

      /***** Compute width of columns (one for each hour) *****/
      /* Maximum number of d�gits. If less than 4, set it to 4 to ensure a minimum width */
      NumDigits = (Hits.Max >= 1000) ? (unsigned) floor (log10 ((double) Hits.Max)) + 1 :
	                               4;
      ColumnWidth = NumDigits * DIGIT_WIDTH + 2;

      /***** Draw the graphic *****/
      mysql_data_seek (mysql_res,0);
      NumRow = 1;
      Tbl_TR_Begin (NULL);
      while (Hour < 24)
	{
	 Hits.Num = 0.0;
	 if (NumRow <= NumRows)	// If not read yet all the results of the query
	   {
	    row = mysql_fetch_row (mysql_res); // Get next result
	    NumRow++;
	    if (sscanf (row[0],"%02u",&ReadHour) != 1)   // In row[0] is the date in HH format
	       Lay_ShowErrorAndExit ("Wrong hour.");

	    for (H = Hour;
		 H < ReadHour;
		 H++, Hour++)
	       Sta_WriteAccessHour (H,&Hits,ColumnWidth);

	    Hits.Num = Str_GetFloatNumFromStr (row[1]) / (float) NumDays;
	    Sta_WriteAccessHour (ReadHour,&Hits,ColumnWidth);

	    Hour++;
	   }
	 else
	    for (H = ReadHour + 1;
		 H < 24;
		 H++, Hour++)
	       Sta_WriteAccessHour (H,&Hits,ColumnWidth);
	}
      Tbl_TR_End ();
     }
  }

/*****************************************************************************/
/**** Write a column of the graphic of the number of clicks in each hour *****/
/*****************************************************************************/

static void Sta_WriteAccessHour (unsigned Hour,struct Sta_Hits *Hits,unsigned ColumnWidth)
  {
   unsigned BarHeight;

   Tbl_TD_Begin ("class=\"DAT_SMALL CB\" style=\"width:%upx;\"",ColumnWidth);

   /* Draw bar with a height porportional to the number of clicks */
   if (Hits->Num > 0.0)
     {
      fprintf (Gbl.F.Out,"%u%%<br />",
	       (unsigned) (((Hits->Num * 100.0) /
		            Hits->Total) + 0.5));
      Str_WriteFloatNumToFile (Gbl.F.Out,Hits->Num);
      fprintf (Gbl.F.Out,"<br />");
      BarHeight = (unsigned) (((Hits->Num * 500.0) / Hits->Max) + 0.5);
      if (BarHeight == 0)
         BarHeight = 1;
      fprintf (Gbl.F.Out,"<img src=\"%s/o1x1.png\""	// Orange background
	                 " alt=\"\" title=\"\""
	                 " style=\"width:10px; height:%upx;\" />",
	       Cfg_URL_ICON_PUBLIC,BarHeight);
     }
   else
      fprintf (Gbl.F.Out,"0%%<br />0");

   /* Write the hour */
   fprintf (Gbl.F.Out,"<br />%uh",Hour);
   Tbl_TD_End ();
  }

/*****************************************************************************/
/**** Show a listing with the number of clicks in every minute of the day ***/
/*****************************************************************************/

#define Sta_NUM_MINUTES_PER_DAY		(60 * 24)	// 1440 minutes in a day
#define Sta_WIDTH_SEMIDIVISION_GRAPHIC	30
#define Sta_NUM_DIVISIONS_X		10

static void Sta_ShowAverageAccessesPerMinute (unsigned long NumRows,MYSQL_RES *mysql_res)
  {
   unsigned long NumRow = 1;
   MYSQL_ROW row;
   unsigned NumDays;
   unsigned MinuteDay = 0;
   unsigned ReadHour;
   unsigned MinuteRead;
   unsigned MinuteDayRead = 0;
   unsigned i;
   struct Sta_Hits Hits;
   float NumClicksPerMin[Sta_NUM_MINUTES_PER_DAY];
   float Power10LeastOrEqual;
   float MaxX;
   float IncX;
   char *Format;

   if ((NumDays = Dat_GetNumDaysBetweenDates (&Gbl.DateRange.DateIni.Date,&Gbl.DateRange.DateEnd.Date)))
     {
      /***** Compute number of clicks (and m�ximo) in every minute *****/
      Hits.Max = 0.0;
      while (MinuteDay < Sta_NUM_MINUTES_PER_DAY)
	{
	 if (NumRow <= NumRows)	// If not all the result of the query are yet read
	   {
	    row = mysql_fetch_row (mysql_res); // Get next result
	    NumRow++;
	    if (sscanf (row[0],"%02u%02u",&ReadHour,&MinuteRead) != 2)   // In row[0] is the date in formato HHMM
	       Lay_ShowErrorAndExit ("Wrong hour-minute.");
	    /* Get number of pages generated */
	    Hits.Num = Str_GetFloatNumFromStr (row[1]);
	    MinuteDayRead = ReadHour * 60 + MinuteRead;
	    for (i = MinuteDay;
		 i < MinuteDayRead;
		 i++, MinuteDay++)
	       NumClicksPerMin[i] = 0.0;
	    NumClicksPerMin[MinuteDayRead] = Hits.Num / (float) NumDays;
	    if (NumClicksPerMin[MinuteDayRead] > Hits.Max)
	       Hits.Max = NumClicksPerMin[MinuteDayRead];
	    MinuteDay++;
	   }
	 else
	    for (i = MinuteDayRead + 1;
		 i < Sta_NUM_MINUTES_PER_DAY;
		 i++, MinuteDay++)
	       NumClicksPerMin[i] = 0.0;
	}

      /***** Compute the maximum value of X and the increment of the X axis *****/
      if (Hits.Max <= 0.000001)
	 MaxX = 0.000001;
      else
	{
	 Power10LeastOrEqual = (float) pow (10.0,floor (log10 ((double) Hits.Max)));
	 MaxX = ceil (Hits.Max / Power10LeastOrEqual) * Power10LeastOrEqual;
	}
      IncX = MaxX / (float) Sta_NUM_DIVISIONS_X;
      if (IncX >= 1.0)
	 Format = "%.0f";
      else if (IncX >= 0.1)
	 Format = "%.1f";
      else if (IncX >= 0.01)
	 Format = "%.2f";
      else if (IncX >= 0.001)
	 Format = "%.3f";
      else
	 Format = "%f";

      /***** X axis tags *****/
      Sta_WriteLabelsXAxisAccMin (IncX,Format);

      /***** Y axis and graphic *****/
      for (i = 0;
	   i < Sta_NUM_MINUTES_PER_DAY;
	   i++)
	 Sta_WriteAccessMinute (i,NumClicksPerMin[i],MaxX);

      /***** X axis *****/
      Tbl_TR_Begin (NULL);

      /* First division (left) */
      Tbl_TD_Begin ("class=\"LM\" style=\"width:%upx;\"",
	            Sta_WIDTH_SEMIDIVISION_GRAPHIC);
      fprintf (Gbl.F.Out,"<img src=\"%s/ejexizq24x1.gif\""
	                 " alt=\"\" title=\"\""
	                 " style=\"display:block; width:%upx; height:1px;\" />",
	       Cfg_URL_ICON_PUBLIC,
	       Sta_WIDTH_SEMIDIVISION_GRAPHIC);
      Tbl_TD_End ();

      /* All the intermediate divisions */
      for (i = 0;
	   i < Sta_NUM_DIVISIONS_X * 2;
	   i++)
	{
	 Tbl_TD_Begin ("class=\"LM\" style=\"width:%upx;\"",
		       Sta_WIDTH_SEMIDIVISION_GRAPHIC);
	 fprintf (Gbl.F.Out,"<img src=\"%s/ejex24x1.gif\""
	                    " alt=\"\" title=\"\""
	                    " style=\"display:block;"
	                    " width:%upx; height:1px;\" />",
		  Cfg_URL_ICON_PUBLIC,
		  Sta_WIDTH_SEMIDIVISION_GRAPHIC);
	 Tbl_TD_End ();
	}

      /* Last division (right) */
      Tbl_TD_Begin ("class=\"LM\" style=\"width:%upx;\"",
	            Sta_WIDTH_SEMIDIVISION_GRAPHIC);
      fprintf (Gbl.F.Out,"<img src=\"%s/tr24x1.gif\""
	                 " alt=\"\" title=\"\""
	                 " style=\"display:block; width:%upx; height:1px;\" />",
	       Cfg_URL_ICON_PUBLIC,
	       Sta_WIDTH_SEMIDIVISION_GRAPHIC);
      Tbl_TD_End ();

      Tbl_TR_End ();

      /***** Write again the labels of the X axis *****/
      Sta_WriteLabelsXAxisAccMin (IncX,Format);
     }
  }

/*****************************************************************************/
/****** Write labels of the X axis in the graphic of clicks per minute *******/
/*****************************************************************************/

#define Sta_WIDTH_DIVISION_GRAPHIC	(Sta_WIDTH_SEMIDIVISION_GRAPHIC * 2)	// 60

static void Sta_WriteLabelsXAxisAccMin (float IncX,const char *Format)
  {
   unsigned i;
   float NumX;

   Tbl_TR_Begin (NULL);
   for (i = 0, NumX = 0;
	i <= Sta_NUM_DIVISIONS_X;
	i++, NumX += IncX)
     {
      Tbl_TD_Begin ("colspan=\"2\" class=\"LOG CB\" style=\"width:%upx;\"",
                    Sta_WIDTH_DIVISION_GRAPHIC);
      fprintf (Gbl.F.Out,Format,NumX);
      Tbl_TD_End ();
     }
   Tbl_TR_End ();
  }

/*****************************************************************************/
/***** Write a row of the graphic with number of clicks in every minute ******/
/*****************************************************************************/

#define Sta_WIDTH_GRAPHIC	(Sta_WIDTH_DIVISION_GRAPHIC * Sta_NUM_DIVISIONS_X)	// 60 * 10 = 600

static void Sta_WriteAccessMinute (unsigned Minute,float HitsNum,float MaxX)
  {
   unsigned BarWidth;

   /***** Start row *****/
   Tbl_TR_Begin (NULL);

   /***** Labels of the Y axis, and Y axis *****/
   if (!Minute)
     {
      // If minute 0
      Tbl_TD_Begin ("rowspan=\"30\" class=\"LOG LT\""
		    " style=\"width:%upx;"
		    " background-image:url('%s/ejey24x30.gif');"
		    " background-size:30px 30px;"
		    " background-repeat:repeat;\"",
                    Sta_WIDTH_SEMIDIVISION_GRAPHIC,Cfg_URL_ICON_PUBLIC);
      fprintf (Gbl.F.Out,"00h");
      Tbl_TD_End ();
     }
   else if (Minute == (Sta_NUM_MINUTES_PER_DAY - 30))
     {
      // If 23:30
      Tbl_TD_Begin ("rowspan=\"30\" class=\"LOG LB\""
		    " style=\"width:%upx;"
		    " background-image:url('%s/ejey24x30.gif');"
		    " background-size:30px 30px;"
		    " background-repeat:repeat;\"",
                    Sta_WIDTH_SEMIDIVISION_GRAPHIC,Cfg_URL_ICON_PUBLIC);
      fprintf (Gbl.F.Out,"24h");
      Tbl_TD_End ();
     }
   else if (!(Minute % 30) && (Minute % 60))
     {
      // If minute is multiple of 30 but not of 60 (i.e.: 30, 90, 150...)
      Tbl_TD_Begin ("rowspan=\"60\" class=\"LOG LM\""
		    " style=\"width:%upx;"
		    " background-image:url('%s/ejey24x60.gif');"
		    " background-size:30px 60px;"
		    " background-repeat:repeat;\"",
	            Sta_WIDTH_SEMIDIVISION_GRAPHIC,Cfg_URL_ICON_PUBLIC);
      fprintf (Gbl.F.Out,"%02uh",(Minute + 30) / 60);
      Tbl_TD_End ();
     }

   /***** Start cell for the graphic *****/
   Tbl_TD_Begin ("colspan=\"%u\" class=\"LB\""
		 " style=\"width:%upx; height:1px;"
		 " background-image:url('%s/malla%c48x1.gif');"
		 " background-size:60px 1px;"
		 " background-repeat:repeat;\"",
	         Sta_NUM_DIVISIONS_X * 2,Sta_WIDTH_GRAPHIC,Cfg_URL_ICON_PUBLIC,
	         (Minute % 60) == 0 ? 'v' :
		                      'h');

   /***** Draw bar with a width proportional to the number of hits *****/
   if (HitsNum != 0.0)
      if ((BarWidth = (unsigned) (((HitsNum * (float) Sta_WIDTH_GRAPHIC / MaxX)) + 0.5)) != 0)
	 fprintf (Gbl.F.Out,"<img src=\"%s/%c1x1.png\""
	                    " alt=\"\" title=\"\""
	                    " style=\"display:block;"
	                    " width:%upx; height:1px;\" />",
                  Cfg_URL_ICON_PUBLIC,
                  (Minute % 60) == 0 ? 'r' :	// red background
                	               'o',	// orange background
                  BarWidth);

   /***** End cell of graphic and end row *****/
   Tbl_TD_End ();
   Tbl_TR_End ();
  }

/*****************************************************************************/
/**** Show a listing of accesses with the number of clicks a each action *****/
/*****************************************************************************/

static void Sta_ShowNumHitsPerAction (unsigned long NumRows,
                                      MYSQL_RES *mysql_res)
  {
   extern const char *Txt_Action;
   extern const char *Txt_STAT_TYPE_COUNT_CAPS[Sta_NUM_COUNT_TYPES];
   unsigned long NumRow;
   struct Sta_Hits Hits;
   MYSQL_ROW row;
   long ActCod;
   char ActTxt[Act_MAX_BYTES_ACTION_TXT + 1];

   /***** Write heading *****/
   Tbl_TR_Begin (NULL);

   Tbl_TH (1,1,"RT",Txt_Action);
   Tbl_TH (1,1,"LT",Txt_STAT_TYPE_COUNT_CAPS[Gbl.Stat.CountType]);

   Tbl_TR_End ();

   /***** Compute maximum number of pages generated per day *****/
   Sta_ComputeMaxAndTotalHits (&Hits,NumRows,mysql_res,1,1);

   /***** Write rows *****/
   mysql_data_seek (mysql_res,0);
   for (NumRow = 1;
	NumRow <= NumRows;
	NumRow++)
     {
      row = mysql_fetch_row (mysql_res);

      /* Write the action */
      ActCod = Str_ConvertStrCodToLongCod (row[0]);

      Tbl_TR_Begin (NULL);

      Tbl_TD_Begin ("class=\"LOG RT\"");
      if (ActCod >= 0)
         fprintf (Gbl.F.Out,"%s&nbsp;",Act_GetActionTextFromDB (ActCod,ActTxt));
      else
         fprintf (Gbl.F.Out,"?&nbsp;");
      Tbl_TD_End ();

      /* Draw bar proportional to number of hits */
      Hits.Num = Str_GetFloatNumFromStr (row[1]);
      Sta_DrawBarNumHits ('o',	// orange background
                          Hits.Num,Hits.Max,Hits.Total,500);

      Tbl_TR_End ();
     }
  }

/*****************************************************************************/
/*************** Show number of clicks distributed by plugin *****************/
/*****************************************************************************/

static void Sta_ShowNumHitsPerPlugin (unsigned long NumRows,
                                      MYSQL_RES *mysql_res)
  {
   extern const char *Txt_Plugin;
   extern const char *Txt_STAT_TYPE_COUNT_CAPS[Sta_NUM_COUNT_TYPES];
   unsigned long NumRow;
   struct Sta_Hits Hits;
   MYSQL_ROW row;
   struct Plugin Plg;

   /***** Write heading *****/
   Tbl_TR_Begin (NULL);

   Tbl_TH (1,1,"RT",Txt_Plugin);
   Tbl_TH (1,1,"LT",Txt_STAT_TYPE_COUNT_CAPS[Gbl.Stat.CountType]);

   Tbl_TR_End ();

   /***** Compute maximum number of pages generated per plugin *****/
   Sta_ComputeMaxAndTotalHits (&Hits,NumRows,mysql_res,1,1);

   /***** Write rows *****/
   mysql_data_seek (mysql_res,0);
   for (NumRow = 1;
	NumRow <= NumRows;
	NumRow++)
     {
      row = mysql_fetch_row (mysql_res);

      Tbl_TR_Begin (NULL);

      /* Write the plugin */
      if (sscanf (row[0],"%ld",&Plg.PlgCod) != 1)
	 Lay_ShowErrorAndExit ("Wrong plugin code.");
      Tbl_TD_Begin ("class=\"LOG RT\"");
      if (Plg_GetDataOfPluginByCod (&Plg))
         fprintf (Gbl.F.Out,"%s",Plg.Name);
      else
         fprintf (Gbl.F.Out,"?");
      fprintf (Gbl.F.Out,"&nbsp;");
      Tbl_TD_End ();

      /* Draw bar proportional to number of hits */
      Hits.Num = Str_GetFloatNumFromStr (row[1]);
      Sta_DrawBarNumHits ('o',	// orange background
                          Hits.Num,Hits.Max,Hits.Total,500);

      Tbl_TR_End ();
     }
  }

/*****************************************************************************/
/******** Show number of clicks distributed by web service function **********/
/*****************************************************************************/

static void Sta_ShowNumHitsPerWSFunction (unsigned long NumRows,
                                          MYSQL_RES *mysql_res)
  {
   extern const char *Txt_Function;
   extern const char *Txt_STAT_TYPE_COUNT_CAPS[Sta_NUM_COUNT_TYPES];
   unsigned long NumRow;
   struct Sta_Hits Hits;
   MYSQL_ROW row;
   long FunCod;

   /***** Write heading *****/
   Tbl_TR_Begin (NULL);

   Tbl_TH (1,1,"LT",Txt_Function);
   Tbl_TH (1,1,"LT",Txt_STAT_TYPE_COUNT_CAPS[Gbl.Stat.CountType]);

   Tbl_TR_End ();

   /***** Compute maximum number of pages generated per function *****/
   Sta_ComputeMaxAndTotalHits (&Hits,NumRows,mysql_res,1,1);

   /***** Write rows *****/
   mysql_data_seek (mysql_res,0);
   for (NumRow = 1;
	NumRow <= NumRows;
	NumRow++)
     {
      row = mysql_fetch_row (mysql_res);

      Tbl_TR_Begin (NULL);

      /* Write the plugin */
      if (sscanf (row[0],"%ld",&FunCod) != 1)
	 Lay_ShowErrorAndExit ("Wrong function code.");

      Tbl_TD_Begin ("class=\"LOG LT\"");
      fprintf (Gbl.F.Out,"%s&nbsp;",API_GetFunctionNameFromFunCod (FunCod));
      Tbl_TD_End ();

      /* Draw bar proportional to number of hits */
      Hits.Num = Str_GetFloatNumFromStr (row[1]);
      Sta_DrawBarNumHits ('o',	// orange background
                          Hits.Num,Hits.Max,Hits.Total,500);

      Tbl_TR_End ();
     }
  }

/*****************************************************************************/
/******** Show number of clicks distributed by web service function **********/
/*****************************************************************************/

static void Sta_ShowNumHitsPerBanner (unsigned long NumRows,
                                      MYSQL_RES *mysql_res)
  {
   extern const char *Txt_Banner;
   extern const char *Txt_STAT_TYPE_COUNT_CAPS[Sta_NUM_COUNT_TYPES];
   unsigned long NumRow;
   float NumClicks;
   float MaxClicks = 0.0;
   float TotalClicks = 0.0;
   MYSQL_ROW row;
   struct Banner Ban;

   /***** Write heading *****/
   Tbl_TR_Begin (NULL);

   Tbl_TH (1,1,"CT",Txt_Banner);
   Tbl_TH (1,1,"LT",Txt_STAT_TYPE_COUNT_CAPS[Gbl.Stat.CountType]);

   Tbl_TR_End ();

   /***** Compute maximum number of clicks per banner *****/
   for (NumRow = 1;
	NumRow <= NumRows;
	NumRow++)
     {
      row = mysql_fetch_row (mysql_res);

      /* Get number of pages generated */
      NumClicks = Str_GetFloatNumFromStr (row[1]);
      if (NumRow == 1)
	 MaxClicks = NumClicks;
      TotalClicks += NumClicks;
     }

   /***** Write rows *****/
   mysql_data_seek (mysql_res,0);
   for (NumRow = 1;
	NumRow <= NumRows;
	NumRow++)
     {
      row = mysql_fetch_row (mysql_res);

      Tbl_TR_Begin (NULL);

      /* Write the banner */
      if (sscanf (row[0],"%ld",&(Ban.BanCod)) != 1)
	 Lay_ShowErrorAndExit ("Wrong banner code.");
      Ban_GetDataOfBannerByCod (&Ban);
      Tbl_TD_Begin ("class=\"LOG LT\"");
      fprintf (Gbl.F.Out,"<a href=\"%s\" title=\"%s\" class=\"DAT\" target=\"_blank\">"
                         "<img src=\"%s/%s\""
                         " alt=\"%s\" title=\"%s\""
                         " class=\"BANNER_SMALL\""
                         " style=\"margin:0 10px 5px 0;\" />"
                         "</a>",
               Ban.WWW,
               Ban.FullName,
               Cfg_URL_BANNER_PUBLIC,
               Ban.Img,
               Ban.ShrtName,
               Ban.FullName);

      /* Draw bar proportional to number of clicks */
      NumClicks = Str_GetFloatNumFromStr (row[1]);
      Sta_DrawBarNumHits ('o',	// orange background
		      	  NumClicks,MaxClicks,TotalClicks,500);

      Tbl_TR_End ();
     }
  }

/*****************************************************************************/
/******* Show a listing with the number of hits distributed by country *******/
/*****************************************************************************/

static void Sta_ShowNumHitsPerCountry (unsigned long NumRows,
                                       MYSQL_RES *mysql_res)
  {
   extern const char *Txt_No_INDEX;
   extern const char *Txt_Country;
   extern const char *Txt_STAT_TYPE_COUNT_CAPS[Sta_NUM_COUNT_TYPES];
   unsigned long NumRow;
   unsigned long Ranking;
   struct Sta_Hits Hits;
   MYSQL_ROW row;
   long CtyCod;

   /***** Write heading *****/
   Tbl_TR_Begin (NULL);

   Tbl_TH (1,1,"CT",Txt_No_INDEX);
   Tbl_TH (1,1,"CT",Txt_Country);
   Tbl_TH (1,1,"LT",Txt_STAT_TYPE_COUNT_CAPS[Gbl.Stat.CountType]);

   Tbl_TR_End ();

   /***** Compute maximum number of hits per country *****/
   Sta_ComputeMaxAndTotalHits (&Hits,NumRows,mysql_res,1,1);

   /***** Write rows *****/
   mysql_data_seek (mysql_res,0);
   for (NumRow = 1, Ranking = 0;
	NumRow <= NumRows;
	NumRow++)
     {
      /* Get country code */
      row = mysql_fetch_row (mysql_res);
      CtyCod = Str_ConvertStrCodToLongCod (row[0]);

      Tbl_TR_Begin (NULL);

      /* Write ranking of this country */
      Tbl_TD_Begin ("class=\"LOG RM\"");
      if (CtyCod > 0)
         fprintf (Gbl.F.Out,"%lu",++Ranking);
      fprintf (Gbl.F.Out,"&nbsp;");
      Tbl_TD_End ();

      /* Write country */
      Sta_WriteCountry (CtyCod);

      /* Draw bar proportional to number of hits */
      Hits.Num = Str_GetFloatNumFromStr (row[1]);
      Sta_DrawBarNumHits ('o',	// orange background
                          Hits.Num,Hits.Max,Hits.Total,375);

      Tbl_TR_End ();
     }
  }

/*****************************************************************************/
/************************ Write country with an icon *************************/
/*****************************************************************************/

static void Sta_WriteCountry (long CtyCod)
  {
   struct Country Cty;

   /***** Start cell *****/
   Tbl_TD_Begin ("class=\"LOG LM\"");

   if (CtyCod > 0)	// Hit with a country selected
     {
      /***** Get data of country *****/
      Cty.CtyCod = CtyCod;
      Cty_GetDataOfCountryByCod (&Cty,Cty_GET_BASIC_DATA);

      /***** Form to go to country *****/
      Cty_DrawCountryMapAndNameWithLink (&Cty,ActSeeCtyInf,
                                         "COUNTRY_TINY",
                                         "COUNTRY_MAP_TINY",
                                         "LOG");
     }
   else			// Hit with no country selected
      /***** No country selected *****/
      fprintf (Gbl.F.Out,"&nbsp;-&nbsp;");

   /***** End cell *****/
   Tbl_TD_End ();
  }

/*****************************************************************************/
/***** Show a listing with the number of hits distributed by institution *****/
/*****************************************************************************/

static void Sta_ShowNumHitsPerInstitution (unsigned long NumRows,
                                           MYSQL_RES *mysql_res)
  {
   extern const char *Txt_No_INDEX;
   extern const char *Txt_Institution;
   extern const char *Txt_STAT_TYPE_COUNT_CAPS[Sta_NUM_COUNT_TYPES];
   unsigned long NumRow;
   unsigned long Ranking;
   struct Sta_Hits Hits;
   MYSQL_ROW row;
   long InsCod;

   /***** Write heading *****/
   Tbl_TR_Begin (NULL);

   Tbl_TH (1,1,"CT",Txt_No_INDEX);
   Tbl_TH (1,1,"CT",Txt_Institution);
   Tbl_TH (1,1,"LT",Txt_STAT_TYPE_COUNT_CAPS[Gbl.Stat.CountType]);

   Tbl_TR_End ();

   /***** Compute maximum number of hits per institution *****/
   Sta_ComputeMaxAndTotalHits (&Hits,NumRows,mysql_res,1,1);

   /***** Write rows *****/
   mysql_data_seek (mysql_res,0);
   for (NumRow = 1, Ranking = 0;
	NumRow <= NumRows;
	NumRow++)
     {
      /* Get institution code */
      row = mysql_fetch_row (mysql_res);
      InsCod = Str_ConvertStrCodToLongCod (row[0]);

      Tbl_TR_Begin (NULL);

      /* Write ranking of this institution */
      Tbl_TD_Begin ("class=\"LOG RT\"");
      if (InsCod > 0)
         fprintf (Gbl.F.Out,"%lu",++Ranking);
      fprintf (Gbl.F.Out,"&nbsp;");
      Tbl_TD_End ();

      /* Write institution */
      Sta_WriteInstitution (InsCod);

      /* Draw bar proportional to number of hits */
      Hits.Num = Str_GetFloatNumFromStr (row[1]);
      Sta_DrawBarNumHits ('o',	// orange background
		      	  Hits.Num,Hits.Max,Hits.Total,375);

      Tbl_TR_End ();
     }
  }

/*****************************************************************************/
/********************** Write institution with an icon ***********************/
/*****************************************************************************/

static void Sta_WriteInstitution (long InsCod)
  {
   struct Instit Ins;

   /***** Start cell *****/
   if (InsCod > 0)	// Hit with an institution selected
     {
      /***** Get data of institution *****/
      Ins.InsCod = InsCod;
      Ins_GetDataOfInstitutionByCod (&Ins,Ins_GET_BASIC_DATA);

      /***** Title in cell *****/
      Tbl_TD_Begin ("class=\"LOG LM\" title=\"%s\"",Ins.FullName);

      /***** Form to go to institution *****/
      Ins_DrawInstitutionLogoAndNameWithLink (&Ins,ActSeeInsInf,
                                              "LOG","CT");
     }
   else			// Hit with no institution selected
     {
      /***** No institution selected *****/
      Tbl_TD_Begin ("class=\"LOG LM\"");
      fprintf (Gbl.F.Out,"&nbsp;-&nbsp;");
     }

   /***** End cell *****/
   Tbl_TD_End ();
  }

/*****************************************************************************/
/******* Show a listing with the number of hits distributed by centre ********/
/*****************************************************************************/

static void Sta_ShowNumHitsPerCentre (unsigned long NumRows,
                                      MYSQL_RES *mysql_res)
  {
   extern const char *Txt_No_INDEX;
   extern const char *Txt_Centre;
   extern const char *Txt_STAT_TYPE_COUNT_CAPS[Sta_NUM_COUNT_TYPES];
   unsigned long NumRow;
   unsigned long Ranking;
   struct Sta_Hits Hits;
   MYSQL_ROW row;
   long CtrCod;

   /***** Write heading *****/
   Tbl_TR_Begin (NULL);

   Tbl_TH (1,1,"CT",Txt_No_INDEX);
   Tbl_TH (1,1,"CT",Txt_Centre);
   Tbl_TH (1,1,"LT",Txt_STAT_TYPE_COUNT_CAPS[Gbl.Stat.CountType]);

   Tbl_TR_End ();

   /***** Compute maximum number of hits per centre *****/
   Sta_ComputeMaxAndTotalHits (&Hits,NumRows,mysql_res,1,1);

   /***** Write rows *****/
   mysql_data_seek (mysql_res,0);
   for (NumRow = 1, Ranking = 0;
	NumRow <= NumRows;
	NumRow++)
     {
      /* Get centre code */
      row = mysql_fetch_row (mysql_res);
      CtrCod = Str_ConvertStrCodToLongCod (row[0]);

      Tbl_TR_Begin (NULL);

      /* Write ranking of this centre */
      Tbl_TD_Begin ("class=\"LOG RT\"");
      if (CtrCod > 0)
         fprintf (Gbl.F.Out,"%lu",++Ranking);
      fprintf (Gbl.F.Out,"&nbsp;");
      Tbl_TD_End ();

      /* Write centre */
      Sta_WriteCentre (CtrCod);

      /* Draw bar proportional to number of hits */
      Hits.Num = Str_GetFloatNumFromStr (row[1]);
      Sta_DrawBarNumHits ('o',	// orange background
		      	  Hits.Num,Hits.Max,Hits.Total,375);

      Tbl_TR_End ();
     }
  }

/*****************************************************************************/
/************************* Write centre with an icon *************************/
/*****************************************************************************/

static void Sta_WriteCentre (long CtrCod)
  {
   struct Centre Ctr;

   /***** Start cell *****/
   if (CtrCod > 0)	// Hit with a centre selected
     {
      /***** Get data of centre *****/
      Ctr.CtrCod = CtrCod;
      Ctr_GetDataOfCentreByCod (&Ctr);

      /***** Title in cell *****/
      Tbl_TD_Begin ("class=\"LOG LM\" title=\"%s\"",Ctr.FullName);

      /***** Form to go to centre *****/
      Ctr_DrawCentreLogoAndNameWithLink (&Ctr,ActSeeCtrInf,
                                         "LOG","CT");
     }
   else			// Hit with no centre selected
     {
      /***** No centre selected *****/
      Tbl_TD_Begin ("class=\"LOG LM\"");
      fprintf (Gbl.F.Out,"&nbsp;-&nbsp;");
     }

   /***** End cell *****/
   Tbl_TD_End ();
  }

/*****************************************************************************/
/******* Show a listing with the number of hits distributed by degree ********/
/*****************************************************************************/

static void Sta_ShowNumHitsPerDegree (unsigned long NumRows,
                                      MYSQL_RES *mysql_res)
  {
   extern const char *Txt_No_INDEX;
   extern const char *Txt_Degree;
   extern const char *Txt_STAT_TYPE_COUNT_CAPS[Sta_NUM_COUNT_TYPES];
   unsigned long NumRow;
   unsigned long Ranking;
   struct Sta_Hits Hits;
   MYSQL_ROW row;
   long DegCod;

   /***** Write heading *****/
   Tbl_TR_Begin (NULL);

   Tbl_TH (1,1,"CT",Txt_No_INDEX);
   Tbl_TH (1,1,"CT",Txt_Degree);
   Tbl_TH (1,1,"LT",Txt_STAT_TYPE_COUNT_CAPS[Gbl.Stat.CountType]);

   Tbl_TR_End ();

   /***** Compute maximum number of hits per degree *****/
   Sta_ComputeMaxAndTotalHits (&Hits,NumRows,mysql_res,1,1);

   /***** Write rows *****/
   mysql_data_seek (mysql_res,0);
   for (NumRow = 1, Ranking = 0;
	NumRow <= NumRows;
	NumRow++)
     {
      /* Get degree code */
      row = mysql_fetch_row (mysql_res);
      DegCod = Str_ConvertStrCodToLongCod (row[0]);

      Tbl_TR_Begin (NULL);

      /* Write ranking of this degree */
      Tbl_TD_Begin ("class=\"LOG RT\"");
      if (DegCod > 0)
         fprintf (Gbl.F.Out,"%lu",++Ranking);
      fprintf (Gbl.F.Out,"&nbsp;");
      Tbl_TD_End ();

      /* Write degree */
      Sta_WriteDegree (DegCod);

      /* Draw bar proportional to number of hits */
      Hits.Num = Str_GetFloatNumFromStr (row[1]);
      Sta_DrawBarNumHits ('o',	// orange background
		      	  Hits.Num,Hits.Max,Hits.Total,375);

      Tbl_TR_End ();
     }
  }

/*****************************************************************************/
/************************* Write degree with an icon *************************/
/*****************************************************************************/

static void Sta_WriteDegree (long DegCod)
  {
   struct Degree Deg;

   /***** Start cell *****/
   if (DegCod > 0)	// Hit with a degree selected
     {
      /***** Get data of degree *****/
      Deg.DegCod = DegCod;
      Deg_GetDataOfDegreeByCod (&Deg);

      /***** Title in cell *****/
      Tbl_TD_Begin ("class=\"LOG LM\" title=\"%s\"",Deg.FullName);

      /***** Form to go to degree *****/
      Deg_DrawDegreeLogoAndNameWithLink (&Deg,ActSeeDegInf,
                                         "LOG","CT");
     }
   else			// Hit with no degree selected
     {
      /***** No degree selected *****/
      Tbl_TD_Begin ("class=\"LOG LM\"");
      fprintf (Gbl.F.Out,"&nbsp;-&nbsp;");
     }

   /***** End cell *****/
   Tbl_TD_End ();
  }

/*****************************************************************************/
/********* Show a listing with the number of clicks to each course ***********/
/*****************************************************************************/

static void Sta_ShowNumHitsPerCourse (unsigned long NumRows,
                                      MYSQL_RES *mysql_res)
  {
   extern const char *Txt_No_INDEX;
   extern const char *Txt_Degree;
   extern const char *Txt_Year_OF_A_DEGREE;
   extern const char *Txt_Course;
   extern const char *Txt_STAT_TYPE_COUNT_CAPS[Sta_NUM_COUNT_TYPES];
   extern const char *Txt_Go_to_X;
   extern const char *Txt_YEAR_OF_DEGREE[1 + Deg_MAX_YEARS_PER_DEGREE];	// Declaration in swad_degree.c
   unsigned long NumRow;
   unsigned long Ranking;
   struct Sta_Hits Hits;
   MYSQL_ROW row;
   bool CrsOK;
   struct Course Crs;

   /***** Write heading *****/
   Tbl_TR_Begin (NULL);

   Tbl_TH (1,1,"CT",Txt_No_INDEX);
   Tbl_TH (1,1,"CT",Txt_Degree);
   Tbl_TH (1,1,"CT",Txt_Year_OF_A_DEGREE);
   Tbl_TH (1,1,"CT",Txt_Course);
   Tbl_TH (1,1,"LT",Txt_STAT_TYPE_COUNT_CAPS[Gbl.Stat.CountType]);

   Tbl_TR_End ();

   /***** Compute maximum number of pages generated per course *****/
   Sta_ComputeMaxAndTotalHits (&Hits,NumRows,mysql_res,1,1);

   /***** Write rows *****/
   mysql_data_seek (mysql_res,0);
   for (NumRow = 1, Ranking = 0;
	NumRow <= NumRows;
	NumRow++)
     {
      /* Get degree, the year and the course */
      row = mysql_fetch_row (mysql_res);

      /* Get course code */
      Crs.CrsCod = Str_ConvertStrCodToLongCod (row[0]);

      /* Get data of current degree */
      CrsOK = Crs_GetDataOfCourseByCod (&Crs);

      Tbl_TR_Begin (NULL);

      /* Write ranking of this course */
      Tbl_TD_Begin ("class=\"LOG RT\"");
      if (CrsOK)
         fprintf (Gbl.F.Out,"%lu",++Ranking);
      fprintf (Gbl.F.Out,"&nbsp;");
      Tbl_TD_End ();

      /* Write degree */
      Sta_WriteDegree (Crs.DegCod);

      /* Write degree year */
      Tbl_TD_Begin ("class=\"LOG CT\"");
      fprintf (Gbl.F.Out,"%s&nbsp;",
               CrsOK ? Txt_YEAR_OF_DEGREE[Crs.Year] :
        	       "-");
      Tbl_TD_End ();

      /* Write course, including link */
      Tbl_TD_Begin ("class=\"LOG LT\"");
      if (CrsOK)
        {
         Frm_StartFormGoTo (ActSeeCrsInf);
         Crs_PutParamCrsCod (Crs.CrsCod);
         snprintf (Gbl.Title,sizeof (Gbl.Title),
                   Txt_Go_to_X,
		   Crs.FullName);
         Frm_LinkFormSubmit (Gbl.Title,"LOG",NULL);
         fprintf (Gbl.F.Out,"%s"
                            "</a>",
                  Crs.ShrtName);
        }
      else
         fprintf (Gbl.F.Out,"-");
      fprintf (Gbl.F.Out,"&nbsp;");
      if (CrsOK)
         Frm_EndForm ();
      Tbl_TD_End ();

      /* Draw bar proportional to number of hits */
      Hits.Num = Str_GetFloatNumFromStr (row[1]);
      Sta_DrawBarNumHits ('o',	// orange background
		      	  Hits.Num,Hits.Max,Hits.Total,375);

      Tbl_TR_End ();
     }
  }

/*****************************************************************************/
/*************** Compute maximum and total number of hits ********************/
/*****************************************************************************/

void Sta_ComputeMaxAndTotalHits (struct Sta_Hits *Hits,
                                 unsigned long NumRows,
                                 MYSQL_RES *mysql_res,unsigned Field,
                                 unsigned Divisor)
  {
   unsigned long NumRow;
   MYSQL_ROW row;

   /***** For each row... *****/
   for (NumRow = 1, Hits->Max = Hits->Total = 0.0;
	NumRow <= NumRows;
	NumRow++)
     {
      /* Get row */
      row = mysql_fetch_row (mysql_res);

      /* Get number of hits */
      Hits->Num = Str_GetFloatNumFromStr (row[Field]);
      if (Divisor > 1)
         Hits->Num /= (float) Divisor;

      /* Update total hits */
      Hits->Total += Hits->Num;

      /* Update maximum hits */
      if (Hits->Num > Hits->Max)
	 Hits->Max = Hits->Num;
     }
  }

/*****************************************************************************/
/********************* Draw a bar with the number of hits ********************/
/*****************************************************************************/

static void Sta_DrawBarNumHits (char Color,
				float HitsNum,float HitsMax,float HitsTotal,
				unsigned MaxBarWidth)
  {
   unsigned BarWidth;

   Tbl_TD_Begin ("class=\"LOG LM\"");

   if (HitsNum != 0.0)
     {
      /***** Draw bar with a with proportional to the number of hits *****/
      BarWidth = (unsigned) (((HitsNum * (float) MaxBarWidth) / HitsMax) + 0.5);
      if (BarWidth == 0)
         BarWidth = 1;
      fprintf (Gbl.F.Out,"<img src=\"%s/%c1x1.png\""	// Background
	                 " alt=\"\" title=\"\""
                         " class=\"LM\""
	                 " style=\"width:%upx; height:10px;\" />"
                         "&nbsp;",
	       Cfg_URL_ICON_PUBLIC,Color,BarWidth);

      /***** Write the number of hits *****/
      Str_WriteFloatNumToFile (Gbl.F.Out,HitsNum);
      fprintf (Gbl.F.Out,"&nbsp;(%u",
               (unsigned) (((HitsNum * 100.0) /
        	            HitsTotal) + 0.5));
     }
   else
      /***** Write the number of clicks *****/
      fprintf (Gbl.F.Out,"0&nbsp;(0");

   fprintf (Gbl.F.Out,"%%)&nbsp;");

   Tbl_TD_End ();
  }

/*****************************************************************************/
/**************** Compute the time used to generate the page *****************/
/*****************************************************************************/

void Sta_ComputeTimeToGeneratePage (void)
  {
   if (gettimeofday (&Gbl.tvPageCreated, &Gbl.tz))
      // Error in gettimeofday
      Gbl.TimeGenerationInMicroseconds = 0;
   else
     {
      if (Gbl.tvPageCreated.tv_usec < Gbl.tvStart.tv_usec)
	{
	 Gbl.tvPageCreated.tv_sec--;
	 Gbl.tvPageCreated.tv_usec += 1000000;
	}
      Gbl.TimeGenerationInMicroseconds = (Gbl.tvPageCreated.tv_sec  - Gbl.tvStart.tv_sec) * 1000000L +
                                          Gbl.tvPageCreated.tv_usec - Gbl.tvStart.tv_usec;
     }
  }

/*****************************************************************************/
/****************** Compute the time used to send the page *******************/
/*****************************************************************************/

void Sta_ComputeTimeToSendPage (void)
  {
   if (gettimeofday (&Gbl.tvPageSent, &Gbl.tz))
      // Error in gettimeofday
      Gbl.TimeSendInMicroseconds = 0;
   else
     {
      if (Gbl.tvPageSent.tv_usec < Gbl.tvPageCreated.tv_usec)
	{
	 Gbl.tvPageSent.tv_sec--;
	 Gbl.tvPageSent.tv_usec += 1000000;
	}
      Gbl.TimeSendInMicroseconds = (Gbl.tvPageSent.tv_sec  - Gbl.tvPageCreated.tv_sec) * 1000000L +
                                    Gbl.tvPageSent.tv_usec - Gbl.tvPageCreated.tv_usec;
     }
  }

/*****************************************************************************/
/************** Write the time to generate and send the page *****************/
/*****************************************************************************/

void Sta_WriteTimeToGenerateAndSendPage (void)
  {
   extern const char *Txt_PAGE1_Page_generated_in;
   extern const char *Txt_PAGE2_and_sent_in;
   char StrTimeGenerationInMicroseconds[Dat_MAX_BYTES_TIME + 1];
   char StrTimeSendInMicroseconds[Dat_MAX_BYTES_TIME + 1];

   Sta_WriteTime (StrTimeGenerationInMicroseconds,Gbl.TimeGenerationInMicroseconds);
   Sta_WriteTime (StrTimeSendInMicroseconds,Gbl.TimeSendInMicroseconds);
   fprintf (Gbl.F.Out,"%s %s %s %s",
            Txt_PAGE1_Page_generated_in,StrTimeGenerationInMicroseconds,
            Txt_PAGE2_and_sent_in,StrTimeSendInMicroseconds);
  }

/*****************************************************************************/
/********* Write time (given in microseconds) depending on amount ************/
/*****************************************************************************/

void Sta_WriteTime (char Str[Dat_MAX_BYTES_TIME],long TimeInMicroseconds)
  {
   if (TimeInMicroseconds < 1000L)
      snprintf (Str,Dat_MAX_BYTES_TIME + 1,
	        "%ld &micro;s",
		TimeInMicroseconds);
   else if (TimeInMicroseconds < 1000000L)
      snprintf (Str,Dat_MAX_BYTES_TIME + 1,
	        "%ld ms",
		TimeInMicroseconds / 1000);
   else if (TimeInMicroseconds < (60 * 1000000L))
      snprintf (Str,Dat_MAX_BYTES_TIME + 1,
	        "%.1f s",
		(float) TimeInMicroseconds / 1E6);
   else
      snprintf (Str,Dat_MAX_BYTES_TIME + 1,
	        "%ld min, %ld s",
                TimeInMicroseconds / (60 * 1000000L),
                (TimeInMicroseconds / 1000000L) % 60);
  }


/*****************************************************************************/
/*************** Put a link to show last clicks in real time *****************/
/*****************************************************************************/

void Sta_PutLinkToLastClicks (void)
  {
   extern const char *Txt_Last_clicks;

   Lay_PutContextualLinkIconText (ActLstClk,NULL,NULL,
				  "mouse-pointer.svg",
				  Txt_Last_clicks);
  }

/*****************************************************************************/
/****************************** Show last clicks *****************************/
/*****************************************************************************/

void Sta_ShowLastClicks (void)
  {
   extern const char *Hlp_USERS_Connected_last_clicks;
   extern const char *Txt_Last_clicks_in_real_time;

   /***** Contextual links *****/
   fprintf (Gbl.F.Out,"<div class=\"CONTEXT_MENU\">");

   /* Put form to go to test edition and configuration */
   Sta_PutLinkToGlobalHits ();

   /* Put form to go to test edition and configuration */
   Sta_PutLinkToCourseHits ();

   fprintf (Gbl.F.Out,"</div>");

   /***** Start box *****/
   Box_StartBox (NULL,Txt_Last_clicks_in_real_time,NULL,
                 Hlp_USERS_Connected_last_clicks,Box_NOT_CLOSABLE);

   /***** Get and show last clicks *****/
   fprintf (Gbl.F.Out,"<div id=\"lastclicks\""	// Used for AJAX based refresh
	              " class=\"CM\">");
   Sta_GetAndShowLastClicks ();
   fprintf (Gbl.F.Out,"</div>");		// Used for AJAX based refresh

   /***** End box *****/
   Box_EndBox ();
  }

/*****************************************************************************/
/**************** Get last clicks from database and show them ****************/
/*****************************************************************************/

void Sta_GetAndShowLastClicks (void)
  {
   extern const char *Txt_Click;
   extern const char *Txt_ELAPSED_TIME;
   extern const char *Txt_Role;
   extern const char *Txt_Country;
   extern const char *Txt_Institution;
   extern const char *Txt_Centre;
   extern const char *Txt_Degree;
   extern const char *Txt_Action;
   extern const char *Txt_ROLES_SINGUL_Abc[Rol_NUM_ROLES][Usr_NUM_SEXS];
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRow;
   unsigned long NumRows;
   long ActCod;
   const char *ClassRow;
   time_t TimeDiff;
   struct Country Cty;
   struct Instit Ins;
   struct Centre Ctr;
   struct Degree Deg;

   /***** Get last clicks from database *****/
   /* Important for maximum performance:
      do the LIMIT in the big log table before the JOIN */
   NumRows = DB_QuerySELECT (&mysql_res,"can not get last clicks",
			     "SELECT last_logs.LogCod,last_logs.ActCod,"
			     "last_logs.Dif,last_logs.Role,"
			     "last_logs.CtyCod,last_logs.InsCod,"
			     "last_logs.CtrCod,last_logs.DegCod,"
			     "actions.Txt"
			     " FROM"
			     " (SELECT LogCod,ActCod,"
			     "UNIX_TIMESTAMP()-UNIX_TIMESTAMP(ClickTime) AS Dif,"
			     "Role,CtyCod,InsCod,CtrCod,DegCod"
			     " FROM log_recent ORDER BY LogCod DESC LIMIT 20)"
			     " AS last_logs LEFT JOIN actions"	// LEFT JOIN because action may be not present in table of actions
			     " ON last_logs.ActCod=actions.ActCod"
			     " WHERE actions.Language='es'"	// TODO: Change to user's language
			     " OR actions.Language IS NULL");	// When action is not present in table of actions

   /***** Write list of connected users *****/
   Tbl_TABLE_BeginCenterPadding (1);
   Tbl_TR_Begin (NULL);

   Tbl_TH (1,1,"LC_CLK",Txt_Click);		// Click
   Tbl_TH (1,1,"LC_TIM",Txt_ELAPSED_TIME);	// Elapsed time
   Tbl_TH (1,1,"LC_ROL",Txt_Role);		// Role
   Tbl_TH (1,1,"LC_CTY",Txt_Country);		// Country
   Tbl_TH (1,1,"LC_INS",Txt_Institution);	// Institution
   Tbl_TH (1,1,"LC_CTR",Txt_Centre);		// Centre
   Tbl_TH (1,1,"LC_DEG",Txt_Degree);		// Degree
   Tbl_TH (1,1,"LC_ACT",Txt_Action);		// Action

   Tbl_TR_End ();

   for (NumRow = 0;
	NumRow < NumRows;
	NumRow++)
     {
      row = mysql_fetch_row (mysql_res);

      /* Get action code (row[1]) */
      ActCod = Str_ConvertStrCodToLongCod (row[1]);

      /* Use a special color for this row depending on the action */
      ClassRow = (Act_GetBrowserTab (Act_GetActionFromActCod (ActCod)) == Act_DOWNLD_FILE) ? "DAT_SMALL_YELLOW" :
	         (ActCod == Act_GetActCod (ActLogIn   ) ||
	          ActCod == Act_GetActCod (ActLogInNew)) ? "DAT_SMALL_GREEN" :
                 (ActCod == Act_GetActCod (ActLogOut  )) ? "DAT_SMALL_RED" :
                 (ActCod == Act_GetActCod (ActWebSvc  )) ? "DAT_SMALL_BLUE" :
                                                           "DAT_SMALL_GREY";

      /* Compute elapsed time from last access */
      if (sscanf (row[2],"%ld",&TimeDiff) != 1)
         TimeDiff = (time_t) 0;

      /* Get country code (row[4]) */
      Cty.CtyCod = Str_ConvertStrCodToLongCod (row[4]);
      Cty_GetCountryName (Cty.CtyCod,Cty.Name[Gbl.Prefs.Language]);

      /* Get institution code (row[5]) */
      Ins.InsCod = Str_ConvertStrCodToLongCod (row[5]);
      Ins_GetShortNameOfInstitution (&Ins);

      /* Get centre code (row[6]) */
      Ctr.CtrCod = Str_ConvertStrCodToLongCod (row[6]);
      Ctr_GetShortNameOfCentreByCod (&Ctr);

      /* Get degree code (row[7]) */
      Deg.DegCod = Str_ConvertStrCodToLongCod (row[7]);
      Deg_GetShortNameOfDegreeByCod (&Deg);

      /* Print table row */
      Tbl_TR_Begin (NULL);

      Tbl_TD_Begin ("class=\"LC_CLK %s\"",ClassRow);
      fprintf (Gbl.F.Out,"%s",row[0]);				// Click
      Tbl_TD_End ();

      Tbl_TD_Begin ("class=\"LC_TIM %s\"",ClassRow);		// Elapsed time
      Dat_WriteHoursMinutesSecondsFromSeconds (TimeDiff);
      Tbl_TD_End ();

      Tbl_TD_Begin ("class=\"LC_ROL %s\"",ClassRow);
      fprintf (Gbl.F.Out,"%s",					// Role
	       Txt_ROLES_SINGUL_Abc[Rol_ConvertUnsignedStrToRole (row[3])][Usr_SEX_UNKNOWN]);
      Tbl_TD_End ();

      Tbl_TD_Begin ("class=\"LC_CTY %s\"",ClassRow);
      fprintf (Gbl.F.Out,"%s",Cty.Name[Gbl.Prefs.Language]);	// Country
      Tbl_TD_End ();

      Tbl_TD_Begin ("class=\"LC_INS %s\"",ClassRow);
      fprintf (Gbl.F.Out,"%s",Ins.ShrtName);			// Institution
      Tbl_TD_End ();

      Tbl_TD_Begin ("class=\"LC_CTR %s\"",ClassRow);
      fprintf (Gbl.F.Out,"%s",Ctr.ShrtName);			// Centre
      Tbl_TD_End ();

      Tbl_TD_Begin ("class=\"LC_DEG %s\"",ClassRow);
      fprintf (Gbl.F.Out,"%s",Deg.ShrtName);			// Degree
      Tbl_TD_End ();

      Tbl_TD_Begin ("class=\"LC_ACT %s\"",ClassRow);
      if (row[8])
	 if (row[8][0])
	    fprintf (Gbl.F.Out,"%s",row[8]);			// Action
      Tbl_TD_End ();

      Tbl_TR_End ();
     }
   Tbl_TABLE_End ();

   /***** Free structure that stores the query result *****/
   mysql_free_result (mysql_res);
  }
