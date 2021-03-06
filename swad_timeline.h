// swad_timeline.c: social timeline

#ifndef _SWAD_TL
#define _SWAD_TL
/*
    SWAD (Shared Workspace At a Distance in Spanish),
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

/*****************************************************************************/
/****************************** Public constants *****************************/
/*****************************************************************************/

#define TL_TIMELINE_SECTION_ID	"timeline"

// Number of recent publishings got and shown the first time, before refreshing
#define TL_MAX_NEW_PUBS_TO_GET_AND_SHOW	10000	// Unlimited
#define TL_MAX_REC_PUBS_TO_GET_AND_SHOW	   10	// Recent publishings to show (first time)
#define TL_MAX_OLD_PUBS_TO_GET_AND_SHOW	   20	// Old publishings are retrieved in packs of this size

/*****************************************************************************/
/******************************** Public types *******************************/
/*****************************************************************************/

#define TL_DEFAULT_WHO	Usr_WHO_FOLLOWED

#define TL_NUM_NOTE_TYPES	13
// If the numbers assigned to each event type change,
// it is necessary to change old numbers to new ones in database table tl_notes
typedef enum
  {
   TL_NOTE_UNKNOWN		=  0,
   /* Start tab */
   TL_NOTE_POST			= 10,	// Post written directly in timeline
   /* Institution tab */
   TL_NOTE_INS_DOC_PUB_FILE	=  1,	// Public file in documents of institution
   TL_NOTE_INS_SHA_PUB_FILE	=  2,	// Public file in shared files of institution
   /* Centre tab */
   TL_NOTE_CTR_DOC_PUB_FILE	=  3,	// Public file in documents of centre
   TL_NOTE_CTR_SHA_PUB_FILE	=  4,	// Public file in shared files of centre
   /* Degree tab */
   TL_NOTE_DEG_DOC_PUB_FILE	=  5,	// Public file in documents of degree
   TL_NOTE_DEG_SHA_PUB_FILE	=  6,	// Public file in shared files of degree
   /* Course tab */
   TL_NOTE_CRS_DOC_PUB_FILE	=  7,	// Public file in documents of course
   TL_NOTE_CRS_SHA_PUB_FILE	=  8,	// Public file in shared files of course
   /* Assessment tab */
   TL_NOTE_EXAM_ANNOUNCEMENT	=  9,	// Exam announcement in a course
   /* Users tab */
   /* Messages tab */
   TL_NOTE_NOTICE		= 12,	// A public notice in a course
   TL_NOTE_FORUM_POST		= 11,	// Post in global/swad forums
   /* Analytics tab */
   /* Profile tab */
  } TL_NoteType_t;

#define TL_NUM_PUB_TYPES	4
// If the numbers assigned to each event type change,
// it is necessary to change old numbers to new ones in database table tl_notes
typedef enum
  {
   TL_PUB_UNKNOWN		= 0,
   TL_PUB_ORIGINAL_NOTE		= 1,
   TL_PUB_SHARED_NOTE		= 2,
   TL_PUB_COMMENT_TO_NOTE	= 3,
  } TL_PubType_t;

#define TL_NUM_TOP_MESSAGES (1 + 6)
typedef enum
  {
   TL_TOP_MESSAGE_NONE		= 0,
   TL_TOP_MESSAGE_COMMENTED	= 1,
   TL_TOP_MESSAGE_FAVED		= 2,
   TL_TOP_MESSAGE_UNFAVED	= 3,
   TL_TOP_MESSAGE_SHARED	= 4,
   TL_TOP_MESSAGE_UNSHARED	= 5,
   TL_TOP_MESSAGE_MENTIONED	= 6,
  } TL_TopMessage_t;

struct TL_Publication
  {
   long PubCod;
   long NotCod;
   long PublisherCod;		// Sharer or writer of the publication
   TL_PubType_t PubType;
   time_t DateTimeUTC;
   TL_TopMessage_t TopMessage;	// Used to show feedback on the action made
  };

struct TL_Timeline
  {
   Usr_Who_t Who;
   long NotCod;		// Used as parameter about social note to be edited, removed...
   long PubCod;		// Used as parameter about social publishing to be edited, removed...
  };

/*****************************************************************************/
/****************************** Public prototypes ****************************/
/*****************************************************************************/

void TL_ResetTimeline (struct TL_Timeline *Timeline);
void TL_ShowTimelineGbl (void);

void TL_ShowTimelineUsr (struct TL_Timeline *Timeline);

void TL_RefreshNewTimelineGbl (void);

void TL_RefreshOldTimelineGbl (void);
void TL_RefreshOldTimelineUsr (void);

void TL_MarkMyNotifAsSeen (void);

void TL_GetParamWho (void);
Usr_Who_t TL_GetGlobalWho (void);

void TL_StoreAndPublishNote (TL_NoteType_t NoteType,long Cod,struct TL_Publication *SocPub);
void TL_MarkNoteAsUnavailableUsingNotCod (long NotCod);
void TL_MarkNoteAsUnavailableUsingNoteTypeAndCod (TL_NoteType_t NoteType,long Cod);
void TL_MarkNoteOneFileAsUnavailable (const char *Path);
void TL_MarkNotesChildrenOfFolderAsUnavailable (const char *Path);

void TL_ReceivePostUsr (void);
void TL_ReceivePostGbl (void);

void TL_ShowHiddenCommentsUsr (void);
void TL_ShowHiddenCommentsGbl (void);

void TL_PutHiddenParamPubCod (long PubCod);

void TL_ReceiveCommentUsr (void);
void TL_ReceiveCommentGbl (void);

void TL_ShowAllSharersNoteUsr (void);
void TL_ShowAllSharersNoteGbl (void);
void TL_ShaNoteUsr (void);
void TL_ShaNoteGbl (void);
void TL_UnsNoteUsr (void);
void TL_UnsNoteGbl (void);

void TL_ShowAllFaversNoteUsr (void);
void TL_ShowAllFaversNoteGbl (void);
void TL_FavNoteUsr (void);
void TL_FavNoteGbl (void);
void TL_UnfNoteUsr (void);
void TL_UnfNoteGbl (void);

void TL_ShowAllFaversComUsr (void);
void TL_ShowAllFaversComGbl (void);
void TL_FavCommentUsr (void);
void TL_FavCommentGbl (void);
void TL_UnfCommentUsr (void);
void TL_UnfCommentGbl (void);

void TL_RequestRemNoteUsr (void);
void TL_RequestRemNoteGbl (void);
void TL_RemoveNoteUsr (void);
void TL_RemoveNoteGbl (void);

void TL_RequestRemComUsr (void);
void TL_RequestRemComGbl (void);
void TL_RemoveComUsr (void);
void TL_RemoveComGbl (void);

void TL_RemoveUsrContent (long UsrCod);

void TL_ClearOldTimelinesDB (void);

void TL_GetNotifPublication (char SummaryStr[Ntf_MAX_BYTES_SUMMARY + 1],
                             char **ContentStr,
                             long PubCod,bool GetContent);

unsigned long TL_GetNumPubsUsr (long UsrCod);

#endif
