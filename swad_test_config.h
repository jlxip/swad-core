// swad_test_config.h: self-assessment tests configuration

#ifndef _SWAD_TST_CFG
#define _SWAD_TST_CFG
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
/********************************* Headers ***********************************/
/*****************************************************************************/

#include <mysql/mysql.h>	// To access MySQL databases
#include <stdbool.h>		// For boolean type

/*****************************************************************************/
/***************************** Public constants ******************************/
/*****************************************************************************/

#define TstCfg_MAX_QUESTIONS_PER_TEST	100	// Absolute maximum number of questions in a test

#define TstCfg_DEFAULT_MIN_QUESTIONS	  1
#define TstCfg_DEFAULT_DEF_QUESTIONS	 20	// Number of questions to be generated by default in a self-assessment test
#define TstCfg_DEFAULT_MAX_QUESTIONS	 30	// Maximum number of questions to be generated in a self-assessment test

/*****************************************************************************/
/******************************* Public types ********************************/
/*****************************************************************************/

#define TstCfg_NUM_OPTIONS_PLUGGABLE	3
typedef enum
  {
   TstCfg_PLUGGABLE_UNKNOWN = 0,
   TstCfg_PLUGGABLE_NO      = 1,
   TstCfg_PLUGGABLE_YES     = 2,
  } TstCfg_Pluggable_t;

/*****************************************************************************/
/***************************** Public prototypes *****************************/
/*****************************************************************************/

void TstCfg_GetConfigFromDB (void);

void TstCfg_GetConfigFromRow (MYSQL_ROW row);
void TstCfg_ReceiveConfigTst (void);

void TstCfg_SetConfigPluggable (TstCfg_Pluggable_t Pluggable);
TstCfg_Pluggable_t TstCfg_GetConfigPluggable (void);
void TstCfg_SetConfigMin (unsigned Min);
unsigned TstCfg_GetConfigMin (void);
void TstCfg_SetConfigDef (unsigned Def);
unsigned TstCfg_GetConfigDef (void);
void TstCfg_SetConfigMax (unsigned Max);
unsigned TstCfg_GetConfigMax (void);
void TstCfg_SetConfigMinTimeNxtTstPerQst (unsigned long MinTimeNxtTstPerQst);
unsigned long TstCfg_GetConfigMinTimeNxtTstPerQst (void);
void TstCfg_SetConfigVisibility (unsigned Visibility);
unsigned TstCfg_GetConfigVisibility (void);

#endif
