// swad_follow.c: user's followers and followed

/*
    SWAD (Shared Workspace At a Distance),
    is a web platform developed at the University of Granada (Spain),
    and used to support university teaching.

    This file is part of SWAD core.
    Copyright (C) 1999-2019 Antonio Ca�as Vargas

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General 3 License as
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
/*********************************** Headers *********************************/
/*****************************************************************************/

#include <stdbool.h>		// For boolean type
#include <string.h>		// For string functions

#include "swad_box.h"
#include "swad_database.h"
#include "swad_follow.h"
#include "swad_form.h"
#include "swad_global.h"
#include "swad_notification.h"
#include "swad_privacy.h"
#include "swad_profile.h"
#include "swad_user.h"

/*****************************************************************************/
/****************************** Public constants *****************************/
/*****************************************************************************/

/*****************************************************************************/
/***************************** Private constants *****************************/
/*****************************************************************************/

#define Fol_NUM_COLUMNS_FOLLOW 3

#define Fol_FOLLOW_SECTION_ID	"follow_section"

/*****************************************************************************/
/****************************** Internal types *******************************/
/*****************************************************************************/

typedef enum
  {
   Fol_SUGGEST_ONLY_USERS_WITH_PHOTO,
   Fol_SUGGEST_ANY_USER,
  } Fol_WhichUsersSuggestToFollowThem_t;

/*****************************************************************************/
/************** External global variables from others modules ****************/
/*****************************************************************************/

extern struct Globals Gbl;

/*****************************************************************************/
/************************* Internal global variables *************************/
/*****************************************************************************/

/*****************************************************************************/
/***************************** Private prototypes ****************************/
/*****************************************************************************/

static unsigned long Fol_GetUsrsToFollow (unsigned long MaxUsrsToShow,
					  Fol_WhichUsersSuggestToFollowThem_t WhichUsersSuggestToFollowThem,
					  MYSQL_RES **mysql_res);

static void Fol_PutIconsWhoToFollow (void);
static void Fol_PutIconToUpdateWhoToFollow (void);

static void Fol_ShowNumberOfFollowingOrFollowers (const struct UsrData *UsrDat,
                                                  unsigned NumUsrs,
                                                  Act_Action_t Action,
                                                  const char *Title);

static void Fol_ListFollowingUsr (struct UsrData *UsrDat);
static void Fol_ListFollowersUsr (struct UsrData *UsrDat);

static void Fol_ShowFollowedOrFollower (struct UsrData *UsrDat);
static void Fol_WriteRowUsrToFollowOnRightColumn (struct UsrData *UsrDat);
static void Fol_PutInactiveIconToFollowUnfollow (void);
static void Fol_PutIconToFollow (struct UsrData *UsrDat);
static void Fol_PutIconToUnfollow (struct UsrData *UsrDat);

static void Fol_RequestFollowUsrs (Act_Action_t NextAction,void (*FuncParams) ());
static void Fol_RequestUnfollowUsrs (Act_Action_t NextAction,void (*FuncParams) ());
static void Fol_GetFollowedFromSelectedUsrs (unsigned *NumFollowed,
                                             unsigned *NumNotFollowed);
static void Fol_PutParamsFollowSelectedStds (void);
static void Fol_PutParamsFollowSelectedTchs (void);
static void Fol_PutParamsUnfollowSelectedStds (void);
static void Fol_PutParamsUnfollowSelectedTchs (void);

static void Fol_FollowUsr (struct UsrData *UsrDat);
static void Fol_UnfollowUsr (struct UsrData *UsrDat);

/*****************************************************************************/
/********************** Put link to show users to follow **********************/
/*****************************************************************************/

void Fol_PutLinkWhoToFollow (void)
  {
   extern const char *Txt_Who_to_follow;

   Lay_PutContextualLinkIconText (ActSeeSocPrf,NULL,NULL,
				  "user-plus.svg",
				  Txt_Who_to_follow);
  }

/*****************************************************************************/
/****************** Show several users to follow on main zone ****************/
/*****************************************************************************/

#define Fol_MAX_USRS_TO_FOLLOW_MAIN_ZONE (Fol_NUM_COLUMNS_FOLLOW * 3)

void Fol_SuggestUsrsToFollowMainZone (void)
  {
   extern const char *Hlp_START_Profiles_who_to_follow;
   extern const char *Txt_Who_to_follow;
   extern const char *Txt_No_user_to_whom_you_can_follow_Try_again_later;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumUsrs;
   unsigned long NumUsr;
   struct UsrData UsrDat;

   /***** Put links to request my public profile and another user's profile *****/
   fprintf (Gbl.F.Out,"<div class=\"CONTEXT_MENU\">");
   Prf_PutLinkMyPublicProfile ();
   Prf_PutLinkRequestAnotherUserProfile ();
   fprintf (Gbl.F.Out,"</div>");

   /***** Get users *****/
   if ((NumUsrs = Fol_GetUsrsToFollow (Fol_MAX_USRS_TO_FOLLOW_MAIN_ZONE,
                                       Fol_SUGGEST_ANY_USER,
                                       &mysql_res)))
     {
      /***** Start box and table *****/
      Box_StartBoxTable ("560px",Txt_Who_to_follow,Fol_PutIconsWhoToFollow,
                         Hlp_START_Profiles_who_to_follow,Box_NOT_CLOSABLE,2);

      /***** Initialize structure with user's data *****/
      Usr_UsrDataConstructor (&UsrDat);

      /***** List users *****/
      for (NumUsr = 0;
	   NumUsr < NumUsrs;
	   NumUsr++)
	{
	 /***** Get user *****/
	 row = mysql_fetch_row (mysql_res);

	 /* Get user's code (row[0]) */
	 UsrDat.UsrCod = Str_ConvertStrCodToLongCod (row[0]);

	 /***** Show user *****/
	 if ((NumUsr % Fol_NUM_COLUMNS_FOLLOW) == 0)
	    fprintf (Gbl.F.Out,"<tr>");
	 if (Usr_ChkUsrCodAndGetAllUsrDataFromUsrCod (&UsrDat,Usr_DONT_GET_PREFS))
	    Fol_ShowFollowedOrFollower (&UsrDat);
	 if ((NumUsr % Fol_NUM_COLUMNS_FOLLOW) == (Fol_NUM_COLUMNS_FOLLOW-1) ||
	     NumUsr == NumUsrs - 1)
	    fprintf (Gbl.F.Out,"</tr>");
	}

      /***** Free memory used for user's data *****/
      Usr_UsrDataDestructor (&UsrDat);

      /***** End table and box *****/
      Box_EndBoxTable ();
     }
   else
      Ale_ShowAlert (Ale_INFO,Txt_No_user_to_whom_you_can_follow_Try_again_later);

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/**************** Show several users to follow on right column ***************/
/*****************************************************************************/

#define Fol_MAX_USRS_TO_FOLLOW_RIGHT_COLUMN 3

void Fol_SuggestUsrsToFollowMainZoneOnRightColumn (void)
  {
   extern const char *Txt_Who_to_follow;
   extern const char *Txt_No_user_to_whom_you_can_follow_Try_again_later;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumUsrs;
   unsigned long NumUsr;
   struct UsrData UsrDat;

   /***** Get users *****/
   if ((NumUsrs = Fol_GetUsrsToFollow (Fol_MAX_USRS_TO_FOLLOW_RIGHT_COLUMN,
                                       Fol_SUGGEST_ONLY_USERS_WITH_PHOTO,
                                       &mysql_res)))
     {
      /***** Start container *****/
      fprintf (Gbl.F.Out,"<div class=\"CONNECTED\">");

      /***** Title with link to suggest more users to follow *****/
      Frm_StartForm (ActSeeSocPrf);
      Frm_LinkFormSubmit (Txt_Who_to_follow,"CONNECTED_TXT",NULL);
      fprintf (Gbl.F.Out,"%s</a>",Txt_Who_to_follow);
      Frm_EndForm ();

      /***** Start table *****/
      fprintf (Gbl.F.Out,"<table>");

      /***** Initialize structure with user's data *****/
      Usr_UsrDataConstructor (&UsrDat);

      /***** List users *****/
      for (NumUsr = 0;
	   NumUsr < NumUsrs;
	   NumUsr++)
	{
	 /***** Get user *****/
	 row = mysql_fetch_row (mysql_res);

	 /* Get user's code (row[0]) */
	 UsrDat.UsrCod = Str_ConvertStrCodToLongCod (row[0]);

	 /***** Show user *****/
	 if (Usr_ChkUsrCodAndGetAllUsrDataFromUsrCod (&UsrDat,Usr_DONT_GET_PREFS))
	    Fol_WriteRowUsrToFollowOnRightColumn (&UsrDat);
	}

      /***** Free memory used for user's data *****/
      Usr_UsrDataDestructor (&UsrDat);

      /***** End table *****/
      fprintf (Gbl.F.Out,"</table>");

      /***** End container *****/
      fprintf (Gbl.F.Out,"</div>");
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/*************************** Get users to follow *****************************/
/*****************************************************************************/

static unsigned long Fol_GetUsrsToFollow (unsigned long MaxUsrsToShow,
					  Fol_WhichUsersSuggestToFollowThem_t WhichUsersSuggestToFollowThem,
					  MYSQL_RES **mysql_res)
  {
   extern const char *Pri_VisibilityDB[Pri_NUM_OPTIONS_PRIVACY];
   char SubQuery1[256];
   char SubQuery2[256];
   char SubQuery3[256];
   char SubQuery4[256];

   /***** Build subqueries related to photos *****/
   switch (WhichUsersSuggestToFollowThem)
     {
      case Fol_SUGGEST_ONLY_USERS_WITH_PHOTO:
	 // Photo visibility should be >= profile visibility in every subquery
	 sprintf (SubQuery1,		// 1. Users followed by my followed
		  " AND usr_data.PhotoVisibility IN ('%s','%s')"
		  " AND usr_data.Photo<>''",
		  Pri_VisibilityDB[Pri_VISIBILITY_SYSTEM],
		  Pri_VisibilityDB[Pri_VISIBILITY_WORLD ]);
	 sprintf (SubQuery2,		// 2. Users who share any course with me
		  " AND usr_data.PhotoVisibility IN ('%s','%s','%s')"
		  " AND usr_data.Photo<>''",
		  Pri_VisibilityDB[Pri_VISIBILITY_COURSE],
		  Pri_VisibilityDB[Pri_VISIBILITY_SYSTEM],
		  Pri_VisibilityDB[Pri_VISIBILITY_WORLD ]);
	 sprintf (SubQuery3,		// 3. Users who share any course with me with another role
		  " AND usr_data.PhotoVisibility IN ('%s','%s','%s','%s')"
		  " AND usr_data.Photo<>''",
		  Pri_VisibilityDB[Pri_VISIBILITY_USER  ],
		  Pri_VisibilityDB[Pri_VISIBILITY_COURSE],
		  Pri_VisibilityDB[Pri_VISIBILITY_SYSTEM],
		  Pri_VisibilityDB[Pri_VISIBILITY_WORLD ]);
	 sprintf (SubQuery4,		// 4. Add some likely unknown random users
		  " AND usr_data.PhotoVisibility IN ('%s','%s')"
		  " AND usr_data.Photo<>''",
		  Pri_VisibilityDB[Pri_VISIBILITY_SYSTEM],
		  Pri_VisibilityDB[Pri_VISIBILITY_WORLD ]);
	 break;
      case Fol_SUGGEST_ANY_USER:
	 SubQuery1[0] = '\0';
	 SubQuery2[0] = '\0';
	 SubQuery3[0] = '\0';
	 SubQuery4[0] = '\0';
	 break;
     }

   /***** Build query to get users to follow *****/
   // Get only users with surname 1 and first name
   return DB_QuerySELECT (mysql_res,"can not get users to follow",
			  "SELECT DISTINCT UsrCod FROM"
			  " ("
			  /***** Likely known users *****/
			  "(SELECT DISTINCT UsrCod FROM"
			  " ("
			  // 1. Users followed by my followed
			  "("
			  "SELECT DISTINCT usr_follow.FollowedCod AS UsrCod"
			  " FROM usr_follow,"
			  "(SELECT FollowedCod FROM usr_follow"
			  " WHERE FollowerCod=%ld) AS my_followed,"
			  " usr_data"
			  " WHERE usr_follow.FollowerCod=my_followed.FollowedCod"
			  " AND usr_follow.FollowedCod<>%ld"
			  " AND usr_follow.FollowedCod=usr_data.UsrCod"
			  " AND usr_data.Surname1<>''"	// Surname 1 not empty
			  " AND usr_data.FirstName<>''"	// First name not empty
			  "%s"				// SubQuery1
			  ")"
			  " UNION "
			  // 2. Users who share any course with me
			  "("
			  "SELECT DISTINCT crs_usr.UsrCod"
			  " FROM crs_usr,"
			  "(SELECT CrsCod FROM crs_usr"
			  " WHERE UsrCod=%ld) AS my_crs,"
			  " usr_data"
			  " WHERE crs_usr.CrsCod=my_crs.CrsCod"
			  " AND crs_usr.UsrCod<>%ld"
			  " AND crs_usr.UsrCod=usr_data.UsrCod"
			  " AND usr_data.Surname1<>''"	// Surname 1 not empty
			  " AND usr_data.FirstName<>''"	// First name not empty
			  "%s"				// SubQuery2
			  ")"
			  " UNION "
			  // 3. Users who share any course with me with another role
			  "("
			  "SELECT DISTINCT crs_usr.UsrCod"
			  " FROM crs_usr,"
			  "(SELECT CrsCod,Role FROM crs_usr"
			  " WHERE UsrCod=%ld) AS my_crs_role,"
			  " usr_data"
			  " WHERE crs_usr.CrsCod=my_crs_role.CrsCod"
			  " AND crs_usr.Role<>my_crs_role.Role"
			  " AND crs_usr.UsrCod=usr_data.UsrCod"
			  " AND usr_data.Surname1<>''"	// Surname 1 not empty
			  " AND usr_data.FirstName<>''"	// First name not empty
			  "%s"				// SubQuery3
			  ")"
			  ") AS LikelyKnownUsrsToFollow"
			  // Do not select my followed
			  " WHERE UsrCod NOT IN"
			  " (SELECT FollowedCod FROM usr_follow"
			  " WHERE FollowerCod=%ld)"
			  // Get only MaxUsrsToShow * 3 users
			  " ORDER BY RAND() LIMIT %lu"
			  ")"
			  " UNION "
			  "("
			  /***** Likely unknown userd *****/
			  // 4. Add some likely unknown random user
			  // Be careful with the method to get some random users
			  // from the big table of users.
			  // It's much faster getting a random code and then get the first users
			  // with codes >= that random code
			  // that getting all users and then ordering by rand.
			  "SELECT usr_data.UsrCod"
			  " FROM usr_data,"
			  "(SELECT ROUND(RAND()*(SELECT MAX(UsrCod) FROM usr_data)) AS RandomUsrCod)"	// a random user code
			  " AS random_usr"
			  " WHERE usr_data.UsrCod<>%ld"
			  " AND usr_data.Surname1<>''"	// Surname 1 not empty
			  " AND usr_data.FirstName<>''"	// First name not empty
			  "%s"				// SubQuery4
			  // Do not select my followed
			  " AND usr_data.UsrCod NOT IN"
			  " (SELECT FollowedCod FROM usr_follow"
			  " WHERE FollowerCod=%ld)"
			  " AND usr_data.UsrCod>=random_usr.RandomUsrCod"	// random user code could not exists in table of users
			  // Get only MaxUsrsToShow users
			  " LIMIT %lu"
			  ")"
			  ") AS UsrsToFollow"
			  // Get only MaxUsrsToShow users
			  " ORDER BY RAND() LIMIT %lu",

			  Gbl.Usrs.Me.UsrDat.UsrCod,
			  Gbl.Usrs.Me.UsrDat.UsrCod,
			  SubQuery1,
			  Gbl.Usrs.Me.UsrDat.UsrCod,
			  Gbl.Usrs.Me.UsrDat.UsrCod,
			  SubQuery2,
			  Gbl.Usrs.Me.UsrDat.UsrCod,
			  SubQuery3,
			  Gbl.Usrs.Me.UsrDat.UsrCod,
			  MaxUsrsToShow * 2,		// 2/3 likely known users

			  Gbl.Usrs.Me.UsrDat.UsrCod,
			  SubQuery4,
			  Gbl.Usrs.Me.UsrDat.UsrCod,
			  MaxUsrsToShow,		// 1/3 likely unknown users

			  MaxUsrsToShow);
  }

/*****************************************************************************/
/****************** Put contextual icons in "who to follow" ******************/
/*****************************************************************************/

static void Fol_PutIconsWhoToFollow (void)
  {
   /***** Put icon to update who to follow *****/
   Fol_PutIconToUpdateWhoToFollow ();

   /***** Put icon to show a figure *****/
   Gbl.Figures.FigureType = Fig_FOLLOW;
   Fig_PutIconToShowFigure ();
  }

/*****************************************************************************/
/********************* Put icon to update who to follow **********************/
/*****************************************************************************/

static void Fol_PutIconToUpdateWhoToFollow (void)
  {
   extern const char *Txt_Update;

   Frm_StartForm (ActSeeSocPrf);
   Frm_LinkFormSubmitAnimated (Txt_Update,NULL,NULL);
   Ico_PutCalculateIcon (Txt_Update);
   Frm_EndForm ();
  }

/*****************************************************************************/
/*************** Check if a user is a follower of another user ***************/
/*****************************************************************************/

bool Fol_CheckUsrIsFollowerOf (long FollowerCod,long FollowedCod)
  {
   if (FollowerCod == FollowedCod)
      return false;

   /***** Check if a user is a follower of another user *****/
   return (DB_QueryCOUNT ("can not get if a user is a follower of another one",
			  "SELECT COUNT(*) FROM usr_follow"
			  " WHERE FollowerCod=%ld AND FollowedCod=%ld",
			  FollowerCod,FollowedCod) != 0);
  }

/*****************************************************************************/
/*************************** Get number of followed **************************/
/*****************************************************************************/

void Fol_FlushCacheFollow (void)
  {
   Gbl.Cache.Follow.UsrCod = -1L;
   Gbl.Cache.Follow.NumFollowing =
   Gbl.Cache.Follow.NumFollowers = 0;
  }

void Fol_GetNumFollow (long UsrCod,
                       unsigned *NumFollowing,unsigned *NumFollowers)
  {
   /***** 1. Fast check: trivial cases *****/
   if (UsrCod <= 0)
     {
      *NumFollowing = *NumFollowers = 0;
      return;
     }

   /***** 2. Fast check: Is number of following already calculated? *****/
   if (UsrCod == Gbl.Cache.Follow.UsrCod)
     {
      *NumFollowing = Gbl.Cache.Follow.NumFollowing;
      *NumFollowers = Gbl.Cache.Follow.NumFollowers;
      return;
     }

   /***** 3. Slow check: Get number of following/followers from database *****/
   Gbl.Cache.Follow.UsrCod = UsrCod;
   *NumFollowing = Gbl.Cache.Follow.NumFollowing =
      (unsigned) DB_QueryCOUNT ("can not get number of followed",
	                        "SELECT COUNT(*) FROM usr_follow"
	                        " WHERE FollowerCod=%ld",
                                UsrCod);
   *NumFollowers = Gbl.Cache.Follow.NumFollowers =
      (unsigned) DB_QueryCOUNT ("can not get number of followers",
			        "SELECT COUNT(*) FROM usr_follow"
			        " WHERE FollowedCod=%ld",
			        UsrCod);
  }

/*****************************************************************************/
/**************** Show following and followers of a user *********************/
/*****************************************************************************/

void Fol_ShowFollowingAndFollowers (const struct UsrData *UsrDat,
                                    unsigned NumFollowing,unsigned NumFollowers,
                                    bool UsrFollowsMe,bool IFollowUsr)
  {
   extern const char *Txt_FOLLOWS_YOU;
   extern const char *Txt_Following;
   extern const char *Txt_Followers;
   extern const char *Txt_Following_unfollow;
   extern const char *Txt_Unfollow;
   extern const char *Txt_Follow;
   bool ItsMe = Usr_ItsMe (UsrDat->UsrCod);

   /***** Start section *****/
   Lay_StartSection (Fol_FOLLOW_SECTION_ID);

   /***** Followed users *****/
   fprintf (Gbl.F.Out,"<div id=\"following_side\">"
                      "<div class=\"FOLLOW_SIDE\">");

   /* User follows me? */
   fprintf (Gbl.F.Out,"<div id=\"follows_me\" class=\"DAT_LIGHT\">");
   if (UsrFollowsMe)
      fprintf (Gbl.F.Out,"%s",Txt_FOLLOWS_YOU);
   fprintf (Gbl.F.Out,"</div>");

   /* Number of followed */
   Fol_ShowNumberOfFollowingOrFollowers (UsrDat,
                                         NumFollowing,
                                         ActSeeFlg,Txt_Following);

   /* End following side */
   fprintf (Gbl.F.Out,"</div>"
                      "</div>");

   /***** Followers *****/
   fprintf (Gbl.F.Out,"<div id=\"followers_side\">"
                      "<div class=\"FOLLOW_SIDE\">");

   /* Number of followers */
   Fol_ShowNumberOfFollowingOrFollowers (UsrDat,
                                         NumFollowers,
                                         ActSeeFlr,Txt_Followers);

   /* I follow user? */
   fprintf (Gbl.F.Out,"<div id=\"follow_usr\">");
   if (Gbl.Usrs.Me.Logged &&	// Logged
       !ItsMe)			// Not me!
     {
      if (IFollowUsr)	// I follow this user
	{
	 Frm_StartForm (ActUnfUsr);
	 Usr_PutParamUsrCodEncrypted (UsrDat->EncryptedUsrCod);
	 Frm_LinkFormSubmit (Txt_Following_unfollow,"REC_DAT_BOLD",NULL);
	 fprintf (Gbl.F.Out,"<div class=\"ICO_HIGHLIGHT\""
			    " style=\"display:inline;\" >"
			    "<img src=\"%s/user-check.svg\""
			    " alt=\"%s\" title=\"%s\""
			    " class=\"ICO40x40\" />"
			    "</div>"
			    "</a>",
		  Cfg_URL_ICON_PUBLIC,
		  Txt_Unfollow,Txt_Following_unfollow);
	 Frm_EndForm ();
	}
      else		// I do not follow this user
	{
	 Frm_StartForm (ActFolUsr);
	 Usr_PutParamUsrCodEncrypted (UsrDat->EncryptedUsrCod);
	 Frm_LinkFormSubmit (Txt_Follow,"REC_DAT_BOLD",NULL);
	 fprintf (Gbl.F.Out,"<div class=\"ICO_HIGHLIGHT\""
			    " style=\"display:inline;\" >"
			    "<img src=\"%s/user-plus.svg\""
			    " alt=\"%s\" title=\"%s\""
			    " class=\"ICO40x40\" />"
			    "</div>"
			    "</a>",
		  Cfg_URL_ICON_PUBLIC,
		  Txt_Follow,Txt_Follow);
	 Frm_EndForm ();
	}
     }
   fprintf (Gbl.F.Out,"</div>");

   /* End followers side */
   fprintf (Gbl.F.Out,"</div>"
	              "</div>");

   /***** End section *****/
   Lay_EndSection ();
  }

/*****************************************************************************/
/**************** Show following and followers of a user *********************/
/*****************************************************************************/

static void Fol_ShowNumberOfFollowingOrFollowers (const struct UsrData *UsrDat,
                                                  unsigned NumUsrs,
                                                  Act_Action_t Action,
                                                  const char *Title)
  {
   extern const char *The_ClassFormOutBox[The_NUM_THEMES];
   extern const char *The_ClassFormOutBoxBold[The_NUM_THEMES];

   /***** Start container *****/
   fprintf (Gbl.F.Out,"<div class=\"FOLLOW_BOX\">");

   /***** Number *****/
   if (NumUsrs)
     {
      /* Form to list users */
      Frm_StartFormAnchor (Action,Fol_FOLLOW_SECTION_ID);
      Usr_PutParamUsrCodEncrypted (UsrDat->EncryptedUsrCod);
      Frm_LinkFormSubmit (Title,
                          (Gbl.Action.Act == Action) ? "FOLLOW_NUM_B" :
        	                                       "FOLLOW_NUM",NULL);
     }
   else
      fprintf (Gbl.F.Out,"<span class=\"%s\">",
	       (Gbl.Action.Act == Action) ? "FOLLOW_NUM_B" :
					    "FOLLOW_NUM");
   fprintf (Gbl.F.Out,"%u",NumUsrs);
   if (NumUsrs)
     {
      fprintf (Gbl.F.Out,"</a>");
      Frm_EndForm ();
     }
   else
      fprintf (Gbl.F.Out,"</span>");

   /***** Text *****/
   fprintf (Gbl.F.Out,"<div class=\"%s\">",
            (Gbl.Action.Act == Action) ? The_ClassFormOutBoxBold[Gbl.Prefs.Theme] :
        	                         The_ClassFormOutBox    [Gbl.Prefs.Theme]);
   if (NumUsrs)
     {
      /* Form to list users */
      Frm_StartFormAnchor (Action,Fol_FOLLOW_SECTION_ID);
      Usr_PutParamUsrCodEncrypted (UsrDat->EncryptedUsrCod);
      Frm_LinkFormSubmit (Title,
                          (Gbl.Action.Act == Action) ? The_ClassFormOutBoxBold[Gbl.Prefs.Theme] :
        	                                       The_ClassFormOutBox    [Gbl.Prefs.Theme],
			  NULL);
     }
   fprintf (Gbl.F.Out,"%s",Title);
   if (NumUsrs)
     {
      fprintf (Gbl.F.Out,"</a>");
      Frm_EndForm ();
     }
   fprintf (Gbl.F.Out,"</div>");

   /***** End container *****/
   fprintf (Gbl.F.Out,"</div>");
  }

/*****************************************************************************/
/***************************** List followed users ***************************/
/*****************************************************************************/

void Fol_ListFollowing (void)
  {
   /***** Get user to view user he/she follows *****/
   Usr_GetParamOtherUsrCodEncryptedAndGetListIDs ();

   if (Gbl.Usrs.Other.UsrDat.UsrCod > 0)
     {
      if (Usr_ChkUsrCodAndGetAllUsrDataFromUsrCod (&Gbl.Usrs.Other.UsrDat,Usr_DONT_GET_PREFS))
	 Fol_ListFollowingUsr (&Gbl.Usrs.Other.UsrDat);
      else
         Ale_ShowAlertUserNotFoundOrYouDoNotHavePermission ();
     }
   else				// If user not specified, view my profile
      Fol_ListFollowingUsr (&Gbl.Usrs.Me.UsrDat);
  }

static void Fol_ListFollowingUsr (struct UsrData *UsrDat)
  {
   extern const char *Txt_Following;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumUsrs;
   unsigned long NumUsr;
   struct UsrData FollowingUsrDat;

   /***** Show user's profile *****/
   if (Prf_ShowUserProfile (UsrDat))
     {
      /***** Check if a user is a follower of another user *****/
      NumUsrs = DB_QuerySELECT (&mysql_res,"can not get followed users",
				"SELECT FollowedCod FROM usr_follow"
				" WHERE FollowerCod=%ld"
				" ORDER BY FollowTime DESC",
				UsrDat->UsrCod);

      if (NumUsrs)
	{
	 /***** Initialize structure with user's data *****/
	 Usr_UsrDataConstructor (&FollowingUsrDat);

         /***** Start box and table *****/
	 Box_StartBoxTable ("560px",Txt_Following,NULL,
	                    NULL,Box_NOT_CLOSABLE,2);

	 for (NumUsr = 0;
	      NumUsr < NumUsrs;
	      NumUsr++)
	   {
	    /***** Get user *****/
	    row = mysql_fetch_row (mysql_res);

	    /* Get user's code (row[0]) */
	    FollowingUsrDat.UsrCod = Str_ConvertStrCodToLongCod (row[0]);

	    /***** Show user *****/
	    if ((NumUsr % Fol_NUM_COLUMNS_FOLLOW) == 0)
	       fprintf (Gbl.F.Out,"<tr>");
	    if (Usr_ChkUsrCodAndGetAllUsrDataFromUsrCod (&FollowingUsrDat,Usr_DONT_GET_PREFS))
	       Fol_ShowFollowedOrFollower (&FollowingUsrDat);
	    if ((NumUsr % Fol_NUM_COLUMNS_FOLLOW) == (Fol_NUM_COLUMNS_FOLLOW-1) ||
		NumUsr == NumUsrs - 1)
	       fprintf (Gbl.F.Out,"</tr>");
	   }

         /***** End table and box *****/
	 Box_EndBoxTable ();

	 /***** Free memory used for user's data *****/
	 Usr_UsrDataDestructor (&FollowingUsrDat);
	}

      /***** Free structure that stores the query result *****/
      DB_FreeMySQLResult (&mysql_res);
     }
   else
      Ale_ShowAlertUserNotFoundOrYouDoNotHavePermission ();
  }

/*****************************************************************************/
/******************************* List followers ******************************/
/*****************************************************************************/

void Fol_ListFollowers (void)
  {
   /***** Get user to view user he/she follows *****/
   Usr_GetParamOtherUsrCodEncryptedAndGetListIDs ();

   if (Gbl.Usrs.Other.UsrDat.UsrCod > 0)
     {
      if (Usr_ChkUsrCodAndGetAllUsrDataFromUsrCod (&Gbl.Usrs.Other.UsrDat,Usr_DONT_GET_PREFS))
	 Fol_ListFollowersUsr (&Gbl.Usrs.Other.UsrDat);
      else
         Ale_ShowAlertUserNotFoundOrYouDoNotHavePermission ();
     }
   else				// If user not specified, view my profile
      Fol_ListFollowersUsr (&Gbl.Usrs.Me.UsrDat);
  }

static void Fol_ListFollowersUsr (struct UsrData *UsrDat)
  {
   extern const char *Txt_Followers;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumUsrs;
   unsigned long NumUsr;
   struct UsrData FollowerUsrDat;
   bool ItsMe;

   /***** Show user's profile *****/
   if (Prf_ShowUserProfile (UsrDat))
     {
      /***** Check if a user is a follower of another user *****/
      NumUsrs = DB_QuerySELECT (&mysql_res,"can not get followers",
				"SELECT FollowerCod FROM usr_follow"
				" WHERE FollowedCod=%ld"
				" ORDER BY FollowTime DESC",
				UsrDat->UsrCod);

      if (NumUsrs)
	{
	 /***** Initialize structure with user's data *****/
	 Usr_UsrDataConstructor (&FollowerUsrDat);

         /***** Start box and table *****/
	 Box_StartBoxTable ("560px",Txt_Followers,NULL,
	                    NULL,Box_NOT_CLOSABLE,2);

	 for (NumUsr = 0;
	      NumUsr < NumUsrs;
	      NumUsr++)
	   {
	    /***** Get user and number of clicks *****/
	    row = mysql_fetch_row (mysql_res);

	    /* Get user's code (row[0]) */
	    FollowerUsrDat.UsrCod = Str_ConvertStrCodToLongCod (row[0]);

	    /***** Show user *****/
	    if ((NumUsr % Fol_NUM_COLUMNS_FOLLOW) == 0)
	       fprintf (Gbl.F.Out,"<tr>");
	    if (Usr_ChkUsrCodAndGetAllUsrDataFromUsrCod (&FollowerUsrDat,Usr_DONT_GET_PREFS))
	       Fol_ShowFollowedOrFollower (&FollowerUsrDat);
	    if ((NumUsr % Fol_NUM_COLUMNS_FOLLOW) == (Fol_NUM_COLUMNS_FOLLOW-1) ||
		NumUsr == NumUsrs - 1)
	       fprintf (Gbl.F.Out,"</tr>");
	   }

         /***** End table and box *****/
	 Box_EndBoxTable ();

	 /***** Free memory used for user's data *****/
	 Usr_UsrDataDestructor (&FollowerUsrDat);
	}

      /***** Free structure that stores the query result *****/
      DB_FreeMySQLResult (&mysql_res);

      /***** If it's me, mark possible notification as seen *****/
      ItsMe = Usr_ItsMe (UsrDat->UsrCod);
      if (ItsMe)
	 Ntf_MarkNotifAsSeen (Ntf_EVENT_FOLLOWER,
			      -1L,-1L,
			      Gbl.Usrs.Me.UsrDat.UsrCod);
     }
   else
      Ale_ShowAlertUserNotFoundOrYouDoNotHavePermission ();
  }

/*****************************************************************************/
/************************* Show followed or follower *************************/
/*****************************************************************************/

static void Fol_ShowFollowedOrFollower (struct UsrData *UsrDat)
  {
   extern const char *Txt_Another_user_s_profile;
   bool ShowPhoto;
   char PhotoURL[PATH_MAX + 1];
   bool Visible = Pri_ShowingIsAllowed (UsrDat->BaPrfVisibility,UsrDat);
   bool ItsMe = Usr_ItsMe (UsrDat->UsrCod);

   /***** Show user's photo *****/
   fprintf (Gbl.F.Out,"<td class=\"FOLLOW_PHOTO\">");
   if (Visible)
     {
      ShowPhoto = Pho_ShowingUsrPhotoIsAllowed (UsrDat,PhotoURL);
      Pho_ShowUsrPhoto (UsrDat,ShowPhoto ? PhotoURL :
					   NULL,
			"PHOTO60x80",Pho_ZOOM,false);
     }
   fprintf (Gbl.F.Out,"</td>");

   /***** Show user's name and icon to follow/unfollow *****/
   fprintf (Gbl.F.Out,"<td class=\"FOLLOW_USR\">");
   if (Visible)
     {
      /* Put form to go to public profile */
      Frm_StartForm (ActSeeOthPubPrf);
      Usr_PutParamUsrCodEncrypted (UsrDat->EncryptedUsrCod);
      fprintf (Gbl.F.Out,"<div class=\"FOLLOW_USR_NAME\">");	// Limited width
      Frm_LinkFormSubmit (Txt_Another_user_s_profile,"DAT",NULL);
      Usr_WriteFirstNameBRSurnames (UsrDat);
      fprintf (Gbl.F.Out,"</a>"
	                 "</div>");
      Frm_EndForm ();
     }

   ItsMe = Usr_ItsMe (UsrDat->UsrCod);
   if (!Gbl.Usrs.Me.Logged ||	// Not logged
       ItsMe)			// It's me
      /* Inactive icon to follow/unfollow */
      Fol_PutInactiveIconToFollowUnfollow ();
   else				// It's not me
     {
      /* Put form to follow / unfollow */
      if (Fol_CheckUsrIsFollowerOf (Gbl.Usrs.Me.UsrDat.UsrCod,UsrDat->UsrCod))	// I follow user
	 /* Form to unfollow */
	 Fol_PutIconToUnfollow (UsrDat);
      else if (Visible)	// I do not follow this user and I can follow
	 /* Form to follow */
	 Fol_PutIconToFollow (UsrDat);
     }
   fprintf (Gbl.F.Out,"</td>");
  }

/*****************************************************************************/
/********************* Write the name of a connected user ********************/
/*****************************************************************************/

static void Fol_WriteRowUsrToFollowOnRightColumn (struct UsrData *UsrDat)
  {
   extern const char *Txt_Another_user_s_profile;
   bool ShowPhoto;
   char PhotoURL[PATH_MAX + 1];
   bool Visible = Pri_ShowingIsAllowed (UsrDat->BaPrfVisibility,UsrDat);
   bool ItsMe = Usr_ItsMe (UsrDat->UsrCod);

   /***** Show user's photo *****/
   fprintf (Gbl.F.Out,"<tr>"
	              "<td class=\"CON_PHOTO COLOR%u\">",
	    Gbl.RowEvenOdd);
   if (Visible)
     {
      ShowPhoto = Pho_ShowingUsrPhotoIsAllowed (UsrDat,PhotoURL);
      Pho_ShowUsrPhoto (UsrDat,ShowPhoto ? PhotoURL :
					    NULL,
			"PHOTO21x28",Pho_ZOOM,false);
     }
   fprintf (Gbl.F.Out,"</td>");

   /***** User's name *****/
   fprintf (Gbl.F.Out,"<td class=\"CON_NAME_FOLLOW CON_CRS COLOR%u\">",
	    Gbl.RowEvenOdd);
   if (Visible)
     {
      /* Put form to go to public profile */
      Frm_StartForm (ActSeeOthPubPrf);
      Usr_PutParamUsrCodEncrypted (UsrDat->EncryptedUsrCod);
      fprintf (Gbl.F.Out,"<div class=\"CON_NAME_FOLLOW\">");	// Limited width
      Frm_LinkFormSubmit (Txt_Another_user_s_profile,"CON_CRS",NULL);
      Usr_WriteFirstNameBRSurnames (UsrDat);
      fprintf (Gbl.F.Out,"</a>"
	                 "</div>");
      Frm_EndForm ();
     }
   fprintf (Gbl.F.Out,"</td>");

   /***** Icon to follow *****/
   fprintf (Gbl.F.Out,"<td class=\"CON_ICON_FOLLOW RIGHT_MIDDLE COLOR%u\">",
            Gbl.RowEvenOdd);
   if (!Gbl.Usrs.Me.Logged ||	// Not logged
       ItsMe)			// It's me
      /* Inactive icon to follow/unfollow */
      Fol_PutInactiveIconToFollowUnfollow ();
   else				// It's not me
     {
      /* Put form to follow / unfollow */
      if (Fol_CheckUsrIsFollowerOf (Gbl.Usrs.Me.UsrDat.UsrCod,UsrDat->UsrCod))	// I follow user
	 /* Form to unfollow */
	 Fol_PutIconToUnfollow (UsrDat);
      else if (Visible)	// I do not follow this user and I can follow
	 /* Form to follow */
	 Fol_PutIconToFollow (UsrDat);
     }
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");

   Gbl.RowEvenOdd = 1 - Gbl.RowEvenOdd;
  }

/*****************************************************************************/
/*********************** Put icon to unfollow another user *********************/
/*****************************************************************************/

static void Fol_PutInactiveIconToFollowUnfollow (void)
  {
   /***** Inactive icon to follow/unfollow *****/
   fprintf (Gbl.F.Out,"<div class=\"FOLLOW_USR_ICO ICO_HIDDEN\">"
		      "<img src=\"%s/user.svg\""
		      " alt=\"\""
		      " class=\"ICO16x16\" />"
		      "</div>",
	    Cfg_URL_ICON_PUBLIC);
   }

/*****************************************************************************/
/*********************** Put icon to unfollow another user *********************/
/*****************************************************************************/

static void Fol_PutIconToFollow (struct UsrData *UsrDat)
  {
   extern const char *Txt_Follow;

   /***** Form to unfollow *****/
   Frm_StartForm (ActFolUsr);
   Usr_PutParamUsrCodEncrypted (UsrDat->EncryptedUsrCod);
   Frm_LinkFormSubmit (Txt_Follow,NULL,NULL);
   fprintf (Gbl.F.Out,"<div class=\"FOLLOW_USR_ICO ICO_HIGHLIGHT\">"
		      "<img src=\"%s/user-plus.svg\""
		      " alt=\"%s\" title=\"%s\""
		      " class=\"ICO16x16\" />"
		      "</div>"
		      "</a>",
	    Cfg_URL_ICON_PUBLIC,
	    Txt_Follow,Txt_Follow);
   Frm_EndForm ();
  }

/*****************************************************************************/
/********************** Put icon to unfollow another user ********************/
/*****************************************************************************/

static void Fol_PutIconToUnfollow (struct UsrData *UsrDat)
  {
   extern const char *Txt_Unfollow;

   /* Form to follow */
   Frm_StartForm (ActUnfUsr);
   Usr_PutParamUsrCodEncrypted (UsrDat->EncryptedUsrCod);
   Frm_LinkFormSubmit (Txt_Unfollow,NULL,NULL);
   fprintf (Gbl.F.Out,"<div class=\"FOLLOW_USR_ICO ICO_HIGHLIGHT\">"
		      "<img src=\"%s/user-check.svg\""
		      " alt=\"%s\" title=\"%s\""
		      " class=\"ICO16x16\" />"
		      "</div>"
		      "</a>",
	    Cfg_URL_ICON_PUBLIC,
	    Txt_Unfollow,Txt_Unfollow);
   Frm_EndForm ();
  }

/*****************************************************************************/
/***************************** Follow another user ***************************/
/*****************************************************************************/

void Fol_FollowUsr1 (void)
  {
   /***** Get user to be followed *****/
   if (Usr_GetParamOtherUsrCodEncryptedAndGetUsrData ())
     {
      // Follow only if I do not follow him/her
      if (!Fol_CheckUsrIsFollowerOf (Gbl.Usrs.Me.UsrDat.UsrCod,
				     Gbl.Usrs.Other.UsrDat.UsrCod))
	 Fol_FollowUsr (&Gbl.Usrs.Other.UsrDat);

      Ale_CreateAlert (Ale_SUCCESS,NULL,"");	// Txt not used
     }
   else
      Ale_CreateAlertUserNotFoundOrYouDoNotHavePermission ();
  }

void Fol_FollowUsr2 (void)
  {
   if (Ale_GetTypeOfLastAlert () == Ale_SUCCESS)
     {
      /***** Show user's profile again *****/
      if (!Prf_ShowUserProfile (&Gbl.Usrs.Other.UsrDat))
	 /* 1) I had permission to follow the user and I've just follow him/her
	    2) User restricted permission, so now I can not view his/her profile
	    3) Now I can not view his/her profile ==> show users I follow */
	 Fol_ListFollowingUsr (&Gbl.Usrs.Me.UsrDat);		// List users I follow
     }
   else
      Ale_ShowAlerts (NULL);
  }

/*****************************************************************************/
/***************************** Unfollow another user *************************/
/*****************************************************************************/

void Fol_UnfollowUsr1 (void)
  {
   /***** Get user to be unfollowed *****/
   if (Usr_GetParamOtherUsrCodEncryptedAndGetUsrData ())
     {
      // Unfollow only if I follow him/her
      if (Fol_CheckUsrIsFollowerOf (Gbl.Usrs.Me.UsrDat.UsrCod,
                                    Gbl.Usrs.Other.UsrDat.UsrCod))
	 Fol_UnfollowUsr (&Gbl.Usrs.Other.UsrDat);

      Ale_CreateAlert (Ale_SUCCESS,NULL,"");	// Txt not used
     }
   else
      Ale_CreateAlertUserNotFoundOrYouDoNotHavePermission ();
  }

void Fol_UnfollowUsr2 (void)
  {
   /***** Get user to be unfollowed *****/
   if (Ale_GetTypeOfLastAlert () == Ale_SUCCESS)
     {
      /***** Show user's profile again *****/
      if (!Prf_ShowUserProfile (&Gbl.Usrs.Other.UsrDat))	// I can not view user's profile
	 /* 1) I followed a user when I had permission
	    2) User restricted permission, so now I can not view his/her profile
	    3) Now I can not view his/her profile ==> show users I follow */
	 Fol_ListFollowingUsr (&Gbl.Usrs.Me.UsrDat);		// List users I follow
     }
   else
      Ale_ShowAlerts (NULL);
  }

/*****************************************************************************/
/***************** Request follow/unfollow several users *********************/
/*****************************************************************************/

void Fol_RequestFollowStds (void)
  {
   Fol_RequestFollowUsrs (ActFolSevStd,Fol_PutParamsFollowSelectedStds);
  }

void Fol_RequestFollowTchs (void)
  {
   Fol_RequestFollowUsrs (ActFolSevTch,Fol_PutParamsFollowSelectedTchs);
  }

static void Fol_RequestFollowUsrs (Act_Action_t NextAction,void (*FuncParams) ())
  {
   extern const char *Txt_Follow;
   extern const char *Txt_Do_you_want_to_follow_the_selected_user_whom_you_do_not_follow_yet;
   extern const char *Txt_Do_you_want_to_follow_the_X_selected_users_whom_you_do_not_follow_yet;
   unsigned NumFollowed;
   unsigned NumNotFollowed;

   // List of selected users is already got

   /***** Go through list of selected users
          getting the number of followed and not followed ****/
   Fol_GetFollowedFromSelectedUsrs (&NumFollowed,&NumNotFollowed);

   /***** Show question to confirm ****/
   if (NumNotFollowed)
     {
      if (NumNotFollowed == 1)
         Ale_ShowAlertAndButton (NextAction,NULL,NULL,
				 FuncParams,
				 Btn_CREATE_BUTTON,Txt_Follow,
				 Ale_QUESTION,Txt_Do_you_want_to_follow_the_selected_user_whom_you_do_not_follow_yet);
      else
         Ale_ShowAlertAndButton (NextAction,NULL,NULL,
				 FuncParams,
				 Btn_CREATE_BUTTON,Txt_Follow,
				 Ale_QUESTION,Txt_Do_you_want_to_follow_the_X_selected_users_whom_you_do_not_follow_yet,
				 NumNotFollowed);
     }

   /***** Free memory used by list of selected users' codes *****/
   Usr_FreeListsSelectedUsrsCods ();
  }

void Fol_RequestUnfollowStds (void)
  {
   Fol_RequestUnfollowUsrs (ActUnfSevStd,Fol_PutParamsUnfollowSelectedStds);
  }

void Fol_RequestUnfollowTchs (void)
  {
   Fol_RequestUnfollowUsrs (ActUnfSevTch,Fol_PutParamsUnfollowSelectedTchs);
  }

static void Fol_RequestUnfollowUsrs (Act_Action_t NextAction,void (*FuncParams) ())
  {
   extern const char *Txt_Do_you_want_to_stop_following_the_selected_user_whom_you_follow;
   extern const char *Txt_Do_you_want_to_stop_following_the_X_selected_users_whom_you_follow;
   extern const char *Txt_Unfollow;
   unsigned NumFollowed;
   unsigned NumNotFollowed;

   // List of selected users is already got

   /***** Go through list of selected users
          getting the number of followed and not followed ****/
   Fol_GetFollowedFromSelectedUsrs (&NumFollowed,&NumNotFollowed);

   /***** Show question to confirm ****/
   if (NumFollowed)
     {
      if (NumFollowed == 1)
         Ale_ShowAlertAndButton (NextAction,NULL,NULL,
				 FuncParams,
				 Btn_CREATE_BUTTON,Txt_Unfollow,
				 Ale_QUESTION,Txt_Do_you_want_to_stop_following_the_selected_user_whom_you_follow);
      else
         Ale_ShowAlertAndButton (NextAction,NULL,NULL,
				 FuncParams,
				 Btn_CREATE_BUTTON,Txt_Unfollow,
				 Ale_QUESTION,Txt_Do_you_want_to_stop_following_the_X_selected_users_whom_you_follow,
				 NumFollowed);
     }

   /***** Free memory used by list of selected users' codes *****/
   Usr_FreeListsSelectedUsrsCods ();
  }

/*****************************************************************************/
/**** Go through the list getting the number of followed and not followed ****/
/*****************************************************************************/

static void Fol_GetFollowedFromSelectedUsrs (unsigned *NumFollowed,
                                             unsigned *NumNotFollowed)
  {
   extern const char *Txt_Select_users_X_Followed_Y_Not_followed_Z;
   struct UsrData UsrDat;
   const char *Ptr;
   bool IFollowUsr;
   unsigned NumUsrs = 0;

   /***** Initialize structure with user's data *****/
   Usr_UsrDataConstructor (&UsrDat);

   /***** Check users to know if I follow them *****/
   *NumFollowed = 0;
   Ptr = Gbl.Usrs.Selected.List[Rol_UNK];
   while (*Ptr)
     {
      Par_GetNextStrUntilSeparParamMult (&Ptr,UsrDat.EncryptedUsrCod,
                                         Cry_BYTES_ENCRYPTED_STR_SHA256_BASE64);
      Usr_GetUsrCodFromEncryptedUsrCod (&UsrDat);
      if (Gbl.Usrs.Me.UsrDat.UsrCod != UsrDat.UsrCod)		// Skip me
	 if (Usr_ChkUsrCodAndGetAllUsrDataFromUsrCod (&UsrDat,Usr_DONT_GET_PREFS))	// Get from the database the data of the student
	    if (Usr_CheckIfUsrBelongsToCurrentCrs (&UsrDat))
	      {
	       /* Check if I follow this user */
	       IFollowUsr = Fol_CheckUsrIsFollowerOf (Gbl.Usrs.Me.UsrDat.UsrCod,
						      UsrDat.UsrCod);

	       /* Update number of users */
	       if (IFollowUsr)
		  (*NumFollowed)++;
	       NumUsrs++;
	      }
     }

   /***** Free memory used for user's data *****/
   Usr_UsrDataDestructor (&UsrDat);

   /***** Show alert ****/
   *NumNotFollowed = NumUsrs - *NumFollowed;
   Ale_ShowAlert (Ale_INFO,Txt_Select_users_X_Followed_Y_Not_followed_Z,
	          NumUsrs,*NumFollowed,*NumNotFollowed);
  }

/*****************************************************************************/
/********************** Follow/unfollow several users ************************/
/*****************************************************************************/

void Fol_FollowUsrs ()
  {
   extern const char *Txt_You_have_followed_one_user;
   extern const char *Txt_You_have_followed_X_users;
   const char *Ptr;
   struct UsrData UsrDat;
   unsigned NumFollowed = 0;

   /***** Get list of selected users if not already got *****/
   Usr_GetListsSelectedUsrsCods ();

   /***** Initialize structure with user's data *****/
   Usr_UsrDataConstructor (&UsrDat);

   /***** Check users to know if I follow them *****/
   Ptr = Gbl.Usrs.Selected.List[Rol_UNK];
   while (*Ptr)
     {
      Par_GetNextStrUntilSeparParamMult (&Ptr,UsrDat.EncryptedUsrCod,
                                         Cry_BYTES_ENCRYPTED_STR_SHA256_BASE64);
      Usr_GetUsrCodFromEncryptedUsrCod (&UsrDat);
      if (Gbl.Usrs.Me.UsrDat.UsrCod != UsrDat.UsrCod)		// Skip me
	 if (Usr_ChkUsrCodAndGetAllUsrDataFromUsrCod (&UsrDat,Usr_DONT_GET_PREFS))	// Get from the database the data of the student
	    if (Usr_CheckIfUsrBelongsToCurrentCrs (&UsrDat))
	       /* If I don't follow this user ==> follow him/her */
	       if (!Fol_CheckUsrIsFollowerOf (Gbl.Usrs.Me.UsrDat.UsrCod,
					      UsrDat.UsrCod))
		 {
		  Fol_FollowUsr (&UsrDat);
		  NumFollowed++;
		 }
     }

   /***** Free memory used for user's data *****/
   Usr_UsrDataDestructor (&UsrDat);

   /***** Free memory used by list of selected users' codes *****/
   Usr_FreeListsSelectedUsrsCods ();

   /***** Show alert *****/
   if (NumFollowed == 1)
      Ale_ShowAlert (Ale_SUCCESS,Txt_You_have_followed_one_user);
   else
      Ale_ShowAlert (Ale_SUCCESS,Txt_You_have_followed_X_users,
	             NumFollowed);
  }

void Fol_UnfollowUsrs (void)
  {
   extern const char *Txt_You_have_stopped_following_one_user;
   extern const char *Txt_You_have_stopped_following_X_users;
   const char *Ptr;
   struct UsrData UsrDat;
   unsigned NumUnfollowed = 0;

   /***** Get list of selected users if not already got *****/
   Usr_GetListsSelectedUsrsCods ();

   /***** Initialize structure with user's data *****/
   Usr_UsrDataConstructor (&UsrDat);

   /***** Check users to know if I follow them *****/
   Ptr = Gbl.Usrs.Selected.List[Rol_UNK];
   while (*Ptr)
     {
      Par_GetNextStrUntilSeparParamMult (&Ptr,UsrDat.EncryptedUsrCod,
                                         Cry_BYTES_ENCRYPTED_STR_SHA256_BASE64);
      Usr_GetUsrCodFromEncryptedUsrCod (&UsrDat);
      if (Gbl.Usrs.Me.UsrDat.UsrCod != UsrDat.UsrCod)		// Skip me
	 if (Usr_ChkUsrCodAndGetAllUsrDataFromUsrCod (&UsrDat,Usr_DONT_GET_PREFS))	// Get from the database the data of the student
	    if (Usr_CheckIfUsrBelongsToCurrentCrs (&UsrDat))
	       /* If I follow this user ==> unfollow him/her */
	       if (Fol_CheckUsrIsFollowerOf (Gbl.Usrs.Me.UsrDat.UsrCod,
					     UsrDat.UsrCod))
		 {
		  Fol_UnfollowUsr (&UsrDat);
		  NumUnfollowed++;
		 }
     }

   /***** Free memory used for user's data *****/
   Usr_UsrDataDestructor (&UsrDat);

   /***** Free memory used by list of selected users' codes *****/
   Usr_FreeListsSelectedUsrsCods ();

   /***** Show alert *****/
   if (NumUnfollowed == 1)
      Ale_ShowAlert (Ale_SUCCESS,Txt_You_have_stopped_following_one_user);
   else
      Ale_ShowAlert (Ale_SUCCESS,Txt_You_have_stopped_following_X_users,
	             NumUnfollowed);
  }

/*****************************************************************************/
/**************** Put parameter with list of selected users ******************/
/*****************************************************************************/

static void Fol_PutParamsFollowSelectedStds (void)
  {
   /***** Hidden parameter with the encrypted codes of users selected *****/
   Usr_PutHiddenParUsrCodAll (ActFolSevStd,Gbl.Usrs.Selected.List[Rol_UNK]);
  }

static void Fol_PutParamsFollowSelectedTchs (void)
  {
   /***** Hidden parameter with the encrypted codes of users selected *****/
   Usr_PutHiddenParUsrCodAll (ActFolSevTch,Gbl.Usrs.Selected.List[Rol_UNK]);
  }

static void Fol_PutParamsUnfollowSelectedStds (void)
  {
   /***** Hidden parameter with the encrypted codes of users selected *****/
   Usr_PutHiddenParUsrCodAll (ActUnfSevStd,Gbl.Usrs.Selected.List[Rol_UNK]);
  }

static void Fol_PutParamsUnfollowSelectedTchs (void)
  {
   /***** Hidden parameter with the encrypted codes of users selected *****/
   Usr_PutHiddenParUsrCodAll (ActUnfSevTch,Gbl.Usrs.Selected.List[Rol_UNK]);
  }

/*****************************************************************************/
/******************************** Follow user ********************************/
/*****************************************************************************/

static void Fol_FollowUsr (struct UsrData *UsrDat)
  {
   bool CreateNotif;
   bool NotifyByEmail;

   /***** Avoid wrong cases *****/
   if (Gbl.Usrs.Me.UsrDat.UsrCod <= 0 ||
       UsrDat->UsrCod <= 0            ||
       Gbl.Usrs.Me.UsrDat.UsrCod == UsrDat->UsrCod)	// Skip me
      return;

   /***** Follow user in database *****/
   DB_QueryREPLACE ("can not follow user",
		    "REPLACE INTO usr_follow"
		    " (FollowerCod,FollowedCod,FollowTime)"
		    " VALUES"
		    " (%ld,%ld,NOW())",
		    Gbl.Usrs.Me.UsrDat.UsrCod,
		    UsrDat->UsrCod);

   /***** Flush cache *****/
   Fol_FlushCacheFollow ();

   /***** This follow must be notified by email? *****/
   CreateNotif = (UsrDat->NtfEvents.CreateNotif & (1 << Ntf_EVENT_FOLLOWER));
   NotifyByEmail = CreateNotif &&
		   (UsrDat->NtfEvents.SendEmail & (1 << Ntf_EVENT_FOLLOWER));

   /***** Create notification for this followed.
	  If this followed wants to receive notifications by email,
	  activate the sending of a notification *****/
   if (CreateNotif)
      Ntf_StoreNotifyEventToOneUser (Ntf_EVENT_FOLLOWER,UsrDat,Gbl.Usrs.Me.UsrDat.UsrCod,
				     (Ntf_Status_t) (NotifyByEmail ? Ntf_STATUS_BIT_EMAIL :
								     0));
  }

/*****************************************************************************/
/******************************* Unfollow user *******************************/
/*****************************************************************************/

static void Fol_UnfollowUsr (struct UsrData *UsrDat)
  {
   /***** Avoid wrong cases *****/
   if (Gbl.Usrs.Me.UsrDat.UsrCod <= 0 ||
       UsrDat->UsrCod <= 0            ||
       Gbl.Usrs.Me.UsrDat.UsrCod == UsrDat->UsrCod)	// Skip me
      return;

   /***** Unfollow user in database *****/
   DB_QueryDELETE ("can not unfollow user",
		   "DELETE FROM usr_follow"
		   " WHERE FollowerCod=%ld AND FollowedCod=%ld",
		   Gbl.Usrs.Me.UsrDat.UsrCod,
		   UsrDat->UsrCod);

   /***** Flush cache *****/
   Fol_FlushCacheFollow ();
  }

/*****************************************************************************/
/****** Get and show ranking of users attending to number of followers *******/
/*****************************************************************************/

void Fol_GetAndShowRankingFollowers (void)
  {
   MYSQL_RES *mysql_res;
   unsigned NumUsrs = 0;	// Initialized to avoid warning

   /***** Get ranking from database *****/
   switch (Gbl.Scope.Current)
     {
      case Hie_SYS:
	 NumUsrs =
         (unsigned) DB_QuerySELECT (&mysql_res,"can not get ranking",
				    "SELECT FollowedCod,COUNT(FollowerCod) AS N"
				    " FROM usr_follow"
				    " GROUP BY FollowedCod"
				    " ORDER BY N DESC,FollowedCod LIMIT 100");
         break;
      case Hie_CTY:
         NumUsrs =
         (unsigned) DB_QuerySELECT (&mysql_res,"can not get ranking",
				    "SELECT usr_follow.FollowedCod,COUNT(DISTINCT usr_follow.FollowerCod) AS N"
				    " FROM institutions,centres,degrees,courses,crs_usr,usr_follow"
				    " WHERE institutions.CtyCod=%ld"
				    " AND institutions.InsCod=centres.InsCod"
				    " AND centres.CtrCod=degrees.CtrCod"
				    " AND degrees.DegCod=courses.DegCod"
				    " AND courses.CrsCod=crs_usr.CrsCod"
				    " AND crs_usr.UsrCod=usr_follow.FollowedCod"
				    " GROUP BY usr_follow.FollowedCod"
				    " ORDER BY N DESC,usr_follow.FollowedCod LIMIT 100",
				    Gbl.Hierarchy.Cty.CtyCod);
         break;
      case Hie_INS:
         NumUsrs =
         (unsigned) DB_QuerySELECT (&mysql_res,"can not get ranking",
				    "SELECT usr_follow.FollowedCod,COUNT(DISTINCT usr_follow.FollowerCod) AS N"
				    " FROM centres,degrees,courses,crs_usr,usr_follow"
				    " WHERE centres.InsCod=%ld"
				    " AND centres.CtrCod=degrees.CtrCod"
				    " AND degrees.DegCod=courses.DegCod"
				    " AND courses.CrsCod=crs_usr.CrsCod"
				    " AND crs_usr.UsrCod=usr_follow.FollowedCod"
				    " GROUP BY usr_follow.FollowedCod"
				    " ORDER BY N DESC,usr_follow.FollowedCod LIMIT 100",
				    Gbl.Hierarchy.Ins.InsCod);
         break;
      case Hie_CTR:
         NumUsrs =
         (unsigned) DB_QuerySELECT (&mysql_res,"can not get ranking",
				    "SELECT usr_follow.FollowedCod,COUNT(DISTINCT usr_follow.FollowerCod) AS N"
				    " FROM degrees,courses,crs_usr,usr_follow"
				    " WHERE degrees.CtrCod=%ld"
				    " AND degrees.DegCod=courses.DegCod"
				    " AND courses.CrsCod=crs_usr.CrsCod"
				    " AND crs_usr.UsrCod=usr_follow.FollowedCod"
				    " GROUP BY usr_follow.FollowedCod"
				    " ORDER BY N DESC,usr_follow.FollowedCod LIMIT 100",
				    Gbl.Hierarchy.Ctr.CtrCod);
         break;
      case Hie_DEG:
         NumUsrs =
         (unsigned) DB_QuerySELECT (&mysql_res,"can not get ranking",
				    "SELECT usr_follow.FollowedCod,COUNT(DISTINCT usr_follow.FollowerCod) AS N"
				    " FROM courses,crs_usr,usr_follow"
				    " WHERE courses.DegCod=%ld"
				    " AND courses.CrsCod=crs_usr.CrsCod"
				    " AND crs_usr.UsrCod=usr_follow.FollowedCod"
				    " GROUP BY usr_follow.FollowedCod"
				    " ORDER BY N DESC,usr_follow.FollowedCod LIMIT 100",
				    Gbl.Hierarchy.Deg.DegCod);
         break;
      case Hie_CRS:
         NumUsrs =
         (unsigned) DB_QuerySELECT (&mysql_res,"can not get ranking",
				    "SELECT usr_follow.FollowedCod,COUNT(DISTINCT usr_follow.FollowerCod) AS N"
				    " FROM crs_usr,usr_follow"
				    " WHERE crs_usr.CrsCod=%ld"
				    " AND crs_usr.UsrCod=usr_follow.FollowedCod"
				    " GROUP BY usr_follow.FollowedCod"
				    " ORDER BY N DESC,usr_follow.FollowedCod LIMIT 100",
				    Gbl.Hierarchy.Crs.Crs.CrsCod);
         break;
      default:
         Lay_WrongScopeExit ();
         break;
     }

   Prf_ShowRankingFigure (&mysql_res,NumUsrs);
  }

/*****************************************************************************/
/********************* Get notification of a new follower ********************/
/*****************************************************************************/
// This function may be called inside a web service, so don't report error

void Fol_GetNotifFollower (char SummaryStr[Ntf_MAX_BYTES_SUMMARY + 1],
                           char **ContentStr)
  {
   SummaryStr[0] = '\0';	// Return nothing on error

   if ((*ContentStr = (char *) malloc (1)))
      *ContentStr[0] = '\0';
  }

/*****************************************************************************/
/*********************** Remove user from user follow ************************/
/*****************************************************************************/

void Fol_RemoveUsrFromUsrFollow (long UsrCod)
  {
   DB_QueryDELETE ("can not remove user from followers and followed",
		   "DELETE FROM usr_follow"
		   " WHERE FollowerCod=%ld OR FollowedCod=%ld",
	           UsrCod,UsrCod);

   /***** Flush cache *****/
   Fol_FlushCacheFollow ();
  }
