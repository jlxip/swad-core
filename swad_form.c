// swad_form.c: forms to go to actions

/*
    SWAD (Shared Workspace At a Distance),
    is a web platform developed at the University of Granada (Spain),
    and used to support university teaching.

    This file is part of SWAD core.
    Copyright (C) 1999-2018 Antonio Ca�as Vargas

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
/*********************************** Headers *********************************/
/*****************************************************************************/

#include "swad_form.h"
#include "swad_global.h"


/*****************************************************************************/
/************** External global variables from others modules ****************/
/*****************************************************************************/

extern struct Globals Gbl;

/*****************************************************************************/
/************************ Internal global variables **************************/
/*****************************************************************************/

/*****************************************************************************/
/**************************** Private prototypes *****************************/
/*****************************************************************************/

static void Frm_StartFormInternal (Act_Action_t NextAction,bool PutParameterLocationIfNoSesion,
                                   const char *Id,const char *Anchor,const char *OnSubmit);

/*****************************************************************************/
/******************************** Start a form *******************************/
/*****************************************************************************/

void Frm_StartFormGoTo (Act_Action_t NextAction)
  {
   Gbl.Form.Num++; // Initialized to -1. The first time it is incremented, it will be equal to 0
   snprintf (Gbl.Form.Id,sizeof (Gbl.Form.Id),
	     "form_%d",
	     Gbl.Form.Num);
   Frm_StartFormInternal (NextAction,false,Gbl.Form.Id,NULL,NULL);	// Do not put now parameter location
  }

void Frm_StartForm (Act_Action_t NextAction)
  {
   Frm_StartFormAnchorOnSubmit (NextAction,NULL,NULL);
  }

void Frm_StartFormAnchor (Act_Action_t NextAction,const char *Anchor)
  {
   Frm_StartFormAnchorOnSubmit (NextAction,Anchor,NULL);
  }

void Frm_StartFormOnSubmit (Act_Action_t NextAction,const char *OnSubmit)
  {
   Frm_StartFormAnchorOnSubmit (NextAction,NULL,OnSubmit);
  }

void Frm_StartFormAnchorOnSubmit (Act_Action_t NextAction,const char *Anchor,const char *OnSubmit)
  {
   Gbl.Form.Num++; // Initialized to -1. The first time it is incremented, it will be equal to 0
   snprintf (Gbl.Form.Id,sizeof (Gbl.Form.Id),
	     "form_%d",
	     Gbl.Form.Num);
   Frm_StartFormInternal (NextAction,true,Gbl.Form.Id,Anchor,OnSubmit);	// Do put now parameter location (if no open session)
  }

void Frm_StartFormUnique (Act_Action_t NextAction)
  {
   Frm_StartFormUniqueAnchor (NextAction,NULL);
  }

void Frm_StartFormUniqueAnchor (Act_Action_t NextAction,const char *Anchor)
  {
   Gbl.Form.Num++; // Initialized to -1. The first time it is incremented, it will be equal to 0
   snprintf (Gbl.Form.UniqueId,sizeof (Gbl.Form.UniqueId),
	     "form_%s_%d",
             Gbl.UniqueNameEncrypted,Gbl.Form.Num);
   Frm_StartFormInternal (NextAction,true,Gbl.Form.UniqueId,Anchor,NULL);	// Do put now parameter location (if no open session)
  }

void Frm_StartFormId (Act_Action_t NextAction,const char *Id)
  {
   Gbl.Form.Num++; // Initialized to -1. The first time it is incremented, it will be equal to 0
   Frm_StartFormInternal (NextAction,true,Id,NULL,NULL);	// Do put now parameter location (if no open session)
  }

// Id can not be NULL
static void Frm_StartFormInternal (Act_Action_t NextAction,bool PutParameterLocationIfNoSesion,
                                   const char *Id,const char *Anchor,const char *OnSubmit)
  {
   extern const char *Txt_STR_LANG_ID[1 + Txt_NUM_LANGUAGES];
   char ParamsStr[Frm_MAX_BYTES_PARAMS_STR + 1];

   if (!Gbl.Form.Inside)
     {
      /* Start form */
      fprintf (Gbl.F.Out,"<form method=\"post\" action=\"%s/%s",
	       Cfg_URL_SWAD_CGI,
	       Txt_STR_LANG_ID[Gbl.Prefs.Language]);
      if (Anchor)
	 if (Anchor[0])
            fprintf (Gbl.F.Out,"#%s",Anchor);
      fprintf (Gbl.F.Out,"\" id=\"%s\"",Id);
      if (OnSubmit)
         if (OnSubmit[0])
            fprintf (Gbl.F.Out," onsubmit=\"%s;\"",OnSubmit);
      switch (Act_GetBrowserTab (NextAction))
	{
	 case Act_BRW_NEW_TAB:
	 case Act_DOWNLD_FILE:
	    fprintf (Gbl.F.Out," target=\"_blank\"");
	    break;
	 default:
	    break;
	}
      if (Act_GetContentType (NextAction) == Act_CONT_DATA)
	 fprintf (Gbl.F.Out," enctype=\"multipart/form-data\"");
      fprintf (Gbl.F.Out," accept-charset=\"windows-1252\">");

      /* Put basic form parameters */
      Frm_SetParamsForm (ParamsStr,NextAction,PutParameterLocationIfNoSesion);
      fprintf (Gbl.F.Out,"%s",ParamsStr);

      Gbl.Form.Inside = true;
     }
  }

void Frm_SetParamsForm (char ParamsStr[Frm_MAX_BYTES_PARAMS_STR + 1],Act_Action_t NextAction,
                        bool PutParameterLocationIfNoSesion)
  {
   char ParamAction[Frm_MAX_BYTES_PARAM_ACTION + 1];
   char ParamSession[Frm_MAX_BYTES_PARAM_SESSION + 1];
   char ParamLocation[Frm_MAX_BYTES_PARAM_LOCATION + 1];

   ParamAction[0] = '\0';
   ParamSession[0] = '\0';
   ParamLocation[0] = '\0';

   if (NextAction != ActUnk)
      snprintf (ParamAction,sizeof (ParamAction),
	        "<input type=\"hidden\" name=\"act\" value=\"%ld\" />",
	        Act_GetActCod (NextAction));

   if (Gbl.Session.Id[0])
      snprintf (ParamSession,sizeof (ParamSession),
	        "<input type=\"hidden\" name=\"ses\" value=\"%s\" />",
	        Gbl.Session.Id);
   else if (PutParameterLocationIfNoSesion)
      // Extra parameters necessary when there's no open session
     {
      /* If session is open, course code will be get from session data,
	 but if there is not an open session, and next action is known,
	 it is necessary to send a parameter with course code */
      if (Gbl.CurrentCrs.Crs.CrsCod > 0)
	 // If course selected...
         snprintf (ParamLocation,sizeof (ParamLocation),
                   "<input type=\"hidden\" name=\"crs\" value=\"%ld\" />",
                   Gbl.CurrentCrs.Crs.CrsCod);
      else if (Gbl.CurrentDeg.Deg.DegCod > 0)
	 // If no course selected, but degree selected...
         snprintf (ParamLocation,sizeof (ParamLocation),
                   "<input type=\"hidden\" name=\"deg\" value=\"%ld\" />",
                   Gbl.CurrentDeg.Deg.DegCod);
      else if (Gbl.CurrentCtr.Ctr.CtrCod > 0)
	 // If no degree selected, but centre selected...
         snprintf (ParamLocation,sizeof (ParamLocation),
                   "<input type=\"hidden\" name=\"ctr\" value=\"%ld\" />",
                   Gbl.CurrentCtr.Ctr.CtrCod);
      else if (Gbl.CurrentIns.Ins.InsCod > 0)
	 // If no centre selected, but institution selected...
         snprintf (ParamLocation,sizeof (ParamLocation),
                   "<input type=\"hidden\" name=\"ins\" value=\"%ld\" />",
                   Gbl.CurrentIns.Ins.InsCod);
      else if (Gbl.CurrentCty.Cty.CtyCod > 0)
	 // If no institution selected, but country selected...
         snprintf (ParamLocation,sizeof (ParamLocation),
                   "<input type=\"hidden\" name=\"cty\" value=\"%ld\" />",
                   Gbl.CurrentCty.Cty.CtyCod);
     }

   snprintf (ParamsStr,Frm_MAX_BYTES_PARAMS_STR + 1,
	     "%s%s%s",
	     ParamAction,ParamSession,ParamLocation);
  }

void Frm_EndForm (void)
  {
   if (Gbl.Form.Inside)
     {
      fprintf (Gbl.F.Out,"</form>");
      Gbl.Form.Inside = false;
     }
  }

/*****************************************************************************/
/******************* Anchor directive used to send a form ********************/
/*****************************************************************************/
// Requires an extern </a>

void Frm_LinkFormSubmit (const char *Title,const char *LinkStyle,
                         const char *OnSubmit)
  {
   Frm_LinkFormSubmitId (Title,LinkStyle,Gbl.Form.Id,OnSubmit);
  }

void Frm_LinkFormSubmitUnique (const char *Title,const char *LinkStyle)
  {
   Frm_LinkFormSubmitId (Title,LinkStyle,Gbl.Form.UniqueId,NULL);
  }

// Title can be NULL
// LinkStyle can be NULL
// Id can not be NULL
// OnSubmit can be NULL

void Frm_LinkFormSubmitId (const char *Title,const char *LinkStyle,
                           const char *Id,const char *OnSubmit)
  {
   fprintf (Gbl.F.Out,"<a href=\"\"");
   if (Title)
      if (Title[0])
         fprintf (Gbl.F.Out," title=\"%s\"",Title);
   if (LinkStyle)
      if (LinkStyle[0])
         fprintf (Gbl.F.Out," class=\"%s\"",LinkStyle);
   fprintf (Gbl.F.Out," onclick=\"");
   if (OnSubmit)	// JavaScript function to be called
			// before submitting the form
      if (OnSubmit[0])
         fprintf (Gbl.F.Out,"%s;",OnSubmit);
   fprintf (Gbl.F.Out,"document.getElementById('%s').submit();"
		      "return false;\">",
	    Id);
  }

void Frm_LinkFormSubmitAnimated (const char *Title,const char *LinkStyle,
                                 const char *OnSubmit)
  {
   fprintf (Gbl.F.Out,"<a href=\"\"");
   if (Title)
      if (Title[0])
         fprintf (Gbl.F.Out," title=\"%s\"",Title);
   if (LinkStyle)
      if (LinkStyle[0])
         fprintf (Gbl.F.Out," class=\"%s\"",LinkStyle);
   fprintf (Gbl.F.Out," onclick=\"");
   if (OnSubmit)	// JavaScript function to be called
			// before submitting the form
      if (OnSubmit[0])
         fprintf (Gbl.F.Out,"%s;",OnSubmit);
   fprintf (Gbl.F.Out,"AnimateIcon(%d);"
		      "document.getElementById('%s').submit();"
		      "return false;\">",
	    Gbl.Form.Num,
	    Gbl.Form.Id);
  }

/*****************************************************************************/
/***************************** Get unique Id *********************************/
/*****************************************************************************/

void Frm_SetUniqueId (char UniqueId[Frm_MAX_BYTES_ID + 1])
  {
   static unsigned CountForThisExecution = 0;

   /***** Create Id. The id must be unique,
          the page content may be updated via AJAX.
          So, Id uses:
          - a name for this execution (Gbl.UniqueNameEncrypted)
          - a number for each element in this execution (CountForThisExecution) *****/
   snprintf (UniqueId,Frm_MAX_BYTES_ID + 1,
	     "id_%s_%u",
             Gbl.UniqueNameEncrypted,
             ++CountForThisExecution);
  }