// swad_attendance.c: control of attendance

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

#define _GNU_SOURCE 		// For asprintf
#include <linux/limits.h>	// For PATH_MAX
#include <mysql/mysql.h>	// To access MySQL databases
#include <stddef.h>		// For NULL
#include <stdio.h>		// For asprintf
#include <stdlib.h>		// For calloc
#include <string.h>		// For string functions

#include "swad_attendance.h"
#include "swad_box.h"
#include "swad_database.h"
#include "swad_form.h"
#include "swad_global.h"
#include "swad_group.h"
#include "swad_HTML.h"
#include "swad_ID.h"
#include "swad_pagination.h"
#include "swad_parameter.h"
#include "swad_photo.h"
#include "swad_QR.h"
#include "swad_setting.h"

/*****************************************************************************/
/*************** External global variables from others modules ***************/
/*****************************************************************************/

extern struct Globals Gbl;

/*****************************************************************************/
/****************************** Private constants ****************************/
/*****************************************************************************/

#define Att_ATTENDANCE_TABLE_ID		"att_table"
#define Att_ATTENDANCE_DETAILS_ID	"att_details"

/*****************************************************************************/
/******************************** Private types ******************************/
/*****************************************************************************/

typedef enum
  {
   Att_VIEW_ONLY_ME,	// View only me
   Att_VIEW_SEL_USR,	// View selected users
   Att_PRNT_ONLY_ME,	// Print only me
   Att_PRNT_SEL_USR,	// Print selected users
  } Att_TypeOfView_t;

/*****************************************************************************/
/****************************** Private variables ****************************/
/*****************************************************************************/

/*****************************************************************************/
/****************************** Private prototypes ***************************/
/*****************************************************************************/

static void Att_ResetEvents (struct Att_Events *Events);

static void Att_ShowAllAttEvents (struct Att_Events *Events);
static void Att_ParamsWhichGroupsToShow (void *Events);
static void Att_PutIconsInListOfAttEvents (void *Events);
static void Att_PutIconToCreateNewAttEvent (struct Att_Events *Events);
static void Att_PutButtonToCreateNewAttEvent (struct Att_Events *Events);
static void Att_PutParamsToCreateNewAttEvent (void *Events);
static void Att_PutParamsToListUsrsAttendance (void *Events);

static void Att_ShowOneAttEvent (struct Att_Events *Events,
                                 struct Att_Event *Event,
                                 bool ShowOnlyThisAttEventComplete);
static void Att_WriteAttEventAuthor (struct Att_Event *Event);
static Dat_StartEndTime_t Att_GetParamAttOrder (void);

static void Att_PutFormsToRemEditOneAttEvent (struct Att_Events *Events,
					      const struct Att_Event *Event,
                                              const char *Anchor);
static void Att_PutParams (void *Events);
static void Att_GetListAttEvents (struct Att_Events *Events,
                                  Att_OrderNewestOldest_t OrderNewestOldest);
static void Att_GetDataOfAttEventByCodAndCheckCrs (struct Att_Event *Event);
static void Att_ResetAttendanceEvent (struct Att_Event *Event);
static void Att_FreeListAttEvents (struct Att_Events *Events);
static void Att_GetAttEventDescriptionFromDB (long AttCod,char Description[Cns_MAX_BYTES_TEXT + 1]);

static void Att_PutParamSelectedAttCod (void *Events);
static void Att_PutParamAttCod (long AttCod);
static long Att_GetParamAttCod (void);

static bool Att_CheckIfSimilarAttEventExists (const char *Field,const char *Value,long AttCod);
static void Att_ShowLstGrpsToEditAttEvent (long AttCod);
static void Att_RemoveAllTheGrpsAssociatedToAnAttEvent (long AttCod);
static void Att_CreateGrps (long AttCod);
static void Att_GetAndWriteNamesOfGrpsAssociatedToAttEvent (struct Att_Event *Event);

static void Att_RemoveAllUsrsFromAnAttEvent (long AttCod);
static void Att_RemoveAttEventFromCurrentCrs (long AttCod);

static void Att_ShowEvent (struct Att_Events *Events);

static void Att_ListAttOnlyMeAsStudent (struct Att_Event *Event);
static void Att_ListAttStudents (struct Att_Events *Events,
	                         struct Att_Event *Event);
static void Att_WriteRowUsrToCallTheRoll (unsigned NumUsr,
                                          struct UsrData *UsrDat,
                                          struct Att_Event *Event);
static void Att_PutLinkAttEvent (struct Att_Event *AttEvent,
				 const char *Title,const char *Txt,
				 const char *Class);
static void Att_PutParamsCodGrps (long AttCod);
static void Att_GetNumStdsTotalWhoAreInAttEvent (struct Att_Event *Event);
static unsigned Att_GetNumUsrsFromAListWhoAreInAttEvent (long AttCod,
							 long LstSelectedUsrCods[],
							 unsigned NumUsrsInList);
static bool Att_CheckIfUsrIsInTableAttUsr (long AttCod,long UsrCod,bool *Present);
static bool Att_CheckIfUsrIsPresentInAttEvent (long AttCod,long UsrCod);
static bool Att_CheckIfUsrIsPresentInAttEventAndGetComments (long AttCod,long UsrCod,
                                                             char CommentStd[Cns_MAX_BYTES_TEXT + 1],
                                                             char CommentTch[Cns_MAX_BYTES_TEXT + 1]);
static void Att_RegUsrInAttEventChangingComments (long AttCod,long UsrCod,bool Present,
                                                  const char *CommentStd,const char *CommentTch);
static void Att_RemoveUsrFromAttEvent (long AttCod,long UsrCod);

static void Att_ReqListOrPrintUsrsAttendanceCrs (void *TypeOfView);
static void Att_ListOrPrintMyAttendanceCrs (Att_TypeOfView_t TypeOfView);
static void Att_GetUsrsAndListOrPrintAttendanceCrs (Att_TypeOfView_t TypeOfView);
static void Att_ListOrPrintUsrsAttendanceCrs (void *TypeOfView);

static void Att_GetListSelectedAttCods (struct Att_Events *Events);

static void Att_PutIconsMyAttList (void *Events);
static void Att_PutFormToPrintMyListParams (void *Events);
static void Att_PutIconsStdsAttList (void *Events);
static void Att_PutParamsToPrintStdsList (void *Events);

static void Att_PutButtonToShowDetails (const struct Att_Events *Events);
static void Att_ListEventsToSelect (const struct Att_Events *Events,
                                    Att_TypeOfView_t TypeOfView);
static void Att_PutIconToViewAttEvents (__attribute__((unused)) void *Args);
static void Att_PutIconToEditAttEvents (__attribute__((unused)) void *Args);
static void Att_ListUsrsAttendanceTable (const struct Att_Events *Events,
                                         Att_TypeOfView_t TypeOfView,
	                                 unsigned NumUsrsInList,
                                         long *LstSelectedUsrCods);
static void Att_WriteTableHeadSeveralAttEvents (const struct Att_Events *Events);
static void Att_WriteRowUsrSeveralAttEvents (const struct Att_Events *Events,
                                             unsigned NumUsr,struct UsrData *UsrDat);
static void Att_PutCheckOrCross (bool Present);
static void Att_ListStdsWithAttEventsDetails (const struct Att_Events *Events,
                                              unsigned NumUsrsInList,
                                              long *LstSelectedUsrCods);
static void Att_ListAttEventsForAStd (const struct Att_Events *Events,
                                      unsigned NumUsr,struct UsrData *UsrDat);

/*****************************************************************************/
/************************** Reset attendance events **************************/
/*****************************************************************************/

static void Att_ResetEvents (struct Att_Events *Events)
  {
   Events->LstIsRead          = false;		// List is not read from database
   Events->Num                = 0;		// Number of attendance events
   Events->Lst                = NULL;		// List of attendance events
   Events->SelectedOrder      = Att_ORDER_DEFAULT;
   Events->AttCod             = -1L;
   Events->ShowDetails        = false;
   Events->StrAttCodsSelected = NULL;
   Events->CurrentPage        = 0;
  }

/*****************************************************************************/
/********************** List all the attendance events ***********************/
/*****************************************************************************/

void Att_SeeAttEvents (void)
  {
   struct Att_Events Events;

   /***** Reset attendance events *****/
   Att_ResetEvents (&Events);

   /***** Get parameters *****/
   Events.SelectedOrder = Att_GetParamAttOrder ();
   Grp_GetParamWhichGroups ();
   Events.CurrentPage = Pag_GetParamPagNum (Pag_ATT_EVENTS);

   /***** Get list of attendance events *****/
   Att_GetListAttEvents (&Events,Att_NEWEST_FIRST);

   /***** Show all the attendance events *****/
   Att_ShowAllAttEvents (&Events);
  }

/*****************************************************************************/
/********************** Show all the attendance events ***********************/
/*****************************************************************************/

static void Att_ShowAllAttEvents (struct Att_Events *Events)
  {
   extern const char *Hlp_USERS_Attendance;
   extern const char *Txt_Events;
   extern const char *Txt_START_END_TIME_HELP[Dat_NUM_START_END_TIME];
   extern const char *Txt_START_END_TIME[Dat_NUM_START_END_TIME];
   extern const char *Txt_Event;
   extern const char *Txt_ROLES_PLURAL_Abc[Rol_NUM_ROLES][Usr_NUM_SEXS];
   extern const char *Txt_No_events;
   struct Pagination Pagination;
   Dat_StartEndTime_t Order;
   Grp_WhichGroups_t WhichGroups;
   unsigned NumAttEvent;
   bool ICanEdit = (Gbl.Usrs.Me.Role.Logged == Rol_TCH ||
		    Gbl.Usrs.Me.Role.Logged == Rol_SYS_ADM);

   /***** Compute variables related to pagination *****/
   Pagination.NumItems = Events->Num;
   Pagination.CurrentPage = (int) Events->CurrentPage;
   Pag_CalculatePagination (&Pagination);
   Events->CurrentPage = (unsigned) Pagination.CurrentPage;

   /***** Begin box *****/
   Box_BoxBegin ("100%",Txt_Events,
                 Att_PutIconsInListOfAttEvents,Events,
		 Hlp_USERS_Attendance,Box_NOT_CLOSABLE);

   /***** Select whether show only my groups or all groups *****/
   if (Gbl.Crs.Grps.NumGrps)
     {
      Set_StartSettingsHead ();
      Grp_ShowFormToSelWhichGrps (ActSeeAtt,
                                  Att_ParamsWhichGroupsToShow,&Events);
      Set_EndSettingsHead ();
     }

   /***** Write links to pages *****/
   Pag_WriteLinksToPagesCentered (Pag_ATT_EVENTS,&Pagination,
				  Events,-1L);

   if (Events->Num)
     {
      /***** Table head *****/
      HTM_TABLE_BeginWideMarginPadding (2);
      HTM_TR_Begin (NULL);

      HTM_TH (1,1,"CONTEXT_COL",NULL);	// Column for contextual icons
      for (Order  = Dat_START_TIME;
	   Order <= Dat_END_TIME;
	   Order++)
	{
	 HTM_TH_Begin (1,1,"LM");

	 Frm_StartForm (ActSeeAtt);
         WhichGroups = Grp_GetParamWhichGroups ();
	 Grp_PutParamWhichGroups (&WhichGroups);
	 Pag_PutHiddenParamPagNum (Pag_ATT_EVENTS,Events->CurrentPage);
	 Dat_PutHiddenParamOrder (Order);
	 HTM_BUTTON_SUBMIT_Begin (Txt_START_END_TIME_HELP[Order],"BT_LINK TIT_TBL",NULL);
	 if (Order == Events->SelectedOrder)
	    HTM_U_Begin ();
	 HTM_Txt (Txt_START_END_TIME[Order]);
	 if (Order == Events->SelectedOrder)
	    HTM_U_End ();
	 HTM_BUTTON_End ();
	 Frm_EndForm ();

	 HTM_TH_End ();
	}
      HTM_TH (1,1,"LM",Txt_Event);
      HTM_TH (1,1,"RM",Txt_ROLES_PLURAL_Abc[Rol_STD][Usr_SEX_UNKNOWN]);

      HTM_TR_End ();

      /***** Write all the attendance events *****/
      for (NumAttEvent  = Pagination.FirstItemVisible, Gbl.RowEvenOdd = 0;
	   NumAttEvent <= Pagination.LastItemVisible;
	   NumAttEvent++)
	 Att_ShowOneAttEvent (Events,
	                      &Events->Lst[NumAttEvent - 1],
	                      false);

      /***** End table *****/
      HTM_TABLE_End ();
     }
   else	// No events created
      Ale_ShowAlert (Ale_INFO,Txt_No_events);

   /***** Write again links to pages *****/
   Pag_WriteLinksToPagesCentered (Pag_ATT_EVENTS,&Pagination,
				  Events,-1L);

   /***** Button to create a new attendance event *****/
   if (ICanEdit)
      Att_PutButtonToCreateNewAttEvent (Events);

   /***** End box *****/
   Box_BoxEnd ();

   /***** Free list of attendance events *****/
   Att_FreeListAttEvents (Events);
  }

/*****************************************************************************/
/***************** Put params to select which groups to show *****************/
/*****************************************************************************/

static void Att_ParamsWhichGroupsToShow (void *Events)
  {
   if (Events)
     {
      Dat_PutHiddenParamOrder (((struct Att_Events *) Events)->SelectedOrder);
      Pag_PutHiddenParamPagNum (Pag_ATT_EVENTS,((struct Att_Events *) Events)->CurrentPage);
     }
  }

/*****************************************************************************/
/************* Put contextual icons in list of attendance events *************/
/*****************************************************************************/

static void Att_PutIconsInListOfAttEvents (void *Events)
  {
   bool ICanEdit;

   if (Events)
     {
      /***** Put icon to create a new attendance event *****/
      ICanEdit = (Gbl.Usrs.Me.Role.Logged == Rol_TCH ||
		  Gbl.Usrs.Me.Role.Logged == Rol_SYS_ADM);
      if (ICanEdit)
	 Att_PutIconToCreateNewAttEvent ((struct Att_Events *) Events);

      /***** Put icon to show attendance list *****/
      if (((struct Att_Events *) Events)->Num)
	 switch (Gbl.Usrs.Me.Role.Logged)
	   {
	    case Rol_STD:
	       Ico_PutContextualIconToShowAttendanceList (ActSeeLstMyAtt,
	                                                  NULL,NULL);
	       break;
	    case Rol_NET:
	    case Rol_TCH:
	    case Rol_SYS_ADM:
	       Ico_PutContextualIconToShowAttendanceList (ActReqLstUsrAtt,
							  Att_PutParamsToListUsrsAttendance,Events);
	       break;
	    default:
	       break;
	   }

      /***** Put icon to print my QR code *****/
      QR_PutLinkToPrintQRCode (ActPrnUsrQR,
			       Usr_PutParamMyUsrCodEncrypted,Gbl.Usrs.Me.UsrDat.EncryptedUsrCod);
     }
  }

/*****************************************************************************/
/**************** Put icon to create a new attendance event ******************/
/*****************************************************************************/

static void Att_PutIconToCreateNewAttEvent (struct Att_Events *Events)
  {
   extern const char *Txt_New_event;

   /***** Put icon to create a new attendance event *****/
   Ico_PutContextualIconToAdd (ActFrmNewAtt,NULL,
			       Att_PutParamsToCreateNewAttEvent,Events,
			       Txt_New_event);
  }

/*****************************************************************************/
/**************** Put button to create a new attendance event ****************/
/*****************************************************************************/

static void Att_PutButtonToCreateNewAttEvent (struct Att_Events *Events)
  {
   extern const char *Txt_New_event;

   Frm_StartForm (ActFrmNewAtt);
   Att_PutParamsToCreateNewAttEvent (Events);
   Btn_PutConfirmButton (Txt_New_event);
   Frm_EndForm ();
  }

/*****************************************************************************/
/************** Put parameters to create a new attendance event **************/
/*****************************************************************************/

static void Att_PutParamsToCreateNewAttEvent (void *Events)
  {
   Grp_WhichGroups_t WhichGroups;

   if (Events)
     {
      Dat_PutHiddenParamOrder (((struct Att_Events *) Events)->SelectedOrder);
      WhichGroups = Grp_GetParamWhichGroups ();
      Grp_PutParamWhichGroups (&WhichGroups);
      Pag_PutHiddenParamPagNum (Pag_ATT_EVENTS,((struct Att_Events *) Events)->CurrentPage);
     }
  }

/*****************************************************************************/
/***************** Put parameters to list users attendance *******************/
/*****************************************************************************/

static void Att_PutParamsToListUsrsAttendance (void *Events)
  {
   Grp_WhichGroups_t WhichGroups;

   if (Events)
     {
      Dat_PutHiddenParamOrder (((struct Att_Events *) Events)->SelectedOrder);
      WhichGroups = Grp_GetParamWhichGroups ();
      Grp_PutParamWhichGroups (&WhichGroups);
      Pag_PutHiddenParamPagNum (Pag_ATT_EVENTS,((struct Att_Events *) Events)->CurrentPage);
     }
  }

/*****************************************************************************/
/************************* Show one attendance event *************************/
/*****************************************************************************/
// Only Event->AttCod must be filled

static void Att_ShowOneAttEvent (struct Att_Events *Events,
                                 struct Att_Event *Event,
                                 bool ShowOnlyThisAttEventComplete)
  {
   extern const char *Txt_View_event;
   char *Anchor = NULL;
   static unsigned UniqueId = 0;
   char *Id;
   Dat_StartEndTime_t StartEndTime;
   char Description[Cns_MAX_BYTES_TEXT + 1];

   /***** Get data of this attendance event *****/
   Att_GetDataOfAttEventByCodAndCheckCrs (Event);
   Att_GetNumStdsTotalWhoAreInAttEvent (Event);

   /***** Set anchor string *****/
   Frm_SetAnchorStr (Event->AttCod,&Anchor);

   /***** Write first row of data of this attendance event *****/
   /* Forms to remove/edit this attendance event */
   HTM_TR_Begin (NULL);

   if (ShowOnlyThisAttEventComplete)
      HTM_TD_Begin ("rowspan=\"2\" class=\"CONTEXT_COL\"");
   else
      HTM_TD_Begin ("rowspan=\"2\" class=\"CONTEXT_COL COLOR%u\"",Gbl.RowEvenOdd);
   switch (Gbl.Usrs.Me.Role.Logged)
     {
      case Rol_TCH:
      case Rol_SYS_ADM:
         Att_PutFormsToRemEditOneAttEvent (Events,Event,Anchor);
	 break;
      default:
         break;
     }
   HTM_TD_End ();

   /* Start/end date/time */
   UniqueId++;
   for (StartEndTime  = (Dat_StartEndTime_t) 0;
	StartEndTime <= (Dat_StartEndTime_t) (Dat_NUM_START_END_TIME - 1);
	StartEndTime++)
     {
      if (asprintf (&Id,"att_date_%u_%u",(unsigned) StartEndTime,UniqueId) < 0)
	 Lay_NotEnoughMemoryExit ();
      if (ShowOnlyThisAttEventComplete)
	 HTM_TD_Begin ("id=\"%s\" class=\"%s LB\"",
		       Id,
		       Event->Hidden ? (Event->Open ? "DATE_GREEN_LIGHT" :
						      "DATE_RED_LIGHT") :
				       (Event->Open ? "DATE_GREEN" :
						      "DATE_RED"));
      else
	 HTM_TD_Begin ("id=\"%s\" class=\"%s LB COLOR%u\"",
		       Id,
		       Event->Hidden ? (Event->Open ? "DATE_GREEN_LIGHT" :
						      "DATE_RED_LIGHT") :
				       (Event->Open ? "DATE_GREEN" :
						      "DATE_RED"),
		       Gbl.RowEvenOdd);
      Dat_WriteLocalDateHMSFromUTC (Id,Event->TimeUTC[StartEndTime],
				    Gbl.Prefs.DateFormat,Dat_SEPARATOR_BREAK,
				    true,true,true,0x7);
      HTM_TD_End ();
      free (Id);
     }

   /* Attendance event title */
   if (ShowOnlyThisAttEventComplete)
      HTM_TD_Begin ("class=\"LT\"");
   else
      HTM_TD_Begin ("class=\"LT COLOR%u\"",Gbl.RowEvenOdd);
   HTM_ARTICLE_Begin (Anchor);
   Att_PutLinkAttEvent (Event,Txt_View_event,Event->Title,
	                Event->Hidden ? "BT_LINK LT ASG_TITLE_LIGHT" :
	                                "BT_LINK LT ASG_TITLE");
   HTM_ARTICLE_End ();
   HTM_TD_End ();

   /* Number of students in this event */
   if (ShowOnlyThisAttEventComplete)
      HTM_TD_Begin ("class=\"%s RT\"",
		    Event->Hidden ? "ASG_TITLE_LIGHT" :
				    "ASG_TITLE");
   else
      HTM_TD_Begin ("class=\"%s RT COLOR%u\"",
		    Event->Hidden ? "ASG_TITLE_LIGHT" :
				    "ASG_TITLE",
		    Gbl.RowEvenOdd);
   HTM_Unsigned (Event->NumStdsTotal);
   HTM_TD_End ();

   HTM_TR_End ();

   /***** Write second row of data of this attendance event *****/
   HTM_TR_Begin (NULL);

   /* Author of the attendance event */
   if (ShowOnlyThisAttEventComplete)
      HTM_TD_Begin ("colspan=\"2\" class=\"LT\"");
   else
      HTM_TD_Begin ("colspan=\"2\" class=\"LT COLOR%u\"",Gbl.RowEvenOdd);
   Att_WriteAttEventAuthor (Event);
   HTM_TD_End ();

   /* Text of the attendance event */
   Att_GetAttEventDescriptionFromDB (Event->AttCod,Description);
   Str_ChangeFormat (Str_FROM_HTML,Str_TO_RIGOROUS_HTML,
                     Description,Cns_MAX_BYTES_TEXT,false);	// Convert from HTML to recpectful HTML
   Str_InsertLinks (Description,Cns_MAX_BYTES_TEXT,60);	// Insert links
   if (ShowOnlyThisAttEventComplete)
      HTM_TD_Begin ("colspan=\"2\" class=\"LT\"");
   else
      HTM_TD_Begin ("colspan=\"2\" class=\"LT COLOR%u\"",Gbl.RowEvenOdd);
   if (Gbl.Crs.Grps.NumGrps)
      Att_GetAndWriteNamesOfGrpsAssociatedToAttEvent (Event);
   HTM_DIV_Begin ("class=\"%s\"",Event->Hidden ? "DAT_LIGHT" :
        	                                 "DAT");
   HTM_Txt (Description);
   HTM_DIV_End ();
   HTM_TD_End ();

   HTM_TR_End ();

   /***** Free anchor string *****/
   Frm_FreeAnchorStr (Anchor);

   Gbl.RowEvenOdd = 1 - Gbl.RowEvenOdd;
  }

/*****************************************************************************/
/****************** Write the author of an attendance event ******************/
/*****************************************************************************/

static void Att_WriteAttEventAuthor (struct Att_Event *Event)
  {
   Usr_WriteAuthor1Line (Event->UsrCod,Event->Hidden);
  }

/*****************************************************************************/
/**** Get parameter with the type or order in list of attendance events ******/
/*****************************************************************************/

static Dat_StartEndTime_t Att_GetParamAttOrder (void)
  {
   return (Dat_StartEndTime_t)
	  Par_GetParToUnsignedLong ("Order",
				    0,
				    Dat_NUM_START_END_TIME - 1,
				    (unsigned long) Att_ORDER_DEFAULT);
  }

/*****************************************************************************/
/************** Put a link (form) to edit one attendance event ***************/
/*****************************************************************************/

static void Att_PutFormsToRemEditOneAttEvent (struct Att_Events *Events,
					      const struct Att_Event *Event,
                                              const char *Anchor)
  {
   Events->AttCod = Event->AttCod;

   /***** Put form to remove attendance event *****/
   Ico_PutContextualIconToRemove (ActReqRemAtt,
                                  Att_PutParams,Events);

   /***** Put form to hide/show attendance event *****/
   if (Event->Hidden)
      Ico_PutContextualIconToUnhide (ActShoAtt,Anchor,
                                     Att_PutParams,Events);
   else
      Ico_PutContextualIconToHide (ActHidAtt,Anchor,
                                   Att_PutParams,Events);

   /***** Put form to edit attendance event *****/
   Ico_PutContextualIconToEdit (ActEdiOneAtt,NULL,
                                Att_PutParams,Events);
  }

/*****************************************************************************/
/***************** Params used to edit an attendance event *******************/
/*****************************************************************************/

static void Att_PutParams (void *Events)
  {
   Grp_WhichGroups_t WhichGroups;

   if (Events)
     {
      Att_PutParamAttCod (((struct Att_Events *) Events)->AttCod);
      Dat_PutHiddenParamOrder (((struct Att_Events *) Events)->SelectedOrder);
      WhichGroups = Grp_GetParamWhichGroups ();
      Grp_PutParamWhichGroups (&WhichGroups);
      Pag_PutHiddenParamPagNum (Pag_ATT_EVENTS,((struct Att_Events *) Events)->CurrentPage);
     }
  }

/*****************************************************************************/
/********************* List all the attendance events ************************/
/*****************************************************************************/

static void Att_GetListAttEvents (struct Att_Events *Events,
                                  Att_OrderNewestOldest_t OrderNewestOldest)
  {
   static const char *HiddenSubQuery[Rol_NUM_ROLES] =
     {
      [Rol_UNK    ] = " AND Hidden='N'",
      [Rol_GST    ] = " AND Hidden='N'",
      [Rol_USR    ] = " AND Hidden='N'",
      [Rol_STD    ] = " AND Hidden='N'",
      [Rol_NET    ] = " AND Hidden='N'",
      [Rol_TCH    ] = "",
      [Rol_DEG_ADM] = " AND Hidden='N'",
      [Rol_CTR_ADM] = " AND Hidden='N'",
      [Rol_INS_ADM] = " AND Hidden='N'",
      [Rol_SYS_ADM] = "",
     };
   static const char *OrderBySubQuery[Dat_NUM_START_END_TIME][Att_NUM_ORDERS_NEWEST_OLDEST] =
     {
      [Dat_START_TIME][Att_NEWEST_FIRST] = "StartTime DESC,EndTime DESC,Title DESC",
      [Dat_START_TIME][Att_OLDEST_FIRST] = "StartTime,EndTime,Title",

      [Dat_END_TIME  ][Att_NEWEST_FIRST] = "EndTime DESC,StartTime DESC,Title DESC",
      [Dat_END_TIME  ][Att_OLDEST_FIRST] = "EndTime,StartTime,Title",
     };
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows;
   unsigned NumAttEvent;

   if (Events->LstIsRead)
      Att_FreeListAttEvents (Events);

   /***** Get list of attendance events from database *****/
   if (Gbl.Crs.Grps.WhichGrps == Grp_MY_GROUPS)
      NumRows = DB_QuerySELECT (&mysql_res,"can not get attendance events",
				"SELECT AttCod"
				" FROM att_events"
				" WHERE CrsCod=%ld%s"
				" AND (AttCod NOT IN (SELECT AttCod FROM att_grp) OR"
				" AttCod IN (SELECT att_grp.AttCod FROM att_grp,crs_grp_usr"
				" WHERE crs_grp_usr.UsrCod=%ld"
				" AND att_grp.GrpCod=crs_grp_usr.GrpCod))"
				" ORDER BY %s",
				Gbl.Hierarchy.Crs.CrsCod,
				HiddenSubQuery[Gbl.Usrs.Me.Role.Logged],
				Gbl.Usrs.Me.UsrDat.UsrCod,
				OrderBySubQuery[Events->SelectedOrder][OrderNewestOldest]);
   else	// Gbl.Crs.Grps.WhichGrps == Grp_ALL_GROUPS
      NumRows = DB_QuerySELECT (&mysql_res,"can not get attendance events",
				"SELECT AttCod"
				" FROM att_events"
				" WHERE CrsCod=%ld%s"
				" ORDER BY %s",
				Gbl.Hierarchy.Crs.CrsCod,
				HiddenSubQuery[Gbl.Usrs.Me.Role.Logged],
				OrderBySubQuery[Events->SelectedOrder][OrderNewestOldest]);

   if (NumRows) // Attendance events found...
     {
      Events->Num = (unsigned) NumRows;

      /***** Create list of attendance events *****/
      if ((Events->Lst = (struct Att_Event *) calloc (NumRows,sizeof (struct Att_Event))) == NULL)
         Lay_NotEnoughMemoryExit ();

      /***** Get the attendance events codes *****/
      for (NumAttEvent = 0;
	   NumAttEvent < Events->Num;
	   NumAttEvent++)
        {
         /* Get next attendance event code */
         row = mysql_fetch_row (mysql_res);
         if ((Events->Lst[NumAttEvent].AttCod = Str_ConvertStrCodToLongCod (row[0])) < 0)
            Lay_ShowErrorAndExit ("Error: wrong attendance event code.");
        }
     }
   else
      Events->Num = 0;

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   Events->LstIsRead = true;
  }

/*****************************************************************************/
/********* Get attendance event data using its code and check course *********/
/*****************************************************************************/

static void Att_GetDataOfAttEventByCodAndCheckCrs (struct Att_Event *Event)
  {
   if (Att_GetDataOfAttEventByCod (Event))
     {
      if (Event->CrsCod != Gbl.Hierarchy.Crs.CrsCod)
         Lay_ShowErrorAndExit ("Attendance event does not belong to current course.");
     }
   else	// Attendance event not found
      Lay_ShowErrorAndExit ("Error when getting attendance event.");
  }

/*****************************************************************************/
/**************** Get attendance event data using its code *******************/
/*****************************************************************************/
// Returns true if attendance event exists
// This function can be called from web service, so do not display messages

bool Att_GetDataOfAttEventByCod (struct Att_Event *Event)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows;
   bool Found = false;

   /***** Reset attendance event data *****/
   Att_ResetAttendanceEvent (Event);

   if (Event->AttCod > 0)
     {
      /***** Build query *****/
      NumRows = DB_QuerySELECT (&mysql_res,"can not get attendance event data",
	                        "SELECT AttCod,CrsCod,Hidden,UsrCod,"
				"UNIX_TIMESTAMP(StartTime),"
				"UNIX_TIMESTAMP(EndTime),"
				"NOW() BETWEEN StartTime AND EndTime,"
				"CommentTchVisible,"
				"Title"
				" FROM att_events"
				" WHERE AttCod=%ld",
				Event->AttCod);

      /***** Get data of attendance event from database *****/
      if ((Found = (NumRows != 0))) // Attendance event found...
	{
	 /* Get row */
	 row = mysql_fetch_row (mysql_res);

	 /* Get code of the attendance event (row[0]) */
	 Event->AttCod = Str_ConvertStrCodToLongCod (row[0]);

	 /* Get code of the course (row[1]) */
	 Event->CrsCod = Str_ConvertStrCodToLongCod (row[1]);

	 /* Get whether the attendance event is hidden or not (row[2]) */
	 Event->Hidden = (row[2][0] == 'Y');

	 /* Get author of the attendance event (row[3]) */
	 Event->UsrCod = Str_ConvertStrCodToLongCod (row[3]);

	 /* Get start date (row[4] holds the start UTC time) */
	 Event->TimeUTC[Att_START_TIME] = Dat_GetUNIXTimeFromStr (row[4]);

	 /* Get end   date (row[5] holds the end   UTC time) */
	 Event->TimeUTC[Att_END_TIME  ] = Dat_GetUNIXTimeFromStr (row[5]);

	 /* Get whether the attendance event is open or closed (row(6)) */
	 Event->Open = (row[6][0] == '1');

	 /* Get whether the attendance event is visible or not (row[7]) */
	 Event->CommentTchVisible = (row[7][0] == 'Y');

	 /* Get the title of the attendance event (row[8]) */
	 Str_Copy (Event->Title,row[8],
	           Att_MAX_BYTES_ATTENDANCE_EVENT_TITLE);
	}

      /***** Free structure that stores the query result *****/
      DB_FreeMySQLResult (&mysql_res);
     }

   return Found;
  }

/*****************************************************************************/
/********************** Clear all attendance event data **********************/
/*****************************************************************************/

static void Att_ResetAttendanceEvent (struct Att_Event *Event)
  {
   if (Event->AttCod <= 0)	// If > 0 ==> keep values of AttCod and Selected
     {
      Event->AttCod = -1L;
      Event->NumStdsTotal = 0;
      Event->NumStdsFromList = 0;
      Event->Selected = false;
     }
   Event->CrsCod = -1L;
   Event->Hidden = false;
   Event->UsrCod = -1L;
   Event->TimeUTC[Att_START_TIME] =
   Event->TimeUTC[Att_END_TIME  ] = (time_t) 0;
   Event->Open = false;
   Event->Title[0] = '\0';
   Event->CommentTchVisible = false;
  }

/*****************************************************************************/
/********************** Free list of attendance events ***********************/
/*****************************************************************************/

static void Att_FreeListAttEvents (struct Att_Events *Events)
  {
   if (Events->LstIsRead && Events->Lst)
     {
      /***** Free memory used by the list of attendance events *****/
      free (Events->Lst);
      Events->Lst       = NULL;
      Events->Num       = 0;
      Events->LstIsRead = false;
     }
  }

/*****************************************************************************/
/***************** Get attendance event text from database *******************/
/*****************************************************************************/

static void Att_GetAttEventDescriptionFromDB (long AttCod,char Description[Cns_MAX_BYTES_TEXT + 1])
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows;

   /***** Get text of attendance event from database *****/
   NumRows = DB_QuerySELECT (&mysql_res,"can not get attendance event text",
			     "SELECT Txt FROM att_events"
			     " WHERE AttCod=%ld AND CrsCod=%ld",
			     AttCod,Gbl.Hierarchy.Crs.CrsCod);

   /***** The result of the query must have one row or none *****/
   if (NumRows == 1)
     {
      /* Get row */
      row = mysql_fetch_row (mysql_res);

      /* Get info text */
      Str_Copy (Description,row[0],
                Cns_MAX_BYTES_TEXT);
     }
   else
      Description[0] = '\0';

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   if (NumRows > 1)
      Lay_ShowErrorAndExit ("Error when getting attendance event text.");
  }

/*****************************************************************************/
/************** Write parameter with code of attendance event ****************/
/*****************************************************************************/

static void Att_PutParamSelectedAttCod (void *Events)
  {
   if (Events)
      Att_PutParamAttCod (((struct Att_Events *) Events)->AttCod);
  }

static void Att_PutParamAttCod (long AttCod)
  {
   Par_PutHiddenParamLong (NULL,"AttCod",AttCod);
  }

/*****************************************************************************/
/*************** Get parameter with code of attendance event *****************/
/*****************************************************************************/

static long Att_GetParamAttCod (void)
  {
   /***** Get code of attendance event *****/
   return Par_GetParToLong ("AttCod");
  }

/*****************************************************************************/
/********* Ask for confirmation of removing of an attendance event ***********/
/*****************************************************************************/

void Att_AskRemAttEvent (void)
  {
   extern const char *Txt_Do_you_really_want_to_remove_the_event_X;
   extern const char *Txt_Remove_event;
   struct Att_Events Events;
   struct Att_Event Event;
   Grp_WhichGroups_t WhichGroups;

   /***** Reset attendance events *****/
   Att_ResetEvents (&Events);

   /***** Get parameters *****/
   Events.SelectedOrder = Att_GetParamAttOrder ();
   Grp_GetParamWhichGroups ();
   Events.CurrentPage = Pag_GetParamPagNum (Pag_ATT_EVENTS);

   /***** Get attendance event code *****/
   if ((Event.AttCod = Att_GetParamAttCod ()) == -1L)
      Lay_ShowErrorAndExit ("Code of attendance event is missing.");

   /***** Get data of the attendance event from database *****/
   Att_GetDataOfAttEventByCodAndCheckCrs (&Event);

   /***** Button of confirmation of removing *****/
   Frm_StartForm (ActRemAtt);
   Att_PutParamAttCod (Event.AttCod);
   Dat_PutHiddenParamOrder (Events.SelectedOrder);
   WhichGroups = Grp_GetParamWhichGroups ();
   Grp_PutParamWhichGroups (&WhichGroups);
   Pag_PutHiddenParamPagNum (Pag_ATT_EVENTS,Events.CurrentPage);

   /* Ask for confirmation of removing */
   Ale_ShowAlert (Ale_WARNING,Txt_Do_you_really_want_to_remove_the_event_X,
                  Event.Title);

   Btn_PutRemoveButton (Txt_Remove_event);
   Frm_EndForm ();

   /***** Show attendance events again *****/
   Att_SeeAttEvents ();
  }

/*****************************************************************************/
/** Get param., remove an attendance event and show attendance events again **/
/*****************************************************************************/

void Att_GetAndRemAttEvent (void)
  {
   extern const char *Txt_Event_X_removed;
   struct Att_Event Event;

   /***** Get attendance event code *****/
   if ((Event.AttCod = Att_GetParamAttCod ()) == -1L)
      Lay_ShowErrorAndExit ("Code of attendance event is missing.");

   /***** Get data of the attendance event from database *****/
   // Inside this function, the course is checked to be the current one
   Att_GetDataOfAttEventByCodAndCheckCrs (&Event);

   /***** Remove the attendance event from database *****/
   Att_RemoveAttEventFromDB (Event.AttCod);

   /***** Write message to show the change made *****/
   Ale_ShowAlert (Ale_SUCCESS,Txt_Event_X_removed,
	          Event.Title);

   /***** Show attendance events again *****/
   Att_SeeAttEvents ();
  }

/*****************************************************************************/
/**************** Remove an attendance event from database *******************/
/*****************************************************************************/

void Att_RemoveAttEventFromDB (long AttCod)
  {
   /***** Remove users registered in the attendance event *****/
   Att_RemoveAllUsrsFromAnAttEvent (AttCod);

   /***** Remove all the groups of this attendance event *****/
   Att_RemoveAllTheGrpsAssociatedToAnAttEvent (AttCod);

   /***** Remove attendance event *****/
   Att_RemoveAttEventFromCurrentCrs (AttCod);
  }

/*****************************************************************************/
/************************* Hide an attendance event **************************/
/*****************************************************************************/

void Att_HideAttEvent (void)
  {
   struct Att_Event Event;

   /***** Get attendance event code *****/
   if ((Event.AttCod = Att_GetParamAttCod ()) == -1L)
      Lay_ShowErrorAndExit ("Code of attendance event is missing.");

   /***** Get data of the attendance event from database *****/
   Att_GetDataOfAttEventByCodAndCheckCrs (&Event);

   /***** Hide attendance event *****/
   DB_QueryUPDATE ("can not hide attendance event",
		   "UPDATE att_events SET Hidden='Y'"
		   " WHERE AttCod=%ld AND CrsCod=%ld",
                   Event.AttCod,Gbl.Hierarchy.Crs.CrsCod);

   /***** Show attendance events again *****/
   Att_SeeAttEvents ();
  }

/*****************************************************************************/
/************************* Show an attendance event **************************/
/*****************************************************************************/

void Att_ShowAttEvent (void)
  {
   struct Att_Event Event;

   /***** Get attendance event code *****/
   if ((Event.AttCod = Att_GetParamAttCod ()) == -1L)
      Lay_ShowErrorAndExit ("Code of attendance event is missing.");

   /***** Get data of the attendance event from database *****/
   Att_GetDataOfAttEventByCodAndCheckCrs (&Event);

   /***** Hide attendance event *****/
   DB_QueryUPDATE ("can not show attendance event",
		   "UPDATE att_events SET Hidden='N'"
		   " WHERE AttCod=%ld AND CrsCod=%ld",
                   Event.AttCod,Gbl.Hierarchy.Crs.CrsCod);

   /***** Show attendance events again *****/
   Att_SeeAttEvents ();
  }

/*****************************************************************************/
/***** Check if the title or the folder of an attendance event exists ********/
/*****************************************************************************/

static bool Att_CheckIfSimilarAttEventExists (const char *Field,const char *Value,long AttCod)
  {
   /***** Get number of attendance events
          with a field value from database *****/
   return (DB_QueryCOUNT ("can not get similar attendance events",
			  "SELECT COUNT(*) FROM att_events"
			  " WHERE CrsCod=%ld"
			  " AND %s='%s' AND AttCod<>%ld",
			  Gbl.Hierarchy.Crs.CrsCod,
			  Field,Value,AttCod) != 0);
  }

/*****************************************************************************/
/*************** Put a form to create a new attendance event *****************/
/*****************************************************************************/

void Att_RequestCreatOrEditAttEvent (void)
  {
   extern const char *Hlp_USERS_Attendance_new_event;
   extern const char *Hlp_USERS_Attendance_edit_event;
   extern const char *Txt_New_event;
   extern const char *Txt_Edit_event;
   extern const char *Txt_Teachers_comment;
   extern const char *Txt_Title;
   extern const char *Txt_Hidden_MALE_PLURAL;
   extern const char *Txt_Visible_MALE_PLURAL;
   extern const char *Txt_Description;
   extern const char *Txt_Create_event;
   extern const char *Txt_Save_changes;
   struct Att_Events Events;
   struct Att_Event Event;
   bool ItsANewAttEvent;
   Grp_WhichGroups_t WhichGroups;
   char Description[Cns_MAX_BYTES_TEXT + 1];
   static const Dat_SetHMS SetHMS[Dat_NUM_START_END_TIME] =
     {
      [Dat_START_TIME] = Dat_HMS_DO_NOT_SET,
      [Dat_END_TIME  ] = Dat_HMS_DO_NOT_SET
     };

   /***** Reset attendance events *****/
   Att_ResetEvents (&Events);

   /***** Get parameters *****/
   Events.SelectedOrder = Att_GetParamAttOrder ();
   Grp_GetParamWhichGroups ();
   Events.CurrentPage = Pag_GetParamPagNum (Pag_ATT_EVENTS);

   /***** Get the code of the attendance event *****/
   Event.AttCod = Att_GetParamAttCod ();
   ItsANewAttEvent = (Event.AttCod <= 0);

   /***** Get from the database the data of the attendance event *****/
   if (ItsANewAttEvent)
     {
      /* Reset attendance event data */
      Event.AttCod = -1L;
      Att_ResetAttendanceEvent (&Event);

      /* Initialize some fields */
      Event.CrsCod = Gbl.Hierarchy.Crs.CrsCod;
      Event.UsrCod = Gbl.Usrs.Me.UsrDat.UsrCod;
      Event.TimeUTC[Att_START_TIME] = Gbl.StartExecutionTimeUTC;
      Event.TimeUTC[Att_END_TIME  ] = Gbl.StartExecutionTimeUTC + (2 * 60 * 60);	// +2 hours
      Event.Open = true;
     }
   else
     {
      /* Get data of the attendance event from database */
      Att_GetDataOfAttEventByCodAndCheckCrs (&Event);

      /* Get text of the attendance event from database */
      Att_GetAttEventDescriptionFromDB (Event.AttCod,Description);
     }

   /***** Begin form *****/
   if (ItsANewAttEvent)
      Frm_StartForm (ActNewAtt);
   else
     {
      Frm_StartForm (ActChgAtt);
      Att_PutParamAttCod (Event.AttCod);
     }
   Dat_PutHiddenParamOrder (Events.SelectedOrder);
   WhichGroups = Grp_GetParamWhichGroups ();
   Grp_PutParamWhichGroups (&WhichGroups);
   Pag_PutHiddenParamPagNum (Pag_ATT_EVENTS,Events.CurrentPage);

   /***** Begin box and table *****/
   if (ItsANewAttEvent)
      Box_BoxTableBegin (NULL,Txt_New_event,
                         NULL,NULL,
			 Hlp_USERS_Attendance_new_event,Box_NOT_CLOSABLE,2);
   else
      Box_BoxTableBegin (NULL,
                         Event.Title[0] ? Event.Title :
                	                  Txt_Edit_event,
                	 NULL,NULL,
			 Hlp_USERS_Attendance_edit_event,Box_NOT_CLOSABLE,2);

   /***** Attendance event title *****/
   HTM_TR_Begin (NULL);

   /* Label */
   Frm_LabelColumn ("RT","Title",Txt_Title);

   /* Data */
   HTM_TD_Begin ("class=\"LT\"");
   HTM_INPUT_TEXT ("Title",Att_MAX_CHARS_ATTENDANCE_EVENT_TITLE,Event.Title,
                   HTM_DONT_SUBMIT_ON_CHANGE,
		   "id=\"Title\" required=\"required\""
		   " class=\"TITLE_DESCRIPTION_WIDTH\"");
   HTM_TD_End ();

   HTM_TR_End ();

   /***** Assignment start and end dates *****/
   Dat_PutFormStartEndClientLocalDateTimes (Event.TimeUTC,
					    Dat_FORM_SECONDS_ON,
					    SetHMS);

   /***** Visibility of comments *****/
   HTM_TR_Begin (NULL);

   /* Label */
   Frm_LabelColumn ("RT","ComTchVisible",Txt_Teachers_comment);

   /* Data */
   HTM_TD_Begin ("class=\"LT\"");
   HTM_SELECT_Begin (HTM_DONT_SUBMIT_ON_CHANGE,
                     "id=\"ComTchVisible\" name=\"ComTchVisible\"");
   HTM_OPTION (HTM_Type_STRING,"N",!Event.CommentTchVisible,false,
	       "%s",Txt_Hidden_MALE_PLURAL);
   HTM_OPTION (HTM_Type_STRING,"Y",Event.CommentTchVisible,false,
	       "%s",Txt_Visible_MALE_PLURAL);
   HTM_SELECT_End ();
   HTM_TD_End ();

   HTM_TR_End ();

   /***** Attendance event description *****/
   HTM_TR_Begin (NULL);

   /* Label */
   Frm_LabelColumn ("RT","Txt",Txt_Description);

   /* Data */
   HTM_TD_Begin ("class=\"LT\"");
   HTM_TEXTAREA_Begin ("id=\"Txt\" name=\"Txt\" rows=\"5\""
	               " class=\"TITLE_DESCRIPTION_WIDTH\"");
   if (!ItsANewAttEvent)
      HTM_Txt (Description);
   HTM_TEXTAREA_End ();
   HTM_TD_End ();

   HTM_TR_End ();

   /***** Groups *****/
   Att_ShowLstGrpsToEditAttEvent (Event.AttCod);

   /***** End table, send button and end box *****/
   if (ItsANewAttEvent)
      Box_BoxTableWithButtonEnd (Btn_CREATE_BUTTON,Txt_Create_event);
   else
      Box_BoxTableWithButtonEnd (Btn_CONFIRM_BUTTON,Txt_Save_changes);

   /***** End form *****/
   Frm_EndForm ();

   /***** Show current attendance events *****/
   Att_GetListAttEvents (&Events,Att_NEWEST_FIRST);
   Att_ShowAllAttEvents (&Events);
  }

/*****************************************************************************/
/************* Show list of groups to edit and attendance event **************/
/*****************************************************************************/

static void Att_ShowLstGrpsToEditAttEvent (long AttCod)
  {
   extern const char *The_ClassFormInBox[The_NUM_THEMES];
   extern const char *Txt_Groups;
   extern const char *Txt_The_whole_course;
   unsigned NumGrpTyp;

   /***** Get list of groups types and groups in this course *****/
   Grp_GetListGrpTypesAndGrpsInThisCrs (Grp_ONLY_GROUP_TYPES_WITH_GROUPS);

   if (Gbl.Crs.Grps.GrpTypes.Num)
     {
      /***** Begin box and table *****/
      HTM_TR_Begin (NULL);

      HTM_TD_Begin ("class=\"%s RT\"",The_ClassFormInBox[Gbl.Prefs.Theme]);
      HTM_TxtColon (Txt_Groups);
      HTM_TD_End ();

      HTM_TD_Begin ("class=\"LT\"");
      Box_BoxTableBegin ("100%",NULL,
                         NULL,NULL,
                         NULL,Box_NOT_CLOSABLE,0);

      /***** First row: checkbox to select the whole course *****/
      HTM_TR_Begin (NULL);

      HTM_TD_Begin ("colspan=\"7\" class=\"DAT LM\"");
      HTM_LABEL_Begin (NULL);
      HTM_INPUT_CHECKBOX ("WholeCrs",HTM_DONT_SUBMIT_ON_CHANGE,
		          "id=\"WholeCrs\" value=\"Y\"%s"
		          " onclick=\"uncheckChildren(this,'GrpCods')\"",
			  Grp_CheckIfAssociatedToGrps ("att_grp","AttCod",AttCod) ? "" :
				                                                    " checked=\"checked\"");
      HTM_TxtF ("%s&nbsp;%s",Txt_The_whole_course,Gbl.Hierarchy.Crs.ShrtName);
      HTM_LABEL_End ();
      HTM_TD_End ();

      HTM_TR_End ();

      /***** List the groups for each group type *****/
      for (NumGrpTyp = 0;
	   NumGrpTyp < Gbl.Crs.Grps.GrpTypes.Num;
	   NumGrpTyp++)
         if (Gbl.Crs.Grps.GrpTypes.LstGrpTypes[NumGrpTyp].NumGrps)
            Grp_ListGrpsToEditAsgAttSvyEvtMch (&Gbl.Crs.Grps.GrpTypes.LstGrpTypes[NumGrpTyp],
                                            AttCod,Grp_ATT_EVENT);

      /***** End table and box *****/
      Box_BoxTableEnd ();
      HTM_TD_End ();
      HTM_TR_End ();
     }

   /***** Free list of groups types and groups in this course *****/
   Grp_FreeListGrpTypesAndGrps ();
  }

/*****************************************************************************/
/*************** Receive form to create a new attendance event ***************/
/*****************************************************************************/

void Att_ReceiveFormAttEvent (void)
  {
   extern const char *Txt_Already_existed_an_event_with_the_title_X;
   extern const char *Txt_You_must_specify_the_title_of_the_event;
   extern const char *Txt_Created_new_event_X;
   extern const char *Txt_The_event_has_been_modified;
   struct Att_Event OldAtt;
   struct Att_Event ReceivedAtt;
   bool ItsANewAttEvent;
   bool ReceivedAttEventIsCorrect = true;
   char Description[Cns_MAX_BYTES_TEXT + 1];

   /***** Get the code of the attendance event *****/
   ItsANewAttEvent = ((ReceivedAtt.AttCod = Att_GetParamAttCod ()) == -1L);

   if (!ItsANewAttEvent)
     {
      /* Get data of the old (current) attendance event from database */
      OldAtt.AttCod = ReceivedAtt.AttCod;
      Att_GetDataOfAttEventByCodAndCheckCrs (&OldAtt);
      ReceivedAtt.Hidden = OldAtt.Hidden;
     }

   /***** Get start/end date-times *****/
   ReceivedAtt.TimeUTC[Att_START_TIME] = Dat_GetTimeUTCFromForm ("StartTimeUTC");
   ReceivedAtt.TimeUTC[Att_END_TIME  ] = Dat_GetTimeUTCFromForm ("EndTimeUTC"  );

   /***** Get boolean parameter that indicates if teacher's comments are visible by students *****/
   ReceivedAtt.CommentTchVisible = Par_GetParToBool ("ComTchVisible");

   /***** Get attendance event title *****/
   Par_GetParToText ("Title",ReceivedAtt.Title,Att_MAX_BYTES_ATTENDANCE_EVENT_TITLE);

   /***** Get attendance event description *****/
   Par_GetParToHTML ("Txt",Description,Cns_MAX_BYTES_TEXT);	// Store in HTML format (not rigorous)

   /***** Adjust dates *****/
   if (ReceivedAtt.TimeUTC[Att_START_TIME] == 0)
      ReceivedAtt.TimeUTC[Att_START_TIME] = Gbl.StartExecutionTimeUTC;
   if (ReceivedAtt.TimeUTC[Att_END_TIME] == 0)
      ReceivedAtt.TimeUTC[Att_END_TIME] = ReceivedAtt.TimeUTC[Att_START_TIME] + 2 * 60 * 60;	// +2 hours // TODO: 2 * 60 * 60 should be in a #define in swad_config.h

   /***** Check if title is correct *****/
   if (ReceivedAtt.Title[0])	// If there's an attendance event title
     {
      /* If title of attendance event was in database... */
      if (Att_CheckIfSimilarAttEventExists ("Title",ReceivedAtt.Title,ReceivedAtt.AttCod))
        {
         ReceivedAttEventIsCorrect = false;

	 Ale_ShowAlert (Ale_WARNING,Txt_Already_existed_an_event_with_the_title_X,
                        ReceivedAtt.Title);
        }
     }
   else	// If there is not an attendance event title
     {
      ReceivedAttEventIsCorrect = false;
      Ale_ShowAlert (Ale_WARNING,Txt_You_must_specify_the_title_of_the_event);
     }

   /***** Create a new attendance event or update an existing one *****/
   if (ReceivedAttEventIsCorrect)
     {
      /* Get groups for this attendance events */
      Grp_GetParCodsSeveralGrps ();

      if (ItsANewAttEvent)
	{
	 ReceivedAtt.Hidden = false;	// New attendance events are visible by default
         Att_CreateAttEvent (&ReceivedAtt,Description);	// Add new attendance event to database

         /***** Write success message *****/
	 Ale_ShowAlert (Ale_SUCCESS,Txt_Created_new_event_X,
		        ReceivedAtt.Title);
	}
      else
	{
         Att_UpdateAttEvent (&ReceivedAtt,Description);

	 /***** Write success message *****/
	 Ale_ShowAlert (Ale_SUCCESS,Txt_The_event_has_been_modified);
	}

      /* Free memory for list of selected groups */
      Grp_FreeListCodSelectedGrps ();
     }
   else
      Att_RequestCreatOrEditAttEvent ();

   /***** Show attendance events again *****/
   Att_SeeAttEvents ();
  }

/*****************************************************************************/
/********************* Create a new attendance event *************************/
/*****************************************************************************/

void Att_CreateAttEvent (struct Att_Event *Event,const char *Description)
  {
   /***** Create a new attendance event *****/
   Event->AttCod =
   DB_QueryINSERTandReturnCode ("can not create new attendance event",
				"INSERT INTO att_events"
				" (CrsCod,Hidden,UsrCod,"
				"StartTime,EndTime,CommentTchVisible,Title,Txt)"
				" VALUES"
				" (%ld,'%c',%ld,"
				"FROM_UNIXTIME(%ld),FROM_UNIXTIME(%ld),'%c','%s','%s')",
				Gbl.Hierarchy.Crs.CrsCod,
				Event->Hidden ? 'Y' :
					      'N',
				Gbl.Usrs.Me.UsrDat.UsrCod,
				Event->TimeUTC[Att_START_TIME],
				Event->TimeUTC[Att_END_TIME  ],
				Event->CommentTchVisible ? 'Y' :
							 'N',
				Event->Title,
				Description);

   /***** Create groups *****/
   if (Gbl.Crs.Grps.LstGrpsSel.NumGrps)
      Att_CreateGrps (Event->AttCod);
  }

/*****************************************************************************/
/****************** Update an existing attendance event **********************/
/*****************************************************************************/

void Att_UpdateAttEvent (struct Att_Event *Event,const char *Description)
  {
   /***** Update the data of the attendance event *****/
   DB_QueryUPDATE ("can not update attendance event",
		   "UPDATE att_events SET "
		   "Hidden='%c',"
		   "StartTime=FROM_UNIXTIME(%ld),"
		   "EndTime=FROM_UNIXTIME(%ld),"
		   "CommentTchVisible='%c',Title='%s',Txt='%s'"
		   " WHERE AttCod=%ld AND CrsCod=%ld",
                   Event->Hidden ? 'Y' :
        	                 'N',
                   Event->TimeUTC[Att_START_TIME],
                   Event->TimeUTC[Att_END_TIME  ],
                   Event->CommentTchVisible ? 'Y' :
        	                            'N',
                   Event->Title,
                   Description,
                   Event->AttCod,Gbl.Hierarchy.Crs.CrsCod);

   /***** Update groups *****/
   /* Remove old groups */
   Att_RemoveAllTheGrpsAssociatedToAnAttEvent (Event->AttCod);

   /* Create new groups */
   if (Gbl.Crs.Grps.LstGrpsSel.NumGrps)
      Att_CreateGrps (Event->AttCod);
  }

/*****************************************************************************/
/****************** Remove groups of an attendance event *********************/
/*****************************************************************************/

static void Att_RemoveAllTheGrpsAssociatedToAnAttEvent (long AttCod)
  {
   /***** Remove groups of the attendance event *****/
   DB_QueryDELETE ("can not remove the groups"
		   " associated to an attendance event",
		   "DELETE FROM att_grp WHERE AttCod=%ld",
		   AttCod);
  }

/*****************************************************************************/
/************* Remove one group from all the attendance events ***************/
/*****************************************************************************/

void Att_RemoveGroup (long GrpCod)
  {
   /***** Remove group from all the attendance events *****/
   DB_QueryDELETE ("can not remove group from the associations"
	           " between attendance events and groups",
		   "DELETE FROM att_grp WHERE GrpCod=%ld",
		   GrpCod);
  }

/*****************************************************************************/
/******** Remove groups of one type from all the attendance events ***********/
/*****************************************************************************/

void Att_RemoveGroupsOfType (long GrpTypCod)
  {
   /***** Remove group from all the attendance events *****/
   DB_QueryDELETE ("can not remove groups of a type from the associations"
		   " between attendance events and groups",
		   "DELETE FROM att_grp USING crs_grp,att_grp"
		   " WHERE crs_grp.GrpTypCod=%ld"
		   " AND crs_grp.GrpCod=att_grp.GrpCod",
                   GrpTypCod);
  }

/*****************************************************************************/
/***************** Create groups of an attendance event **********************/
/*****************************************************************************/

static void Att_CreateGrps (long AttCod)
  {
   unsigned NumGrpSel;

   /***** Create groups of the attendance event *****/
   for (NumGrpSel = 0;
	NumGrpSel < Gbl.Crs.Grps.LstGrpsSel.NumGrps;
	NumGrpSel++)
      /* Create group */
      DB_QueryINSERT ("can not associate a group to an attendance event",
		      "INSERT INTO att_grp"
		      " (AttCod,GrpCod)"
		      " VALUES"
		      " (%ld,%ld)",
                      AttCod,
		      Gbl.Crs.Grps.LstGrpsSel.GrpCods[NumGrpSel]);
  }

/*****************************************************************************/
/****** Get and write the names of the groups of an attendance event *********/
/*****************************************************************************/

static void Att_GetAndWriteNamesOfGrpsAssociatedToAttEvent (struct Att_Event *Event)
  {
   extern const char *Txt_Group;
   extern const char *Txt_Groups;
   extern const char *Txt_and;
   extern const char *Txt_The_whole_course;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumGrp;
   unsigned NumGrps;

   /***** Get groups associated to an attendance event from database *****/
   NumGrps = (unsigned) DB_QuerySELECT (&mysql_res,"can not get groups of an attendance event",
				        "SELECT crs_grp_types.GrpTypName,"
				               "crs_grp.GrpName,"
				               "rooms.ShortName"
					" FROM (att_grp,crs_grp,crs_grp_types)"
				        " LEFT JOIN rooms"
				        " ON crs_grp.RooCod=rooms.RooCod"
					" WHERE att_grp.AttCod=%ld"
					" AND att_grp.GrpCod=crs_grp.GrpCod"
					" AND crs_grp.GrpTypCod=crs_grp_types.GrpTypCod"
					" ORDER BY crs_grp_types.GrpTypName,crs_grp.GrpName",
					Event->AttCod);

   /***** Write heading *****/
   HTM_DIV_Begin ("class=\"%s\"",Event->Hidden ? "ASG_GRP_LIGHT" :
        	                               "ASG_GRP");
   HTM_TxtColonNBSP (NumGrps == 1 ? Txt_Group  :
                                    Txt_Groups);

   /***** Write groups *****/
   if (NumGrps) // Groups found...
     {
      /* Get and write the group types and names */
      for (NumGrp = 0;
	   NumGrp < NumGrps;
	   NumGrp++)
        {
         /* Get next group */
         row = mysql_fetch_row (mysql_res);

         /* Write group type name (row[0]) and group name (row[1]) */
         HTM_TxtF ("%s&nbsp;%s",row[0],row[1]);

         /* Write the name of the room (row[2]) */
	 if (row[2])	// May be NULL because of LEFT JOIN
	    if (row[2][0])
               HTM_TxtF ("&nbsp;(%s)",row[2]);

	 /* Write separator */
         if (NumGrps >= 2)
           {
            if (NumGrp == NumGrps - 2)
               HTM_TxtF (" %s ",Txt_and);
            if (NumGrps >= 3)
              if (NumGrp < NumGrps - 2)
                  HTM_Txt (", ");
           }
        }
     }
   else
      HTM_TxtF ("%s&nbsp;%s",Txt_The_whole_course,Gbl.Hierarchy.Crs.ShrtName);

   HTM_DIV_End ();

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/*********** Remove all users registered in an attendance event **************/
/*****************************************************************************/

static void Att_RemoveAllUsrsFromAnAttEvent (long AttCod)
  {
   DB_QueryDELETE ("can not remove attendance event",
		   "DELETE FROM att_usr WHERE AttCod=%ld",
		   AttCod);
  }

/*****************************************************************************/
/* Remove one user from all the attendance events where he/she is registered */
/*****************************************************************************/

void Att_RemoveUsrFromAllAttEvents (long UsrCod)
  {
   /***** Remove group from all the attendance events *****/
   DB_QueryDELETE ("can not remove user from all attendance events",
		   "DELETE FROM att_usr WHERE UsrCod=%ld",
		   UsrCod);
  }

/*****************************************************************************/
/*********** Remove one student from all the attendance events ***************/
/*****************************************************************************/

void Att_RemoveUsrFromCrsAttEvents (long UsrCod,long CrsCod)
  {
   /***** Remove group from all the attendance events *****/
   DB_QueryDELETE ("can not remove user from attendance events of a course",
		   "DELETE FROM att_usr USING att_events,att_usr"
		   " WHERE att_events.CrsCod=%ld"
		   " AND att_events.AttCod=att_usr.AttCod"
		   " AND att_usr.UsrCod=%ld",
                   CrsCod,UsrCod);
  }

/*****************************************************************************/
/*********************** Remove an attendance event **************************/
/*****************************************************************************/

static void Att_RemoveAttEventFromCurrentCrs (long AttCod)
  {
   DB_QueryDELETE ("can not remove attendance event",
		   "DELETE FROM att_events"
		   " WHERE AttCod=%ld AND CrsCod=%ld",
                   AttCod,Gbl.Hierarchy.Crs.CrsCod);
  }

/*****************************************************************************/
/*************** Remove all the attendance events of a course ****************/
/*****************************************************************************/

void Att_RemoveCrsAttEvents (long CrsCod)
  {
   /***** Remove students *****/
   DB_QueryDELETE ("can not remove all the students registered"
		   " in events of a course",
		   "DELETE FROM att_usr USING att_events,att_usr"
		   " WHERE att_events.CrsCod=%ld"
		   " AND att_events.AttCod=att_usr.AttCod",
                   CrsCod);

   /***** Remove groups *****/
   DB_QueryDELETE ("can not remove all the groups associated"
		   " to attendance events of a course",
		   "DELETE FROM att_grp USING att_events,att_grp"
		   " WHERE att_events.CrsCod=%ld"
		   " AND att_events.AttCod=att_grp.AttCod",
                   CrsCod);

   /***** Remove attendance events *****/
   DB_QueryDELETE ("can not remove all the attendance events of a course",
		   "DELETE FROM att_events WHERE CrsCod=%ld",
		   CrsCod);
  }

/*****************************************************************************/
/*************** Get number of attendance events in a course *****************/
/*****************************************************************************/

unsigned Att_GetNumAttEventsInCrs (long CrsCod)
  {
   /***** Get number of attendance events in a course from database *****/
   return
   (unsigned) DB_QueryCOUNT ("can not get number of attendance events"
			     " in course",
			     "SELECT COUNT(*) FROM att_events"
			     " WHERE CrsCod=%ld",
			     CrsCod);
  }

/*****************************************************************************/
/*************** Get number of courses with attendance events ****************/
/*****************************************************************************/
// Returns the number of courses with attendance events
// in this location (all the platform, current degree or current course)

unsigned Att_GetNumCoursesWithAttEvents (Hie_Level_t Scope)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumCourses;

   /***** Get number of courses with attendance events from database *****/
   switch (Scope)
     {
      case Hie_SYS:
         DB_QuerySELECT (&mysql_res,"can not get number of courses with attendance events",
			 "SELECT COUNT(DISTINCT CrsCod)"
			 " FROM att_events"
			 " WHERE CrsCod>0");
         break;
      case Hie_INS:
         DB_QuerySELECT (&mysql_res,"can not get number of courses with attendance events",
			 "SELECT COUNT(DISTINCT att_events.CrsCod)"
			 " FROM centres,degrees,courses,att_events"
			 " WHERE centres.InsCod=%ld"
			 " AND centres.CtrCod=degrees.CtrCod"
			 " AND degrees.DegCod=courses.DegCod"
			 " AND courses.Status=0"
			 " AND courses.CrsCod=att_events.CrsCod",
                         Gbl.Hierarchy.Ins.InsCod);
         break;
      case Hie_CTR:
         DB_QuerySELECT (&mysql_res,"can not get number of courses with attendance events",
			 "SELECT COUNT(DISTINCT att_events.CrsCod)"
			 " FROM degrees,courses,att_events"
			 " WHERE degrees.CtrCod=%ld"
			 " AND degrees.DegCod=courses.DegCod"
			 " AND courses.Status=0"
			 " AND courses.CrsCod=att_events.CrsCod",
                         Gbl.Hierarchy.Ctr.CtrCod);
         break;
      case Hie_DEG:
         DB_QuerySELECT (&mysql_res,"can not get number of courses with attendance events",
			 "SELECT COUNT(DISTINCT att_events.CrsCod)"
			 " FROM courses,att_events"
			 " WHERE courses.DegCod=%ld"
			 " AND courses.Status=0"
			 " AND courses.CrsCod=att_events.CrsCod",
                         Gbl.Hierarchy.Deg.DegCod);
         break;
      case Hie_CRS:
         DB_QuerySELECT (&mysql_res,"can not get number of courses with attendance events",
			 "SELECT COUNT(DISTINCT CrsCod)"
			 " FROM att_events"
			 " WHERE CrsCod=%ld",
                         Gbl.Hierarchy.Crs.CrsCod);
         break;
      default:
	 Lay_WrongScopeExit ();
	 break;
     }

   /***** Get number of courses *****/
   row = mysql_fetch_row (mysql_res);
   if (sscanf (row[0],"%u",&NumCourses) != 1)
      Lay_ShowErrorAndExit ("Error when getting number of courses with attendance events.");

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   return NumCourses;
  }

/*****************************************************************************/
/********************* Get number of attendance events ***********************/
/*****************************************************************************/
// Returns the number of attendance events
// in this location (all the platform, current degree or current course)

unsigned Att_GetNumAttEvents (Hie_Level_t Scope,unsigned *NumNotif)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumAttEvents;

   /***** Get number of attendance events from database *****/
   switch (Scope)
     {
      case Hie_SYS:
         DB_QuerySELECT (&mysql_res,"can not get number of attendance events",
			 "SELECT COUNT(*),SUM(NumNotif)"
			 " FROM att_events"
			 " WHERE CrsCod>0");
         break;
      case Hie_INS:
         DB_QuerySELECT (&mysql_res,"can not get number of attendance events",
			 "SELECT COUNT(*),SUM(att_events.NumNotif)"
			 " FROM centres,degrees,courses,att_events"
			 " WHERE centres.InsCod=%ld"
			 " AND centres.CtrCod=degrees.CtrCod"
			 " AND degrees.DegCod=courses.DegCod"
			 " AND courses.CrsCod=att_events.CrsCod",
                         Gbl.Hierarchy.Ins.InsCod);
         break;
      case Hie_CTR:
         DB_QuerySELECT (&mysql_res,"can not get number of attendance events",
			 "SELECT COUNT(*),SUM(att_events.NumNotif)"
			 " FROM degrees,courses,att_events"
			 " WHERE degrees.CtrCod=%ld"
			 " AND degrees.DegCod=courses.DegCod"
			 " AND courses.CrsCod=att_events.CrsCod",
                         Gbl.Hierarchy.Ctr.CtrCod);
         break;
      case Hie_DEG:
         DB_QuerySELECT (&mysql_res,"can not get number of attendance events",
			 "SELECT COUNT(*),SUM(att_events.NumNotif)"
			 " FROM courses,att_events"
			 " WHERE courses.DegCod=%ld"
			 " AND courses.CrsCod=att_events.CrsCod",
                         Gbl.Hierarchy.Deg.DegCod);
         break;
      case Hie_CRS:
         DB_QuerySELECT (&mysql_res,"can not get number of attendance events",
			 "SELECT COUNT(*),SUM(NumNotif)"
			 " FROM att_events"
			 " WHERE CrsCod=%ld",
                         Gbl.Hierarchy.Crs.CrsCod);
         break;
      default:
	 Lay_WrongScopeExit ();
	 break;
     }

   /***** Get number of attendance events *****/
   row = mysql_fetch_row (mysql_res);
   if (sscanf (row[0],"%u",&NumAttEvents) != 1)
      Lay_ShowErrorAndExit ("Error when getting number of attendance events.");

   /***** Get number of notifications by email *****/
   if (row[1])
     {
      if (sscanf (row[1],"%u",NumNotif) != 1)
         Lay_ShowErrorAndExit ("Error when getting number of notifications of attendance events.");
     }
   else
      *NumNotif = 0;

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   return NumAttEvents;
  }

/*****************************************************************************/
/************************ Show one attendance event **************************/
/*****************************************************************************/

void Att_SeeOneAttEvent (void)
  {
   struct Att_Events Events;

   /***** Reset attendance events *****/
   Att_ResetEvents (&Events);

   /***** Get attendance event code *****/
   if ((Events.AttCod = Att_GetParamAttCod ()) == -1L)
      Lay_ShowErrorAndExit ("Code of attendance event is missing.");

   /***** Show event *****/
   Att_ShowEvent (&Events);
  }

static void Att_ShowEvent (struct Att_Events *Events)
  {
   extern const char *Hlp_USERS_Attendance;
   extern const char *Txt_Event;
   struct Att_Event Event;

   /***** Get parameters *****/
   Events->SelectedOrder = Att_GetParamAttOrder ();
   Grp_GetParamWhichGroups ();
   Events->CurrentPage = Pag_GetParamPagNum (Pag_ATT_EVENTS);

   /***** Begin box and table *****/
   Box_BoxTableBegin (NULL,Txt_Event,
                      NULL,NULL,
                      Hlp_USERS_Attendance,Box_NOT_CLOSABLE,2);

   Event.AttCod = Events->AttCod;
   Att_ShowOneAttEvent (Events,&Event,true);

   /***** End table and box *****/
   Box_BoxTableEnd ();

   switch (Gbl.Usrs.Me.Role.Logged)
     {
      case Rol_STD:
	 Att_ListAttOnlyMeAsStudent (&Event);
	 break;
      case Rol_NET:
      case Rol_TCH:
      case Rol_SYS_ADM:
	 /***** Show list of students *****/
         Att_ListAttStudents (Events,&Event);
         break;
      default:
         break;
     }
  }

/*****************************************************************************/
/*********************** List me as student in one event *********************/
/*****************************************************************************/
// Event must be filled before calling this function

static void Att_ListAttOnlyMeAsStudent (struct Att_Event *Event)
  {
   extern const char *Hlp_USERS_Attendance;
   extern const char *Txt_Attendance;
   extern const char *Txt_Student_comment;
   extern const char *Txt_Teachers_comment;
   extern const char *Txt_ROLES_SINGUL_Abc[Rol_NUM_ROLES][Usr_NUM_SEXS];
   extern const char *Txt_Save_changes;

   /***** Get my setting about photos in users' list for current course *****/
   Usr_GetMyPrefAboutListWithPhotosFromDB ();

   /***** Begin form *****/
   if (Event->Open)
     {
      Frm_StartForm (ActRecAttMe);
      Att_PutParamAttCod (Event->AttCod);
     }

   /***** List students (only me) *****/
   /* Begin box */
   Box_BoxBegin (NULL,Txt_Attendance,
                 NULL,NULL,
                 Hlp_USERS_Attendance,Box_NOT_CLOSABLE);

   /* Begin table */
   HTM_TABLE_BeginWideMarginPadding (2);

   /* Header */
   HTM_TR_Begin (NULL);

   HTM_TH_Empty (3);
   if (Gbl.Usrs.Listing.WithPhotos)
      HTM_TH_Empty (1);
   HTM_TH (1,2,"TIT_TBL LM",Txt_ROLES_SINGUL_Abc[Rol_STD][Usr_SEX_UNKNOWN]);
   HTM_TH (1,1,"LM",Txt_Student_comment);
   HTM_TH (1,1,"LM",Txt_Teachers_comment);

   HTM_TR_End ();

   /* List of students (only me) */
   Att_WriteRowUsrToCallTheRoll (1,&Gbl.Usrs.Me.UsrDat,Event);

   /* End table */
   HTM_TABLE_End ();

   /* Send button */
   if (Event->Open)
     {
      Btn_PutConfirmButton (Txt_Save_changes);
      Frm_EndForm ();
     }

   /* End box */
   Box_BoxEnd ();
  }

/*****************************************************************************/
/*************** List students who attended to one event *********************/
/*****************************************************************************/
// Event must be filled before calling this function

static void Att_ListAttStudents (struct Att_Events *Events,
	                         struct Att_Event *Event)
  {
   extern const char *Hlp_USERS_Attendance;
   extern const char *Txt_Attendance;
   extern const char *Txt_Student_comment;
   extern const char *Txt_Teachers_comment;
   extern const char *Txt_ROLES_SINGUL_Abc[Rol_NUM_ROLES][Usr_NUM_SEXS];
   extern const char *Txt_Save_changes;
   unsigned NumUsr;
   struct UsrData UsrDat;

   /***** Get groups to show ******/
   Grp_GetParCodsSeveralGrpsToShowUsrs ();

   /***** Get and order list of students in this course *****/
   Usr_GetListUsrs (Hie_CRS,Rol_STD);

   /***** Begin box *****/
   Box_BoxBegin (NULL,Txt_Attendance,
                 NULL,NULL,
                 Hlp_USERS_Attendance,Box_NOT_CLOSABLE);

   /***** Form to select groups *****/
   Grp_ShowFormToSelectSeveralGroups (Att_PutParamSelectedAttCod,Events,
                                      Grp_MY_GROUPS);

   /***** Start section with user list *****/
   HTM_SECTION_Begin (Usr_USER_LIST_SECTION_ID);

   if (Gbl.Usrs.LstUsrs[Rol_STD].NumUsrs)
     {
      /***** Get my preference about photos in users' list for current course *****/
      Usr_GetMyPrefAboutListWithPhotosFromDB ();

      /***** Initialize structure with user's data *****/
      Usr_UsrDataConstructor (&UsrDat);

      /* Begin form */
      Frm_StartForm (ActRecAttStd);
      Att_PutParamAttCod (Event->AttCod);
      Grp_PutParamsCodGrps ();

      /* Begin table */
      HTM_TABLE_BeginWideMarginPadding (2);

      /* Header */
      HTM_TR_Begin (NULL);

      HTM_TH_Empty (3);
      if (Gbl.Usrs.Listing.WithPhotos)
         HTM_TH_Empty (1);
      HTM_TH (1,2,"LM",Txt_ROLES_SINGUL_Abc[Rol_STD][Usr_SEX_UNKNOWN]);
      HTM_TH (1,1,"LM",Txt_Student_comment);
      HTM_TH (1,1,"LM",Txt_Teachers_comment);

      HTM_TR_End ();

      /* List of students */
      for (NumUsr = 0, Gbl.RowEvenOdd = 0;
	   NumUsr < Gbl.Usrs.LstUsrs[Rol_STD].NumUsrs;
	   NumUsr++)
        {
	 /* Copy user's basic data from list */
         Usr_CopyBasicUsrDataFromList (&UsrDat,&Gbl.Usrs.LstUsrs[Rol_STD].Lst[NumUsr]);

	 /* Get list of user's IDs */
         ID_GetListIDsFromUsrCod (&UsrDat);

         Att_WriteRowUsrToCallTheRoll (NumUsr + 1,&UsrDat,Event);
        }

      /* End table */
      HTM_TABLE_End ();

      /* Send button */
      Btn_PutConfirmButton (Txt_Save_changes);

      /***** End form *****/
      Frm_EndForm ();

      /***** Free memory used for user's data *****/
      Usr_UsrDataDestructor (&UsrDat);
     }
   else	// Gbl.Usrs.LstUsrs[Rol_STD].NumUsrs == 0
      /***** Show warning indicating no students found *****/
      Usr_ShowWarningNoUsersFound (Rol_STD);

   /***** End section with user list *****/
   HTM_SECTION_End ();

   /***** End box *****/
   Box_BoxEnd ();

   /***** Free memory for students list *****/
   Usr_FreeUsrsList (Rol_STD);

   /***** Free memory for list of selected groups *****/
   Grp_FreeListCodSelectedGrps ();
  }

/*****************************************************************************/
/************** Write a row of a table with the data of a user ***************/
/*****************************************************************************/

static void Att_WriteRowUsrToCallTheRoll (unsigned NumUsr,
                                          struct UsrData *UsrDat,
                                          struct Att_Event *Event)
  {
   bool Present;
   char PhotoURL[PATH_MAX + 1];
   bool ShowPhoto;
   char CommentStd[Cns_MAX_BYTES_TEXT + 1];
   char CommentTch[Cns_MAX_BYTES_TEXT + 1];
   bool ItsMe;
   bool ICanChangeStdAttendance;
   bool ICanEditStdComment;
   bool ICanEditTchComment;

   /***** Set who can edit *****/
   switch (Gbl.Usrs.Me.Role.Logged)
     {
      case Rol_STD:
	 // A student can see only her/his attendance
	 ItsMe = Usr_ItsMe (UsrDat->UsrCod);
	 if (!ItsMe)
	    Lay_ShowErrorAndExit ("Wrong call.");
	 ICanChangeStdAttendance = false;
	 ICanEditStdComment = Event->Open;	// Attendance event is open
	 ICanEditTchComment = false;
	 break;
      case Rol_TCH:
	 ICanChangeStdAttendance = true;
	 ICanEditStdComment = false;
	 ICanEditTchComment = true;
	 break;
      case Rol_SYS_ADM:
	 ICanChangeStdAttendance = true;
	 ICanEditStdComment = false;
	 ICanEditTchComment = false;
	 break;
      default:
	 ICanChangeStdAttendance = false;
	 ICanEditStdComment = false;
	 ICanEditTchComment = false;
	 break;
     }

   /***** Check if this student is already present in the current event *****/
   Present = Att_CheckIfUsrIsPresentInAttEventAndGetComments (Event->AttCod,UsrDat->UsrCod,CommentStd,CommentTch);

   /***** Icon to show if the user is already present *****/
   HTM_TR_Begin (NULL);

   HTM_TD_Begin ("class=\"BT%u\"",Gbl.RowEvenOdd);
   HTM_LABEL_Begin ("for=\"Std%u\"",NumUsr);
   Att_PutCheckOrCross (Present);
   HTM_LABEL_End ();
   HTM_TD_End ();

   /***** Checkbox to select user *****/
   HTM_TD_Begin ("class=\"CT COLOR%u\"",Gbl.RowEvenOdd);
   HTM_INPUT_CHECKBOX ("UsrCodStd",HTM_DONT_SUBMIT_ON_CHANGE,
		       "id=\"Std%u\" value=\"%s\"%s%s",
	               NumUsr,UsrDat->EncryptedUsrCod,
		       Present ? " checked=\"checked\"" : "",
		       ICanChangeStdAttendance ? "" : " disabled=\"disabled\"");
   HTM_TD_End ();

   /***** Write number of student in the list *****/
   HTM_TD_Begin ("class=\"%s RT COLOR%u\"",
		 UsrDat->Accepted ? "DAT_N" :
				    "DAT",
		 Gbl.RowEvenOdd);
   HTM_Unsigned (NumUsr);
   HTM_TD_End ();

   /***** Show student's photo *****/
   if (Gbl.Usrs.Listing.WithPhotos)
     {
      HTM_TD_Begin ("class=\"LT COLOR%u\"",Gbl.RowEvenOdd);
      ShowPhoto = Pho_ShowingUsrPhotoIsAllowed (UsrDat,PhotoURL);
      Pho_ShowUsrPhoto (UsrDat,ShowPhoto ? PhotoURL :
                                           NULL,
                        "PHOTO45x60",Pho_ZOOM,false);
      HTM_TD_End ();
     }

   /***** Write user's ID ******/
   HTM_TD_Begin ("class=\"%s LT COLOR%u\"",
		 UsrDat->Accepted ? "DAT_SMALL_N" :
				    "DAT_SMALL",
		 Gbl.RowEvenOdd);
   ID_WriteUsrIDs (UsrDat,NULL);
   HTM_TD_End ();

   /***** Write student's name *****/
   HTM_TD_Begin ("class=\"%s LT COLOR%u\"",
		 UsrDat->Accepted ? "DAT_SMALL_N" :
				    "DAT_SMALL",
		 Gbl.RowEvenOdd);
   HTM_Txt (UsrDat->Surname1);
   if (UsrDat->Surname2[0])
      HTM_TxtF ("&nbsp;%s",UsrDat->Surname2);
   HTM_TxtF (", %s",UsrDat->FirstName);
   HTM_TD_End ();

   /***** Student's comment: write form or text */
   HTM_TD_Begin ("class=\"DAT_SMALL LT COLOR%u\"",Gbl.RowEvenOdd);
   if (ICanEditStdComment)	// Show with form
     {
      HTM_TEXTAREA_Begin ("name=\"CommentStd%s\" cols=\"40\" rows=\"3\"",
	                  UsrDat->EncryptedUsrCod);
      HTM_Txt (CommentStd);
      HTM_TEXTAREA_End ();
     }
   else				// Show without form
     {
      Str_ChangeFormat (Str_FROM_HTML,Str_TO_RIGOROUS_HTML,
                        CommentStd,Cns_MAX_BYTES_TEXT,false);
      HTM_Txt (CommentStd);
     }
   HTM_TD_End ();

   /***** Teacher's comment: write form, text or nothing */
   HTM_TD_Begin ("class=\"DAT_SMALL LT COLOR%u\"",Gbl.RowEvenOdd);
   if (ICanEditTchComment)		// Show with form
     {
      HTM_TEXTAREA_Begin ("name=\"CommentTch%s\" cols=\"40\" rows=\"3\"",
			  UsrDat->EncryptedUsrCod);
      HTM_Txt (CommentTch);
      HTM_TEXTAREA_End ();
     }
   else	if (Event->CommentTchVisible)	// Show without form
     {
      Str_ChangeFormat (Str_FROM_HTML,Str_TO_RIGOROUS_HTML,
                        CommentTch,Cns_MAX_BYTES_TEXT,false);
      HTM_Txt (CommentTch);
     }
   HTM_TD_End ();
   HTM_TR_End ();

   Gbl.RowEvenOdd = 1 - Gbl.RowEvenOdd;
  }

/*****************************************************************************/
/**************** Put link to view one attendance event **********************/
/*****************************************************************************/

static void Att_PutLinkAttEvent (struct Att_Event *AttEvent,
				 const char *Title,const char *Txt,
				 const char *Class)
  {
   Frm_StartForm (ActSeeOneAtt);
   Att_PutParamAttCod (AttEvent->AttCod);
   Att_PutParamsCodGrps (AttEvent->AttCod);
   HTM_BUTTON_SUBMIT_Begin (Title,Class,NULL);
   HTM_Txt (Txt);
   HTM_BUTTON_End ();
   Frm_EndForm ();
  }

/*****************************************************************************/
/****** Put parameters with the default groups in an attendance event ********/
/*****************************************************************************/

static void Att_PutParamsCodGrps (long AttCod)
  {
   extern const char *Par_SEPARATOR_PARAM_MULTIPLE;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumGrp;
   unsigned NumGrps;
   size_t MaxLengthGrpCods;
   char *GrpCods;

   /***** Get groups associated to an attendance event from database *****/
   if (Gbl.Crs.Grps.NumGrps)
      NumGrps = (unsigned) DB_QuerySELECT (&mysql_res,"can not get groups of an attendance event",
					   "SELECT GrpCod FROM att_grp"
					   " WHERE att_grp.AttCod=%ld",
					   AttCod);
   else
      NumGrps = 0;

   /***** Get groups *****/
   if (NumGrps) // Groups found...
     {
      MaxLengthGrpCods = NumGrps * (1 + 20) - 1;
      if ((GrpCods = (char *) malloc (MaxLengthGrpCods + 1)) == NULL)
	 Lay_NotEnoughMemoryExit ();
      GrpCods[0] = '\0';

      /* Get groups */
      for (NumGrp = 0;
	   NumGrp < NumGrps;
	   NumGrp++)
        {
         /* Get next group */
         row = mysql_fetch_row (mysql_res);

         /* Append group code to list */
         if (NumGrp)
            Str_Concat (GrpCods,Par_SEPARATOR_PARAM_MULTIPLE,MaxLengthGrpCods);
         Str_Concat (GrpCods,row[0],MaxLengthGrpCods);
        }

      Par_PutHiddenParamString (NULL,"GrpCods",GrpCods);
      free (GrpCods);
     }
   else
      /***** Write the boolean parameter that indicates if all the groups must be listed *****/
      Par_PutHiddenParamChar ("AllGroups",'Y');

   /***** Free structure that stores the query result *****/
   if (Gbl.Crs.Grps.NumGrps)
      DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/*************** Save me as students who attended to an event ****************/
/*****************************************************************************/

void Att_RegisterMeAsStdInAttEvent (void)
  {
   extern const char *Txt_Your_comment_has_been_updated;
   struct Att_Events Events;
   struct Att_Event Event;
   bool Present;
   char CommentStd[Cns_MAX_BYTES_TEXT + 1];
   char CommentTch[Cns_MAX_BYTES_TEXT + 1];

   /***** Reset attendance events *****/
   Att_ResetEvents (&Events);

   /***** Get attendance event code *****/
   if ((Event.AttCod = Att_GetParamAttCod ()) == -1L)
      Lay_ShowErrorAndExit ("Code of attendance event is missing.");
   Att_GetDataOfAttEventByCodAndCheckCrs (&Event);	// This checks that event belong to current course

   if (Event.Open)
     {
      /***** Get comments for this student *****/
      Present = Att_CheckIfUsrIsPresentInAttEventAndGetComments (Event.AttCod,Gbl.Usrs.Me.UsrDat.UsrCod,
	                                                         CommentStd,CommentTch);
      Par_GetParToHTML (Str_BuildStringStr ("CommentStd%s",
					    Gbl.Usrs.Me.UsrDat.EncryptedUsrCod),
			CommentStd,Cns_MAX_BYTES_TEXT);
      Str_FreeString ();

      if (Present ||
	  CommentStd[0] ||
	  CommentTch[0])
	 /***** Register student *****/
	 Att_RegUsrInAttEventChangingComments (Event.AttCod,Gbl.Usrs.Me.UsrDat.UsrCod,
	                                       Present,CommentStd,CommentTch);
      else
	 /***** Remove student *****/
	 Att_RemoveUsrFromAttEvent (Event.AttCod,Gbl.Usrs.Me.UsrDat.UsrCod);

      /***** Write final message *****/
      Ale_ShowAlert (Ale_SUCCESS,Txt_Your_comment_has_been_updated);
     }

   /***** Show the attendance event again *****/
   Events.AttCod = Event.AttCod;
   Att_ShowEvent (&Events);
  }

/*****************************************************************************/
/***************** Save students who attended to an event ********************/
/*****************************************************************************/
/* Algorithm:
   1. Get list of students in the groups selected: Gbl.Usrs.LstUsrs[Rol_STD]
   2. Mark all students in the groups selected setting Remove=true
   3. Get list of students marked as present by me: Gbl.Usrs.Selected.List[Rol_STD]
   4. Loop over the list Gbl.Usrs.Selected.List[Rol_STD],
      that holds the list of the students marked as present,
      marking the students in Gbl.Usrs.LstUsrs[Rol_STD].Lst as Remove=false
   5. Delete from att_usr all the students marked as Remove=true
   6. Replace (insert without duplicated) into att_usr all the students marked as Remove=false
 */
void Att_RegisterStudentsInAttEvent (void)
  {
   extern const char *Txt_Presents;
   extern const char *Txt_Absents;
   struct Att_Events Events;
   struct Att_Event Event;
   unsigned NumUsr;
   const char *Ptr;
   bool Present;
   unsigned NumStdsPresent;
   unsigned NumStdsAbsent;
   struct UsrData UsrData;
   char CommentStd[Cns_MAX_BYTES_TEXT + 1];
   char CommentTch[Cns_MAX_BYTES_TEXT + 1];

   /***** Reset attendance events *****/
   Att_ResetEvents (&Events);

   /***** Get attendance event code *****/
   if ((Event.AttCod = Att_GetParamAttCod ()) == -1L)
      Lay_ShowErrorAndExit ("Code of attendance event is missing.");
   Att_GetDataOfAttEventByCodAndCheckCrs (&Event);	// This checks that event belong to current course

   /***** Get groups selected *****/
   Grp_GetParCodsSeveralGrpsToShowUsrs ();

   /***** 1. Get list of students in the groups selected: Gbl.Usrs.LstUsrs[Rol_STD] *****/
   /* Get list of students in the groups selected */
   Usr_GetListUsrs (Hie_CRS,Rol_STD);

   if (Gbl.Usrs.LstUsrs[Rol_STD].NumUsrs)	// If there are students in the groups selected...
     {
      /***** 2. Mark all students in the groups selected setting Remove=true *****/
      for (NumUsr = 0;
           NumUsr < Gbl.Usrs.LstUsrs[Rol_STD].NumUsrs;
           NumUsr++)
         Gbl.Usrs.LstUsrs[Rol_STD].Lst[NumUsr].Remove = true;

      /***** 3. Get list of students marked as present by me: Gbl.Usrs.Selected.List[Rol_STD] *****/
      Usr_GetListsSelectedEncryptedUsrsCods (&Gbl.Usrs.Selected);

      /***** Initialize structure with user's data *****/
      Usr_UsrDataConstructor (&UsrData);

      /***** 4. Loop over the list Gbl.Usrs.Selected.List[Rol_STD],
                that holds the list of the students marked as present,
                marking the students in Gbl.Usrs.LstUsrs[Rol_STD].Lst as Remove=false *****/
      Ptr = Gbl.Usrs.Selected.List[Rol_STD];
      while (*Ptr)
	{
	 Par_GetNextStrUntilSeparParamMult (&Ptr,UsrData.EncryptedUsrCod,
	                                    Cry_BYTES_ENCRYPTED_STR_SHA256_BASE64);
	 Usr_GetUsrCodFromEncryptedUsrCod (&UsrData);
	 if (UsrData.UsrCod > 0)	// Student exists in database
	    /***** Mark student to not be removed *****/
	    for (NumUsr = 0;
		 NumUsr < Gbl.Usrs.LstUsrs[Rol_STD].NumUsrs;
		 NumUsr++)
	       if (Gbl.Usrs.LstUsrs[Rol_STD].Lst[NumUsr].UsrCod == UsrData.UsrCod)
		 {
		  Gbl.Usrs.LstUsrs[Rol_STD].Lst[NumUsr].Remove = false;
	          break;	// Found! Exit loop
	         }
	}

      /***** Free memory used for user's data *****/
      Usr_UsrDataDestructor (&UsrData);

      /***** Free memory *****/
      /* Free memory used by list of selected students' codes */
      Usr_FreeListsSelectedEncryptedUsrsCods (&Gbl.Usrs.Selected);

      // 5. Delete from att_usr all the students marked as Remove=true
      // 6. Replace (insert without duplicated) into att_usr all the students marked as Remove=false
      for (NumUsr = 0, NumStdsAbsent = NumStdsPresent = 0;
	   NumUsr < Gbl.Usrs.LstUsrs[Rol_STD].NumUsrs;
	   NumUsr++)
	{
	 /***** Get comments for this student *****/
	 Att_CheckIfUsrIsPresentInAttEventAndGetComments (Event.AttCod,Gbl.Usrs.LstUsrs[Rol_STD].Lst[NumUsr].UsrCod,CommentStd,CommentTch);
	 Par_GetParToHTML (Str_BuildStringStr ("CommentTch%s",
					       Gbl.Usrs.LstUsrs[Rol_STD].Lst[NumUsr].EncryptedUsrCod),
			   CommentTch,Cns_MAX_BYTES_TEXT);
	 Str_FreeString ();

	 Present = !Gbl.Usrs.LstUsrs[Rol_STD].Lst[NumUsr].Remove;

	 if (Present ||
	     CommentStd[0] ||
	     CommentTch[0])
	    /***** Register student *****/
	    Att_RegUsrInAttEventChangingComments (Event.AttCod,Gbl.Usrs.LstUsrs[Rol_STD].Lst[NumUsr].UsrCod,
					          Present,CommentStd,CommentTch);
	 else
	    /***** Remove student *****/
	    Att_RemoveUsrFromAttEvent (Event.AttCod,Gbl.Usrs.LstUsrs[Rol_STD].Lst[NumUsr].UsrCod);

	 if (Present)
            NumStdsPresent++;
      	 else
	    NumStdsAbsent++;
	}

      /***** Free memory for students list *****/
      Usr_FreeUsrsList (Rol_STD);

      /***** Write final message *****/
      Ale_ShowAlert (Ale_INFO,"%s: %u<br />"
		              "%s: %u",
		     Txt_Presents,NumStdsPresent,
		     Txt_Absents ,NumStdsAbsent );
     }
   else	// Gbl.Usrs.LstUsrs[Rol_STD].NumUsrs == 0
      /***** Show warning indicating no students found *****/
      Usr_ShowWarningNoUsersFound (Rol_STD);

   /***** Show the attendance event again *****/
   Events.AttCod = Event.AttCod;
   Att_ShowEvent (&Events);

   /***** Free memory for list of groups selected *****/
   Grp_FreeListCodSelectedGrps ();
  }

/*****************************************************************************/
/******* Get number of students from a list who attended to an event *********/
/*****************************************************************************/

static void Att_GetNumStdsTotalWhoAreInAttEvent (struct Att_Event *Event)
  {
   /***** Count number of students registered in an event in database *****/
   Event->NumStdsTotal =
   (unsigned) DB_QueryCOUNT ("can not get number of students"
			     " who are registered in an event",
			     "SELECT COUNT(*) FROM att_usr"
			     " WHERE AttCod=%ld AND Present='Y'",
			     Event->AttCod);
  }

/*****************************************************************************/
/********* Get number of users from a list who attended to an event **********/
/*****************************************************************************/

static unsigned Att_GetNumUsrsFromAListWhoAreInAttEvent (long AttCod,
							 long LstSelectedUsrCods[],
							 unsigned NumUsrsInList)
  {
   char *SubQueryUsrs;
   unsigned NumUsrsInAttEvent;

   if (NumUsrsInList)
     {
      /***** Create subquery string *****/
      Usr_CreateSubqueryUsrCods (LstSelectedUsrCods,NumUsrsInList,
				 &SubQueryUsrs);

      /***** Get number of users in attendance event from database ****/
      NumUsrsInAttEvent =
      (unsigned) DB_QueryCOUNT ("can not get number of students"
			        " from a list who are registered in an event",
				"SELECT COUNT(*) FROM att_usr"
				" WHERE AttCod=%ld"
				" AND UsrCod IN (%s) AND Present='Y'",
				AttCod,SubQueryUsrs);

      /***** Free memory for subquery string *****/
      Usr_FreeSubqueryUsrCods (SubQueryUsrs);
     }
   else
      NumUsrsInAttEvent = 0;

   return NumUsrsInAttEvent;
  }

/*****************************************************************************/
/***************** Check if a student attended to an event *******************/
/*****************************************************************************/

static bool Att_CheckIfUsrIsInTableAttUsr (long AttCod,long UsrCod,bool *Present)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows;
   bool InDBTable;

   /***** Check if a student is registered in an event in database *****/
   NumRows = DB_QuerySELECT (&mysql_res,"can not get if a student"
					" is already registered"
					" in an event",
			     "SELECT Present FROM att_usr"
			     " WHERE AttCod=%ld AND UsrCod=%ld",
			     AttCod,UsrCod);
   if (NumRows)
     {
      InDBTable = true;

      /* Get row */
      row = mysql_fetch_row (mysql_res);

      /* Get if present (row[0]) */
      *Present = (row[0][0] == 'Y');
     }
   else	// User is not present
     {
      InDBTable = false;
      *Present = false;
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   return InDBTable;
  }

/*****************************************************************************/
/***************** Check if a student attended to an event *******************/
/*****************************************************************************/

static bool Att_CheckIfUsrIsPresentInAttEvent (long AttCod,long UsrCod)
  {
   bool Present;

   Att_CheckIfUsrIsInTableAttUsr (AttCod,UsrCod,&Present);

   return Present;
  }

/*****************************************************************************/
/***************** Check if a student attended to an event *******************/
/*****************************************************************************/

static bool Att_CheckIfUsrIsPresentInAttEventAndGetComments (long AttCod,long UsrCod,
                                                             char CommentStd[Cns_MAX_BYTES_TEXT + 1],
                                                             char CommentTch[Cns_MAX_BYTES_TEXT + 1])
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows;
   bool Present;

   /***** Check if a students is registered in an event in database *****/
   NumRows = DB_QuerySELECT (&mysql_res,"can not get if a student"
				        " is already registered"
				        " in an event",
			     "SELECT Present,CommentStd,CommentTch"
			     " FROM att_usr"
			     " WHERE AttCod=%ld AND UsrCod=%ld",
			     AttCod,UsrCod);
   if (NumRows)
     {
      /* Get row */
      row = mysql_fetch_row (mysql_res);

      /* Get if present (row[0]) */
      Present = (row[0][0] == 'Y');

      /* Get student's comment (row[1]) */
      Str_Copy (CommentStd,row[1],
                Cns_MAX_BYTES_TEXT);

      /* Get teacher's comment (row[2]) */
      Str_Copy (CommentTch,row[2],
                Cns_MAX_BYTES_TEXT);
     }
   else	// User is not present
     {
      Present = false;
      CommentStd[0] =
      CommentTch[0] = '\0';
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   return Present;
  }

/*****************************************************************************/
/******* Register a user in an attendance event not changing comments ********/
/*****************************************************************************/

void Att_RegUsrInAttEventNotChangingComments (long AttCod,long UsrCod)
  {
   bool Present;

   /***** Check if user is already in table att_usr (present or not) *****/
   if (Att_CheckIfUsrIsInTableAttUsr (AttCod,UsrCod,&Present))	// User is in table att_usr
     {
      // If already present ==> nothing to do
      if (!Present)
	 /***** Set user as present in database *****/
	 DB_QueryUPDATE ("can not set user as present in an event",
			 "UPDATE att_usr SET Present='Y'"
			 " WHERE AttCod=%ld AND UsrCod=%ld",
		         AttCod,UsrCod);
     }
   else			// User is not in table att_usr
      Att_RegUsrInAttEventChangingComments (AttCod,UsrCod,true,"","");
  }

/*****************************************************************************/
/********* Register a user in an attendance event changing comments **********/
/*****************************************************************************/

static void Att_RegUsrInAttEventChangingComments (long AttCod,long UsrCod,bool Present,
                                                  const char *CommentStd,const char *CommentTch)
  {
   /***** Register user as assistant to an event in database *****/
   DB_QueryREPLACE ("can not register user in an event",
		    "REPLACE INTO att_usr"
		    " (AttCod,UsrCod,Present,CommentStd,CommentTch)"
		    " VALUES"
		    " (%ld,%ld,'%c','%s','%s')",
                    AttCod,UsrCod,
                    Present ? 'Y' :
        	              'N',
                    CommentStd,
                    CommentTch);
  }

/*****************************************************************************/
/********************** Remove a user from an event **************************/
/*****************************************************************************/

static void Att_RemoveUsrFromAttEvent (long AttCod,long UsrCod)
  {
   /***** Remove user if there is no comment in database *****/
   DB_QueryDELETE ("can not remove student from an event",
		   "DELETE FROM att_usr WHERE AttCod=%ld AND UsrCod=%ld",
                   AttCod,UsrCod);
  }

/*****************************************************************************/
/************ Remove users absent without comments from an event *************/
/*****************************************************************************/

void Att_RemoveUsrsAbsentWithoutCommentsFromAttEvent (long AttCod)
  {
   /***** Clean table att_usr *****/
   DB_QueryDELETE ("can not remove users absent"
	           " without comments from an event",
		   "DELETE FROM att_usr"
		   " WHERE AttCod=%ld AND Present='N'"
		   " AND CommentStd='' AND CommentTch=''",
	           AttCod);
  }

/*****************************************************************************/
/********** Request listing attendance of users to several events ************/
/*****************************************************************************/

void Att_ReqListUsrsAttendanceCrs (void)
  {
   Att_TypeOfView_t TypeOfView = Att_VIEW_SEL_USR;

   Att_ReqListOrPrintUsrsAttendanceCrs (&TypeOfView);
  }

static void Att_ReqListOrPrintUsrsAttendanceCrs (void *TypeOfView)
  {
   extern const char *Hlp_USERS_Attendance_attendance_list;
   extern const char *Txt_Attendance_list;
   extern const char *Txt_View_attendance;
   struct Att_Events Events;

   switch (*((Att_TypeOfView_t *) TypeOfView))
     {
      case Att_VIEW_SEL_USR:
      case Att_PRNT_SEL_USR:
	 /***** Reset attendance events *****/
	 Att_ResetEvents (&Events);

	 /***** Get list of attendance events *****/
	 Att_GetListAttEvents (&Events,Att_OLDEST_FIRST);

	 /***** List users to select some of them *****/
	 Usr_PutFormToSelectUsrsToGoToAct (&Gbl.Usrs.Selected,
					   ActSeeLstUsrAtt,
					   NULL,NULL,
					   Txt_Attendance_list,
					   Hlp_USERS_Attendance_attendance_list,
					   Txt_View_attendance,
					   false);	// Do not put form with date range

	 /***** Free list of attendance events *****/
	 Att_FreeListAttEvents (&Events);
	 break;
      default:
	 Lay_WrongTypeOfViewExit ();
	 break;
     }
  }

/*****************************************************************************/
/********** List my attendance (I am a student) to several events ************/
/*****************************************************************************/

void Att_ListMyAttendanceCrs (void)
  {
   Att_ListOrPrintMyAttendanceCrs (Att_VIEW_ONLY_ME);
  }

void Att_PrintMyAttendanceCrs (void)
  {
   Att_ListOrPrintMyAttendanceCrs (Att_PRNT_ONLY_ME);
  }

static void Att_ListOrPrintMyAttendanceCrs (Att_TypeOfView_t TypeOfView)
  {
   extern const char *Hlp_USERS_Attendance_attendance_list;
   extern const char *Txt_Attendance;
   struct Att_Events Events;
   unsigned NumAttEvent;

   switch (TypeOfView)
     {
      case Att_VIEW_ONLY_ME:
      case Att_PRNT_ONLY_ME:
	 /***** Reset attendance events *****/
	 Att_ResetEvents (&Events);

	 /***** Get list of attendance events *****/
	 Att_GetListAttEvents (&Events,Att_OLDEST_FIRST);

	 /***** Get boolean parameter that indicates if details must be shown *****/
	 Events.ShowDetails = Par_GetParToBool ("ShowDetails");

	 /***** Get list of groups selected ******/
	 Grp_GetParCodsSeveralGrpsToShowUsrs ();

	 /***** Get number of students in each event *****/
	 for (NumAttEvent = 0;
	      NumAttEvent < Events.Num;
	      NumAttEvent++)
	    /* Get number of students in this event */
	    Events.Lst[NumAttEvent].NumStdsFromList =
	    Att_GetNumUsrsFromAListWhoAreInAttEvent (Events.Lst[NumAttEvent].AttCod,
						     &Gbl.Usrs.Me.UsrDat.UsrCod,1);

	 /***** Get list of attendance events selected *****/
	 Att_GetListSelectedAttCods (&Events);

	 /***** Begin box *****/
	 switch (TypeOfView)
	   {
	    case Att_VIEW_ONLY_ME:
	       Box_BoxBegin (NULL,Txt_Attendance,
			     Att_PutIconsMyAttList,&Events,
			     Hlp_USERS_Attendance_attendance_list,Box_NOT_CLOSABLE);
	       break;
	    case Att_PRNT_ONLY_ME:
	       Box_BoxBegin (NULL,Txt_Attendance,
			     NULL,NULL,
			     NULL,Box_NOT_CLOSABLE);
	       break;
	    default:
	       Lay_WrongTypeOfViewExit ();
	       break;
	   }

	 /***** List events to select *****/
	 Att_ListEventsToSelect (&Events,TypeOfView);

	 /***** Get my preference about photos in users' list for current course *****/
	 Usr_GetMyPrefAboutListWithPhotosFromDB ();

	 /***** Show table with attendances for every student in list *****/
	 Att_ListUsrsAttendanceTable (&Events,TypeOfView,1,&Gbl.Usrs.Me.UsrDat.UsrCod);

	 /***** Show details or put button to show details *****/
	 if (Events.ShowDetails)
	    Att_ListStdsWithAttEventsDetails (&Events,1,&Gbl.Usrs.Me.UsrDat.UsrCod);

	 /***** End box *****/
	 Box_BoxEnd ();

	 /***** Free memory for list of attendance events selected *****/
	 free (Events.StrAttCodsSelected);

	 /***** Free list of groups selected *****/
	 Grp_FreeListCodSelectedGrps ();

	 /***** Free list of attendance events *****/
	 Att_FreeListAttEvents (&Events);
	 break;
      default:
	 Lay_WrongTypeOfViewExit ();
	 break;
     }
  }

/*****************************************************************************/
/*************** List attendance of users to several events ******************/
/*****************************************************************************/

void Att_ListUsrsAttendanceCrs (void)
  {
   Att_GetUsrsAndListOrPrintAttendanceCrs (Att_VIEW_SEL_USR);
  }

void Att_PrintUsrsAttendanceCrs (void)
  {
   Att_GetUsrsAndListOrPrintAttendanceCrs (Att_PRNT_SEL_USR);
  }

static void Att_GetUsrsAndListOrPrintAttendanceCrs (Att_TypeOfView_t TypeOfView)
  {
   Usr_GetSelectedUsrsAndGoToAct (&Gbl.Usrs.Selected,
				  Att_ListOrPrintUsrsAttendanceCrs,&TypeOfView,
                                  Att_ReqListOrPrintUsrsAttendanceCrs,&TypeOfView);
  }

static void Att_ListOrPrintUsrsAttendanceCrs (void *TypeOfView)
  {
   extern const char *Hlp_USERS_Attendance_attendance_list;
   extern const char *Txt_Attendance_list;
   struct Att_Events Events;
   unsigned NumUsrsInList;
   long *LstSelectedUsrCods;
   unsigned NumAttEvent;

   switch (*((Att_TypeOfView_t *) TypeOfView))
     {
      case Att_VIEW_SEL_USR:
      case Att_PRNT_SEL_USR:
	 /***** Reset attendance events *****/
	 Att_ResetEvents (&Events);

	 /***** Get parameters *****/
	 /* Get boolean parameter that indicates if details must be shown */
	 Events.ShowDetails = Par_GetParToBool ("ShowDetails");

	 /* Get list of groups selected */
	 Grp_GetParCodsSeveralGrpsToShowUsrs ();

	 /***** Count number of valid users in list of encrypted user codes *****/
	 NumUsrsInList = Usr_CountNumUsrsInListOfSelectedEncryptedUsrCods (&Gbl.Usrs.Selected);

	 if (NumUsrsInList)
	   {
	    /***** Get list of students selected to show their attendances *****/
	    Usr_GetListSelectedUsrCods (&Gbl.Usrs.Selected,NumUsrsInList,&LstSelectedUsrCods);

	    /***** Get list of attendance events *****/
	    Att_GetListAttEvents (&Events,Att_OLDEST_FIRST);

	    /***** Get number of students in each event *****/
	    for (NumAttEvent = 0;
		 NumAttEvent < Events.Num;
		 NumAttEvent++)
	       /* Get number of students in this event */
	       Events.Lst[NumAttEvent].NumStdsFromList =
	       Att_GetNumUsrsFromAListWhoAreInAttEvent (Events.Lst[NumAttEvent].AttCod,
							LstSelectedUsrCods,NumUsrsInList);

	    /***** Get list of attendance events selected *****/
	    Att_GetListSelectedAttCods (&Events);

	    /***** Begin box *****/
	    switch (*((Att_TypeOfView_t *) TypeOfView))
	      {
	       case Att_VIEW_SEL_USR:
		  Box_BoxBegin (NULL,Txt_Attendance_list,
				Att_PutIconsStdsAttList,&Events,
				Hlp_USERS_Attendance_attendance_list,Box_NOT_CLOSABLE);
		  break;
	       case Att_PRNT_SEL_USR:
		  Box_BoxBegin (NULL,Txt_Attendance_list,
				NULL,NULL,
				NULL,Box_NOT_CLOSABLE);
		  break;
	       default:
		  Lay_WrongTypeOfViewExit ();
	      }

	    /***** List events to select *****/
	    Att_ListEventsToSelect (&Events,*((Att_TypeOfView_t *) TypeOfView));

	    /***** Get my preference about photos in users' list for current course *****/
	    Usr_GetMyPrefAboutListWithPhotosFromDB ();

	    /***** Show table with attendances for every student in list *****/
	    Att_ListUsrsAttendanceTable (&Events,*((Att_TypeOfView_t *) TypeOfView),
	                                 NumUsrsInList,LstSelectedUsrCods);

	    /***** Show details or put button to show details *****/
	    if (Events.ShowDetails)
	       Att_ListStdsWithAttEventsDetails (&Events,NumUsrsInList,LstSelectedUsrCods);

	    /***** End box *****/
	    Box_BoxEnd ();

	    /***** Free memory for list of attendance events selected *****/
	    free (Events.StrAttCodsSelected);

	    /***** Free list of attendance events *****/
	    Att_FreeListAttEvents (&Events);

	    /***** Free list of user codes *****/
	    Usr_FreeListSelectedUsrCods (LstSelectedUsrCods);
	   }

	 /***** Free list of groups selected *****/
	 Grp_FreeListCodSelectedGrps ();
	 break;
      default:
	 Lay_WrongTypeOfViewExit ();
	 break;
     }
  }

/*****************************************************************************/
/****************** Get list of attendance events selected *******************/
/*****************************************************************************/

static void Att_GetListSelectedAttCods (struct Att_Events *Events)
  {
   size_t MaxSizeListAttCodsSelected;
   unsigned NumAttEvent;
   const char *Ptr;
   long AttCod;
   char LongStr[Cns_MAX_DECIMAL_DIGITS_LONG + 1];
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumGrpsInThisEvent;
   unsigned NumGrpInThisEvent;
   long GrpCodInThisEvent;
   unsigned NumGrpSel;

   /***** Allocate memory for list of attendance events selected *****/
   MaxSizeListAttCodsSelected = (size_t) Events->Num * (Cns_MAX_DECIMAL_DIGITS_LONG + 1);
   if ((Events->StrAttCodsSelected = (char *) malloc (MaxSizeListAttCodsSelected + 1)) == NULL)
      Lay_NotEnoughMemoryExit ();

   /***** Get parameter multiple with list of attendance events selected *****/
   Par_GetParMultiToText ("AttCods",Events->StrAttCodsSelected,MaxSizeListAttCodsSelected);

   /***** Set which attendance events will be shown as selected (checkboxes on) *****/
   if (Events->StrAttCodsSelected[0])	// There are events selected
     {
      /* Reset selection */
      for (NumAttEvent = 0;
	   NumAttEvent < Events->Num;
	   NumAttEvent++)
	 Events->Lst[NumAttEvent].Selected = false;

      /* Set some events as selected */
      for (Ptr = Events->StrAttCodsSelected;
	   *Ptr;
	  )
	{
	 /* Get next attendance event selected */
	 Par_GetNextStrUntilSeparParamMult (&Ptr,LongStr,Cns_MAX_DECIMAL_DIGITS_LONG);
	 AttCod = Str_ConvertStrCodToLongCod (LongStr);

	 /* Set each event in *StrAttCodsSelected as selected */
	 for (NumAttEvent = 0;
	      NumAttEvent < Events->Num;
	      NumAttEvent++)
	    if (Events->Lst[NumAttEvent].AttCod == AttCod)
	      {
	       Events->Lst[NumAttEvent].Selected = true;
	       break;
	      }
	}
     }
   else				// No events selected
     {
      /***** Set which events will be marked as selected by default *****/
      if (!Gbl.Crs.Grps.NumGrps ||	// Course has no groups
          Gbl.Usrs.ClassPhoto.AllGroups)	// All groups selected
	 /* Set all events as selected */
	 for (NumAttEvent = 0;
	      NumAttEvent < Events->Num;
	      NumAttEvent++)
	    Events->Lst[NumAttEvent].Selected = true;
      else					// Course has groups and not all of them are selected
	 for (NumAttEvent = 0;
	      NumAttEvent < Events->Num;
	      NumAttEvent++)
	   {
	    /* Reset selection */
	    Events->Lst[NumAttEvent].Selected = false;

	    /* Set this event as selected? */
	    if (Events->Lst[NumAttEvent].NumStdsFromList)	// Some students attended to this event
	       Events->Lst[NumAttEvent].Selected = true;
	    else						// No students attended to this event
	      {
	       /***** Get groups associated to an attendance event from database *****/
	       NumGrpsInThisEvent = (unsigned) DB_QuerySELECT (&mysql_res,"can not get groups"
									  " of an attendance event",
							       "SELECT GrpCod FROM att_grp"
							       " WHERE att_grp.AttCod=%ld",
							       Events->Lst[NumAttEvent].AttCod);
	       if (NumGrpsInThisEvent)	// This event is associated to groups
		  /* Get groups associated to this event */
		  for (NumGrpInThisEvent = 0;
		       NumGrpInThisEvent < NumGrpsInThisEvent &&
		       !Events->Lst[NumAttEvent].Selected;
		       NumGrpInThisEvent++)
		    {
		     /* Get next group associated to this event */
		     row = mysql_fetch_row (mysql_res);
		     if ((GrpCodInThisEvent = Str_ConvertStrCodToLongCod (row[0])) > 0)
			/* Check if this group is selected */
			for (NumGrpSel = 0;
			     NumGrpSel < Gbl.Crs.Grps.LstGrpsSel.NumGrps &&
			     !Events->Lst[NumAttEvent].Selected;
			     NumGrpSel++)
			   if (Gbl.Crs.Grps.LstGrpsSel.GrpCods[NumGrpSel] == GrpCodInThisEvent)
			      Events->Lst[NumAttEvent].Selected = true;
		    }
	       else			// This event is not associated to groups
		  Events->Lst[NumAttEvent].Selected = true;

	       /***** Free structure that stores the query result *****/
	       DB_FreeMySQLResult (&mysql_res);
	      }
	   }
     }
  }

/*****************************************************************************/
/******* Put contextual icons when listing my assistance (as student) ********/
/*****************************************************************************/

static void Att_PutIconsMyAttList (void *Events)
  {
   if (Events)
     {
      /***** Put icon to print my assistance (as student) to several events *****/
      Ico_PutContextualIconToPrint (ActPrnLstMyAtt,
				    Att_PutFormToPrintMyListParams,Events);

      /***** Put icon to print my QR code *****/
      QR_PutLinkToPrintQRCode (ActPrnUsrQR,
			       Usr_PutParamMyUsrCodEncrypted,Gbl.Usrs.Me.UsrDat.EncryptedUsrCod);
     }
  }

static void Att_PutFormToPrintMyListParams (void *Events)
  {
   if (Events)
     {
      if (((struct Att_Events *) Events)->ShowDetails)
	 Par_PutHiddenParamChar ("ShowDetails",'Y');
      if (((struct Att_Events *) Events)->StrAttCodsSelected)
	 if (((struct Att_Events *) Events)->StrAttCodsSelected[0])
	    Par_PutHiddenParamString (NULL,"AttCods",((struct Att_Events *) Events)->StrAttCodsSelected);
     }
  }

/*****************************************************************************/
/******** Put icon to print assistance of students to several events *********/
/*****************************************************************************/

static void Att_PutIconsStdsAttList (void *Events)
  {
   if (Events)
     {
      /***** Put icon to print assistance of students to several events *****/
      Ico_PutContextualIconToPrint (ActPrnLstUsrAtt,
				    Att_PutParamsToPrintStdsList,Events);

      /***** Put icon to print my QR code *****/
      QR_PutLinkToPrintQRCode (ActPrnUsrQR,
			       Usr_PutParamMyUsrCodEncrypted,Gbl.Usrs.Me.UsrDat.EncryptedUsrCod);
     }
  }

static void Att_PutParamsToPrintStdsList (void *Events)
  {
   if (Events)
     {
      if (((struct Att_Events *) Events)->ShowDetails)
	 Par_PutHiddenParamChar ("ShowDetails",'Y');
      Grp_PutParamsCodGrps ();
      Usr_PutHiddenParSelectedUsrsCods (&Gbl.Usrs.Selected);
      if (((struct Att_Events *) Events)->StrAttCodsSelected)
	 if (((struct Att_Events *) Events)->StrAttCodsSelected[0])
	    Par_PutHiddenParamString (NULL,"AttCods",((struct Att_Events *) Events)->StrAttCodsSelected);
     }
  }

/*****************************************************************************/
/**** Put a link (form) to list assistance of students to several events *****/
/*****************************************************************************/

static void Att_PutButtonToShowDetails (const struct Att_Events *Events)
  {
   extern const char *Txt_Show_more_details;

   /***** Button to show more details *****/
   Frm_StartFormAnchor (Gbl.Action.Act,Att_ATTENDANCE_DETAILS_ID);
   Par_PutHiddenParamChar ("ShowDetails",'Y');
   Grp_PutParamsCodGrps ();
   Usr_PutHiddenParSelectedUsrsCods (&Gbl.Usrs.Selected);
   if (Events->StrAttCodsSelected)
      if (Events->StrAttCodsSelected[0])
	 Par_PutHiddenParamString (NULL,"AttCods",Events->StrAttCodsSelected);
   Btn_PutConfirmButton (Txt_Show_more_details);
   Frm_EndForm ();
  }

/*****************************************************************************/
/********** Write list of those attendance events that have students *********/
/*****************************************************************************/

static void Att_ListEventsToSelect (const struct Att_Events *Events,
                                    Att_TypeOfView_t TypeOfView)
  {
   extern const char *The_ClassFormLinkInBoxBold[The_NUM_THEMES];
   extern const char *Txt_Events;
   extern const char *Txt_Event;
   extern const char *Txt_ROLES_PLURAL_Abc[Rol_NUM_ROLES][Usr_NUM_SEXS];
   extern const char *Txt_Update_attendance;
   unsigned UniqueId;
   char *Id;
   unsigned NumAttEvent;
   bool NormalView = (TypeOfView == Att_VIEW_ONLY_ME ||
                      TypeOfView == Att_VIEW_SEL_USR);

   /***** Begin box *****/
   switch (TypeOfView)
     {
      case Att_VIEW_ONLY_ME:
	 Box_BoxBegin (NULL,Txt_Events,
		       Att_PutIconToViewAttEvents,NULL,
		       NULL,Box_NOT_CLOSABLE);
	 break;
      case Att_VIEW_SEL_USR:
	 Box_BoxBegin (NULL,Txt_Events,
		       Att_PutIconToEditAttEvents,NULL,
		       NULL,Box_NOT_CLOSABLE);
	 break;
      case Att_PRNT_ONLY_ME:
      case Att_PRNT_SEL_USR:
	 Box_BoxBegin (NULL,Txt_Events,
		       NULL,NULL,
		       NULL,Box_NOT_CLOSABLE);
	 break;
     }

   /***** Begin form to update the attendance
	  depending on the events selected *****/
   if (NormalView)
     {
      Frm_StartFormAnchor (Gbl.Action.Act,Att_ATTENDANCE_TABLE_ID);
      Grp_PutParamsCodGrps ();
      Usr_PutHiddenParSelectedUsrsCods (&Gbl.Usrs.Selected);
     }

   /***** Begin table *****/
   HTM_TABLE_BeginWidePadding (2);

   /***** Heading row *****/
   HTM_TR_Begin (NULL);

   HTM_TH (1,4,"LM",Txt_Event);
   HTM_TH (1,1,"RM",Txt_ROLES_PLURAL_Abc[Rol_STD][Usr_SEX_UNKNOWN]);

   HTM_TR_End ();

   /***** List the events *****/
   for (NumAttEvent = 0, UniqueId = 1, Gbl.RowEvenOdd = 0;
	NumAttEvent < Events->Num;
	NumAttEvent++, UniqueId++, Gbl.RowEvenOdd = 1 - Gbl.RowEvenOdd)
     {
      /* Get data of the attendance event from database */
      Att_GetDataOfAttEventByCodAndCheckCrs (&Events->Lst[NumAttEvent]);
      Att_GetNumStdsTotalWhoAreInAttEvent (&Events->Lst[NumAttEvent]);

      /* Write a row for this event */
      HTM_TR_Begin (NULL);

      HTM_TD_Begin ("class=\"DAT CT COLOR%u\"",Gbl.RowEvenOdd);
      HTM_INPUT_CHECKBOX ("AttCods",HTM_DONT_SUBMIT_ON_CHANGE,
			  "id=\"Event%u\" value=\"%ld\"%s",
			  NumAttEvent,Events->Lst[NumAttEvent].AttCod,
			  Events->Lst[NumAttEvent].Selected ? " checked=\"checked\"" :
				                              "");
      HTM_TD_End ();

      HTM_TD_Begin ("class=\"DAT RT COLOR%u\"",Gbl.RowEvenOdd);
      HTM_LABEL_Begin ("for=\"Event%u\"",NumAttEvent);
      HTM_TxtF ("%u:",NumAttEvent + 1);
      HTM_LABEL_End ();
      HTM_TD_End ();

      if (asprintf (&Id,"att_date_start_%u",UniqueId) < 0)
	 Lay_NotEnoughMemoryExit ();
      HTM_TD_Begin ("class=\"DAT LT COLOR%u\"",Gbl.RowEvenOdd);
      HTM_LABEL_Begin ("for=\"Event%u\"",NumAttEvent);
      HTM_SPAN_Begin ("id=\"%s\"",Id);
      HTM_SPAN_End ();
      HTM_LABEL_End ();
      Dat_WriteLocalDateHMSFromUTC (Id,Events->Lst[NumAttEvent].TimeUTC[Att_START_TIME],
				    Gbl.Prefs.DateFormat,Dat_SEPARATOR_COMMA,
				    true,true,true,0x7);
      HTM_TD_End ();
      free (Id);

      HTM_TD_Begin ("class=\"DAT LT COLOR%u\"",Gbl.RowEvenOdd);
      HTM_Txt (Events->Lst[NumAttEvent].Title);
      HTM_TD_End ();

      HTM_TD_Begin ("class=\"DAT RT COLOR%u\"",Gbl.RowEvenOdd);
      HTM_Unsigned (Events->Lst[NumAttEvent].NumStdsTotal);
      HTM_TD_End ();

      HTM_TR_End ();
     }

   /***** Put button to refresh *****/
   if (NormalView)
     {
      HTM_TR_Begin (NULL);

      HTM_TD_Begin ("colspan=\"5\" class=\"CM\"");
      HTM_BUTTON_Animated_Begin (Txt_Update_attendance,
	                         The_ClassFormLinkInBoxBold[Gbl.Prefs.Theme],
				 NULL);
      Ico_PutCalculateIconWithText (Txt_Update_attendance);
      HTM_BUTTON_End ();
      HTM_TD_End ();

      HTM_TR_End ();
     }

   /***** End table *****/
   HTM_TABLE_End ();

   /***** End form *****/
   if (NormalView)
      Frm_EndForm ();

   /***** End box *****/
   Box_BoxEnd ();
  }

/*****************************************************************************/
/*********** Put icon to list (without edition) attendance events ************/
/*****************************************************************************/

static void Att_PutIconToViewAttEvents (__attribute__((unused)) void *Args)
  {
   Ico_PutContextualIconToView (ActSeeAtt,
				NULL,NULL);
  }

/*****************************************************************************/
/************ Put icon to list (with edition) attendance events **************/
/*****************************************************************************/

static void Att_PutIconToEditAttEvents (__attribute__((unused)) void *Args)
  {
   Ico_PutContextualIconToEdit (ActSeeAtt,NULL,
				NULL,NULL);
  }

/*****************************************************************************/
/************ Show table with attendances for every user in list *************/
/*****************************************************************************/

static void Att_ListUsrsAttendanceTable (const struct Att_Events *Events,
                                         Att_TypeOfView_t TypeOfView,
	                                 unsigned NumUsrsInList,
                                         long *LstSelectedUsrCods)
  {
   extern const char *Txt_Number_of_users;
   struct UsrData UsrDat;
   unsigned NumUsr;
   unsigned NumAttEvent;
   unsigned Total;
   bool PutButtonShowDetails = (TypeOfView == Att_VIEW_ONLY_ME ||
	                        TypeOfView == Att_VIEW_SEL_USR) &&
	                        !Events->ShowDetails;

   /***** Initialize structure with user's data *****/
   Usr_UsrDataConstructor (&UsrDat);

   /***** Start section with attendance table *****/
   HTM_SECTION_Begin (Att_ATTENDANCE_TABLE_ID);

   /***** Begin table *****/
   HTM_TABLE_BeginCenterPadding (2);

   /***** Heading row *****/
   Att_WriteTableHeadSeveralAttEvents (Events);

   /***** List the users *****/
   for (NumUsr = 0, Gbl.RowEvenOdd = 0;
	NumUsr < NumUsrsInList;
	NumUsr++)
     {
      UsrDat.UsrCod = LstSelectedUsrCods[NumUsr];
      if (Usr_ChkUsrCodAndGetAllUsrDataFromUsrCod (&UsrDat,Usr_DONT_GET_PREFS))		// Get from the database the data of the student
	 if (Usr_CheckIfICanViewAtt (&UsrDat))
	   {
	    UsrDat.Accepted = Usr_CheckIfUsrHasAcceptedInCurrentCrs (&UsrDat);
	    Att_WriteRowUsrSeveralAttEvents (Events,NumUsr,&UsrDat);
	   }
     }

   /***** Last row with the total of users present in each event *****/
   if (NumUsrsInList > 1)
     {
      HTM_TR_Begin (NULL);

      HTM_TD_Begin ("colspan=\"%u\" class=\"DAT_N LINE_TOP RM\"",
		    Gbl.Usrs.Listing.WithPhotos ? 4 :
						  3);
      HTM_TxtColon (Txt_Number_of_users);
      HTM_TD_End ();

      for (NumAttEvent = 0, Total = 0;
	   NumAttEvent < Events->Num;
	   NumAttEvent++)
	 if (Events->Lst[NumAttEvent].Selected)
	   {
	    HTM_TD_Begin ("class=\"DAT_N LINE_TOP RM\"");
	    HTM_Unsigned (Events->Lst[NumAttEvent].NumStdsFromList);
	    HTM_TD_End ();

	    Total += Events->Lst[NumAttEvent].NumStdsFromList;
	   }

      HTM_TD_Begin ("class=\"DAT_N LINE_TOP RM\"");
      HTM_Unsigned (Total);
      HTM_TD_End ();

      HTM_TR_End ();
     }

   /***** End table *****/
   HTM_TABLE_End ();

   /***** Button to show more details *****/
   if (PutButtonShowDetails)
      Att_PutButtonToShowDetails (Events);

   /***** End section with attendance table *****/
   HTM_SECTION_End ();

   /***** Free memory used for user's data *****/
   Usr_UsrDataDestructor (&UsrDat);
  }

/*****************************************************************************/
/* Write table heading for listing of students in several attendance events **/
/*****************************************************************************/

static void Att_WriteTableHeadSeveralAttEvents (const struct Att_Events *Events)
  {
   extern const char *Txt_ROLES_SINGUL_Abc[Rol_NUM_ROLES][Usr_NUM_SEXS];
   extern const char *Txt_Attendance;
   unsigned NumAttEvent;
   char StrNumAttEvent[Cns_MAX_DECIMAL_DIGITS_UINT + 1];

   HTM_TR_Begin (NULL);

   HTM_TH (1,Gbl.Usrs.Listing.WithPhotos ? 4 :
				           3,
           "LM",Txt_ROLES_SINGUL_Abc[Rol_USR][Usr_SEX_UNKNOWN]);

   for (NumAttEvent = 0;
	NumAttEvent < Events->Num;
	NumAttEvent++)
      if (Events->Lst[NumAttEvent].Selected)
	{
	 /***** Get data of this attendance event *****/
	 Att_GetDataOfAttEventByCodAndCheckCrs (&Events->Lst[NumAttEvent]);

	 /***** Put link to this attendance event *****/
	 HTM_TH_Begin (1,1,"CM");
	 snprintf (StrNumAttEvent,sizeof (StrNumAttEvent),
		   "%u",
		   NumAttEvent + 1);
	 Att_PutLinkAttEvent (&Events->Lst[NumAttEvent],
			      Events->Lst[NumAttEvent].Title,
			      StrNumAttEvent,
			      "BT_LINK TIT_TBL");
	 HTM_TH_End ();
	}

   HTM_TH (1,1,"RM",Txt_Attendance);

   HTM_TR_End ();
  }

/*****************************************************************************/
/************** Write a row of a table with the data of a user ***************/
/*****************************************************************************/

static void Att_WriteRowUsrSeveralAttEvents (const struct Att_Events *Events,
                                             unsigned NumUsr,struct UsrData *UsrDat)
  {
   char PhotoURL[PATH_MAX + 1];
   bool ShowPhoto;
   unsigned NumAttEvent;
   bool Present;
   unsigned NumTimesPresent;

   /***** Write number of user in the list *****/
   HTM_TR_Begin (NULL);

   HTM_TD_Begin ("class=\"%s RM COLOR%u\"",
		 UsrDat->Accepted ? "DAT_N" :
				    "DAT",
		 Gbl.RowEvenOdd);
   HTM_Unsigned (NumUsr + 1);
   HTM_TD_End ();

   /***** Show user's photo *****/
   if (Gbl.Usrs.Listing.WithPhotos)
     {
      HTM_TD_Begin ("class=\"LM COLOR%u\"",Gbl.RowEvenOdd);
      ShowPhoto = Pho_ShowingUsrPhotoIsAllowed (UsrDat,PhotoURL);
      Pho_ShowUsrPhoto (UsrDat,ShowPhoto ? PhotoURL :
                                           NULL,
                        "PHOTO21x28",Pho_ZOOM,false);
      HTM_TD_End ();
     }

   /***** Write user's ID ******/
   HTM_TD_Begin ("class=\"%s LM COLOR%u\"",
		 UsrDat->Accepted ? "DAT_SMALL_N" :
				    "DAT_SMALL",
		 Gbl.RowEvenOdd);
   ID_WriteUsrIDs (UsrDat,NULL);
   HTM_TD_End ();

   /***** Write user's name *****/
   HTM_TD_Begin ("class=\"%s LM COLOR%u\"",
		 UsrDat->Accepted ? "DAT_SMALL_N" :
				    "DAT_SMALL",
		 Gbl.RowEvenOdd);
   HTM_Txt (UsrDat->Surname1);
   if (UsrDat->Surname2[0])
      HTM_TxtF ("&nbsp;%s",UsrDat->Surname2);
   HTM_TxtF (", %s",UsrDat->FirstName);
   HTM_TD_End ();

   /***** Check/cross to show if the user is present/absent *****/
   for (NumAttEvent = 0, NumTimesPresent = 0;
	NumAttEvent < Events->Num;
	NumAttEvent++)
      if (Events->Lst[NumAttEvent].Selected)
	{
	 /* Check if this student is already registered in the current event */
	 // Here it is not necessary to get comments
	 Present = Att_CheckIfUsrIsPresentInAttEvent (Events->Lst[NumAttEvent].AttCod,
	                                              UsrDat->UsrCod);

	 /* Write check or cross */
	 HTM_TD_Begin ("class=\"BM%u\"",Gbl.RowEvenOdd);
	 Att_PutCheckOrCross (Present);
	 HTM_TD_End ();

	 if (Present)
	    NumTimesPresent++;
	}

   /***** Last column with the number of times this user is present *****/
   HTM_TD_Begin ("class=\"DAT_N RM COLOR%u\"",Gbl.RowEvenOdd);
   HTM_Unsigned (NumTimesPresent);
   HTM_TD_End ();

   HTM_TR_End ();

   Gbl.RowEvenOdd = 1 - Gbl.RowEvenOdd;
  }

/*****************************************************************************/
/*********************** Put check or cross character ************************/
/*****************************************************************************/

static void Att_PutCheckOrCross (bool Present)
  {
   extern const char *Txt_Present;
   extern const char *Txt_Absent;

   if (Present)
     {
      HTM_DIV_Begin ("class=\"ATT_CHECK\" title=\"%s\"",Txt_Present);
      HTM_Txt ("&check;");
     }
   else
     {
      HTM_DIV_Begin ("class=\"ATT_CROSS\" title=\"%s\"",Txt_Absent);
      HTM_Txt ("&cross;");
     }
   HTM_DIV_End ();
  }

/*****************************************************************************/
/**************** List the students with details and comments ****************/
/*****************************************************************************/

static void Att_ListStdsWithAttEventsDetails (const struct Att_Events *Events,
                                              unsigned NumUsrsInList,
                                              long *LstSelectedUsrCods)
  {
   extern const char *Txt_Details;
   struct UsrData UsrDat;
   unsigned NumUsr;

   /***** Initialize structure with user's data *****/
   Usr_UsrDataConstructor (&UsrDat);

   /***** Start section with attendance details *****/
   HTM_SECTION_Begin (Att_ATTENDANCE_DETAILS_ID);

   /***** Begin box and table *****/
   Box_BoxTableBegin (NULL,Txt_Details,
                      NULL,NULL,
	              NULL,Box_NOT_CLOSABLE,2);

   /***** List students with attendance details *****/
   for (NumUsr = 0, Gbl.RowEvenOdd = 0;
	NumUsr < NumUsrsInList;
	NumUsr++)
     {
      UsrDat.UsrCod = LstSelectedUsrCods[NumUsr];
      if (Usr_ChkUsrCodAndGetAllUsrDataFromUsrCod (&UsrDat,Usr_DONT_GET_PREFS))	// Get from the database the data of the student
	 if (Usr_CheckIfICanViewAtt (&UsrDat))
	   {
	    UsrDat.Accepted = Usr_CheckIfUsrHasAcceptedInCurrentCrs (&UsrDat);
	    Att_ListAttEventsForAStd (Events,NumUsr,&UsrDat);
	   }
     }

   /***** End table and box *****/
   Box_BoxTableEnd ();

   /***** End section with attendance details *****/
   HTM_SECTION_End ();

   /***** Free memory used for user's data *****/
   Usr_UsrDataDestructor (&UsrDat);
  }

/*****************************************************************************/
/*************** Write list of attendance events for a student ***************/
/*****************************************************************************/

static void Att_ListAttEventsForAStd (const struct Att_Events *Events,
                                      unsigned NumUsr,struct UsrData *UsrDat)
  {
   extern const char *Txt_Student_comment;
   extern const char *Txt_Teachers_comment;
   char PhotoURL[PATH_MAX + 1];
   bool ShowPhoto;
   unsigned NumAttEvent;
   unsigned UniqueId;
   char *Id;
   bool Present;
   bool ShowCommentStd;
   bool ShowCommentTch;
   char CommentStd[Cns_MAX_BYTES_TEXT + 1];
   char CommentTch[Cns_MAX_BYTES_TEXT + 1];

   /***** Write number of student in the list *****/
   NumUsr++;
   HTM_TR_Begin (NULL);

   HTM_TD_Begin ("class=\"%s RM COLOR%u\"",
		 UsrDat->Accepted ? "DAT_N" :
				    "DAT",
		 Gbl.RowEvenOdd);
   HTM_TxtF ("%u:",NumUsr);
   HTM_TD_End ();

   /***** Show student's photo *****/
   HTM_TD_Begin ("colspan=\"2\" class=\"RM COLOR%u\"",Gbl.RowEvenOdd);
   ShowPhoto = Pho_ShowingUsrPhotoIsAllowed (UsrDat,PhotoURL);
   Pho_ShowUsrPhoto (UsrDat,ShowPhoto ? PhotoURL :
				        NULL,
		     "PHOTO21x28",Pho_ZOOM,false);
   HTM_TD_End ();

   /***** Write user's ID ******/
   HTM_TD_Begin ("class=\"LM COLOR%u\"",Gbl.RowEvenOdd);
   HTM_TABLE_Begin (NULL);
   HTM_TR_Begin (NULL);

   HTM_TD_Begin ("class=\"%s LM\"",
		 UsrDat->Accepted ? "DAT_N" :
				    "DAT");
   ID_WriteUsrIDs (UsrDat,NULL);
   HTM_TD_End ();

   /***** Write student's name *****/
   HTM_TD_Begin ("class=\"%s LM\"",
		 UsrDat->Accepted ? "DAT_SMALL_N" :
				    "DAT_SMALL");
   HTM_Txt (UsrDat->Surname1);
   if (UsrDat->Surname2[0])
      HTM_TxtF ("&nbsp;%s",UsrDat->Surname2);
   HTM_TxtF (", %s",UsrDat->FirstName);
   HTM_TD_End ();

   HTM_TR_End ();
   HTM_TABLE_End ();
   HTM_TD_End ();
   HTM_TR_End ();

   /***** List the events with students *****/
   for (NumAttEvent = 0, UniqueId = 1;
	NumAttEvent < Events->Num;
	NumAttEvent++, UniqueId++)
      if (Events->Lst[NumAttEvent].Selected)
	{
	 /***** Get data of the attendance event from database *****/
	 Att_GetDataOfAttEventByCodAndCheckCrs (&Events->Lst[NumAttEvent]);
         Att_GetNumStdsTotalWhoAreInAttEvent (&Events->Lst[NumAttEvent]);

	 /***** Get comments for this student *****/
	 Present = Att_CheckIfUsrIsPresentInAttEventAndGetComments (Events->Lst[NumAttEvent].AttCod,UsrDat->UsrCod,CommentStd,CommentTch);
         ShowCommentStd = CommentStd[0];
	 ShowCommentTch = CommentTch[0] &&
	                  (Gbl.Usrs.Me.Role.Logged == Rol_TCH ||
	                   Events->Lst[NumAttEvent].CommentTchVisible);

	 /***** Write a row for this event *****/
	 HTM_TR_Begin (NULL);

	 HTM_TD_ColouredEmpty (1);

	 HTM_TD_Begin ("class=\"%s RT COLOR%u\"",
		       Present ? "DAT_GREEN" :
				 "DAT_RED",
		       Gbl.RowEvenOdd);
	 HTM_TxtF ("%u:",NumAttEvent + 1);
	 HTM_TD_End ();

	 HTM_TD_Begin ("class=\"BT%u\"",Gbl.RowEvenOdd);
         Att_PutCheckOrCross (Present);
	 HTM_TD_End ();

	 if (asprintf (&Id,"att_date_start_%u_%u",NumUsr,UniqueId) < 0)
	    Lay_NotEnoughMemoryExit ();
	 HTM_TD_Begin ("class=\"DAT LT COLOR%u\"",Gbl.RowEvenOdd);
	 HTM_SPAN_Begin ("id=\"%s\"",Id);
	 HTM_SPAN_End ();
         HTM_BR ();
	 HTM_Txt (Events->Lst[NumAttEvent].Title);
	 Dat_WriteLocalDateHMSFromUTC (Id,Events->Lst[NumAttEvent].TimeUTC[Att_START_TIME],
				       Gbl.Prefs.DateFormat,Dat_SEPARATOR_COMMA,
				       true,true,true,0x7);
	 HTM_TD_End ();
         free (Id);

	 HTM_TR_End ();

	 /***** Write comments for this student *****/
	 if (ShowCommentStd || ShowCommentTch)
	   {
	    HTM_TR_Begin (NULL);

	    HTM_TD_ColouredEmpty (2);

	    HTM_TD_Begin ("class=\"BT%u\"",Gbl.RowEvenOdd);
	    HTM_TD_End ();

	    HTM_TD_Begin ("class=\"DAT LM COLOR%u\"",Gbl.RowEvenOdd);

	    HTM_DL_Begin ();
	    if (ShowCommentStd)
	      {
	       Str_ChangeFormat (Str_FROM_HTML,Str_TO_RIGOROUS_HTML,
				 CommentStd,Cns_MAX_BYTES_TEXT,false);
	       HTM_DT_Begin ();
	       HTM_TxtColon (Txt_Student_comment);
	       HTM_DT_End ();
	       HTM_DD_Begin ();
	       HTM_Txt (CommentStd);
	       HTM_DD_End ();
	      }
	    if (ShowCommentTch)
	      {
	       Str_ChangeFormat (Str_FROM_HTML,Str_TO_RIGOROUS_HTML,
				 CommentTch,Cns_MAX_BYTES_TEXT,false);
	       HTM_DT_Begin ();
	       HTM_TxtColon (Txt_Teachers_comment);
	       HTM_DT_End ();
	       HTM_DD_Begin ();
	       HTM_Txt (CommentTch);
	       HTM_DD_End ();
	      }
	    HTM_DL_End ();

	    HTM_TD_End ();

	    HTM_TR_End ();
	   }
	}

   Gbl.RowEvenOdd = 1 - Gbl.RowEvenOdd;
  }
