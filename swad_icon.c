// swad_icon.c: icons

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
#include <stdio.h>		// For asprintf, fprintf, etc.
#include <string.h>

#include "swad_box.h"
#include "swad_config.h"
#include "swad_database.h"
#include "swad_form.h"
#include "swad_global.h"
#include "swad_icon.h"
#include "swad_layout.h"
#include "swad_parameter.h"
#include "swad_preference.h"

/*****************************************************************************/
/************** External global variables from others modules ****************/
/*****************************************************************************/

extern struct Globals Gbl;

/*****************************************************************************/
/******************************** Private constants **************************/
/*****************************************************************************/

#define Ico_MAX_BYTES_ICON_SET_ID 16

const char *Ico_IconSetId[Ico_NUM_ICON_SETS] =
  {
   "awesome",
   "nuvola",
  };

const char *Ico_IconSetNames[Ico_NUM_ICON_SETS] =
  {
   "Awesome",
   "Nuvola",
  };

/*****************************************************************************/
/***************************** Private prototypes ****************************/
/*****************************************************************************/

static void Ico_PutIconsIconSet (void);

/*****************************************************************************/
/*********** Get icon with extension from icon without extension *************/
/*****************************************************************************/

#define Ico_NUM_ICON_EXTENSIONS 3

const char *Ico_GetIcon (const char *IconWithoutExtension)
  {
   static const char *Ico_IconExtensions[Ico_NUM_ICON_EXTENSIONS] =
     {	// In order of preference
      "svg",
      "png",
      "gif",
     };
   static char IconWithExtension[NAME_MAX + 1];
   char PathIcon[PATH_MAX + 1];
   unsigned NumExt;

   for (NumExt = 0;
        NumExt < Ico_NUM_ICON_EXTENSIONS;
        NumExt++)
     {
      snprintf (IconWithExtension,sizeof (IconWithExtension),
		"%s.%s",
		IconWithoutExtension,Ico_IconExtensions[NumExt]);
      snprintf (PathIcon,sizeof (PathIcon),
		"%s/%s/%s/%s/%s",
		Cfg_PATH_SWAD_PUBLIC,Cfg_FOLDER_PUBLIC_ICON,
		Cfg_ICON_FOLDER_ICON_SETS,Ico_IconSetId[Gbl.Prefs.IconSet],
		IconWithExtension);
      if (Fil_CheckIfPathExists (PathIcon))
	 return IconWithExtension;
     }

   return "default.svg";
  }

/*****************************************************************************/
/************************ Put icons to select a IconSet **********************/
/*****************************************************************************/

void Ico_PutIconsToSelectIconSet (void)
  {
   extern const char *Hlp_PROFILE_Preferences_icons;
   extern const char *Txt_Icons;
   Ico_IconSet_t IconSet;

   Box_StartBox (NULL,Txt_Icons,Ico_PutIconsIconSet,
                 Hlp_PROFILE_Preferences_icons,Box_NOT_CLOSABLE);
   fprintf (Gbl.F.Out,"<div class=\"PREF_CONTAINER\">");
   for (IconSet = (Ico_IconSet_t) 0;
	IconSet < Ico_NUM_ICON_SETS;
	IconSet++)
     {
      fprintf (Gbl.F.Out,"<div class=\"%s\">",
               IconSet == Gbl.Prefs.IconSet ? "PREF_ON" :
        	                              "PREF_OFF");
      Frm_StartForm (ActChgIco);
      Par_PutHiddenParamString ("IconSet",Ico_IconSetId[IconSet]);
      fprintf (Gbl.F.Out,"<input type=\"image\" src=\"%s/%s/%s/heart64x64.gif\""
	                 " alt=\"%s\" title=\"%s\" class=\"ICO25x25\" />",
               Gbl.Prefs.URLIcons,
               Cfg_ICON_FOLDER_ICON_SETS,
               Ico_IconSetId[IconSet],
               Ico_IconSetNames[IconSet],
               Ico_IconSetNames[IconSet]);
      Frm_EndForm ();
      fprintf (Gbl.F.Out,"</div>");
     }
   fprintf (Gbl.F.Out,"</div>");
   Box_EndBox ();
  }

/*****************************************************************************/
/*************** Put contextual icons in icon-set preference *****************/
/*****************************************************************************/

static void Ico_PutIconsIconSet (void)
  {
   /***** Put icon to show a figure *****/
   Gbl.Stat.FigureType = Sta_ICON_SETS;
   Sta_PutIconToShowFigure ();
  }

/*****************************************************************************/
/***************************** Change icon set *******************************/
/*****************************************************************************/

void Ico_ChangeIconSet (void)
  {
   /***** Get param with icon set *****/
   Gbl.Prefs.IconSet = Ico_GetParamIconSet ();
   snprintf (Gbl.Prefs.URLIconSet,sizeof (Gbl.Prefs.URLIconSet),
	     "%s/%s/%s/%s",
             Cfg_URL_SWAD_PUBLIC,Cfg_FOLDER_PUBLIC_ICON,
             Cfg_ICON_FOLDER_ICON_SETS,
             Ico_IconSetId[Gbl.Prefs.IconSet]);

   /***** Store icon set in database *****/
   if (Gbl.Usrs.Me.Logged)
      DB_QueryUPDATE ("can not update your preference about icon set",
		      "UPDATE usr_data SET IconSet='%s' WHERE UsrCod=%ld",
		      Ico_IconSetId[Gbl.Prefs.IconSet],
		      Gbl.Usrs.Me.UsrDat.UsrCod);

   /***** Set preferences from current IP *****/
   Pre_SetPrefsFromIP ();
  }

/*****************************************************************************/
/*********************** Get parameter with icon set *************************/
/*****************************************************************************/

Ico_IconSet_t Ico_GetParamIconSet (void)
  {
   char IconSetId[Ico_MAX_BYTES_ICON_SET_ID + 1];
   Ico_IconSet_t IconSet;

   Par_GetParToText ("IconSet",IconSetId,Ico_MAX_BYTES_ICON_SET_ID);
   for (IconSet = (Ico_IconSet_t) 0;
	IconSet < Ico_NUM_ICON_SETS;
	IconSet++)
      if (!strcmp (IconSetId,Ico_IconSetId[IconSet]))
         return IconSet;

   return Ico_ICON_SET_DEFAULT;
  }

/*****************************************************************************/
/************************* Get icon set from string **************************/
/*****************************************************************************/

Ico_IconSet_t Ico_GetIconSetFromStr (const char *Str)
  {
   Ico_IconSet_t IconSet;

   for (IconSet = (Ico_IconSet_t) 0;
	IconSet < Ico_NUM_ICON_SETS;
	IconSet++)
      if (!strcasecmp (Str,Ico_IconSetId[IconSet]))
	 return IconSet;

   return Ico_ICON_SET_DEFAULT;
  }

/*****************************************************************************/
/***** Show contextual icons to remove, edit, view, hide, unhide, print ******/
/*****************************************************************************/

void Ico_PutContextualIconToRemove (Act_Action_t NextAction,void (*FuncParams) ())
  {
   extern const char *Txt_Remove;

   Lay_PutContextualLink (NextAction,NULL,FuncParams,
                          "trash.svg",
                          Txt_Remove,NULL,
                          NULL);
  }

void Ico_PutContextualIconToEdit (Act_Action_t NextAction,void (*FuncParams) ())
  {
   extern const char *Txt_Edit;

   Lay_PutContextualLink (NextAction,NULL,FuncParams,
                          "pen.svg",
                          Txt_Edit,NULL,
                          NULL);
  }

void Ico_PutContextualIconToViewFiles (Act_Action_t NextAction,void (*FuncParams) ())
  {
   extern const char *Txt_Files;

   Lay_PutContextualLink (NextAction,NULL,FuncParams,
			  "folder64x64.gif",
			  Txt_Files,NULL,
                          NULL);
  }

void Ico_PutContextualIconToView (Act_Action_t NextAction,void (*FuncParams) ())
  {
   extern const char *Txt_View;

   Lay_PutContextualLink (NextAction,NULL,FuncParams,
			  "eye-on64x64.png",
			  Txt_View,NULL,
                          NULL);
  }

void Ico_PutContextualIconToHide (Act_Action_t NextAction,void (*FuncParams) ())
  {
   extern const char *Txt_Hide;

   Lay_PutContextualLink (NextAction,NULL,FuncParams,
                          "eye-on64x64.png",
                          Txt_Hide,NULL,
                          NULL);
  }

void Ico_PutContextualIconToUnhide (Act_Action_t NextAction,void (*FuncParams) ())
  {
   extern const char *Txt_Show;

   Lay_PutContextualLink (NextAction,NULL,FuncParams,
                          "eye-slash-on64x64.png",
                          Txt_Show,NULL,
                          NULL);
  }

void Ico_PutContextualIconToPrint (Act_Action_t NextAction,void (*FuncParams) ())
  {
   extern const char *Txt_Print;

   Lay_PutContextualLink (NextAction,NULL,FuncParams,
                          "print64x64.png",
                          Txt_Print,NULL,
                          NULL);
  }

/*****************************************************************************/
/****************** Show an icon with a link (without text) ******************/
/*****************************************************************************/

void Ico_PutIconLink (const char *Icon,const char *Title,const char *Text,
                      const char *LinkStyle,const char *OnSubmit)
  {
   Frm_LinkFormSubmit (Title,LinkStyle,OnSubmit);
   if (Text)
      Ico_PutIconWithText (Icon,Title,Text);
   else
      fprintf (Gbl.F.Out,"<img src=\"%s/%s\" alt=\"%s\" title=\"%s\""
			 " class=\"CONTEXT_OPT ICO_HIGHLIGHT ICO16x16\" />",
	       Gbl.Prefs.URLIcons,Icon,Title,Title);
   fprintf (Gbl.F.Out,"</a>");
  }

/*****************************************************************************/
/********************** Put an inactive/disabled icon ************************/
/*****************************************************************************/

void Ico_PutIconOff (const char *Icon,const char *Alt)
  {
   fprintf (Gbl.F.Out,"<img src=\"%s/%s\" alt=\"%s\" title=\"%s\""
	              " class=\"CONTEXT_OPT ICO_HIDDEN ICO16x16\" />",
            Gbl.Prefs.URLIcons,Icon,Alt,Alt);
  }

/*****************************************************************************/
/**************** Put a icon with a text to submit a form ********************/
/*****************************************************************************/

void Ico_PutIconWithText (const char *Icon,const char *Alt,const char *Text)
  {
   /***** Print icon and optional text *****/
   fprintf (Gbl.F.Out,"<div class=\"CONTEXT_OPT ICO_HIGHLIGHT\">"
	              "<img src=\"%s/%s\" alt=\"%s\" title=\"%s\""
	              " class=\"ICO16x16\" />",
            Gbl.Prefs.URLIcons,Icon,Alt,Text ? Text :
        	                               Alt);
   if (Text)
      if (Text[0])
	 fprintf (Gbl.F.Out,"&nbsp;%s",Text);
   fprintf (Gbl.F.Out,"</div>");
  }

/*****************************************************************************/
/********** Put a icon to submit a form.                            **********/
/********** When clicked, the icon will be replaced by an animation **********/
/*****************************************************************************/

void Ico_PutCalculateIcon (const char *Alt)
  {
   fprintf (Gbl.F.Out,"<div class=\"CONTEXT_OPT ICO_HIGHLIGHT\">"
	              "<img id=\"update_%d\" src=\"%s/recycle16x16.gif\""	// TODO: change name and resolution to refresh64x64.png
	              " alt=\"%s\" title=\"%s\""
		      " class=\"ICO20x20\" />"
		      "<img id=\"updating_%d\" src=\"%s/working16x16.gif\""	// TODO: change name and resolution to refreshing64x64.gif
		      " alt=\"%s\" title=\"%s\""
		      " class=\"ICO20x20\" style=\"display:none;\" />"	// Animated icon hidden
		      "</div>"
		      "</a>",
	    Gbl.Form.Num,Gbl.Prefs.URLIcons,Alt,Alt,
	    Gbl.Form.Num,Gbl.Prefs.URLIcons,Alt,Alt);
  }

/*****************************************************************************/
/********** Put a icon with a text to submit a form.                **********/
/********** When clicked, the icon will be replaced by an animation **********/
/*****************************************************************************/

void Ico_PutCalculateIconWithText (const char *Alt,const char *Text)
  {
   fprintf (Gbl.F.Out,"<div class=\"ICO_HIGHLIGHT\""
	              " style=\"margin:0 6px 0 0; display:inline;\">"
	              "<img id=\"update_%d\" src=\"%s/recycle16x16.gif\""
	              " alt=\"%s\" title=\"%s\""
		      " class=\"ICO20x20\" />"
		      "<img id=\"updating_%d\" src=\"%s/working16x16.gif\""
		      " alt=\"%s\" title=\"%s\""
		      " class=\"ICO20x20\" style=\"display:none;\" />"	// Animated icon hidden
		      "&nbsp;%s"
		      "</div>"
		      "</a>",
	    Gbl.Form.Num,Gbl.Prefs.URLIcons,Alt,Text,
	    Gbl.Form.Num,Gbl.Prefs.URLIcons,Alt,Text,
	    Text);
  }

/*****************************************************************************/
/******** Put a disabled icon indicating that removal is not allowed *********/
/*****************************************************************************/

void Ico_PutIconRemovalNotAllowed (void)
  {
   extern const char *Txt_Removal_not_allowed;

   Ico_PutIconOff ("trash.svg",Txt_Removal_not_allowed);
  }

/*****************************************************************************/
/******** Put an icon indicating that removal is not allowed *********/
/*****************************************************************************/

void Ico_PutIconRemove (void)
  {
   extern const char *Txt_Remove;

   fprintf (Gbl.F.Out,"<input type=\"image\" src=\"%s/trash.svg\""
		      " alt=\"%s\" title=\"%s\""
		      " class=\"CONTEXT_OPT ICO_HIGHLIGHT ICO16x16\" />",
	    Gbl.Prefs.URLIcons,
	    Txt_Remove,
	    Txt_Remove);
  }
