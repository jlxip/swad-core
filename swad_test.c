// swad_test.c: self-assessment tests

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
/*********************************** Headers *********************************/
/*****************************************************************************/

#include <limits.h>		// For UINT_MAX
#include <linux/limits.h>	// For PATH_MAX
#include <linux/stddef.h>	// For NULL
#include <mysql/mysql.h>	// To access MySQL databases
#include <stdbool.h>		// For boolean type
#include <stdio.h>		// For fprintf, etc.
#include <stdlib.h>		// For exit, system, malloc, free, etc
#include <string.h>		// For string functions
#include <sys/stat.h>		// For mkdir
#include <sys/types.h>		// For mkdir

#include "swad_action.h"
#include "swad_box.h"
#include "swad_database.h"
#include "swad_form.h"
#include "swad_global.h"
#include "swad_ID.h"
#include "swad_language.h"
#include "swad_media.h"
#include "swad_parameter.h"
#include "swad_table.h"
#include "swad_theme.h"
#include "swad_test.h"
#include "swad_test_import.h"
#include "swad_user.h"
#include "swad_xml.h"

/*****************************************************************************/
/***************************** Public constants ******************************/
/*****************************************************************************/

// strings are limited to Tst_MAX_BYTES_FEEDBACK_TYPE bytes
const char *Tst_FeedbackXML[Tst_NUM_TYPES_FEEDBACK] =
  {
   "nothing",
   "totalResult",
   "eachResult",
   "eachGoodBad",
   "fullFeedback",
  };

// strings are limited to Tst_MAX_BYTES_ANSWER_TYPE characters
const char *Tst_StrAnswerTypesXML[Tst_NUM_ANS_TYPES] =
  {
   "int",
   "float",
   "TF",
   "uniqueChoice",
   "multipleChoice",
   "text",
  };

/*****************************************************************************/
/**************************** Private constants ******************************/
/*****************************************************************************/

#define Tst_MAX_BYTES_TAGS_LIST		(16 * 1024)
#define Tst_MAX_BYTES_FLOAT_ANSWER	30	// Maximum length of the strings that store an floating point answer

const char *Tst_PluggableDB[Tst_NUM_OPTIONS_PLUGGABLE] =
  {
   "unknown",
   "N",
   "Y",
  };

// Feedback to students in tests
const char *Tst_FeedbackDB[Tst_NUM_TYPES_FEEDBACK] =
  {
   "nothing",		// No feedback
   "total_result",	// Little
   "each_result",	// Medium
   "each_good_bad",	// High
   "full_feedback",	// Maximum
  };

const char *Tst_StrAnswerTypesDB[Tst_NUM_ANS_TYPES] =
  {
   "int",
   "float",
   "true_false",
   "unique_choice",
   "multiple_choice",
   "text",
  };

// Test images will be saved with:
// - maximum width of Tst_IMAGE_SAVED_MAX_HEIGHT
// - maximum height of Tst_IMAGE_SAVED_MAX_HEIGHT
// - maintaining the original aspect ratio (aspect ratio recommended: 3:2)
#define Tst_IMAGE_SAVED_MAX_WIDTH	768
#define Tst_IMAGE_SAVED_MAX_HEIGHT	512
#define Tst_IMAGE_SAVED_QUALITY		 75	// 1 to 100

/*****************************************************************************/
/******************************* Internal types ******************************/
/*****************************************************************************/

#define Tst_NUM_STATUS 2
typedef enum
  {
   Tst_STATUS_SHOWN_BUT_NOT_ASSESSED	= 0,
   Tst_STATUS_ASSESSED			= 1,
   Tst_STATUS_ERROR			= 2,
  } Tst_Status_t;

/*****************************************************************************/
/************** External global variables from others modules ****************/
/*****************************************************************************/

extern struct Globals Gbl;

/*****************************************************************************/
/************************* Internal global variables *************************/
/*****************************************************************************/

/*****************************************************************************/
/***************************** Internal prototypes ***************************/
/*****************************************************************************/

static void Tst_PutFormToViewResultsOfUsersTests (Act_Action_t Action);

static void Tst_GetQuestionsAndAnswersFromForm (void);
static void Tst_ShowTstTotalMark (double TotalScore);
static bool Tst_CheckIfNextTstAllowed (void);
static void Tst_SetTstStatus (unsigned NumTst,Tst_Status_t TstStatus);
static Tst_Status_t Tst_GetTstStatus (unsigned NumTst);
static unsigned Tst_GetNumAccessesTst (void);
static void Tst_ShowTestQuestionsWhenSeeing (MYSQL_RES *mysql_res);
static void Tst_ShowTestResultAfterAssess (long TstCod,unsigned *NumQstsNotBlank,double *TotalScore);
static void Tst_WriteQstAndAnsTest (Tst_ActionToDoWithQuestions_t ActionToDoWithQuestions,
			            struct UsrData *UsrDat,
                                    struct Game *Game,
                                    unsigned NumQst,long QstCod,MYSQL_ROW row,
                                    double *ScoreThisQst,bool *AnswerIsNotBlank);
static void Tst_PutFormToEditQstMedia (struct Media *Media,int NumMediaInForm,
                                       bool OptionsDisabled);
static void Tst_UpdateScoreQst (long QstCod,float ScoreThisQst,bool AnswerIsNotBlank);
static void Tst_UpdateMyNumAccessTst (unsigned NumAccessesTst);
static void Tst_UpdateLastAccTst (void);
static bool Tst_CheckIfICanEditTests (void);
static void Tst_PutIconsTests (void);
static void Tst_PutButtonToAddQuestion (void);

static long Tst_GetParamTagCode (void);
static bool Tst_CheckIfCurrentCrsHasTestTags (void);
static unsigned long Tst_GetAllTagsFromCurrentCrs (MYSQL_RES **mysql_res);
static unsigned long Tst_GetEnabledTagsFromThisCrs (MYSQL_RES **mysql_res);
static void Tst_ShowFormSelTags (unsigned long NumRows,MYSQL_RES *mysql_res,
                                 bool ShowOnlyEnabledTags,unsigned NumCols);
static void Tst_ShowFormEditTags (void);
static void Tst_PutIconEnable (long TagCod,const char *TagTxt);
static void Tst_PutIconDisable (long TagCod,const char *TagTxt);
static void Tst_ShowFormConfigTst (void);
static void Tst_PutInputFieldNumQst (const char *Field,const char *Label,
                                     unsigned Value);
static void Tst_GetConfigTstFromDB (void);
static Tst_Pluggable_t Tst_GetPluggableFromForm (void);
static Tst_Feedback_t Tst_GetFeedbackTypeFromForm (void);
static void Tst_CheckAndCorrectNumbersQst (void);
static void Tst_ShowFormAnswerTypes (unsigned NumCols);
static unsigned long Tst_GetQuestions (MYSQL_RES **mysql_res);
static unsigned long Tst_GetQuestionsForTest (MYSQL_RES **mysql_res);
static void Tst_ListOneQstToEdit (void);
static void Tst_ListOneOrMoreQuestionsForEdition (unsigned long NumRows,
                                                  MYSQL_RES *mysql_res);
static void Tst_ListOneOrMoreQuestionsForSelection (long GamCod,
                                                    unsigned long NumRows,
                                                    MYSQL_RES *mysql_res);

static void Tst_WriteAnswersEdit (long QstCod);
static void Tst_WriteAnswersTestToAnswer (unsigned NumQst,long QstCod,bool Shuffle);
static void Tst_WriteAnswersTestResult (struct UsrData *UsrDat,
                                        unsigned NumQst,long QstCod,
                                        double *ScoreThisQst,bool *AnswerIsNotBlank);

static void Tst_WriteTFAnsViewTest (unsigned NumQst);
static void Tst_WriteTFAnsAssessTest (struct UsrData *UsrDat,
				      unsigned NumQst,MYSQL_RES *mysql_res,
                                      double *ScoreThisQst,bool *AnswerIsNotBlank);

static void Tst_WriteChoiceAnsViewTest (unsigned NumQst,long QstCod,bool Shuffle);
static void Tst_WriteChoiceAnsAssessTest (struct UsrData *UsrDat,
				          unsigned NumQst,MYSQL_RES *mysql_res,
                                          double *ScoreThisQst,bool *AnswerIsNotBlank);
static void Tst_WriteChoiceAnsViewGame (struct Game *Game,
                                        unsigned NumQst,long QstCod,
                                        const char *Class,
                                        bool ShowResult);

static void Tst_WriteTextAnsViewTest (unsigned NumQst);
static void Tst_WriteTextAnsAssessTest (struct UsrData *UsrDat,
				        unsigned NumQst,MYSQL_RES *mysql_res,
                                        double *ScoreThisQst,bool *AnswerIsNotBlank);

static void Tst_WriteIntAnsViewTest (unsigned NumQst);
static void Tst_WriteIntAnsAssessTest (struct UsrData *UsrDat,
				       unsigned NumQst,MYSQL_RES *mysql_res,
                                       double *ScoreThisQst,bool *AnswerIsNotBlank);

static void Tst_WriteFloatAnsViewTest (unsigned NumQst);
static void Tst_WriteFloatAnsAssessTest (struct UsrData *UsrDat,
				         unsigned NumQst,MYSQL_RES *mysql_res,
                                         double *ScoreThisQst,bool *AnswerIsNotBlank);

static void Tst_WriteHeadUserCorrect (struct UsrData *UsrDat);
static void Tst_WriteScoreStart (unsigned ColSpan);
static void Tst_WriteScoreEnd (void);
static void Tst_WriteParamQstCod (unsigned NumQst,long QstCod);
static bool Tst_GetParamsTst (Tst_ActionToDoWithQuestions_t ActionToDoWithQuestions);
static unsigned Tst_GetAndCheckParamNumTst (void);
static void Tst_GetParamNumQst (void);
static bool Tst_GetCreateXMLFromForm (void);
static int Tst_CountNumTagsInList (void);
static int Tst_CountNumAnswerTypesInList (void);
static void Tst_PutFormEditOneQst (char Stem[Cns_MAX_BYTES_TEXT + 1],
                                   char Feedback[Cns_MAX_BYTES_TEXT + 1]);
static void Tst_PutFloatInputField (const char *Label,const char *Field,
                                    double Value);
static void Tst_PutTFInputField (const char *Label,char Value);

static void Tst_FreeTextChoiceAnswers (void);
static void Tst_FreeTextChoiceAnswer (unsigned NumOpt);

static void Tst_ResetMediaOfQuestion (void);
static void Tst_FreeMediaOfQuestion (void);

static void Tst_GetQstDataFromDB (char Stem[Cns_MAX_BYTES_TEXT + 1],
                                  char Feedback[Cns_MAX_BYTES_TEXT + 1]);
static long Tst_GetMedCodFromDB (int NumOpt);
static void Tst_GetMediaFromDB (int NumOpt,struct Media *Media);

static Tst_AnswerType_t Tst_ConvertFromUnsignedStrToAnsTyp (const char *UnsignedStr);
static void Tst_GetQstFromForm (char *Stem,char *Feedback);
static void Tst_MoveMediaToDefinitiveDirectories (void);

static long Tst_GetTagCodFromTagTxt (const char *TagTxt);
static long Tst_CreateNewTag (long CrsCod,const char *TagTxt);
static void Tst_EnableOrDisableTag (long TagCod,bool TagHidden);

static void Tst_PutIconToRemoveOneQst (void);
static void Tst_PutParamsRemoveOneQst (void);
static void Tst_PutParamsRemoveQst (void);

static long Tst_GetQstCod (void);

static void Tst_InsertOrUpdateQstIntoDB (void);
static void Tst_InsertTagsIntoDB (void);
static void Tst_InsertAnswersIntoDB (void);

static void Tst_RemAnsFromQst (void);
static void Tst_RemTagsFromQst (void);
static void Tst_RemoveUnusedTagsFromCurrentCrs (void);

static void Tst_RemoveAllMedFilesFromStemOfAllQstsInCrs (long CrsCod);
static void Tst_RemoveMediaFromAllAnsOfQst (long CrsCod,long QstCod);
static void Tst_RemoveAllMedFilesFromAnsOfAllQstsInCrs (long CrsCod);

static unsigned Tst_GetNumTstQuestions (Hie_Level_t Scope,Tst_AnswerType_t AnsType,struct Tst_Stats *Stats);
static unsigned Tst_GetNumCoursesWithTstQuestions (Hie_Level_t Scope,Tst_AnswerType_t AnsType);
static unsigned Tst_GetNumCoursesWithPluggableTstQuestions (Hie_Level_t Scope,Tst_AnswerType_t AnsType);

static long Tst_CreateTestResultInDB (void);
static void Tst_StoreScoreOfTestResultInDB (long TstCod,
                                          unsigned NumQstsNotBlank,double Score);
static void Tst_ShowHeaderTestResults (void);
static void Tst_ShowTestResults (struct UsrData *UsrDat);
static void Tst_ShowDataUsr (struct UsrData *UsrDat,unsigned NumExams);
static void Tst_PutParamTstCod (long TstCod);
static long Tst_GetParamTstCod (void);
static void Tst_ShowTestResultsSummaryRow (bool ItsMe,
                                           unsigned NumExams,
                                           unsigned NumTotalQsts,
                                           unsigned NumTotalQstsNotBlank,
                                           double TotalScoreOfAllTests);
static void Tst_ShowTestResult (time_t TstTimeUTC);
static void Tst_GetTestResultDataByTstCod (long TstCod,time_t *TstTimeUTC,
                                           unsigned *NumQsts,unsigned *NumQstsNotBlank,double *Score);
static void Tst_StoreOneTestResultQstInDB (long TstCod,long QstCod,unsigned NumQst,double Score);
static void Tst_GetTestResultQuestionsFromDB (long TstCod);

/*****************************************************************************/
/*************** Show form to generate a self-assessment test ****************/
/*****************************************************************************/

void Tst_ShowFormAskTst (void)
  {
   extern const char *Hlp_ASSESSMENT_Tests;
   extern const char *The_ClassFormInBox[The_NUM_THEMES];
   extern const char *Txt_Take_a_test;
   extern const char *Txt_No_of_questions;
   extern const char *Txt_Generate_test;
   extern const char *Txt_No_test_questions;
   MYSQL_RES *mysql_res;
   unsigned long NumRows;

   /***** Read test configuration from database *****/
   Tst_GetConfigTstFromDB ();

   /***** Put link to view tests results *****/
   switch (Gbl.Usrs.Me.Role.Logged)
     {
      case Rol_STD:
         Tst_PutFormToViewResultsOfUsersTests (ActReqSeeMyTstRes);
         break;
      case Rol_NET:
      case Rol_TCH:
      case Rol_SYS_ADM:
         Tst_PutFormToViewResultsOfUsersTests (ActReqSeeUsrTstRes);
	 break;
      default:
	 break;
     }

   /***** Start box *****/
   Box_StartBox (NULL,Txt_Take_a_test,Tst_PutIconsTests,
                 Hlp_ASSESSMENT_Tests,Box_NOT_CLOSABLE);

   /***** Get tags *****/
   if ((NumRows = Tst_GetEnabledTagsFromThisCrs (&mysql_res)) != 0)
     {
      /***** Check if minimum date-time of next access to test is older than now *****/
      if (Tst_CheckIfNextTstAllowed ())
        {
         Frm_StartForm (ActSeeTst);

         Tbl_StartTable (2);

         /***** Selection of tags *****/
         Tst_ShowFormSelTags (NumRows,mysql_res,true,1);

         /***** Selection of types of answers *****/
         Tst_ShowFormAnswerTypes (1);

         /***** Number of questions to generate ****/
         fprintf (Gbl.F.Out,"<tr>"
                            "<td class=\"RIGHT_MIDDLE\">"
                            "<label for=\"NumQst\" class=\"%s\">"
                            "%s:"
                            "</label>"
                            "</td>"
                            "<td class=\"LEFT_MIDDLE\">"
                            "<input type=\"number\""
                            " id=\"NumQst\" name=\"NumQst\""
                            " min=\"%u\" max=\"%u\" value=\"%u\"",
                  The_ClassFormInBox[Gbl.Prefs.Theme],Txt_No_of_questions,
                  Gbl.Test.Config.Min,
                  Gbl.Test.Config.Max,
                  Gbl.Test.Config.Def);
         if (Gbl.Test.Config.Min == Gbl.Test.Config.Max)
            fprintf (Gbl.F.Out," disabled=\"disabled\"");
         fprintf (Gbl.F.Out," />"
                            "</td>"
                            "</tr>");

         Tbl_EndTable ();

         /***** Send button *****/
         Btn_PutConfirmButton (Txt_Generate_test);
         Frm_EndForm ();
        }
     }
   else
     {
      /***** Warning message *****/
      Ale_ShowAlert (Ale_INFO,Txt_No_test_questions);

      /***** Button to create a new question *****/
      if (Tst_CheckIfICanEditTests ())
         Tst_PutButtonToAddQuestion ();
     }

   /***** End box *****/
   Box_EndBox ();

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/*************** Write a form to go to result of users' tests ****************/
/*****************************************************************************/

static void Tst_PutFormToViewResultsOfUsersTests (Act_Action_t Action)
  {
   extern const char *Txt_Test_results;

   fprintf (Gbl.F.Out,"<div class=\"CONTEXT_MENU\">");
   Lay_PutContextualLinkIconText (Action,NULL,NULL,
				  "tasks.svg",
				  Txt_Test_results);
   fprintf (Gbl.F.Out,"</div>");
  }

/*****************************************************************************/
/********************** Generate self-assessment test ************************/
/*****************************************************************************/

void Tst_ShowNewTest (void)
  {
   extern const char *Hlp_ASSESSMENT_Tests;
   extern const char *The_ClassFormInBox[The_NUM_THEMES];
   extern const char *Txt_No_questions_found_matching_your_search_criteria;
   extern const char *Txt_Test;
   extern const char *Txt_Allow_teachers_to_consult_this_test;
   extern const char *Txt_Done_assess_test;
   MYSQL_RES *mysql_res;
   unsigned long NumRows;
   unsigned NumAccessesTst;

   /***** Read test configuration from database *****/
   Tst_GetConfigTstFromDB ();

   if (Tst_CheckIfNextTstAllowed ())
     {
      /***** Check that all parameters used to generate a test are valid *****/
      if (Tst_GetParamsTst (Tst_SHOW_TEST_TO_ANSWER))	// Get parameters from form
        {
         /***** Get questions *****/
         if ((NumRows = Tst_GetQuestionsForTest (&mysql_res)) == 0)	// Query database
           {
            Ale_ShowAlert (Ale_INFO,Txt_No_questions_found_matching_your_search_criteria);
            Tst_ShowFormAskTst ();					// Show the form again
           }
         else
           {
            /***** Get and update number of hits *****/
            NumAccessesTst = Tst_GetNumAccessesTst () + 1;
            if (Gbl.Usrs.Me.IBelongToCurrentCrs)
	       Tst_UpdateMyNumAccessTst (NumAccessesTst);

	    /***** Start box *****/
	    Box_StartBox (NULL,Txt_Test,NULL,
	                  Hlp_ASSESSMENT_Tests,Box_NOT_CLOSABLE);
	    Lay_WriteHeaderClassPhoto (false,false,
				       Gbl.Hierarchy.Ins.InsCod,
				       Gbl.Hierarchy.Deg.DegCod,
				       Gbl.Hierarchy.Crs.CrsCod);

            /***** Start form *****/
            Frm_StartForm (ActAssTst);
  	    Gbl.Test.NumQsts = (unsigned) NumRows;
            Par_PutHiddenParamUnsigned ("NumTst",NumAccessesTst);
            Par_PutHiddenParamUnsigned ("NumQst",Gbl.Test.NumQsts);

            /***** List the questions *****/
            Tbl_StartTableWideMargin (10);
            Tst_ShowTestQuestionsWhenSeeing (mysql_res);
	    Tbl_EndTable ();

	    /***** Test result will be saved? *****/
	    fprintf (Gbl.F.Out,"<div class=\"CENTER_MIDDLE\""
		               " style=\"margin-top:20px;\">"
	                       "<label class=\"%s\">"
			       "<input type=\"checkbox\" name=\"Save\""
			       " value=\"Y\"",
		     The_ClassFormInBox[Gbl.Prefs.Theme]);
	    if (Gbl.Test.AllowTeachers)
	       fprintf (Gbl.F.Out," checked=\"checked\"");
	    fprintf (Gbl.F.Out," />"
		               "&nbsp;%s"
	                       "</label>"
			       "</div>",
		     Txt_Allow_teachers_to_consult_this_test);

            /***** End form *****/
            Btn_PutConfirmButton (Txt_Done_assess_test);
            Frm_EndForm ();

            /***** End box *****/
	    Box_EndBox ();

            /***** Set test status *****/
            Tst_SetTstStatus (NumAccessesTst,Tst_STATUS_SHOWN_BUT_NOT_ASSESSED);

            /***** Update date-time of my next allowed access to test *****/
            if (Gbl.Usrs.Me.Role.Logged == Rol_STD)
               Tst_UpdateLastAccTst ();
           }

         /***** Free structure that stores the query result *****/
         DB_FreeMySQLResult (&mysql_res);
        }
      else
         Tst_ShowFormAskTst ();							// Show the form again

      /***** Free memory used for by the list of tags *****/
      Tst_FreeTagsList ();
     }
  }

/*****************************************************************************/
/******************************** Assess a test ******************************/
/*****************************************************************************/

void Tst_AssessTest (void)
  {
   extern const char *Hlp_ASSESSMENT_Tests;
   extern const char *Txt_Test_result;
   extern const char *Txt_Test_No_X_that_you_make_in_this_course;
   extern const char *Txt_The_test_X_has_already_been_assessed_previously;
   extern const char *Txt_There_was_an_error_in_assessing_the_test_X;
   unsigned NumTst;
   long TstCod = -1L;	// Initialized to avoid warning
   unsigned NumQstsNotBlank;
   double TotalScore;

   /***** Read test configuration from database *****/
   Tst_GetConfigTstFromDB ();

   /***** Get number of this test from form *****/
   NumTst = Tst_GetAndCheckParamNumTst ();

   /****** Get test status in database for this session-course-num.test *****/
   switch (Tst_GetTstStatus (NumTst))
     {
      case Tst_STATUS_SHOWN_BUT_NOT_ASSESSED:
         /***** Get the parameters of the form *****/
         /* Get number of questions */
         Tst_GetParamNumQst ();

         /***** Get if test must be saved *****/
	 Gbl.Test.AllowTeachers = Par_GetParToBool ("Save");

	 /***** Get questions and answers from form to assess a test *****/
	 Tst_GetQuestionsAndAnswersFromForm ();

	 /***** Create new test in database to store the result *****/
	 TstCod = Tst_CreateTestResultInDB ();

	 /***** Start box *****/
	 Box_StartBox (NULL,Txt_Test_result,NULL,
	               Hlp_ASSESSMENT_Tests,Box_NOT_CLOSABLE);
	 Lay_WriteHeaderClassPhoto (false,false,
				    Gbl.Hierarchy.Ins.InsCod,
				    Gbl.Hierarchy.Deg.DegCod,
				    Gbl.Hierarchy.Crs.CrsCod);

	 /***** Header *****/
	 if (Gbl.Usrs.Me.IBelongToCurrentCrs)
	   {
	    fprintf (Gbl.F.Out,"<div class=\"TEST_SUBTITLE\">");
	    fprintf (Gbl.F.Out,Txt_Test_No_X_that_you_make_in_this_course,NumTst);
	    fprintf (Gbl.F.Out,"</div>");
	   }

	 /***** Write answers and solutions *****/
         Tbl_StartTableWideMargin (10);
	 Tst_ShowTestResultAfterAssess (TstCod,&NumQstsNotBlank,&TotalScore);
	 Tbl_EndTable ();

	 /***** Write total mark of test *****/
	 if (Gbl.Test.Config.Feedback != Tst_FEEDBACK_NOTHING)
	    Tst_ShowTstTotalMark (TotalScore);

	 /***** End box *****/
	 Box_EndBox ();

	 /***** Store test result in database *****/
	 Tst_StoreScoreOfTestResultInDB (TstCod,
				         NumQstsNotBlank,TotalScore);

         /***** Set test status *****/
         Tst_SetTstStatus (NumTst,Tst_STATUS_ASSESSED);
         break;
      case Tst_STATUS_ASSESSED:
         Ale_ShowAlert (Ale_WARNING,Txt_The_test_X_has_already_been_assessed_previously,
                        NumTst);
         break;
      case Tst_STATUS_ERROR:
         Ale_ShowAlert (Ale_WARNING,Txt_There_was_an_error_in_assessing_the_test_X,
                        NumTst);
         break;
     }
  }

/*****************************************************************************/
/*********** Get questions and answers from form to assess a test ************/
/*****************************************************************************/

static void Tst_GetQuestionsAndAnswersFromForm (void)
  {
   unsigned NumQst;
   char StrQstIndOrAns[3 + 10 + 1];	// "Qstxx...x", "Indxx...x" or "Ansxx...x"

   /***** Get questions and answers *****/
   for (NumQst = 0;
	NumQst < Gbl.Test.NumQsts;
	NumQst++)
     {
      /* Get question code */
      snprintf (StrQstIndOrAns,sizeof (StrQstIndOrAns),
	        "Qst%06u",
		NumQst);
      if ((Gbl.Test.QstCodes[NumQst] = Par_GetParToLong (StrQstIndOrAns)) <= 0)
	 Lay_ShowErrorAndExit ("Code of question is missing.");

      /* Get indexes for this question */
      snprintf (StrQstIndOrAns,sizeof (StrQstIndOrAns),
	        "Ind%06u",
		NumQst);
      Par_GetParMultiToText (StrQstIndOrAns,Gbl.Test.StrIndexesOneQst[NumQst],
                             Tst_MAX_BYTES_INDEXES_ONE_QST);  /* If choice ==> "0", "1", "2",... */

      /* Get answers selected by user for this question */
      snprintf (StrQstIndOrAns,sizeof (StrQstIndOrAns),
	        "Ans%06u",
		NumQst);
      Par_GetParMultiToText (StrQstIndOrAns,Gbl.Test.StrAnswersOneQst[NumQst],
                             Tst_MAX_BYTES_ANSWERS_ONE_QST);  /* If answer type == T/F ==> " ", "T", "F"; if choice ==> "0", "2",... */
     }
  }

/*****************************************************************************/
/************************** Show total mark of a test ************************/
/*****************************************************************************/

static void Tst_ShowTstTotalMark (double TotalScore)
  {
   extern const char *Txt_Score;
   extern const char *Txt_out_of_PART_OF_A_SCORE;
   double TotalScoreOverSCORE_MAX = TotalScore * Tst_SCORE_MAX / (double) Gbl.Test.NumQsts;

   /***** Write total mark ****/
   fprintf (Gbl.F.Out,"<div class=\"DAT CENTER_MIDDLE\""
	              " style=\"margin-top:20px;\">"
		      "%s: <span class=\"%s\">%.2lf (%.2lf %s %u)</span>"
		      "</div>",
	    Txt_Score,
	    (TotalScoreOverSCORE_MAX >= (double) TotalScoreOverSCORE_MAX / 2.0) ? "ANS_OK" :
					                                          "ANS_BAD",
	    TotalScore,
	    TotalScoreOverSCORE_MAX,Txt_out_of_PART_OF_A_SCORE,Tst_SCORE_MAX);
  }

/*****************************************************************************/
/************** Check minimum date-time of next access to test ***************/
/*****************************************************************************/
// Return true if allowed date-time of next access to test is older than now

static bool Tst_CheckIfNextTstAllowed (void)
  {
   extern const char *Hlp_ASSESSMENT_Tests;
   extern const char *Txt_Test;
   extern const char *Txt_You_can_not_take_a_new_test_until;
   extern const char *Txt_Today;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   long NumSecondsFromNowToNextAccTst = -1L;	// Access allowed when this number <= 0
   time_t TimeNextTestUTC = (time_t) 0;

   /***** Teachers and superusers are allowed to do all tests they want *****/
   if (Gbl.Usrs.Me.Role.Logged == Rol_TCH ||
       Gbl.Usrs.Me.Role.Logged == Rol_SYS_ADM)
      return true;

   /***** Get date of next allowed access to test from database *****/
   if (DB_QuerySELECT (&mysql_res,"can not get last access to test",
		       "SELECT UNIX_TIMESTAMP(LastAccTst+INTERVAL (NumQstsLastTst*%lu) SECOND)-"
			      "UNIX_TIMESTAMP(),"						// row[0]
			      "UNIX_TIMESTAMP(LastAccTst+INTERVAL (NumQstsLastTst*%lu) SECOND)"	// row[1]
		       " FROM crs_usr"
		       " WHERE CrsCod=%ld AND UsrCod=%ld",
		       Gbl.Test.Config.MinTimeNxtTstPerQst,
		       Gbl.Test.Config.MinTimeNxtTstPerQst,
		       Gbl.Hierarchy.Crs.CrsCod,Gbl.Usrs.Me.UsrDat.UsrCod) == 1)
     {
      /* Get seconds from now to next access to test */
      row = mysql_fetch_row (mysql_res);
      if (row[0])
         if (sscanf (row[0],"%ld",&NumSecondsFromNowToNextAccTst) == 1)
            /* Time UTC of next access allowed (row[1]) */
            TimeNextTestUTC = Dat_GetUNIXTimeFromStr (row[1]);
     }
   else
      Lay_ShowErrorAndExit ("Error when reading date of next allowed access to test.");

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   /***** Check if access is allowed *****/
   if (NumSecondsFromNowToNextAccTst > 0)
     {
      /***** Write warning *****/
      Ale_ShowAlert (Ale_WARNING,"%s:<br /><span id=\"date_next_test\"></span>."
		     "<script type=\"text/javascript\">"
		     "writeLocalDateHMSFromUTC('date_next_test',%ld,"
		     "%u,',&nbsp;','%s',true,true,0x7);"
		     "</script>",
		     Txt_You_can_not_take_a_new_test_until,
		     (long) TimeNextTestUTC,
		     (unsigned) Gbl.Prefs.DateFormat,Txt_Today);

      return false;
     }
   return true;
  }

/*****************************************************************************/
/****************************** Update test status ***************************/
/*****************************************************************************/

static void Tst_SetTstStatus (unsigned NumTst,Tst_Status_t TstStatus)
  {
   /***** Delete old status from expired sessions *****/
   DB_QueryDELETE ("can not remove old status of tests",
		   "DELETE FROM tst_status"
                   " WHERE SessionId NOT IN"
                   " (SELECT SessionId FROM sessions)");

   /***** Update database *****/
   DB_QueryREPLACE ("can not update status of test",
		    "REPLACE INTO tst_status"
	            " (SessionId,CrsCod,NumTst,Status)"
	            " VALUES"
	            " ('%s',%ld,%u,%u)",
		    Gbl.Session.Id,Gbl.Hierarchy.Crs.CrsCod,
		    NumTst,(unsigned) TstStatus);
  }

/*****************************************************************************/
/****************************** Update test status ***************************/
/*****************************************************************************/

static Tst_Status_t Tst_GetTstStatus (unsigned NumTst)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows;
   unsigned UnsignedNum;
   Tst_Status_t TstStatus = Tst_STATUS_ERROR;

   /***** Get status of test from database *****/
   NumRows = DB_QuerySELECT (&mysql_res,"can not get status of test",
			     "SELECT Status"		// row[0]
			     " FROM tst_status"
			     " WHERE SessionId='%s'"
			     " AND CrsCod=%ld"
			     " AND NumTst=%u",
			     Gbl.Session.Id,Gbl.Hierarchy.Crs.CrsCod,NumTst);

   if (NumRows == 1)
     {
      /* Get number of hits */
      row = mysql_fetch_row (mysql_res);
      if (row[0])
         if (sscanf (row[0],"%u",&UnsignedNum) == 1)
            if (UnsignedNum < Tst_NUM_STATUS)
               TstStatus = (Tst_Status_t) UnsignedNum;
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   return TstStatus;
  }

/*****************************************************************************/
/************************* Get number of hits to test ************************/
/*****************************************************************************/

static unsigned Tst_GetNumAccessesTst (void)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows;
   unsigned NumAccessesTst = 0;

   if (Gbl.Usrs.Me.IBelongToCurrentCrs)
     {
      /***** Get number of hits to test from database *****/
      NumRows = DB_QuerySELECT (&mysql_res,"can not get number of hits to test",
				"SELECT NumAccTst"	// row[0]
				" FROM crs_usr"
				" WHERE CrsCod=%ld AND UsrCod=%ld",
				Gbl.Hierarchy.Crs.CrsCod,
				Gbl.Usrs.Me.UsrDat.UsrCod);

      if (NumRows == 0)
         NumAccessesTst = 0;
      else if (NumRows == 1)
        {
         /* Get number of hits */
         row = mysql_fetch_row (mysql_res);
         if (row[0] == NULL)
            NumAccessesTst = 0;
         else if (sscanf (row[0],"%u",&NumAccessesTst) != 1)
            NumAccessesTst = 0;
        }
      else
         Lay_ShowErrorAndExit ("Error when getting number of hits to test.");

      /***** Free structure that stores the query result *****/
      DB_FreeMySQLResult (&mysql_res);
     }

   return NumAccessesTst;
  }

/*****************************************************************************/
/*************************** Write the test questions ************************/
/*****************************************************************************/
// NumRows must hold the number of rows of a MySQL query
// In each row mysql_res holds: in the column 0 the code of a question, in the column 1 the type of answer, and in the column 2 the stem

static void Tst_ShowTestQuestionsWhenSeeing (MYSQL_RES *mysql_res)
  {
   unsigned NumQst;
   long QstCod;
   MYSQL_ROW row;
   double ScoreThisQst;		// Not used here
   bool AnswerIsNotBlank;	// Not used here
   /*
   row[0] QstCod
   row[1] UNIX_TIMESTAMP(EditTime)
   row[2] AnsType
   row[3] Shuffle
   row[4] Stem
   row[5] Feedback
   row[6] MedCod
   row[7] NumHits
   row[8] NumHitsNotBlank
   row[9] Score
   */

   /***** Write rows *****/
   for (NumQst = 0;
	NumQst < Gbl.Test.NumQsts;
	NumQst++)
     {
      Gbl.RowEvenOdd = NumQst % 2;

      /***** Get the row next of the result of the query in the database *****/
      row = mysql_fetch_row (mysql_res);

      /***** Get the code of question (row[0]) *****/
      if ((QstCod = Str_ConvertStrCodToLongCod (row[0])) < 0)
         Lay_ShowErrorAndExit ("Wrong code of question.");

      Tst_WriteQstAndAnsTest (Tst_SHOW_TEST_TO_ANSWER,
			      &Gbl.Usrs.Me.UsrDat,
                              NULL,NumQst,QstCod,row,
	                      &ScoreThisQst,		// Not used here
	                      &AnswerIsNotBlank);	// Not used here
     }
  }

/*****************************************************************************/
/******************** Show test tags in this test result *********************/
/*****************************************************************************/

static void Tst_ShowTstTagsPresentInATestResult (long TstCod)
  {
   extern const char *Txt_no_tags;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows;
   unsigned long NumRow;

   /***** Get all tags of questions in this test *****/
   NumRows = DB_QuerySELECT (&mysql_res,"can not get tags"
					" present in a test result",
			     "SELECT tst_tags.TagTxt"	// row[0]
			     " FROM"
			     " (SELECT DISTINCT(tst_question_tags.TagCod)"
			     " FROM tst_question_tags,tst_exam_questions"
			     " WHERE tst_exam_questions.TstCod=%ld"
			     " AND tst_exam_questions.QstCod=tst_question_tags.QstCod)"
			     " AS TagsCods,tst_tags"
			     " WHERE TagsCods.TagCod=tst_tags.TagCod"
			     " ORDER BY tst_tags.TagTxt",
			     TstCod);
   if (NumRows)
     {
      /***** Write the tags *****/
      fprintf (Gbl.F.Out,"<ul>");
      for (NumRow = 0;
	   NumRow < NumRows;
	   NumRow++)
        {
         row = mysql_fetch_row (mysql_res);
         fprintf (Gbl.F.Out,"<li>%s</li>",
                  row[0]);
        }
      fprintf (Gbl.F.Out,"</ul>");
     }
   else
      fprintf (Gbl.F.Out,"%s",
               Txt_no_tags);

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/******************* Show the result of assessing a test *********************/
/*****************************************************************************/

static void Tst_ShowTestResultAfterAssess (long TstCod,unsigned *NumQstsNotBlank,double *TotalScore)
  {
   extern const char *Txt_Question_removed;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumQst;
   long QstCod;
   double ScoreThisQst;
   bool AnswerIsNotBlank;

   /***** Initialize score and number of questions not blank *****/
   *TotalScore = 0.0;
   *NumQstsNotBlank = 0;

   for (NumQst = 0;
	NumQst < Gbl.Test.NumQsts;
	NumQst++)
     {
      Gbl.RowEvenOdd = NumQst % 2;

      /***** Query database *****/
      if (Tst_GetOneQuestionByCod (Gbl.Test.QstCodes[NumQst],&mysql_res))	// Question exists
	{
	 /***** Get row of the result of the query *****/
	 row = mysql_fetch_row (mysql_res);
	 /*
	 row[0] QstCod
	 row[1] UNIX_TIMESTAMP(EditTime)
	 row[2] AnsType
	 row[3] Shuffle
	 row[4] Stem
	 row[5] Feedback
	 row[6] MedCod
	 row[7] NumHits
	 row[8] NumHitsNotBlank
	 row[9] Score
	 */

	 /***** Get the code of question (row[0]) *****/
	 if ((QstCod = Str_ConvertStrCodToLongCod (row[0])) < 0)
	    Lay_ShowErrorAndExit ("Wrong code of question.");

	 /***** Write question and answers *****/
	 Tst_WriteQstAndAnsTest (Tst_SHOW_TEST_RESULT,
	                         &Gbl.Usrs.Me.UsrDat,
				 NULL,NumQst,QstCod,row,
				 &ScoreThisQst,&AnswerIsNotBlank);

	 /***** Store test result question in database *****/
	 Tst_StoreOneTestResultQstInDB (TstCod,QstCod,
				        NumQst,	// 0, 1, 2, 3...
				        ScoreThisQst);

	 /***** Compute total score *****/
	 *TotalScore += ScoreThisQst;
	 if (AnswerIsNotBlank)
	    (*NumQstsNotBlank)++;

	 /***** Update the number of accesses and the score of this question *****/
	 if (Gbl.Usrs.Me.Role.Logged == Rol_STD)
	    Tst_UpdateScoreQst (QstCod,ScoreThisQst,AnswerIsNotBlank);
	}
      else
	 /***** Question does not exists *****/
         fprintf (Gbl.F.Out,"<tr>"
	                    "<td class=\"BIG_INDEX RIGHT_TOP COLOR%u\">"
	                    "%u"
	                    "</td>"
	                    "<td class=\"DAT_LIGHT LEFT_TOP COLOR%u\">"
	                    "%s"
	                    "</td>"
	                    "</tr>",
                  Gbl.RowEvenOdd,NumQst + 1,
                  Gbl.RowEvenOdd,Txt_Question_removed);

      /***** Free structure that stores the query result *****/
      DB_FreeMySQLResult (&mysql_res);
     }
  }

/*****************************************************************************/
/********** Write a row of a test, with one question and its answer **********/
/*****************************************************************************/

static void Tst_WriteQstAndAnsTest (Tst_ActionToDoWithQuestions_t ActionToDoWithQuestions,
			            struct UsrData *UsrDat,
                                    struct Game *Game,
                                    unsigned NumQst,long QstCod,MYSQL_ROW row,
                                    double *ScoreThisQst,bool *AnswerIsNotBlank)
  {
   extern const char *Txt_TST_STR_ANSWER_TYPES[Tst_NUM_ANS_TYPES];
   /*
   row[0] QstCod
   row[1] UNIX_TIMESTAMP(EditTime)
   row[2] AnsType
   row[3] Shuffle
   row[4] Stem
   row[5] Feedback
   row[6] MedCod
   row[7] NumHits
   row[8] NumHitsNotBlank
   row[9] Score
   */

   /***** Create test question *****/
   Tst_QstConstructor ();
   Gbl.Test.QstCod = QstCod;

   /***** Write number of question *****/
   fprintf (Gbl.F.Out,"<tr>"
	              "<td class=\"RIGHT_TOP COLOR%u\">"
	              "<div class=\"BIG_INDEX\">%u</div>",
            Gbl.RowEvenOdd,
            NumQst + 1);

   /***** Write answer type (row[2]) *****/
   Gbl.Test.AnswerType = Tst_ConvertFromStrAnsTypDBToAnsTyp (row[2]);
   fprintf (Gbl.F.Out,"<div class=\"DAT_SMALL\">%s</div>"
	              "</td>",
            Txt_TST_STR_ANSWER_TYPES[Gbl.Test.AnswerType]);

   /***** Write stem (row[4]) *****/
   fprintf (Gbl.F.Out,"<td class=\"LEFT_TOP COLOR%u\">",
            Gbl.RowEvenOdd);
   Tst_WriteQstStem (row[4],"TEST_EXA");

   /***** Get and show media (row[6]) *****/
   Gbl.Test.Media.MedCod = Str_ConvertStrCodToLongCod (row[6]);
   Med_GetMediaDataByCod (&Gbl.Test.Media);
   Med_ShowMedia (&Gbl.Test.Media,
                  "TEST_MED_SHOW_CONTAINER",
                  "TEST_MED_SHOW");

   /***** Write answers depending on shuffle (row[3]) and feedback (row[5])  *****/
   switch (ActionToDoWithQuestions)
     {
      case Tst_SHOW_TEST_TO_ANSWER:
         Tst_WriteAnswersTestToAnswer (NumQst,QstCod,(row[3][0] == 'Y'));
	 break;
      case Tst_SHOW_TEST_RESULT:
	 Tst_WriteAnswersTestResult (UsrDat,NumQst,QstCod,ScoreThisQst,AnswerIsNotBlank);

	 /* Write question feedback (row[5]) */
	 if (Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK)
	    Tst_WriteQstFeedback (row[5],"TEST_EXA_LIGHT");
	 break;
      case Tst_EDIT_TEST:
	 break;
      case Tst_SELECT_QUESTIONS_FOR_GAME:
	 break;
      case Tst_SHOW_GAME_TO_ANSWER:
	 Tst_WriteAnswersGameResult (Game,NumQst,QstCod,
	                             "GAM_PLAY_QST",false);	// Don't show result
	 break;
      case Tst_SHOW_GAME_RESULT:
	 Tst_WriteAnswersGameResult (Game,NumQst,QstCod,
	                             "GAM_PLAY_QST",true);	// Show result
	 break;
     }
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");

   /***** Destroy test question *****/
   Tst_QstDestructor ();
  }

/*****************************************************************************/
/********************* Write the stem of a test question *********************/
/*****************************************************************************/

void Tst_WriteQstStem (const char *Stem,const char *ClassStem)
  {
   unsigned long StemLength;
   char *StemRigorousHTML;

   /***** Convert the stem, that is in HTML, to rigorous HTML *****/
   StemLength = strlen (Stem) * Str_MAX_BYTES_PER_CHAR;
   if ((StemRigorousHTML = (char *) malloc (StemLength + 1)) == NULL)
      Lay_NotEnoughMemoryExit ();
   Str_Copy (StemRigorousHTML,Stem,
             StemLength);
   Str_ChangeFormat (Str_FROM_HTML,Str_TO_RIGOROUS_HTML,
	             StemRigorousHTML,StemLength,false);

   /***** Write the stem *****/
   fprintf (Gbl.F.Out,"<div class=\"%s\">"
	              "%s"
	              "</div>",
	    ClassStem,StemRigorousHTML);

   /***** Free memory allocated for the stem *****/
   free ((void *) StemRigorousHTML);
  }

/*****************************************************************************/
/************* Put form to upload a new image for a test question ************/
/*****************************************************************************/

static void Tst_PutFormToEditQstMedia (struct Media *Media,int NumMediaInForm,
                                       bool OptionsDisabled)
  {
   extern const char *The_ClassFormInBox[The_NUM_THEMES];
   extern const char *Txt_No_image_video;
   extern const char *Txt_Current_image_video;
   extern const char *Txt_Change_image_video;
   static unsigned UniqueId = 0;
   struct ParamUploadMedia ParamUploadMedia;

   if (Media->Name[0])
     {
      /***** Set names of parameters depending on number of image in form *****/
      Med_SetParamNames (&ParamUploadMedia,NumMediaInForm);

      /***** Start container *****/
      fprintf (Gbl.F.Out,"<div class=\"TEST_MED_EDIT_FORM\">");

      /***** Choice 1: No media *****/
      fprintf (Gbl.F.Out,"<label class=\"%s\">"
	                 "<input type=\"radio\" name=\"%s\" value=\"%u\"",
	       The_ClassFormInBox[Gbl.Prefs.Theme],
	       ParamUploadMedia.Action,Med_ACTION_NO_MEDIA);
      if (OptionsDisabled)
	 fprintf (Gbl.F.Out," disabled=\"disabled\"");
      fprintf (Gbl.F.Out," />"
			 "%s"
			 "</label><br />",
	       Txt_No_image_video);

      /***** Choice 2: Current media *****/
      fprintf (Gbl.F.Out,"<label class=\"%s\">"
	                 "<input type=\"radio\" name=\"%s\" value=\"%u\""
	                 " checked=\"checked\"",
	       The_ClassFormInBox[Gbl.Prefs.Theme],
	       ParamUploadMedia.Action,Med_ACTION_KEEP_MEDIA);
      if (OptionsDisabled)
	 fprintf (Gbl.F.Out," disabled=\"disabled\"");
      fprintf (Gbl.F.Out," />"
			 "%s"
			 "</label>",
	       Txt_Current_image_video);
      Med_ShowMedia (Media,
	             "TEST_MED_EDIT_ONE_CONTAINER",
		     "TEST_MED_EDIT_ONE");

      /***** Choice 3: Change media *****/
      UniqueId++;
      fprintf (Gbl.F.Out,"<label class=\"%s\">"
	                 "<input type=\"radio\" id=\"chg_img_%u\" name=\"%s\""
			 " value=\"%u\"",
	       The_ClassFormInBox[Gbl.Prefs.Theme],
	       UniqueId,ParamUploadMedia.Action,
	       Med_ACTION_NEW_MEDIA);	// Replace existing image by new image
      if (OptionsDisabled)
	 fprintf (Gbl.F.Out," disabled=\"disabled\"");
      fprintf (Gbl.F.Out," />"
			 "%s: "
			 "</label>",
	       Txt_Change_image_video);
      Med_PutMediaUploader (NumMediaInForm,"TEST_MED_INPUT");

      /***** End container *****/
      fprintf (Gbl.F.Out,"</div>");
     }
   else	// No current image
      /***** Attached media *****/
      Med_PutMediaUploader (NumMediaInForm,"TEST_MED_INPUT");
  }

/*****************************************************************************/
/******************* Write the feedback of a test question *******************/
/*****************************************************************************/

void Tst_WriteQstFeedback (const char *Feedback,const char *ClassFeedback)
  {
   unsigned long FeedbackLength;
   char *FeedbackRigorousHTML;

   if (Feedback)
      if (Feedback[0])
	{
	 /***** Convert the feedback, that is in HTML, to rigorous HTML *****/
	 FeedbackLength = strlen (Feedback) * Str_MAX_BYTES_PER_CHAR;
	 if ((FeedbackRigorousHTML = (char *) malloc (FeedbackLength + 1)) == NULL)
	    Lay_NotEnoughMemoryExit ();
	 Str_Copy (FeedbackRigorousHTML,Feedback,
	           FeedbackLength);
	 Str_ChangeFormat (Str_FROM_HTML,Str_TO_RIGOROUS_HTML,
			   FeedbackRigorousHTML,FeedbackLength,false);

	 /***** Write the feedback *****/
	 fprintf (Gbl.F.Out,"<div class=\"%s\">"
			    "%s"
			    "</div>",
		  ClassFeedback,FeedbackRigorousHTML);

	 /***** Free memory allocated for the feedback *****/
	 free ((void *) FeedbackRigorousHTML);
	}
  }

/*****************************************************************************/
/*********************** Update the score of a question **********************/
/*****************************************************************************/

static void Tst_UpdateScoreQst (long QstCod,float ScoreThisQst,bool AnswerIsNotBlank)
  {
   /***** Update number of clicks and score of the question *****/
   Str_SetDecimalPointToUS ();	// To print the floating point as a dot
   if (AnswerIsNotBlank)
      DB_QueryUPDATE ("can not update the score of a question",
		      "UPDATE tst_questions"
	              " SET NumHits=NumHits+1,NumHitsNotBlank=NumHitsNotBlank+1,"
	              "Score=Score+(%lf)"
                      " WHERE QstCod=%ld",
		      ScoreThisQst,QstCod);
   else	// The answer is blank
      DB_QueryUPDATE ("can not update the score of a question",
		      "UPDATE tst_questions"
	              " SET NumHits=NumHits+1"
                      " WHERE QstCod=%ld",
		      QstCod);
   Str_SetDecimalPointToLocal ();	// Return to local system
  }

/*****************************************************************************/
/*********** Update my number of accesses to test in this course *************/
/*****************************************************************************/

static void Tst_UpdateMyNumAccessTst (unsigned NumAccessesTst)
  {
   /***** Update my number of accesses to test in this course *****/
   DB_QueryUPDATE ("can not update the number of accesses to test",
		   "UPDATE crs_usr SET NumAccTst=%u"
                   " WHERE CrsCod=%ld AND UsrCod=%ld",
		   NumAccessesTst,
		   Gbl.Hierarchy.Crs.CrsCod,Gbl.Usrs.Me.UsrDat.UsrCod);
  }

/*****************************************************************************/
/************ Update date-time of my next allowed access to test *************/
/*****************************************************************************/

static void Tst_UpdateLastAccTst (void)
  {
   /***** Update date-time and number of questions of this test *****/
   DB_QueryUPDATE ("can not update time and number of questions of this test",
		   "UPDATE crs_usr SET LastAccTst=NOW(),NumQstsLastTst=%u"
                   " WHERE CrsCod=%ld AND UsrCod=%ld",
		   Gbl.Test.NumQsts,
		   Gbl.Hierarchy.Crs.CrsCod,
		   Gbl.Usrs.Me.UsrDat.UsrCod);
  }

/*****************************************************************************/
/******* Select tags and dates for edition of the self-assessment test *******/
/*****************************************************************************/

void Tst_ShowFormAskEditTsts (void)
  {
   extern const char *Hlp_ASSESSMENT_Tests;
   extern const char *Txt_No_test_questions;
   extern const char *Txt_List_edit_questions;
   extern const char *Txt_Show_questions;
   MYSQL_RES *mysql_res;
   unsigned long NumRows;

   /***** Contextual menu *****/
   fprintf (Gbl.F.Out,"<div class=\"CONTEXT_MENU\">");
   TsI_PutFormToImportQuestions ();	// Put link (form) to import questions from XML file
   fprintf (Gbl.F.Out,"</div>");

   /***** Start box *****/
   Box_StartBox (NULL,Txt_List_edit_questions,Tst_PutIconsTests,
                 Hlp_ASSESSMENT_Tests,Box_NOT_CLOSABLE);

   /***** Get tags already present in the table of questions *****/
   if ((NumRows = Tst_GetAllTagsFromCurrentCrs (&mysql_res)))
     {
      Frm_StartForm (ActLstTstQst);
      Par_PutHiddenParamUnsigned ("Order",(unsigned) Tst_ORDER_STEM);

      Tbl_StartTable (2);

      /***** Selection of tags *****/
      Tst_ShowFormSelTags (NumRows,mysql_res,false,2);

      /***** Selection of types of answers *****/
      Tst_ShowFormAnswerTypes (2);

      /***** Starting and ending dates in the search *****/
      Dat_PutFormStartEndClientLocalDateTimesWithYesterdayToday (false);

      Tbl_EndTable ();

      /***** Send button *****/
      Btn_PutConfirmButton (Txt_Show_questions);
      Frm_EndForm ();
     }
   else	// No test questions
     {
      /***** Warning message *****/
      Ale_ShowAlert (Ale_INFO,Txt_No_test_questions);

      /***** Button to create a new question *****/
      Tst_PutButtonToAddQuestion ();
     }

   /***** End box *****/
   Box_EndBox ();

   /* Free structure that stores the query result */
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/************** Show form select test questions for a game *******************/
/*****************************************************************************/

void Tst_ShowFormAskSelectTstsForGame (long GamCod)
  {
   extern const char *Hlp_ASSESSMENT_Tests;
   extern const char *Txt_No_test_questions;
   extern const char *Txt_Select_questions;
   extern const char *Txt_Show_questions;
   MYSQL_RES *mysql_res;
   unsigned long NumRows;

   /***** Start box *****/
   Box_StartBox (NULL,Txt_Select_questions,NULL,
                 Hlp_ASSESSMENT_Tests,Box_NOT_CLOSABLE);

   /***** Get tags already present in the table of questions *****/
   if ((NumRows = Tst_GetAllTagsFromCurrentCrs (&mysql_res)))
     {
      Frm_StartForm (ActGamLstTstQst);
      Gam_PutParamGameCod (GamCod);

      Tbl_StartTable (2);

      /***** Selection of tags *****/
      Tst_ShowFormSelTags (NumRows,mysql_res,false,2);

      /***** Starting and ending dates in the search *****/
      Dat_PutFormStartEndClientLocalDateTimesWithYesterdayToday (false);

      Tbl_EndTable ();

      /***** Send button *****/
      Btn_PutConfirmButton (Txt_Show_questions);
      Frm_EndForm ();
     }
   else	// No test questions
     {
      /***** Warning message *****/
      Ale_ShowAlert (Ale_INFO,Txt_No_test_questions);

      /***** Button to create a new question *****/
      Tst_PutButtonToAddQuestion ();
     }

   /***** End box *****/
   Box_EndBox ();

   /* Free structure that stores the query result */
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/************************* Check if I can edit tests *************************/
/*****************************************************************************/

static bool Tst_CheckIfICanEditTests (void)
  {
   return (bool) (Gbl.Usrs.Me.Role.Logged == Rol_TCH ||
                  Gbl.Usrs.Me.Role.Logged == Rol_SYS_ADM);
  }

/*****************************************************************************/
/********************* Put contextual icons in tests *************************/
/*****************************************************************************/

static void Tst_PutIconsTests (void)
  {
   extern const char *Txt_New_question;
   extern const char *Txt_Configure;

   if (Tst_CheckIfICanEditTests ())
     {
      /***** Put form to edit existing test questions *****/
      if (Gbl.Action.Act != ActEdiTstQst)
         Ico_PutContextualIconToEdit (ActEdiTstQst,NULL);

      /***** Put form to create a new test question *****/
      if (Gbl.Action.Act != ActEdiOneTstQst)
	 Ico_PutContextualIconToAdd (ActEdiOneTstQst,NULL,NULL,
				     Txt_New_question);

      /***** Put form to go to test configuration *****/
      if (Gbl.Action.Act != ActCfgTst)
	 Lay_PutContextualLinkOnlyIcon (ActCfgTst,NULL,NULL,
				        "cog.svg",
					Txt_Configure);
     }

   /***** Put icon to show a figure *****/
   Gbl.Figures.FigureType = Fig_TESTS;
   Fig_PutIconToShowFigure ();
  }

/*****************************************************************************/
/**************** Put button to create a new test question *******************/
/*****************************************************************************/

static void Tst_PutButtonToAddQuestion (void)
  {
   extern const char *Txt_New_question;

   Frm_StartForm (ActEdiOneTstQst);
   Btn_PutConfirmButton (Txt_New_question);
   Frm_EndForm ();
  }

/*****************************************************************************/
/***************************** Form to rename tags ***************************/
/*****************************************************************************/

void Tst_ShowFormConfig (void)
  {
   extern const char *Txt_Please_specify_if_you_allow_access_to_test_questions_from_mobile_applications;

   /***** If current course has tests and pluggable is unknown... *****/
   if (Tst_CheckIfCourseHaveTestsAndPluggableIsUnknown ())
      Ale_ShowAlert (Ale_WARNING,Txt_Please_specify_if_you_allow_access_to_test_questions_from_mobile_applications);

   /***** Form to configure test *****/
   Tst_ShowFormConfigTst ();

   /***** Form to edit tags *****/
   Tst_ShowFormEditTags ();
  }

/*****************************************************************************/
/******************************* Enable a test tag ***************************/
/*****************************************************************************/

void Tst_EnableTag (void)
  {
   long TagCod = Tst_GetParamTagCode ();

   /***** Change tag status to enabled *****/
   Tst_EnableOrDisableTag (TagCod,false);

   /***** Show again the form to configure test *****/
   Tst_ShowFormConfig ();
  }

/*****************************************************************************/
/****************************** Disable a test tag ***************************/
/*****************************************************************************/

void Tst_DisableTag (void)
  {
   long TagCod = Tst_GetParamTagCode ();

   /***** Change tag status to disabled *****/
   Tst_EnableOrDisableTag (TagCod,true);

   /***** Show again the form to configure test *****/
   Tst_ShowFormConfig ();
  }

/*****************************************************************************/
/************************* Get parameter with tag code ***********************/
/*****************************************************************************/

static long Tst_GetParamTagCode (void)
  {
   long TagCod;

   /***** Get tag code *****/
   if ((TagCod = Par_GetParToLong ("TagCod")) <= 0)
      Lay_ShowErrorAndExit ("Code of tag is missing.");

   return TagCod;
  }

/*****************************************************************************/
/************************ Rename a tag of test questions *********************/
/*****************************************************************************/

void Tst_RenameTag (void)
  {
   extern const char *Txt_You_can_not_leave_the_name_of_the_tag_X_empty;
   extern const char *Txt_The_tag_X_has_been_renamed_as_Y;
   extern const char *Txt_The_tag_X_has_not_changed;
   char OldTagTxt[Tst_MAX_BYTES_TAG + 1];
   char NewTagTxt[Tst_MAX_BYTES_TAG + 1];
   long ExistingTagCod;
   long OldTagCod;
   bool ComplexRenaming;

   /***** Get old and new tags from the form *****/
   Par_GetParToText ("OldTagTxt",OldTagTxt,Tst_MAX_BYTES_TAG);
   Par_GetParToText ("NewTagTxt",NewTagTxt,Tst_MAX_BYTES_TAG);

   /***** Check that the new tag is not empty *****/
   if (!NewTagTxt[0])	// New tag empty
      Ale_ShowAlert (Ale_WARNING,Txt_You_can_not_leave_the_name_of_the_tag_X_empty,
                     OldTagTxt);
   else			// New tag not empty
     {
      /***** Check if the old tag is equal to the new one *****/
      if (!strcmp (OldTagTxt,NewTagTxt))	// The old and the new tag
						// are exactly the same (case sensitively).
						// This happens when user press INTRO
						// without changing anything in the form.
         Ale_ShowAlert (Ale_INFO,Txt_The_tag_X_has_not_changed,
                        NewTagTxt);
      else					// The old and the new tag
						// are not exactly the same (case sensitively).
	{
	 /***** Check if renaming is complex or easy *****/
	 ComplexRenaming = false;
	 if (strcasecmp (OldTagTxt,NewTagTxt))	// The old and the new tag
						// are not the same (case insensitively)
	    /* Check if the new tag text is equal to any of the tags
	       already present in the database */
	    if ((ExistingTagCod = Tst_GetTagCodFromTagTxt (NewTagTxt)) > 0)
	       // The new tag was already in database
	       ComplexRenaming = true;

	 if (ComplexRenaming)	// Renaming is not easy
	   {
	    /***** Complex update made to not repeat tags:
		   - If the new tag existed for a question ==>
		     delete old tag from tst_question_tags;
		     the new tag will remain
		   - If the new tag did not exist for a question ==>
		     change old tag to new tag in tst_question_tags *****/
	    /* Get tag code of the old tag */
	    if ((OldTagCod =  Tst_GetTagCodFromTagTxt (OldTagTxt)) < 0)
	       Lay_ShowErrorAndExit ("Tag does not exists.");

	    /* Create a temporary table with all the question codes
	       that had the new tag as one of their tags */
	    DB_Query ("can not remove temporary table",
		      "DROP TEMPORARY TABLE IF EXISTS tst_question_tags_tmp");

	    DB_Query ("can not create temporary table",
		      "CREATE TEMPORARY TABLE tst_question_tags_tmp"
		      " ENGINE=MEMORY"
		      " SELECT QstCod FROM tst_question_tags"
	   	      " WHERE TagCod=%ld",
		      ExistingTagCod);

	    /* Remove old tag in questions where it would be repeated */
	    // New tag existed for a question ==> delete old tag
	    DB_QueryDELETE ("can not remove a tag from some questions",
			    "DELETE FROM tst_question_tags"
			    " WHERE TagCod=%ld"
			    " AND QstCod IN"
			    " (SELECT QstCod FROM tst_question_tags_tmp)",
			    OldTagCod);

	    /* Change old tag to new tag in questions where it would not be repeated */
	    // New tag did not exist for a question ==> change old tag to new tag
	    DB_QueryUPDATE ("can not update a tag in some questions",
			    "UPDATE tst_question_tags"
			    " SET TagCod=%ld"
			    " WHERE TagCod=%ld"
			    " AND QstCod NOT IN"
			    " (SELECT QstCod FROM tst_question_tags_tmp)",
			    ExistingTagCod,
			    OldTagCod);

	    /* Drop temporary table, no longer necessary */
	    DB_Query ("can not remove temporary table",
		      "DROP TEMPORARY TABLE IF EXISTS tst_question_tags_tmp");

	    /***** Delete old tag from tst_tags
		   because it is not longer used *****/
	    DB_QueryDELETE ("can not remove old tag",
			    "DELETE FROM tst_tags WHERE TagCod=%ld",
			    OldTagCod);
	   }
	 else			// Renaming is easy
	   {
	    /***** Simple update replacing each instance of the old tag by the new tag *****/
	    DB_QueryUPDATE ("can not update tag",
			    "UPDATE tst_tags SET TagTxt='%s',ChangeTime=NOW()"
			    " WHERE tst_tags.CrsCod=%ld"
			    " AND tst_tags.TagTxt='%s'",
			    NewTagTxt,Gbl.Hierarchy.Crs.CrsCod,OldTagTxt);
	   }

	 /***** Write message to show the change made *****/
	 Ale_ShowAlert (Ale_SUCCESS,Txt_The_tag_X_has_been_renamed_as_Y,
		        OldTagTxt,NewTagTxt);
	}
     }

   /***** Show again the form to configure test *****/
   Tst_ShowFormConfig ();
  }

/*****************************************************************************/
/******************* Check if current course has test tags *******************/
/*****************************************************************************/
// Return the number of rows of the result

static bool Tst_CheckIfCurrentCrsHasTestTags (void)
  {
   /***** Get available tags from database *****/
   return (DB_QueryCOUNT ("can not check if course has tags",
			  "SELECT COUNT(*) FROM tst_tags"
			  " WHERE CrsCod=%ld",
			  Gbl.Hierarchy.Crs.CrsCod) != 0);
  }

/*****************************************************************************/
/********* Get all (enabled or disabled) test tags for this course ***********/
/*****************************************************************************/
// Return the number of rows of the result

static unsigned long Tst_GetAllTagsFromCurrentCrs (MYSQL_RES **mysql_res)
  {
   /***** Get available tags from database *****/
   return DB_QuerySELECT (mysql_res,"can not get available tags",
			  "SELECT TagCod,"	// row[0]
			         "TagTxt,"	// row[1]
			         "TagHidden"	// row[2]
			  " FROM tst_tags"
			  " WHERE CrsCod=%ld"
			  " ORDER BY TagTxt",
			  Gbl.Hierarchy.Crs.CrsCod);
  }

/*****************************************************************************/
/********************** Get enabled test tags for this course ****************/
/*****************************************************************************/
// Return the number of rows of the result

static unsigned long Tst_GetEnabledTagsFromThisCrs (MYSQL_RES **mysql_res)
  {
   /***** Get available not hidden tags from database *****/
   return DB_QuerySELECT (mysql_res,"can not get available enabled tags",
			  "SELECT TagCod,TagTxt FROM tst_tags"
			  " WHERE CrsCod=%ld AND TagHidden='N'"
			  " ORDER BY TagTxt",
			  Gbl.Hierarchy.Crs.CrsCod);
  }

/*****************************************************************************/
/********************* Show a form to select test tags ***********************/
/*****************************************************************************/

static void Tst_ShowFormSelTags (unsigned long NumRows,MYSQL_RES *mysql_res,
                                 bool ShowOnlyEnabledTags,unsigned NumCols)
  {
   extern const char *The_ClassFormInBox[The_NUM_THEMES];
   extern const char *Txt_Tags;
   extern const char *Txt_All_tags;
   extern const char *Txt_Tag_not_allowed;
   extern const char *Txt_Tag_allowed;
   unsigned long NumRow;
   MYSQL_ROW row;
   bool TagHidden = false;
   const char *Ptr;
   char TagText[Tst_MAX_BYTES_TAG + 1];
   /*
   row[0] TagCod
   row[1] TagTxt
   row[2] TagHidden
   */

   /***** Label *****/
   fprintf (Gbl.F.Out,"<tr>"
		      "<td class=\"RIGHT_TOP %s\">"
		      "%s:"
                      "</td>",
	    The_ClassFormInBox[Gbl.Prefs.Theme],Txt_Tags);

   /***** Select all tags *****/
   fprintf (Gbl.F.Out,"<td");
   if (NumCols > 1)
      fprintf (Gbl.F.Out," colspan=\"%u\"",NumCols);
   fprintf (Gbl.F.Out," class=\"LEFT_TOP\">");
   Tbl_StartTable (2);
   fprintf (Gbl.F.Out,"<tr>");
   if (!ShowOnlyEnabledTags)
      fprintf (Gbl.F.Out,"<td></td>");
   fprintf (Gbl.F.Out,"<td class=\"LEFT_MIDDLE\">"
                      "<label class=\"%s\">"
	              "<input type=\"checkbox\" name=\"AllTags\" value=\"Y\"",
            The_ClassFormInBox[Gbl.Prefs.Theme]);
   if (Gbl.Test.Tags.All)
      fprintf (Gbl.F.Out," checked=\"checked\"");
   fprintf (Gbl.F.Out," onclick=\"togglecheckChildren(this,'ChkTag');\" />"
                      "&nbsp;%s"
                      "</label>"
                      "</td>"
                      "</tr>",
            Txt_All_tags);

   /***** Select tags one by one *****/
   for (NumRow = 1;
	NumRow <= NumRows;
	NumRow++)
     {
      row = mysql_fetch_row (mysql_res);
      fprintf (Gbl.F.Out,"<tr>");
      if (!ShowOnlyEnabledTags)
        {
         TagHidden = (row[2][0] == 'Y');
         fprintf (Gbl.F.Out,"<td class=\"LEFT_MIDDLE\">"
                            "<img src=\"%s/",
                  Cfg_URL_ICON_PUBLIC);
         if (TagHidden)
            fprintf (Gbl.F.Out,"eye-slash.svg\" alt=\"%s\" title=\"%s",
                     Txt_Tag_not_allowed,
                     Txt_Tag_not_allowed);
         else
            fprintf (Gbl.F.Out,"eye.svg\" alt=\"%s\" title=\"%s",
                     Txt_Tag_allowed,
                     Txt_Tag_allowed);
         fprintf (Gbl.F.Out,"\" class=\"ICO_HIDDEN ICO16x16\" />"
                            "</td>");
        }
      fprintf (Gbl.F.Out,"<td class=\"LEFT_MIDDLE\">"
                         "<label class=\"DAT\">"
	                 "<input type=\"checkbox\" name=\"ChkTag\" value=\"%s\"",
	       row[1]);
      if (Gbl.Test.Tags.List)
        {
         Ptr = Gbl.Test.Tags.List;
         while (*Ptr)
           {
            Par_GetNextStrUntilSeparParamMult (&Ptr,TagText,Tst_MAX_BYTES_TAG);
            if (!strcmp (row[1],TagText))
               fprintf (Gbl.F.Out," checked=\"checked\"");
           }
        }
      fprintf (Gbl.F.Out," onclick=\"checkParent(this,'AllTags');\" />"
	                 "&nbsp;%s"
	                 "</label>"
	                 "</td>"
	                 "</tr>",
	       row[1]);
     }

   Tbl_EndTable ();
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");
  }

/*****************************************************************************/
/************* Show a form to enable/disable and rename test tags ************/
/*****************************************************************************/

static void Tst_ShowFormEditTags (void)
  {
   extern const char *Hlp_ASSESSMENT_Tests_writing_a_question;
   extern const char *Txt_No_test_questions;
   extern const char *Txt_Tags;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRow,NumRows;
   long TagCod;

   /***** Get current tags in current course *****/
   if ((NumRows = Tst_GetAllTagsFromCurrentCrs (&mysql_res)))
     {
      /***** Start box and table *****/
      Box_StartBoxTable (NULL,Txt_Tags,NULL,
                         Hlp_ASSESSMENT_Tests_writing_a_question,Box_NOT_CLOSABLE,2);

      /***** Show tags *****/
      for (NumRow = 0;
	   NumRow < NumRows;
	   NumRow++)
        {
         row = mysql_fetch_row (mysql_res);
	 /*
	 row[0] TagCod
	 row[1] TagTxt
	 row[2] TagHidden
	 */
         if ((TagCod = Str_ConvertStrCodToLongCod (row[0])) < 0)
            Lay_ShowErrorAndExit ("Wrong code of tag.");

         fprintf (Gbl.F.Out,"<tr>");

         /* Form to enable / disable this tag */
         if (row[2][0] == 'Y')	// Tag disabled
            Tst_PutIconEnable (TagCod,row[1]);
         else
            Tst_PutIconDisable (TagCod,row[1]);

         /* Form to rename this tag */
         fprintf (Gbl.F.Out,"<td class=\"LEFT_MIDDLE\">");
         Frm_StartForm (ActRenTag);
         Par_PutHiddenParamString ("OldTagTxt",row[1]);
         fprintf (Gbl.F.Out,"<input type=\"text\" name=\"NewTagTxt\""
                            " size=\"36\" maxlength=\"%u\" value=\"%s\""
                            " onchange=\"document.getElementById('%s').submit();\" />",
                  Tst_MAX_CHARS_TAG,row[1],Gbl.Form.Id);
         Frm_EndForm ();
         fprintf (Gbl.F.Out,"</td>"
                            "</tr>");
        }

      /***** End table and box *****/
      Box_EndBoxTable ();
     }
   else
      Ale_ShowAlert (Ale_INFO,Txt_No_test_questions);

   /* Free structure that stores the query result */
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/******************* Put a link and an icon to enable a tag ******************/
/*****************************************************************************/

static void Tst_PutIconEnable (long TagCod,const char *TagTxt)
  {
   extern const char *Txt_Tag_X_not_allowed_Click_to_allow_it;

   fprintf (Gbl.F.Out,"<td class=\"BM\">");
   Frm_StartForm (ActEnableTag);
   Par_PutHiddenParamLong ("TagCod",TagCod);
   snprintf (Gbl.Title,sizeof (Gbl.Title),
	     Txt_Tag_X_not_allowed_Click_to_allow_it,
	     TagTxt);
   Ico_PutIconLink ("eye-slash.svg",Gbl.Title);
   Frm_EndForm ();
   fprintf (Gbl.F.Out,"</td>");
  }

/*****************************************************************************/
/****************** Put a link and an icon to disable a tag ******************/
/*****************************************************************************/

static void Tst_PutIconDisable (long TagCod,const char *TagTxt)
  {
   extern const char *Txt_Tag_X_allowed_Click_to_disable_it;

   fprintf (Gbl.F.Out,"<td class=\"BM\">");
   Frm_StartForm (ActDisableTag);
   Par_PutHiddenParamLong ("TagCod",TagCod);
   snprintf (Gbl.Title,sizeof (Gbl.Title),
	     Txt_Tag_X_allowed_Click_to_disable_it,
	     TagTxt);
   Ico_PutIconLink ("eye.svg",Gbl.Title);
   Frm_EndForm ();
   fprintf (Gbl.F.Out,"</td>");
  }

/*****************************************************************************/
/********************* Show a form to to configure test **********************/
/*****************************************************************************/

static void Tst_ShowFormConfigTst (void)
  {
   extern const char *Hlp_ASSESSMENT_Tests;
   extern const char *The_ClassFormInBox[The_NUM_THEMES];
   extern const char *Txt_Configure_tests;
   extern const char *Txt_Plugins;
   extern const char *Txt_TST_PLUGGABLE[Tst_NUM_OPTIONS_PLUGGABLE];
   extern const char *Txt_No_of_questions;
   extern const char *Txt_minimum;
   extern const char *Txt_default;
   extern const char *Txt_maximum;
   extern const char *Txt_Minimum_time_seconds_per_question_between_two_tests;
   extern const char *Txt_Feedback_to_students;
   extern const char *Txt_TST_STR_FEEDBACK[Tst_NUM_TYPES_FEEDBACK];
   extern const char *Txt_Save_changes;
   Tst_Pluggable_t Pluggable;
   Tst_Feedback_t Feedback;

   /***** Read test configuration from database *****/
   Tst_GetConfigTstFromDB ();

   /***** Start box *****/
   Box_StartBox (NULL,Txt_Configure_tests,Tst_PutIconsTests,
                 Hlp_ASSESSMENT_Tests,Box_NOT_CLOSABLE);

   /***** Start form *****/
   Frm_StartForm (ActRcvCfgTst);

   /***** Tests are visible from plugins? *****/
   Tbl_StartTable (2);
   fprintf (Gbl.F.Out,"<tr>"
	              "<td class=\"%s RIGHT_TOP\">"
	              "%s:"
	              "</td>"
	              "<td class=\"LEFT_BOTTOM\">",
            The_ClassFormInBox[Gbl.Prefs.Theme],
            Txt_Plugins);
   for (Pluggable = Tst_PLUGGABLE_NO;
	Pluggable <= Tst_PLUGGABLE_YES;
	Pluggable++)
     {
      fprintf (Gbl.F.Out,"<label class=\"DAT\">"
	                 "<input type=\"radio\" name=\"Pluggable\" value=\"%u\"",
	       (unsigned) Pluggable);
      if (Pluggable == Gbl.Test.Config.Pluggable)
         fprintf (Gbl.F.Out," checked=\"checked\"");
      fprintf (Gbl.F.Out," />"
	                 "%s"
	                 "</label><br />",
               Txt_TST_PLUGGABLE[Pluggable]);
     }
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");

   /***** Number of questions *****/
   fprintf (Gbl.F.Out,"<tr>"
	              "<td class=\"%s RIGHT_TOP\">"
	              "%s:"
	              "</td>"
                      "<td class=\"LEFT_BOTTOM\">",
            The_ClassFormInBox[Gbl.Prefs.Theme],
            Txt_No_of_questions);
   Tbl_StartTable (2);
   Tst_PutInputFieldNumQst ("NumQstMin",Txt_minimum,
                            Gbl.Test.Config.Min);	// Minimum number of questions
   Tst_PutInputFieldNumQst ("NumQstDef",Txt_default,
                            Gbl.Test.Config.Def);	// Default number of questions
   Tst_PutInputFieldNumQst ("NumQstMax",Txt_maximum,
                            Gbl.Test.Config.Max);	// Maximum number of questions
   Tbl_EndTable ();
   fprintf (Gbl.F.Out,"</td>"
                      "</tr>");

   /***** Minimum time between consecutive tests, per question *****/
   fprintf (Gbl.F.Out,"<tr>"
	              "<td class=\"RIGHT_TOP\">"
	              "<label for=\"MinTimeNxtTstPerQst\" class=\"%s\">"
	              "%s:"
	              "</label>"
	              "</td>"
                      "<td class=\"LEFT_BOTTOM\">"
                      "<input type=\"text\""
                      " id=\"MinTimeNxtTstPerQst\" name=\"MinTimeNxtTstPerQst\""
                      " size=\"7\" maxlength=\"7\" value=\"%lu\""
                      " required=\"required\" />"
                      "</td>"
                      "</tr>",
            The_ClassFormInBox[Gbl.Prefs.Theme],
            Txt_Minimum_time_seconds_per_question_between_two_tests,
            Gbl.Test.Config.MinTimeNxtTstPerQst);

   /***** Feedback to students *****/
   fprintf (Gbl.F.Out,"<tr>"
	              "<td class=\"%s RIGHT_TOP\">"
	              "%s:"
	              "</td>"
	              "<td class=\"LEFT_BOTTOM\">",
            The_ClassFormInBox[Gbl.Prefs.Theme],Txt_Feedback_to_students);
   for (Feedback = (Tst_Feedback_t) 0;
	Feedback < Tst_NUM_TYPES_FEEDBACK;
	Feedback++)
     {
      fprintf (Gbl.F.Out,"<label class=\"DAT\">"
	                 "<input type=\"radio\" name=\"Feedback\" value=\"%u\"",
	       (unsigned) Feedback);
      if (Feedback == Gbl.Test.Config.Feedback)
         fprintf (Gbl.F.Out," checked=\"checked\"");
      fprintf (Gbl.F.Out," />"
	                 "%s"
	                 "</label><br />",
               Txt_TST_STR_FEEDBACK[Feedback]);
     }
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");
   Tbl_EndTable ();

   /***** Send button *****/
   Btn_PutConfirmButton (Txt_Save_changes);

   /***** End form *****/
   Frm_EndForm ();

   /***** End box *****/
   Box_EndBox ();
  }

/*****************************************************************************/
/*************** Get configuration of test for current course ****************/
/*****************************************************************************/

static void Tst_PutInputFieldNumQst (const char *Field,const char *Label,
                                     unsigned Value)
  {
   fprintf (Gbl.F.Out,"<tr>"
	              "<td class=\"RIGHT_MIDDLE\">"
	              "<label for=\"%s\" class=\"DAT\">%s</label>"
	              "</td>"
                      "<td class=\"LEFT_MIDDLE\">"
                      "<input type=\"text\""
                      " id=\"%s\" name=\"%s\""
                      " size=\"3\" maxlength=\"3\" value=\"%u\""
                      " required=\"required\" />"
                      "</td>"
                      "</tr>",
           Field,Label,
           Field,Field,
           Value);
  }

/*****************************************************************************/
/*************** Get configuration of test for current course ****************/
/*****************************************************************************/

static void Tst_GetConfigTstFromDB (void)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows;

   /***** Get configuration of test for current course from database *****/
   NumRows = DB_QuerySELECT (&mysql_res,"can not get configuration of test",
			     "SELECT Pluggable,"		// row[0]
				    "Min,"			// row[1]
				    "Def,"			// row[2]
				    "Max,"			// row[3]
				    "MinTimeNxtTstPerQst,"	// row[4]
				    "Feedback"			// row[5]
			     " FROM tst_config WHERE CrsCod=%ld",
			     Gbl.Hierarchy.Crs.CrsCod);

   Gbl.Test.Config.Feedback = Tst_FEEDBACK_DEFAULT;
   Gbl.Test.Config.MinTimeNxtTstPerQst = 0UL;
   if (NumRows == 0)
     {
      Gbl.Test.Config.Pluggable = Tst_PLUGGABLE_UNKNOWN;
      Gbl.Test.Config.Min = Tst_CONFIG_DEFAULT_MIN_QUESTIONS;
      Gbl.Test.Config.Def = Tst_CONFIG_DEFAULT_DEF_QUESTIONS;
      Gbl.Test.Config.Max = Tst_CONFIG_DEFAULT_MAX_QUESTIONS;
     }
   else // NumRows == 1
     {
      /***** Get minimun, default and maximum *****/
      row = mysql_fetch_row (mysql_res);
      Tst_GetConfigFromRow (row);
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/************ Get configuration values from a database table row *************/
/*****************************************************************************/

void Tst_GetConfigFromRow (MYSQL_ROW row)
  {
   int IntNum;
   long LongNum;
   Tst_Pluggable_t Pluggable;
   Tst_Feedback_t Feedback;

   /***** Get whether test are visible via plugins or not *****/
   Gbl.Test.Config.Pluggable = Tst_PLUGGABLE_UNKNOWN;
   for (Pluggable = Tst_PLUGGABLE_NO;
	Pluggable <= Tst_PLUGGABLE_YES;
	Pluggable++)
      if (!strcmp (row[0],Tst_PluggableDB[Pluggable]))
        {
         Gbl.Test.Config.Pluggable = Pluggable;
         break;
        }

   /***** Get number of questions *****/
   if (sscanf (row[1],"%d",&IntNum) == 1)
      Gbl.Test.Config.Min = (IntNum < 1) ? 1 :
	                                   (unsigned) IntNum;
   else
      Gbl.Test.Config.Min = Tst_CONFIG_DEFAULT_MIN_QUESTIONS;

   if (sscanf (row[2],"%d",&IntNum) == 1)
      Gbl.Test.Config.Def = (IntNum < 1) ? 1 :
	                                   (unsigned) IntNum;
   else
      Gbl.Test.Config.Def = Tst_CONFIG_DEFAULT_DEF_QUESTIONS;

   if (sscanf (row[3],"%d",&IntNum) == 1)
      Gbl.Test.Config.Max = (IntNum < 1) ? 1 :
	                                   (unsigned) IntNum;
   else
      Gbl.Test.Config.Max = Tst_CONFIG_DEFAULT_MAX_QUESTIONS;

   /***** Check and correct numbers *****/
   Tst_CheckAndCorrectNumbersQst ();

   /***** Get minimum time between consecutive tests, per question (row[4]) *****/
   if (sscanf (row[4],"%ld",&LongNum) == 1)
      Gbl.Test.Config.MinTimeNxtTstPerQst = (LongNum < 1L) ? 0UL :
	                                                     (unsigned long) LongNum;

   /***** Get feedback type (row[5]) *****/
   for (Feedback = (Tst_Feedback_t) 0;
	Feedback < Tst_NUM_TYPES_FEEDBACK;
	Feedback++)
      if (!strcmp (row[5],Tst_FeedbackDB[Feedback]))
        {
         Gbl.Test.Config.Feedback = Feedback;
         break;
        }
  }

/*****************************************************************************/
/*************** Get configuration of test for current course ****************/
/*****************************************************************************/
// Returns true if course has test tags and pluggable is unknown
// Return false if course has no test tags or pluggable is known

bool Tst_CheckIfCourseHaveTestsAndPluggableIsUnknown (void)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows;
   Tst_Pluggable_t Pluggable;

   /***** Get pluggability of tests for current course from database *****/
   NumRows = DB_QuerySELECT (&mysql_res,"can not get configuration of test",
			     "SELECT Pluggable"		// row[0]
			     " FROM tst_config"
			     " WHERE CrsCod=%ld",
			     Gbl.Hierarchy.Crs.CrsCod);

   if (NumRows == 0)
      Gbl.Test.Config.Pluggable = Tst_PLUGGABLE_UNKNOWN;
   else // NumRows == 1
     {
      /***** Get whether test are visible via plugins or not *****/
      row = mysql_fetch_row (mysql_res);

      Gbl.Test.Config.Pluggable = Tst_PLUGGABLE_UNKNOWN;
      for (Pluggable = Tst_PLUGGABLE_NO;
	   Pluggable <= Tst_PLUGGABLE_YES;
	   Pluggable++)
         if (!strcmp (row[0],Tst_PluggableDB[Pluggable]))
           {
            Gbl.Test.Config.Pluggable = Pluggable;
            break;
           }
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   /***** Get if current course has tests from database *****/
   if (Gbl.Test.Config.Pluggable == Tst_PLUGGABLE_UNKNOWN)
      return Tst_CheckIfCurrentCrsHasTestTags ();	// Return true if course has tests

   return false;	// Pluggable is not unknown
  }

/*****************************************************************************/
/************* Receive configuration of test for current course **************/
/*****************************************************************************/

void Tst_ReceiveConfigTst (void)
  {
   extern const char *Txt_The_test_configuration_has_been_updated;

   /***** Get whether test are visible via plugins or not *****/
   Gbl.Test.Config.Pluggable = Tst_GetPluggableFromForm ();

   /***** Get number of questions *****/
   /* Get minimum number of questions */
   Gbl.Test.Config.Min = (unsigned)
	                 Par_GetParToUnsignedLong ("NumQstMin",
	                                           1,
	                                           UINT_MAX,
	                                           1);

   /* Get default number of questions */
   Gbl.Test.Config.Def = (unsigned)
	                 Par_GetParToUnsignedLong ("NumQstDef",
	                                           1,
	                                           UINT_MAX,
	                                           1);

   /* Get maximum number of questions */
   Gbl.Test.Config.Max = (unsigned)
	                 Par_GetParToUnsignedLong ("NumQstMax",
	                                           1,
	                                           UINT_MAX,
	                                           1);

   /* Check and correct numbers */
   Tst_CheckAndCorrectNumbersQst ();

   /***** Get minimum time between consecutive tests, per question *****/
   Gbl.Test.Config.MinTimeNxtTstPerQst = Par_GetParToUnsignedLong ("MinTimeNxtTstPerQst",
                                                                   0,
                                                                   ULONG_MAX,
                                                                   0);

   /***** Get type of feedback from form *****/
   Gbl.Test.Config.Feedback = Tst_GetFeedbackTypeFromForm ();

   /***** Update database *****/
   DB_QueryREPLACE ("can not save configuration of tests",
		    "REPLACE INTO tst_config"
	            " (CrsCod,Pluggable,Min,Def,Max,MinTimeNxtTstPerQst,Feedback)"
                    " VALUES"
                    " (%ld,'%s',%u,%u,%u,'%lu','%s')",
		    Gbl.Hierarchy.Crs.CrsCod,
		    Tst_PluggableDB[Gbl.Test.Config.Pluggable],
		    Gbl.Test.Config.Min,Gbl.Test.Config.Def,Gbl.Test.Config.Max,
		    Gbl.Test.Config.MinTimeNxtTstPerQst,
		    Tst_FeedbackDB[Gbl.Test.Config.Feedback]);

   /***** Show confirmation message *****/
   Ale_ShowAlert (Ale_SUCCESS,Txt_The_test_configuration_has_been_updated);

   /***** Show again the form to configure test *****/
   Tst_ShowFormConfig ();
  }

/*****************************************************************************/
/******************* Get if tests are pluggable from form ********************/
/*****************************************************************************/

static Tst_Pluggable_t Tst_GetPluggableFromForm (void)
  {
   return (Tst_Pluggable_t)
	  Par_GetParToUnsignedLong ("Pluggable",
	                            0,
                                    Tst_NUM_OPTIONS_PLUGGABLE - 1,
                                    (unsigned long) Tst_PLUGGABLE_UNKNOWN);
  }

/*****************************************************************************/
/*********************** Get type of feedback from form **********************/
/*****************************************************************************/

static Tst_Feedback_t Tst_GetFeedbackTypeFromForm (void)
  {
   return (Tst_Feedback_t)
	  Par_GetParToUnsignedLong ("Feedback",
	                            0,
                                    Tst_NUM_TYPES_FEEDBACK - 1,
                                    (unsigned long) Tst_FEEDBACK_DEFAULT);
  }

/*****************************************************************************/
/**** Check and correct minimum, default and maximum numbers of questions ****/
/*****************************************************************************/

static void Tst_CheckAndCorrectNumbersQst (void)
  {
   /***** Check if minimum is correct *****/
   if (Gbl.Test.Config.Min < 1)
      Gbl.Test.Config.Min = 1;
   else if (Gbl.Test.Config.Min > Tst_MAX_QUESTIONS_PER_TEST)
      Gbl.Test.Config.Min = Tst_MAX_QUESTIONS_PER_TEST;

   /***** Check if maximum is correct *****/
   if (Gbl.Test.Config.Max < 1)
      Gbl.Test.Config.Max = 1;
   else if (Gbl.Test.Config.Max > Tst_MAX_QUESTIONS_PER_TEST)
      Gbl.Test.Config.Max = Tst_MAX_QUESTIONS_PER_TEST;

   /***** Check if minimum is lower than maximum *****/
   if (Gbl.Test.Config.Min > Gbl.Test.Config.Max)
      Gbl.Test.Config.Min = Gbl.Test.Config.Max;

   /***** Check if default is correct *****/
   if (Gbl.Test.Config.Def < Gbl.Test.Config.Min)
      Gbl.Test.Config.Def = Gbl.Test.Config.Min;
   else if (Gbl.Test.Config.Def > Gbl.Test.Config.Max)
      Gbl.Test.Config.Def = Gbl.Test.Config.Max;
  }

/*****************************************************************************/
/***************** Show form for select the types of answers *****************/
/*****************************************************************************/

static void Tst_ShowFormAnswerTypes (unsigned NumCols)
  {
   extern const char *The_ClassFormInBox[The_NUM_THEMES];
   extern const char *Txt_Types_of_answers;
   extern const char *Txt_All_types_of_answers;
   extern const char *Txt_TST_STR_ANSWER_TYPES[Tst_NUM_ANS_TYPES];
   Tst_AnswerType_t AnsType;
   char UnsignedStr[10 + 1];
   const char *Ptr;

   /***** Label *****/
   fprintf (Gbl.F.Out,"<tr>"
		      "<td class=\"RIGHT_TOP %s\">"
		      "%s:"
                      "</td>",
	    The_ClassFormInBox[Gbl.Prefs.Theme],Txt_Types_of_answers);

   /***** Select all types of answers *****/
   fprintf (Gbl.F.Out,"<td");
   if (NumCols > 1)
      fprintf (Gbl.F.Out," colspan=\"%u\"",NumCols);
   fprintf (Gbl.F.Out," class=\"LEFT_TOP\">");
   Tbl_StartTable (2);
   fprintf (Gbl.F.Out,"<tr>"
	              "<td class=\"LEFT_MIDDLE\">"
                      "<label class=\"%s\">"
                      "<input type=\"checkbox\" name=\"AllAnsTypes\" value=\"Y\"",
            The_ClassFormInBox[Gbl.Prefs.Theme]);
   if (Gbl.Test.AllAnsTypes)
      fprintf (Gbl.F.Out," checked=\"checked\"");
   fprintf (Gbl.F.Out," onclick=\"togglecheckChildren(this,'AnswerType');\" />"
                      "&nbsp;%s"
                      "</label>"
                      "</td>"
                      "</tr>",
            Txt_All_types_of_answers);

   /***** Type of answer *****/
   for (AnsType = (Tst_AnswerType_t) 0;
	AnsType < Tst_NUM_ANS_TYPES;
	AnsType++)
     {
      fprintf (Gbl.F.Out,"<tr>"
	                 "<td class=\"LEFT_MIDDLE\">"
                         "<label class=\"DAT\">"
                         "<input type=\"checkbox\" name=\"AnswerType\" value=\"%u\"",
               (unsigned) AnsType);
      Ptr = Gbl.Test.ListAnsTypes;
      while (*Ptr)
        {
         Par_GetNextStrUntilSeparParamMult (&Ptr,UnsignedStr,10);
         if (Tst_ConvertFromUnsignedStrToAnsTyp (UnsignedStr) == AnsType)
            fprintf (Gbl.F.Out," checked=\"checked\"");
        }
      fprintf (Gbl.F.Out," onclick=\"checkParent(this,'AllAnsTypes');\" />"
	                 "&nbsp;%s"
	                 "</label>"
	                 "</td>"
	                 "</tr>",
               Txt_TST_STR_ANSWER_TYPES[AnsType]);
     }

   Tbl_EndTable ();
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");
  }

/*****************************************************************************/
/***************** List several test questions for edition *******************/
/*****************************************************************************/

void Tst_ListQuestionsToEdit (void)
  {
   MYSQL_RES *mysql_res;
   unsigned long NumRows;

   /***** Get parameters, query the database and list the questions *****/
   if (Tst_GetParamsTst (Tst_EDIT_TEST))	// Get parameters from the form
     {
      if ((NumRows = Tst_GetQuestions (&mysql_res)) != 0)	// Query database
        {
	 /* Buttons for edition */
         fprintf (Gbl.F.Out,"<div class=\"CONTEXT_MENU\">");
	 if (Gbl.Test.XML.CreateXML)
	    /* Create XML file for exporting questions
	       and put a link to download it */
            TsI_CreateXML (NumRows,mysql_res);
         else
            /* Button to export questions */
            TsI_PutFormToExportQuestions ();
	 fprintf (Gbl.F.Out,"</div>");

	 /* Show the table with the questions */
         Tst_ListOneOrMoreQuestionsForEdition (NumRows,mysql_res);
        }

      /***** Free structure that stores the query result *****/
      DB_FreeMySQLResult (&mysql_res);
     }
   else
      /* Show the form again */
      Tst_ShowFormAskEditTsts ();

   /***** Free memory used by the list of tags *****/
   Tst_FreeTagsList ();
  }

/*****************************************************************************/
/**************** List several test questions for selection ******************/
/*****************************************************************************/

void Tst_ListQuestionsToSelect (void)
  {
   long GamCod;
   MYSQL_RES *mysql_res;
   unsigned long NumRows;

   /***** Get game code *****/
   if ((GamCod = Gam_GetParamGameCod ()) == -1L)
      Lay_ShowErrorAndExit ("Code of game is missing.");

   /***** Get parameters, query the database and list the questions *****/
   if (Tst_GetParamsTst (Tst_SELECT_QUESTIONS_FOR_GAME))	// Get parameters from the form
     {
      if ((NumRows = Tst_GetQuestions (&mysql_res)) != 0)	// Query database
	 /* Show the table with the questions */
         Tst_ListOneOrMoreQuestionsForSelection (GamCod,NumRows,mysql_res);

      /***** Free structure that stores the query result *****/
      DB_FreeMySQLResult (&mysql_res);
     }
   else
      /* Show the form again */
      Tst_ShowFormAskSelectTstsForGame (GamCod);

   /***** Free memory used by the list of tags *****/
   Tst_FreeTagsList ();
  }

/*****************************************************************************/
/********** Get from the database several test questions for listing *********/
/*****************************************************************************/

#define Tst_MAX_BYTES_QUERY_TEST (16 * 1024 - 1)

static unsigned long Tst_GetQuestions (MYSQL_RES **mysql_res)
  {
   extern const char *Txt_No_questions_found_matching_your_search_criteria;
   char *Query = NULL;
   unsigned long NumRows;
   long LengthQuery;
   unsigned NumItemInList;
   const char *Ptr;
   char TagText[Tst_MAX_BYTES_TAG + 1];
   char LongStr[1 + 10 + 1];
   char UnsignedStr[10 + 1];
   Tst_AnswerType_t AnsType;
   char CrsCodStr[1 + 10 + 1];

   /***** Allocate space for query *****/
   if ((Query = (char *) malloc (Tst_MAX_BYTES_QUERY_TEST + 1)) == NULL)
      Lay_NotEnoughMemoryExit ();

   /***** Select questions *****/
   /* Start query */
   snprintf (Query,Tst_MAX_BYTES_QUERY_TEST + 1,
	     "SELECT tst_questions.QstCod,"			// row[0]
	            "UNIX_TIMESTAMP(tst_questions.EditTime),"	// row[1]
	            "tst_questions.AnsType,"			// row[2]
	            "tst_questions.Shuffle,"			// row[3]
	            "tst_questions.Stem,"			// row[4]
	            "tst_questions.Feedback,"			// row[5]
	            "tst_questions.MedCod,"			// row[6]
	            "tst_questions.NumHits,"			// row[7]
	            "tst_questions.NumHitsNotBlank,"		// row[8]
	            "tst_questions.Score"			// row[9]
	     " FROM tst_questions");
   if (!Gbl.Test.Tags.All)
      Str_Concat (Query,",tst_question_tags,tst_tags",
                  Tst_MAX_BYTES_QUERY_TEST);

   Str_Concat (Query," WHERE tst_questions.CrsCod='",
               Tst_MAX_BYTES_QUERY_TEST);
   snprintf (CrsCodStr,sizeof (CrsCodStr),
	     "%ld",
	     Gbl.Hierarchy.Crs.CrsCod);
   Str_Concat (Query,CrsCodStr,
               Tst_MAX_BYTES_QUERY_TEST);
   Str_Concat (Query,"' AND tst_questions.EditTime>=FROM_UNIXTIME('",
               Tst_MAX_BYTES_QUERY_TEST);
   snprintf (LongStr,sizeof (LongStr),
	     "%ld",
	     (long) Gbl.DateRange.TimeUTC[0]);
   Str_Concat (Query,LongStr,
               Tst_MAX_BYTES_QUERY_TEST);
   Str_Concat (Query,"') AND tst_questions.EditTime<=FROM_UNIXTIME('",
               Tst_MAX_BYTES_QUERY_TEST);
   snprintf (LongStr,sizeof (LongStr),
	     "%ld",
	     (long) Gbl.DateRange.TimeUTC[1]);
   Str_Concat (Query,LongStr,
               Tst_MAX_BYTES_QUERY_TEST);
   Str_Concat (Query,"')",
               Tst_MAX_BYTES_QUERY_TEST);

   /* Add the tags selected */
   if (!Gbl.Test.Tags.All)
     {
      Str_Concat (Query," AND tst_questions.QstCod=tst_question_tags.QstCod"
	                " AND tst_question_tags.TagCod=tst_tags.TagCod"
                        " AND tst_tags.CrsCod='",
                  Tst_MAX_BYTES_QUERY_TEST);
      Str_Concat (Query,CrsCodStr,
                  Tst_MAX_BYTES_QUERY_TEST);
      Str_Concat (Query,"'",
                  Tst_MAX_BYTES_QUERY_TEST);
      LengthQuery = strlen (Query);
      NumItemInList = 0;
      Ptr = Gbl.Test.Tags.List;
      while (*Ptr)
        {
         Par_GetNextStrUntilSeparParamMult (&Ptr,TagText,Tst_MAX_BYTES_TAG);
         LengthQuery = LengthQuery + 35 + strlen (TagText) + 1;
         if (LengthQuery > Tst_MAX_BYTES_QUERY_TEST - 256)
            Lay_ShowErrorAndExit ("Query size exceed.");
         Str_Concat (Query,
                     NumItemInList ? " OR tst_tags.TagTxt='" :
                                     " AND (tst_tags.TagTxt='",
                     Tst_MAX_BYTES_QUERY_TEST);
         Str_Concat (Query,TagText,
                     Tst_MAX_BYTES_QUERY_TEST);
         Str_Concat (Query,"'",
                     Tst_MAX_BYTES_QUERY_TEST);
         NumItemInList++;
        }
      Str_Concat (Query,")",
                  Tst_MAX_BYTES_QUERY_TEST);
     }

   /* Add the types of answer selected */
   if (!Gbl.Test.AllAnsTypes)
     {
      LengthQuery = strlen (Query);
      NumItemInList = 0;
      Ptr = Gbl.Test.ListAnsTypes;
      while (*Ptr)
        {
         Par_GetNextStrUntilSeparParamMult (&Ptr,UnsignedStr,Tst_MAX_BYTES_TAG);
	 AnsType = Tst_ConvertFromUnsignedStrToAnsTyp (UnsignedStr);
         LengthQuery = LengthQuery + 35 + strlen (Tst_StrAnswerTypesDB[AnsType]) + 1;
         if (LengthQuery > Tst_MAX_BYTES_QUERY_TEST - 256)
            Lay_ShowErrorAndExit ("Query size exceed.");
         Str_Concat (Query,
                     NumItemInList ? " OR tst_questions.AnsType='" :
                                     " AND (tst_questions.AnsType='",
                     Tst_MAX_BYTES_QUERY_TEST);
         Str_Concat (Query,Tst_StrAnswerTypesDB[AnsType],
                     Tst_MAX_BYTES_QUERY_TEST);
         Str_Concat (Query,"'",
                     Tst_MAX_BYTES_QUERY_TEST);
         NumItemInList++;
        }
      Str_Concat (Query,")",
                  Tst_MAX_BYTES_QUERY_TEST);
     }

   /* End the query */
   Str_Concat (Query," GROUP BY tst_questions.QstCod",
               Tst_MAX_BYTES_QUERY_TEST);

   switch (Gbl.Test.SelectedOrder)
     {
      case Tst_ORDER_STEM:
         Str_Concat (Query," ORDER BY tst_questions.Stem",
                     Tst_MAX_BYTES_QUERY_TEST);
         break;
      case Tst_ORDER_NUM_HITS:
         Str_Concat (Query," ORDER BY tst_questions.NumHits DESC,"
				     "tst_questions.Stem",
                     Tst_MAX_BYTES_QUERY_TEST);
         break;
      case Tst_ORDER_AVERAGE_SCORE:
         Str_Concat (Query," ORDER BY tst_questions.Score/tst_questions.NumHits DESC,"
				     "tst_questions.NumHits DESC,"
				     "tst_questions.Stem",
                     Tst_MAX_BYTES_QUERY_TEST);
         break;
      case Tst_ORDER_NUM_HITS_NOT_BLANK:
         Str_Concat (Query," ORDER BY tst_questions.NumHitsNotBlank DESC,"
				     "tst_questions.Stem",
                     Tst_MAX_BYTES_QUERY_TEST);
         break;
      case Tst_ORDER_AVERAGE_SCORE_NOT_BLANK:
         Str_Concat (Query," ORDER BY tst_questions.Score/tst_questions.NumHitsNotBlank DESC,"
				     "tst_questions.NumHitsNotBlank DESC,"
				     "tst_questions.Stem",
                     Tst_MAX_BYTES_QUERY_TEST);
         break;
     }

   /* Make the query */
   NumRows = DB_QuerySELECT (mysql_res,"can not get questions",
			     "%s",
			     Query);

   if (NumRows == 0)
      Ale_ShowAlert (Ale_INFO,Txt_No_questions_found_matching_your_search_criteria);

   return NumRows;
  }

/*****************************************************************************/
/********* Get from the database several test questions to list them *********/
/*****************************************************************************/

static unsigned long Tst_GetQuestionsForTest (MYSQL_RES **mysql_res)
  {
   char *Query = NULL;
   long LengthQuery;
   unsigned NumItemInList;
   const char *Ptr;
   char TagText[Tst_MAX_BYTES_TAG + 1];
   char UnsignedStr[10 + 1];
   Tst_AnswerType_t AnsType;
   char StrNumQsts[10 + 1];

   /***** Allocate space for query *****/
   if ((Query = (char *) malloc (Tst_MAX_BYTES_QUERY_TEST + 1)) == NULL)
      Lay_NotEnoughMemoryExit ();

   /***** Select questions without hidden tags *****/
   /* Start query */
   // Reject questions with any tag hidden
   // Select only questions with tags
   // DISTINCTROW is necessary to not repeat questions
   snprintf (Query,Tst_MAX_BYTES_QUERY_TEST + 1,
	     "SELECT DISTINCTROW tst_questions.QstCod,"				// row[0]
	                        "UNIX_TIMESTAMP(tst_questions.EditTime),"	// row[1]
	                        "tst_questions.AnsType,"			// row[2]
	                        "tst_questions.Shuffle,"			// row[3]
	                        "tst_questions.Stem,"				// row[4]
	                        "tst_questions.Feedback,"			// row[5]
	                        "tst_questions.MedCod,"				// row[6]
	                        "tst_questions.NumHits,"			// row[7]
	                        "tst_questions.NumHitsNotBlank,"		// row[8]
	                        "tst_questions.Score"				// row[9]
	     " FROM tst_questions,tst_question_tags,tst_tags"
	     " WHERE tst_questions.CrsCod=%ld"
	     " AND tst_questions.QstCod NOT IN"
	     " (SELECT tst_question_tags.QstCod"
	     " FROM tst_tags,tst_question_tags"
	     " WHERE tst_tags.CrsCod=%ld AND tst_tags.TagHidden='Y'"
	     " AND tst_tags.TagCod=tst_question_tags.TagCod)"
	     " AND tst_questions.QstCod=tst_question_tags.QstCod"
	     " AND tst_question_tags.TagCod=tst_tags.TagCod"
	     " AND tst_tags.CrsCod=%ld",
	     Gbl.Hierarchy.Crs.CrsCod,
	     Gbl.Hierarchy.Crs.CrsCod,
	     Gbl.Hierarchy.Crs.CrsCod);

   if (!Gbl.Test.Tags.All) // User has not selected all the tags
     {
      /* Add selected tags */
      LengthQuery = strlen (Query);
      NumItemInList = 0;
      Ptr = Gbl.Test.Tags.List;
      while (*Ptr)
        {
         Par_GetNextStrUntilSeparParamMult (&Ptr,TagText,Tst_MAX_BYTES_TAG);
         LengthQuery = LengthQuery + 35 + strlen (TagText) + 1;
         if (LengthQuery > Tst_MAX_BYTES_QUERY_TEST - 128)
            Lay_ShowErrorAndExit ("Query size exceed.");
         Str_Concat (Query,
                     NumItemInList ? " OR tst_tags.TagTxt='" :
                                     " AND (tst_tags.TagTxt='",
                     Tst_MAX_BYTES_QUERY_TEST);
         Str_Concat (Query,TagText,
                     Tst_MAX_BYTES_QUERY_TEST);
         Str_Concat (Query,"'",
                     Tst_MAX_BYTES_QUERY_TEST);
         NumItemInList++;
        }
      Str_Concat (Query,")",
                  Tst_MAX_BYTES_QUERY_TEST);
     }

   /* Add answer types selected */
   if (!Gbl.Test.AllAnsTypes)
     {
      LengthQuery = strlen (Query);
      NumItemInList = 0;
      Ptr = Gbl.Test.ListAnsTypes;
      while (*Ptr)
        {
         Par_GetNextStrUntilSeparParamMult (&Ptr,UnsignedStr,Tst_MAX_BYTES_TAG);
	 AnsType = Tst_ConvertFromUnsignedStrToAnsTyp (UnsignedStr);
         LengthQuery = LengthQuery + 35 + strlen (Tst_StrAnswerTypesDB[AnsType]) + 1;
         if (LengthQuery > Tst_MAX_BYTES_QUERY_TEST - 128)
            Lay_ShowErrorAndExit ("Query size exceed.");
         Str_Concat (Query,
                     NumItemInList ? " OR tst_questions.AnsType='" :
                                     " AND (tst_questions.AnsType='",
                     Tst_MAX_BYTES_QUERY_TEST);
         Str_Concat (Query,Tst_StrAnswerTypesDB[AnsType],
                     Tst_MAX_BYTES_QUERY_TEST);
         Str_Concat (Query,"'",
                     Tst_MAX_BYTES_QUERY_TEST);
         NumItemInList++;
        }
      Str_Concat (Query,")",
                  Tst_MAX_BYTES_QUERY_TEST);
     }

   /* End query */
   Str_Concat (Query," ORDER BY RAND(NOW()) LIMIT ",
               Tst_MAX_BYTES_QUERY_TEST);
   snprintf (StrNumQsts,sizeof (StrNumQsts),
	     "%u",
	     Gbl.Test.NumQsts);
   Str_Concat (Query,StrNumQsts,
               Tst_MAX_BYTES_QUERY_TEST);
/*
   if (Gbl.Usrs.Me.Roles.LoggedRole == Rol_SYS_ADM)
      Lay_ShowAlert (Lay_INFO,Query);
*/
   /* Make the query */
   return DB_QuerySELECT (mysql_res,"can not get questions",
			  "%s",
			  Query);
  }

/*****************************************************************************/
/*********************** List a test question for edition ********************/
/*****************************************************************************/

static void Tst_ListOneQstToEdit (void)
  {
   MYSQL_RES *mysql_res;

   /***** Query database *****/
   if (Tst_GetOneQuestionByCod (Gbl.Test.QstCod,&mysql_res))
      /***** Show the question ready to edit it *****/
      Tst_ListOneOrMoreQuestionsForEdition (1,mysql_res);
   else
      Lay_ShowErrorAndExit ("Can not get question.");

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/*********************** Get data of one test question ***********************/
/*****************************************************************************/
// Return true on success, false on error

bool Tst_GetOneQuestionByCod (long QstCod,MYSQL_RES **mysql_res)
  {
   /***** Get data of a question from database *****/
   return (DB_QuerySELECT (mysql_res,"can not get data of a question",
			   "SELECT QstCod,"			// row[0]
			          "UNIX_TIMESTAMP(EditTime),"	// row[1]
			          "AnsType,"			// row[2]
			          "Shuffle,"			// row[3]
			          "Stem,"			// row[4]
			          "Feedback,"			// row[5]
			          "MedCod,"			// row[6]
			          "NumHits,"			// row[7]
			          "NumHitsNotBlank,"		// row[8]
			          "Score"			// row[9]
			   " FROM tst_questions"
			   " WHERE QstCod=%ld",
			   QstCod) == 1);
  }

/*****************************************************************************/
/****************** List for edition one or more test questions **************/
/*****************************************************************************/

static void Tst_ListOneOrMoreQuestionsForEdition (unsigned long NumRows,
                                                  MYSQL_RES *mysql_res)
  {
   extern const char *Hlp_ASSESSMENT_Tests;
   extern const char *Txt_Questions;
   extern const char *Txt_No_INDEX;
   extern const char *Txt_Code;
   extern const char *Txt_Date;
   extern const char *Txt_Tags;
   extern const char *Txt_TST_STR_ORDER_FULL[Tst_NUM_TYPES_ORDER_QST];
   extern const char *Txt_TST_STR_ORDER_SHORT[Tst_NUM_TYPES_ORDER_QST];
   extern const char *Txt_TST_STR_ANSWER_TYPES[Tst_NUM_ANS_TYPES];
   extern const char *Txt_Shuffle;
   extern const char *Txt_Today;
   Tst_QuestionsOrder_t Order;
   unsigned long NumRow;
   MYSQL_ROW row;
   unsigned UniqueId;
   time_t TimeUTC;
   unsigned long NumHitsThisQst;
   unsigned long NumHitsNotBlankThisQst;
   double TotalScoreThisQst;

   /***** Start box *****/
   Box_StartBox (NULL,Txt_Questions,Tst_PutIconsTests,
		 Hlp_ASSESSMENT_Tests,Box_NOT_CLOSABLE);

   /***** Write the heading *****/
   Tbl_StartTableWideMargin (2);
   fprintf (Gbl.F.Out,"<tr>"
                      "<th></th>"
                      "<th class=\"CENTER_TOP\">"
                      "%s"
                      "</th>"
                      "<th class=\"CENTER_TOP\">"
                      "%s"
                      "</th>"
                      "<th class=\"CENTER_TOP\">"
                      "%s"
                      "</th>"
                      "<th class=\"CENTER_TOP\">"
                      "%s"
                      "</th>"
                      "<th class=\"CENTER_TOP\">"
                      "%s"
                      "</th>",
            Txt_No_INDEX,
            Txt_Code,
            Txt_Date,
            Txt_Tags,
            Txt_Shuffle);

   /* Stem and answers of question */
   /* Number of times that the question has been answered */
   /* Average score */
   for (Order = (Tst_QuestionsOrder_t) 0;
	Order < (Tst_QuestionsOrder_t) Tst_NUM_TYPES_ORDER_QST;
	Order++)
     {
      fprintf (Gbl.F.Out,"<th class=\"LEFT_TOP\">");
      if (NumRows > 1)
        {
         Frm_StartForm (ActLstTstQst);
         Dat_WriteParamsIniEndDates ();
         Tst_WriteParamEditQst ();
         Par_PutHiddenParamUnsigned ("Order",(unsigned) Order);
         Frm_LinkFormSubmit (Txt_TST_STR_ORDER_FULL[Order],"TIT_TBL",NULL);
         if (Order == Gbl.Test.SelectedOrder)
            fprintf (Gbl.F.Out,"<u>");
        }
      fprintf (Gbl.F.Out,"%s",Txt_TST_STR_ORDER_SHORT[Order]);
      if (NumRows > 1)
        {
         if (Order == Gbl.Test.SelectedOrder)
            fprintf (Gbl.F.Out,"</u>");
         fprintf (Gbl.F.Out,"</a>");
         Frm_EndForm ();
        }
      fprintf (Gbl.F.Out,"</th>");
     }

   fprintf (Gbl.F.Out,"</tr>");

   /***** Write rows *****/
   for (NumRow = 0, UniqueId = 1;
	NumRow < NumRows;
	NumRow++, UniqueId++)
     {
      Gbl.RowEvenOdd = NumRow % 2;

      row = mysql_fetch_row (mysql_res);
      /*
      row[0] QstCod
      row[1] UNIX_TIMESTAMP(EditTime)
      row[2] AnsType
      row[3] Shuffle
      row[4] Stem
      row[5] Feedback
      row[6] MedCod
      row[7] NumHits
      row[8] NumHitsNotBlank
      row[9] Score
      */
      /***** Create test question *****/
      Tst_QstConstructor ();

      /* row[0] holds the code of the question */
      if ((Gbl.Test.QstCod = Str_ConvertStrCodToLongCod (row[0])) < 0)
         Lay_ShowErrorAndExit ("Wrong code of question.");

      /***** Icons *****/
      fprintf (Gbl.F.Out,"<tr>"
                         "<td class=\"BT%u\">",Gbl.RowEvenOdd);

      /* Write icon to remove the question */
      Frm_StartForm (ActReqRemTstQst);
      Tst_PutParamQstCod ();
      if (NumRows == 1)
         Par_PutHiddenParamChar ("OnlyThisQst",'Y'); // If there are only one row, don't list again after removing
      Dat_WriteParamsIniEndDates ();
      Tst_WriteParamEditQst ();
      Ico_PutIconRemove ();
      Frm_EndForm ();

      /* Write icon to edit the question */
      Ico_PutContextualIconToEdit (ActEdiOneTstQst,Tst_PutParamQstCod);

      fprintf (Gbl.F.Out,"</td>");

      /* Write number of question */
      fprintf (Gbl.F.Out,"<td class=\"RIGHT_TOP COLOR%u\">"
			 "<div class=\"BIG_INDEX\">%lu</div>",
	       Gbl.RowEvenOdd,
	       NumRow + 1);

      /* Write answer type (row[2]) */
      Gbl.Test.AnswerType = Tst_ConvertFromStrAnsTypDBToAnsTyp (row[2]);
      fprintf (Gbl.F.Out,"<div class=\"DAT_SMALL\">%s</div>"
			 "</td>",
	       Txt_TST_STR_ANSWER_TYPES[Gbl.Test.AnswerType]);

      /* Write question code */
      fprintf (Gbl.F.Out,"<td class=\"DAT_SMALL CENTER_TOP COLOR%u\">"
	                 "%ld&nbsp;"
	                 "</td>",
               Gbl.RowEvenOdd,Gbl.Test.QstCod);

      /* Write the date (row[1] has the UTC date-time) */
      TimeUTC = Dat_GetUNIXTimeFromStr (row[1]);
      fprintf (Gbl.F.Out,"<td id=\"tst_date_%u\""
	                 " class=\"DAT_SMALL CENTER_TOP COLOR%u\">"
                         "<script type=\"text/javascript\">"
                         "writeLocalDateHMSFromUTC('tst_date_%u',%ld,"
                         "%u,'<br />','%s',true,false,0x7);"
                         "</script>"
                         "</td>",
               UniqueId,Gbl.RowEvenOdd,
               UniqueId,(long) TimeUTC,
               (unsigned) Gbl.Prefs.DateFormat,Txt_Today);

      /* Write the question tags */
      fprintf (Gbl.F.Out,"<td class=\"LEFT_TOP COLOR%u\">",
               Gbl.RowEvenOdd);
      Tst_GetAndWriteTagsQst (Gbl.Test.QstCod);
      fprintf (Gbl.F.Out,"</td>");

      /* Write if shuffle is enabled (row[3]) */
      fprintf (Gbl.F.Out,"<td class=\"DAT_SMALL CENTER_TOP COLOR%u\">",
	       Gbl.RowEvenOdd);
      if (Gbl.Test.AnswerType == Tst_ANS_UNIQUE_CHOICE ||
          Gbl.Test.AnswerType == Tst_ANS_MULTIPLE_CHOICE)
        {
         Frm_StartForm (ActShfTstQst);
         Tst_PutParamQstCod ();
         Dat_WriteParamsIniEndDates ();
         Tst_WriteParamEditQst ();
         if (NumRows == 1)
	    Par_PutHiddenParamChar ("OnlyThisQst",'Y'); // If editing only one question, don't edit others
         Par_PutHiddenParamUnsigned ("Order",(unsigned) Gbl.Test.SelectedOrder);
         fprintf (Gbl.F.Out,"<input type=\"checkbox\" name=\"Shuffle\" value=\"Y\"");
         if (row[3][0] == 'Y')
            fprintf (Gbl.F.Out," checked=\"checked\"");
         fprintf (Gbl.F.Out," onclick=\"document.getElementById('%s').submit();\" />",
                  Gbl.Form.Id);
         Frm_EndForm ();
        }
      fprintf (Gbl.F.Out,"</td>");

      /* Write stem (row[4]) */
      fprintf (Gbl.F.Out,"<td class=\"LEFT_TOP COLOR%u\">",
	       Gbl.RowEvenOdd);
      Tst_WriteQstStem (row[4],"TEST_EDI");

      /***** Get and show media (row[6]) *****/
      Gbl.Test.Media.MedCod = Str_ConvertStrCodToLongCod (row[6]);
      Med_GetMediaDataByCod (&Gbl.Test.Media);
      Med_ShowMedia (&Gbl.Test.Media,
                     "TEST_MED_EDIT_LIST_CONTAINER",
                     "TEST_MED_EDIT_LIST");

      /* Write feedback (row[5]) and answers */
      Tst_WriteQstFeedback (row[5],"TEST_EDI_LIGHT");
      Tst_WriteAnswersEdit (Gbl.Test.QstCod);
      fprintf (Gbl.F.Out,"</td>");

      /* Get number of hits
         (number of times that the question has been answered,
         including blank answers) (row[7]) */
      if (sscanf (row[7],"%lu",&NumHitsThisQst) != 1)
         Lay_ShowErrorAndExit ("Wrong number of hits to a question.");

      /* Get number of hits not blank
         (number of times that the question has been answered
         with a not blank answer) (row[8]) */
      if (sscanf (row[8],"%lu",&NumHitsNotBlankThisQst) != 1)
         Lay_ShowErrorAndExit ("Wrong number of hits not blank to a question.");

      /* Get the acumulated score of the question (row[9]) */
      Str_SetDecimalPointToUS ();	// To get the decimal point as a dot
      if (sscanf (row[9],"%lf",&TotalScoreThisQst) != 1)
         Lay_ShowErrorAndExit ("Wrong score of a question.");
      Str_SetDecimalPointToLocal ();	// Return to local system

      /* Write number of times this question has been answered */
      fprintf (Gbl.F.Out,"<td class=\"DAT_SMALL CENTER_TOP COLOR%u\">"
	                 "%lu"
	                 "</td>",
               Gbl.RowEvenOdd,NumHitsThisQst);

      /* Write average score */
      fprintf (Gbl.F.Out,"<td class=\"DAT_SMALL CENTER_TOP COLOR%u\">",
               Gbl.RowEvenOdd);
      if (NumHitsThisQst)
         fprintf (Gbl.F.Out,"%.2f",TotalScoreThisQst /
                                   (double) NumHitsThisQst);
      else
         fprintf (Gbl.F.Out,"N.A.");
      fprintf (Gbl.F.Out,"</td>");

      /* Write number of times this question has been answered (not blank) */
      fprintf (Gbl.F.Out,"<td class=\"DAT_SMALL CENTER_TOP COLOR%u\">"
	                 "%lu"
	                 "</td>",
               Gbl.RowEvenOdd,
               NumHitsNotBlankThisQst);

      /* Write average score (not blank) */
      fprintf (Gbl.F.Out,"<td class=\"DAT_SMALL CENTER_TOP COLOR%u\">",
               Gbl.RowEvenOdd);
      if (NumHitsNotBlankThisQst)
         fprintf (Gbl.F.Out,"%.2f",TotalScoreThisQst /
                                   (double) NumHitsNotBlankThisQst);
      else
         fprintf (Gbl.F.Out,"N.A.");
      fprintf (Gbl.F.Out,"</td>"
	                 "</tr>");

      /***** Destroy test question *****/
      Tst_QstDestructor ();
     }

   /***** End table *****/
   Tbl_EndTable ();

   /***** Button to add a new question *****/
   Tst_PutButtonToAddQuestion ();

   /***** End box *****/
   Box_EndBox ();
  }

/*****************************************************************************/
/****************** List for edition one or more test questions **************/
/*****************************************************************************/

static void Tst_ListOneOrMoreQuestionsForSelection (long GamCod,
                                                    unsigned long NumRows,
                                                    MYSQL_RES *mysql_res)
  {
   extern const char *Hlp_ASSESSMENT_Tests;
   extern const char *Txt_Questions;
   extern const char *Txt_No_INDEX;
   extern const char *Txt_Code;
   extern const char *Txt_Date;
   extern const char *Txt_Tags;
   extern const char *Txt_Type;
   extern const char *Txt_TST_STR_ANSWER_TYPES[Tst_NUM_ANS_TYPES];
   extern const char *Txt_Shuffle;
   extern const char *Txt_Question;
   extern const char *Txt_Today;
   extern const char *Txt_Add_questions;
   unsigned long NumRow;
   MYSQL_ROW row;
   unsigned UniqueId;
   time_t TimeUTC;

   /***** Start box *****/
   Box_StartBox (NULL,Txt_Questions,NULL,
		 Hlp_ASSESSMENT_Tests,Box_NOT_CLOSABLE);

   /***** Start form *****/
   Frm_StartForm (ActAddTstQstToGam);
   Gam_PutParamGameCod (GamCod);

   /***** Write the heading *****/
   Tbl_StartTableWideMargin (2);
   fprintf (Gbl.F.Out,"<tr>"
                      "<th></th>"
                      "<th class=\"CENTER_TOP\">"
                      "%s"
                      "</th>"
                      "<th class=\"CENTER_TOP\">"
                      "%s"
                      "</th>"
                      "<th class=\"CENTER_TOP\">"
                      "%s"
                      "</th>"
                      "<th class=\"LEFT_TOP\">"
                      "%s"
                      "</th>"
                      "<th class=\"CENTER_TOP\">"
                      "%s"
                      "</th>"
                      "<th class=\"CENTER_TOP\">"
                      "%s"
                      "</th>"
                      "<th class=\"CENTER_TOP\">"
                      "%s"
                      "</th>"
                      "</tr>",
            Txt_No_INDEX,
            Txt_Code,
            Txt_Date,
            Txt_Tags,
            Txt_Type,
            Txt_Shuffle,
            Txt_Question);

   /***** Write rows *****/
   for (NumRow = 0, UniqueId = 1;
	NumRow < NumRows;
	NumRow++, UniqueId++)
     {
      Gbl.RowEvenOdd = NumRow % 2;

      row = mysql_fetch_row (mysql_res);
      /*
      row[0] QstCod
      row[1] UNIX_TIMESTAMP(EditTime)
      row[2] AnsType
      row[3] Shuffle
      row[4] Stem
      row[5] Feedback
      row[6] MedCod
      row[7] NumHits
      row[8] NumHitsNotBlank
      row[9] Score
      */
      /***** Create test question *****/
      Tst_QstConstructor ();

      /* row[0] holds the code of the question */
      if ((Gbl.Test.QstCod = Str_ConvertStrCodToLongCod (row[0])) < 0)
         Lay_ShowErrorAndExit ("Wrong code of question.");

      /***** Icons *****/
      fprintf (Gbl.F.Out,"<tr>"
                         "<td class=\"BT%u\">",Gbl.RowEvenOdd);

      /* Write checkbox to select the question */
      fprintf (Gbl.F.Out,"<input type=\"checkbox\" name=\"QstCods\""
	                 " value=\"%ld\" />",
	       Gbl.Test.QstCod);

      /* Write number of question */
      fprintf (Gbl.F.Out,"<td class=\"DAT_SMALL CENTER_TOP COLOR%u\">"
	                 "%lu&nbsp;"
	                 "</td>",
               Gbl.RowEvenOdd,NumRow + 1);

      /* Write question code */
      fprintf (Gbl.F.Out,"<td class=\"DAT_SMALL CENTER_TOP COLOR%u\">"
	                 "%ld&nbsp;"
	                 "</td>",
               Gbl.RowEvenOdd,Gbl.Test.QstCod);

      /* Write the date (row[1] has the UTC date-time) */
      TimeUTC = Dat_GetUNIXTimeFromStr (row[1]);
      fprintf (Gbl.F.Out,"<td id=\"tst_date_%u\""
	                 " class=\"DAT_SMALL CENTER_TOP COLOR%u\">"
                         "<script type=\"text/javascript\">"
                         "writeLocalDateHMSFromUTC('tst_date_%u',%ld,"
                         "%u,'<br />','%s',true,false,0x7);"
                         "</script>"
                         "</td>",
               UniqueId,Gbl.RowEvenOdd,
               UniqueId,(long) TimeUTC,
               (unsigned) Gbl.Prefs.DateFormat,Txt_Today);

      /* Write the question tags */
      fprintf (Gbl.F.Out,"<td class=\"LEFT_TOP COLOR%u\">",
               Gbl.RowEvenOdd);
      Tst_GetAndWriteTagsQst (Gbl.Test.QstCod);
      fprintf (Gbl.F.Out,"</td>");

      /* Write the question type (row[2]) */
      Gbl.Test.AnswerType = Tst_ConvertFromStrAnsTypDBToAnsTyp (row[2]);
      fprintf (Gbl.F.Out,"<td class=\"DAT_SMALL CENTER_TOP COLOR%u\">"
	                 "%s&nbsp;"
	                 "</td>",
	       Gbl.RowEvenOdd,
               Txt_TST_STR_ANSWER_TYPES[Gbl.Test.AnswerType]);

      /* Write if shuffle is enabled (row[3]) */
      fprintf (Gbl.F.Out,"<td class=\"DAT_SMALL CENTER_TOP COLOR%u\">",
	       Gbl.RowEvenOdd);

      fprintf (Gbl.F.Out,"<input type=\"checkbox\" name=\"Shuffle\" value=\"Y\"");
      if (row[3][0] == 'Y')
	 fprintf (Gbl.F.Out," checked=\"checked\"");
      fprintf (Gbl.F.Out," disabled=\"disabled\" />");

      fprintf (Gbl.F.Out,"</td>");

      /* Write stem (row[4]) */
      fprintf (Gbl.F.Out,"<td class=\"LEFT_TOP COLOR%u\">",
	       Gbl.RowEvenOdd);
      Tst_WriteQstStem (row[4],"TEST_EDI");

      /***** Get and show media (row[6]) *****/
      Gbl.Test.Media.MedCod = Str_ConvertStrCodToLongCod (row[6]);
      Med_GetMediaDataByCod (&Gbl.Test.Media);
      Med_ShowMedia (&Gbl.Test.Media,
                     "TEST_MED_EDIT_LIST_CONTAINER",
                     "TEST_MED_EDIT_LIST");

      /* Write feedback (row[5]) */
      Tst_WriteQstFeedback (row[5],"TEST_EDI_LIGHT");

      /* Write answers */
      Tst_WriteAnswersEdit (Gbl.Test.QstCod);
      fprintf (Gbl.F.Out,"</td>"
	                 "</tr>");

      /***** Destroy test question *****/
      Tst_QstDestructor ();
     }

   /***** End table *****/
   Tbl_EndTable ();

   /***** Button to add questions *****/
   Btn_PutConfirmButton (Txt_Add_questions);

   /***** End form *****/
   Frm_EndForm ();

   /***** End box *****/
   Box_EndBox ();
  }

/*****************************************************************************/
/*********** Write hidden parameters for edition of test questions ***********/
/*****************************************************************************/

void Tst_WriteParamEditQst (void)
  {
   Par_PutHiddenParamChar   ("AllTags",
                             Gbl.Test.Tags.All ? 'Y' :
                        	                'N');
   Par_PutHiddenParamString ("ChkTag",
                             Gbl.Test.Tags.List ? Gbl.Test.Tags.List :
                        	                 "");
   Par_PutHiddenParamChar   ("AllAnsTypes",
                             Gbl.Test.AllAnsTypes ? 'Y' :
                        	                    'N');
   Par_PutHiddenParamString ("AnswerType",Gbl.Test.ListAnsTypes);
  }

/*****************************************************************************/
/*************** Get answers of a test question from database ****************/
/*****************************************************************************/

unsigned Tst_GetAnswersQst (long QstCod,MYSQL_RES **mysql_res,bool Shuffle)
  {
   unsigned long NumRows;

   /***** Get answers of a question from database *****/
   NumRows = DB_QuerySELECT (mysql_res,"can not get answers of a question",
			     "SELECT AnsInd,"		// row[0]
			            "Answer,"		// row[1]
			            "Feedback,"		// row[2]
			            "MedCod,"		// row[3]
			            "Correct"		// row[4]
			     " FROM tst_answers WHERE QstCod=%ld ORDER BY %s",
			     QstCod,
			     Shuffle ? "RAND(NOW())" :
				       "AnsInd");
   if (!NumRows)
      Ale_ShowAlert (Ale_ERROR,"Error when getting answers of a question.");

   return (unsigned) NumRows;
  }

/*****************************************************************************/
/**************** Get and write the answers of a test question ***************/
/*****************************************************************************/

static void Tst_WriteAnswersEdit (long QstCod)
  {
   extern const char *Txt_TST_Answer_given_by_the_teachers;
   unsigned NumOpt;
   unsigned i;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   char *Answer;
   char *Feedback;
   size_t LengthAnswer;
   size_t LengthFeedback;
   double FloatNum[2];

   Gbl.Test.Answer.NumOptions = Tst_GetAnswersQst (QstCod,&mysql_res,false);
   /*
   row[0] AnsInd
   row[1] Answer
   row[2] Feedback
   row[3] MedCod
   row[4] Correct
   */
   /***** Write the answers *****/
   switch (Gbl.Test.AnswerType)
     {
      case Tst_ANS_INT:
         Tst_CheckIfNumberOfAnswersIsOne ();
         row = mysql_fetch_row (mysql_res);
         fprintf (Gbl.F.Out,"<span class=\"TEST_EDI\">(%ld)</span>",
                  Tst_GetIntAnsFromStr (row[1]));
         break;
      case Tst_ANS_FLOAT:
	 if (Gbl.Test.Answer.NumOptions != 2)
            Lay_ShowErrorAndExit ("Wrong float range.");

         for (i = 0;
              i < 2;
              i++)
           {
            row = mysql_fetch_row (mysql_res);
            FloatNum[i] = Tst_GetFloatAnsFromStr (row[1]);
           }
         fprintf (Gbl.F.Out,"<span class=\"TEST_EDI\">([%lg; %lg])</span>",
                  FloatNum[0],FloatNum[1]);
         break;
      case Tst_ANS_TRUE_FALSE:
         Tst_CheckIfNumberOfAnswersIsOne ();
         row = mysql_fetch_row (mysql_res);
         fprintf (Gbl.F.Out,"<span class=\"TEST_EDI\">(");
         Tst_WriteAnsTF (row[1][0]);
         fprintf (Gbl.F.Out,")</span>");
         break;
      case Tst_ANS_UNIQUE_CHOICE:
      case Tst_ANS_MULTIPLE_CHOICE:
      case Tst_ANS_TEXT:
         Tbl_StartTable (2);
         for (NumOpt = 0;
              NumOpt < Gbl.Test.Answer.NumOptions;
              NumOpt++)
           {
            row = mysql_fetch_row (mysql_res);

            /* Convert the answer (row[1]), that is in HTML, to rigorous HTML */
            LengthAnswer = strlen (row[1]) * Str_MAX_BYTES_PER_CHAR;
            if ((Answer = (char *) malloc (LengthAnswer + 1)) == NULL)
               Lay_NotEnoughMemoryExit ();
            Str_Copy (Answer,row[1],
                      LengthAnswer);
            Str_ChangeFormat (Str_FROM_HTML,Str_TO_RIGOROUS_HTML,
                              Answer,LengthAnswer,false);

            /* Convert the feedback (row[2]), that is in HTML, to rigorous HTML */
            LengthFeedback = 0;
            Feedback = NULL;
            if (row[2])
               if (row[2][0])
        	 {
		  LengthFeedback = strlen (row[2]) * Str_MAX_BYTES_PER_CHAR;
		  if ((Feedback = (char *) malloc (LengthFeedback + 1)) == NULL)
		     Lay_NotEnoughMemoryExit ();
		  Str_Copy (Feedback,row[2],
		            LengthFeedback);
                  Str_ChangeFormat (Str_FROM_HTML,Str_TO_RIGOROUS_HTML,
                                    Feedback,LengthFeedback,false);
        	 }

	    /* Get media (row[3]) */
	    Gbl.Test.Answer.Options[NumOpt].Media.MedCod = Str_ConvertStrCodToLongCod (row[3]);
	    Med_GetMediaDataByCod (&Gbl.Test.Answer.Options[NumOpt].Media);

            /* Put an icon that indicates whether the answer
               is correct or wrong (row[4]) */
            fprintf (Gbl.F.Out,"<tr>"
        	               "<td class=\"BT%u\">",
        	     Gbl.RowEvenOdd);
            if (row[4][0] == 'Y')
               fprintf (Gbl.F.Out,"<img src=\"%s/check.svg\""
        	                  " alt=\"%s\" title=\"%s\""
        	                  " class=\"CONTEXT_ICO_16x16\" />",
                        Cfg_URL_ICON_PUBLIC,
                        Txt_TST_Answer_given_by_the_teachers,
                        Txt_TST_Answer_given_by_the_teachers);
            fprintf (Gbl.F.Out,"</td>");

            /* Write the number of option */
            fprintf (Gbl.F.Out,"<td class=\"DAT_SMALL LEFT_TOP\">"
        	               "%c)&nbsp;"
        	               "</td>",
                     'a' + (char) NumOpt);

            /* Write the text of the answer and the image */
            fprintf (Gbl.F.Out,"<td class=\"LEFT_TOP\">"
        	               "<div class=\"TEST_EDI\">"
        	               "%s",
                     Answer);
	    Med_ShowMedia (&Gbl.Test.Answer.Options[NumOpt].Media,
	                   "TEST_MED_EDIT_LIST_CONTAINER",
	                   "TEST_MED_EDIT_LIST");
            fprintf (Gbl.F.Out,"</div>");

            /* Write the text of the feedback */
            fprintf (Gbl.F.Out,"<div class=\"TEST_EDI_LIGHT\">");
            if (LengthFeedback)
	       fprintf (Gbl.F.Out,"%s",Feedback);
            fprintf (Gbl.F.Out,"</div>"
        	               "</td>"
        	               "</tr>");

	    /* Free memory allocated for the answer and the feedback */
	    free ((void *) Answer);
	    if (LengthFeedback)
	       free ((void *) Feedback);
           }
         Tbl_EndTable ();
	 break;
      default:
         break;
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/************** Write answers of a question when viewing a test **************/
/*****************************************************************************/

static void Tst_WriteAnswersTestToAnswer (unsigned NumQst,long QstCod,bool Shuffle)
  {
   /***** Write parameter with question code *****/
   Tst_WriteParamQstCod (NumQst,QstCod);

   /***** Write answer depending on type *****/
   switch (Gbl.Test.AnswerType)
     {
      case Tst_ANS_INT:
         Tst_WriteIntAnsViewTest (NumQst);
         break;
      case Tst_ANS_FLOAT:
         Tst_WriteFloatAnsViewTest (NumQst);
         break;
      case Tst_ANS_TRUE_FALSE:
         Tst_WriteTFAnsViewTest (NumQst);
         break;
      case Tst_ANS_UNIQUE_CHOICE:
      case Tst_ANS_MULTIPLE_CHOICE:
         Tst_WriteChoiceAnsViewTest (NumQst,QstCod,Shuffle);
         break;
      case Tst_ANS_TEXT:
         Tst_WriteTextAnsViewTest (NumQst);
         break;
      default:
         break;
     }
  }

/*****************************************************************************/
/************* Write answers of a question when assessing a test *************/
/*****************************************************************************/

static void Tst_WriteAnswersTestResult (struct UsrData *UsrDat,
                                        unsigned NumQst,long QstCod,
                                        double *ScoreThisQst,bool *AnswerIsNotBlank)
  {
   MYSQL_RES *mysql_res;

   /***** Get answers of a question from database *****/
   Gbl.Test.Answer.NumOptions = Tst_GetAnswersQst (QstCod,&mysql_res,false);
   /*
   row[0] AnsInd
   row[1] Answer
   row[2] Feedback
   row[3] MedCod
   row[4] Correct
   */
   /***** Write answer depending on type *****/
   switch (Gbl.Test.AnswerType)
     {
      case Tst_ANS_INT:
         Tst_WriteIntAnsAssessTest    (UsrDat,NumQst,mysql_res,ScoreThisQst,AnswerIsNotBlank);
         break;
      case Tst_ANS_FLOAT:

	 Tst_WriteFloatAnsAssessTest  (UsrDat,NumQst,mysql_res,ScoreThisQst,AnswerIsNotBlank);
         break;
      case Tst_ANS_TRUE_FALSE:
         Tst_WriteTFAnsAssessTest     (UsrDat,NumQst,mysql_res,ScoreThisQst,AnswerIsNotBlank);
         break;
      case Tst_ANS_UNIQUE_CHOICE:
      case Tst_ANS_MULTIPLE_CHOICE:
         Tst_WriteChoiceAnsAssessTest (UsrDat,NumQst,mysql_res,ScoreThisQst,AnswerIsNotBlank);
         break;
      case Tst_ANS_TEXT:
         Tst_WriteTextAnsAssessTest   (UsrDat,NumQst,mysql_res,ScoreThisQst,AnswerIsNotBlank);
         break;
      default:
         break;
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/************** Write answers of a question when viewing a game **************/
/*****************************************************************************/

void Tst_WriteAnswersGameResult (struct Game *Game,unsigned NumQst,long QstCod,
                                 const char *Class,bool ShowResult)
  {
   /***** Write parameter with question code *****/
   // Tst_WriteParamQstCod (NumQst,QstCod);

   /***** Write answer depending on type *****/
   switch (Gbl.Test.AnswerType)
     {
      case Tst_ANS_INT:
      case Tst_ANS_FLOAT:
      case Tst_ANS_TRUE_FALSE:
      case Tst_ANS_TEXT:
         Ale_ShowAlert (Ale_ERROR,"Type of answer not valid in a game.");
         break;
      case Tst_ANS_UNIQUE_CHOICE:
      case Tst_ANS_MULTIPLE_CHOICE:
         Tst_WriteChoiceAnsViewGame (Game,NumQst,QstCod,
                                     Class,ShowResult);
         break;
      default:
         break;
     }
  }

/*****************************************************************************/
/************** Write false / true answer when viewing a test ****************/
/*****************************************************************************/

static void Tst_WriteTFAnsViewTest (unsigned NumQst)
  {
   extern const char *Txt_TF_QST[2];

   /***** Write selector for the answer *****/
   fprintf (Gbl.F.Out,"<select name=\"Ans%06u\">"
                      "<option value=\"\" selected=\"selected\">&nbsp;</option>"
                      "<option value=\"T\">%s</option>"
                      "<option value=\"F\">%s</option>"
                      "</select>",
            NumQst,
            Txt_TF_QST[0],
            Txt_TF_QST[1]);
  }

/*****************************************************************************/
/************** Write false / true answer when viewing a test ****************/
/*****************************************************************************/

void Tst_WriteAnsTF (char AnsTF)
  {
   extern const char *Txt_TF_QST[2];

   switch (AnsTF)
     {
      case 'T':		// true
         fprintf (Gbl.F.Out,"%s",Txt_TF_QST[0]);
         break;
      case 'F':		// false
         fprintf (Gbl.F.Out,"%s",Txt_TF_QST[1]);
         break;
      default:		// no answer
         fprintf (Gbl.F.Out,"&nbsp;");
         break;
     }
  }

/*****************************************************************************/
/************** Write false / true answer when assessing a test **************/
/*****************************************************************************/

static void Tst_WriteTFAnsAssessTest (struct UsrData *UsrDat,
				      unsigned NumQst,MYSQL_RES *mysql_res,
                                      double *ScoreThisQst,bool *AnswerIsNotBlank)
  {
   MYSQL_ROW row;
   char AnsTF;
   /*
   row[0] AnsInd
   row[1] Answer
   row[2] Feedback
   row[3] MedCod
   row[4] Correct
   */
   /***** Check if number of rows is correct *****/
   Tst_CheckIfNumberOfAnswersIsOne ();

   /***** Get answer true or false *****/
   row = mysql_fetch_row (mysql_res);

   /***** Compute the mark for this question *****/
   AnsTF = Gbl.Test.StrAnswersOneQst[NumQst][0];
   if (AnsTF == '\0')			// User has omitted the answer (the answer is blank)
     {
      *AnswerIsNotBlank = false;
      *ScoreThisQst = 0.0;
     }
   else
     {
      *AnswerIsNotBlank = true;
      if (AnsTF == row[1][0])		// Correct
         *ScoreThisQst = 1.0;
      else				// Wrong
         *ScoreThisQst = -1.0;
     }

   /***** Header with the title of each column *****/
   Tbl_StartTable (2);
   fprintf (Gbl.F.Out,"<tr>");
   Tst_WriteHeadUserCorrect (UsrDat);
   fprintf (Gbl.F.Out,"</tr>");

   /***** Write the user answer *****/
   fprintf (Gbl.F.Out,"<tr>"
	              "<td class=\"%s CENTER_MIDDLE\">",
            (Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_GOOD_BAD ||
             Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK) ?
            (AnsTF == row[1][0] ? "ANS_OK" :
        	                  "ANS_BAD") :
            "ANS_0");
   Tst_WriteAnsTF (AnsTF);
   fprintf (Gbl.F.Out,"</td>");

   /***** Write the correct answer *****/
   fprintf (Gbl.F.Out,"<td class=\"ANS_0 CENTER_MIDDLE\">");
   if (Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_GOOD_BAD ||
       Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK)
      Tst_WriteAnsTF (row[1][0]);
   else
      fprintf (Gbl.F.Out,"?");
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");

   /***** Write the mark *****/
   if (Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_RESULT ||
       Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_GOOD_BAD ||
       Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK)
     {
      Tst_WriteScoreStart (2);
      if (AnsTF == '\0')		// If user has omitted the answer
         fprintf (Gbl.F.Out,"ANS_0\">%.2lf",0.0);
      else if (AnsTF == row[1][0])	// If correct
         fprintf (Gbl.F.Out,"ANS_OK\">%.2lf",1.0);
      else				// If wrong
         fprintf (Gbl.F.Out,"ANS_BAD\">%.2lf",-1.0);
      Tst_WriteScoreEnd ();
     }

   Tbl_EndTable ();
  }

/*****************************************************************************/
/******** Write single or multiple choice answer when viewing a test *********/
/*****************************************************************************/

static void Tst_WriteChoiceAnsViewTest (unsigned NumQst,long QstCod,bool Shuffle)
  {
   unsigned NumOpt;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned Index;
   bool ErrorInIndex = false;
   char ParamName[3 + 6 + 1];

   /***** Get answers of a question from database *****/
   Gbl.Test.Answer.NumOptions = Tst_GetAnswersQst (QstCod,&mysql_res,Shuffle);
   /*
   row[0] AnsInd
   row[1] Answer
   row[2] Feedback
   row[3] MedCod
   row[4] Correct
   */

   /***** Start table *****/
   Tbl_StartTable (2);

   for (NumOpt = 0;
	NumOpt < Gbl.Test.Answer.NumOptions;
	NumOpt++)
     {
      /***** Get next answer *****/
      row = mysql_fetch_row (mysql_res);

      /***** Allocate memory for text in this choice answer *****/
      if (!Tst_AllocateTextChoiceAnswer (NumOpt))
         /* Abort on error */
	 Ale_ShowAlertsAndExit ();

      /***** Assign index (row[0]).
             Index is 0,1,2,3... if no shuffle
             or 1,3,0,2... (example) if shuffle *****/
      if (sscanf (row[0],"%u",&Index) == 1)
        {
         if (Index >= Tst_MAX_OPTIONS_PER_QUESTION)
            ErrorInIndex = true;
        }
      else
         ErrorInIndex = true;
      if (ErrorInIndex)
         Lay_ShowErrorAndExit ("Wrong index of answer when showing a test.");

      /***** Copy text (row[1]) and convert it, that is in HTML, to rigorous HTML ******/
      Str_Copy (Gbl.Test.Answer.Options[NumOpt].Text,row[1],
                Tst_MAX_BYTES_ANSWER_OR_FEEDBACK);
      Str_ChangeFormat (Str_FROM_HTML,Str_TO_RIGOROUS_HTML,
                        Gbl.Test.Answer.Options[NumOpt].Text,
                        Tst_MAX_BYTES_ANSWER_OR_FEEDBACK,false);

      /***** Get media (row[3]) *****/
      Gbl.Test.Answer.Options[NumOpt].Media.MedCod = Str_ConvertStrCodToLongCod (row[3]);
      Med_GetMediaDataByCod (&Gbl.Test.Answer.Options[NumOpt].Media);

      /***** Write selectors and letter of this option *****/
      fprintf (Gbl.F.Out,"<tr>"
	                 "<td class=\"LEFT_TOP\">");
      snprintf (ParamName,sizeof (ParamName),
	        "Ind%06u",
		NumQst);
      Par_PutHiddenParamUnsigned (ParamName,Index);
      fprintf (Gbl.F.Out,"<input type=\"");
      if (Gbl.Test.AnswerType == Tst_ANS_UNIQUE_CHOICE)
         fprintf (Gbl.F.Out,"radio\""
                            " onclick=\"selectUnselectRadio(this,this.form.Ans%06u,%u);\"",
                  NumQst,Gbl.Test.Answer.NumOptions);
      else // Gbl.Test.AnswerType == Tst_ANS_MULTIPLE_CHOICE
         fprintf (Gbl.F.Out,"checkbox\"");
      fprintf (Gbl.F.Out," id=\"Ans%06u_%u\" name=\"Ans%06u\" value=\"%u\" />"
	                 "</td>",
               NumQst,NumOpt,
               NumQst,Index);
      fprintf (Gbl.F.Out,"<td class=\"LEFT_TOP\">"
                         "<label for=\"Ans%06u_%u\" class=\"ANS_TXT\">"
	                 "%c)&nbsp;"
	                 "</label>"
	                 "</td>",
	       NumQst,NumOpt,
	       'a' + (char) NumOpt);

      /***** Write the option text *****/
      fprintf (Gbl.F.Out,"<td class=\"LEFT_TOP\">"
                         "<label for=\"Ans%06u_%u\" class=\"ANS_TXT\">"
	                 "%s"
	                 "</label>",
               NumQst,NumOpt,
               Gbl.Test.Answer.Options[NumOpt].Text);
      Med_ShowMedia (&Gbl.Test.Answer.Options[NumOpt].Media,
                     "TEST_MED_SHOW_CONTAINER",
                     "TEST_MED_SHOW");
      fprintf (Gbl.F.Out,"</td>"
	                 "</tr>");
     }

   /***** End table *****/
   Tbl_EndTable ();

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/******* Write single or multiple choice answer when assessing a test ********/
/*****************************************************************************/

static void Tst_WriteChoiceAnsAssessTest (struct UsrData *UsrDat,
				          unsigned NumQst,MYSQL_RES *mysql_res,
                                          double *ScoreThisQst,bool *AnswerIsNotBlank)
  {
   extern const char *Txt_TST_Answer_given_by_the_user;
   extern const char *Txt_TST_Answer_given_by_the_teachers;
   unsigned NumOpt;
   MYSQL_ROW row;
   char StrOneIndex[10 + 1];
   const char *Ptr;
   unsigned Indexes[Tst_MAX_OPTIONS_PER_QUESTION];	// Indexes of all answers of this question
   int AnsUsr;
   bool AnswersUsr[Tst_MAX_OPTIONS_PER_QUESTION];
   struct
     {
      char *Class;
      char *Str;
     } Ans;
   unsigned NumOptTotInQst = 0;
   unsigned NumOptCorrInQst = 0;
   unsigned NumAnsGood = 0;
   unsigned NumAnsBad = 0;

   /***** Get text and correctness of answers for this question
          from database (one row per answer) *****/
   /*
   row[0] AnsInd
   row[1] Answer
   row[2] Feedback
   row[3] MedCod
   row[4] Correct
   */
   for (NumOpt = 0;
	NumOpt < Gbl.Test.Answer.NumOptions;
	NumOpt++)
     {
      /***** Get next answer *****/
      row = mysql_fetch_row (mysql_res);

      /***** Allocate memory for text in this choice option *****/
      if (!Tst_AllocateTextChoiceAnswer (NumOpt))
	 /* Abort on error */
	 Ale_ShowAlertsAndExit ();

      /***** Copy answer text (row[1]) and convert it,
             that is in HTML, to rigorous HTML ******/
      Str_Copy (Gbl.Test.Answer.Options[NumOpt].Text,row[1],
                Tst_MAX_BYTES_ANSWER_OR_FEEDBACK);
      Str_ChangeFormat (Str_FROM_HTML,Str_TO_RIGOROUS_HTML,
                        Gbl.Test.Answer.Options[NumOpt].Text,
                        Tst_MAX_BYTES_ANSWER_OR_FEEDBACK,false);

      /***** Copy answer feedback (row[2]) and convert it,
             that is in HTML, to rigorous HTML ******/
      if (Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK)
	 if (row[2])
	    if (row[2][0])
	      {
	       Str_Copy (Gbl.Test.Answer.Options[NumOpt].Feedback,row[2],
	                 Tst_MAX_BYTES_ANSWER_OR_FEEDBACK);
	       Str_ChangeFormat (Str_FROM_HTML,Str_TO_RIGOROUS_HTML,
	                         Gbl.Test.Answer.Options[NumOpt].Feedback,
	                         Tst_MAX_BYTES_ANSWER_OR_FEEDBACK,false);
	      }

      /***** Get media (row[3]) *****/
      Gbl.Test.Answer.Options[NumOpt].Media.MedCod = Str_ConvertStrCodToLongCod (row[3]);
      Med_GetMediaDataByCod (&Gbl.Test.Answer.Options[NumOpt].Media);

      /***** Assign correctness (row[4]) of this answer (this option) *****/
      Gbl.Test.Answer.Options[NumOpt].Correct = (row[4][0] == 'Y');
     }

   /***** Get indexes for this question from string *****/
   for (NumOpt = 0, Ptr = Gbl.Test.StrIndexesOneQst[NumQst];
	NumOpt < Gbl.Test.Answer.NumOptions;
	NumOpt++)
     {
      Par_GetNextStrUntilSeparParamMult (&Ptr,StrOneIndex,10);
      if (sscanf (StrOneIndex,"%u",&(Indexes[NumOpt])) != 1)
         Lay_ShowErrorAndExit ("Wrong index of answer when assessing a test.");
     }

   /***** Get the user's answers for this question from string *****/
   for (NumOpt = 0;
	NumOpt < Tst_MAX_OPTIONS_PER_QUESTION;
	NumOpt++)
      AnswersUsr[NumOpt] = false;
   for (NumOpt = 0, Ptr = Gbl.Test.StrAnswersOneQst[NumQst];
	NumOpt < Gbl.Test.Answer.NumOptions;
	NumOpt++)
      if (*Ptr)
        {
         Par_GetNextStrUntilSeparParamMult (&Ptr,StrOneIndex,10);
         if (sscanf (StrOneIndex,"%d",&AnsUsr) != 1)
            Lay_ShowErrorAndExit ("Bad user's answer.");
         if (AnsUsr < 0 || AnsUsr >= Tst_MAX_OPTIONS_PER_QUESTION)
            Lay_ShowErrorAndExit ("Bad user's answer.");
         AnswersUsr[AnsUsr] = true;
        }

   /***** Start table *****/
   Tbl_StartTable (2);
   fprintf (Gbl.F.Out,"<tr>");
   Tst_WriteHeadUserCorrect (UsrDat);
   fprintf (Gbl.F.Out,"<td></td>"
	              "<td></td>"
	              "</tr>");

   /***** Write answers (one row per answer) *****/
   for (NumOpt = 0;
	NumOpt < Gbl.Test.Answer.NumOptions;
	NumOpt++)
     {
      fprintf (Gbl.F.Out,"<tr>");

      /* Draw icon depending on user's answer */
      if (AnswersUsr[Indexes[NumOpt]] == true)	// This answer has been selected by the user
        {
         if (Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_GOOD_BAD ||
             Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK)
           {
            if (Gbl.Test.Answer.Options[Indexes[NumOpt]].Correct)
              {
               Ans.Class = "ANS_OK";
               Ans.Str   = "&check;";
              }
            else
              {
               Ans.Class = "ANS_BAD";
               Ans.Str   = "&cross;";
              }
           }
         else
	   {
	    Ans.Class = "ANS_0";
	    Ans.Str   = "&bull;";
	   }
	 fprintf (Gbl.F.Out,"<td class=\"%s CENTER_TOP\" title=\"%s\">%s</td>",
			    Ans.Class,Txt_TST_Answer_given_by_the_user,Ans.Str);
        }
      else	// This answer has NOT been selected by the user
	 fprintf (Gbl.F.Out,"<td></td>");

      /* Draw icon that indicates whether the answer is correct */

      if (Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_GOOD_BAD ||
          Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK)
        {
         if (Gbl.Test.Answer.Options[Indexes[NumOpt]].Correct)
	    fprintf (Gbl.F.Out,"<td class=\"ANS_0 CENTER_TOP\" title=\"%s\">&bull;</td>",
		     Txt_TST_Answer_given_by_the_teachers);
         else
	    fprintf (Gbl.F.Out,"<td></td>");
        }
      else
	 fprintf (Gbl.F.Out,"<td class=\"ANS_0 CENTER_TOP\">?</td>");

      /* Answer letter (a, b, c,...) */
      fprintf (Gbl.F.Out,"<td class=\"ANS_TXT LEFT_TOP\">"
	                 "%c)&nbsp;"
	                 "</td>",
               'a' + (char) NumOpt);

      /* Answer text and feedback */
      fprintf (Gbl.F.Out,"<td class=\"LEFT_TOP\">"
	                 "<div class=\"ANS_TXT\">"
	                 "%s",
               Gbl.Test.Answer.Options[Indexes[NumOpt]].Text);
      Med_ShowMedia (&Gbl.Test.Answer.Options[Indexes[NumOpt]].Media,
                     "TEST_MED_SHOW_CONTAINER",
                     "TEST_MED_SHOW");
      fprintf (Gbl.F.Out,"</div>");
      if (Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK)
	 if (Gbl.Test.Answer.Options[Indexes[NumOpt]].Feedback)
	    if (Gbl.Test.Answer.Options[Indexes[NumOpt]].Feedback[0])
	       fprintf (Gbl.F.Out,"<div class=\"TEST_EXA_LIGHT\">"
				  "%s"
				  "</div>",
			Gbl.Test.Answer.Options[Indexes[NumOpt]].Feedback);
      fprintf (Gbl.F.Out,"</td>"
	                 "</tr>");

      NumOptTotInQst++;
      if (AnswersUsr[Indexes[NumOpt]] == true)	// This answer has been selected by the user
        {
         if (Gbl.Test.Answer.Options[Indexes[NumOpt]].Correct)
            NumAnsGood++;
         else
            NumAnsBad++;
        }
      if (Gbl.Test.Answer.Options[Indexes[NumOpt]].Correct)
         NumOptCorrInQst++;
     }

   /***** The answer is blank? *****/
   *AnswerIsNotBlank = NumAnsGood != 0 || NumAnsBad != 0;

   /***** Compute and write the total score of this question *****/
   if (*AnswerIsNotBlank)
     {
      /* Compute the score */
      if (Gbl.Test.AnswerType == Tst_ANS_UNIQUE_CHOICE)
        {
         if (NumOptTotInQst >= 2)	// It should be 2 options at least
            *ScoreThisQst = (double) NumAnsGood -
                            (double) NumAnsBad / (double) (NumOptTotInQst - 1);
         else			// 0 or 1 options (impossible)
            *ScoreThisQst = (double) NumAnsGood;
        }
      else	// Gbl.Test.AnswerType == Tst_ANS_MULTIPLE_CHOICE
        {
         if (NumOptCorrInQst)	// There are correct options in the question
           {
            if (NumOptCorrInQst < NumOptTotInQst)	// If there are correct options and wrong options (typical case)
               *ScoreThisQst = (double) NumAnsGood / (double) NumOptCorrInQst -
                               (double) NumAnsBad / (double) (NumOptTotInQst - NumOptCorrInQst);
            else					// Si todas the opciones son correctas (caso raro)
               *ScoreThisQst = (double) NumAnsGood / (double) NumOptCorrInQst;
           }
         else
           {
            if (NumOptTotInQst)	// There are options but none is correct (extrange case)
               *ScoreThisQst = - (double) NumAnsBad / (double) NumOptTotInQst;
            else			// There are no options (impossible!)
               *ScoreThisQst = 0.0;
           }
        }
     }
   else	// Answer is blank
      *ScoreThisQst = 0.0;

   /* Write the score */
   if (Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_RESULT ||
       Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_GOOD_BAD ||
       Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK)
     {
      Tst_WriteScoreStart (4);
      if (*ScoreThisQst == 0.0)
         fprintf (Gbl.F.Out,"ANS_0");
      else if (*ScoreThisQst > 0.0)
         fprintf (Gbl.F.Out,"ANS_OK");
      else
         fprintf (Gbl.F.Out,"ANS_BAD");
      fprintf (Gbl.F.Out,"\">%.2lf",*ScoreThisQst);
      Tst_WriteScoreEnd ();
     }

   /***** End table *****/
   Tbl_EndTable ();
  }

/*****************************************************************************/
/******** Write single or multiple choice answer when viewing a test *********/
/*****************************************************************************/

static void Tst_WriteChoiceAnsViewGame (struct Game *Game,
                                        unsigned NumQst,long QstCod,
                                        const char *Class,
                                        bool ShowResult)
  {
   unsigned NumOpt;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned AnsInd;
   bool ErrorInIndex = false;

   /***** Get answers of a question from database *****/
   Gbl.Test.Answer.NumOptions = Tst_GetAnswersQst (QstCod,&mysql_res,false);
   /*
   row[0] AnsInd
   row[1] Answer
   row[2] Feedback
   row[3] MedCod
   row[4] Correct
   */

   /***** Start table *****/
   Tbl_StartTable (2);

   for (NumOpt = 0;
	NumOpt < Gbl.Test.Answer.NumOptions;
	NumOpt++)
     {
      /***** Get next answer *****/
      row = mysql_fetch_row (mysql_res);

      /***** Allocate memory for text in this choice answer *****/
      if (!Tst_AllocateTextChoiceAnswer (NumOpt))
	 /* Abort on error */
	 Ale_ShowAlertsAndExit ();

      /***** Assign index (row[0]).
             Index is 0,1,2,3... if no shuffle
             or 1,3,0,2... (example) if shuffle *****/
      if (sscanf (row[0],"%u",&AnsInd) == 1)
        {
         if (AnsInd >= Tst_MAX_OPTIONS_PER_QUESTION)
            ErrorInIndex = true;
        }
      else
         ErrorInIndex = true;
      if (ErrorInIndex)
         Lay_ShowErrorAndExit ("Wrong index of answer when showing a test.");

      /***** Copy text (row[1]) and convert it, that is in HTML, to rigorous HTML ******/
      Str_Copy (Gbl.Test.Answer.Options[NumOpt].Text,row[1],
                Tst_MAX_BYTES_ANSWER_OR_FEEDBACK);
      Str_ChangeFormat (Str_FROM_HTML,Str_TO_RIGOROUS_HTML,
                        Gbl.Test.Answer.Options[NumOpt].Text,
                        Tst_MAX_BYTES_ANSWER_OR_FEEDBACK,false);

      /***** Get media (row[3]) *****/
      Gbl.Test.Answer.Options[NumOpt].Media.MedCod = Str_ConvertStrCodToLongCod (row[3]);
      Med_GetMediaDataByCod (&Gbl.Test.Answer.Options[NumOpt].Media);

      /***** Write letter of this option *****/
      fprintf (Gbl.F.Out,"<tr>"
	                 "<td class=\"LEFT_TOP\">"
                         "<label for=\"Ans%06u_%u\" class=\"%s\">"
	                 "%c)&nbsp;"
	                 "</label>"
	                 "</td>",
	       NumQst,NumOpt,Class,
	       'a' + (char) NumOpt);

      /***** Write the option text *****/
      fprintf (Gbl.F.Out,"<td class=\"LEFT_TOP\">"
                         "<label for=\"Ans%06u_%u\" class=\"%s\">"
	                 "%s"
	                 "</label>",
               NumQst,NumOpt,Class,
               Gbl.Test.Answer.Options[NumOpt].Text);
      Med_ShowMedia (&Gbl.Test.Answer.Options[NumOpt].Media,
                     "TEST_MED_SHOW_CONTAINER",
                     "TEST_MED_SHOW");
      fprintf (Gbl.F.Out,"</td>"
	                 "</tr>");

      /***** Show result (number of users who answered? *****/
      fprintf (Gbl.F.Out,"<tr>"
	                 "<td class=\"LEFT_TOP\">");
      if (ShowResult)
	 /* Get number of users who selected this answer
	    and draw proportional bar */
	 Gam_GetAndDrawBarNumUsrsWhoAnswered (Game,QstCod,AnsInd);
      else
         fprintf (Gbl.F.Out,"&nbsp;");
      fprintf (Gbl.F.Out,"</td>"
	                 "</tr>");
     }

   /***** End table *****/
   Tbl_EndTable ();

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/******************** Write text answer when viewing a test ******************/
/*****************************************************************************/

static void Tst_WriteTextAnsViewTest (unsigned NumQst)
  {
   /***** Write input field for the answer *****/
   fprintf (Gbl.F.Out,"<input type=\"text\" name=\"Ans%06u\""
	              " size=\"40\" maxlength=\"%u\" value=\"\" />",
            NumQst,Tst_MAX_BYTES_ANSWERS_ONE_QST);
  }

/*****************************************************************************/
/***************** Write text answer when assessing a test *******************/
/*****************************************************************************/

static void Tst_WriteTextAnsAssessTest (struct UsrData *UsrDat,
				        unsigned NumQst,MYSQL_RES *mysql_res,
                                        double *ScoreThisQst,bool *AnswerIsNotBlank)
  {
   unsigned NumOpt;
   MYSQL_ROW row;
   char TextAnsUsr[Tst_MAX_BYTES_ANSWERS_ONE_QST + 1];
   char TextAnsOK[Tst_MAX_BYTES_ANSWERS_ONE_QST + 1];
   bool Correct = false;
   /*
   row[0] AnsInd
   row[1] Answer
   row[2] Feedback
   row[3] MedCod
   row[4] Correct
   */
   /***** Get text and correctness of answers for this question from database (one row per answer) *****/
   for (NumOpt = 0;
	NumOpt < Gbl.Test.Answer.NumOptions;
	NumOpt++)
     {
      /***** Get next answer *****/
      row = mysql_fetch_row (mysql_res);

      /***** Allocate memory for text in this choice answer *****/
      if (!Tst_AllocateTextChoiceAnswer (NumOpt))
	 /* Abort on error */
	 Ale_ShowAlertsAndExit ();

      /***** Copy answer text (row[1]) and convert it, that is in HTML, to rigorous HTML ******/
      Str_Copy (Gbl.Test.Answer.Options[NumOpt].Text,row[1],
                Tst_MAX_BYTES_ANSWER_OR_FEEDBACK);
      Str_ChangeFormat (Str_FROM_HTML,Str_TO_RIGOROUS_HTML,
                        Gbl.Test.Answer.Options[NumOpt].Text,
                        Tst_MAX_BYTES_ANSWER_OR_FEEDBACK,false);

      /***** Copy answer feedback (row[2]) and convert it, that is in HTML, to rigorous HTML ******/
      if (Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK)
	 if (row[2])
	    if (row[2][0])
	      {
	       Str_Copy (Gbl.Test.Answer.Options[NumOpt].Feedback,row[2],
	                 Tst_MAX_BYTES_ANSWER_OR_FEEDBACK);
	       Str_ChangeFormat (Str_FROM_HTML,Str_TO_RIGOROUS_HTML,
	                         Gbl.Test.Answer.Options[NumOpt].Feedback,
	                         Tst_MAX_BYTES_ANSWER_OR_FEEDBACK,false);
	      }

      /***** Assign correctness (row[4]) of this answer (this option) *****/
      Gbl.Test.Answer.Options[NumOpt].Correct = (row[4][0] == 'Y');
     }

   /***** Header with the title of each column *****/
   Tbl_StartTable (2);
   fprintf (Gbl.F.Out,"<tr>");
   Tst_WriteHeadUserCorrect (UsrDat);
   fprintf (Gbl.F.Out,"</tr>");

   /***** Write the user answer *****/
   fprintf (Gbl.F.Out,"<tr>"
	              "<td");
   if (Gbl.Test.StrAnswersOneQst[NumQst][0])	// If user has answered the question
     {
      /* Filter the user answer */
      Str_Copy (TextAnsUsr,Gbl.Test.StrAnswersOneQst[NumQst],
                Tst_MAX_BYTES_ANSWERS_ONE_QST);

      /* In order to compare student answer to stored answer,
	 the text answers are stored avoiding two or more consecurive spaces */
      Str_ReplaceSeveralSpacesForOne (TextAnsUsr);

      Str_ConvertToComparable (TextAnsUsr);

      for (NumOpt = 0;
	   NumOpt < Gbl.Test.Answer.NumOptions;
	   NumOpt++)
        {
         /* Filter this correct answer */
         Str_Copy (TextAnsOK,Gbl.Test.Answer.Options[NumOpt].Text,
                   Tst_MAX_BYTES_ANSWERS_ONE_QST);
         Str_ConvertToComparable (TextAnsOK);

         /* Check is user answer is correct */
         if (!strcoll (TextAnsUsr,TextAnsOK))
           {
            Correct = true;
            break;
           }
        }
      fprintf (Gbl.F.Out," class=\"%s CENTER_TOP\">"
	                 "%s",
               (Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_GOOD_BAD ||
                Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK) ?
                (Correct ? "ANS_OK" :
                           "ANS_BAD") :
                "ANS_0",
               Gbl.Test.StrAnswersOneQst[NumQst]);
     }
   else						// If user has omitted the answer
      fprintf (Gbl.F.Out,">");
   fprintf (Gbl.F.Out,"</td>");

   /***** Write the correct answers *****/
   if (Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_GOOD_BAD ||
       Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK)
     {
      fprintf (Gbl.F.Out,"<td class=\"CENTER_TOP\">");
      Tbl_StartTable (2);

      for (NumOpt = 0;
	   NumOpt < Gbl.Test.Answer.NumOptions;
	   NumOpt++)
        {
         /* Answer letter (a, b, c,...) */
         fprintf (Gbl.F.Out,"<td class=\"ANS_0 LEFT_TOP\">"
                            "%c)&nbsp;"
                            "</td>",
                  'a' + (char) NumOpt);

         /* Answer text and feedback */
         fprintf (Gbl.F.Out,"<td class=\"LEFT_TOP\">"
                            "<div class=\"ANS_0\">"
                            "%s"
                            "</div>",
                  Gbl.Test.Answer.Options[NumOpt].Text);
	 if (Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK)
	    if (Gbl.Test.Answer.Options[NumOpt].Feedback)
	       if (Gbl.Test.Answer.Options[NumOpt].Feedback[0])
		  fprintf (Gbl.F.Out,"<div class=\"TEST_EXA_LIGHT\">"
				     "%s"
				     "</div>",
			   Gbl.Test.Answer.Options[NumOpt].Feedback);
	 fprintf (Gbl.F.Out,"</td>"
			    "</tr>");
        }

      Tbl_EndTable ();
     }
   else
      fprintf (Gbl.F.Out,"<td class=\"ANS_0 CENTER_TOP\">"
	                 "?");
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");

   /***** Compute the mark *****/
   if (!Gbl.Test.StrAnswersOneQst[NumQst][0])	// If user has omitted the answer
     {
      *AnswerIsNotBlank = false;
      *ScoreThisQst = 0.0;
     }
   else
     {
      *AnswerIsNotBlank = true;
      if (Correct)				// If correct
         *ScoreThisQst = 1.0;
      else					// If wrong
         *ScoreThisQst = 0.0;
     }

   /***** Write the mark *****/
   if (Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_RESULT ||
       Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_GOOD_BAD ||
       Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK)
     {
      Tst_WriteScoreStart (4);
      if (!Gbl.Test.StrAnswersOneQst[NumQst][0])	// If user has omitted the answer
         fprintf (Gbl.F.Out,"ANS_0\">%.2lf",0.0);
      else if (Correct)				// If correct
         fprintf (Gbl.F.Out,"ANS_OK\">%.2lf",1.0);
      else					// If wrong
         fprintf (Gbl.F.Out,"ANS_BAD\">%.2lf",0.0);
      Tst_WriteScoreEnd ();
     }

   Tbl_EndTable ();
  }

/*****************************************************************************/
/****************** Write integer answer when viewing a test *****************/
/*****************************************************************************/

static void Tst_WriteIntAnsViewTest (unsigned NumQst)
  {
   /***** Write input field for the answer *****/
   fprintf (Gbl.F.Out,"<input type=\"text\" name=\"Ans%06u\""
	              " size=\"11\" maxlength=\"11\" value=\"\" />",
            NumQst);
  }

/*****************************************************************************/
/**************** Write integer answer when assessing a test *****************/
/*****************************************************************************/

static void Tst_WriteIntAnsAssessTest (struct UsrData *UsrDat,
				       unsigned NumQst,MYSQL_RES *mysql_res,
                                       double *ScoreThisQst,bool *AnswerIsNotBlank)
  {
   MYSQL_ROW row;
   long IntAnswerUsr;
   long IntAnswerCorr;
   /*
   row[0] AnsInd
   row[1] Answer
   row[2] Feedback
   row[3] MedCod
   row[4] Correct
   */
   /***** Check if number of rows is correct *****/
   Tst_CheckIfNumberOfAnswersIsOne ();

   /***** Get the numerical value of the correct answer *****/
   row = mysql_fetch_row (mysql_res);
   if (sscanf (row[1],"%ld",&IntAnswerCorr) != 1)
      Lay_ShowErrorAndExit ("Wrong integer answer.");

   /***** Header with the title of each column *****/
   Tbl_StartTable (2);
   fprintf (Gbl.F.Out,"<tr>");
   Tst_WriteHeadUserCorrect (UsrDat);
   fprintf (Gbl.F.Out,"</tr>");

   /***** Write the user answer *****/
   fprintf (Gbl.F.Out,"<tr>"
	              "<td");
   if (Gbl.Test.StrAnswersOneQst[NumQst][0])		// If user has answered the question
     {
      if (sscanf (Gbl.Test.StrAnswersOneQst[NumQst],"%ld",&IntAnswerUsr) == 1)
         fprintf (Gbl.F.Out," class=\"%s CENTER_MIDDLE\">"
                            "%ld",
                  (Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_GOOD_BAD ||
                   Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK) ?
                   (IntAnswerUsr == IntAnswerCorr ? "ANS_OK" :
                	                            "ANS_BAD") :
                   "ANS_0",
                  IntAnswerUsr);
      else
        {
         Gbl.Test.StrAnswersOneQst[NumQst][0] = '\0';
         fprintf (Gbl.F.Out," class=\"ANS_0 CENTER_MIDDLE\">"
                            "?");
        }
     }
   else							// If user has omitted the answer
      fprintf (Gbl.F.Out,">");
   fprintf (Gbl.F.Out,"</td>");

   /***** Write the correct answer *****/
   fprintf (Gbl.F.Out,"<td class=\"ANS_0 CENTER_MIDDLE\">");
   if (Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_GOOD_BAD ||
       Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK)
      fprintf (Gbl.F.Out,"%ld",IntAnswerCorr);
   else
      fprintf (Gbl.F.Out,"?");
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");

   /***** Compute the score *****/
   if (!Gbl.Test.StrAnswersOneQst[NumQst][0])	// If user has omitted the answer
     {
      *AnswerIsNotBlank = false;
      *ScoreThisQst = 0.0;
     }
   else
     {
      *AnswerIsNotBlank = true;
      if (IntAnswerUsr == IntAnswerCorr)	// If correct
         *ScoreThisQst = 1.0;
      else					// If wrong
         *ScoreThisQst = 0.0;
     }

   /***** Write the score *****/
   if (Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_RESULT ||
       Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_GOOD_BAD ||
       Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK)
     {
      Tst_WriteScoreStart (2);
      if (!Gbl.Test.StrAnswersOneQst[NumQst][0])	// If user has omitted the answer
         fprintf (Gbl.F.Out,"ANS_0\">%.2lf",0.0);
      else if (IntAnswerUsr == IntAnswerCorr)	// If correct
         fprintf (Gbl.F.Out,"ANS_OK\">%.2lf",1.0);
      else					// If wrong
         fprintf (Gbl.F.Out,"ANS_BAD\">%.2lf",0.0);
      Tst_WriteScoreEnd ();
     }

   Tbl_EndTable ();
  }

/*****************************************************************************/
/****************** Write float answer when viewing a test *******************/
/*****************************************************************************/

static void Tst_WriteFloatAnsViewTest (unsigned NumQst)
  {
   /***** Write input field for the answer *****/
   fprintf (Gbl.F.Out,"<input type=\"text\" name=\"Ans%06u\""
	              " size=\"11\" maxlength=\"%u\" value=\"\" />",
            NumQst,Tst_MAX_BYTES_FLOAT_ANSWER);
  }

/*****************************************************************************/
/***************** Write float answer when assessing a test ******************/
/*****************************************************************************/

static void Tst_WriteFloatAnsAssessTest (struct UsrData *UsrDat,
				         unsigned NumQst,MYSQL_RES *mysql_res,
                                         double *ScoreThisQst,bool *AnswerIsNotBlank)
  {
   MYSQL_ROW row;
   unsigned i;
   double FloatAnsUsr = 0.0,Tmp;
   double FloatAnsCorr[2];
   /*
   row[0] AnsInd
   row[1] Answer
   row[2] Feedback
   row[3] MedCod
   row[4] Correct
   */
   /***** Check if number of rows is correct *****/
   if (Gbl.Test.Answer.NumOptions != 2)
      Lay_ShowErrorAndExit ("Wrong float range.");

   /***** Get the numerical value of the minimum and maximum correct answers *****/
   for (i = 0;
	i < 2;
	i++)
     {
      row = mysql_fetch_row (mysql_res);
      FloatAnsCorr[i] = Tst_GetFloatAnsFromStr (row[1]);
     }
   if (FloatAnsCorr[0] > FloatAnsCorr[1]) 	// The maximum and the minimum are swapped
    {
      /* Swap maximum and minimum */
      Tmp = FloatAnsCorr[0];
      FloatAnsCorr[0] = FloatAnsCorr[1];
      FloatAnsCorr[1] = Tmp;
     }

   /***** Header with the title of each column *****/
   Tbl_StartTable (2);
   fprintf (Gbl.F.Out,"<tr>");
   Tst_WriteHeadUserCorrect (UsrDat);
   fprintf (Gbl.F.Out,"</tr>");

   /***** Write the user answer *****/
   fprintf (Gbl.F.Out,"<tr>"
	              "<td");
   if (Gbl.Test.StrAnswersOneQst[NumQst][0])	// If user has answered the question
     {
      FloatAnsUsr = Tst_GetFloatAnsFromStr (Gbl.Test.StrAnswersOneQst[NumQst]);
      if (Gbl.Test.StrAnswersOneQst[NumQst][0])	// It's a correct floating point number
         fprintf (Gbl.F.Out," class=\"%s CENTER_MIDDLE\">%lg",
                  (Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_GOOD_BAD ||
                   Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK) ?
                   ((FloatAnsUsr >= FloatAnsCorr[0] &&
                     FloatAnsUsr <= FloatAnsCorr[1]) ? "ANS_OK" :
                	                               "ANS_BAD") :
                   "ANS_0",
                  FloatAnsUsr);
      else				// Not a floating point number
         fprintf (Gbl.F.Out," class=\"ANS_0 CENTER_MIDDLE\">?");
     }
   else					// If user has omitted the answer
      fprintf (Gbl.F.Out,">");
   fprintf (Gbl.F.Out,"</td>");

   /***** Write the correct answer *****/
   fprintf (Gbl.F.Out,"<td class=\"ANS_0 CENTER_MIDDLE\">");
   if (Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_GOOD_BAD ||
       Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK)
      fprintf (Gbl.F.Out,"[%lg; %lg]",FloatAnsCorr[0],FloatAnsCorr[1]);
   else
      fprintf (Gbl.F.Out,"?");
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");

   /***** Compute mark *****/
   if (!Gbl.Test.StrAnswersOneQst[NumQst][0])	// If user has omitted the answer
     {
      *AnswerIsNotBlank = false;
      *ScoreThisQst = 0.0;
     }
   else
     {
      *AnswerIsNotBlank = true;
      if (FloatAnsUsr >= FloatAnsCorr[0] &&
          FloatAnsUsr <= FloatAnsCorr[1])	// If correct (inside the interval)
         *ScoreThisQst = 1.0;
      else						// If wrong (outside the interval)
         *ScoreThisQst = 0.0;
     }

   /***** Write mark *****/
   if (Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_RESULT ||
       Gbl.Test.Config.Feedback == Tst_FEEDBACK_EACH_GOOD_BAD ||
       Gbl.Test.Config.Feedback == Tst_FEEDBACK_FULL_FEEDBACK)
     {
      Tst_WriteScoreStart (2);
      if (!Gbl.Test.StrAnswersOneQst[NumQst][0])	// If user has omitted the answer
         fprintf (Gbl.F.Out,"ANS_0\">%.2lf",0.0);
      else if (FloatAnsUsr >= FloatAnsCorr[0] &&
               FloatAnsUsr <= FloatAnsCorr[1])	// If correct (inside the interval)
         fprintf (Gbl.F.Out,"ANS_OK\">%.2lf",1.0);
      else					// If wrong (outside the interval)
         fprintf (Gbl.F.Out,"ANS_BAD\">%.2lf",0.0);
      Tst_WriteScoreEnd ();
     }

   Tbl_EndTable ();
  }

/*****************************************************************************/
/********* Write head with two columns:                               ********/
/********* one for the user's answer and other for the correct answer ********/
/*****************************************************************************/

static void Tst_WriteHeadUserCorrect (struct UsrData *UsrDat)
  {
   extern const char *Txt_User[Usr_NUM_SEXS];
   extern const char *Txt_ROLES_PLURAL_Abc[Rol_NUM_ROLES][Usr_NUM_SEXS];

   fprintf (Gbl.F.Out,"<td class=\"DAT_SMALL CENTER_MIDDLE\">"
	              "%s"
	              "</td>"
                      "<td class=\"DAT_SMALL CENTER_MIDDLE\">"
                      "%s"
                      "</td>",
            Txt_User[UsrDat->Sex],
	    Txt_ROLES_PLURAL_Abc[Rol_TCH][Usr_SEX_UNKNOWN]);
  }

/*****************************************************************************/
/*********** Write the start ans the end of the score of an answer ***********/
/*****************************************************************************/

static void Tst_WriteScoreStart (unsigned ColSpan)
  {
   extern const char *Txt_Score;

   fprintf (Gbl.F.Out,"<tr>"
	              "<td colspan=\"%u\" class=\"DAT_SMALL LEFT_MIDDLE\">"
	              "%s: <span class=\"",
            ColSpan,Txt_Score);
  }

static void Tst_WriteScoreEnd (void)
  {
   fprintf (Gbl.F.Out,"</span>"
	              "</td>"
	              "</tr>");
  }

/*****************************************************************************/
/*************** Write parameter with the code of a question *****************/
/*****************************************************************************/

static void Tst_WriteParamQstCod (unsigned NumQst,long QstCod)
  {
   char ParamName[3 + 6 + 1];

   snprintf (ParamName,sizeof (ParamName),
	     "Qst%06u",
	     NumQst);
   Par_PutHiddenParamLong (ParamName,QstCod);
  }

/*****************************************************************************/
/********************* Check if number of answers is one *********************/
/*****************************************************************************/

void Tst_CheckIfNumberOfAnswersIsOne (void)
  {
   if (Gbl.Test.Answer.NumOptions != 1)
      Lay_ShowErrorAndExit ("Wrong answer.");
  }

/*****************************************************************************/
/************************* Get tags of a test question ***********************/
/*****************************************************************************/

unsigned long Tst_GetTagsQst (long QstCod,MYSQL_RES **mysql_res)
  {
   /***** Get the tags of a question from database *****/
   return DB_QuerySELECT (mysql_res,"can not get the tags of a question",
			  "SELECT tst_tags.TagTxt FROM tst_question_tags,tst_tags"
			  " WHERE tst_question_tags.QstCod=%ld"
			  " AND tst_question_tags.TagCod=tst_tags.TagCod"
			  " AND tst_tags.CrsCod=%ld"
			  " ORDER BY tst_question_tags.TagInd",
			  QstCod,Gbl.Hierarchy.Crs.CrsCod);
  }

/*****************************************************************************/
/******************** Get and write tags of a test question ******************/
/*****************************************************************************/

void Tst_GetAndWriteTagsQst (long QstCod)
  {
   extern const char *Txt_no_tags;
   unsigned long NumRow,NumRows;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;

   if ((NumRows = Tst_GetTagsQst (QstCod,&mysql_res)))	// Result: TagTxt
     {
      /***** Write the tags *****/
      fprintf (Gbl.F.Out,"<ul class=\"TEST_TAG_LIST DAT_SMALL\">");
      for (NumRow = 0;
	   NumRow < NumRows;
	   NumRow++)
        {
         row = mysql_fetch_row (mysql_res);
         fprintf (Gbl.F.Out,"<li>%s</li>",row[0]);
        }
      fprintf (Gbl.F.Out,"</ul>");
     }
   else
      fprintf (Gbl.F.Out,"<span class=\"DAT_SMALL\">(%s)</span>",
               Txt_no_tags);

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/************ Get parameters for the selection of test questions *************/
/*****************************************************************************/
// Return true (OK) if all parameters are found, or false (error) if any necessary parameter is not found

static bool Tst_GetParamsTst (Tst_ActionToDoWithQuestions_t ActionToDoWithQuestions)
  {
   extern const char *Txt_You_must_select_one_ore_more_tags;
   extern const char *Txt_You_must_select_one_ore_more_types_of_answer;
   extern const char *Txt_The_number_of_questions_must_be_in_the_interval_X;
   bool Error = false;
   char UnsignedStr[10 + 1];
   unsigned UnsignedNum;

   /***** Tags *****/
   /* Get parameter that indicates whether all tags are selected */
   Gbl.Test.Tags.All = Par_GetParToBool ("AllTags");

   /* Get the tags */
   if ((Gbl.Test.Tags.List = (char *) malloc (Tst_MAX_BYTES_TAGS_LIST + 1)) == NULL)
      Lay_NotEnoughMemoryExit ();
   Par_GetParMultiToText ("ChkTag",Gbl.Test.Tags.List,Tst_MAX_BYTES_TAGS_LIST);

   /* Check number of tags selected */
   if (Tst_CountNumTagsInList () == 0)	// If no tags selected...
     {					// ...write alert
      Ale_ShowAlert (Ale_WARNING,Txt_You_must_select_one_ore_more_tags);
      Error = true;
     }

   /***** Types of answer *****/
   switch (ActionToDoWithQuestions)
     {
      case Tst_SHOW_TEST_TO_ANSWER:
      case Tst_EDIT_TEST:
	 /* Get parameter that indicates if all types of answer are selected */
	 Gbl.Test.AllAnsTypes = Par_GetParToBool ("AllAnsTypes");

	 /* Get types of answer */
	 Par_GetParMultiToText ("AnswerType",Gbl.Test.ListAnsTypes,Tst_MAX_BYTES_LIST_ANSWER_TYPES);

	 /* Check number of types of answer */
	 if (Tst_CountNumAnswerTypesInList () == 0)	// If no types of answer selected...
	   {						// ...write warning alert
	    Ale_ShowAlert (Ale_WARNING,Txt_You_must_select_one_ore_more_types_of_answer);
	    Error = true;
	   }
	 break;
      case Tst_SELECT_QUESTIONS_FOR_GAME:
	 /* The unique allowed type of answer in a game is unique choice */
	 Gbl.Test.AllAnsTypes = false;
	 snprintf (Gbl.Test.ListAnsTypes,sizeof (Gbl.Test.ListAnsTypes),
	           "%u",
		   (unsigned) Tst_ANS_UNIQUE_CHOICE);
	 break;
      default:
	 break;
     }

   /***** Get other parameters, depending on action *****/
   switch (ActionToDoWithQuestions)
     {
      case Tst_SHOW_TEST_TO_ANSWER:
	 Tst_GetParamNumQst ();
	 if (Gbl.Test.NumQsts < Gbl.Test.Config.Min ||
	     Gbl.Test.NumQsts > Gbl.Test.Config.Max)
	   {
	    Ale_ShowAlert (Ale_WARNING,Txt_The_number_of_questions_must_be_in_the_interval_X,
		           Gbl.Test.Config.Min,Gbl.Test.Config.Max);
	    Error = true;
	   }
	 break;
      case Tst_EDIT_TEST:
	 /* Get starting and ending dates */
	 Dat_GetIniEndDatesFromForm ();

	 /* Get ordering criteria */
	 Par_GetParMultiToText ("Order",UnsignedStr,10);
	 if (sscanf (UnsignedStr,"%u",&UnsignedNum) == 1)
	    Gbl.Test.SelectedOrder = (Tst_QuestionsOrder_t) ((UnsignedNum < Tst_NUM_TYPES_ORDER_QST) ? UnsignedNum :
												       0);
	 else
	    Gbl.Test.SelectedOrder = (Tst_QuestionsOrder_t) 0;

	 /* Get whether we must create the XML file or not */
	 Gbl.Test.XML.CreateXML = Tst_GetCreateXMLFromForm ();
	 break;
      case Tst_SELECT_QUESTIONS_FOR_GAME:
	 /* Get starting and ending dates */
	 Dat_GetIniEndDatesFromForm ();

	 /* Order question by stem */
	 Gbl.Test.SelectedOrder = Tst_ORDER_STEM;
	 break;
      default:
	 break;
     }

   return !Error;
  }

/*****************************************************************************/
/******************** Get parameter with the number of test ******************/
/*****************************************************************************/

static unsigned Tst_GetAndCheckParamNumTst (void)
  {
   return (unsigned) Par_GetParToUnsignedLong ("NumTst",
                                               1,
                                               UINT_MAX,
                                               1);
  }

/*****************************************************************************/
/***** Get parameter with the number of questions to generate in an test *****/
/*****************************************************************************/

static void Tst_GetParamNumQst (void)
  {
   Gbl.Test.NumQsts = (unsigned)
	              Par_GetParToUnsignedLong ("NumQst",
	                                        (unsigned long) Gbl.Test.Config.Min,
	                                        (unsigned long) Gbl.Test.Config.Max,
	                                        (unsigned long) Gbl.Test.Config.Def);
  }

/*****************************************************************************/
/****************** Get whether to create XML file from form *****************/
/*****************************************************************************/

static bool Tst_GetCreateXMLFromForm (void)
  {
   return Par_GetParToBool ("CreateXML");
  }

/*****************************************************************************/
/***************** Count number of tags in the list of tags ******************/
/*****************************************************************************/

static int Tst_CountNumTagsInList (void)
  {
   const char *Ptr;
   int NumTags = 0;
   char TagText[Tst_MAX_BYTES_TAG + 1];

   /***** Go over the list Gbl.Test.Tags.List counting the number of tags *****/
   if (Gbl.Test.Tags.List)
     {
      Ptr = Gbl.Test.Tags.List;
      while (*Ptr)
        {
         Par_GetNextStrUntilSeparParamMult (&Ptr,TagText,Tst_MAX_BYTES_TAG);
         NumTags++;
        }
     }
   return NumTags;
  }

/*****************************************************************************/
/**** Count the number of types of answers in the list of types of answers ***/
/*****************************************************************************/

static int Tst_CountNumAnswerTypesInList (void)
  {
   const char *Ptr;
   int NumAnsTypes = 0;
   char UnsignedStr[10 + 1];

   /***** Go over the list Gbl.Test.ListAnsTypes counting the number of types of answer *****/
   Ptr = Gbl.Test.ListAnsTypes;
   while (*Ptr)
     {
      Par_GetNextStrUntilSeparParamMult (&Ptr,UnsignedStr,10);
      Tst_ConvertFromUnsignedStrToAnsTyp (UnsignedStr);
      NumAnsTypes++;
     }
   return NumAnsTypes;
  }

/*****************************************************************************/
/**************** Free memory allocated for the list of tags *****************/
/*****************************************************************************/

void Tst_FreeTagsList (void)
  {
   if (Gbl.Test.Tags.List)
     {
      free ((void *) Gbl.Test.Tags.List);
      Gbl.Test.Tags.List = NULL;
      Gbl.Test.Tags.Num = 0;
     }
  }

/*****************************************************************************/
/******************** Show form to edit one test question ********************/
/*****************************************************************************/

void Tst_ShowFormEditOneQst (void)
  {
   char Stem[Cns_MAX_BYTES_TEXT + 1];
   char Feedback[Cns_MAX_BYTES_TEXT + 1];

   /***** Create test question *****/
   Tst_QstConstructor ();
   Gbl.Test.QstCod = Tst_GetQstCod ();
   Stem[0] = Feedback[0] = '\0';
   if (Gbl.Test.QstCod > 0)	// If question already exists in the database
      Tst_GetQstDataFromDB (Stem,Feedback);

   /***** Put form to edit question *****/
   Tst_PutFormEditOneQst (Stem,Feedback);

   /***** Destroy test question *****/
   Tst_QstDestructor ();
  }

/*****************************************************************************/
/******************** Show form to edit one test question ********************/
/*****************************************************************************/

// This function may be called from three places:
// 1. By clicking "New question" icon
// 2. By clicking "Edit" icon in a listing of existing questions
// 3. From the action associated to reception of a question, on error in the parameters received from the form

static void Tst_PutFormEditOneQst (char Stem[Cns_MAX_BYTES_TEXT + 1],
                                   char Feedback[Cns_MAX_BYTES_TEXT + 1])
  {
   extern const char *Hlp_ASSESSMENT_Tests_writing_a_question;
   extern const char *The_ClassFormInBox[The_NUM_THEMES];
   extern const char *Txt_Question_code_X;
   extern const char *Txt_New_question;
   extern const char *Txt_Tags;
   extern const char *Txt_new_tag;
   extern const char *Txt_Stem;
   extern const char *Txt_Feedback;
   extern const char *Txt_optional;
   extern const char *Txt_Type;
   extern const char *Txt_TST_STR_ANSWER_TYPES[Tst_NUM_ANS_TYPES];
   extern const char *Txt_Answers;
   extern const char *Txt_Integer_number;
   extern const char *Txt_Real_number_between_A_and_B_1;
   extern const char *Txt_Real_number_between_A_and_B_2;
   extern const char *Txt_TF_QST[2];
   extern const char *Txt_Shuffle;
   extern const char *Txt_Expand;
   extern const char *Txt_Contract;
   extern const char *Txt_Save_changes;
   extern const char *Txt_Create_question;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows;
   unsigned long NumRow;
   unsigned NumOpt;
   Tst_AnswerType_t AnsType;
   unsigned NumTag;
   bool TagNotFound;
   bool OptionsDisabled;
   bool AnswerHasContent;
   bool DisplayRightColumn;

   /***** Start box *****/
   if (Gbl.Test.QstCod > 0)	// The question already has assigned a code
     {
      snprintf (Gbl.Title,sizeof (Gbl.Title),
	        Txt_Question_code_X,
		Gbl.Test.QstCod);
      Box_StartBox (NULL,Gbl.Title,Tst_PutIconToRemoveOneQst,
                    Hlp_ASSESSMENT_Tests_writing_a_question,Box_NOT_CLOSABLE);
     }
   else
      Box_StartBox (NULL,Txt_New_question,NULL,
                    Hlp_ASSESSMENT_Tests_writing_a_question,Box_NOT_CLOSABLE);

   /***** Start form *****/
   Frm_StartForm (ActRcvTstQst);
   if (Gbl.Test.QstCod > 0)	// The question already has assigned a code
      Tst_PutParamQstCod ();

   /***** Start table *****/
   Tbl_StartTable (2);	// Table for this question

   /***** Help for text editor *****/
   fprintf (Gbl.F.Out,"<tr>"
                      "<td colspan=\"2\">");
   Lay_HelpPlainEditor ();
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");

   /***** Get tags already existing for questions in current course *****/
   NumRows = Tst_GetAllTagsFromCurrentCrs (&mysql_res);

   /***** Write the tags *****/
   fprintf (Gbl.F.Out,"<tr>"
                      "<td class=\"RIGHT_TOP %s\">"
                      "%s:"
                      "</td>"
                      "<td class=\"LEFT_TOP\">",
            The_ClassFormInBox[Gbl.Prefs.Theme],Txt_Tags);
   Tbl_StartTable (2);	// Table for tags

   for (NumTag = 0;
	NumTag < Tst_MAX_TAGS_PER_QUESTION;
	NumTag++)
     {
      fprintf (Gbl.F.Out,"<tr>");

      /***** Write the tags already existing in a selector *****/
      fprintf (Gbl.F.Out,"<td class=\"LEFT_MIDDLE\">"
	                 "<select id=\"SelDesc%u\" name=\"SelDesc%u\""
	                 " class=\"TAG_SEL\" onchange=\"changeTxtTag('%u')\">",
               NumTag,NumTag,NumTag);
      fprintf (Gbl.F.Out,"<option value=\"\">&nbsp;</option>");
      mysql_data_seek (mysql_res,0);
      TagNotFound = true;
      for (NumRow = 1;
	   NumRow <= NumRows;
	   NumRow++)
        {
         row = mysql_fetch_row (mysql_res);
	 /*
	 row[0] TagCod
	 row[1] TagTxt
	 row[2] TagHidden
	 */
         fprintf (Gbl.F.Out,"<option value=\"%s\"",row[1]);
         if (!strcasecmp (Gbl.Test.Tags.Txt[NumTag],row[1]))
           {
  	    fprintf (Gbl.F.Out," selected=\"selected\"");
            TagNotFound = false;
           }
         fprintf (Gbl.F.Out,">%s</option>",row[1]);
        }
      /* If it's a new tag received from the form */
      if (TagNotFound && Gbl.Test.Tags.Txt[NumTag][0])
         fprintf (Gbl.F.Out,"<option value=\"%s\" selected=\"selected\">%s</option>",
                  Gbl.Test.Tags.Txt[NumTag],Gbl.Test.Tags.Txt[NumTag]);
      fprintf (Gbl.F.Out,"<option value=\"\">[%s]</option>"
	                 "</select>"
	                 "</td>",
               Txt_new_tag);

      /***** Input of a new tag *****/
      fprintf (Gbl.F.Out,"<td class=\"RIGHT_MIDDLE\">"
                         "<input type=\"text\" id=\"TagTxt%u\" name=\"TagTxt%u\""
                         " class=\"TAG_TXT\" maxlength=\"%u\" value=\"%s\""
                         " onchange=\"changeSelTag('%u')\" />"
                         "</td>",
	       NumTag,NumTag,Tst_MAX_CHARS_TAG,Gbl.Test.Tags.Txt[NumTag],NumTag);

      fprintf (Gbl.F.Out,"</tr>");
     }

   Tbl_EndTable ();	// Table for tags
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");

   /* Free structure that stores the query result */
   DB_FreeMySQLResult (&mysql_res);

   /***** Stem and image *****/
   fprintf (Gbl.F.Out,"<tr>"
	              "<td class=\"RIGHT_TOP\">"
	              "<label for=\"Stem\" class=\"%s\">%s:</label>"
	              "</td>"
                      "<td class=\"LEFT_TOP\">"
                      "<textarea id=\"Stem\" name=\"Stem\""
                      " class=\"STEM_TEXTAREA\" rows=\"5\" required=\"required\">"
                      "%s"
                      "</textarea><br />",
            The_ClassFormInBox[Gbl.Prefs.Theme],
            Txt_Stem,
            Stem);
   Tst_PutFormToEditQstMedia (&Gbl.Test.Media,-1,
                              false);

   /***** Feedback *****/
   fprintf (Gbl.F.Out,"<label class=\"%s\">"
	              "%s (%s):<br />"
                      "<textarea name=\"Feedback\""
                      " class=\"STEM_TEXTAREA\" rows=\"2\">",
            The_ClassFormInBox[Gbl.Prefs.Theme],
            Txt_Feedback,Txt_optional);
   if (Feedback)
      if (Feedback[0])
	 fprintf (Gbl.F.Out,"%s",Feedback);
   fprintf (Gbl.F.Out,"</textarea>"
                      "</label>"
                      "</td>"
                      "</tr>");

   /***** Type of answer *****/
   fprintf (Gbl.F.Out,"<tr>"
	              "<td class=\"RIGHT_TOP %s\">"
	              "%s:"
	              "</td>"
                      "<td class=\"%s LEFT_TOP\">",
            The_ClassFormInBox[Gbl.Prefs.Theme],
            Txt_Type,
            The_ClassFormInBox[Gbl.Prefs.Theme]);
   for (AnsType = (Tst_AnswerType_t) 0;
	AnsType < Tst_NUM_ANS_TYPES;
	AnsType++)
     {
      fprintf (Gbl.F.Out,"<label>"
	                 "<input type=\"radio\" name=\"AnswerType\" value=\"%u\"",
               (unsigned) AnsType);
      if (AnsType == Gbl.Test.AnswerType)
         fprintf (Gbl.F.Out," checked=\"checked\"");
      fprintf (Gbl.F.Out," onclick=\"enableDisableAns(this.form);\" />"
	                 "%s&nbsp;"
	                 "</label><br />",
               Txt_TST_STR_ANSWER_TYPES[AnsType]);
     }
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");

   /***** Answers *****/
   /* Integer answer */
   fprintf (Gbl.F.Out,"<tr>"
                      "<td class=\"RIGHT_TOP %s\">"
                      "%s:"
                      "</td>"
                      "<td class=\"LEFT_TOP\">"
	              "<label class=\"%s\">"
                      "%s:&nbsp;"
                      "<input type=\"text\" name=\"AnsInt\""
                      " size=\"11\" maxlength=\"11\" value=\"%ld\"",
            The_ClassFormInBox[Gbl.Prefs.Theme],Txt_Answers,
            The_ClassFormInBox[Gbl.Prefs.Theme],Txt_Integer_number,
            Gbl.Test.Answer.Integer);
   if (Gbl.Test.AnswerType != Tst_ANS_INT)
      fprintf (Gbl.F.Out," disabled=\"disabled\"");
   fprintf (Gbl.F.Out," required=\"required\" />"
                      "</label>"
	              "</td>"
	              "</tr>");

   /* Floating point answer */
   fprintf (Gbl.F.Out,"<tr>"
	              "<td></td>"
                      "<td class=\"LEFT_TOP\">");
   Tst_PutFloatInputField (Txt_Real_number_between_A_and_B_1,"AnsFloatMin",
                           Gbl.Test.Answer.FloatingPoint[0]);
   Tst_PutFloatInputField (Txt_Real_number_between_A_and_B_2,"AnsFloatMax",
                           Gbl.Test.Answer.FloatingPoint[1]);
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");

   /* T/F answer */
   fprintf (Gbl.F.Out,"<tr>"
	              "<td></td>"
                      "<td class=\"LEFT_TOP\">");
   Tst_PutTFInputField (Txt_TF_QST[0],'T');
   Tst_PutTFInputField (Txt_TF_QST[1],'F');
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");

   /* Questions can be shuffled? */
   fprintf (Gbl.F.Out,"<tr>"
	              "<td></td>"
                      "<td class=\"LEFT_TOP\">"
                      "<label class=\"%s\">"
                      "<input type=\"checkbox\" name=\"Shuffle\" value=\"Y\"",
            The_ClassFormInBox[Gbl.Prefs.Theme]);
   if (Gbl.Test.Shuffle)
      fprintf (Gbl.F.Out," checked=\"checked\"");
   if (Gbl.Test.AnswerType != Tst_ANS_UNIQUE_CHOICE &&
       Gbl.Test.AnswerType != Tst_ANS_MULTIPLE_CHOICE)
      fprintf (Gbl.F.Out," disabled=\"disabled\"");
   fprintf (Gbl.F.Out," />"
	              "%s"
                      "</label>"
	              "</td>"
	              "</tr>",
            Txt_Shuffle);

   /* Simple or multiple choice answers */
   fprintf (Gbl.F.Out,"<tr>"
	              "<td></td>"
	              "<td class=\"LEFT_TOP\">");
   Tbl_StartTable (2);	// Table with choice answers

   OptionsDisabled = Gbl.Test.AnswerType != Tst_ANS_UNIQUE_CHOICE &&
                     Gbl.Test.AnswerType != Tst_ANS_MULTIPLE_CHOICE &&
	             Gbl.Test.AnswerType != Tst_ANS_TEXT;
   for (NumOpt = 0;
	NumOpt < Tst_MAX_OPTIONS_PER_QUESTION;
	NumOpt++)
     {
      Gbl.RowEvenOdd = NumOpt % 2;

      AnswerHasContent = false;
      if (Gbl.Test.Answer.Options[NumOpt].Text)
	 if (Gbl.Test.Answer.Options[NumOpt].Text[0] ||				// Text
	     Gbl.Test.Answer.Options[NumOpt].Media.Type != Med_TYPE_NONE)	// or media
	    AnswerHasContent = true;
      DisplayRightColumn = NumOpt < 2 ||	// Display at least the two first options
	                   AnswerHasContent;

      /***** Left column: selectors *****/
      fprintf (Gbl.F.Out,"<tr>"
	                 "<td class=\"TEST_EDI_ANS_LEFT_COL COLOR%u\">",
	       Gbl.RowEvenOdd);

      /* Radio selector for unique choice answers */
      fprintf (Gbl.F.Out,"<input type=\"radio\" name=\"AnsUni\" value=\"%u\"",
	       NumOpt);
      if (Gbl.Test.AnswerType != Tst_ANS_UNIQUE_CHOICE)
         fprintf (Gbl.F.Out," disabled=\"disabled\"");
      if (Gbl.Test.Answer.Options[NumOpt].Correct)
         fprintf (Gbl.F.Out," checked=\"checked\"");
      if (NumOpt < 2)	// First or second options required
         fprintf (Gbl.F.Out," required=\"required\"");
      fprintf (Gbl.F.Out," />");

      /* Checkbox for multiple choice answers */
      fprintf (Gbl.F.Out,"<input type=\"checkbox\" name=\"AnsMulti\" value=\"%u\"",
	       NumOpt);
      if (Gbl.Test.AnswerType != Tst_ANS_MULTIPLE_CHOICE)
         fprintf (Gbl.F.Out," disabled=\"disabled\"");
      if (Gbl.Test.Answer.Options[NumOpt].Correct)
         fprintf (Gbl.F.Out," checked=\"checked\"");
      fprintf (Gbl.F.Out," />");

      fprintf (Gbl.F.Out,"</td>");

      /***** Center column: letter of the answer and expand / contract icon *****/
      fprintf (Gbl.F.Out,"<td class=\"%s TEST_EDI_ANS_CENTER_COL COLOR%u\">"
	                 "%c)",
	       The_ClassFormInBox[Gbl.Prefs.Theme],Gbl.RowEvenOdd,
               'a' + (char) NumOpt);

      /* Icon to expand (show the answer) */
      snprintf (Gbl.Title,sizeof (Gbl.Title),
	        "%s %c)",
		Txt_Expand,'a' + (char) NumOpt);
      fprintf (Gbl.F.Out,"<a href=\"\" id=\"exp_%u\"",NumOpt);
      if (DisplayRightColumn)	// Answer does not have content
	 fprintf (Gbl.F.Out," style=\"display:none;\"");	// Hide icon
      fprintf (Gbl.F.Out," onclick=\"toggleAnswer('%u'); return false;\" />"
                         "<img src=\"%s/caret-right.svg\""
			 " alt=\"%s\" title=\"%s\" class=\"ICO16x16\" />"
	                 "</a>",
               NumOpt,Cfg_URL_ICON_PUBLIC,
	       Gbl.Title,Gbl.Title);

      /* Icon to contract (hide the answer) */
      snprintf (Gbl.Title,sizeof (Gbl.Title),
	        "%s %c)",
		Txt_Contract,'a' + (char) NumOpt);
      fprintf (Gbl.F.Out,"<a href=\"\" id=\"con_%u\"",NumOpt);
      if (!DisplayRightColumn)	// Answer does not have content
	 fprintf (Gbl.F.Out," style=\"display:none;\"");	// Hide icon
      fprintf (Gbl.F.Out," onclick=\"toggleAnswer(%u); return false;\" />"
                         "<img src=\"%s/caret-down.svg\""
			 " alt=\"%s\" title=\"%s\" class=\"ICO16x16\" />"
	                 "</a>",
               NumOpt,Cfg_URL_ICON_PUBLIC,
	       Gbl.Title,Gbl.Title);

      fprintf (Gbl.F.Out,"</td>");

      /***** Right column: content of the answer *****/
      fprintf (Gbl.F.Out,"<td class=\"TEST_EDI_ANS_RIGHT_COL COLOR%u\">"
	                 "<div id=\"ans_%u\"",
	       Gbl.RowEvenOdd,
	       NumOpt);
      if (!DisplayRightColumn)	// Answer does not have content
	 fprintf (Gbl.F.Out," style=\"display:none;\"");	// Hide column
      fprintf (Gbl.F.Out,">");

      /* Answer text */
      fprintf (Gbl.F.Out,"<textarea name=\"AnsStr%u\""
	                 " class=\"ANSWER_TEXTAREA\" rows=\"5\"",
	       NumOpt);
      if (OptionsDisabled)
         fprintf (Gbl.F.Out," disabled=\"disabled\"");
      fprintf (Gbl.F.Out,">");
      if (AnswerHasContent)
         fprintf (Gbl.F.Out,"%s",Gbl.Test.Answer.Options[NumOpt].Text);
      fprintf (Gbl.F.Out,"</textarea>");

      /* Media */
      Tst_PutFormToEditQstMedia (&Gbl.Test.Answer.Options[NumOpt].Media,
                                 (int) NumOpt,
                                 OptionsDisabled);

      /* Feedback */
      fprintf (Gbl.F.Out,"<label class=\"%s\">%s (%s):<br />"
	                 "<textarea name=\"FbStr%u\""
	                 " class=\"ANSWER_TEXTAREA\" rows=\"2\"",
	       The_ClassFormInBox[Gbl.Prefs.Theme],Txt_Feedback,Txt_optional,
	       NumOpt);
      if (OptionsDisabled)
         fprintf (Gbl.F.Out," disabled=\"disabled\"");
      fprintf (Gbl.F.Out,">");
      if (Gbl.Test.Answer.Options[NumOpt].Feedback)
         if (Gbl.Test.Answer.Options[NumOpt].Feedback[0])
            fprintf (Gbl.F.Out,"%s",Gbl.Test.Answer.Options[NumOpt].Feedback);
      fprintf (Gbl.F.Out,"</textarea>"
	                 "</label>");

      /* End of right column */
      fprintf (Gbl.F.Out,"</div>"
                         "</td>"
	                 "</tr>");
     }
   Tbl_EndTable ();	// Table with choice answers
   fprintf (Gbl.F.Out,"</td>"
	              "</tr>");

   /***** End table *****/
   Tbl_EndTable ();	// Table for this question

   /***** Send button *****/
   if (Gbl.Test.QstCod > 0)	// The question already has assigned a code
      Btn_PutConfirmButton (Txt_Save_changes);
   else
      Btn_PutCreateButton (Txt_Create_question);

   /***** End form *****/
   Frm_EndForm ();

   /***** End box *****/
   Box_EndBox ();
  }

/*****************************************************************************/
/********************* Put input field for floating answer *******************/
/*****************************************************************************/

static void Tst_PutFloatInputField (const char *Label,const char *Field,
                                    double Value)
  {
   extern const char *The_ClassFormInBox[The_NUM_THEMES];

   fprintf (Gbl.F.Out,"<label class=\"%s\">%s&nbsp;"
                      "<input type=\"text\" name=\"%s\""
                      " size=\"11\" maxlength=\"%u\""
                      " value=\"%lg\"",
            The_ClassFormInBox[Gbl.Prefs.Theme],Label,
            Field,
            Tst_MAX_BYTES_FLOAT_ANSWER,
            Value);
   if (Gbl.Test.AnswerType != Tst_ANS_FLOAT)
      fprintf (Gbl.F.Out," disabled=\"disabled\"");
   fprintf (Gbl.F.Out," required=\"required\" />"
	              "</label>");
  }

/*****************************************************************************/
/*********************** Put input field for T/F answer **********************/
/*****************************************************************************/

static void Tst_PutTFInputField (const char *Label,char Value)
  {
   extern const char *The_ClassFormInBox[The_NUM_THEMES];

   fprintf (Gbl.F.Out,"<label class=\"%s\">"
                      "<input type=\"radio\" name=\"AnsTF\" value=\"%c\"",
            The_ClassFormInBox[Gbl.Prefs.Theme],Value);
   if (Gbl.Test.Answer.TF == Value)
      fprintf (Gbl.F.Out," checked=\"checked\"");
   if (Gbl.Test.AnswerType != Tst_ANS_TRUE_FALSE)
      fprintf (Gbl.F.Out," disabled=\"disabled\"");
   fprintf (Gbl.F.Out," required=\"required\" />"
	              "%s"
	              "</label>",
	    Label);
  }

/*****************************************************************************/
/********************* Initialize a new question to zero *********************/
/*****************************************************************************/

void Tst_QstConstructor (void)
  {
   unsigned NumOpt;

   Gbl.Test.QstCod = -1L;
   Gbl.Test.Stem.Text = NULL;
   Gbl.Test.Stem.Length = 0;
   Gbl.Test.Feedback.Text = NULL;
   Gbl.Test.Feedback.Length = 0;
   Gbl.Test.Shuffle = false;
   Gbl.Test.AnswerType = Tst_ANS_UNIQUE_CHOICE;
   Gbl.Test.Answer.NumOptions = 0;
   Gbl.Test.Answer.TF = ' ';

   /***** Initialize image attached to stem *****/
   Med_MediaConstructor (&Gbl.Test.Media);

   for (NumOpt = 0;
	NumOpt < Tst_MAX_OPTIONS_PER_QUESTION;
	NumOpt++)
     {
      Gbl.Test.Answer.Options[NumOpt].Correct  = false;
      Gbl.Test.Answer.Options[NumOpt].Text     = NULL;
      Gbl.Test.Answer.Options[NumOpt].Feedback = NULL;

      /***** Initialize image attached to option *****/
      Med_MediaConstructor (&Gbl.Test.Answer.Options[NumOpt].Media);
     }
   Gbl.Test.Answer.Integer = 0;
   Gbl.Test.Answer.FloatingPoint[0] =
   Gbl.Test.Answer.FloatingPoint[1] = 0.0;
  }

/*****************************************************************************/
/***************** Free memory allocated for test question *******************/
/*****************************************************************************/

void Tst_QstDestructor (void)
  {
   Tst_FreeTextChoiceAnswers ();
   Tst_FreeMediaOfQuestion ();
  }

/*****************************************************************************/
/******************* Allocate memory for a choice answer *********************/
/*****************************************************************************/

int Tst_AllocateTextChoiceAnswer (unsigned NumOpt)
  {
   Tst_FreeTextChoiceAnswer (NumOpt);

   if ((Gbl.Test.Answer.Options[NumOpt].Text =
	(char *) malloc (Tst_MAX_BYTES_ANSWER_OR_FEEDBACK + 1)) == NULL)
     {
      Ale_CreateAlert (Ale_ERROR,NULL,
		       "Not enough memory to store answer.");
      return 0;
     }
   if ((Gbl.Test.Answer.Options[NumOpt].Feedback =
	(char *) malloc (Tst_MAX_BYTES_ANSWER_OR_FEEDBACK + 1)) == NULL)
     {
      Ale_CreateAlert (Ale_ERROR,NULL,
		       "Not enough memory to store feedback.");
      return 0;
     }

   Gbl.Test.Answer.Options[NumOpt].Text[0] =
   Gbl.Test.Answer.Options[NumOpt].Feedback[0] = '\0';
   return 1;
  }

/*****************************************************************************/
/******************** Free memory of all choice answers **********************/
/*****************************************************************************/

static void Tst_FreeTextChoiceAnswers (void)
  {
   unsigned NumOpt;

   for (NumOpt = 0;
	NumOpt < Tst_MAX_OPTIONS_PER_QUESTION;
	NumOpt++)
      Tst_FreeTextChoiceAnswer (NumOpt);
  }

/*****************************************************************************/
/********************** Free memory of a choice answer ***********************/
/*****************************************************************************/

static void Tst_FreeTextChoiceAnswer (unsigned NumOpt)
  {
   if (Gbl.Test.Answer.Options[NumOpt].Text)
     {
      free ((void *) Gbl.Test.Answer.Options[NumOpt].Text);
      Gbl.Test.Answer.Options[NumOpt].Text = NULL;
     }
   if (Gbl.Test.Answer.Options[NumOpt].Feedback)
     {
      free ((void *) Gbl.Test.Answer.Options[NumOpt].Feedback);
      Gbl.Test.Answer.Options[NumOpt].Feedback = NULL;
     }
  }

/*****************************************************************************/
/***************** Initialize images of a question to zero *******************/
/*****************************************************************************/

static void Tst_ResetMediaOfQuestion (void)
  {
   unsigned NumOpt;

   /***** Reset media for stem *****/
   Med_ResetMedia (&Gbl.Test.Media);

   /***** Reset media for every answer option *****/
   for (NumOpt = 0;
	NumOpt < Tst_MAX_OPTIONS_PER_QUESTION;
	NumOpt++)
      Med_ResetMedia (&Gbl.Test.Answer.Options[NumOpt].Media);
  }

/*****************************************************************************/
/*********************** Free images of a question ***************************/
/*****************************************************************************/

static void Tst_FreeMediaOfQuestion (void)
  {
   unsigned NumOpt;

   Med_MediaDestructor (&Gbl.Test.Media);
   for (NumOpt = 0;
	NumOpt < Tst_MAX_OPTIONS_PER_QUESTION;
	NumOpt++)
      Med_MediaDestructor (&Gbl.Test.Answer.Options[NumOpt].Media);
  }

/*****************************************************************************/
/****************** Get data of a question from database *********************/
/*****************************************************************************/

static void Tst_GetQstDataFromDB (char Stem[Cns_MAX_BYTES_TEXT + 1],
                                  char Feedback[Cns_MAX_BYTES_TEXT + 1])
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows;
   unsigned long NumRow;
   unsigned NumOpt;

   /***** Get the type of answer and the stem from the database *****/
   /* Get the question from database */
   DB_QuerySELECT (&mysql_res,"can not get a question",
		   "SELECT AnsType,"		// row[0]
		          "Shuffle,"		// row[1]
		          "Stem,"		// row[2]
		          "Feedback,"		// row[3]
	                  "MedCod"		// row[4]
		   " FROM tst_questions"
		   " WHERE QstCod=%ld AND CrsCod=%ld",
		   Gbl.Test.QstCod,Gbl.Hierarchy.Crs.CrsCod);
   row = mysql_fetch_row (mysql_res);

   /* Get the type of answer */
   Gbl.Test.AnswerType = Tst_ConvertFromStrAnsTypDBToAnsTyp (row[0]);

   /* Get shuffle (row[1]) */
   Gbl.Test.Shuffle = (row[1][0] == 'Y');

   /* Get the stem of the question from the database (row[2]) */
   Str_Copy (Stem,row[2],
             Cns_MAX_BYTES_TEXT);

   /* Get the feedback of the question from the database (row[3]) */
   Feedback[0] = '\0';
   if (row[3])
      if (row[3][0])
	 Str_Copy (Feedback,row[3],
	           Cns_MAX_BYTES_TEXT);

   /* Get media (row[4]) */
   Gbl.Test.Media.MedCod = Str_ConvertStrCodToLongCod (row[4]);
   Med_GetMediaDataByCod (&Gbl.Test.Media);

   /* Free structure that stores the query result */
   DB_FreeMySQLResult (&mysql_res);

   /***** Get the tags from the database *****/
   NumRows = Tst_GetTagsQst (Gbl.Test.QstCod,&mysql_res);
   for (NumRow = 0;
	NumRow < NumRows;
	NumRow++)
     {
      row = mysql_fetch_row (mysql_res);
      Str_Copy (Gbl.Test.Tags.Txt[NumRow],row[0],
                Tst_MAX_BYTES_TAG);
     }

   /* Free structure that stores the query result */
   DB_FreeMySQLResult (&mysql_res);

   /***** Get the answers from the database *****/
   Gbl.Test.Answer.NumOptions = Tst_GetAnswersQst (Gbl.Test.QstCod,&mysql_res,false);
   /*
   row[0] AnsInd
   row[1] Answer
   row[2] Feedback
   row[3] MedCod
   row[4] Correct
   */
   for (NumOpt = 0;
	NumOpt < Gbl.Test.Answer.NumOptions;
	NumOpt++)
     {
      row = mysql_fetch_row (mysql_res);
      switch (Gbl.Test.AnswerType)
	{
	 case Tst_ANS_INT:
	    if (Gbl.Test.Answer.NumOptions != 1)
	       Lay_ShowErrorAndExit ("Wrong answer.");
	    Gbl.Test.Answer.Integer = Tst_GetIntAnsFromStr (row[1]);
	    break;
	 case Tst_ANS_FLOAT:
	    if (Gbl.Test.Answer.NumOptions != 2)
	       Lay_ShowErrorAndExit ("Wrong answer.");
	    Gbl.Test.Answer.FloatingPoint[NumOpt] = Tst_GetFloatAnsFromStr (row[1]);
	    break;
	 case Tst_ANS_TRUE_FALSE:
	    if (Gbl.Test.Answer.NumOptions != 1)
	       Lay_ShowErrorAndExit ("Wrong answer.");
	    Gbl.Test.Answer.TF = row[1][0];
	    break;
	 case Tst_ANS_UNIQUE_CHOICE:
	 case Tst_ANS_MULTIPLE_CHOICE:
	 case Tst_ANS_TEXT:
	    if (Gbl.Test.Answer.NumOptions > Tst_MAX_OPTIONS_PER_QUESTION)
	       Lay_ShowErrorAndExit ("Wrong answer.");
	    if (!Tst_AllocateTextChoiceAnswer (NumOpt))
	       /* Abort on error */
	       Ale_ShowAlertsAndExit ();

	    Str_Copy (Gbl.Test.Answer.Options[NumOpt].Text,row[1],
	              Tst_MAX_BYTES_ANSWER_OR_FEEDBACK);

	    // Feedback (row[2]) is initialized to empty string
	    if (row[2])
	       if (row[2][0])
		  Str_Copy (Gbl.Test.Answer.Options[NumOpt].Feedback,row[2],
		            Tst_MAX_BYTES_ANSWER_OR_FEEDBACK);

            /* Get media (row[3]) */
	    Gbl.Test.Answer.Options[NumOpt].Media.MedCod = Str_ConvertStrCodToLongCod (row[3]);
	    Med_GetMediaDataByCod (&Gbl.Test.Answer.Options[NumOpt].Media);

            /* Get if this option is correct (row[4]) */
	    Gbl.Test.Answer.Options[NumOpt].Correct = (row[4][0] == 'Y');
	    break;
	 default:
	    break;
	}
     }
   /* Free structure that stores the query result */
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/******* Get media code associated with a test question from database ********/
/*****************************************************************************/
// NumOpt <  0 ==> media associated to stem
// NumOpt >= 0 ==> media associated to answer

static long Tst_GetMedCodFromDB (int NumOpt)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows;
   long MedCod = -1L;

   if (Gbl.Test.QstCod > 0)	// Existing question
     {
      /***** Query depending on NumOpt *****/
      if (NumOpt < 0)
	 // Get media associated to stem
	 NumRows = DB_QuerySELECT (&mysql_res,"can not get media",
				   "SELECT MedCod"		// row[0]
				   " FROM tst_questions"
				   " WHERE QstCod=%ld AND CrsCod=%ld",
				   Gbl.Test.QstCod,Gbl.Hierarchy.Crs.CrsCod);
      else
	 // Get media associated to answer
	 NumRows = DB_QuerySELECT (&mysql_res,"can not get media",
				   "SELECT MedCod"		// row[0]
				   " FROM tst_answers"
				   " WHERE QstCod=%ld AND AnsInd=%u",
				  Gbl.Test.QstCod,(unsigned) NumOpt);

      if (NumRows)
	{
	 if (NumRows == 1)
	   {
	    /***** Get media code (row[0]) *****/
	    row = mysql_fetch_row (mysql_res);
	    MedCod = Str_ConvertStrCodToLongCod (row[0]);
	   }
	 else	// NumRows > 1
	    Lay_ShowErrorAndExit ("Duplicated media in database.");
	}

      /***** Free structure that stores the query result *****/
      DB_FreeMySQLResult (&mysql_res);
     }

   return MedCod;
  }

/*****************************************************************************/
/***** Get possible media associated with a test question from database ******/
/*****************************************************************************/
// NumOpt <  0 ==> media associated to stem
// NumOpt >= 0 ==> media associated to an answer option

static void Tst_GetMediaFromDB (int NumOpt,struct Media *Media)
  {
   /***** Get media *****/
   Media->MedCod = Tst_GetMedCodFromDB (NumOpt);
   Med_GetMediaDataByCod (Media);
  }

/*****************************************************************************/
/** Convert a string with the type of answer in database to type of answer ***/
/*****************************************************************************/

Tst_AnswerType_t Tst_ConvertFromStrAnsTypDBToAnsTyp (const char *StrAnsTypeBD)
  {
   Tst_AnswerType_t AnsType;

   if (StrAnsTypeBD != NULL)
      for (AnsType = (Tst_AnswerType_t) 0;
	   AnsType < Tst_NUM_ANS_TYPES;
	   AnsType++)
         if (!strcmp (StrAnsTypeBD,Tst_StrAnswerTypesDB[AnsType]))
            return AnsType;

   Lay_ShowErrorAndExit ("Wrong type of answer.");
   return (Tst_AnswerType_t) 0;	// Not reached
  }

/*****************************************************************************/
/************ Convert a string with an unsigned to answer type ***************/
/*****************************************************************************/

static Tst_AnswerType_t Tst_ConvertFromUnsignedStrToAnsTyp (const char *UnsignedStr)
  {
   unsigned AnsType;

   if (sscanf (UnsignedStr,"%u",&AnsType) != 1)
      Lay_ShowErrorAndExit ("Wrong type of answer.");
   if (AnsType >= Tst_NUM_ANS_TYPES)
      Lay_ShowErrorAndExit ("Wrong type of answer.");
   return (Tst_AnswerType_t) AnsType;
  }

/*****************************************************************************/
/*************** Receive a question of the self-assessment test **************/
/*****************************************************************************/

void Tst_ReceiveQst (void)
  {
   char Stem[Cns_MAX_BYTES_TEXT + 1];
   char Feedback[Cns_MAX_BYTES_TEXT + 1];

   /***** Create test question *****/
   Tst_QstConstructor ();

   /***** Get parameters of the question from form *****/
   Stem[0] = Feedback[0] = '\0';
   Tst_GetQstFromForm (Stem,Feedback);

   /***** Make sure that tags, text and answer are not empty *****/
   if (Tst_CheckIfQstFormatIsCorrectAndCountNumOptions ())
     {
      /***** Move images to definitive directories *****/
      Tst_MoveMediaToDefinitiveDirectories ();

      /***** Insert or update question, tags and answer in the database *****/
      Tst_InsertOrUpdateQstTagsAnsIntoDB ();

      /***** Show the question just inserted in the database *****/
      Tst_ListOneQstToEdit ();
     }
   else	// Question is wrong
     {
      /***** Whether images has been received or not, reset images *****/
      Tst_ResetMediaOfQuestion ();

      /***** Put form to edit question again *****/
      Tst_PutFormEditOneQst (Stem,Feedback);
     }

   /***** Destroy test question *****/
   Tst_QstDestructor ();
  }

/*****************************************************************************/
/**************** Get parameters of a test question from form ****************/
/*****************************************************************************/

static void Tst_GetQstFromForm (char *Stem,char *Feedback)
  {
   unsigned NumTag;
   unsigned NumTagRead;
   unsigned NumOpt;
   char UnsignedStr[10 + 1];
   char TagStr[6 + 10 + 1];
   char AnsStr[6 + 10 + 1];
   char FbStr[5 + 10 + 1];
   char StrMultiAns[Tst_MAX_BYTES_ANSWERS_ONE_QST + 1];
   char TF[1 + 1];	// (T)rue or (F)alse
   const char *Ptr;
   unsigned NumCorrectAns;

   /***** Get question code *****/
   Gbl.Test.QstCod = Tst_GetQstCod ();

   /***** Get answer type *****/
   Gbl.Test.AnswerType = (Tst_AnswerType_t)
                         Par_GetParToUnsignedLong ("AnswerType",
                                                   0,
                                                   Tst_NUM_ANS_TYPES - 1,
                                                   (unsigned long) Tst_ANS_ALL);
   if (Gbl.Test.AnswerType == Tst_ANS_ALL)
      Lay_ShowErrorAndExit ("Wrong type of answer.");

   /***** Get question tags *****/
   for (NumTag = 0;
	NumTag < Tst_MAX_TAGS_PER_QUESTION;
	NumTag++)
     {
      snprintf (TagStr,sizeof (TagStr),
	        "TagTxt%u",
		NumTag);
      Par_GetParToText (TagStr,Gbl.Test.Tags.Txt[NumTag],Tst_MAX_BYTES_TAG);

      if (Gbl.Test.Tags.Txt[NumTag][0])
        {
         Str_ChangeFormat (Str_FROM_FORM,Str_TO_TEXT,
                           Gbl.Test.Tags.Txt[NumTag],Tst_MAX_BYTES_TAG,true);
         /* Check if not repeated */
         for (NumTagRead = 0;
              NumTagRead < NumTag;
              NumTagRead++)
            if (!strcmp (Gbl.Test.Tags.Txt[NumTagRead],Gbl.Test.Tags.Txt[NumTag]))
              {
               Gbl.Test.Tags.Txt[NumTag][0] = '\0';
               break;
              }
        }
     }

   /***** Get question stem *****/
   Par_GetParToHTML ("Stem",Stem,Cns_MAX_BYTES_TEXT);

   /***** Get question feedback *****/
   Par_GetParToHTML ("Feedback",Feedback,Cns_MAX_BYTES_TEXT);

   /***** Get media associated to the stem (action, file and title) *****/
   Gbl.Test.Media.Width   = Tst_IMAGE_SAVED_MAX_WIDTH;
   Gbl.Test.Media.Height  = Tst_IMAGE_SAVED_MAX_HEIGHT;
   Gbl.Test.Media.Quality = Tst_IMAGE_SAVED_QUALITY;
   Med_GetMediaFromForm (-1,	// < 0 ==> the image associated to the stem
                         &Gbl.Test.Media,Tst_GetMediaFromDB,
			 NULL);
   Ale_ShowAlerts (NULL);

   /***** Get answers *****/
   Gbl.Test.Shuffle = false;
   switch (Gbl.Test.AnswerType)
     {
      case Tst_ANS_INT:
         if (!Tst_AllocateTextChoiceAnswer (0))
	    /* Abort on error */
	    Ale_ShowAlertsAndExit ();

	 Par_GetParToText ("AnsInt",Gbl.Test.Answer.Options[0].Text,1 + 10);
	 break;
      case Tst_ANS_FLOAT:
         if (!Tst_AllocateTextChoiceAnswer (0))
	    /* Abort on error */
	    Ale_ShowAlertsAndExit ();

	 Par_GetParToText ("AnsFloatMin",Gbl.Test.Answer.Options[0].Text,
	                   Tst_MAX_BYTES_FLOAT_ANSWER);

         if (!Tst_AllocateTextChoiceAnswer (1))
	    /* Abort on error */
	    Ale_ShowAlertsAndExit ();

	 Par_GetParToText ("AnsFloatMax",Gbl.Test.Answer.Options[1].Text,
	                   Tst_MAX_BYTES_FLOAT_ANSWER);
	 break;
      case Tst_ANS_TRUE_FALSE:
	 Par_GetParToText ("AnsTF",TF,1);
	 Gbl.Test.Answer.TF = TF[0];
	 break;
      case Tst_ANS_UNIQUE_CHOICE:
      case Tst_ANS_MULTIPLE_CHOICE:
         /* Get shuffle */
         Gbl.Test.Shuffle = Par_GetParToBool ("Shuffle");
	 /* falls through */
	 /* no break */
      case Tst_ANS_TEXT:
         /* Get the texts of the answers */
         for (NumOpt = 0;
              NumOpt < Tst_MAX_OPTIONS_PER_QUESTION;
              NumOpt++)
           {
            if (!Tst_AllocateTextChoiceAnswer (NumOpt))
	       /* Abort on error */
	       Ale_ShowAlertsAndExit ();

            /* Get answer */
            snprintf (AnsStr,sizeof (AnsStr),
        	      "AnsStr%u",
		      NumOpt);
	    Par_GetParToHTML (AnsStr,Gbl.Test.Answer.Options[NumOpt].Text,
	                      Tst_MAX_BYTES_ANSWER_OR_FEEDBACK);
	    if (Gbl.Test.AnswerType == Tst_ANS_TEXT)
	       /* In order to compare student answer to stored answer,
	          the text answers are stored avoiding two or more consecurive spaces */
               Str_ReplaceSeveralSpacesForOne (Gbl.Test.Answer.Options[NumOpt].Text);

            /* Get feedback */
            snprintf (FbStr,sizeof (FbStr),
        	      "FbStr%u",
		      NumOpt);
	    Par_GetParToHTML (FbStr,Gbl.Test.Answer.Options[NumOpt].Feedback,
	                      Tst_MAX_BYTES_ANSWER_OR_FEEDBACK);

	    /* Get media associated to the answer (action, file and title) */
	    if (Gbl.Test.AnswerType == Tst_ANS_UNIQUE_CHOICE ||
		Gbl.Test.AnswerType == Tst_ANS_MULTIPLE_CHOICE)
	      {
	       Gbl.Test.Answer.Options[NumOpt].Media.Width   = Tst_IMAGE_SAVED_MAX_WIDTH;
	       Gbl.Test.Answer.Options[NumOpt].Media.Height  = Tst_IMAGE_SAVED_MAX_HEIGHT;
	       Gbl.Test.Answer.Options[NumOpt].Media.Quality = Tst_IMAGE_SAVED_QUALITY;
	       Med_GetMediaFromForm ((int) NumOpt,	// >= 0 ==> the image associated to an answer
	                             &Gbl.Test.Answer.Options[NumOpt].Media,
				     Tst_GetMediaFromDB,
				     NULL);
	       Ale_ShowAlerts (NULL);
	      }
           }

         /* Get the numbers of correct answers */
         if (Gbl.Test.AnswerType == Tst_ANS_UNIQUE_CHOICE)
           {
	    NumCorrectAns = (unsigned) Par_GetParToUnsignedLong ("AnsUni",
	                                                         0,
	                                                         Tst_MAX_OPTIONS_PER_QUESTION - 1,
	                                                         0);
            Gbl.Test.Answer.Options[NumCorrectAns].Correct = true;
           }
      	 else if (Gbl.Test.AnswerType == Tst_ANS_MULTIPLE_CHOICE)
           {
	    Par_GetParMultiToText ("AnsMulti",StrMultiAns,Tst_MAX_BYTES_ANSWERS_ONE_QST);
 	    Ptr = StrMultiAns;
            while (*Ptr)
              {
  	       Par_GetNextStrUntilSeparParamMult (&Ptr,UnsignedStr,10);
	       if (sscanf (UnsignedStr,"%u",&NumCorrectAns) != 1)
	          Lay_ShowErrorAndExit ("Wrong selected answer.");
               if (NumCorrectAns >= Tst_MAX_OPTIONS_PER_QUESTION)
	          Lay_ShowErrorAndExit ("Wrong selected answer.");
               Gbl.Test.Answer.Options[NumCorrectAns].Correct = true;
              }
           }
         else // Tst_ANS_TEXT
            for (NumOpt = 0;
        	 NumOpt < Tst_MAX_OPTIONS_PER_QUESTION;
        	 NumOpt++)
               if (Gbl.Test.Answer.Options[NumOpt].Text[0])
                  Gbl.Test.Answer.Options[NumOpt].Correct = true;	// All the answers are correct
	 break;
      default:
         break;
     }

   /***** Adjust global variables related to this test question *****/
   for (NumTag = 0, Gbl.Test.Tags.Num = 0;
        NumTag < Tst_MAX_TAGS_PER_QUESTION;
        NumTag++)
      if (Gbl.Test.Tags.Txt[NumTag][0])
         Gbl.Test.Tags.Num++;
   Gbl.Test.Stem.Text = Stem;
   Gbl.Test.Stem.Length = strlen (Gbl.Test.Stem.Text);
   Gbl.Test.Feedback.Text = Feedback;
   Gbl.Test.Feedback.Length = strlen (Gbl.Test.Feedback.Text);
  }

/*****************************************************************************/
/*********************** Check if a question is correct **********************/
/*****************************************************************************/
// Returns false if question format is wrong
// Counts Gbl.Test.Answer.NumOptions
// Computes Gbl.Test.Answer.Integer and Gbl.Test.Answer.FloatingPoint[0..1]

bool Tst_CheckIfQstFormatIsCorrectAndCountNumOptions (void)
  {
   extern const char *Txt_You_must_type_at_least_one_tag_for_the_question;
   extern const char *Txt_You_must_type_the_stem_of_the_question;
   extern const char *Txt_You_must_select_a_T_F_answer;
   extern const char *Txt_You_can_not_leave_empty_intermediate_answers;
   extern const char *Txt_You_must_type_at_least_the_first_two_answers;
   extern const char *Txt_You_must_mark_an_answer_as_correct;
   extern const char *Txt_You_must_type_at_least_the_first_answer;
   extern const char *Txt_You_must_enter_an_integer_value_as_the_correct_answer;
   extern const char *Txt_You_must_enter_the_range_of_floating_point_values_allowed_as_answer;
   extern const char *Txt_The_lower_limit_of_correct_answers_must_be_less_than_or_equal_to_the_upper_limit;
   unsigned NumOpt;
   unsigned NumLastOpt;
   bool ThereIsEndOfAnswers;
   unsigned i;

   /***** This function also counts the number of options. Initialize this number to 0. *****/
   Gbl.Test.Answer.NumOptions = 0;

   /***** A question must have at least one tag *****/
   if (!Gbl.Test.Tags.Num) // There are no tags with text
     {
      Ale_ShowAlert (Ale_WARNING,Txt_You_must_type_at_least_one_tag_for_the_question);
      return false;
     }

   /***** A question must have a stem*****/
   if (!Gbl.Test.Stem.Length)
     {
      Ale_ShowAlert (Ale_WARNING,Txt_You_must_type_the_stem_of_the_question);
      return false;
     }

   /***** Check answer *****/
   switch (Gbl.Test.AnswerType)
     {
      case Tst_ANS_INT:
	 /* First option should be filled */
         if (!Gbl.Test.Answer.Options[0].Text)
           {
            Ale_ShowAlert (Ale_WARNING,Txt_You_must_enter_an_integer_value_as_the_correct_answer);
            return false;
           }
         if (!Gbl.Test.Answer.Options[0].Text[0])
           {
            Ale_ShowAlert (Ale_WARNING,Txt_You_must_enter_an_integer_value_as_the_correct_answer);
            return false;
           }

         Gbl.Test.Answer.Integer = Tst_GetIntAnsFromStr (Gbl.Test.Answer.Options[0].Text);
         Gbl.Test.Answer.NumOptions = 1;
         break;
      case Tst_ANS_FLOAT:
	 /* First two options should be filled */
         if (!Gbl.Test.Answer.Options[0].Text ||
             !Gbl.Test.Answer.Options[1].Text)
           {
            Ale_ShowAlert (Ale_WARNING,Txt_You_must_enter_the_range_of_floating_point_values_allowed_as_answer);
            return false;
           }
         if (!Gbl.Test.Answer.Options[0].Text[0] ||
             !Gbl.Test.Answer.Options[1].Text[0])
           {
            Ale_ShowAlert (Ale_WARNING,Txt_You_must_enter_the_range_of_floating_point_values_allowed_as_answer);
            return false;
           }

         /* Lower limit should be <= upper limit */
         for (i = 0;
              i < 2;
              i++)
            Gbl.Test.Answer.FloatingPoint[i] = Tst_GetFloatAnsFromStr (Gbl.Test.Answer.Options[i].Text);
         if (Gbl.Test.Answer.FloatingPoint[0] >
             Gbl.Test.Answer.FloatingPoint[1])
           {
            Ale_ShowAlert (Ale_WARNING,Txt_The_lower_limit_of_correct_answers_must_be_less_than_or_equal_to_the_upper_limit);
            return false;
           }

         Gbl.Test.Answer.NumOptions = 2;
         break;
      case Tst_ANS_TRUE_FALSE:
	 /* Answer should be 'T' or 'F' */
         if (Gbl.Test.Answer.TF != 'T' &&
             Gbl.Test.Answer.TF != 'F')
           {
            Ale_ShowAlert (Ale_WARNING,Txt_You_must_select_a_T_F_answer);
            return false;
           }

         Gbl.Test.Answer.NumOptions = 1;
         break;
      case Tst_ANS_UNIQUE_CHOICE:
      case Tst_ANS_MULTIPLE_CHOICE:
	 /* No option should be empty before a non-empty option */
         for (NumOpt = 0, NumLastOpt = 0, ThereIsEndOfAnswers = false;
              NumOpt < Tst_MAX_OPTIONS_PER_QUESTION;
              NumOpt++)
            if (Gbl.Test.Answer.Options[NumOpt].Text)
              {
               if (Gbl.Test.Answer.Options[NumOpt].Text[0] ||			// Text
        	   Gbl.Test.Answer.Options[NumOpt].Media.Type != Med_TYPE_NONE)	// or media
                 {
                  if (ThereIsEndOfAnswers)
                    {
                     Ale_ShowAlert (Ale_WARNING,Txt_You_can_not_leave_empty_intermediate_answers);
                     return false;
                    }
                  NumLastOpt = NumOpt;
                  Gbl.Test.Answer.NumOptions++;
                 }
               else
                  ThereIsEndOfAnswers = true;
              }
            else
               ThereIsEndOfAnswers = true;

         /* The two first options must be filled */
         if (NumLastOpt < 1)
           {
            Ale_ShowAlert (Ale_WARNING,Txt_You_must_type_at_least_the_first_two_answers);
            return false;
           }

         /* Its mandatory to mark at least one option as correct */
         for (NumOpt = 0;
              NumOpt <= NumLastOpt;
              NumOpt++)
            if (Gbl.Test.Answer.Options[NumOpt].Correct)
               break;
         if (NumOpt > NumLastOpt)
           {
            Ale_ShowAlert (Ale_WARNING,Txt_You_must_mark_an_answer_as_correct);
            return false;
           }
         break;
      case Tst_ANS_TEXT:
	 /* First option should be filled */
         if (!Gbl.Test.Answer.Options[0].Text)		// If the first answer is empty
           {
            Ale_ShowAlert (Ale_WARNING,Txt_You_must_type_at_least_the_first_answer);
            return false;
           }
         if (!Gbl.Test.Answer.Options[0].Text[0])	// If the first answer is empty
           {
            Ale_ShowAlert (Ale_WARNING,Txt_You_must_type_at_least_the_first_answer);
            return false;
           }

	 /* No option should be empty before a non-empty option */
         for (NumOpt=0, ThereIsEndOfAnswers=false;
              NumOpt<Tst_MAX_OPTIONS_PER_QUESTION;
              NumOpt++)
            if (Gbl.Test.Answer.Options[NumOpt].Text)
              {
               if (Gbl.Test.Answer.Options[NumOpt].Text[0])
                 {
                  if (ThereIsEndOfAnswers)
                    {
                     Ale_ShowAlert (Ale_WARNING,Txt_You_can_not_leave_empty_intermediate_answers);
                     return false;
                    }
                  Gbl.Test.Answer.NumOptions++;
                 }
               else
                  ThereIsEndOfAnswers = true;
              }
            else
               ThereIsEndOfAnswers = true;
         break;
      default:
         break;
     }

    return true;	// Question format without errors
   }

/*****************************************************************************/
/* Move images associates to a test question to their definitive directories */
/*****************************************************************************/

static void Tst_MoveMediaToDefinitiveDirectories (void)
  {
   unsigned NumOpt;
   long CurrentMedCodInDB;

   /***** Media associated to question stem *****/
   CurrentMedCodInDB = Tst_GetMedCodFromDB (-1L);	// Get current media code associated to stem
   Med_RemoveKeepOrStoreMedia (CurrentMedCodInDB,&Gbl.Test.Media);

   /****** Move media associated to answers *****/
   if (Gbl.Test.AnswerType == Tst_ANS_UNIQUE_CHOICE ||
       Gbl.Test.AnswerType == Tst_ANS_MULTIPLE_CHOICE)
      for (NumOpt = 0;
	   NumOpt < Gbl.Test.Answer.NumOptions;
	   NumOpt++)
	{
         CurrentMedCodInDB = Tst_GetMedCodFromDB (NumOpt);	// Get current media code associated to this option
         Med_RemoveKeepOrStoreMedia (CurrentMedCodInDB,&Gbl.Test.Answer.Options[NumOpt].Media);
	}
  }

/*****************************************************************************/
/******************** Get a integer number from a string *********************/
/*****************************************************************************/

long Tst_GetIntAnsFromStr (char *Str)
  {
   long LongNum;

   if (Str == NULL)
      return 0.0;

   /***** The string is "scanned" as long *****/
   if (sscanf (Str,"%ld",&LongNum) != 1)	// If the string does not hold a valid integer number...
     {
      LongNum = 0L;	// ...the number is reset to 0
      Str[0] = '\0';	// ...and the string is reset to ""
     }

   return LongNum;
  }

/*****************************************************************************/
/************ Get a float number from a string in floating point *************/
/*****************************************************************************/

double Tst_GetFloatAnsFromStr (char *Str)
  {
   double DoubleNum;

   if (Str == NULL)
      return 0.0;

   /***** Change commnas to points *****/
   Str_ConvertStrFloatCommaToStrFloatPoint (Str);

   /***** The string is "scanned" in floating point (it must have a point, not a colon as decimal separator) *****/
   Str_SetDecimalPointToUS ();	// To get the decimal point as a dot
   if (sscanf (Str,"%lg",&DoubleNum) != 1)	// If the string does not hold a valid floating point number...
     {
      DoubleNum = 0.0;	// ...the number is reset to 0
      Str[0] = '\0';	// ...and the string is reset to ""
     }
   Str_SetDecimalPointToLocal ();	// Return to local system

   return DoubleNum;
  }

/*****************************************************************************/
/***************** Check if this tag exists for current course ***************/
/*****************************************************************************/

static long Tst_GetTagCodFromTagTxt (const char *TagTxt)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned long NumRows;
   long TagCod = -1L;	// -1 means that the tag does not exist in database

   /***** Get tag code from database *****/
   NumRows = DB_QuerySELECT (&mysql_res,"can not get tag",
			     "SELECT TagCod FROM tst_tags"
			     " WHERE CrsCod=%ld AND TagTxt='%s'",
			     Gbl.Hierarchy.Crs.CrsCod,TagTxt);
   if (NumRows == 1)
     {
      /***** Get tag code *****/
      row = mysql_fetch_row (mysql_res);
      if ((TagCod = Str_ConvertStrCodToLongCod (row[0])) < 0)
         Ale_CreateAlert (Ale_ERROR,NULL,
                          "Wrong code of tag.");
     }
   else if (NumRows > 1)
      Ale_CreateAlert (Ale_ERROR,NULL,
	               "Duplicated tag.");

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   /***** Abort on error *****/
   if (Ale_GetTypeOfLastAlert () == Ale_ERROR)
      Ale_ShowAlertsAndExit ();

   return TagCod;
  }

/*****************************************************************************/
/********************* Insert new tag into tst_tags table ********************/
/*****************************************************************************/

static long Tst_CreateNewTag (long CrsCod,const char *TagTxt)
  {
   /***** Insert new tag into tst_tags table *****/
   return
   DB_QueryINSERTandReturnCode ("can not create new tag",
				"INSERT INTO tst_tags"
				" (CrsCod,ChangeTime,TagTxt,TagHidden)"
				" VALUES"
				" (%ld,NOW(),'%s','N')",
				CrsCod,TagTxt);
  }

/*****************************************************************************/
/********** Change visibility of an existing tag into tst_tags table *********/
/*****************************************************************************/

static void Tst_EnableOrDisableTag (long TagCod,bool TagHidden)
  {
   /***** Insert new tag into tst_tags table *****/
   DB_QueryUPDATE ("can not update the visibility of a tag",
		   "UPDATE tst_tags SET TagHidden='%c',ChangeTime=NOW()"
                   " WHERE TagCod=%ld AND CrsCod=%ld",
		   TagHidden ? 'Y' :
			       'N',
		   TagCod,Gbl.Hierarchy.Crs.CrsCod);
  }

/*****************************************************************************/
/********************* Put icon to remove one question ***********************/
/*****************************************************************************/

static void Tst_PutIconToRemoveOneQst (void)
  {
   Ico_PutContextualIconToRemove (ActReqRemTstQst,Tst_PutParamsRemoveOneQst);
  }

/*****************************************************************************/
/******************** Request the removal of a question **********************/
/*****************************************************************************/

void Tst_RequestRemoveQst (void)
  {
   extern const char *Txt_Do_you_really_want_to_remove_the_question_X;
   extern const char *Txt_Remove_question;
   bool EditingOnlyThisQst;

   /***** Get main parameters from form *****/
   /* Get the question code */
   Gbl.Test.QstCod = Tst_GetQstCod ();
   if (Gbl.Test.QstCod <= 0)
      Lay_ShowErrorAndExit ("Wrong code of question.");

   /* Get a parameter that indicates whether it's necessary
      to continue listing the rest of questions */
   EditingOnlyThisQst = Par_GetParToBool ("OnlyThisQst");

   /* Get other parameters */
   if (!EditingOnlyThisQst)
      if (!Tst_GetParamsTst (Tst_EDIT_TEST))
	 Lay_ShowErrorAndExit ("Wrong test parameters.");

   /***** Show question and button to remove question *****/
   Ale_ShowAlertAndButton (ActRemTstQst,NULL,NULL,
			   EditingOnlyThisQst ? Tst_PutParamsRemoveOneQst :
						Tst_PutParamsRemoveQst,
			   Btn_REMOVE_BUTTON,Txt_Remove_question,
			   Ale_QUESTION,Txt_Do_you_really_want_to_remove_the_question_X,
			   (unsigned long) Gbl.Test.QstCod);

   /***** Continue editing questions *****/
   if (EditingOnlyThisQst)
      Tst_ListOneQstToEdit ();
   else
     {
      Tst_FreeTagsList ();
      Tst_ListQuestionsToEdit ();
     }
  }

/*****************************************************************************/
/***** Put parameter to remove question when editing only one question *******/
/*****************************************************************************/

static void Tst_PutParamsRemoveOneQst (void)
  {
   Tst_PutParamQstCod ();
   Par_PutHiddenParamChar ("OnlyThisQst",'Y');
  }

/*****************************************************************************/
/***** Put parameter to remove question when editing several questions *******/
/*****************************************************************************/

static void Tst_PutParamsRemoveQst (void)
  {
   Tst_PutParamQstCod ();
   Dat_WriteParamsIniEndDates ();
   Tst_WriteParamEditQst ();
  }

/*****************************************************************************/
/***************************** Remove a question *****************************/
/*****************************************************************************/

void Tst_RemoveQst (void)
  {
   extern const char *Txt_Question_removed;
   bool EditingOnlyThisQst;
   long MedCod;

   /***** Get the question code *****/
   Gbl.Test.QstCod = Tst_GetQstCod ();
   if (Gbl.Test.QstCod <= 0)
      Lay_ShowErrorAndExit ("Wrong code of question.");

   /***** Get a parameter that indicates whether it's necessary
          to continue listing the rest of questions ******/
   EditingOnlyThisQst = Par_GetParToBool ("OnlyThisQst");

   /***** Remove media associated to question *****/
   /* Remove media associated to answers */
   Tst_RemoveMediaFromAllAnsOfQst (Gbl.Hierarchy.Crs.CrsCod,Gbl.Test.QstCod);

   /* Remove media associated to stem */
   MedCod = Tst_GetMedCodFromDB (-1L);
   Med_RemoveMedia (MedCod);

   /***** Remove the question from all the tables *****/
   /* Remove answers and tags from this test question */
   Tst_RemAnsFromQst ();
   Tst_RemTagsFromQst ();
   Tst_RemoveUnusedTagsFromCurrentCrs ();

   /* Remove the question itself */
   DB_QueryDELETE ("can not remove a question",
		   "DELETE FROM tst_questions"
                   " WHERE QstCod=%ld AND CrsCod=%ld",
		   Gbl.Test.QstCod,Gbl.Hierarchy.Crs.CrsCod);

   if (!mysql_affected_rows (&Gbl.mysql))
      Lay_ShowErrorAndExit ("The question to be removed does not exist or belongs to another course.");

   /***** Write message *****/
   Ale_ShowAlert (Ale_SUCCESS,Txt_Question_removed);

   /***** Continue editing questions *****/
   if (!EditingOnlyThisQst)
      Tst_ListQuestionsToEdit ();
  }

/*****************************************************************************/
/*********************** Change the shuffle of a question ********************/
/*****************************************************************************/

void Tst_ChangeShuffleQst (void)
  {
   extern const char *Txt_The_answers_of_the_question_with_code_X_will_appear_shuffled;
   extern const char *Txt_The_answers_of_the_question_with_code_X_will_appear_without_shuffling;
   bool EditingOnlyThisQst;
   bool Shuffle;

   /***** Get the question code *****/
   Gbl.Test.QstCod = Tst_GetQstCod ();
   if (Gbl.Test.QstCod <= 0)
      Lay_ShowErrorAndExit ("Wrong code of question.");

   /***** Get a parameter that indicates whether it's necessary to continue listing the rest of questions ******/
   EditingOnlyThisQst = Par_GetParToBool ("OnlyThisQst");

   /***** Get a parameter that indicates whether it's possible to shuffle the answers of this question ******/
   Shuffle = Par_GetParToBool ("Shuffle");

   /***** Remove the question from all the tables *****/
   /* Update the question changing the current shuffle */
   DB_QueryUPDATE ("can not update the shuffle type of a question",
		   "UPDATE tst_questions SET Shuffle='%c'"
                   " WHERE QstCod=%ld AND CrsCod=%ld",
		   Shuffle ? 'Y' :
			     'N',
		   Gbl.Test.QstCod,Gbl.Hierarchy.Crs.CrsCod);

   /***** Write message *****/
   Ale_ShowAlert (Ale_SUCCESS,Shuffle ? Txt_The_answers_of_the_question_with_code_X_will_appear_shuffled :
                                        Txt_The_answers_of_the_question_with_code_X_will_appear_without_shuffling,
                  Gbl.Test.QstCod);

   /***** Continue editing questions *****/
   if (EditingOnlyThisQst)
      Tst_ListOneQstToEdit ();
   else
      Tst_ListQuestionsToEdit ();
  }

/*****************************************************************************/
/************ Get the parameter with the code of a test question *************/
/*****************************************************************************/

static long Tst_GetQstCod (void)
  {
   /***** Get code of test question *****/
   return Par_GetParToLong ("QstCod");
  }

/*****************************************************************************/
/************ Put parameter with question code to edit, remove... ************/
/*****************************************************************************/

void Tst_PutParamQstCod (void)
  {
   Par_PutHiddenParamLong ("QstCod",Gbl.Test.QstCod);
  }

/*****************************************************************************/
/******** Insert or update question, tags and anser in the database **********/
/*****************************************************************************/

void Tst_InsertOrUpdateQstTagsAnsIntoDB (void)
  {
   /***** Insert or update question in the table of questions *****/
   Tst_InsertOrUpdateQstIntoDB ();

   /***** Insert tags in the tags table *****/
   Tst_InsertTagsIntoDB ();

   /***** Remove unused tags in current course *****/
   Tst_RemoveUnusedTagsFromCurrentCrs ();

   /***** Insert answers in the answers table *****/
   Tst_InsertAnswersIntoDB ();
  }

/*****************************************************************************/
/*********** Insert or update question in the table of questions *************/
/*****************************************************************************/

static void Tst_InsertOrUpdateQstIntoDB (void)
  {
   if (Gbl.Test.QstCod < 0)	// It's a new question
     {
      /***** Insert question in the table of questions *****/
      Gbl.Test.QstCod =
      DB_QueryINSERTandReturnCode ("can not create question",
				   "INSERT INTO tst_questions"
				   " (CrsCod,EditTime,AnsType,Shuffle,"
				   "Stem,Feedback,MedCod,"
				   "NumHits,Score)"
				   " VALUES"
				   " (%ld,NOW(),'%s','%c',"
				   "'%s','%s',%ld,"
				   "0,0)",
				   Gbl.Hierarchy.Crs.CrsCod,
				   Tst_StrAnswerTypesDB[Gbl.Test.AnswerType],
				   Gbl.Test.Shuffle ? 'Y' :
						      'N',
				   Gbl.Test.Stem.Text,
				   Gbl.Test.Feedback.Text ? Gbl.Test.Feedback.Text : "",
				   Gbl.Test.Media.MedCod);
     }
   else				// It's an existing question
     {
      /***** Update existing question *****/
      /* Update question in database */
      DB_QueryUPDATE ("can not update question",
		      "UPDATE tst_questions"
		      " SET EditTime=NOW(),AnsType='%s',Shuffle='%c',"
		      "Stem='%s',Feedback='%s',MedCod=%ld"
		      " WHERE QstCod=%ld AND CrsCod=%ld",
		      Tst_StrAnswerTypesDB[Gbl.Test.AnswerType],
		      Gbl.Test.Shuffle ? 'Y' :
					 'N',
		      Gbl.Test.Stem.Text,
		      Gbl.Test.Feedback.Text ? Gbl.Test.Feedback.Text : "",
		      Gbl.Test.Media.MedCod,
		      Gbl.Test.QstCod,Gbl.Hierarchy.Crs.CrsCod);

      /* Remove answers and tags from this test question */
      Tst_RemAnsFromQst ();
      Tst_RemTagsFromQst ();
     }
  }

/*****************************************************************************/
/*********************** Insert tags in the tags table ***********************/
/*****************************************************************************/

static void Tst_InsertTagsIntoDB (void)
  {
   unsigned NumTag;
   unsigned TagIdx;
   long TagCod;

   /***** For each tag... *****/
   for (NumTag = 0, TagIdx = 0;
        TagIdx < Gbl.Test.Tags.Num;
        NumTag++)
      if (Gbl.Test.Tags.Txt[NumTag][0])
        {
         /***** Check if this tag exists for current course *****/
         if ((TagCod = Tst_GetTagCodFromTagTxt (Gbl.Test.Tags.Txt[NumTag])) < 0)
            /* This tag is new for current course. Add it to tags table */
            TagCod = Tst_CreateNewTag (Gbl.Hierarchy.Crs.CrsCod,Gbl.Test.Tags.Txt[NumTag]);

         /***** Insert tag in tst_question_tags *****/
         DB_QueryINSERT ("can not create tag",
			 "INSERT INTO tst_question_tags"
                         " (QstCod,TagCod,TagInd)"
                         " VALUES"
                         " (%ld,%ld,%u)",
			 Gbl.Test.QstCod,TagCod,TagIdx);

         TagIdx++;
        }
  }

/*****************************************************************************/
/******************* Insert answers in the answers table *********************/
/*****************************************************************************/

static void Tst_InsertAnswersIntoDB (void)
  {
   unsigned NumOpt;
   unsigned i;

   /***** Insert answers in the answers table *****/
   switch (Gbl.Test.AnswerType)
     {
      case Tst_ANS_INT:
         DB_QueryINSERT ("can not create answer",
			 "INSERT INTO tst_answers"
                         " (QstCod,AnsInd,Answer,Feedback,MedCod,Correct)"
                         " VALUES"
                         " (%ld,0,%ld,'',-1,'Y')",
			 Gbl.Test.QstCod,
			 Gbl.Test.Answer.Integer);
         break;
      case Tst_ANS_FLOAT:
	 Str_SetDecimalPointToUS ();		// To print the floating point as a dot
   	 for (i = 0;
   	      i < 2;
   	      i++)
            DB_QueryINSERT ("can not create answer",
        		    "INSERT INTO tst_answers"
                            " (QstCod,AnsInd,Answer,Feedback,MedCod,Correct)"
                            " VALUES"
                            " (%ld,%u,'%lg','',-1,'Y')",
			    Gbl.Test.QstCod,i,
			    Gbl.Test.Answer.FloatingPoint[i]);
         Str_SetDecimalPointToLocal ();	// Return to local system
         break;
      case Tst_ANS_TRUE_FALSE:
         DB_QueryINSERT ("can not create answer",
			 "INSERT INTO tst_answers"
                         " (QstCod,AnsInd,Answer,Feedback,MedCod,Correct)"
                         " VALUES"
                         " (%ld,0,'%c','',-1,'Y')",
			 Gbl.Test.QstCod,
			 Gbl.Test.Answer.TF);
         break;
      case Tst_ANS_UNIQUE_CHOICE:
      case Tst_ANS_MULTIPLE_CHOICE:
      case Tst_ANS_TEXT:
         for (NumOpt = 0;
              NumOpt < Gbl.Test.Answer.NumOptions;
              NumOpt++)
            if (Gbl.Test.Answer.Options[NumOpt].Text[0] ||			// Text
        	Gbl.Test.Answer.Options[NumOpt].Media.Type != Med_TYPE_NONE)	// or media
              {
               DB_QueryINSERT ("can not create answer",
        		       "INSERT INTO tst_answers"
                               " (QstCod,AnsInd,Answer,Feedback,MedCod,Correct)"
                               " VALUES"
                               " (%ld,%u,'%s','%s',%ld,'%c')",
			       Gbl.Test.QstCod,NumOpt,
			       Gbl.Test.Answer.Options[NumOpt].Text,
			       Gbl.Test.Answer.Options[NumOpt].Feedback ? Gbl.Test.Answer.Options[NumOpt].Feedback : "",
			       Gbl.Test.Answer.Options[NumOpt].Media.MedCod,
			       Gbl.Test.Answer.Options[NumOpt].Correct ? 'Y' :
									 'N');

               /* Update image status */
	       if (Gbl.Test.Answer.Options[NumOpt].Media.Type != Med_TYPE_NONE)
		  Gbl.Test.Answer.Options[NumOpt].Media.Status = Med_STORED_IN_DB;
              }
	 break;
      default:
         break;
     }
  }

/*****************************************************************************/
/******************** Remove answers from a test question ********************/
/*****************************************************************************/

static void Tst_RemAnsFromQst (void)
  {
   /***** Remove answers *****/
   DB_QueryDELETE ("can not remove the answers of a question",
		   "DELETE FROM tst_answers WHERE QstCod=%ld",
		   Gbl.Test.QstCod);
  }

/*****************************************************************************/
/************************** Remove tags from a test question *****************/
/*****************************************************************************/

static void Tst_RemTagsFromQst (void)
  {
   /***** Remove tags *****/
   DB_QueryDELETE ("can not remove the tags of a question",
		   "DELETE FROM tst_question_tags WHERE QstCod=%ld",
		   Gbl.Test.QstCod);
  }

/*****************************************************************************/
/******************** Remove unused tags in current course *******************/
/*****************************************************************************/

static void Tst_RemoveUnusedTagsFromCurrentCrs (void)
  {
   /***** Remove unused tags from tst_tags *****/
   DB_QueryDELETE ("can not remove unused tags",
		   "DELETE FROM tst_tags"
	           " WHERE CrsCod=%ld AND TagCod NOT IN"
                   " (SELECT DISTINCT tst_question_tags.TagCod"
                   " FROM tst_questions,tst_question_tags"
                   " WHERE tst_questions.CrsCod=%ld"
                   " AND tst_questions.QstCod=tst_question_tags.QstCod)",
		   Gbl.Hierarchy.Crs.CrsCod,
		   Gbl.Hierarchy.Crs.CrsCod);
  }

/*****************************************************************************/
/** Remove all media associated to stems of all test questions in a course ***/
/*****************************************************************************/

static void Tst_RemoveAllMedFilesFromStemOfAllQstsInCrs (long CrsCod)
  {
   MYSQL_RES *mysql_res;
   unsigned NumMedia;

   /***** Get media codes associated to stems of test questions from database *****/
   NumMedia =
   (unsigned) DB_QuerySELECT (&mysql_res,"can not get media",
			      "SELECT MedCod"	// row[0]
			      " FROM tst_questions"
			      " WHERE CrsCod=%ld",
			      CrsCod);

   /***** Go over result removing media files *****/
   Med_RemoveMediaFromAllRows (NumMedia,mysql_res);

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/******* Remove all media associated to all answers of a test question *******/
/*****************************************************************************/

static void Tst_RemoveMediaFromAllAnsOfQst (long CrsCod,long QstCod)
  {
   MYSQL_RES *mysql_res;
   unsigned NumMedia;

   /***** Get media codes associated to answers of test questions from database *****/
   NumMedia =
   (unsigned) DB_QuerySELECT (&mysql_res,"can not get media",
			      "SELECT tst_answers.MedCod"	// row[0]
			      " FROM tst_questions,tst_answers"
			      " WHERE tst_questions.CrsCod=%ld"	// Extra check
			      " AND tst_questions.QstCod=%ld"	// Extra check
			      " AND tst_questions.QstCod=tst_answers.QstCod"
			      " AND tst_answers.QstCod=%ld",
			      CrsCod,QstCod,QstCod);

   /***** Go over result removing media *****/
   Med_RemoveMediaFromAllRows (NumMedia,mysql_res);

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/* Remove media associated to all answers of all test questions in a course **/
/*****************************************************************************/

static void Tst_RemoveAllMedFilesFromAnsOfAllQstsInCrs (long CrsCod)
  {
   MYSQL_RES *mysql_res;
   unsigned NumMedia;

   /***** Get names of media files associated to answers of test questions from database *****/
   NumMedia =
   (unsigned) DB_QuerySELECT (&mysql_res,"can not get media",
			      "SELECT tst_answers.MedCod"	// row[0]
			      " FROM tst_questions,tst_answers"
			      " WHERE tst_questions.CrsCod=%ld"
			      " AND tst_questions.QstCod=tst_answers.QstCod",
			      CrsCod);

   /***** Go over result removing media files *****/
   Med_RemoveMediaFromAllRows (NumMedia,mysql_res);

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/*********************** Get stats about test questions **********************/
/*****************************************************************************/

void Tst_GetTestStats (Tst_AnswerType_t AnsType,struct Tst_Stats *Stats)
  {
   Stats->NumQsts = 0;
   Stats->NumCoursesWithQuestions = Stats->NumCoursesWithPluggableQuestions = 0;
   Stats->AvgQstsPerCourse = 0.0;
   Stats->NumHits = 0L;
   Stats->AvgHitsPerCourse = 0.0;
   Stats->AvgHitsPerQuestion = 0.0;
   Stats->TotalScore = 0.0;
   Stats->AvgScorePerQuestion = 0.0;

   if (Tst_GetNumTstQuestions (Gbl.Scope.Current,AnsType,Stats))
     {
      if ((Stats->NumCoursesWithQuestions = Tst_GetNumCoursesWithTstQuestions (Gbl.Scope.Current,AnsType)) != 0)
        {
         Stats->NumCoursesWithPluggableQuestions = Tst_GetNumCoursesWithPluggableTstQuestions (Gbl.Scope.Current,AnsType);
         Stats->AvgQstsPerCourse = (float) Stats->NumQsts / (float) Stats->NumCoursesWithQuestions;
         Stats->AvgHitsPerCourse = (float) Stats->NumHits / (float) Stats->NumCoursesWithQuestions;
        }
      Stats->AvgHitsPerQuestion = (float) Stats->NumHits / (float) Stats->NumQsts;
      if (Stats->NumHits)
         Stats->AvgScorePerQuestion = Stats->TotalScore / (double) Stats->NumHits;
     }
  }

/*****************************************************************************/
/*********************** Get number of test questions ************************/
/*****************************************************************************/
// Returns the number of test questions
// in this location (all the platform, current degree or current course)

static unsigned Tst_GetNumTstQuestions (Hie_Level_t Scope,Tst_AnswerType_t AnsType,struct Tst_Stats *Stats)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;

   /***** Get number of test questions from database *****/
   switch (Scope)
     {
      case Hie_SYS:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of test questions",
        		    "SELECT COUNT(*),SUM(NumHits),SUM(Score)"
        	            " FROM tst_questions");
         else
            DB_QuerySELECT (&mysql_res,"can not get number of test questions",
        		    "SELECT COUNT(*),SUM(NumHits),SUM(Score)"
        	            " FROM tst_questions"
                            " WHERE AnsType='%s'",
			    Tst_StrAnswerTypesDB[AnsType]);
         break;
      case Hie_CTY:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of test questions",
        		    "SELECT COUNT(*),SUM(NumHits),SUM(Score)"
        	            " FROM institutions,centres,degrees,courses,tst_questions"
                            " WHERE institutions.CtyCod=%ld"
                            " AND institutions.InsCod=centres.InsCod"
                            " AND centres.CtrCod=degrees.CtrCod"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod",
			    Gbl.Hierarchy.Cty.CtyCod);
         else
            DB_QuerySELECT (&mysql_res,"can not get number of test questions",
        		    "SELECT COUNT(*),SUM(NumHits),SUM(Score)"
        	            " FROM institutions,centres,degrees,courses,tst_questions"
                            " WHERE institutions.CtyCod=%ld"
                            " AND institutions.InsCod=centres.InsCod"
                            " AND centres.CtrCod=degrees.CtrCod"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod"
                            " AND tst_questions.AnsType='%s'",
			    Gbl.Hierarchy.Cty.CtyCod,
			    Tst_StrAnswerTypesDB[AnsType]);
         break;
      case Hie_INS:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of test questions",
        		    "SELECT COUNT(*),SUM(NumHits),SUM(Score)"
        	            " FROM centres,degrees,courses,tst_questions"
                            " WHERE centres.InsCod=%ld"
                            " AND centres.CtrCod=degrees.CtrCod"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod",
			    Gbl.Hierarchy.Ins.InsCod);
         else
            DB_QuerySELECT (&mysql_res,"can not get number of test questions",
        		    "SELECT COUNT(*),SUM(NumHits),SUM(Score)"
        	            " FROM centres,degrees,courses,tst_questions"
                            " WHERE centres.InsCod=%ld"
                            " AND centres.CtrCod=degrees.CtrCod"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod"
                            " AND tst_questions.AnsType='%s'",
			    Gbl.Hierarchy.Ins.InsCod,
			    Tst_StrAnswerTypesDB[AnsType]);
         break;
      case Hie_CTR:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of test questions",
        		    "SELECT COUNT(*),SUM(NumHits),SUM(Score)"
        	            " FROM degrees,courses,tst_questions"
                            " WHERE degrees.CtrCod=%ld"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod",
			    Gbl.Hierarchy.Ctr.CtrCod);
         else
            DB_QuerySELECT (&mysql_res,"can not get number of test questions",
        		    "SELECT COUNT(*),SUM(NumHits),SUM(Score)"
        	            " FROM degrees,courses,tst_questions"
                            " WHERE degrees.CtrCod=%ld"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod"
                            " AND tst_questions.AnsType='%s'",
			    Gbl.Hierarchy.Ctr.CtrCod,
			    Tst_StrAnswerTypesDB[AnsType]);
         break;
      case Hie_DEG:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of test questions",
        		    "SELECT COUNT(*),SUM(NumHits),SUM(Score)"
        	            " FROM courses,tst_questions"
                            " WHERE courses.DegCod=%ld"
                            " AND courses.CrsCod=tst_questions.CrsCod",
			    Gbl.Hierarchy.Deg.DegCod);
         else
            DB_QuerySELECT (&mysql_res,"can not get number of test questions",
        		    "SELECT COUNT(*),SUM(NumHits),SUM(Score)"
        	            " FROM courses,tst_questions"
                            " WHERE courses.DegCod=%ld"
                            " AND courses.CrsCod=tst_questions.CrsCod"
                            " AND tst_questions.AnsType='%s'",
			    Gbl.Hierarchy.Deg.DegCod,
			    Tst_StrAnswerTypesDB[AnsType]);
         break;
      case Hie_CRS:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of test questions",
        		    "SELECT COUNT(*),SUM(NumHits),SUM(Score)"
        	            " FROM tst_questions"
                            " WHERE CrsCod=%ld",
			    Gbl.Hierarchy.Crs.CrsCod);
         else
            DB_QuerySELECT (&mysql_res,"can not get number of test questions",
        		    "SELECT COUNT(*),SUM(NumHits),SUM(Score)"
        	            " FROM tst_questions"
                            " WHERE CrsCod=%ld AND AnsType='%s'",
			    Gbl.Hierarchy.Crs.CrsCod,
			    Tst_StrAnswerTypesDB[AnsType]);
         break;
      default:
	 Lay_WrongScopeExit ();
	 break;
     }

   /***** Get number of questions *****/
   row = mysql_fetch_row (mysql_res);
   if (sscanf (row[0],"%u",&(Stats->NumQsts)) != 1)
      Lay_ShowErrorAndExit ("Error when getting number of test questions.");

   if (Stats->NumQsts)
     {
      if (sscanf (row[1],"%lu",&(Stats->NumHits)) != 1)
         Lay_ShowErrorAndExit ("Error when getting total number of hits in test questions.");

      Str_SetDecimalPointToUS ();	// To get the decimal point as a dot
      if (sscanf (row[2],"%lf",&(Stats->TotalScore)) != 1)
         Lay_ShowErrorAndExit ("Error when getting total score in test questions.");
      Str_SetDecimalPointToLocal ();	// Return to local system
     }
   else
     {
      Stats->NumHits = 0L;
      Stats->TotalScore = 0.0;
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   return Stats->NumQsts;
  }

/*****************************************************************************/
/**************** Get number of courses with test questions ******************/
/*****************************************************************************/
// Returns the number of courses with test questions
// in this location (all the platform, current degree or current course)

static unsigned Tst_GetNumCoursesWithTstQuestions (Hie_Level_t Scope,Tst_AnswerType_t AnsType)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumCourses;

   /***** Get number of courses with test questions from database *****/
   switch (Scope)
     {
      case Hie_SYS:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with test questions",
        		    "SELECT COUNT(DISTINCT CrsCod)"
        	            " FROM tst_questions");
         else
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with test questions",
        		    "SELECT COUNT(DISTINCT CrsCod)"
        	            " FROM tst_questions"
                            " WHERE AnsType='%s'",
			    Tst_StrAnswerTypesDB[AnsType]);
         break;
      case Hie_CTY:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with test questions",
        		    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM institutions,centres,degrees,courses,tst_questions"
                            " WHERE institutions.CtyCod=%ld"
                            " AND institutions.InsCod=centres.InsCod"
                            " AND centres.CtrCod=degrees.CtrCod"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod",
			    Gbl.Hierarchy.Cty.CtyCod);
         else
           DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with test questions",
        		    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM institutions,centres,degrees,courses,tst_questions"
                            " WHERE institutions.CtyCod=%ld"
                            " AND institutions.InsCod=centres.InsCod"
                            " AND centres.CtrCod=degrees.CtrCod"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod"
                            " AND tst_questions.AnsType='%s'",
			    Gbl.Hierarchy.Cty.CtyCod,
			    Tst_StrAnswerTypesDB[AnsType]);
         break;
      case Hie_INS:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with test questions",
        		    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM centres,degrees,courses,tst_questions"
                            " WHERE centres.InsCod=%ld"
                            " AND centres.CtrCod=degrees.CtrCod"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod",
			    Gbl.Hierarchy.Ins.InsCod);
         else
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with test questions",
        		    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM centres,degrees,courses,tst_questions"
                            " WHERE centres.InsCod=%ld"
                            " AND centres.CtrCod=degrees.CtrCod"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod"
                            " AND tst_questions.AnsType='%s'",
			    Gbl.Hierarchy.Ins.InsCod,
			    Tst_StrAnswerTypesDB[AnsType]);
         break;
      case Hie_CTR:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with test questions",
        		    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM degrees,courses,tst_questions"
                            " WHERE degrees.CtrCod=%ld"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod",
			    Gbl.Hierarchy.Ctr.CtrCod);
         else
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with test questions",
        		    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM degrees,courses,tst_questions"
                            " WHERE degrees.CtrCod=%ld"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod"
                            " AND tst_questions.AnsType='%s'",
			    Gbl.Hierarchy.Ctr.CtrCod,
			    Tst_StrAnswerTypesDB[AnsType]);
         break;
      case Hie_DEG:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with test questions",
        		    "SELECT COUNTDISTINCT (tst_questions.CrsCod)"
        	            " FROM courses,tst_questions"
                            " WHERE courses.DegCod=%ld"
                            " AND courses.CrsCod=tst_questions.CrsCod",
			    Gbl.Hierarchy.Deg.DegCod);
         else
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with test questions",
        		    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM courses,tst_questions"
                            " WHERE courses.DegCod=%ld"
                            " AND courses.CrsCod=tst_questions.CrsCod"
                            " AND tst_questions.AnsType='%s'",
			    Gbl.Hierarchy.Deg.DegCod,
			    Tst_StrAnswerTypesDB[AnsType]);
         break;
      case Hie_CRS:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with test questions",
        		    "SELECT COUNT(DISTINCT CrsCod)"
        	            " FROM tst_questions"
                            " WHERE CrsCod=%ld",
			    Gbl.Hierarchy.Crs.CrsCod);
         else
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with test questions",
        		    "SELECT COUNT(DISTINCT CrsCod)"
			    " FROM tst_questions"
			    " WHERE CrsCod=%ld"
			    " AND AnsType='%s'",
			    Gbl.Hierarchy.Crs.CrsCod,
			    Tst_StrAnswerTypesDB[AnsType]);
         break;
      default:
	 Lay_WrongScopeExit ();
	 break;
     }

   /***** Get number of courses *****/
   row = mysql_fetch_row (mysql_res);
   if (sscanf (row[0],"%u",&NumCourses) != 1)
      Lay_ShowErrorAndExit ("Error when getting number of courses with test questions.");

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   return NumCourses;
  }

/*****************************************************************************/
/*********** Get number of courses with pluggable test questions *************/
/*****************************************************************************/
// Returns the number of courses with pluggable test questions
// in this location (all the platform, current degree or current course)

static unsigned Tst_GetNumCoursesWithPluggableTstQuestions (Hie_Level_t Scope,Tst_AnswerType_t AnsType)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumCourses;

   /***** Get number of courses with test questions from database *****/
   switch (Scope)
     {
      case Hie_SYS:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with pluggable test questions",
			    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM tst_questions,tst_config"
                            " WHERE tst_questions.CrsCod=tst_config.CrsCod"
                            " AND tst_config.pluggable='%s'",
			    Tst_PluggableDB[Tst_PLUGGABLE_YES]);
         else
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with pluggable test questions",
			    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM tst_questions,tst_config"
                            " WHERE tst_questions.AnsType='%s'"
                            " AND tst_questions.CrsCod=tst_config.CrsCod"
                            " AND tst_config.pluggable='%s'",
			    Tst_StrAnswerTypesDB[AnsType],
			    Tst_PluggableDB[Tst_PLUGGABLE_YES]);
         break;
      case Hie_CTY:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with pluggable test questions",
			    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM institutions,centres,degrees,courses,tst_questions,tst_config"
                            " WHERE institutions.CtyCod=%ld"
                            " AND institutions.InsCod=centres.InsCod"
                            " AND centres.CtrCod=degrees.CtrCod"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod"
                            " AND tst_questions.CrsCod=tst_config.CrsCod"
                            " AND tst_config.pluggable='%s'",
			    Gbl.Hierarchy.Cty.CtyCod,
			    Tst_PluggableDB[Tst_PLUGGABLE_YES]);
         else
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with pluggable test questions",
			    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM institutions,centres,degrees,courses,tst_questions,tst_config"
                            " WHERE institutions.CtyCod=%ld"
                            " AND institutions.InsCod=centres.InsCod"
                            " AND centres.CtrCod=degrees.CtrCod"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod"
                            " AND tst_questions.AnsType='%s'"
                            " AND tst_questions.CrsCod=tst_config.CrsCod"
                            " AND tst_config.pluggable='%s'",
			    Gbl.Hierarchy.Cty.CtyCod,
			    Tst_StrAnswerTypesDB[AnsType],
			    Tst_PluggableDB[Tst_PLUGGABLE_YES]);
         break;
      case Hie_INS:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with pluggable test questions",
			    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM centres,degrees,courses,tst_questions,tst_config"
                            " WHERE centres.InsCod=%ld"
                            " AND centres.CtrCod=degrees.CtrCod"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod"
                            " AND tst_questions.CrsCod=tst_config.CrsCod"
                            " AND tst_config.pluggable='%s'",
			    Gbl.Hierarchy.Ins.InsCod,
			    Tst_PluggableDB[Tst_PLUGGABLE_YES]);
         else
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with pluggable test questions",
			    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM centres,degrees,courses,tst_questions,tst_config"
                            " WHERE centres.InsCod=%ld"
                            " AND centres.CtrCod=degrees.CtrCod"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod"
                            " AND tst_questions.AnsType='%s'"
                            " AND tst_questions.CrsCod=tst_config.CrsCod"
                            " AND tst_config.pluggable='%s'",
			    Gbl.Hierarchy.Ins.InsCod,
			    Tst_StrAnswerTypesDB[AnsType],
			    Tst_PluggableDB[Tst_PLUGGABLE_YES]);
         break;
      case Hie_CTR:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with pluggable test questions",
			    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM degrees,courses,tst_questions,tst_config"
                            " WHERE degrees.CtrCod=%ld"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod"
                            " AND tst_questions.CrsCod=tst_config.CrsCod"
                            " AND tst_config.pluggable='%s'",
			    Gbl.Hierarchy.Ctr.CtrCod,
			    Tst_PluggableDB[Tst_PLUGGABLE_YES]);
         else
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with pluggable test questions",
			    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM degrees,courses,tst_questions,tst_config"
                            " WHERE degrees.CtrCod=%ld"
                            " AND degrees.DegCod=courses.DegCod"
                            " AND courses.CrsCod=tst_questions.CrsCod"
                            " AND tst_questions.AnsType='%s'"
                            " AND tst_questions.CrsCod=tst_config.CrsCod"
                            " AND tst_config.pluggable='%s'",
			    Gbl.Hierarchy.Ctr.CtrCod,
			    Tst_StrAnswerTypesDB[AnsType],
			    Tst_PluggableDB[Tst_PLUGGABLE_YES]);
         break;
      case Hie_DEG:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with pluggable test questions",
			    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM courses,tst_questions,tst_config"
                            " WHERE courses.DegCod=%ld"
                            " AND courses.CrsCod=tst_questions.CrsCod"
                            " AND tst_questions.CrsCod=tst_config.CrsCod"
                            " AND tst_config.pluggable='%s'",
			    Gbl.Hierarchy.Deg.DegCod,
			    Tst_PluggableDB[Tst_PLUGGABLE_YES]);
         else
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with pluggable test questions",
			    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM courses,tst_questions,tst_config"
                            " WHERE courses.DegCod=%ld"
                            " AND courses.CrsCod=tst_questions.CrsCod"
                            " AND tst_questions.AnsType='%s'"
                            " AND tst_questions.CrsCod=tst_config.CrsCod"
                            " AND tst_config.pluggable='%s'",
			    Gbl.Hierarchy.Deg.DegCod,
			    Tst_StrAnswerTypesDB[AnsType],
			    Tst_PluggableDB[Tst_PLUGGABLE_YES]);
         break;
      case Hie_CRS:
         if (AnsType == Tst_ANS_ALL)
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with pluggable test questions",
			    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM tst_questions,tst_config"
                            " WHERE tst_questions.CrsCod=%ld"
                            " AND tst_questions.CrsCod=tst_config.CrsCod"
                            " AND tst_config.pluggable='%s'",
			    Gbl.Hierarchy.Crs.CrsCod,
			    Tst_PluggableDB[Tst_PLUGGABLE_YES]);
         else
            DB_QuerySELECT (&mysql_res,"can not get number of courses"
        			       " with pluggable test questions",
			    "SELECT COUNT(DISTINCT tst_questions.CrsCod)"
        	            " FROM tst_questions,tst_config"
                            " WHERE tst_questions.CrsCod=%ld"
                            " AND tst_questions.AnsType='%s'"
                            " AND tst_questions.CrsCod=tst_config.CrsCod"
                            " AND tst_config.pluggable='%s'",
			    Gbl.Hierarchy.Crs.CrsCod,
			    Tst_StrAnswerTypesDB[AnsType],
			    Tst_PluggableDB[Tst_PLUGGABLE_YES]);
         break;
      default:
	 Lay_WrongScopeExit ();
	 break;
     }

   /***** Get number of courses *****/
   row = mysql_fetch_row (mysql_res);
   if (sscanf (row[0],"%u",&NumCourses) != 1)
      Lay_ShowErrorAndExit ("Error when getting number of courses with pluggable test questions.");

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   return NumCourses;
  }

/*****************************************************************************/
/************ Select users and dates to show their test results **************/
/*****************************************************************************/

void Tst_SelUsrsToSeeUsrsTestResults (void)
  {
   extern const char *Hlp_ASSESSMENT_Tests_test_results;
   extern const char *The_ClassFormInBox[The_NUM_THEMES];
   extern const char *Txt_Test_results;
   extern const char *Txt_Users;
   extern const char *Txt_View_test_results;
   unsigned NumTotalUsrs;

   /***** Get and update type of list,
          number of columns in class photo
          and preference about viewing photos *****/
   Usr_GetAndUpdatePrefsAboutUsrList ();

   /***** Get groups to show ******/
   Grp_GetParCodsSeveralGrpsToShowUsrs ();

   /***** Get and order lists of users from this course *****/
   Usr_GetListUsrs (Hie_CRS,Rol_STD);
   Usr_GetListUsrs (Hie_CRS,Rol_NET);
   Usr_GetListUsrs (Hie_CRS,Rol_TCH);
   NumTotalUsrs = Gbl.Usrs.LstUsrs[Rol_STD].NumUsrs +
	          Gbl.Usrs.LstUsrs[Rol_NET].NumUsrs +
	          Gbl.Usrs.LstUsrs[Rol_TCH].NumUsrs;

   /***** Start box *****/
   Box_StartBox (NULL,Txt_Test_results,NULL,
                 Hlp_ASSESSMENT_Tests_test_results,Box_NOT_CLOSABLE);

   /***** Show form to select the groups *****/
   Grp_ShowFormToSelectSeveralGroups (ActReqSeeUsrTstRes,Grp_ONLY_MY_GROUPS);

   /***** Start section with user list *****/
   Lay_StartSection (Usr_USER_LIST_SECTION_ID);

   if (NumTotalUsrs)
     {
      if (Usr_GetIfShowBigList (NumTotalUsrs,NULL))
        {
	 /***** Form to select type of list used for select several users *****/
	 Usr_ShowFormsToSelectUsrListType (ActReqSeeUsrTstRes);

         /***** Start form *****/
         Frm_StartForm (ActSeeUsrTstRes);
         Grp_PutParamsCodGrps ();

         /***** Put list of users to select some of them *****/
         Tbl_StartTableCenter (2);
         fprintf (Gbl.F.Out,"<tr>"
			    "<td class=\"%s RIGHT_TOP\">"
			    "%s:"
			    "</td>"
			    "<td colspan=\"2\" class=\"%s LEFT_TOP\">",
                  The_ClassFormInBox[Gbl.Prefs.Theme],Txt_Users,
                  The_ClassFormInBox[Gbl.Prefs.Theme]);
         Tbl_StartTable (2);
         Usr_ListUsersToSelect (Rol_TCH);
         Usr_ListUsersToSelect (Rol_NET);
         Usr_ListUsersToSelect (Rol_STD);
         Tbl_EndTable ();
         fprintf (Gbl.F.Out,"</td>"
                            "</tr>");

         /***** Starting and ending dates in the search *****/
         Dat_PutFormStartEndClientLocalDateTimesWithYesterdayToday (false);

         Tbl_EndTable ();

         /***** Send button *****/
	 Btn_PutConfirmButton (Txt_View_test_results);

         /***** End form *****/
         Frm_EndForm ();
        }
     }
   else	// NumTotalUsrs == 0
      /***** Show warning indicating no students found *****/
      Usr_ShowWarningNoUsersFound (Rol_UNK);

   /***** End section with user list *****/
   Lay_EndSection ();

   /***** End box *****/
   Box_EndBox ();

   /***** Free memory for users' list *****/
   Usr_FreeUsrsList (Rol_TCH);
   Usr_FreeUsrsList (Rol_NET);
   Usr_FreeUsrsList (Rol_STD);

   /***** Free memory used by list of selected users' codes *****/
   Usr_FreeListsSelectedUsrsCods ();

   /***** Free memory for list of selected groups *****/
   Grp_FreeListCodSelectedGrps ();
  }

/*****************************************************************************/
/******************* Select dates to show my test results ********************/
/*****************************************************************************/

void Tst_SelDatesToSeeMyTestResults (void)
  {
   extern const char *Hlp_ASSESSMENT_Tests_test_results;
   extern const char *Txt_Test_results;
   extern const char *Txt_View_test_results;

   /***** Start form *****/
   Frm_StartForm (ActSeeMyTstRes);

   /***** Start box and table *****/
   Box_StartBoxTable (NULL,Txt_Test_results,NULL,
                      Hlp_ASSESSMENT_Tests_test_results,Box_NOT_CLOSABLE,2);
   Dat_PutFormStartEndClientLocalDateTimesWithYesterdayToday (false);

   /***** End table, send button and end box *****/
   Box_EndBoxTableWithButton (Btn_CONFIRM_BUTTON,Txt_View_test_results);

   /***** End form *****/
   Frm_EndForm ();
  }

/*****************************************************************************/
/********************* Store test result in database *************************/
/*****************************************************************************/

static long Tst_CreateTestResultInDB (void)
  {
   /***** Insert new test result into table *****/
   return
   DB_QueryINSERTandReturnCode ("can not create new test result",
				"INSERT INTO tst_exams"
				" (CrsCod,UsrCod,AllowTeachers,TstTime,NumQsts)"
				" VALUES"
				" (%ld,%ld,'%c',NOW(),%u)",
				Gbl.Hierarchy.Crs.CrsCod,
				Gbl.Usrs.Me.UsrDat.UsrCod,
				Gbl.Test.AllowTeachers ? 'Y' :
							 'N',
				Gbl.Test.NumQsts);
  }

/*****************************************************************************/
/********************* Store test result in database *************************/
/*****************************************************************************/

static void Tst_StoreScoreOfTestResultInDB (long TstCod,
                                          unsigned NumQstsNotBlank,double Score)
  {
   /***** Update score in test result *****/
   Str_SetDecimalPointToUS ();	// To print the floating point as a dot
   DB_QueryUPDATE ("can not update result of test result",
		   "UPDATE tst_exams"
	           " SET NumQstsNotBlank=%u,Score='%lf'"
	           " WHERE TstCod=%ld",
		   NumQstsNotBlank,Score,
		   TstCod);
   Str_SetDecimalPointToLocal ();	// Return to local system
  }

/*****************************************************************************/
/******************** Show test results for several users ********************/
/*****************************************************************************/

void Tst_ShowUsrsTestResults (void)
  {
   extern const char *Hlp_ASSESSMENT_Tests_test_results;
   extern const char *Txt_Test_results;
   extern const char *Txt_You_must_select_one_ore_more_users;
   const char *Ptr;

   /***** Get list of the selected users's IDs *****/
   Usr_GetListsSelectedUsrsCods ();

   /***** Get starting and ending dates *****/
   Dat_GetIniEndDatesFromForm ();

   /***** Check the number of users whose tests results will be shown *****/
   if (Usr_CountNumUsrsInListOfSelectedUsrs ())	// If some users are selected...
     {
      /***** Start box and table *****/
      Box_StartBoxTable (NULL,Txt_Test_results,NULL,
                         Hlp_ASSESSMENT_Tests_test_results,Box_NOT_CLOSABLE,2);

      /***** Header of the table with the list of users *****/
      Tst_ShowHeaderTestResults ();

      /***** List the test exams of the selected users *****/
      Ptr = Gbl.Usrs.Selected.List[Rol_UNK];
      while (*Ptr)
	{
	 Par_GetNextStrUntilSeparParamMult (&Ptr,Gbl.Usrs.Other.UsrDat.EncryptedUsrCod,
	                                    Cry_BYTES_ENCRYPTED_STR_SHA256_BASE64);
	 Usr_GetUsrCodFromEncryptedUsrCod (&Gbl.Usrs.Other.UsrDat);
	 if (Usr_ChkUsrCodAndGetAllUsrDataFromUsrCod (&Gbl.Usrs.Other.UsrDat,Usr_DONT_GET_PREFS))               // Get of the database the data of the user
	    if (Usr_CheckIfICanViewTst (&Gbl.Usrs.Other.UsrDat))
	       /***** Show test results *****/
	       Tst_ShowTestResults (&Gbl.Usrs.Other.UsrDat);
	}

      /***** End table and box *****/
      Box_EndBoxTable ();
     }
   else	// If no users are selected...
     {
      // ...write warning alert
      Ale_ShowAlert (Ale_WARNING,Txt_You_must_select_one_ore_more_users);
      // ...and show again the form
      Tst_SelUsrsToSeeUsrsTestResults ();
     }

   /***** Free memory used by list of selected users' codes *****/
   Usr_FreeListsSelectedUsrsCods ();
  }

/*****************************************************************************/
/*********************** Show header of my test results **********************/
/*****************************************************************************/

static void Tst_ShowHeaderTestResults (void)
  {
   extern const char *Txt_User[Usr_NUM_SEXS];
   extern const char *Txt_Date;
   extern const char *Txt_Questions;
   extern const char *Txt_Non_blank_BR_questions;
   extern const char *Txt_Total_BR_score;
   extern const char *Txt_Average_BR_score_BR_per_question_BR_from_0_to_1;
   extern const char *Txt_Score;
   extern const char *Txt_out_of_PART_OF_A_SCORE;

   fprintf (Gbl.F.Out,"<tr>"
		      "<th colspan=\"2\" class=\"CENTER_TOP\">"
		      "%s"
		      "</th>"
		      "<th class=\"RIGHT_TOP\">"
		      "%s"
		      "</th>"
		      "<th class=\"RIGHT_TOP\">"
		      "%s"
		      "</th>"
		      "<th class=\"RIGHT_TOP\">"
		      "%s"
		      "</th>"
		      "<th class=\"RIGHT_TOP\">"
		      "%s"
		      "</th>"
		      "<th class=\"RIGHT_TOP\">"
		      "%s"
		      "</th>"
		      "<th class=\"RIGHT_TOP\">"
		      "%s<br />%s<br />%u"
		      "</th>"
		      "<th></th>"
		      "</tr>",
	    Txt_User[Usr_SEX_UNKNOWN],
	    Txt_Date,
	    Txt_Questions,
	    Txt_Non_blank_BR_questions,
	    Txt_Total_BR_score,
	    Txt_Average_BR_score_BR_per_question_BR_from_0_to_1,
	    Txt_Score,Txt_out_of_PART_OF_A_SCORE,Tst_SCORE_MAX);
  }

/*****************************************************************************/
/***************************** Show my test results **************************/
/*****************************************************************************/

void Tst_ShowMyTestResults (void)
  {
   extern const char *Hlp_ASSESSMENT_Tests_test_results;
   extern const char *Txt_Test_results;

   /***** Get starting and ending dates *****/
   Dat_GetIniEndDatesFromForm ();

   /***** Start box and table *****/
   Box_StartBoxTable (NULL,Txt_Test_results,NULL,
                      Hlp_ASSESSMENT_Tests_test_results,Box_NOT_CLOSABLE,2);

   /***** Header of the table with the list of users *****/
   Tst_ShowHeaderTestResults ();

   /***** List my test results *****/
   Tst_GetConfigTstFromDB ();	// To get feedback type
   Tst_ShowTestResults (&Gbl.Usrs.Me.UsrDat);

   /***** End table and box *****/
   Box_EndBoxTable ();
  }

/*****************************************************************************/
/*********** Show the test results of a user in the current course ***********/
/*****************************************************************************/

static void Tst_ShowTestResults (struct UsrData *UsrDat)
  {
   extern const char *Txt_Today;
   extern const char *Txt_View_test;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumExams;
   unsigned NumTest;
   static unsigned UniqueId = 0;
   long TstCod;
   unsigned NumQstsInThisTest;
   unsigned NumQstsNotBlankInThisTest;
   unsigned NumTotalQsts = 0;
   unsigned NumTotalQstsNotBlank = 0;
   double ScoreInThisTest;
   double TotalScoreOfAllTests = 0.0;
   unsigned NumExamsVisibleByTchs = 0;
   bool ItsMe = Usr_ItsMe (UsrDat->UsrCod);
   bool ICanViewTest;
   bool ICanViewScore;
   time_t TimeUTC;
   char *ClassDat;

   /***** Make database query *****/
   NumExams =
   (unsigned) DB_QuerySELECT (&mysql_res,"can not get test exams of a user",
			      "SELECT TstCod,"			// row[0]
			             "AllowTeachers,"		// row[1]
			             "UNIX_TIMESTAMP(TstTime),"	// row[2]
			             "NumQsts,"			// row[3]
			             "NumQstsNotBlank,"		// row[4]
			             "Score"			// row[5]
			      " FROM tst_exams"
			      " WHERE CrsCod=%ld AND UsrCod=%ld"
			      " AND TstTime>=FROM_UNIXTIME(%ld)"
			      " AND TstTime<=FROM_UNIXTIME(%ld)"
			      " ORDER BY TstCod",
			      Gbl.Hierarchy.Crs.CrsCod,
			      UsrDat->UsrCod,
			      (long) Gbl.DateRange.TimeUTC[0],
			      (long) Gbl.DateRange.TimeUTC[1]);

   /***** Show user's data *****/
   fprintf (Gbl.F.Out,"<tr>");
   Tst_ShowDataUsr (UsrDat,NumExams);

   /***** Get and print test results *****/
   if (NumExams)
     {
      for (NumTest = 0;
           NumTest < NumExams;
           NumTest++)
        {
         row = mysql_fetch_row (mysql_res);

         /* Get test code (row[0]) */
	 if ((TstCod = Str_ConvertStrCodToLongCod (row[0])) < 0)
	    Lay_ShowErrorAndExit ("Wrong code of test result.");

	 /* Get if teachers are allowed to see this test result (row[1]) */
	 Gbl.Test.AllowTeachers = (row[1][0] == 'Y');
	 ClassDat = Gbl.Test.AllowTeachers ? "DAT" :
	                                     "DAT_LIGHT";

	 switch (Gbl.Usrs.Me.Role.Logged)
	   {
	    case Rol_STD:
	       ICanViewTest  = ItsMe;
	       ICanViewScore = ItsMe &&
		               Gbl.Test.Config.Feedback != Tst_FEEDBACK_NOTHING;
	       break;
	    case Rol_NET:
	    case Rol_TCH:
	    case Rol_DEG_ADM:
	    case Rol_CTR_ADM:
	    case Rol_INS_ADM:
	       ICanViewTest  =
	       ICanViewScore = ItsMe ||
	                       Gbl.Test.AllowTeachers;
	       break;
	    case Rol_SYS_ADM:
	       ICanViewTest  =
	       ICanViewScore = true;
	       break;
	    default:
	       ICanViewTest  =
	       ICanViewScore = false;
               break;
	   }

         if (NumTest)
            fprintf (Gbl.F.Out,"<tr>");

         /* Write date and time (row[2] holds UTC date-time) */
         TimeUTC = Dat_GetUNIXTimeFromStr (row[2]);
         UniqueId++;
	 fprintf (Gbl.F.Out,"<td id =\"tst_date_%u\" class=\"%s RIGHT_TOP COLOR%u\">"
			    "<script type=\"text/javascript\">"
			    "writeLocalDateHMSFromUTC('tst_date_%u',%ld,"
			    "%u,',&nbsp;','%s',true,false,0x7);"
			    "</script>"
			    "</td>",
	          UniqueId,ClassDat,Gbl.RowEvenOdd,
	          UniqueId,(long) TimeUTC,
	          (unsigned) Gbl.Prefs.DateFormat,Txt_Today);

         /* Get number of questions (row[3]) */
         if (sscanf (row[3],"%u",&NumQstsInThisTest) != 1)
            NumQstsInThisTest = 0;
	 if (Gbl.Test.AllowTeachers)
	    NumTotalQsts += NumQstsInThisTest;

         /* Get number of questions not blank (row[4]) */
         if (sscanf (row[4],"%u",&NumQstsNotBlankInThisTest) != 1)
            NumQstsNotBlankInThisTest = 0;
	 if (Gbl.Test.AllowTeachers)
	    NumTotalQstsNotBlank += NumQstsNotBlankInThisTest;

         /* Get score (row[5]) */
	 Str_SetDecimalPointToUS ();		// To get the decimal point as a dot
         if (sscanf (row[5],"%lf",&ScoreInThisTest) != 1)
            ScoreInThisTest = 0.0;
         Str_SetDecimalPointToLocal ();	// Return to local system
	 if (Gbl.Test.AllowTeachers)
	    TotalScoreOfAllTests += ScoreInThisTest;

         /* Write number of questions */
	 fprintf (Gbl.F.Out,"<td class=\"%s RIGHT_TOP COLOR%u\">",
	          ClassDat,Gbl.RowEvenOdd);
	 if (ICanViewTest)
	    fprintf (Gbl.F.Out,"%u",NumQstsInThisTest);
	 fprintf (Gbl.F.Out,"</td>");

         /* Write number of questions not blank */
	 fprintf (Gbl.F.Out,"<td class=\"%s RIGHT_TOP COLOR%u\">",
	          ClassDat,Gbl.RowEvenOdd);
	 if (ICanViewTest)
	    fprintf (Gbl.F.Out,"%u",NumQstsNotBlankInThisTest);
	 fprintf (Gbl.F.Out,"</td>");

	 /* Write score */
	 fprintf (Gbl.F.Out,"<td class=\"%s RIGHT_TOP COLOR%u\">",
	          ClassDat,Gbl.RowEvenOdd);
	 if (ICanViewScore)
	    fprintf (Gbl.F.Out,"%.2lf",ScoreInThisTest);
	 fprintf (Gbl.F.Out,"</td>");

         /* Write average score per question */
	 fprintf (Gbl.F.Out,"<td class=\"%s RIGHT_TOP COLOR%u\">",
	          ClassDat,Gbl.RowEvenOdd);
	 if (ICanViewScore)
	    fprintf (Gbl.F.Out,"%.2lf",
		     NumQstsInThisTest ? ScoreInThisTest / (double) NumQstsInThisTest :
			                 0.0);
	 fprintf (Gbl.F.Out,"</td>");

         /* Write score over Tst_SCORE_MAX */
	 fprintf (Gbl.F.Out,"<td class=\"%s RIGHT_TOP COLOR%u\">",
	          ClassDat,Gbl.RowEvenOdd);
	 if (ICanViewScore)
	    fprintf (Gbl.F.Out,"%.2lf",
		     NumQstsInThisTest ? ScoreInThisTest * Tst_SCORE_MAX / (double) NumQstsInThisTest :
			                 0.0);
	 fprintf (Gbl.F.Out,"</td>");

	 /* Link to show this result */
	 fprintf (Gbl.F.Out,"<td class=\"RIGHT_TOP COLOR%u\">",
		  Gbl.RowEvenOdd);
	 if (ICanViewTest)
	   {
	    Frm_StartForm (Gbl.Action.Act == ActSeeMyTstRes ? ActSeeOneTstResMe :
						              ActSeeOneTstResOth);
	    Tst_PutParamTstCod (TstCod);
	    Ico_PutIconLink ("tasks.svg",Txt_View_test);
	    Frm_EndForm ();
	   }
	 fprintf (Gbl.F.Out,"</td>"
                            "</tr>");

	 if (Gbl.Test.AllowTeachers)
            NumExamsVisibleByTchs++;
        }

      /***** Write totals for this user *****/
      Tst_ShowTestResultsSummaryRow (ItsMe,NumExamsVisibleByTchs,
                                     NumTotalQsts,NumTotalQstsNotBlank,
                                     TotalScoreOfAllTests);
     }
   else
      fprintf (Gbl.F.Out,"<td class=\"COLOR%u\"></td>"
	                 "<td class=\"COLOR%u\"></td>"
	                 "<td class=\"COLOR%u\"></td>"
	                 "<td class=\"COLOR%u\"></td>"
	                 "<td class=\"COLOR%u\"></td>"
	                 "<td class=\"COLOR%u\"></td>"
	                 "<td class=\"COLOR%u\"></td>"
	                 "</tr>",
	       Gbl.RowEvenOdd,
	       Gbl.RowEvenOdd,
	       Gbl.RowEvenOdd,
	       Gbl.RowEvenOdd,
	       Gbl.RowEvenOdd,
	       Gbl.RowEvenOdd,
	       Gbl.RowEvenOdd);

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);

   Gbl.RowEvenOdd = 1 - Gbl.RowEvenOdd;
  }

/*****************************************************************************/
/******************** Show a row with the data of a user *********************/
/*****************************************************************************/

static void Tst_ShowDataUsr (struct UsrData *UsrDat,unsigned NumExams)
  {
   bool ShowPhoto;
   char PhotoURL[PATH_MAX + 1];
   Act_Action_t NextAction;

   /***** Show user's photo and name *****/
   fprintf (Gbl.F.Out,"<td ");
   if (NumExams)
      fprintf (Gbl.F.Out,"rowspan=\"%u\"",NumExams + 1);
   fprintf (Gbl.F.Out," class=\"LEFT_TOP COLOR%u\">",
	    Gbl.RowEvenOdd);
   ShowPhoto = Pho_ShowingUsrPhotoIsAllowed (UsrDat,PhotoURL);
   Pho_ShowUsrPhoto (UsrDat,ShowPhoto ? PhotoURL :
                	                NULL,
                     "PHOTO45x60",Pho_ZOOM,false);
   fprintf (Gbl.F.Out,"</td>");

   /***** Start form to go to user's record card *****/
   fprintf (Gbl.F.Out,"<td ");
   if (NumExams)
      fprintf (Gbl.F.Out,"rowspan=\"%u\"",NumExams + 1);
   fprintf (Gbl.F.Out," class=\"LEFT_TOP COLOR%u\">",
	    Gbl.RowEvenOdd);
   switch (UsrDat->Roles.InCurrentCrs.Role)
     {
      case Rol_STD:
	 NextAction = ActSeeRecOneStd;
	 break;
      case Rol_NET:
      case Rol_TCH:
	 NextAction = ActSeeRecOneTch;
	 break;
      default:
	 NextAction = ActUnk;
	 Lay_ShowErrorAndExit ("Wrong role.");
	 break;
     }
   Frm_StartForm (NextAction);
   Usr_PutParamUsrCodEncrypted (UsrDat->EncryptedUsrCod);
   Frm_LinkFormSubmit (UsrDat->FullName,"AUTHOR_TXT",NULL);

   /***** Show user's ID *****/
   ID_WriteUsrIDs (UsrDat,NULL);

   /***** Show user's name *****/
   fprintf (Gbl.F.Out,"<br />%s",UsrDat->Surname1);
   if (UsrDat->Surname2[0])
      fprintf (Gbl.F.Out," %s",UsrDat->Surname2);
   if (UsrDat->FirstName[0])
      fprintf (Gbl.F.Out,",<br />%s",UsrDat->FirstName);

   /***** End form *****/
   Frm_EndForm ();
   fprintf (Gbl.F.Out,"</td>");
  }

/*****************************************************************************/
/******************** Write parameter with code of test **********************/
/*****************************************************************************/

static void Tst_PutParamTstCod (long TstCod)
  {
   Par_PutHiddenParamLong ("TstCod",TstCod);
  }

/*****************************************************************************/
/********************* Get parameter with code of test ***********************/
/*****************************************************************************/

static long Tst_GetParamTstCod (void)
  {
   /***** Get code of test *****/
   return Par_GetParToLong ("TstCod");
  }

/*****************************************************************************/
/**************** Show row with summary of user's test results ***************/
/*****************************************************************************/

static void Tst_ShowTestResultsSummaryRow (bool ItsMe,
                                           unsigned NumExams,
                                           unsigned NumTotalQsts,
                                           unsigned NumTotalQstsNotBlank,
                                           double TotalScoreOfAllTests)
  {
   extern const char *Txt_Visible_tests;
   bool ICanViewTotalScore;

   switch (Gbl.Usrs.Me.Role.Logged)
     {
      case Rol_STD:
	 ICanViewTotalScore = ItsMe &&
			      Gbl.Test.Config.Feedback != Tst_FEEDBACK_NOTHING;
	 break;
      case Rol_NET:
      case Rol_TCH:
      case Rol_DEG_ADM:
      case Rol_CTR_ADM:
      case Rol_INS_ADM:
	 ICanViewTotalScore = ItsMe ||
			      NumExams;
	 break;
      case Rol_SYS_ADM:
	 ICanViewTotalScore = true;
	 break;
      default:
	 ICanViewTotalScore = false;
	 break;
     }

   /***** Start row *****/
   fprintf (Gbl.F.Out,"<tr>");

   /***** Row title *****/
   fprintf (Gbl.F.Out,"<td class=\"DAT_N_LINE_TOP RIGHT_MIDDLE COLOR%u\">"
		      "%s: %u"
		      "</td>",
	    Gbl.RowEvenOdd,
	    Txt_Visible_tests,NumExams);

   /***** Write total number of questions *****/
   fprintf (Gbl.F.Out,"<td class=\"DAT_N_LINE_TOP RIGHT_MIDDLE COLOR%u\">",
	    Gbl.RowEvenOdd);
   if (NumExams)
      fprintf (Gbl.F.Out,"%u",NumTotalQsts);
   fprintf (Gbl.F.Out,"</td>");

   /***** Write total number of questions not blank *****/
   fprintf (Gbl.F.Out,"<td class=\"DAT_N_LINE_TOP RIGHT_MIDDLE COLOR%u\">",
	    Gbl.RowEvenOdd);
   if (NumExams)
      fprintf (Gbl.F.Out,"%u",NumTotalQstsNotBlank);
   fprintf (Gbl.F.Out,"</td>");

   /***** Write total score *****/
   fprintf (Gbl.F.Out,"<td class=\"DAT_N_LINE_TOP RIGHT_MIDDLE COLOR%u\">",
	    Gbl.RowEvenOdd);
   if (ICanViewTotalScore)
      fprintf (Gbl.F.Out,"%.2lf",TotalScoreOfAllTests);
   fprintf (Gbl.F.Out,"</td>");

   /***** Write average score per question *****/
   fprintf (Gbl.F.Out,"<td class=\"DAT_N_LINE_TOP RIGHT_MIDDLE COLOR%u\">",
	    Gbl.RowEvenOdd);
   if (ICanViewTotalScore)
      fprintf (Gbl.F.Out,"%.2lf",
	       NumTotalQsts ? TotalScoreOfAllTests / (double) NumTotalQsts :
			      0.0);
   fprintf (Gbl.F.Out,"</td>");

   /***** Write score over Tst_SCORE_MAX *****/
   fprintf (Gbl.F.Out,"<td class=\"DAT_N_LINE_TOP RIGHT_MIDDLE COLOR%u\">",
	    Gbl.RowEvenOdd);
   if (ICanViewTotalScore)
      fprintf (Gbl.F.Out,"%.2lf",
	       NumTotalQsts ? TotalScoreOfAllTests * Tst_SCORE_MAX /
			      (double) NumTotalQsts :
			      0.0);
   fprintf (Gbl.F.Out,"</td>");

   /***** Last cell *****/
   fprintf (Gbl.F.Out,"<td class=\"DAT_N_LINE_TOP COLOR%u\"></td>",
	    Gbl.RowEvenOdd);

   /***** End row *****/
   fprintf (Gbl.F.Out,"</tr>");
  }

/*****************************************************************************/
/******************* Show one test result of another user ********************/
/*****************************************************************************/

void Tst_ShowOneTestResult (void)
  {
   extern const char *Hlp_ASSESSMENT_Tests_test_results;
   extern const char *Txt_Test_result;
   extern const char *Txt_ROLES_SINGUL_Abc[Rol_NUM_ROLES][Usr_NUM_SEXS];
   extern const char *Txt_Date;
   extern const char *Txt_Today;
   extern const char *Txt_Questions;
   extern const char *Txt_non_blank_QUESTIONS;
   extern const char *Txt_Score;
   extern const char *Txt_out_of_PART_OF_A_SCORE;
   extern const char *Txt_Tags;
   long TstCod;
   time_t TstTimeUTC = 0;	// Test result UTC date-time, initialized to avoid warning
   unsigned NumQstsNotBlank;
   double TotalScore;
   bool ShowPhoto;
   char PhotoURL[PATH_MAX + 1];
   bool ItsMe;
   bool ICanViewTest;
   bool ICanViewScore;

   /***** Get the code of the test *****/
   if ((TstCod = Tst_GetParamTstCod ()) == -1L)
      Lay_ShowErrorAndExit ("Code of test is missing.");

   /***** Get test result data *****/
   Tst_GetTestResultDataByTstCod (TstCod,&TstTimeUTC,&Gbl.Test.NumQsts,&NumQstsNotBlank,&TotalScore);
   Gbl.Test.Config.Feedback = Tst_FEEDBACK_FULL_FEEDBACK;   // Initialize feedback to maximum

   /***** Check if I can view this test result *****/
   ItsMe = Usr_ItsMe (Gbl.Usrs.Other.UsrDat.UsrCod);
   switch (Gbl.Usrs.Me.Role.Logged)
     {
      case Rol_STD:
	 ICanViewTest = ItsMe;
	 if (ItsMe)
	   {
	    Tst_GetConfigTstFromDB ();	// To get feedback type
	    ICanViewScore = Gbl.Test.Config.Feedback != Tst_FEEDBACK_NOTHING;
	   }
	 else
	    ICanViewScore = false;
	 break;
      case Rol_TCH:
      case Rol_DEG_ADM:
      case Rol_CTR_ADM:
      case Rol_INS_ADM:
	 switch (Gbl.Action.Act)
	   {
	    case ActSeeOneTstResMe:
	       ICanViewTest  =
	       ICanViewScore = ItsMe;
	       break;
	    case ActSeeOneTstResOth:
	       ICanViewTest  =
	       ICanViewScore = ItsMe ||
			       Gbl.Test.AllowTeachers;
	       break;
	    default:
	       ICanViewTest  =
	       ICanViewScore = false;
	       break;
	   }
	 break;
      case Rol_SYS_ADM:
	 ICanViewTest  =
	 ICanViewScore = true;
	 break;
      default:
	 ICanViewTest  =
	 ICanViewScore = false;
	 break;
     }

   if (ICanViewTest)	// I am allowed to view this test result
     {
      /***** Get questions and user's answers of the test result from database *****/
      Tst_GetTestResultQuestionsFromDB (TstCod);

      /***** Start box *****/
      Box_StartBox (NULL,Txt_Test_result,NULL,
                    Hlp_ASSESSMENT_Tests_test_results,Box_NOT_CLOSABLE);
      Lay_WriteHeaderClassPhoto (false,false,
				 Gbl.Hierarchy.Ins.InsCod,
				 Gbl.Hierarchy.Deg.DegCod,
				 Gbl.Hierarchy.Crs.CrsCod);

      /***** Start table *****/
      Tbl_StartTableWideMargin (10);

      /***** Header row *****/
      /* Get data of the user who made the test */
      if (!Usr_ChkUsrCodAndGetAllUsrDataFromUsrCod (&Gbl.Usrs.Other.UsrDat,Usr_DONT_GET_PREFS))
	 Lay_ShowErrorAndExit ("User does not exists.");
      if (!Usr_CheckIfICanViewTst (&Gbl.Usrs.Other.UsrDat))
	 Lay_ShowErrorAndExit ("You can not view this test result.");

      /* User */
      fprintf (Gbl.F.Out,"<tr>"
			 "<td class=\"DAT_N RIGHT_TOP\">"
			 "%s:"
			 "</td>"
			 "<td class=\"DAT LEFT_TOP\">",
	       Txt_ROLES_SINGUL_Abc[Gbl.Usrs.Other.UsrDat.Roles.InCurrentCrs.Role][Gbl.Usrs.Other.UsrDat.Sex]);
      ID_WriteUsrIDs (&Gbl.Usrs.Other.UsrDat,NULL);
      fprintf (Gbl.F.Out," %s",
	       Gbl.Usrs.Other.UsrDat.Surname1);
      if (Gbl.Usrs.Other.UsrDat.Surname2[0])
	 fprintf (Gbl.F.Out," %s",
		  Gbl.Usrs.Other.UsrDat.Surname2);
      if (Gbl.Usrs.Other.UsrDat.FirstName[0])
	 fprintf (Gbl.F.Out,", %s",
		  Gbl.Usrs.Other.UsrDat.FirstName);
      fprintf (Gbl.F.Out,"<br />");
      ShowPhoto = Pho_ShowingUsrPhotoIsAllowed (&Gbl.Usrs.Other.UsrDat,PhotoURL);
      Pho_ShowUsrPhoto (&Gbl.Usrs.Other.UsrDat,ShowPhoto ? PhotoURL :
							   NULL,
			"PHOTO45x60",Pho_ZOOM,false);
      fprintf (Gbl.F.Out,"</td>"
			 "</tr>");

      /* Test date */
      fprintf (Gbl.F.Out,"<tr>"
			 "<td class=\"DAT_N RIGHT_TOP\">"
			 "%s:"
			 "</td>"
			 "<td id=\"test\" class=\"DAT LEFT_TOP\">"
			 "<script type=\"text/javascript\">"
			 "writeLocalDateHMSFromUTC('test',%ld,"
			 "%u,',&nbsp;','%s',true,true,0x7);"
			 "</script>"
			 "</td>"
			 "</tr>",
	       Txt_Date,TstTimeUTC,
	       (unsigned) Gbl.Prefs.DateFormat,Txt_Today);

      /* Number of questions */
      fprintf (Gbl.F.Out,"<tr>"
			 "<td class=\"DAT_N RIGHT_TOP\">"
			 "%s:"
			 "</td>"
			 "<td class=\"DAT LEFT_TOP\">"
			 "%u (%u %s)"
			 "</td>"
			 "</tr>",
	       Txt_Questions,
	       Gbl.Test.NumQsts,NumQstsNotBlank,Txt_non_blank_QUESTIONS);

      /* Score */
      fprintf (Gbl.F.Out,"<tr>"
			 "<td class=\"DAT_N RIGHT_TOP\">"
			 "%s:"
			 "</td>"
			 "<td class=\"DAT LEFT_TOP\">",
	       Txt_Score);
      if (ICanViewScore)
	 fprintf (Gbl.F.Out,"%.2lf (%.2lf",
		  TotalScore,
		  Gbl.Test.NumQsts ? TotalScore * Tst_SCORE_MAX / (double) Gbl.Test.NumQsts :
				     0.0);
      else
	 fprintf (Gbl.F.Out,"? (?");	// No feedback
      fprintf (Gbl.F.Out," %s %u)</td>"
			 "</tr>",
	       Txt_out_of_PART_OF_A_SCORE,Tst_SCORE_MAX);

      /* Tags present in this test */
      fprintf (Gbl.F.Out,"<tr>"
			 "<td class=\"DAT_N RIGHT_TOP\">"
			 "%s:"
			 "</td>"
			 "<td class=\"DAT LEFT_TOP\">",
	       Txt_Tags);
      Tst_ShowTstTagsPresentInATestResult (TstCod);
      fprintf (Gbl.F.Out,"</td>"
			 "</tr>");

      /***** Write answers and solutions *****/
      Tst_ShowTestResult (TstTimeUTC);

      /***** Write total mark of test *****/
      if (ICanViewScore)
	 Tst_ShowTstTotalMark (TotalScore);

      /***** End table *****/
      Tbl_EndTable ();

      /***** End box *****/
      Box_EndBox ();
     }
   else	// I am not allowed to view this test result
      Lay_ShowErrorAndExit ("You can not view this test result.");
  }

/*****************************************************************************/
/************************* Show the result of a test *************************/
/*****************************************************************************/

static void Tst_ShowTestResult (time_t TstTimeUTC)
  {
   extern const char *Txt_Question_modified;
   extern const char *Txt_Question_removed;
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumQst;
   long QstCod;
   double ScoreThisQst;
   bool AnswerIsNotBlank;
   bool ThisQuestionHasBeenEdited;
   time_t EditTimeUTC;

   for (NumQst = 0;
	NumQst < Gbl.Test.NumQsts;
	NumQst++)
     {
      Gbl.RowEvenOdd = NumQst % 2;

      /***** Query database *****/
      if (Tst_GetOneQuestionByCod (Gbl.Test.QstCodes[NumQst],&mysql_res))	// Question exists
	{
	 /***** Get row of the result of the query *****/
	 row = mysql_fetch_row (mysql_res);
	 /*
	 row[0] QstCod
	 row[1] UNIX_TIMESTAMP(EditTime)
	 row[2] AnsType
	 row[3] Shuffle
	 row[4] Stem
	 row[5] Feedback
	 row[6] MedCod
	 row[7] NumHits
	 row[8] NumHitsNotBlank
	 row[9] Score
	 */
	 /***** If this question has been edited later than test time
	        ==> don't show question ****/
	 EditTimeUTC = Dat_GetUNIXTimeFromStr (row[1]);
	 ThisQuestionHasBeenEdited = false;
	 // if (TstTimeUTC)
	    if (EditTimeUTC > TstTimeUTC)
	       ThisQuestionHasBeenEdited = true;

	 if (ThisQuestionHasBeenEdited)
	    /***** Question has been edited *****/
	    fprintf (Gbl.F.Out,"<tr>"
			       "<td class=\"BIG_INDEX RIGHT_TOP COLOR%u\">"
			       "%u"
			       "</td>"
			       "<td class=\"DAT_LIGHT LEFT_TOP COLOR%u\">"
			       "%s"
			       "</td>"
			       "</tr>",
		     Gbl.RowEvenOdd,NumQst + 1,
		     Gbl.RowEvenOdd,Txt_Question_modified);
	 else
	   {
	    /***** Get the code of question (row[0]) *****/
	    if ((QstCod = Str_ConvertStrCodToLongCod (row[0])) < 0)
	       Lay_ShowErrorAndExit ("Wrong code of question.");

	    /***** Write questions and answers *****/
	    Tst_WriteQstAndAnsTest (Tst_SHOW_TEST_RESULT,
	                            &Gbl.Usrs.Other.UsrDat,
				    NULL,NumQst,QstCod,row,
				    &ScoreThisQst,	// Not used here
				    &AnswerIsNotBlank);	// Not used here
	   }
	}
      else
	 /***** Question does not exists *****/
         fprintf (Gbl.F.Out,"<tr>"
	                    "<td class=\"BIG_INDEX RIGHT_TOP COLOR%u\">"
	                    "%u"
	                    "</td>"
	                    "<td class=\"DAT_LIGHT LEFT_TOP COLOR%u\">"
	                    "%s"
	                    "</td>"
	                    "</tr>",
                  Gbl.RowEvenOdd,NumQst + 1,
                  Gbl.RowEvenOdd,Txt_Question_removed);

      /***** Free structure that stores the query result *****/
      DB_FreeMySQLResult (&mysql_res);
     }
  }

/*****************************************************************************/
/********* Get data of a test result using its test result code **************/
/*****************************************************************************/

static void Tst_GetTestResultDataByTstCod (long TstCod,time_t *TstTimeUTC,
                                           unsigned *NumQsts,unsigned *NumQstsNotBlank,double *Score)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;

   /***** Make database query *****/
   if (DB_QuerySELECT (&mysql_res,"can not get data"
				  " of a test result of a user",
		       "SELECT UsrCod,"				// row[0]
		              "AllowTeachers,"			// row[1]
			      "UNIX_TIMESTAMP(TstTime),"	// row[2]
		              "NumQsts,"			// row[3]
		              "NumQstsNotBlank,"		// row[4]
		              "Score"				// row[5]
		       " FROM tst_exams"
		       " WHERE TstCod=%ld AND CrsCod=%ld",
		       TstCod,
		       Gbl.Hierarchy.Crs.CrsCod) == 1)
     {
      row = mysql_fetch_row (mysql_res);

      /* Get user code (row[0]) */
      Gbl.Usrs.Other.UsrDat.UsrCod = Str_ConvertStrCodToLongCod (row[0]);

      /* Get if teachers are allowed to see this test result (row[1]) */
      Gbl.Test.AllowTeachers = (row[1][0] == 'Y');

      /* Get date-time (row[2] holds UTC date-time) */
      *TstTimeUTC = Dat_GetUNIXTimeFromStr (row[2]);

      /* Get number of questions (row[3]) */
      if (sscanf (row[3],"%u",NumQsts) != 1)
	 *NumQsts = 0;

      /* Get number of questions not blank (row[4]) */
      if (sscanf (row[4],"%u",NumQstsNotBlank) != 1)
	 *NumQstsNotBlank = 0;

      /* Get score (row[5]) */
      Str_SetDecimalPointToUS ();	// To get the decimal point as a dot
      if (sscanf (row[5],"%lf",Score) != 1)
	 *Score = 0.0;
      Str_SetDecimalPointToLocal ();	// Return to local system
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/************ Store user's answers of an test result into database ***********/
/*****************************************************************************/

static void Tst_StoreOneTestResultQstInDB (long TstCod,long QstCod,unsigned NumQst,double Score)
  {
   char Indexes[Tst_MAX_BYTES_INDEXES_ONE_QST + 1];
   char Answers[Tst_MAX_BYTES_ANSWERS_ONE_QST + 1];

   /***** Replace each separator of multiple parameters by a comma *****/
   /* In database commas are used as separators instead of special chars */
   Par_ReplaceSeparatorMultipleByComma (Gbl.Test.StrIndexesOneQst[NumQst],Indexes);
   Par_ReplaceSeparatorMultipleByComma (Gbl.Test.StrAnswersOneQst[NumQst],Answers);

   /***** Insert question and user's answers into database *****/
   Str_SetDecimalPointToUS ();	// To print the floating point as a dot
   DB_QueryINSERT ("can not insert a question of a test result",
		   "INSERT INTO tst_exam_questions"
		   " (TstCod,QstCod,QstInd,Score,Indexes,Answers)"
		   " VALUES"
		   " (%ld,%ld,%u,'%lf','%s','%s')",
		   TstCod,QstCod,
		   NumQst,	// 0, 1, 2, 3...
		   Score,
		   Indexes,
		   Answers);
   Str_SetDecimalPointToLocal ();	// Return to local system
  }

/*****************************************************************************/
/************ Get the questions of a test result from database ***************/
/*****************************************************************************/

static void Tst_GetTestResultQuestionsFromDB (long TstCod)
  {
   MYSQL_RES *mysql_res;
   MYSQL_ROW row;
   unsigned NumQst;

   /***** Get questions of a test result from database *****/
   Gbl.Test.NumQsts =
   (unsigned) DB_QuerySELECT (&mysql_res,"can not get questions"
					 " of a test result",
			      "SELECT QstCod,"	// row[0]
			             "Indexes,"	// row[1]
			             "Answers"	// row[2]
			      " FROM tst_exam_questions"
			      " WHERE TstCod=%ld ORDER BY QstInd",
			      TstCod);

   /***** Get questions codes *****/
   for (NumQst = 0;
	NumQst < Gbl.Test.NumQsts;
	NumQst++)
     {
      row = mysql_fetch_row (mysql_res);

      /* Get question code */
      if ((Gbl.Test.QstCodes[NumQst] = Str_ConvertStrCodToLongCod (row[0])) < 0)
	 Lay_ShowErrorAndExit ("Wrong code of question.");

      /* Get indexes for this question (row[1]) */
      Str_Copy (Gbl.Test.StrIndexesOneQst[NumQst],row[1],
                Tst_MAX_BYTES_INDEXES_ONE_QST);

      /* Get answers selected by user for this question (row[2]) */
      Str_Copy (Gbl.Test.StrAnswersOneQst[NumQst],row[2],
                Tst_MAX_BYTES_ANSWERS_ONE_QST);

      /* Replace each comma by a separator of multiple parameters */
      /* In database commas are used as separators instead of special chars */
      Par_ReplaceCommaBySeparatorMultiple (Gbl.Test.StrIndexesOneQst[NumQst]);
      Par_ReplaceCommaBySeparatorMultiple (Gbl.Test.StrAnswersOneQst[NumQst]);
     }

   /***** Free structure that stores the query result *****/
   DB_FreeMySQLResult (&mysql_res);
  }

/*****************************************************************************/
/********************** Remove test results made by a user ********************/
/*****************************************************************************/

void Tst_RemoveTestResultsMadeByUsrInAllCrss (long UsrCod)
  {
   /***** Remove test results made by the specified user *****/
   DB_QueryDELETE ("can not remove test results made by a user",
		   "DELETE FROM tst_exam_questions"
	           " USING tst_exams,tst_exam_questions"
                   " WHERE tst_exams.UsrCod=%ld"
                   " AND tst_exams.TstCod=tst_exam_questions.TstCod",
		   UsrCod);

   DB_QueryDELETE ("can not remove test results made by a user",
		   "DELETE FROM tst_exams"
	           " WHERE UsrCod=%ld",
		   UsrCod);
  }

/*****************************************************************************/
/************** Remove test results made by a user in a course ***************/
/*****************************************************************************/

void Tst_RemoveTestResultsMadeByUsrInCrs (long UsrCod,long CrsCod)
  {
   /***** Remove test results made by the specified user *****/
   DB_QueryDELETE ("can not remove test results made by a user in a course",
		   "DELETE FROM tst_exam_questions"
	           " USING tst_exams,tst_exam_questions"
                   " WHERE tst_exams.CrsCod=%ld AND tst_exams.UsrCod=%ld"
                   " AND tst_exams.TstCod=tst_exam_questions.TstCod",
		   CrsCod,UsrCod);

   DB_QueryDELETE ("can not remove test results made by a user in a course",
		   "DELETE FROM tst_exams"
	           " WHERE CrsCod=%ld AND UsrCod=%ld",
		   CrsCod,UsrCod);
  }

/*****************************************************************************/
/****************** Remove all test results made in a course *****************/
/*****************************************************************************/

void Tst_RemoveCrsTestResults (long CrsCod)
  {
   /***** Remove questions of test results made in the course *****/
   DB_QueryDELETE ("can not remove test results made in a course",
		   "DELETE FROM tst_exam_questions"
	           " USING tst_exams,tst_exam_questions"
                   " WHERE tst_exams.CrsCod=%ld"
                   " AND tst_exams.TstCod=tst_exam_questions.TstCod",
		   CrsCod);

   /***** Remove test results made in the course *****/
   DB_QueryDELETE ("can not remove test results made in a course",
		   "DELETE FROM tst_exams WHERE CrsCod=%ld",
		   CrsCod);
  }

/*****************************************************************************/
/******************* Remove all test exams made in a course ******************/
/*****************************************************************************/

void Tst_RemoveCrsTests (long CrsCod)
  {
   /***** Remove tests status in the course *****/
   DB_QueryDELETE ("can not remove status of tests of a course",
		   "DELETE FROM tst_status WHERE CrsCod=%ld",
		   CrsCod);

   /***** Remove test configuration of the course *****/
   DB_QueryDELETE ("can not remove configuration of tests of a course",
		   "DELETE FROM tst_config WHERE CrsCod=%ld",
		   CrsCod);

   /***** Remove associations between test questions
          and test tags in the course *****/
   DB_QueryDELETE ("can not remove tags associated"
		   " to questions of tests of a course",
		   "DELETE FROM tst_question_tags"
	           " USING tst_questions,tst_question_tags"
                   " WHERE tst_questions.CrsCod=%ld"
                   " AND tst_questions.QstCod=tst_question_tags.QstCod",
		   CrsCod);

   /***** Remove test tags in the course *****/
   DB_QueryDELETE ("can not remove tags of test of a course",
		   "DELETE FROM tst_tags WHERE CrsCod=%ld",
		   CrsCod);

   /***** Remove test answers in the course *****/
   DB_QueryDELETE ("can not remove answers of tests of a course",
		   "DELETE FROM tst_answers USING tst_questions,tst_answers"
                   " WHERE tst_questions.CrsCod=%ld"
                   " AND tst_questions.QstCod=tst_answers.QstCod",
		   CrsCod);

   /***** Remove media associated to test questions in the course *****/
   Tst_RemoveAllMedFilesFromAnsOfAllQstsInCrs (CrsCod);
   Tst_RemoveAllMedFilesFromStemOfAllQstsInCrs (CrsCod);

   /***** Remove test questions in the course *****/
   DB_QueryDELETE ("can not remove test questions of a course",
		   "DELETE FROM tst_questions WHERE CrsCod=%ld",
		   CrsCod);
  }
