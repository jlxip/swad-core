// swad_QR.c: QR codes

/*
    SWAD (Shared Workspace At a Distance),
    is a web platform developed at the University of Granada (Spain),
    and used to support university teaching.

    This file is part of SWAD core.
    Copyright (C) 1999-2015 Antonio Ca�as Vargas

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

#include <string.h>	// For strncpy...

#include "swad_action.h"
#include "swad_global.h"
#include "swad_ID.h"
#include "swad_parameter.h"
#include "swad_QR.h"

/*****************************************************************************/
/****************************** Public constants *****************************/
/*****************************************************************************/

/*****************************************************************************/
/***************************** Internal constants ****************************/
/*****************************************************************************/

//#define QR_CODE_SIZE	((6+21+6)*6)
#define QR_CODE_SIZE	((6+25+6)*6)
#define QR_DEFAULT_TYPE QR_ID

/*****************************************************************************/
/****************************** Internal types *******************************/
/*****************************************************************************/

/*****************************************************************************/
/************** External global variables from others modules ****************/
/*****************************************************************************/

extern struct Globals Gbl;
extern struct Act_Actions Act_Actions[Act_NUM_ACTIONS];

/*****************************************************************************/
/************************* Internal global variables *************************/
/*****************************************************************************/

/*****************************************************************************/
/***************************** Internal prototypes ***************************/
/*****************************************************************************/

static void QR_ImageQRCode (const char *QRString);

/*****************************************************************************/
/***************** Put a link to a print view of a QR code *******************/
/*****************************************************************************/

void QR_PutLinkToPrintQRCode (QR_QRType_t QRType,struct UsrData *UsrDat,bool PrintText)
  {
   extern const char *The_ClassFormul[The_NUM_THEMES];
   extern const char *Txt_QR_code;
   char NicknameWithArroba[Nck_MAX_BYTES_NICKNAME_WITH_ARROBA+1];

   /***** Link to print view *****/
   Act_FormStart (ActPrnUsrQR);
   switch (QRType)
     {
      case QR_ID:
         Par_PutHiddenParamString ("QRString",UsrDat->IDs.List[0].ID);	// TODO: First ID?
         break;
      case QR_NICKNAME:
         sprintf (NicknameWithArroba,"@%s",UsrDat->Nickname);
         Par_PutHiddenParamString ("QRString",NicknameWithArroba);
         break;
      case QR_EMAIL:
         Par_PutHiddenParamString ("QRString",UsrDat->Email);
         break;
     }
   Act_LinkFormSubmit (Txt_QR_code,The_ClassFormul[Gbl.Prefs.Theme]);
   fprintf (Gbl.F.Out,"<img src=\"%s/qr16x16.gif\" alt=\"%s\""
	              " class=\"ICON16x16\" style=\"margin-left:10px;\" />",
            Gbl.Prefs.IconsURL,
            Txt_QR_code);
   if (PrintText)
      fprintf (Gbl.F.Out," %s",Txt_QR_code);
   fprintf (Gbl.F.Out,"</a>"
                      "</form>");
  }

/*****************************************************************************/
/******************************* Show a QR code ******************************/
/*****************************************************************************/

void QR_PrintQRCode (void)
  {
   char QRString[Cns_MAX_BYTES_STRING+1];

   /***** Get QR string *****/
   Par_GetParToText ("QRString",QRString,Cns_MAX_BYTES_STRING);

   /***** Show QR code *****/
   QR_ImageQRCode (QRString);
  }

/*****************************************************************************/
/******************** Write an QR (image) based on a string ******************/
/*****************************************************************************/

static void QR_ImageQRCode (const char *QRString)
  {
   fprintf (Gbl.F.Out,"<div style=\"width:%upx; text-align:center;\">"
                      "<img src=\"https://chart.googleapis.com/chart?cht=qr&amp;chs=%ux%u&amp;chl=%s\""
                      " alt=\"%s\" style=\"width:%upx; height:%upx;"
                      " border:1px dashed silver;\" /><br />"
                      "<span class=\"DAT\">%s</span>"
                      "</div>",
            QR_CODE_SIZE,
            QR_CODE_SIZE,QR_CODE_SIZE,
            QRString,
            QRString,
            QR_CODE_SIZE,QR_CODE_SIZE,
            QRString);
  }

/*****************************************************************************/
/*************** Show QR code with direct link (shortcut URL) ****************/
/*****************************************************************************/

void QR_LinkTo (unsigned Size,const char *ParamStr,long Cod)
  {
   extern const char *Txt_Shortcut;
   extern const char *Txt_STR_LANG_ID[Txt_NUM_LANGUAGES];

   /***** Show QR code with direct link to the current centre *****/
   fprintf (Gbl.F.Out,"<img src=\"https://chart.googleapis.com/chart?cht=qr&amp;chs=%ux%u&amp;chl=%s/%s?%s=%ld\""
                      " alt=\"%s\" style=\"width:%upx; height:%upx;\" />",
            Size,Size,
            Cfg_HTTPS_URL_SWAD_CGI,Txt_STR_LANG_ID[Gbl.Prefs.Language],ParamStr,Cod,
            Txt_Shortcut,
            Size,Size);
  }

/*****************************************************************************/
/*********** Show QR code with direct link to an exam announcement ***********/
/*****************************************************************************/

void QR_ExamAnnnouncement (void)
  {
   extern const char *Txt_Link_to_announcement_of_exam;

   /***** Show QR code with direct link to the exam announcement *****/
   fprintf (Gbl.F.Out,"<div style=\"text-align:center;\">"
                      "<img src=\"https://chart.googleapis.com/chart?cht=qr&amp;chs=%ux%u&amp;chl=%s/?crs=%ld%%26act=%ld\""
                      " alt=\"%s\" style=\"width:200px; height:200px;\" />"
                      "</div>",
            200,200,
            Cfg_HTTPS_URL_SWAD_CGI,Gbl.CurrentCrs.Crs.CrsCod,Act_Actions[ActSeeExaAnn].ActCod,
            Txt_Link_to_announcement_of_exam);
  }
