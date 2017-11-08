// swad_changelog.h: changelog

#ifndef _SWAD_CHG
#define _SWAD_CHG
/*
    SWAD (Shared Workspace At a Distance),
    is a web platform developed at the University of Granada (Spain),
    and used to support university teaching.

    This file is part of SWAD core.
    Copyright (C) 1999-2017 Antonio Ca�as Vargas

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
/*********************************** TODO ************************************/
/*****************************************************************************/

// TODO: Is it necessary FileType in table "files"?
// TODO: When notifications are marked as seen, do it in a priori function
// TODO: Use the library http://fukuchi.org/works/qrencode/index.html.en instead Google QR
// TODO: [Ceferino Bustos Valdivia]:
//	 Enhorabuena por esta nueva aplicaci�n. La he utilizado con los c�digos QR y va muy bien.
//	 Me genera las listas de alumnos y es bastante r�pida la lectura del c�digo QR.
//	 Ser�a completa esta aplicaci�n, si la asistencia se incluyera, autom�ticamente, en el apartado de evaluaci�n de cada alumno. De esta forma podr�amos ver sus trabajos y asistencias de forma personalizada.
// TODO: OpenSWAD.org: �Qu� deber�a tener una buena gesti�n de grupos? Es para tomar ideas ;)
//       Jos� Ra�l Canay @raulcanay: por ejemplo, que puedas sacar un listado exportable de en qu� grupos est� una persona de quienes est�n en un grupo.
/* Propuestas de Fidel Rodr�guez L�pez:

==========================================================
1  @@ TUTORIAS GRUPALES/INDIVIDUALES "VIRTUALES"
==========================================================
        # Permite realizar grupos de chat, de uno o varios profesores con sus alumnos.
        �til para la resoluci�n de ejercicios y dudas sencillas.
        # Notificaci�n, citas y aviso de tutorias virtuales.
        # Permitir el env�o de im�genes y fotos a trav�s de la sala de Chat.s
==========================================================
2  @@ MEJORA DEL SISTEMA DE ANOTACI�N DE ASISTENCIA
==========================================================
        # Sistema din�mico que permita la anotaci�n de asistencia de los alumnos a las
        clases pr�cticas y que facilite la administraci�n de los mismos (notificaci�n al
        profesor cuando todos los alumnos sean registrados, por ejemplo). Implementaci�n
        v�a AJAX para ser m�s agradable.
==========================================================
3  @@ GENERADOR DE CALIFICACIONES EN PDF
==========================================================
        # El servidor es capaz de generar un PDF para cualquier tarea (por ejemplo, con
        las calificaciones de los alumnos para publicar en el tabl�n, con las convocatorias
        de examen...). Para esto se puede usar alguna libreria libre tipo FPDF.
        # Algunos profesores prefieren tener en papel anotadas las calificaciones.
==========================================================
4  @@ TUTORIAL INICIACI�N
==========================================================
        # Es importante indicar tanto al profesor, Administrador y alumno de qu� es capaz
        SWAD, para poder as� explotar todas sus posibilidades. Un tutorial amigable e inicial
        a base de Tooltips o im�genes, puede ser bastante �til.
==========================================================
5  @@ MEJORA DE LA INTERFAZ
==========================================================
        # Botones, colores e iconos m�s modernos. Los iconos se pueden suprimir por iconos en
        fuentes (como font-awesome, open-source). Los botones en CSS3 le dar�an un aire m�s
        moderno y renovado (se podr�a basar en alg�n framework open-source).
==========================================================
6  @@ INCORPORACI�N DE UN TABL�N DE PROGRAMACI�N DE ASIGNATURA
==========================================================
        # Hacer peque�as modificaciones para ver la zona de Actividades de otra forma. Por ejemplo,
        una estructura de �rbol (como la zona de Documentos) pero con las tareas programadas para la
        semana proporcionar�a una visi�n m�s global (como una "agenda") de lo que hay que realizar.
        # El tabl�n incorporar�a solo los t�tulos de los ejercicios. Por ejemplo:

        ==>Semana 16 Febr. -> 22 Marzo
        ----------Pr�cticas
        ----------------Entrega Pr�ctica 2 [Fecha L�mite] (Color verde o rojo, si ha pasado la fecha, como actualmente en Actividades)
        ----------Teor�a
        ----------------Tema 2
        ----------Otros
        ----------------Entregar Relaci�n de Ejercicios
        ==>Semana 23 Febr.-> 1 Marzo
        ----------Pr�cticas
        ----------Teor�a
        ----------------Examen Tema 2
        ----------Otros
        ....Etc.....
*/
// TODO: A teacher should may confirm a student ID? In what conditions? (Necessary in order to a student can view his/her marks)
// TODO: Put headers Content-type and Content-disposition when redirecting with Location:
// TODO: When a new assignment/attendance/survey is incorrect, the second time the form is shown, it should be filled with partial data, now is always empty
// TODO: Show message indicating that mail could be in SPAM folder?
// TODO: Show nicknames of users in lists?
// TODO: Form to register and remove system admins
// TODO: FIX BUG: when an admin clicks on "Use this email" in the email of another user the email was confirmed, the email becomes unconfirmed
// TODO: When error ("Ya exist�a un evento con el t�tulo") creating a new event, the data of the event is empty
// TODO: When teacher clicks in attendance of a student (button in her/his record),
//	 only (the groups he/she should have attend) union (the groups he/she really attended)
//	 should be shown.
// TODO: When page is refreshed in course works, prevent users to be duplicated
// TODO: Reply to all
// TODO: Hour in exam announcement should start at six a.m.
// TODO: Forum SWAD should be always named "SWAD"?
// TODO: Enable chat for guests?
// TODO: Go to forum post (or at least to forum thread) from social timeline and notifications?
// TODO: Width of column for data in notifications is too short
// TODO: Change size of DegTypName in table deg_types from VARCHAR(32) TO VARCHAR(255)
// TODO: Insert "http://" to WWW when WWW does not start with "*://"
// TODO: To avoid wrong email addresses, when a user fills his/her email address, check if the domain is in the white list of allowed domains. If not, ask for confirmation.
// TODO: Filtering email addresses --> an email address can not finish in "."
// TODO: Change PhotoAttribution in table centres from TEXT to VARCHAR(255) (check maximum length first)
// TODO: In social refreshing via AJAX, an error occurs when session expirates
// TODO: Messages in msg_content_deleted older than a certain time should be deleted to ensure the protection of personal data
// TODO: FIX BUG: A teacher uploads a document in course documents zone, then he/she unregister from course, then he/she search for his/her documents, a document is shown in results but he/she can not view it
// TODO: Add Stack Exchange to webs/networks
// TODO: Optimize slow query searching messages received
// TODO: FIX BUG: Searching messages received gives unordered list

// TODO: In search of users and findUsers banned users should not appear in listing!!!!!

// TODO: Send attached photos via SOAP?
// TODO: Upload photos/files from SWADroid using MIME attachments

// TODO: If a user creates a request for inscription, and the user is removed ==> mark possible notifications as removed

// TODO: When SWAD prompts the user to confirm elimination of a group, list the groups below the button

// TODO: Send a message to the user when rejecting a request for enrolment. Suggested by Javier Fern�ndez Baldomero.
// TODO: Create a table with IPs to block temporarily multiple access to the action used to check ID for empty accounts
// TODO: Every action should have a minimum time between two clicks on it (for example "Generate a report" should have 60 s)

// TODO: Generar informe -> �Realmente desea generar un informe... (puede tardar varios segundos...)? -> Generar informe.

// TODO: Writing a message to several recipients: include message to indicate that list of nicknames must be comma separated
// TODO: When requesting inscription, I can not click in the photo of my record. Change the form?

// TODO: Import ID, email, Surname1, Surname2, Name from CSV, suggested by Javier Melero

// TODO: Remove columns DegCod,CrsCod from database table surveys

// TODO: STATS > Figures > Folders and files --> statistics about documents and shared in institutions, centres and degrees

// TODO: When an assignment has the same name or the same folder as another existing assignment, after error the fields are empty!

// TODO: Remove all files inside the root folder (except the root folder itself) just as made with copy-paste?

// TODO: Cuando se seleccionan varios alumnos para sus trabajos y despu�s se vuelve atr�s y se vuelven a ver, salen los estudiantes duplicados ==> comprobar cada uno de los que se muestra si ya se ha mostrado para no hacerlo de nuevo

// TODO: Comprobar que cuando se da de baja un estudiante y luego se vuelve a dar de alta se han eliminado sus trabajos (Javier dice que no se eliminan)

// TODO: Hay un problema cuando se eliminan usuarios de una asignatura: quedan sus trabajos sin eliminar para siempre, ocupando espacio
// TODO: Se elimina un �nico estudiante de la asignatura. �Se eliminan sus trabajos? Si no, habr�a que cambiar el mensaje "eliminando sus trabajos...".

// TODO: Los administradores deber�an ver el DNI de los usuarios no confirmados?

// TODO: Default type of degree should be "Other" or empty by default

// TODO: Change forms to change my webs / social networks to one unique form with a "Save changes" button

// TODO: In USERS > Attendance > Select event, if the list of users is big, the user should be requested to see big list in the same way that it is made in other options

// TODO: List only confirmed emails to send a message with MESSAGES > Email ?

// TODO: Add SWAD API to https://www.programmableweb.com/add/api

// TODO: When activating folders in groups, go to section for that type of group
// TODO: I can not remove a folder inside assignments (see swad.ugr.es -> EC -> assignment acanas)

// TODO: Fix bug in notification content about files sent to plugins like SWADroid: do not write internal name (for example "comun"). Call instead Brw_GetFileNameToShow or similar.

// TODO: Draw future dates in attendance, surveys, assignments in blue?
// TODO: Fix bug in generate a test: after entering a number of questions with empty tags and type of answers, number of questions is not remembered
// TODO: Fix bug when editing a test question with images: when "Change image" is selected but no image is uploades, other images (for example in answers) are lost
// TODO: Fix bug when creating a new attendance event: if title is repeated, form is cleared
// TODO: Fix bug: When registering an administrator, the user's name changes are ignored

// TODO: In list of indicators, alternate background colors also when not all the number of indicators are selected

// TODO: Ver soluci�n de Guillermo G�mez Trenado cuando se pulsa con el bot�n derecho para abrir una descarga en otra ventana

// TODO: En los detalles de asistencia deber�a salir la fecha de cada sesi�n
// TODO: System admin should view link "Visits to course"
// TODO: Un administrador de instituci�n, �deber�a poder cambiar la contrase�a de un usuario de esa instituci�n? No lo tengo claro.

// TODO: Subject in forum_post should be VARCHAR(255) in order to be copied directly to SummaryStr
// TODO: Add a Subject (VARCHAR(255)) to notices to be used as SummaryStr
// TODO: Subject in msg_content and msg_content_deleted should be VARCHAR(255) in order to be copied directly to SummaryStr
// TODO: Check how to get a summary of a social post or comments

// TODO: Check default values for ImageName, ImageTitle and ImageURL in database tables

// TODO: Cuando hay un fallo al rellenar el formulario de nuevo evento en la agenda se borran los datos
// TODO: Comprobar cabecera de una orla cuando el �mbito actual es un centro
// TODO: Include type of degree in form of degree configuration

// TODO: Cuando un admin. pulsa en "Inscribir profesor/a" y el usuario es invitado, deber�a salir en el rol por defecto "profesor" y no "estudiante"
// TODO: Tras inscribir a un profesor en una asignatura, nos vamos a otra pesta�a, luego regresamos a USUARIOS y la opci�n por defecto es Otros, cuando deber�a ser Profesores.

// TODO: Comprobar que, para detectar el n�mero de filas de cabecera y pie autom�ticamente en los archivos de calificaciones, SWAD mira no s�lo que parezcan un ID, sino que coincida con alguno de los DNI de la asignatura (por ejemplo en el caso de los DNI alemanes formados por varias letras)

// TODO: En informaci�n de un centro, poner icono que enlace a edici�n de lugares, y posibilidad de editar lugar del centro en un men� desplegable. Lugar deber�a ir encima de Acceso directo.

// TODO: Fix bug: En Compartidos, si se borra la posibilidad de carpetas en un grupo ==> sale mensaje "The group has no file zones."

// TODO: Change numbers of notifications in program and database to match order in tabs

// TODO: Sugerencias pendientes de Sandra Daniela Tazzioli Barroso realizadas en mayo de 2017:
// - Mejor ayuda sobre c�mo pasar de Excel al formato HTML necesario para las calificaciones.
// - Puede ser interesante que un administrador pueda cambiar el DNI de un usuario.
// - Comprobar que las direcciones de correo de cada uno de los estudiantes aparecen en Bcc: (copia oculta) en cualquier gestor de correo.
// - Informar a otros usuarios de que es el cumplea�os de otros usuarios para que los feliciten (habr�a que a�adir una categor�a de privacidad nueva para la visibilidad de la fecha de cumplea�os).
// - La informaci�n sobre nuevas notificaciones deber�a aparecer tambi�n en pantallas peque�as (m�viles). Tal vez podr�a aparecer tambi�n en el icono de notificaciones de la pesta�a MENSAJES.
// - Hay alumnos que han generado el usuario en forma incompleta entre ellos no cargaron el DNI y no asisten a clase para rectificarlo. Hay forma de mi posicion con profesora o adminsitradora del centro ingresar, completar los datos aun conocer su clave?

// TODO: Fix bug: When a centre is associated with a place and it is moved to another institution, what happens with the place?
// TODO: Check what happens with places when a centre is removed

// TODO: Green lock in documents is not shown when name is shortened (see OpenSWAD -> Creative Commons -> Files -> Documents)

// TODO: Limit length of very big institution name in record card

// TODO: Link names are too short ==> add more length, for example: "A guide to building and understanding the physics of Water Rockets"
// TODO: Fix bug: Error when a link end in a dot. Example: "A guide to building..." --> "A guide to building._..url" (two dots)

// TODO: Cuando s�lo se cambian los grupos y no el rol de un profesor ya existente, no sale ning�n mensaje. se haga lo que se haga en la edici�n deber�a salir un mensaje del tipo "Cambios realizados"

// TODO: Deber�a haber un modo "ver" en las actividades, como en los documentos, para que el profesor tenga la visi�n del estudiante.

// TODO: "Solicitar inscripci�n" como superusuario: "Usted no tiene permiso para realizar esta acci�n"

// TODO: Poner icono con enlace a d�as festivos en calendario de centro, titulaci�n y asignatura, adem�s del actual calendario de instituci�n

// TODO: Cuando vamos a inscribir a un nuevo usuario en la asignatura,
// debe salir por defecto estudiante en la opci�n Estudiantes y profesor en la opci�n Profesores,
// pero si el usuario ya existe en la asignatura, debe salir siempre el rol actual
// (para no cambiarlo sin querer si lo �nico que queremos hacer es cambiar la inscripci�n a grupos)

// TODO: No se ve bien la lista de usuarios conectados (en Usuarios > Conectados). El ancho de la tabla no es correcto debido a la fecha demasiado ancha (por ej. los mi�rcoles)
// TODO: Fix bug: Un superusuario se apunta a grupos, cuando pulsa "Cambiar de grupos" no ocurre nada.

// TODO: "Administrar varios profesores no editores" -> deber�a poder hacerlo un profesor (Perico)

// TODO: Sugerencia del profesor Crist�bal Lozano por correo del 3/11/2017:
// He detectado este a�o que, al mostrar la lista de los alumnos (p.ej., en varias columnas) en una hoja A4,
// en muchos casos no aparece el nombre/apellido completo del alumno si �ste excede un n� determinado de caracteres.
// Por ejemplo, tengo una alumna llamada 'Mar�a Maldonado', pero s�lo me aparece 'Maldon...'.
// Al tener varios alumnos extranjeros, cuando no me aparece el apellido completo, me es complicado averiguarlo.
// Me gusta tener una lista con sus fotos para ir haciendo anotaciones manuales.
// �Ser�a posible permitir que apareciesen m�s caracteres en sus nombres debajo de la foto?

// TODO: A superuser can not see records of teachers? Why?

/*****************************************************************************/
/****************************** Public constants *****************************/
/*****************************************************************************/

#define Log_PLATFORM_VERSION	"SWAD 17.26 (2017-11-08)"
#define CSS_FILE		"swad17.25.4.css"
#define JS_FILE			"swad17.17.1.js"

// Number of lines (includes comments but not blank lines) has been got with the following command:
// nl swad*.c swad*.h css/swad*.css py/swad*.py js/swad*.js soap/swad*?.h sql/swad*.sql | tail -1
/*
        Version 17.26:    Nov 08, 2017  Tables converted to MyISAM.
					? changes necessary in database:
UPDATE crs_usr SET LastAccTst=FROM_UNIXTIME(0) WHERE LastAccTst=0;
DROP TABLE IF EXISTS deg_admin;
DROP TABLE IF EXISTS imported_groups;
DROP TABLE IF EXISTS imported_sessions;
DROP TABLE IF EXISTS imported_students;
DROP TABLE IF EXISTS log;
DROP TABLE IF EXISTS log_new;
DROP TABLE IF EXISTS new_institutions;
DROP TABLE IF EXISTS notif_backup;
DROP TABLE IF EXISTS sta_notif_backup;
DROP TABLE IF EXISTS timetable_crs_backup;
DROP TABLE IF EXISTS timetable_tut_backup;
DROP TABLE IF EXISTS usr_data_backup;
UPDATE usr_figures SET FirstClickTime=FROM_UNIXTIME(0) WHERE FirstClickTime=0;
UPDATE usr_last SET LastAccNotif=FROM_UNIXTIME(0) WHERE LastAccNotif=0;

        Version 17.25.4:  Oct 24, 2017  Fixed bug in assignments, games, surveys and agenda. (234352 lines)
        Version 17.25.3:  Oct 16, 2017  Surnames before names in listing of projects. Suggested by Pedro Villar Castro. (234351 lines)
        Version 17.25.2:  Oct 11, 2017  Students' IDs removed from listing of projects. Suggested by Pedro Villar Castro.
					Fixed bug in listing of projects. (234345 lines)
        Version 17.25.1:  Oct 10, 2017  Fixed problem in layout of projects. (234357 lines)
        Version 17.25:    Oct 10, 2017  New column with the number of the project in listing of projects. (234336 lines)
        Version 17.24.2:  Oct 10, 2017  Code refactoring in projects filter. (234209 lines)
        Version 17.24.1:  Oct 10, 2017  Changes in pagination of projects. (234228 lines)
        Version 17.24:    Oct 10, 2017  Filter projects by department. (234227 lines)
        Version 17.23.2:  Oct 10, 2017  Code refactoring in filter of projects. (234137 lines)
        Version 17.23.1:  Oct 10, 2017  Changes in selectors of departments. (234133 lines)
        Version 17.23:    Oct 10, 2017  New filter to select preassigned/non-preassigned projects.
					New filter to select hidden/visible projects.
					Code refactoring in parameters of projects. (234140 lines)
        Version 17.22.1:  Oct 08, 2017  Changed commentars on actions. (233756 lines)
        Version 17.22:    Oct 08, 2017  New file browser for project assessment. (233755 lines)
					19 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1716','es','N','Editar eval. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1717','es','N','Subir archivo a eval. proyecto (ant.)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1718','es','N','Eliminar archivo eval. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1719','es','N','Crear carpeta en eval. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1720','es','N','Eliminar carpeta eval. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1721','es','N','Renombrar carpeta en eval. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1722','es','N','Eliminar &aacute;rbol eval. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1723','es','N','Solicitar elim. arch. eval. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1724','es','N','Copiar de eval. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1725','es','N','Pegar en eval. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1726','es','N','Expandir carpeta eval. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1727','es','N','Contraer carpeta eval. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1728','es','N','Solic. nuevo arch/carp eval. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1729','es','N','Ver datos arch. eval. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1730','es','N','Cambiar datos arch. eval. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1731','es','N','Descargar arch. eval. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1732','es','N','Crear ZIP carpeta eval. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1733','es','N','Subir archivo a eval. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1734','es','N','Crear enlace en eval. proyecto');

        Version 17.21.6:  Oct 08, 2017  Changes related with project file browsers. (233315 lines)
        Version 17.21.5:  Oct 08, 2017  Code refactoring in removal of files from database when removing course. (233317 lines)
        Version 17.21.4:  Oct 08, 2017  Fixed bug: remove group files from database when removing course.
					Removed project files from database when removing course.
					Fixed bug in project documents. (233330 lines)
        Version 17.21.3:  Oct 08, 2017  Remove project files from database when removing project. (233169 lines)
        Version 17.21.2:  Oct 08, 2017  Renamed root folder for project documents. (233115 lines)
        Version 17.21.1:  Oct 08, 2017  Remove project directories when removing project. (233114 lines)
        Version 17.21:    Oct 07, 2017  Start and end dates in projects are replaced by time of creation and time of modification. (233106 lines)
					8 changes necessary in database:
ALTER TABLE projects ADD COLUMN CreatTime DATETIME AFTER EndTime;
ALTER TABLE projects ADD COLUMN ModifTime DATETIME AFTER CreatTime;
UPDATE projects SET CreatTime=StartTime;
UPDATE projects SET ModifTime=CreatTime;
ALTER TABLE projects CHANGE COLUMN CreatTime CreatTime DATETIME NOT NULL;
ALTER TABLE projects CHANGE COLUMN ModifTime ModifTime DATETIME NOT NULL;
ALTER TABLE projects ADD INDEX(CrsCod,CreatTime);
ALTER TABLE projects ADD INDEX(CrsCod,ModifTime);
					2 more changes necessary in database:
ALTER TABLE projects DROP COLUMN StartTime;
ALTER TABLE projects DROP COLUMN EndTime;

        Version 17.20.7:  Oct 07, 2017  Only publishers can remove subtrees in project documents. (233105 lines)
        Version 17.20.6:  Oct 07, 2017  Fixed bug in project documents. (233027 lines)
        Version 17.20.5:  Oct 07, 2017  Title of project is now a link to show the project and its documents. (233024 lines)
        Version 17.20.4:  Oct 07, 2017  Changes in layout of project. (232972 lines)
        Version 17.20.3:  Oct 07, 2017  Changes in layout of project.
					Fixed bug when hiding/shown a project. (232971 lines)
        Version 17.20.2:  Oct 07, 2017  Fixed bugs and changes in layout of project file browser. (232960 lines)
        Version 17.20.1:  Oct 07, 2017  Fixed bug in file browser clipboard. (232938 lines)
        Version 17.20:    Oct 06, 2017  New file browser for project documents. Not finished. (232915 lines)
					18 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1698','es','N','Subir archivo a docs. proyecto (ant.)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1699','es','N','Eliminar archivo docs. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1700','es','N','Crear carpeta en docs. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1701','es','N','Eliminar carpeta docs. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1702','es','N','Renombrar carpeta en docs. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1703','es','N','Eliminar �rbol docs. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1704','es','N','Solicitar elim. arch. docs. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1705','es','N','Copiar de docs. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1706','es','N','Pegar en docs. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1707','es','N','Expandir carpeta docs. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1708','es','N','Contraer carpeta docs. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1709','es','N','Solic. nuevo arch/carp docs. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1710','es','N','Ver datos arch. docs. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1711','es','N','Cambiar datos arch. docs. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1712','es','N','Descargar arch. docs. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1713','es','N','Crear ZIP carpeta docs. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1714','es','N','Subir archivo a docs. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1715','es','N','Crear enlace en docs. proyecto');

        Version 17.19:    Oct 06, 2017  New file browser for project documents. Not finished. (232781 lines)
        Version 17.18:    Oct 06, 2017  New view in projects for project file browser. (232547 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1697','es','N','Editar archivos proyecto');

        Version 17.17.5:  Oct 05, 2017	Fixed bug in contextual icons in records. (232467 lines)
        Version 17.17.4:  Oct 05, 2017	Department is filled automatically with user's department when creating a new project. (232385 lines)
        Version 17.17.3:  Oct 04, 2017	Fixed problems with icons when expanding / contracting a folder. (232383 lines)
        Version 17.17.2:  Oct 04, 2017	Changes related with output with status 204 No Content. (232332 lines)
        Version 17.17.1:  Oct 04, 2017	Changed folder icon when expanding / contracting a folder. (232330 lines)
        Version 17.17:    Oct 04, 2017	Changing expansion of file subtrees. (232273 lines)
        Version 17.16.3:  Oct 03, 2017	Changing expansion of file subtrees. Not finished. (232113 lines)
        Version 17.16.2:  Oct 03, 2017	Fixed bug in pagination of projects. (232061 lines)
        Version 17.16.1:  Oct 03, 2017	Changing expansion of file subtrees. Not finished.
					Fixed bug in notifications about new private files. (232060 lines)
        Version 17.16:    Oct 02, 2017	Briefcase moved from profile tab to files tab. (232028 lines)
        Version 17.15:    Oct 02, 2017	Status in projects changed for proposal. (231985 lines)
					4 changes necessary in database:
ALTER TABLE projects ADD COLUMN Proposal ENUM('new','modified','unmodified') NOT NULL DEFAULT 'new' AFTER Status;
UPDATE projects SET Proposal='new' WHERE Status='new';
UPDATE projects SET Proposal='unmodified' WHERE Status='reproposed';
ALTER TABLE projects DROP COLUMN Status;

        Version 17.14:    Oct 02, 2017	New field number of students in projects. (231937 lines)
					1 change necessary in database:
ALTER TABLE projects ADD COLUMN NumStds INT NOT NULL DEFAULT 1 AFTER Preassigned;

        Version 17.13:    Oct 02, 2017	New field status in projects.
					Fixed bug in hidden projects. (231867 lines)
					1 change necessary in database:
ALTER TABLE projects ADD COLUMN Status ENUM('new','reproposed') NOT NULL DEFAULT 'new' AFTER Preassigned;

        Version 17.12.1:  Oct 01, 2017	Fixed bug when printing a project. (231717 lines)
        Version 17.12:    Oct 01, 2017	Changes in layout of projects. (231712 lines)
Copy the following icons to icon public directory:
sudo cp icon/more64x64.png /var/www/html/swad/icon/
sudo cp icon/less64x64.png /var/www/html/swad/icon/

        Version 17.11.8:  Oct 01, 2017	Changes in edition of projects. (231638 lines)
        Version 17.11.7:  Oct 01, 2017	Changes in layout of projects. (231634 lines)
        Version 17.11.6:  Oct 01, 2017	Fixed bug in edition of projects. (231632 lines)
        Version 17.11.5:  Oct 01, 2017	Changes in order of roles in projects.
					Changes in edition of assignments, games, surveys and attendance events. (231536 lines)
        Version 17.11.4:  Sep 30, 2017	Changes in edition of a project. (231521 lines)
        Version 17.11.3:  Sep 30, 2017	Changes in layout of projects. (231517 lines)
        Version 17.11.2:  Sep 30, 2017	Changes in layout of projects. (231511 lines)
        Version 17.11.1:  Sep 30, 2017	Code refactoring in projects. (231510 lines)
        Version 17.11:    Sep 29, 2017	Select which projects to show: only my projects or all projects. (231531 lines)
        Version 17.10.7:  Sep 29, 2017	Icons to select which projects to show: only my projects or all projects. (231456 lines)
        Version 17.10.6:  Sep 25, 2017	Fixed bug when receiving projects. (231357 lines)
        Version 17.10.5:  Sep 25, 2017	Removed unused code in projects. (231354 lines)
        Version 17.10.4:  Sep 25, 2017	Changes in form behaviour when editing a project. (231365 lines)
        Version 17.10.3:  Sep 25, 2017	Check permission when editing projects.
					Check permission when removing a user from a project. (231359 lines)
        Version 17.10.2:  Sep 24, 2017	Add users to projects even if they don't belong to course. (231335 lines)
        Version 17.10.1:  Sep 24, 2017	Remove user in project when removing the user.
					Remove projects (and users in them) in a course when removing the course. (231345 lines)
        Version 17.10:    Sep 24, 2017	Changes in permissions to view/edit projects.
					Show project URL in listings of projects. (231319 lines)
        Version 17.9.4:   Sep 24, 2017	Changes in listing of projects. (231216 lines)
        Version 17.9.3:   Sep 24, 2017	Write students' IDs in listing of projects. (231214 lines)
        Version 17.9.2:   Sep 24, 2017	Header in listing of all projects in a table. (231206 lines)
        Version 17.9.1:   Sep 24, 2017	Changes in listing of all projects in a table. (231172 lines)
        Version 17.9:     Sep 24, 2017	Action to show all projects in a table. (231151 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1696','es','N','Ver tabla proyectos');

        Version 17.8.1:   Sep 23, 2017	Icon to show all projects in a table. (230980 lines)
        Version 17.8:     Sep 23, 2017	Projects can be ordered by start, end, title and department. (230963 lines)
        Version 17.7.1:   Sep 23, 2017	Changes in listing of projects. (230764 lines)
        Version 17.7:     Sep 23, 2017	Remove user from project. (230798 lines)
        Version 17.6:     Sep 22, 2017	Actions to remove user from project. (230559 lines)
					6 changes necessary in database:
UPDATE actions SET Txt='Solicitar ID a&ntilde;adir evaluador a proyecto' WHERE ActCod='1686' AND Language='es';
UPDATE actions SET Txt='A&ntilde;adir evaluador a proyecto' WHERE ActCod='1689' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1690','es','N','Solicitar ID eliminar estudiante de proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1691','es','N','Solicitar ID eliminar tutor de proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1692','es','N','Solicitar ID eliminar evaluador de proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1693','es','N','Eliminar estudiante de proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1694','es','N','Eliminar tutor de proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1695','es','N','Eliminar evaluador de proyecto');

        Version 17.5.10:  Sep 22, 2017	Actions to remove user from project. Not finished. (230429 lines)
        Version 17.5.9:   Sep 22, 2017	Icon to remove user from project. (230350 lines)
        Version 17.5.8:   Sep 22, 2017	Name of department in listing of projects. (230336 lines)
        Version 17.5.7:   Sep 21, 2017	Selector for department in edition of a project. (230363 lines)
        Version 17.5.6:   Sep 21, 2017	Added evaluators to projects. (230347 lines)
        Version 17.5.5:   Sep 21, 2017	Changes in listing and edition of projects. (230318 lines)
        Version 17.5.4:   Sep 21, 2017	Added field in projects for department. (230304 lines)
					2 changes necessary in database:
ALTER TABLE projects ADD COLUMN DptCod INT NOT NULL DEFAULT -1 NOT NULL AFTER CrsCod;
ALTER TABLE projects ADD INDEX(CrsCod,DptCod);

        Version 17.5.3:   Sep 21, 2017	Changes in listing of projects. (230301 lines)
        Version 17.5.2:   Sep 21, 2017	Do not display users when creating a new project. (230270 lines)
        Version 17.5.1:   Sep 21, 2017	Fixed bug when removing a project. (230267 lines)
        Version 17.5:     Sep 21, 2017	Users can be added to a project. (230250 lines)
        Version 17.4.3:   Sep 21, 2017	Changes in listing and edition of projects. (230222 lines)
        Version 17.4.2:   Sep 20, 2017	Changes in listing and edition of projects. (230203 lines)
        Version 17.4.1:   Sep 20, 2017	Changes adding users to a project. Not finished. (230141 lines)
        Version 17.4:     Sep 20, 2017	Actions to add users to a project. (230137 lines)
					6 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1684','es','N','Solicitar ID a&ntilde;adir estudiante a proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1685','es','N','Solicitar ID a&ntilde;adir tutor a proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1686','es','N','Solicitar ID a&ntilde;adir revisor a proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1687','es','N','A&ntilde;adir estudiante a proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1688','es','N','A&ntilde;adir tutor a proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1689','es','N','A&ntilde;adir revisor a proyecto');

        Version 17.3.13:  Sep 20, 2017	Links to add users to a project. Not finished. (230058 lines)
        Version 17.3.12:  Sep 20, 2017	Code refactoring in listing of projects. (229977 lines)
        Version 17.3.11:  Sep 20, 2017	Code refactoring in listing of projects. (229981 lines)
        Version 17.3.10:  Sep 19, 2017	New row in listing of projects for students. (230009 lines)
        Version 17.3.9:   Sep 19, 2017	Tutor code moved from table projects to table prj_grp. (229987 lines)
					1 change necessary in database:
ALTER TABLE projects DROP COLUMN UsrCod;

        Version 17.3.8:   Sep 19, 2017	Table for users in projects. (230001 lines)
					2 changes necessary in database:
DROP TABLE IF EXISTS prj_grp;
CREATE TABLE IF NOT EXISTS prj_usr (PrjCod INT NOT NULL,RoleInProject TINYINT NOT NULL DEFAULT 0,UsrCod INT NOT NULL,UNIQUE INDEX(PrjCod,RoleInProject,UsrCod));

        Version 17.3.7:   Sep 19, 2017	New row in listing of projects for tutors. Not finished. (229983 lines)
        Version 17.3.6:   Sep 19, 2017	Changes in listing of projects. (229849 lines)
        Version 17.3.5:   Sep 19, 2017	Removed groups in projects. (229858 lines)
        Version 17.3.4:   Sep 19, 2017	Changes in edition of projects. (230170 lines)
        Version 17.3.3:   Sep 19, 2017	Changes in listing of projects. (230169 lines)
        Version 17.3.2:   Sep 19, 2017	Changes in listing of projects. (230168 lines)
        Version 17.3.1:   Sep 19, 2017	Fix bugs in projects. (230089 lines)
        Version 17.3:     Sep 19, 2017	Changes in fields of database table used for projects. (230089 lines)
					2 changes necessary in database:
DROP TABLE IF EXISTS projects;
CREATE TABLE IF NOT EXISTS projects (PrjCod INT NOT NULL AUTO_INCREMENT,CrsCod INT NOT NULL DEFAULT -1,Hidden ENUM('N','Y') NOT NULL DEFAULT 'N',Preassigned ENUM('N','Y') NOT NULL DEFAULT 'N',UsrCod INT NOT NULL,StartTime DATETIME NOT NULL,EndTime DATETIME NOT NULL,Title VARCHAR(2047) NOT NULL,Description TEXT NOT NULL,Knowledge TEXT NOT NULL,Materials TEXT NOT NULL,URL VARCHAR(255) NOT NULL,UNIQUE INDEX(PrjCod),INDEX(CrsCod,Hidden));

        Version 17.2.5:   Sep 18, 2017	Get URL from form in projects. (229940 lines)
        Version 17.2.4:   Sep 18, 2017	New field URL in projects. (229929 lines)
					1 change necessary in database:
ALTER TABLE projects ADD COLUMN URL VARCHAR(255) NOT NULL AFTER Txt;

        Version 17.2.3:   Sep 17, 2017	New option for projects. Not finished. (229906 lines)
					3 changes necessary in database:
ALTER TABLE projects DROP COLUMN NumNotif;
ALTER TABLE projects DROP COLUMN Folder;
ALTER TABLE projects ADD COLUMN Preassigned ENUM('N','Y') NOT NULL DEFAULT 'N' AFTER Title;
Copy the following icons to icon public directory:
sudo cp icon/usr64x64.png /var/www/html/swad/icon/
sudo cp icon/usr_off64x64.png /var/www/html/swad/icon/

        Version 17.2.2:   Sep 17, 2017	New option for projects. Not finished. (229964 lines)
        Version 17.2.1:   Sep 17, 2017	New option for projects. Not finished. (229884 lines)
        Version 17.2:     Sep 17, 2017	New option for projects. Not finished. (229575 lines)
					12 changes necessary in database:
CREATE TABLE IF NOT EXISTS projects (PrjCod INT NOT NULL AUTO_INCREMENT,CrsCod INT NOT NULL DEFAULT -1,Hidden ENUM('N','Y') NOT NULL DEFAULT 'N',NumNotif INT NOT NULL DEFAULT 0,UsrCod INT NOT NULL,StartTime DATETIME NOT NULL,EndTime DATETIME NOT NULL,Title VARCHAR(2047) NOT NULL,Folder VARBINARY(255) NOT NULL,Txt TEXT NOT NULL,UNIQUE INDEX(PrjCod),INDEX(CrsCod,Hidden));
CREATE TABLE IF NOT EXISTS prj_grp (PrjCod INT NOT NULL,GrpCod INT NOT NULL,UNIQUE INDEX(PrjCod,GrpCod));
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1674','es','N','Ver proyectos');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1675','es','N','Solicitar creaci&oacute;n proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1676','es','N','Solicitar edici&oacute;n proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1677','es','N','Imprimir proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1678','es','N','Crear proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1679','es','N','Modificar proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1680','es','N','Solicitar elim. proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1681','es','N','Eliminar proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1682','es','N','Ocultar proyecto');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1683','es','N','Mostrar actividad');

        Version 17.1:     Sep 15, 2017	New module swad_project. Not finished. (229123 lines)
Copy the following icons to icon public directory:
sudo cp icon/iconset/awesome/action64x64/guide64x64.png /var/www/html/swad/icon/iconset/awesome/action64x64/
sudo cp icon/iconset/awesome/action64x64/project64x64.png /var/www/html/swad/icon/iconset/awesome/action64x64/
sudo cp icon/iconset/awesome/action64x64/remote64x64.png /var/www/html/swad/icon/iconset/awesome/action64x64/
sudo cp icon/iconset/nuvola/action64x64/guide64x64.png /var/www/html/swad/icon/iconset/nuvola/action64x64/
sudo cp icon/iconset/nuvola/action64x64/project64x64.png /var/www/html/swad/icon/iconset/nuvola/action64x64/
sudo cp icon/iconset/nuvola/action64x64/remote64x64.png /var/www/html/swad/icon/iconset/nuvola/action64x64/

        Version 17.0.4:   Sep 14, 2017	Show current question with answers when playing a game. (227379 lines)
        Version 17.0.3:   Sep 14, 2017	Action to show current question with answers when playing a game. (227350 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1673','es','N','Mostrar respuestas de pregunta juego');

        Version 17.0.2:   Sep 14, 2017	Action to show next question when playing a game. (227293 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1671','es','N','Mostrar primera pregunta juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1672','es','N','Mostrar siguiente pregunta juego');

        Version 17.0.1:   Sep 13, 2017	Icon to show question answers when playing a game. (227283 lines)
Copy the following icon to icon public directory:
sudo cp icon/step-forward64x64.png /var/www/html/swad/icon/

        Version 17.0:     Sep 13, 2017	New action to display a game question. (227245 lines)
        Version 16.254.2: Sep 13, 2017	Removed notifications in games.
					Start playing a game.
					Changes in pagination in games. (227141 lines)
        Version 16.254.1: Sep 13, 2017	Icon to start playing a game. (227122 lines)
Copy the following icon to icon public directory:
sudo cp icon/play64x64.png /var/www/html/swad/icon/

        Version 16.254:   Sep 12, 2017	Action to start playing a game. (227113 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1670','es','N','Bajar posici&oacute;n pregunta juego');

        Version 16.253:   Sep 11, 2017	Actions to move up and down questions in a game. (227070 lines)
					4 changes necessary in database:
UPDATE actions SET Txt='Subir posici&oacute;n item teor&iacute;a' WHERE ActCod='221' AND Language='es';
UPDATE actions SET Txt='Bajar posici&oacute;n item teor&iacute;a' WHERE ActCod='220' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1668','es','N','Subir posici&oacute;n pregunta juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1669','es','N','Bajar posici&oacute;n pregunta juego');

        Version 16.252.3: Sep 11, 2017	HTML optimization in icons. (226822 lines)
        Version 16.252.2: Sep 10, 2017	Code refactoring in edition of syllabus. (226819 lines)
        Version 16.252.1: Sep 09, 2017	Icons to move up and down questions in a game. (226864 lines)
        Version 16.252:   Sep 08, 2017	Icons to move up and down questions in a game. (226852 lines)
        Version 16.251.13:Sep 07, 2017	Option Remote Control moved after Test. (226873 lines)
        Version 16.251.12:Sep 07, 2017	Module swad_game is renamed swad_game. (226872 lines)
        Version 16.251.11:Sep 07, 2017	Listing games for remote control. Not finished. (226868 lines)
        Version 16.251.10:Sep 06, 2017	Listing games for remote control. Not finished. (226913 lines)
					2 changes necessary in database:
DROP TABLE IF EXISTS gam_answers;
CREATE TABLE IF NOT EXISTS gam_answers (GamCod INT NOT NULL,QstCod INT NOT NULL,AnsInd TINYINT NOT NULL,NumUsrs INT NOT NULL DEFAULT 0,UNIQUE INDEX(GamCod,QstCod,AnsInd));

        Version 16.251.9: Sep 06, 2017	Listing games for remote control. Not finished. (227273 lines)
        Version 16.251.8: Sep 06, 2017	Listing games for remote control. Not finished. (227211 lines)
        Version 16.251.7: Sep 04, 2017	Listing games for remote control. Not finished. (227218 lines)
        Version 16.251.6: Sep 01, 2017	Listing games for remote control. Not finished. (227111 lines)
        Version 16.251.5: Sep 01, 2017	Listing games for remote control. Not finished. (227045 lines)
					2 changes necessary in database:
ALTER TABLE gam_answers DROP COLUMN Answer;
UPDATE actions SET Obsolete='Y' WHERE ActCod='1663';

        Version 16.251.4: Sep 01, 2017	Listing games for remote control. Not finished. (227184 lines)
					2 changes necessary in database:
DROP TABLE IF EXISTS gam_questions;
CREATE TABLE IF NOT EXISTS gam_questions (GamCod INT NOT NULL,QstCod INT NOT NULL,QstInd INT NOT NULL DEFAULT 0,INDEX(GamCod),INDEX(QstCod));

        Version 16.251.3: Jul 18, 2017	Listing games for remote control. Not finished. (227148 lines)
        Version 16.251.2: Jul 16, 2017	Listing games for remote control. Not finished. (227062 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1667','es','N','A&ntilde;adir tests a juego');

        Version 16.251.1: Jul 16, 2017	Listing games for remote control. Not finished. (226888 lines)
        Version 16.251:   Jul 16, 2017	Listing games for remote control. Not finished. (226867 lines)
					19 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1648','es','N','Cambiar lugar de centro');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1649','es','N','Ver juegos');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1650','es','N','Ver un juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1651','es','N','Responder juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1652','es','N','Solicitar creaci&oacute;n juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1653','es','N','Solicitar edici&oacute;n juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1654','es','N','Crear juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1655','es','N','Modificar juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1656','es','N','Solicitar elim. juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1657','es','N','Eliminar juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1658','es','N','Solicitar puesta a cero juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1659','es','N','Poner a cero juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1660','es','N','Ocultar juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1661','es','N','Mostrar juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1662','es','N','Solicitar selec. tests para juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1663','es','N','Enviar pregunta juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1664','es','N','Preguntar si eliminar pregunta juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1665','es','N','Eliminar pregunta juego');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1666','es','N','Selec. descriptores tests para juego');

        Version 16.250:   Jul 09, 2017	Listing games for remote control. Not finished. (226738 lines)
					5 changes necessary in database:
CREATE TABLE IF NOT EXISTS games (GamCod INT NOT NULL AUTO_INCREMENT,Scope ENUM('Sys','Cty','Ins','Ctr','Deg','Crs') NOT NULL DEFAULT 'Sys',Cod INT NOT NULL DEFAULT -1,Hidden ENUM('N','Y') NOT NULL DEFAULT 'N',NumNotif INT NOT NULL DEFAULT 0,Roles INT NOT NULL DEFAULT 0,UsrCod INT NOT NULL,StartTime DATETIME NOT NULL,EndTime DATETIME NOT NULL,Title VARCHAR(2047) NOT NULL,Txt TEXT NOT NULL,UNIQUE INDEX(GamCod),INDEX(Scope,Cod));
CREATE TABLE IF NOT EXISTS gam_answers (QstCod INT NOT NULL,AnsInd TINYINT NOT NULL,NumUsrs INT NOT NULL DEFAULT 0,Answer TEXT NOT NULL,UNIQUE INDEX(QstCod,AnsInd));
CREATE TABLE IF NOT EXISTS gam_grp (GamCod INT NOT NULL,GrpCod INT NOT NULL,UNIQUE INDEX(GamCod,GrpCod));
CREATE TABLE IF NOT EXISTS gam_questions (QstCod INT NOT NULL AUTO_INCREMENT,GamCod INT NOT NULL,QstInd INT NOT NULL DEFAULT 0,AnsType ENUM('unique_choice','multiple_choice') NOT NULL,Stem TEXT NOT NULL,UNIQUE INDEX(QstCod),INDEX(GamCod));
CREATE TABLE IF NOT EXISTS gam_users (GamCod INT NOT NULL,UsrCod INT NOT NULL,UNIQUE INDEX(GamCod,UsrCod));

        Version 16.249.5: Jul 06, 2017	Listing games for remote control. Not finished. (226564 lines)
        Version 16.249.4: Jul 06, 2017	Listing games for remote control. Not finished. (226463 lines)
        Version 16.249.3: Jul 04, 2017	Listing games for remote control. Not finished. (226322 lines)
        Version 16.249.2: Jul 04, 2017	Listing games for remote control. Not finished. (226281 lines)
        Version 16.249.1: Jul 04, 2017	Listing games for remote control. Not finished. (226162 lines)
        Version 16.249:   Jul 04, 2017	Listing games for remote control. Not finished. (226021 lines)
        Version 16.248:   Jul 03, 2017	New option to emulate a remote control. (222272 lines)
        Version 16.247.6: Jul 02, 2017	Fixed bug in user's enrolment. (222151 lines)
        Version 16.247.5: Jul 02, 2017	Fixed error in layout when searching for courses. (222138 lines)
        Version 16.247.4: Jul 02, 2017	Fixed bug in surveys. (222138 lines)
        Version 16.247.3: Jul 02, 2017	Students can not view records of some users who do not share groups with them. (222132 lines)
        Version 16.247.2: Jun 23, 2017	Non-editing teachers can not view records of some users who do not share groups with them.
					Code refactoring related with roles. (222094 lines)
        Version 16.247.1: Jun 23, 2017	Non-editing teachers can select all groups when writing a message. (222068 lines)
        Version 16.247:   Jun 20, 2017	Fixed bug when removing a user from a course.
					Code refactoring related with caches. (222063 lines)
        Version 16.246:   Jun 20, 2017	Fixed bugs and code refactoring in groups. (221975 lines)
        Version 16.245:   Jun 12, 2017	Place editable in centre configuration. (221876 lines)
        Version 16.244.1: Jun 12, 2017	Fix bug in edition of centres. (221817 lines)
        Version 16.244:   Jun 12, 2017	Code refactoring related with boxes. (221815 lines)
        Version 16.243:   Jun 12, 2017	Code refactoring related with boxes. (222174 lines)
        Version 16.242:   Jun 11, 2017	Code refactoring related with boxes. (222128 lines)
        Version 16.241.2: Jun 11, 2017	Changes in alignment of dates in statistics. (222131 lines)
        Version 16.241.1: Jun 11, 2017	Fix bug confirming another user's ID. (222132 lines)
        Version 16.241:   Jun 11, 2017	New module swad_table to draw tables. (222132 lines)
        Version 16.240.1: Jun 11, 2017	Some functions moved from swad_layout to swad_icon. (222024 lines)
        Version 16.240:   Jun 11, 2017	New module swad_button to draw buttons. (222023 lines)
        Version 16.239:   Jun 10, 2017	New module swad_box to draw boxes. (221970 lines)
        Version 16.238:   Jun 10, 2017	Icon to close frames. (221803 lines)
        Version 16.237:   Jun 09, 2017	Code refactoring related to users belonging to a course.
					Changes in permissions to access students by non-editing teachers. (221303 lines)
        Version 16.236:   Jun 08, 2017	Changes in permissions related to record card contextual commands.
					Code refactoring related to roles. (221306 lines)
        Version 16.235.3: Jun 04, 2017	Non-editing teachers can not select/unselect groups to which they don't belong. (221107 lines)
        Version 16.235.2: Jun 04, 2017	Non-editing teachers can access and edit works of students. (221032 lines)
        Version 16.235.1: Jun 04, 2017	Limited length of connected users at right column. (221027 lines)
        Version 16.235:   Jun 04, 2017	Fixed bug in notifications about enrolment as a non-editing teacher.
					Fixed bugs and code refactoring related to users enrolment. (221018 lines)
					2 changes necessary in database:
UPDATE usr_data SET NotifNtfEvents = (NotifNtfEvents | ((NotifNtfEvents & 0x100) << 12)) & 0x7FFFFFFF;
UPDATE usr_data SET EmailNtfEvents = (EmailNtfEvents | ((EmailNtfEvents & 0x100) << 12)) & 0x7FFFFFFF;

        Version 16.234:   Jun 04, 2017	Removed some verbose messages when removing a user.
					Modification / removing of a user is performed in an a priori function. (220995 lines)
        Version 16.233:   Jun 04, 2017	Code refactoring related with roles. (221175 lines)
        Version 16.232.8: Jun 03, 2017	Fix bug (an URL like https://openswad.org/es?a gived Internal Server Error). (221147 lines)
        Version 16.232.7: Jun 03, 2017	Changed contextual links in institutional links and banners. (221148 lines)
Copy the following icon to icon public directory:
sudo cp icon/link64x64.gif /var/www/html/swad/icon/

        Version 16.232.6: Jun 03, 2017	Changed layout of edition of banners. (221123 lines)
        Version 16.232.5: Jun 03, 2017	Changed layout of edition of institutional links. (221108 lines)
        Version 16.232.4: Jun 01, 2017	Changed alert when session expired. (221092 lines)
        Version 16.232.3: Jun 01, 2017	Fixed bug in permission of deletion of forum threads. (221091 lines)
        Version 16.232.2: May 31, 2017	Fixed bug in selection of user's centre and department. (221089 lines)
        Version 16.232.1: May 31, 2017	Improvements in selection of user's institution, centre and department. (221085 lines)
        Version 16.232:   May 31, 2017	Fix bug in selection of user's centre. (221042 lines)
        Version 16.231.4: May 31, 2017	Changes in layout of statistics about institutions.
					Fixed bug in search. (221039 lines)
        Version 16.231.3: May 31, 2017	Changes in layout of statistics about institutions. (221037 lines)
        Version 16.231.2: May 31, 2017	Fixed bug in connected users. (221036 lines)
        Version 16.231.1: May 31, 2017	Fixed bugs in enrolment and groups. (221031 lines)
        Version 16.231:   May 31, 2017	Non-editing teachers can edit (with restrictions) in private zones. (221031 lines)
        Version 16.230:   May 30, 2017	Number of teachers and non-editing teachers in list of groups. (220953 lines)
        Version 16.229.1: May 30, 2017	Code refactoring in selection of groups.
					Fixed bug when printing list of teachers. (220762 lines)
        Version 16.229:   May 30, 2017	Selection of groups also for listing of teachers. (220748 lines)
        Version 16.228.2: May 30, 2017	Fixed bug in selection of users. (220726 lines)
        Version 16.228.1: May 30, 2017	By default, photos are displayed in listing of users. (220722 lines)
        Version 16.228:   May 30, 2017	Non-editing teachers can not change their selection of groups.
					Removed alert with origin of copy when pasting. (220722 lines)
        Version 16.227.2: May 30, 2017	Button to upload photo integrated in alert. (220694 lines)
        Version 16.227.1: May 30, 2017	Button to check email integrated in alert. (220693 lines)
        Version 16.227:   May 30, 2017	Changes in selector or roles in record cards. (220694 lines)
        Version 16.226.1: May 29, 2017	Changes in contextual links related to users enrolment. (220637 lines)
        Version 16.226:   May 29, 2017	Code refactoring in buttons. (220607 lines)
        Version 16.225.12:May 29, 2017	Button to register students in connected and requests. (220587 lines)
        Version 16.225.11:May 29, 2017	Button to register students everytime an alert with no students in course is displayed. (220581 lines)
        Version 16.225.10:May 29, 2017	Buttons to register students in listing of teacher's courses. (220561 lines)
        Version 16.225.9: May 29, 2017	Fixed minor bug in user administration. (220540 lines)
        Version 16.225.8: May 29, 2017	Statistics of hits in course accessible by non-editing teachers.
					Changes in error handlong in stats of hits. (220514 lines)
        Version 16.225.7: May 26, 2017	Fix bug in order of users when listing record cards of teachers.
					Code refactoring in checkboxes to select users. (220509 lines)
        Version 16.225.6: May 26, 2017	Fixed bug printing classphoto of teachers. (220553 lines)
        Version 16.225.5: May 26, 2017	Removed link to show all data of students. (220543 lines)
        Version 16.225.4: May 26, 2017	Removed link to show all data of teachers. (220562 lines)
        Version 16.225.3: May 26, 2017	Removed link to show all data of guests. (220582 lines)
        Version 16.225.2: May 26, 2017	Fixed bug in lists of teachers. (220601 lines)
        Version 16.225.1: May 26, 2017	Fixed bug related with new role. (220580 lines)
        Version 16.225:   May 25, 2017	Changes and bug fixing related with new role. (220580 lines)
        Version 16.224.2: May 25, 2017	Button to show big list of users/courses integrated in alert. (220465 lines)
        Version 16.224.1: May 25, 2017	User lists are shown when changing type of listing. (220463 lines)
        Version 16.224:   May 25, 2017	Code refactoring related to sections. (220459 lines)
        Version 16.223.2: May 25, 2017	User lists are shown when clicking on "Update students". (220435 lines)
        Version 16.223.1: May 25, 2017	Alerts closable or not depending on their type. (220400 lines)
        Version 16.223:   May 24, 2017	Changes in lists of users related with new role. (220384 lines)
        Version 16.222.2: May 24, 2017	Fixed bug in course records related with new role. (220369 lines)
        Version 16.222.1: May 23, 2017	Fixed bugs in records and forums related with new role. (220359 lines)
        Version 16.222:   May 23, 2017	Alerts can be closed. (220313 lines)
Copy the following icon to icon public directory:
sudo cp -a icon/close64x64.png /var/www/html/swad/icon/

        Version 16.221.1: May 23, 2017	Fixed bugs related with new role.
					Fixed bug in surveys.
					Fixed bug in attendance events. (220260 lines)
        Version 16.221:   May 23, 2017	Changes in database related with new role. Not finished. (220253 lines)
					8 changes necessary in database:
UPDATE announcements SET Roles = (((Roles & 0xFF0) << 1) | (Roles & 0x00F));
UPDATE connected SET RoleInLastCrs = RoleInLastCrs + (RoleInLastCrs >= 4);
UPDATE crs_usr SET Role = Role + (Role >= 4);
UPDATE crs_usr_requests SET Role = Role + (Role >= 4);
UPDATE sessions SET Role = Role + (Role >= 4);
UPDATE surveys SET Roles = (((Roles & 0xFF0) << 1) | (Roles & 0x00F));
UPDATE log_recent SET Role = Role + (Role >= 4);
UPDATE log_full SET Role = Role + (Role >= 4);

        Version 16.220.14:May 23, 2017	Changes related with new role. Not finished. (220243 lines)
        Version 16.220.13:May 22, 2017	Changes related with new role. Not finished. (220190 lines)
        Version 16.220.12:May 22, 2017	Changes related with new role. Not finished. (220155 lines)
        Version 16.220.11:May 22, 2017	Changes related with new role. Not finished. (220019 lines)
        Version 16.220.10:May 22, 2017	Changes related with new role. Not finished. (219775 lines)
        Version 16.220.9: May 22, 2017	Changes related with new role. Not finished. (219720 lines)
        Version 16.220.8: May 22, 2017	Changes related with new role. Not finished. (219663 lines)
					4 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1644','es','N','Confirmar modif. profesor no editor');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1645','es','N','Crear usuario como profesor no editor');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1646','es','N','Modificar datos profesor no editor');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1647','es','N','Eliminar profesor no editor asignatura');

        Version 16.220.7: May 21, 2017	Changes related with new role. Not finished. (219535 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1643','es','N','Inscribir/eliminar varios profesores no editores');

        Version 16.220.6: May 21, 2017	Changes related with new role. Not finished. (219485 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1642','es','N','Administrar varios profesores no editores');

        Version 16.220.5: May 21, 2017	Changes related with new role. Not finished. (219452 lines)
					3 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1639','es','N','Solicitar acept. inscrip. profesor no editor');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1640','es','N','Aceptar inscrip. como profesor no editor');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1641','es','N','Rechazar inscrip. como profesor no editor');

        Version 16.220.4: May 21, 2017	Changes related with new role. Not finished. (219401 lines)
        Version 16.220.3: May 21, 2017	Changes related with new role. Not finished. (219327 lines)
        Version 16.220.2: May 21, 2017	Changes related with new role. Not finished. (219319 lines)
        Version 16.220.1: May 20, 2017	Changes related with new role. Not finished. (219332 lines)
        Version 16.220:   May 19, 2017	Changes permission of actions after creating new role. Not finished. (219227 lines)
        Version 16.219:   May 19, 2017	New role: non-editing teacher. Not finished. (219226 lines)
        Version 16.218:   May 19, 2017	Changes in permissions of actions. (218888 lines)
        Version 16.217:   May 18, 2017	Role user is the default when no course is selected. (218887 lines)
        Version 16.216:   May 18, 2017	Role visitor is renamed as role user. (218867 lines)
        Version 16.215.4: May 18, 2017	Changed layout of assignments.
					Removed alert with number of users notified via email. (218868 lines)
        Version 16.215.3: May 15, 2017	Fixed bug in layout of alerts. (218950 lines)
        Version 16.215.2: May 13, 2017	Default theme: grey. (218946 lines)
        Version 16.215.1: May 12, 2017	Fixed bug in layout of indicators. (218946 lines)
        Version 16.215:   May 11, 2017	New module swad_alert for alerts. (218944 lines)
        Version 16.214:   May 11, 2017	Changes in edition of institutions.
					Code refactoring related to alerts. (218867 lines)
        Version 16.213:   May 11, 2017	Changes in edition of centres. (218881 lines)
        Version 16.212:   May 11, 2017	Changes in edition of courses and degrees. (218870 lines)
        Version 16.211.2: May 11, 2017	Fixed bug in alerts. (218879 lines)
        Version 16.211.1: May 11, 2017	Change in layout of forms to edit groups. (218882 lines)
        Version 16.211:   May 10, 2017	Code refactoring related to alerts. (218879 lines)
        Version 16.210:   May 10, 2017	Changes in layout of records. (218869 lines)
        Version 16.209.4: May 10, 2017	Changes in layout of list of guests' records. (218838 lines)
        Version 16.209.3: May 10, 2017	Changes in layout of list of teachers' records. (218830 lines)
        Version 16.209.2: May 09, 2017	Fixed bug in hidden parameters. (218831 lines)
					1 change necessary in database:
ALTER TABLE hidden_params CHANGE COLUMN ParamValue ParamValue LONGTEXT NOT NULL;

        Version 16.209.1: May 09, 2017	Removed unused code in records. (218828 lines)
        Version 16.209:   May 09, 2017	Confirmation of user's ID is made directly, without confirmation. (218836 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod IN ('1565','1566','1567');

        Version 16.208.7: May 09, 2017	Changes in layout of student records. (218771 lines)
        Version 16.208.6: May 09, 2017	Changes in alerts.
					Changes in layout of student records. (218744 lines)
        Version 16.208.5: May 09, 2017	Changes in edition of students records. (218750 lines)
        Version 16.208.4: May 08, 2017	Changes in edition of students records. (218728 lines)
        Version 16.208.3: May 08, 2017	Changes in edition of students records. (218713 lines)
        Version 16.208.2: May 08, 2017	Changes in confirmation of user's ID. (218704 lines)
        Version 16.208.1: May 08, 2017	Fixed problem in character encoding in files of marks, reported by Sandra Daniela Tazzioli Barroso. (218696 lines)
        Version 16.208:   May 07, 2017	New module swad_language for preference on language. (218695 lines)
        Version 16.207.2: May 07, 2017	Changes in help on preferences. (218621 lines)
        Version 16.207.1: May 05, 2017	Minor changes in layout. (218577 lines)
        Version 16.207:   May 05, 2017	Changes in layout of preferences. (218577 lines)
        Version 16.206.4: May 05, 2017	Changes in question to change language. (218534 lines)
        Version 16.206.3: May 05, 2017	Fixed bug in Javascript function to write local date and time. (218527 lines)
        Version 16.206.2: May 05, 2017	Changes in date and time of connected users. (218527 lines)
        Version 16.206.1: May 05, 2017	Changes in Javascript function to write local date and time. (218524 lines)
        Version 16.206:   May 05, 2017	Date format applied to dates written directly instead of using Javascript. (218514 lines)
        Version 16.205.4: May 05, 2017	Help about date format. (218487 lines)
        Version 16.205.3: May 04, 2017	Changes in statistics about date format. (218447 lines)
        Version 16.205.2: May 04, 2017	Changes in form to select date format. (218439 lines)
        Version 16.205.1: May 04, 2017	Removed unused code. (218440 lines)
        Version 16.205:   May 04, 2017	Dates written depending on user's preferences. (218721 lines)
        Version 16.204:   May 04, 2017	Form in preferences to change date format. (218656 lines)
					3 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1638','es','N','Cambiar formato de fecha');
ALTER TABLE usr_data ADD COLUMN DateFormat TINYINT NOT NULL DEFAULT 0 AFTER FirstDayOfWeek,ADD INDEX (DateFormat);
ALTER TABLE IP_prefs ADD COLUMN DateFormat TINYINT NOT NULL DEFAULT 0 AFTER FirstDayOfWeek;

        Version 16.203:   May 04, 2017	Form in preferences to change date format. Not finished. (218447 lines)
        Version 16.202.2: May 03, 2017	Fixed bug in attendance events, reported by Francisco Oca�a Lara. (218306 lines)
        Version 16.202.1: May 02, 2017	Changed help on surveys from STATS to ASSESSMENT. (218296 lines)
        Version 16.202:   May 02, 2017	Changes in layout of alerts. (218295 lines)
Copy the following icons to icon public directory:
sudo cp -a icon/info64x64.png /var/www/html/swad/icon/
sudo cp -a icon/success64x64.png /var/www/html/swad/icon/
sudo cp -a icon/question64x64.gif /var/www/html/swad/icon/
sudo cp -a icon/warning64x64.gif /var/www/html/swad/icon/
sudo cp -a icon/warning64x64.png /var/www/html/swad/icon/
sudo cp -a icon/error64x64.gif /var/www/html/swad/icon/

        Version 16.201.5: May 02, 2017	Changes in layout of assignment print view. (218270 lines)
        Version 16.201.4: May 02, 2017	Icon to view course info when editing source of information. (218243 lines)
        Version 16.201.3: May 02, 2017	Code refactoring in forums. (218237 lines)
        Version 16.201.2: May 02, 2017	Code refactoring in messages. (218242 lines)
        Version 16.201.1: May 02, 2017	Code refactoring in messages. (218241 lines)
        Version 16.201:   May 02, 2017	Change layout of contextual icons in assignments, surveys and attendance. (218344 lines)
        Version 16.200.1: May 02, 2017	Fixed problem printing view of students, teachers and guests. (218331 lines)
        Version 16.200:   May 02, 2017	Print view of an assignment, suggested by Sandra Daniela Tazzioli Barroso. (218337 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1637','es','N','Imprimir actividad');

        Version 16.199:   May 02, 2017	Code refactoring related to message author. (218291 lines)
        Version 16.198.9: May 02, 2017	Code refactoring related to message author. (218345 lines)
        Version 16.198.8: May 01, 2017	Code refactoring related to tables. (218323 lines)
        Version 16.198.7: May 01, 2017	Code refactoring related to tables. (218319 lines)
        Version 16.198.6: May 01, 2017	Code refactoring related to tables. (218312 lines)
        Version 16.198.5: May 01, 2017	Code refactoring related to tables. (218302 lines)
        Version 16.198.4: Apr 30, 2017	Code refactoring related to contextual icons to remove. (218294 lines)
        Version 16.198.3: Apr 30, 2017	Code refactoring related to contextual icons to hide and unhide. (218320 lines)
        Version 16.198.2: Apr 30, 2017	Changes in layout of edition of places. (218341 lines)
        Version 16.198.1: Apr 30, 2017	Changes in layout of edition of countries. (218332 lines)
        Version 16.198:   Apr 30, 2017	Code refactoring related to contextual icons to edit, view and print. (218361 lines)
        Version 16.197:   Apr 30, 2017	Changes in layout of edition of degree types. (218579 lines)
        Version 16.196.3: Apr 30, 2017	Changes in layout of edition of courses. (218540 lines)
        Version 16.196.2: Apr 30, 2017	Changes in layout of edition of degrees. (218537 lines)
        Version 16.196.1: Apr 30, 2017	Changes in layout of edition of centres. (218538 lines)
        Version 16.196:   Apr 30, 2017	Changes in layout of edition of institutions. (218525 lines)
        Version 16.195.8: Apr 29, 2017	Changes in layout of file browser. (218511 lines)
        Version 16.195.7: Apr 29, 2017	Fixed bug in tabs. (218511 lines)
        Version 16.195.6: Apr 29, 2017	Code refactoring in edition of groups. (218507 lines)
        Version 16.195.5: Apr 29, 2017	Changes in layout of edition of groups. (218513 lines)
        Version 16.195.4: Apr 29, 2017	Changes in layout of edition of groups. (218513 lines)
        Version 16.195.3: Apr 29, 2017	Add plus icon in box "Groups" when editing groups. (218537 lines)
        Version 16.195.2: Apr 29, 2017	Add plus icon in box "Types of group" when editing groups. (218512 lines)
        Version 16.195.1: Apr 29, 2017	Code refactoring in edition of groups. (218474 lines)
        Version 16.195:   Apr 29, 2017	Changes in edition of groups. (218470 lines)
        Version 16.194.22:Apr 29, 2017	Changes in edition of groups. (218433 lines)
        Version 16.194.21:Apr 29, 2017	Changes in edition of groups. (218435 lines)
        Version 16.194.20:Apr 28, 2017	Changes in edition of groups. (218420 lines)
        Version 16.194.19:Apr 28, 2017	Changed dialog to remove a non empty type of group. (218397 lines)
        Version 16.194.18:Apr 28, 2017	Changed dialog to remove a survey. (218388 lines)
        Version 16.194.17:Apr 28, 2017	Changed dialog to remove a test question. (218388 lines)
        Version 16.194.16:Apr 28, 2017	Changed dialog to remove a survey question. (218370 lines)
        Version 16.194.15:Apr 28, 2017	Changed dialog to remove another user's photo.
					Fixed bug when uploading photo. (218370 lines)
        Version 16.194.14:Apr 28, 2017	Changed dialog to remove a group. (218368 lines)
        Version 16.194.13:Apr 28, 2017	Changed dialog to remove a social note. (218417 lines)
        Version 16.194.12:Apr 28, 2017	Changed dialog to remove a social comment. (218398 lines)
        Version 16.194.11:Apr 28, 2017	Changed dialog to remove a notice. (218379 lines)
        Version 16.194.10:Apr 28, 2017	Changed dialog to remove an exam announcement. (218379 lines)
        Version 16.194.9: Apr 28, 2017	Changed dialog to remove a non empty folder. (218379 lines)
        Version 16.194.8: Apr 28, 2017	Changed dialog to remove a file/link. (218369 lines)
        Version 16.194.7: Apr 28, 2017	Changed dialog to remove a record field. (218359 lines)
        Version 16.194.6: Apr 28, 2017	Changed dialog to remove an agenda event. (218347 lines)
        Version 16.194.5: Apr 28, 2017	Changed dialog to remove a forum thread. (218347 lines)
        Version 16.194.4: Apr 28, 2017	Changed dialog to remove an assignment. (218332 lines)
        Version 16.194.3: Apr 28, 2017	Changed dialog to remove all students in course. (218340 lines)
        Version 16.194.2: Apr 28, 2017	Changed dialog to reject user's enrolment request. (218336 lines)
        Version 16.194.1: Apr 28, 2017	Changed dialog to register user as administrator. (218335 lines)
        Version 16.194:   Apr 28, 2017	Fixed bugs in filtering sent and received messages. (218332 lines)
        Version 16.193.8: Apr 28, 2017	Changed dialog to remove messages. (218299 lines)
        Version 16.193.7: Apr 27, 2017	Changed dialog to remove my account. (218328 lines)
        Version 16.193.6: Apr 27, 2017	Changed dialog to remove user account. (218322 lines)
        Version 16.193.5: Apr 27, 2017	Changed dialog to remove user from course. (218316 lines)
        Version 16.193.4: Apr 27, 2017	Changes in layout of alerts.
					Changed dialog to remove user as administrator. (218311 lines)
        Version 16.193.3: Apr 27, 2017	Changes in layout of alerts.
					Changed dialog to remove my photo. (218291 lines)
        Version 16.193.2: Apr 27, 2017	Statistics tab is renamed as Analytics tab. (218265 lines)
        Version 16.193.1: Apr 27, 2017	Exam announcements action moved to end of assessment tab. (218263 lines)
        Version 16.193:   Apr 27, 2017	Surveys action moved from statistics to assessment tab. (218262 lines)
        Version 16.192.1: Apr 25, 2017	Code refactoring in timetable. (218258 lines)
        Version 16.192:   Apr 25, 2017	Changes in layout and code refactoring in timetable. (218262 lines)
        Version 16.191:   Apr 25, 2017	Changes in layout of timetable. (218183 lines)
        Version 16.190:   Apr 25, 2017	Code refactoring in timetable. (218106 lines)
        Version 16.189:   Apr 25, 2017	Changes in timetable. (218027 lines)
					7 changes necessary in database:
ALTER TABLE timetable_crs DROP COLUMN DayOld;
ALTER TABLE timetable_crs DROP COLUMN HourOld;
ALTER TABLE timetable_crs DROP COLUMN DurationOld;
ALTER TABLE timetable_crs DROP COLUMN ClassTypeOld;

ALTER TABLE timetable_tut DROP COLUMN DayOld;
ALTER TABLE timetable_tut DROP COLUMN HourOld;
ALTER TABLE timetable_tut DROP COLUMN DurationOld;

        Version 16.188:   Apr 24, 2017	Changes in timetable. Not finished. (218029 lines)
        Version 16.187.3: Apr 24, 2017	Changes in timetable. Not finished. (218040 lines)
        Version 16.187.2: Apr 24, 2017	Changes in timetable. Not finished. (217963 lines)
        Version 16.187.1: Apr 24, 2017	Changes in timetable. Not finished. (217958 lines)
        Version 16.187:   Apr 24, 2017	Changes in timetable. Not finished. (217947 lines)
					35 changes necessary in database:
ALTER TABLE timetable_crs CHANGE COLUMN Day DayOld ENUM('L','M','X','J','V','S','D') NOT NULL;
ALTER TABLE timetable_crs CHANGE COLUMN Hour HourOld TINYINT NOT NULL;
ALTER TABLE timetable_crs CHANGE COLUMN Duration DurationOld TINYINT NOT NULL;
ALTER TABLE timetable_crs CHANGE COLUMN ClassType ClassTypeOld ENUM('libre','teoria','practicas') NOT NULL;

ALTER TABLE timetable_crs ADD COLUMN Weekday TINYINT NOT NULL AFTER GrpCod;
ALTER TABLE timetable_crs ADD COLUMN StartTime TIME NOT NULL AFTER Weekday;
ALTER TABLE timetable_crs ADD COLUMN Duration TIME NOT NULL AFTER StartTime;
ALTER TABLE timetable_crs ADD COLUMN ClassType ENUM('free','lecture','practical') NOT NULL AFTER Duration;

UPDATE timetable_crs SET Weekday=0 WHERE DayOld='L';
UPDATE timetable_crs SET Weekday=1 WHERE DayOld='M';
UPDATE timetable_crs SET Weekday=2 WHERE DayOld='X';
UPDATE timetable_crs SET Weekday=3 WHERE DayOld='J';
UPDATE timetable_crs SET Weekday=4 WHERE DayOld='V';
UPDATE timetable_crs SET Weekday=5 WHERE DayOld='S';
UPDATE timetable_crs SET Weekday=6 WHERE DayOld='D';

UPDATE timetable_crs SET StartTime=ADDTIME('06:00:00',SEC_TO_TIME(TIME_TO_SEC('00:30:00')*HourOld));

UPDATE timetable_crs SET Duration=SEC_TO_TIME(TIME_TO_SEC('00:30:00')*DurationOld);

UPDATE timetable_crs SET ClassType='free' WHERE ClassTypeOld='libre';
UPDATE timetable_crs SET ClassType='lecture' WHERE ClassTypeOld='teoria';
UPDATE timetable_crs SET ClassType='practical' WHERE ClassTypeOld='practicas';

ALTER TABLE timetable_tut CHANGE COLUMN Day DayOld ENUM('L','M','X','J','V','S','D') NOT NULL;
ALTER TABLE timetable_tut CHANGE COLUMN Hour HourOld TINYINT NOT NULL;
ALTER TABLE timetable_tut CHANGE COLUMN Duration DurationOld TINYINT NOT NULL;

ALTER TABLE timetable_tut ADD COLUMN Weekday TINYINT NOT NULL AFTER UsrCod;
ALTER TABLE timetable_tut ADD COLUMN StartTime TIME NOT NULL AFTER Weekday;
ALTER TABLE timetable_tut ADD COLUMN Duration TIME NOT NULL AFTER StartTime;

UPDATE timetable_tut SET Weekday=0 WHERE DayOld='L';
UPDATE timetable_tut SET Weekday=1 WHERE DayOld='M';
UPDATE timetable_tut SET Weekday=2 WHERE DayOld='X';
UPDATE timetable_tut SET Weekday=3 WHERE DayOld='J';
UPDATE timetable_tut SET Weekday=4 WHERE DayOld='V';
UPDATE timetable_tut SET Weekday=5 WHERE DayOld='S';
UPDATE timetable_tut SET Weekday=6 WHERE DayOld='D';

UPDATE timetable_tut SET StartTime=ADDTIME('06:00:00',SEC_TO_TIME(TIME_TO_SEC('00:30:00')*HourOld));

UPDATE timetable_tut SET Duration=SEC_TO_TIME(TIME_TO_SEC('00:30:00')*DurationOld);

        Version 16.186.1: Apr 24, 2017	Changes in timetable. Not finished. (217881 lines)
        Version 16.186:   Apr 24, 2017	Changes in timetable. Not finished. (217867 lines)
        Version 16.185.4: Apr 21, 2017	Warning about Java no longer working. (217849 lines)
        Version 16.185.3: Apr 20, 2017	Timetables end at 24h, suggested by Sandra Daniela Tazzioli Barroso. (217797 lines)
        Version 16.185.2: Apr 19, 2017	Changes in forums. (217793 lines)
        Version 16.185.1: Apr 19, 2017	Changes in forums.
					New help URLs for forums. (217793 lines)
        Version 16.185:   Apr 19, 2017	Code refactoring in forums. (217708 lines)
        Version 16.184:   Apr 18, 2017	Changes in layout of page numbers. (217724 lines)
        Version 16.183.1: Apr 18, 2017	Changes in layout of forums. (217719 lines)
        Version 16.183:   Apr 18, 2017	Code refactoring in forums. (217738 lines)
        Version 16.182:   Apr 18, 2017	Code refactoring in forums and pagination. (217678 lines)
        Version 16.181.7: Apr 18, 2017	Changes in layout of forums. (217641 lines)
        Version 16.181.6: Apr 18, 2017	Code refactoring in forums. (217648 lines)
        Version 16.181.5: Apr 18, 2017	Code refactoring in forums. (217646 lines)
        Version 16.181.4: Apr 18, 2017	Changes in layout of forums. (217621 lines)
        Version 16.181.3: Apr 18, 2017	Code refactoring in forums. (217604 lines)
        Version 16.181.2: Apr 18, 2017	Code refactoring in forums. (217590 lines)
        Version 16.181.1: Apr 17, 2017	Code refactoring in forums. (217538 lines)
        Version 16.181:   Apr 17, 2017	Changes in layout of messages.
					Changes in layout of some tables.
					Fixed bug in forums. (217560 lines)
        Version 16.180.1: Apr 17, 2017	New '+' icon in list of thread posts to add a new post. (217558 lines)
        Version 16.180:   Apr 17, 2017	Code refactoring in forums. (217521 lines)
        Version 16.179:   Apr 16, 2017	Code refactoring in forums. (217444 lines)
        Version 16.178.1: Apr 16, 2017	Code refactoring in forums. (217601 lines)
        Version 16.178:   Apr 16, 2017	Code refactoring in forums. (217595 lines)
        Version 16.177.4: Apr 14, 2017	Changes in layout of forums. (217611 lines)
        Version 16.177.3: Apr 14, 2017	Highlight current thread in forums. (217609 lines)
        Version 16.177.2: Apr 14, 2017	Code refactoring in forums. (217604 lines)
        Version 16.177.1: Apr 13, 2017	Changes in layout of forums. (217590 lines)
        Version 16.177:   Apr 13, 2017	Code refactoring related to pagination.
					Changes in layout of forums. (217609 lines)
        Version 16.176.3: Apr 12, 2017	Remove unused code in forums. (217582 lines)
        Version 16.176.2: Apr 12, 2017	Changes in layout of forums. (217705 lines)
        Version 16.176.1: Apr 11, 2017	Changes in layout of forums. (217699 lines)
        Version 16.176:   Apr 11, 2017	Code refactoring in pagination.
					Changes in layout of forums. (217680 lines)
        Version 16.175.1: Apr 11, 2017	Changes in layout of forums. (217667 lines)
        Version 16.175:   Apr 11, 2017	Code refactoring and changes in layout of forums. (217669 lines)
        Version 16.174.1: Apr 10, 2017	Change in layout of frames (white boxes).
					New '+' icon in list of forum threads to add a new thread. (217591 lines)
        Version 16.174:   Apr 07, 2017	Suffix .en added to help URLs in English. (217568 lines)
        Version 16.173.5: Apr 07, 2017	Help URLs in Spanish for users with languages ca and gn. (217567 lines)
        Version 16.173.4: Apr 07, 2017	Some help URLs translated to spanish. (217566 lines)
        Version 16.173.3: Apr 05, 2017	Some help URLs translated to spanish. (217545 lines)
        Version 16.173.2: Apr 04, 2017	Some help URLs translated to spanish. (217544 lines)
        Version 16.173.1: Apr 04, 2017	Some help URLs translated to spanish. (217542 lines)
        Version 16.173:   Apr 03, 2017	Some help URLs translated to spanish.
					Fixed bugs and code refactoring in public profiles. (217541 lines)
        Version 16.172.3: Mar 31, 2017	Some help URLs translated to spanish. (217556 lines)
        Version 16.172.2: Mar 30, 2017	Some help URLs translated to spanish. (217555 lines)
        Version 16.172.1: Mar 30, 2017	Some help URLs translated to spanish. (217554 lines)
        Version 16.172:   Mar 30, 2017	Some help URLs translated to spanish.
					Changed "enroll" and "enrollment" to "enrol" and "enrolment", except in web service API. (217531 lines)
        Version 16.171.15:Mar 30, 2017	Some help URLs translated to spanish. (217528 lines)
        Version 16.171.14:Mar 29, 2017	Some help URLs translated to spanish. (217526 lines)
        Version 16.171.13:Mar 29, 2017	Some help URLs translated to spanish. (217526 lines)
        Version 16.171.12:Mar 29, 2017	Some help URLs translated to spanish. (217525 lines)
        Version 16.171.11:Mar 29, 2017	Changes in translations related to statistics. (217523 lines)
        Version 16.171.10:Mar 28, 2017	Some help URLs translated to spanish. (217522 lines)
        Version 16.171.9: Mar 28, 2017	Some help URLs translated to spanish. (217522 lines)
        Version 16.171.8: Mar 27, 2017	Some help URLs translated to spanish. (217521 lines)
        Version 16.171.7: Mar 27, 2017	Some help URLs translated to spanish. (217520 lines)
        Version 16.171.6: Mar 27, 2017	Some help URLs translated to spanish. (217519 lines)
        Version 16.171.5: Mar 27, 2017	Changed buttons to create a new link, banner, place or holiday. (217518 lines)
        Version 16.171.4: Mar 27, 2017	Some help URLs translated to spanish. (217605 lines)
        Version 16.171.3: Mar 27, 2017	Fixed bug in assignments, reported by Eva Mart�nez Ortigosa. (217604 lines)
        Version 16.171.2: Mar 27, 2017	Some help URLs translated to spanish. (217603 lines)
        Version 16.171.1: Mar 27, 2017	Code refactoring in banners. (217602 lines)
        Version 16.171:   Mar 27, 2017	Action to remove old courses removed from main menu. (217534 lines)
        Version 16.170.2: Mar 27, 2017	Link to remove old courses from Hierarchy. (217573 lines)
        Version 16.170.1: Mar 27, 2017	Pending is renamed as Hierarchy. (217554 lines)
        Version 16.170:   Mar 27, 2017	Action to view banners removed from main menu. (217553 lines)
        Version 16.169.3: Mar 26, 2017	Link to banners from institutional links. (217592 lines)
Copy the following icon to icon public directory:
sudo cp -a icon/picture64x64.gif /var/www/html/swad/icon/

        Version 16.169.2: Mar 26, 2017	Fix bug in institutional links.
					Some messages removed in edition of banners, links, etc. (217570 lines)
        Version 16.169.1: Mar 26, 2017	Changed help URLs related to degree types. (217661 lines)
        Version 16.169:   Mar 26, 2017	Action to view degree types removed from main menu. (217660 lines)
        Version 16.168.1: Mar 26, 2017	Link to view degree types in degrees. (217694 lines)
        Version 16.168:   Mar 26, 2017	Action to view places removed from main menu. (217679 lines)
        Version 16.167.16:Mar 26, 2017	Link to view places in centres. (217718 lines)
        Version 16.167.15:Mar 26, 2017	Code refactoring in enrolment. (217694 lines)
        Version 16.167.14:Mar 26, 2017	Code refactoring in follow/unfollow. (217691 lines)
        Version 16.167.13:Mar 26, 2017	Code refactoring in uploading of photos. (217686 lines)
        Version 16.167.12:Mar 26, 2017	Code refactoring in edition of institutions. (217685 lines)
        Version 16.167.11:Mar 26, 2017	Code refactoring in edition of degrees. (217686 lines)
        Version 16.167.10:Mar 26, 2017	Code refactoring in edition of courses. (217688 lines)
        Version 16.167.9: Mar 26, 2017	Code refactoring in edition of centres. (217680 lines)
        Version 16.167.8: Mar 26, 2017	Changes in edition of holidays. (217681 lines)
        Version 16.167.7: Mar 26, 2017	Changes in edition of holidays. (217674 lines)
        Version 16.167.6: Mar 26, 2017	Changes in edition of holidays. (217667 lines)
        Version 16.167.5: Mar 26, 2017	Changes in edition of holidays. (217660 lines)
        Version 16.167.4: Mar 26, 2017	Changes in edition of holidays. (217643 lines)
        Version 16.167.3: Mar 25, 2017	Changes in edition of holidays. (217632 lines)
        Version 16.167.2: Mar 25, 2017	Contextual link to holidays is changed to a left-top contextual icon. (217577 lines)
        Version 16.167.1: Mar 25, 2017	Changes in icons related to calendar and holidays. (217595 lines)
Copy the following icons to icon public directory:
sudo cp -a icon/iconset/awesome/action64x64/calendar64x64.png /var/www/html/swad/icon/iconset/awesome/action64x64/
sudo cp -a icon/iconset/nuvola/action64x64/calendar64x64.png /var/www/html/swad/icon/iconset/nuvola/action64x64/
sudo cp -a icon/calendar64x64.png /var/www/html/swad/icon/
sudo cp -a icon/holiday64x64.png /var/www/html/swad/icon/

        Version 16.167:   Mar 25, 2017	Holidays removed from main manu. (217589 lines)
        Version 16.166.2: Mar 25, 2017	Contextual link to edit holidays in calendar. (217628 lines)
        Version 16.166.1: Mar 25, 2017	Contextual link to holidays in calendar. (217630 lines)
        Version 16.166:   Mar 24, 2017	Changes in layout related with institutional links.
					Fixed bug in swad.sql reported by user "proxa" in GitHub. (217613 lines)
        Version 16.165:   Mar 24, 2017	Allowed mail domains option is moved from system tab to messages tab. (217591 lines)
        Version 16.164.5: Mar 24, 2017	Link in notifications to allowed mail domains. (217632 lines)
        Version 16.164.4: Mar 24, 2017	Link to figures in types of degree. (217604 lines)
        Version 16.164.3: Mar 24, 2017	Fixed bug in types of degree.
					Types of degree in system tab restricted to system admins only. (217579 lines)
        Version 16.164.2: Mar 24, 2017	Code refactoring in types of degree. (217576 lines)
        Version 16.164.1: Mar 24, 2017	Statistics about types of degree depends on the selected scope. (217575 lines)
        Version 16.164:   Mar 24, 2017	New figure in statistics: types of degree. (217510 lines)
        Version 16.163:   Mar 24, 2017	Types of degree can be ordered by name and by number of degrees.
					Integer fields in database queries without single quotes. (217434 lines)
        Version 16.162:   Mar 23, 2017	Changes in layout of types of degree. (217330 lines)
        Version 16.161.8: Mar 23, 2017	Some help URLs translated to spanish. (217386 lines)
        Version 16.161.7: Mar 23, 2017	Some help URLs translated to spanish. (217385 lines)
        Version 16.161.6: Mar 23, 2017	Some help URLs translated to spanish. (217384 lines)
        Version 16.161.5: Mar 23, 2017	Some help URLs translated to spanish. (217383 lines)
        Version 16.161.4: Mar 22, 2017	Some help URLs translated to spanish. (217381 lines)
        Version 16.161.3: Mar 22, 2017	Some help URLs translated to spanish. (217381 lines)
        Version 16.161.2: Mar 22, 2017	Some help URLs translated to spanish. (217381 lines)
        Version 16.161.1: Mar 22, 2017	Changes in agenda. (217380 lines)
        Version 16.161:   Mar 22, 2017	Changes in agenda. (217404 lines)
        Version 16.160.2: Mar 21, 2017	Changes in agenda. Not finished. (217348 lines)
        Version 16.160.1: Mar 21, 2017	Changes in agenda. Not finished. (217347 lines)
        Version 16.160:   Mar 21, 2017	Changes in agenda. Not finished. (217413 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='1617';

        Version 16.159:   Mar 21, 2017	Changes in agenda. Not finished. (217511 lines)
        Version 16.158.1: Mar 21, 2017	Changes in agenda. Not finished. (217395 lines)
        Version 16.158:   Mar 21, 2017	Changes in agenda. Not finished.
					Fixed bug in user enrolment. (217394 lines)
Copy the following icons to icon public directory:
sudo cp -a past64x64.png /var/www/html/swad/icon/
sudo cp -a future64x64.png /var/www/html/swad/icon/

        Version 16.157.3: Mar 20, 2017	Contextual help in edition of custom record card fields. (217046 lines)
        Version 16.157.2: Mar 20, 2017	Some help URLs translated to spanish. (217041 lines)
        Version 16.157.1: Mar 20, 2017	Some help URLs translated to spanish. (217040 lines)
        Version 16.157:   Mar 17, 2017	Changes in edition of institutions, centres, degrees and courses. (217046 lines)
        Version 16.156.2: Mar 16, 2017	Fixed bug in passwords. (217076 lines)
        Version 16.156.1: Mar 15, 2017	Force vertical scrollbar. (217078 lines)
        Version 16.156:   Mar 15, 2017	Fixed bug in messages, reported by Francisco Oca�a Lara. (217063 lines)
        Version 16.155.35:Mar 15, 2017	Some help URLs translated to spanish. (217047 lines)
        Version 16.155.34:Mar 15, 2017	Changes in keywords. (217046 lines)
        Version 16.155.33:Mar 14, 2017	Some help URLs translated to spanish. (217042 lines)
        Version 16.155.32:Mar 14, 2017	Some help URLs translated to spanish. (217041 lines)
        Version 16.155.31:Mar 13, 2017	Fixed bug in connected users. (217040 lines)
        Version 16.155.30:Mar 13, 2017	Adjusting size of database fields. (217037 lines)
        Version 16.155.29:Mar 13, 2017	Adjusting size of database fields. (217034 lines)
					1 change necessary in database:
ALTER TABLE usr_emails CHANGE COLUMN E_mail E_mail VARCHAR(255) COLLATE latin1_general_ci NOT NULL;

        Version 16.155.28:Mar 13, 2017	Adjusting size of database fields. (217035 lines)
					7 changes necessary in database:
ALTER TABLE usr_data CHANGE COLUMN Surname1 Surname1 VARCHAR(511) COLLATE latin1_spanish_ci NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN Surname2 Surname2 VARCHAR(511) COLLATE latin1_spanish_ci NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN FirstName FirstName VARCHAR(511) COLLATE latin1_spanish_ci NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN Office Office VARCHAR(2047) NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN LocalAddress LocalAddress VARCHAR(2047) NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN FamilyAddress FamilyAddress VARCHAR(2047) NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN OriginPlace OriginPlace VARCHAR(2047) NOT NULL DEFAULT '';

        Version 16.155.27:Mar 13, 2017	Adjusting size of database fields. (217038 lines)
					4 changes necessary in database:
ALTER TABLE tst_answers CHANGE COLUMN ImageTitle ImageTitle VARCHAR(2047) NOT NULL;
ALTER TABLE tst_questions CHANGE COLUMN ImageTitle ImageTitle VARCHAR(2047) NOT NULL;
DROP INDEX TagTxt ON tst_tags;
ALTER TABLE tst_tags CHANGE COLUMN TagTxt TagTxt VARCHAR(2047) NOT NULL;

        Version 16.155.26:Mar 13, 2017	Adjusting size of database fields. (217009 lines)
					3 changes necessary in database:
ALTER TABLE timetable_crs CHANGE COLUMN Place Place VARCHAR(511) NOT NULL;
ALTER TABLE timetable_crs CHANGE COLUMN GroupName GroupName VARCHAR(2047) NOT NULL;
ALTER TABLE timetable_tut CHANGE COLUMN Place Place VARCHAR(511) NOT NULL;

        Version 16.155.25:Mar 13, 2017	Adjusting size of database fields. (216995 lines)
					1 change necessary in database:
ALTER TABLE surveys CHANGE COLUMN Title Title VARCHAR(2047) NOT NULL;

        Version 16.155.24:Mar 13, 2017	Adjusting size of database fields. (216988 lines)
					2 changes necessary in database:
ALTER TABLE social_comments CHANGE COLUMN ImageTitle ImageTitle VARCHAR(2047) NOT NULL;
ALTER TABLE social_posts CHANGE COLUMN ImageTitle ImageTitle VARCHAR(2047) NOT NULL;

        Version 16.155.23:Mar 13, 2017	Adjusting size of database fields. (216957 lines)
					4 changes necessary in database:
ALTER TABLE plugins CHANGE COLUMN Name Name VARCHAR(511) NOT NULL;
ALTER TABLE plugins CHANGE COLUMN Description Description VARCHAR(2047) NOT NULL;
ALTER TABLE plugins CHANGE COLUMN Logo Logo VARCHAR(31) NOT NULL;
ALTER TABLE plugins CHANGE COLUMN AppKey AppKey VARCHAR(31) NOT NULL;

        Version 16.155.22:Mar 13, 2017	Adjusting size of database fields. (216957 lines)
					2 changes necessary in database:
ALTER TABLE places CHANGE COLUMN ShortName ShortName VARCHAR(511) NOT NULL;
ALTER TABLE places CHANGE COLUMN FullName FullName VARCHAR(2047) NOT NULL;

        Version 16.155.21:Mar 13, 2017	Adjusting size of database fields. (216948 lines)
					1 change necessary in database:
ALTER TABLE pending_emails CHANGE COLUMN E_mail E_mail VARCHAR(255) COLLATE latin1_general_ci NOT NULL;

        Version 16.155.20:Mar 13, 2017	Adjusting size of database fields. (216944 lines)
					1 change necessary in database:
ALTER TABLE msg_content_deleted CHANGE COLUMN ImageTitle ImageTitle VARCHAR(2047) NOT NULL DEFAULT '';

        Version 16.155.19:Mar 13, 2017	Adjusting size of database fields. (216765 lines)
					1 change necessary in database:
ALTER TABLE msg_content CHANGE COLUMN ImageTitle ImageTitle VARCHAR(2047) NOT NULL DEFAULT '';

        Version 16.155.18:Mar 13, 2017	Adjusting size of database fields. (216761 lines)
					3 changes necessary in database:
DROP INDEX Info ON mail_domains;
ALTER TABLE mail_domains CHANGE COLUMN Domain Domain VARCHAR(255) NOT NULL;
ALTER TABLE mail_domains CHANGE COLUMN Info Info VARCHAR(2047) NOT NULL;

        Version 16.155.17:Mar 12, 2017	Adjusting size of database fields. (216755 lines)
					2 changes necessary in database:
ALTER TABLE log_search CHANGE COLUMN SearchStr SearchStr VARCHAR(2047) NOT NULL;
ALTER TABLE sessions CHANGE COLUMN SearchString SearchStr VARCHAR(2047) NOT NULL DEFAULT '';

        Version 16.155.16:Mar 12, 2017	Adjusting size of database fields. (216749 lines)
					1 change necessary in database:
ALTER TABLE log_comments CHANGE COLUMN Comments Comments TEXT NOT NULL;

        Version 16.155.15:Mar 12, 2017	Adjusting size of database fields. (216746 lines)
					2 changes necessary in database:
ALTER TABLE links CHANGE COLUMN ShortName ShortName VARCHAR(511) NOT NULL;
ALTER TABLE links CHANGE COLUMN FullName FullName VARCHAR(2047) NOT NULL;

        Version 16.155.14:Mar 12, 2017	Adjusting size of database fields. (216737 lines)
					2 changes necessary in database:
ALTER TABLE institutions CHANGE COLUMN ShortName ShortName VARCHAR(511) COLLATE latin1_spanish_ci NOT NULL;
ALTER TABLE institutions CHANGE COLUMN FullName FullName VARCHAR(2047) COLLATE latin1_spanish_ci NOT NULL;

        Version 16.155.13:Mar 12, 2017	Adjusting size of database fields. (216734 lines)
					1 change necessary in database:
ALTER TABLE holidays CHANGE COLUMN Name Name VARCHAR(2047) NOT NULL;

        Version 16.155.12:Mar 12, 2017	Fixed bug while sending a message to several users, reported by Javier Fern�ndez Baldomero. (216721 lines)
        Version 16.155.11:Mar 11, 2017	Adjusting size of database fields. (216711 lines)
					1 change necessary in database:
ALTER TABLE forum_post CHANGE COLUMN ImageTitle ImageTitle VARCHAR(2047) NOT NULL;

        Version 16.155.10:Mar 11, 2017	Adjusting size of database fields. (216695 lines)
					2 changes necessary in database:
ALTER TABLE exam_announcements CHANGE COLUMN CrsFullName CrsFullName VARCHAR(2047) NOT NULL;
ALTER TABLE exam_announcements CHANGE COLUMN ExamSession ExamSession VARCHAR(2047) NOT NULL;

        Version 16.155.9: Mar 11, 2017	Adjusting size of database fields. (216690 lines)
					2 changes necessary in database:
ALTER TABLE departments CHANGE COLUMN ShortName ShortName VARCHAR(511) NOT NULL;
ALTER TABLE departments CHANGE COLUMN FullName FullName VARCHAR(2047) NOT NULL;

        Version 16.155.8: Mar 11, 2017	Adjusting size of database fields. (216684 lines)
					2 changes necessary in database:
ALTER TABLE degrees CHANGE COLUMN ShortName ShortName VARCHAR(511) COLLATE latin1_spanish_ci NOT NULL;
ALTER TABLE degrees CHANGE COLUMN FullName FullName VARCHAR(2047) COLLATE latin1_spanish_ci NOT NULL;

        Version 16.155.7: Mar 11, 2017	Adjusting size of database fields. (216682 lines)
					1 change necessary in database:
ALTER TABLE deg_types CHANGE COLUMN DegTypName DegTypName VARCHAR(511) NOT NULL;

        Version 16.155.6: Mar 11, 2017	Adjusting size of database fields. (216677 lines)
					1 change necessary in database:
ALTER TABLE crs_record_fields CHANGE COLUMN FieldName FieldName VARCHAR(2047) NOT NULL;

        Version 16.155.5: Mar 11, 2017	Adjusting size of database fields. (216663 lines)
					2 changes necessary in database:
ALTER TABLE crs_grp CHANGE COLUMN GrpName GrpName VARCHAR(2047) NOT NULL;
ALTER TABLE crs_grp_types CHANGE COLUMN GrpTypName GrpTypName VARCHAR(2047) NOT NULL;

        Version 16.155.4: Mar 10, 2017	Adjusting size of database fields. (216626 lines)
					2 changes necessary in database:
ALTER TABLE courses CHANGE COLUMN ShortName ShortName VARCHAR(511) COLLATE latin1_spanish_ci NOT NULL;
ALTER TABLE courses CHANGE COLUMN FullName FullName VARCHAR(2047) COLLATE latin1_spanish_ci NOT NULL;

        Version 16.155.3: Mar 10, 2017	Adjusting size of database fields.
					Code refactoring related with renaming country name. (216620 lines)
					9 changes necessary in database:
ALTER TABLE countries CHANGE COLUMN Name_ca Name_ca VARCHAR(767) NOT NULL;
ALTER TABLE countries CHANGE COLUMN Name_de Name_de VARCHAR(767) NOT NULL;
ALTER TABLE countries CHANGE COLUMN Name_en Name_en VARCHAR(767) NOT NULL;
ALTER TABLE countries CHANGE COLUMN Name_es Name_es VARCHAR(767) NOT NULL;
ALTER TABLE countries CHANGE COLUMN Name_fr Name_fr VARCHAR(767) NOT NULL;
ALTER TABLE countries CHANGE COLUMN Name_gn Name_gn VARCHAR(767) NOT NULL;
ALTER TABLE countries CHANGE COLUMN Name_it Name_it VARCHAR(767) NOT NULL;
ALTER TABLE countries CHANGE COLUMN Name_pl Name_pl VARCHAR(767) NOT NULL;
ALTER TABLE countries CHANGE COLUMN Name_pt Name_pt VARCHAR(767) NOT NULL;

        Version 16.155.2: Mar 09, 2017	Adjusting size of database fields. (216596 lines)
					1 change necessary in database:
ALTER TABLE chat CHANGE COLUMN RoomCode RoomCode VARCHAR(16) NOT NULL;

        Version 16.155.1: Mar 09, 2017	Adjusting size of database fields. (216593 lines)
					2 changes necessary in database:
ALTER TABLE centres CHANGE COLUMN ShortName ShortName VARCHAR(511) COLLATE latin1_spanish_ci NOT NULL;
ALTER TABLE centres CHANGE COLUMN FullName FullName VARCHAR(2047) COLLATE latin1_spanish_ci NOT NULL;

        Version 16.155:   Mar 09, 2017	Adjusting size of database fields.
					Code refactoring related with renaming some fields. (216583 lines)
					2 changes necessary in database:
ALTER TABLE banners CHANGE COLUMN ShortName ShortName VARCHAR(511) NOT NULL;
ALTER TABLE banners CHANGE COLUMN FullName FullName VARCHAR(2047) NOT NULL;

        Version 16.154.4: Mar 09, 2017	Adjusting size of database fields.
					Bug fixing related to agendas, assignments and attendance events. (216504 lines)
					1 change necessary in database:
ALTER TABLE att_events CHANGE COLUMN Title Title VARCHAR(2047) NOT NULL;

        Version 16.154.3: Mar 08, 2017	Bug fixing related to folders in assignments and file browsers. (216473 lines)
        Version 16.154.2: Mar 08, 2017	Adjusting size of database fields. (216466 lines)
					2 changes necessary in database:
ALTER TABLE assignments CHANGE COLUMN Title Title VARCHAR(2047) NOT NULL;
ALTER TABLE assignments CHANGE COLUMN Folder Folder VARBINARY(255) NOT NULL;

        Version 16.154.1: Mar 08, 2017	Adjusting size of database fields. (216462 lines)
					2 changes necessary in database:
ALTER TABLE agendas CHANGE COLUMN Event Event VARCHAR(2047) NOT NULL;
ALTER TABLE agendas CHANGE COLUMN Location Location VARCHAR(2047) NOT NULL;

        Version 16.154:   Mar 08, 2017	Adjusting size of database fields.
					Code refactoring related to number of chars and bytes. (216458 lines)
        Version 16.153:   Mar 08, 2017	Code refactoring related to number of chars and bytes. (216450 lines)
        Version 16.152:   Mar 07, 2017	Code refactoring related to number of chars and bytes. (216428 lines)
        Version 16.151:   Mar 07, 2017	Code refactoring related to number of chars and bytes. (216422 lines)
        Version 16.150:   Mar 07, 2017	Code refactoring related to number of chars and bytes. (216409 lines)
        Version 16.149:   Mar 07, 2017	Code refactoring related to number of chars and bytes. (216385 lines)
        Version 16.148:   Mar 06, 2017	Code refactoring in notifications. (216393 lines)
        Version 16.147.11:Mar 06, 2017	Changes in names of zipped files. (216388 lines)
        Version 16.147.10:Mar 06, 2017	Changes in layout of users' listing. (216394 lines)
        Version 16.147.9: Mar 06, 2017	Changes in layout of header. (216389 lines)
        Version 16.147.8: Mar 05, 2017	Changes in layout of timetable. (216374 lines)
        Version 16.147.7: Mar 05, 2017	Changes in layout of social timeline. (216372 lines)
        Version 16.147.6: Mar 05, 2017	Changes in layout of record card. (216370 lines)
        Version 16.147.5: Mar 05, 2017	Changes in layout of record card. (216346 lines)
        Version 16.147.4: Mar 05, 2017	Changes in layout of enrolment requesters.
					Code refactoring writing user's name. (216345 lines)
        Version 16.147.3: Mar 05, 2017	Changes in layout of followed users, users to follow and connected users. (216365 lines)
        Version 16.147.2: Mar 05, 2017	Changes in layout of notice author. (216316 lines)
        Version 16.147.1: Mar 05, 2017	Changes in layout of notice author. (216300 lines)
        Version 16.147:   Mar 04, 2017	Changes in layout of file browser. (216292 lines)
        Version 16.146.9: Mar 04, 2017	Changes in layout of assignments and works. (216271 lines)
        Version 16.146.8: Mar 04, 2017	Changes in layout of connected users. (216284 lines)
        Version 16.146.7: Mar 04, 2017	Changes in layout of my courses. (216339 lines)
        Version 16.146.6: Mar 04, 2017	Changes in layout of countries. (216331 lines)
        Version 16.146.5: Mar 04, 2017	Code refactoring related with author of assignments, attendance events and surveys. (216319 lines)
        Version 16.146.4: Mar 04, 2017	Changes in layout of surveys. (216349 lines)
        Version 16.146.3: Mar 04, 2017	Changes in layout of edition of institutions, centres and degrees. (216363 lines)
        Version 16.146.2: Mar 04, 2017	Changes in layout of authors. (216354 lines)
        Version 16.146.1: Mar 03, 2017	Changes in layout of assigments. (216350 lines)
        Version 16.146:   Mar 03, 2017	Changes in layout of class photos. (216351 lines)
        Version 16.145.2: Mar 02, 2017	Some help URLs translated to spanish. (216338 lines)
        Version 16.145.1: Mar 02, 2017	Changes in several help URLs. (216337 lines)
        Version 16.145:   Mar 02, 2017	Support for languages in help.
					New module swad_help_URL.c. (216336 lines)
        Version 16.144.6: Mar 02, 2017	Code refactoring in user's photo caption. (213439 lines)
        Version 16.144.5: Mar 02, 2017	Label with the user's institution under her/his enlarged photo. (213441 lines)
        Version 16.144.4: Mar 01, 2017	Fixed bug in link to another user's QR code from record card. (213338 lines)
        Version 16.144.3: Mar 01, 2017	Button to create a new place when no places have been created. (213334 lines)
        Version 16.144.2: Mar 01, 2017	Button to create a new group when no groups have been created. (213300 lines)
        Version 16.144.1: Mar 01, 2017	Zoom photo even if user has no photo.
					Country name is cached. (213290 lines)
        Version 16.144:   Mar 01, 2017	Label with the user's country under her/his enlarged photo. (213265 lines)
        Version 16.143.4: Mar 01, 2017	New place for QR icon in user's record card. (213249 lines)
        Version 16.143.3: Feb 28, 2017	Changes in layout of page head. (213238 lines)
        Version 16.143.2: Feb 28, 2017	Fixed bug in user timeline. (213237 lines)
        Version 16.143.1: Feb 28, 2017	Changed link to help on timeline. (213232 lines)
        Version 16.143:   Feb 28, 2017	Search countries. (213231 lines)
        Version 16.142.1: Feb 27, 2017	Public activity is renamed as timeline. (213085 lines)
        Version 16.142:   Feb 27, 2017	Searches are registered anonymously. (213077 lines)
					1 change necessary in database:
CREATE TABLE IF NOT EXISTS log_search (LogCod INT NOT NULL,SearchStr VARCHAR(255) NOT NULL,UNIQUE INDEX(LogCod));
					If you prefer MyISAM tables:
ALTER TABLE log_search ENGINE=MyISAM;

        Version 16.141.2: Feb 26, 2017	Search from top of page always uses system scope. (213039 lines)
        Version 16.141.1: Feb 26, 2017	Fixed bug in range of dates. (213056 lines)
        Version 16.141  : Feb 26, 2017	Default range of dates for stats starts at HH:MM:SS=00:00:00 and ends at HH:MM:SS=23:59:59. (213053 lines)
        Version 16.140.5: Feb 25, 2017	Days in recent log changed from 8 to 15. (213008 lines)
        Version 16.140.4: Feb 24, 2017	Fixed bug in T/F test questions, reported by Nuria Torres Rosell. (213006 lines)
        Version 16.140.3: Feb 24, 2017	Changed refreshing of timeline from 30 to 10 seconds.
					Code refactoring in timeline when I do not follow any user. (213006 lines)
        Version 16.140.2: Feb 24, 2017	Changed text of users to view in global timeline. (212994 lines)
        Version 16.140.1: Feb 24, 2017	Hidden parameter with users to view in all actions related to global timeline.
					Code refactoring of forms in social timeline. (212993 lines)
        Version 16.140:   Feb 24, 2017	Two global timelines available: users I follow and all users. Not finished. (213039 lines)
        Version 16.139.6: Feb 24, 2017	Code refactoring in selector of which forums to display. (? lines)
        Version 16.139.5: Feb 22, 2017	Changed contextual links in my profile. (212832 lines)
        Version 16.139.4: Feb 17, 2017	Fixed bug in social timeline. (212835 lines)
        Version 16.139.3: Feb 17, 2017	Fixed bug in social timeline. (212835 lines)
        Version 16.139.2: Feb 17, 2017	Code refactoring in actions. (212836 lines)
        Version 16.139.1: Feb 17, 2017	Code refactoring in parameters related to social timeline. (212817 lines)
        Version 16.139:   Feb 17, 2017	Fixed bugs showing my public profile.
					Fixed bugs in user timeline. (212819 lines)
					1 change necessary in database:
DELETE FROM actions WHERE ActCod='1637';

        Version 16.138:   Feb 17, 2017	On right column show only users to follow with visible photo. (212873 lines)
        Version 16.137:   Feb 17, 2017	Link to view my public profile. (212808 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1637','es','N','Ver mi perfil p&uacute;blico');

        Version 16.136:   Feb 16, 2017	Suggested users to follow on right column. (212646 lines)
        Version 16.135.7: Feb 16, 2017	Code refactoring in users to follow. (212512 lines)
        Version 16.135.6: Feb 13, 2017	Changes in contextual help after changes in tabs. (212501 lines)
        Version 16.135.5: Feb 11, 2017	Change in button to register teacher when no teachers found.
					Changed color in statistics of countries. (212503 lines)
        Version 16.135.4: Feb 11, 2017	Force filling of ID in form to check if account exists. (212502 lines)
        Version 16.135.3: Feb 09, 2017	Code refactoring related to enroling teachers and students. (212499 lines)
        Version 16.135.2: Feb 09, 2017	Changes in layout of stats on degrees. (212456 lines)
        Version 16.135.1: Feb 09, 2017	Link from current date to calendar depending on scope. (212461 lines)
        Version 16.135:   Feb 09, 2017	Changes in layout of tabs. (212452 lines)
        Version 16.134.3: Feb 09, 2017	Changes in visible tabs depending on scope. (212451 lines)
        Version 16.134.2: Feb 09, 2017	Changed country, institution, centre and degree configuration icons. (212447 lines)
        Version 16.134.1: Feb 09, 2017	Link from current month to calendar depending on scope. (212446 lines)
Copy the following icons to icon public directory:
sudo cp icon/iconset/awesome/action64x64/crs64x64.gif /var/www/html/swad/icon/iconset/awesome/action64x64/crs64x64.gif
sudo cp icon/iconset/nuvola/action64x64/crs64x64.gif /var/www/html/swad/icon/iconset/nuvola/action64x64/crs64x64.gif
sudo cp icon/iconset/awesome/action64x64/fil64x64.gif /var/www/html/swad/icon/iconset/awesome/action64x64/fil64x64.gif
sudo cp icon/iconset/nuvola/action64x64/fil64x64.gif /var/www/html/swad/icon/iconset/nuvola/action64x64/fil64x64.gif

        Version 16.134:   Feb 09, 2017	New calendars options. (212428 lines)
					18 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1622','es','N','Ver calendario plataforma');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1623','es','N','Imprimir calendario plataforma');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1624','es','N','Cambiar 1er. d&iacute;a calendario plataforma');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1625','es','N','Ver calendario pa&iacute;s');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1626','es','N','Imprimir calendario pa&iacute;s');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1627','es','N','Cambiar 1er. d&iacute;a calendario pa&iacute;s');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1628','es','N','Ver calendario instituci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1629','es','N','Imprimir calendario instituci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1630','es','N','Cambiar 1er. d&iacute;a calendario instituci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1631','es','N','Ver calendario centro');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1632','es','N','Imprimir calendario centro');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1633','es','N','Cambiar 1er. d&iacute;a calendario centro');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1634','es','N','Ver calendario titulaci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1635','es','N','Imprimir calendario titulaci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1636','es','N','Cambiar 1er. d&iacute;a calendario titulaci&oacute;n');
UPDATE actions SET Txt='Ver calendario asignatura' WHERE ActCod='16' AND Language='es';
UPDATE actions SET Txt='Imprimir calendario asignatura' WHERE ActCod='71' AND Language='es';
UPDATE actions SET Txt='Cambiar 1er. d&iacute;a calendario asignatura' WHERE ActCod='1485' AND Language='es';

        Version 16.133.1: Feb 08, 2017	Timetable option moved to the end of the menu. (212071 lines)
        Version 16.133:   Feb 08, 2017	New tab "Files".
					New permissions for actions. (212070 lines)
        Version 16.132.15:Feb 06, 2017	Bug fixing related to AJAX and automatic refresh. (211916 lines)
        Version 16.132.14:Feb 05, 2017	Bug fixing related to AJAX and automatic refresh. (211916 lines)
        Version 16.132.13:Feb 01, 2017	Global connected in right column is shown only for system admins. (211904 lines)
        Version 16.132.12:Feb 01, 2017	Code refactoring and bug fixing in parameters related to users list type. (211900 lines)
        Version 16.132.11:Feb 01, 2017	Help on indicators of courses. (211900 lines)
        Version 16.132.10:Feb 01, 2017	Help on stats about visits. (211896 lines)
        Version 16.132.9: Jan 31, 2017	Help on degrees stats. (211890 lines)
        Version 16.132.8: Jan 31, 2017	Help on remove all students in a course. (211884 lines)
        Version 16.132.7: Jan 31, 2017	Changes in layout of removal of all students in a course. (211880 lines)
        Version 16.132.6: Jan 31, 2017	Help on administration of students. (211895 lines)
        Version 16.132.5: Jan 30, 2017	Fixed bug in checking consent on dangerous action. (211888 lines)
        Version 16.132.4: Jan 30, 2017	Changes in connected users. (211887 lines)
        Version 16.132.3: Jan 30, 2017	Code refactoring in connected users. (211881 lines)
        Version 16.132.2: Jan 30, 2017	Fixed bug in file browser. (211911 lines)
        Version 16.132.1: Jan 29, 2017	Fixed bug in listing of users. (211916 lines)
        Version 16.132:   Jan 29, 2017	Code refactoring in unsigned parameters. (211915 lines)
        Version 16.131:   Jan 29, 2017	Code refactoring in unsigned parameters. Not finished. (211856 lines)
        Version 16.130:   Jan 29, 2017	Code refactoring in unsigned parameters. Not finished. (211843 lines)
        Version 16.129:   Jan 28, 2017	Code refactoring in Y/N parameters. (211974 lines)
        Version 16.128:   Jan 28, 2017	Code refactoring in long parameters. (212037 lines)
        Version 16.127.3: Jan 28, 2017	Fixed bug in permissions to change another user's data. (212127 lines)
        Version 16.127.2: Jan 28, 2017	Fixed bug creating a new account. (212124 lines)
        Version 16.127.1: Jan 28, 2017	Fixed bug in listing of users. (212125 lines)
        Version 16.127:   Jan 28, 2017	Code refactoring. (212118 lines)
        Version 16.126.6: Jan 27, 2017	Fixed bug related with list of students. (212098 lines)
        Version 16.126.5: Jan 27, 2017	Changes in permissions to view/change another user's data. (212093 lines)
        Version 16.126.4: Jan 27, 2017	Code refactoring related to permissions to view/change another user's data. (212105 lines)
        Version 16.126.3: Jan 27, 2017	Fixed bug in search of user, reported by Javier Fern�ndez Baldomero. (212086 lines)
        Version 16.126.2: Jan 27, 2017	When creating a new user, the record card don't have contextual icons. (212083 lines)
        Version 16.126.1: Jan 27, 2017	A teacher can create and register a new teacher directly in a course. (212080 lines)
        Version 16.126:   Jan 27, 2017	Code refactoring related to permissions to view/change another user's data. (212086 lines)
        Version 16.125:   Jan 27, 2017	Code refactoring related to permissions to view/change another user's data. (212152 lines)
        Version 16.124:   Jan 26, 2017	Code refactoring related to permissions to view/change another user's data. (212163 lines)
        Version 16.123.4: Jan 26, 2017	Surname1 and first name can not be deleted when editing user's data. (212000 lines)
        Version 16.123.3: Jan 25, 2017	Code refactoring in edition of user's record. (211987 lines)
        Version 16.123.2: Jan 25, 2017	Teachers can edit names of students/teachers with no name/email. (211989 lines)
        Version 16.123.1: Jan 20, 2017	Minor change in text to show agenda. (211952 lines)
        Version 16.123:   Jan 20, 2017	Link to agenda in current day (left-top date-time). (211950 lines)
        Version 16.122:   Jan 20, 2017	Link to calendar in current month (left-top date-time). (211937 lines)
        Version 16.121:   Jan 19, 2017	Fixed bug in groups selection, reported by Eva Mart�nez Ortigosa.
					Code refactoring in groups selected. (211915 lines)
        Version 16.120.2: Jan 19, 2017	Changed the question to confirm user's enrolment. (211954 lines)
        Version 16.120.1: Jan 19, 2017	From now on, a teacher A can enrol another user B as a teacher in their courses, although user B is not yet a teacher in any course. (? lines)
        Version 16.120:   Jan 19, 2017	Fixed bug when limiting length of links. (211934 lines)
        Version 16.119.4: Jan 18, 2017	Changes in layout of current data and time. (211882 lines)
        Version 16.119.3: Jan 17, 2017	Changes in layout of current data and time. (211881 lines)
        Version 16.119.2: Jan 17, 2017	Fixed bug in web service related to attendance. (211879 lines)
        Version 16.119.1: Jan 17, 2017	Fixed bug in attendance. (211877 lines)
        Version 16.119:   Jan 17, 2017	Changes in layout of current data and time.
					Fixed bug in attendance, reported by Javier Fern�ndez Baldomero. (211874 lines)
        Version 16.118.2: Jan 17, 2017	Fix bug in string concatenation. (211854 lines)
        Version 16.118.1: Jan 17, 2017	Code refactoring related to string concatenation. (211850 lines)
        Version 16.118:   Jan 17, 2017	Code refactoring related to string copy. (211676 lines)
        Version 16.117.1: Jan 16, 2017	Code refactoring related to string concatenation. (211229 lines)
        Version 16.117:   Jan 16, 2017	Code refactoring related to string concatenation. (211244 lines)
        Version 16.116:   Jan 15, 2017	Code refactoring related to string copy. (211195 lines)
        Version 16.115:   Jan 15, 2017	Code refactoring related to string copy. (211046 lines)
        Version 16.114.3: Jan 15, 2017	Fixed bug in chat. (211412 lines)
        Version 16.114.2: Jan 15, 2017	Fixed bug in file browser. (211412 lines)
        Version 16.114.1: Jan 15, 2017	Fixed bug in string concatenation. (211411 lines)
        Version 16.114:   Jan 14, 2017	Fixed bug in dates, reported by Jos� Luis Bernier Villamor.
					Javascript code refactoring related to dates. (211490 lines)
        Version 16.113:   Jan 13, 2017	Some strcpy changed to strncpy. (211497 lines)
        Version 16.112:   Jan 13, 2017	Some strcpy changed to strncpy. (211415 lines)
        Version 16.111.16:Dec 30, 2016	Fixed minor bug in user's ID, reported by Coverity. (211185 lines)
        Version 16.111.15:Dec 30, 2016	Fixed bug in forums, reported by Coverity. (211182 lines)
        Version 16.111.14:Dec 29, 2016	Fixed bug in attendance events. (211189 lines)
        Version 16.111.13:Dec 29, 2016	Fixed bug in attendance events, reported by Coverity. (211173 lines)
        Version 16.111.12:Dec 29, 2016	Fixed minor bug in groups, reported by Coverity. (? lines)
        Version 16.111.11:Dec 29, 2016	Fixed minor bug inserting links to nicknames, reported by Coverity. (211149 lines)
        Version 16.111.10:Dec 29, 2016	Fixed minor bug in groups, reported by Coverity. (211146 lines)
        Version 16.111.9: Dec 29, 2016	Fixed bugs in lstat, reported by Coverity. (211138 lines)
        Version 16.111.8: Dec 28, 2016	Fixed bug in notifications, reported by Coverity. (211119 lines)
        Version 16.111.7: Dec 28, 2016	Fixed minor bug in groups, reported by Coverity. (211114 lines)
        Version 16.111.6: Dec 28, 2016	Fixed bug in assigments, reported by Coverity. (211109 lines)
        Version 16.111.5: Dec 28, 2016	Changes in CSS related to tabs. (211084 lines)
        Version 16.111.4: Dec 28, 2016	Fixed bug in file browser. (211336 lines)
        Version 16.111.3: Dec 28, 2016	Unused code removed in file browsers. (211335 lines)
        Version 16.111.2: Dec 28, 2016	Code refactoring in groups and course. (211352 lines)
        Version 16.111.1: Dec 28, 2016	Code refactoring in file browsers. (211352 lines)
        Version 16.111:   Dec 28, 2016	Code refactoring in file browsers. (211331 lines)
        Version 16.110:   Dec 27, 2016	Code refactoring in file browsers. (211340 lines)
        Version 16.109.1: Dec 27, 2016	Code refactoring in file browsers. (211460 lines)
        Version 16.109:   Dec 27, 2016	Code refactoring in file browsers. (211458 lines)
        Version 16.108:   Dec 27, 2016	Changes in layout.
					Changes in behaviour of labels in forms. (211608 lines)
        Version 16.107.10:Dec 27, 2016	Changes in behaviour of labels in forms. (211606 lines)
        Version 16.107.9: Dec 27, 2016	Changes in edition of new notice. (211607 lines)
        Version 16.107.8: Dec 27, 2016	Changes in behaviour of labels in forms. (211598 lines)
        Version 16.107.7: Dec 27, 2016	Code refactoring in forums. (211594 lines)
        Version 16.107.6: Dec 27, 2016	Changes in behaviour of labels in forms. (211593 lines)
        Version 16.107.5: Dec 26, 2016	Changes in behaviour of labels in forms. (211540 lines)
        Version 16.107.4: Dec 26, 2016	Code refactoring in announcements. (211532 lines)
        Version 16.107.3: Dec 26, 2016	Changes in behaviour of labels in forms. (211531 lines)
        Version 16.107.2: Dec 26, 2016	Changes in behaviour of labels in forms. (211525 lines)
        Version 16.107.1: Dec 26, 2016	Changes in behaviour of labels in forms.
                                        Code refactoring in tests. (211515 lines)
        Version 16.107:   Dec 26, 2016	Changes in behaviour of labels in forms.
                                        Code refactoring in tests. (211510 lines)
        Version 16.106.20:Dec 26, 2016	Changes in warning about next time to take a test. (211486 lines)
        Version 16.106.19:Dec 26, 2016	Changes in login form (autofocus). (211494 lines)
        Version 16.106.18:Dec 25, 2016	Changes in behaviour of labels in forms. (211496 lines)
        Version 16.106.17:Dec 25, 2016	Changes in behaviour of labels in forms. (211486 lines)
        Version 16.106.16:Dec 25, 2016	Changes in behaviour of labels in forms. (211473 lines)
        Version 16.106.15:Dec 25, 2016	Changes in behaviour of labels in forms. (211460 lines)
        Version 16.106.14:Dec 25, 2016	Changes in behaviour of labels in forms. (211423 lines)
        Version 16.106.13:Dec 24, 2016	Changes in behaviour of labels in forms. (211421 lines)
        Version 16.106.12:Dec 24, 2016	Changes in behaviour of labels in forms. (211418 lines)
        Version 16.106.11:Dec 24, 2016	Changes in behaviour of labels in forms. (211415 lines)
        Version 16.106.10:Dec 24, 2016	Changes in behaviour of labels in forms. (211412 lines)
        Version 16.106.9: Dec 24, 2016	Changes in layout of preferences. (211400 lines)
        Version 16.106.8: Dec 24, 2016	Changes in behaviour of labels in forms. (211404 lines)
        Version 16.106.7: Dec 24, 2016	Changes in behaviour of labels in forms. (211401 lines)
        Version 16.106.6: Dec 24, 2016	Changes in layout when changing language. (211400 lines)
        Version 16.106.5: Dec 24, 2016	Changes in layout of preferences. (211393 lines)
        Version 16.106.4: Dec 24, 2016	Changes in behaviour of labels in forms. (211408 lines)
        Version 16.106.3: Dec 24, 2016	Changes in behaviour of labels in forms. (211405 lines)
        Version 16.106.2: Dec 24, 2016	Changes in behaviour of labels in forms. (211409 lines)
        Version 16.106.1: Dec 24, 2016	Changes in stats about preferences. (211408 lines)
        Version 16.106:   Dec 23, 2016	Changes in preferences. (211407 lines)
        Version 16.105.13:Dec 23, 2016	Changes in behaviour of labels in forms. (211411 lines)
        Version 16.105.12:Dec 21, 2016	Changes in behaviour of labels in forms. (211408 lines)
        Version 16.105.11:Dec 21, 2016	Changes in behaviour of labels in forms. (211323 lines)
        Version 16.105.10:Dec 21, 2016	Changes in behaviour of labels in forms. (211320 lines)
        Version 16.105.9: Dec 21, 2016	Changes in behaviour of labels in forms. (211316 lines)
        Version 16.105.8: Dec 20, 2016	Changes in behaviour of labels in forms. (211313 lines)
        Version 16.105.7: Dec 20, 2016	Changes in behaviour of labels in forms. (211302 lines)
        Version 16.105.6: Dec 20, 2016	Changes in behaviour of labels in forms. (211300 lines)
        Version 16.105.5: Dec 20, 2016	Changes in behaviour of labels in forms. (211299 lines)
        Version 16.105.4: Dec 20, 2016	Changes in behaviour of labels in forms. (211281 lines)
        Version 16.105.3: Dec 20, 2016	Changes in contextual checkboxes. (211280 lines)
        Version 16.105.2: Dec 20, 2016	Changes in behaviour of labels in forms. (211279 lines)
        Version 16.105.1: Dec 20, 2016	Changes in behaviour of labels in forms. (211244 lines)
        Version 16.105:   Dec 20, 2016	Changes in behaviour of labels in forms.
					Changes in layout for Safari browser. (211237 lines)
        Version 16.104.3: Dec 19, 2016	Changes in search input fields. (211196 lines)
        Version 16.104.2: Dec 19, 2016	New links to phones. (211194 lines)
        Version 16.104.1: Dec 19, 2016	New address element. (211187 lines)
        Version 16.104:   Dec 19, 2016	Change in checkbox to confirm that I have read a course info.
					Changes in layout of lists. (211183 lines)
        Version 16.103.7: Dec 18, 2016	Change in checkbox to force students to read couse info. (211206 lines)
        Version 16.103.6: Dec 18, 2016	Some div elements changed to header, footer, aside and nav elements. (211212 lines)
        Version 16.103.5: Dec 17, 2016	Code refactoring in user enrolment. (211207 lines)
        Version 16.103.4: Dec 17, 2016	Code refactoring in user enrolment. (211170 lines)
        Version 16.103.3: Dec 17, 2016	Code refactoring in user enrolment. (211149 lines)
        Version 16.103.2: Dec 17, 2016	Code refactoring in user enrolment. (211133 lines)
        Version 16.103.1: Dec 16, 2016	Changes in layout of tabs. (211123 lines)
        Version 16.103:   Dec 16, 2016	Contextual help on course info. (211130 lines)
        Version 16.102:   Dec 15, 2016	Fixed bug in mail to students, reported by GitHub user raistmaj.
					Fixed bug in file of marks, reported by GitHub user raistmaj.
					Fixed bug in course indicators, reported by GitHub user raistmaj.
					Fixed bug related with user roles, reported by GitHub user raistmaj. (211011 lines)
        Version 16.101:   Dec 14, 2016	Interval in range of times is 5 minutes when no seconds needed.
					Code refactoring related to start and end date-time. (210806 lines)
        Version 16.100.1: Dec 13, 2016	Fixed bugs checking if users share courses. (210828 lines)
        Version 16.100:   Dec 13, 2016	Changes in layout of list of attendance events.
					Code refactoring related with roles. (210820 lines)
        Version 16.99.1:  Dec 13, 2016	Changes in translation of start and end.
					Changes in layout of assignments and surveys. (210742 lines)
        Version 16.99:    Dec 12, 2016	Changes in script to write local date and time. (210782 lines)
        Version 16.98:    Dec 12, 2016	Changes in help about homework.
					Changes in script to write local date and time. (210739 lines)
        Version 16.97.1:  Dec 12, 2016	Changes in translation about announcements of exams. (210748 lines)
        Version 16.97:    Dec 12, 2016	Exam announcements can be hidden. (210739 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1620','es','N','Ocultar convocatoria de examen');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1621','es','N','Mostrar convocatoria de examen');

        Version 16.96:    Dec 11, 2016	Now removing an announcement of exam requires confirmation. (210563 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1619','es','N','Solicitar eliminaci&oacute;n de convocatoria');

        Version 16.95.2:  Dec 11, 2016	Code refactoring related with notices. (210514 lines)
        Version 16.95.1:  Dec 11, 2016	Code refactoring related with global announcements. (210534 lines)
        Version 16.95:    Dec 11, 2016	Code refactoring related with dates. (210559 lines)
        Version 16.94.2:  Dec 11, 2016	Contextual help on calendar.
					Changes in exam announcements. (210421 lines)
        Version 16.94.1:  Dec 10, 2016	Fixed bug in layout of breadcrumb. (210417 lines)
        Version 16.94:    Dec 10, 2016	Changes in layout for responsive design. (210421 lines)
        Version 16.93:    Dec 10, 2016	Changes in layout of heading rows and tabs. (210308 lines)
        Version 16.92.5:  Dec 10, 2016	Contextual help on calendar. (210318 lines)
        Version 16.92.4:  Dec 09, 2016	Fixed bug in edition of user's agenda. (210313 lines)
        Version 16.92.3:  Dec 09, 2016	Contextual help on user's public profile. (210310 lines)
        Version 16.92.2:  Dec 09, 2016	New icons to view user's profile ant to view user's record card in public agenda. (210308 lines)
        Version 16.92.1:  Dec 09, 2016	New icon to view user's profile in user's record card. (210277 lines)
        Version 16.92:    Dec 09, 2016	Teachers' record cards can be viewed by users sharing courses with them even if no course is selected.
					Code refactoring in user's record card. (210269 lines)
        Version 16.91.6:  Dec 09, 2016	Code refactoring in agenda. (210128 lines)
        Version 16.91.5:  Dec 08, 2016	Removed author in agenda events. (210148 lines)
        Version 16.91.4:  Dec 07, 2016	Default ordering in agendas is start date. (210189 lines)
        Version 16.91.3:  Dec 07, 2016	Dates in agendas are ordered from old to new. (210188 lines)
        Version 16.91.2:  Dec 07, 2016	Changes in permissions to view public agendas.
					Changes in help on public agendas. (210187 lines)
        Version 16.91.1:  Dec 07, 2016	Fixed bug in notifications. (210192 lines)
        Version 16.91:    Dec 07, 2016	Changes in QR codes. (210204 lines)
        Version 16.90.3:  Dec 06, 2016	Changes related with agenda QR. (210156 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1618','es','N','Imprimir c&oacute;digo QR agenda');

        Version 16.90.2:  Dec 06, 2016	Changes in JavaScript code to write date-time. (210149 lines)
        Version 16.90.1:  Dec 06, 2016	Code refactoring in agenda. (210147 lines)
        Version 16.90:    Dec 06, 2016	Changes in layout of agenda. (210147 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1617','es','N','Ver mi agenda p&uacute;blica');

        Version 16.89.2:  Dec 06, 2016	Fixed bugs in agenda.
					Public agenda available for any user, not only teachers. (210167 lines)
        Version 16.89.1:  Dec 06, 2016	Changes in help on agenda. (210166 lines)
        Version 16.89:    Dec 06, 2016	Agenda events now are replaced by agenda events today. (210139 lines)
        Version 16.88.1:  Dec 06, 2016	Fixed bugs in agenda. (210118 lines)
        Version 16.88:    Dec 06, 2016	Current events in user's agenda are displayed in a highlighted box. (210102 lines)
        Version 16.87.2:  Dec 05, 2016	Icon to edit my agenda. (209987 lines)
        Version 16.87.1:  Dec 05, 2016	Temporary permissions to debug agendas. (209969 lines)
Copy the following icons to icon public directory:
sudo cp icon/lock-on64x64.png /var/www/html/swad/icon/
sudo cp icon/lock-off64x64.png /var/www/html/swad/icon/
sudo cp icon/unlock-off64x64.png /var/www/html/swad/icon/
sudo cp icon/unlock-on64x64.png /var/www/html/swad/icon/
sudo cp icon/lockunlock64x64.png /var/www/html/swad/icon/
sudo cp icon/unlock64x64.png /var/www/html/swad/icon/
sudo cp icon/iconset/awesome/action64x64/hierarchy64x64.png /var/www/html/swad/icon/iconset/awesome/action64x64/
sudo cp icon/iconset/nuvola/action64x64/hierarchy64x64.png /var/www/html/swad/icon/iconset/nuvola/action64x64/
sudo cp icon/hierarchy64x64.png /var/www/html/swad/icon/
sudo cp icon/myhierarchy64x64.png /var/www/html/swad/icon/
					If you prefer MyISAM tables:
ALTER TABLE agendas ENGINE=MyISAM;

        Version 16.87:    Dec 05, 2016	Change language after log in to view another user's public agenda. (209952 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1616','es','N','Cambiar idioma para ver agenda otro usuario');

        Version 16.86.11: Dec 05, 2016	Link to QR in user's agenda. (209908 lines)
        Version 16.86.10: Dec 05, 2016	Code refactoring related to user's QR (nickname). (209904 lines)
        Version 16.86.9:  Dec 05, 2016	Link to QR in user's agenda. Not finished. (209889 lines)
        Version 16.86.8:  Dec 05, 2016	Changes in layout of user's agenda. (209864 lines)
        Version 16.86.7:  Dec 05, 2016	Changes in icons for open/closed and public/private. (209852 lines)
        Version 16.86.6:  Dec 04, 2016	Removed contextual icons to change which groups to show. (209876 lines)
        Version 16.86.5:  Dec 04, 2016	Changes in layout of forms to change which groups to show / first day of week. (209913 lines)
        Version 16.86.4:  Dec 04, 2016	Contextual icons to change which groups to show. (209864 lines)
        Version 16.86.3:  Dec 04, 2016	Changed icons related to hierarchy. (209815 lines)
        Version 16.86.2:  Dec 04, 2016	Fixed bugs in my timetable. (209814 lines)
        Version 16.86.1:  Dec 04, 2016	New action to show another user's agenda after log in. (209806 lines)
					4 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1612','es','N','Ocultar evento agenda');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1613','es','N','Mostrar evento agenda');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1614','es','N','Formulario log in para ver agenda');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1615','es','N','Ver agenda otro usuario tras log in');

        Version 16.86:    Dec 04, 2016	New action to put form to log in and show another user's agenda. (209781 lines)
        Version 16.85.1:  Dec 03, 2016	Changed title of public agenda. (209722 lines)
        Version 16.85:    Dec 03, 2016	Form to choice between all events / only public events in agenda. (209721 lines)
        Version 16.84.2:  Dec 03, 2016	Only hours and minutes (seconds = 0) in agenda events. (209606 lines)
        Version 16.84.1:  Dec 03, 2016	Remove user's agenda when removing user's account. (? lines)
        Version 16.84:    Dec 03, 2016	Agenda events can be hidden. (209580 lines)
					3 changes necessary in database:
ALTER TABLE agendas ADD COLUMN Hidden ENUM('N','Y') NOT NULL DEFAULT 'N' AFTER Public;
DROP INDEX UsrCod ON agendas;
ALTER TABLE agendas ADD INDEX(UsrCod,Public,Hidden);

        Version 16.83:    Dec 03, 2016	Change in layout of agenda.
					Agenda events are private by default. (209488 lines)
					5 changes necessary in database:
ALTER TABLE agendas ADD COLUMN Public ENUM('N','Y') NOT NULL DEFAULT 'N' AFTER Hidden;
UPDATE agendas SET Public='Y' WHERE Hidden='N';
DROP INDEX UsrCod ON agendas;
ALTER TABLE agendas DROP COLUMN Hidden;
ALTER TABLE agendas ADD INDEX(UsrCod,Public);

        Version 16.82.2:  Dec 02, 2016	Changes in colors of dates in agenda.
					Default order of events in agenda now is "by end date". (209477 lines)
        Version 16.82.1:  Dec 02, 2016	Changes in another user's agenda. (209450 lines)
        Version 16.82:    Dec 02, 2016	New action to show agenda another user's agenda. (209425 lines)
					10 changes necessary in database:
UPDATE actions SET Txt='Ver mi agenda' WHERE ActCod='1602' AND Language='es';
UPDATE actions SET Txt='Solicitar creaci&oacute;n evento mi agenda' WHERE ActCod='1603' AND Language='es';
UPDATE actions SET Txt='Solicitar edici&oacute;n evento mi agenda' WHERE ActCod='1604' AND Language='es';
UPDATE actions SET Txt='Crear evento mi agenda' WHERE ActCod='1605' AND Language='es';
UPDATE actions SET Txt='Cambiar evento mi agenda' WHERE ActCod='1606' AND Language='es';
UPDATE actions SET Txt='Solicitar eliminaci&oacute;n evento mi agenda' WHERE ActCod='1607' AND Language='es';
UPDATE actions SET Txt='Eliminar evento mi agenda' WHERE ActCod='1608' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1609','es','N','Hacer privado evento mi agenda');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1610','es','N','Hacer p&uacute;blico evento mi agenda');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1611','es','N','Ver agenda otro usuario');

        Version 16.81.5:  Dec 01, 2016	Link in teacher's record card to show agenda. (209313 lines)
Copy the following icon to icon public directory:
sudo cp icon/calendar64x64.png /var/www/html/swad/icon/

        Version 16.81.4:  Dec 01, 2016	Changes in texts related to agenda. (209268 lines)
        Version 16.81.3:  Dec 01, 2016	Icon view replaced by lock in agenda.
					Changes in texts related to agenda. (209227 lines)
        Version 16.81.2:  Dec 01, 2016	New file types for GeoGebra, suggested by Sandra Quero Ramones. (209174 lines)
Copy the following icons to icon public directory:
sudo cp icon/filext16x16/ggb16x16.gif /var/www/html/swad/icon/filext16x16/
sudo cp icon/filext16x16/ggt16x16.gif /var/www/html/swad/icon/filext16x16/
sudo cp icon/filext32x32/ggb32x32.gif /var/www/html/swad/icon/filext32x32/
sudo cp icon/filext32x32/ggt32x32.gif /var/www/html/swad/icon/filext32x32/

        Version 16.81.1:  Dec 01, 2016	Table locations moved to table agendas. (209164 lines)
					3 changes necessary in database:
CREATE TABLE IF NOT EXISTS agendas (AgdCod INT NOT NULL AUTO_INCREMENT,UsrCod INT NOT NULL,Hidden ENUM('N','Y') NOT NULL DEFAULT 'N',StartTime DATETIME NOT NULL,EndTime DATETIME NOT NULL,Event VARCHAR(255) NOT NULL,Location VARCHAR(255) NOT NULL,Txt TEXT NOT NULL,UNIQUE INDEX(AgdCod),INDEX(UsrCod,Hidden));
INSERT INTO agendas (UsrCod,Hidden,StartTime,EndTime,Event,Location,Txt) SELECT UsrCod,Hidden,StartTime,EndTime,Event,Location,Txt FROM locations;
DROP TABLE locations;

        Version 16.81:    Dec 01, 2016	Code refactoring in agenda.
					Removed unused texts. (209182 lines)
        Version 16.80.1:  Nov 30, 2016	Changed order of event and location in agenda.
					Removed unused texts. (209320 lines)
        Version 16.80:    Nov 30, 2016	Changed location to agenda. (209428 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='1581';

        Version 16.79.2:  Nov 30, 2016	Changed location to event in agenda. (209566 lines)
        Version 16.79.1:  Nov 30, 2016	Changed location to event in agenda. (209559 lines)
        Version 16.79:    Nov 30, 2016	In locations, split Title into Event (What?) and Location (Where?). (209534 lines)
					4 changes necessary in database:
ALTER TABLE locations ADD COLUMN Location VARCHAR(255) NOT NULL AFTER Title;
ALTER TABLE locations ADD COLUMN Event VARCHAR(255) NOT NULL AFTER Location;
UPDATE locations SET Location=Title,Event=Title;
ALTER TABLE locations DROP COLUMN Title;

        Version 16.78:    Nov 30, 2016	Changes in location. Not finished. (209486 lines)
					2 changes necessary in database:
DROP TABLE IF EXISTS locations;
CREATE TABLE IF NOT EXISTS locations (LocCod INT NOT NULL AUTO_INCREMENT,UsrCod INT NOT NULL,Hidden ENUM('N','Y') NOT NULL DEFAULT 'N',NumNotif INT NOT NULL DEFAULT 0,StartTime DATETIME NOT NULL,EndTime DATETIME NOT NULL,Title VARCHAR(255) NOT NULL,Txt TEXT NOT NULL,UNIQUE INDEX(LocCod),INDEX(UsrCod,Hidden));

        Version 16.77:    Nov 29, 2016	Changes in location. Not finished. (209483 lines)
        Version 16.76:    Nov 29, 2016	New form to create location. Not finished. (208936 lines)
					8 changes necessary in database:
CREATE TABLE IF NOT EXISTS locations (LocCod INT NOT NULL AUTO_INCREMENT,StartDate DATE NOT NULL,EndDate DATE NOT NULL,Location VARCHAR(255) NOT NULL,UNIQUE INDEX(LocCod));
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1602','es','N','Ver mis ubicaciones');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1603','es','N','Editar mis ubicaciones');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1604','es','N','Solicitar creaci&oacute;n de ubicaci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1605','es','N','Eliminar ubicaci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1606','es','N','Cambiar fecha inicial de ubicaci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1607','es','N','Cambiar fecha final de ubicaci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1608','es','N','Renombrar ubicaci&oacute;n');

        Version 16.75:    Nov 28, 2016	New option to show my location. (207947 lines)
        Version 16.74.1:  Nov 28, 2016	Changed icon to show places. (207807 lines)
Copy icon/mapmarker64x64.png to public icons directory
Copy icon/iconset/awesome/action64x64/mapmarker64x64.png to public icons directory
Copy icon/iconset/nuvola/action64x64/mapmarker64x64.png to public icons directory

        Version 16.74:    Nov 28, 2016	Contextual help on holidays.
					Fixed bugs and code refactoring in holidays. (207803 lines)
        Version 16.73.10: Nov 28, 2016	Contextual help on places. (207794 lines)
        Version 16.73.9:  Nov 28, 2016	Contextual help on departments. (207786 lines)
        Version 16.73.8:  Nov 28, 2016	Contextual help on system links. (207778 lines)
        Version 16.73.7:  Nov 28, 2016	Contextual help on banners. (207772 lines)
        Version 16.73.6:  Nov 28, 2016	Contextual help on email domains. (207764 lines)
        Version 16.73.5:  Nov 28, 2016	Contextual help on types of degrees. (207757 lines)
        Version 16.73.4:  Nov 28, 2016	Contextual help on old courses. (207749 lines)
        Version 16.73.3:  Nov 27, 2016	Better contextual help on attendance. (207746 lines)
        Version 16.73.2:  Nov 27, 2016	Contextual help on assignments. (207725 lines)
        Version 16.73.1:  Nov 27, 2016	Contextual help on search. (? lines)
        Version 16.73:    Nov 27, 2016	Changes in teacher's record card. (207711 lines)
        Version 16.72.2:  Nov 27, 2016	Fixed bug in answers of survey. (207704 lines)
        Version 16.72.1:  Nov 27, 2016	Fixed bug in answers of test questions, reported by Javier Fern�ndez Baldomero. (207704 lines)
        Version 16.72:    Nov 27, 2016	Link to show all users' data.
					Code refactoring in scopes. (207698 lines)
        Version 16.71.9:  Nov 27, 2016	Fixed bug in list of students, reported by Javier Fern�ndez Baldomero. (207641 lines)
        Version 16.71.8:  Nov 26, 2016	Contextual help on email to students.
					Contextual help on global announcements. (207634 lines)
        Version 16.71.7:  Nov 26, 2016	Code refactoring in timetable. (207626 lines)
        Version 16.71.6:  Nov 26, 2016	Selector "Only my groups"/"All groups" only of the course has groups.
					Code refactoring in timetable. (207619 lines)
        Version 16.71.5:  Nov 26, 2016	Contextual help on course timetable. (207611 lines)
        Version 16.71.4:  Nov 26, 2016	Contextual help on user's timetable. (207607 lines)
        Version 16.71.3:  Nov 26, 2016	Change in layout of notifications. (207597 lines)
        Version 16.71.2:  Nov 25, 2016	Fix bug in shared record card. (207569 lines)
        Version 16.71.1:  Nov 25, 2016	Contextual help on test results. (207568 lines)
        Version 16.71:    Nov 25, 2016	Fixed bugs and changes in layout related with selection of groups. (207560 lines)
        Version 16.70.5:  Nov 24, 2016	Changes in layout of attendance list. (207549 lines)
        Version 16.70.4:  Nov 24, 2016	Changes in layout of students' list. (207547 lines)
        Version 16.70.3:  Nov 24, 2016	Changes in layout of selection of students to view homework. (207547 lines)
        Version 16.70.2:  Nov 24, 2016	Changes in layout of course stats. (207546 lines)
        Version 16.70.1:  Nov 24, 2016	Fixed minor bug in titles of frames. (207545 lines)
        Version 16.70:    Nov 24, 2016	Changes in permissions to view test results. (207545 lines)
        Version 16.69.1:  Nov 24, 2016	Contextual help on messages. (207484 lines)
        Version 16.69:    Nov 23, 2016	Changes in layout of frame title. (207473 lines)
        Version 16.68.10: Nov 23, 2016	Changes in layout of messages received / sent. (207459 lines)
        Version 16.68.9:  Nov 23, 2016	Changes in layout of messages received / sent. (207458 lines)
        Version 16.68.8:  Nov 23, 2016	Changes in layout of messages received / sent. (207462 lines)
        Version 16.68.7:  Nov 23, 2016	Changes in layout of messages received / sent. (207463 lines)
        Version 16.68.6:  Nov 23, 2016	Contextual help on messages. (207455 lines)
        Version 16.68.5:  Nov 22, 2016	Code refactoring and change in layout of file browser and contextual links. (207443 lines)
        Version 16.68.4:  Nov 22, 2016	Code refactoring in file browser. (207447 lines)
        Version 16.68.3:  Nov 22, 2016	Change in layout of contextual links in file browsers. (207436 lines)
        Version 16.68.2:  Nov 22, 2016	Code refactoring related to file browser and contextual checkbox. (207438 lines)
        Version 16.68.1:  Nov 22, 2016	Code refactoring related to notifications and contextual checkbox. (207438 lines)
        Version 16.68:    Nov 22, 2016	Change in text related to notifications.
					Changes in contextual links in notifications. (207406 lines)
        Version 16.67.3:  Nov 22, 2016	Contextual help on notifications. (207355 lines)
        Version 16.67.2:  Nov 21, 2016	Contextual help on sign up in a course and enrolment requests. (207351 lines)
        Version 16.67.1:  Nov 21, 2016	Contextual help on connected users and last clicks in real time. (207346 lines)
        Version 16.67:    Nov 21, 2016	Refactoring in tests.
					"Test exam" is now called "Test result". (207336 lines)
        Version 16.66:    Nov 21, 2016	Change icons for attendance.
					Change icons for announcements.
					Change icons for notices.
					Change icon for exam. (207394 lines)
Copy icon/rollcall64x64.png to public icons directory
Copy icon/iconset/awesome/action64x64/rollcall64x64.png to public icons directory
Copy icon/iconset/nuvola/action64x64/rollcall64x64.png to public icons directory
Copy icon/iconset/awesome/action64x64/announcement64x64.png to public icons directory
Copy icon/iconset/nuvola/action64x64/announcement64x64.png to public icons directory
Copy icon/notice64x64.png to public icons directory
Copy icon/iconset/awesome/action64x64/notice64x64.png to public icons directory
Copy icon/iconset/nuvola/action64x64/notice64x64.png to public icons directory
Copy icon/exam64x64.png to public icons directory

        Version 16.65.3:  Nov 20, 2016	Contextual help on duplicated users. (207378 lines)
        Version 16.65.2:  Nov 20, 2016	Changed order of links in other users. (207371 lines)
        Version 16.65.1:  Nov 20, 2016	Changes in help of administrators and guests. (207370 lines)
        Version 16.65:    Nov 20, 2016	Changes in selector of my courses. (207369 lines)
        Version 16.64.34: Nov 20, 2016	Input fields for URLs are changed from text to url. (207376 lines)
        Version 16.64.33: Nov 20, 2016	Input fields for email are changed from text to email. (207375 lines)
        Version 16.64.32: Nov 20, 2016	Input fields for phone numbers are changed from text to tel. (207374 lines)
        Version 16.64.31: Nov 19, 2016	Input field required in new holiday form. (207373 lines)
        Version 16.64.30: Nov 19, 2016	Input fields required in new place form. (207371 lines)
        Version 16.64.29: Nov 19, 2016	Input fields required in new department form. (207368 lines)
        Version 16.64.28: Nov 19, 2016	Input fields required in new plugin form. (207364 lines)
        Version 16.64.27: Nov 19, 2016	Input fields required in new institutional link form. (207357 lines)
        Version 16.64.26: Nov 19, 2016	Input fields required in new banner form. (207353 lines)
        Version 16.64.25: Nov 19, 2016	Input fields required in new email domain form. (207348 lines)
        Version 16.64.24: Nov 19, 2016	Input field required in new type of degree form. (207345 lines)
        Version 16.64.23: Nov 19, 2016	Input fields required in new record field form. (207343 lines)
        Version 16.64.22: Nov 19, 2016	Input fields required in form to edit one test question. (207340 lines)
        Version 16.64.21: Nov 19, 2016	Input fields required in form to configure tests. (207324 lines)
        Version 16.64.20: Nov 19, 2016	Input field required in new link form. (207316 lines)
        Version 16.64.19: Nov 19, 2016	Input field required in new folder form. (207314 lines)
        Version 16.64.18: Nov 19, 2016	Input field required in search form. (207312 lines)
        Version 16.64.17: Nov 19, 2016	Input field required in new attendance event form. (207310 lines)
        Version 16.64.16: Nov 19, 2016	Input field required in new survey form. (207308 lines)
        Version 16.64.15: Nov 19, 2016	Input field required in new assignment form. (207306 lines)
        Version 16.64.14: Nov 19, 2016	Input field required in administration of one user. (207304 lines)
        Version 16.64.13: Nov 19, 2016	Input fields required in edition of groups. (207302 lines)
        Version 16.64.12: Nov 19, 2016	Input fields required in edition of course. (207298 lines)
        Version 16.64.11: Nov 19, 2016	Input fields required in edition of degree. (207295 lines)
        Version 16.64.10: Nov 19, 2016	Input fields required in edition of centre. (207290 lines)
        Version 16.64.9:  Nov 19, 2016	Input fields required in edition of institution. (207286 lines)
        Version 16.64.8:  Nov 19, 2016	Input fields required in edition of country. (207282 lines)
        Version 16.64.7:  Nov 19, 2016	Input fields required in new account form.
					Input fields required in password forms. (207278 lines)
        Version 16.64.6:  Nov 18, 2016	Input fields required in log in form. (207270 lines)
        Version 16.64.5:  Nov 18, 2016	Input fields required in record card form. (207268 lines)
        Version 16.64.4:  Nov 18, 2016	Contextual help on guests and administrators. (207264 lines)
        Version 16.64.3:  Nov 18, 2016	Contextual help on teachers. (207259 lines)
        Version 16.64.2:  Nov 17, 2016	Code refactoring in record cards. (207253 lines)
        Version 16.64.1:  Nov 17, 2016	Changes in texts related to record cards. (207252 lines)
        Version 16.64:    Nov 17, 2016	Contextual help on students.
					Code refactoring in record cards. (207231 lines)
        Version 16.63.6:  Nov 16, 2016	Contextual help on email unconfirmed. (207184 lines)
        Version 16.63.5:  Nov 16, 2016	Contextual help on edition of countries. (207180 lines)
        Version 16.63.4:  Nov 16, 2016	Contextual help on edition of institutions. (207177 lines)
        Version 16.63.3:  Nov 16, 2016	Contextual help on edition of centres. (207174 lines)
        Version 16.63.2:  Nov 16, 2016	Contextual help on edition of degrees. (207171 lines)
        Version 16.63.1:  Nov 16, 2016	Contextual help on edition of courses. (207168 lines)
        Version 16.63:    Nov 16, 2016	"e-mail" changed to "email", except in database fields. (207164 lines)
        Version 16.62.2:  Nov 14, 2016	Changed message related to clipboard.
					Fixed minor bug when copying all files inside root folder. (207158 lines)
        Version 16.62.1:  Nov 14, 2016	Code refactoring in clipboard. (207151 lines)
        Version 16.62:    Nov 14, 2016	Copy of all files inside root folder is now allowed (root folder not copied). (207165 lines)
        Version 16.61.8:  Nov 14, 2016	Code refactoring in clipboard. (207148 lines)
        Version 16.61.7:  Nov 14, 2016	Code refactoring in clipboard. (207155 lines)
        Version 16.61.6:  Nov 14, 2016	Code refactoring in clipboard. (207167 lines)
        Version 16.61.5:  Nov 14, 2016	New alert type: clipboard. (207161 lines)
        Version 16.61.4:  Nov 14, 2016	Copy source is displayed as an alert. (207158 lines)
        Version 16.61.3:  Nov 14, 2016	Copy in flie browsers is allowed from any folder. (207156 lines)
        Version 16.61.2:  Nov 14, 2016	Code refactoring in file browser. (207166 lines)
        Version 16.61.1:  Nov 14, 2016	Code refactoring in hierarchy. (207150 lines)
        Version 16.61:    Nov 14, 2016	New module swad_hierarchy. (207159 lines)
        Version 16.60.1:  Nov 14, 2016	Changes in layout of file browser. (207096 lines)
        Version 16.60:    Nov 14, 2016	Code refactoring in file browser.
					Fixed bug in assigments, reported by Javier Fern�ndez Baldomero. (207101 lines)
        Version 16.59.3:  Nov 14, 2016	Code refactoring in file browser. (207072 lines)
        Version 16.59.2:  Nov 14, 2016	Fix bug in user's usage report. (207077 lines)
        Version 16.59.1:  Nov 14, 2016	Fix bug in deletion of a notice, reported by Javier Fern�ndez Baldomero and others. (207076 lines)
        Version 16.59:    Nov 14, 2016	Changes in layout of contextual help icon.
					Changes in CSS. (207075 lines)
        Version 16.58:    Nov 14, 2016	Code refactoring related to layout of frames and tables. (207062 lines)
        Version 16.57.3:  Nov 13, 2016	Code refactoring related to layout of frames and tables. (207119 lines)
        Version 16.57.2:  Nov 13, 2016	Contextual help on usage report. (207102 lines)
        Version 16.57.1:  Nov 13, 2016	Contextual help on frequent actions. (207099 lines)
        Version 16.57:    Nov 13, 2016	Contextual help on figures. (207096 lines)
        Version 16.56.8:  Nov 13, 2016	Contextual help on notices. (207002 lines)
        Version 16.56.7:  Nov 13, 2016	Contextual help on forums. (206996 lines)
        Version 16.56.6:  Nov 13, 2016	Contextual help on who to follow. (206968 lines)
        Version 16.56.5:  Nov 13, 2016	Contextual help on view public profile. (206963 lines)
        Version 16.56.4:  Nov 13, 2016	Contextual help on public activity. (206960 lines)
        Version 16.56.3:  Nov 13, 2016	Contextual help on attendance. (206955 lines)
        Version 16.56.2:  Nov 13, 2016	Contextual help on groups. (206938 lines)
        Version 16.56.1:  Nov 13, 2016	Code refactoring and changes in layout related with titles. (206923 lines)
        Version 16.56:    Nov 13, 2016	Contextual help on tests.
					Change in layout of test import. (206935 lines)
        Version 16.55:    Nov 13, 2016	Contextual help on file zones. (206912 lines)
        Version 16.54.23: Nov 13, 2016	Contextual help on pending institutions, centres, degrees and courses. (206846 lines)
        Version 16.54.22: Nov 13, 2016	Contextual help on course information. (206832 lines)
        Version 16.54.21: Nov 13, 2016	Contextual help on degree information. (206826 lines)
        Version 16.54.20: Nov 13, 2016	Contextual help on centre information. (206821 lines)
        Version 16.54.19: Nov 13, 2016	Contextual help on institution information. (206816 lines)
        Version 16.54.18: Nov 13, 2016	Contextual help on country information.
					Fixed bug in country information. (206811 lines)
        Version 16.54.17: Nov 13, 2016	Code refactoring in contextual help links. (206804 lines)
        Version 16.54.16: Nov 13, 2016	Contextual help on courses. (206829 lines)
        Version 16.54.15: Nov 13, 2016	Contextual help on degrees. (206823 lines)
        Version 16.54.14: Nov 13, 2016	Contextual help on centres. (206817 lines)
        Version 16.54.13: Nov 13, 2016	Contextual help on institutions. (206811 lines)
        Version 16.54.12: Nov 13, 2016	Contextual help on countries. (206805 lines)
        Version 16.54.11: Nov 13, 2016	Contextual help on my webs / social networks. (206799 lines)
        Version 16.54.10: Nov 13, 2016	Contextual help on my institution, centre and department. (206795 lines)
        Version 16.54.9:  Nov 13, 2016	Contextual help on record card. (206789 lines)
        Version 16.54.8:  Nov 13, 2016	Contextual help on my courses. (206782 lines)
        Version 16.54.7:  Nov 13, 2016	Changes in translation related to role. (206778 lines)
        Version 16.54.6:  Nov 13, 2016	Link to session in head. (206777 lines)
        Version 16.54.5:  Nov 13, 2016	Frame and contextual help on role. (206771 lines)
        Version 16.54.4:  Nov 13, 2016	Contextual help on creating a new account (sign up). (206747 lines)
        Version 16.54.3:  Nov 13, 2016	Contextual help on log in. (206740 lines)
        Version 16.54.2:  Nov 13, 2016	Contextual help on user's password. (206735 lines)
        Version 16.54.1:  Nov 13, 2016	Contextual help on user's account. (206729 lines)
        Version 16.54:    Nov 13, 2016	Contextual help on user's preferences. (206722 lines)
        Version 16.53.1:  Nov 13, 2016	Contextual help on user's photo. (206686 lines)
        Version 16.53:    Nov 13, 2016	Contextual help on surveys. (206681 lines)
        Version 16.52:    Nov 12, 2016	Code refactoring in frames to include contextual help. (206653 lines)
        Version 16.51:    Nov 12, 2016	Contextual help on the top right corner of frames. (206606 lines)
Copy icon/help64x64.png to icon directory

        Version 16.50.2:  Nov 12, 2016	Changes in layout of record cards. (206556 lines)
        Version 16.50.1:  Nov 12, 2016	Minor corrections in some messages. (206559 lines)
        Version 16.50:    Nov 10, 2016	My frequent actions are moved from PROFILE tab to STATS tab.
					Some messages translated. (206558 lines)
        Version 16.49.1:  Nov 10, 2016	Message translated. (206556 lines)
        Version 16.49:    Nov 08, 2016	Set some user's figures to 0 when creating an account. (206555 lines)
        Version 16.48.12: Nov 07, 2016	Icon in my webs / social networks to show figure (statistics). (206537 lines)
        Version 16.48.11: Nov 07, 2016	Changes in order of figures (statistics). (206523 lines)
        Version 16.48.10: Nov 07, 2016	Changes in text related to notifications stats. (206522 lines)
        Version 16.48.9:  Nov 07, 2016	Icon in my notifications to show figure (statistics). (206521 lines)
        Version 16.48.8:  Nov 07, 2016	Icon in preferences-notifications to show figure (statistics). (206518 lines)
        Version 16.48.7:  Nov 07, 2016	Icon in preferences-privacy to show figure (statistics). (206505 lines)
        Version 16.48.6:  Nov 07, 2016	Icon in preferences-columns to show figure (statistics). (206492 lines)
        Version 16.48.5:  Nov 07, 2016	Icon in preferences-menu to show figure (statistics). (206479 lines)
        Version 16.48.4:  Nov 07, 2016	Icon in preferences-theme to show figure (statistics). (206465 lines)
        Version 16.48.3:  Nov 07, 2016	Icon in preferences-icons to show figure (statistics). (206449 lines)
        Version 16.48.2:  Nov 07, 2016	Icon in preferences-first-day-of-week to show figure (statistics). (206433 lines)
        Version 16.48.1:  Nov 07, 2016	Icon in preferences-language to show figure (statistics). (206429 lines)
        Version 16.48:    Nov 07, 2016	Icon in messages to show figure (statistics).
                                        Code refactoring in messages. (206418 lines)
        Version 16.47.19: Nov 07, 2016	Icon in forums to show figure (statistics). (206361 lines)
        Version 16.47.18: Nov 07, 2016	Icon in "who to follow" to show figure (statistics). (206350 lines)
        Version 16.47.17: Nov 07, 2016	Icon in public activity to show figure (statistics). (206337 lines)
        Version 16.47.16: Nov 07, 2016	Icon in file browser to show figure (statistics). (206326 lines)
        Version 16.47.15: Nov 07, 2016	Icon in file browser to show figure (statistics). (206313 lines)
        Version 16.47.14: Nov 07, 2016	Icon in list of assignments to show figure (statistics). (206314 lines)
        Version 16.47.13: Nov 07, 2016	Icon in list of guests to show figure (statistics). (206295 lines)
        Version 16.47.12: Nov 07, 2016	Icon in list of teachers to show figure (statistics). (206278 lines)
        Version 16.47.11: Nov 07, 2016	Icon in list of students to show figure (statistics). (206262 lines)
        Version 16.47.10: Nov 07, 2016	Icon in list of surveys to show figure (statistics). (206246 lines)
        Version 16.47.9:  Nov 07, 2016	Icon in list of courses to show figure (statistics). (206231 lines)
        Version 16.47.8:  Nov 06, 2016	Icon in list of degrees to show figure (statistics). (206211 lines)
        Version 16.47.7:  Nov 06, 2016	Icon in list of centres to show figure (statistics). (206191 lines)
        Version 16.47.6:  Nov 06, 2016	Code refactoring in countries. (206171 lines)
        Version 16.47.5:  Nov 06, 2016	Code refactoring in countries. (206162 lines)
        Version 16.47.4:  Nov 06, 2016	Code refactoring in notices. (206153 lines)
        Version 16.47.3:  Nov 06, 2016	Icon in list of institutions to show figure (statistics). (206155 lines)
        Version 16.47.2:  Nov 06, 2016	Icon in list of countries to show figure (statistics). (206141 lines)
        Version 16.47.1:  Nov 06, 2016	Icon in list of notices to show figure (statistics). (206129 lines)
Copy icon/pie64x64.gif to icon directory

        Version 16.47:    Nov 06, 2016	New functions to put a contextual icon to show a figure (statistics). (206113 lines)
        Version 16.46.10: Nov 06, 2016	Code refactoring in statistics. (206080 lines)
        Version 16.46.9:  Nov 06, 2016	Fixed bug in figures (statistics). (206064 lines)
        Version 16.46.8:  Nov 05, 2016	Changes in texts related to notifications. (206047 lines)
        Version 16.46.7:  Nov 05, 2016	Changes in text related to forum statistics. (206046 lines)
        Version 16.46.6:  Nov 05, 2016	Changes in texts related to public social activity statistics. (206045 lines)
        Version 16.46.5:  Nov 05, 2016	Changes in texts related to folders and files statistics. (206043 lines)
        Version 16.46.4:  Nov 03, 2016	Fixed text related to order of centres. (206042 lines)
        Version 16.46.3:  Nov 03, 2016	Fixed bug in creation of a new centre. (206041 lines)
        Version 16.46.2:  Nov 01, 2016	Fixed bugs in surveys. (206040 lines)
        Version 16.46.1:  Nov 01, 2016	Fixed bugs in javascript function to set a date range to a given day. (206035 lines)
        Version 16.46:    Oct 29, 2016	Fixed bugs in surveys.
                                        View only the surveys of my groups. (206032 lines)
        Version 16.45.10: Oct 28, 2016	Messages translated. (205995 lines)
        Version 16.45.9:  Oct 28, 2016	Free lists of user's countries, institutions, centres and degrees.
					Code refactoring related to hierarchy. (205994 lines)
        Version 16.45.8:  Oct 28, 2016	Remove surveys of a country when removing it. (205930 lines)
        Version 16.45.7:  Oct 28, 2016	Remove forums and surveys of an institution when removing it. (205927 lines)
        Version 16.45.6:  Oct 28, 2016	Remove forums and surveys of a centre when removing it. (205922 lines)
        Version 16.45.5:  Oct 28, 2016	Code refactoring in forums. (205919 lines)
        Version 16.45.4:  Oct 27, 2016	Code refactoring in degree forums. (205916 lines)
        Version 16.45.3:  Oct 27, 2016	Code refactoring in course forums. (205906 lines)
        Version 16.45.2:  Oct 27, 2016	Code refactoring related with scopes. (205896 lines)
        Version 16.45.1:  Oct 27, 2016	Fixed bugs in surveys. (? lines)
        Version 16.45:    Oct 27, 2016	Changes in database table for surveys. Not finished. (205927 lines)
					10 changes necessary in database:
ALTER TABLE surveys ADD COLUMN Scope ENUM('Sys','Cty','Ins','Ctr','Deg','Crs') NOT NULL DEFAULT 'Sys' AFTER SvyCod;
ALTER TABLE surveys ADD COLUMN Cod INT NOT NULL DEFAULT -1 AFTER Scope;

UPDATE surveys SET Scope='Sys' WHERE DegCod<='0' AND CrsCod<='0';
UPDATE surveys SET Scope='Deg' WHERE DegCod>'0' AND CrsCod<='0';
UPDATE surveys SET Scope='Crs' WHERE CrsCod>'0';

UPDATE surveys SET Cod='-1' WHERE Scope='Sys';
UPDATE surveys SET Cod=DegCod WHERE Scope='Deg';
UPDATE surveys SET Cod=CrsCod WHERE Scope='Crs';

DROP INDEX DegCod ON surveys;
ALTER TABLE surveys ADD INDEX(Scope,Cod);

        Version 16.44:    Oct 27, 2016	New scopes (centre, institution, country) of surveys. Not finished. (205826 lines)
        Version 16.43:    Oct 26, 2016	Fixed bugs and code refactoring in scope of surveys. (205537 lines)
        Version 16.42.1:  Oct 25, 2016	Added button "New question" in survey when the survey has no questions.
					Change in layout of survey questions. (205485 lines)
        Version 16.42:    Oct 24, 2016	Changres in rules to admin other user and change other user's photo. (205481 lines)
        Version 16.41.10: Oct 24, 2016	Column ID in statistic of hits in a course is now left aligned. (205474 lines)
        Version 16.41.9:  Oct 24, 2016	Changed permissions to edit institution logo and institution WWW. (205474 lines)
        Version 16.41.8:  Oct 24, 2016	Changed permissions to edit degree logo and degree WWW. (205472 lines)
        Version 16.41.7:  Oct 24, 2016	Changed permissions to edit centre WWW. (205466 lines)
        Version 16.41.6:  Oct 24, 2016	Changed permissions to edit logo, photo and photo attribution of centre. (205464 lines)
        Version 16.41.5:  Oct 23, 2016	Code refactoring in countries, institutions, centres, degrees and courses. (205456 lines)
        Version 16.41.4:  Oct 23, 2016	Code refactoring in institutions. (205398 lines)
        Version 16.41.3:  Oct 23, 2016	Code refactoring in centres. (205408 lines)
        Version 16.41.2:  Oct 23, 2016	Code refactoring in degrees. (205430 lines)
        Version 16.41.1:  Oct 23, 2016	Code refactoring in courses. (205443 lines)
        Version 16.41:    Oct 23, 2016	New form in degree configuration to change short name. (205453 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1601','es','N','Cambiar nombre breve asignatura');

        Version 16.40:    Oct 23, 2016	New form in degree configuration to change full name. (205427 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1600','es','N','Cambiar nombre completo asig.');

        Version 16.39:    Oct 23, 2016	New form in degree configuration to change WWW. (205404 lines)
					2 changes necessary in database:
UPDATE actions SET Txt='Cambiar web de titulaci&oacute;n' WHERE ActCod='554' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1599','es','N','Cambiar web de titulaci&oacute;n');

        Version 16.38:    Oct 23, 2016	New form in degree configuration to change full name. (205354 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1598','es','N','Cambiar nombre breve titulac.');

        Version 16.37:    Oct 23, 2016	New form in degree configuration to change full name. (205326 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1597','es','N','Cambiar nombre completo titulac.');

        Version 16.36:    Oct 23, 2016	New form in centre configuration to change WWW. (205311 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1596','es','N','Cambiar web de centro');

        Version 16.35:    Oct 23, 2016	New form in centre configuration to change short name. (205248 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1595','es','N','Cambiar nombre breve centro');

        Version 16.34:    Oct 22, 2016	New form in centre configuration to change full name. (205223 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1594','es','N','Cambiar nombre completo centro');

        Version 16.33:    Oct 22, 2016	New form in institution configuration to change WWW. (205208 lines)
					2 changes necessary in database:
UPDATE actions SET Txt='Cambiar web de instituci&oacute;n' WHERE ActCod='700' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1593','es','N','Cambiar web de instituci&oacute;n');

        Version 16.32.3:  Oct 22, 2016	Width of degree web limited using CSS. (205156 lines)
        Version 16.32.2:  Oct 22, 2016	Width of centre web limited using CSS. (205154 lines)
        Version 16.32.1:  Oct 22, 2016	Width of institution web limited using CSS. (205152 lines)
        Version 16.32:    Oct 22, 2016	New form in institution configuration to change short name. (205141 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1592','es','N','Cambiar nombre breve ins.');

        Version 16.31.4:  Oct 22, 2016	Code refactoring in courses. (205114 lines)
        Version 16.31.3:  Oct 22, 2016	Code refactoring in degrees. (205112 lines)
        Version 16.31.2:  Oct 22, 2016	Code refactoring in centres. (205108 lines)
        Version 16.31.1:  Oct 22, 2016	Code optimization in renaming of institution. (205104 lines)
        Version 16.31:    Oct 22, 2016	New form in institution configuration to change full name. (205097 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1591','es','N','Cambiar nombre completo ins.');

        Version 16.30.2:  Oct 21, 2016	Code refactoring related to actions. (205077 lines)
        Version 16.30.1:  Oct 20, 2016	Code refactoring related to tabs. (205077 lines)
        Version 16.30:    Oct 20, 2016	Removed country in edition of institutions. (205058 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='865';

        Version 16.29:    Oct 20, 2016	Removed institution in edition of centres. (205159 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='720';

        Version 16.28:    Oct 20, 2016	Removed centre in edition of degrees. (205362 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='1049';

        Version 16.27:    Oct 20, 2016	Removed degree in edition of courses. (205433 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='565';

        Version 16.26.1:  Oct 20, 2016	Removed warning message in course edition. (205524 lines)
        Version 16.26:    Oct 20, 2016	New form in institution configuration to move institution to another country. (205566 lines)
					2 changes necessary in database:
UPDATE actions SET Txt='Mover instituci&oacute;n a otro pa&iacute;s' WHERE ActCod='865' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1590','es','N','Mover instituci&oacute;n a otro pa&iacute;s');

        Version 16.25:    Oct 20, 2016	New form in centre configuration to move centre to another institution. (205416 lines)
					4 changes necessary in database:
UPDATE actions SET Txt='Mover titulaci&oacute;n a otro centro' WHERE ActCod='1049' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1588','es','N','Mover titulaci&oacute;n a otro centro');
UPDATE actions SET Txt='Mover centro a otra instituci&oacute;n' WHERE ActCod='720' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1589','es','N','Mover centro a otra instituci&oacute;n');

        Version 16.24:    Oct 20, 2016	Code refactoring in list of degrees. (205326 lines)
        Version 16.23.2:  Oct 19, 2016	Code refactoring in course edition. (205448 lines)
        Version 16.23.1:  Oct 19, 2016	Code refactoring in degree edition. (205441 lines)
        Version 16.23:    Oct 19, 2016	New form in degree configuration to move degree to another centre. (205434 lines)
        Version 16.22.2:  Oct 19, 2016	Code refactoring in course configuration.
					Fixed bug in selector of courses in breadcrumb. (205350 lines)
        Version 16.22.1:  Oct 19, 2016	Fixed bug in course configuration. (205373 lines)
        Version 16.22:    Oct 19, 2016	New form in course configuration to move course to another degree.
					Code refactorization related with courses and degrees. (205369 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1587','es','N','Mover asignatura a otra titulac.');

        Version 16.21.3:  Oct 12, 2016	Changes in layout of file browser. (205266 lines)
        Version 16.21.2:  Oct 12, 2016	Unused code removed from actions. (205263 lines)
        Version 16.21.1:  Oct 12, 2016	Code refactoring in actions. (205280 lines)
        Version 16.21:    Oct 12, 2016	Code refactoring related with warning about email notifications.
					Notification mail is sent only if email is confirmed. (205278 lines)
        Version 16.20:    Oct 11, 2016	New button to edit/view file browser. (205241 lines)
        Version 16.19.3:  Oct 11, 2016	Code optimization in file browser. (205210 lines)
        Version 16.19.2:  Oct 11, 2016	Code optimization in file browser. (205198 lines)
        Version 16.19.1:  Oct 10, 2016	Message to report that confirmation email has been sent. (205203 lines)
        Version 16.19:    Oct 10, 2016	Warning in User > Session to confirm user's email address.
					Changes in form to edit user's account. (205194 lines)
        Version 16.18.1:  Oct 10, 2016	Warning in User > Account to confirm user's email address. (205116 lines)
        Version 16.18:    Oct 09, 2016	Removed external authentication (previously used at University of Granada). (205092 lines)
					4 changes necessary in database:
DROP TABLE IF EXISTS imported_groups;
DROP TABLE IF EXISTS imported_sessions;
DROP TABLE IF EXISTS imported_students;
UPDATE actions SET Obsolete='Y' WHERE ActCod IN ('794','796');

        Version 16.17.17: Oct 09, 2016	Fixed bug in user's usage report. (206245 lines)
        Version 16.17.16: Oct 09, 2016	Code refactoring in user's usage report. (206236 lines)
        Version 16.17.15: Oct 09, 2016	Code refactoring in user's usage report. (206236 lines)
        Version 16.17.14: Oct 09, 2016	Code refactoring in user's usage report. (206268 lines)
        Version 16.17.13: Oct 09, 2016	Code refactoring in user's usage report. (206273 lines)
        Version 16.17.12: Oct 09, 2016	Code refactoring in user's usage report. (206278 lines)
        Version 16.17.11: Oct 09, 2016	Code refactoring in user's usage report. (206279 lines)
        Version 16.17.10: Oct 09, 2016	Code refactoring in user's usage report. (206285 lines)
        Version 16.17.9:  Oct 08, 2016	Code refactoring in user's usage report. (206286 lines)
        Version 16.17.8:  Oct 08, 2016	Code refactoring in user's usage report. (206289 lines)
        Version 16.17.7:  Oct 08, 2016	Code refactoring in user's usage report. (206290 lines)
        Version 16.17.6:  Oct 08, 2016	Code refactoring in user's usage report. (206291 lines)
        Version 16.17.5:  Oct 08, 2016	Code refactoring in user's usage report. (206301 lines)
        Version 16.17.4:  Oct 08, 2016	Code refactoring in user's usage report. (206292 lines)
        Version 16.17.3:  Oct 08, 2016	Message translated in user's usage report. (206255 lines)
        Version 16.17.2:  Oct 08, 2016	Message translated in user's usage report. (206233 lines)
        Version 16.17.1:  Oct 08, 2016	Message translated in user's usage report. (206211 lines)
        Version 16.17:    Oct 08, 2016	List of most frequent actions in user's usage report. (206189 lines)
        Version 16.16.1:  Oct 07, 2016	Fixed bug in user's usage report. (206108 lines)
        Version 16.16:    Oct 07, 2016	Order current courses in user's usage report by number of clicks.
				        Do not write number of users in historic courses.
				        Clicks without course selected are shown apart. (206085 lines)
        Version 16.15.6:  Oct 06, 2016	Message in user's usage reports. (206027 lines)
        Version 16.15.5:  Oct 06, 2016	Fix bug removing user's usage reports. (206023 lines)
        Version 16.15.4:  Oct 06, 2016	When a user is removed, all his/her user's usage reports are removed. (206022 lines)
        Version 16.15.3:  Oct 06, 2016	Do not write date when requesting a user's usage report. (205965 lines)
        Version 16.15.2:  Oct 06, 2016	Code refactoring in user's usage report. (205966 lines)
					1 change necessary in database:
CREATE TABLE IF NOT EXISTS usr_report (RepCod INT NOT NULL AUTO_INCREMENT,UsrCod INT NOT NULL,ReportTimeUTC DATETIME NOT NULL,UniqueDirL CHAR(2) NOT NULL,UniqueDirR CHAR(41) NOT NULL,Filename VARCHAR(255) NOT NULL,Permalink VARCHAR(255) NOT NULL,UNIQUE INDEX(RepCod),INDEX(UsrCod));

        Version 16.15.1:  Oct 06, 2016	User's usage reports are stored into database. (205950 lines)
        Version 16.15:    Oct 06, 2016	New database table to store user's usage reports. (205896 lines)
        Version 16.14:    Oct 06, 2016	By default, teachers see (do not edit) documents. Now they must click on small edit icon to edit. (205875 lines)
        Version 16.13.2:  Oct 05, 2016	User's usage report is open to any logged user. (205907 lines)
        Version 16.13.1:  Oct 04, 2016	Changes in user's usage report. (205906 lines)
        Version 16.13:    Oct 04, 2016	New filename for user's usage report. (205883 lines)
        Version 16.12:    Oct 04, 2016	Code refactoring in user's usage report. (205882 lines)
Copy icon/report64x64.png to icon directory

        Version 16.11.3:  Oct 04, 2016	Code refactoring in user's usage report. (205834 lines)
        Version 16.11.2:  Oct 04, 2016	Code refactoring in user's usage report. (205829 lines)
        Version 16.11.1:  Oct 04, 2016	Code refactoring in user's usage report. (205824 lines)
        Version 16.11:    Oct 03, 2016	Created permalinks to report files. (205793 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='1583';

        Version 16.10:    Oct 03, 2016	All user's figures are calculated in one click. (205795 lines)
        Version 16.9:     Oct 03, 2016	User's usage report is written into a temporary file. (205795 lines)
        Version 16.8.1:   Oct 03, 2016	Do not show courses with a few clicks in user's usage report. (205747 lines)
        Version 16.8:     Oct 03, 2016	New option to view user's usage report. (205720 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1586','es','N','Solicitar mi informe de uso');

        Version 16.7.9:   Oct 03, 2016	Code refactoring in user's usage report. (205650 lines)
        Version 16.7.8:   Oct 03, 2016	Code refactoring in user's usage report. (205632 lines)
        Version 16.7.7:   Oct 03, 2016	Code refactoring in user's usage report. (205609 lines)
        Version 16.7.6:   Oct 03, 2016	Code refactoring in user's usage report. (205591 lines)
        Version 16.7.5:   Oct 03, 2016	Code refactoring in user's usage report. (205570 lines)
        Version 16.7.4:   Oct 03, 2016	Code refactoring in user's usage report. (205557 lines)
        Version 16.7.3:   Oct 03, 2016	User's ID is removed from user's usage report. (205533 lines)
        Version 16.7.2:   Oct 02, 2016	Change in order of hits in user's usage report. (205546 lines)
        Version 16.7.1:   Oct 02, 2016	Fixed bugs in user's usage report.
					Removed unused code in user's usage report. (205545 lines)
        Version 16.7:     Oct 02, 2016	Improvements user's usage report. (205645 lines)
        Version 16.6.3:   Oct 02, 2016	Current time in user's usage report. (205559 lines)
        Version 16.6.2:   Oct 02, 2016	Message translated. (205456 lines)
        Version 16.6.1:   Oct 02, 2016	Message translated. (205454 lines)
        Version 16.6:     Oct 02, 2016	Hits in historic courses in user's usage report. (205432 lines)
        Version 16.5:     Oct 02, 2016	Changes in hits in current courses in user's usage report. (205364 lines)
        Version 16.4.1:   Sep 29, 2016	Changes in hits in current courses in user's usage report. (205316 lines)
        Version 16.4:     Sep 29, 2016	Hits in current courses in user's usage report. (205299 lines)
        Version 16.3.2:   Sep 28, 2016	Changes in behaviour of button to follow user. (205277 lines)
        Version 16.3.1:   Sep 28, 2016	Two different messages related to new account merged into one.
					Changes in form to create new account. (205271 lines)
        Version 16.3:     Sep 26, 2016	Message translated.
					Code refactoring of list with empty accounts for an ID. (205287 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1585','es','N','Autenticar usuario en cuenta vac&iacute;a');

        Version 16.2.13:  Sep 26, 2016	Message translated. (205203 lines)
        Version 16.2.12:  Sep 26, 2016	Message translated. (205181 lines)
        Version 16.2.11:  Sep 26, 2016	Message translated. (205159 lines)
        Version 16.2.10:  Sep 26, 2016	Message translated. (205137 lines)
        Version 16.2.9:   Sep 26, 2016	Message translated. (205115 lines)
        Version 16.2.8:   Sep 26, 2016	Message translated. (205093 lines)
        Version 16.2.7:   Sep 26, 2016	Message translated. (205071 lines)
        Version 16.2.6:   Sep 26, 2016	Message translated. (205049 lines)
        Version 16.2.5:   Sep 26, 2016	Check if an account already exists. (205019 lines)
        Version 16.2.4:   Sep 22, 2016	Check if an account already exists. Not finished. (205016 lines)
        Version 16.2.3:   Sep 22, 2016	Check if an account already exists. Not finished. (204984 lines)
        Version 16.2.2:   Sep 22, 2016	Fixed bug in photo visibility. (204951 lines)
        Version 16.2.1:   Sep 22, 2016	Check if an account already exists. Not finished. (204946 lines)
        Version 16.2:     Sep 22, 2016	New option to check if an account already exists. Not finished. (204901 lines)
					4 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1581','es','N','Mostrar mi agenda');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1582','es','N','Ver mi informe de uso');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1583','es','N','Imprimir mi informe de uso');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1584','es','N','Comprobar si existe cuenta');

        Version 16.1.1:   Sep 22, 2016	Index optimization in tables of users' courses. (204829 lines)
        Version 16.1:     Sep 22, 2016	Temporary table with other user's courses. (204828 lines)
        Version 16.0:     Sep 22, 2016	Temporary table with my courses.
					Added indexes to database. (204811 lines)
					1 change necessary in database:
ALTER TABLE connected ADD INDEX (RoleInLastCrs);

        Version 15.256.3: Sep 19, 2016	Fixed bug in web service function sendAttendanceEvent, reported by Rub�n Mart�n Hidalgo. (204755 lines)
        Version 15.256.2: Sep 19, 2016	Fixed bug in web service function removeAttendanceEvent, reported by Rub�n Mart�n Hidalgo. (204750 lines)
        Version 15.256.1: Sep 19, 2016	Fixed bug in web service while sending a message.
					Added default values in some database fields. (204741 lines)
					6 changes necessary in database:
ALTER TABLE msg_content CHANGE COLUMN ImageName ImageName VARCHAR(43) NOT NULL DEFAULT '';
ALTER TABLE msg_content CHANGE COLUMN ImageTitle ImageTitle VARCHAR(255) NOT NULL DEFAULT '';
ALTER TABLE msg_content CHANGE COLUMN ImageURL ImageURL VARCHAR(255) NOT NULL DEFAULT '';
ALTER TABLE msg_content_deleted CHANGE COLUMN ImageName ImageName VARCHAR(43) NOT NULL DEFAULT '';
ALTER TABLE msg_content_deleted CHANGE COLUMN ImageTitle ImageTitle VARCHAR(255) NOT NULL DEFAULT '';
ALTER TABLE msg_content_deleted CHANGE COLUMN ImageURL ImageURL VARCHAR(255) NOT NULL DEFAULT '';

        Version 15.256:   Sep 19, 2016	PRADO web service removed from swad.ugr.es. (204722 lines)
        Version 15.255:   Sep 18, 2016	Migration to new server. (204804 lines)
        Version 15.254.2: Sep 13, 2016	Fixed bug while getting summary and content of a social note related to a new file. (204706 lines)
        Version 15.254.1: Sep 13, 2016	Fixed bugs related to tests, reported by Rafael Montoya Ju�rez. (204702 lines)
        Version 15.254:   Sep 12, 2016	Hits per year in user's usage report. (204685 lines)
        Version 15.253.1: Sep 12, 2016	Fixed bug in user's usage report. (204609 lines)
        Version 15.253:   Sep 12, 2016	Changes in layout of hits per month in user's usage report. (204608 lines)
        Version 15.252.1: Sep 12, 2016	Hits per month in user's usage report. Not finished. (204619 lines)
        Version 15.252:   Sep 12, 2016	Hits per month in user's usage report. Not finished. (204589 lines)
        Version 15.251.2: Sep 11, 2016	Changes in layout of user's usage report. (204465 lines)
        Version 15.251.1: Sep 11, 2016	Changes in user's usage report. (204406 lines)
        Version 15.251:   Sep 11, 2016	List of courses in user's usage report. (204416 lines)
        Version 15.250.10:Sep 11, 2016	Details of user's profile removed from user's usage report. (? lines)
        Version 15.250.9: Sep 11, 2016	Number of courses in user's usage report. (204307 lines)
        Version 15.250.8: Sep 11, 2016	Number of messages sent in user's usage report. (204278 lines)
        Version 15.250.7: Sep 11, 2016	Number of posts in forums in user's usage report. (204235 lines)
        Version 15.250.6: Sep 11, 2016	Number of downloads in user's usage report. (204207 lines)
        Version 15.250.5: Sep 11, 2016	Number of clicks in user's usage report. (204189 lines)
        Version 15.250.4: Sep 11, 2016	Time of first click in user's usage report. (204171 lines)
        Version 15.250.3: Sep 11, 2016	Number of puslished files in user's usage report. (? lines)
        Version 15.250.2: Sep 11, 2016	Record card replaced by raw user's data in user's usage report. (204100 lines)
        Version 15.250.1: Sep 09, 2016	Changes in layout of user's usage report. (204056 lines)
        Version 15.250:   Sep 09, 2016	New option to print user's usage report. (204053 lines)
        Version 15.249.5: Sep 08, 2016	Changes in layout of user's usage report. (204019 lines)
        Version 15.249.4: Sep 08, 2016	List of courses in user's usage report. (203993 lines)
        Version 15.249.3: Sep 08, 2016	Fixed bug related with search of users. (203983 lines)
        Version 15.249.2: Sep 08, 2016	Shared record card and details of user's profile are shown in user's usage report. (203978 lines)
        Version 15.249.1: Sep 08, 2016	Removed lines with NULL values in swad_text.c. (203977 lines)
        Version 15.249:   Sep 08, 2016	New option Statistics > Report. Not implemented. (204341 lines)
        Version 15.248.3: Sep 07, 2016	Fixed bug related to attendance events in web service, reported by Rub�n Mart�n Hidalgo. (? lines)
        Version 15.248.2: Sep 07, 2016	Code refactoring related to boolean fields in database. (204201 lines)
        Version 15.248.1: Sep 07, 2016	Fixed bugs related to hidden attendance events, reported by Rub�n Mart�n Hidalgo. (204200 lines)
        Version 15.248:   Sep 06, 2016	New web service function removeAttendanceEvent. (204186 lines)
        Version 15.247.2: Jul 30, 2016	Fixed bug in listings of users, reported by Jos� Abraham Mora S�nchez. (204104 lines)
        Version 15.247.1: Jul 27, 2016	In listings of users ready for printing, show institution name instead of logo. (204102 lines)
        Version 15.247:   Jul 26, 2016	In listings of users, show institution logo with link to institution. (204093 lines)
        Version 15.246.2: Jul 26, 2016	List of users in web service use the list filled instead of querying database a second time to get all user's data.
                                        Code refactoring in copying of user's data in web service. (204089 lines)
        Version 15.246.1: Jul 26, 2016	Attendance of users and email to users use the list filled instead of querying database a second time to get all user's data. (204093 lines)
        Version 15.246:   Jul 26, 2016	Classphoto of users uses the list filled instead of querying database a second time to get all user's data.
                                        Origin place is removed from classphoto. (204094 lines)
        Version 15.245.5: Jul 26, 2016	Fixed bug in user's data. (204116 lines)
        Version 15.245.4: Jul 26, 2016	Fixed bug in user's data. (204100 lines)
        Version 15.245.3: Jul 25, 2016	Listing of guests uses the list filled instead of querying database a second time to get all user's data. (204088 lines)
        Version 15.245.2: Jul 25, 2016	Listing of students/teachers uses the list filled instead of querying database a second time to get all user's data. (204088 lines)
        Version 15.245.1: Jul 25, 2016	Code refactoring in building of queries to get/search users. (204081 lines)
        Version 15.245:   Jul 25, 2016	List of users is filled with user's name and user's institution.
					Search of users uses the list filled instead of querying database a second time to get all user's data. (204192 lines)
        Version 15.244:   Jul 25, 2016	Removed email column from some lists of users (usually it's not necessary). (203969 lines)
        Version 15.243.2: Jul 24, 2016	Guests now have permission to write messages. (203963 lines)
        Version 15.243.1: Jul 21, 2016	Fixed bugs in web service. (203962 lines)
        Version 15.243:   Jul 20, 2016	Fixed bug in web service (getNotifications).
					Code refactoring in web service. (203956 lines)
        Version 15.242.6: Jul 15, 2016	Fixed bug in creation of database table tst_config. (203850 lines)
        Version 15.242.5: Jul 15, 2016	Fixed bug in user's registration. (203849 lines)
        Version 15.242.4: Jul 15, 2016	Fixed bug in creation of database table file_browser_size. (203847 lines)
        Version 15.242.3: Jul 14, 2016	Fixed bug in getting/finding users, due to default mysql mode in 5.7 which is ONLY_FULL_GROUP_BY. (203846 lines)
        Version 15.242.2: Jul 08, 2016	Code refactoring related to users' photos.
					Fixed bug in chat. (203778 lines)
        Version 15.242.1: Jul 08, 2016	URLs for users' photos in web service are sent with https instead of http. (203782 lines)
        Version 15.242:   Jul 07, 2016	New option Profile > Agenda. Not implemented. (203781 lines)
        Version 15.241.7: Jul 06, 2016	Removed cookies in swad.ugr.es.
					Removed unused database tables. (203641 lines)
					1 change necessary in database:
DROP TABLE IF EXISTS notif_backup,sta_notif_backup,usr_data_backup;

        Version 15.241.6: Jul 04, 2016	Added path to information about file in notifications. (203630 lines)
        Version 15.241.5: Jul 04, 2016	Fix bug in form to compose message. (203628 lines)
        Version 15.241.4: Jul 04, 2016	Changes in form to compose message. (203624 lines)
        Version 15.241.3: Jul 04, 2016	Changes in layout of form to compose message.
					Code optimization in list of selected users. (203628 lines)
        Version 15.241.2: Jul 04, 2016	Changes in layout of form to compose message. (203619 lines)
        Version 15.241.1: Jul 04, 2016	Code refactoring related with forms submit. (203620 lines)
        Version 15.241:   Jul 04, 2016	When composing a new message, subject and content are shown even if list of users is too big, in order to keep messages that had begun to be written. (203618 lines)
        Version 15.240.5: Jul 04, 2016	Code refactoring related to big lists of users. (203595 lines)
        Version 15.240.4: Jul 04, 2016	Fixed bugs in web service, reported by Rub�n Mart�n Hidalgo. (203592 lines)
        Version 15.240.3: Jul 04, 2016	N.A. year is shown at foot in edition of courses. (? lines)
        Version 15.240.2: Jul 02, 2016	Fixed bug in edition of courses. (203548 lines)
        Version 15.240.1: Jul 02, 2016	Fixed bug in web service. (203539 lines)
        Version 15.240:   Jul 01, 2016	Copy subject and content of message to hidden param when clicking on link. (203534 lines)
        Version 15.239:   Jul 01, 2016	Code refactoring related to contextual menus. (203531 lines)
        Version 15.238.1: Jul 01, 2016	New javascript function to animate icon. (203292 lines)
        Version 15.238:   Jul 01, 2016	Hidden fields in in selectors of class photo / list with message subject and content. (203274 lines)
        Version 15.237.3: Jul 01, 2016	Fixed bug in messages. (203257 lines)
        Version 15.237.2: Jul 01, 2016	Fixed minor problem in textarea for message content. (203261 lines)
        Version 15.237.1: Jul 01, 2016	Code refactoring in function to send email to students. (203254 lines)
        Version 15.237:   Jul 01, 2016	The form "Show more recipients" now have a hidden field with the content. (203243 lines)
        Version 15.236:   Jun 30, 2016	The form "Show more recipients" now have a hidden field with the subject. (? lines)
        Version 15.235.1: Jun 30, 2016	Minor change in icon to download files. (203206 lines)
Copy icons icon/download64x64.png

        Version 15.235:   Jun 30, 2016	Some file metadata are written in content of notifications. (203204 lines)
        Version 15.234:   Jun 29, 2016	New web service functions getUsers and findUsers. (203169 lines)
        Version 15.233.4: Jun 27, 2016	Changes in links in list of notifications. (203065 lines)
        Version 15.233.3: Jun 27, 2016	Fixed bug in list of admins. (203074 lines)
        Version 15.233.2: Jun 27, 2016	Code refactoring in search of users. (203072 lines)
        Version 15.233.1: Jun 27, 2016	New restrictions in length of search string. (203117 lines)
        Version 15.233:   Jun 27, 2016	Simplified search for users with a role. Now all users can be found. (203103 lines)
        Version 15.232.1: Jun 27, 2016	Fixed bug in search of any user (acceptation of enrolment was wrong). (203702 lines)
        Version 15.232:   Jun 27, 2016	Search any user, independently on role. (203657 lines)
        Version 15.231.4: Jun 24, 2016	Fix bug (buffer overflow) when getting users. (203474 lines)
        Version 15.231.3: Jun 24, 2016	Parameter name used for "Scope" now depends on context. (203472 lines)
        Version 15.231.2: Jun 24, 2016	Search is restricted by default to the smaller (down in hierarchy) scope. (203475 lines)
        Version 15.231.1: Jun 24, 2016	All users can search guests. (203429 lines)
        Version 15.231:   Jun 24, 2016	Speed up in search of users by using temporary tables. (203427 lines)
        Version 15.230.4: Jun 23, 2016	Optimization in search of users. (203369 lines)
        Version 15.230.3: Jun 23, 2016	Optimization in search of users. (203378 lines)
        Version 15.230.2: Jun 23, 2016	Fixed bug in search of users. (203388 lines)
        Version 15.230.1: Jun 23, 2016	Fixed bug in search of users. (203375 lines)
        Version 15.230:   Jun 23, 2016	Optimization and bug fixing in search of users. (203374 lines)
        Version 15.229:   Jun 23, 2016	Search of students available for all users. (203194 lines)
        Version 15.228.2: Jun 23, 2016	Code refactoring in lists of users. (202791 lines)
        Version 15.228.1: Jun 23, 2016	Code refactoring in lists of users. (202794 lines)
        Version 15.228:   Jun 23, 2016	Code refactoring in lists of users. (202814 lines)
        Version 15.227.1: Jun 23, 2016	Code refactoring in getting/searching users. (202854 lines)
        Version 15.227:   Jun 21, 2016	Query optimization in search of users. (202820 lines)
        Version 15.226.7: Jun 19, 2016	One new social network. (202969 lines)
					1 change necessary in database:
ALTER TABLE usr_webs CHANGE Web Web ENUM('www', '500px', 'delicious', 'deviantart', 'diaspora', 'edmodo', 'facebook', 'flickr', 'foursquare', 'github', 'gnusocial', 'googleplus', 'googlescholar', 'identica', 'instagram', 'linkedin', 'orcid', 'paperli', 'pinterest', 'quitter', 'researchgate', 'researcherid', 'scoopit', 'slideshare', 'stackoverflow', 'storify', 'tumblr', 'twitter', 'wikipedia', 'youtube') NOT NULL;
Copy icon stackoverflow64x64.png to icon directory

        Version 15.226.6: Jun 18, 2016	Changes in query about similar users. (202962 lines)
        Version 15.226.5: Jun 18, 2016	Code refactoring in duplicate users. (202948 lines)
        Version 15.226.4: Jun 17, 2016	Translation of buttons in list of similar users. (202934 lines)
        Version 15.226.3: Jun 17, 2016	Request the removal of a user account from list of similar users. (202912 lines)
        Version 15.226.2: Jun 17, 2016	New action to remove a user from list of possible duplicate users. (202921 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1580','es','N','Quitar de usuarios duplicados');

        Version 15.226.1: Jun 17, 2016	Changed layout of listing of similar users. (202887 lines)
        Version 15.226:   Jun 17, 2016	Fixed bug in photo zoom. (202870 lines)
        Version 15.225.21:Jun 17, 2016	Fixed bug in photo zoom, reported by Javier Fern�ndez Baldomero. (202894 lines)
        Version 15.225.20:Jun 17, 2016	Show details of user's profile in similar users. (202893 lines)
        Version 15.225.19:Jun 17, 2016	Changed layout of listing of similar users. (202927 lines)
        Version 15.225.18:Jun 17, 2016	Show number of files of a user in similar users. (202904 lines)
        Version 15.225.17:Jun 17, 2016	Show similar users. Not finished. (202878 lines)
        Version 15.225.16:Jun 16, 2016	New action to show similar users. Not finished. (202887 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1579','es','N','Listar usuarios similares');

        Version 15.225.15:Jun 16, 2016	Button in possible duplicate user to show similar users. (202781 lines)
        Version 15.225.14:Jun 16, 2016	Code refactoring in listing of users. (202741 lines)
        Version 15.225.13:Jun 16, 2016	Check if a user in listing of possible duplicate users has accepted all his/her courses. (202790 lines)
        Version 15.225.12:Jun 16, 2016	New module swad_duplicate for possible duplicate users. (202758 lines)
        Version 15.225.11:Jun 16, 2016	Listing possible duplicate users. (202667 lines)
        Version 15.225.10:Jun 15, 2016	Listing possible duplicate users. (202626 lines)
        Version 15.225.9: Jun 15, 2016	Code refactoring in list of students and teachers. (202636 lines)
        Version 15.225.8: Jun 15, 2016	Code refactoring in list of students and teachers. (202779 lines)
        Version 15.225.7: Jun 15, 2016	Listing possible duplicate users. (202775 lines)
        Version 15.225.6: Jun 15, 2016	Listing possible duplicate users. (202761 lines)
        Version 15.225.5: Jun 15, 2016	Simplification in texts (links) to view guests and admins. (202586 lines)
        Version 15.225.4: Jun 15, 2016	Listing possible duplicate users. Not finished. (202623 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1578','es','N','Listar usuarios duplicados');

        Version 15.225.3: Jun 15, 2016	New database table to report users as possible duplicates. (202550 lines)
					1 change necessary in database:
CREATE TABLE IF NOT EXISTS usr_duplicated (UsrCod INT NOT NULL,InformerCod INT NOT NULL,InformTime DATETIME NOT NULL,UNIQUE INDEX(UsrCod,InformerCod),INDEX(UsrCod));

        Version 15.225.2: Jun 15, 2016	New option in user administration to report a user as possible duplicate. Not finished. (202497 lines)
        Version 15.225.1: Jun 14, 2016	New option in user administration to report a user as possible duplicate. Not finished. (202468 lines)
        Version 15.225:   Jun 14, 2016	Removing a user's photo now requires confirmation. (202425 lines)
					5 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1574','es','N','Preguntar si eliminar foto otro usr.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1575','es','N','Preguntar si eliminar foto estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1576','es','N','Preguntar si eliminar foto profesor');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1577','es','N','Preguntar si eliminar mi foto');
UPDATE actions SET Txt='Eliminar mi foto' WHERE ActCod='428' AND Language='es';

        Version 15.224.3: Jun 13, 2016	Fix bug while finding cell content in file of marks. (202266 lines)
        Version 15.224.2: Jun 12, 2016	Info about number of files is moved to bottom of file browsers. (202266 lines)
        Version 15.224.1: Jun 12, 2016	Change in layout of file browsers. (202266 lines)
        Version 15.224:   Jun 12, 2016	Now a user can unfollow users followed by him/her whose profile is not visible.
					Code refactoring in list of follwing/followers. (202304 lines)
        Version 15.223.3: Jun 12, 2016	Fixed layout of list of forums. (202282 lines)
        Version 15.223.2: Jun 12, 2016	Fixed bug in results of search of students. (202277 lines)
        Version 15.223.1: Jun 12, 2016	Show only users to follow with first name and surname 1. (202276 lines)
        Version 15.223:   Jun 12, 2016	New forms to change course configuration. (202268 lines)
					4 changes necessary in database:
UPDATE actions SET Txt='Cambiar c&oacute;d. institucional asignatura' WHERE ActCod='1024' AND Language='es';
UPDATE actions SET Txt='Cambiar c&oacute;d. institucional asignatura' WHERE ActCod='1025' AND Language='es';
UPDATE actions SET Txt='Cambiar a&ntilde;o/semestre asignatura' WHERE ActCod='561' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1573','es','N','Cambiar a&ntilde;o/semestre asignatura');

        Version 15.222.6: Jun 11, 2016	Fixed layout in students column in result of search of teachers. (202214 lines)
        Version 15.222.5: Jun 10, 2016	Indicators of a course are always calculated and stored in course configuration. (202214 lines)
        Version 15.222.4: Jun 09, 2016	Code refactoring in indicators. (202208 lines)
        Version 15.222.3: Jun 09, 2016	Indicators form is sended automatically when any parameter changes. (202214 lines)
        Version 15.222.2: Jun 09, 2016	Code refactoring in indicators. (202233 lines)
        Version 15.222.1: Jun 09, 2016	Optimization in number of indicators. (202230 lines)
        Version 15.222:   Jun 09, 2016	Number of indicators is stored in table of courses. (202224 lines)
        Version 15.221.4: Jun 09, 2016	New columns in table of courses to store number of indicators. (202153 lines)
					1 change necessary in database:
ALTER TABLE courses ADD COLUMN NumIndicators INT NOT NULL DEFAULT -1 AFTER FullName;

        Version 15.221.3: Jun 08, 2016	Fix bug in calculation of number of files in groups of a course. (202146 lines)
        Version 15.221.2: Jun 08, 2016	Fix bug in calculation of number of files in groups of a course. (202149 lines)
        Version 15.221.1: Jun 08, 2016	Fix bug in form of indicators.
				        Speed up database queries to get course info when getting indicators. (? lines)
        Version 15.221:   Jun 08, 2016	Single (radio) selection of number of indicators changed to multiple (checkbox) selection. (202028 lines)
        Version 15.220.1: Jun 08, 2016	Code refactoring in form to view indicators of courses. (202031 lines)
        Version 15.220:   Jun 08, 2016	Number of indicators now appears inside form. (202029 lines)
        Version 15.219.1: Jun 05, 2016	Highlighted row with number of indicators selected. (202023 lines)
        Version 15.219:   Jun 04, 2016	Changes in listing of indicators of courses. (202001 lines)
        Version 15.218.5: Jun 04, 2016	Fix bug in social notes, reported by Raymon Moreno Colina. (201940 lines)
					2 changes necessary in database:
DROP INDEX NoteType ON social_notes;
CREATE INDEX NoteType ON social_notes (NoteType,Cod);

        Version 15.218.4: Jun 04, 2016	User can change license of files in briefcase.
					Code refactoring related to file metadata. (201936 lines)
        Version 15.218.3: Jun 04, 2016	Code refactoring related to setlocale. (201952 lines)
        Version 15.218.2: Jun 04, 2016	Checked value returned by setlocale in all calls. (201957 lines)
        Version 15.218.1: Jun 04, 2016	Change in listing of courses. (201934 lines)
        Version 15.218:   Jun 03, 2016	New module swad_MFU for most frequently used actions.
					Change in layout of most frequent actions. (201934 lines)
        Version 15.217.2: Jun 03, 2016	Fixed bug in copy-paste from briefcase to another file browser. (201827 lines)
        Version 15.217.1: Jun 03, 2016	Fixed bug when creating zip file for students works, reported by Francisco Barranco Exp�sito. (201809 lines)
        Version 15.217:   Jun 01, 2016	When clicking on a social note associated to an exam announcement, show that exam announcement highlighted. (201801 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1572','es','N','Ver convocatoria');

        Version 15.216:   Jun 01, 2016	When clicking on a day on calendar, all exam announcements in that day are shown highlighted. (201751 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1571','es','N','Ver convocatorias de una fecha');

        Version 15.215:   Jun 01, 2016	List exam announcements as a list, like notices. (201670 lines)
        Version 15.214:   May 31, 2016	Fixed bug in renaming test tags. (201589 lines)
        Version 15.213:   May 31, 2016	Code refactoring related to renaming test tags.
					Fixed bug in dates, reported by Christian A. Morillas Guti�rrez. (201581 lines)
        Version 15.212.9: May 30, 2016	Link to list institutions of country in country configuration. (201567 lines)
        Version 15.212.8: May 30, 2016	Link to list centres of institution in institution configuration. (201555 lines)
        Version 15.212.7: May 30, 2016	Link to list degrees of centre in centre configuration. (201547 lines)
        Version 15.212.6: May 30, 2016	Link to list courses of degree in degree configuration. (201539 lines)
        Version 15.212.5: May 30, 2016	Fixed bug in number of courses of a degree. (201531 lines)
        Version 15.212.4: May 30, 2016	Changed links from external degrees to internal degrees in STATS > Degrees. (201529 lines)
        Version 15.212.3: May 30, 2016	Fixed bug in test question. A student can see now his/her test results. (201522 lines)
        Version 15.212.2: May 30, 2016	Fixed bug in test question. Button "Add a new question" in tests will not be available for students. (201516 lines)
        Version 15.212.1: May 30, 2016	Fixed bug in course information. (201515 lines)
        Version 15.212:   May 30, 2016	Fixed bug in edition of syllabus, reported by Raymon Moreno Colina and Javier Fern�ndez Baldomero.
					Code refactoring related to course info. (201471 lines)
        Version 15.211.2: May 13, 2016	Fixed bug in test questions, reported by Nuria Torres Rosell. (201394 lines)
        Version 15.211.1: May 07, 2016	Files in briefcase older than 1 year are removed automatically. (201397 lines)
        Version 15.211:   May 07, 2016	Code refactoring related to removing old files in briefcase.
                                        Old files in briefcase are removed automatically. (201396 lines)
        Version 15.210.4: May 05, 2016	Link to change logo of degree is moved to top of window. (201385 lines)
        Version 15.210.3: May 05, 2016	Link to change logo of centre is moved to top of window. (201407 lines)
        Version 15.210.2: May 05, 2016	Link to change logo of institution is moved to top of window. (201399 lines)
        Version 15.210.1: May 05, 2016	New icon to change logo of institution/centre/degree. (201388 lines)
					1 change necessary in icons:
Copy file icon/logo64x64.png to public icon directory

        Version 15.210:   May 05, 2016	Commands/shortcuts in record are removed from left of window. (201389 lines)
        Version 15.209:   May 05, 2016	Commands/shortcuts in record are cloned to top of window. (201614 lines)
        Version 15.208.6: May 05, 2016	Integrated "Configure" link in tests at top of window, near "Edit" icon. (201463 lines)
        Version 15.208.5: May 05, 2016	Changes in Statistics > Degrees. (201469 lines)
        Version 15.208.4: May 05, 2016	In Statistics > Degrees, show all degrees with students. (201458 lines)
        Version 15.208.3: May 03, 2016	Fixed bug in sessions database table when no social publishings available. (201446 lines)
        Version 15.208.2: May 01, 2016	Add default values in some database fields. (201439 lines)
					1 change necessary in database:
ALTER TABLE sessions CHANGE COLUMN SearchString SearchString VARCHAR(255) NOT NULL DEFAULT '';

        Version 15.208.2: May 01, 2016	Fixed bug in user's last data. (201439 lines)
        Version 15.208.1: May 01, 2016	Minor fix to avoid warning related to size_t. (201438 lines)
        Version 15.208:   May 01, 2016	Changes in config for swadberry.ugr.es.
					Removed cevuna.una.py from config. (201435 lines)
        Version 15.207.2: May 01, 2016	Fixed bug in database table log_full. (201515 lines)
        Version 15.207.1: May 01, 2016	Minor fix to avoid warning related to char type limits. (201515 lines)
        Version 15.207:   May 01, 2016	Add default values for fields in users' data table. (201514 lines)
					Birthday default value now is NULL.
					18 changes necessary in database:
ALTER TABLE usr_data CHANGE COLUMN EncryptedUsrCod EncryptedUsrCod CHAR(43) NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN Password Password CHAR(86) COLLATE latin1_bin NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN Surname1 Surname1 VARCHAR(32) COLLATE latin1_spanish_ci NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN Surname2 Surname2 VARCHAR(32) COLLATE latin1_spanish_ci NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN FirstName FirstName VARCHAR(32) COLLATE latin1_spanish_ci NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN Theme Theme CHAR(16) NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN IconSet IconSet CHAR(16) NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN Language Language CHAR(2) NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN Photo Photo CHAR(43) NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN Office Office VARCHAR(127) NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN OfficePhone OfficePhone CHAR(16) NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN LocalAddress LocalAddress VARCHAR(127) NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN LocalPhone LocalPhone CHAR(16) NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN FamilyAddress FamilyAddress VARCHAR(127) NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN FamilyPhone FamilyPhone CHAR(16) NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN OriginPlace OriginPlace VARCHAR(127) NOT NULL DEFAULT '';
ALTER TABLE usr_data CHANGE COLUMN Birthday Birthday DATE;
UPDATE usr_data SET Birthday=NULL WHERE Birthday='0000-00-00';

        Version 15.206:   Apr 30, 2016	Remove default DATETIME fields to avoid errors if 0 is not allowed in date in database. (201424 lines)
					7 changes necessary in database:
ALTER TABLE connected CHANGE COLUMN LastTime LastTime DATETIME NOT NULL;
ALTER TABLE crs_grp_types CHANGE COLUMN OpenTime OpenTime DATETIME NOT NULL;
ALTER TABLE crs_last CHANGE COLUMN LastTime LastTime DATETIME NOT NULL;
ALTER TABLE sessions CHANGE COLUMN LastTime LastTime DATETIME NOT NULL;
ALTER TABLE sessions CHANGE COLUMN LastRefresh LastRefresh DATETIME NOT NULL;
ALTER TABLE usr_last CHANGE COLUMN LastTime LastTime DATETIME NOT NULL;
ALTER TABLE usr_last CHANGE COLUMN LastAccNotif LastAccNotif DATETIME NOT NULL;

        Version 15.205:   Apr 30, 2016	New demo server added to swad_config.h: swadberry.ugr.es. (201409 lines)
        Version 15.204.1: Apr 25, 2016	Fixed bug in changes in database necessary in version 15.201. (201366 lines)
					6 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1565','es','N','Solicitar conf. ID invitado');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1566','es','N','Solicitar conf. ID estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1567','es','N','Solicitar conf. ID profesor');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1568','es','N','Confirmar ID invitado');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1569','es','N','Confirmar ID estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1570','es','N','Confirmar ID profesor');

        Version 15.204:   Apr 25, 2016	New form to confirm another user's ID. (201326 lines)
        Version 15.203.6: Apr 24, 2016	New form to confirm another user's ID. Not finished. (201295 lines)
        Version 15.203.5: Apr 24, 2016	New form to confirm another user's ID. Not finished. (201230 lines)
        Version 15.203.4: Apr 23, 2016	New form to confirm another user's ID. Not finished. (201140 lines)
        Version 15.203.3: Apr 23, 2016	New form to confirm another user's ID. Not finished. (201113 lines)
        Version 15.203.2: Apr 23, 2016	New form to confirm another user's ID. Not finished. (201104 lines)
        Version 15.203.1: Apr 23, 2016	New form to confirm another user's ID. Not finished. (201090 lines)
        Version 15.203:   Apr 23, 2016	Changes in layout and CSS related to record card. (201076 lines)
        Version 15.202.19:Apr 23, 2016	User's ID is shown in green or red in user's account. (201120 lines)
        Version 15.202.18:Apr 23, 2016	Change of message about ID in user's account. (201092 lines)
        Version 15.202.17:Apr 23, 2016	Code refactoring in record card. (201081 lines)
        Version 15.202.16:Apr 23, 2016	Code refactoring in record card. (201070 lines)
        Version 15.202.15:Apr 23, 2016	Code refactoring in record card. (201059 lines)
        Version 15.202.14:Apr 23, 2016	Code refactoring in record card. (201048 lines)
        Version 15.202.13:Apr 23, 2016	Code refactoring in record card. (201037 lines)
        Version 15.202.12:Apr 23, 2016	Code refactoring in record card. (201026 lines)
        Version 15.202.11:Apr 23, 2016	Code refactoring in record card. (201014 lines)
        Version 15.202.10:Apr 23, 2016	Code refactoring in record card. (201000 lines)
        Version 15.202.9: Apr 23, 2016	Code refactoring in record card. (200987 lines)
        Version 15.202.8: Apr 23, 2016	Code refactoring in record card. (200979 lines)
        Version 15.202.7: Apr 23, 2016	Code refactoring in record card. (200964 lines)
        Version 15.202.6: Apr 23, 2016	Code refactoring in record card. (200949 lines)
        Version 15.202.5: Apr 23, 2016	Code refactoring in record card. (200934 lines)
        Version 15.202.4: Apr 23, 2016	Code refactoring in record card. (200926 lines)
        Version 15.202.3: Apr 23, 2016	Code refactoring in record card. (200911 lines)
        Version 15.202.2: Apr 23, 2016	Code refactoring in record card. (200899 lines)
        Version 15.202.1: Apr 23, 2016	Code refactoring related to showing user's email. (200887 lines)
        Version 15.202:   Apr 23, 2016	User's IDs are show in green or red.
					Code refactoring and bug fixing related to showing user's ID and email. (200908 lines)
        Version 15.201.9: Apr 22, 2016	Code refactoring in record card.
					Changes in layout of record cards. (200895 lines)
        Version 15.201.8: Apr 22, 2016	Code refactoring in record card.
					Fixed bugs in record cards. (200881 lines)
        Version 15.201.7: Apr 22, 2016	Code refactoring in record card.
					Fixed bugs in record cards. (200862 lines)
        Version 15.201.6: Apr 22, 2016	Code refactoring in record card. (200842 lines)
        Version 15.201.5: Apr 22, 2016	Code refactoring in record card. (200832 lines)
        Version 15.201.4: Apr 22, 2016	Code refactoring in record card. (200822 lines)
        Version 15.201.3: Apr 22, 2016	Code refactoring in record card. (200812 lines)
        Version 15.201.2: Apr 22, 2016	Code refactoring in record card. (200806 lines)
        Version 15.201.1: Apr 22, 2016	Code refactoring in record card. (200778 lines)
        Version 15.201:   Apr 21, 2016	New private file zone for the teachers who belong to a course. (200768 lines)
					82 changes necessary in database:
CREATE TABLE notif_backup LIKE notif;
INSERT INTO notif_backup SELECT * FROM notif;

CREATE TABLE sta_notif_backup LIKE sta_notif;
INSERT INTO sta_notif_backup SELECT * FROM sta_notif;

CREATE TABLE usr_data_backup LIKE usr_data;
INSERT INTO usr_data_backup SELECT * FROM usr_data;

UPDATE notif SET NotifyEvent=19 WHERE NotifyEvent=18;
UPDATE notif SET NotifyEvent=18 WHERE NotifyEvent=17;
UPDATE notif SET NotifyEvent=17 WHERE NotifyEvent=16;
UPDATE notif SET NotifyEvent=16 WHERE NotifyEvent=15;
UPDATE notif SET NotifyEvent=15 WHERE NotifyEvent=14;
UPDATE notif SET NotifyEvent=14 WHERE NotifyEvent=13;
UPDATE notif SET NotifyEvent=13 WHERE NotifyEvent=12;
UPDATE notif SET NotifyEvent=12 WHERE NotifyEvent=11;
UPDATE notif SET NotifyEvent=11 WHERE NotifyEvent=10;
UPDATE notif SET NotifyEvent=10 WHERE NotifyEvent=9;
UPDATE notif SET NotifyEvent=9  WHERE NotifyEvent=8;
UPDATE notif SET NotifyEvent=8  WHERE NotifyEvent=7;
UPDATE notif SET NotifyEvent=7  WHERE NotifyEvent=6;
UPDATE notif SET NotifyEvent=6  WHERE NotifyEvent=5;
UPDATE notif SET NotifyEvent=5  WHERE NotifyEvent=4;
UPDATE notif SET NotifyEvent=4  WHERE NotifyEvent=3;
UPDATE notif SET NotifyEvent=3  WHERE NotifyEvent=2;

UPDATE sta_notif SET NotifyEvent=19 WHERE NotifyEvent=18;
UPDATE sta_notif SET NotifyEvent=18 WHERE NotifyEvent=17;
UPDATE sta_notif SET NotifyEvent=17 WHERE NotifyEvent=16;
UPDATE sta_notif SET NotifyEvent=16 WHERE NotifyEvent=15;
UPDATE sta_notif SET NotifyEvent=15 WHERE NotifyEvent=14;
UPDATE sta_notif SET NotifyEvent=14 WHERE NotifyEvent=13;
UPDATE sta_notif SET NotifyEvent=13 WHERE NotifyEvent=12;
UPDATE sta_notif SET NotifyEvent=12 WHERE NotifyEvent=11;
UPDATE sta_notif SET NotifyEvent=11 WHERE NotifyEvent=10;
UPDATE sta_notif SET NotifyEvent=10 WHERE NotifyEvent=9;
UPDATE sta_notif SET NotifyEvent=9  WHERE NotifyEvent=8;
UPDATE sta_notif SET NotifyEvent=8  WHERE NotifyEvent=7;
UPDATE sta_notif SET NotifyEvent=7  WHERE NotifyEvent=6;
UPDATE sta_notif SET NotifyEvent=6  WHERE NotifyEvent=5;
UPDATE sta_notif SET NotifyEvent=5  WHERE NotifyEvent=4;
UPDATE sta_notif SET NotifyEvent=4  WHERE NotifyEvent=3;
UPDATE sta_notif SET NotifyEvent=3  WHERE NotifyEvent=2;

UPDATE usr_data  SET NotifNtfEvents = (((NotifNtfEvents & ~0x3) << 1) | (NotifNtfEvents & 0x3) | 0x04);
UPDATE usr_data  SET EmailNtfEvents = (((EmailNtfEvents & ~0x3) << 1) | (EmailNtfEvents & 0x3));

INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1525','es','N','Administrar zona de profesores');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1526','es','N','Cambiar a admin. archivos prof.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1527','es','N','Administrar archivos prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1528','es','N','Solicitar elim. arch. prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1529','es','N','Eliminar archivo prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1530','es','N','Eliminar carpeta prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1531','es','N','Copiar de zona prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1532','es','N','Pegar en zona prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1533','es','N','Eliminar &aacute;rbol prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1534','es','N','Solic.nuevo arch/carp prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1535','es','N','Crear carpeta prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1536','es','N','Crear enlace prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1537','es','N','Renombrar carpeta prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1538','es','N','Subir archivo prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1539','es','N','Subir archivo prof. asg. (ant.)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1540','es','N','Expandir carpeta prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1541','es','N','Contraer carpeta prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1542','es','N','Crear ZIP prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1543','es','N','Ver metadatos arch. prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1544','es','N','Cambiar metadatos arch. prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1545','es','N','Descargar arch. prof. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1546','es','N','Administrar archivos prof. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1547','es','N','Solicitar elim. arch. prof. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1548','es','N','Eliminar archivo prof. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1549','es','N','Eliminar carpeta prof. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1550','es','N','Copiar de zona prof. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1551','es','N','Pegar en zona prof. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1552','es','N','Eliminar &aacute;rbol prof. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1553','es','N','Solic.nuevo arch/carp prof. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1554','es','N','Crear carpeta prof. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1555','es','N','Crear enlace prof. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1556','es','N','Renombrar carpeta prof. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1557','es','N','Subir archivo prof. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1558','es','N','Subir archivo prof. asg. (grp.)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1559','es','N','Expandir carpeta prof. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1560','es','N','Contraer carpeta prof. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1561','es','N','Crear ZIP prof. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1562','es','N','Ver metadatos arch. prof. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1563','es','N','Cambiar metadatos arch. prof. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1564','es','N','Descargar arch. prof. grp.');

        Version 15.200.5: Apr 18, 2016	Removed link to enter from external service. (199764 lines)
        Version 15.200.4: Apr 18, 2016	Changes in configuration. (199785 lines)
        Version 15.200.3: Apr 18, 2016	Fixed bug in tests, reported by Miguel Damas Hermoso. (199788 lines)
        Version 15.200.2: Apr 18, 2016	Fixed bugs in searching for documents. (199774 lines)
        Version 15.200.1: Apr 18, 2016	Fixed bug in searching for documents. (199726 lines)
        Version 15.200:   Apr 18, 2016	New layout of search form. (199718 lines)
        Version 15.199.8: Apr 18, 2016	Changes in title of results of searching documents. (199723 lines)
        Version 15.199.7: Apr 17, 2016	Changes in title of results of searching users. (199639 lines)
        Version 15.199.6: Apr 17, 2016	Changes in title of results of searching courses. (199647 lines)
        Version 15.199.5: Apr 17, 2016	Changes in title of results of searching degrees. (199652 lines)
        Version 15.199.4: Apr 17, 2016	Changes in title of results of searching centres. (199657 lines)
        Version 15.199.3: Apr 17, 2016	Changes in title of results of searching institutions. (199663 lines)
        Version 15.199.2: Apr 16, 2016	Fixed bug in social comments, reported by Javier Fern�ndez Baldomero. (199666 lines)
        Version 15.199.1: Apr 16, 2016	Fixed bug in Statistics > Degrees. (199665 lines)
        Version 15.199:   Apr 16, 2016	In Statistics > Degrees, show only degrees with students.
                                        Code optimization in number of courses in a degree. (199665 lines)
        Version 15.198:   Apr 15, 2016	Changes in layout related to image URL. (199646 lines)
        Version 15.197:   Apr 15, 2016	Input field to enter image URL. (199518 lines)
        Version 15.196:   Apr 15, 2016	Changes related to image URL. (199467 lines)
        Version 15.195.1: Apr 15, 2016	New fields in images to store URL when a user click in the image. (199390 lines)
					7 changes necessary in database:
ALTER TABLE tst_questions ADD COLUMN ImageURL VARCHAR(255) NOT NULL AFTER ImageTitle;
ALTER TABLE tst_answers ADD COLUMN ImageURL VARCHAR(255) NOT NULL AFTER ImageTitle;
ALTER TABLE social_posts ADD COLUMN ImageURL VARCHAR(255) NOT NULL AFTER ImageTitle;
ALTER TABLE social_comments ADD COLUMN ImageURL VARCHAR(255) NOT NULL AFTER ImageTitle;
ALTER TABLE forum_post ADD COLUMN ImageURL VARCHAR(255) NOT NULL AFTER ImageTitle;
ALTER TABLE msg_content ADD COLUMN ImageURL VARCHAR(255) NOT NULL AFTER ImageTitle;
ALTER TABLE msg_content_deleted ADD COLUMN ImageURL VARCHAR(255) NOT NULL AFTER ImageTitle;

        Version 15.195:   Apr 14, 2016	Code refactoring related to uploading images. (199355 lines)
        Version 15.194.2: Apr 14, 2016	Code refactoring related to uploading images. (199384 lines)
        Version 15.194.1: Apr 14, 2016	Code refactoring related to uploading images. (199391 lines)
        Version 15.194:   Apr 14, 2016	Code refactoring related to uploading images. (199391 lines)
        Version 15.193:   Apr 14, 2016	New layout of button used to upload images. (199357 lines)
        Version 15.192.13:Apr 13, 2016	Fixed bug in file upload, reported by Christian Morillas Guti�rrez. (199341 lines)
        Version 15.192.12:Apr 12, 2016	Fixed bug in groups reported by user raistmaj in GitHub. (199339 lines)
        Version 15.192.11:Apr 12, 2016	Image quality in social posts reduced to 50 in order to speed up page loading.
					Change in layout of message post. (199337 lines)
        Version 15.192.10:Apr 11, 2016	Fixed bug in edition of test questions. (199335 lines)
        Version 15.192.9: Apr 11, 2016	Changed layout of form to send attached image in forum posts and messages. (199333 lines)
        Version 15.192.8: Apr 11, 2016	Contextual icon to remove user's photo. (199331 lines)
        Version 15.192.7: Apr 11, 2016	Changes in forms to upload user's photo. (199340 lines)
        Version 15.192.6: Apr 11, 2016	Changes in layout of test edition.
					Changes in forms to upload files. (199359 lines)
        Version 15.192.5: Apr 11, 2016	Fixed bug in edition of tests. (199369 lines)
        Version 15.192.4: Apr 11, 2016	Fixed bug when removing messages. (199370 lines)
        Version 15.192.3: Apr 11, 2016	Fixed bugs when receiving forum image. (199355 lines)
        Version 15.192.2: Apr 11, 2016	Fixed bug when getting forum post content. (199344 lines)
        Version 15.192.1: Apr 10, 2016	Remove image attached to forum post when the post is removed. (199340 lines)
        Version 15.192:   Apr 10, 2016	Get image attached to forum post / message. (199332 lines)
					6 changes necessary in database:
ALTER TABLE forum_post ADD COLUMN ImageName VARCHAR(43) NOT NULL AFTER Content;
ALTER TABLE forum_post ADD COLUMN ImageTitle VARCHAR(255) NOT NULL AFTER ImageName;
ALTER TABLE msg_content ADD COLUMN ImageName VARCHAR(43) NOT NULL AFTER Content;
ALTER TABLE msg_content ADD COLUMN ImageTitle VARCHAR(255) NOT NULL AFTER ImageName;
ALTER TABLE msg_content_deleted ADD COLUMN ImageName VARCHAR(43) NOT NULL AFTER Content;
ALTER TABLE msg_content_deleted ADD COLUMN ImageTitle VARCHAR(255) NOT NULL AFTER ImageName;

        Version 15.191:   Apr 10, 2016	Forms to attach an image to a forum posts and messages. Not finished. (199247 lines)
        Version 15.190.6: Apr 09, 2016	Changes in layout of form to write a new message. (199152 lines)
        Version 15.190.5: Apr 09, 2016	Changes in layout of form to write a new forum post. (199156 lines)
        Version 15.190.4: Apr 09, 2016	Changes in layout of form to write a new forum post. (199152 lines)
        Version 15.190.3: Apr 09, 2016	Change in format of image title/attribution. (199145 lines)
        Version 15.190.2: Apr 09, 2016	Fixed bug in test images. (199144 lines)
        Version 15.190.1: Apr 08, 2016	Removed unused JavaScript code. (199132 lines)
        Version 15.190:   Apr 08, 2016	Get image attached to a social comment. (199142 lines)
					2 changes necessary in database:
ALTER TABLE social_comments ADD COLUMN ImageName VARCHAR(43) NOT NULL AFTER Content;
ALTER TABLE social_comments ADD COLUMN ImageTitle VARCHAR(255) NOT NULL AFTER ImageName;

        Version 15.189.2: Apr 08, 2016	Remove file of the image attached to a social post when the post is removed. (199017 lines)
        Version 15.189.1: Apr 08, 2016	Change in layout of form to write a new social post. (198997 lines)
        Version 15.189:   Apr 08, 2016	Get image attached to a social post. (198991 lines)
					2 changes necessary in database:
ALTER TABLE social_posts ADD COLUMN ImageName VARCHAR(43) NOT NULL AFTER Content;
ALTER TABLE social_posts ADD COLUMN ImageTitle VARCHAR(255) NOT NULL AFTER ImageName;

        Version 15.188:   Apr 08, 2016	Form to attach an image to a social post. Not finished. (198904 lines)
        Version 15.187.2: Apr 08, 2016	Changed CSS of alerts. (198873 lines)
        Version 15.187.1: Apr 08, 2016	Code optimization in function to change format of string. (198872 lines)
        Version 15.187:   Apr 07, 2016	Fixed bug in forms sent using content type multipart/form-data. (198938 lines)
        Version 15.186:   Apr 07, 2016	Changes in edition of a test question. (198840 lines)
        Version 15.185.4: Apr 07, 2016	Changes in edition of a test question. (198832 lines)
        Version 15.185.3: Apr 07, 2016	Changed icons to expand / contract. (198801 lines)
Copy file icon/expand64x64.png to icon directory
Copy file icon/contract64x64.png to icon directory

        Version 15.185.2: Apr 07, 2016	Change in title of take a test. (198797 lines)
        Version 15.185.1: Apr 07, 2016	Change in title of edition of test questions. (198776 lines)
        Version 15.185:   Apr 07, 2016	Fixed bugs in edition of test questions. (198755 lines)
        Version 15.184.8: Apr 07, 2016	Change in edition of a test question. (198708 lines)
        Version 15.184.7: Apr 07, 2016	Change in layout of edition of a test question. (198706 lines)
        Version 15.184.6: Apr 07, 2016	Change in layout of edition of a test question. (198670 lines)
        Version 15.184.5: Apr 07, 2016	Fixed bugs exporting/importing test questions. (198679 lines)
        Version 15.184.4: Apr 06, 2016	Fixed minor bug in layout of test exam. (198676 lines)
        Version 15.184.3: Apr 06, 2016	Fixed minor bug in layout of test exam. (198677 lines)
        Version 15.184.2: Apr 06, 2016	Fixed minor bug in layout of test exam. (198679 lines)
        Version 15.184.1: Apr 06, 2016	Fixed bug when removing a test question.
					Code refactoring in tests. (198680 lines)
        Version 15.184:   Apr 06, 2016	Code refactoring in tests and images.
                                        Fixed bugs in test questions. (198682 lines)
        Version 15.183.1: Apr 06, 2016	Fix bug in feedback of test question. (198706 lines)
        Version 15.183:   Apr 06, 2016	Change in length of title/attribution of images. (198704 lines)
					2 changes necessary in database:
ALTER TABLE tst_questions CHANGE COLUMN ImageTitle ImageTitle VARCHAR(255) NOT NULL;
ALTER TABLE tst_answers CHANGE COLUMN ImageTitle ImageTitle VARCHAR(255) NOT NULL;

        Version 15.182:   Apr 06, 2016	Get fields title/attribution of images from form. (198610 lines)
					4 changes necessary in database:
ALTER TABLE tst_questions CHANGE COLUMN Image ImageName VARCHAR(43) NOT NULL;
ALTER TABLE tst_questions ADD COLUMN ImageTitle TEXT NOT NULL AFTER ImageName;
ALTER TABLE tst_answers CHANGE COLUMN Image ImageName VARCHAR(43) NOT NULL;
ALTER TABLE tst_answers ADD COLUMN ImageTitle TEXT NOT NULL AFTER ImageName;

        Version 15.181:   Apr 05, 2016	New fields in test questions with the title/attribution of images. (198515 lines)
        Version 15.180.7: Apr 05, 2016	Changes in layout of editor of question. (198473 lines)
        Version 15.180.6: Apr 05, 2016	Changes in layout of editor of question. (198490 lines)
        Version 15.180.5: Apr 05, 2016	Changes in layout of editor of question. (198512 lines)
        Version 15.180.4: Apr 05, 2016	Fixed bug in images in test questions. (198512 lines)
        Version 15.180.3: Apr 05, 2016	Changed layout of confirm / reject registration. (198482 lines)
        Version 15.180.2: Apr 05, 2016	Contextual link to remove one survey question. (198459 lines)
        Version 15.180.1: Apr 05, 2016	Fized minor layout bug in edition of a test question. (198430 lines)
        Version 15.180:   Apr 05, 2016	Changes in form to edit a test question.
					Ask for confirmation when removing a survey question. (198429 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1524','es','N','Preguntar si eliminar pregunta encuesta');

        Version 15.179.1: Apr 05, 2016	Contextual link to remove one test question. (198384 lines)
        Version 15.179:   Apr 05, 2016	Fixed bug when getting a parameter.
					Ask for confirmation when removing a test question. (198355 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1523','es','N','Preguntar si eliminar pregunta test');

        Version 15.178.2: Apr 05, 2016	Changes in JavaScript related to images in test questions. (198265 lines)
        Version 15.178.1: Apr 04, 2016	Changes in CSS related to images in test questions. (198240 lines)
        Version 15.178:   Apr 04, 2016	Code refactoring related to images in test questions. (198244 lines)
					5 changes necessary in database:
ALTER TABLE tst_questions CHANGE COLUMN Image ImageOld CHAR(43) NOT NULL;
ALTER TABLE tst_questions ADD COLUMN Image VARCHAR(43) NOT NULL AFTER Feedback;
UPDATE tst_questions SET Image=ImageOld;
ALTER TABLE tst_questions DROP COLUMN ImageOld;
ALTER TABLE tst_answers ADD COLUMN Image VARCHAR(43) NOT NULL AFTER Feedback;

        Version 15.177:   Apr 04, 2016	Code refactoring related to images. (198083 lines)
        Version 15.176:   Apr 04, 2016	Code refactoring related to images. (198019 lines)
        Version 15.175.11:Apr 04, 2016	Code refactoring related to image associated to a test question.
                                        Forms to edit image in every unique/multiple answer. (197968 lines)
        Version 15.175.10:Apr 04, 2016	Changes in CSS. (197955 lines)
        Version 15.175.9: Apr 04, 2016	Fixed bug in edition of a test question. (197953 lines)
        Version 15.175.8: Apr 04, 2016	Code refactoring related to receiving and processing image file. (197945 lines)
        Version 15.175.7: Apr 04, 2016	Code refactoring related to image associated to a test question.
					Fixed bug while exporting test questions. (197923 lines)
        Version 15.175.6: Apr 03, 2016	Remove image associated to a test question. (197926 lines)
        Version 15.175.5: Apr 03, 2016	Changes in reception of form to edit one test question. (197930 lines)
        Version 15.175.4: Apr 03, 2016	Changes in form to edit one test question. (197897 lines)
        Version 15.175.3: Apr 03, 2016	Change in edition of test question. (197804 lines)
        Version 15.175.2: Apr 03, 2016	Code refactoring related to deleting image file. (197773 lines)
        Version 15.175.1: Apr 03, 2016	Code refactoring related to deleting tests of a course. (197763 lines)
        Version 15.175:   Apr 03, 2016	Fixed bug when editing a test question.
					Lot of code refactoring related to edition of questions.
					New module swad_image for edition of images. (197754 lines)
        Version 15.174:   Apr 01, 2016	Image in a test question is removed when the test question is removed.
					All images in test questions of a course are removed when course is removed. (197504 lines)
        Version 15.173.4: Apr 01, 2016	Old image in test question is removed when replaced by a new one. (197481 lines)
        Version 15.173.3: Apr 01, 2016	Changes in layout of class photo. (197447 lines)
        Version 15.173.2: Apr 01, 2016	Changes in CSS related to tests. (197446 lines)
        Version 15.173.1: Apr 01, 2016	Changes in CSS related to tests. (197452 lines)
        Version 15.173:   Apr 01, 2016	Show image in test question, if present. (197435 lines)
        Version 15.172.1: Apr 01, 2016	Reception of test image. (197398 lines)
        Version 15.172:   Apr 01, 2016	Reception of test image. Not finished. (197339 lines)
        Version 15.171.2: Apr 01, 2016	Change in message related to user photo. (197272 lines)
        Version 15.171.1: Apr 01, 2016	Code refactoring in functions to get a parameter and to receive file. (197271 lines)
        Version 15.171:   Apr 01, 2016	Code refactoring in function to get a parameter. (197259 lines)
        Version 15.170:   Apr 01, 2016	Code refactoring in function to receive file. (197310 lines)
        Version 15.169.6: Mar 31, 2016	Code refactoring in function to get a parameter. (197313 lines)
        Version 15.169.5: Mar 31, 2016	Code refactoring in function to get a parameter. (197292 lines)
        Version 15.169.4: Mar 31, 2016	Code refactoring in list of parameters. (197280 lines)
        Version 15.169.3: Mar 31, 2016	Code refactoring in list of parameters. (197271 lines)
        Version 15.169.2: Mar 31, 2016	Code refactoring in list of parameters. Not finished. (197298 lines)
        Version 15.169.1: Mar 31, 2016	Code refactoring in list of parameters. (197298 lines)
        Version 15.169:   Mar 31, 2016	When content is data, all parameters are retrieved in a list. (197306 lines)
        Version 15.168.3: Mar 30, 2016	Code refactoring in list of parameters. (197124 lines)
        Version 15.168.2: Mar 30, 2016	Code refactoring in list of parameters. (197128 lines)
        Version 15.168.1: Mar 30, 2016	Fixed bug in list of parameters. (197097 lines)
        Version 15.168:   Mar 30, 2016	When content is normal, all parameters are retrieved in a list. (197085 lines)
        Version 15.167:   Mar 30, 2016	Query string is allocated to the exact needed size to optimize memory. (196998 lines)
        Version 15.166.1: Mar 30, 2016	Fixed bug while reading a parameter. (196959 lines)
        Version 15.166:   Mar 30, 2016	Changes in form to edit a test question.
					Fixed bug while reading a parameter. (196943 lines)
        Version 15.165.8: Mar 30, 2016	Changes related to image in test questions. (196896 lines)
        Version 15.165.7: Mar 29, 2016	New field for image in database table of test questions. (196879 lines)
					1 change necessary in database:
ALTER TABLE tst_questions ADD COLUMN Image CHAR(43) NOT NULL AFTER Stem;

        Version 15.165.6: Mar 29, 2016	Changes in temporary directories for images. (196846 lines)
        Version 15.165.5: Mar 29, 2016	Changes in form to edit a test question. (196843 lines)
        Version 15.165.4: Mar 29, 2016	Changes in form to upload XML file to import questions. (196813 lines)
        Version 15.165.3: Mar 29, 2016	Changes in form to upload user's photo. (196811 lines)
        Version 15.165.2: Mar 29, 2016	Changes in forms to upload logos. (196805 lines)
        Version 15.165.1: Mar 29, 2016	Changes in form to upload photo of centre. (196803 lines)
        Version 15.165:   Mar 28, 2016	Use ImageMagick 'convert' after uploading an image of a centre. (196805 lines)
        Version 15.164:   Mar 28, 2016	Scope selector inside frame in listing of students.
					Fix bug in edition of groups. (196664 lines)
        Version 15.163.4: Mar 28, 2016	Scope selector inside frame in listing of teachers. (196656 lines)
        Version 15.163.3: Mar 28, 2016	Scope selector inside frame in listing of admins. (196652 lines)
        Version 15.163.2: Mar 27, 2016	Scope selector inside frame in listing of guests. (196655 lines)
        Version 15.163.1: Mar 27, 2016	Added HTML directives "label" for scope and other form fields. (196654 lines)
        Version 15.163:   Mar 27, 2016	Requests for enrolment older than one month are automatically removed. (196656 lines)
        Version 15.162.7: Mar 25, 2016	Changed message after enrolment request. (196625 lines)
        Version 15.162.6: Mar 24, 2016	Smaller buttons to see results of surveys.
					Fix bug in surveys. (196615 lines)
        Version 15.162.5: Mar 24, 2016	Default role in registration form will be teacher for the creator of a course. (196615 lines)
        Version 15.162.4: Mar 24, 2016	Changes in button shown after editing institutions. (196612 lines)
        Version 15.162.3: Mar 24, 2016	Changes in button shown after editing centres. (196609 lines)
        Version 15.162.2: Mar 24, 2016	Changes in button shown after editing degrees. (196606 lines)
        Version 15.162.1: Mar 24, 2016	Changes in buttons shown after editing courses. (196603 lines)
        Version 15.162:   Mar 24, 2016	Code refactoring in edition of courses.
					When a user creates or edits a course, put button to register him/her. (196593 lines)
        Version 15.161.4: Mar 24, 2016	In MESSAGES > Announcements, any user can see obsolete announcements, not only system admins. (196576 lines)
        Version 15.161.3: Mar 24, 2016	Changed icon to reset survey. (196577 lines)
Copy file recycle64x64.png to icon directory

        Version 15.161.2: Mar 24, 2016	Removed contextual icon "update" integrated in frame in "enrolment requests". (196577 lines)
        Version 15.161.1: Mar 24, 2016	Code refactoring in forms to select roles.
					Put contextual icon "update" integrated in frame in "enrolment requests". (196598 lines)
        Version 15.161:   Mar 24, 2016	Changes related to enrolments requests. (196581 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1522','es','N','Actualizar inscripciones pendientes');

        Version 15.160.4: Mar 24, 2016	Put contextual icon "update" integrated in frame in "Connected users". (196543 lines)
        Version 15.160.3: Mar 24, 2016	Put contextual icon "update" integrated in frame in "Who to follow". (196538 lines)
        Version 15.160.2: Mar 22, 2016	Changes in layout of tagline. (196512 lines)
        Version 15.160.1: Mar 21, 2016	Changes in layout of tagline. (196504 lines)
        Version 15.160:   Mar 21, 2016	Changes in forms to login and to create new account. (196423 lines)
        Version 15.159.5: Mar 21, 2016	Changes in CSS related to tables. (196423 lines)
        Version 15.159.4: Mar 21, 2016	Icon to print stats on degrees integrated in frame. (196426 lines)
        Version 15.159.3: Mar 21, 2016	Icon to add new announcement integrated in frame.
                                        Button to add new announcement. (196427 lines)
        Version 15.159.2: Mar 21, 2016	Button to add new notice. (196410 lines)
        Version 15.159.1: Mar 21, 2016	Icon to add new notice integrated in frame. (196395 lines)
        Version 15.159:   Mar 21, 2016	Number of sent/received messages integrated in frame title. (196372 lines)
        Version 15.158.4: Mar 21, 2016	Icon to remove sent/received messages integrated in frame. (196300 lines)
        Version 15.158.3: Mar 21, 2016	Change in layout of form to import test questions. (196277 lines)
        Version 15.158.2: Mar 21, 2016	Icon to add new test question integrated in frame. (196283 lines)
        Version 15.158.1: Mar 21, 2016	Button to add new test question. (196284 lines)
        Version 15.158:   Mar 21, 2016	Changes in layout of tests. (196274 lines)
        Version 15.157.3: Mar 20, 2016	Changes in layout of tests. (196247 lines)
        Version 15.157.2: Mar 20, 2016	Fixed bug in surveys. (196241 lines)
        Version 15.157.1: Mar 20, 2016	Changed names of functions related to degree types. (196237 lines)
        Version 15.157:   Mar 20, 2016	Changes in tagline and title. (196227 lines)
        Version 15.156.1: Mar 20, 2016	Change in layout of banners.
					Changes in layout of types of degree. (196139 lines)
        Version 15.156:   Mar 20, 2016	New module swad_degree_type for degree types. (196137 lines)
        Version 15.155.7: Mar 20, 2016	Changes in layout of listing of institutions. (196051 lines)
        Version 15.155.6: Mar 20, 2016	Changes in layout of listing of centres. (196055 lines)
        Version 15.155.5: Mar 20, 2016	Changes in behaviour of hierarchy breadcrumb.
					Changes in layout of listing of degrees. (196057 lines)
        Version 15.155.4: Mar 20, 2016	Changes in layout of listing of courses. (196059 lines)
        Version 15.155.3: Mar 20, 2016	Changes in some help messages. (196062 lines)
        Version 15.155.2: Mar 20, 2016	Fixed bug in surveys.
					Changes in edition of questions of surveys. (196060 lines)
        Version 15.155.1: Mar 20, 2016	Icon to print list of attendance integrated in frame. (196042 lines)
        Version 15.155:   Mar 20, 2016	Icon to add new attendance event integrated in frame.
					Changes in layout of attendance events. (196038 lines)
        Version 15.154:   Mar 19, 2016	Icon to add new survey integrated in frame.
					Changes in layout of surveys. (196018 lines)
        Version 15.153.1: Mar 19, 2016	Fixed bug in edition of assignments. (195980 lines)
        Version 15.153:   Mar 19, 2016	Icon to add new assignment integrated in frame.
					Changes in layout of assignments. (195984 lines)
        Version 15.152:   Mar 19, 2016	Changes in layout of head. Not finished. (195949 lines)
        Version 15.151.5: Mar 19, 2016	Icon to edit holidays integrated in frame. (195931 lines)
        Version 15.151.4: Mar 19, 2016	Icon to edit plugins integrated in frame. (195933 lines)
        Version 15.151.3: Mar 19, 2016	Icon to edit mail domains integrated in frame. (195934 lines)
        Version 15.151.2: Mar 19, 2016	Changes in layout of groups. (195935 lines)
        Version 15.151.1: Mar 18, 2016	Changes in layout of groups. (195929 lines)
        Version 15.151:   Mar 18, 2016	Changes in layout of header class photo. (195930 lines)
        Version 15.150.10:Mar 18, 2016	Icon to print calendar integrated in frame. (195961 lines)
        Version 15.150.9: Mar 18, 2016	Icons to print/show-all students/teachers integrated in frame. (195957 lines)
        Version 15.150.8: Mar 18, 2016	Icons to print/show-all guests integrated in frame. (195967 lines)
        Version 15.150.7: Mar 18, 2016	Icon to print degree configuration integrated in frame. (195970 lines)
        Version 15.150.6: Mar 18, 2016	Icon to print centre configuration integrated in frame. (195959 lines)
        Version 15.150.5: Mar 18, 2016	Icon to print institution configuration integrated in frame. (195961 lines)
        Version 15.150.4: Mar 18, 2016	Icon to print country configuration integrated in frame. (195952 lines)
        Version 15.150.3: Mar 18, 2016	Changes in layout of course info. (195956 lines)
        Version 15.150.2: Mar 18, 2016	Icon to print course configuration integrated in frame.
					Changes in layout of frames. (195950 lines)
        Version 15.150.1: Mar 18, 2016	Icon to view timetable integrated in frame. (195936 lines)
        Version 15.150:   Mar 18, 2016	Icons to edit/print timetable integrated in frame. (195923 lines)
        Version 15.149.1: Mar 18, 2016	Button Done after internal editor of syllabus.
					Focus on new item in internal editor of syllabus.
					Removed links to view course info. (195920 lines)
        Version 15.149:   Mar 17, 2016	Icon to edit course info integrated in frame.
					Code refacgtoring related to course info. (195949 lines)
        Version 15.148:   Mar 17, 2016	Icon to view/edit file browser integrated in frame. (195914 lines)
        Version 15.147.6: Mar 16, 2016	Icon to edit departments integrated in frame. (195862 lines)
        Version 15.147.5: Mar 16, 2016	Icon to edit institutional links integrated in frame. (195863 lines)
        Version 15.147.4: Mar 16, 2016	Icon to edit places integrated in frame. (195866 lines)
        Version 15.147.3: Mar 16, 2016	Icon to edit groups integrated in frame. (195867 lines)
        Version 15.147.2: Mar 16, 2016	Icon to edit degree types integrated in frame. (195866 lines)
        Version 15.147.1: Mar 16, 2016	Removed links to view when editing. (195854 lines)
        Version 15.147:   Mar 16, 2016	New buttons to create another institution / centre / degree / course. (195903 lines)
        Version 15.146.6: Mar 16, 2016	Icon to edit courses integrated in frame. (195814 lines)
        Version 15.146.5: Mar 16, 2016	Icon to edit degrees integrated in frame. (195806 lines)
        Version 15.146.4: Mar 16, 2016	Icon to edit centres integrated in frame. (195798 lines)
        Version 15.146.3: Mar 16, 2016	Icon to edit institutions integrated in frame. (195790 lines)
        Version 15.146.2: Mar 16, 2016	Icon to edit countries integrated in frame. (195781 lines)
        Version 15.146.1: Mar 16, 2016	Icons to remove/edit/print an exam announcement integrated in frame. (195771 lines)
        Version 15.146:   Mar 16, 2016	Now frame rectangles can have icons at top.
					Icon to edit banners integrated in frame. (195772 lines)
        Version 15.145.6: Mar 16, 2016	Fixed bug in connected users. (195757 lines)
        Version 15.145.5: Mar 15, 2016	Removed links in right column. (195752 lines)
        Version 15.145.4: Mar 15, 2016	Link to RSS file is moved to the end of notices in left column. (195751 lines)
Copy file swad112x32.png to icon directory

        Version 15.145.3: Mar 15, 2016	Changes in layout of connected users. (195746 lines)
        Version 15.145.2: Mar 15, 2016	Changes in layout of frequent actions. (195762 lines)
        Version 15.145.1: Mar 15, 2016	Changes in layout of frequent actions. (195751 lines)
        Version 15.145:   Mar 15, 2016	Changes in layout of connected users. (195729 lines)
        Version 15.144.3: Mar 15, 2016	Changes in layout of connected users (right column). (195742 lines)
        Version 15.144.2: Mar 05, 2016	Fixed minor bugs in edition of centres. (195761 lines)
        Version 15.144.1: Mar 03, 2016	Fixed bugs related to last chages made. (195756 lines)
        Version 15.144:   Mar 03, 2016	Direct login always available. (195755 lines)
					3 changes necessary in database:
ALTER TABLE courses DROP COLUMN AllowDirectLogIn;
ALTER TABLE deg_types DROP COLUMN AllowDirectLogIn;
UPDATE actions SET Obsolete='Y' WHERE ActCod='1006';

        Version 15.143.6: Mar 03, 2016	Direct login forms unavailable when external login service unavailable. (196219 lines)
        Version 15.143.5: Mar 02, 2016	Changed text messages related to edition of courses. (196166 lines)
        Version 15.143.4: Mar 02, 2016	Changed text messages related to edition of degrees. (196126 lines)
        Version 15.143.3: Mar 02, 2016	Changed text messages related to edition of centres. (196126 lines)
        Version 15.143.2: Mar 02, 2016	Changed text messages related to edition of institutions. (196086 lines)
        Version 15.143.1: Mar 02, 2016	Changes in actions of buttons after editing. (196042 lines)
        Version 15.143:   Mar 01, 2016	Button to go to another institution after editing. (196041 lines)
        Version 15.142.1: Mar 01, 2016	Changes in some help messages. (195985 lines)
        Version 15.142:   Mar 01, 2016	Button to go to another centre after editing. (195984 lines)
        Version 15.141.1: Mar 01, 2016	Fixed bug when creating a new degree. (195897 lines)
        Version 15.141:   Mar 01, 2016	Button to go to another degree after editing. (195898 lines)
        Version 15.140.1: Mar 01, 2016	The link to go to another course after editing is replaced by a button. (195817 lines)
        Version 15.140:   Mar 01, 2016	New option "landing page". (195817 lines)
					2 changes necessary in database:
UPDATE actions SET Txt='Formularios cambiar rol y cerrar sesi&oacute;n' WHERE ActCod='843' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1521','es','N','P&aacute;gina de inicio');

        Version 15.139.2: Feb 29, 2016	Changed message related to year/semester. (195731 lines)
        Version 15.139.1: Feb 29, 2016	Remove fields FirstYear and LastYear from database table degrees. (195732 lines)
					1 change necessary in database:
ALTER TABLE degrees DROP COLUMN FirstYear,DROP COLUMN LastYear;

        Version 15.139:   Feb 29, 2016	Remove edition of years in edition of degrees. Every degree now have years from 1 to 12 (and N.A.) (195734 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod IN ('550','551');

        Version 15.138:   Feb 29, 2016	Form to sign up is shown below login form. (196012 lines)
        Version 15.137.3: Feb 29, 2016	Change in button when file uploading is done. (195986 lines)
        Version 15.137.2: Feb 13, 2016	Fixed bug in search of my documents. (195985 lines)
        Version 15.137.1: Feb 13, 2016	Fixed bug at the end of ranking (number too low). (195968 lines)
        Version 15.137:   Feb 07, 2016	New figure about number of social notes. (195953 lines)
        Version 15.136.7: Feb 06, 2016	Fixed bug in social tab when not logged. (195715 lines)
        Version 15.136.6: Feb 06, 2016	Change in graphic with number of users per country. (195703 lines)
        Version 15.136.5: Feb 04, 2016	Change in layout of main title (country/institution/centre/degree/course). (195702 lines)
        Version 15.136.4: Feb 04, 2016	Change in layout of main title (country/institution/centre/degree/course). (195687 lines)
        Version 15.136.3: Feb 04, 2016	Change in layout of main title (country/institution/centre/degree/course). (195686 lines)
        Version 15.136.2: Feb 04, 2016	Change in layout of most frequent actions. (195689 lines)
        Version 15.136.1: Feb 02, 2016	Change in layout of tabs and menus. (195689 lines)
        Version 15.136:   Jan 30, 2016	Average number of followed/followers per follower/followed. (195674 lines)
        Version 15.135.1: Jan 30, 2016	Fixed bug in figure about number of following and followers. (195518 lines)
        Version 15.135:   Jan 30, 2016	New figure about number of following and followers. (195516 lines)
        Version 15.134.2: Jan 29, 2016	New order of figures. (195327 lines)
        Version 15.134.1: Jan 29, 2016	Changes in lists of following/followers. (195326 lines)
        Version 15.134:   Jan 29, 2016	Change in menu of social tab. (195328 lines)
        Version 15.133.8: Jan 29, 2016	Link to request a user's profile. (195328 lines)
        Version 15.133.7: Jan 28, 2016	Changes in layout of followed/followers. (195288 lines)
        Version 15.133.6: Jan 28, 2016	Changes in layout of "Who to follow". (195287 lines)
        Version 15.133.5: Jan 28, 2016	Unlogged users can not view "Who to follow". (195276 lines)
        Version 15.133.4: Jan 28, 2016	Fixed bug in notifications about timeline events. Reported by Juan Miguel Boyero Corral. (195272 lines)
        Version 15.133.3: Jan 28, 2016	Changes in query to get users to follow. (195256 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1520','es','N','A qui&eacute;n seguir');

        Version 15.133.2: Jan 28, 2016	Changes in query to get users to follow. (195241 lines)
        Version 15.133.1: Jan 27, 2016	Change in link to show users to follow. (195238 lines)
        Version 15.133:   Jan 27, 2016	List users to follow. (195236 lines)
        Version 15.132.1: Jan 27, 2016	Build query to get users to follow. (195158 lines)
        Version 15.132:   Jan 27, 2016	Put link to show users to follow. (195116 lines)
					2 changes necessary in database:
CREATE INDEX PhotoVisibility ON usr_data (PhotoVisibility);
CREATE INDEX ProfileVisibility ON usr_data (ProfileVisibility);

        Version 15.131.4: Jan 27, 2016	Fixed bug in ranking of followed. (195030 lines)
        Version 15.131.3: Jan 27, 2016	Animated link to view old timeline. (195015 lines)
        Version 15.131.2: Jan 26, 2016	Change in default color of links. (194998 lines)
        Version 15.131.1: Jan 26, 2016	Optimization in the function that insert links in text. (194993 lines)
        Version 15.131:   Jan 26, 2016	Link around @nickname is inserted as form. (195011 lines)
        Version 15.130.2: Jan 26, 2016	Optimization in the function that insert links in text. (194928 lines)
        Version 15.130.1: Jan 26, 2016	Fixed bug when mentioning a user who does not exist. (194914 lines)
        Version 15.130:   Jan 26, 2016	Do not notify new social notes. (194913 lines)
					22 changes necessary in database:
UPDATE notif SET NotifyEvent= 9 WHERE NotifyEvent=10;
UPDATE notif SET NotifyEvent=10 WHERE NotifyEvent=11;
UPDATE notif SET NotifyEvent=11 WHERE NotifyEvent=12;
UPDATE notif SET NotifyEvent=12 WHERE NotifyEvent=13;
UPDATE notif SET NotifyEvent=13 WHERE NotifyEvent=14;
UPDATE notif SET NotifyEvent=14 WHERE NotifyEvent=15;
UPDATE notif SET NotifyEvent=15 WHERE NotifyEvent=16;
UPDATE notif SET NotifyEvent=16 WHERE NotifyEvent=17;
UPDATE notif SET NotifyEvent=17 WHERE NotifyEvent=18;
UPDATE notif SET NotifyEvent=18 WHERE NotifyEvent=19;
UPDATE sta_notif SET NotifyEvent= 9 WHERE NotifyEvent=10;
UPDATE sta_notif SET NotifyEvent=10 WHERE NotifyEvent=11;
UPDATE sta_notif SET NotifyEvent=11 WHERE NotifyEvent=12;
UPDATE sta_notif SET NotifyEvent=12 WHERE NotifyEvent=13;
UPDATE sta_notif SET NotifyEvent=13 WHERE NotifyEvent=14;
UPDATE sta_notif SET NotifyEvent=14 WHERE NotifyEvent=15;
UPDATE sta_notif SET NotifyEvent=15 WHERE NotifyEvent=16;
UPDATE sta_notif SET NotifyEvent=16 WHERE NotifyEvent=17;
UPDATE sta_notif SET NotifyEvent=17 WHERE NotifyEvent=18;
UPDATE sta_notif SET NotifyEvent=18 WHERE NotifyEvent=19;
UPDATE usr_data SET NotifNtfEvents = ((NotifNtfEvents & ~0x7FE00) | ((NotifNtfEvents & 0xFFC00) >> 1));
UPDATE usr_data SET EmailNtfEvents = ((EmailNtfEvents & ~0xFFE00) | ((EmailNtfEvents & 0xFFC00) >> 1));

        Version 15.129.3: Jan 25, 2016	Changed text in some messages. (194964 lines)
        Version 15.129.2: Jan 25, 2016	Mark timeline notifications as seen when new social activity is shown. (194967 lines)
        Version 15.129.1: Jan 25, 2016	Do not show top message in social timeline for original publishings. (194979 lines)
        Version 15.129:   Jan 25, 2016	Do not notify new social sharings or comments. (194979 lines)
        Version 15.128.5: Jan 25, 2016	Fixed bug and code optimization when sharing a social note. (194958 lines)
        Version 15.128.4: Jan 25, 2016	Fixed bug when removing a social note. (194961 lines)
        Version 15.128.3: Jan 25, 2016	Optimization in top message of social note. (194969 lines)
        Version 15.128.2: Jan 25, 2016	Optimization in top message of social note. (194960 lines)
        Version 15.128.1: Jan 25, 2016	Optimization in top message of social note. (194965 lines)
        Version 15.128:   Jan 25, 2016	View highlighted social note when clicking in timeline notification. (194980 lines)
        Version 15.127.3: Jan 25, 2016	Changed ComCod to PubCod in social_comments and social_comments_fav. (194841 lines)
					2 changes necessary in database:
ALTER TABLE social_comments_fav CHANGE COLUMN ComCod PubCod BIGINT NOT NULL;
ALTER TABLE social_comments CHANGE COLUMN ComCod PubCod BIGINT NOT NULL;

        Version 15.127.2: Jan 25, 2016	Mark timeline notifications as removed when a social note/comment is unfaved. (194838 lines)
        Version 15.127.1: Jan 25, 2016	Mark timeline notifications as removed when a social note is unshared. (194831 lines)
        Version 15.127:   Jan 25, 2016	New function to count the number of @nicknames in a text and store it in social publishing. (194825 lines)
        Version 15.126.1: Jan 24, 2016	Optimization in code to insert links. (194736 lines)
        Version 15.126:   Jan 24, 2016	In any text where URL is replaced by anchor, now @nickname is also replaced to link to user's profile. (194727 lines)
					2 changes necessary in database:
UPDATE social_posts SET Content=REPLACE(Content,'&#64;','@');
UPDATE social_comments SET Content=REPLACE(Content,'&#64;','@');

        Version 15.125.3: Jan 24, 2016	Fixed bug when unsharing a social note. (194610 lines)
        Version 15.125.2: Jan 24, 2016	Mark notifications as removed after removing a comment to a social note. (194609 lines)
        Version 15.125.1: Jan 24, 2016	Mark notifications as removed after removing a social note. (194603 lines)
        Version 15.125:   Jan 24, 2016	Code refactoring in social timeline. (194563 lines)
        Version 15.124.3: Jan 24, 2016	Mark timeline notifications as seen after viewing social timeline. (194554 lines)
        Version 15.124.2: Jan 23, 2016	Changed numeration of notifications in database. (194546 lines)
					14 changes necessary in database:
UPDATE notif SET NotifyEvent=17 WHERE NotifyEvent=9;
UPDATE notif SET NotifyEvent=15 WHERE NotifyEvent=10;
UPDATE notif SET NotifyEvent=16 WHERE NotifyEvent=11;
UPDATE notif SET NotifyEvent=18 WHERE NotifyEvent=12;
UPDATE notif SET NotifyEvent=19 WHERE NotifyEvent=13;
UPDATE sta_notif SET NotifyEvent=17 WHERE NotifyEvent=9;
UPDATE sta_notif SET NotifyEvent=15 WHERE NotifyEvent=10;
UPDATE sta_notif SET NotifyEvent=16 WHERE NotifyEvent=11;
UPDATE sta_notif SET NotifyEvent=18 WHERE NotifyEvent=12;
UPDATE sta_notif SET NotifyEvent=19 WHERE NotifyEvent=13;
UPDATE usr_data SET NotifNtfEvents = ((NotifNtfEvents & ~0xF8000) | ((NotifNtfEvents & 0x0200) << 8) | ((NotifNtfEvents & 0x0C00) << 5) | ((NotifNtfEvents & 0x3000) << 6));
UPDATE usr_data SET EmailNtfEvents = ((EmailNtfEvents & ~0xF8000) | ((EmailNtfEvents & 0x0200) << 8) | ((EmailNtfEvents & 0x0C00) << 5) | ((EmailNtfEvents & 0x3000) << 6));
UPDATE usr_data SET NotifNtfEvents = (NotifNtfEvents |  0x3E00);
UPDATE usr_data SET EmailNtfEvents = (EmailNtfEvents & ~0x3E00);

        Version 15.124.1: Jan 23, 2016	New social notifications. Not finished. (194530 lines)
        Version 15.124:   Jan 23, 2016	New social notifications. Not finished. (194466 lines)
        Version 15.123.7: Jan 22, 2016	New social notifications. Not finished. (194347 lines)
        Version 15.123.6: Jan 22, 2016	New social notifications. Not finished. (194342 lines)
        Version 15.123.5: Jan 22, 2016	New social notifications. Not finished. (194328 lines)
        Version 15.123.4: Jan 22, 2016	New social notifications. Not finished. (194281 lines)
        Version 15.123.3: Jan 22, 2016	New social notifications. Not finished. (194280 lines)
        Version 15.123.2: Jan 22, 2016	New social notifications. Not finished. (194279 lines)
        Version 15.123.1: Jan 22, 2016	New social notifications. Not finished. (194277 lines)
        Version 15.123:   Jan 22, 2016	New social notifications. Not finished. (194277 lines)
        Version 15.122.1: Jan 20, 2016	Changes in table of actions. (194018 lines)
					8 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1512','es','N','Marcar post social como favorito (global)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1513','es','N','Desmarcar post social como favorito (global)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1514','es','N','Marcar post social como favorito (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1515','es','N','Desmarcar post social como favorito (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1516','es','N','Marcar comentario social como favorito (global)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1517','es','N','Desmarcar comentario social como favorito (global)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1518','es','N','Marcar comentario social como favorito (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1519','es','N','Desmarcar comentario social como favorito (usuario)');

        Version 15.122:   Jan 20, 2016	Notifications of new followers go to follower's profile.
					Changes in notifications. (194008 lines)
        Version 15.121.9: Jan 20, 2016	Code optimization in social comments. (193952 lines)
        Version 15.121.8: Jan 20, 2016	Fixed bugs removing favs to social comments. (193960 lines)
        Version 15.121.7: Jan 20, 2016	Changes in layout of social timeline. (193955 lines)
        Version 15.121.6: Jan 20, 2016	Remove favs when a social comment is removed.
					Remove favs when a user is removed. (193953 lines)
        Version 15.121.5: Jan 20, 2016	New layout of form to write a new social comment. (193922 lines)
        Version 15.121.4: Jan 20, 2016	Fixed bug related to favs in social comments. (193904 lines)
        Version 15.121.3: Jan 20, 2016	Favs in social comments. Not finished.
					Changes in layout of social timeline. (193901 lines)
        Version 15.121.2: Jan 19, 2016	Favs in social comments. Not finished. (193868 lines)
        Version 15.121.1: Jan 19, 2016	Favs in social comments. Not finished. (193868 lines)
        Version 15.121:   Jan 19, 2016	Favs in social comments. Not finished. (193782 lines)
					1 change necessary in database:
CREATE TABLE IF NOT EXISTS social_comments_fav (FavCod BIGINT AUTO_INCREMENT,ComCod BIGINT NOT NULL,UsrCod INT NOT NULL,TimeFav DATETIME NOT NULL,UNIQUE INDEX(FavCod),UNIQUE INDEX(ComCod,UsrCod),INDEX(UsrCod));

        Version 15.120.5: Jan 19, 2016	Remove favs when a social note is removed.
					Remove favs when a user is removed. (193544 lines)
        Version 15.120.4: Jan 19, 2016	Changes in database table social_notes_fav. (193527 lines)
					2 changes necessary in database:
DROP TABLE social_notes_fav;
CREATE TABLE IF NOT EXISTS social_notes_fav (FavCod BIGINT AUTO_INCREMENT,NotCod BIGINT NOT NULL,UsrCod INT NOT NULL,TimeFav DATETIME NOT NULL,UNIQUE INDEX(FavCod),UNIQUE INDEX(NotCod,UsrCod),INDEX(UsrCod));

        Version 15.120.3: Jan 19, 2016	Minor changes in layout of timeline. (193517 lines)
        Version 15.120.2: Jan 19, 2016	Code optimization on sharers or favers. (193491 lines)
        Version 15.120.1: Jan 19, 2016	Show number of users who marked a social note as favourite. (193533 lines)
        Version 15.120:   Jan 19, 2016	Mark/unmark social notes as favourites. (193475 lines)
        Version 15.119:   Jan 18, 2016	New table to mark social notes as favourites. (193036 lines)
					1 change necessary in database:
CREATE TABLE IF NOT EXISTS social_notes_fav (NotCod BIGINT NOT NULL,UsrCod INT NOT NULL,TimeFav DATETIME NOT NULL,UNIQUE INDEX(NotCod,UsrCod),INDEX(UsrCod),INDEX(TimeFav));

        Version 15.118.4: Jan 18, 2016	Date-time without seconds in user's public profile. (192984 lines)
        Version 15.118.3: Jan 18, 2016	Fixed bug in date-time of files in file-browsers. (192975 lines)
        Version 15.118.2: Jan 18, 2016	Fixed bug in social_timelines table. (192973 lines)
ALTER TABLE sessions CHANGE SessionId SessionId CHAR(43) COLLATE latin1_spanish_ci NOT NULL;
ALTER TABLE tst_status CHANGE SessionId SessionId CHAR(43) COLLATE latin1_spanish_ci NOT NULL;
ALTER TABLE hidden_params CHANGE SessionId SessionId CHAR(43) COLLATE latin1_spanish_ci NOT NULL;
ALTER TABLE imported_groups CHANGE SessionId SessionId CHAR(43) COLLATE latin1_spanish_ci NOT NULL;
ALTER TABLE imported_sessions CHANGE SessionId SessionId CHAR(43) COLLATE latin1_spanish_ci NOT NULL;

        Version 15.118.1: Jan 18, 2016	Fixed bugs in social timeline. (192966 lines)
        Version 15.118:   Jan 18, 2016	Change in privacy options (new value "unknown"). (192967 lines)
					3 changes necessary in database:
ALTER TABLE usr_data CHANGE COLUMN PhotoVisibility PhotoVisibility ENUM('unknown','user','course','system','world') NOT NULL DEFAULT 'unknown';
ALTER TABLE usr_data CHANGE COLUMN ProfileVisibility ProfileVisibility ENUM('unknown','user','course','system','world') NOT NULL DEFAULT 'unknown';
UPDATE usr_data SET ProfileVisibility='unknown' WHERE ProfileVisibility IN ('user','course');

        Version 15.117.1: Jan 18, 2016	Summary of changes in database made from version 15.74.7 to 15.117.1. (? lines)
Edit file swad_copy.sh changing swad.js to swad*.js
					29 changes necessary in database:
CREATE TABLE IF NOT EXISTS social_comments (ComCod BIGINT NOT NULL,Content LONGTEXT NOT NULL,UNIQUE INDEX(ComCod),FULLTEXT(Content)) ENGINE = MYISAM;
CREATE TABLE IF NOT EXISTS social_notes (NotCod BIGINT NOT NULL AUTO_INCREMENT,NoteType TINYINT NOT NULL,Cod INT NOT NULL DEFAULT -1,UsrCod INT NOT NULL,HieCod INT NOT NULL DEFAULT -1,Unavailable ENUM('N','Y') NOT NULL DEFAULT 'N',TimeNote DATETIME NOT NULL,UNIQUE INDEX(NotCod),UNIQUE INDEX(NoteType,Cod),INDEX(UsrCod),INDEX(TimeNote));
CREATE TABLE IF NOT EXISTS social_posts (PstCod INT NOT NULL AUTO_INCREMENT,Content LONGTEXT NOT NULL,UNIQUE INDEX(PstCod),FULLTEXT(Content)) ENGINE = MYISAM;
CREATE TABLE IF NOT EXISTS social_pubs (PubCod BIGINT NOT NULL AUTO_INCREMENT,NotCod BIGINT NOT NULL,PublisherCod INT NOT NULL,PubType TINYINT NOT NULL,TimePublish DATETIME NOT NULL,UNIQUE INDEX(PubCod),INDEX(NotCod,PublisherCod,PubType),INDEX(PublisherCod),INDEX(PubType),INDEX(TimePublish));
CREATE TABLE IF NOT EXISTS social_timelines (SessionId CHAR(43) NOT NULL,NotCod BIGINT NOT NULL,UNIQUE INDEX(SessionId,NotCod));
ALTER TABLE sessions ADD COLUMN LastPubCod BIGINT NOT NULL DEFAULT 0 AFTER LastRefresh;
ALTER TABLE sessions ADD COLUMN FirstPubCod BIGINT NOT NULL DEFAULT 0 AFTER LastRefresh;
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1490','es','N','Ver actividad social (global)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1491','es','Y','Redactar public. social (global)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1492','es','N','Crear public. social (global)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1493','es','N','Eliminar public. social (global)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1494','es','N','Solicitar elim. public. social (global)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1495','es','N','Compartir public. social (global)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1496','es','N','Dejar de compartir public. social (global)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1497','es','Y','Redactar public. social (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1498','es','N','Crear public. social (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1499','es','N','Compartir public. social (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1500','es','N','Dejar de compartir public. social (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1501','es','N','Solicitar elim. public. social (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1502','es','N','Eliminar public. social (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1503','es','N','Crear comentario social (global)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1504','es','N','Crear comentario social (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1505','es','N','Solicitar elim. coment. social (global)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1506','es','N','Solicitar elim. coment. social (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1507','es','N','Eliminar coment. social (global)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1508','es','N','Eliminar coment. social (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1509','es','N','Refrescar timeline global (nuevos mensajes)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1510','es','N','Cargar mensajes anteriores en timeline global');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1511','es','N','Cargar mensajes anteriores en timeline usuario');

        Version 15.117:   Jan 17, 2016	Change in styles of messages, forums, surveys and tests. (192872 lines)
        Version 15.116.1: Jan 17, 2016	Help when writing social posts or comments. (192877 lines)
        Version 15.116:   Jan 17, 2016	Optimization in the query to get timeline. (192878 lines)
        Version 15.115.2: Jan 17, 2016	Code refactoring in timeline. (192860 lines)
        Version 15.115.1: Jan 17, 2016	Code refactoring in timeline. (192857 lines)
        Version 15.115:   Jan 17, 2016	Code refactoring in actions. (192823 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1509','es','N','Refrescar timeline global (nuevos mensajes)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1510','es','N','Cargar mensajes anteriores en timeline global');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1511','es','N','Cargar mensajes anteriores en timeline usuario');

        Version 15.114.5: Jan 17, 2016	Distinct default visibility for photo and profile. (192827 lines)
        Version 15.114.4: Jan 17, 2016	Change related to button to remove a social post. (192821 lines)
        Version 15.114.3: Jan 17, 2016	Fixed bug when getting timeline. (192820 lines)
        Version 15.114.2: Jan 17, 2016	Fixed bug when getting timeline. (192811 lines)
        Version 15.114.1: Jan 17, 2016	Optimized queries to get timeline. (192790 lines)
        Version 15.114:   Jan 17, 2016	Optimized queries to get timeline. (192817 lines)
        Version 15.113.17:Jan 16, 2016	Fixed bug in user's timeline. (192763 lines)
        Version 15.113.16:Jan 16, 2016	Removed unused code and messages. (192743 lines)
        Version 15.113.15:Jan 16, 2016	Changes in feedback on request to remove a social note. (192871 lines)
        Version 15.113.14:Jan 16, 2016	Changes in feedback when a social comment is published. (192850 lines)
        Version 15.113.13:Jan 16, 2016	Changes in feedback when a social note is unshared. (192846 lines)
        Version 15.113.12:Jan 15, 2016	When a new social post is commented, shared or unshared, fade its background. (192807 lines)
        Version 15.113.11:Jan 15, 2016	When a new social post is received, fade its background. (192803 lines)
        Version 15.113.10:Jan 15, 2016	When a social note is not available, the button to comment is disabled.
					Fixed bug receiving a comment to a social note. (192781 lines)
        Version 15.113.9: Jan 15, 2016	"..." changed by HTML entity "&hellip;". (192775 lines)
        Version 15.113.8: Jan 15, 2016	Changes in textareas to enter new social posts or comments. (192751 lines)
        Version 15.113.7: Jan 15, 2016	New social publishings fade in when user click to view them.
					New style for textarea where enter a new social post. (192715 lines)
        Version 15.113.6: Jan 14, 2016	Sinchronize timeline in other actions. (192684 lines)
        Version 15.113.5: Jan 14, 2016	Change in message.
					Removed unused code in JavaScript. (192684 lines)
        Version 15.113.4: Jan 14, 2016	Optimize JavaScript not concatenating big strings in new timeline. (192687 lines)
        Version 15.113.3: Jan 14, 2016	Divs to get new publishings only in global timeline. (192673 lines)
        Version 15.113.2: Jan 14, 2016	Removed unused code. (192668 lines)
        Version 15.113.1: Jan 14, 2016	Remove unused function. (192675 lines)
        Version 15.113:   Jan 14, 2016	Code refactoring related to forms with unique identifiers.
					Use unique identifiers in forms of lists of connected users. (192680 lines)
        Version 15.112:   Jan 14, 2016	Fixed bugs in user's timeline.
					All forms in social timeline must have unique identifiers. (192676 lines)
        Version 15.111:   Jan 14, 2016	Fixed bug in user's timeline.
					Code refactoring related with social timeline. (192622 lines)
        Version 15.110.18:Jan 13, 2016	Optimization in query to get initial recent timeline. (192564 lines)
        Version 15.110.17:Jan 13, 2016	Reviewed code related to Soc_MAX_RECENT_PUBS_TO_GET. (192568 lines)
        Version 15.110.16:Jan 13, 2016	Form to go to public profile of author of comment in timeline. (192563 lines)
        Version 15.110.15:Jan 13, 2016	Form to go to public profile of author of note in timeline.
					Extensions '.exe' and '.com' not allowed (Google reported malware). (192543 lines)
        Version 15.110.14:Jan 13, 2016	Form to go to public profile of sharer/commenter in timeline. (192525 lines)
        Version 15.110.13:Jan 13, 2016	Time in social timeline without seconds. (192495 lines)
        Version 15.110.12:Jan 13, 2016	Display name of sharer/commenter in timeline. (192456 lines)
        Version 15.110.11:Jan 13, 2016	Fixed bug in social timeline. (192403 lines)
        Version 15.110.10:Jan 13, 2016	New temporary tables used to optimize querying timeline. (192398 lines)
        Version 15.110.9: Jan 12, 2016	Do not get social notes already present in timeline. (192388 lines)
        Version 15.110.8: Jan 12, 2016	Insert social notes in current social timeline in database.
					Show old social notes only if they are not already shown in timeline. (192354 lines)
        Version 15.110.7: Jan 12, 2016	Remove unused old social timelines in database. (192320 lines)
        Version 15.110.6: Jan 12, 2016	Social timeline for this session in database is cleared when timeline is reloaded. (192303 lines)
        Version 15.110.5: Jan 12, 2016	New database table used to store social notes already present in every timeline. (192288 lines)
					1 change necessary in database:
CREATE TABLE IF NOT EXISTS social_timelines (SessionId CHAR(43) NOT NULL,NotCod BIGINT NOT NULL,UNIQUE INDEX(SessionId,NotCod));

        Version 15.110.4: Jan 12, 2016	Database table with social comments renamed. (192263 lines)
					2 changes necessary in database:
DROP TABLE social_comments;
RENAME TABLE social_comments_content TO social_comments;

        Version 15.110.3: Jan 12, 2016	Database table with social publishings renamed. (192259 lines)
					1 change necessary in database:
RENAME TABLE social_timeline TO social_pubs;

        Version 15.110.2: Jan 12, 2016	Changes in social timeline. (192256 lines)
        Version 15.110.1: Jan 12, 2016	Removed database table for social comments and unused code. (192237 lines)
					1 change necessary in database:
DROP TABLE social_comments;

        Version 15.110:   Jan 12, 2016	Comments are included in database table for timeline. (192352 lines)
					2 changes necessary in database:
DROP INDEX NotCod ON social_timeline;
CREATE INDEX NotCod ON social_timeline (NotCod,PublisherCod,PubType);

        Version 15.109.1: Jan 11, 2016	New field with the type of publishing in the database table for timeline. (192264 lines)
					5 changes necessary in database:
ALTER TABLE social_timeline ADD COLUMN PubType TINYINT NOT NULL AFTER PublisherCod,ADD INDEX (PubType);
CREATE TABLE IF NOT EXISTS social_notes_new (NotCod BIGINT NOT NULL AUTO_INCREMENT,NoteType TINYINT NOT NULL,Cod INT NOT NULL DEFAULT -1,UsrCod INT NOT NULL,HieCod INT NOT NULL DEFAULT -1,Unavailable ENUM('N','Y') NOT NULL DEFAULT 'N',TimeNote DATETIME NOT NULL,UNIQUE INDEX(NotCod),UNIQUE INDEX(NoteType,Cod),INDEX(UsrCod),INDEX(TimeNote));
INSERT INTO social_notes_new (NotCod,NoteType,Cod,UsrCod,HieCod,Unavailable,TimeNote) SELECT NotCod,NoteType,Cod,UsrCod,HieCod,Unavailable,TimeNote FROM social_notes;
DROP TABLE social_notes;
RENAME TABLE social_notes_new TO social_notes;

        Version 15.109:   Jan 11, 2016	Fixed bug when comenting an unshared social note.
					Refatored lot of code in timeline. (192221 lines)
        Version 15.108:   Jan 11, 2016	Field with the author of a social note is removed from table with timeline, because it can be obtained from table with social notes. (192289 lines)
					1 change necessary in database:
ALTER TABLE social_timeline DROP COLUMN AuthorCod;

        Version 15.107.7: Jan 11, 2016	Show error message when trying to share a removed social note. (192284 lines)
        Version 15.107.6: Jan 11, 2016	Show error message when trying to send a comment in a removed social note. (192276 lines)
        Version 15.107.5: Jan 11, 2016	Check if a user can comment a social note. (192225 lines)
        Version 15.107.4: Jan 11, 2016	Show separated social note at top when a new comment is added.
					Message of success when publishing a social note or comment. (192199 lines)
        Version 15.107.3: Jan 11, 2016	Fixed bug and code refactoring in social comments. (192160 lines)
        Version 15.107.2: Jan 11, 2016	Hide link to see old publishings when oldest publishing is reached.
					Some constants related to social timeline changed. (192159 lines)
        Version 15.107.1: Jan 10, 2016	Id of dates in timeline must be unique.
					Changes in JavaScript to render dates and mathenatics. (192137 lines)
        Version 15.107:   Jan 10, 2016	Get older publishings from social timeline. (192112 lines)
					1 change necessary in database:
ALTER TABLE sessions ADD COLUMN FirstPubCod BIGINT NOT NULL DEFAULT 0 AFTER LastRefresh;

        Version 15.106:   Jan 10, 2016	Link at the bottom of social timeline to get older publishings. (192028 lines)
        Version 15.105.1: Jan 10, 2016	Change in style writing recent social publishings. (191909 lines)
        Version 15.105:   Jan 10, 2016	Get only publishings newest than a give publishing. (191912 lines)
					1 change necessary in database:
ALTER TABLE sessions ADD COLUMN LastPubCod BIGINT NOT NULL DEFAULT 0 AFTER LastRefresh;

        Version 15.104.4: Jan 09, 2016	Last social publishing code is sent and got via AJAX. (191831 lines)
        Version 15.104.3: Jan 09, 2016	Changes in layout of social timeline. (191784 lines)
        Version 15.104.2: Jan 09, 2016	Refresh recent social timeline via AJAX. (191769 lines)
        Version 15.104.1: Jan 09, 2016	Refresh recent social timeline via AJAX. (191727 lines)
        Version 15.104:   Jan 09, 2016	Refresh recent social timeline via AJAX. Not finished. (191720 lines)
        Version 15.103.2: Jan 09, 2016	Check that social post or comments are not empty. (191652 lines)
        Version 15.103.1: Jan 08, 2016	Changes in timeline. (191646 lines)
        Version 15.103:   Jan 08, 2016	Changes in the form to enter a new publishing. (191625 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod IN ('1491','1497');

        Version 15.102.4: Jan 08, 2016	Text in icon to comment a social note. (191644 lines)
        Version 15.102.3: Jan 08, 2016	The social notes of the user are shown in her/his global timeline even if he/she does not follow any user. (191603 lines)
        Version 15.102.2: Jan 08, 2016	When a user is eliminated, all her/his social publshing are removed. (191606 lines)
        Version 15.102.1: Jan 08, 2016	Changes in layout of social timeline. (191557 lines)
        Version 15.102:   Jan 08, 2016	Remove a comment in social timeline. (191534 lines)
					4 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1505','es','N','Solicitar elim. coment. social (global)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1506','es','N','Solicitar elim. coment. social (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1507','es','N','Eliminar coment. social (global)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1508','es','N','Eliminar coment. social (usuario)');

        Version 15.101.3: Jan 07, 2016	Changes in CSS related to social timeline. (191229 lines)
        Version 15.101.2: Jan 07, 2016	Show social comments in social notes. (191229 lines)
        Version 15.101.1: Jan 07, 2016	Show social comments in social notes. Not finished. (191181 lines)
        Version 15.101:   Jan 07, 2016	Store social comment in database. (191091 lines)
					16 changes necessary in database:
CREATE TABLE IF NOT EXISTS social_comments (ComCod BIGINT NOT NULL AUTO_INCREMENT,UsrCod INT NOT NULL,NotCod INT NOT NULL,TimeComment DATETIME NOT NULL,UNIQUE INDEX(ComCod),INDEX(UsrCod),INDEX(NotCod,TimeComment),INDEX(TimeComment));
CREATE TABLE IF NOT EXISTS social_comments_content (ComCod BIGINT NOT NULL,Content LONGTEXT NOT NULL,UNIQUE INDEX(ComCod),FULLTEXT(Content)) ENGINE = MYISAM;
UPDATE actions SET Txt='Redactar public. social (global)'            WHERE ActCod='1491' AND Language='es';
UPDATE actions SET Txt='Crear public. social (global)'               WHERE ActCod='1492' AND Language='es';
UPDATE actions SET Txt='Eliminar public. social (global)'            WHERE ActCod='1493' AND Language='es';
UPDATE actions SET Txt='Solicitar elim. public. social (global)'     WHERE ActCod='1494' AND Language='es';
UPDATE actions SET Txt='Compartir public. social (global)'           WHERE ActCod='1495' AND Language='es';
UPDATE actions SET Txt='Dejar de compartir public. social (global)'  WHERE ActCod='1496' AND Language='es';
UPDATE actions SET Txt='Redactar public. social (usuario)'           WHERE ActCod='1497' AND Language='es';
UPDATE actions SET Txt='Crear public. social (usuario)'              WHERE ActCod='1498' AND Language='es';
UPDATE actions SET Txt='Compartir public. social (usuario)'          WHERE ActCod='1499' AND Language='es';
UPDATE actions SET Txt='Dejar de compartir public. social (usuario)' WHERE ActCod='1500' AND Language='es';
UPDATE actions SET Txt='Solicitar elim. public. social (usuario)'    WHERE ActCod='1501' AND Language='es';
UPDATE actions SET Txt='Eliminar public. social (usuario)'           WHERE ActCod='1502' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1503','es','N','Crear comentario social (global)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1504','es','N','Crear comentario social (usuario)');

        Version 15.100.4: Jan 07, 2016	Fixed bug when sharing a social note.
					Receive social comment. (191012 lines)
        Version 15.100.3: Jan 07, 2016	Fixed bug when sharing and unsharing a social note. (190999 lines)
        Version 15.100.2: Jan 07, 2016	Hidden form to reply a social note. (190983 lines)
        Version 15.100.1: Jan 07, 2016	New icon in social note to reply. (190937 lines)
        Version 15.100:   Jan 06, 2016	When an author removes a social note, all the publishings of this note are removed. (190842 lines)
        Version 15.99.1:  Jan 05, 2016	Change in message in social timeline. (190917 lines)
        Version 15.99:    Jan 05, 2016	Changes in social timeline. (190892 lines)
        Version 15.98:    Jan 05, 2016	If a social note is unavailable and is shared, put button to unshare.
					Several bugs in social timeline fixed. (190789 lines)
        Version 15.97.5:  Jan 04, 2016	Button to remove a social note is not shown if author already removed the note (but it yet exists because another user shared it). (190763 lines)
        Version 15.97.4:  Jan 04, 2016	Guests can see and write in global forums. (190758 lines)
        Version 15.97.3:  Jan 04, 2016	Link to user's privacy in my public profile. (190755 lines)
        Version 15.97.2:  Jan 04, 2016	New option in user's privacy. (190743 lines)
        Version 15.97.1:  Jan 04, 2016	User's timeline below user's profile visible only by logged users. (190719 lines)
        Version 15.97:    Jan 04, 2016	Show list of some users who has shared a social note. (190718 lines)
					3 changes necessary in database:
CREATE TABLE IF NOT EXISTS social_timeline_new (PubCod BIGINT NOT NULL AUTO_INCREMENT, NotCod BIGINT NOT NULL, PublisherCod INT NOT NULL, AuthorCod INT NOT NULL, TimePublish DATETIME NOT NULL, UNIQUE INDEX(PubCod), UNIQUE INDEX(NotCod,PublisherCod), INDEX(PublisherCod), INDEX(AuthorCod), INDEX(TimePublish)) SELECT PubCod,NotCod,PublisherCod,AuthorCod,TimePublish FROM social_timeline ORDER BY PubCod;
DROP TABLE social_timeline;
RENAME TABLE social_timeline_new TO social_timeline;

        Version 15.96.1:  Jan 04, 2016	Changes in title of user's timeline. (190641 lines)
        Version 15.96:    Jan 04, 2016	Display number of times a social note is shared. (190616 lines)
        Version 15.95.2:  Jan 04, 2016	Unavailable social notes can not be shared. (190556 lines)
        Version 15.95.1:  Jan 04, 2016	Optimization in writing social note. (190555 lines)
        Version 15.95:    Jan 04, 2016	Fixed bug in social timeline: social notes can be shared, not social publishings. (190583 lines)
        Version 15.94.1:  Jan 04, 2016	Changes when writing an unavailable social note. (190557 lines)
        Version 15.94:    Jan 04, 2016	Social notes are marked as unavailable when a folder and its children are removed. (190551 lines)
        Version 15.93.8:  Jan 04, 2016	Social note is marked as unavailable when a public file is removed. (190486 lines)
        Version 15.93.7:  Jan 03, 2016	Social note is marked as unavailable when a public forum post is removed. (190419 lines)
        Version 15.93.6:  Jan 03, 2016	Social note is marked as unavailable when a exam announcement is removed. (190409 lines)
        Version 15.93.5:  Jan 03, 2016	Social note is marked as unavailable when a notice is removed. (190406 lines)
					6 changes necessary in database:
CREATE UNIQUE INDEX NotCod ON social_notes (NotCod);
DROP INDEX SocCod ON social_notes;
DROP INDEX TimeEvent ON social_notes;
CREATE INDEX TimeNote ON social_notes (TimeNote);
DROP INDEX SocialEvent ON social_notes;
CREATE UNIQUE INDEX NoteTypeCod ON social_notes (NoteType,Cod);

        Version 15.93.4:  Jan 03, 2016	Changes in links to actions in social timeline.
					New field Unavailable in table of social notes. (190354 lines)
					1 change necessary in database:
ALTER TABLE social_notes ADD COLUMN Unavailable ENUM('N','Y') NOT NULL DEFAULT 'N' AFTER Cod;

        Version 15.93.3:  Jan 03, 2016	Code refactoring in file browser related with file metadata. (190345 lines)
        Version 15.93.2:  Jan 03, 2016	To show a file in social timeline, file code is passed as parameter instead of path. (190360 lines)
        Version 15.93.1:  Jan 03, 2016	Show text indicating document not available in social note. (190358 lines)
        Version 15.93:    Jan 03, 2016	Fixing bugs in file metadata of files in institution, centre and degree zones. (190335 lines)
        Version 15.92:    Jan 02, 2016	Fixing bugs in file metadata of files in institution, centre and degree zones. Not finished. (190365 lines)
					1 change necessary in database:
ALTER TABLE social_notes DROP COLUMN DegCod;

        Version 15.91.4:  Jan 02, 2016	Fixed minor bugs in social timeline. (190184 lines)
        Version 15.91.3:  Jan 02, 2016	Just before to remove a social note, display at top the note to remove. (190183 lines)
        Version 15.91.2:  Jan 02, 2016	After unsharing a social note, display at top the note just unshared. (190175 lines)
        Version 15.91.1:  Jan 02, 2016	After sharing a social note, display at top the note just shared. (190168 lines)
        Version 15.91:    Jan 02, 2016	Every social note can be shared.
                                        Fixed bug when unsharing a social note. (190161 lines)
        Version 15.90:    Jan 02, 2016	Change in fields of database table that stores social notes. (190122 lines)
					6 changes necessary in database:
ALTER TABLE social_notes ADD COLUMN HieCod INT NOT NULL DEFAULT -1 AFTER UsrCod;
UPDATE social_notes SET HieCod=InsCod WHERE NoteType IN ('1','2');
UPDATE social_notes SET HieCod=CtrCod WHERE NoteType IN ('3','4');
UPDATE social_notes SET HieCod=DegCod WHERE NoteType IN ('5','6');
UPDATE social_notes SET HieCod=CrsCod WHERE NoteType IN ('7','8','9','12');
ALTER TABLE social_notes DROP COLUMN CtyCod,DROP COLUMN InsCod,DROP COLUMN CtrCod,DROP COLUMN CrsCod;

        Version 15.89.2:  Jan 02, 2016	Change in layout of user profile. (190141 lines)
        Version 15.89.1:  Jan 02, 2016	Change in layout of user profile. (190137 lines)
        Version 15.89:    Jan 02, 2016	Social timeline can be updated from user profile. (190084 lines)
					13 changes necessary in database:
UPDATE actions SET Txt='Ver actividad social (global)' WHERE ActCod='1490' AND Language='es';
UPDATE actions SET Txt='Redactar comentario social (global)' WHERE ActCod='1491' AND Language='es';
UPDATE actions SET Txt='Crear comentario social (global)' WHERE ActCod='1492' AND Language='es';
UPDATE actions SET Txt='Eliminar comentario social (global)' WHERE ActCod='1493' AND Language='es';
UPDATE actions SET Txt='Solicitar elim. coment. social (global)' WHERE ActCod='1494' AND Language='es';
UPDATE actions SET Txt='Compartir comentario social (global)' WHERE ActCod='1495' AND Language='es';
UPDATE actions SET Txt='Dejar de compartir coment. social (global)' WHERE ActCod='1496' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1497','es','N','Redactar comentario social (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1498','es','N','Crear comentario social (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1502','es','N','Eliminar comentario social (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1501','es','N','Solicitar elim. coment. social (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1499','es','N','Compartir comentario social (usuario)');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1500','es','N','Dejar de compartir coment. social (usuario)');

        Version 15.88.3:  Jan 01, 2016	Changes in contextual options in social timeline. (189907 lines)
        Version 15.88.2:  Jan 01, 2016	Messages translated. (189902 lines)
        Version 15.88.1:  Jan 01, 2016	Changes in layout of social timeline. (189854 lines)
        Version 15.88:    Jan 01, 2016	New buttons in social timeline to share/unshare a social publishing. (189841 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1495','es','N','Compartir comentario social');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1496','es','N','Dejar de compartir comentario social');

        Version 15.87:    Dec 31, 2015	New database table to store users who write or share social notes. (189594 lines)
					3 changes necessary in database:
CREATE TABLE IF NOT EXISTS social_timeline (PubCod BIGINT NOT NULL AUTO_INCREMENT,AuthorCod INT NOT NULL,PublisherCod INT NOT NULL,NotCod BIGINT NOT NULL,TimePublish DATETIME NOT NULL,UNIQUE INDEX(PubCod),INDEX(AuthorCod),INDEX(PublisherCod),INDEX(NotCod),INDEX(TimePublish));
INSERT INTO social_timeline (AuthorCod,PublisherCod,NotCod,TimePublish) SELECT UsrCod,UsrCod,NotCod,TimeNote FROM social_notes ORDER BY NotCod;
ALTER TABLE social_notes CHANGE COLUMN SocialNote NoteType TINYINT NOT NULL;

        Version 15.86.1:  Dec 31, 2015	Change in database table with social notes. (189457 lines)
					1 change necessary in database:
ALTER TABLE social_notes CHANGE COLUMN SocCod NotCod BIGINT NOT NULL AUTO_INCREMENT;

        Version 15.86:    Dec 30, 2015	Renamed database tables related to social events. (189451 lines)
					7 changes necessary in database:
RENAME TABLE social_post TO social_posts;
RENAME TABLE social TO social_notes;
ALTER TABLE social_notes CHANGE COLUMN SocialEvent SocialNote TINYINT NOT NULL;
ALTER TABLE social_notes CHANGE COLUMN TimeEvent TimeNote DATETIME NOT NULL;
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1492','es','N','Crear comentario social');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1493','es','N','Eliminar comentario social');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1494','es','N','Solicitar elim. comentario social');

        Version 15.85.1:  Dec 30, 2015	Remove social events when a user is removed. (189439 lines)
        Version 15.85:    Dec 30, 2015	Forums option is moved to social tab. (189439 lines)
        Version 15.84.5:  Dec 30, 2015	Form to update connected users. (189430 lines)
        Version 15.84.4:  Dec 30, 2015	Code refactoring in profile. (189427 lines)
        Version 15.84.3:  Dec 30, 2015	Form to update timeline. (189444 lines)
        Version 15.84.2:  Dec 30, 2015	Help about editor in new social post. (189422 lines)
        Version 15.84.1:  Dec 30, 2015	Request confirmation before removing a social post. (189419 lines)
        Version 15.84:    Dec 30, 2015	Remove social post. (189316 lines)
        Version 15.83.1:  Dec 30, 2015	Fixed bug in user's public profile. (189210 lines)
        Version 15.83:    Dec 30, 2015	Form to remove a social post. (189209 lines)
        Version 15.82.4:  Dec 30, 2015	Action to write a new social post is removed form menu. (189159 lines)
        Version 15.82.3:  Dec 30, 2015	Contextual link to write a new social post. (189199 lines)
        Version 15.82.2:  Dec 30, 2015	Current timeline is displayed below the form to write a new social post. (189183 lines)
        Version 15.82.1:  Dec 29, 2015	Public social posts are displayed in timeline. (189176 lines)
        Version 15.82:    Dec 29, 2015	Public social post is received and stored in a new database table. (189136 lines)
					1 change necessary in database:
CREATE TABLE IF NOT EXISTS social_post (PstCod INT NOT NULL AUTO_INCREMENT,Content LONGTEXT NOT NULL,UNIQUE INDEX(PstCod),FULLTEXT(Content)) ENGINE = MYISAM;

        Version 15.81.1:  Dec 29, 2015	Form to write a public social post. Not finished. (189094 lines)
        Version 15.81:    Dec 29, 2015	New option to write a public social post. Not finished. (189030 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1491','es','N','Redactar comentario social');

        Version 15.80.2:  Dec 29, 2015	Change in layout of social events (timeline). (188972 lines)
        Version 15.80.1:  Dec 29, 2015	Message translated. (188967 lines)
        Version 15.80:    Dec 29, 2015	Changes in layout of user's profile. (188944 lines)
        Version 15.79.2:  Dec 29, 2015	Include my public activity in timeline. (188838 lines)
        Version 15.79.1:  Dec 29, 2015	Changes in layout of user's profile. (188836 lines)
        Version 15.79:    Dec 29, 2015	Show timeline of a selected user.
                                        Changes in CSS related to social activity. (188834 lines)
        Version 15.78.1:  Dec 29, 2015	Go directly to notice in notifications and social events. (188785 lines)
        Version 15.78:    Dec 29, 2015	Links in social events.
					Actions related to shared files renamed. (188776 lines)
        Version 15.77.8:  Dec 29, 2015	Author's name length is limited in social events. (188662 lines)
        Version 15.77.7:  Dec 29, 2015	When a date-time is written in local time, and the date is today, 'Today' is displayed instead of date. (188660 lines)
					1 change necessary:
Edit file swad_copy.sh changing swad.js to swad*.js

        Version 15.77.6:  Dec 29, 2015	Show type of event on social events (timeline). (188647 lines)
        Version 15.77.5:  Dec 29, 2015	Insert social event when publishing an exam announcement.
                                        Changes in CSS related to social activity. (188392 lines)
        Version 15.77.4:  Dec 29, 2015	Insert social event when publishing a new notice.
                                        Changes in CSS related to social activity. (188410 lines)
        Version 15.77.3:  Dec 28, 2015	Insert social event when changing file from private to public. (188402 lines)
        Version 15.77.2:  Dec 28, 2015	Changes in CSS related to social activity. (188368 lines)
        Version 15.77.1:  Dec 28, 2015	Insert social event when posting in a global forum. (188342 lines)
        Version 15.77:    Dec 28, 2015	New table in database for social activity. (188287 lines)
					2 changes necessary in database:
CREATE TABLE IF NOT EXISTS social (SocCod BIGINT NOT NULL AUTO_INCREMENT,SocialEvent TINYINT NOT NULL,UsrCod INT NOT NULL,CtyCod INT NOT NULL DEFAULT -1,InsCod INT NOT NULL DEFAULT -1,CtrCod INT NOT NULL DEFAULT -1,DegCod INT NOT NULL DEFAULT -1,CrsCod INT NOT NULL DEFAULT -1,Cod INT NOT NULL DEFAULT -1,TimeEvent DATETIME NOT NULL,UNIQUE INDEX(SocCod),INDEX(SocialEvent),INDEX(UsrCod),INDEX(TimeEvent));
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1490','es','N','Ver actividad social (timeline)');

        Version 15.76.1:  Dec 28, 2015	Refactoring in swad_database. (187926 lines)
        Version 15.76:    Dec 28, 2015	New option for social network activity (timeline).
					New module swad_social. (187807 lines)
        Version 15.75:    Dec 28, 2015	New tab SOCIAL for social networking. (187664 lines)
        Version 15.74.8:  Dec 28, 2015	"Course" is moved to the top of the menu used to go directly to my courses. (187564 lines)
        Version 15.74.7:  Dec 26, 2015	Changes in small icons. (187563 lines)
        Version 15.74.6:  Dec 26, 2015	Reply icon is shown without text. (187562 lines)
        Version 15.74.5:  Dec 26, 2015	Text in link to remove messages is now shorter.
					Fixed bug in messages. (187563 lines)
        Version 15.74.4:  Dec 26, 2015	Change in text when no course is selected. (187757 lines)
        Version 15.74.3:  Dec 26, 2015	Changes in layout of breadcrumb. (187770 lines)
        Version 15.74.2:  Dec 26, 2015	Changes in list of countries. (187761 lines)
        Version 15.74.1:  Dec 26, 2015	Changes in layout of breadcrumb. (187775 lines)
        Version 15.74:    Dec 26, 2015	New layout of breadcrumb. (187750 lines)
        Version 15.73.4:  Dec 26, 2015	Code refactoring in breadcrumb. (187719 lines)
        Version 15.73.3:  Dec 25, 2015	Select one of my courses goes to the current action. (187710 lines)
        Version 15.73.2:  Dec 25, 2015	Background of selected course in light blue. (187698 lines)
        Version 15.73.1:  Dec 25, 2015	Background of selected privacy options in light blue. (187704 lines)
        Version 15.73:    Dec 24, 2015	Fixed bug in privacy form.
					Privacy is moved to preferences.
					Default privacy is changed from "user" to "system". (187700 lines)
        Version 15.72.7:  Dec 24, 2015	Remove message to teachers to verify indicators. (187695 lines)
        Version 15.72.6:  Dec 24, 2015	Debug message removed. (187796 lines)
        Version 15.72.5:  Dec 24, 2015	Fixed bugs while sending a message to a unique recipient. (187794 lines)
        Version 15.72.4:  Dec 23, 2015	Show recipient when writing message to only one recipient. (187775 lines)
        Version 15.72.3:  Dec 23, 2015	Message translated. (187733 lines)
        Version 15.72.2:  Dec 23, 2015	Write message only to one recipient. Not finished.
					Some small icons changed. (187750 lines)
        Version 15.72.1:  Dec 22, 2015	Write message only to one recipient. Not finished. (187724 lines)
        Version 15.72:    Dec 22, 2015	Reply message only to sender, suggested by Francisco Oca�a Lara. Not finished. (187705 lines)
        Version 15.71.3:  Dec 22, 2015	Changes in birthday range of years. (187669 lines)
        Version 15.71.2:  Dec 21, 2015	Fixed bugs in removing old files in briefcase. (187666 lines)
        Version 15.71.1:  Dec 21, 2015	Fixed bugs in removing old files in briefcase. (187665 lines)
        Version 15.71:    Dec 21, 2015	Removing old files in briefcase. Finished. (187614 lines)
        Version 15.70.7:  Dec 21, 2015	Code refactoring in file browser. (187614 lines)
        Version 15.70.6:  Dec 21, 2015	Removing old files in briefcase. Not finished. (187604 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1488','es','N','Solicitar eliminar archivos antiguos malet&iacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1489','es','N','Eliminar archivos antiguos malet&iacute;n');

        Version 15.70.5:  Dec 21, 2015	Code refactoring in file browser. (187569 lines)
        Version 15.70.4:  Dec 21, 2015	Code refactoring in file browser. (187527 lines)
        Version 15.70.3:  Dec 21, 2015	Code refactoring in file browser. (187490 lines)
        Version 15.70.2:  Dec 21, 2015	Users can selected number of months in form to remove old files in briefcase. Not finished. (187473 lines)
        Version 15.70.1:  Dec 21, 2015	Removing old files in briefcase. Not finished. (187408 lines)
        Version 15.70:    Dec 21, 2015	New action to remove old files in briefcase. Not finished. (187352 lines)
        Version 15.69:    Dec 20, 2015	New option in briefcase to ask for removing old files. (187270 lines)
        Version 15.68.1:  Dec 20, 2015	Fixed bugs in connected users. (187171 lines)
        Version 15.68:    Dec 19, 2015	Show guests in connected users.
					Optimization in queries about connected users. (187158 lines)
        Version 15.67.1:  Dec 15, 2015	Changed some messages. (187141 lines)
        Version 15.67:    Dec 15, 2015	Fixed bugs in queries of enrolment requests. (187140 lines)
        Version 15.66.3:  Dec 15, 2015	Changes in some small icons.
					Fixed bug in record card. (187004 lines)
					2 changes necessary in database:
UPDATE actions SET Txt='Ver informaci&oacute;n pa&iacute;s' WHERE ActCod='1155' AND Language='es';
UPDATE actions SET Txt='Imprimir informaci&oacute;n pa&iacute;s' WHERE ActCod='1156' AND Language='es';

        Version 15.66.2:  Dec 14, 2015	Changes in edition of institutions, centres and degrees. (186994 lines)
        Version 15.66.1:  Dec 14, 2015	Removed unused code related to list of countries, institutions and centres by reasons of speed. (186966 lines)
        Version 15.66:    Dec 14, 2015	Number of teachers and number of students removed from list of countries, institutions and centres by reasons of speed. (187057 lines)
        Version 15.65.1:  Dec 14, 2015	Changes in layout of profile. (187031 lines)
        Version 15.65:    Dec 13, 2015	Change in layout of contextual links. (187009 lines)
        Version 15.64.18: Dec 13, 2015	Code refactoring in contextual links. (187004 lines)
        Version 15.64.17: Dec 13, 2015	Code refactoring in contextual links.
					Fixed minor bug in tests. (187009 lines)
        Version 15.64.16: Dec 13, 2015	Code refactoring in contextual links.
					Fixed minor bug in surveys. (187012 lines)
        Version 15.64.15: Dec 13, 2015	Code refactoring in contextual links. (187025 lines)
        Version 15.64.14: Dec 13, 2015	Code refactoring in contextual links. (187037 lines)
        Version 15.64.13: Dec 13, 2015	Code refactoring in contextual links. (187031 lines)
        Version 15.64.12: Dec 13, 2015	Code refactoring in contextual links. (187061 lines)
        Version 15.64.11: Dec 13, 2015	Code refactoring in contextual links. (187056 lines)
        Version 15.64.10: Dec 13, 2015	Code refactoring in contextual links. (187060 lines)
        Version 15.64.9:  Dec 13, 2015	Code refactoring in contextual links. (187061 lines)
        Version 15.64.8:  Dec 13, 2015	Code refactoring in contextual links. (187066 lines)
        Version 15.64.7:  Dec 13, 2015	Code refactoring in contextual links. (187063 lines)
        Version 15.64.6:  Dec 13, 2015	Code refactoring in contextual links. (186974 lines)
        Version 15.64.5:  Dec 13, 2015	Code refactoring in record card. (186944 lines)
        Version 15.64.4:  Dec 13, 2015	Links to web / social networks are not displayed when printing record card. (186986 lines)
        Version 15.64.3:  Dec 13, 2015	Changes in layout of records. (186979 lines)
        Version 15.64.2:  Dec 13, 2015	Changes in some CSS names. (186975 lines)
        Version 15.64.1:  Dec 12, 2015	Changes in layout of user's public profile. (186980 lines)
        Version 15.64:    Dec 12, 2015	Changes in layout of user's public profile.
					Changes in some small icons. (187004 lines)
        Version 15.63:    Dec 12, 2015	New layout for user's public profile.
                                        Fixed bug when removing user's account. (186932 lines)
					2 changes necessary in database:
DELETE FROM usr_follow WHERE FollowerCod NOT IN (SELECT UsrCod FROM usr_data);
DELETE FROM usr_follow WHERE FollowedCod NOT IN (SELECT UsrCod FROM usr_data);

        Version 15.62:    Dec 09, 2015	Changes in data of centres. (186949 lines)
        Version 15.61:    Dec 09, 2015	Bug fixing and lot of changes in data of countries and institutions. (186852 lines)
        Version 15.60.6:  Dec 08, 2015	Changed icons in login. (186592 lines)
        Version 15.60.5:  Dec 08, 2015	Changed some messages related to login. (186589 lines)
        Version 15.60.4:  Dec 08, 2015	Change in text about first / last year of a degree. (186589 lines)
        Version 15.60.3:  Dec 08, 2015	Fixed bug in country.
					Code refactoring in menu. (186588 lines)
        Version 15.60.2:  Dec 08, 2015	Link to user's country in public profile. (186592 lines)
        Version 15.60.1:  Dec 07, 2015	Fixed names of some small icons.
					New link in student's record card to see her/his attendance. (186576 lines)
        Version 15.60:    Dec 07, 2015	By default, the language of a new user is unknown.
					Fixed bugs when registering a new user. (? lines)
        Version 15.59.4:  Dec 06, 2015	Changes in some small icons. (186462 lines)
        Version 15.59.3:  Dec 06, 2015	If photo zoom is out of the screen at left, it is shown at right of the photo. (186445 lines)
        Version 15.59.2:  Dec 06, 2015	Courses with year not applicable are listed at the end. (186439 lines)
        Version 15.59.1:  Dec 06, 2015	Removed unused code related to year for optional courses. (186405 lines)
					1 change necessary in database:
ALTER TABLE degrees DROP COLUMN OptYear;

        Version 15.59:    Dec 06, 2015	Year for optional courses is now always present. (186692 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='552';

        Version 15.58.1:  Dec 06, 2015	Removed unused code related to semester. (186604 lines)
					1 change necessary in database:
ALTER TABLE courses DROP COLUMN Semester;

        Version 15.58:    Dec 06, 2015	Year and semester are fusioned to one number: year/semester. (186960 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='562';

        Version 15.57:    Dec 05, 2015	Lot of changes in forms to edit institutions, centres, degrees, courses, etc. (186903 lines)
        Version 15.56.6:  Dec 05, 2015	Fixed minor bug in edition of degrees. (186843 lines)
        Version 15.56.5:  Dec 05, 2015	Go to degree in table of degrees (statistic). (186843 lines)
        Version 15.56.4:  Dec 05, 2015	Changed icons in forums and chat. (186844 lines)
        Version 15.56.3:  Dec 05, 2015	Changed icons in statistics about forums. (186843 lines)
        Version 15.56.2:  Dec 05, 2015	Fixed bug in statistics about OERs. (186842 lines)
        Version 15.56.1:  Dec 02, 2015	Changes in HTML and CSS to validate HTML5 in W3C validator.
					Removed about zone in versions of the page to be printed. (186837 lines)
        Version 15.56:    Dec 02, 2015	Fixed bug and changes in layout related to time to generate and send page. (188597 lines)
        Version 15.55:    Dec 01, 2015	Change in layout background. (186805 lines)
        Version 15.54.7:  Dec 01, 2015	Change in title (country, institution, centre, degree, course). (186783 lines)
        Version 15.54.6:  Dec 01, 2015	Change in layout of file browser. (186768 lines)
        Version 15.54.5:  Dec 01, 2015	Fixed bug in file browser, reported by Jacinto Dom�nguez Rull. (186763 lines)
        Version 15.54.4:  Dec 01, 2015	Fixed bug in title (country, institution, centre, degree, course). (186761 lines)
        Version 15.54.3:  Dec 01, 2015	Minor changes in tabs, menus and CSS. (186758 lines)
        Version 15.54.2:  Dec 01, 2015	Minor changes in CSS. (186751 lines)
        Version 15.54.1:  Dec 01, 2015	Some settings related with timetables. (186746 lines)
        Version 15.54:    Dec 01, 2015	Selection of the first day of the week in timetables. (186740 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1486','es','N','Cambiar primer d&iacute;a semana en horario asig.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1487','es','N','Cambiar primer d&iacute;a semana en mi horario');

        Version 15.53:    Nov 30, 2015	Sunday can be the first day of the week in timetable, depending on user's preferences. (186689 lines)
        Version 15.52:    Nov 30, 2015	Timetable now starts at 6 a.m. instead of 7 a.m.
					Changes in layout of timetable. (186656 lines)
					6 changes necessary in database:
CREATE TABLE timetable_crs_backup LIKE timetable_crs;
INSERT INTO timetable_crs_backup SELECT * FROM timetable_crs;
CREATE TABLE timetable_tut_backup LIKE timetable_tut;
INSERT INTO timetable_tut_backup SELECT * FROM timetable_tut;
UPDATE timetable_crs SET Hour=Hour+2;
UPDATE timetable_tut SET Hour=Hour+2;

        Version 15.51.4:  Nov 30, 2015	Calendar is now distributed in rows with 3 months instead of 4.
					Changes in layout. (186644 lines)
        Version 15.51.3:  Nov 30, 2015	Responsive design in login-logout. (186641 lines)
        Version 15.51.2:  Nov 30, 2015	Responsive design in tabs. (186688 lines)
        Version 15.51.1:  Nov 30, 2015	Responsive design in logo.
					Responsive design in breadcrumb. (186638 lines)
        Version 15.51:    Nov 30, 2015	Responsive design in title (country, institution, centre, degree, course).
					CSS file is renamed after changes in order to invalidate browser cache. (186564 lines)
        Version 15.50.2:  Nov 30, 2015	Changes in layout of preferences. (186517 lines)
        Version 15.50.1:  Nov 30, 2015	Changes in layout of heading. (186494 lines)
        Version 15.50:    Nov 30, 2015	Changes in layout of menus.
					Responsive design in menu: when screen is small, horizontal menu is drawn. (186514 lines)
        Version 15.49.4:  Nov 29, 2015	Changes in layout of menus. (186481 lines)
        Version 15.49.3:  Nov 29, 2015	Changes in layout of menus. (186502 lines)
        Version 15.49.2:  Nov 29, 2015	Changes in layout of menus. (186501 lines)
        Version 15.49.1:  Nov 27, 2015	Length of country names limited in listing. of countries. (186460 lines)
        Version 15.49:    Nov 27, 2015	Fixed width in edition of institutional links.
					Lot of fixing in CSS related to sizes. (186455 lines)
        Version 15.48:    Nov 27, 2015	Fixed problems in layout. (186384 lines)
        Version 15.47.4:  Nov 27, 2015	Fixed CSS problem in layout. (186374 lines)
        Version 15.47.3:  Nov 27, 2015	Responsive design in notifications. (186365 lines)
        Version 15.47.2:  Nov 27, 2015	Responsive design in current local date and time. (? lines)
        Version 15.47.1:  Nov 27, 2015	swad_desktop.css is renamed to swad.css.
					swad_mobile.css is removed. (186343 lines)
        Version 15.47:    Nov 27, 2015	Mobile layout is removed (only 1.45% of users used it and now it is replaced by responsive design). (187426 lines)
					3 changes necessary in database:
ALTER TABLE IP_prefs DROP COLUMN Layout;
ALTER TABLE usr_data DROP COLUMN Layout;
UPDATE actions SET Obsolete='Y' WHERE ActCod='672';

        Version 15.46.1:  Nov 27, 2015	Responsive design in search box (top of the page). (188094 lines)
        Version 15.46:    Nov 27, 2015	First version with responsive design: both side columns are hidden when screen size is small. (188078 lines)
        Version 15.45.15: Nov 25, 2015	Some CSS styles related to layout are moved to CSS file. (188021 lines)
        Version 15.45.14: Nov 25, 2015	Some CSS styles related to layout are moved to CSS file. (188037 lines)
        Version 15.45.13: Nov 25, 2015	Some CSS styles related to layout are moved to CSS file. (188059 lines)
        Version 15.45.12: Nov 25, 2015	Some CSS styles related to layout are moved to CSS file. (188047 lines)
        Version 15.45.11: Nov 25, 2015	Some CSS styles related to layout are moved to CSS file. (188017 lines)
        Version 15.45.10: Nov 25, 2015	Some CSS styles related to layout are moved to CSS file. (187992 lines)
        Version 15.45.9:  Nov 25, 2015	Some CSS styles related to layout are moved to CSS file. (188020 lines)
        Version 15.45.8:  Nov 25, 2015	Some CSS styles related to layout are moved to CSS file. (188018 lines)
        Version 15.45.7:  Nov 25, 2015	Some CSS styles related to layout are moved to CSS file. (188015 lines)
        Version 15.45.6:  Nov 25, 2015	Some CSS styles related to layout are moved to CSS file. (188007 lines)
        Version 15.45.5:  Nov 25, 2015	Some CSS styles related to layout are moved to CSS file. (187992 lines)
        Version 15.45.4:  Nov 25, 2015	Some CSS styles related to layout are moved to CSS file. (187979 lines)
        Version 15.45.3:  Nov 25, 2015	Some CSS styles related to layout are moved to CSS file. (187963 lines)
        Version 15.45.2:  Nov 25, 2015	Some CSS styles related to layout are moved to CSS file. (187929 lines)
        Version 15.45.1:  Nov 25, 2015	Changes in layout of page (some tables used with layout purposes are replaced by CSS). (187884 lines)
        Version 15.45:    Nov 25, 2015	Changes in layout of page (some tables used with layout purposes are replaced by CSS). (187877 lines)
        Version 15.44.8:  Nov 24, 2015	Code refactoring in layout. (187895 lines)
        Version 15.44.7:  Nov 24, 2015	Code refactoring in layout. (187901 lines)
        Version 15.44.6:  Nov 24, 2015	Code refactoring in layout. (187893 lines)
        Version 15.44.5:  Nov 24, 2015	Code refactoring in layout. (187894 lines)
        Version 15.44.4:  Nov 24, 2015	Code refactoring in layout. (187893 lines)
        Version 15.44.3:  Nov 23, 2015	Improvement of speed in list of administrators. (187841 lines)
					1 change necessary in database:
ALTER TABLE admin ADD INDEX (Scope,Cod);

        Version 15.44.2:  Nov 23, 2015	Improvement of speed in list of administrators. (187819 lines)
        Version 15.44.1:  Nov 23, 2015	Fixed bug in list of administrators. (187809 lines)
        Version 15.44:    Nov 23, 2015	Changes in list of institution, centre and degree administrators. (187788 lines)
        Version 15.43.2:  Nov 22, 2015	Minor changes in CSS. (187719 lines)
        Version 15.43.1:  Nov 22, 2015	New figure (statistic) about first day of week. (187722 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1485','es','N','Cambiar primer d&iacute;a semana en calendario');

        Version 15.43:    Nov 21, 2015	Form in calendar to select first day of week. (187693 lines)
        Version 15.42.4:  Nov 21, 2015	Fixed bug when getting first day of week from IP preferences. (187535 lines)
        Version 15.42.3:  Nov 21, 2015	Stats of hits distributed by week depend on user's preference about first day of the week. (187528 lines)
        Version 15.42.2:  Nov 21, 2015	Fixed bug in JavaScript code to draw months. (187524 lines)
        Version 15.42.1:  Nov 21, 2015	Calendar is drawn depending on user's preference about first day of the week. (187523 lines)
        Version 15.42:    Nov 21, 2015	New option in preferences to select first day of the week. (187520 lines)
					3 changes necessary in database:
ALTER TABLE IP_prefs ADD COLUMN FirstDayOfWeek TINYINT NOT NULL DEFAULT 0 AFTER LastChange;
ALTER TABLE usr_data ADD COLUMN FirstDayOfWeek TINYINT NOT NULL DEFAULT 0 AFTER Language,ADD INDEX (FirstDayOfWeek);
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1484','es','N','Cambiar primer d&iacute;a de semana');

        Version 15.41:    Nov 21, 2015	JavaScript code to draw a month with sunday as the first day of a week. (187316 lines)
        Version 15.40.8:  Nov 20, 2015	Password is requested only one time when creating a new account. (187309 lines)
        Version 15.40.7:  Nov 20, 2015	New help button to register students in courses without them. (187319 lines)
        Version 15.40.6:  Nov 20, 2015	New button to register students in courses without them.
					New buttons to create institution, centre, degree and course. (187285 lines)
        Version 15.40.5:  Nov 19, 2015	Code refactoring in degrees. (187274 lines)
        Version 15.40.4:  Nov 19, 2015	Code refactoring in centres. (187279 lines)
        Version 15.40.3:  Nov 19, 2015	Code refactoring in institutions. (187281 lines)
        Version 15.40.2:  Nov 19, 2015	Some links to countries, institutions, centres and degrees removed. (187281 lines)
        Version 15.40.1:  Nov 19, 2015	Changes in behaviour of links to countries, institutions, centres and degrees. (187288 lines)
        Version 15.40:    Nov 17, 2015	Changes in statistics of number of users.
					Changes in listing of countries. (187275 lines)
        Version 15.39.1:  Nov 16, 2015	User' email can be removed even if it is the unique. (187236 lines)
        Version 15.39:    Nov 16, 2015	Administrators can edit another user' emails. (187268 lines)
					9 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1475','es','N','Solicitar edici&oacute;n dir. correo invitado');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1476','es','N','Solicitar edici&oacute;n dir. correo estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1477','es','N','Solicitar edici&oacute;n dir. correo profesor');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1478','es','N','Eliminar dir. correo invitado');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1479','es','N','Eliminar dir. correo estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1480','es','N','Eliminar dir. correo profesor');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1481','es','N','Cambiar dir. correo invitado');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1482','es','N','Cambiar dir. correo estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1483','es','N','Cambiar dir. correo profesor');

        Version 15.38.3:  Nov 15, 2015	Changes in listing of places. (187073 lines)
        Version 15.38.2:  Nov 15, 2015	Fixed bug in user's privacy preferences. (187071 lines)
        Version 15.38.1:  Nov 14, 2015	Changes in permission to query the whole range of dates in stats, suggested by Francisco Oca�a Lara. (187073 lines)
        Version 15.38:    Nov 13, 2015	New option for students: list of my attendance. (187063 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1473','es','N','Listar mi asistencia a varios eventos');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1474','es','N','Imprimir mi asistencia a varios eventos');

        Version 15.37.6:  Nov 12, 2015	Changes in degree logo.
					Changes in layout of hits stats. (186910 lines)
        Version 15.37.5:  Nov 12, 2015	Changes in centre logo. (186886 lines)
        Version 15.37.4:  Nov 12, 2015	Changes in institution logo. (186891 lines)
        Version 15.37.3:  Nov 11, 2015	Changes in country map image. (186898 lines)
        Version 15.37.2:  Nov 11, 2015	Change in layout (background). (186912 lines)
        Version 15.37.1:  Nov 11, 2015	Minor changes in sessions. (186912 lines)
        Version 15.37:    Nov 11, 2015	Changes to speed up queries related to time.
					Time to next test printed in user's local time.
					Table table IP_last removed. (186909 lines)
					1 change necessary in database:
DROP TABLE IF EXISTS IP_last;

        Version 15.36:    Nov 10, 2015	Fixed bug in assignments, reported by Javier Fern�ndez Baldomero.
					Changes in automatic creation of assignment folders. (186985 lines)
        Version 15.35.5:  Nov 09, 2015	Fixed bug in swad.sql, reported by Florent H. Carr�.
					Fixed bug in groups, reported by Francisco Oca�a Lara. (186977 lines)
        Version 15.35.4:  Nov 09, 2015	Fixed bug in test exams. (186975 lines)
        Version 15.35.3:  Nov 09, 2015	Fixed bug in test exams, reported by Antonio Arcos Cebri�n.
					Fixed bug in marks, reported by Javier Fern�ndez Baldomero. (186973 lines)
        Version 15.35.2:  Nov 04, 2015	Fixed bug in edition of degrees. (186963 lines)
        Version 15.35.1:  Nov 03, 2015	Confirmation message after removing a notice. (186979 lines)
        Version 15.35:    Nov 03, 2015	Request confirmation on notice removal. (186944 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1472','es','N','Solicitar elim. aviso');

        Version 15.34.4:  Nov 03, 2015	Fixed bug in account creation, reported by H�ctor Garc�a Marcos. (186881 lines)
        Version 15.34.3:  Nov 03, 2015	Fixed bug in photo uploading, reported by Fernando Rojas Ruiz. (186879 lines)
        Version 15.34.2:  Nov 02, 2015	Fixed bugs in date forms. (186878 lines)
        Version 15.34.1:  Nov 01, 2015	Global announcements can be seen by all. (186938 lines)
        Version 15.34:    Nov 01, 2015	Global announcements can be hidden. (186896 lines)
					3 changes necessary in database:
ALTER TABLE announcements ADD COLUMN Status TINYINT NOT NULL DEFAULT 0 AFTER AnnCod,ADD INDEX (Status);
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1470','es','N','Ocultar anuncio global');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1471','es','N','Mostrar anuncio global');

        Version 15.33.1:  Nov 01, 2015	New layout of global announcements. (186755 lines)
        Version 15.33:    Nov 01, 2015	New layout of notices. (186708 lines)
        Version 15.32.2:  Oct 31, 2015	Fixed bug in form submit. (186618 lines)
        Version 15.32.1:  Oct 31, 2015	Dates in holidays are shown in big-endian.
					All dates are now shown in ISO 8601 format (YYYY-MM-DD). (186627 lines)
        Version 15.32:    Oct 30, 2015	Code refactoring and cleaning related to calendar. (186617 lines)
        Version 15.31:    Oct 30, 2015	Calendar is drawn in browser using JavaScript. (186902 lines)
        Version 15.30:    Oct 30, 2015	Current month is drawn in browser using JavaScript. (186795 lines)
        Version 15.29.4:  Oct 29, 2015	Writing JavaScript function to draw a month in browser. Not finished. (186729 lines)
        Version 15.29.3:  Oct 29, 2015	Writing JavaScript function to draw a month in browser. Not finished. (186703 lines)
        Version 15.29.2:  Oct 29, 2015	Writing JavaScript function to draw a month in browser. Not finished. (186694 lines)
        Version 15.29.1:  Oct 29, 2015	Writing JavaScript function to draw a month in browser. Not finished. (186694 lines)
        Version 15.29:    Oct 29, 2015	Writing JavaScript function to draw a month in browser. Not finished. (186684 lines)
        Version 15.28.1:  Oct 28, 2015	Fixed bug in the calculation of some statistics. (186416 lines)
        Version 15.28:    Oct 28, 2015	IANA zone names are used in the calculation of some statistics. (186407 lines)
					1 change necessary in database:
Populate the Time Zone Tables by following the instructions in http://dev.mysql.com/doc/refman/5.1/en/time-zone-support.html:
mysql_tzinfo_to_sql /usr/share/zoneinfo | mysql -u root -p mysql

					1 script necessary:
Download JavaScript jstz script, available in:
        https://bitbucket.org/pellepim/jstimezonedetect/
        http://pellepim.bitbucket.org/jstz/
Create new directory jstz in public HTML directory.
Copy script jstz.js from https://bitbucket.org/pellepim/jstimezonedetect/ > Download repository > pellepim-jstimezonedetect-3a00f59861bd.zip > dist > jstz.js to /var/www/html/swad/jstz/jstz.js

        Version 15.27:    Oct 28, 2015	Fixed bug in dates. (186302 lines)
        Version 15.26.1:  Oct 27, 2015	Fixed bug in statistics. (186282 lines)
        Version 15.26:    Oct 27, 2015	Statistics are computed properly for clients with time-zones different to that of the server. (186278 lines)
        Version 15.25.4:  Oct 27, 2015	Day and month are displayed in clock. (186251 lines)
        Version 15.25.3:  Oct 27, 2015	Statistics computed using time UTC. (186225 lines)
        Version 15.25.2:  Oct 26, 2015	Changes related to date-times. (186247 lines)
        Version 15.25.1:  Oct 26, 2015	Warning about stats computed in server time instead of user's local time. (186263 lines)
        Version 15.25:    Oct 26, 2015	Clicks in real time are displayed as time ago. (186211 lines)
        Version 15.24.2:  Oct 26, 2015	Change in function that write clock. (186219 lines)
        Version 15.24.1:  Oct 26, 2015	Remove unused code related to date-times. (186217 lines)
        Version 15.24:    Oct 26, 2015	Lot of changes in swad.js related to date-times. (186336 lines)
        Version 15.23:    Oct 26, 2015	Date-times to open groups are displayed in client local date-time. (186259 lines)
        Version 15.22.1:  Oct 26, 2015	Date-times in notices are displayed in client local date-time. (186344 lines)
        Version 15.22:    Oct 26, 2015	First click time in users' profiles is displayed in client local date-time. (186328 lines)
        Version 15.21.3:  Oct 26, 2015	Changes in date-times of test exams. (186325 lines)
        Version 15.21.2:  Oct 26, 2015	Date-times in tests are displayed in client local date-time. (186315 lines)
        Version 15.21.1:  Oct 26, 2015	New forms to enter range of date-times in statistic and test. (186313 lines)
        Version 15.21:    Oct 25, 2015	New forms to enter range of date-times in statistic and test. Not finished. (186549 lines)
        Version 15.20.2:  Oct 24, 2015	Code refactoring related to date-times forms. (186294 lines)
        Version 15.20.1:  Oct 24, 2015	Detailed clics are displayed in client local date-time. (186333 lines)
        Version 15.20:    Oct 24, 2015	Date-times in notifications, messages and forums are displayed in client local date-time. (186334 lines)
        Version 15.19:    Oct 23, 2015	Surveys date-times are displayed in client local date-time. (186309 lines)
        Version 15.18:    Oct 23, 2015	Attendance date-times are displayed in client local date-time.
				        Changes in JavaScript functions related to date-time. (186403 lines)
        Version 15.17.3:  Oct 23, 2015	Changes in JavaScript functions related to date-time. (186477 lines)
        Version 15.17.2:  Oct 23, 2015	Code refactoring related to forms. (186472 lines)
        Version 15.17.1:  Oct 23, 2015	Code refactoring related to dates and JavaScript. (186407 lines)
        Version 15.17:    Oct 22, 2015	Code refactoring related to dates and JavaScript.
                                        Fixed bug in internal editor of syllabus. (186379 lines)
        Version 15.16.2:  Oct 22, 2015	Assignment date-times are displayed in client local date-time. Not finished. (186479 lines)
        Version 15.16.1:  Oct 22, 2015	Assignment date-times are displayed in client local date-time. Not finished. (186468 lines)
        Version 15.16:    Oct 22, 2015	Assignment date-times are displayed in client local date-time. Not finished. (186436 lines)
        Version 15.15.3:  Oct 21, 2015	File date-time is displayed in client local date-time. (186264 lines)
        Version 15.15.2:  Oct 21, 2015	Fixed bug in client local date-time. (186254 lines)
        Version 15.15.1:  Oct 21, 2015	Top-left date-time is now displayed in local time. (186249 lines)
        Version 15.15:    Oct 19, 2015	Statistic figure about institutions can be displayed as a class photo or as a list. (186251 lines)
        Version 15.14.2:  Oct 16, 2015	Minor changes in layout. (186166 lines)
        Version 15.14.1:  Oct 16, 2015	Fixed minor bug in edition of degrees. (186170 lines)
        Version 15.14:    Oct 15, 2015	Table log (historical log) renamed as log_full. (186169 lines)
					1 change necessary in database:
RENAME TABLE log TO log_full;

        Version 15.13.3:  Oct 14, 2015	swad_bool.h replaced by stdbool.h. (? lines)
        Version 15.13.2:  Oct 11, 2015	Refactoring and bug fixing in statistics. (186183 lines)
        Version 15.13.1:  Oct 11, 2015	Refactoring and bug fixing in statistics related to scopes. (186300 lines)
					6 optional slow changes in database (may spend many minutes or even hours depending on the size of log tables):
UPDATE log_recent,degrees SET log_recent.CtrCod=degrees.CtrCod WHERE log_recent.DegCod=degrees.DegCod;
UPDATE log_recent,centres SET log_recent.InsCod=centres.InsCod WHERE log_recent.CtrCod=centres.CtrCod;
UPDATE log_recent,institutions SET log_recent.CtyCod=institutions.CtyCod WHERE log_recent.InsCod=institutions.InsCod;
UPDATE log,degrees SET log.CtrCod=degrees.CtrCod WHERE log.DegCod=degrees.DegCod;
UPDATE log,centres SET log.InsCod=centres.InsCod WHERE log.CtrCod=centres.CtrCod;
UPDATE log,institutions SET log.CtyCod=institutions.CtyCod WHERE log.InsCod=institutions.InsCod;

        Version 15.13:    Oct 11, 2015	Code refactoring in module swad_statistic.
					New statistics distributed by country, institution and centre. (186282 lines)
        Version 15.12:    Oct 10, 2015	Country, institution and centre are now stored in log. (185817 lines)
					2 slow changes necessary in database (may spend many minutes or even hours depending on the size of log tables):
ALTER TABLE log_recent ADD COLUMN CtyCod INT NOT NULL DEFAULT -1 AFTER ActCod,ADD INDEX (CtyCod),ADD COLUMN InsCod INT NOT NULL DEFAULT -1 AFTER CtyCod,ADD INDEX (InsCod),ADD COLUMN CtrCod INT NOT NULL DEFAULT -1 AFTER InsCod,ADD INDEX (CtrCod);
ALTER TABLE log ADD COLUMN CtyCod INT NOT NULL DEFAULT -1 AFTER ActCod,ADD INDEX (CtyCod),ADD COLUMN InsCod INT NOT NULL DEFAULT -1 AFTER CtyCod,ADD INDEX (InsCod),ADD COLUMN CtrCod INT NOT NULL DEFAULT -1 AFTER InsCod,ADD INDEX (CtrCod);

        Version 15.11:    Oct 08, 2015	Changes in layout of sent / received messages. (185680 lines)
        Version 15.10.5:  Oct 07, 2015	Change in layout of removal of old users. (185669 lines)
        Version 15.10.4:  Oct 07, 2015	Change in layout of removal of old courses. (185665 lines)
        Version 15.10.3:  Oct 07, 2015	New contextual link in visits to last clicks. (185641 lines)
        Version 15.10.2:  Oct 07, 2015	Fixed bug in nicknames, reported by Javier Fdez. Baldomero.
					New MIME type, suggested by Javier Fdez. Baldomero.
					Some code refactoring related to nicknames. (185639 lines)
        Version 15.10.1:  Oct 06, 2015	New row in contextual help to upload photo. (185581 lines)
        Version 15.10:    Oct 06, 2015	New module swad_help for contextual help to enhance usability. (185561 lines)
					1 change necessary in Makefile:
Add swad_help.o to list of object files

        Version 15.9.1:   Oct 06, 2015	Minor change in contextual help. (185514 lines)
        Version 15.9:     Oct 05, 2015	Contextual help to encourage enrolment. (185512 lines)
        Version 15.8:     Oct 04, 2015	Changes in file browser layout.
					Folder owners are not displayed. (185022 lines)
        Version 15.7.2:   Oct 04, 2015	New layout in enrolment requests. (185018 lines)
        Version 15.7.1:   Oct 03, 2015	Message translated. (184984 lines)
        Version 15.7:     Oct 03, 2015	Height of tabs reduced.
					Changes in layout of students record cards in course. (184962 lines)
        Version 15.6.3:   Oct 03, 2015	Radius of rounded corners is reduced. (184966 lines)
        Version 15.6.2:   Oct 03, 2015	Changes in layout of file browsers. (184963 lines)
        Version 15.6.1:   Oct 03, 2015	Changes in layout of file browsers. (184956 lines)
        Version 15.6  :   Oct 02, 2015	Now a click in a form may go to a part of the destination page. (184959 lines)
        Version 15.5.2:   Oct 02, 2015	Changes in layout of tests exams. (184934 lines)
        Version 15.5.1:   Oct 02, 2015	Changes in layout of forms with initial and final dates. (184938 lines)
        Version 15.5:     Oct 01, 2015	Changes in layout of selectors classphoto/list. (184909 lines)
        Version 15.4.2:   Oct 01, 2015	Institution admins can edit holidays. (184863 lines)
        Version 15.4.1:   Sep 30, 2015	Code refactoring in global/course statistics. (184861 lines)
        Version 15.4:     Sep 30, 2015	Changes in global/course statistics forms. (184877 lines)
        Version 15.3.3:   Sep 30, 2015	Changes in layout of form to selected initial and final dates. (185152 lines)
        Version 15.3.2:   Sep 30, 2015	Changes in layout of statistical figures. (185156 lines)
        Version 15.3.1:   Sep 29, 2015	Changes in background and fonts. (185156 lines)
        Version 15.3:     Sep 28, 2015	All sizes are multiplied by 0.8333 (25% bigger than those previous to version 15.0). (185148 lines)
        Version 15.2.2:   Sep 27, 2015	Fixed minor problem in CSS. (185172 lines)
        Version 15.2.1:   Sep 27, 2015	Change in layout. Background images are changed to CSS linear gradients. (185171 lines)
        Version 15.2:     Sep 27, 2015	Change in layout. Background images are changed to CSS linear gradients. (185185 lines)
        Version 15.1.3:   Sep 27, 2015	Change in CSS. (185173 lines)
        Version 15.1.2:   Sep 27, 2015	Fixed bug in requests for registration. (185170 lines)
        Version 15.1.1:   Sep 26, 2015	Fixed bug in statistics. (185084 lines)
        Version 15.1:     Sep 25, 2015	Changes in layout of statistics of clicks. (185058 lines)
        Version 15.0.3:   Sep 24, 2015	Fixed bug when printing country information. (185013 lines)
        Version 15.0.2:   Sep 24, 2015	New layout in preferences. (185011 lines)
        Version 15.0.1:   Sep 23, 2015	Fix bugs in size of background images. (185006 lines)
        Version 15.0:     Sep 23, 2015	All sizes are multiplied by 1.5 (50% bigger). (184885 lines)
        Version 14.135:   Sep 23, 2015	Option to show last clicks in real time is moved inside "Users - Connected". (184883 lines)
        Version 14.134.9: Sep 22, 2015	Link in connected users to clicks in real time. (184923 lines)
        Version 14.134.8: Sep 20, 2015	Fixed bug in list of guests.
                                        Changes in listing of connected users. (184885 lines)
        Version 14.134.7: Sep 18, 2015	Fixed bug in list of guests. (184879 lines)
        Version 14.134.6: Sep 18, 2015	Fixed bug in user enrolment. (184878 lines)
        Version 14.134.5: Sep 18, 2015	Change in order of options in user menu.
                                        Change in styles and layout related to users. (184877 lines)
        Version 14.134.4: Sep 18, 2015	Changes in user enrolment. (184876 lines)
        Version 14.134.3: Sep 18, 2015	Changes in user enrolment. (184875 lines)
					4 changes necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='82';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1467','es','N','Cambiar contrase&ntilde;a otro usuario');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1468','es','N','Cambiar contrase&ntilde;a estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1469','es','N','Cambiar contrase&ntilde;a profesor');

        Version 14.134.2: Sep 17, 2015	Changes in user enrolment. (184852 lines)
					4 changes necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='598';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1464','es','N','Solicitar cambio contrase&ntilde;a otro usuario');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1465','es','N','Solicitar cambio contrase&ntilde;a estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1466','es','N','Solicitar cambio contrase&ntilde;a profesor');

        Version 14.134.1: Sep 17, 2015	Changes in user enrolment. (184834 lines)
					3 changes necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='58';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1462','es','N','Eliminar estudiante asignatura');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1463','es','N','Eliminar profesor asignatura');

        Version 14.134:   Sep 17, 2015	Changes in user enrolment.
					Changes in notifications about enrolment. (184823 lines)
					25 changes necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='592';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1456','es','N','Solicitar acept. inscrip. estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1457','es','N','Solicitar acept. inscrip. profesor');
UPDATE actions SET Obsolete='Y' WHERE ActCod='558';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1458','es','N','Aceptar inscrip. como estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1459','es','N','Aceptar inscrip. como profesor');
UPDATE actions SET Obsolete='Y' WHERE ActCod='559';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1460','es','N','Rechazar inscrip. como estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1461','es','N','Rechazar inscrip. como profesor');
UPDATE notif SET NotifyEvent=14 WHERE NotifyEvent=13;
UPDATE notif SET NotifyEvent=13 WHERE NotifyEvent=12;
UPDATE notif SET NotifyEvent=12 WHERE NotifyEvent=11;
UPDATE notif SET NotifyEvent=11 WHERE NotifyEvent=10;
UPDATE notif SET NotifyEvent=10 WHERE NotifyEvent=9;
UPDATE notif SET NotifyEvent=9  WHERE NotifyEvent=8;
UPDATE notif SET NotifyEvent=8  WHERE NotifyEvent=7;
UPDATE sta_notif SET NotifyEvent=14 WHERE NotifyEvent=13;
UPDATE sta_notif SET NotifyEvent=13 WHERE NotifyEvent=12;
UPDATE sta_notif SET NotifyEvent=12 WHERE NotifyEvent=11;
UPDATE sta_notif SET NotifyEvent=11 WHERE NotifyEvent=10;
UPDATE sta_notif SET NotifyEvent=10 WHERE NotifyEvent=9;
UPDATE sta_notif SET NotifyEvent=9  WHERE NotifyEvent=8;
UPDATE sta_notif SET NotifyEvent=8  WHERE NotifyEvent=7;
UPDATE usr_data SET NotifNtfEvents=(((NotifNtfEvents & ~0x7F) << 1) | (NotifNtfEvents & 0x7F) | 0x80);
UPDATE usr_data SET EmailNtfEvents=(((EmailNtfEvents & ~0x7F) << 1) | (EmailNtfEvents & 0x7F));

        Version 14.133.1: Sep 17, 2015	Changes in user enrolment. (184669 lines)
        Version 14.133:   Sep 17, 2015	Changes in user enrolment. (184665 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod IN ('1417','1421','1425');

        Version 14.132.7: Sep 17, 2015	Changes in user enrolment. (184706 lines)
        Version 14.132.6: Sep 17, 2015	Changes in users' IDs. (184640 lines)
					12 changes necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='1239';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1447','es','N','Solicitar edici&oacute;n ID invitado');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1448','es','N','Solicitar edici&oacute;n ID estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1449','es','N','Solicitar edici&oacute;n ID profesor');
UPDATE actions SET Obsolete='Y' WHERE ActCod='1240';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1450','es','N','Eliminar ID invitado');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1451','es','N','Eliminar ID estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1452','es','N','Eliminar ID profesor');
UPDATE actions SET Obsolete='Y' WHERE ActCod='1241';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1453','es','N','Crear ID invitado');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1454','es','N','Crear ID estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1455','es','N','Crear ID profesor');

        Version 14.132.5: Sep 16, 2015	Changes in user enrolment. (184592 lines)
					4 changes necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='440';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1444','es','N','Crear invitado');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1445','es','N','Crear usuario como estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1446','es','N','Crear usuario como profesor');

        Version 14.132.4: Sep 16, 2015	Changes in users' photos. (184536 lines)
					4 changes necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='429';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1441','es','N','Eliminar foto otro usr.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1442','es','N','Eliminar foto estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1443','es','N','Eliminar foto profesor');

        Version 14.132.3: Sep 16, 2015	Changes in users' photos. (184518 lines)
					4 changes necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='374';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1438','es','N','Cambiar foto otro usr.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1439','es','N','Cambiar foto estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1440','es','N','Cambiar foto profesor');

        Version 14.132.2: Sep 16, 2015	Changes in users' photos. (184501 lines)
					4 changes necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='695';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1435','es','N','Detectar rostros foto otro usr.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1436','es','N','Detectar rostros foto estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1437','es','N','Detectar rostros foto profesor');

        Version 14.132.1: Sep 16, 2015	Changes in users' photos. (184458 lines)
        Version 14.132:   Sep 16, 2015	Changes in users' photos. (184416 lines)
					4 changes necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='375';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1432','es','N','Solicitar cambio foto otro usr.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1433','es','N','Solicitar cambio foto estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1434','es','N','Solicitar cambio foto profesor');

        Version 14.131.1: Sep 16, 2015	Changes in user enrolment. (184403 lines)
        Version 14.131:   Sep 15, 2015	Changes in user enrolment. (184408 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1430','es','N','Preguntar si eliminar mi cuenta');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1431','es','N','Eliminar mi cuenta');

        Version 14.130.3: Sep 14, 2015	Changes in user enrolment. (184358 lines)
        Version 14.130.2: Sep 14, 2015	Changes in user enrolment. (184359 lines)
        Version 14.130.1: Sep 14, 2015	Changes and bug fixing in user enrolment. (184416 lines)
					3 changes necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='799';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1428','es','N','Inscribir/eliminar varios estudiantes');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1429','es','N','Inscribir/eliminar varios profesores');

        Version 14.130:   Sep 13, 2015	Changes in user enrolment. (184379 lines)
					3 changes necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='797';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1426','es','N','Administrar varios estudiantes');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1427','es','N','Administrar varios profesores');

        Version 14.129:   Sep 12, 2015	New links to admin a user in list of students.
					Changes in user enrolment.
					Fixed bug in user enrolment. (184411 lines)
					15 changes necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='177';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1414','es','N','Solicitar ID modif. invitado');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1415','es','N','Solicitar ID modif. estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1416','es','N','Solicitar ID modif. profesor');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1417','es','N','Solicitar ID modif. administrador');
UPDATE actions SET Obsolete='Y' WHERE ActCod='161';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1418','es','N','Confirmar modif. invitado');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1419','es','N','Confirmar modif. estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1420','es','N','Confirmar modif. profesor');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1421','es','N','Confirmar modif. administrador');
UPDATE actions SET Obsolete='Y' WHERE ActCod='439';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1422','es','N','Modificar datos invitado');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1423','es','N','Modificar datos estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1424','es','N','Modificar datos profesor');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1425','es','N','Modificar datos administrador');

        Version 14.128.1: Sep 11, 2015	Two new social networks. (184278 lines)
					1 change necessary in database:
ALTER TABLE usr_webs CHANGE Web Web ENUM('www','500px','delicious','deviantart','diaspora','edmodo','facebook','flickr','foursquare','github','gnusocial','googleplus','googlescholar','identica','instagram','linkedin','orcid','paperli','pinterest','quitter','researchgate','researcherid','scoopit','slideshare','storify','tumblr','twitter','wikipedia','youtube') NOT NULL;

        Version 14.128:   Sep 09, 2015	Fixed some bugs in layout.
				        Two new social networks. (184269 lines)
					1 change necessary in database:
ALTER TABLE usr_webs CHANGE Web Web ENUM('www','500px','delicious','deviantart','diaspora','edmodo','facebook','flickr','foursquare','github','googleplus','googlescholar','instagram','linkedin','orcid','paperli','pinterest','quitter','researchgate','researcherid','scoopit','slideshare','storify','tumblr','twitter','wikipedia','youtube') NOT NULL;

        Version 14.127.7: Sep 09, 2015	Fixed some bugs in layout.
					Fixed bug in information about an institution. (184258 lines)
        Version 14.127.6: Sep 06, 2015	Fixed some bugs in layout. (184253 lines)
        Version 14.127.5: Sep 06, 2015	Changes in styles. (184252 lines)
        Version 14.127.4: Sep 06, 2015	Changes in styles. (184245 lines)
        Version 14.127.3: Sep 06, 2015	Changes in styles. (184219 lines)
        Version 14.127.2: Sep 06, 2015	Changes in styles. (184244 lines)
        Version 14.127.1: Sep 06, 2015	Justify alignment is no longer used. (184259 lines)
        Version 14.127:   Sep 05, 2015	Changes in styles. (184273 lines)
        Version 14.126.2: Sep 05, 2015	Changes in styles. (184250 lines)
        Version 14.126.1: Sep 05, 2015	Changes in styles. (184213 lines)
        Version 14.126:   Sep 04, 2015	Changes in styles. (184213 lines)
        Version 14.125.3: Sep 04, 2015	Changes in styles. (184263 lines)
        Version 14.125.2: Sep 04, 2015	Changes in styles. (184344 lines)
        Version 14.125.1: Sep 03, 2015	Changes in styles. (184399 lines)
        Version 14.125:   Sep 03, 2015	Changes in styles. (184521 lines)
        Version 14.124.10:Sep 02, 2015	Changes in styles. (184571 lines)
        Version 14.124.9: Sep 02, 2015	Changes in styles. (184584 lines)
        Version 14.124.8: Sep 02, 2015	Changes in styles. (184635 lines)
        Version 14.124.7: Aug 24, 2015	Changes in styles. (184648 lines)
        Version 14.124.6: Aug 24, 2015	Changes in styles. (184735 lines)
        Version 14.124.5: Aug 06, 2015	Changes in CSS of forms. (184760 lines)
        Version 14.124.4: Aug 06, 2015	Fixed bug in function to draw institution / centre / degree logo.
					Changes in CSS of forms. (184781 lines)
        Version 14.124.3: Aug 06, 2015	Changes in CSS of forms.
                                        Changes in function to draw institution / centre / degree logo. (184782 lines)
        Version 14.124.2: Aug 03, 2015	Changes in CSS of forms. (184768 lines)
        Version 14.124.1: Aug 03, 2015	Changes in CSS of forms.
					Fixed bug in edition of institutions. (184782 lines)
        Version 14.124:   Aug 03, 2015	Changes in CSS of forms.
                                        New layout to select source of course info. (184799 lines)
        Version 14.123.8: Aug 03, 2015	Changes in CSS of forms. (184801 lines)
        Version 14.123.7: Aug 01, 2015	Changes in CSS of forms. (184848 lines)
        Version 14.123.6: Aug 01, 2015	Changes in CSS of forms. (184874 lines)
        Version 14.123.5: Jul 31, 2015	Changes in CSS of forms. (184902 lines)
        Version 14.123.4: Jul 31, 2015	Changes in CSS of forms. (184905 lines)
        Version 14.123.3: Jul 28, 2015	Changes in CSS of forms. (184929 lines)
        Version 14.123.2: Jul 28, 2015	Changes in CSS of forms. (184953 lines)
        Version 14.123.1: Jul 28, 2015	Changes in CSS of forms. (184984 lines)
        Version 14.123:   Jul 28, 2015	Changes in CSS of forms. (184877 lines)
        Version 14.122:   Jul 27, 2015	Changes in CSS of forms. (184892 lines)
        Version 14.121:   Jul 27, 2015	Changes in CSS of forms. (184890 lines)
        Version 14.120.9: Jul 27, 2015	Changes in CSS of forms. (184855 lines)
        Version 14.120.8: Jul 26, 2015	Changes in CSS of assignments. (184881 lines)
        Version 14.120.7: Jul 26, 2015	Changes in CSS of forms.
                                        Changes in CSS of surveys. (184877 lines)
        Version 14.120.6: Jul 26, 2015	Changes in CSS of forms. (184889 lines)
        Version 14.120.5: Jul 26, 2015	Changes in CSS of forms. (184924 lines)
        Version 14.120.4: Jul 25, 2015	Changes in CSS of forms. (184937 lines)
        Version 14.120.3: Jul 25, 2015	Changes in selectors of country, institution, centre, degree and course. (184893 lines)
        Version 14.120.2: Jul 25, 2015	Changes in CSS of forms.
                                        Fixed bug in selectors of country, institution, centre and degree. (184819 lines)
        Version 14.120.1: Jul 25, 2015	Changes in CSS of forms.
                                        Update connected users when change scope selection. (184856 lines)
        Version 14.120:   Jul 25, 2015	Changes in CSS of forms. (184879 lines)
        Version 14.119.5: Jul 24, 2015	Changes in CSS of lists. (184902 lines)
        Version 14.119.4: Jul 24, 2015	New layout of institutional links as a list. (184872 lines)
        Version 14.119.3: Jul 24, 2015	New layout of MFU actions as a list. (184875 lines)
        Version 14.119.2: Jul 22, 2015	Changes in page title and platform name. (184880 lines)
        Version 14.119.1: Jul 22, 2015	Undo some changes in file browsers. (184883 lines)
        Version 14.119:   Jul 22, 2015	Function to put an icon to remove. (184828 lines)
        Version 14.118:   Jul 22, 2015	Function to put a disabled icon indicating that removal is not allowed. (185361 lines)
        Version 14.117.17:Jul 22, 2015	Checking "alt" and "title" in all images. (185437 lines)
        Version 14.117.16:Jul 22, 2015	Checking "alt" and "title" in all images. (185368 lines)
        Version 14.117.15:Jul 22, 2015	Checking "alt" and "title" in all images. (185279 lines)
        Version 14.117.14:Jul 22, 2015	Checking "alt" and "title" in all images. (185203 lines)
        Version 14.117.13:Jul 22, 2015	Checking "alt" and "title" in all images.
                                        Removed unused function about mail domains. (185153 lines)
        Version 14.117.12:Jul 22, 2015	Checking "alt" and "title" in all images. (185213 lines)
        Version 14.117.11:Jul 22, 2015	Checking "alt" and "title" in all images. (185187 lines)
        Version 14.117.10:Jul 21, 2015	Checking "alt" and "title" in all images. (185157 lines)
        Version 14.117.9: Jul 21, 2015	Checking "alt" and "title" in all images. (185157 lines)
        Version 14.117.8: Jul 21, 2015	Checking "alt" and "title" in all images. (185027 lines)
        Version 14.117.7: Jul 21, 2015	Checking "alt" and "title" in all images. (185008 lines)
        Version 14.117.6: Jul 21, 2015	Checking "alt" and "title" in all images.
                                        Changes in most frequent actions. (184948 lines)
        Version 14.117.5: Jul 21, 2015	Fixed HTML bug in account form. (184930 lines)
        Version 14.117.4: Jul 17, 2015	Fixed bug in course record, reported by Antonio Pe�a Freire. (184929 lines)
        Version 14.117.3: Jul 17, 2015	Fixed recent bug in cache control. (184893 lines)
        Version 14.117.2: Jul 16, 2015	Changes in cache control. (184893 lines)
        Version 14.117.1: Jul 15, 2015	Fixed minor bug when showing user's photo. (184892 lines)
        Version 14.117:   Jul 15, 2015	User's ID is no longer mandatory. (184885 lines)
        Version 14.116.1: Jul 14, 2015	Changes in some messages. (184850 lines)
        Version 14.116:   Jul 13, 2015	New function createAccount in web service. (184849 lines)
        Version 14.115.2: Jul 07, 2015	Fixed bug in search of open documents, reported by Javier S�nchez Lirola. (184769 lines)
        Version 14.115.1: Jun 23, 2015	Fixed bugs in web service function getTrivialQuestion. (184768 lines)
        Version 14.115:   Jun 16, 2015	New function getTrivialQuestion in web service. (184747 lines)
        Version 14.114:   Jun 15, 2015	Fixed bug in tests query (some questions were repeated in exams). (184550 lines)
        Version 14.113.5: Jun 01, 2015	Changes in sign up form. (184541 lines)
        Version 14.113.4: Apr 25, 2015	Fixed bug in layout. (184537 lines)
        Version 14.113.3: Apr 15, 2015	New welcome message after creating an account. (184536 lines)
        Version 14.113.2: Apr 14, 2015	Fixed bug when compressing users' works. (184486 lines)
        Version 14.113.1: Apr 14, 2015	Fixed bug when compressing users' works, reported by Jos� Mart�nez Aroza. (184484 lines)
        Version 14.113:   Apr 12, 2015	Refactoring code related to frames. (184462 lines)
        Version 14.112.1: Apr 12, 2015	Refactoring code related to frames with ending buttons. (184583 lines)
        Version 14.112:   Apr 12, 2015	Optimization in frames with ending buttons.
					Changes in form to upload logo.
					Changes in form to photo of centre.
					Changes in edition of course timetable. (184572 lines)
        Version 14.111.1: Apr 11, 2015	Message translated. (184745 lines)
        Version 14.111:   Apr 11, 2015	Changes in layout of surveys.
					Fixed bugs in surveys.
					Changes in layout of edition of test questions.
					Changes in layout of users' attendance. (184723 lines)
        Version 14.110.13:Apr 11, 2015	Changes in layout of users' attendance. (184750 lines)
        Version 14.110.12:Apr 11, 2015	Changes in layout of users' works. (184728 lines)
        Version 14.110.11:Apr 11, 2015	Changes in layout of selection of class photo / list. (184722 lines)
        Version 14.110.10:Apr 11, 2015	Changes in layout of selection of groups. (184725 lines)
        Version 14.110.9: Apr 11, 2015	Changes in layout of edition of a new assignment. (184723 lines)
        Version 14.110.8: Apr 11, 2015	Help on writing forum posts.
                                        Changes in layout of edition of a new forum post. (184718 lines)
        Version 14.110.7: Apr 11, 2015	Help on writing messages.
                                        Changes in layout of edition of a new message. (184719 lines)
        Version 14.110.6: Apr 11, 2015	Help on edition of test questions.
					Changes in layout of edition of test questions. (184702 lines)
        Version 14.110.5: Apr 11, 2015	Fixed bug in tests. (184685 lines)
        Version 14.110.4: Apr 11, 2015	Changes in MathJax configuration. (184685 lines)
        Version 14.110.3: Apr 09, 2015	Changes in conversion from Markdown to HTML5 using pandoc. (184599 lines)
        Version 14.110.2: Apr 09, 2015	Changes in CSS and scripts.
					MathJax installed in local by default. (184555 lines)
					1 change in installation:
It's necessary to install a copy of MathJax in the server at directory MathJax inside main html directory followin the instructions in http://docs.mathjax.org/en/latest/start.html

        Version 14.110.1: Apr 08, 2015	Changes in CSS. (184539 lines)
        Version 14.110:   Apr 07, 2015	Testing rich text editor using Markdown, suggested by Pedro A. Garc�a S�nchez. (184508 lines)
					Fixed bug in file browser, reported by Eva Mart�nez Ortigosa.
					2 changes necessary in database:
ALTER TABLE crs_info_txt CHANGE COLUMN InfoTxt InfoTxtHTML LONGTEXT NOT NULL;
ALTER TABLE crs_info_txt ADD COLUMN InfoTxtMD LONGTEXT NOT NULL AFTER InfoTxtHTML;
					1 change in installation:
It's necessary to install pandoc 1.10 or later.

        Version 14.109.2: Apr 06, 2015	Changed text in file browser. (184347 lines)
        Version 14.109.1: Apr 06, 2015	Changed text of button in course info. (184325 lines)
        Version 14.109:   Apr 06, 2015	Links in file browsers can be uploaded with a name. (184324 lines)
        Version 14.108.4: Apr 04, 2015	Changes in statistic about institutions. (184267 lines)
        Version 14.108.3: Apr 03, 2015	Fixed bug in statistic about institutions. (184144 lines)
        Version 14.108.2: Apr 03, 2015	Fixed bug in statistic about institutions. (184142 lines)
        Version 14.108.1: Apr 03, 2015	Fixed bug in statistic about institutions. (184137 lines)
        Version 14.108:   Apr 03, 2015	New statistic (figure) about institutions. (184136 lines)
        Version 14.107.8: Apr 03, 2015	Changes in statistics (figures about hierarchy). (183774 lines)
        Version 14.107.7: Apr 03, 2015	Changes in layout of sent / received messages. (183753 lines)
        Version 14.107.6: Apr 03, 2015	Changes in layout of form to change password. (183748 lines)
        Version 14.107.5: Apr 03, 2015	Changes in layout of form to change photo. (183718 lines)
        Version 14.107.4: Apr 03, 2015	Fixed bugs when changing user's ID, password and photo. (183699 lines)
        Version 14.107.3: Apr 02, 2015	Changes in icons in attendance events.
					Fixed bug in assignments. (183702 lines)
        Version 14.107.2: Apr 02, 2015	Changes in icons in surveys. (183717 lines)
        Version 14.107.1: Apr 02, 2015	Changes in icons in assignments. (183748 lines)
        Version 14.107:   Apr 02, 2015	Refactoring contextual menus. (183776 lines)
        Version 14.106.2: Apr 02, 2015	Refactoring some contextual menus. (183747 lines)
        Version 14.106.1: Apr 02, 2015	Changes in layout of icon with links. (183925 lines)
        Version 14.106:   Apr 02, 2015	Lot of changes in layout of icon with links. (183909 lines)
        Version 14.105.3: Apr 01, 2015	Changes in layout of course configuration and exam announcements. (183912 lines)
        Version 14.105.2: Mar 30, 2015	Some messages translated. (183941 lines)
        Version 14.105.1: Mar 30, 2015	Shared record card is shown after updating course record card. (183937 lines)
        Version 14.105:   Mar 30, 2015	Removed option in the assessment tab to view course card. (183932 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='299';

        Version 14.104.6: Mar 30, 2015	Changes in course cards. (184030 lines)
        Version 14.104.5: Mar 29, 2015	Changes in course cards. (184023 lines)
        Version 14.104.4: Mar 29, 2015	Changes in layout of record cards. (184047 lines)
        Version 14.104.3: Mar 29, 2015	Changes in layout of course record cards. (184048 lines)
        Version 14.104.2: Mar 29, 2015	Changes in layout of record cards. (184042 lines)
        Version 14.104.1: Mar 29, 2015	Changes in layout of record cards. (184039 lines)
        Version 14.104:   Mar 29, 2015	Changes in layout of record cards. (184021 lines)
        Version 14.103:   Mar 29, 2015	New table to ban users out of ranking. (184014 lines)
					1 change necessary in database:
CREATE TABLE IF NOT EXISTS usr_banned (UsrCod INT NOT NULL,UNIQUE INDEX(UsrCod));

        Version 14.102.12:Mar 28, 2015	Some help messages changed. (183953 lines)
        Version 14.102.11:Mar 28, 2015	Some help messages changed. (183943 lines)
        Version 14.102.10:Mar 28, 2015	Create user's figures when a user account is created. (183942 lines)
        Version 14.102.9: Mar 28, 2015	Changes in help messages while filling in user's record card. (183927 lines)
        Version 14.102.8: Mar 27, 2015	Fixed bug in form to edit account. (183989 lines)
        Version 14.102.7: Mar 27, 2015	Minor change in help text. (183988 lines)
        Version 14.102.6: Mar 27, 2015	Changes in layout of sign up. (183987 lines)
        Version 14.102.5: Mar 27, 2015	Changes in layout of log in. (183909 lines)
        Version 14.102.4: Mar 25, 2015	Fixed bug in ranking. (183883 lines)
        Version 14.102.3: Mar 25, 2015	Fixed bug in record. (183882 lines)
        Version 14.102.2: Mar 25, 2015	Fixed bug in record. (183882 lines)
        Version 14.102.1: Mar 24, 2015	Fixed minor bugs. (183871 lines)
        Version 14.102:   Mar 24, 2015	Changes in some update buttons. (183865 lines)
        Version 14.101:   Mar 24, 2015	Fixed bug in form to create a new account.
					Fixed bug in statistics.
					New design of buttons. (183753 lines)
        Version 14.100.3: Mar 24, 2015	Fixed bug in link to institution in record. (183753 lines)
        Version 14.100.2: Mar 24, 2015	Changes in profile.
					Link to institution in record. (183752 lines)
        Version 14.100.1: Mar 24, 2015	Only font-awesome icons for notifications. (183724 lines)
        Version 14.100:   Mar 23, 2015	Notification when a user gets a new follower. (183723 lines)
					1 change necessary in database:
UPDATE usr_data SET NotifNtfEvents=(NotifNtfEvents | 0x2000) WHERE NotifNtfEvents<>0;

        Version 14.99.2:  Mar 23, 2015	Ranking attending to number of followers. (183614 lines)
        Version 14.99.1:  Mar 21, 2015	Change in file_view to accelerate queries. (183521 lines)
					1 change necessary in database:
CREATE INDEX UsrCod ON file_view (UsrCod);

        Version 14.99:    Mar 21, 2015	Icons instead of text in user's profile.
					Lot of changes in layout of user's profile. (183519 lines)
        Version 14.98.5:  Mar 21, 2015	New icons for some options. (183431 lines)
        Version 14.98.4:  Mar 20, 2015	Fixed bugs in marks. (183430 lines)
        Version 14.98.3:  Mar 19, 2015	Changes in user's profile to speedup page generation. (183397 lines)
        Version 14.98.2:  Mar 19, 2015	Fixed bug in record related to follow/unfollow. (183366 lines)
        Version 14.98.1:  Mar 19, 2015	Changes in lists of following and followers. (183360 lines)
        Version 14.98:    Mar 19, 2015	Show lists of following and followers. (183300 lines)
        Version 14.97:    Mar 18, 2015	Show number of following and followers in user's profile. (183130 lines)
        Version 14.96.3:  Mar 18, 2015	Changes in record cards. (183017 lines)
        Version 14.96.2:  Mar 18, 2015	Changes in record cards. (183011 lines)
        Version 14.96.1:  Mar 18, 2015	Changes in user's profile. (183015 lines)
        Version 14.96:    Mar 18, 2015	New layout in record cards. (183011 lines)
        Version 14.95.1:  Mar 18, 2015	Show user's profile after followin or unfollowing. (182985 lines)
        Version 14.95:    Mar 18, 2015	New module swad_follow for follow users. (182903 lines)
					1 change necessary in Makefile:
Add swad_follow.o to list of object files
					3 changes necessary in database:
CREATE TABLE IF NOT EXISTS usr_follow (FollowerCod INT NOT NULL,FollowedCod INT NOT NULL,FollowTime DATETIME NOT NULL,UNIQUE INDEX (FollowerCod,FollowedCod),UNIQUE INDEX (FollowedCod,FollowerCod),INDEX (FollowTime));
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1410','es','N','Seguir a un usuario');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1411','es','N','Dejar de seguir a un usuario');

        Version 14.94.4:  Mar 17, 2015	Changes in ranking layout. (182679 lines)
        Version 14.94.3:  Mar 17, 2015	Changes in record fonts. (182667 lines)
        Version 14.94.2:  Mar 17, 2015	Fixed bug in ranking. (182668 lines)
        Version 14.94.1:  Mar 17, 2015	Fixed bug in user's profile. (182667 lines)
        Version 14.94:    Mar 17, 2015	Ranking for downloads, forum posts and messages in user's profile. (182666 lines)
        Version 14.93.4:  Mar 17, 2015	Links in user's figures to ranking. (182661 lines)
        Version 14.93.3:  Mar 16, 2015	Changes in rankings. (182630 lines)
        Version 14.93.2:  Mar 16, 2015	New rankings for downloads, forum posts and messages sent. (182628 lines)
        Version 14.93.1:  Mar 16, 2015	Photos and links to users' profiles in users' ranking. (182560 lines)
        Version 14.93:    Mar 16, 2015	Users' ranking. (182527 lines)
        Version 14.92.2:  Mar 15, 2015	Fixed minor bug in photo edition. (182208 lines)
        Version 14.92.1:  Mar 14, 2015	Changes on request of a nickname to view user's profile. (182205 lines)
        Version 14.92:    Mar 14, 2015	New module swad_profile for public user's profile. (182202 lines)
					1 change necessary in Makefile:
Add swad_profile.o to list of object files

        Version 14.91.3:  Mar 14, 2015	By default, the logged user's nickname is shown in form to view user's profile
					Action to view QR code is now public. (182108 lines)
        Version 14.91.2:  Mar 14, 2015	URL of public profile is embedded under user's photo in record card.
					Changes in layout of record card. (182124 lines)
        Version 14.91.1:  Mar 14, 2015	Current figures separated from historic figures in layout of public user profile.
					Show only number of files, not folder or links. (182112 lines)
        Version 14.91:    Mar 14, 2015	Statistics for number of file views added to user figures. (182091 lines)
					2 changes necessary in database:
ALTER TABLE usr_figures ADD COLUMN NumFileViews INT NOT NULL DEFAULT -1 AFTER NumClicks;
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1409','es','N','Calcular n&uacute;mero de descargas de archivos');

        Version 14.90.4:  Mar 13, 2015	Changes in layout of public user profile. (181885 lines)
        Version 14.90.3:  Mar 13, 2015	Fixed bug in connected users when the scope is a country. (181884 lines)
        Version 14.90.2:  Mar 13, 2015	Fixed minor bug in layout of user's figures. (181831 lines)
        Version 14.90.1:  Mar 13, 2015	Fixed bug in form used to upload files. (181827 lines)
        Version 14.90:    Mar 13, 2015	Animated gif icon when calculating user's figures.
					Changes in all submit links. (181826 lines)
        Version 14.89.1:  Mar 13, 2015	Optimization in database query for first click time. (181771 lines)
        Version 14.89:    Mar 13, 2015	Lots of changes related with forms and photos. (181769 lines)
        Version 14.88:    Mar 12, 2015	Changes in layout of public user profile. (181718 lines)
        Version 14.87.4:  Mar 11, 2015	Added indexes to usr_figures. (181629 lines)
					2 changes necessary in database:
CREATE INDEX FirstClickTime ON usr_figures (FirstClickTime);
CREATE INDEX NumClicks ON usr_figures (NumClicks);

        Version 14.87.3:  Mar 11, 2015	Fix bug comparing first click dates in user's figures. (181623 lines)
        Version 14.87.2:  Mar 11, 2015	Fix bug comparing first click dates in user's figures. (181614 lines)
        Version 14.87.1:  Mar 11, 2015	Fix style in CSS and record. (181613 lines)
        Version 14.87:    Mar 11, 2015	Ranking according to the number of clicks. (181611 lines)
        Version 14.86:    Mar 11, 2015	Show user's figures per days. (181525 lines)
        Version 14.85.2:  Mar 11, 2015	Refactoring in user's figures. (181478 lines)
        Version 14.85.1:  Mar 11, 2015	Remove user's figures when removing user's account. (181472 lines)
        Version 14.85:    Mar 11, 2015	Number of messages sent is stored in user's figures. (181457 lines)
					2 changes necessary in database:
ALTER TABLE usr_figures ADD COLUMN NumForPst INT NOT NULL DEFAULT -1 AFTER NumClicks;
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1408','es','N','Calcular n&uacute;mero de mensajes en foros');

        Version 14.84:    Mar 10, 2015	Number of messages sent is stored in user's figures. (181374 lines)
					2 changes necessary in database:
ALTER TABLE usr_figures ADD COLUMN NumMsgSnt INT NOT NULL DEFAULT -1 AFTER NumClicks;
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1407','es','N','Calcular n&uacute;mero de mensajes enviados');

        Version 14.83:    Mar 10, 2015	New action to compute first click when not available in public profile. (181287 lines)
					3 changes necessary in database:
ALTER TABLE usr_figures CHANGE COLUMN NumClicks NumClicks INT NOT NULL DEFAULT -1;
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1405','es','N','Calcular fecha del primer clic');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1406','es','N','Calcular n&uacute;mero de clics');

        Version 14.82.1:  Mar 10, 2015	Added new social networks. (181143 lines)
					1 change necessary in database:
ALTER TABLE usr_webs CHANGE Web Web ENUM('www','500px','delicious','deviantart','diaspora','edmodo','facebook','flickr','foursquare','github','googleplus','googlescholar','instagram','linkedin','paperli','pinterest','quitter','researchgate','scoopit','slideshare','storify','tumblr','twitter','wikipedia','youtube') NOT NULL;

        Version 14.82:    Mar 10, 2015	New database table usr_figures. (179898 lines)
					1 change necessary in database:
CREATE TABLE IF NOT EXISTS usr_figures (UsrCod INT NOT NULL,FirstClickTime DATETIME NOT NULL,NumClicks INT NOT NULL DEFAULT 0,PRIMARY KEY(UsrCod));

        Version 14.81.4:  Mar 09, 2015	Information about number of public files in user's public profile. (179733 lines)
        Version 14.81.3:  Mar 09, 2015	Information about number of files in user's public profile. (179670 lines)
        Version 14.81.2:  Mar 09, 2015	Information about number of courses in user's public profile. (179641 lines)
        Version 14.81.1:  Mar 09, 2015	Information about number of messages sent in user's public profile. (179545 lines)
        Version 14.81:    Mar 09, 2015	Information about number of forum posts in user's public profile. (179512 lines)
        Version 14.80.19: Mar 09, 2015	Fixed bug in web service, reported by Juan Miguel Boyero Corral. (179482 lines)
        Version 14.80.18: Mar 09, 2015	Fixed bug in statistic of degrees and courses. (179481 lines)
        Version 14.80.17: Mar 09, 2015	Scope country for statistic of users. (179480 lines)
        Version 14.80.16: Mar 09, 2015	Scope country for statistic of degrees and courses. (179439 lines)
        Version 14.80.15: Mar 09, 2015	Scope country for statistic of webs / social networks. (179393 lines)
        Version 14.80.14: Mar 09, 2015	Scope country for statistic of OERs. (179379 lines)
        Version 14.80.13: Mar 09, 2015	Scope country for statistic of assignments. (179362 lines)
        Version 14.80.12: Mar 09, 2015	Scope country for statistic of tests. (179340 lines)
        Version 14.80.11: Mar 09, 2015	Scope country for statistic of notifications. (179267 lines)
        Version 14.80.10: Mar 09, 2015	Scope country for statistic of notices. (179211 lines)
        Version 14.80.9:  Mar 09, 2015	Scope country for statistic of messages. (179187 lines)
        Version 14.80.8:  Mar 09, 2015	Scope country for statistic of forums. (179138 lines)
        Version 14.80.7:  Mar 09, 2015	Scope country for statistic of surveys. (179021 lines)
        Version 14.80.6:  Mar 09, 2015	Scope country for statistic of columns. (178987 lines)
        Version 14.80.5:  Mar 09, 2015	Scope country for statistic of menu. (178974 lines)
        Version 14.80.4:  Mar 09, 2015	Scope country for statistic of icon set. (178961 lines)
        Version 14.80.3:  Mar 09, 2015	Scope country for statistic of theme. (178948 lines)
        Version 14.80.2:  Mar 09, 2015	Scope country for statistic of layout. (178936 lines)
        Version 14.80.1:  Mar 09, 2015	Scope country for statistic of language. (178923 lines)
        Version 14.80:    Mar 09, 2015	New statistic for privacy. (178909 lines)
        Version 14.79.6:  Mar 08, 2015	Check if file "swad.lock" exists to disable SWAD. (178736 lines)
        Version 14.79.5:  Mar 08, 2015	Fixed bug when refreshing connected users. (178730 lines)
        Version 14.79.4:  Mar 07, 2015	Parameter "CtyCod" is renamed to "cty".
					Parameter "InsCod" is renamed to "ins".
					Parameter "CtrCod" is renamed to "ctr".
					Parameter "DegCod" is renamed to "deg".
					Parameter "CrsCod" is renamed to "crs", but "CrsCod" is allowed for compatibility with old links.
					Parameter "Usr" is renamed to "usr".
					Parameter "Layout" is not allowed with GET method.
					Parameter "IdSes" is renamed to "ses".
					Parameter "MailKey" is renamed to "key". (178699 lines)
        Version 14.79.3:  Mar 07, 2015	Parameter "UsrNick" is renamed to "Usr".
					URL is shown in public profile.
					Language in direct URLs. (178677 lines)
        Version 14.79.2:  Mar 06, 2015	Public profile is shown depending on visibility. (178759 lines)
        Version 14.79.1:  Mar 06, 2015	Add options to change visibility of public profile. (178769 lines)
					3 changes necessary in database:
ALTER TABLE usr_data ADD COLUMN ProfileVisibility ENUM('user','course','system','world') NOT NULL DEFAULT 'user' AFTER PhotoVisibility;
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1403','es','N','Editar privacidad');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1404','es','N','Cambiar privacidad perfil p&uacute;blico');

        Version 14.79:    Mar 06, 2015	New module swad_privacy. (178678 lines)
					1 change necessary in Makefile:
Add swad_privacy.o to list of object files

        Version 14.78.3:  Mar 06, 2015	Changes in messages about privacity. (178631 lines)
        Version 14.78.2:  Mar 06, 2015	New action to edit privacy options. (178630 lines)
        Version 14.78.1:  Mar 06, 2015	New link to privacy options (unfinished). (178595 lines)
        Version 14.78:    Mar 06, 2015	Four degrees of visibility in photos.
					Field PublicPhoto is replaced by field PhotoVisibility in usr_data table. (178577 lines)
					4 changes necessary in database:
ALTER TABLE usr_data ADD COLUMN PhotoVisibility ENUM('user','course','system','world') NOT NULL DEFAULT 'user' AFTER PublicPhoto;
UPDATE usr_data SET PhotoVisibility='user' WHERE PublicPhoto='N';
UPDATE usr_data SET PhotoVisibility='system' WHERE PublicPhoto='Y';
ALTER TABLE usr_data DROP COLUMN PublicPhoto;

        Version 14.77.3:  Mar 05, 2015	Changes in shared record. (178509 lines)
        Version 14.77.2:  Mar 05, 2015	Changes in shared record. (178504 lines)
        Version 14.77.1:  Mar 05, 2015	New option to select the user and then go to his/her public user's profile. (178475 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1401','es','N','Solicitar perfil de usuario');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1402','es','N','Ver perfil de usuario');

        Version 14.77:    Mar 04, 2015	New parameter "UsrNick" to go to a public user's profile. (178406 lines)
        Version 14.76.5:  Mar 03, 2015	Added new social networks, suggested by Marta G�mez Mac�as. (178293 lines)
					1 change necessary in database:
ALTER TABLE usr_webs CHANGE Web Web ENUM('www','500px','delicious','deviantart','edmodo','facebook','flickr','foursquare','github','googleplus','googlescholar','instagram','linkedin','paperli','pinterest','researchgate','scoopit','slideshare','storify','tumblr','twitter','wikipedia','youtube') NOT NULL;

        Version 14.76.4:  Mar 03, 2015	Nickname is always visible on record. (178270 lines)
        Version 14.76.3:  Mar 03, 2015	Fixed bug while sending a message to a guest. (178274 lines)
        Version 14.76.2:  Mar 02, 2015	Button in student record to view student's attendance. (178242 lines)
        Version 14.76.1:  Mar 02, 2015	Button in student record to view student's test exams. (178225 lines)
        Version 14.76:    Mar 02, 2015	Button in student record to view student's works. (178197 lines)
        Version 14.75.2:  Mar 02, 2015	New icon for new notifications. (178176 lines)
        Version 14.75.1:  Mar 02, 2015	Fixed bug in file browser, reported by Germ�n Luz�n Gonz�lez and Javier Fern�ndez Baldomero. (178175 lines)
        Version 14.75:    Feb 27, 2015	Fixed bug when getting user's IDs from marks tables. (178172 lines)
        Version 14.74.12: Feb 23, 2015	Fixed bug in file browser, reported by Germ�n Luz�n Gonz�lez and Javier Fern�ndez Baldomero. (178119 lines)
        Version 14.74.11: Feb 10, 2015	Changes in swad.js. (178060 lines)
        Version 14.74.10: Feb 10, 2015	Removed meta tag. (178060 lines)
        Version 14.74.9:  Feb 10, 2015	Description/keywords meta tags and canonical URL added to head. (178060 lines)
        Version 14.74.8:  Feb 10, 2015	Fixed bug in degree logo. (178045 lines)
        Version 14.74.7:  Feb 08, 2015	Link to register/remove several users. (178029 lines)
        Version 14.74.6:  Feb 08, 2015	Changes in layout of form to register/remove one user. (178009 lines)
        Version 14.74.5:  Feb 08, 2015	Changes in layout of form to register/remove users. (178038 lines)
        Version 14.74.4:  Feb 08, 2015	Changes in layout of form to register/remove users. (178000 lines)
        Version 14.74.3:  Feb 08, 2015	Button to view banners.
					Button to view mail domains.
					Button to view institutional links.
					Button to view plugins.
					Button to view departments.
					Button to view holidays.
					Button to view places. (178027 lines)
        Version 14.74.2:  Feb 08, 2015	Fixed bug in banners. (177994 lines)
        Version 14.74.1:  Feb 06, 2015	Fixed bug in shared files. (177986 lines)
        Version 14.74:    Feb 05, 2015	Shared files for institution, centre and degree. (177982 lines)
        Version 14.73.5:  Feb 05, 2015	New message when user does not belong to any course. (177170 lines)
        Version 14.73.4:  Feb 04, 2015	Changes in institution configuration. (177103 lines)
        Version 14.73.3:  Feb 04, 2015	Changes in degree configuration. (177080 lines)
        Version 14.73.2:  Feb 04, 2015	Changes in centre configuration. (177057 lines)
        Version 14.73.1:  Feb 04, 2015	Fixed bug in logo of centre. (177012 lines)
        Version 14.73:    Feb 03, 2015	New options to remove institution, centre and degree icons. (177007 lines)
					3 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1341','es','N','Eliminar logo de instituci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1342','es','N','Eliminar logo de centro');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1343','es','N','Eliminar logo de titulaci&oacute;n');

        Version 14.72.6:  Feb 03, 2015	Changes in icons for degrees and centres. (176883 lines)
        Version 14.72.5:  Feb 01, 2015	Changed related to administrators. (176805 lines)
        Version 14.72.4:  Feb 01, 2015	Refactoring of registering and removing admins. (176787 lines)
        Version 14.72.3:  Feb 01, 2015	Refactoring of registering and removing admins. (177139 lines)
        Version 14.72.2:  Feb 01, 2015	Refactoring of registering and removing admins. (177496 lines)
        Version 14.72.1:  Feb 01, 2015	New options to register and remove institution and centre admins. (177477 lines)
					6 changes necessary in database:
UPDATE actions SET Txt='A&ntilde;adir administrador de titulaci&oacute;n' WHERE ActCod='586' AND Language='es';
UPDATE actions SET Txt='Eliminar administrador de titulaci&oacute;n' WHERE ActCod='584' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1337','es','N','A&ntilde;adir administrador de instituci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1338','es','N','Eliminar administrador de instituci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1339','es','N','A&ntilde;adir administrador de centro');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1340','es','N','Eliminar administrador de centro');

        Version 14.72:    Jan 31, 2015	New options to register and remove institution and centre admins (not finished). (176934 lines)
        Version 14.71:    Jan 29, 2015	New table admin to store system, institution, centre or degree admins. (176255 lines)
					4 changes necessary in database:
CREATE TABLE IF NOT EXISTS admin (UsrCod INT NOT NULL,Scope ENUM('Sys','Ins','Ctr','Deg') NOT NULL,Cod INT NOT NULL,UNIQUE INDEX(UsrCod,Scope,Cod));
INSERT INTO admin (UsrCod,Scope,Cod) SELECT UsrCod,'Deg',DegCod FROM deg_admin WHERE DegCod>'0';
INSERT INTO admin (UsrCod,Scope,Cod) SELECT UsrCod,'Sys',DegCod FROM deg_admin WHERE DegCod='-2';
DROP TABLE IF EXISTS deg_admin;

        Version 14.70.6:  Jan 28, 2015	Buttons to view list of institutions, centres, degrees and courses in information options. (176242 lines)
        Version 14.70.5:  Jan 28, 2015	Links in list of countries, institutions, centres and degrees now go to information options. (176219 lines)
        Version 14.70.4:  Jan 28, 2015	Links in breadcrumbs now go to information options. (176218 lines)
        Version 14.70.3:  Jan 28, 2015	Some button messages changed.
					Buttons to view when editing. (176217 lines)
        Version 14.70.2:  Jan 27, 2015	Changed permissions in documents zones of institution, centre and degree. (176188 lines)
        Version 14.70.1:  Jan 27, 2015	Fixed bug when removing a user from a course. (176187 lines)
        Version 14.70:    Jan 26, 2015	Fixed bugs in statistics about sizes of file browsers.
                                        Statistics about sizes of group zones.
                                        New scope (country) in statistics about sizes of file browsers. (176181 lines)
        Version 14.69.2:  Jan 26, 2015	Fixed bugs in statistics about sizes of file browsers. (175864 lines)
        Version 14.69.1:  Jan 26, 2015	Code refactoring related to file browsers. (175822 lines)
        Version 14.69:    Jan 26, 2015	Changes in search of documents. (175818 lines)
        Version 14.68.2:  Jan 25, 2015	Fixed bugs in search of documents.
					Search of user's documents now extended to institution, centre and degree. (175644 lines)
        Version 14.68.1:  Jan 25, 2015	Changes in codes stores in expanded_folders table. (175618 lines)
					7 changes necessary in database:
UPDATE expanded_folders SET FileBrowser='3' WHERE FileBrowser='1';
UPDATE expanded_folders SET FileBrowser='8' WHERE FileBrowser='2';
UPDATE expanded_folders SET FileBrowser='11' WHERE FileBrowser='10';
UPDATE expanded_folders SET FileBrowser='13' WHERE FileBrowser='12';
UPDATE expanded_folders SET FileBrowser='17' WHERE FileBrowser='16';
UPDATE expanded_folders SET FileBrowser='19' WHERE FileBrowser='18';
UPDATE expanded_folders SET FileBrowser='21' WHERE FileBrowser='20';

        Version 14.68:    Jan 25, 2015	Changes in file_browser_size table. (175606 lines)
					9 changes necessary in database:
DROP INDEX FileBrowser ON file_browser_size;
DROP INDEX UsrCod ON file_browser_size;
ALTER TABLE file_browser_size CHANGE COLUMN UsrCod ZoneUsrCod INT NOT NULL DEFAULT -1,ADD INDEX (ZoneUsrCod);
ALTER TABLE file_browser_size ADD COLUMN Cod INT NOT NULL DEFAULT -1 AFTER FileBrowser;
UPDATE file_browser_size SET Cod=CrsCod WHERE CrsCod>'0' AND GrpCod<='0';
UPDATE file_browser_size SET Cod=GrpCod WHERE GrpCod>'0';
ALTER TABLE file_browser_size DROP COLUMN CrsCod;
ALTER TABLE file_browser_size DROP COLUMN GrpCod;
CREATE UNIQUE INDEX FileBrowser ON file_browser_size (FileBrowser,Cod,ZoneUsrCod);

        Version 14.67.1:  Jan 25, 2015	Fixed bugs in file browsers. (175395 lines)
        Version 14.67:    Jan 25, 2015	Changes in files table. (175369 lines)
					17 changes necessary in database:
ALTER TABLE files ADD COLUMN ZoneUsrCod2 INT NOT NULL DEFAULT -1 AFTER FileBrowser;
UPDATE files SET ZoneUsrCod2=ZoneUsrCod;
ALTER TABLE files DROP COLUMN ZoneUsrCod;
ALTER TABLE files CHANGE COLUMN ZoneUsrCod2 ZoneUsrCod INT NOT NULL DEFAULT -1;
ALTER TABLE files ADD COLUMN Cod INT NOT NULL DEFAULT -1 AFTER FileBrowser, ADD INDEX (FileBrowser,Cod,ZoneUsrCod);
CREATE INDEX ZoneUsrCod ON files (ZoneUsrCod);
UPDATE files SET Cod=InsCod WHERE InsCod>'0';
UPDATE files SET Cod=CtrCod WHERE CtrCod>'0';
UPDATE files SET Cod=DegCod WHERE DegCod>'0';
UPDATE files SET Cod=CrsCod WHERE CrsCod>'0' AND GrpCod<='0';
UPDATE files SET Cod=GrpCod WHERE GrpCod>'0';
ALTER TABLE files DROP COLUMN InsCod;
ALTER TABLE files DROP COLUMN CtrCod;
ALTER TABLE files DROP COLUMN DegCod;
ALTER TABLE files DROP COLUMN CrsCod;
ALTER TABLE files DROP COLUMN GrpCod;
DROP INDEX Location ON files;

        Version 14.66.1:  Jan 24, 2015	Fixed bug in clipboard. (175404 lines)
        Version 14.66:    Jan 24, 2015	Changes in clipboard table. (175401 lines)
					13 changes necessary in database:
DROP INDEX FileBrowser ON clipboard;
ALTER TABLE clipboard ADD COLUMN Cod INT NOT NULL DEFAULT -1 AFTER FileBrowser, ADD INDEX (FileBrowser,Cod);
CREATE INDEX WorksUsrCod ON clipboard (WorksUsrCod);
UPDATE clipboard SET Cod=InsCod WHERE InsCod>'0';
UPDATE clipboard SET Cod=CtrCod WHERE CtrCod>'0';
UPDATE clipboard SET Cod=DegCod WHERE DegCod>'0';
UPDATE clipboard SET Cod=CrsCod WHERE CrsCod>'0' AND GrpCod<='0';
UPDATE clipboard SET Cod=GrpCod WHERE GrpCod>'0';
ALTER TABLE clipboard DROP COLUMN InsCod;
ALTER TABLE clipboard DROP COLUMN CtrCod;
ALTER TABLE clipboard DROP COLUMN DegCod;
ALTER TABLE clipboard DROP COLUMN CrsCod;
ALTER TABLE clipboard DROP COLUMN GrpCod;

        Version 14.65:    Jan 24, 2015	Changes in expanded_folders table. (175394 lines)
					13 changes necessary in database:
DROP INDEX UsrCod ON expanded_folders;
ALTER TABLE expanded_folders ADD COLUMN Cod INT NOT NULL DEFAULT -1 AFTER FileBrowser, ADD INDEX (UsrCod,FileBrowser,Cod), ADD INDEX (FileBrowser,Cod);
CREATE INDEX WorksUsrCod ON expanded_folders (WorksUsrCod);
UPDATE expanded_folders SET Cod=InsCod WHERE InsCod>'0';
UPDATE expanded_folders SET Cod=CtrCod WHERE CtrCod>'0';
UPDATE expanded_folders SET Cod=DegCod WHERE DegCod>'0';
UPDATE expanded_folders SET Cod=CrsCod WHERE CrsCod>'0' AND GrpCod<='0';
UPDATE expanded_folders SET Cod=GrpCod WHERE GrpCod>'0';
ALTER TABLE expanded_folders DROP COLUMN InsCod;
ALTER TABLE expanded_folders DROP COLUMN CtrCod;
ALTER TABLE expanded_folders DROP COLUMN DegCod;
ALTER TABLE expanded_folders DROP COLUMN CrsCod;
ALTER TABLE expanded_folders DROP COLUMN GrpCod;

        Version 14.64:    Jan 22, 2015	Changes in last accesses to group file browsers. (175510 lines)
					6 changes necessary in database:
REPLACE INTO file_browser_last (UsrCod,FileBrowser,Cod,LastClick) SELECT UsrCod,'11',GrpCod,LastAccDownloadGrp FROM crs_grp_usr WHERE LastAccDownloadGrp>0;
REPLACE INTO file_browser_last (UsrCod,FileBrowser,Cod,LastClick) SELECT UsrCod,'5',GrpCod,LastAccCommonGrp FROM crs_grp_usr WHERE LastAccCommonGrp>0;
REPLACE INTO file_browser_last (UsrCod,FileBrowser,Cod,LastClick) SELECT UsrCod,'13',GrpCod,LastAccCommonGrp FROM crs_grp_usr WHERE LastAccMarksGrp>0;
ALTER TABLE crs_grp_usr DROP COLUMN LastAccDownloadGrp;
ALTER TABLE crs_grp_usr DROP COLUMN LastAccCommonGrp;
ALTER TABLE crs_grp_usr DROP COLUMN LastAccMarksGrp;

        Version 14.63.6:  Jan 22, 2015	Changes in last accesses to course file browsers. (175588 lines)
					10 changes necessary in database:
REPLACE INTO file_browser_last (UsrCod,FileBrowser,Cod,LastClick) SELECT UsrCod,'3',CrsCod,LastAccDownloadCrs FROM crs_usr WHERE LastAccDownloadCrs>0;
REPLACE INTO file_browser_last (UsrCod,FileBrowser,Cod,LastClick) SELECT UsrCod,'4',CrsCod,LastAccCommonCrs FROM crs_usr WHERE LastAccCommonCrs>0;
REPLACE INTO file_browser_last (UsrCod,FileBrowser,Cod,LastClick) SELECT UsrCod,'8',CrsCod,LastAccMarksCrs FROM crs_usr WHERE LastAccMarksCrs>0;
REPLACE INTO file_browser_last (UsrCod,FileBrowser,Cod,LastClick) SELECT UsrCod,'14',CrsCod,LastAccMyWorks FROM crs_usr WHERE LastAccMyWorks>0;
REPLACE INTO file_browser_last (UsrCod,FileBrowser,Cod,LastClick) SELECT UsrCod,'15',CrsCod,LastAccMyWorks FROM crs_usr WHERE LastAccCrsWorks>0;
ALTER TABLE crs_usr DROP COLUMN LastAccDownloadCrs;
ALTER TABLE crs_usr DROP COLUMN LastAccCommonCrs;
ALTER TABLE crs_usr DROP COLUMN LastAccMarksCrs;
ALTER TABLE crs_usr DROP COLUMN LastAccMyWorks;
ALTER TABLE crs_usr DROP COLUMN LastAccCrsWorks;

        Version 14.63.5:  Jan 22, 2015	Changes in last accesses to file browsers (briefcases). (? lines)
					2 changes necessary in database:
REPLACE INTO file_browser_last (UsrCod,FileBrowser,Cod,LastClick) SELECT UsrCod,'9','-1',LastAccBriefcase FROM usr_last WHERE LastAccBriefcase>0;
ALTER TABLE usr_last DROP COLUMN LastAccBriefcase;

        Version 14.63.4:  Jan 21, 2015	Fixed bug in listing of degree administrators, reported by Antonio Fern�ndez Ares. (175548 lines)
        Version 14.63.3:  Jan 21, 2015	Fixed bug when expanding folders, reported by Nadir Rom�n Guerrero and others. (175545 lines)
        Version 14.63.2:  Jan 21, 2015	Fixed bug in search of courses. (175542 lines)
        Version 14.63.1:  Jan 21, 2015	Remove last accesses to file browsers related with an institution, a centre or a degree. (175531 lines)
        Version 14.63:    Jan 21, 2015	See/admin documents of degree, centre and institution. (175512 lines)
					1 change necessary in database:
CREATE TABLE IF NOT EXISTS file_browser_last (UsrCod INT NOT NULL,FileBrowser TINYINT NOT NULL,Cod INT NOT NULL DEFAULT -1,LastClick DATETIME NOT NULL,UNIQUE INDEX(UsrCod,FileBrowser,Cod));

        Version 14.62.4:  Jan 20, 2015	Fixed bug in search of courses. (175443 lines)
        Version 14.62.3:  Jan 20, 2015	See/admin documents of degree, centre and institution (not finished). (? lines)
        Version 14.62.2:  Jan 20, 2015	See/admin documents of degree, centre and institution (not finished). (175407 lines)
        Version 14.62.1:  Jan 20, 2015	See/admin documents of degree, centre and institution (not finished). (175371 lines)
        Version 14.62:    Jan 20, 2015	See/admin documents of degree, centre and institution (not finished). (175296 lines)
					3 changes necessary in database:
ALTER TABLE expanded_folders ADD COLUMN InsCod INT NOT NULL DEFAULT -1 AFTER FileBrowser, ADD INDEX (InsCod);
ALTER TABLE expanded_folders ADD COLUMN CtrCod INT NOT NULL DEFAULT -1 AFTER InsCod, ADD INDEX (CtrCod);
ALTER TABLE expanded_folders ADD COLUMN DegCod INT NOT NULL DEFAULT -1 AFTER CtrCod, ADD INDEX (DegCod);

        Version 14.61:    Jan 20, 2015	See/admin documents of degree, centre and institution (not finished). (175112 lines)
					6 changes necessary in database:
ALTER TABLE files ADD COLUMN InsCod INT NOT NULL DEFAULT -1 AFTER FilCod;
ALTER TABLE files ADD COLUMN CtrCod INT NOT NULL DEFAULT -1 AFTER InsCod;
ALTER TABLE files ADD COLUMN DegCod INT NOT NULL DEFAULT -1 AFTER CtrCod;
DROP INDEX CrsCod ON files;
DROP INDEX CrsCod_GrpCod_ZoneUsrCod_FileBrowser ON files;
CREATE INDEX Location ON files (InsCod,CtrCod,DegCod,CrsCod,GrpCod,FileBrowser);

	Version 14.60:    Jan 20, 2015	See/admin documents of degree, centre and institution (not finished). (175079 lines)
					3 changes necessary in database:
ALTER TABLE clipboard ADD COLUMN InsCod INT NOT NULL DEFAULT -1 AFTER FileBrowser, ADD INDEX (InsCod);
ALTER TABLE clipboard ADD COLUMN CtrCod INT NOT NULL DEFAULT -1 AFTER InsCod, ADD INDEX (CtrCod);
ALTER TABLE clipboard ADD COLUMN DegCod INT NOT NULL DEFAULT -1 AFTER CtrCod, ADD INDEX (DegCod);

	Version 14.59.3:  Jan 20, 2015	See/admin documents of degree, centre and institution (not finished). (174862 lines)
	Version 14.59.2:  Jan 19, 2015	See/admin documents of degree, centre and institution (not finished). (174864 lines)
	Version 14.59.1:  Jan 19, 2015	See/admin documents of degree, centre and institution (not finished). (174711 lines)
	Version 14.59:    Jan 19, 2015	See/admin documents of degree, centre and institution (not finished). (174559 lines)
	Version 14.58.1:  Jan 18, 2015	Fixed minor bugs in layout. (174178 lines)
	Version 14.58:    Jan 18, 2015	New options to see/admin documents of degree, centre and institution (not finished). (174174 lines)
	Version 14.57.3:  Jan 18, 2015	Fixed bug in statistics. (173694 lines)
	Version 14.57.2:  Jan 18, 2015	Icon of institution, centre or degree is not drawn on title when logo does not exist. (173693 lines)
	Version 14.57.1:  Jan 17, 2015	Code refactoring related to logos of institutions, centres and degrees. (173674 lines)
	Version 14.57:    Jan 17, 2015	New option to upload logo of degree. (173720 lines)
					3 changes necessary in database:
UPDATE actions SET Txt='Cambiar logo de titulaci&oacute;n' WHERE ActCod='553' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1246','es','N','Solicitar env&iacute;o de logo de titulaci&oacute;n');
ALTER TABLE degrees DROP COLUMN Logo;

	Version 14.56:    Jan 17, 2015	New module swad_logo for drawing logos of institutions, centres and degrees.
					New option to upload logo of institution. (173735 lines)
					1 change necessary in Makefile:
Add swad_logo.o to list of object files
					3 changes necessary in database:
UPDATE actions SET Txt='Cambiar logo de instituci&oacute;n' WHERE ActCod='699' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1245','es','N','Solicitar env&iacute;o de logo de instituci&oacute;n');
ALTER TABLE institutions DROP COLUMN Logo;

	Version 14.55.5:  Jan 16, 2015	Code refactoring related to maps of countries. (173573 lines)
	Version 14.55.4:  Jan 16, 2015	Fixed bugs in enrolment.
					New MIME type, suggested by Francisco Oca�a Lara. (173576 lines)
	Version 14.55.3:  Jan 16, 2015	Link in course information to request registration in course.
					Fixed bug in enrolment. (173570 lines)
	Version 14.55.2:  Jan 15, 2015	Logo of centre, degree, institution or country in title. (173546 lines)
	Version 14.55.1:  Jan 14, 2015	Fixed bug in user's photo. (173510 lines)
	Version 14.55:    Jan 14, 2015	New option to upload logo of a centre. (173509 lines)
					3 changes necessary in database:
UPDATE actions SET Txt='Cambiar logo de centro' WHERE ActCod='1051' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1244','es','N','Solicitar env&iacute;o de logo de centro');
ALTER TABLE centres DROP COLUMN Logo;

	Version 14.54.7:  Jan 13, 2015	Fixed bug in edition of centres. (173349 lines)
	Version 14.54.6:  Jan 13, 2015	Tab and menu icons always in 64x64 pixels. (173348 lines)
	Version 14.54.5:  Jan 13, 2015	Icons for institutions, centres and degrees always in 64x64 pixels. (173346 lines)
	Version 14.54.4:  Jan 13, 2015	Fized bug in photos. (173345 lines)
	Version 14.54.3:  Jan 06, 2015	Changes in themes and colors. (173344 lines)
	Version 14.54.2:  Jan 05, 2015	Changes in yellow theme. (173339 lines)
	Version 14.54.1:  Jan 04, 2015	Removed unused fields in user's data. (173338 lines)
					1 change necessary in database:
ALTER TABLE usr_data DROP COLUMN WWW,DROP COLUMN Twitter,DROP COLUMN Skype;

	Version 14.54:    Jan 04, 2015	New module swad_tab for drawing tabs. (173341 lines)
					1 change necessary in Makefile:
Add swad_tab.o to list of object files

	Version 14.53.1:  Jan 04, 2015	Some functions moved from swad_layout to swad_menu. (173256 lines)
	Version 14.53:    Jan 04, 2015	New module swad_calendar for drawing months and calendar. (173254 lines)
					1 change necessary in Makefile:
Add swad_calendar.o to list of object files

	Version 14.52.6:  Jan 04, 2015	Fixed bugs for HTML5 validation. (173187 lines)
	Version 14.52.5:  Jan 02, 2015	Changes in CSS. (173170 lines)
	Version 14.52.4:  Jan 02, 2015	Removed unnecessary titles in statistics. (173169 lines)
	Version 14.52.3:  Jan 02, 2015	Titles in statistics. (173258 lines)
	Version 14.52.2:  Jan 02, 2015	Fixed bugs in layout.
					Statistics about number of users who hace chosen a menu. (173163 lines)
	Version 14.52.1:  Jan 02, 2015	Link to calendar in name of month. (173021 lines)
	Version 14.52:    Jan 02, 2015	Calendar is moved from course tab to assessment tab. (? lines)
	Version 14.51.2:  Jan 02, 2015	Changes in layout and titles of some options. (172987 lines)
	Version 14.51.1:  Jan 02, 2015	Changes in layout and titles of some options. (172963 lines)
	Version 14.51:    Jan 01, 2015	Users can select horizontal or vertical menu. (172958 lines)
					1 change necessary in Makefile:
Add swad_menu.o to list of object files
					5 changes necessary in database:
ALTER TABLE usr_data ADD COLUMN Menu TINYINT NOT NULL DEFAULT 0 AFTER Comments;
ALTER TABLE usr_data ADD INDEX (Menu);
UPDATE usr_data SET Menu=1;
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1243','es','N','Cambiar men&uacute;');
ALTER TABLE IP_prefs ADD COLUMN Menu TINYINT NOT NULL DEFAULT 0 AFTER IconSet;

	Version 14.50.4:  Jan 01, 2015	Changes in preferences and photos. (172705 lines)
	Version 14.50.3:  Jan 01, 2015	Changes in layout of preferences. (172691 lines)
	Version 14.50.2:  Jan 01, 2015	Changes in CSS. (172739 lines)
	Version 14.50.1:  Dec 31, 2014	Horizontal menu (not finished). (172736 lines)
	Version 14.50:    Dec 31, 2014	Horizontal menu (not finished). (172735 lines)
	Version 14.49.2:  Dec 31, 2014	Removed text of menu options with BR. (172611 lines)
	Version 14.49.1:  Dec 31, 2014	Changes in layout. (174432 lines)
	Version 14.49:    Dec 31, 2014	Option to admin one user removed from main menu.
					Fixed bugs in layout.
					Fixed bugs in admin of one user. (174419 lines)
	Version 14.48.7:  Dec 31, 2014	New link to admin one user in option to admin several users. (174440 lines)
	Version 14.48.6:  Dec 31, 2014	Option to see accesses to the current course removed from main menu.
					The text of some menu items has been reduced to a single word. (174404 lines)
	Version 14.48.5:  Dec 31, 2014	New link to see accesses to the current course in platform accesses. (174463 lines)
	Version 14.48.4:  Dec 31, 2014	The text of some menu items has been reduced to a single word.
					Fixed bugs in layout. (174420 lines)
	Version 14.48.3:  Dec 30, 2014	Options to see the results of test exams removed from main menu. (174418 lines)
	Version 14.48.2:  Dec 30, 2014	New links to see the results of test exams. (174539 lines)
	Version 14.48.1:  Dec 30, 2014	The text of some menu items has been reduced to a single word.
					Fixed bugs in layout. (174502 lines)
	Version 14.48:    Dec 30, 2014	Changes in layout and CSS. (174502 lines)
	Version 14.47.7:  Dec 30, 2014	Changes in CSS font families. (174554 lines)
	Version 14.47.6:  Dec 30, 2014	Changes in HTML and CSS. (174551 lines)
	Version 14.47.5:  Dec 29, 2014	Restricted length of links in user's courses. (174549 lines)
	Version 14.47.4:  Dec 29, 2014	Changes in HTML and CSS. (174535 lines)
	Version 14.47.3:  Dec 29, 2014	Removed old code not used. (174499 lines)
	Version 14.47.2:  Dec 29, 2014	Changes in listing of chat rooms, forums and user's courses. (174677 lines)
	Version 14.47.1:  Dec 29, 2014	Changes in listing of chat rooms, forums and user's courses.
	                                Fixed bugs in CSS. (174619 lines)
	Version 14.47:    Dec 29, 2014	Changes in listing of user's courses. (174659 lines)
	Version 14.46.2:  Dec 28, 2014	Code refactoring in forums. (174400 lines)
	Version 14.46.1:  Dec 28, 2014	Changes in listing of forums. (174342 lines)
	Version 14.46:    Dec 28, 2014	Course syllabus are shown in one unique option. (174340 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1242','es','N','Ver programa');

	Version 14.45:    Dec 27, 2014	Course syllabus are shown in one unique option (unfinished). (174346 lines)
	Version 14.44.3:  Dec 27, 2014	Fixed bugs in layout. (174246 lines)
	Version 14.44.2:  Dec 27, 2014	Fixed bugs in layout columns. (174262 lines)
	Version 14.44.1:  Dec 27, 2014	Changes in layout columns. (174251 lines)
	Version 14.44:    Dec 27, 2014	Removed vendor-specific properties in CSS. (174230 lines)
	Version 14.43.1:  Dec 27, 2014	Fixed bugs in HTML and CSS. (174319 lines)
	Version 14.43:    Dec 27, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (174314 lines)
	Version 14.42.1:  Dec 26, 2014	Fixed bug in HTML of tabs.
					Changes in foot.html to validate HTML5 in W3C validator.
					Changes in HTML and CSS to validate HTML5 in W3C validator. (173987 lines)
	Version 14.42:    Dec 26, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (173703 lines)
	Version 14.41:    Dec 26, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (173630 lines)
	Version 14.40.12: Dec 26, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (173478 lines)
	Version 14.40.11: Dec 25, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (173288 lines)
	Version 14.40.10: Dec 25, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (173201 lines)
	Version 14.40.9:  Dec 25, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (172986 lines)
	Version 14.40.8:  Dec 24, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (172653 lines)
	Version 14.40.7:  Dec 23, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (172552 lines)
	Version 14.40.6:  Dec 23, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (172400 lines)
	Version 14.40.5:  Dec 23, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (172269 lines)
	Version 14.40.4:  Dec 23, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (172112 lines)
	Version 14.40.3:  Dec 22, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (171698 lines)
	Version 14.40.2:  Dec 22, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (171588 lines)
	Version 14.40.1:  Dec 22, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator.
					Fixed problem in edition of departaments. (171417 lines)
	Version 14.40:    Dec 22, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (171118 lines)
	Version 14.39.6:  Dec 22, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (? lines)
	Version 14.39.5:  Dec 21, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (170725 lines)
	Version 14.39.4:  Dec 21, 2014	Changes in HTML and CSS to validate HTML5 in W3C validator. (170688 lines)
	Version 14.39.3:  Dec 21, 2014	Changes to validate HTML5 in W3C validator.
					Charset is changed from ISO-8859-1 to windows-1252. (170664 lines)
	Version 14.39.2:  Dec 21, 2014	Changes in layout and graphics. (170739 lines)
	Version 14.39.1:  Dec 20, 2014	Change from XHTML 1.0 Transitional to HTML5. (170747 lines)
	Version 14.39:    Dec 20, 2014	iframes removed from course info. (170746 lines)
	Version 14.38.9:  Dec 20, 2014	The words "Course" and "Group" are removed from selection of course and groups in Documents and Shared files. (? lines)
	Version 14.38.8:  Dec 18, 2014	Fixed bug in banned users. (170793 lines)
	Version 14.38.7:  Dec 17, 2014	Link to banned users is shown only if user has banned one or more users. (170792 lines)
	Version 14.38.6:  Dec 17, 2014	Banned users option is integrated in received messages option. (170776 lines)
	Version 14.38.5:  Dec 16, 2014	User's institution option is integrated in record card option. (170819 lines)
	Version 14.38.4:  Dec 16, 2014	User's photo option is integrated in record card option. (170941 lines)
	Version 14.38.3:  Dec 16, 2014	Lost password option is integrated in session option.
					User's password option is integrated in account option. (171001 lines)
	Version 14.38.2:  Dec 14, 2014	Changes in search. (171124 lines)
	Version 14.38.1:  Dec 13, 2014	Changes in ID messages.
					Changes in search. (171118 lines)
	Version 14.38:    Dec 13, 2014	Link in "my courses" to courses, degrees, centres, institutions or countries. (171083 lines)
	Version 14.37.3:  Dec 13, 2014	The title of list of courses in current degree now contains the short name of the degree. (170898 lines)
	Version 14.37.2:  Dec 13, 2014	Changes in search of courses link in "my courses". (170874 lines)
	Version 14.37.1:  Dec 13, 2014	Fixed bug in HTML entities in swad_text.c (translation).
					Guests can view option "my courses" on profile tab.
					Reordering of options in profile tab. (170866 lines)
	Version 14.37:    Dec 12, 2014	New module swad_account for user's account. (170863 lines)
					1 change necessary in Makefile:
Add swad_account.o to list of object files

	Version 14.36:    Dec 12, 2014	New module swad_role for user's roles. (170764 lines)
					1 change necessary in Makefile:
Add swad_role.o to list of object files

	Version 14.35:    Dec 12, 2014	New MIME type, suggested by Benito Palacios.
					New module swad_enrolment for enrolment (registration) or removing of users. (170725 lines)
					1 change necessary in Makefile:
Add swad_enrolment.o to list of object files

	Version 14.34.4:  Dec 08, 2014	Some messages translated. (170623 lines)
	Version 14.34.3:  Dec 08, 2014	Tables are printed with rounded corners. (170602 lines)
	Version 14.34.2:  Dec 08, 2014	Button to show more details on list of attendances. (170728 lines)
	Version 14.34.1:  Dec 08, 2014	Translation of a message when refreshing list of attendances. (170710 lines)
	Version 14.34:    Dec 08, 2014	Teachers can refresh list of attendances after selecting events. (170688 lines)
	Version 14.33:    Dec 07, 2014	A teacher can select attendance events in list of attendances. (170595 lines)
	Version 14.32.1:  Dec 06, 2014	Added new social network. (170518 lines)
					1 change necessary in database:
ALTER TABLE usr_webs CHANGE Web Web ENUM('www','delicious','edmodo','facebook','flickr','foursquare','github','googleplus','googlescholar','instagram','linkedin','paperli','pinterest','researchgate','scoopit','slideshare','storify','tumblr','twitter','wikipedia','youtube') NOT NULL;

        Version 14.32:    Dec 06, 2014	Changes in listing of attendance. (170509 lines)
        Version 14.31.1:  Dec 05, 2014	Comments are shown in list of attendances when several students are listed. (170450 lines)
        Version 14.31:    Dec 04, 2014	Comments are shown in list of attendances when one unique student is listed. (170353 lines)
        Version 14.30.2:  Dec 03, 2014	Changes in sign up.
					Fixed bug when deleting user's IDs. (170252 lines)
        Version 14.30.1:  Nov 30, 2014	Internal course code is shown in course configuration. (170275 lines)
        Version 14.30:    Nov 29, 2014	Database password and email password are moved from executable files to configuration files. (170248 lines)
					1 change necessary in Makefile:
Add swad_config.o to list of object files

        Version 14.29:    Nov 29, 2014	New structure of source code directories for SWAD core. (170066 lines)
        Version 14.28.2:  Nov 28, 2014	Fixed bug in web service function sendAttendanceUsers.
					New MIME type for file uploading, problem reported by Marta G�mez Mac�as. (170380 lines)
        Version 14.28.1:  Nov 28, 2014	Fixed bugs in web service function sendAttendanceUsers. (170377 lines)
        Version 14.28:    Nov 25, 2014	Changes in edition of users' IDs. (170365 lines)
					5 changes necessary in database:
UPDATE actions SET Txt='Solicitar la creaci&oacute;n de un anuncio global' WHERE ActCod='1237' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1238','es','N','Crear anuncio global');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1239','es','N','Solicitar edici&oacute;n ID otro usuario');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1240','es','N','Eliminar ID de otro usuario');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1241','es','N','Crear ID de otro usuario');

        Version 14.27:    Nov 25, 2014	Superusers can edit other users' IDs. (170238 lines)
        Version 14.26.3:  Nov 24, 2014	Code refactoring. (170145 lines)
        Version 14.26.2:  Nov 24, 2014	QR available in control of attendance. (170169 lines)
        Version 14.26.1:  Nov 24, 2014	QR available in record card. (170161 lines)
        Version 14.26:    Nov 23, 2014	Superusers can create a global announcement.
					Code refactoring in notices. (170173 lines)
					2 changes necessary in database:
UPDATE actions SET Txt='Solicitar la creaci&oacute;n de una cuenta' WHERE ActCod='36' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1237','es','N','Crear anuncio global');

        Version 14.25.1:  Nov 21, 2014	Superusers can remove a global announcement. (169976 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1236','es','N','Eliminar anuncio global');

        Version 14.25:    Nov 20, 2014	Superusers can view all the global announcements. (169948 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1235','es','N','Ver anuncios globales');

        Version 14.24.5:  Nov 19, 2014	Fixed bug in web service functions getAttendanceUsers, reported by Juan Miguel Boyero Corral. (169810 lines)
        Version 14.24.4:  Nov 19, 2014	Fixed bug in web service functions getAttendanceUsers, reported by Juan Miguel Boyero Corral. (169807 lines)
        Version 14.24.3:  Nov 19, 2014	Fixed bug in web service functions getTestQuestions, reported by Marta Mu�oz L�pez. (169804 lines)
        Version 14.24.2:  Nov 19, 2014	Fixed bug in web service functions getAttendanceUsers and sendAttendanceUsers, reported by Juan Miguel Boyero Corral. (169804 lines)
        Version 14.24.1:  Nov 19, 2014	Changes in global announcements. (169803 lines)
					1 change necessary in database:
ALTER TABLE announcements ADD COLUMN Subject TEXT NOT NULL AFTER Roles;

        Version 14.24:    Nov 19, 2014	New module swad_announcement for global announcements. (169760 lines)
					1 change necessary in Makefile:
Add swad_announcement.o to list of object files
					3 changes necessary in database:
CREATE TABLE IF NOT EXISTS announcements (AnnCod INT NOT NULL AUTO_INCREMENT,Roles INT NOT NULL DEFAULT 0,Content TEXT NOT NULL,UNIQUE INDEX(AnnCod));
CREATE TABLE IF NOT EXISTS ann_seen (AnnCod INT NOT NULL,UsrCod INT NOT NULL,UNIQUE INDEX(AnnCod,UsrCod));
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1234','es','N','Marcar anuncio global como visto');

        Version 14.23.5:  Nov 18, 2014	Fixed bug in web service functions getAttendanceUsers and sendAttendanceUsers, reported by Juan Miguel Boyero Corral. (169485 lines)
        Version 14.23.4:  Nov 17, 2014	A user can not remove her/his ID if it's confirmed. (169484 lines)
        Version 14.23.3:  Nov 17, 2014	Refactoring in deletion of a course. (169457 lines)
        Version 14.23.2:  Nov 17, 2014	For security, assignments and works will not be removed when several users are removed from a course,
					but they will be removed when one student or all students are removed. (169438 lines)
        Version 14.23.1:  Nov 16, 2014	Changes related to user's ID. (169424 lines)
        Version 14.23:    Nov 16, 2014	Refactored (simplified) code related with ending alpha letter in user's ID. (169358 lines)
        Version 14.22.2:  Nov 16, 2014	Ending alpha letters in users' IDs are not removed.
					Fixed bug in administration of a user. (169396 lines)
        Version 14.22.1:  Nov 15, 2014	Fixed bug in web service function getAttendanceEvents, reported by Juan Miguel Boyero Corral. (169392 lines)
        Version 14.22:    Nov 15, 2014	Link to remove my account in Profile > Account.
					Fixed bug in web service function getAttendanceEvents, reported by Juan Miguel Boyero Corral. (169375 lines)
        Version 14.21.1:  Nov 15, 2014	Option for admin one user is available for all logged users. (169368 lines)
        Version 14.21:    Nov 14, 2014	Include consent and password on dangerous actions.
					Fixed bug in web service function getAttendanceEvents, reported by Juan Miguel Boyero Corral.
					New MIME type for file uploading, problem reported by Mar�a Luisa Palaz�n Garrido. (169321 lines)
        Version 14.20.2:  Nov 13, 2014	All users, except superusers, can eliminate themselves. (169125 lines)
        Version 14.20.1:  Nov 13, 2014	Changes in web service functions. (? lines)
        Version 14.20:    Nov 11, 2014	Implemented web service function getMarks. (169089 lines)
        Version 14.19:    Nov 11, 2014	Refactoring of web service functions. (169031 lines)
        Version 14.18:    Nov 11, 2014	Nickname and birthday is returned in some web service functions.
					Removed obsolete fields in web service functions. (169452 lines)
        Version 14.17.1:  Nov 10, 2014	User's QR is generated only for @nickname. (? lines)
        Version 14.17:    Nov 09, 2014	Implemented web service function sendAttendanceEvent.
					Refactoring of web service functions. (169492 lines)
        Version 14.16:    Nov 09, 2014	Implemented web service function sendAttendanceUsers. (169422 lines)
        Version 14.15.6:  Nov 08, 2014	Changes in web service function getAttendanceUsers. (169304 lines)
        Version 14.15.5:  Nov 08, 2014	Changes in web service function getAttendanceUsers. (169304 lines)
        Version 14.15.4:  Nov 06, 2014	Fixed bug related to filenames with heading spaces, reported by Germ�n Rodr�guez Salido. (169297 lines)
        Version 14.15.3:  Nov 06, 2014	Fixed bug in date of email in swad_smtp.py, reported by Ana Ferreira Blanco. (169297 lines)
        Version 14.15.2:  Nov 06, 2014	Changes in web service function getAttendanceUsers. (169296 lines)
        Version 14.15.1:  Nov 05, 2014	Fixed bug when zipping folders, reported by Javier Fern�ndez Baldomero and others. (169246 lines)
        Version 14.15:    Nov 04, 2014	Implemented web service function getAttendanceUsers. (169246 lines)
        Version 14.14:    Nov 03, 2014	Implemented skeletons of 2 new web service functions.
					Implemented web service function getAttendanceEvents. (168986 lines)
        Version 14.13:    Nov 02, 2014	Implemented skeletons of 4 new web service functions related with control of attendance. (168797 lines)
        Version 14.12.1:  Nov 02, 2014	Code refactoring related with parameters. (168622 lines)
        Version 14.12:    Nov 02, 2014	Code refactoring related with parameters.
					Fixed bug in tests. (168621 lines)
        Version 14.11.9:  Nov 02, 2014	Code refactoring related with parameters. (168624 lines)
        Version 14.11.8:  Nov 01, 2014	Code refactoring related with parameters. (168977 lines)
        Version 14.11.7:  Nov 01, 2014	Code refactoring related with parameters. (169279 lines)
        Version 14.11.6:  Oct 30, 2014	Code refactoring in file browsers. (168688 lines)
        Version 14.11.5:  Oct 29, 2014	Code refactoring in file browsers. (168555 lines)
        Version 14.11.4:  Oct 29, 2014	Code refactoring in file browsers. (168411 lines)
        Version 14.11.3:  Oct 29, 2014	Information about type of file is stores in database (table files). (168350 lines)
					1 change necessary in database:
ALTER TABLE files ADD COLUMN FileType TINYINT NOT NULL DEFAULT 0 AFTER PublisherUsrCod;

        Version 14.11.2:  Oct 28, 2014	Lot of code refactoring in file browsers (not finished). (168298 lines)
					1 change necessary in database:
ALTER TABLE clipboard ADD COLUMN FileType TINYINT NOT NULL DEFAULT 0 AFTER WorksUsrCod;

        Version 14.11.1:  Oct 28, 2014	Lot of code refactoring in file browsers (not finished). (168210 lines)
        Version 14.11:    Oct 27, 2014	Changes in links in file browsers (not finished).
					Refactored code in file browsers. (168158 lines)
        Version 14.10:    Oct 26, 2014	Users can send link to URL in file browsers (not finished). (168054 lines)
					9 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1225','es','N','Crear enlace documentos asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1226','es','N','Crear enlace com&uacute;n asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1227','es','N','Crear enlace com&uacute;n grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1228','es','N','Crear enlace mis trabajos');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1229','es','N','Crear enlace trabajos asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1230','es','N','Crear enlace en malet&iacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1231','es','N','Crear enlace documentos grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1232','es','N','Crear enlace mis actividades');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1233','es','N','Crear enlace activid. asg.');

        Version 14.9.2:   Oct 24, 2014	Some code cleaning. (167569 lines)
        Version 14.9.1:   Oct 22, 2014	Google Analytics without cookies for openswad.org (code from http://stackoverflow.com/a/19995629). (167555 lines)
        Version 14.9:     Oct 21, 2014  Removed the use of http icons.
					Fixed bug in scope of users' listing. (167544 lines)
					2 changes necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='760';
ALTER TABLE usr_data DROP COLUMN SecureIcons;

        Version 14.8.3:   Oct 20, 2014  Fixed bug in Internet Explorer reported by Francisco A. Oca�a Lara: event onchange does not work on input radio, replaced by onclick. (167631 lines)
        Version 14.8.2:   Oct 20, 2014  Fixed bug in tests importing. (167630 lines)
					1 change necessary in database:
UPDATE tst_questions SET Feedback='' WHERE Feedback='(null)';

        Version 14.8.1:   Oct 19, 2014  Fixed bug in tests importing, reported by Olga Valenzuela. (167627 lines)
        Version 14.8:     Oct 19, 2014  New module swad_test_import for export and import tests. (167594 lines)
					1 change necessary in Makefile:
Add swad_test_import.o to list of object files
        .
        Version 14.7:     Oct 16, 2014  Changes in swad_smtp.py.
				        Now automatic mails are sent in secure way. (167494 lines)
					1 change necessary:
Copy the new swad_smtp.py into swad cgi directory

        Version 14.6.2:   Oct 15, 2014  Big icon to upload files.
					Fixed bug in classic upload of files. (167541 lines)
        Version 14.6.1:   Oct 15, 2014  Some messages translated.
					Nickname appears in record card with a bigger font.
					Changes in some icons. (167508 lines)
        Version 14.6:     Oct 13, 2014  Changes in translations (polish characters).
					Button for admin user in other user's common record check.
					Changes in admin of one user for students. (167497 lines)
        Version 14.5.2:   Oct 11, 2014  One message translated.
					Minor code refactoring. (167396 lines)
        Version 14.5.1:   Oct 08, 2014  Minor changes related to messages in automatic emails. (167374 lines)
        Version 14.5:     Oct 07, 2014  Fixed issues related with charset. (167368 lines)
        Version 14.4.2:   Oct 07, 2014  Changes in upload of files. (167354 lines)
        Version 14.4.1:   Oct 07, 2014  Changes in upload of files. (167351 lines)
        Version 14.4:     Oct 07, 2014  Changes in upload of files. (167360 lines)
					22 changes necessary in database:
UPDATE actions SET Txt='Subir archivo doc. asg. (ant.)'         WHERE ActCod='482' AND Language='es';
UPDATE actions SET Txt='Subir archivo doc. grp. (ant.)'         WHERE ActCod='483' AND Language='es';
UPDATE actions SET Txt='Subir archivo com&uacute;n asg. (ant.)' WHERE ActCod='326' AND Language='es';
UPDATE actions SET Txt='Subir archivo com&uacute;n grp. (ant.)' WHERE ActCod='335' AND Language='es';
UPDATE actions SET Txt='Subir archivo mis activid. (ant.)'      WHERE ActCod='832' AND Language='es';
UPDATE actions SET Txt='Subir archivo mis trabajos (ant.)'      WHERE ActCod='148' AND Language='es';
UPDATE actions SET Txt='Subir archivo activid. asg. (ant.)'     WHERE ActCod='846' AND Language='es';
UPDATE actions SET Txt='Subir archivo trabajos asg. (ant.)'     WHERE ActCod='207' AND Language='es';
UPDATE actions SET Txt='Subir archivo calif. asg. (ant.)'       WHERE ActCod='516' AND Language='es';
UPDATE actions SET Txt='Subir archivo calif. grp. (ant.)'       WHERE ActCod='514' AND Language='es';
UPDATE actions SET Txt='Subir archivo a malet&iacute;n (ant.)'  WHERE ActCod='153' AND Language='es';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1214','es','N','Subir archivo doc. asg.'        );
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1215','es','N','Subir archivo doc. grp.'        );
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1216','es','N','Subir archivo com&uacute;n asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1217','es','N','Subir archivo com&uacute;n grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1218','es','N','Subir archivo mis activid.'     );
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1219','es','N','Subir archivo mis trabajos'     );
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1220','es','N','Subir archivo activid. asg.'    );
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1221','es','N','Subir archivo trabajos asg.'    );
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1222','es','N','Subir archivo calif. asg.'      );
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1223','es','N','Subir archivo calif. grp.'      );
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1224','es','N','Subir archivo a malet&iacute;n' );

        Version 14.3:     Oct 06, 2014  Changes in upload of files.
					Refactoring of files of marks. (167340 lines)
        Version 14.2:     Oct 06, 2014  Changes in upload of files.
					Now both methods are allowed for uploading: Dropzone.js and classic. (167320 lines)
        Version 14.1.2:   Oct 05, 2014  Changes in upload of files. (167209 lines)
        Version 14.1.1:   Oct 04, 2014  Changes in upload of files. (167102 lines)
        Version 14.1:     Oct 02, 2014  Form to drag-and-drop to upload files. (167101 lines)
        Version 14.0:     Sep 26, 2014  "Invited" users are changed to "guests".
        				Search of guests users. (167028 lines)
                                        2 changes necessary in database:
UPDATE sessions SET WhatToSearch=WhatToSearch+1 WHERE WhatToSearch>=7;
UPDATE usr_last SET WhatToSearch=WhatToSearch+1 WHERE WhatToSearch>=7;

	Version 13.85.5:  Sep 20, 2014	Fixed bug in registration of several users. (167150 lines)
	Version 13.85.4:  Sep 16, 2014	The function getUsers in web service is now available for students. (167147 lines)
	Version 13.85.3:  Sep 16, 2014	In edition of centres, only institutions of the current country are shown.
					In edition of degrees, only centres of the current institution are shown.
					In edition of courses, only degrees of the current centre are shown. (167148 lines)
	Version 13.85.2:  Sep 16, 2014	Minor bugs fixed. (167146 lines)
	Version 13.85.1:  Sep 16, 2014	Code refactoring. (167142 lines)
	Version 13.85:    Sep 11, 2014	Banner can be hidden. (166326 lines)
					4 changes necessary in database:
ALTER TABLE banners ADD COLUMN Hidden ENUM('N','Y') NOT NULL DEFAULT 'N' AFTER BanCod;
CREATE INDEX Hidden ON banners (Hidden);
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1212','es','N','Mostrar banner');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1213','es','N','Ocultar banner');

	Version 13.84.2:  Jul 29, 2014	Changes in layout of page foot. (166190 lines)
	Version 13.84.1:  Jul 29, 2014	Fixed bug in layout of page. (166163 lines)
	Version 13.84:    Jul 29, 2014	HTML file included at the bottom of page. (166157 lines)
	Version 13.83:    Jul 28, 2014	Search of students. (166135 lines)
					2 changes necessary in database:
UPDATE sessions SET WhatToSearch=WhatToSearch+1 WHERE WhatToSearch>=6;
UPDATE usr_last SET WhatToSearch=WhatToSearch+1 WHERE WhatToSearch>=6;

	Version 13.82.3:  Jul 23, 2014	When searching from top of page, the scope is set to the platform. (166233 lines)
	Version 13.82.2:  Jul 23, 2014	Fixed bug in data of a place.
					Fixed bug in data of an institution.
					Change in order of results for institutions, centres and degrees. (166231 lines)
	Version 13.82.1:  Jul 23, 2014	Search in top of page is simplified to global search. (166221 lines)
	Version 13.82:    Jul 23, 2014	Changes in search. (166205 lines)
	Version 13.81.2:  Jul 23, 2014	Global search of institutions, centres, degrees, courses, teachers, documents... (166082 lines)
	Version 13.81.1:  Jul 22, 2014	Search of institutions and centres. (166126 lines)
	Version 13.81:    Jul 22, 2014	Search of degrees. (166089 lines)
	Version 13.80:    Jul 22, 2014	Changes in search.
					New search types: institutions, centres and degrees. (165976 lines)
					2 changes necessary in database:
UPDATE sessions SET WhatToSearch=WhatToSearch+3 WHERE WhatToSearch>0;
UPDATE usr_last SET WhatToSearch=WhatToSearch+3 WHERE WhatToSearch>0;

	Version 13.79:    Jul 18, 2014	New statistics about institutions with centres, degrees, courses and users.
					New statistics about centres with degrees, courses and users.
					New statistics about degrees with courses and users.
					New statistics about courses with users. (165943 lines)
	Version 13.78:    Jul 17, 2014	New statistics about countries with institutions, centres, degrees, courses and users. (165552 lines)
	Version 13.77.4:  Jul 15, 2014	Any user can view teachers' records.
					Changes in layout of timetable and classphoto. (165378 lines)
	Version 13.77.3:  Jul 15, 2014	Fixed bug in file browser (course works), reported by F. Javier Fern&aacute;ndez Baldomero. (165366 lines)
	Version 13.77.2:  Jul 15, 2014	Fixed bug in list of institutions. (165364 lines)
	Version 13.77.1:  Jul 15, 2014	Fixed bug in file browser (shared files). (165363 lines)
	Version 13.77:    Jul 13, 2014	New statistics: number of countries, institutions and centres. (165360 lines)
	Version 13.76.8:  Jul 12, 2014	Edition of degree logo is moved to configuration of degree. (165250 lines)
	Version 13.76.7:  Jul 12, 2014	Edition of centre logo is moved to configuration of centre. (165284 lines)
	Version 13.76.6:  Jul 12, 2014	Edition of institution logo is moved to configuration of institution. (165288 lines)
	Version 13.76.5:  Jul 11, 2014	Changes in degree logo. (165304 lines)
	Version 13.76.4:  Jul 11, 2014	Changes in centre logo. (165271 lines)
	Version 13.76.3:  Jul 10, 2014	Changes in edition of institutions, centres and degrees. (165238 lines)
	Version 13.76.2:  Jul 09, 2014	Fixed bugs edition of centres and institutions.
					Some warning messages have been simplified. (165279 lines)
	Version 13.76.1:  Jul 08, 2014	Fixed bugs in listing and edition of centres and degrees. (165308 lines)
	Version 13.76:    Jul 08, 2014	All users can create new institutions. (165295 lines)
					5 changes necessary in database:
ALTER TABLE institutions ADD COLUMN Status TINYINT NOT NULL DEFAULT 0 AFTER CtyCod;
ALTER TABLE institutions ADD COLUMN RequesterUsrCod INT NOT NULL DEFAULT -1 AFTER Status;
CREATE INDEX Status ON institutions (Status);
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1210','es','N','Solicitar instituci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1211','es','N','Cambiar estado instituci&oacute;n');

	Version 13.75.2:  Jul 08, 2014	List institutions with pending centres. (164806 lines)
	Version 13.75.1:  Jul 07, 2014	Changes in edition of centres. (164332 lines)
	Version 13.75:    Jul 06, 2014	All users can create new centres. (164005 lines)
					5 changes necessary in database:
ALTER TABLE centres ADD COLUMN Status TINYINT NOT NULL DEFAULT 0 AFTER PlcCod;
ALTER TABLE centres ADD COLUMN RequesterUsrCod INT NOT NULL DEFAULT -1 AFTER Status;
CREATE INDEX Status ON centres (Status);
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1208','es','N','Solicitar centro');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1209','es','N','Cambiar estado centro');

	Version 13.74.5:  Jul 06, 2014	Changes in list of centres. (163947 lines)
	Version 13.74.4:  Jul 06, 2014	Internal changes in swad_text.c. (163934 lines)
	Version 13.74.3:  Jul 06, 2014	List centres with pending degrees. (163825 lines)
	Version 13.74.2:  Jul 05, 2014	List centres with pending degrees. (163770 lines)
					1 change necessary in database:
UPDATE actions SET Txt='Ver instit., centros, titul. y asig. pendientes' WHERE ActCod='1060';

	Version 13.74.1:  Jul 05, 2014	Changes in edition of degrees. (163670 lines)
	Version 13.74:    Jul 05, 2014	All users can create new degrees. (163656 lines)
					5 changes necessary in database:
ALTER TABLE degrees ADD COLUMN Status TINYINT NOT NULL DEFAULT 0 AFTER DegTypCod;
ALTER TABLE degrees ADD COLUMN RequesterUsrCod INT NOT NULL DEFAULT -1 AFTER Status;
CREATE INDEX Status ON degrees (Status);
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1206','es','N','Solicitar titulaci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1207','es','N','Cambiar estado titulaci&oacute;n');

	Version 13.73.3:  Jul 04, 2014	Changes in editing institutions. (163272 lines)
	Version 13.73.2:  Jul 03, 2014	Changes in editing degrees.
	                                Changes in editing centres. (163266 lines)
	Version 13.73.1:  Jul 03, 2014	Changes in some table headers.
					Changes in editing courses. (163289 lines)
	Version 13.73:    Jul 02, 2014	Refactoring file browser. (163659 lines)
					15 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1195','es','N','Cambiar a ver documentos');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1196','es','N','Cambiar a admin. documentos');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1197','es','N','Cambiar a admin. archivos compartidos');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1198','es','N','Cambiar a ver archivos calificaciones');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1199','es','N','Cambiar a admin. archivos calificaciones');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1078','es','N','Ver documentos asignatura');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1200','es','N','Ver documentos grupo');
UPDATE actions SET Txt='Administrar documentos asignatura' WHERE ActCod='12';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1201','es','N','Administrar documentos grupo');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1202','es','N','Administrar archivos compartidos asignatura');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1203','es','N','Administrar archivos compartidos grupo');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1079','es','N','Ver calificaciones asignatura');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1204','es','N','Ver calificaciones grupo');
UPDATE actions SET Txt='Administrar calificaciones asignatura' WHERE ActCod='284';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1205','es','N','Administrar calificaciones grupo');

	Version 13.72.4:  Jul 01, 2014	Refactoring file browser. (163606 lines)
	Version 13.72.3:  Jun 30, 2014	Internal changes in color strings.
					New type of alert: success. (163676 lines)
	Version 13.72.2:  Jun 30, 2014	Refactoring file browser. (163679 lines)
	Version 13.72.1:  Jun 30, 2014	Refactoring file browser. (163665 lines)
	Version 13.72:    Jun 29, 2014	Refactoring file browser. (163851 lines)
					4 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1191','es','N','Habilitar arch/carp calif. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1192','es','N','Inhabilitar arch/carp calif. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1193','es','N','Habilitar arch/carp calif. asig.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1194','es','N','Inhabilitar arch/carp calif. asig.');

	Version 13.71:    Jun 25, 2014	Refactoring file browser. (163583 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod IN ('267','268','463','466','467','475','499','500','525','526');

	Version 13.70:    Jun 24, 2014	Selection of course/group in file browser with radio buttons.
					Fixed bug in list of intitution forums. (163628 lines)
	Version 13.69.4:  Jun 23, 2014	Selection of course/group in file browser with radio buttons. (163596 lines)
	Version 13.69.3:  Jun 08, 2014	SWADroid advertisement on right column. (163574 lines)
	Version 13.69.2:  Jun 08, 2014	Fixed bug in web service, reported by Juan Miguel Boyero Corral. (163539 lines)
	Version 13.69.1:  Jun 07, 2014	Now it is not necessary to create places in an institution before creating centres. (163538 lines)
	Version 13.69:    Jun 07, 2014	Changes in listing of invited users.
					Code refactoring in list of users. (163587 lines)
					7 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1078','es','N','Ver documentos');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1079','es','N','Ver calificaciones');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1186','es','N','Ver orla o lista de invitados');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1187','es','N','Ver fichas invitados');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1188','es','N','Imprimir fichas invitados');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1189','es','N','Ver lista invitados');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1190','es','N','Imprimir orla ionvitados');

	Version 13.68:    Jun 07, 2014	Records of invited users. (164189 lines)
	Version 13.67:    Jun 06, 2014	List of invited users.
					Institution fullname length changes from 127 to 1024. (163975 lines)
					1 change necessary in database:
ALTER TABLE institutions CHANGE COLUMN FullName FullName TEXT NOT NULL;

	Version 13.66.1:  Jun 02, 2014	Fixed errors in search. (163637 lines)
	Version 13.66:    Jun 02, 2014	Search option in several tabs.
					Changes in search scopes.
					Fixed bug in forms. (163737 lines)
					12 changes necessary in database:
UPDATE actions SET Txt='Solicitar b&uacute;squeda desde plataforma' WHERE ActCod='627';
UPDATE actions SET Txt='Buscar desde plataforma' WHERE ActCod='628';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1176','es','N','Solicitar b&uacute;squeda desde pa&iacute;s');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1177','es','N','Solicitar b&uacute;squeda desde instituci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1178','es','N','Solicitar b&uacute;squeda desde centro');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1179','es','N','Solicitar b&uacute;squeda desde titulaci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1180','es','N','Solicitar b&uacute;squeda desde asignatura');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1181','es','N','Buscar desde pa&iacute;s');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1182','es','N','Buscar desde instituci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1183','es','N','Buscar desde centro');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1184','es','N','Buscar desde titulaci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1185','es','N','Buscar desde asignatura');

	Version 13.65:    May 31, 2014	Changes related to institution logo. (162951 lines)
	Version 13.64.9:  May 30, 2014	Changes in edition of institutions. (163002 lines)
					1 change necessary in database:
ALTER TABLE institutions CHANGE Logo Logo VARCHAR(32) NOT NULL;

	Version 13.64.8:  May 27, 2014	Changes in institution logos. (162985 lines)
	Version 13.64.7:  May 26, 2014	Added new social networks. (? lines)
					1 change necessary in database:
ALTER TABLE usr_webs CHANGE Web Web ENUM('www','delicious','edmodo','facebook','flickr','foursquare','googleplus','googlescholar','instagram','linkedin','paperli','pinterest','researchgate','scoopit','slideshare','storify','tumblr','twitter','wikipedia','youtube') NOT NULL;

	Version 13.64.6:  May 26, 2014	Fixed bug in new automatic passwords, reported by Carolina Gonz&aacute;lez Madrigal. (162940 lines)
	Version 13.64.5:  May 22, 2014	Added new social networks. (162908 lines)
					1 change necessary in database:
ALTER TABLE usr_webs CHANGE Web Web ENUM('www','edmodo','facebook','flickr','googleplus','googlescholar','instagram','linkedin','paperli','pinterest','researchgate','scoopit','slideshare','storify','tumblr','twitter','wikipedia','youtube') NOT NULL;

	Version 13.64.4:  May 15, 2014	Google Analytics for openswad.org. (162892 lines)
	Version 13.64.3:  May 14, 2014	Changes in layout.
					Changing swad_smtp.py (not finished). (162884 lines)
	Version 13.64.2:  May 06, 2014	Fixed minor bug when creating a new notice, reported by Eva Mart&iacute;nez Ortigosa. (162825 lines)
	Version 13.64.1:  May 06, 2014	Internal changes to actions and tabs.
					Icons for countries are moved to icons directory. (162825 lines)
					1 change necessary:
mv /var/www/html/swad/cty /var/www/html/swad/icon/country

	Version 13.64:    May 05, 2014	Changes in tabs.
					Search/select is changed to search only.
					Removed messages when no country/institution/centre/degree selected. (162836 lines)
	Version 13.63:    May 04, 2014	Tabs are reduced from 11 to 6. (163163 lines)
	Version 13.62.3:  May 03, 2014	Fixed bug in list of departments, reported by Francisco A. Oca�a Lara. (163116 lines)
	Version 13.62.2:  May 03, 2014	Fixed bug in swad.sql. (163115 lines)
	Version 13.62.1:  Apr 26, 2014	Added new social network. (163115 lines)
					1 change necessary in database:
ALTER TABLE usr_webs CHANGE Web Web ENUM('www','edmodo','facebook','flickr','googleplus','googlescholar','instagram','linkedin','pinterest','researchgate','slideshare','tumblr','twitter','wikipedia','youtube') NOT NULL;

	Version 13.62:    Apr 24, 2014	Fixed important bug in forms. (163103 lines)
	Version 13.61.4:  Apr 23, 2014	Some messages translated. (163102 lines)
	Version 13.61.3:  Apr 23, 2014	Fixed bug when removing a user.
					Changed icons for administer one / several users.
					Link in user's record to user's administration. (163079 lines)
					1 change necessary in database:
DELETE FROM usr_webs WHERE UsrCod NOT IN (SELECT UsrCod FROM usr_data);

	Version 13.61.2:  Apr 23, 2014	Fixed bug in file browser when the first level is hidden. (163053 lines)
	Version 13.61.1:  Apr 22, 2014	Added new social network. (163051 lines)
					1 change necessary in database:
ALTER TABLE usr_webs CHANGE Web Web ENUM('www','edmodo','facebook','flickr','googleplus','instagram','linkedin','pinterest','slideshare','tumblr','twitter','youtube') NOT NULL;

	Version 13.61:    Apr 21, 2014	Fixed bug in links of right menu to users' records.
					Link to user's record from works file browser. (163045 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1174','es','N','Ver ficha estudiante');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1175','es','N','Ver ficha profesor');

	Version 13.60.1:  Apr 21, 2014	Fixed bug in links of right menu.
					Fixed bug in institution.
					A user can write a message to another from user's record. (162992 lines)
	Version 13.60:    Apr 21, 2014	Students can view restricted versions of records of other students.
					Links to records in connected users. (162954 lines)
	Version 13.59.6:  Apr 21, 2014	Fixed bug in statistics about webs / social networks. (162993 lines)
	Version 13.59.5:  Apr 20, 2014	Changes in statistics about webs / social networks. (162992 lines)
	Version 13.59.4:  Apr 20, 2014	Fixed bug in statistics about webs / social networks. (162968 lines)
	Version 13.59.3:  Apr 20, 2014	Fixed bug when registering a user. (162967 lines)
	Version 13.59.2:  Apr 20, 2014	Added new social network. (162963 lines)
					1 change necessary in database:
ALTER TABLE usr_webs CHANGE Web Web ENUM('www','facebook','flickr','googleplus','instagram','linkedin','pinterest','slideshare','tumblr','twitter','youtube') NOT NULL;

	Version 13.59.1:  Apr 17, 2014	Changes in webs / social networks. (162916 lines)
	Version 13.59:    Apr 16, 2014	WWW, Skype and Twitter are removed from user's data.
					Links to users' web pages removed. (162914 lines)
					2 changes necessary in database:
REPLACE INTO usr_webs SELECT UsrCod,'www',WWW FROM usr_data WHERE WWW LIKE 'http%';
REPLACE INTO usr_webs SELECT UsrCod,'twitter',CONCAT('https://twitter.com/',Twitter) FROM usr_data WHERE Twitter<>'' AND Twitter NOT LIKE 'http%';

	Version 13.58.10: Apr 16, 2014	Changes in user's institution, centre and department. (163370 lines)
	Version 13.58.9:  Apr 16, 2014	Changes in user's common record. (163385 lines)
	Version 13.58.8:  Apr 15, 2014	Added new social networks. (163375 lines)
					1 change necessary in database:
ALTER TABLE usr_webs CHANGE Web Web ENUM('www','facebook','flickr','googleplus','instagram','linkedin','pinterest','slideshare','twitter','youtube') NOT NULL;

	Version 13.58.7:  Apr 14, 2014	Skype is removed from web / social networks. (163387 lines)
					2 changes necessary in database:
DELETE FROM usr_webs WHERE Web='skype';
ALTER TABLE usr_webs CHANGE Web Web ENUM('www','facebook','flickr','googleplus','linkedin','twitter','youtube') NOT NULL;

	Version 13.58.6:  Apr 14, 2014	Webs / social networks are shown in user's record. (163390 lines)
	Version 13.58.5:  Apr 14, 2014	Editing social networks. (163346 lines)
	Version 13.58.4:  Apr 13, 2014	Editing social networks. (163337 lines)
					1 change necessary in database:
CREATE TABLE IF NOT EXISTS usr_webs (UsrCod INT NOT NULL,Web ENUM('www','facebook','flickr','googleplus','linkedin','skype','twitter','youtube') NOT NULL,URL VARCHAR(255) NOT NULL,UNIQUE INDEX(UsrCod,Web));

	Version 13.58.3:  Apr 11, 2014	New module swad_network for user' web and social networks. (163286 lines)
					1 change necessary in Makefile:
Add swad_network.o to list of object files

	Version 13.58.2:  Apr 10, 2014	Editing social networks. (? lines)
	Version 13.58.1:  Apr 09, 2014	Editing social networks. (163222 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1173','es','N','Cambiar redes sociales');

	Version 13.58:    Apr 08, 2014	New action to edit social networks. (163144 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1172','es','N','Editar redes sociales');

	Version 13.57.6:  Apr 08, 2014	Changes in edition of user's common record. (163011 lines)
	Version 13.57.5:  Apr 07, 2014	Changes in user's institution, centre and department. (163027 lines)
					10 changes necessary in database:
ALTER TABLE usr_data ADD COLUMN InsCtyCod INT NOT NULL DEFAULT -1 AFTER CtyCod;
ALTER TABLE usr_data ADD INDEX (InsCtyCod);
UPDATE usr_data,institutions SET usr_data.InsCtyCod=institutions.CtyCod WHERE usr_data.InsCod=institutions.InsCod;
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1165','es','N','Editar instituci&oacute;n, centro, departamento');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1166','es','N','Cambiar pa&iacute;s instituci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1167','es','N','Cambiar instituci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1168','es','N','Cambiar centro');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1169','es','N','Cambiar departamento');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1170','es','N','Cambiar despacho');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1171','es','N','Cambiar tel&eacute;fono despacho');

	Version 13.57.4:  Apr 06, 2014	Edition of office and office phone are moved from common card to institution. (162969 lines)
	Version 13.57.3:  Apr 04, 2014	Changes in user's institution, centre and department. (162916 lines)
	Version 13.57.2:  Apr 02, 2014	Changes in user's institution, centre and department. (162943 lines)
	Version 13.57.1:  Apr 01, 2014	Changes in user's institution, centre and department. (162846 lines)
	Version 13.57:    Apr 01, 2014	User's institution, centre and department are separated from common record. (162679 lines)
	Version 13.56.2:  Mar 30, 2014	Changes in notices.
					Fixed bug in syllabus. (162587 lines)
	Version 13.56.1:  Mar 30, 2014	Changes in notices. (162594 lines)
	Version 13.56:    Mar 29, 2014	Notices in the left side of the screen are limited to a number of charecters. (162516 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1164','es','N','Mostrar aviso completo');

	Version 13.55.8:  Mar 25, 2014	Fixed bugs in web service function getCourseInfo. (162405 lines)
	Version 13.55.7:  Mar 25, 2014	Fixed bugs in web service function getCourseInfo reported by Jos� Antonio Guerrero Avil�s. (162486 lines)
	Version 13.55.6:  Mar 24, 2014	Changes in course info related to web service function getCourseInfo.
					Changes in icons for notifications. (162358 lines)
	Version 13.55.5:  Mar 22, 2014	Changes in course info related to web service function getCourseInfo. (162265 lines)
	Version 13.55.4:  Mar 22, 2014	Changes in course info related to web service function getCourseInfo. (162235 lines)
	Version 13.55.3:  Mar 21, 2014	Changes in syllabus related to web service function getCourseInfo. (162160 lines)
	Version 13.55.2:  Mar 20, 2014	Changes in syllabus related to web service function getCourseInfo. (162145 lines)
	Version 13.55.1:  Mar 19, 2014	Changes in tests. (162210 lines)
					1 change necessary in database:
UPDATE tst_questions SET Feedback='' WHERE (Feedback='(null)' OR Feedback='&#40;null&#41;');

	Version 13.55:    Mar 19, 2014	Fixed bug in tests, reported by Amalia Morales.
					Database queries with NOT EXISTS replaced by NOT IN. (162149 lines)
	Version 13.54.1:  Mar 18, 2014	Internal changes in syllabus.
					Syllabus is written to temporary file to implement web service getCourseInfo. (162013 lines)
	Version 13.54:    Mar 12, 2014	Partially implemented web service function getCourseInfo. (161958 lines)
	Version 13.53:    Mar 11, 2014	Implemented web service function getNewPassword. (161861 lines)
	Version 13.52.1:  Mar 09, 2014	Error 503 returned when server is in maintenance. (161737 lines)
	Version 13.52:    Mar 04, 2014	New module swad_ID for users' IDs management. (161730 lines)
					1 change necessary in Makefile:
Add swad_ID.o to list of object files

	Version 13.51.1:  Mar 04, 2014	Some functions moved to module swad_mail. (161632 lines)
	Version 13.51:    Mar 04, 2014	New module swad_nickname for nicknames management. (161632 lines)
					1 change necessary in Makefile:
Add swad_nickname.o to list of object files

	Version 13.50:    Mar 04, 2014	New module swad_password for passwords management. (161551 lines)
					1 change necessary in Makefile:
Add swad_password.o to list of object files

	Version 13.49.7:  Mar 03, 2014	Changes in some messages and buttons. (161466 lines)
	Version 13.49.6:  Feb 28, 2014	Leading zeros are removed in new users' IDs.
					Changes in functions that remove leading zeros/arrobas. (161411 lines)
	Version 13.49.5:  Feb 23, 2014	Fixed bug in list of forum threads. (161374 lines)
	Version 13.49.4:  Feb 21, 2014	Fixed problem in tabs layout with MSIE 9, reported by Mar�a Rosa Moreno-Torres Herrera. (161395 lines)
	Version 13.49.3:  Feb 18, 2014	By default, users only can view the notifications not seen. (161375 lines)
	Version 13.49.2:  Feb 16, 2014	"Without courses" is replaced by "guests" in connected and statistics. (161331 lines)
	Version 13.49.1:  Feb 15, 2014	Users' IDs not confirmed are shown in red.
					Statistics about visitors and guest users. (161372 lines)
	Version 13.49:    Feb 15, 2014	Changes in size of some database fields. (161301 lines)
					1 change necessary at University of Granada:
Compile and install version 1.1.13 of PRADO (includes changes in role codes for student and teacher)
					35 changes necessary in database:
DROP TABLE IF EXISTS usr_data_copy;
DROP TABLE IF EXISTS file_public;
DROP TABLE IF EXISTS files_backup;

ALTER TABLE usr_data CHANGE Surname1 Surname1 VARCHAR(32) COLLATE latin1_spanish_ci NOT NULL;
ALTER TABLE usr_data CHANGE Surname2 Surname2 VARCHAR(32) COLLATE latin1_spanish_ci NOT NULL;
ALTER TABLE usr_data CHANGE FirstName FirstName VARCHAR(32) COLLATE latin1_spanish_ci NOT NULL;
ALTER TABLE usr_data CHANGE Office Office VARCHAR(127) NOT NULL;
ALTER TABLE usr_data CHANGE LocalAddress LocalAddress VARCHAR(127) NOT NULL;
ALTER TABLE usr_data CHANGE FamilyAddress FamilyAddress VARCHAR(127) NOT NULL;
ALTER TABLE usr_data CHANGE OriginPlace OriginPlace VARCHAR(127) NOT NULL;

ALTER TABLE exam_announcements CHANGE CrsFullName CrsFullName VARCHAR(127) NOT NULL;
ALTER TABLE exam_announcements CHANGE ExamSession ExamSession VARCHAR(127) NOT NULL;

ALTER TABLE mail_domains CHANGE Domain Domain VARCHAR(127) NOT NULL;
ALTER TABLE mail_domains CHANGE Info Info VARCHAR(127) NOT NULL;

ALTER TABLE usr_emails CHANGE E_mail E_mail VARCHAR(127) COLLATE latin1_general_ci NOT NULL;
ALTER TABLE pending_emails CHANGE E_mail E_mail VARCHAR(127) COLLATE latin1_general_ci NOT NULL;

ALTER TABLE imported_students CHANGE Surname1 Surname1 VARCHAR(32) NOT NULL;
ALTER TABLE imported_students CHANGE Surname2 Surname2 VARCHAR(32) NOT NULL;
ALTER TABLE imported_students CHANGE FirstName FirstName VARCHAR(32) NOT NULL;
ALTER TABLE imported_students CHANGE E_mail E_mail VARCHAR(32) NOT NULL;

ALTER TABLE courses CHANGE FullName FullName VARCHAR(127) COLLATE latin1_spanish_ci NOT NULL;
ALTER TABLE degrees CHANGE FullName FullName VARCHAR(127) COLLATE latin1_spanish_ci NOT NULL;
ALTER TABLE centres CHANGE FullName FullName VARCHAR(127) COLLATE latin1_spanish_ci NOT NULL;
ALTER TABLE institutions CHANGE FullName FullName VARCHAR(127) COLLATE latin1_spanish_ci NOT NULL;
ALTER TABLE departments CHANGE FullName FullName VARCHAR(127) NOT NULL;
ALTER TABLE deg_types CHANGE DegTypName DegTypName VARCHAR(32) NOT NULL;
ALTER TABLE imported_groups CHANGE DegName DegName VARCHAR(127) NOT NULL;
ALTER TABLE imported_groups CHANGE CrsName CrsName VARCHAR(127) NOT NULL;
ALTER TABLE links CHANGE FullName FullName VARCHAR(127) NOT NULL;
ALTER TABLE places CHANGE FullName FullName VARCHAR(127) NOT NULL;
ALTER TABLE plugins CHANGE Name Name VARCHAR(127) NOT NULL;
ALTER TABLE timetable_crs CHANGE Place Place VARCHAR(127) NOT NULL;
ALTER TABLE timetable_tut CHANGE Place Place VARCHAR(127) NOT NULL;
ALTER TABLE banners CHANGE FullName FullName VARCHAR(127) NOT NULL;
ALTER TABLE holidays CHANGE Name Name VARCHAR(127) NOT NULL;

	Version 13.48.3:  Feb 09, 2014	Guest users without courses view documents the same way as unknown users.
					Guest users without courses can not create folders or files in their briefcases.
					Guest users without courses can not view their courses. (161259 lines)
	Version 13.48.2:  Feb 09, 2014	Guest users without courses can not view forums.
					Guest users without courses can not use chat.
					Guest users without courses can not send messages. (161239 lines)
	Version 13.48.1:  Feb 09, 2014	Action permissions adapted to new roles. (161310 lines)
	Version 13.48:    Feb 09, 2014	Role "invited" is renamed as "visitor", and new role "invited" is created for users without courses.
					Not finished! Changes in action permissions required! (161240 lines)
					1 change necessary at University of Granada:
Compile and install version 1.1.12 of PRADO (includes changes in role codes for student and teacher)
					15 changes necessary in database:
UPDATE connected SET RoleInLastCrs='8' WHERE RoleInLastCrs='5';
UPDATE connected SET RoleInLastCrs=RoleInLastCrs+1 WHERE RoleInLastCrs>='1' AND RoleInLastCrs<='4';

UPDATE crs_usr SET Role='4' WHERE Role='3';
UPDATE crs_usr SET Role='3' WHERE Role='2';

UPDATE crs_usr_requests SET Role='4' WHERE Role='3';
UPDATE crs_usr_requests SET Role='3' WHERE Role='2';

UPDATE imported_sessions SET ImportedRole='4' WHERE ImportedRole='3';
UPDATE imported_sessions SET ImportedRole='3' WHERE ImportedRole='2';

UPDATE sessions SET Role='8' WHERE Role='5';
UPDATE sessions SET Role=Role+1 WHERE Role>='1' AND Role<='4';

UPDATE surveys SET Roles=Roles*2;

UPDATE log_recent SET Role='8' WHERE Role='5';
UPDATE log_recent SET Role=Role+1 WHERE Role>='1' AND Role<='4';

UPDATE log SET Role='8' WHERE Role='5';
UPDATE log SET Role=Role+1 WHERE Role>='1' AND Role<='4';

	Version 13.47:    Feb 06, 2014	Changes related to login. (160145 lines)
	Version 13.46.7:  Feb 06, 2014	User's ID is confirmed when a user is logged in from external service. (160218 lines)
	Version 13.46.6:  Feb 06, 2014	Refactoring functions related to external login. (160162 lines)
	Version 13.46.5:  Feb 05, 2014	Refactoring functions related to register users. (160138 lines)
	Version 13.46.4:  Feb 05, 2014	Refactoring functions related to login. (160132 lines)
	Version 13.46.3:  Feb 05, 2014	To avoid duplications, a user is not registered when he/she comes from external login and does not exist.
					Refactoring functions related to login. (160128 lines)
	Version 13.46.2:  Feb 05, 2014	Changes in register / removing users. (160100 lines)
	Version 13.46.1:  Feb 04, 2014	If the maximum role of a user is "invited", he/she can not upload documents. (160079 lines)
	Version 13.46:    Feb 03, 2014	Removed long messages used to help edition. (160077 lines)
	Version 13.45.4:  Feb 03, 2014	Added some titles in forms. (161706 lines)
	Version 13.45.3:  Feb 02, 2014	Added some titles in forms. (161742 lines)
	Version 13.45.2:  Feb 02, 2014	Added some titles in forms. (161738 lines)
	Version 13.45.1:  Feb 02, 2014	Added some titles in forms. (161647 lines)
	Version 13.45:    Feb 01, 2014	Added some titles in forms. (161679 lines)
	Version 13.44.4:  Feb 01, 2014	Fixed bug in holidays.
					Added some titles in forms. (161477 lines)
	Version 13.44.3:  Jan 31, 2014	Minor changes related to password. (161462 lines)
	Version 13.44.2:  Jan 31, 2014	Minor changes related to login / logout. (161453 lines)
	Version 13.44.1:  Jan 30, 2014	Role option is integrated in session option. (161423 lines)
	Version 13.44:    Jan 30, 2014	Password is included in form to create a new account.
					Changes in logout. (161425 lines)
					2 changes necessary in database:
DELETE FROM actions WHERE ActCod='1164';
UPDATE actions SET Obsolete='Y' WHERE ActCod='1162';

	Version 13.43:    Jan 29, 2014	A new account must be created with a @nick, an email and an ID. (161513 lines)
	Version 13.42.3:  Jan 28, 2014	Changes related to user importation. (161385 lines)
	Version 13.42.2:  Jan 27, 2014	The number of user's IDs for a user is now limited. (161386 lines)
	Version 13.42.1:  Jan 26, 2014	Some messages translated. (161348 lines)
	Version 13.42:    Jan 26, 2014	New option to sign in. (161251 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1164','es','N','Confirmar creaci&oacute;n de nueva cuenta');

	Version 13.41:    Jan 23, 2014	Starting a new option to sign in. (161147 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1162','es','N','Solicitar la creaci&oacute;n de una cuenta');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1163','es','N','Crear una nueva cuenta');

	Version 13.40.1:  Jan 22, 2014	Changes in enrolment of users. (161007 lines)
	Version 13.40:    Jan 20, 2014	Changes in listing of countries.
					Removed options to go to country, institution, centre, etc. (160965 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod IN ('842','1027','1026','1050','1012','988','1010');

	Version 13.39.3:  Jan 20, 2014	Fixed several bugs related to user's login. (160944 lines)
	Version 13.39.2:  Jan 19, 2014	Allow login with ID if there are several users with the same ID, but only one with the password typed by the user.
					Code refactoring related to users' IDs. (160931 lines)
	Version 13.39.1:  Jan 18, 2014	Code refactoring in user's authentication. (160906 lines)
	Version 13.39:    Jan 16, 2014	Change in order from ID-@nick-email to @nick-email-ID.
					New module swad_photo for photo management. (160876 lines)
					1 change necessary in Makefile
Add swad_photo.o to list of object files

	Version 13.38.1:  Jan 15, 2014	Code refactoring in user's authentication. (160780 lines)
	Version 13.38:    Jan 14, 2014	Get list of users' codes for a given ID. (160712 lines)
	Version 13.37.2:  Jan 13, 2014	Get list of users' codes for a given ID. Not finished.
					Fixed bug when redirecting to another language, reported by Monserrat Bosch Olives. (160694 lines)
	Version 13.37.1:  Jan 13, 2014	Get list of users' codes for a given ID. Not finished. (160611 lines)
	Version 13.37:    Jan 12, 2014	Code refactoring related to enrolment of users. (160575 lines)
	Version 13.36:    Jan 11, 2014	When a teacher tries to enrol a user in a course and there are more than one user with the same ID, the teacher can choose what user to enrol. (160605 lines)
	Version 13.35.2:  Jan 11, 2014	Implemented function to remove one of my user's IDs. (160531 lines)
	Version 13.35.1:  Jan 11, 2014	Changes in layout and CSS. (160453 lines)
	Version 13.35:    Jan 11, 2014	Changes in CSS for icons. (160491 lines)
	Version 13.34.9:  Jan 10, 2014	Navigation bar (tabs) now uses a list and CSS. (160378 lines)
	Version 13.34.8:  Jan 09, 2014	Lot of minor fixes in XHTML. (160401 lines)
	Version 13.34.7:  Jan 09, 2014	Change in layout to fix misaligned central div in MSIE. (160406 lines)
	Version 13.34.6:  Jan 08, 2014	Changes in layout of left and right buttons to expand the central zone. (160396 lines)
	Version 13.34.5:  Jan 07, 2014	Changes in CSS for tabs.
					Changes in tab labels. (160389 lines)
	Version 13.34.4:  Jan 06, 2014	Changes in CSS for mobile layout. (160387 lines)
	Version 13.34.3:  Jan 04, 2014	Changes in CSS for desktop layout. (160439 lines)
	Version 13.34.2:  Jan 04, 2014	Changes in CSS for some lists. (160433 lines)
	Version 13.34.1:  Jan 02, 2014	Code refactoring related to users and groups. (160441 lines)
	Version 13.34:    Jan 02, 2014	Changes in register/remove one user.
					Code refactoring related to users and groups. (160399 lines)
	Version 13.33:    Jan 02, 2014	Changes in edition of record fields.
					Removed help messages in edition of record fields.
					A student can change his/her groups in option to register/remove user. (160414 lines)
	Version 13.32.5:  Jan 01, 2014	Removed help messages in edition of groups.
					Fixed bug in listing of students. (160698 lines)
	Version 13.32.4:  Jan 01, 2014	Changes in edition of groups. (161028 lines)
	Version 13.32.3:  Jan 01, 2014	Changes in class photos. (160981 lines)
	Version 13.32.2:  Jan 01, 2014	Removed no longer used functions related to records. (160960 lines)
	Version 13.32.1:  Jan 01, 2014	Changes in selection of teachers to show teachers' records. (161107 lines)
	Version 13.32:    Jan 01, 2014	Teachers' records are integrated into teachers' list. (161057 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='306';

	Version 13.31:    Dec 31, 2013	Enrolment tab is merged with users tab.
					Connected option is moved to the end of users tab. (161065 lines)
	Version 13.30.3:  Dec 31, 2013	Link to edit groups in option to select my groups. (161188 lines)
	Version 13.30.2:  Dec 31, 2013	Options to edit group types is integrated in option to edit groups. (161244 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='1004';

	Version 13.30.1:  Dec 31, 2013	Options to remove all students and old users are moved to option to register/remove several users. (161339 lines)
	Version 13.30:    Dec 30, 2013	Options to remove all students and old users are linked from option to register/remove several users. (161463 lines)
	Version 13.29:    Dec 29, 2013	Removing a user is integrated in register a user.
					Register / removing an administrator is integrated in register a user. (161391 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod IN ('57','61','229','231','580','581','582','585');

	Version 13.28:    Dec 29, 2013	Teachers's office hours are integrated in listing of teachers' records. (161581 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='21';

	Version 13.27.4:  Dec 28, 2013	Changes in current action when no user logged. (161608 lines)
	Version 13.27.3:  Dec 28, 2013	Changes in layout related to country, institution, centre, degree and course. (161592 lines)
	Version 13.27.2:  Dec 27, 2013	Changes in layout related to country, institution, centre, degree and course. (161570 lines)
	Version 13.27.1:  Dec 24, 2013	Fixed bug when getting place data from database. (161471 lines)
	Version 13.27:    Dec 24, 2013	Highlight of tabs and menu options. (161476 lines)
	Version 13.26.3:  Dec 24, 2013	Fixed bug when sending password. Reported by @Jasgrf via Twitter. (161209 lines)
	Version 13.26.2:  Dec 22, 2013	Country map attribution now uses a textarea and bigger size. (161207 lines)
	Version 13.26.1:  Dec 22, 2013	Fixed bug in authentication with ID.
					Changes in database fields for attribution.
					Centre photo attribution now uses a textarea and bigger size. (161205 lines)
					2 changes necessary in database:
ALTER TABLE centres CHANGE COLUMN PhotoAttribution PhotoAttribution TEXT NOT NULL;
ALTER TABLE countries CHANGE COLUMN MapAttribution MapAttribution TEXT NOT NULL;

	Version 13.26:    Dec 21, 2013	Form to send a photo of the current centre. (161141 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1160','es','N','Solicitar env&iacute;o de foto del centro');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1161','es','N','Enviar foto del centro');

	Version 13.25.1:  Dec 21, 2013	Changes in attribution of centre. (160998 lines)
					1 change necessary in database:
ALTER TABLE centres ADD COLUMN PhotoAttribution VARCHAR(255) NOT NULL AFTER WWW;

	Version 13.25:    Dec 20, 2013	In information about the current centre a photo is shown.
					New option to set the attribution of centre photo. (160970 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1159','es','N','Cambiar atribuci&oacute;n foto centro');

	Version 13.24:    Dec 18, 2013	New option to set the attribution of SVG map of country. (160885 lines)
					12 changes necessary in database:
ALTER TABLE countries ADD COLUMN MapAttribution VARCHAR(255) NOT NULL AFTER Alpha2;
ALTER TABLE countries ADD COLUMN WWW_ca VARCHAR(255) NOT NULL AFTER Name_pt;
ALTER TABLE countries ADD COLUMN WWW_de VARCHAR(255) NOT NULL AFTER WWW_ca;
ALTER TABLE countries ADD COLUMN WWW_en VARCHAR(255) NOT NULL AFTER WWW_de;
ALTER TABLE countries ADD COLUMN WWW_es VARCHAR(255) NOT NULL AFTER WWW_en;
ALTER TABLE countries ADD COLUMN WWW_fr VARCHAR(255) NOT NULL AFTER WWW_es;
ALTER TABLE countries ADD COLUMN WWW_gn VARCHAR(255) NOT NULL AFTER WWW_fr;
ALTER TABLE countries ADD COLUMN WWW_it VARCHAR(255) NOT NULL AFTER WWW_gn;
ALTER TABLE countries ADD COLUMN WWW_pl VARCHAR(255) NOT NULL AFTER WWW_it;
ALTER TABLE countries ADD COLUMN WWW_pt VARCHAR(255) NOT NULL AFTER WWW_pl;
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1157','es','N','Cambiar web pa&iacute;s');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1158','es','N','Cambiar atribuci&oacute;n mapa pa&iacute;s');

	Version 13.23.2:  Dec 16, 2013	Show SVG map of country if exists. (160656 lines)
Copy maps of countries from swad source directory to public directory.
Example:
cp -R /home/<user>/swad/swad/cty /var/www/html/swad/

	Version 13.23.1:  Dec 16, 2013	Changes in layour of tabs. (160631 lines)
	Version 13.23:    Dec 15, 2013	Options for places and holidays are moved from system tab to institution tab.
					Changes in tabs. (160630 lines)
					10 changes necessary in database:
ALTER TABLE places ADD COLUMN InsCod INT NOT NULL AFTER PlcCod, ADD INDEX (InsCod);
ALTER TABLE holidays ADD COLUMN InsCod INT NOT NULL AFTER HldCod, ADD INDEX (InsCod);

	// ugr.es:
UPDATE places SET InsCod='1';
UPDATE holidays SET InsCod='1';

	// una.py:
UPDATE places SET InsCod='92';
UPDATE holidays SET InsCod='92';

ALTER TABLE centres ADD COLUMN PlcCod INT NOT NULL DEFAULT -1 AFTER InsCod, ADD INDEX (PlcCod);
DROP TABLE IF EXISTS tmp_centres_places;
CREATE TEMPORARY TABLE tmp_centres_places (InsCod INT NOT NULL,CtrCod INT NOT NULL,PlcCod INT NOT NULL,INDEX(InsCod),INDEX (CtrCod),INDEX(PlcCod)) SELECT DISTINCTROW centres.InsCod,centres.CtrCod,degrees.PlcCod FROM degrees,centres WHERE degrees.CtrCod=centres.CtrCod ORDER BY centres.InsCod,centres.CtrCod,degrees.PlcCod DESC;
UPDATE centres,tmp_centres_places SET centres.PlcCod=tmp_centres_places.PlcCod WHERE centres.CtrCod=tmp_centres_places.CtrCod AND centres.InsCod=tmp_centres_places.InsCod;
DROP TABLE tmp_centres_places;
ALTER TABLE degrees DROP COLUMN PlcCod;
	Version 13.22.4:  Dec 15, 2013	List only the departments of the current institution. (160514 lines)
	Version 13.22.3:  Dec 15, 2013	Option to show and edit departments is moved from system tab to institution tab. (160510 lines)
	Version 13.22.2:  Dec 15, 2013	Show number of departments in an institution. (160506 lines)
	Version 13.22.1:  Dec 14, 2013	Fixed minor bug in selection of centres.
					Some code refeactoring. (160483 lines)
	Version 13.22:    Dec 13, 2013	Information on country and institution. (160446 lines)
	Version 13.21:    Dec 13, 2013	Big title in heading show the country, the institution, the centre, the degree or the course.
					New options to view country, institution and centre configuration. (160040 lines)
					6 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1151','es','N','Ver informaci&oacute;n centro');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1152','es','N','Imprimir informaci&oacute;n centro');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1153','es','N','Ver informaci&oacute;n instituci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1154','es','N','Imprimir informaci&oacute;n instituci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1155','es','N','Ver informaci&oacute;n pa&iacute;s');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1156','es','N','Imprimir informaci&oacute;n pa&iacute;s');

	Version 13.20:    Dec 12, 2013	Course configuration is integrated with course introduction.
					New option to view degree configuration. (159558 lines)
					3 changes necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='1023';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1149','es','N','Ver configuraci&oacute;n titulaci&oacute;n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1150','es','N','Imprimir configuraci&oacute;n titulaci&oacute;n');

	Version 13.19:    Dec 11, 2013	Four new tabs: country, institution, centre, degree.
					My courses changes to profile tab. (159405 lines)
					7 changes necessary in database:
UPDATE usr_last SET LastTab=12 WHERE LastTab=8;
UPDATE usr_last SET LastTab=11 WHERE LastTab=7;
UPDATE usr_last SET LastTab=10 WHERE LastTab=6;
UPDATE usr_last SET LastTab=9 WHERE LastTab=5;
UPDATE usr_last SET LastTab=8 WHERE LastTab=4;
UPDATE usr_last SET LastTab=7 WHERE LastTab=3;
UPDATE usr_last SET LastTab=6 WHERE LastTab=2;

	Version 13.18.8:  Dec 10, 2013	Changes related to user's IDs. (158895 lines)
	Version 13.18.7:  Dec 10, 2013	Changes related to user's IDs. (158755 lines)
					2 changes necessary in database:
ALTER TABLE usr_IDs ADD COLUMN Confirmed ENUM('N','Y') NOT NULL DEFAULT 'N' AFTER CreatTime;
UPDATE usr_IDs SET Confirmed='Y';

	Version 13.18.6:  Dec 10, 2013	Changes related to user's IDs. (158724 lines)
	Version 13.18.5:  Dec 09, 2013	Changes related to user's IDs. Not finished. (158755 lines)
	Version 13.18.4:  Dec 07, 2013	Changes related to user's IDs. (158667 lines)
	Version 13.18.3:  Dec 05, 2013	User's ID removed from log. (158680 lines)
					2 slow changes necessary in database (may spend many minutes or even hours depending on the size of log tables):
ALTER TABLE log_recent DROP COLUMN UsrID;
ALTER TABLE log DROP COLUMN UsrID;

	Version 13.18.2:  Dec 03, 2013	Changes related to list of user's ID. (158681 lines)
	Version 13.18.1:  Dec 01, 2013	Changes in messages related to user's ID. (158640 lines)
	Version 13.18:    Dec 01, 2013	User's directories for activities and works are stored using user's code instead of user's ID. (158785 lines)
Important change necessary to move old works to new works:
Configure swad_convert_works.c (change defines), compile it and execute swad_convert_works as root.
Check if all works fine.

	Version 13.17:    Nov 26, 2013	User's private briefcase is stored using user's code instead of user's ID. (158598 lines)
Important change necessary to move old briefcases to new briefcases:
Configure swad_convert_BRIEFs.c (change defines), compile it and execute swad_convert_BRIEFs as root.
Check if all works fine, and then remove usr_backup private directory.

	Version 13.16:    Nov 26, 2013	User's private photo is stored using user's code instead of user's ID. (158473 lines)
Important change necessary to move old private photos to new private photos:
Configure swad_convert_photos.c (change defines), compile it and execute swad_convert_photos as root.
Check if all works fine, and then remove photo_backup private directory.

	Version 13.15.1:  Nov 24, 2013	Changes related to user's ID. (158319 lines)
					1 change necessary in database:
ALTER TABLE usr_data DROP COLUMN UsrID;

	Version 13.15:    Nov 22, 2013	New table for user's IDs. (158043 lines)
					4 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1147','es','N','Eliminar uno de mis ID');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1148','es','N','Crear un nuevo ID para m&iacute;');
CREATE TABLE IF NOT EXISTS usr_IDs (UsrCod INT NOT NULL,UsrID CHAR(16) NOT NULL,CreatTime DATETIME NOT NULL,UNIQUE INDEX(UsrCod,UsrID),INDEX(UsrID));
INSERT INTO usr_IDs (UsrCod,UsrID,CreatTime) SELECT UsrCod,UsrID,NOW() FROM usr_data WHERE UsrID<>'';

	Version 13.14.2:  Nov 22, 2013	It is mandatory to fill the nickname. (158034 lines)
	Version 13.14.2:  Nov 21, 2013	Changes in user's account related to QR codes. (157959 lines)
	Version 13.14.1:  Nov 19, 2013	Changes in web service functions getNotifications and markNotificationsAsRead. (157963 lines)
	Version 13.14:    Nov 19, 2013	New web service function markNotificationAsRead.
					Changes in database table of notifications. (157944 lines)
					1 change necessary in database:
ALTER TABLE notif ADD NtfCod INT NOT NULL AUTO_INCREMENT FIRST, ADD PRIMARY KEY (NtfCod);

	Version 13.13.11: Nov 17, 2013	Several icons changed from 12x12 to 16x16.
					Changes in messages and forums. (157880 lines)
Copy icons from swad source directory to icons public directory.

	Version 13.13.10: Nov 16, 2013	Several icons changed from 12x12 and 12x16 to 16x16. (157839 lines)
	Version 13.13.9:  Nov 16, 2013	Several icons changed from 12x12 and 12x16 to 16x16. (157835 lines)
	Version 13.13.8:  Nov 16, 2013	Several icons changed from 12x12 to 16x16. (157779 lines)
	Version 13.13.7:  Nov 15, 2013	Several icons changed from 12x12 to 16x16. (157725 lines)
	Version 13.13.6:  Nov 15, 2013	Several icons changed from 12x12 to 16x16. (157703 lines)
	Version 13.13.5:  Nov 15, 2013	Several icons changed from 12x12 to 16x16. (157696 lines)
	Version 13.13.4:  Nov 14, 2013	Link in course record to edit common record. (157644 lines)
	Version 13.13.3:  Nov 14, 2013	Link in common record to view record in course. (157613 lines)
	Version 13.13.2:  Nov 14, 2013	Edition of record fields is now integrated in listing of students. (157573 lines)
	Version 13.13.1:  Nov 14, 2013	Official students are now accessible from students' list. (157586 lines)
	Version 13.13:    Nov 13, 2013	Links in right column to view connected users. (157600 lines)
	Version 13.12:    Nov 13, 2013	New MIME type.
					Option for changing another user's password is integrated in modification of another user's data. (157508 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='162';

	Version 13.11:    Nov 08, 2013	Option for changing another user's photo is integrated in modification of another user's data.
					Superusers can add/modify another user when no course selected. (? lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='373';

	Version 13.10.2:  Nov 07, 2013	Several messages translated in timetables. (157580 lines)
	Version 13.10.1:  Nov 03, 2013	Changes in timetables. (157452 lines)
	Version 13.10:    Nov 02, 2013	Users can mark all notifications as seen.
					Notifications are no longer marked as seen when mobile external service get them. (157443 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1146','es','N','Marcar notificaciones como vistas');

	Version 13.9.1:   Nov 02, 2013	Timetables from monday to sunday. (157408 lines)
					2 changes necessary in database:
ALTER TABLE timetable_crs CHANGE COLUMN Day Day ENUM('L','M','X','J','V','S','D') NOT NULL;
ALTER TABLE timetable_tut CHANGE COLUMN Day Day ENUM('L','M','X','J','V','S','D') NOT NULL;

	Version 13.9:     Nov 02, 2013	Edition of tutorial hours is integrated in my timetable. (157395 lines)
	Version 13.8:     Nov 02, 2013	Tutorial hours are shown in my timetable. (157480 lines)
	Version 13.7.10:  Nov 01, 2013	Changes in my record. (157457 lines)
	Version 13.7.9:   Nov 01, 2013	Changes in edition of test questions. (157439 lines)
	Version 13.7.8:   Oct 31, 2013	Changes in common record. (157420 lines)
	Version 13.7.7:   Oct 31, 2013	Changes in common record, unfinished. (157410 lines)
	Version 13.7.6:   Oct 28, 2013	Changes in common record, unfinished. (157447 lines)
	Version 13.7.5:   Oct 26, 2013	Statistics of clicks in banners. (157367 lines)
	Version 13.7.4:   Oct 26, 2013	Changes in banners. (157187 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1145','es','N','Clic en un banner');

	Version 13.7.3:   Oct 24, 2013	Clicks on banners are logged. (157180 lines)
					1 change necessary in database:
CREATE TABLE IF NOT EXISTS log_banners (LogCod INT NOT NULL,BanCod INT NOT NULL,UNIQUE INDEX(LogCod),INDEX(BanCod));

	Version 13.7.2:   Oct 21, 2013	Banners are shown in right column. (157117 lines)
	Version 13.7.1:   Oct 21, 2013	Changes in banners. (157116 lines)
					9 changes necessary in database:
ALTER TABLE banners ADD COLUMN Img VARCHAR(255) NOT NULL AFTER FullName;
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1137','es','N','Ver banners');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1138','es','N','Editar banners');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1139','es','N','Crear banner');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1140','es','N','Eliminar banner');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1141','es','N','Cambiar nombre corto de banner');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1142','es','N','Cambiar nombre largo de banner (');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1143','es','N','Cambiar URL de banner');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1144','es','N','Cambiar imagen de banner');

	Version 13.7:     Oct 20, 2013	New module swad_banner for banners. (156936 lines)
					1 change necessary in Makefile
Add swad_banner.o to list of object files
					1 change necessary in database:
CREATE TABLE IF NOT EXISTS banners (BanCod INT NOT NULL AUTO_INCREMENT,ShortName VARCHAR(32) NOT NULL,FullName VARCHAR(255) NOT NULL,WWW VARCHAR(255) NOT NULL,UNIQUE INDEX(BanCod));

	Version 13.6:     Oct 20, 2013	A teacher/admin can register another user in groups. (155736 lines)
	Version 13.5:     Oct 17, 2013	Register / remove several users using nicknames or emails, not only IDs. (155339 lines)
	Version 13.4.8:   Oct 16, 2013	Fixed bug in searching of documents. (155326 lines)
	Version 13.4.7:   Oct 16, 2013	Creating ZIP is permitted for all users. (155305 lines)
	Version 13.4.6:   Oct 16, 2013	Create ZIP only if full size is less than a limit. (155281 lines)
	Version 13.4.5:   Oct 16, 2013	Changes in ZIP of works and folders. (155227 lines)
	Version 13.4.4:   Oct 15, 2013	Changes in ZIP of a folder. (155152 lines)
	Version 13.4.3:   Oct 15, 2013	Update number of my views of files inside a folder when creating a ZIP file of that folder. (155148 lines)
	Version 13.4.2:   Oct 14, 2013	Compress files of a folder to download them. (155046 lines)
					12 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1125','es','N','Crear ZIP doc. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1126','es','N','Crear ZIP admin.doc.asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1127','es','N','Crear ZIP admin.doc.grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1128','es','N','Crear ZIP compart.asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1129','es','N','Crear ZIP compart.grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1130','es','N','Crear ZIP carpeta mis activid.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1131','es','N','Crear ZIP carpeta mis trabajos');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1132','es','N','Crear ZIP carpeta activid. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1133','es','N','Crear ZIP carpeta trabajos asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1134','es','N','Crear ZIP carpeta admin. calif. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1135','es','N','Crear ZIP carpeta admin. calif. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1136','es','N','Crear ZIP carpeta malet&iacute;n');

	Version 13.4.1:   Oct 14, 2013	Compress files of a folder to download them. (154976 lines)
	Version 13.4:     Oct 13, 2013	New module swad_zip for compressing files.
					Starting new button to ZIP folders for downloading in future versions. (154835 lines)
					1 change necessary in Makefile
Add swad_zip.o to list of object files
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1124','es','N','Crear ZIP doc. asg.');

	Version 13.3.2:   Oct 12, 2013	Clicks to download files are marked in yellow in last clicks.
					Different holidays can have the same name. (154636 lines)
	Version 13.3.1:   Oct 11, 2013	Files are downloaded directly by CGI in all zones. (154668 lines)
					12 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1112','es','N','Descargar arch. doc. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1113','es','N','Descargar arch. doc. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1114','es','N','Descargar arch. doc. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1115','es','N','Descargar arch. com. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1116','es','N','Descargar arch. com. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1117','es','N','Descargar arch. mis actividades');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1118','es','N','Descargar arch. mis trabajos');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1119','es','N','Descargar arch. activ. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1120','es','N','Descargar arch. trab. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1121','es','N','Descargar arch. admin.calif.asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1122','es','N','Descargar arch. admin.calif.grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1123','es','N','Descargar arch. malet&iacute;n');

	Version 13.3:     Oct 10, 2013	Files are downloaded directly by CGI in documents zone. (154772 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1111','es','N','Descargar arch. doc. asg.');

	Version 13.2.2:   Oct 09, 2013	User's QR code is moved to user's account.
					New QR code for confirmed email. (154443 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='1021';

	Version 13.2.1:   Oct 08, 2013	Fixed minor bug when updating last access to courses. (154537 lines)
	Version 13.2:     Oct 08, 2013	New option to remove old courses. (154532 lines)
					5 changes necessary in database:
CREATE TABLE IF NOT EXISTS crs_last (CrsCod INT NOT NULL,LastTime DATETIME NOT NULL DEFAULT 0,UNIQUE INDEX(CrsCod),INDEX(LastTime));
REPLACE INTO crs_last (CrsCod,LastTime) SELECT CrsCod,MAX(ClickTime) FROM log WHERE Role>='2' GROUP BY CrsCod;
DELETE FROM crs_last WHERE CrsCod NOT IN (SELECT CrsCod FROM courses);
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1109','es','N','Solicitar elim. asignaturas antiguas');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1110','es','N','Eliminar asignaturas antiguas');

	Version 13.1.2:   Oct 04, 2013	Changes in temporary directories for download. (154222 lines)
	Version 13.1.1:   Oct 02, 2013	Fixed bug in temporary directories for download. (154174 lines)
	Version 13.1:     Oct 01, 2013	New MIME type.
					Files are downloaded automatically (without pop-up blocking) (154161 lines)
	Version 13.0.1:   Sep 25, 2013	Files are downloaded automatically. (154138 lines)
	Version 13.0:     Sep 23, 2013	Users can choose which notifications they want to receive. (154129 lines)
					2 changes necessary in database:
ALTER TABLE usr_data CHANGE NotifyEvents EmailNtfEvents INT NOT NULL DEFAULT 0;
ALTER TABLE usr_data ADD COLUMN NotifNtfEvents INT NOT NULL DEFAULT -1 AFTER SecureIcons;

	Version 12.70:    Sep 22, 2013	New option in course info to test a new WYSIWYG text editor. (154041 lines)
					40 changes necessary in database:
ALTER TABLE crs_info_src ADD COLUMN InfoSrc2 ENUM('none','editor','plain_text','rich_text','page','URL') NOT NULL AFTER InfoSrc;
UPDATE crs_info_src SET InfoSrc2='none' WHERE InfoSrc='none';
UPDATE crs_info_src SET InfoSrc2='editor' WHERE InfoSrc='editor';
UPDATE crs_info_src SET InfoSrc2='plain_text' WHERE InfoSrc='text';
UPDATE crs_info_src SET InfoSrc2='page' WHERE InfoSrc='page';
UPDATE crs_info_src SET InfoSrc2='URL' WHERE InfoSrc='URL';
ALTER TABLE crs_info_src DROP COLUMN InfoSrc;
ALTER TABLE crs_info_src CHANGE COLUMN InfoSrc2 InfoSrc ENUM('none','editor','plain_text','rich_text','page','URL') NOT NULL;

UPDATE actions SET Txt='Editor de texto plano de info. asg.'   WHERE ActCod='850' AND Language='es';
UPDATE actions SET Txt='Editor de texto plano de gu�a docente' WHERE ActCod='787' AND Language='es';
UPDATE actions SET Txt='Editor de texto plano de teor�a'       WHERE ActCod='379' AND Language='es';
UPDATE actions SET Txt='Editor de texto plano de pr�cticas'    WHERE ActCod='389' AND Language='es';
UPDATE actions SET Txt='Editor de texto plano de bibliograf�a' WHERE ActCod='377' AND Language='es';
UPDATE actions SET Txt='Editor de texto plano de FAQ'          WHERE ActCod='405' AND Language='es';
UPDATE actions SET Txt='Editor de texto plano de enlaces'      WHERE ActCod='400' AND Language='es';
UPDATE actions SET Txt='Editor de texto plano de evaluaci�n'   WHERE ActCod='387' AND Language='es';

UPDATE actions SET Txt='Enviar texto plano de info. asg.'   WHERE ActCod='851' AND Language='es';
UPDATE actions SET Txt='Enviar texto plano de gu�a docente' WHERE ActCod='790' AND Language='es';
UPDATE actions SET Txt='Enviar texto plano de teor�a'       WHERE ActCod='394' AND Language='es';
UPDATE actions SET Txt='Enviar texto plano de pr�cticas'    WHERE ActCod='396' AND Language='es';
UPDATE actions SET Txt='Enviar texto plano de bibliograf�a' WHERE ActCod='398' AND Language='es';
UPDATE actions SET Txt='Enviar texto plano de FAQ'          WHERE ActCod='406' AND Language='es';
UPDATE actions SET Txt='Enviar texto plano de enlaces'      WHERE ActCod='401' AND Language='es';
UPDATE actions SET Txt='Enviar texto plano de evaluaci�n'   WHERE ActCod='397' AND Language='es';

INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1093','es','N','Editor de texto enriq. de info. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1094','es','N','Editor de texto enriq. de gu�a docente');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1095','es','N','Editor de texto enriq. de teor�a');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1096','es','N','Editor de texto enriq. de pr�cticas');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1097','es','N','Editor de texto enriq. de bibliograf�a');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1098','es','N','Editor de texto enriq. de FAQ');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1099','es','N','Editor de texto enriq. de enlaces');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1100','es','N','Editor de texto enriq. de evaluaci�n');

INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1101','es','N','Enviar texto enriq. de info. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1102','es','N','Enviar texto enriq. de gu�a docente');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1103','es','N','Enviar texto enriq. de teor�a');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1104','es','N','Enviar texto enriq. de pr�cticas');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1105','es','N','Enviar texto enriq. de bibliograf�a');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1106','es','N','Enviar texto enriq. de FAQ');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1107','es','N','Enviar texto enriq. de enlaces');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1108','es','N','Enviar texto enriq. de evaluaci�n');

	Version 12.69:    Sep 22, 2013	Statistics about use of icons. (153664 lines)
	Version 12.68.5:  Sep 20, 2013	Default icon set is changed from Nuvola to Awesome. (153535 lines)
	Version 12.68.4:  Sep 19, 2013	Internal changes in creation of new users. (153534 lines)
	Version 12.68.3:  Sep 19, 2013	Fixed bug in creation of new users. (153508 lines)
	Version 12.68.2:  Sep 19, 2013	Fixed bug in notifications about enrolment requests, reported by Antonio Mart�nez L�pez. (153491 lines)
	Version 12.68.1:  Sep 18, 2013	Fixed bug in icon set. (153479 lines)
	Version 12.68:    Sep 18, 2013	Users can choose two icon sets: Awesome and Nuvola. (153475 lines)
Create directory for icon set in directory of icons:

cd <public_swad_directory>/icon
mkdir iconset
cd iconset
mkdir awesome
mkdir nuvola
cp -a ../action16x16 awesome
cp -a ../action32x32 awesome
cp -a ../action64x64 awesome
cp -a ../action16x16 nuvola
cp -a ../action32x32 nuvola
cp -a ../action64x64 nuvola

					5 changes necessary in database:
ALTER TABLE usr_data ADD COLUMN IconSet CHAR(16) NOT NULL AFTER Theme, ADD INDEX (IconSet);
UPDATE usr_data SET IconSet='nuvola';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1092','es','N','Cambiar conjunto de iconos');
ALTER TABLE IP_prefs ADD COLUMN IconSet CHAR(16) NOT NULL AFTER Theme;
UPDATE IP_prefs SET IconSet='nuvola';

	Version 12.67.6:  Sep 17, 2013	Changes in layout. (153199 lines)
	Version 12.67.5:  Sep 15, 2013	Changes in layout. (153161 lines)
	Version 12.67.4:  Sep 14, 2013	Changes in layout. (153175 lines)
	Version 12.67.3:  Sep 14, 2013	Fixed bug in form to enter ID, @nick or email. (153198 lines)
	Version 12.67.2:  Sep 14, 2013	Fixed bug in statistics. (153198 lines)
	Version 12.67.1:  Sep 14, 2013	Changes in nickname form.
					Several messages translated. (153197 lines)
	Version 12.67:    Sep 13, 2013	Nickname and email are allowed to create, remove, change a user. (153137 lines)
	Version 12.66.1:  Sep 12, 2013	User's encrypted code is used instead user's ID when possible. (153107 lines)
	Version 12.66:    Sep 11, 2013	User's code instead of user's ID in log tables. (153015 lines)
					4 slow changes necessary in database (may spend many minutes or even hours depending on the size of log tables):
ALTER TABLE log_recent ADD COLUMN UsrCod INT NOT NULL DEFAULT -1 AFTER CrsCod, ADD INDEX (UsrCod);
ALTER TABLE log ADD COLUMN UsrCod INT NOT NULL DEFAULT -1 AFTER CrsCod, ADD INDEX (UsrCod);
UPDATE log_recent,usr_data SET log_recent.UsrCod=usr_data.UsrCod WHERE log_recent.UsrCod='-1' AND log_recent.UsrID<>'' AND log_recent.UsrID=usr_data.UsrID;
UPDATE log,usr_data SET log.UsrCod=usr_data.UsrCod WHERE log.UsrCod='-1' AND log.UsrID<>'' AND log.UsrID=usr_data.UsrID;

	Version 12.65.4: Sep 10, 2013	Shortcut to degree. (152979 lines)
	Version 12.65.3: Sep 06, 2013	New awesome-font icons. (152916 lines)
	Version 12.65.2: Sep 05, 2013	Hide test exam total result when no feedback is selected. (152909 lines)
	Version 12.65.1: Sep 04, 2013	Fixed bug when refreshing last clicks. (152898 lines)
	Version 12.65:   Sep 04, 2013	When a user logs in and no course is selected, use last visited course.
					Clear list of user's courses when he/she logs out. (152890 lines)
	Version 12.64.6: Jul 17, 2013	Fixed bug in mail domains.
					Changes in email form in user's account. (152848 lines)
	Version 12.64.5: Jun 29, 2013	Fixed bug in web service, reported by Juan Miguel Boyero Corral.
					New MIME type allowed. (152811 lines)
	Version 12.64.4: Jun 11, 2013	Users can log in web service with email. (152706 lines)
	Version 12.64.3: Jun 11, 2013	Fixed bug in nicknames, reported by Juan Miguel Boyero Corral. (152704 lines)
	Version 12.64.2: Jun 11, 2013	Several messages translated to spanish and english. (152715 lines)
	Version 12.64.1: Jun 11, 2013	New button to confirm email. (152335 lines)
	Version 12.64:   Jun 11, 2013	Users can login with email.
					Messages can be sent with email.
					Fixed bug when changing the email, reported by Francisco A. Oca�a Lara. (152361 lines)
	Version 12.63.12:Jun 10, 2013	Changes in user's record card.
					Email field removed from user's data table. (152320 lines)
					4 changes necessary in database:
DROP TABLE IF EXISTS usr_data_copy;
CREATE TABLE usr_data_copy LIKE usr_data;
INSERT INTO usr_data_copy SELECT * FROM usr_data;
ALTER TABLE usr_data DROP COLUMN E_mail;

	Version 12.63.11:Jun 06, 2013	Changes in user's record card.
					Nickname field removed from user's data table. (152303 lines)
					1 change necessary in database:
ALTER TABLE usr_data DROP COLUMN Nickname;

	Version 12.63.10:Jun 06, 2013	Changes in user's record card. (152251 lines)
	Version 12.63.9: May 29, 2013	Email address is no longer filled in user's record card, only in account.
					Nickname and email are shown in record card after user's ID. (152189 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1091','es','N','Confirmar direcci&oacute;n de correo');

	Version 12.63.8: May 29, 2013	Email address can be confirmed. (152163 lines)
	Version 12.63.7: May 29, 2013	Email address can be confirmed. (152117 lines)
	Version 12.63.6: May 28, 2013	Email address can be confirmed. (152109 lines)
					1 change necessary in database:
CREATE TABLE IF NOT EXISTS pending_emails (UsrCod INT NOT NULL,E_mail VARCHAR(255) COLLATE latin1_general_ci NOT NULL,MailKey CHAR(43) COLLATE latin1_bin NOT NULL,DateAndTime DATETIME NOT NULL,INDEX(UsrCod),UNIQUE INDEX(MailKey));

	Version 12.63.5: May 27, 2013	Email address can be confirmed. (152061 lines)
					1 change necessary in database:
ALTER TABLE usr_emails ADD COLUMN Confirmed ENUM('N','Y') NOT NULL DEFAULT 'N' AFTER CreatTime;

	Version 12.63.4: May 23, 2013	Several messages translated. (151958 lines)
	Version 12.63.3: May 23, 2013	Old nicknames can be removed. (151875 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1089','es','N','Eliminar apodo');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1090','es','N','Eliminar direcci&oacute;n de correo');

	Version 12.63.2: May 23, 2013	Changing user's email. (151732 lines)
					3 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1086','es','N','Ver datos arch. calif. asg.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1087','es','N','Ver datos arch. calif. grp.');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1088','es','N','Cambiar mi direcci&oacute;n de correo');

	Version 12.63.1: May 22, 2013	Changing user's email. (151625 lines)
	Version 12.63:   May 21, 2013	Option "Nickname" is changed to option "User's account". (151396 lines)
					1 change necessary in database:
RENAME TABLE nicknames TO usr_nicknames;
CREATE TABLE IF NOT EXISTS usr_emails (UsrCod INT NOT NULL,E_mail VARCHAR(255) COLLATE latin1_general_ci NOT NULL,CreatTime DATETIME NOT NULL,UNIQUE INDEX(UsrCod,E_mail),UNIQUE INDEX(E_mail));
INSERT IGNORE INTO usr_emails (UsrCod,E_mail,CreatTime) SELECT UsrCod,E_mail,NOW() FROM usr_data WHERE E_mail<>'';

	Version 12.62.5: May 16, 2013	Big warning icon for insuficient indicators in a course. (151188 lines)
	Version 12.62.4: May 13, 2013	Warning message about number of indicators in a course. (151179 lines)
	Version 12.62.3: May 10, 2013	Fixed bug in number of questions in tests, reported by Antonio Manuel Fern�ndez P�rez. (151151 lines)
	Version 12.62.2: May 10, 2013	Fixed bug when getting UNIX_TIMESTAMP fields from database and value returned is NULL.
					Changes in Makefile for MariaDB. (151156 lines)
					1 change necessary in Makefile:
Change old line:
LIBS = -lmysqlclient -lz -L/usr/lib64/mysql -lm -lgsoap
in Makefile to new line:
LIBS = -lssl -lcrypto -lpthread -lrt -lmysqlclient -lz -L/usr/lib64/mysql -lm -lgsoap

	Version 12.62.1: May 05, 2013	Change of order in options of menu statistics. (151145 lines)
	Version 12.62:   May 03, 2013	Default forums are local forums instead of all forums.
					New notification for post in course forums. (151144 lines)
					5 changes necessary in database:
UPDATE notif SET NotifyEvent=12 WHERE NotifyEvent=11;
UPDATE notif SET NotifyEvent=11 WHERE NotifyEvent=9;
UPDATE sta_notif SET NotifyEvent=12 WHERE NotifyEvent=11;
UPDATE sta_notif SET NotifyEvent=11 WHERE NotifyEvent=9;
UPDATE usr_data SET NotifyEvents=((NotifyEvents & b'10111111111') | ((NotifyEvents << 2) & b'100000000000') | ((NotifyEvents << 1) & b'1000000000000')) WHERE NotifyEvents<>0;

	Version 12.61.2: Apr 28, 2013	Changes in formatting of source code. (151021 lines)
	Version 12.61.1: Apr 24, 2013	Birthdays already congratulated are not congratulated again in the same day. (150434 lines)
					1 change necessary in database:
CREATE TABLE IF NOT EXISTS birthdays_today (UsrCod INT NOT NULL,Today DATE NOT NULL,UNIQUE INDEX(UsrCod),INDEX(Today));

	Version 12.61:   Apr 24, 2013	Added birthday wishes. (150387 lines)
	Version 12.60.2: Apr 23, 2013	Optimizations in user's last data.
					Fixed bug when removing old users. (150205 lines)
	Version 12.60.1: Apr 23, 2013	Fixed bug in user's last data. (150187 lines)
	Version 12.60:   Apr 22, 2013	User's data are splitted into two tables: usr_data (static data) and usr_last (dynamic data). (150169 lines)
					2 changes necessary in database:
CREATE TABLE IF NOT EXISTS usr_last (
	UsrCod INT NOT NULL,
	WhatToSearch TINYINT NOT NULL DEFAULT 0,
	LastCrs INT NOT NULL DEFAULT -1,
	LastTab TINYINT NOT NULL DEFAULT 0,
	LastTime DATETIME NOT NULL DEFAULT 0,
	LastAccNotif DATETIME NOT NULL DEFAULT 0,
	LastAccBriefcase DATETIME NOT NULL DEFAULT 0,
	UNIQUE INDEX(UsrCod),
	INDEX(LastTime)) SELECT UsrCod,WhatToSearch,LastCrs,LastTab,LastTime,LastAccNotif,LastAccBriefcase FROM usr_data;
ALTER TABLE usr_data DROP COLUMN WhatToSearch,DROP COLUMN LastCrs,DROP COLUMN LastTab,DROP COLUMN LastTime,DROP COLUMN LastAccNotif,DROP COLUMN LastAccBriefcase;

	Version 12.59.1: Apr 20, 2013	Last type of search is stored in user's data.
					Removed old translations. (150089 lines)
					1 change necessary in database:
ALTER TABLE usr_data ADD COLUMN WhatToSearch TINYINT NOT NULL DEFAULT 0 AFTER NotifyEvents;

	Version 12.59:   Apr 18, 2013	Last search is stored in session. (150294 lines)
					2 changes necessary in database:
ALTER TABLE sessions ADD COLUMN WhatToSearch TINYINT NOT NULL DEFAULT 0 AFTER LastPageMsgSnt;
ALTER TABLE sessions ADD COLUMN SearchString VARCHAR(255) NOT NULL AFTER WhatToSearch;

	Version 12.58.3: Apr 18, 2013	Changes in maximum number of users in listings.
					Removed field SideCols from sessions table.
					Removed backup tables created in version 12.50.
					Removed table hidden_downloads. (150250 lines)
					4 changes necessary in database:
ALTER TABLE sessions DROP COLUMN SideCols;
DROP TABLE IF EXISTS notif_backup;
DROP TABLE IF EXISTS marks_backup;
DROP TABLE IF EXISTS hidden_downloads;

	Version 12.58.2: Apr 17, 2013	Fixed bug in list of administrators. (150230 lines)
	Version 12.58.1: Apr 17, 2013	Changes in words to search when searching for documents. (150218 lines)
	Version 12.58:   Apr 16, 2013	Search of documents is splitted into my documents, documents in my courses and open documents. (150214 lines)
	Version 12.57.1: Apr 16, 2013	Optimizations in search of documents. (150033 lines)
	Version 12.57:   Apr 16, 2013	Changes in search of documents.
					Fixed bug when deleting a course, reported by Mar�a Dolores Ruiz L�pez. (149941 lines)
	Version 12.56:   Apr 16, 2013	Changes in search. (149823 lines)
	Version 12.55:   Apr 15, 2013	Changes in search. (149549 lines)
	Version 12.54.1: Apr 09, 2013	Changes in search form. (149642 lines)
	Version 12.54:   Apr 09, 2013	Changes in selection of my courses.
					New module swad_search for search. (149639 lines)
	Version 12.53:   Apr 09, 2013	New form to search on page top. (149548 lines)
	Version 12.52.3: Apr 04, 2013	When a user is removed completely, his/her notifications are removed. (149420 lines)
	Version 12.52.2: Apr 04, 2013	Files of marks can be hidden. (149404 lines)
	Version 12.52.1: Apr 04, 2013	Fixed bug when seeing marks, reported by Jos� Samos Jim�nez.
					Changed border style when downloading a file. (149396 lines)
	Version 12.52:   Apr 04, 2013	Hidden documents are now stored in table of files in database. (149306 lines)
					10 changes necessary in database:
ALTER TABLE files ADD COLUMN Hidden ENUM('N','Y') NOT NULL DEFAULT 'N' AFTER Path;

DROP TABLE IF EXISTS files_copy;
CREATE TABLE files_copy LIKE files;
INSERT INTO files_copy SELECT * FROM files;

INSERT INTO files_copy (CrsCod,GrpCod,ZoneUsrCod,FileBrowser,PublisherUsrCod,Path,Hidden,Public,License) SELECT CrsCod,GrpCod,'-1','3','-1',Path,'Y','N','1' FROM hidden_downloads WHERE GrpCod='-1' AND NOT EXISTS (SELECT * FROM files_copy WHERE files_copy.FileBrowser='3' AND hidden_downloads.CrsCod=files_copy.CrsCod AND hidden_downloads.GrpCod=files_copy.GrpCod AND hidden_downloads.Path=files_copy.Path);
INSERT INTO files_copy (CrsCod,GrpCod,ZoneUsrCod,FileBrowser,PublisherUsrCod,Path,Hidden,Public,License) SELECT CrsCod,GrpCod,'-1','11','-1',Path,'Y','N','1' FROM hidden_downloads WHERE GrpCod>'0' AND NOT EXISTS (SELECT * FROM files_copy WHERE files_copy.FileBrowser='3' AND hidden_downloads.CrsCod=files_copy.CrsCod AND hidden_downloads.GrpCod=files_copy.GrpCod AND hidden_downloads.Path=files_copy.Path);

UPDATE files_copy,hidden_downloads SET files_copy.Hidden='Y' WHERE files_copy.FileBrowser='3' AND files_copy.CrsCod=hidden_downloads.CrsCod AND hidden_downloads.GrpCod='-1' AND files_copy.Path=hidden_downloads.Path;
UPDATE files_copy,hidden_downloads SET files_copy.Hidden='Y' WHERE files_copy.FileBrowser='11' AND files_copy.CrsCod=hidden_downloads.CrsCod AND hidden_downloads.GrpCod>'0' AND files_copy.GrpCod=hidden_downloads.GrpCod AND files_copy.Path=hidden_downloads.Path;

DROP TABLE files;
RENAME TABLE files_copy TO files;

	Version 12.51.2: Apr 03, 2013	Changes in notificactions about new files. (149386 lines)
	Version 12.51.1: Apr 03, 2013	Fixed bug in test importation, reported by Amalia Morales Hern�ndez. (149335 lines)
	Version 12.51:   Apr 02, 2013	Changes in marks zones. (149328 lines)
	Version 12.50:   Apr 02, 2013	Use FilCod in notif instead of MrkCod for marks files. (149292 lines)
					30 changes necessary in database:
CREATE TABLE files_backup LIKE files;
INSERT INTO files_backup SELECT * FROM files;
CREATE TABLE marks_backup LIKE marks;
INSERT INTO marks_backup SELECT * FROM marks;
CREATE TABLE notif_backup LIKE notif;
INSERT INTO notif_backup SELECT * FROM notif;

DROP TABLE IF EXISTS files_copy;
CREATE TABLE files_copy LIKE files;
INSERT INTO files_copy SELECT * FROM files;

DROP TABLE IF EXISTS marks_copy;
CREATE TABLE marks_copy LIKE marks;
INSERT INTO marks_copy SELECT * FROM marks;
DELETE FROM marks_copy WHERE (LOWER(Path) NOT LIKE '%.htm' AND LOWER(Path) NOT LIKE '%.html');

INSERT INTO files_copy (CrsCod,GrpCod,ZoneUsrCod,FileBrowser,PublisherUsrCod,Path,Public,License) SELECT CrsCod,GrpCod,'-1','8','-1',Path,'N','1' FROM marks_copy WHERE GrpCod='-1' AND NOT EXISTS (SELECT * FROM files_copy WHERE files_copy.FileBrowser='8' AND marks_copy.CrsCod=files_copy.CrsCod AND marks_copy.GrpCod=files_copy.GrpCod AND marks_copy.Path=files_copy.Path);
INSERT INTO files_copy (CrsCod,GrpCod,ZoneUsrCod,FileBrowser,PublisherUsrCod,Path,Public,License) SELECT CrsCod,GrpCod,'-1','13','-1',Path,'N','1' FROM marks_copy WHERE GrpCod>'0' AND NOT EXISTS (SELECT * FROM files_copy WHERE files_copy.FileBrowser='13' AND marks_copy.CrsCod=files_copy.CrsCod AND marks_copy.GrpCod=files_copy.GrpCod AND marks_copy.Path=files_copy.Path);

DROP TABLE IF EXISTS marks_properties;
CREATE TABLE IF NOT EXISTS marks_properties (FilCod INT NOT NULL,Header INT NOT NULL,Footer INT NOT NULL,UNIQUE INDEX(FilCod));
INSERT INTO marks_properties (FilCod,Header,Footer) SELECT files_copy.FilCod,marks_copy.Header,marks_copy.Footer FROM files_copy,marks_copy WHERE (files_copy.FileBrowser='8' AND marks_copy.GrpCod='-1' AND marks_copy.CrsCod=files_copy.CrsCod AND marks_copy.GrpCod=files_copy.GrpCod AND marks_copy.Path=files_copy.Path) OR (files_copy.FileBrowser='13' AND marks_copy.GrpCod>'0' AND marks_copy.CrsCod=files_copy.CrsCod AND marks_copy.GrpCod=files_copy.GrpCod AND marks_copy.Path=files_copy.Path);

DROP TABLE IF EXISTS notif_copy;
CREATE TABLE notif_copy LIKE notif;
INSERT INTO notif_copy SELECT * FROM notif;
UPDATE notif_copy,marks_copy,files_copy SET notif_copy.Cod=files_copy.FilCod WHERE notif_copy.NotifyEvent='5' AND notif_copy.Cod=marks_copy.MrkCod AND marks_copy.GrpCod='-1' AND marks_copy.CrsCod=files_copy.CrsCod AND marks_copy.GrpCod=files_copy.GrpCod AND marks_copy.Path=files_copy.Path AND files_copy.FileBrowser='8';
UPDATE notif_copy,marks_copy,files_copy SET notif_copy.Cod=files_copy.FilCod WHERE notif_copy.NotifyEvent='5' AND notif_copy.Cod=marks_copy.MrkCod AND marks_copy.GrpCod>'0' AND marks_copy.CrsCod=files_copy.CrsCod AND marks_copy.GrpCod=files_copy.GrpCod AND marks_copy.Path=files_copy.Path AND files_copy.FileBrowser='13';
UPDATE notif_copy SET Cod='-1' WHERE NotifyEvent='5' AND Cod NOT IN (SELECT FilCod FROM marks_properties);

DROP TABLE files;
RENAME TABLE files_copy TO files;

DROP TABLE marks;
DROP TABLE marks_copy;

DROP TABLE notif;
RENAME TABLE notif_copy TO notif;

	Version 12.49.5: Mar 31, 2013	Expand not-expanded folders when accessing a file from notifications. (149271 lines)
	Version 12.49.4: Mar 31, 2013	Changes in notifications when uploading or pasting files.
					"Common zone" now is called "Shared files area". (149227 lines)
	Version 12.49.3: Mar 28, 2013	Notifications are not created when uploading / pasting into a folder not visible. (149201 lines)
	Version 12.49.2: Mar 28, 2013	Check if a file is hidden or deleted before showing it. (149196 lines)
	Version 12.49.1: Mar 27, 2013	Changes in notifications. (149135 lines)
	Version 12.49:   Mar 26, 2013	Some icons changed.
					Changes in notifications.
					QR code for nickname now starts by @. (149062 lines)
	Version 12.48.5: Mar 26, 2013	Changes in notifications. (148926 lines)
					1 change necessary in database:
ALTER TABLE notif ADD INDEX (CrsCod);

	Version 12.48.4: Mar 25, 2013	Changes in notifications. (149136 lines)
	Version 12.48.3: Mar 25, 2013	Changes in notifications. (149078 lines)
					1 change necessary in database:
ALTER TABLE crs_usr_requests ADD COLUMN ReqCod INT NOT NULL AUTO_INCREMENT FIRST, ADD PRIMARY KEY (ReqCod);

	Version 12.48.2: Mar 25, 2013	Changes in notifications. (148972 lines)
	Version 12.48.1: Mar 25, 2013	Changes in notifications of documents or shared files. (148951 lines)
	Version 12.48:   Mar 24, 2013	Four new events in notifications. (148912 lines)
					29 changes necessary in database:
UPDATE notif SET NotifyEvent=53 WHERE NotifyEvent=1;
UPDATE notif SET NotifyEvent=54 WHERE NotifyEvent=2;
UPDATE notif SET NotifyEvent=55 WHERE NotifyEvent=3;
UPDATE notif SET NotifyEvent=58 WHERE NotifyEvent=4;
UPDATE notif SET NotifyEvent=59 WHERE NotifyEvent=5;
UPDATE notif SET NotifyEvent=60 WHERE NotifyEvent=6;
UPDATE notif SET NotifyEvent=61 WHERE NotifyEvent=7;
UPDATE sta_notif SET NotifyEvent=53 WHERE NotifyEvent=1;
UPDATE sta_notif SET NotifyEvent=54 WHERE NotifyEvent=2;
UPDATE sta_notif SET NotifyEvent=55 WHERE NotifyEvent=3;
UPDATE sta_notif SET NotifyEvent=58 WHERE NotifyEvent=4;
UPDATE sta_notif SET NotifyEvent=59 WHERE NotifyEvent=5;
UPDATE sta_notif SET NotifyEvent=60 WHERE NotifyEvent=6;
UPDATE sta_notif SET NotifyEvent=61 WHERE NotifyEvent=7;
UPDATE notif SET NotifyEvent=3 WHERE NotifyEvent=53;
UPDATE notif SET NotifyEvent=4 WHERE NotifyEvent=54;
UPDATE notif SET NotifyEvent=5 WHERE NotifyEvent=55;
UPDATE notif SET NotifyEvent=8 WHERE NotifyEvent=58;
UPDATE notif SET NotifyEvent=9 WHERE NotifyEvent=59;
UPDATE notif SET NotifyEvent=10 WHERE NotifyEvent=60;
UPDATE notif SET NotifyEvent=11 WHERE NotifyEvent=61;
UPDATE sta_notif SET NotifyEvent=3 WHERE NotifyEvent=53;
UPDATE sta_notif SET NotifyEvent=4 WHERE NotifyEvent=54;
UPDATE sta_notif SET NotifyEvent=5 WHERE NotifyEvent=55;
UPDATE sta_notif SET NotifyEvent=8 WHERE NotifyEvent=58;
UPDATE sta_notif SET NotifyEvent=9 WHERE NotifyEvent=59;
UPDATE sta_notif SET NotifyEvent=10 WHERE NotifyEvent=60;
UPDATE sta_notif SET NotifyEvent=11 WHERE NotifyEvent=61;
UPDATE usr_data SET NotifyEvents=(((NotifyEvents << 4) & b'111100000000') | ((NotifyEvents << 2) & b'111000')) WHERE NotifyEvents<>0;

	Version 12.47.3: Mar 20, 2013	Changes related to minimum time between two consecutive tests. (148456 lines)
					4 changes necessary in database:
ALTER TABLE crs_usr ADD COLUMN LastAccTst DATETIME NOT NULL AFTER NextAccTst;
ALTER TABLE crs_usr ADD COLUMN NumQstsLastTst INT NOT NULL DEFAULT 0 AFTER LastAccTst;
UPDATE crs_usr SET LastAccTst=NOW();
ALTER TABLE crs_usr DROP COLUMN NextAccTst;

	Version 12.47.2: Mar 20, 2013	Test exam questions older than modifications in question are not shown.
					Code refactoring in tests. (148416 lines)
	Version 12.47.1: Mar 19, 2013	Database table tst_exam_answers is joined into table tst_exam_questions. (148288 lines)
					4 changes necessary in database:
ALTER TABLE tst_exam_questions ADD COLUMN Indexes TEXT NOT NULL AFTER Score;
ALTER TABLE tst_exam_questions ADD COLUMN Answers TEXT NOT NULL AFTER Indexes;
UPDATE tst_exam_questions,tst_exam_answers SET tst_exam_questions.Indexes=tst_exam_answers.Indexes,tst_exam_questions.Answers=tst_exam_answers.Answers WHERE tst_exam_questions.TstCod=tst_exam_answers.TstCod AND tst_exam_questions.QstCod=tst_exam_answers.QstCod;
DROP TABLE tst_exam_answers;

	Version 12.47:   Mar 18, 2013	Changes in test exams. (148369 lines)
	Version 12.46:   Mar 17, 2013	Students can view their results in past test exams. (148193 lines)
					5 changes necessary in database:
ALTER TABLE tst_exams ADD COLUMN AllowTeachers ENUM('N','Y') NOT NULL DEFAULT 'N' AFTER UsrCod;
UPDATE tst_exams SET AllowTeachers='Y';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1083','es','N','Seleccionar fechas para mis result. test');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1084','es','N','Ver mis resultados de tests');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1085','es','N','Ver un examen de test m&iacute;o ya realizado');

	Version 12.45.1: Mar 17, 2013	Students can decide if an exam will be saved.
					View of exams available for teachers. (148034 lines)
					1 change advisable (not mandatory) in database:
DELETE FROM tst_exams; DELETE FROM tst_exam_questions; DELETE FROM tst_exam_answers;

	Version 12.45:   Mar 17, 2013	Changes in tests.
					Several messages translated. (147981 lines)
	Version 12.44:   Mar 16, 2013	User, date, number of questions and tags of questions in heading of test exam result. (147846 lines)
	Version 12.43.2: Mar 16, 2013	Fixed bugs in test exams related to floating point. (147697 lines)
	Version 12.43.1: Mar 16, 2013	Changes in test exams results. (147682 lines)
	Version 12.43:   Mar 15, 2013	Changes in test exams results. (147640 lines)
					5 changes necessary in database:
DROP TABLE tst_exam_questions;
CREATE TABLE IF NOT EXISTS tst_exam_questions (TstCod INT NOT NULL,QstCod INT NOT NULL,QstInd INT NOT NULL,Score DOUBLE PRECISION NOT NULL DEFAULT 0,INDEX(TstCod,QstCod));
DROP TABLE tst_exam_answers;
CREATE TABLE IF NOT EXISTS tst_exam_answers (TstCod INT NOT NULL,QstCod INT NOT NULL,Indexes TEXT NOT NULL,Answers TEXT NOT NULL,INDEX(TstCod,QstCod));
DELETE FROM tst_exams;

	Version 12.42.3: Mar 14, 2013	Changes in test exams results. (147503 lines)
	Version 12.42.2: Mar 14, 2013	Test exams are selected in a range of dates. (147433 lines)
	Version 12.42.1: Mar 14, 2013	Changes in results of test exams. (147421 lines)
					2 changes necessary in database:
DROP TABLE tst_exams;
CREATE TABLE IF NOT EXISTS tst_exams (TstCod INT NOT NULL AUTO_INCREMENT,CrsCod INT NOT NULL,UsrCod INT NOT NULL,TstTime DATETIME NOT NULL,NumQsts INT NOT NULL DEFAULT 0,NumQstsNotBlank INT NOT NULL DEFAULT 0,Score DOUBLE PRECISION NOT NULL DEFAULT 0,UNIQUE INDEX(TstCod),INDEX(CrsCod,UsrCod));

	Version 12.42:   Mar 14, 2013	Changes in results of test exams. (147389 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1082','es','N','Ver un examen de test ya realizado');

	Version 12.41.1: Mar 12, 2013	Results of a test exam are stored in database and shown on web. (147294 lines)
	Version 12.41:   Mar 11, 2013	New option to select students to see the results of test exams. (147186 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1080','es','N','Seleccionar usuarios para result. test');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1081','es','N','Ver resultados de tests de usuarios');

	Version 12.40.5: Mar 10, 2013	New tables to store result of test exams. (146962 lines)
					3 changes necessary in database:
CREATE TABLE IF NOT EXISTS tst_exams (TstCod INT NOT NULL AUTO_INCREMENT,CrsCod INT NOT NULL,UsrCod INT NOT NULL,TstTime DATETIME NOT NULL,Score DOUBLE PRECISION NOT NULL DEFAULT 0,NumQuestions INT NOT NULL DEFAULT 0,UNIQUE INDEX(TstCod),INDEX(CrsCod,UsrCod));
CREATE TABLE IF NOT EXISTS tst_exam_questions (TstCod INT NOT NULL,QstCod INT NOT NULL,Score DOUBLE PRECISION NOT NULL DEFAULT 0,INDEX(TstCod,QstCod));
CREATE TABLE IF NOT EXISTS tst_exam_answers (TstCod INT NOT NULL,QstCod INT NOT NULL,AnsInd INT NOT NULL,Answer TEXT NOT NULL,INDEX(TstCod,QstCod));

	Version 12.40.4: Mar 10, 2013	Added feedback to web service function getTests. (146898 lines)
	Version 12.40.3: Mar 09, 2013	New column with number of teachers in pending requests for inscription. (146890 lines)
	Version 12.40.2: Mar 08, 2013	Change in tst_questions database table. (146878 lines)
					1 change necessary in database:
ALTER TABLE tst_questions CHANGE AnsType AnsType ENUM ('int','float','true_false','unique_choice','multiple_choice','text') NOT NULL;

	Version 12.40.1: Mar 07, 2013	Fixed bug related to length of Twitter.
					Fixed bug related to length of Nickname in login.
					Fixed bug in search of courses. (146875 lines)
	Version 12.40:   Mar 07, 2013	Export/import test questions with feedback. (146810 lines)
	Version 12.39.2: Mar 06, 2013	Feedback when solving test. (146738 lines)
	Version 12.39.1: Mar 05, 2013	Feedback in test questions. (146698 lines)
					2 changes necessary in database:
ALTER TABLE tst_questions ADD COLUMN Feedback TEXT NOT NULL AFTER Stem;
ALTER TABLE tst_config CHANGE Feedback Feedback ENUM('nothing','total_result','each_result','each_good_bad','full_feedback') NOT NULL;

	Version 12.39:   Mar 04, 2013	Changed icons to create and download zip file.
					Feedback for answers in test questions (not finished). (146598 lines)
					1 change necessary in database:
ALTER TABLE tst_answers ADD COLUMN Feedback TEXT NOT NULL AFTER Answer;

	Version 12.38.6: Mar 02, 2013	Changed icons to create and download zip file.
					In course works/assignment file zone, all files are shown by default. (146395 lines)
	Version 12.38.5: Feb 27, 2013	Added alt attributes to all image buttons. (146398 lines)
	Version 12.38.4: Feb 26, 2013	Speed optimization in query of last clicks. (146195 lines)
	Version 12.38.3: Feb 22, 2013	Fixed bug in notifications about marks. (146195 lines)
	Version 12.38.2: Feb 20, 2013	Fixed bug in users' directories.
					Users' directories are now created automatically. (146192 lines)
	Version 12.38.1: Feb 13, 2013	Fixed bug in statistics. (146183 lines)
	Version 12.38:   Feb 13, 2013	New scope: current institution. (146176 lines)
	Version 12.37:   Feb 13, 2013	New scope: current institution (still under development). (145587 lines)
	Version 12.36:   Feb 12, 2013	New module swad_scope.
					Code refactoring related to scope (location ranges). (145170 lines)
	Version 12.35:   Feb 12, 2013	Unification of interface and code related to location ranges. (145003 lines)
	Version 12.34.1: Feb 11, 2013	Unification of interface and code related to location ranges in connected users. (144481 lines)
	Version 12.34:   Feb 11, 2013	Unification of interface and code related to location ranges.
					Statistics of centres. (144425 lines)
	Version 12.33:   Feb 10, 2013	New users' range for current centre.
					Refactorization of code related to users' lists location. (144061 lines)
	Version 12.32.5: Jan 30, 2013	Changes in the text of several options in menu.
					Changes in order of options of enrolment tab. (143920 lines)
	Version 12.32.4: Jan 29, 2013	Fixed bug in calendar. (143918 lines)
	Version 12.32.3: Jan 15, 2013	New file extension allowed, requested by Javier Mateos: .dmg (Disk Image in Masc OS). (143896 lines)
	Version 12.32.2: Dec 30, 2012	Changed DNI to UsrID in tables log and log_recent. (143896 lines)
					2 changes necessary in database (it could take a long time, even several hours):
ALTER TABLE log_recent CHANGE DNI UsrID CHAR(16) NOT NULL;
ALTER TABLE log CHANGE DNI UsrID CHAR(16) NOT NULL;

	Version 12.32.2: Dec 29, 2012	Several messages translated to french. (143895 lines)
	Version 12.32.1: Dec 29, 2012	Changed DNI to UsrID in tables usr_data and imported_students. (144051 lines)
					2 changes necessary in database:
ALTER TABLE imported_students CHANGE DNI UsrID CHAR(16) NOT NULL;
ALTER TABLE usr_data CHANGE DNI UsrID CHAR(16) NOT NULL;

	Version 12.32:   Dec 29, 2012	Changes in rules to accept users' IDs and nicknames. (144043 lines)
	Version 12.31:   Dec 29, 2012	Nicknames must be entered with '@'. (144088 lines)
	Version 12.30:   Dec 28, 2012	Code refactoring related to user's ID. (144171 lines)
	Version 12.29.5: Dec 28, 2012	Several messages translated (0 messages to be translated) and other minor changes. (144127 lines)
	Version 12.29.4: Dec 27, 2012	Removed old nickname check. (143975 lines)
	Version 12.29.3: Dec 26, 2012	Several messages translated (3 messages to be translated). (144033 lines)
	Version 12.29.2: Dec 23, 2012	Notices are drawn without tables, and with different widths. (143997 lines)
	Version 12.29.1: Dec 22, 2012	Notices are drawn using CSS3 instead of images. (143998 lines)
					1 update necessary:
Remove directory with icons for notices.
Example:
rm -Rf /var/www/html/swad/icon/notice/

	Version 12.29:   Dec 18, 2012	Changes in actions related to see or admin documents. (144043 lines)
					4 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1078','es','N','Ver documentos');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1079','es','N','Ver calificaciones');
UPDATE actions SET Txt='Ver o administrar documentos' WHERE ActCod='0';
UPDATE actions SET Txt='Ver o administrar calificaciones' WHERE ActCod='17';

	Version 12.28.4: Dec 18, 2012	Old action to admin docs is removed. (144004 lines)
					1 change necessary in database:
UPDATE actions SET Obsolete='Y' WHERE ActCod='1062';
					1 update necessary:
Copy new icon visible16x16.gif from swad source directory to icons public directory.
Example:
cp /home/<user>/swad/swad/icon/visible16x16.gif /var/www/html/swad/icon/

	Version 12.28.3: Dec 16, 2012	Changes in CSS related to user's photo. (143946 lines)
	Version 12.28.2: Dec 16, 2012	Fixed bugs in web service related to file browser. (143874 lines)
	Version 12.28.1: Dec 16, 2012	Abort if click is too fast.
					Changes in redirection to language. (143857 lines)
	Version 12.28:   Dec 16, 2012	Change language after user authentication. (143831 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1077','es','N','Cambiar de idioma tras autenticar');

	Version 12.27.10:Dec 12, 2012	Small change in design of iframe for course info. (? lines)
	Version 12.27.9: Dec 11, 2012	Rounded rectangles for months now use CSS instead of tables. (143811 lines)
	Version 12.27.8: Dec 11, 2012	Fixed bugs in attendance events. (143800 lines)
	Version 12.27.7: Dec 11, 2012	Rectangles 2D without shadow now use CSS instead of tables. (143800 lines)
	Version 12.27.6: Dec 10, 2012	Rectangles 3D with shadow now use CSS instead of tables. (143793 lines)
	Version 12.27.5: Dec 10, 2012	Action title now use CSS instead of tables. (143770 lines)
	Version 12.27.4: Dec 10, 2012	Changes in CSS of alerts. (143739 lines)
	Version 12.27.3: Dec 10, 2012	Alerts now use CSS instead of tables. (143789 lines)
	Version 12.27.2: Dec 09, 2012	Several messages translated (5 messages to be translated).
					Optimizations on attendance. (143714 lines)
	Version 12.27.1: Dec 09, 2012	Optimizations on file browser and web service. (143662 lines)
	Version 12.27:   Dec 05, 2012	Changes in web service getFile function. (143748 lines)
	Version 12.26:   Dec 04, 2012	Changes in web service getFile function. (143643 lines)
	Version 12.25:   Dec 02, 2012	Changes in buffers related to TEXT and LONGTEXT database fields.
					Refactoring of code in every module. (143471 lines)
	Version 12.24.5: Nov 30, 2012	Changes in space used for buffer related to TEXT and LONGTEXT database fields. (142652 lines)
					2 changes necessary in database:
ALTER TABLE notices CHANGE Content Content TEXT NOT NULL;
ALTER TABLE notices_deleted CHANGE Content Content TEXT NOT NULL;

	Version 12.24.4: Nov 29, 2012	Changes in comments inside attendance events. (142624 lines)
					2 changes necessary in database:
ALTER TABLE att_usr CHANGE CommentStd CommentStd TEXT NOT NULL;
ALTER TABLE att_usr CHANGE CommentTch CommentTch TEXT NOT NULL;

	Version 12.24.3: Nov 29, 2012	Changes in web service getDirectoryTree function. (142573 lines)
	Version 12.24.2: Nov 27, 2012	Changes in web service getDirectoryTree function. (142561 lines)
	Version 12.24.1: Nov 27, 2012	Fixed bug in attendance events, reported by Jos� Miguel Colella Carbonara. (142521 lines)
	Version 12.24:   Nov 26, 2012	Students can see attendance events. (142520 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1076','es','N','Registrar estudiante asistente a evento');

	Version 12.23.3: Nov 26, 2012	Changes in attendance events. (142407 lines)
					3 changes necessary in database:
ALTER TABLE att_usr CHANGE Comment CommentStd VARCHAR(255) NOT NULL;
ALTER TABLE att_usr ADD COLUMN CommentTch VARCHAR(255) NOT NULL AFTER CommentStd;
ALTER TABLE att_events CHANGE CommentVisibility CommentTchVisible ENUM('N','Y') NOT NULL DEFAULT 'N' AFTER EndTime;

	Version 12.23.2: Nov 26, 2012	Changes in attendance events. (142406 lines)
					1 change necessary in database:
ALTER TABLE att_usr ADD COLUMN Present ENUM('N','Y') NOT NULL DEFAULT 'Y' AFTER UsrCod;

	Version 12.23.1: Nov 26, 2012	Changes in attendance events. (142397 lines)
	Version 12.23:   Nov 24, 2012	Adding comments to attendance events. (142378 lines)
					2 changes necessary in database:
ALTER TABLE att_events ADD COLUMN CommentVisibility TINYINT NOT NULL DEFAULT 0 AFTER EndTime;
ALTER TABLE att_usr ADD COLUMN Comment VARCHAR(255) NOT NULL AFTER UsrCod;

	Version 12.22.2: Nov 23, 2012	Internal changes to remove warnings in Eclipse. (142185 lines)
	Version 12.22.1: Nov 20, 2012	Changed database collation to latin1_spanish_ci, because latin1_general_ci does not order properly. (142032 lines)
					10 changes necessary in database:
ALTER DATABASE swad DEFAULT CHARACTER SET=latin1 DEFAULT COLLATE latin1_spanish_ci;
ALTER TABLE centres CONVERT TO CHARACTER SET latin1 COLLATE latin1_spanish_ci;
ALTER TABLE courses CONVERT TO CHARACTER SET latin1 COLLATE latin1_spanish_ci;
ALTER TABLE degrees CONVERT TO CHARACTER SET latin1 COLLATE latin1_spanish_ci;
ALTER TABLE institutions CONVERT TO CHARACTER SET latin1 COLLATE latin1_spanish_ci;
ALTER TABLE usr_data CONVERT TO CHARACTER SET latin1 COLLATE latin1_spanish_ci;
ALTER TABLE nicknames CONVERT TO CHARACTER SET latin1 COLLATE latin1_spanish_ci;
ALTER TABLE nicknames CHANGE Nickname Nickname CHAR(16) COLLATE latin1_spanish_ci NOT NULL;
Remove manually entries with duplicated values for Nickname in table nicknames and retry the last two lines.
UPDATE usr_data SET Nickname='' WHERE Nickname<>'' AND Nickname NOT IN (SELECT Nickname COLLATE latin1_bin FROM nicknames);

	Version 12.22:   Nov 20, 2012	Changes in rules to validate a nickname. (142019 lines)
	Version 12.21:   Nov 19, 2012	New table in database to remember nicknames used by a user. (141971 lines)
					2 changes necessary in database:
CREATE TABLE IF NOT EXISTS nicknames (UsrCod INT NOT NULL,Nickname CHAR(16) COLLATE latin1_spanish_ci NOT NULL,CreatTime DATETIME NOT NULL,UNIQUE INDEX(UsrCod,Nickname),UNIQUE INDEX(Nickname));
REPLACE INTO nicknames (UsrCod,Nickname,CreatTime) SELECT UsrCod,NickName,NOW() FROM usr_data WHERE Nickname<>'';

	Version 12.20.7: Nov 18, 2012	Changes in attendance events. (141847 lines)
	Version 12.20.6: Nov 18, 2012	Changes in attendance events. (141843 lines)
	Version 12.20.5: Nov 18, 2012	Changes in attendance events. (141836 lines)
	Version 12.20.4: Nov 17, 2012	Several messages translated (0 messages to be translated). (141798 lines)
	Version 12.20.3: Nov 17, 2012	Changes in attendance events. (141624 lines)
	Version 12.20.2: Nov 17, 2012	Refactorization of code. Lot of returns (in the middle of funcions) removed. (141568 lines)
	Version 12.20.1: Nov 16, 2012	Changes in function getDirectoryTree of the web service. (141571 lines)
	Version 12.20:   Nov 13, 2012	Lot of icons changed.
					Fixed several bugs.
					Control of attendance published. (141564 lines)
					1 update necessary:
Copy new icons from swad source directory to icons public directory.
Example:
cp /home/<user>/swad/swad/icon/<files>.png /var/www/html/swad/icon/
cp /home/<user>/swad/swad/icon/<files>.gif /var/www/html/swad/icon/

	Version 12.19.1: Nov 12, 2012	Changes in attendance events. (141438 lines)
	Version 12.19:   Nov 12, 2012	Changes in attendance events. (141375 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1073','es','N','Selec. estud. y eventos listado asistencia');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1074','es','N','Listar asistencia a varios eventos');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1075','es','N','Imprimir asistencia a varios eventos');

	Version 12.18.4: Nov 11, 2012	Changes in attendance events. (141205 lines)
	Version 12.18.3: Nov 07, 2012	Changes in attendance events. (141042 lines)
	Version 12.18.2: Nov 07, 2012	Fixed bug in attendance module when removing a user. (140998 lines)
	Version 12.18.1: Nov 07, 2012	Fixed bug when removing an attendance event. (140997 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1072','es','N','Registrar estudiantes asistentes a evento');

	Version 12.18:   Nov 07, 2012	Changes in attendance events.
					Fixed bugs when removing users, groups and courses. (140997 lines)
	Version 12.17:   Nov 06, 2012	Changes in view of an attendance event. (140859 lines)
	Version 12.16.1: Nov 06, 2012	Minor changes in configuration. (140828 lines)
	Version 12.16:   Nov 06, 2012	Changes in view of an attendance event. (140826 lines)
	Version 12.15:   Nov 05, 2012	Minor changes.
					New table att_usr to store users who attended an event.
					New option to see one attendance event. (140776 lines)
					2 changes necessary in database:
CREATE TABLE IF NOT EXISTS att_usr (AttCod INT NOT NULL,UsrCod INT NOT NULL,UNIQUE INDEX(AttCod,UsrCod),INDEX(UsrCod));
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1071','es','N','Listar estudiantes asistentes a evento');

	Version 12.14.2: Nov 04, 2012	Edition of attendance events. (140639 lines)
					10 changes necessary in database:
CREATE TABLE IF NOT EXISTS att_events (AttCod INT NOT NULL AUTO_INCREMENT,CrsCod INT NOT NULL DEFAULT -1,Hidden ENUM('N','Y') NOT NULL DEFAULT 'N',UsrCod INT NOT NULL,StartTime DATETIME NOT NULL,EndTime DATETIME NOT NULL,Title VARCHAR(255) NOT NULL,Txt TEXT NOT NULL,UNIQUE INDEX(AttCod),INDEX(CrsCod,Hidden));
CREATE TABLE IF NOT EXISTS att_grp (AttCod INT NOT NULL,GrpCod INT NOT NULL,UNIQUE INDEX(AttCod,GrpCod));
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1063','es','N','Solicitar creaci�n evento asistencia');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1064','es','N','Solicitar edici�n evento asistencia');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1065','es','N','Crear evento asistencia');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1066','es','N','Modificar evento asistencia');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1067','es','N','Solicitar elim. evento asistencia');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1068','es','N','Eliminar evento asistencia');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1069','es','N','Ocultar evento asistencia');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1070','es','N','Mostrar evento asistencia');
					1 change neccesary in Makefile:
Add swad_attendance.o to list of OBJS in Makefile

	Version 12.14.1: Nov 04, 2012	Edition of attendance events. (140546 lines)
	Version 12.14:   Nov 03, 2012	New module swad_attendance for control of attendance.
					Starting edition of attendance events. (140301 lines)
	Version 12.13.1: Nov 03, 2012	Changes related to avoiding fast clicks from the same IP.
					Several messages translated (0 messages to be translated). (138799 lines)
	Version 12.13:   Nov 03, 2012	Temporary option to admin documents (to be removed in december 2012). (138684 lines)
					2 changes necessary in database:
UPDATE actions SET Obsolete='N' WHERE ActCod IN (1054,1055,1056,1057,1058,1059,1060,1061);
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1062','es','N','Administrar documentos (opci�n antigua)');

	Version 12.12.4: Nov 02, 2012	Advertisement of mobile version. (138586 lines)
	Version 12.12.3: Nov 02, 2012	Several messages translated (still 3 messages to be translated). (138530 lines)
	Version 12.12.2: Nov 01, 2012	Several messages translated (still 19 messages to be translated). (138223 lines)
	Version 12.12.1: Nov 01, 2012	Changes in expanded folders. (138017 lines)
					2 changes recommended (not mandatory) in database:
DELETE FROM expanded_folders WHERE FileBrowser IN (3,8,11,13);
OPTIMIZE TABLE expanded_folders;

	Version 12.12:   Oct 30, 2012	Changes in function getDirectoryTree in the web service.
					New function getFile in the web service. (137943 lines)
	Version 12.11.1: Oct 29, 2012	Changes in function getDirectoryTree in the web service. (137876 lines)
	Version 12.11:   Oct 29, 2012	Changes in function getDirectoryTree in the web service. (137866 lines)
	Version 12.10.13:Oct 28, 2012	Several messages translated (still 28 messages to be translated). (137736 lines)
	Version 12.10.12:Oct 27, 2012	Configuration of tests is removed from menu.
					Removed file swad_changelog.c (? lines)
					It is necessary to remove swad_changelog.o from OBJS in Makefile:
Remove swad_changelog.o from OBJS in Makefile.

	Version 12.10.11:Oct 27, 2012	New button in tests to configure them. (137680 lines)
	Version 12.10.10:Oct 27, 2012	Edition of tests is removed from menu. (137656 lines)
	Version 12.10.9: Oct 27, 2012	New button in tests to edit them. (137716 lines)
	Version 12.10.7: Oct 27, 2012	Marks admin is removed from menu. (137697 lines)
	Version 12.10.6: Oct 27, 2012	New button in marks file zone to edit them. (137752 lines)
	Version 12.10.5: Oct 26, 2012	Docs admin is removed from menu. (137740 lines)
	Version 12.10.4: Oct 25, 2012	New button in documents zone to edit them. (137800 lines)
	Version 12.10.3: Oct 25, 2012	Changes in database inserts related to deleted notices and messages. (137776 lines)
	Version 12.10.2: Oct 23, 2012	Changes check for valid ID. (137775 lines)
	Version 12.10.1: Oct 17, 2012	Changes in file browsers. (137774 lines)
	Version 12.10:   Oct 16, 2012	Opening time is shown when selecting groups.
					Several messages translated (still 38 messages to be translated). (137767 lines)
	Version 12.9.1:  Oct 15, 2012	Fixed bugs in web service function sendMyGroups, reported by Helena Rodr�guez Gij�n. (137599 lines)
	Version 12.9:    Oct 14, 2012	Change user's groups in web service function sendMyGroups. (137598 lines)
	Version 12.8.4:  Oct 14, 2012	Optimizations in groups. (137576 lines)
	Version 12.8.3:  Oct 14, 2012	Changes in web services.
					Optimizations in groups. (137585 lines)
	Version 12.8.2:  Oct 13, 2012	New web service function sendMyGroups. (137532 lines)
	Version 12.8.1:  Oct 13, 2012	New fields in web service function getGroupTypes. (137426 lines)
	Version 12.8:    Oct 12, 2012	New web service function getGroupTypes. (137406 lines)
	Version 12.7:    Oct 12, 2012	New fields in web service functions getGroups and getNotifications. (137324 lines)
	Version 12.6.5:  Oct 11, 2012	Added MIME type for documents uploaded. (137275 lines)
	Version 12.6.4:  Oct 10, 2012	Changes in input image forms. (137188 lines)
	Version 12.6.3:  Oct 10, 2012	Fixed bug in automatic open of groups. (137190 lines)
	Version 12.6.2:  Oct 09, 2012	The default option in a tab is now the last option instead of the most frequent option.
					Last course is stored in user's data, to use in the future. (137164 lines).
					1 change necessary in database:
ALTER TABLE usr_data ADD COLUMN LastCrs INT NOT NULL DEFAULT -1 AFTER NotifyEvents;

	Version 12.6.1:  Oct 09, 2012	Optimization in deletes of file_view table. (137140 lines).
	Version 12.6:    Oct 09, 2012	New statistic for Open Educational Resources (OERs).
					Fixed bug when removing a course, a group or a user. (137125 lines)
					6 changes necessary in database:
DELETE FROM file_view WHERE FilCod IN (SELECT FilCod FROM files WHERE CrsCod>0 AND CrsCod NOT IN (SELECT CrsCod FROM courses));
DELETE FROM file_view WHERE FilCod IN (SELECT FilCod FROM files WHERE GrpCod>0 AND GrpCod NOT IN (SELECT GrpCod FROM crs_grp));
DELETE FROM file_view WHERE FilCod IN (SELECT FilCod FROM files WHERE ZoneUsrCod>0 AND ZoneUsrCod NOT IN (SELECT UsrCod FROM usr_data));
DELETE FROM files WHERE CrsCod>0 AND CrsCod NOT IN (SELECT CrsCod FROM courses);
DELETE FROM files WHERE GrpCod>0 AND GrpCod NOT IN (SELECT GrpCod FROM crs_grp);
DELETE FROM files WHERE ZoneUsrCod>0 AND ZoneUsrCod NOT IN (SELECT UsrCod FROM usr_data);

	Version 12.5.2:  Oct 08, 2012	Change in menu of my courses. (136967 lines)
	Version 12.5.1:  Oct 07, 2012	Fixed minor bug in unlocking tables after inscription. (136954 lines)
	Version 12.5:    Oct 07, 2012	Optimizations in selection of groups.
					Inscription in groups is now an atomic operation using LOCK TABLES. (136949 lines)
	Version 12.4.3:  Oct 06, 2012	Optimization in selection of groups. (136925 lines)
	Version 12.4.2:  Oct 06, 2012	Degree is now visible in selector of my courses. (136925 lines)
	Version 12.4.1:  Oct 05, 2012	Optimizations in selection of groups. (136924 lines)
	Version 12.4:    Oct 04, 2012	New field in table of test questions to store the number of times each question is answered with a non blank answer. (136815 lines)
					2 changes necessary in database:
ALTER TABLE tst_questions ADD COLUMN NumHitsNotBlank INT NOT NULL DEFAULT 0 AFTER NumHits;
UPDATE tst_questions SET NumHitsNotBlank=NumHits;

	Version 12.3:    Oct 02, 2012	Changes in edition of group types. (136616 lines)
					1 change necessary in database:
DROP INDEX OpenTime ON crs_grp_types;

	Version 12.2:    Oct 01, 2012	New field date in form of group types. (136589 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1061','es','Y','Cambiar fecha apertura tipo grupo');
ALTER TABLE crs_grp_types ADD COLUMN MustBeOpened ENUM('N','Y') NOT NULL DEFAULT 'N' AFTER Multiple;

	Version 12.1:    Sep 30, 2012	New field date in group types table. (136396 lines)
					2 changes necessary in database:
ALTER TABLE crs_grp_types ADD COLUMN OpenTime DATETIME NOT NULL DEFAULT 0 AFTER Multiple;
CREATE INDEX OpenTime ON crs_grp_types (OpenTime);

	Version 12.0.5:  Sep 30, 2012	Changes in listing of pending requests for enrolment. (136342 lines)
	Version 12.0.4:  Sep 30, 2012	Selection of students or teachers when listing pending requests for enrolment. (136337 lines)
	Version 12.0.3:  Sep 30, 2012	Change in menu of system tab. (136273 lines)
	Version 12.0.2:  Sep 29, 2012	Minor change in configuration of tests. (136272 lines)
	Version 12.0.1:  Sep 24, 2012	Minor change in order of option in system tab. (136268 lines)
	Version 12.0:    Sep 24, 2012	New option to show degrees with pending courses. (136268 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1060','es','Y','Titul. con asignat. pendientes');

	Version 11.79.3: Sep 24, 2012	Fixed bug in database when importing students. (135981 lines)
	Version 11.79.2: Sep 23, 2012	Fixed bug in mail domains, reported by Francisco Oca�a Lara. (135977 lines)
	Version 11.79.1: Sep 23, 2012	"Academic year" no longer used.
					Changes in calendar. (135975 lines)
	Version 11.79:   Sep 23, 2012	Confirmation of rejection of a request for inscription. (135961 lines)
					2 changes necessary in database:
UPDATE actions SET Txt='Preguntar si rechazar solicitud inscripci�n' WHERE ActCod='1058';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1059','es','Y','Rechazar solicitud inscripci�n');

	Version 11.78.1: Sep 20, 2012	New button to reject a request for inscription in a course. (135881 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1058','es','Y','Rechazar solicitud inscripci�n');

	Version 11.78:   Sep 19, 2012	New button to accept a request for inscription in a course. (135864 lines)
	Version 11.77.2: Sep 19, 2012	Changes in listing of inscription requests. (135798 lines)
	Version 11.77.1: Sep 19, 2012	Code optimization when creating database tables. (135754 lines)
	Version 11.77:   Sep 17, 2012	Listing of inscription requests. (135964 lines)
	Version 11.76.4: Sep 17, 2012	Listing of inscription requests for the current course. (135850 lines)
	Version 11.76.3: Sep 17, 2012	New database table for requesting inscrption in courses crs_usr_requests.
					Changes in request for inscription in courses. (135795 lines)
					1 change necessary in database:
CREATE TABLE IF NOT EXISTS crs_usr_requests (CrsCod INT NOT NULL DEFAULT -1,UsrCod INT NOT NULL,Role TINYINT NOT NULL DEFAULT 0,RequestTime DATETIME NOT NULL,UNIQUE INDEX(CrsCod,UsrCod),INDEX(UsrCod));

	Version 11.76.2: Sep 17, 2012	Changes related to requesting courses.
					New option to show pending inscription requests. (135766 lines)
					1 change necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1057','es','Y','Listar inscripciones pendientes');

	Version 11.76.1: Sep 16, 2012	New column when editing courses for requester of a course.
					New option to confirm application for registrarion. (135687 lines)
					2 changes necessary in database:
UPDATE actions SET Txt='Formulario solicitud inscripci�n' WHERE ActCod='1054';
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1056','es','Y','Solicitar inscripci�n');

	Version 11.76:   Sep 15, 2012	Programming new option to request the creation of a new course.
					New option to apply for enrolment.
					New option to change the status of a course. (135559 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1054','es','Y','Solicitar inscripci�n');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1055','es','Y','Cambiar estado asignatura');

	Version 11.75.9: Sep 14, 2012	Programming new option to request the creation of a new course. (135172 lines)
	Version 11.75.8: Sep 14, 2012	Programming new option to request the creation of a new course. (135157 lines)
					2 changes necessary in database:
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1052','es','Y','Solicitar asignaturas');
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1053','es','N','Solicitar asignatura');

	Version 11.75.7: Sep 13, 2012	Programming new option to request the creation of a new course. (135143 lines)
					3 changes necessary in database:
ALTER TABLE courses ADD COLUMN Status TINYINT NOT NULL DEFAULT 0 AFTER AllowDirectLogIn;
ALTER TABLE courses ADD COLUMN RequesterUsrCod INT NOT NULL DEFAULT -1 AFTER Status;
CREATE INDEX Status ON courses (Status);

	Version 11.75.6: Sep 12, 2012	Programming new option to request the creation of a new course. (135135 lines)
	Version 11.75.5: Sep 11, 2012	Programming new option to request the creation of a new course. (135090 lines)
	Version 11.75.4: Sep 10, 2012	Programming new option to request the creation of a new course. (134762 lines)
	Version 11.75.3: Sep 09, 2012	Starting new option to request the creation of a new course. (134758 lines)
	Version 11.75.2: Aug 02, 2012	Maximum number of years in a degree grows from 6 to 12. (134751 lines)
	Version 11.75.1: Jul 31, 2012	Fixed wrong path of script, reported by Justa G�mez Navajas. (134667 lines)
	Version 11.75:   Jul 30, 2012	Module swad_course is moved to swad_info.
					Functions related to edition of courses are moved to a new module swad_course. (134666 lines)
	Version 11.74.4: Jul 30, 2012	Changes in main paths. (134564 lines)
	Version 11.74.3: Jul 30, 2012	Minor changes in forums related to layout and icons. (? lines)
	Version 11.74.2: Jul 30, 2012	Fixed bug in notifications about new posts in forums of centres. (134547 lines)
	Version 11.74.1: Jul 27, 2012	Changed several messages in swad_text.c.
					Teachers can not change other users' passwords. (134539 lines)
	Version 11.74:   Jul 24, 2012	Changed "dar de alta/baja" to "inscribir/eliminar" in Spanish and Catala.
					Changed the order of several options in menus.
					Users' tab is moved after enrolment tab. (134512 lines)
	Version 11.73:   Jul 21, 2012	Fixed important bug in trimming leading spaces of strings. (134526 lines)
	Version 11.72.9: Jul 20, 2012	Fixed bug in edition of plugins. (134511 lines)
	Version 11.72.8: Jul 20, 2012	Changed database collation to latin1_spanish_ci. (134486 lines)
					6 changes necessary in database:
ALTER DATABASE swad DEFAULT CHARACTER SET=latin1 DEFAULT COLLATE latin1_spanish_ci;
ALTER TABLE usr_data CONVERT TO CHARACTER SET latin1 COLLATE latin1_spanish_ci;
ALTER TABLE courses CONVERT TO CHARACTER SET latin1 COLLATE latin1_spanish_ci;
ALTER TABLE degrees CONVERT TO CHARACTER SET latin1 COLLATE latin1_spanish_ci;
ALTER TABLE centres CONVERT TO CHARACTER SET latin1 COLLATE latin1_spanish_ci;
ALTER TABLE institutions CONVERT TO CHARACTER SET latin1 COLLATE latin1_spanish_ci;

	Version 11.72.7: Jul 11, 2012	Changes in common record layout.
					It is necessary to change the name of the file with the data protection policy. (134608 lines)
cd &lt;public_swad_directory&gt;
mkdir data_protection
mv swad_data_protection.html data_protection/index.html

	Version 11.72.6: Jul 11, 2012	The word "DNI" is changed to "ID (DNI/c&eacute;dula/pasaporte) in Spanish. (134447 lines)
	Version 11.72.5: Jul 09, 2012	The data protection clause is moved to an external file. (134445 lines)
	Version 11.72.4: Jul 09, 2012	Changes in configuration related with automatic sending of email. (134601 lines)
	Version 11.72.3: Jul 08, 2012	New statistics of forums of centres. (134595 lines)
	Version 11.72.2: Jul 08, 2012	Fixed bug in forums. (134592 lines)
	Version 11.72.1: Jul 08, 2012	Fixed bug in list of countries. (134591 lines)
	Version 11.72:   Jul 08, 2012	New feature: forums for centres.
					New field CtrCod in database table of notifications.
					Centres now have icons.
					New field Logo in database table of centres.
					Fixed bug in edition of centres. (134575 lines)
					It is necessary to create a new directory for the icons of the centres. For example:
mkdir /var/www/html/swad/icon/centre
					3 changes necessary in database:

ALTER TABLE notif ADD COLUMN CtrCod INT NOT NULL DEFAULT -1 AFTER InsCod;
ALTER TABLE centres ADD COLUMN Logo VARCHAR(16) NOT NULL AFTER FullName;
INSERT INTO actions (ActCod,Language,Obsolete,Txt) VALUES ('1051','es','N','Cambiar logo de centro');

	Version 11.71.2: Jul 07, 2012	Changes in mobile layout. (134228 lines)
	Version 11.71.1: Jul 06, 2012	Fixed minor bug in selection of centres. (134227 lines)
	Version 11.71:   Jul 05, 2012	Lot of changes in countries, institutions, centres, degrees and courses.
					DegTypCod removed from sessions table. (134220 lines)
	Version 11.70.1: Jul 05, 2012	Fixed bug in ordering of centres. (134249 lines)
	Version 11.70:   Jul 05, 2012	Changes in edition of centres and institutions. (134246 lines)
	Version 11.69.1: Jul 04, 2012	Fixed bug related to degree types and institutions. (134192 lines)
	Version 11.69:   Jul 04, 2012	Field InsCod removed from tables of degrees and degree types. (134211 lines)
	Version 11.68:   Jul 03, 2012	Changes in edition of degrees.
					Fixed bug in edition of degrees. (134198 lines)
	Version 11.67.1: Jun 29, 2012	A degree must belong to a centre, it can not belong to an unknown centre. (134239 lines)
	Version 11.67:   Jun 29, 2012	Lot of changes in selection of institutions, centres, degrees. (134293 lines)
	Version 11.66:   Jun 28, 2012	A degree now belongs to a centre.
					Changes in selection of centres and degree types. (134203 lines)
	Version 11.65.1: Jun 27, 2012	Changes in edition of degrees. (133930 lines)
	Version 11.65:   Jun 25, 2012	Changes in edition of degrees, centres and departments.
					Fixed bug in selection of degrees.
					lockfile based semaphore is no longer used. (133915 lines)
	Version 11.64.8: Jun 25, 2012	Changes in selection of countries.
					Fixed bug in edition of institutions. (133966 lines)
	Version 11.64.7: Jun 23, 2012	More changes resulting from the use of -Wextra. (133946 lines)
	Version 11.64.6: Jun 22, 2012	Using Eclipse CDT and -Wextra. (133928 lines)
	Version 11.64.5: Jun 19, 2012	Changes related with the command to send automatic emails. (133865 lines)
	Version 11.64.4: Jun 18, 2012	Changes to use without external authentication service. (? lines)
	Version 11.64.3: Jun 17, 2012	Changes in configuration and layout. (133785 lines)
	Version 11.64.2: Jun 09, 2012	Changes in configuration. (133737 lines)
	Version 11.64.1: Jun 02, 2012	Changes in internal directories. (133736 lines)
	Version 11.64:   Jun 01, 2012	Lot of changes in internal directories. (133793 lines)
	Version 11.63.2: May 31, 2012	Changes in names of variables related to file browsers. (133778 lines)
	Version 11.63.1: May 28, 2012	Changes in configuration, for installation in Ubuntu. (133737 lines)
	Version 11.63:   May 28, 2012	Changes in configuration, for installation in Ubuntu. (133735 lines)
	Version 11.62.6: May 04, 2012	Changes in link to download a file. (133630 lines)
	Version 11.62.5: May 02, 2012	Several messages translated (still 28 messages to be translated).
					Changes in file metadata. (133611 lines)
	Version 11.62.4: May 01, 2012	File extensions 12x12 icons are replaced by 16x16 icons. (133335 lines)
	Version 11.62.3: May 01, 2012	New file extensions icons, in 12x12 and 32x32 sizes. (133330 lines)
	Version 11.62.2: Apr 30, 2012	Fixed bug in file browsers. (133327 lines)
	Version 11.62.1: Apr 30, 2012	Fixed bug in file browsers. (133434 lines)
	Version 11.62:   Apr 29, 2012	Files are not linked directly. User must click on filename to access file metadata and download it.
					Number of views of a file is stored in database. (133427 lines)
	Version 11.61:   Apr 29, 2012	New fields size and date in file metadata. (133230 lines)
	Version 11.60.5: Apr 28, 2012	Change related with file metadata. (133188 lines)
	Version 11.60.4: Apr 28, 2012	File metadata is extended to the briefcase. (133181 lines)
	Version 11.60.3: Apr 28, 2012	File metadata is extended to the zones of course assignments and course works. (133167 lines)
	Version 11.60.2: Apr 28, 2012	File metadata is extended to the zones of my assignments and my works. (133129 lines)
	Version 11.60.1: Apr 28, 2012	File metadata is extended to the zone of administration of files of marks. (133099 lines)
	Version 11.60:   Apr 28, 2012	Link to download file when showing file metadata. (133056 lines)
	Version 11.59.5: Apr 26, 2012	Fixed bug in file browsers, reported by Francisco Miguel Reyes Rodr�guez. (133010 lines)
	Version 11.59.4: Apr 26, 2012	Fixed bug in user's briefcase. (133007 lines)
	Version 11.59.3: Apr 26, 2012	Group file zones can not have public files. (133006 lines)
	Version 11.59.2: Apr 26, 2012	Lot of changes in file zones and groups. (132983 lines)
	Version 11.59.1: Apr 25, 2012	Changes in selection of file browser zone. (132926 lines)
	Version 11.59:   Apr 25, 2012	Changes in selection of file browser zone. (132917 lines)
	Version 11.58.4: Apr 24, 2012	Width in number of survey answer is now fixed. (132938 lines)
	Version 11.58.3: Apr 23, 2012	Fixed bug in table of files. (132938 lines)
	Version 11.58.2: Apr 22, 2012	Course common zone is now public (only for public files, of course). (132905 lines)
	Version 11.58.1: Apr 22, 2012	Added unknown/other license for files. (132894 lines)
	Version 11.58:   Apr 22, 2012	File license is stored in database.
					Changes in edition of file properties.
					Publicity/license of files made available for users. (132850 lines)
	Version 11.57.1: Apr 22, 2012	File/folder names are limited in length when shown. (132832 lines)
	Version 11.57:   Apr 21, 2012	Database table 'file_public' is removed. The publicity of a file is stored in table 'files'. (132826 lines)
	Version 11.56.1: Apr 21, 2012	Changes in file browser layouts. (132920 lines)
	Version 11.56:   Apr 21, 2012	All the folders/files are stored in database, including the creator. (132920 lines)
	Version 11.55.10:Apr 20, 2012	Internal optimizations in functions related to file browsers. (132889 lines)
	Version 11.55.10:Apr 19, 2012	New field 'FileBrowser' in database table 'files'. (132853 lines)
	Version 11.55.9: Apr 19, 2012	Database table 'crs_SHARE_files' is renamed to files. (132829 lines)
	Version 11.55.8: Apr 19, 2012	File licenses translated to several languages. (132828 lines)
	Version 11.55.7: Apr 19, 2012	Changes in web service function getCourses. (132827 lines)
	Version 11.55.6: Apr 18, 2012	Changes in selection of languages. (132657 lines)
	Version 11.55.5: Apr 18, 2012	Fixed bug in file browsers. Changed icon for nickname action. (132580 lines)
	Version 11.55.4: Apr 15, 2012	Minor changes in web service. (132575 lines)
	Version 11.55.3: Apr 15, 2012	New function getGroups in the web service. (132571 lines)
	Version 11.55.2: Apr 14, 2012	New function getDirectoryTree in the web service. (132478 lines)
	Version 11.55.1: Apr 13, 2012	Now the privacy of a file changes when receiving the form. (132443 lines)
	Version 11.55:   Apr 12, 2012	New options to edit metadata of a file. (132421 lines)
	Version 11.54:   Apr 12, 2012	Documents are open to public not logged. (132323 lines)
	Version 11.53.4: Apr 11, 2012	Changes in search of courses. (132258 lines)
	Version 11.53.3: Apr 04, 2012	Changes in selection of courses.
					Changes in "About" messages. (132224 lines)
	Version 11.53.2: Apr 04, 2012	Changes in selection of countries and institutions. (132232 lines)
	Version 11.53.1: Apr 04, 2012	Changes in selection of countries and institutions.
					Fixed bug in web service function getCourses, reported by Helena Rodr�guez Gij�n. (132247 lines)
	Version 11.53:   Apr 03, 2012	Changes in web service functions. (132237 lines)
	Version 11.52.3: Apr 02, 2012	Lot of words translated to Guarani. (132323 lines)
	Version 11.52.2: Apr 01, 2012	More words translated to Guarani. (132323 lines)
	Version 11.52.1: Apr 01, 2012	Some words translated to Guarani.
					Fixed bug in edition of institutional links. (132321 lines)
	Version 11.52:   Apr 01, 2012	New language: Guarani. Some words translated. (132314 lines)
	Version 11.51.1: Mar 28, 2012	The number of indicators is shown in configuration of a course. (127821 lines)
	Version 11.51:   Mar 28, 2012	Changes in selection of courses. (127838 lines)
	Version 11.50.3: Mar 27, 2012	Removed view of XHTML code for direct access to a course. (127844 lines)
	Version 11.50.2: Mar 26, 2012	Optimizations in links to print views. (127917 lines)
	Version 11.50.1: Mar 26, 2012	Minor changes in configuration of the current course. (127938 lines)
	Version 11.50:   Mar 26, 2012	Course year and semester can be edited in configuration of the current course.
					Print view of configuration of the current course. (127936 lines)
	Version 11.49.1: Mar 26, 2012	Legal notice included in file browsers. (127830 lines)
	Version 11.49:   Mar 26, 2012	New table IP_last to prevent more than one click from the same IP in the same second. (127816 lines)
	Version 11.48.2: Mar 26, 2012	Optimization in layout of class photo. (127762 lines)
	Version 11.48.1: Mar 25, 2012	Configuration of the current course is available for all users. (127784 lines)
	Version 11.48:   Mar 25, 2012	More information in configuration of the current course. (127771 lines)
	Version 11.47.2: Mar 24, 2012	Changes in parameters with action and tab.
					QR code with link to an exam announcement. (127660 lines)
	Version 11.47.1: Mar 18, 2012	Few changes in selectors of country, institution, degree type, degree, year and course. (127635 lines)
	Version 11.47:   Mar 18, 2012	Changes in selectors of country, institution, degree type, degree, year and course. (127670 lines)
	Version 11.46.3: Mar 18, 2012	Added selectors of country, institution and degree type before list of degrees. (127691 lines)
	Version 11.46.2: Mar 17, 2012	Added selector of country before list of institutions.
					Added selectors of country and institution before list of degree types.
					Fixed bug in web service, reported by Juan Miguel Boyero Corral. (127691 lines)
	Version 11.46.1: Mar 16, 2012	Fixed bugs in forms with country or institution codes. (127648 lines)
	Version 11.46:   Mar 15, 2012	Added selectors Country and Institution to menus for selection of courses. (127623 lines)
	Version 11.45:   Mar 15, 2012	Lot of changes in selections of countries in the "System" tab. (127429 lines)
	Version 11.44:   Mar 14, 2012	Lot of changes in selections of institutions and degree types in the "System" tab. (127314 lines)
	Version 11.43:   Mar 13, 2012	Changes in order of options in the "System" tab. (127178 lines)
	Version 11.42.5: Mar 12, 2012	Fixed bug while getting actions from database when actions table is empty.
					Fixed bug while counting number of students in countries. (127169 lines)
	Version 11.42.4: Mar 11, 2012	Changes related to configuration, setup, directories. (127163 lines)
	Version 11.42.3: Mar 08, 2012	Changed "alumno/a" to "estudiante" in Spanish and Catal�. (127155 lines)
	Version 11.42.2: Mar 08, 2012	Configuring swad to work when no database available. (127154 lines)
	Version 11.42.1: Mar 07, 2012	Configuring swad to work when no database available. (127141 lines)
	Version 11.42:   Mar 04, 2012	New function sendNotice in the web service. Still need to send notifications. (127136 lines)
	Version 11.41.1: Mar 04, 2012	Changes in selection of degree types, degrees and courses for mobile version. (127061 lines)
	Version 11.41:   Mar 04, 2012	Changes in selection of degree types, degrees and courses for desktop version. (127044 lines)
	Version 11.40.5: Mar 04, 2012	Passing parameter with the degree type if no session opened. (127028 lines)
	Version 11.40.4: Feb 27, 2012	Minor change in connected users. (127038 lines)
	Version 11.40.3: Feb 26, 2012	Fixed bug sending notifications by email. (127037 lines)
	Version 11.40.2: Feb 26, 2012	Removed <acronym> directive. (127036 lines)
	Version 11.40.1: Feb 13, 2012	Fixed bug in notifications, reported by prof. Antonio Gonz�lez Casado. (127045 lines)
	Version 11.40:   Feb 07, 2012	New module swad_QR.
					QR codes can be selected with IDs or nicknames.
					'@' is inserted automatically and the start of a Twitter account. (127021 lines)
	Version 11.39:   Feb 06, 2012	The institutional code of a course can be changed in the edition of courses. (126819 lines)
	Version 11.38:   Feb 06, 2012	The institutional code of a course can be changed in the configuration of the course. (126704 lines)
	Version 11.37.6: Feb 04, 2012	Several messages translated (still 25 messages to be translated). (126659 lines)
	Version 11.37.5: Feb 04, 2012	Several messages translated (still 38 messages to be translated). (126411 lines)
	Version 11.37.4: Feb 04, 2012	New column in table of indicators with the number of indicators. (126238 lines)
	Version 11.37.3: Feb 04, 2012	New module swad_indicator. (126228 lines)
	Version 11.37.2: Feb 03, 2012	New filter to select the number of indicators of courses. (126127 lines)
	Version 11.37.1: Feb 03, 2012	Lot of changes in indicators of courses. (126085 lines)
	Version 11.37:   Feb 02, 2012	A brief version of indicators of courses is shown directly in the main window. (126032 lines)
	Version 11.36.2: Feb 01, 2012	Changes in indicators of courses.
					Parameter NxtAct is changed to ActCod. (125916 lines)
	Version 11.36.1: Jan 30, 2012	Changes in layout. (125867 lines)
	Version 11.36:   Jan 30, 2012	Statistics about courses can be filtered by type of degree. (125839 lines)
	Version 11.35:   Jan 30, 2012	Statistics about courses can be filtered by department. (125754 lines)
	Version 11.34:   Jan 29, 2012	Now teachers can select if the students in a course may log in directly or from the external authentication service. (125666 lines)
	Version 11.33:   Jan 23, 2012	If a student logs in directly, and the course selected belongs to a degree without direct authentication, his/her role is changed to invited.
					The duration of a session is changed from 1 hour to 2 hours. (125407 lines)
	Version 11.32.1: Jan 20, 2012	Published the new option to show user's QR code. (125405 lines)
	Version 11.32:   Jan 20, 2012	New option to show user's QR code. (125393 lines)
	Version 11.31.1: Jan 19, 2012	Changes in Google Geochart settings for listing of countries. (125306 lines)
	Version 11.31:   Jan 19, 2012	Added Google Geochart to list of countries. (125268 lines)
	Version 11.30.8: Jan 17, 2012	Fixed bug when exporting test questions. (125186 lines)
	Version 11.30.7: Jan 15, 2012	Several messages translated (still 39 messages to be translated). (125168 lines)
	Version 11.30.6: Jan 14, 2012	Several messages translated (still 53 messages to be translated).
					Other minor changes. (124913 lines)
	Version 11.30.5: Jan 14, 2012	Pending notifications are sent by email on refresh actions. (124802 lines)
	Version 11.30.4: Jan 14, 2012	When a user is removed from a course or from a group, it is better not to set notifications as inaccessible because the user can be enroled again. (124802 lines)
	Version 11.30.3: Jan 14, 2012	When a course is removed, the notifications in that course are set as removed.
					When a user is removed from a course or from a group, the notifications inaccessible are set as inaccessible. (124842 lines)
	Version 11.30.2: Jan 13, 2012	When a group is removed, the notifications about files of marks in that group are set as removed. (124757 lines)
	Version 11.30.1: Jan 11, 2012	Lot of changes in notifications. (124739 lines)
	Version 11.30:   Jan 10, 2012	The marks of a student are sent in HTML as the content of a notification via web service. (124707 lines)
	Version 11.29.5: Jan 10, 2012	Notifications marked as removed when a file of marks, or a folder which contains file(s) of marks, is removed. (124706 lines)
	Version 11.29.4: Jan 10, 2012	Changes in notification of new file of marks. (124660 lines)
	Version 11.29.3: Jan 10, 2012	Changes in notification of new file of marks. (124589 lines)
	Version 11.29.2: Jan 09, 2012	Several functions moved from swad_file_browser to swad_mark. (? lines)
	Version 11.29.1: Jan 03, 2012	Fixed bug in notifications of new messages. Reported by Gu�a Carmona Tienda. (124516 lines)
	Version 11.29:   Dec 30, 2011	Optimizations when deleting all the messages of a user or several users. (124513 lines)
	Version 11.28.1: Dec 28, 2011	Changes in layout. (124471 lines)
	Version 11.28:   Dec 23, 2011	New options to contract an expanded message. (124470 lines)
	Version 11.27.2: Dec 22, 2011	Optimizations on pagination of items. (124389 lines)
	Version 11.27.1: Dec 22, 2011	Number of new messages is removed. (124461 lines)
	Version 11.27:   Dec 21, 2011	Changes in received and sent messages.
					When a user clicks on a notification of message, that message is expanded. (124461 lines)
	Version 11.26.2: Dec 20, 2011	Notifications of removed events are marked with light colors. (124430 lines)
	Version 11.26.1: Dec 19, 2011	Notifications of removed events are marked with red background. (124279 lines)
	Version 11.26:   Dec 19, 2011	Notifications are marked as removed when the source of the notification is removed.
					Notifications marked as removed are shown, but not as new notifications. (124231 lines)
	Version 11.25.3: Dec 17, 2011	Several messages translated. (124191 lines)
	Version 11.25.2: Dec 12, 2011	Fixed bug when writing a private message.
					Removing the use of the struct for another user in parst of the code. (124044 lines)
	Version 11.25.1: Dec 11, 2011	More changes in layout. (124011 lines)
	Version 11.25:   Dec 10, 2011	More changes in layout. (124038 lines)
	Version 11.24.3: Dec 09, 2011	Optimization in header layout. (123954 lines)
	Version 11.24.2: Dec 09, 2011	Reordering of several options in groups. (123952 lines)
	Version 11.24.1: Dec 08, 2011	Fixed bug in test tags related to sending tests to plugins via web service, reported by Juan Miguel Boyero Corral. (123951 lines)
	Version 11.24:   Dec 08, 2011	Lot of changes in layout.
					Options are grouped in menus. (123948 lines)
	Version 11.23.2: Dec 07, 2011	More changes in layout. (123982 lines)
	Version 11.23.1: Dec 06, 2011	More changes in layout. (123980 lines)
	Version 11.23:   Dec 06, 2011	Lot of changes in layout. (123975 lines)
	Version 11.22.9: Nov 29, 2011	More optimizations for big lists of users. (124061 lines)
	Version 11.22.8: Nov 28, 2011	Fixed problem with big lists of users, reported by Francisco A. Oca�a Lara. (124056 lines)
	Version 11.22.7: Nov 28, 2011	Fixed bug when registering users in a course. (124051 lines)
	Version 11.22.6: Nov 28, 2011	More optimizations for big lists of users. (124050 lines)
	Version 11.22.5: Nov 28, 2011	More optimizations for big lists of users. (124117 lines)
	Version 11.22.4: Nov 28, 2011	Fixed bug in list of teacher, reported by Francisco Serrano Carmona.
					Fixed bug in link to teacher's card. (124099 lines)
	Version 11.22.3: Nov 28, 2011	More optimizations for big lists of users. (124116 lines)
	Version 11.22.2: Nov 28, 2011	Fixed bugs in lists of users. (124077 lines)
	Version 11.22.1: Nov 27, 2011	Lot of optimizations in lists of users. (123959 lines)
	Version 11.22:   Nov 26, 2011	Lot of optimizations in lists of users. (124073 lines)
	Version 11.21.3: Nov 25, 2011	Optimizations in lists of users. (124080 lines)
	Version 11.21.2: Nov 25, 2011	New form to confirm that user wants to see big lists of students. ( lines)
	Version 11.21.1: Nov 25, 2011	Indexes for ordering lists of users are no longer needed, so they are removed. (124130 lines)
	Version 11.21	:Nov 24, 2011	Users lists are now ordered in database queries, not in C postprocessing.
					Bug in message about allowed post, reported by Francisco A. Oca�a Lara. (124115 lines)
	Version 11.20.3	:Nov 23, 2011	Optimization when renaming test tags. (124070 lines)
	Version 11.20.2	:Nov 23, 2011	Fixed bug when renaming test tags. (124095 lines)
	Version 11.20.1	:Nov 22, 2011	Accesses to web service are highlighted in Stats > Last clicks. (124068 lines)
	Version 11.20	:Nov 22, 2011	New option to list banned senders of private messages.
					A user can not send messages to users who have banned him/her. (124015 lines)
	Version 11.19	:Nov 22, 2011	New option to ban/unban senders of private messages. (123833 lines)
	Version 11.18.2: Nov 22, 2011	Starting new option to ban/unban senders of private messages. (123770 lines)
	Version 11.18.1: Nov 20, 2011	Internal changes in marks. Now a temporary html file is used to store student's marks. (123705 lines)
	Version 11.18:   Nov 19, 2011	Teachers must indicate if they want that test may be pluggable or not. (123597 lines)
	Version 11.17:   Nov 19, 2011	New statistics related with accesses from plugins. (123434 lines)
	Version 11.16:   Nov 17, 2011	New database table to log the web functions called. (123238 lines)
	Version 11.15.4: Nov 17, 2011	When a user closes the session, the click is logged with his/her current role, not with unknown role as before. (123159 lines)
	Version 11.15.3: Nov 14, 2011	Web service action is logged into log table. (123102 lines)
	Version 11.15.2: Nov 14, 2011	Added legal data protection clause when editing personal record. (123097 lines)
	Version 11.15.1: Nov 13, 2011	New institutional course codes now can overwrite previous institutional course codes when importing students. (122920 lines)
	Version 11.15:   Nov 12, 2011	Institutional course codes are imported when importing students to a course.
					Institutional course codes are shown in lists of courses. (122921 lines)
	Version 11.14:   Nov 08, 2011	New option to show degree types. (122875 lines)
	Version 11.13:   Nov 08, 2011	New option to show degrees. (122725 lines)
	Version 11.12.2: Nov 06, 2011	Fixed bug when accesing table for translating unique action code to Act_Action_t. (122509 lines)
	Version 11.12.1: Nov 06, 2011	Invariable action code ActCod is stored in table of MFU actions instead of Action. (122510 lines)
	Version 11.12:   Nov 06, 2011	Using new table for translating unique action code to Act_Action_t. (122494 lines)
	Version 11.11.3: Nov 05, 2011	Building new table for translating unique action code to Act_Action_t. (122889 lines)
	Version 11.11.2: Nov 04, 2011	Changes in listing of courses of a degree. (124523 lines)
	Version 11.11.1: Nov 04, 2011	Changes in listing of courses of a degree. (124517 lines)
	Version 11.11:   Nov 03, 2011	New option to show courses of a degree. (124535 lines)
	Version 11.10.2: Nov 02, 2011	Optimization in XML module. (124342 lines)
	Version 11.10.1: Nov 01, 2011	Removed extra parameters from Actions struct. (124358 lines)
	Version 11.10:   Nov 01, 2011	When a user changes the tab, her/his most used option option is shown. (124374 lines)
	Version 11.9:    Nov 01, 2011	When a user changes the tab, the first available option is shown. (124351 lines)
	Version 11.8.3:  Oct 28, 2011	Sharing code to put 16x16 icons. (124331 lines)
	Version 11.8.2:  Oct 26, 2011	Size of some icons changed from 32x32 to 16x16. (124428 lines)
	Version 11.8.1:  Oct 26, 2011	Small changes in XML format when exporting/importing test questions.
					Added MIME type for documents uploaded. (124423 lines)
	Version 11.8:    Oct 25, 2011	Exporting/importing test question is published for teachers. (124406 lines)
	Version 11.7.4:  Oct 25, 2011	Size of more icons changed from 12x12 to 32x32.
					Optimizations in test module. (124405 lines)
	Version 11.7.3:  Oct 25, 2011	Size of some icons changed from 12x12 to 32x32. (124365 lines)
	Version 11.7.2:  Oct 24, 2011	Fixed lot of bugs and optimized code in test module. (124339 lines)
	Version 11.7.1:  Oct 24, 2011	Fixed bugs when importing questions are storing them in database. (124245 lines)
	Version 11.7:    Oct 24, 2011	Imported questions are stored in database. (124225 lines)
	Version 11.6.12: Oct 24, 2011	Imported questions are prepared to be stored in database. (124221 lines)
	Version 11.6.11: Oct 23, 2011	Fixed bug in XML parsing.
					Implementing exporting/importing of test questions. (124191 lines)
	Version 11.6.10: Oct 23, 2011	Implementing exporting/importing of test questions. Check the existence of each imported question in database. (124031 lines)
	Version 11.6.9:  Oct 22, 2011	Implementing exporting/importing of test questions. (123971 lines)
	Version 11.6.8:  Oct 22, 2011	Implementing exporting/importing of test questions. (123854 lines)
	Version 11.6.7:  Oct 22, 2011	Implementing exporting/importing of test questions. (123855 lines)
	Version 11.6.6:  Oct 21, 2011	Implementing exporting/importing of test questions. (123786 lines)
	Version 11.6.5:  Oct 21, 2011	Implementing exporting/importing of test questions (changes in XML file). (123821 lines)
	Version 11.6.4:  Oct 19, 2011	Implementing exporting/importing of test questions (changes in XML file). (123821 lines)
	Version 11.6.3:  Oct 19, 2011	Implementing exporting/importing of test questions (changes in XML file). (123835 lines)
	Version 11.6.2:  Oct 19, 2011	Implementing new functions to analyze and get XML elements. (123835 lines)
	Version 11.6.1:  Oct 18, 2011	Implementing new functions to analyze and get XML elements. (123719 lines)
	Version 11.6:    Oct 17, 2011	New functions to analyze and get XML elements. (123619 lines)
	Version 11.5.2:  Oct 16, 2011	Implementing importing of test questions. (123443 lines)
	Version 11.5.1:  Oct 16, 2011	New actions to implement importing of test questions. (123407 lines)
	Version 11.5:    Oct 13, 2011	New field in table of imported groups to store the external course code. Suggested by Francisco A. Oca�a Lara. (123342 lines)
	Version 11.4.3:  Oct 12, 2011	Changes in XML format for test questions. (123338 lines)
	Version 11.4.2:  Oct 11, 2011	Implementing importing of test questions. (123314 lines)
	Version 11.4.1:  Oct 10, 2011	Implementing exporting of test questions (changes in XML file). (123130 lines)
	Version 11.4:    Oct 10, 2011	Implementing exporting of test questions (correct XML file created). (123150 lines)
	Version 11.3.2:  Oct 09, 2011	Fixed bugs in edition of group types.
					Implementing exporting of test questions. (123075 lines)
	Version 11.3.1:  Oct 09, 2011	Implementing exporting of test questions. (122900 lines)
	Version 11.3:    Oct 09, 2011	The option to edit a new test question is integrated inside another option. (122843 lines)
	Version 11.2.2:  Oct 08, 2011	Warning to alert that in the future, users must be logged in from external service in some degree types. (122855 lines)
	Version 11.2.1:  Oct 08, 2011	Changes in the selection of one of my courses. (122847 lines)
	Version 11.2:    Oct 07, 2011	When a user directly logged belongs to the current course, but the current degree requires external log in, the role is changed to invited. (122822 lines)
	Version 11.1.1:  Oct 06, 2011	Changes in edition of degree types. (122819 lines)
	Version 11.1:    Oct 06, 2011	New fields in degree types to select between login directly or login from the external authentication service. (122785 lines)
	Version 11.0.3:  Oct 04, 2011	Changes in layout of users' common records. (122754 lines)
	Version 11.0.2:  Oct 02, 2011	New link to export test questions (not yet implemented). (122724 lines)
	Version 11.0.1:  Oct 02, 2011	New icons for groups. (122704 lines)
	Version 11.0:    Sep 29, 2011	Groups edition splitted into two options: group types edition and groups edition. (122703 lines)
	Version 10.57.3: Sep 27, 2011	Fixed bugs in WS function sendMessage. (122628 lines)
	Version 10.57.2: Sep 25, 2011	Fixed bug in users' listing. (122629 lines)
	Version 10.57.1: Jul 19, 2011	Optimizations on file browser. (122536 lines)
	Version 10.57:   Jul 18, 2011	Files in documents and common zones will be able to be public. Only available for superusers at the moment. (122535 lines)
	Version 10.56.2: Jul 18, 2011	Fixed bug in WS function sendMessage, reported by Juan Miguel Boyero Corral. (122206 lines)
	Version 10.56.1: Jul 05, 2011	Lost of changes in the new option to show last clicks in real time. (122209 lines)
	Version 10.56:   Jul 05, 2011	New option to show last clicks in real time. (122104 lines)
	Version 10.55:   Jul 04, 2011	Finished programming of new WS function sendMessage. (121957 lines)
	Version 10.54.3: Jul 04, 2011	Optimization in sending of a message. (121868 lines)
	Version 10.54.2: Jul 03, 2011	Programming new WS function sendMessage. (121922 lines)
	Version 10.54.1: Jul 02, 2011	Programming new WS function sendMessage. (121865 lines)
	Version 10.54:   Jul 02, 2011	New field added to WS function getUsers.
					Programming new WS function sendMessage. (121829 lines)
	Version 10.53.5: Jul 02, 2011	New fields added to WS function getNotifications. (121749 lines)
	Version 10.53.4: Jun 30, 2011	Changes in database tables. (121743 lines)
	Version 10.53.3: Jun 30, 2011	Change in enrolment of users with a new role. (121742 lines)
	Version 10.53.2: Jun 14, 2011	New statistics about number of test questions. (121716 lines)
	Version 10.53.1: Jun 13, 2011	New statistics about number of test questions. (121638 lines)
	Version 10.53:   Jun 10, 2011	A lot of messages translated to Polish by Wojtek Kieca, Tomasz Olechowski and Mateusz Stanko, students from Zespol Szkol Techniczny nr. 9. (121585 lines)
	Version 10.52.2: Jun 02, 2011	Changes in styles for statistics of use of the platform. (121635 lines)
	Version 10.52.1: Jun 02, 2011	Statistics about number of test questions splitted by type of answer. (121604 lines)
	Version 10.52:   Jun 01, 2011	New statistics about number of test questions. (121527 lines)
	Version 10.51:   Jun 01, 2011	New statistics about number of assignments. (121418 lines)
	Version 10.50.4: May 31, 2011	Changes in statistics about number of users with Twitter and Skype. (121294 lines)
	Version 10.50.3: May 30, 2011	New statistics about number of users with Twitter and Skype. (121273 lines)
	Version 10.50.2: May 30, 2011	New statistics about number of surveys. (121192 lines)
	Version 10.50.1: May 26, 2011	Preparing statistics about number of assignments and number of test questions. (121132 lines)
	Version 10.50:   May 26, 2011	Statistics about number of surveys. (121069 lines)
	Version 10.49:   May 24, 2011	New action to show my recent most frequently used actions. (120917 lines)
	Version 10.48.2: May 24, 2011	Fixed bug in list of countries. (120760 lines)
	Version 10.48.1: May 24, 2011	Focus is set to login form. Suggested by Alberto Villegas Erce. (120750 lines)
	Version 10.48:   May 23, 2011	Shortened the maximum length of Skype user name from 255 to 60.
					New field in user's data for Twitter. (120745 lines)
	Version 10.47.3: May 18, 2011	Several messages translated to catalan by Joan Llu�s D�az Rodr�guez. (120618 lines)
	Version 10.47.2: May 13, 2011	The notifications about assignments and surveys are marked as seen when the user clicks on them. (120616 lines)
	Version 10.47.1: May 13, 2011	Changes in notifications about surveys. (120631 lines)
	Version 10.47:   May 12, 2011	Assignments and surveys are notified. (120621 lines)
	Version 10.46:   May 12, 2011	Preparing code and database for two new notifications. (120412 lines)
	Version 10.45.6: May 11, 2011	Several messages translated to Catalan. (120249 lines)
	Version 10.45.5: May 11, 2011	Several messages translated to Catalan. (120248 lines)
	Version 10.45.4: May 10, 2011	Several messages translated to Catalan. (120247 lines)
	Version 10.45.3: May 10, 2011	Several messages translated to Catalan.
					New field in countries table for names in Catalan. (120227 lines)
	Version 10.45.2: May 10, 2011	Several messages translated to Catalan. (120221 lines)
	Version 10.45.1: May 10, 2011	Several messages translated to Catalan. (120220 lines)
	Version 10.45:   May 09, 2011	Code ready for translation to Catalan. (120219 lines)
	Version 10.44.6: May 09, 2011	Translating to Catalan.
					Fixed bug in web service function getTest, reported by Juan Miguel Boyero Corral. (120019 lines)
	Version 10.44.5: May 06, 2011	New web service function getTestConfig.
					Changes in web service function getTests. (115982 lines)
	Version 10.44.4: May 05, 2011	Fixed bug in web service function getTest, reported by Juan Miguel Boyero Corral. (115920 lines)
	Version 10.44.3: May 05, 2011	Changes storing and processing the number of new notifications. (115923 lines)
	Version 10.44.2: May 04, 2011	Removed matching of external role with any of the internal roles availables for a user. (115917 lines)
	Version 10.44.1: May 02, 2011	Teachers can configure if they want to deliver test questions to plugins. (115912 lines)
	Version 10.44:   May 01, 2011	New field ChangeTime in tst_tags table.
					getTests function completed. (115854 lines)
	Version 10.43.4: Apr 30, 2011	Implementing getTests function, part of the web service. (115823 lines)
	Version 10.43.3: Apr 30, 2011	Implementing getTests function, part of the web service. (115708 lines)
	Version 10.43.2: Apr 28, 2011	Implementing getTests function, part of the web service. (115590 lines)
	Version 10.43.1: Apr 26, 2011	Several messages translated. (115422 lines)
	Version 10.43:   Apr 25, 2011	Hundred of messages (1263 lines) translated (or revised) to italian, by Giuseppe Antonio Pagin & Antonella Grande. (115298 lines)
	Version 10.42.2: Apr 13, 2011	Messages are marked as open and notifications are marked as seen when they are sent via web service. (115288 lines)
	Version 10.42.1: Apr 13, 2011	The summary of a notification is trimmed before sending it via web service. (115265 lines)
	Version 10.42:   Apr 13, 2011	The summary of a notification is now splitted in brief summary and full content.
					WSDL has changed. (115260 lines)
	Version 10.41.8: Apr 09, 2011	Corrections in italian translation. (115190 lines)
	Version 10.41.7: Apr 09, 2011	Several messages translated to italian, by Francisco Manuel Herrero P�rez. (115191 lines)
	Version 10.41.6: Apr 09, 2011	The teaching guide is considered as an indicator of assessment criteria. (115188 lines)
	Version 10.41.5: Apr 08, 2011	The full content of a notification is sent in web service. (115184 lines)
	Version 10.41.4: Apr 08, 2011	Fixed bug when getting data of a centre. (115169 lines)
	Version 10.41.3: Apr 07, 2011	Fixed bug in assigment dates, reported by Jos� Samos Guti�rrez. (115169 lines)
	Version 10.41.2: Apr 04, 2011	Several messages translated to polish. (115167 lines)
	Version 10.41.1: Apr 04, 2011	Several messages translated to polish. (115149 lines)
	Version 10.41:   Apr 03, 2011	Added polish language (translation needed). (115154 lines)
	Version 10.40.2: Apr 01, 2011	Changes in AJAX to refresh in the current language. (110963 lines)
	Version 10.40.1: Apr 01, 2011	Language is no longer stored in IP_prefs table.
					Changes related to languages. (110958 lines)
	Version 10.40:   Apr 01, 2011	The big change to a new translation system has been finished. (110943 lines)
	Version 10.39.2: Mar 31, 2011	The big change to a new translation system continues. (110895 lines)
	Version 10.39.1: Mar 30, 2011	The big change to a new translation system continues. (111028 lines)
	Version 10.39:   Mar 25, 2011	One swad for each language instead of the same swad for all languages. (101215 lines)
	Version 10.38.4: Mar 25, 2011	Changes in selector of courses. (101004 lines)
	Version 10.38.3: Mar 23, 2011	Changed search icon size from 32x32 to 20x20. (101018 lines)
	Version 10.38.2: Mar 22, 2011	Changed search icon size from 20x20 to 32x32. (101017 lines)
	Version 10.38.1: Mar 21, 2011	Changed warning message related to external login service. (101016 lines)
	Version 10.38:   Mar 14, 2011	Superusers can see users connected belonging to current course, degree or platform. (101013 lines)
	Version 10.37.3: Mar 13, 2011	Optimization querying number of users in an institution. (100727 lines)
	Version 10.37.2: Mar 13, 2011	Changes in messages about users not belonging to any course. (100732 lines)
	Version 10.37.1: Mar 12, 2011	Changes in roles stored in table of connected users. (100706 lines)
	Version 10.37:   Mar 10, 2011	Lists of institutions, degrees and courses now have an attribute with the role of the user.
					A lot of changes related to permissions to see forums. (100659 lines)
	Version 10.36:   Mar 10, 2011	The number of students in a group is not stored in database, but counted when necessary. (100585 lines)
	Version 10.35.4: Mar 10, 2011	Users who do not belong to any course are logged as invited.
					Changes in permissions related to forums and chat.
					Fixed bug counting number of users in countries. (100666 lines)
	Version 10.35.3: Mar 10, 2011	Removed obsolete action to register an existing user in a course. (100666 lines)
	Version 10.35.2: Mar 10, 2011	Fixed bug in statistics of users in a course, reported by Nicola Comunale Rizzo. (100706 lines)
	Version 10.35.1: Mar 10, 2011	Counting number of students in a group instead of getting directly. This change should be reversed if slow. (100725 lines)
	Version 10.35:   Mar 10, 2011	Changes and bug fixes counting number of students in groups. (? lines)
	Version 10.34:   Mar 09, 2011	Lot of changes in enrolment of users. (100696 lines)
	Version 10.33.3: Mar 08, 2011	Changes in enrolment of users. (100666 lines)
	Version 10.33.2: Mar 08, 2011	Fixed bugs and other changes related to importation of students from an external site. (100659 lines)
	Version 10.33.1: Mar 08, 2011	Few changes related to roles in courses. (100638 lines)
	Version 10.33:   Mar 08, 2011	Added indexes to table crs_usr.
					Removed slow query when getting data of a degree.
					Removing old entries of some tables is made when refreshing users. (100631 lines)
	Version 10.32.3: Mar 08, 2011	Different roles are obtained from crs_usr table, never from usr_data. (100676 lines)
	Version 10.32.2: Mar 08, 2011	List of departments counts only teachers with courses. (100695 lines)
	Version 10.32.1: Mar 07, 2011	List of centres counts only teachers with courses. (100699 lines)
	Version 10.32:   Mar 07, 2011	Changes in global statistics about number of users.
					Fixed bug in query related to search of teachers. (100696 lines)
	Version 10.31.1: Mar 07, 2011	UsrDat->Roles is not computed, but read from usr_data table. (100666 lines)
	Version 10.31:   Mar 06, 2011	A user can be enroled as "teacher" in course A and as "student" in course B. (100660 lines)
	Version 10.30.4: Mar 06, 2011	Changed "UsrType" to "Role" in log and log_recent database tables. (100587 lines)
	Version 10.30.3: Mar 06, 2011	Internal changes related to roles. (100587 lines)
	Version 10.30.2: Mar 06, 2011	Changed "Gender" for "Sex" in database tables. (100580 lines)
	Version 10.30.1: Mar 06, 2011	Changes in database table of connected users. (100582 lines)
	Version 10.30:   Mar 06, 2011	Changes related to roles put into production.
					Few bugs fixed. (100564 lines)
	Version 10.29.1: Mar 06, 2011	Lot of internal changes related to roles. (100564 lines)
	Version 10.29:   Mar 05, 2011	Lot of internal changes related to roles.
					"Gender" is changed for "Sex". (100520 lines)
	Version 10.28.1: Mar 03, 2011	Lot of internal changes related to roles. (? lines)
	Version 10.28:   Mar 03, 2011	Lot of internal changes related to roles. (100386 lines)
	Version 10.27.1: Mar 02, 2011	Internal changes related to roles. (? lines)
	Version 10.27:   Mar 02, 2011	User type related variables and database fields are changed form UsrType to Role, except fields in log tables. (100252 lines)
	Version 10.26:   Mar 01, 2011	After sending a file to a folder, a new form to send another file to the same folder is shown. (100231 lines)
	Version 10.25.10:Mar 01, 2011	Text "Class photo" or "List" is shown near icons in selection of type of users' listing. (100183 lines)
	Version 10.25.9: Mar 01, 2011	Acceptance of enrolment is set to true when importing students from official groups. (100164 lines)
	Version 10.25.8: Feb 22, 2011	beginTime and eventTime are changed to long. Reported by Juan Miguel Boyero Corral. (100127 lines)
	Version 10.25.7: Feb 22, 2011	eventTime is changed from time_t to unsigned long. Reported by Juan Miguel Boyero Corral. (100126 lines)
	Version 10.25.6: Feb 22, 2011	Deleting a user removes her/him from connected and sessions tables. (100125 lines)
	Version 10.25.5: Feb 22, 2011	Notifications affected by a removed message will be removed. Reported by Juan Traverso Viagas. (100119 lines)
	Version 10.25.4: Feb 22, 2011	Changes in actions which wait for lock file.
					Reduction of times to delete entries in several tables. (100111 lines)
	Version 10.25.3: Feb 15, 2011	Fixed bugs in notifications, reported by Abdelilah Aoulad Yachou. (100110 lines)
	Version 10.25.2: Feb 15, 2011	The new SOAP web service function to get notifications now fills a summary of the event. (100104 lines)
	Version 10.25.1: Feb 14, 2011	Changes in the SOAP web service function to get notifications of a user. (100080 lines)
	Version 10.25:   Feb 14, 2011	Almost finished the new SOAP web service function to get notifications of a user. (100071 lines)
	Version 10.24.2: Feb 14, 2011	Change in notifications: a user can see the number of unseen notifications and the number of new notifications. (99952 lines)
	Version 10.24.1: Feb 13, 2011	Removed form to new messages. (99895 lines)
	Version 10.24:   Feb 13, 2011	Blinking warning for new notifications. (99895 lines)
	Version 10.23.2: Feb 12, 2011	Link to edit preferences in notifications. (99812 lines)
	Version 10.23.1: Feb 12, 2011	Fixed bug in status of notifications when sent by email. (99797 lines)
	Version 10.23:   Feb 12, 2011	All the events are listed in Messages > Notifications, even those who don't have to be sent by email. (99795 lines)
	Version 10.22.7: Feb 11, 2011	"Sent" and "Seen" are replaced by status in notif table. (? lines)
	Version 10.22.6: Feb 11, 2011	Mark filenames are shown in notifications. (99743 lines)
	Version 10.22.5: Feb 10, 2011	Internal changes related to notifications. (99707 lines)
	Version 10.22.4: Feb 10, 2011	A short view of exam announcements is included in notifications.
					Marks code is added to marks table, in order to be used in notifications table. (99725 lines)
	Version 10.22.3: Feb 10, 2011	Forum type is no longer stored in notifications table. Instead, forum type is got from post code.
					Fixed bug when storing in notifications table the institution, degree and course codes for forums. (99679 lines)
	Version 10.22.2: Feb 10, 2011	Exam announcement code is stored in notifications table when a teacher creates/modifies an exam announcement. (99645 lines)
	Version 10.22.1: Feb 09, 2011	A short view of notices, messages and forum replies is included in notifications. (99644 lines)
	Version 10.22:   Feb 08, 2011	Sending of email in a notification is cancelled when user visit the event which caused the notification. (99518 lines)
	Version 10.21.3: Feb 07, 2011	Changes in list my notifications. (99461 lines)
	Version 10.21.2: Feb 06, 2011	Changes in list my notifications. (99432 lines)
	Version 10.21.1: Feb 06, 2011	Changes in list my notifications. (99395 lines)
	Version 10.21:   Feb 05, 2011	New option to list my notifications. (99280 lines)
	Version 10.20.1: Feb 04, 2011	Starting new SOAP web service function to get notifications of a user.
					gsoap updated to version 2.8.1 (2011/01/14) (99111 lines)
	Version 10.20:   Jan 24, 2011	Administrators options are moved to users tab.
					Degree tab is renamed as system tab.
					Users tab is splitted into two tabs: users and enrolment. (99030 lines)
	Version 10.19.1: Jan 23, 2011	Changes in option to view the connected users. (98898 lines)
	Version 10.19:   Jan 23, 2011	New option to view the connected users. (98882 lines)
	Version 10.18:   Jan 22, 2011	New option to change the rol of the logged user. (98788 lines)
	Version 10.17:   Jan 20, 2011	Checked and fixed order of messages translated. (98724 lines)
	Version 10.16.12:Jan 20, 2011	Several messages translated, only to english and spanish (0 messages to be translated). (98748 lines)
	Version 10.16.11:Jan 19, 2011	Several messages translated, only to english and spanish (still 8 messages to be translated). (98629 lines)
	Version 10.16.10:Jan 18, 2011	Several messages translated, only to english and spanish (still 11 messages to be translated). (98563 lines)
	Version 10.16.9: Jan 18, 2011	Several messages translated, only to english and spanish (still 21 messages to be translated). (98490 lines)
	Version 10.16.8: Jan 17, 2011	Several messages translated, only to english and spanish (still 31 messages to be translated). (98353 lines)
	Version 10.16.7: Jan 04, 2011	Added subtitle for each action. (98290 lines)
	Version 10.16.6: Jan 04, 2011	Message "0 new messages" is no longer shown when user has not new messages. (98242 lines)
	Version 10.16.5: Jan 04, 2011	Changes in layout for mobile devices. (98245 lines)
	Version 10.16.4: Jan 03, 2011	Changes in layout for mobile devices. (98245 lines)
	Version 10.16.3: Jan 03, 2011	Changes in layout for mobile devices. (98244 lines)
	Version 10.16.2: Dec 31, 2010	Changes in layout for mobile devices. (98241 lines)
	Version 10.16.1: Dec 30, 2010	Changes in layout for mobile devices. (98235 lines)
	Version 10.16:   Dec 29, 2010	Changes in layout for mobile devices. (98231 lines)
	Version 10.15.5: Dec 28, 2010	Changes in layout for mobile devices. (98120 lines)
	Version 10.15.4: Dec 27, 2010	Several messages translated, only to english and spanish (still 37 messages to be translated). (98354 lines)
	Version 10.15.3: Dec 26, 2010	Several messages translated, only to english and spanish (still 46 messages to be translated). (98265 lines)
	Version 10.15.2: Dec 21, 2010	Changes in layout for mobile devices. (98197 lines)
	Version 10.15.1: Dec 19, 2010	Changes in layout for mobile devices.
	Version 10.15:   Dec 19, 2010	Changes in layout for mobile devices. (97736 lines)
	Version 10.14.1: Dec 14, 2010	Size is not shown for folders.
	Version 10.14:   Dec 14, 2010	Support for web services accessed directly via mobile devices. (97595 lines)
	Version 10.13.15:Dec 12, 2010	Private directory of photos moves from /var/lib/mysql/photo to /var/www/swad/photo. (97391 lines)

					Above this, blank lines are not numbered
					Below this, blank lines are numbered

	Version 10.13.14:Dec 09, 2010	Several messages translated, only to english and spanish (still 49 messages to be translated). (109059 lines)
	Version 10.13.13:Dec 08, 2010	Several messages translated, only to english and spanish (still 55 messages to be translated). (108991 lines)
	Version 10.13.12:Dec 08, 2010	Several messages translated, only to english and spanish (still 68 messages to be translated). (108845 lines)
	Version 10.13.11:Dec 08, 2010	Several messages translated, some of them only to english and spanish (still 73 messages to be translated). (108777 lines)
	Version 10.13.10:Dec 08, 2010	Several messages translated, only to english and spanish (still 83 messages to be translated). (108727 lines)
	Version 10.13.9: Dec 07, 2010	Several messages translated, only to english and spanish (still 91 messages to be translated). (108601 lines)
	Version 10.13.8: Dec 07, 2010	Several messages translated, only to english and spanish (still 99 messages to be translated). (108485 lines)
	Version 10.13.7: Dec 07, 2010	Several messages translated, only to english and spanish (still 105 messages to be translated). (108425 lines)
	Version 10.13.6: Dec 06, 2010	Several messages translated, only to english and spanish (still 118 messages to be translated). (108280 lines)
	Version 10.13.5: Dec 06, 2010	Several messages translated, some of them only to english and spanish (still 127 messages to be translated). (108140 lines)
	Version 10.13.4: Dec 06, 2010	Several messages translated, only to english and spanish (still 141 messages to be translated). (107992 lines)
	Version 10.13.3: Dec 04, 2010	Fixed small bug in surveys. (107874 lines)
	Version 10.13.2: Dec 01, 2010	Several messages translated, some of them only to english and spanish (still 153 messages to be translated). (107873 lines)
	Version 10.13.1: Dec 01, 2010	Several messages translated, only to english and spanish (still 172 messages to be translated). (107730 lines)
	Version 10.13:   Nov 30, 2010	New option to reset surveys (set answers to zero). (107679 lines)
	Version 10.12.2: Nov 30, 2010	Graphic changes in surveys. (107529 lines)
	Version 10.12.1: Nov 30, 2010	Increase disk quota for briefcases.
					Changes in forums.
					Changed small icon for update/recycle.
					Changed icon for surveys. (107532 lines)
	Version 10.12:   Nov 29, 2010	Surveys are published for all users. (107522 lines)
	Version 10.11.9: Nov 29, 2010	Changes in surveys. (107521 lines)
	Version 10.11.8: Nov 29, 2010	Changes in surveys. (107447 lines)
	Version 10.11.7: Nov 28, 2010	Changes in surveys related to location range. (107390 lines)
	Version 10.11.6: Nov 28, 2010	Changes in surveys related to location range. (107371 lines)
	Version 10.11.5: Nov 26, 2010	Changes in surveys.
					Changes in location ranges (platform, degree, course). (107314 lines)
	Version 10.11.4: Nov 25, 2010	Optimizations in surveys. (107246 lines)
	Version 10.11.3: Nov 24, 2010	Icons based on CD/DVD are changed to icons based on folders. (107246 lines)
	Version 10.11.2: Nov 24, 2010	Hidden files/folders are shown in light style. (107245 lines)
	Version 10.11.1: Nov 24, 2010	Hidden/visible icons are replaced by eyes. (107225 lines)
	Version 10.11:   Nov 23, 2010	Optimization in database queries when counting number of rows. (107222 lines)
	Version 10.10.5: Nov 23, 2010	Changes in surveys related to answers in database. (108448 lines)
	Version 10.10.4: Nov 22, 2010	"Download" is renamed as "Documents". (108477 lines)
	Version 10.10.3: Nov 22, 2010	Graphic changes in white theme, now selected as default. (108476 lines)
	Version 10.10.2: Nov 21, 2010	Changes in surveys. (108475 lines)
	Version 10.10.1: Nov 21, 2010	Changes in surveys. (108432 lines)
	Version 10.10:   Nov 21, 2010	Changes in layout. (108338 lines)
	Version 10.9.5:  Nov 21, 2010	Showed status of each survey. (108339 lines)
	Version 10.9.4:  Nov 21, 2010	Graphic changes in assignments. (108189 lines)
	Version 10.9.3:  Nov 21, 2010	Form to select types of users who can answer a survey. (108174 lines)
	Version 10.9.2:  Nov 20, 2010	Several messages translated, some of them only to english and spanish (still 178 messages to be translated). (108124 lines)
	Version 10.9.1:  Nov 19, 2010	Several messages translated, some of them only to english and spanish (still 192 messages to be translated). (107981 lines)
	Version 10.9:    Nov 18, 2010	Internal changes in parameters with multiple values.
					The number of recipients in messages written by students is now limited. (107871 lines)
	Version 10.8.3:  Nov 15, 2010	Several messages translated, some of them only to english and spanish (still 196 messages to be translated). (107795 lines)
	Version 10.8.2:  Nov 15, 2010	Changes in surveys. (107711 lines)
	Version 10.8.1:  Nov 15, 2010	Changes in surveys related with deletions in database. (107677 lines)
	Version 10.8:    Nov 14, 2010	Added new white theme. (107583 lines)
	Version 10.7.14: Nov 08, 2010	Several messages translated, only to english and spanish (still 206 messages to be translated). (107550 lines)
	Version 10.7.13: Nov 07, 2010	Changes in layout of tables with frame. (107536 lines)
	Version 10.7.12: Nov 07, 2010	Changes in surveys. (107536 lines)
	Version 10.7.11: Nov 07, 2010	Changes in surveys. (107440 lines)
	Version 10.7.10: Nov 07, 2010	Changes in surveys. (107355 lines)
	Version 10.7.9:  Nov 06, 2010	User's answers to a survey are stored into database. (107214 lines)
	Version 10.7.8:  Nov 06, 2010	Change in list of threads inside a forum. (107028 lines)
	Version 10.7.7:  Nov 04, 2010	Several messages translated, only to english and spanish (still 216 messages to be translated).
					Added file extension .dwd (Davka writer, an hebrew word processor). (107012 lines)
	Version 10.7.6:  Nov 03, 2010	Degree administrators can register/remove several teachers in/from a course. (106850 lines)
	Version 10.7.5:  Nov 03, 2010	Changes in edition of surveys.
					Several internal optimizations. (106835 lines)
	Version 10.7.4:  Nov 02, 2010	Changes in edition of surveys. (106841 lines)
	Version 10.7.3:  Oct 31, 2010	Changes in edition of surveys. (106685 lines)
	Version 10.7.2:  Oct 31, 2010	Changes in edition of surveys. (106679 lines)
	Version 10.7.1:  Oct 30, 2010	Changes in edition of surveys. (106742 lines)
	Version 10.7:    Oct 30, 2010	Internal change: new functions to send hidden parameters. (106709 lines)
	Version 10.6.4:  Oct 30, 2010	Changes in edition of surveys.
					Several messages translated, only to english and spanish (still 224 messages to be translated) (106817 lines)
	Version 10.6.3:  Oct 30, 2010	Hidden/visible icons are replaced by closed/open icons. (107109 lines)
	Version 10.6.2:  Oct 29, 2010	Changes in edition of surveys. (107032 lines)
	Version 10.6.1:  Oct 28, 2010	Changes in edition of surveys. (106866 lines)
	Version 10.6:    Oct 24, 2010	Changes in edition of surveys. (105796 lines)
	Version 10.5.2:  Oct 23, 2010	Several messages translated, only to english and spanish (still 222 messages to be translated). (105080 lines)
	Version 10.5.1:  Oct 21, 2010	Changes in style sheet and images for grey theme. (104925 lines)
	Version 10.5:    Oct 20, 2010	Added new grey theme. (104924 lines)
	Version 10.4.1:  Oct 17, 2010	Fixed bug in table of actions. (104868 lines)
	Version 10.4:    Oct 17, 2010	New option to create surveys. (104867 lines)
	Version 10.3.4:  Oct 16, 2010	Actions like "show print view" are shown in a new blank window, not in a popup. (103722 lines)
	Version 10.3.3:  Oct 16, 2010	Changes in search/select option. (103738 lines)
	Version 10.3.2:  Oct 14, 2010	Hidden assignments are shown in lighter colours. (103729 lines)
	Version 10.3.1:  Oct 14, 2010	Fixed bug reported by F. Javier Fern�ndez Baldomero: no folders must be created for hidden assignments. (103716 lines)
	Version 10.3:    Oct 13, 2010	Assignments can be hidden/shown by teachers. (103714 lines)
	Version 10.2.4:  Oct 13, 2010	Basic information about exam announcements is placed in RSS feeds. (103559 lines)
	Version 10.2.3:  Oct 13, 2010	New module swad_RSS for managing RSS feeds. (103446 lines)
	Version 10.2.2:  Oct 13, 2010	Fixed bug when creating RSS file. (103351 lines)
	Version 10.2.1:  Oct 13, 2010	Links to RSS files are shown. (103341 lines)
	Version 10.2:    Oct 12, 2010	Notices are published via RSS 2.0. One RSS file is created for each course. (103277 lines)
	Version 10.1:    Oct 06, 2010	Green icon is replace by a button when editing new items. (103045 lines)
	Version 10.0:    Sep 26, 2010	Change to year 2010-2011.
					New items are moved to top of page when editing degrees, courses, etc. (102898 lines)
	Version  9.51.1: Jul 21, 2010	Small changes related to connected users. (102921 lines)
	Version  9.51:   Jul 21, 2010	Connected users are "refreshed" after a few seconds using Javascript on client side. (102923 lines)
	Version  9.50.1: Jul 20, 2010	Fixed bug in drawing of a month. (102756 lines)
	Version  9.50:   Jul 20, 2010	The parameters NxtTab and NxtAct are passed as numbers instead of strings. (102754 lines)
	Version  9.49.2: Jul 20, 2010	Small change in table log_comments. (102755 lines)
	Version  9.49.1: Jul 20, 2010	Fixed bug in statistics per course. (102754 lines)
	Version  9.49:   Jun 20, 2010	IP address size is changed from 40 to 15 in order to reduce space required in database tables.
					Log comments are stored in a separate table. (102753 lines)
	Version  9.48.3: Jun 19, 2010	Several messages translated (still 223 messages to be translated). (102689 lines)
	Version  9.48.2: Jul 17, 2010	Added column Obsolete to actions table. (102517 lines)
	Version  9.48.1: Jul 16, 2010	The text of the actions is no longer stored in log tables. (102517 lines)
	Version  9.48:   Jul 16, 2010	The text of the actions is moved to a database table. (102517 lines)
	Version  9.47:   Jul 15, 2010	The text of the actions is moved to a new internal table. (102458 lines)
	Version  9.46.27:Jun 22, 2010	Several messages translated, only to english and spanish (still 234 messages to be translated). (99532 lines)
	Version  9.46.26:Jun 21, 2010	Fixed bug in exam annoluncements, reported by Eva Mart�nez Ortigosa. (99461 lines)
	Version  9.46.25:Jun 20, 2010	Several messages translated, only to english and spanish (still 239 messages to be translated). (99461 lines)
	Version  9.46.24:Jun 19, 2010	Several messages translated, only to english and spanish (still 249 messages to be translated).
					Don't show error message when sending pending notification mails. (99320 lines)
	Version  9.46.23:Jun 19, 2010	Several messages translated, only to english and spanish (still 255 messages to be translated). (99218 lines)
	Version  9.46.22:Jun 19, 2010	Several messages translated, only to english and spanish (still 269 messages to be translated). (99049 lines)
	Version  9.46.21:Jun 15, 2010	Several messages translated, only to english and spanish (still 280 messages to be translated).
					New icon to select/search course in head. (98944 lines)
	Version  9.46.20:Jun 12, 2010	Several messages translated, only to english and spanish (still 285 messages to be translated). (98884 lines)
	Version  9.46.19:Jun 12, 2010	Several messages translated, only to english and spanish (still 299 messages to be translated). (98704 lines)
	Version  9.46.18:Jun 11, 2010	Several messages translated, only to english and spanish (still 306 messages to be translated). (98635 lines)
	Version  9.46.17:Jun 11, 2010	Several messages translated, only to english and spanish (still 312 messages to be translated).
					Filenames not valid are converted to valid when uploaded. (98581 lines)
	Version  9.46.16:Jun 10, 2010	Several messages translated, only to english and spanish (still 320 messages to be translated). (98488 lines)
	Version  9.46.15:Jun 09, 2010	Several messages translated, only to english and spanish (still 326 messages to be translated). (98432 lines)
	Version  9.46.14:Jun 09, 2010	Several messages translated, only to english and spanish (still 332 messages to be translated). (98396 lines)
	Version  9.46.13:Jun 09, 2010	Several messages translated, only to english and spanish (still 337 messages to be translated). (98337 lines)
	Version  9.46.12:Jun 07, 2010	Several messages translated, only to english and spanish (still 346 messages to be translated). (98207 lines)
	Version  9.46.11:Jun 07, 2010	Several messages translated, only to english and spanish (still 349 messages to be translated). (98125 lines)
	Version  9.46.10:May 28, 2010	Several messages translated, only to english and spanish (still 353 messages to be translated). (98072 lines)
	Version  9.46.9: May 27, 2010	Several messages translated, only to english and spanish (still 363 messages to be translated). (97973 lines)
	Version  9.46.8: May 25, 2010	Several messages translated, only to english and spanish (still 369 messages to be translated). (97948 lines)
	Version  9.46.7: May 25, 2010	Several messages translated, only to english and spanish (still 377 messages to be translated). (97878 lines)
	Version  9.46.6: May 25, 2010	Several messages translated, only to english and spanish (still 386 messages to be translated). (97788 lines)
	Version  9.46.5: May 20, 2010	Icons and users' photos are moved to other directories in disk.
					Several messages translated, only to english and spanish (still 405 messages to be translated). (97152 lines)
	Version  9.46.4: May 04, 2010	Fixed bug in MFU actions, reported by Rafael �balos Marco. (97084 lines)
	Version  9.46.3: May 03, 2010	Several messages translated, only to english and spanish (still 411 messages to be translated). (97076 lines)
	Version  9.46.2: May 03, 2010	Several messages translated, only to english and spanish (still 427 messages to be translated). (96350 lines)
	Version  9.46.1: May 03, 2010	Several messages translated, only to english and spanish (still 449 messages to be translated). (96129 lines)
	Version  9.46:   Apr 27, 2010	When a student confirms that he has read a given course info, this fact is stored in database. (95955 lines)
	Version  9.45.6: Apr 27, 2010	Students are informed about course info that must read. (95737 lines)
	Version  9.45.5: Apr 27, 2010	Teacher's preference about whether info must be read by students is stored in database. (95598 lines)
	Version  9.45.4: Apr 22, 2010	New checkbox for teachers to force student to read some course info. (95452 lines)
	Version  9.45.3: Apr 21, 2010	Fixed bug when changing to a new course after creation. (95355 lines)
	Version  9.45.2: Apr 12, 2010	The list of countries can be ordered by number of users. (95353 lines)
	Version  9.45.1: Apr 08, 2010	The list of countries can be retrieved without the number of institutions. (95341 lines)
	Version  9.45:   Mar 24, 2010	Added country field to common record.
					The number of users from each country is listed in listing of countries. (95318 lines)
	Version  9.44.6: Mar 23, 2010	Country is shown for each entry in list of institutions. (95177 lines)
	Version  9.44.5: Mar 23, 2010	Changes in edition of countries. (95124 lines)
	Version  9.44.4: Mar 23, 2010	Changes in edition of institutions: new field country for each institution. (95129 lines)
	Version  9.44.3: Mar 23, 2010	Changes in edition of countries. (95007 lines)
	Version  9.44.2: Mar 22, 2010	Changes in edition of countries. (94909 lines)
	Version  9.44.1: Mar 22, 2010	Changes in edition of countries.
					Assignments are ordered also by title. (94841 lines)
	Version  9.44:   Mar 22, 2010	New option to see and edit countries. (94819 lines)
	Version  9.43.10:Mar 21, 2010	Several messages translated, only to english and spanish (still 457 messages to be translated).
					The word "hebra" is renamed as "discusi�n" in Spanish. (93666 lines)
	Version  9.43.9: Mar 20, 2010	When a course is created or updated, a link is shown to go to that course.
					Added two new allowed file extensions. (93627 lines)
	Version  9.43.8: Mar 20, 2010	Several messages translated, only to english and spanish (still 457 messages to be translated). (93581 lines)
	Version  9.43.7: Mar 20, 2010	Several messages translated, only to english and spanish (still 472 messages to be translated). (93430 lines)
	Version  9.43.6: Mar 18, 2010	Fixed MySQL wrong query. (93224 lines)
	Version  9.43.5: Mar 17, 2010	Fixed small bug, reported by Germ�n Luz�n Gonz�lez.
					Several messages translated, only to english and spanish (still 498 messages to be translated). (93223 lines)
	Version  9.43.4: Mar 07, 2010	New link to enter from external site. (93126 lines)
	Version  9.43.3: Mar 07, 2010	Non existing users coming from external site are created as new swad users. (93104 lines)
	Version  9.43.2: Mar 05, 2010	New option to call the roll, still empty. (93086 lines)
	Version  9.43.1: Feb 27, 2010	Changed some criteria for indicators. (93054 lines)
	Version  9.43:   Feb 27, 2010	When a teacher changes to one of his/her courses, a warning is shown to indicate how many indicators are valid. (93055 lines)
	Version  9.42.8: Feb 27, 2010	A count of how many courses have n indicators valid is shown in statistics of courses.
					New function to compute the indicators of a course. (93032 lines)
	Version  9.42.7: Feb 25, 2010	Fixed bug in search of user's ID in first column of an HTML table of marks. (92975 lines)
	Version  9.42.6: Feb 25, 2010	Changes in search of user's ID in first column of an HTML table of marks. (92978 lines)
	Version  9.42.5: Feb 24, 2010	Changes in Deutsch translation, by Rafael Barranco-Droege.
					Fixed bug in homework zip file. (92947 lines)
	Version  9.42.4: Feb 19, 2010	Changes in the function used to convert strings that allow to write in other alphabets, such as Cyrillic. (92930 lines)
	Version  9.42.3: Feb 17, 2010	Changes in file of translations. (92911 lines)
	Version  9.42.2: Feb 17, 2010	Mail domains can be ordered by three criteria: domain, info or number of users. (90496 lines)
	Version  9.42.1: Feb 16, 2010	Several actions no longer use lockfile.
					The number of users with each of the mail domains is listed. (90365 lines)
	Version  9.42:   Feb 15, 2010	New action to edit mail domains allowed for notifications. (90291 lines)
	Version  9.41.5: Feb 13, 2010	New table notices_deleted.
					Deleted notices are moved to notices_deleted. (89403 lines)
	Version  9.41.4: Feb 12, 2010	Changes in detection of user's ID in first column of the HTML table inside a file of marks.
					A superuser can eliminate totally from the platform a list of users who belong to a course. (89316 lines)
	Version  9.41.3: Feb 09, 2010	Changes in style sheet to display properly in Ubuntu.
					Notifications are sent only to selected mail domains. (89233 lines)
	Version  9.41.2: Feb 09, 2010	Changes in calculation of most frequently actions. (89182 lines)
	Version  9.41.1: Feb 08, 2010	Changes in calculation of most frequently actions. (89173 lines)
	Version  9.41:   Feb 07, 2010	Most frequently actions are shown in a quick menu. (89150 lines)
	Version  9.40.1: Feb 04, 2010	Added new allowed file extension and MIME type. (88948 lines)
	Version  9.40:   Feb 04, 2010	Changed column LastPageMsgRec to LastPageMsgRcv in table os sessions.
					New table msg_content_deleted.
					Contents of fully deleted messages are moved to msg_content_deleted. (88945 lines)
	Version  9.39.5: Feb 04, 2010	Fixed bug in pagination of messages. (88909 lines)
	Version  9.39.4: Feb 04, 2010	Recipients of messages are listed alphabetically. (88907 lines)
	Version  9.39.3: Feb 04, 2010	New table msg_rcv_deleted.
					Deleted received messages are moved to msg_rcv_deleted. (88877 lines)
	Version  9.39.2: Feb 03, 2010	New table msg_snt_deleted.
					Deleted sent messages are moved to msg_snt_deleted. (88826 lines)
	Version  9.39.1: Feb 03, 2010	Optimization for speed in querys of messages filtered by content. (88796 lines)
	Version  9.39:   Feb 02, 2010	New table msg_content with subject and content of sent messages. (88944 lines)
	Version  9.38.2: Feb 02, 2010	Table messages is renamed to msg_snt.
					Table msg_received is renamed to msg_rcv. (88714 lines)
	Version  9.38.1: Feb 02, 2010	Changes in file of translations. (88684 lines)
	Version  9.38:   Feb 01, 2010	New option with introductory information about the course.
					Changes in edition of course info. (87958 lines)
	Version  9.37.2: Feb 01, 2010	Changes in file of translations. (87805 lines)
	Version  9.37.1: Feb 01, 2010	New row for total stats of forums. (87245 lines)
	Version  9.37:   Feb 01, 2010	All modules in swad have been renamed at the suggestion of J.J.Merelo. (87110 lines)
	Version  9.36.8: Jan 31, 2010	Small change in reception of ZIP file for course info. (87109 lines)
	Version  9.36.7: Jan 31, 2010	Several messages translated (still 492 messages to be translated). (87106 lines)
	Version  9.36.6: Jan 30, 2010	Changes in layout of header.
					Deprecated "align" attributes in tags "img" are changed to CSS styles. (87096 lines)
	Version  9.36.5: Jan 30, 2010	Changes in layout of header. (87043 lines)
	Version  9.36.4: Jan 27, 2010	Fixed security hole in call to command, detected by J.J.Merelo. (87092 lines)
	Version  9.36.3: Jan 26, 2010	A new parameter passed by the GET method allows entering SWAD with a selected layout. (87031 lines)
	Version  9.36.2: Jan 26, 2010	Changes in log in / log out. (87016 lines)
	Version  9.36.1: Jan 26, 2010	Changes in layout of top of page. (86978 lines)
	Version  9.36:   Jan 25, 2010	A lot of changes in log in / log out. (86994 lines)
	Version  9.35.7: Jan 24, 2010	My courses are now ordered by degree short name / course short name.
					Changes in selector of layout and theme. (86905 lines)
	Version  9.35.6: Jan 24, 2010	Changes in selection and search of courses. (86844 lines)
	Version  9.35.5: Jan 23, 2010	Changes in Stats > Courses stats. (86739 lines)
	Version  9.35.4: Jan 22, 2010	Fixed bug in deletion of a course. (86697 lines)
	Version  9.35.3: Jan 22, 2010	Changes in date-time shown above the current month.
					New criteria to assess courses in Stats > Courses stats. (86690 lines)
	Version  9.35.2: Jan 21, 2010	Changes in selection and search of courses.
					Changes in date-time shown above the current month. (86649 lines)
	Version  9.35.1: Jan 21, 2010	Small changes in selector of courses. (86530 lines)
	Version  9.35:   Jan 20, 2010	Changes in layout.
					Selection of courses is moved from left menu to search option. (86508 lines)
	Version  9.34.2: Jan 18, 2010	Layout is stored in table of IP preferences, so it is remembered even when no user logged. (86452 lines)
	Version  9.34.1: Jan 18, 2010	Stats about number of users with each layout are shown. (86437 lines)
	Version  9.34:   Jan 18, 2010	Layout is selected in preferences and stored in database. (86364 lines)
	Version  9.33.2: Jan 18, 2010	Changes in layout of top of page. (86208 lines)
	Version  9.33.1: Jan 17, 2010	Changes in layout of top of page. (86184 lines)
	Version  9.33:   Jan 17, 2010	Changes in layout of top of page. (86185 lines)
	Version  9.32.4: Jan 17, 2010	Changes in layout of connected users.
					Fixed bug in connected users. (86190 lines)
	Version  9.32.3: Jan 16, 2010	Changes in interface.
					Simplified the search for courses from the home page. (86141 lines)
	Version  9.32.2: Jan 15, 2010	Changes in simple interface.
					Fixed bug when a student want to change his/her groups. (86006 lines)
	Version  9.32.1: Jan 15, 2010	Changes in simple interface. (85870 lines)
	Version  9.32:   Jan 15, 2010	New interface more simple for small and mobile devices. (85853 lines)
	Version  9.31.2: Jan 13, 2010	Moved icons for front page of swad from icon folder to another folder.
					Moved images for yellow notes to a special folder inside icon directory. (85441 lines)
	Version  9.31.1: Jan 13, 2010	New field in table of actions to describe if an action send normal plain forms or data (files). (85432 lines)
	Version  9.31:   Jan 12, 2010	New checks to validate indicators in statistics of courses.
					When getting info source of a course, new checks are made to detect empty info. (85442 lines)
	Version  9.30.2: Jan 11, 2010	Changes in the names of several actions.
					Icons directory has been cleaned, deleting obsolete icons.
					Several messages have been marked as needing translation (still 485 messages to be translated). (85263 lines)
	Version  9.30.1: Jan 09, 2010	The external module to import courses and students is moved completely apart from swad.
					The number of lines in the source code of SWAD does not take into account that external module. (85219 lines)
	Version  9.30:   Jan 08, 2010	Some constants of the external module to import courses and students are moved to swad_config.h.
					First version published as free software in the SWAD web. (85721 lines)
	Version  9.29.13:Jan 08, 2010	Minor changes in soap server. (85318 lines)
	Version  9.29.12:Jan 08, 2010	Several messages translated, only to english and spanish (still 475 messages to be translated). (85317 lines)
	Version  9.29.11:Jan 08, 2010	Several messages translated, only to english and spanish (still 498 messages to be translated). (85090 lines)
	Version  9.29.10:Jan 07, 2010	Database constants are defined in swad_config.h.
					Several messages translated (still 536 messages to be translated). (84930 lines)
	Version  9.29.9: Jan 07, 2010	The list of future improvements is moved apart from the source code. (84784 lines)
	Version  9.29.8: Jan 07, 2010	Several messages translated (still 548 messages to be translated). (84957 lines)
	Version  9.29.7: Jan 07, 2010	Several messages translated (still 565 messages to be translated). (84764 lines)
	Version  9.29.6: Jan 07, 2010	Several messages translated, only to english and spanish (still 578 messages to be translated). (84633 lines)
	Version  9.29.5: Jan 07, 2010	Several messages translated, only to english and spanish (still 588 messages to be translated). (84600 lines)
	Version  9.29.4: Jan 07, 2010	Several messages translated, only to english and spanish (still 595 messages to be translated). (84533 lines)
	Version  9.29.3: Jan 06, 2010	Commands called from SWAD are defined in swad_config.h.
					Expiration times and other periods are defined in swad_config.h (84358 lines)
	Version  9.29.2: Jan 04, 2010	Several messages translated, only to english and spanish (still 607 messages to be translated). (84322 lines)
	Version  9.29.1: Jan 01, 2010	Several messages translated, only to english and spanish (still 611 messages to be translated).
					Changes on table marks in database. (84286 lines)
	Version  9.29:   Jan 01, 2010	Several messages translated, only to english and spanish (still 613 messages to be translated).
					window.status messages deleted because the don't work in recent web browsers. (84233 lines)
	Versi�n  9.28.2: 31/dic/2009	A�adida felicitaci�n de a�o nuevo. (84197 l�neas)
	Versi�n  9.28.1: 31/dic/2009	Durante la creaci�n de las tablas de la base de datos se muestran comentarios por pantalla.
					Comentarios en el archivo de configuraci�n swad_config.h.
					A�adidos comentarios iniciales en cada uno de los archivos fuente. (84175 l�neas)
	Versi�n  9.28:   31/dic/2009	Marcados m�s mensajes para futura traducci�n (617 en total).
					Numerosos cambios internos en mensajes. (83900 l�neas)
	Versi�n  9.27.2: 31/dic/2009	Se eliminan las sesiones importadas antiguas. (83954 l�neas)
	Versi�n  9.27.1: 31/dic/2009	Renombradas las dos tablas de la base de datos relacionadas con las fichas de alumnos en la asignatura. (83936 l�neas)
	Versi�n  9.27:   31/dic/2009	Los campos de la tabla log se renombran para ser exactos a los de la tabla log_recent. (83935 l�neas)
	Versi�n  9.26.8: 30/dic/2009	Algunos cambios internos. (83973 l�neas)
	Versi�n  9.26.7: 30/dic/2009	Cambiados los nombres de los directorios y de los ficheros que guardan los temarios.
					Eliminados archivos antiguos de configuraci�n y descripci�n de asignaturas que ya no se usan. (83962 l�neas)
	Versi�n  9.26.6: 30/dic/2009	Marcados m�s mensajes para futura traducci�n (534 en total). (83956 l�neas)
	Versi�n  9.26.5: 30/dic/2009	Traducci�n de varios mensajes. (83961 l�neas)
	Versi�n  9.26.4: 30/dic/2009	Marcados otros 125 mensajes para futura traducci�n (475 en total). (83924 l�neas)
	Versi�n  9.26.3: 30/dic/2009	Traducci�n de varios mensajes.
					Marcados unos 350 mensajes para futura traducci�n. (83884 l�neas)
	Versi�n  9.26.2: 30/dic/2009	Corregido error en estilo. (83880 l�neas)
	Versi�n  9.26.1: 29/dic/2009	Traducci�n de varios mensajes. (83879 l�neas)
	Versi�n  9.26:   29/dic/2009	Traducci�n de varios mensajes.
					Revisi�n exhaustiva de todos los includes.
					Cambio de algunos m�dulos por otros.
					Numerosos cambios internos. (83844 l�neas)
	Versi�n  9.25.8: 29/dic/2009	Traducci�n de varios mensajes de error. (83701 l�neas)
	Versi�n  9.25.7: 28/dic/2009	Traducci�n de varios mensajes de error. (83766 l�neas)
	Versi�n  9.25.6: 28/dic/2009	Traducci�n de varios mensajes de error. (83734 l�neas)
	Versi�n  9.25.5: 28/dic/2009	Optimizaci�n en todas las consultas INSERT que devuelven el c�digo del �ltimo item insertado. (83715 l�neas)
	Versi�n  9.25.4: 28/dic/2009	Optimizaci�n y traducci�n al ingl�s de mensajes de error en todas las consultas REPLACE a la base de datos. (83712 l�neas)
	Versi�n  9.25.3: 28/dic/2009	Optimizaci�n y traducci�n al ingl�s de mensajes de error en todas las consultas INSERT a la base de datos.
					Eliminado archivo fuente no usado. (83709 l�neas)
	Versi�n  9.25.2: 28/dic/2009	Optimizaci�n y traducci�n al ingl�s de mensajes de error en todas las consultas UPDATE a la base de datos. (84522 l�neas)
	Versi�n  9.25.1: 28/dic/2009	Optimizaci�n y traducci�n al ingl�s de mensajes de error en todas las consultas DELETE a la base de datos. (84723 l�neas)
	Versi�n  9.25:   28/dic/2009	Traducidos a ingl�s muchos mensajes de error relacionados con la base de datos. (84849 l�neas)
	Versi�n  9.24.2: 27/dic/2009	Optimizaci�n en el n�mero de filas devueltas por las consultas SELECT en la base de datos. (84390 l�neas)
	Versi�n  9.24.1: 27/dic/2009	Optimizaci�n en todas las consultas SELECT en la base de datos. (85147 l�neas)
	Versi�n  9.24:   26/dic/2009	Optimizaci�n en muchas consultas SELECT en la base de datos. (85884 l�neas)
	Versi�n  9.23.2: 26/dic/2009	Traducidos a ingl�s muchos mensajes de error relacionados con la base de datos. (86494 l�neas)
	Versi�n  9.23.1: 26/dic/2009	Peque�os cambios internos relacionados con la base de datos. (86492 l�neas)
	Versi�n  9.23:   26/dic/2009	Creaci�n autom�tica de todas las tablas de la base de datos si no existen. (86490 l�neas)
	Versi�n  9.22.3: 25/dic/2009	Nueva opci�n para instalar SWAD (a�n vac�a). (86047 l�neas)
	Versi�n  9.22.2: 24/dic/2009	Cambios en algunos directorios de iconos. (85945 l�neas)
	Versi�n  9.22.1: 24/dic/2009	El nombre del servicio de autenticaci�n externo (PRADO) se convierte en una constante f�cilmente alterable. (85945 l�neas)
	Versi�n  9.22:   24/dic/2009	El cliente de PRADO se convierte en un programa externo llamado desde SWAD.
					SWAD ya no incluye un cliente (PRADO) y un servidor (servicio web de SWAD) SOAP, sino s�lo el servidor. (85919 l�neas)
	Versi�n  9.21.4: 23/dic/2009	Cambios internos en el m�dulo relacionado con PRADO. (85173 l�neas)
	Versi�n  9.21.3: 23/dic/2009	Cambios internos en el m�dulo relacionado con PRADO. (85002 l�neas)
	Versi�n  9.21.2: 23/dic/2009	Muchos cambios internos en el m�dulo relacionado con PRADO.
					Nuevo m�dulo swad_import.c para alumnos y grupos importados. (84968 l�neas)
	Versi�n  9.21.1: 22/dic/2009	Algunos cambios internos en el m�dulo relacionado con PRADO. (84876 l�neas)
	Versi�n  9.21:   21/dic/2009	Numerosos cambios en swad_web_service.c.
					Cambios en las reglas para aceptar un ID y un apodo (84958 l�neas)
	Versi�n  9.20:   20/dic/2009	Cambios de nombres de algunas tablas y muchos campos de tablas de la base de datos. (85201 l�neas)
	Versi�n  9.19.9: 18/dic/2009	Mejorados comentarios en servicios web y PRADO.
					Corregido error en edici�n de actividades. (85134 l�neas)
	Versi�n  9.19.8: 17/dic/2009	En las estad�sticas de asignaturas se muestra el n�mero de profesores y el n�mero de alumnos de cada asignatura. (85181 l�neas)
	Versi�n  9.19.7: 17/dic/2009	El registro de cambios se sit�a en un archivo aparte swad_changelog.c. (85142 l�neas)
	Versi�n  9.19.6: 16/dic/2009	Se contemplan m�s apartados a evaluar en los indicadores de las estad�sticas de asignaturas. (85097 l�neas)
	Versi�n  9.19.5: 16/dic/2009	Se contemplan m�s apartados a evaluar en los indicadores de las estad�sticas de asignaturas. (85091 l�neas)
	Versi�n  9.19.4: 16/dic/2009	Corregido bug reciente en env�o de trabajos detectado por Ferm�n S�nchez de Medina L�pez-Huertas y Juan Santana Lario. (85000 l�neas)
	Versi�n  9.19.3: 16/dic/2009	En las estad�sticas de asignaturas se muestra si cada asignatura tiene tutor�as virtuales o no. (84999 l�neas)
	Versi�n  9.19.2: 15/dic/2009	En las estad�sticas de asignaturas se usan colores verde y rojo, y se muestra si cada asignatura tiene actividades o no. (84886 l�neas)
	Versi�n  9.19.1: 15/dic/2009	Una vez se ha definido una actividad con env�o de archivos activado, �ste se permite desactivar siempre que a�n no haya ninguna carpeta para esa actividad. (84832 l�neas)
	Versi�n  9.19:   14/dic/2009	Cuando se elimina una actividad, se eliminan las carpetas asociadas.
					Una vez se ha definido una actividad con env�o de archivos activado, no se puede desactivar (para desactivar el env�o s�lo queda la posibilidad de eliminar la actividad).
					Los nombres de las carpetas de actividades se almacenan en la base de datos con sensibilidad a las may�sculas, pudiendo haber dos carpetas con el mismo nombre y diferentes may�sculas/min�sculas.
					No se permite cambiar el nombre a una carpeta de actividad si ya existe alguna carpeta con ese nombre para alg�n usuario.
					Cuando se renombra una carpeta de actividad, se eliminan los portapapeles afectados y se renombran las carpetas expandidas afectadas. (84812 l�neas)
	Versi�n  9.18.2: 14/dic/2009	Se muestran estad�sticas de archivos para las actividades. (84579 l�neas)
	Versi�n  9.18.1: 13/dic/2009	Peque�as correcciones en la edici�n de actividades. (84562 l�neas)
	Versi�n  9.18:   13/dic/2009	Se publica para todos la edici�n de actividades. (84540 l�neas)
	Versi�n  9.17.1: 12/dic/2009	Traducci�n de algunos mensajes relacionados con la paginaci�n. (84514 l�neas)
	Versi�n  9.17:   12/dic/2009	Cambios internos relacionados con la paginaci�n.
					Las actividades se listan por p�ginas. (84487 l�neas)
	Versi�n  9.16.7: 12/dic/2009	Cambio interno en la forma de listar actividades. (84399 l�neas)
	Versi�n  9.16.6: 12/dic/2009	Se chequea si un usuario tiene permiso para crear archivos en una determinada actividad. (84431 l�neas)
	Versi�n  9.16.5: 12/dic/2009	Desde la lista de actividades, se pueden enviar archivos a la carpeta correspondiente. (84393 l�neas)
	Versi�n  9.16.4: 11/dic/2009	Corregidos algunos bugs relacionados con la edici�n de actividades. (84344 l�neas)
	Versi�n  9.16.3: 11/dic/2009	Cuando se elimina un grupo, se eliminan todas sus asociaciones con actividades.
					Cuando se elimina un tipo de grupo, se eliminan todas las asociaciones de grupos de ese tipo con actividades. (84332 l�neas)
	Versi�n  9.16.2: 11/dic/2009	Cuando se elimina una asignatura, se eliminan todas sus actividades. (84291 l�neas)
	Versi�n  9.16.1: 11/dic/2009	Se muestran los nombres de los grupos en los listados de actividades. (84263 l�neas)
	Versi�n  9.16:   08/dic/2009	Selector para mostrar las actividades asociadas al usuario o todas las actividades. (84185 l�neas)
	Versi�n  9.15:   08/dic/2009	Los grupos asociados a una actividad se almacenan en la base de datos. (84146 l�neas)
	Versi�n  9.14.2: 08/dic/2009	Se uniformizan m�s los listados de grupos. (83935 l�neas)
	Versi�n  9.14.1: 08/dic/2009	Se uniformizan los listados de grupos. (83953 l�neas)
	Versi�n  9.14:   07/dic/2009	Se incorpora la selecci�n de grupos al formulario de edici�n de una actividad. (83999 l�neas)
	Versi�n  9.13.16:07/dic/2009	Cambio en la tabla de actividades de la base de datos. (83953 l�neas)
	Versi�n  9.13.15:07/dic/2009	Corregido peque�o bug en zonas comunes, detectado por Estefan�a Sanjuan Cortijo. (83952 l�neas)
	Versi�n  9.13.14:04/dic/2009	Cambios en la edici�n de actividades. (83990 l�neas)
	Versi�n  9.13.13:04/dic/2009	En el archivo ZIP con los trabajos, se incluyen actividades y trabajos. (83995 l�neas)
	Versi�n  9.13.12:04/dic/2009	S�lo se crean carpetas de una actividad si la actividad est� abierta. (83948 l�neas)
	Versi�n  9.13.11:02/dic/2009	Comprobaciones para impedir modificar archivos en actividades cerradas. (83943 l�neas)
	Versi�n  9.13.10:02/dic/2009	Cambio en el enlace a los foros. (83872 l�neas)
	Versi�n  9.13.9: 02/dic/2009	Corregido bug en zonas comunes, detectado por el profesor Francisco G�mez Mula. (83846 l�neas)
	Versi�n  9.13.8: 02/dic/2009	Aumentados muchos tipos de letra, y los iconos de los men�s de 24x24 a 32x32. (83848 l�neas)
	Versi�n  9.13.7: 01/dic/2009	En las carpetas de actividades, se muestran las fechas de inicio y final. (83847 l�neas)
	Versi�n  9.13.6: 30/nov/2009	Cambia la palabra activity por assignment en ingl�s.
					Al editar una actividad, cuando se cambia el nombre de una carpeta de archivos de esa actividad, se renombra esa carpeta para todos los usuarios. (83730 l�neas)
	Versi�n  9.13.5: 30/nov/2009	Las carpetas de actividades se crean de nuevo internamente con el nombre dado por el profesor.
					Traducci�n de algunos mensajes relacionados con la exploraci�n de archivos.
					Cambios internos en la exploraci�n de archivos. (83647 l�neas)
	Versi�n  9.13.4: 29/nov/2009	Las carpetas de actividades se crean internamente con un nombre num�rico.
					Cambios internos en la exploraci�n de archivos. (83623 l�neas)
	Versi�n  9.13.3: 28/nov/2009	Traducidos los mensajes de los resultados de la busqueda de asignaturas / profesores. (83581 l�neas)
	Versi�n  9.13.2: 28/nov/2009	Cambios en la busqueda de asignaturas / profesores. (83531 l�neas)
	Versi�n  9.13.1: 28/nov/2009	Cambios en la busqueda de asignaturas / profesores. (83510 l�neas)
	Versi�n  9.13:   28/nov/2009	Cambios en el dise�o de la cabecera y el pie de la p�gina.
					En la cabecera de la p�gina se pueden buscar asignaturas. (83500 l�neas)
	Versi�n  9.12.9: 27/nov/2009	Desaparece la informaci�n del tama�o de las carpetas en la exploraci�n de archivos.
					Cambio en el dise�o de la cabecera de la p�gina. (83447 l�neas)
	Versi�n  9.12.8: 26/nov/2009	Numerosas optimizaciones internas en la exploraci�n de archivos. (83433 l�neas)
	Versi�n  9.12.7: 26/nov/2009	Cambios internos en la exploraci�n de archivos. (83577 l�neas)
	Versi�n  9.12.6: 26/nov/2009	Cambios internos en la exploraci�n de archivos.
					Se muestran tambi�n las actividades en "Mis trabajos". (83547 l�neas)
	Versi�n  9.12.5: 25/nov/2009	Numerosos cambios internos y comprobaciones de seguridad en la exploraci�n de archivos. (83507 l�neas)
	Versi�n  9.12.4: 24/nov/2009	Se crean autom�ticamente las carpetas del primer nivel de actividades. (83467 l�neas)
	Versi�n  9.12.3: 22/nov/2009	Peque�os cambios en la edici�n de actividades. (83410 l�neas)
	Versi�n  9.12.2: 21/nov/2009	Cambios en la edici�n de actividades. Se especifica la carpeta donde ir�n los archivos de la actividad. (83404 l�neas)
	Versi�n  9.12.1: 21/nov/2009	En las actividades se cambian las fechas de inicio y fin de solo fecha a fecha-hora. (83338 l�neas)
	Versi�n  9.12:   14/nov/2009	Contin�a la creaci�n de zona de archivos para actividades.
					Numerosos cambios internos en la exploraci�n de archivos. (83165 l�neas)
	Versi�n  9.11:   13/nov/2009	Comienza la creaci�n de zona de archivos para actividades. (82981 l�neas)
	Versi�n  9.10.6: 12/nov/2009	Cambios en la edici�n de actividades. (82550 l�neas)
	Versi�n  9.10.5: 11/nov/2009	Numerosos cambios en la edici�n de actividades. (82761 l�neas)
	Versi�n  9.10.4: 09/nov/2009	Cambios en la edici�n de actividades. (82765 l�neas)
	Versi�n  9.10.3: 09/nov/2009	Cambios en la edici�n de actividades. (82731 l�neas)
	Versi�n  9.10.2: 09/nov/2009	Cada actividad se puede editar por separado. (82765 l�neas)
	Versi�n  9.10.1: 05/nov/2009	Numerosos cambios en la opci�n para proponer actividades. (82684 l�neas)
	Versi�n  9.10:   04/nov/2009	Nueva opci�n para proponer actividades. (82505 l�neas)
	Versi�n  9.9.3:  03/nov/2009	Corregidos errores en el listado de los datos de los alumnos de una titulaci�n. (81318 l�neas)
	Versi�n  9.9.2:  02/nov/2009	En la eliminaci�n de usuarios antiguos se puede elegir entre alumnos y profesores. (81295 l�neas)
	Versi�n  9.9.1:  02/nov/2009	Eliminado el c�digo usado para cambiar los ID que empezaban por cero. (81272 l�neas)
	Versi�n  9.9:    02/nov/2009	La opci�n de dar de alta/baja alumnos ahora permite dar de alta/baja varios profesores. (81389 l�neas)
	Versi�n  9.8:    27/oct/2009	Termina la eliminaci�n de los ceros al principio de un ID en los formularios.
					La tabla usr_data se cambia eliminando todos los ceros iniciales en los ID.
					Se renombran todas las carpetas y archivos de usuarios cuyo ID empieza por cero.
					�Ser�a necesario cambiar tambi�n las tablas log y log_recent! No se hace de momento por el tiempo que requerir�a (varias horas). (81335 l�neas)
	Versi�n  9.7.2:  25/oct/2009	Comienza la eliminaci�n de los ceros al principio de un ID en los formularios. (81086 l�neas)
	Versi�n  9.7.1:  19/oct/2009	Optimizaci�n interna relacionada con la memoria ocupada por la lista de alumnos de la asignatura.
					Corregido peque�o bug en la lista de correos electr�nicos de estudiantes. (80982 l�neas)
	Versi�n  9.7:    19/oct/2009	En el alta/baja de varios alumnos, ahora hay una cuarta acci�n: dar de baja a los alumnos no especificados, y de alta a los alumnos especificados. (80963 l�neas)
	Versi�n  9.6:    17/oct/2009	Eliminadas las opciones de dar de alta varios alumnos y dar de baja varios alumnos, y las correspondientes funciones del programa. (80936 l�neas)
	Versi�n  9.5:    17/oct/2009	Termina la implementaci�n de alta/baja de alumnos importados de PRADO. (81542 l�neas)
	Versi�n  9.4.2:  15/oct/2009	Contin�a la implementaci�n de alta/baja de alumnos importados de PRADO. (81442 l�neas)
	Versi�n  9.4.1:  15/oct/2009	Comienza la implementaci�n de alta/baja de alumnos importados de PRADO. (81318 l�neas)
	Versi�n  9.4:    15/oct/2009	Termina la unificaci�n de las altas y bajas de varios alumnos en una �nica opci�n, aunque a�n no se abre a todos los profesores. (81408 l�neas)
	Versi�n  9.3.2:  14/oct/2009	Comienza la unificaci�n de las altas y bajas de varios alumnos en una �nica opci�n. (80800 l�neas)
	Versi�n  9.3.1:  14/oct/2009	Las listas oficiales de grupos y alumnos no se obtienen de PRADO m�s de una vez por sesi�n. (80707 l�neas)
	Versi�n  9.3:    14/oct/2009	Los profesores tienen acceso a sus listas oficiales de grupos y alumnos. (80678 l�neas)
	Versi�n  9.2.7:  13/oct/2009	Contin�a la implementaci�n de la opci�n para importar la lista de alumnos de una asignatura desde PRADO.
					Se guarda la consulta de alumnos en una tabla de la base de datos. (80692 l�neas)
	Versi�n  9.2.6:  09/oct/2009	Contin�a la implementaci�n de la opci�n para importar la lista de alumnos de una asignatura desde PRADO. (80518 l�neas)
	Versi�n  9.2.5:  08/oct/2009	Contin�a la implementaci�n de la opci�n para importar la lista de alumnos de una asignatura desde PRADO.
					Se parsea el listado de asignaturas y alumnos devuelto por PRADO en formato XML. (80373 l�neas)
	Versi�n  9.2.4:  06/oct/2009	Mejoras internas en la identificaci�n desde PRADO. Se almacena la sesi�n de PRADO en una tabla de la base de datos.
					Comienza la implementaci�n de una nueva opci�n para importar la lista de alumnos de una asignatura desde PRADO. (80133 l�neas)
	Versi�n  9.2.3:  06/oct/2009	Ya se puede entrar en SWAD desde PRADO, con validaci�n del usuario. (79966 l�neas)
	Versi�n  9.2.2:  05/oct/2009	Contin�an pruebas de PRADO. (79956 l�neas)
	Versi�n  9.2.1:  05/oct/2009	Contin�an pruebas de PRADO. (79826 l�neas)
	Versi�n  9.2:    02/oct/2009	Comienzan pruebas de PRADO. (79524 l�neas)
	Versi�n  9.1:    01/oct/2009	Los profesores pueden acceder a "Mis trabajos".
					Peque�o cambio en las reglas sobre qui�n puede ver qu� fotos.
					A�adido nuevo tipo MIME. (79342 l�neas)
	Versi�n  9.0.1:  30/sep/2009	Comienza edici�n del c�digo institucional de cada asignatura. (79288 l�neas)
	Versi�n  9.0:    30/sep/2009	La descripci�n de la asignatura pasa a ser ahora la gu�a docente de la asignatura. (79261 l�neas)

	Versi�n  8.55.3: 17/sep/2009	Algunos cambios menores internos. (79260 l�neas)
	Versi�n  8.55.2: 11/sep/2009	Mejoras internas relacionadas con el rango de usuarios en los listados. (79255 l�neas)
	Versi�n  8.55.1: 10/sep/2009	Cambios relacionados con el listado de los alumnos de una titulaci�n completa. (79233 l�neas)
	Versi�n  8.55:   03/sep/2009	Se permite ver un listado de los alumnos de una titulaci�n completa. (79191 l�neas)
	Versi�n  8.54.2: 08/jul/2009	Peque�os cambios relacionados con el servicio web. (79016 l�neas)
	Versi�n  8.54.1: 06/jul/2009	Se comprueba que se llama a cada operaci�n del servicio web desde una IP autorizada. (79015 l�neas)
	Versi�n  8.54:   06/jul/2009	Edici�n de los plugins. (78892 l�neas)
	Versi�n  8.53.11:05/jul/2009	Nuevo m�dulo swad_web_service para implementar el servicio web. (77954 l�neas)
	Versi�n  8.53.10:02/jul/2009	Contin�a la implementaci�n del servicio web. (77867 l�neas)
	Versi�n  8.53.9: 02/jul/2009	Contin�a la implementaci�n del servicio web. (77812 l�neas)
	Versi�n  8.53.8: 02/jul/2009	Contin�a la implementaci�n del servicio web. (77793 l�neas)
	Versi�n  8.53.7: 02/jul/2009	Contin�a la implementaci�n del servicio web. (77801 l�neas)
	Versi�n  8.53.6: 30/jun/2009	Contin�a la implementaci�n del servicio web. (77786 l�neas)
	Versi�n  8.53.5: 30/jun/2009	Contin�a la implementaci�n del servicio web.
					A�adida tabla ws_keys a la base de datos para almacenar las claves del servicio web. (77725 l�neas)
	Versi�n  8.53.4: 29/jun/2009	Contin�a la implementaci�n del servicio web. (77412 l�neas)
	Versi�n  8.53.3: 29/jun/2009	Contin�a la implementaci�n del servicio web. (77263 l�neas)
	Versi�n  8.53.2: 29/jun/2009	Contin�a la implementaci�n del servicio web. (77275 l�neas)
	Versi�n  8.53.1: 28/jun/2009	Contin�a la implementaci�n del servicio web. (77143 l�neas)
	Versi�n  8.53:   28/jun/2009	Contin�a la implementaci�n del servicio web, usando la biblioteca gSOAP. (77145 l�neas)
	Versi�n  8.52.2: 24/jun/2009	Contin�a la implementaci�n del plugin de control de asistencia a clase. (77052 l�neas)
	Versi�n  8.52.1: 24/jun/2009	Comienza la implementaci�n del plugin de control de asistencia a clase. (76935 l�neas)
	Versi�n  8.52:   23/jun/2009	Nueva pesta�a para complementos (plugins). (76896 l�neas)
	Versi�n  8.51.2: 23/jun/2009	No se env�an notificaciones de mensajes de foros eliminados. (76730 l�neas)
	Versi�n  8.51.1: 23/jun/2009	No se env�an notificaciones de avisos eliminados. (76725 l�neas)
	Versi�n  8.51:   22/jun/2009	Las notificaciones se env�an diferidamente. (76676 l�neas)
	Versi�n  8.50.2: 22/jun/2009	En las estad�sticas de notificaciones se distingue entre eventos notificados y correo enviados. (76473 l�neas)
	Versi�n  8.50.1: 21/jun/2009	Cambios internos relacionados con las notificaciones por mail. (76430 l�neas)
	Versi�n  8.50:   20/jun/2009	Traducci�n de varios mensajes, de variables y comentarios internos, y de tablas, relacionados con las convocatorias de examen. (76421 l�neas)
	Versi�n  8.49.5: 20/jun/2009	Cambios en los nombres de campos de algunas tablas de la base de datos. (76274 l�neas)
	Versi�n  8.49.4: 18/jun/2009	Cambios en los nombres de campos de algunas tablas de la base de datos. (76209 l�neas)
	Versi�n  8.49.3: 18/jun/2009	Traducci�n de varios mensajes, y de variables y comentarios internos relacionados con altas y bajas de usuarios. (76200 l�neas)
	Versi�n  8.49.2: 17/jun/2009	Traducci�n de varios mensajes. (76154 l�neas)
	Versi�n  8.49.1: 15/jun/2009	Traducci�n de varios mensajes, y de variables y comentarios internos relacionados con los test. (76107 l�neas)
	Versi�n  8.49:   08/jun/2009	Traducci�n de variables y comentarios internos.
					Nuevo m�dulo swad_record para las funciones relacionadas con las fichas. (75969 l�neas)
	Versi�n  8.48.7: 06/jun/2009	Traducci�n de varios mensajes, y de variables y comentarios internos. (75827 l�neas)
	Versi�n  8.48.6: 06/jun/2009	Optimizaciones internas al escribir los nombres de los usuarios. (75724 l�neas)
	Versi�n  8.48.5: 05/jun/2009	Nuevo m�dulo swad_notice para las funciones relacionadas con los avisos (notas amarillas). (75722 l�neas)
	Versi�n  8.48.4: 05/jun/2009	Nuevo m�dulo swad_pagination para las funciones relacionadas con la paginaci�n de mensajes y foros. (75634 l�neas)
	Versi�n  8.48.3: 05/jun/2009	Nuevo m�dulo swad_forum para las funciones relacionadas con los foros. (75543 l�neas)
	Versi�n  8.48.2: 04/jun/2009	Nuevo m�dulo swad_chat para las funciones relacionadas con el chat. (75418 l�neas)
	Versi�n  8.48.1: 03/jun/2009	Numerosos cambios internos y traducci�n de algunos mensajes. (75331 l�neas)
	Versi�n  8.48:   03/jun/2009	Cambios internos en la forma de almacenar los textos de los men�s.
					Traducci�n de algunos mensajes relacionados con la notificaci�n por correo electr�nico. (75230 l�neas)
	Versi�n  8.47.3: 03/jun/2009	Traducci�n de algunos mensajes relacionados con la notificaci�n por correo electr�nico. (75099 l�neas)
	Versi�n  8.47.2: 03/jun/2009	Traducci�n de algunos mensajes.
					Renombradas las funciones relacionadas con foros y con chat para situarlas en m�dulos separados del m�dulo de mensajes. (75073 l�neas)
	Versi�n  8.47.1: 03/jun/2009	Se evitan notificaciones al autor del evento. (75039 l�neas)
	Versi�n  8.47:   02/jun/2009	Se env�an notificaciones por correo de nuevos archivos de calificaciones.
					Se muestran estad�sticas del n�mero de notificaciones de cada evento.
					Todas las estad�sticas de notificaciones se ponen a 0. (75029 l�neas)
	Versi�n  8.46.3: 02/jun/2009	Cambios internos relacionados con en el env�o de notificaciones. (74787 l�neas)
	Versi�n  8.46.2: 02/jun/2009	En la estad�stica de foros se muestra el n�mero de notificaciones en cada tipo de foro. (74720 l�neas)
	Versi�n  8.46.1: 02/jun/2009	Cambios internos y algunas traducciones en el env�o de notificaciones. (74703 l�neas)
	Versi�n  8.46:   02/jun/2009	Se env�an notificaciones por correo de respuestas en los foros. (74623 l�neas)
	Versi�n  8.45:   01/jun/2009	Nuevo m�dulo swad_notification sobre notificaciones por correo electr�nico.
					Se env�an notificaciones por correo de convocatorias de examen. (74520 l�neas)
	Versi�n  8.44.1: 30/may/2009	Las convocatorias de examen no se eliminan realmente, sino que se marcan como eliminadas, para poder llevar en el futuro la estad�stica de las convocatorias totales. (74398 l�neas)
	Versi�n  8.44:   30/may/2009	Nueva estad�stica con el n�mero de usuarios que han elegido cada tipo de notificaci�n. (74378 l�neas)
	Versi�n  8.43:   30/may/2009	Se env�an notificaciones por correo de avisos nuevos. (74270 l�neas)
	Versi�n  8.42.6: 30/may/2009	En las estad�sticas de avisos, se muestra el n�mero de correos electr�nicos de notificaci�n enviados para avisar de avisos nuevos. (74096 l�neas)
	Versi�n  8.42.5: 30/may/2009	Cambios internos en la forma de almacenar el estado de los avisos. (74067 l�neas)
	Versi�n  8.42.4: 30/may/2009	Los avisos no se eliminan realmente, sino que se marcan como eliminados, para poder llevar la estad�stica de los avisos totales. (74044 l�neas)
	Versi�n  8.42.3: 30/may/2009	Se almacenan en las preferencias del usuario, en la base de datos, todas las posibles causas de notificaci�n por correo electr�nico. (74021 l�neas)
	Versi�n  8.42.2: 29/may/2009	En las preferencias, el usuario puede elegir en qu� ocasiones desea que se le env�en notificaciones por correo electr�nico. (74034 l�neas)
	Versi�n  8.42.1: 28/may/2009	Traducci�n de varios comentarios y variables internos. (73916 l�neas)
	Versi�n  8.42:   28/may/2009	Termina la implementaci�n del env�o de correos electr�nicos de aviso. (73909 l�neas)
	Versi�n  8.41.4: 28/may/2009	Contin�a la implementaci�n del env�o de correos electr�nicos de aviso. (73839 l�neas)
	Versi�n  8.41.3: 28/may/2009	Contin�a la implementaci�n del env�o de correos electr�nicos de aviso. (73828 l�neas)
	Versi�n  8.41.2: 27/may/2009	Contin�a la implementaci�n del env�o de correos electr�nicos de aviso. (73750 l�neas)
	Versi�n  8.41.1: 27/may/2009	Comienza la implementaci�n del env�o de correos electr�nicos de aviso. (73698 l�neas)
	Versi�n  8.41:   26/may/2009	Cambios en la traducci�n al portugu�s, sugeridos por Rui Raposo (Univ. Aveiro, Portugal).
					En las preferencias se puede cambiar la privacidad de la foto. (73650 l�neas)
	Versi�n  8.40.1: 25/may/2009	Traducci�n de varios mensajes. (73584 l�neas)
	Versi�n  8.40:   25/may/2009	A�adidas cabeceras de licencia "GNU Affero General Public License, versi�n 3". (73566 l�neas)
	Versi�n  8.39.8: 24/may/2009	Traducci�n de varios mensajes, y de comentarios y variables internos. (72427 l�neas)
	Versi�n  8.39.7: 23/may/2009	Traducci�n de varios mensajes. (72096 l�neas)
	Versi�n  8.39.6: 22/may/2009	Traducci�n de varios comentarios y variables internos. (72063 l�neas)
	Versi�n  8.39.5: 20/may/2009	Traducci�n de varios comentarios y variables internos. (72059 l�neas)
	Versi�n  8.39.4: 19/may/2009	Traducci�n de varios mensajes. (72066 l�neas)
	Versi�n  8.39.3: 19/may/2009	Traducci�n de varios mensajes. (71997 l�neas)
	Versi�n  8.39.2: 19/may/2009	Traducci�n de varios mensajes. (71884 l�neas)
	Versi�n  8.39.1: 18/may/2009	Corregido peque�o bug en configuraci�n de test, detectado por Isabel Mar�a S�nchez Calle. (71841 l�neas)
	Versi�n  8.39:   18/may/2009	Los nombres de las carpetas ra�z de los exploradores de archivos aparecen en m�s sitios en el idioma seleccionado. (71838 l�neas)
	Versi�n  8.38.9: 18/may/2009	El archivo zip con los trabajos de cada alumno ya no contiene una carpeta "trabajos" dentro de la carpeta de cada alumno. (71832 l�neas)
	Versi�n  8.38.8: 18/may/2009	Los nombres de las carpetas ra�z de los exploradores de archivos aparecen en algunos sitios en el idioma seleccionado. (71836 l�neas)
	Versi�n  8.38.7: 17/may/2009	Traducci�n de varios mensajes, y de comentarios internos. (71767 l�neas)
	Versi�n  8.38.6: 16/may/2009	Traducci�n de varios mensajes, y de comentarios internos. (71661 l�neas)
	Versi�n  8.38.5: 15/may/2009	Traducci�n de varios mensajes, y de comentarios internos. (71574 l�neas)
	Versi�n  8.38.4: 12/may/2009	Traducci�n de varios mensajes, y de comentarios internos. (71397 l�neas)
	Versi�n  8.38.3: 11/may/2009	Traducci�n de varios mensajes, y de comentarios internos. (71338 l�neas)
	Versi�n  8.38.2: 11/may/2009	Traducci�n de varios mensajes, y de comentarios internos. (71240 l�neas)
	Versi�n  8.38.1: 10/may/2009	Muchas optimizaciones internas relacionadas con los c�digos de usuarios cifrados. (71103 l�neas)
	Versi�n  8.38:   08/may/2009	El cifrado AES para c�digos de usuarios se sustituye por SHA-256. (71166 l�neas)
	Versi�n  8.37.1: 07/may/2009	Eliminado c�digo temporal para crear los c�digos de usuario cifrados. (71202 l�neas)
	Versi�n  8.37:   07/may/2009	Traducci�n de comentarios y variables internos.
					A�adido c�digo �nico cifrado mediante SHA-256 para cada usuario. (71269 l�neas)
	Versi�n  8.36.4: 06/may/2009	Traducci�n de comentarios y variables internos. (71369 l�neas)
	Versi�n  8.36.3: 04/abr/2009	Traducci�n de algunos mensajes. (71104 l�neas)
	Versi�n  8.36.2: 01/abr/2009	Modificaciones internas relacionadas con los permisos de acceso a cada foro. (71040 l�neas)
	Versi�n  8.36.1: 01/abr/2009	Cambios en el listado de asignaturas y sus medidores. (71031 l�neas)
	Versi�n  8.36:   31/mar/2009	Nueva funcionalidad en estad�sticas para listar las asignaturas y sus medidores. (70900 l�neas)
	Versi�n  8.35.2: 31/mar/2009	Traducci�n de algunos mensajes. (70648 l�neas)
	Versi�n  8.35.1: 30/mar/2009	Traducci�n de algunos mensajes relacionados con los foros y con el env�o de fotos. (70585 l�neas)
	Versi�n  8.35:   30/mar/2009	El usuario puede elegir si desea ver todos sus foros o s�lo los de la instituci�n/titulaci�n/asignatura seleccionada. (70472 l�neas)
	Versi�n  8.34.1: 29/mar/2009	Traducci�n de algunos mensajes relacionados con los foros. (70378 l�neas)
	Versi�n  8.34:   29/mar/2009	Cambios en el listado de los foros cuando el usuario est� identificado como administrador o como superusuario. (70345 l�neas)
	Versi�n  8.33.3: 29/mar/2009	Optimizaciones internas relacionadas con las listas de instituciones, titulaciones y asignaturas del usuario identificado. (70258 l�neas)
	Versi�n  8.33.2: 28/mar/2009	Cambios en el texto del listado de foros, y en las comprobaciones efectuadas al eliminar mensajes y hebras de los foros. (70225 l�neas)
	Versi�n  8.33.1: 28/mar/2009	Cambios en el orden del listado de foros. (70178 l�neas)
	Versi�n  8.33:   28/mar/2009	Cambios en el listado de foros para mostrar todos los foros del usuario. (70177 l�neas)
	Versi�n  8.32.3: 28/mar/2009	Traducci�n al ingl�s de numerosos comentarios y variables. (70036 l�neas)
	Versi�n  8.32.2: 27/mar/2009	Comienza la implementaci�n de cambios en el listado de foros para mostrar todos los foros del usuario. (70011 l�neas)
	Versi�n  8.32.1: 24/mar/2009	En el chat, los enlaces a las titulaciones aparecen con sus correspondientes iconos. (69865 l�neas)
	Versi�n  8.32:   21/mar/2009	Nueva estad�stica con el n�mero de avisos activos y obsoletos. (69794 l�neas)
	Versi�n  8.31.2: 19/mar/2009	Cambios internos relacionados con el chat. El nuevo chat se pone a disposici�n de los usuarios. (69641 l�neas)
	Versi�n  8.31.1: 16/mar/2009	Cambios en el listado de salas de chat. (69630 l�neas)
	Versi�n  8.31:   16/mar/2009	Incorporada la nueva versi�n del chat en pruebas, programada por Daniel Jes�s Calandria Hern�ndez. (69672 l�neas)
	Versi�n  8.30.4: 16/mar/2009	Traducci�n de numerosos mensajes. (69446 l�neas)
	Versi�n  8.30.3: 16/mar/2009	Corregido peque�o bug en javascript relacionado con las estad�sticas.
					Traducci�n de varios mensajes. (69372 l�neas)
	Versi�n  8.30.2: 15/mar/2009	Traducci�n de varios mensajes. (69285 l�neas)
	Versi�n  8.30.1: 15/mar/2009	Cambios en el dibujo de los avisos. (69063 l�neas)
	Versi�n  8.30:   14/mar/2009	Los avisos se pueden marcar como obsoletos sin tener que eliminarlos. (69060 l�neas)
	Versi�n  8.29:   14/mar/2009	Cambios en la creaci�n y eliminaci�n de avisos. (68897 l�neas)
	Versi�n  8.28.1: 03/mar/2009	Mejora interna y traducci�n en las estad�sticas de acceso. (68807 l�neas)
	Versi�n  8.28:   02/mar/2009	Las estad�sticas de uso de la plataforma se muestran desglosadas. (68736 l�neas)
	Versi�n  8.27.4: 01/mar/2009	Traducci�n de algunos mensajes y comentarios internos relacionados con los test de autoevaluaci�n.
					Cambios en las tablas de la base de datos relacionadas con los test de autoevaluaci�n. (68527 l�neas)
	Versi�n  8.27.3: 28/feb/2009	Traducci�n de algunos mensajes y comentarios internos relacionados con los test de autoevaluaci�n. (68433 l�neas)
	Versi�n  8.27.2: 28/feb/2009	Traducci�n de algunos mensajes y comentarios internos relacionados con los test de autoevaluaci�n. (68273 l�neas)
	Versi�n  8.27.1: 26/feb/2009	Traducci�n de algunos mensajes. (68189 l�neas)
	Versi�n  8.27:   25/feb/2009	Finaliza la implementaci�n de la elecci�n entre iconos seguros (https) y no seguros (http). (68101 l�neas)
	Versi�n  8.26.2: 24/feb/2009	Comienza la implementaci�n de la elecci�n entre iconos seguros (https) y no seguros (http). (68064 l�neas)
	Versi�n  8.26.1: 19/feb/2009	Reducido el n�mero de d�as en el log reciente de 32 a 15 d�as. (67940 l�neas)
	Versi�n  8.26:   18/feb/2009	En las estad�sticas de tama�o de las zonas de archivos se muestra el n�mero de carpetas, archivos, y tama�o promedio por asignatura y por usuario. (67930 l�neas)
	Versi�n  8.25.3: 18/feb/2009	Mejora interna en las estad�sticas de tama�o de las zonas de archivos. (67798 l�neas)
	Versi�n  8.25.2: 17/feb/2009	En Estad�sticas se muestra el tama�o de otras zonas de archivos. (67860 l�neas)
	Versi�n  8.25.1: 17/feb/2009	En Estad�sticas se muestra el tama�o de las zonas de descarga de archivos. (67667 l�neas)
	Versi�n  8.25:   16/feb/2009	Cada vez que un usuario ve una zona de almacenamiento de archivos, se guarda en una tabla de la base de datos, el tama�o de esa zona. (67475 l�neas)
	Versi�n  8.24.4: 16/feb/2009	Ampliaci�n de las cuotas de almacenamiento. (67283 l�neas)
	Versi�n  8.24.3: 15/feb/2009	Cambio a un nuevo servidor. (67273 l�neas)
	Versi�n  8.24.2: 13/feb/2009	En Estad�sticas se muestra el n�mero de mensajes eliminados. (67272 l�neas)
	Versi�n  8.24.1: 13/feb/2009	Corregido problema est�tico en listado de archivos en IE7. (67197 l�neas)
	Versi�n  8.24:   12/feb/2009	Corregido bug en la eliminaci�n de titulaciones: cuando se eliminaba una titulaci�n, no se eliminaban las entradas correspondientes en la tabla de estad�sticas de titulaciones.
					En Estad�sticas se muestra el n�mero total de mensajes enviados y recibidos en toda la plataforma, en la titulaci�n, o en la asignatura. (67172 l�neas)
	Versi�n  8.23.3: 11/feb/2009	Insertado anuncio Taller SWAD. (66941 l�neas)
	Versi�n  8.23.2: 10/feb/2009	En Estad�sticas > Uso de SWAD, se muestran estad�sticas de n�mero de hebras por foro, n�mero de mensajes por hebra y n�mero de mensajes por foro. (66940 l�neas)
	Versi�n  8.23.1: 10/feb/2009	Traducci�n interna al ingl�s de numerosos comentarios, variables, etc. (66884 l�neas)
	Versi�n  8.23:   10/feb/2009	Termina la implementaci�n de estad�sticas de uso de los foros. (66883 l�neas)
	Versi�n  8.22.1: 10/feb/2009	Comienza la implementaci�n de estad�sticas de uso de los foros. (66745 l�neas)
	Versi�n  8.22:   09/feb/2009	Se crea un foro dentro de cada asignatura exclusivo para profesores. (66403 l�neas)
	Versi�n  8.21.2: 09/feb/2009	Traducci�n de numerosos mensajes.
					Traducci�n interna al ingl�s de numerosos comentarios, variables, etc. (66395 l�neas)
	Versi�n  8.21.1: 08/feb/2009	Los enlaces institucionales de la derecha de la p�gina se toman de la base de datos. (66003 l�neas)
	Versi�n  8.21:   08/feb/2009	Finaliza la implementaci�n de la edici�n de enlaces institucionales. (66008 l�neas)
	Versi�n  8.20.1: 05/feb/2009	Comienza la implementaci�n de la edici�n de enlaces institucionales. (65199 l�neas)
	Versi�n  8.20:   05/feb/2009	El usuario puede elegir si quiere ver todos los mensajes recibidos o s�lo los no le�dos. (65192 l�neas)
	Versi�n  8.19.3: 05/feb/2009	Optimizaci�n en el c�lculo del n�mero de mensajes no le�dos. (65096 l�neas)
	Versi�n  8.19.2: 04/feb/2009	Corregido bug en el n�mero de mensajes no le�dos cuando se filtran los mensajes recibidos por asignatura, autor o contenido. (65048 l�neas)
	Versi�n  8.19.1: 04/feb/2009	Las estad�sticas de preferencias (idioma, color y columnas) de la plataforma se pueden restringir a la titulaci�n seleccionada o a la asignatura seleccionada.
					Por defecto, las estad�sticas de uso global se restringen a la titulaci�n (en caso de que haya una titulaci�n seleccionada). (64994 l�neas)
	Versi�n  8.19:   04/feb/2009	Las estad�sticas de uso global de la plataforma se pueden restringir a la titulaci�n seleccionada o a la asignatura seleccionada. (64934 l�neas)
	Versi�n  8.18.9: 01/feb/2009	El usuario puede escoger la titulaci�n cuya foto promedio desea actualizar. (64785 l�neas)
	Versi�n  8.18.8: 01/feb/2009	Para actualizar las fotograf�as promedio, en lugar de escoger la titulaci�n con la fotograf�a menos recientemente calculada, se escoje entre varias de las que tienen las fotograf�as menos recientemente calculadas. (64645 l�neas)
	Versi�n  8.18.7: 15/ene/2009	Corregido error en el c�lculo del tiempo para calcular la fotograf�a promedio de una titulaci�n. (64631 l�neas)
	Versi�n  8.18.6: 15/ene/2009	Corregido error en el c�lculo del tiempo estimado para calcular la fotograf�a promedio de la titulaci�n menos recientemente actualizada. (64610 l�neas)
	Versi�n  8.18.5: 15/ene/2009	En la fotograf�a promedio se muestra el tiempo estimado para calcular la fotograf�a promedio de la titulaci�n menos recientemente actualizada. (64604 l�neas)
	Versi�n  8.18.4: 13/ene/2009	En la tabla de estad�sticas de cada titulaci�n se almacena el tiempo empleado en calcular la fotograf�a promedio. (64514 l�neas)
	Versi�n  8.18.3: 12/ene/2009	Peque�o cambio en la escritura del pie de la p�gina. (64484 l�neas)
	Versi�n  8.18.2: 07/ene/2009	Corregido peque�o bug en la compresi�n de trabajos. (64475 l�neas)
	Versi�n  8.18.1: 31/dic/2008	Corregido peque�o bug en los foros. (64470 l�neas)
	Versi�n  8.18:   22/dic/2008	El c�lculo de las fotograf�as promedio lo puede ir realizando cada usuario al verlas. Si el usuario pulsa en el enlace indicado a tal efecto, se calculan las fotograf�as promedio correspondientes a una titulaci�n que llevaban m�s tiempo sin calcularse. (64465 l�neas)
	Versi�n  8.17.2: 21/dic/2008	Traducci�n de algunos mensajes. (64419 l�neas)
	Versi�n  8.17.1: 21/dic/2008	Peque�os cambios en el dise�o de la p�gina. (64416 l�neas)
	Versi�n  8.17:   21/dic/2008	Se a�aden dos nuevos tipos de foros: temas generales y temas generalas para profesores. Se crean adem�s dos foros para cada instituci�n: uno para usuarios en general y otro para profesores. (64432 l�neas)
	Versi�n  8.16.6: 18/dic/2008	Los nombres �nicos, usados para directorios temporales, nombres de fotos, etc. ahora incorporan tambi�n la direcci�n IP, y adem�s se cifran mediante SHA-256. (64056 l�neas)
	Versi�n  8.16.5: 17/dic/2008	En los ex�menes de autoevaluaci�n aparece el logo de la instituci�n asociada a la titulaci�n elegida.
					Optimizada la escritura de la cabecera de los ex�menes de autoevaluaci�n. (64077 l�neas)
	Versi�n  8.16.4: 17/dic/2008	En las orlas, listas, calendarios y horarios aparece el logo de la instituci�n asociada a la titulaci�n elegida. (64092 l�neas)
	Versi�n  8.16.3: 14/dic/2008	Cambios internos y en la base de datos relacionados con las convocatorias de examen.
					En las convocatorias de examen aparece el logo y el nombre de la instituci�n a la que pertenece la titulaci�n. (64065 l�neas)
	Versi�n  8.16.2: 14/dic/2008	En el listado de instituciones se muestran el n�mero de centros, de departamentos y de titulaciones de cada instituci�n. (64065 l�neas)
	Versi�n  8.16.1: 14/dic/2008	Adem�s de asociarse a un lugar, cada titulaci�n se asocia a una instituci�n.
					Se permite la edici�n de la titulaci�n en centros y departamentos existentes. (63846 l�neas)
	Versi�n  8.16:   13/dic/2008	Numerosos cambios internos.
					Nuevo m�dulo para la gesti�n de par�metros de llamada al CGI. (63617 l�neas)
	Versi�n  8.15.4: 13/dic/2008	Separado m�dulo para la gesti�n de grupos del m�dulo de gesti�n de usuarios. (63476 l�neas)
	Versi�n  8.15.3: 11/dic/2008	Optimizada la consulta de titulaciones que se realiza antes de presentar el formulario de la ficha. Ahora no se obtiene el n�mero de usuarios de cada titulaci�n, innecesario en ese caso.
					En el listado de titulaciones se muestran, para cada titulaci�n, el n�mero de usuarios totales, el n�mero de alumnos y el n�mero de profesores. (63393 l�neas)
	Versi�n  8.15.2: 11/dic/2008	Corregido bug en el listado de usuarios relacionado con las instituciones. (63373 l�neas)
	Versi�n  8.15.1: 10/dic/2008	En el listado de instituciones se muestra el n�mero de usuarios de cada instituci�n, no el n�mero de profesores. (63370 l�neas)
	Versi�n  8.15:   10/dic/2008	Los alumnos tambi�n deben rellenar la instituci�n (universidad) a la que pertenecen. (63338 l�neas)
	Versi�n  8.14.6: 10/dic/2008	Traducci�n de algunos mensajes.
					Eliminada la etiqueta "font" del c�digo XHTML. (63331 l�neas)
	Versi�n  8.14.5: 10/dic/2008	Nuevo modulo swad_file para operaciones con archivos.
					Cambios internos: cambio de m�dulo de algunas funciones. (63259 l�neas)
	Versi�n  8.14.4: 10/dic/2008	Corregido peque�o bug relacionado con la selecci�n de temas de dise�o. (63199 l�neas)
	Versi�n  8.14.3: 09/dic/2008	Optimizaci�n del c�digo para dibujar un mes. (63193 l�neas)
	Versi�n  8.14.2: 09/dic/2008	Sustituci�n del c�digo antiguo de gesti�n de per�odos no lectivos por el nuevo. (63175 l�neas)
	Versi�n  8.14.1: 09/dic/2008	Modificaci�n en la edici�n de d�as festivos y per�odos no lectivos. (63254 l�neas)
	Versi�n  8.14:   09/dic/2008	Se pueden editar los per�odos no lectivos. (63213 l�neas)
	Versi�n  8.13.3: 08/dic/2008	Traducci�n de algunos mensajes. (62979 l�neas)
	Versi�n  8.13.2: 08/dic/2008	Eliminado c�digo antiguo de gesti�n de d�as festivos y campus. (62949 l�neas)
	Versi�n  8.13.1: 08/dic/2008	Los d�as festivos usados en el dibujo de los meses se toman de la base de datos. (63201 l�neas)
	Versi�n  8.13:   08/dic/2008	Finaliza la implementaci�n de la edici�n de d�as festivos. (63158 l�neas)
	Versi�n  8.12.6: 08/dic/2008	Contin�a la implementaci�n de la edici�n de d�as festivos. (63159 l�neas)
	Versi�n  8.12.5: 07/dic/2008	Contin�a la implementaci�n de la edici�n de d�as festivos. (62940 l�neas)
	Versi�n  8.12.4: 06/dic/2008	Comienza la implementaci�n de la edici�n de d�as festivos. (62132 l�neas)
	Versi�n  8.12.3: 06/dic/2008	Algunos cambios en el m�dulo de estad�sticas.
					Los profesores pueden volver a consultar accesos en per�odos largos de tiempo, pero s�lo en el caso de accesos a la asignatura, no globales. (62113 l�neas)
	Versi�n  8.12.2: 06/dic/2008	Algunos cambios en el m�dulo de fechas. (62104 l�neas)
	Versi�n  8.12.1: 06/dic/2008	Se puede elegir lugar al editar una titulaci�n. (62150 l�neas)
	Versi�n  8.12:   06/dic/2008	Se pueden editar lugares o localidades (por ejemplo, Granada, Ceuta, Melilla). (62043 l�neas)
	Versi�n  8.11.8: 05/dic/2008	En la fichas de un profesor aparece el logo y el nombre de la instituci�n a la que pertenece ese profesor. (61146 l�neas)
	Versi�n  8.11.7: 04/dic/2008	Corregido bug en la edici�n de instituciones. (61118 l�neas)
	Versi�n  8.11.6: 03/dic/2008	Cambios relacionados con el tama�o m�ximo de un archivo recibido. (61110 l�neas)
	Versi�n  8.11.5: 01/dic/2008	Contin�a la implementaci�n de la instituci�n asociada a cada centro y departamento. (61106 l�neas)
	Versi�n  8.11.4: 30/nov/2008	Corregido peque�o bug en la ficha de profesores, detectado por Luis E. Hueli Amador.
					A�adidos nuevos tipos de archivos. (60970 l�neas)
	Versi�n  8.11.3: 30/nov/2008	Comienza la implementaci�n de la instituci�n asociada a cada centro y departamento. (60969 l�neas)
	Versi�n  8.11.2: 30/nov/2008	En el listado de instituciones aparece el logo de cada instituci�n. (60954 l�neas)
	Versi�n  8.11.1: 30/nov/2008	A�adida instituci�n a la ficha del profesor. (60936 l�neas)
	Versi�n  8.11:   30/nov/2008	Edici�n de instituciones (universidades, institutos, etc.). (60849 l�neas)
	Versi�n  8.10.3: 29/nov/2008	No se permite a los usuarios (excepto al superusuario) consultar estad�sticas de acceso con un rango de fechas mayor que cierto n�mero de d�as. (59767 l�neas)
	Versi�n  8.10.2: 24/nov/2008	Muchos cambios internos. Cambiado el archivo swad_const.h por swad_paths.h
					Aumentadas algunas cuotas de almacenamiento. (59698 l�neas)
	Versi�n  8.10.1: 24/nov/2008	Los administradores de titulaci�n pueden ver los ID de los profesores de esa titulaci�n.
					Muchos cambios internos. Eliminado el archivo swad_types.h (59688 l�neas)
	Versi�n  8.10:   21/nov/2008	La nueva versi�n del programa de procesamiento de fotograf�as se pone a disposici�n de todos los usuarios y se elimina la versi�n anterior.
					Optimizaciones en la eliminaci�n de fotograf�as. (59705 l�neas)
	Versi�n  8.9.1:  20/nov/2008	Nueva versi�n en pruebas del programa de procesamiento de fotograf�as. (59868 l�neas)
	Versi�n  8.9:    20/nov/2008	Nueva versi�n en pruebas del programa de procesamiento de fotograf�as, que permite seleccionar un rostro entre varios detectados. (59840 l�neas)
	Versi�n  8.8:    20/nov/2008	Los listados de centros y de departamentos se pueden ordenar por n�mero de profesores. (59747 l�neas)
	Versi�n  8.7.2:  18/nov/2008	Traducci�n de varios mensajes y diversas optimizaciones de c�digo. (59553 l�neas)
	Versi�n  8.7.1:  18/nov/2008	Mejoras en la traducci�n al alem�n, por Rafael Barranco-Droege.
					Traducci�n de varios mensajes. (59520 l�neas)
	Versi�n  8.7:    09/nov/2008	Optimizada la selecci�n de usuarios antiguos a eliminar. (59413 l�neas)
	Versi�n  8.6.16: 08/nov/2008	Corregido bug al buscar los ID en la lista de alumnos a dar de alta o de baja. (59390 l�neas)
	Versi�n  8.6.15: 08/nov/2008	Corregido bug al enviar un mensaje indicando el ID del destinatario. (59386 l�neas)
	Versi�n  8.6.14: 03/nov/2008	Traducci�n de varios mensajes.
					Mejoras en la implementaci�n del programa de procesamiento de fotograf�as. (59374 l�neas)
	Versi�n  8.6.13: 03/nov/2008	Se admite como ID una cadena que contenga al menos 4 d�gitos (hasta ahora el m�nimo era 5 d�gitos) y un grupo de hasta 5 letras (hasta ahora el m�ximo era 3).
					Cambios menores en el env�o del par�metro oculto con el ID de otro usuario.
					Traducci�n de varios mensajes. (59313 l�neas)
	Versi�n  8.6.12: 03/nov/2008	Traducci�n de varios mensajes. (59298 l�neas)
	Versi�n  8.6.11: 02/nov/2008	Traducci�n de varios mensajes. (59214 l�neas)
	Versi�n  8.6.10: 02/nov/2008	Cambios en el dise�o. (59090 l�neas)
	Versi�n  8.6.9:  01/nov/2008	Traducci�n de varios mensajes. (59077 l�neas)
	Versi�n  8.6.8:  01/nov/2008	Nuevo m�dulo swad_exam con funciones relacionadas con las convocatorias de examen (calls for exams). (59017 l�neas)
	Versi�n  8.6.7:  01/nov/2008	Nuevo m�dulo swad_layout con funciones relacionadas con el dise�o (layout) de la p�gina. (58961 l�neas)
	Versi�n  8.6.6:  01/nov/2008	Se limita la longitud de los nombres de titulaci�n y asignatura en la parte superior de la pantalla.
					Cambios en el dise�o. (58995 l�neas)
	Versi�n  8.6.5:  31/oct/2008	Traducci�n de la ficha personal. (58955 l�neas)
	Versi�n  8.6.4:  31/oct/2008	Traducci�n de varios mensajes. (58698 l�neas)
	Versi�n  8.6.3:  31/oct/2008	Nueva opci�n provisional para hacer pruebas con el nuevos programa de procesamiento de fotograf�as.
					Traducci�n de los t�tulos de cada acci�n. (58481 l�neas)
	Versi�n  8.6.2:  30/oct/2008	Traducci�n de varios elementos relacionados con centros y departamentos. (58096 l�neas)
	Versi�n  8.6.1:  29/oct/2008	Se obliga a los profesores a elegir centro y departamento.
					Mejoras en el listado de centros y departamentos (57982 l�neas)
	Versi�n  8.6:    28/oct/2008	Se muestra junto a cada foro el n�mero de hebras con mensajes nuevos, y junto a cada hebra el n�mero de mensajes sin leer. (57801 l�neas)
	Versi�n  8.5:    27/oct/2008	Finaliza la edici�n de departamentos. (57628 l�neas)
	Versi�n  8.4:    27/oct/2008	Finaliza la edici�n de centros. (57041 l�neas)
	Versi�n  8.3.5:  27/oct/2008	Contin�a la edici�n de centros. (56750 l�neas)
	Versi�n  8.3.4:  21/oct/2008	Contin�a la edici�n de centros. (56578 l�neas)
	Versi�n  8.3.3:  21/oct/2008	Corregido peque�o bug que ocurr�a al eliminar un tipo de titulaci�n. (56460 l�neas)
	Versi�n  8.3.2:  20/oct/2008	Comienza la edici�n de centros. (56433 l�neas)
	Versi�n  8.3.1:  20/oct/2008	Traducci�n de algunos mensajes. (56326 l�neas)
	Versi�n  8.3:    20/oct/2008	Finaliza la implementaci�n de nuevas estad�sticas de titulaciones en forma de lista. (56223 l�neas)
	Versi�n  8.2.5:  20/oct/2008	Contin�a la implementaci�n de nuevas estad�sticas de titulaciones en forma de lista. (56181 l�neas)
	Versi�n  8.2.4:  19/oct/2008	Contin�a la implementaci�n de nuevas estad�sticas de titulaciones en forma de lista. (56156 l�neas)
	Versi�n  8.2.3:  19/oct/2008	Las titulaciones de la orla de fotos promedio de titulaciones se pueden ordenar seg�n cuatro criterios. (56051 l�neas)
	Versi�n  8.2.2:  18/oct/2008	Contin�a la implementaci�n de nuevas estad�sticas de titulaciones en forma de lista. (55890 l�neas)
	Versi�n  8.2.1:  18/oct/2008	Comienza la implementaci�n de nuevas estad�sticas de titulaciones en forma de lista. (55837 l�neas)
	Versi�n  8.2:    17/oct/2008	Nuevos m�dulos swad_centre y swad_department para la edici�n de centros y departamentos. (55775 l�neas)
	Versi�n  8.1:    16/oct/2008	Nuevo m�dulo swad_syllabus para la edici�n de los temarios. (55671 l�neas)
	Versi�n  8.0:    15/oct/2008	Para seleccionar el idioma, en lugar de usar banderitas, se usa un selector desplegable. (55641 l�neas)
	Versi�n  7.73.1: 29/sep/2008	Cambiadas las fiestas para el curso 2008-2009. (55606 l�neas)
	Versi�n  7.73:   17/sep/2008	Se guarda en la base de datos la preferencia sobre la visualizaci�n de las fotos en las listas de usuarios. (55561 l�neas)
	Versi�n  7.72:   17/sep/2008	Las opciones para ver la orla y la lista de alumnos/profesores se juntan en una opci�n �nica. (55418 l�neas)
	Versi�n  7.71.1: 16/sep/2008	Mejoras est�ticas en la selecci�n de usuarios.
					Se disingue el g�nero en el plural del tipo de usuario (por ejemplo, si hay tres alumnas en una clase y ning�n alumno, aparecer� la palabra "Alumnas" en lugar de "Alumnos"). (55295 l�neas)
	Versi�n  7.71:   15/sep/2008	En la selecci�n de usuarios (para ver fichas, ver trabajos, escribir mensaje o ver estad�sticas), se puede elegir entre una orla y una lista. Sugerencia del profesor Miguel Gea Meg�as. (55245 l�neas)
	Versi�n  7.70.5: 15/sep/2008	Traducci�n del tipo de usuario a 6 idiomas teniendo en cuenta el g�nero y el n�mero.
					Cambios en la forma de enviar en la ficha el par�metro con el tipo de usuario. (55129 l�neas)
	Versi�n  7.70.4: 13/sep/2008	Cambios relacionados con el listado de usuarios para la selecci�n de algunos de ellos. (55089 l�neas)
	Versi�n  7.70.3: 13/sep/2008	Cambios relacionados con el listado de usuarios para la selecci�n de algunos de ellos. (55038 l�neas)
	Versi�n  7.70.2: 11/sep/2008	Cambios internos relacionados con el listado de usuarios para la selecci�n de algunos de ellos. (54852 l�neas)
	Versi�n  7.70.1: 09/sep/2008	Bajo el formulario de creaci�n de una nueva carpeta o env�o de un archivo se muestra el �rbol de archivos.
					Cambios relacionados con el lenguaje y el tema por defecto. (54841 l�neas)
	Versi�n  7.70:   09/sep/2008	El t�tulo de las zonas de descarga se presenta dentro de la ventana de listado de archivos. (54832 l�neas)
	Versi�n  7.69.11:01/sep/2008	Corregido peque�o problema con el tiempo entre un test y otro detectado por Salvador Manuel G�mez L�pez. (54757 l�neas)
	Versi�n  7.69.10:01/sep/2008	Cambio en el t�tulo de la p�gina.
					En los nombres de los foros se cambian las denominaciones largas de titulaciones y asignaturas por las abreviadas. (54751 l�neas)
	Versi�n  7.69.9: 29/jul/2008	Cambios internos relacionados con los temas de dise�o. El tema por defecto pasa a ser el azul, en lugar del cl�sico amarillo. (54748 l�neas)
	Versi�n  7.69.8: 28/jul/2008	Desglosado departamento en dos.
					Mejoras en la traducci�n, principalmente a alem�n, realizadas por Rafael Barranco-Droege.
					Comienza la edici�n de departamentos. (54888 l�neas)
	Versi�n  7.69.7: 17/jul/2008	Corregido peque�o bug en numeraci�n de p�ginas de una hebra. (54830 l�neas)
	Versi�n  7.69.6: 17/jul/2008	Mejoras en la traducci�n, principalmente a alem�n, realizadas por Rafael Barranco-Droege. (54829 l�neas)
	Versi�n  7.69.5: 15/jul/2008	Cambio interno en el directorio de logos. (54826 l�neas)
	Versi�n  7.69.4: 07/jul/2008	Nuevo logo de SWAD dise�ado por dixi (http://www.dixi.es/). (54824 l�neas)
	Versi�n  7.69.3: 02/jul/2008	Corregido bug en consulta a base de datos en el listado de mensajes enviados. (54823 l�neas)
	Versi�n  7.69.2: 01/jul/2008	Los mensajes recibidos y enviados se pueden filtrar tambi�n por su contenido. (54818 l�neas)
	Versi�n  7.69.1: 01/jul/2008	Modificaci�n en el filtrado de los mensajes por autores: ahora s�lo se buscan nombres y apellidos en el orden en el que han sido indicados por el usuario. (54748 l�neas)
	Versi�n  7.69:   01/jul/2008	Los mensajes recibidos se pueden filtrar por el autor y los enviados por el destinatario (disponible para todos los usuarios).
					Cambios internos en par�metros relacionados con los mensajes y los foros. (54747 l�neas)
	Versi�n  7.68.2: 30/jun/2008	Los mensajes recibidos se pueden filtrar por el autor (de momento s�lo disponible para el superusuario). (54626 l�neas)
	Versi�n  7.68.1: 26/jun/2008	Mejora en la edici�n de los temarios sugerida por Jos� Carlos Calvo Tudela. (54543 l�neas)
	Versi�n  7.68:   25/jun/2008	La opci�n de listado de centros y departamentos se desglosa en dos: centros por un lado y departamentos por otro.
					Se listan el n�mero de profesores en cada centro y en cada departamento. (54525 l�neas)
	Versi�n  7.67.8: 24/jun/2008	A�adida nueva opci�n en la pesta�a de titulaci�n para listar los centros y departamentos. (54381 l�neas)
	Versi�n  7.67.7: 24/jun/2008	Corregido problema con los administradores o superusuarios en el listado de usuarios conectados. (54377 l�neas)
	Versi�n  7.67.6: 24/jun/2008	Corregido error en la ordenaci�n de titulaciones administradas por un usuario. (54376 l�neas)
	Versi�n  7.67.5: 23/jun/2008	Cuando s�lo se muestran los primeros destinatarios de un mensaje, se ofrece la posibilidad de mostrar todos los detinatarios. (54377 l�neas)
	Versi�n  7.67.4: 23/jun/2008	Cuando hay muchos destinatarios en un mensaje, s�lo se muestran los primeros. Idea basada en una sugerencia de Ricardo Palma Dur�n. (54356 l�neas)
	Versi�n  7.67.3: 22/jun/2008	Los n�meros de las p�ginas de mensajes enviados/recibidos vistas m�s recientemente se almacenan en la tabla de sesiones en lugar de en la tabla de usuarios. (54305 l�neas)
	Versi�n  7.67.2: 22/jun/2008	Se almacenan los n�meros de las p�ginas de mensajes enviados/recibidos vistas m�s recientemente, de forma que se entre en ellas al pulsar en las opciones de ver mensajes enviados/recibidos. Sugerencia del profesor Sergio Alonso Burgos. (54278 l�neas)
	Versi�n  7.67.1: 21/jun/2008	Corregido peque�o problema con las preferencias al expirar las sesiones. (54214 l�neas)
	Versi�n  7.67:   21/jun/2008	Finaliza el desarrollo de una nueva opci�n para fijar las preferencias del usuario. (54221 l�neas)
	Versi�n  7.66:   20/jun/2008	Ya no se pasan las preferencias como par�metros de todos los formularios cuando no hay ning�n usuario identificado, sino que se toman de la tabla de preferencias asociadas a IP. (54180 l�neas)
	Versi�n  7.65.4: 20/jun/2008	En la tabla de las preferencias se almacena tambi�n el c�digo de usuario. As�, si un usuario cambia sus preferencias en un ordenador estando identificado, y m�s tarde las vuelve a cambiar en un segundo ordenador estando tambi�n identificado, cuando vuelva al primero no se recordar�n las preferencias m�s antiguas alteradas en el primer ordenador, sino las m�s recientes alteradas en el segundo ordenador. (54212 l�neas)
	Versi�n  7.65.3: 20/jun/2008	El m�dulo swad_theme pasa a llamarse swad_preference y contendr� las funciones relacionadas con las preferencias.
					Se almacena en una nueva tabla de la base de datos las preferencias de la �ltima vez que se accedi� desde la misma IP. De este modo, al entrar desde la misma IP sin identificar, SWAD recordar� los colores, el idioma y la configuraci�n de columnas. (54200 l�neas)
	Versi�n  7.65.2: 20/jun/2008	Comienza el desarrollo de una nueva opci�n para fijar las preferencias del usuario. (53838 l�neas)
	Versi�n  7.65.1: 20/jun/2008	El marco de los usuarios conectados siempre es el mismo, independientemente del tema de dise�o, para que el refresco de conectados no afecte a su aspecto. (53833 l�neas)
	Versi�n  7.65:   19/jun/2008	Nuevo m�dulo swad_preference relacionado con los temas de dise�o.
					Los usuarios pueden elegir el tema de dise�o, que queda almacenado en sus datos. (53832 l�neas)
	Versi�n  7.64.1: 18/jun/2008	Algunos cambios relacionados con el dise�o de los temas. (53608 l�neas)
	Versi�n  7.64:   17/jun/2008	Se permiten dos temas de dise�o: el cl�sico en negro y amarillo y uno nuevo en celeste y blanco. (53607 l�neas)
	Versi�n  7.63.3: 12/jun/2008	Corregido peque�o bug en listado de accesos recientes dentro de una asignatura. (53405 l�neas)
	Versi�n  7.63.2: 08/jun/2008	El logo de SWAD y el de la instituci�n aparecen en sendos rect�ngulos blancos. De este modo, ser� m�s f�cil cambiarlos por otros logos personalizados cuando se ofrezca como software libre. (53404 l�neas)
	Versi�n  7.63.1: 04/jun/2008	Peque�a modificaci�n de estilo en el listado de archivos de calificaciones.
					Corregido problema con el men� de mis asignaturas. (53352 l�neas)
	Versi�n  7.63:   04/jun/2008	Cambios est�ticos en la cabecera de la p�gina. Se separan los profesores de los alumnos en el listado de usuarios conectados. (53340 l�neas)
	Versi�n  7.62.1: 04/jun/2008	Se elimina el campo de lista de usuarios destino de la tabla de mensajes recibidos. Ello supone perder la lista de destinatarios en los mensajes m�s antiguos que el 29 de junio de 2006. A cambio, se espera una ganancia en velocidad en dicha tabla de la base de datos. (53301 l�neas)
	Versi�n  7.62:   03/jun/2008	Los accesos del �ltimo mes o m�s recientes se consultan en la tabla de accesos recientes. (53302 l�neas)
	Versi�n  7.61.4: 03/jun/2008	La tabla del registro de accesos se divide en dos: una con el registro hist�rico de accesos y otra con el registro de los accesos recientes. Cada acceso se registra en ambas tablas. (53208 l�neas)
	Versi�n  7.61.3: 31/may/2008	En "Estad�sticas>Uso de SWAD" se muestra el n�mero de usuarios que han elegido cada uno de los cuatro posibles dise�os de Las columnas laterales izquierda y derecha. (53165 l�neas)
	Versi�n  7.61.2: 28/may/2008	Las columnas laterales izquierda y derecha se ocultan o muestran de manera independiente. (53099 l�neas)
	Versi�n  7.61.1: 28/may/2008	Cambios est�ticos en la cabecera de la p�gina. Se muestra la fotograf�a del usuario identificado. (53021 l�neas)
	Versi�n  7.61:   28/may/2008	Termina la implementaci�n de la posibilidad de expandir la zona central hacia los lados. (53022 l�neas)
	Versi�n  7.60.5: 28/may/2008	Comienza la implementaci�n de la posibilidad de expandir la zona central hacia los lados. (52880 l�neas)
	Versi�n  7.60.4: 26/may/2008	Los nombres de las fotos de los usuarios se cifran con el algoritmo SHA-256 en lugar de hacerlo con el algoritmo AES. (52810 l�neas)
	Versi�n  7.60.3: 26/may/2008	Los identificadores de sesi�n se cifran con el algoritmo SHA-256 en lugar de hacerlo con el algoritmo AES. (52818 l�neas)
	Versi�n  7.60.2: 21/may/2008	M�s cambios internos relacionados con el env�o de la contrase�a. (52824 l�neas)
	Versi�n  7.60.1: 21/may/2008	Cambios internos relacionados con el env�o de la contrase�a. (52812 l�neas)
	Versi�n  7.60:   21/may/2008	Cuando falla la autenticaci�n de un usuario, no se indica en los mensajes de error si ha fallado la identificaci�n o la contrase�a.
					Cambios en los formularios para el env�o de una nueva contrase�a por correo electr�nico. (52823 l�neas)
	Versi�n  7.64.1: 18/jun/2008	Algunos cambios relacionados con el dise�o de los temas. (53608 l�neas)
	Versi�n  7.64:   17/jun/2008	Se permiten dos temas de dise�o: el cl�sico en negro y amarillo y uno nuevo en celeste y blanco. (53607 l�neas)
	Versi�n  7.63.3: 12/jun/2008	Corregido peque�o bug en listado de accesos recientes dentro de una asignatura. (53405 l�neas)
	Versi�n  7.63.2: 08/jun/2008	El logo de SWAD y el de la instituci�n aparecen en sendos rect�ngulos blancos. De este modo, ser� m�s f�cil cambiarlos por otros logos personalizados cuando se ofrezca como software libre. (53404 l�neas)
	Versi�n  7.63.1: 04/jun/2008	Peque�a modificaci�n de estilo en el listado de archivos de calificaciones.
					Corregido problema con el men� de mis asignaturas. (53352 l�neas)
	Versi�n  7.63:   04/jun/2008	Cambios est�ticos en la cabecera de la p�gina. Se separan los profesores de los alumnos en el listado de usuarios conectados. (53340 l�neas)
	Versi�n  7.62.1: 04/jun/2008	Se elimina el campo de lista de usuarios destino de la tabla de mensajes recibidos. Ello supone perder la lista de destinatarios en los mensajes m�s antiguos que el 29 de junio de 2006. A cambio, se espera una ganancia en velocidad en dicha tabla de la base de datos. (53301 l�neas)
	Versi�n  7.62:   03/jun/2008	Los accesos del �ltimo mes o m�s recientes se consultan en la tabla de accesos recientes. (53302 l�neas)
	Versi�n  7.61.4: 03/jun/2008	La tabla del registro de accesos se divide en dos: una con el registro hist�rico de accesos y otra con el registro de los accesos recientes. Cada acceso se registra en ambas tablas. (53208 l�neas)
	Versi�n  7.61.3: 31/may/2008	En "Estad�sticas>Uso de SWAD" se muestra el n�mero de usuarios que han elegido cada uno de los cuatro posibles dise�os de Las columnas laterales izquierda y derecha. (53165 l�neas)
	Versi�n  7.61.2: 28/may/2008	Las columnas laterales izquierda y derecha se ocultan o muestran de manera independiente. (53099 l�neas)
	Versi�n  7.61.1: 28/may/2008	Cambios est�ticos en la cabecera de la p�gina. Se muestra la fotograf�a del usuario identificado. (53021 l�neas)
	Versi�n  7.61:   28/may/2008	Termina la implementaci�n de la posibilidad de expandir la zona central hacia los lados. (53022 l�neas)
	Versi�n  7.60.5: 28/may/2008	Comienza la implementaci�n de la posibilidad de expandir la zona central hacia los lados. (52880 l�neas)
	Versi�n  7.60.4: 26/may/2008	Los nombres de las fotos de los usuarios se cifran con el algoritmo SHA-256 en lugar de hacerlo con el algoritmo AES. (52810 l�neas)
	Versi�n  7.60.3: 26/may/2008	Los identificadores de sesi�n se cifran con el algoritmo SHA-256 en lugar de hacerlo con el algoritmo AES. (52818 l�neas)
	Versi�n  7.60.2: 21/may/2008	M�s cambios internos relacionados con el env�o de la contrase�a. (52824 l�neas)
	Versi�n  7.60.1: 21/may/2008	Cambios internos relacionados con el env�o de la contrase�a. (52812 l�neas)
	Versi�n  7.60:   21/may/2008	Cuando falla la autenticaci�n de un usuario, no se indica en los mensajes de error si ha fallado la identificaci�n o la contrase�a.
					Cambios en los formularios para el env�o de una nueva contrase�a por correo electr�nico. (52823 l�neas)
	Versi�n  7.59.4: 20/may/2008	Se obliga de nuevo a cambiar las contrase�as poco seguras.
					Se rechazan las contrase�as iguales a m�s de n contrase�as de otros usuarios. (52736 l�neas)
	Versi�n  7.59.3: 19/may/2008	Las contrase�as planas pasan de tener un l�mite de 16 caracteres a tener una longitud m�xima de 256 caracteres. (52654 l�neas)
	Versi�n  7.59.2: 19/may/2008	Eliminadas contrase�as antiguas de la base de datos. (52658 l�neas)
	Versi�n  7.59.1: 19/may/2008	Eliminado el sistema de contrase�as antiguo. (52658 l�neas)
	Versi�n  7.59:   19/may/2008	Las contrase�as se almacenan usando el algoritmo SHA-512. Se ha usado la implementaci�n de Olivier Gay disponible en http://www.ouah.org/ogay/sha2/. (52782 l�neas)
	Versi�n  7.58:   16/may/2008	Eliminado el mecanismo de respuesta secreta para recordar la contrase�a.
					En ning�n momento se recupera la contrase�a plana de la base de datos. Todas las comparaciones se hacen entre contrase�as cifradas. (52572 l�neas)
	Versi�n  7.57:   15/may/2008	En lugar de enviar por correo la contrase�a actual, se env�a una nueva contrase�a aleatoria, que s�lo se activa si se usa. (52833 l�neas)
	Versi�n  7.56:   08/may/2008	En la tabla de log se almacenan las IP desde las que acceden los usuarios. (52699 l�neas)
	Versi�n  7.55.8: 07/may/2008	En la paginaci�n de mensajes se muestran dos nuevos enlaces, uno a la p�gina intermedia entre la primera y la actual, y otro a la p�gina intermedia entre la actual y la �ltima. (52671 l�neas)
	Versi�n  7.55.7: 06/may/2008	Cambios en la presentaci�n de mensajes enviados y recibidos, y en la paginaci�n de mensajes. (52611 l�neas)
	Versi�n  7.55.6: 06/may/2008	Cambios est�ticos en la presentaci�n de mensajes enviados y recibidos. (52615 l�neas)
	Versi�n  7.55.5: 06/may/2008	En los mensajes enviados y recibidos, se muestra junto a cada autor y destinatario, si �ste ha le�do o eliminado el mensaje. (52612 l�neas)
	Versi�n  7.55.4: 05/may/2008	La longitud m�nima de la contrase�a aumenta a 10 caracteres.
					Cuando la contrase�a no es suficientemente segura, se obliga al usuario a cambiarla. (52581 l�neas)
	Versi�n  7.55.3: 05/may/2008	En los mensajes enviados o recibidos, s�lo se consulta el texto del mensaje cuando est� expandido. (52528 l�neas)
	Versi�n  7.55.2: 05/may/2008	Los mensajes enviados se muestran en el nuevo formato (igual que los recibidos). (52486 l�neas)
	Versi�n  7.55.1: 04/may/2008	Se muestra el campo De: en el mensaje recibido expandido. (52497 l�neas)
	Versi�n  7.55:   04/may/2008	Cambios en el almacenamiento de mensajes enviados y recibidos. La tabla msg_sent ya no se usa. (52501 l�neas)
	Versi�n  7.54.2: 03/may/2008	Cambios en el formato de los mensajes recibidos. (52494 l�neas)
	Versi�n  7.54.1: 03/may/2008	Separada en dos la funci�n para mostrar un mensaje enviado o recibido. (52423 l�neas)
	Versi�n  7.54:   03/may/2008	S�lo se muestran las cabeceras de los mensajes recibidos. Ahora hay que expandirlos para ver su contenido. (52383 l�neas)
	Versi�n  7.53.1: 02/may/2008	Separadas las funciones empleadas para mostrar los mensajes entre usuarios de las usadas para mostrar los mensajes de los foros. (52285 l�neas)
	Versi�n  7.53:   02/may/2008	Eliminada la tabla msg_new. El n�mero de mensajes nuevos pasa a la tabla usr_data. (52163 l�neas)
	Versi�n  7.52:   02/may/2008	M�s cambios internos relacionados con los ID de los usuarios.
					Las listas de usuarios seleccionados ya no contienen ID sino c�digos de usuario. (52270 l�neas)

	--------------------------------

	Versi�n 0.9:    21/sep/1999	 Comienzo de la versi�n inicial.
	--------------------------------
	Versi�n 1.0:    1999-2000	Primera versi�n, que s�lo inclu�a las fichas,las calificaciones y la descarga de documentos.
	Versi�n 1.1:    2000-2001	Inclu�a el env�o autom�tico de la foto.
	--------------------------------
	Versi�n  2.0:    14/mar/2002	Segunda versi�n, mandada a Jenui 2002, que inclu�a foros de discusi�n y test de autoevaluaci�n. (3233 l�neas)
	Versi�n  2.1:    10/abr/2002	Listado jer�rquico de directorios de descarga.
					Foto del usuario identificado en la pantalla.
					Foto de los usuarios de los foros.
					Evaluaci�n del test.
	Versi�n  2.2:    27/may/2002	Corregido bug en los foros.
	Versi�n  2.3:    04/jul/2002	Corregido bug en presentaci�n de calificaciones.
					Corregido bug en recepci�n de fotograf�a.
					Ampliado tama�o de fotograf�a de 20 KiB a 100 KiB.
					Corregido bug en recepci�n de par�metros en modo datos.
	Versi�n  2.4:    23/sep/2002	Corregido peque�o bug al presentar mensaje de error en ver calificaciones.
	Versi�n  2.5:    28/oct/2002	Listado de fichas de estudiantes.
	Versi�n  2.6:    29/oct/2002	Nuevas opciones in menu para profesores (no todas implementadas a�n).
					Vista de acccesos de usuarios.
	Versi�n  2.7:    30/oct/2002	Puesta a cero del archivo de accesos de usuarios.
					El sistema recuerda la �ltima acci�n realizada por cada usuario.
	Versi�n  2.7.1:  31/oct/2002	Cambio interno: automatizaci�n de la forma de presentar el men�.
	Versi�n  2.8:    31/oct/2002	Se usa un nuevo archivo-tabla: usuarios en esta asignatura.
	Versi�n  2.9:    05/nov/2002	Alta / baja de usuario en la asignatura presente.
	Versi�n  2.10:   06/nov/2002	Borrado de la ficha de un usuario.
	--------------------------------
	Versi�n  3.0:    06/nov/2002	Corregido peque�o bug: un usuario que no estaba en la lista de esta asignatura pod�a acceder, ahora no.
					Versi�n con la que se confeccion� el p�ster ICTE 2002.
	Versi�n  3.1:    10/ene/2003	He protegido mediante .htaccess y .htpasswd el directorio de descarga.
					Esta versi�n evita que se listen los archivos .*
	Versi�n  3.2:    14/ene/2003	Lee un archivo HTML para cada asignatura donde aparece el nombre de usuario y el password para descarga de archivos
	Versi�n  3.3:    22/abr/2003	Definici�n de 3 tipos de usuario: administrador, profesor, estudiante, invitado y distintos privilegios para cada uno. (5480 l�neas)
	Versi�n  3.3.1:  30/abr/2003	Aumentado el tama�o del cuerpo de los mensajes del foro.
	Versi�n  3.4:    30/abr/2003	Corregidos tama�os de cadenas en algunas funciones.
	Versi�n  3.5:    05/may/2003	Cambio del sitio donde aparece el nombre del alumno.
					Cambio del modo de seleccionar una opci�n del men�. Ahora con un bot�n directo por opci�n. (5646 l�neas)
	Versi�n  3.6:    06/may/2003	Cambio del modo de seleccionar una opci�n del men�. Ahora con un enlace directo por opci�n.
					Cambio en la forma de presentar la ayuda de las opciones del men�. (5647 l�neas)
	--------------------------------
	Versi�n  4.0:    06/may/2003	El programa presenta el tabl�n de anuncios y el men� con 9 opciones est�ticas:
					profesor, Tutor�as, Horario, Teor�a, Pr�cticas, Bibliograf�a, Evaluaci�n, FAQ y Enlaces. (6038 l�neas)
	Versi�n  4.0.1:  07/may/2003	Peque�as mejoras est�ticas.
	Versi�n  4.0.2:  07/may/2003	Peque�as mejoras est�ticas.
	Versi�n  4.0.3:  07/may/2003	Corregido peque�o bug.
	Versi�n  4.0.4:  08/may/2003	Ajustes para que se visualice bien en Netscape. Correcto con Microsoft Internet Explorer 6.0 y Netscape 7.0
	Versi�n  4.0.5:  08/may/2003	Acceso directo a los mensajes de cada tema del foro pinchando sobre el tema.
	Versi�n  4.1:    08/may/2003	Paso de par�metros a trav�s de un archivo de configuraci�n.
	Versi�n  4.2:    12/may/2003	Usuarios "demostraci�n". Versiones de demostraci�n.
					Recepci�n de fotograf�a a trav�s de un archivo temporal.
	Versi�n  4.2.1:  13/may/2003	Ocultaci�n con asteriscos de los datos presentados en las versiones de demostraci�n.
					Versi�n con la que se pidi� el proyecto de innovaci�n docente "Plataforma SWAD (Sistema Web a Apoyo a la Docencia)".
	Versi�n  4.3:    20/may/2003	Env�o de correo electr�nico a todos los alumnos de una asignatura.
	Versi�n  4.3.1:  01/oct/2003	Peque�o cambio en el sitio donde aparece el men� principal.
	Versi�n  4.3.2:  15/oct/2003	Todos los iconos se cargan ahora de "/~acanas/swad/iconos/" en lugar de "/iconos/".
	Versi�n  4.3.3:  22/oct/2003	Modificaci�n en la presentaci�n gr�fica de los avisos (ahora simulando notas amarillas).
	Versi�n  4.4:    22/oct/2003	Env�o y presentaci�n autom�tica de avisos.
	Versi�n  4.4.1:  23/oct/2003	Aparici�n de los avisos en orden inverso a la fecha de env�o.
	Versi�n  4.5:    23/oct/2003	Eliminaci�n selectiva de avisos. (6878 l�neas)
	Versi�n  4.6:    28/oct/2003	Cambio importante en el aspecto gr�fico de la plataforma. Men�s separados en pesta�as.
	Versi�n  4.6.1:  30/oct/2003	Peque�os cambios est�ticos.
	Versi�n  4.6.2:  30/oct/2003	Llamada directa a la funci�n de cada acci�n. Cambios en los tipos de letra de algunas opciones.
	Versi�n  4.6.3:  30/oct/2003	Se recuerda para cada usuario la �ltima pesta�a abierta en la sesi�n anterior.
	Versi�n  4.7:    05/nov/2003	Incorporaci�n de las opciones del men� horizontal y otras relacionadas con la asignatura a la pesta�a de la asignatura.
	Versi�n  4.7.1:  05/nov/2003	Se muestra el t�tulo de la opci�n elegida.
	Versi�n  4.8:    05/nov/2003	Env�o de un archivo HTML con la tabla de calificaciones. (7223 l�neas)
	Versi�n  4.8.1:  06/nov/2003	Mejoras internas en la recepci�n de foto y calificaciones.
	Versi�n  4.8.2:  07/nov/2003	Env�o del n�mero de filas de cabecera y de pie de la tabla de calificaciones.
	Versi�n  4.8.3:  10/nov/2003	Cambio en la presentaci�n. No aparece arriba el nombre de la asignatura.
	Versi�n  4.8.4:  11/nov/2003	Nuevo men� izquierdo con acceso a la Universidad, al Centro, a los departamentos, etc.
	Versi�n  4.8.5:  12/nov/2003	Si no se pasan par�metros, directamente salta a la p�gina de ayuda de la plataforma.
	Versi�n  4.9:    12/nov/2003	Aparece un calendario con el mes actual.
	Versi�n  4.9.1:  13/nov/2003	Corregido bug en la grabaci�n de la tabla de �ltimo acceso.
					Mejora en la presentaci�n del registro de accesos.
	Versi�n  4.9.2:  13/nov/2003	Reducci�n del tama�o de los avisos (notas amarillas). Versi�n correspondiente a las III Jornadas Andaluzas de Software Libre.
	Versi�n  4.9.3:  14/nov/2003	Cambio del logo y del nombre de la plataforma (Sistema de Servicios Web de Apoyo al Aprendizaje y a la Docencia a Distancia)
	Versi�n  4.9.4:  17/nov/2003	Peque�os cambios est�ticos.
	Versi�n  4.10:   18/nov/2003	A�adido men� de asignaturas de Ingeniero en Inform�tica.
	Versi�n  4.10.1: 19/nov/2003	A�adidos men�s de asignaturas de ITIG e ITIS.
	Versi�n  4.10.2: 20/nov/2003	A�adido men� de asignaturas optativas de las ITI. Se puede navegar entre asignaturas sin perder el ID y la contrase�a.
					Obtenci�n autom�tica de los nombres de archivos a partir del path de la asignatura, en lugar de leerlos del archivo de configuraci�n. (8218 l�neas)
	Versi�n  4.10.3: 21/nov/2003	Corregido peque�os bugs en la presentaci�n.
	Versi�n  4.10.4: 24/nov/2003	Mejoras en la presentaci�n del men� de asignaturas. (8221 l�neas)
	Versi�n  4.10.5: 24/nov/2003	Ligeros cambios est�ticos.
	Versi�n  4.11:   25/nov/2003	Gesti�n de descarga de archivos muy mejorada.
					Comienzo de la administraci�n de descarga de archivos.
	Versi�n  4.12:   25/nov/2003	Borrado de archivos de descarga.
	Versi�n  4.12.1: 26/nov/2003	Corregidos algunos bugs. Presentaci�n de archivos para descarga mejorada.
	Versi�n  4.13:   26/nov/2003	Env�o de archivos para descarga.
	Versi�n  4.13.1: 27/nov/2003	Borrado recursivo de carpetas de descarga no vac�as. A�adido reloj en Javascript.
	Versi�n  4.13.2: 27/nov/2003	A�adida carpeta ra�z para poder crear carpetas o archivos directamente en la carpeta de descargas.
	Versi�n  4.13.3: 28/nov/2003	Peque�os cambios internos. Corregido peque�o bug en la presentaci�n de las notas amarillas.
	Versi�n  4.13.4: 02/dic/2003	Ligeros cambios est�ticos, principalmente en la orla.
	Versi�n  4.13.5: 03/dic/2003	Presentaci�n del listado de accesos en orden inverso (comenzando por el �ltimo acceso).
	Versi�n  4.14:   03/dic/2003	Presentaci�n del listado de accesos en varias p�ginas.
	Versi�n  4.14.1: 04/dic/2003	Mejoras en el listado de accesos. (8910 l�neas)
	Versi�n  4.14.2: 04/dic/2003	El sistema recuerda el n�mero de columnas en la orla, el n�mero de columnas en el listado de fichas y el n�mero de filas por p�gina en el listado de accesos.
	Versi�n  4.14.3: 05/dic/2003	En el mes se muestran en rojo los d�as festivos y en un color especial los d�as no lectivos.
					En el futuro las listas de dias festivos y no lectivos deber�an poder introducirse a trav�s de un formulario.
	Versi�n  4.15:   05/dic/2003	Lista de accesos en los �ltimos minutos (usuarios conectados). Se listan distintos accesos del mismo usuario. (9109 l�neas)
	Versi�n  4.16:   09/dic/2003	Lista de usuarios conectados (los que han accedido en los �ltimos minutos y no han salido). (9318 l�neas)
	Versi�n  4.16.1: 10/dic/2003	Los administradores no dejar�n huella en el registro.
	Versi�n  4.16.2: 10/dic/2003	Algunas mejoras gr�ficas en los anuncios. (9324 l�neas)
	Versi�n  4.16.3: 12/dic/2003	En los mensajes del foro se respetan los espacios en blanco originales.
	Versi�n  4.17:   16/dic/2003	Integrado el sistema de mejora de fotos realizado en el proyecto fin de carrera de Alberto Rodrigo G�miz y Jes�s �lvarez Mart�n. (9379 l�neas)
	Versi�n  4.17.1: 16/dic/2003	En los mensajes del foro se respetan los tabuladores originales.
	Versi�n  4.17.2: 17/dic/2003	Solucionado problema en los mensajes del foro. (9419 l�neas)
	Versi�n  4.17.3: 17/dic/2003	Modificaci�n en la presentaci�n de usuarios conectados: ahora se muestra la foto. (9463 l�neas)
	Versi�n  4.17.4: 22/dic/2003	Al seleccionar un SELECT, se env�a el formulario. (9467 l�neas)
	Versi�n  4.18:   19/ene/2004	En los temas de los foros aparece la fecha del env�o m�s reciente. (9616 l�neas)
	Versi�n  4.19:   20/ene/2004	Los temas de los foros se ordenan por defecto seg�n la fecha del env�o m�s reciente, y pueden ordenarse seg�n la fecha del primer env�o. (9700 l�neas)
	Versi�n  4.19.1: 22/ene/2004	Mejoras internas en la presentaci�n de los temas del foro.
	Versi�n  4.19.2: 22/ene/2004	El formato de fecha almacenada en los archivos pasa a ser: segundos desde 1970. (9751 l�neas)
					Por compatibilidad a�n se leen fechas en formato dia/mes/a�o hora:minuto. Este formato se abandonar� en el futuro.
	Versi�n  4.19.3: 28/ene/2004	Corregido bug en mensaje de error.
	Versi�n  4.20:   04/feb/2004	Generaci�n autom�tica del horario de una asignatura a partir de los datos de un archivo. (9967 l�neas)
	Versi�n  4.21:   06/feb/2004	Modificaci�n del horario de una asignatura por parte de un profesor. (10158 l�neas)
	Versi�n  4.22:   09/feb/2004	Env�o de enlaces a p�ginas de teor�a, pr�cticas, bibliograf�a, evaluaci�n, profesores, tutor�as, FAQ y enlaces.
					Corregido peque�o bug en descarga de archivo. (10382 l�neas)
	Versi�n  4.22.1: 10/feb/2004	Cambios en los men�s. Versi�n enviada a Jenui 2004.
	Versi�n  4.22.2: 12/feb/2004	Cambios en el env�o de enlaces a p�ginas de teor�a, pr�cticas, etc. (10368 l�neas)
	Versi�n  4.23:   12/feb/2004	Nueva organizaci�n de las pesta�as. (10297 l�neas)
	Versi�n  4.24:   13/feb/2004	Recepci�n de archivos HTML y ZIP con teor�a, pr�cticas, etc. (10384 l�neas)
	Versi�n  4.24.1: 16/feb/2004	Corregido peque�o bug en teor�a, pr�cticas, etc. (10386 l�neas)
	Versi�n  4.24.2: 17/feb/2004	Peque�o cambio en los men�s de administraci�n de asignatura y usuarios.
					A�adido campo "Grupo" a los horarios. (10447 l�neas)
	Versi�n  4.25:   17/feb/2004	Mejorada la gesti�n de la tabla de usuarios de una asignatura. (10504 l�neas)
	Versi�n  4.26:   18/feb/2004	En la orla, la lista de usuarios, la lista de fichas y el env�o de correo, los usuarios se ordenan alfab�ticamente independientemente del orden en el que est�n almacenados en la lista de usuarios de la asignatura. (10566 l�neas)
	Versi�n  4.27:   19/feb/2004	Nueva opci�n para a�adir los usuarios de una lista con ID a la asignatura. (10806 l�neas)
	Versi�n  4.27.1: 24/feb/2004	Peque�os cambios internos. (10921 l�neas)
	Versi�n  4.27.2: 24/feb/2004	Corregido bug en el env�o de carpetas.
	Versi�n  4.28:   25/feb/2004	Reorganizaci�n de las pesta�as y men�s. (10846 l�neas)
	Versi�n  4.28.1: 26/feb/2004	Cambio en el nombre del archivo de usuarios de una asignatura.
	Versi�n  4.28.2: 01/mar/2004	Corregidos peque�os bugs.
					Se puede ejecutar el cgi sin par�metros (sin una asignatura concreta). (10868 l�neas)
	Versi�n  4.28.3: 01/mar/2004	Cabecera de pesta�as con bordes redondeados. (10876 l�neas)
	Versi�n  4.28.4: 02/mar/2004	Cambio en los bordes de las pesta�as. (10873 l�neas)
	Versi�n  4.28.5: 04/mar/2004	Se crea el directorio de avisos si no existe.
					Cambio est�tico: fondo superior pasa a degradado.
	Versi�n  4.29:   05/mar/2004	Uso de sesiones, en lugar de propagar el ID y el password. (11011 l�neas)
	Versi�n  4.29.1: 08/mar/2004	Corregido bug en las sesiones. Algunas mejoras internas. (10991 l�neas)
	Versi�n  4.29.2: 11/mar/2004	Corregido bug en inicio de sesi�n. (10999 l�neas)
	Versi�n  4.30:   12/mar/2004	Implementando cambio importante en la forma de almacenar los datos de los usuarios. Ahora cada usuario tendr� un directorio.
	Versi�n  4.30.1: 13/mar/2004	Contin�a implementaci�n de nueva forma de almacenar usuarios. No finalizada. (11106 l�neas)
	Versi�n  4.31:   15/mar/2004	Implementaci�n del maletin de almacenamiento personal. Falta comprobar cuota de disco. (11352 l�neas)
	Versi�n  4.31.1: 16/mar/2004	Cuotas en el malet�n y otras mejoras internas. (11248 l�neas)
	Versi�n  4.31.2: 17/mar/2004	Terminando de implementar cambios relacionados con la nueva forma de almacenar las fichas de los usuarios. (11334 l�neas)
	Versi�n  4.31.3: 17/mar/2004	Terminando de implementar cambios relacionados con la nueva forma de almacenar las fichas de los usuarios. (11335 l�neas)
	Versi�n  4.31.4: 17/mar/2004	Terminada nueva forma de almacenar las fichas de los usuarios. Eliminada opci�n de cambiar fichas al sistema nuevo. (11264 l�neas)
	Versi�n  4.31.5: 18/mar/2004	Se eliminan algunos espacios entre etiquetas HTML para que la p�gina se muestre correctamente en Explorer 5. (11280 l�neas)
	Versi�n  4.32:   19/mar/2004	Mejora en la presentaci�n de la lista de usuarios. Diversas mejoras internas. (11203 l�neas)
	Versi�n  4.32.1: 23/mar/2004	Se eliminan algunos espacios entre etiquetas HTML para que la p�gina se muestre correctamente en Explorer 5. (11203 l�neas)
	Versi�n  4.33:   25/mar/2004	Env�o de mensaje instant�neo a usuarios conectados.
	Versi�n  4.33.1: 26/mar/2004	Recepci�n de mensajes y almacenamiento en carpeta de mensajes recibidos. (11459 l�neas)
	Versi�n  4.33.2: 02/abr/2004	Corregido el error de seguridad en el malet�n informado por el estudiante Luis Miguel Merino Bonilla. (11537 l�neas)
	Versi�n  4.34:   08/abr/2004	Env�o de mensaje a cualquier usuario. (11711 l�neas)
	Versi�n  4.34.1: 08/abr/2004	Posibilidad de seleccionar todos los usuarios como destinatarios de un mensaje. (11738 l�neas)
	Versi�n  4.35:   08/abr/2004	Borrado selectivo de mensajes recibidos de otros usuarios. (11896 l�neas)
	Versi�n  4.36:   11/abr/2004	Cambios importantes en la implementaci�n interna del foro. (11903 l�neas)
	Versi�n  4.36.1: 12/abr/2004	Resaltados los mensajes sin leer. Cambios en la presentaci�n. (11921 l�neas)
	Versi�n  4.36.2: 13/abr/2004	Peque�os cambios en la presentaci�n. (11905 l�neas)
	Versi�n  4.36.3: 13/abr/2004	Mejoras internas en el env�o de formularios. (11896 l�neas)
	Versi�n  4.37:   14/abr/2004	Almacenamiento de mensajes enviados. (11980 l�neas)
	Versi�n  4.37.1: 14/abr/2004	Borrado de mensajes enviados. Cambio en redacci�n de mensajes a un usuario conectado. (11979 l�neas)
	Versi�n  4.37.2: 15/abr/2004	Cambio en presentaci�n del env�o de correo a todos los estudiantes. (11978 l�neas)
	Versi�n  4.37.3: 18/abr/2004	Corregido peque�o bug y eliminaci�n de la presentaci�n de la constrase�a en la lista de usuarios. (11983 l�neas)
	Versi�n  4.37.4: 20/abr/2004	Mejoras en el cierre de sesiones. Aparece tiempo de generaci�n de la p�gina. (12031 l�neas)
	Versi�n  4.37.5: 20/abr/2004	Mejoras internas y correcciones de bugs relacionadas con en el cierre de sesiones. (12027 l�neas)
	Versi�n  4.38:   22/abr/2004	Respuesta a mensaje recibido. (12058 l�neas)
	Versi�n  4.38.1: 24/abr/2004	Corregido peque�o bug en la creaci�n del fichero con el n�mero de mensajes nuevos. (12060 l�neas)
	Versi�n  4.38.2: 25/abr/2004	Cambio en el listado de usuarios conectados. Ahora aparece tiempo de inactividad. (12074 l�neas)
	Versi�n  4.38.3: 25/abr/2004	El registro de usuarios conectados pasa a ser com�n a todas las asignaturas. (12090 l�neas)
	Versi�n  4.39:   25/abr/2004	Comienza la implementaci�n de la edici�n del temario de teor�a, almacenado en un fichero XML. (12233 l�neas)
	Versi�n  4.39.1: 27/abr/2004	Algunos cambios est�ticos. (12161 l�neas)
	Versi�n  4.39.2: 27/abr/2004	Contin�a la implementaci�n de la edici�n del temario de teor�a. (12260 l�neas)
	Versi�n  4.39.3: 30/abr/2004	Contin�a la implementaci�n de la edici�n del temario de teor�a. (12303 l�neas)
	Versi�n  4.39.4: 03/may/2004	Contin�a la implementaci�n de la edici�n del temario de teor�a. (12355 l�neas)
	Versi�n  4.39.5: 03/may/2004	Contin�a la implementaci�n de la edici�n del temario de teor�a. (12552 l�neas)
	Versi�n  4.39.6: 04/may/2004	Termina la implementaci�n de la edici�n del temario de teor�a. (12814 l�neas)
	Versi�n  4.39.7: 04/may/2004	Mejoras para aumentar la rapidez de carga. (12807 l�neas)
	Versi�n  4.39.8: 05/may/2004	Extensi�n de edici�n del temario de teor�a al de pr�cticas. (12835 l�neas)
	Versi�n  4.39.9: 06/may/2004	Mejoras en la inserci�n de nuevos items en un temario. (12870 l�neas)
	Versi�n  4.39.10:06/may/2004	Mejoras en la presentaci�n de un temario. (12863 l�neas)
	Versi�n  4.39.11:07/may/2004	Las flechas de subir y bajar items del temario permiten ahora mover el sub�rbol. (12932 l�neas)
	Versi�n  4.39.12:07/may/2004	Desactivaci�n de las flechas de borrar, subir, y bajar items del temario que no tengan sentido. (13004 l�neas)
	Versi�n  4.39.13:09/may/2004	Peque�as optimizaciones en la edici�n de los temarios. (12993 l�neas)
	Versi�n  4.40:   10/may/2004	Cambio en la especificaci�n de titulaci�n, curso, cuatrimestre y asignatura. Se utilizar� un �nico c�digo. (13011 l�neas)
	Versi�n  4.40.1: 13/may/2004	Cambios en la edici�n del temario. (13005 l�neas)
	Versi�n  4.41:   26/may/2004	Se muestra la orla lista para imprimir. (13057 l�neas)
	Versi�n  4.41.1: 27/may/2004	Al mostrar nombres o apellidos de alumnos, aparece la primera letra en may�scula y el resto en min�sculas. (13109 l�neas)
	Versi�n  4.42:   02/jun/2004	Cambio en el lugar donde se introduce el ID y la contrase�a. (13098 l�neas)
	Versi�n  4.42.1: 02/jun/2004	Se muestran pesta�as aunque no haya un usuario autenticado. (13120 l�neas)
	Versi�n  4.42.2: 03/jun/2004	Diversos cambios internos relacionados con los usuarios conectados. (13181 l�neas)
	Versi�n  4.42.3: 07/jun/2004	Aparece un icono en cada pesta�a. (13185 l�neas)
	Versi�n  4.42.4: 22/jun/2004	Comienza la implementaci�n del chat. (13236 l�neas)
	Versi�n  4.42.5: 29/jun/2004	Aparece el n�mero de usuarios conectados en toda la plataforma y en la asignatura seleccionada. (13232 l�neas)
	Versi�n  4.43:   02/jul/2004	A�adido el plan de estudios de Ingeniero Electr�nico. (13373 l�neas)
	Versi�n  4.43.1: 07/jul/2004	Al cambiar la contrase�a, se pide la original. (13403 l�neas)
	Versi�n  4.44:   21/jul/2004	Se incluye el algoritmo Rijndael. (13450 l�neas)
					Compilar con: gcc swad.c rijndael.c -o swad -Wall
	Versi�n  4.44.1: 22/jul/2004	Nombres de las fotograf�as cifrados (enlaces simb�licos a un directorio privado). (13491 l�neas)
	Versi�n  4.44.2: 22/jul/2004	Se borran todas las fotos temporales creadas hace m�s de cierto tiempo. (13487 l�neas)
	Versi�n  4.44.3: 23/jul/2004	Las contrase�as se almacenan cifradas a partir de esta versi�n. (13553 l�neas)
	Versi�n  4.44.4: 27/jul/2004	Mejora en el almacenamiento interno de los enlaces de la izquierda (Universidad, etc.). (13425 l�neas)
	Versi�n  4.45:   27/jul/2004	Nuevas fichas para profesores. (13464 l�neas)
	Versi�n  4.45.1: 29/jul/2004	Cambio en las fichas para profesores. El departamento y el centro se eligen de una lista. (13525 l�neas)
	Versi�n  4.45.2: 29/jul/2004	Se presentan los profesores de una asignatura a partir de los datos de sus fichas. (13532 l�neas)
	Versi�n  4.45.3: 29/jul/2004	Mejoras en la presentaci�n de las fichas. (13532 l�neas)
	Versi�n  4.45.4: 30/jul/2004	Opci�n de mostrar las fichas listas para imprimir. (13603 l�neas)
	Versi�n  4.45.5: 30/jul/2004	Mejoras en la presentaci�n de las fichas.
					A�adidos todos los centros y algunos departamentos (13636 l�neas)
	Versi�n  4.45.6: 31/jul/2004	A�adidos todos los departamentos (13707 l�neas)
	Versi�n  4.45.7: 02/sep/2004	Corregido peque�o bug en presentaci�n del t�tulo del calendario (13708 l�neas)
	Versi�n  4.45.8: 03/sep/2004	A�adidos iconos para ver y crear convocatorias de examen (opciones a�n no implementadas) (13715 l�neas)
	Versi�n  4.45.9: 03/sep/2004	En el men� de la izquierda aparecen todos los departamentos (13713 l�neas)
	Versi�n  4.45.10:10/sep/2004	En el men� de la izquierda aparecen todos los centros. Corregido peque�o bug en listado descargas y malet�n (13718 l�neas)
	Versi�n  4.46:   10/sep/2004	Posibilidad de renombrar las carpetas de descarga. (13852 l�neas)
	Versi�n  4.46.1: 13/sep/2004	Nueva pesta�a de administraci�n de la asignatura, ya que el men� de asignatura era demasiado grande. (13858 l�neas)
	Versi�n  4.46.2: 14/sep/2004	Corregido peque�o bug en pesta�as. (13859 l�neas)
	Versi�n  4.46.3: 20/sep/2004	Cada tipo de usuario tiene una cuota distinta en el malet�n. (13883 l�neas)
	Versi�n  4.46.4: 20/sep/2004	Creaci�n autom�tica del directorio de foros si no existe. (13917 l�neas)
	Versi�n  4.47:   20/sep/2004	Elecci�n de la titulaci�n, el curso y la asignatura mediante selectores. (13891 l�neas)
	Versi�n  4.47.1: 20/sep/2004	Los avisos pasan a la izquierda y el men� de enlaces a la derecha. (13887 l�neas)
	Versi�n  4.47.2: 21/sep/2004	Corregido peque�o bug y optimizaci�n en el dibujo de las pesta�as. (13886 l�neas)
	Versi�n  4.47.3: 21/sep/2004	Las ventanas emergentes (por ej. para descarga) aparecen sin barra de direcciones. (13887 l�neas)
	Versi�n  4.47.4: 21/sep/2004	Mejora interna la implementaci�n de las acciones relacionadas con descarga y maletin. (13872 l�neas)
	Versi�n  4.47.5: 21/sep/2004	Cada titulaci�n lleva su escudo. (13879 l�neas)
	Versi�n  4.48:   21/sep/2004	Posibilidad de borrar en el foro los mensajes sin respuesta enviados por uno mismo. (13986 l�neas)
	Versi�n  4.49:   22/sep/2004	Se puede obtener el c�digo HTML de acceso directo a una asignatura. (14047 l�neas)
	Versi�n  4.49.1: 22/sep/2004	El c�digo HTML de acceso directo no ser� editable. (14057 l�neas)
	Versi�n  4.49.2: 22/sep/2004	A�adidas asignaturas en Ceuta. (14132 l�neas)
	Versi�n  4.49.3: 23/sep/2004	Se insertan retornos de p�gina en la vista de impresi�n de las fichas. (14172 l�neas)
	Versi�n  4.49.4: 24/sep/2004	Se crean autom�ticamente los directorios de una asignatura si no exist�an. (14260 l�neas)
	Versi�n  4.49.5: 24/sep/2004	Corregido peque�o bug en el env�o de correo a los alumnos. (14266 l�neas)
	Versi�n  4.49.6: 26/sep/2004	Se crea autom�ticamente el fichero de configuraci�n de una asignatura si no existe.
					Eliminado par�metro CURSO_ACADEMICO del fichero de configuraci�n.
					Corregidos bugs en la creaci�n autom�tica de ciertos ficheros de una asignatura. (14278 l�neas)
	Versi�n  4.49.7: 27/sep/2004	Comienza la implementaci�n del uso de apodos (nicknames). (14333 l�neas)
	Versi�n  4.50:   28/sep/2004	Posibilidad de que aparezcan las fotos en el listado de datos de alumnos. (14380 l�neas)
	Versi�n  4.50.1: 28/sep/2004	Contin�a la implementaci�n del uso de apodos (nicknames). (14512 l�neas)
	Versi�n  4.51:   28/sep/2004	Incorporando la primera versi�n buena del chat realizada por Ra�l Jim�nez Ben�tez. (14518 l�neas)
	Versi�n  4.51.1: 29/sep/2004	Contin�a la implementaci�n del uso de apodos (nicknames). (14519 l�neas)
	Versi�n  4.51.2: 30/sep/2004	Creaci�n autom�tica de los directorios de bibliograf�a, evaluaci�n, tutor�as,... si no existen. (14533 l�neas)
	Versi�n  4.51.3: 30/sep/2004	Contin�a la implementaci�n del uso de apodos (nicknames).
					Corregido bug en la recepci�n de ficheros HTML con tipo MIME text/plain. (14546 l�neas)
	Versi�n  4.51.4: 30/sep/2004	Corregidos varios bugs peque�os. (14562 l�neas)
	Versi�n  4.52:   01/oct/2004	A partir de ahora no se contemplan los usuarios de demostraci�n.
					Cambios en el significado de fotos p�blicas y privadas para profesores y alumnos.
					Mejora en la presentaci�n del nivel 1 del foro. (14301 l�neas)
	Versi�n  4.52.1: 03/oct/2004	Termina la implementaci�n de la recepci�n de apodo (nickname) nuevo. (14347 l�neas)
	Versi�n  4.53:   03/oct/2004	Se puede hacer login con el ID o con el apodo (nickname). (14459 l�neas)
	Versi�n  4.54:   04/oct/2004	Cada profesor edita su horario de tutor�as.
					Se muestra el horario de todos los profesores de la asignatura (14561 l�neas)
	Versi�n  4.54.1: 04/oct/2004	Ligeros cambios en tama�os de fotos en orlas.
					Mejora en la presentaci�n de los mensajes de aviso y de error. (14534 l�neas)
	Versi�n  4.54.2: 04/oct/2004	Cambios en llamada al chat. Se deja funcionando una versi�n del chat con errores m�nimos. (14604 l�neas)
	Versi�n  4.55:   05/oct/2004	Eliminaci�n de todos los alumnos de la asignatura (para el comienzo de un nuevo curso, por ejemplo).
					Versi�n con la que se hizo la presentaci�n del CIAWI 2004 (14668 l�neas)
	Versi�n  4.55.1: 10/oct/2004	Se comprueba y mejora el formato de la fecha de nacimiento. (14828 l�neas)
	Versi�n  4.55.2: 10/oct/2004	A�adido campo de lugar de tutor�as al horario de tutor�as.
					Cambio en la forma de almacenar los horarios (14836 l�neas)
	Versi�n  4.55.3: 10/oct/2004	Mejorado mensaje inicial en descarga de archivos. (14843 l�neas)
	Versi�n  4.55.4: 12/oct/2004	Peque�a modificaci�n en la escritura de mensajes en el fichero de registro. (14848 l�neas)
	Versi�n  4.55.5: 12/oct/2004	Eliminado el tipo de orla que s�lo permite ver los usuarios con foto. (14831 l�neas)
	Versi�n  4.56:   12/oct/2004	Permitir al administrador o a un profesor el cambio de contrase�a de otro usuario. (14967 l�neas)
	Versi�n  4.57:   13/oct/2004	Comienza la implementaci�n de la descripci�n de la asignatura (sugerida por Jos� Luis Bernier). (14983 l�neas)
	Versi�n  4.57.1: 15/oct/2004	Algunos cambios en el chat. (14992 l�neas)
	Versi�n  4.57.2: 16/oct/2004	Se utiliza un tiempo peque�o para usuarios conectados y un tiempo mayor para el cierre de sesi�n. (14996 l�neas)
	Versi�n  4.58:   17/oct/2004	Cambio en el aspecto de las pesta�as. (15017 l�neas)
	Versi�n  4.58.1: 18/oct/2004	Ligera modificaci�n a los cambios anteriores en el aspecto de las pesta�as. (15015 l�neas)
	Versi�n  4.59:   18/oct/2004	Calendario acad�mico de la asignatura. (15113 l�neas)
	Versi�n  4.59.1: 20/oct/2004	Cambios en el calendario acad�mico de la asignatura. (15207 l�neas)
	Versi�n  4.59.2: 21/oct/2004	Se muestra una vista de impresi�n del calendario acad�mico de la asignatura. (15230 l�neas)
	Versi�n  4.59.3: 21/oct/2004	Reorganizaci�n en la presentaci�n de algunos men�s, avisos, etc. (15249 l�neas)
	Versi�n  4.59.4: 22/oct/2004	Ligeros cambios est�ticos. (15249 l�neas)
	Versi�n  4.60:   22/oct/2004	Chat: acceso a varias salas aparte de la de la asignatura. (15326 l�neas)
	Versi�n  4.60.1: 24/oct/2004	Se muestra la descripci�n de la asignatura. (15416 l�neas)
	Versi�n  4.60.2: 24/oct/2004	Edici�n de la descripci�n de la asignatura. (15583 l�neas)
	Versi�n  4.60.3: 25/oct/2004	Antes de entrar en el chat, los espacios del apodo se cambian por "_" (el chat no acepta apodos con espacios). (15590 l�neas)
					Versi�n con la que se hizo la memoria del proyecto de innovaci�n docente "Plataforma SWAD..." y la presentaci�n en el congreso SIIE'04.
	Versi�n  4.61:   27/oct/2004	Primera versi�n en http://swad.ugr.es/ (15591 l�neas)
	Versi�n  4.61.1: 03/nov/2004	Versi�n en https://swad.ugr.es/ (15593 l�neas)
	Versi�n  4.61.2: 08/nov/2004	Corregido bug en edici�n del horario (15612 l�neas)
	Versi�n  4.61.3: 08/nov/2004	Aumentada la cuota de disco para el malet�n de los alumnos (15613 l�neas)
	Versi�n  4.62:   09/nov/2004	A�adiendo algunas carreras de Ceuta. (15833 l�neas)
	Versi�n  4.62.1: 11/nov/2004	Cambio en el almacenamiento de las asignaturas. (15802 l�neas)
	Versi�n  4.63:   13/nov/2004	La salida del CGI va a un fichero, y de ese fichero se escribe al final en la salida est�ndar. (15901 l�neas)
	Versi�n  4.63.1: 22/nov/2004	Aumentada la cuota de los alumnos a 30 MiB. Se escribe la cuota ocupada al presentar el malet�n y el listado de alumnos. (15943 l�neas)
	Versi�n  4.63.2: 23/nov/2004	Peque�o cambio en la presentaci�n de la orla para env�o de mensajes. (15944 l�neas)
	Versi�n  4.63.3: 25/nov/2004	Se evita que se env�en mensajes sin destinatario. (15963 l�neas)
	Versi�n  4.63.4: 30/nov/2004	Se aceptan ID de hasta 10 d�gitos, desde 6 d�gitos, y tambi�n los que empiecen por dos letras.
					Corregido bug en usuarios conectados: se puede ver una foto de otro usuario siguiendo los mismos criterios que en la orla.
					Corregido peque�o bug en selecci�n de todos los usuarios en el env�o de mensajes (16000 l�neas)
	Versi�n  4.63.5: 30/nov/2004	En el env�o de mensajes internos se usa el ID encriptado. (16014 l�neas)
	Versi�n  4.63.6: 30/nov/2004	Al pasar el rat�n sobre una fotograf�a, se muestra en grande a la derecha. (16027 l�neas)
	Versi�n  4.63.7: 01/dic/2004	Mejorada la conversi�n a may�sculas/min�sculas de letras con tilde y la comparaci�n de cadenas. (16083 l�neas)
	Versi�n  4.64:   01/dic/2004	En el env�o de mensajes entre usuarios aparece una orla de profesores y otra de alumnos. (16141 l�neas)
	Versi�n  4.65:   01/dic/2004	En los mensajes entre usuarios se incluye el mensaje original al responder. (16208 l�neas)
	Versi�n  4.65.1: 02/dic/2004	Ligeras modificaciones internas en el dibujo de la orla. (16245 l�neas)
	Versi�n  4.65.2: 02/dic/2004	Al ordenar la lista de usuarios, los que no tienen nombre y apellidos aparecen al final. (16278 l�neas)
	Versi�n  4.65.3: 07/dic/2004	Cambios para que las pesta�as se vean bien en el Explorer 5. (16282 l�neas)
	Versi�n  4.65.4: 07/dic/2004	Cambios internos en la tabla de acciones. (16304 l�neas)
	Versi�n  4.65.5: 08/dic/2004	El c�digo de acceso directo se presenta en XHTML. (16302 l�neas)
	Versi�n  4.65.6: 08/dic/2004	Peque�as mejoras est�ticas. (16295 l�neas)
	Versi�n  4.66:   13/dic/2004	El registro de accesos se guarda en una tabla MySQL. (16349 l�neas)
					gcc swad.c rijndael.c -o swad -Wall -O2 -lmysqlclient -lz -L/usr/lib/mysql
	Versi�n  4.66.1: 14/dic/2004	Los accesos se toman de la tabla MySQL. (16105 l�neas)
	Versi�n  4.66.2: 14/dic/2004	Cambios en la presentaci�n de los accesos. (16214 l�neas)
	Versi�n  4.66.3: 15/dic/2004	Cambios en la presentaci�n de los accesos. (16225 l�neas)
	Versi�n  4.66.4: 15/dic/2004	Corregido bug en evaluaci�n del test. (16202 l�neas)
	Versi�n  4.66.5: 15/dic/2004	A�adida la titulaci�n Licenciado en Econom�a. (16368 l�neas)
	Versi�n  4.66.6: 16/dic/2004	Los ficheros de evaluaci�n, FAQ, etc., aparte index.html, tambi�n pueden llamarse index.htm. (16395 l�neas)
	Versi�n  4.66.7: 22/dic/2004	Se obliga a los usuarios a introducir el nombre y el primer apellido. (16412 l�neas)
	Versi�n  4.67:   27/dic/2004	Los alumnos matriculados en cada asignatura se almacenan en una tabla MySQL. (16737 l�neas)
	Versi�n  4.67.1: 28/dic/2004	La �ltima pesta�a y los accesos al test se almacenan en la misma tabla MySQL. (16398 l�neas)
	Versi�n  4.68:   28/dic/2004	Los usuarios pueden identificarse al entrar a la plataforma, antes de elegir asignatura. (16389 l�neas)
	Versi�n  4.69:   29/dic/2004	Aparece un men� con acceso a las asignaturas en las que est� dado de alta el usuario. (16471 l�neas)
	Versi�n  4.70:   29/dic/2004	Los apodos se almacenan en una tabla MySQL. (16555 l�neas)
	Versi�n  4.70.1: 29/dic/2004	Peque�os cambios internos. (16476 l�neas)
	Versi�n  4.71:   31/dic/2004	Comienza implementaci�n de consulta de accesos entre dos fechas. (16581 l�neas)
	Versi�n  4.71.1: 03/ene/2005	Consulta de accesos entre dos fechas. (16622 l�neas)
	Versi�n  4.71.2: 08/ene/2005	Corregido bug en presentaci�n del calendario. Nuevo formulario para la fecha de nacimiento. (16648 l�neas)
	Versi�n  4.72:   09/ene/2005	N�mero de accesos de cada usuario. (16793 l�neas)
	Versi�n  4.72.1: 09/ene/2005	Aparece la fotograf�a en el n�mero de accesos de cada usuario. (16802 l�neas)
	Versi�n  4.72.2: 10/ene/2005	En la consulta de accesos cada fila aparece con un color. (16809 l�neas)
	Versi�n  4.73:   10/ene/2005	N�mero de accesos por fechas. (16879 l�neas)
	Versi�n  4.73.1: 11/ene/2005	Cambios en la presentaci�n del n�mero de accesos. (16869 l�neas)
	Versi�n  4.73.2: 11/ene/2005	Cambios en el zoom de las fotograf�as al pasar el rat�n sobre ellas. (16901 l�neas)
	Versi�n  4.74:   12/ene/2005	N�mero de accesos por tipo de acci�n. (16980 l�neas)
	Versi�n  4.74.1: 13/ene/2005	Mejora gr�fica en las estad�sticas de acceso. (16990 l�neas)
	Versi�n  4.75:   15/ene/2005	Estad�sticas de acceso por horas. (17094 l�neas)
	Versi�n  4.75.1: 15/ene/2005	Mejora gr�fica en las estad�sticas de acceso por horas. (17096 l�neas)
	Versi�n  4.76:   16/ene/2005	Estad�sticas de acceso de todos los alumnos de SWAD a todas las asignaturas. (17177 l�neas)
	Versi�n  4.76.1: 17/ene/2005	Estad�sticas de acceso de todos los alumnos, profesores, o usuarios de SWAD a todas las asignaturas. (17212 l�neas)
	Versi�n  4.76.2: 17/ene/2005	Peque�as mejoras internas. Corregidos algunos bugs. (17215 l�neas)
	Versi�n  4.76.3: 18/ene/2005	Corregido peque�o bug. (17232 l�neas)
	Versi�n  4.76.4: 18/ene/2005	Mejora en la elecci�n de fechas para accesos y estad�sticas. (17286 l�neas)
	Versi�n  4.77:   18/ene/2005	Comienza implementaci�n de ex�menes de test en MySQL. (17381 l�neas)
	Versi�n  4.77.1: 19/ene/2005	Contin�a implementaci�n de ex�menes de test en MySQL. Corregido peque�o problema en impresi�n de fotos profesores. (17455 l�neas)
	Versi�n  4.77.2: 19/ene/2005	Contin�a implementaci�n de ex�menes de test en MySQL. (17521 l�neas)
	Versi�n  4.77.3: 20/ene/2005	Contin�a implementaci�n de ex�menes de test en MySQL. (17622 l�neas)
	Versi�n  4.77.4: 20/ene/2005	Contin�a implementaci�n de ex�menes de test en MySQL.
					Jos� Luis Bernier aporta la idea de a�adir descriptores o palabras clave a cada pregunta de test. (17647 l�neas)
	Versi�n  4.77.5: 24/ene/2005	Contin�a implementaci�n de ex�menes de test en MySQL. (17657 l�neas)
	Versi�n  4.77.6: 24/ene/2005	Contin�a implementaci�n de ex�menes de test en MySQL. (17958 l�neas)
	Versi�n  4.77.7: 25/ene/2005	Contin�a implementaci�n de ex�menes de test en MySQL. (18038 l�neas)
	Versi�n  4.77.8: 25/ene/2005	Contin�a implementaci�n de ex�menes de test en MySQL. (18180 l�neas)
	Versi�n  4.77.9: 26/ene/2005	Actualizado MySQL de versi�n 3.23.58 a versi�n 4.1.9 (el comportamiento de TIMESTAMP es diferente en los SELECT) (18194 l�neas)
	Versi�n  4.77.10:27/ene/2005	Contin�a implementaci�n de ex�menes de test en MySQL. (18425 l�neas)
	Versi�n  4.77.11:27/ene/2005	Contin�a implementaci�n de ex�menes de test en MySQL. (18512 l�neas)
	Versi�n  4.77.12:28/ene/2005	Contin�a implementaci�n de ex�menes de test en MySQL. (18623 l�neas)
	Versi�n  4.77.13:29/ene/2005	Terminada la edici�n de preguntas de ex�menes de test en MySQL. (18688 l�neas)
	Versi�n  4.77.14:29/ene/2005	Contin�a implementaci�n de ex�menes de test. (18625 l�neas)
	Versi�n  4.77.15:31/ene/2005	Contin�a implementaci�n de ex�menes de test y otras mejoras. (19130 l�neas)
	Versi�n  4.77.16:31/ene/2005	Contin�a implementaci�n de ex�menes de test. (19228 l�neas)
	Versi�n  4.77.17:01/feb/2005	Contin�a implementaci�n de ex�menes de test. (19339 l�neas)
	Versi�n  4.77.18:01/feb/2005	Contin�a implementaci�n de ex�menes de test. (19489 l�neas)
	Versi�n  4.77.19:02/feb/2005	Contin�a implementaci�n de ex�menes de test. (19491 l�neas)
	Versi�n  4.77.20:03/feb/2005	Importaci�n de los test antiguos a la base de datos. (19646 l�neas)
	Versi�n  4.77.21:04/feb/2005	Contin�a implementaci�n de ex�menes de test, aparte de otros m�nimos cambios. (19656 l�neas)
	Versi�n  4.77.22:04/feb/2005	Termina de momento la implementaci�n de ex�menes de test. Se ponen a disposici�n de los alumnos. (19737 l�neas)
	Versi�n  4.77.23:08/feb/2005	A�adida posibilidad de que se deje en blanco una pregunta de elecci�n simple tras seleccionar una opci�n. (19764 l�neas)
	Versi�n  4.78:   08/feb/2005	A�adida opci�n para renombrar descriptores de preguntas de test. (19867 l�neas)
	Versi�n  4.79:   08/feb/2005	Eliminado el test antiguo. (19322 l�neas)
	Versi�n  4.79.1: 09/feb/2005	Corregido peque�o bug. (19335 l�neas)
	Versi�n  4.79.2: 09/feb/2005	Mejoras internas y correci�n de peque�o bug. (19339 l�neas)
	Versi�n  4.79.3: 09/feb/2005	Se evita que haya dos descriptores iguales en la misma pregunta al renombrar descriptores. (19406 l�neas)
	Versi�n  4.79.4: 10/feb/2005	Se evita que haya dos descriptores iguales en la recepci�n de una pregunta. (19416 l�neas)
	Versi�n  4.80:   10/feb/2005	Se almacenan las respuestas de los alumnos en el test para calcular la dificultad de las preguntas. (19504 l�neas)
	Versi�n  4.80.1: 10/feb/2005	Todos los profesores tienen acceso a editar el test. (19506 l�neas)
	Versi�n  4.80.2: 14/feb/2005	Corregido peque�o bug en los foros cuando el asunto de un mensaje es nulo. (19514 l�neas)
	Versi�n  4.80.3: 14/feb/2005	Las titulaciones y sus asignaturas pasan al fichero asignaturas.c
					A�adida titulaci�n de Caminos. (18578 l�neas)
	Versi�n  4.81:   14/feb/2005	Los alumnos conectados se guardan en la base de datos. (18595 l�neas)
	Versi�n  4.82:   15/feb/2005	Nueva estad�stica que permite comparar accesos por asignaturas. (18672 l�neas)
	Versi�n  4.82.1: 16/feb/2005	A�adida titulaci�n de Maestro: Educaci�n Especial (Ceuta) y peque�os cambios. (18669 l�neas)
	Versi�n  4.82.2: 17/feb/2005	Peque�os cambios est�ticos. Se muestra la plataforma en la Facultad de Empresariales. (18685 l�neas)
	Versi�n  4.82.3: 18/feb/2005	Impedir que se env�en ficheros con caracteres no permitidos como tildes. (18702 l�neas)
	Versi�n  4.82.4: 18/feb/2005	"Mis asignaturas" salen ordenadas por c�digo.
					Desdoblados algunos grupos de Licenciado en Econom�a.
					A�adidos 1� y 2� de Teleco.
					A�adidas optativas de L.A.D.E. (18706 l�neas)
	Versi�n  4.82.5: 19/feb/2005	A�adida titulaci�n Licenciado en Antropolog�a Social y Cultural. (18707 l�neas)
	Versi�n  4.82.6: 20/feb/2005	Ampliados mensajes de ayuda. (18712 l�neas)
	Versi�n  4.83:   20/feb/2005	Las fichas se exportan a la base de datos. (18876 l�neas)
	Versi�n  4.83.1: 20/feb/2005	Corregidos algunos bugs en la exportaci�n de las fichas. (19763 l�neas)
	Versi�n  4.84:   20/feb/2005	Se usan las fichas de la base de datos. (18772 l�neas)
	Versi�n  4.84.1: 21/feb/2005	Corregido bug en acceso a base de datos.
					Desdoblados algunos grupos de Licenciado en Econom�a. A�adida asignatura Econlab. (18771 l�neas)
	Versi�n  4.84.2: 21/feb/2005	A�adida Diplomado en Ciencias Empresariales.
					Desdoblada asignatura ITIG12TC.
					A�adido enlace a la asignatura al ver estad�sticas por asignaturas. Otros peque�os cambios. (18755 l�neas)
	Versi�n  4.84.3: 21/feb/2005	Se respetan retornos de carro en los avisos (notas amarillas).
					Cambio en la eliminaci�n de espacios iniciales y finales. (18896 l�neas)
	Versi�n  4.84.4: 21/feb/2005	Cambio m�nimo en edici�n de temarios. (18897 l�neas)
	Versi�n  4.84.5: 22/feb/2005	Se impide que un profesor pueda dar de alta o de baja a un usuario existente que no sea alumno.
					A partir de ahora no se indican aqu� las nuevas titulaciones o asignaturas a�adidas. (18929 l�neas)
	Versi�n  4.84.6: 22/feb/2005	Peque�o cambio en presentaci�n de temarios. (18931 l�neas)
	Versi�n  4.84.7: 23/feb/2005	Peque�o cambio en presentaci�n de orlita para mensajes y accesos. (18932 l�neas)
	Versi�n  4.85:   24/feb/2005	Se muestra la asignatura origen de un mensaje entre usuarios y se responde en esa asignatura. (18956 l�neas)
	Versi�n  4.85.1: 24/feb/2005	A�adido departamento nuevo. (18957 l�neas)
	Versi�n  4.85.2: 25/feb/2005	A�adidos o mejorados algunos mensajes de ayuda a profesores. (18977 l�neas)
	Versi�n  4.86:   27/feb/2005	Foro para profesores de SWAD. (19065 l�neas)
	Versi�n  4.86.1: 27/feb/2005	Se impide que aparezca la foto de un profesor con foto no p�blica en el foro para profesores. (19101 l�neas)
	Versi�n  4.86.2: 28/feb/2005	Cambios internos, est�ticos y nuevos mensajes de ayuda. (19143 l�neas)
	Versi�n  4.86.3: 01/mar/2005	Cambios est�ticos. La foto del listado de alumnos pasa a la primera columna, para que sea m�s f�cil guardar las notas. (19133 l�neas)
	Versi�n  4.87:   01/mar/2005	Enlaces a la p�gina del autor del mensaje en los foros.
					Un profesor puede crear, renombrar y borrar grupos de teor�a y de pr�cticas de una asignatura. (19631 l�neas)
	Versi�n  4.87.1: 02/mar/2005	Cambios en edici�n de grupos de teor�a o pr�cticas. (19705 l�neas)
	Versi�n  4.87.2: 02/mar/2005	A�adido l�mite de alumnos en grupos de teor�a o pr�cticas. (19826 l�neas)
	Versi�n  4.87.3: 02/mar/2005	Un alumno puede ver el grupo de teor�a y el de pr�cticas a los que pertenece dentro de una asignatura. (20021 l�neas)
	Versi�n  4.87.4: 08/mar/2005	Se impide que se introduzcan direcciones de p�ginas web personales sin un ".". (20057 l�neas)
	Versi�n  4.87.5: 08/mar/2005	Corregidos peque�os bugs en el foro y en la consulta individualizada de calificaciones. (20060 l�neas)
	Versi�n  4.87.6: 09/mar/2005	Corregido peque�o bug en ordenaci�n de usuarios para lista.
					Se guarda en la base de datos el n�mero de columnas de la orla para cada usuario y asignatura. (20156 l�neas)
	Versi�n  4.87.7: 09/mar/2005	Cambiado icono de foro y chat. Alterado orden de opciones de la pesta�a "Mensajes". (20157 l�neas)
	Versi�n  4.87.8: 15/mar/2005	Nuevo acceso al correo de la UGR. (20158 l�neas)
	Versi�n  4.88:   15/mar/2005	Se a�ade nueva columna en la edici�n de preguntas de test: n� de veces que se ha contestado la pregunta.
					Las preguntas se pueden ordenar por puntuaci�n promedio. (20244 l�neas)
	Versi�n  4.88.1: 31/mar/2005	Peque�os cambios en mensajes de ayuda.
					Se muestra el t�tulo de una hebra del foro al ver sus mensajes. (20297 l�neas)
	Versi�n  4.88.2: 05/abr/2005	Mejoras gr�ficas en el listado de archivos. (20337 l�neas)
	Versi�n  4.88.3: 06/abr/2005	Mejoras gr�ficas en el listado de archivos. (20323 l�neas)
	Versi�n  4.88.4: 06/abr/2005	Mejoras gr�ficas en el listado de archivos. (20345 l�neas)
	Versi�n  4.89:   06/abr/2005	Cambio en los colores de muchas opciones. (20347 l�neas)
	Versi�n  4.89.1: 13/abr/2005	Mejora gr�fica en los foros. (20374 l�neas)
	Versi�n  4.89.2: 14/abr/2005	Algunas mejoras gr�ficas. (20395 l�neas)
	Versi�n  4.90:   17/abr/2005	A�adido nuevo foro com�n para todos los usuarios de SWAD. (20555 l�neas)
	Versi�n  4.90.1: 19/abr/2005	Subidas las cuotas de maletines y descargas. (20556 l�neas)
	Versi�n  4.90.2: 19/abr/2005	Para aumentar la velocidad se usa una hoja de estilo aparte (estilos.css), se a�ade un estilo para las fotos, y el acceso a centros y departamentos se realiza como una acci�n aparte. (20527 l�neas)
	Versi�n  4.91:   20/abr/2005	Los mensajes de foros, los mensajes enviados y los recibidos se listan de 10 en 10. (20629 l�neas)
	Versi�n  4.91.1: 21/abr/2005	Cambios relacionados con la divisi�n en p�ginas de los mensajes. (20641 l�neas)
	Versi�n  4.91.2: 22/abr/2005	Los mensajes recibidos y enviados se muestran desde el m�s antiguo al m�s reciente.
					Otros cambios relacionados con mensajes. (20642 l�neas)
	Versi�n  4.91.3: 22/abr/2005	Se repiten los enlaces a las distintas p�ginas tras listar los mensajes. (20663 l�neas)
	Versi�n  4.91.4: 25/abr/2005	Contin�a la implementaci�n de los grupos de alumnos. (20933 l�neas)
	Versi�n  4.92:   25/abr/2005	Comienza la utilizaci�n de los grupos de alumnos para ver orlas, fichas, etc. (21135 l�neas)
	Versi�n  4.92.1: 26/abr/2005	Contin�a la utilizaci�n de los grupos de alumnos para ver orlas, fichas, etc. (21199 l�neas)
	Versi�n  4.93:   27/abr/2005	A�adido listado de datos resumidos de alumnos en la ventana principal. (21226 l�neas)
	Versi�n  4.93.1: 28/abr/2005	Se utiliza un �nico formulario para apuntarse a grupos de teor�a y pr�cticas. (21349 l�neas)
	Versi�n  4.93.2: 29/abr/2005	Es necesario confirmar el borrado de grupos de teor�a o pr�cticas cuando contengan alumnos. (21430 l�neas)
	Versi�n  4.93.3: 29/abr/2005	Cuando se seleccionan los alumnos de un grupo en una orla, tambi�n aparecen todos los profesores de la asignatura. (21471 l�neas)
	Versi�n  4.93.4: 01/may/2005	Cambios en el dise�o del horario. (21490 l�neas)
	Versi�n  4.93.5: 01/may/2005	Mostrar vista de impresi�n del horario. (21505 l�neas)
	Versi�n  4.93.6: 02/may/2005	Se pueden seleccionar los alumnos que no pertenecen a ning�n grupo de teor�a o de pr�cticas. (21688 l�neas)
	Versi�n  4.93.7: 04/may/2005	Corregido peque�o bug en mensaje a usuario desde usuarios conectados. (21689 l�neas)
	Versi�n  4.93.8: 11/may/2005	Cambio interno en la b�squeda en la base de datos de los alumnos que no pertenecen a ning�n grupo. (21663 l�neas)
	Versi�n  4.93.9: 11/may/2005	Cambio en la fecha inicial en las acciones de ver accesos y estad�sticas.
					En la cabecera de la orla aparecen los grupos seleccionados. (21749 l�neas)
	Versi�n  4.93.10:12/may/2005	Corregido peque�o bug en selecci�n de usuarios para escribirles un mensaje o para ver sus accesos. (21776 l�neas)
	Versi�n  4.94:   18/may/2005	Los iconos de los men�s pasan de tama�o 48x32 a 32x32. (21778 l�neas)
	Versi�n  4.94.1: 19/may/2005	Cambios en la llamada al chat. (21780 l�neas)
	Versi�n  4.95:   23/may/2005	Los horarios de las asignaturas y de tutor�as pasan a almacenarse en la base de datos. (22032 l�neas)
	Versi�n  4.95.1: 23/may/2005	Peque�os cambios en el almacenamiento de los horarios en la base de datos. (22041 l�neas)
	Versi�n  4.95.2: 31/may/2005	Corregido peque�o bug en c�lculo de fecha inicial del �ltimo mes para presentaci�n de estad�sticas. (22042 l�neas)
	Versi�n  4.95.3: 31/may/2005	Se a�ade bot�n "Ayer" al formulario de selecci�n de rango de fechas. (22068 l�neas)
	Versi�n  4.95.4: 31/may/2005	Todas las asignaturas cuelgan ahora de un directorio asg. (22079 l�neas)
	Versi�n  4.96:   31/may/2005	Nueva forma de descargar los archivos: no se abre una ventana nueva con un enlace, sino que se crean enlaces a todos los archivos en la ventana principal. (22122 l�neas)
	Versi�n  4.96.1: 01/jun/2005	Corregido bug en recepci�n de archivos y bug en borrado de descargas antiguas. (22124 l�neas)
	Versi�n  4.97:   01/jun/2005	Nueva opci�n que muestra el uso de SWAD: n�mero de asignaturas, de alumnos, de profesores... (22289 l�neas)
	Versi�n  4.98:   02/jun/2005	Comienza la implementaci�n de convocatorias de ex�menes. (22408 l�neas)
	Versi�n  4.98.1: 13/jun/2005	Contin�a la implementaci�n de convocatorias de ex�menes. (22495 l�neas)
	Versi�n  4.98.2: 14/jun/2005	Contin�a la implementaci�n de convocatorias de ex�menes. (22645 l�neas)
	Versi�n  4.98.3: 14/jun/2005	Eliminados espacios en c�digo HTML para que el men� de pesta�as se vea bien en MSIE 5.0 (aulas de pr�cticas). (22631 l�neas)
			01/jul/2005	 El jurado del Premio de Innovaci�n Docente 2005 de la Universidad de Granada ha decidido concederuna de las cuatro Menciones Honor�ficas a la Innovaci�n Docente 2005 al trabajo "Plataforma SWAD (Shared Workspace At a Distance)".
	Versi�n  4.98.4: 02/sep/2005	Peque�a modificaci�n interna. (22633 l�neas)
	Versi�n  4.98.5: 05/sep/2005	Contin�a la implementaci�n de convocatorias de ex�menes. (22627 l�neas)
	Versi�n  4.98.6: 07/sep/2005	Corregido bug en la presentaci�n de las fichas. Peque�o cambio interno en lectura de una fecha de un formulario.
					Contin�a la implementaci�n de convocatorias de ex�menes. (22724 l�neas)
	Versi�n  4.98.7: 08/sep/2005	Contin�a la implementaci�n de convocatorias de ex�menes. (23078 l�neas)
	Versi�n  4.98.8: 08/sep/2005	Contin�a la implementaci�n de convocatorias de ex�menes. (23098 l�neas)
	Versi�n  4.99:   18/sep/2005	A partir de hoy las asignaturas optativas y de libre configuraci�n se agrupar�n en el curso 0. (23101 l�neas)
	Versi�n  4.99.1: 19/sep/2005	Finaliza la implementaci�n de convocatorias de ex�menes. (23269 l�neas)
	Versi�n  4.99.2: 20/sep/2005	Peque�as modificaciones. (23276 l�neas)
	Versi�n  4.99.3: 20/sep/2005	Las convocatorias de examen aparecen destacadas en el calendario, con el correspondiente enlace.
					Corregido bug en tama�o reservado para almacenar mensajes de ayuda, gracias a un error detectado por Francisco Pert��ez V�lchez. (23429 l�neas)
	Versi�n  4.99.4: 21/sep/2005	Cambia la ordenaci�n de algunas opciones del men� de usuarios. Otros peque�os cambios. (23430 l�neas)
	Versi�n  4.100:  22/sep/2005	Si en una asignatura se han creado grupos, se obliga a los alumnos a apuntarse a ellos. (23504 l�neas)
	Versi�n  4.100.1:27/sep/2005	Corregido bug en edici�n de preguntas de test. (23505 l�neas)
	Versi�n  4.101:  27/sep/2005	Comienza la implementaci�n de la zona de env�o de trabajos a los profesores. (23558 l�neas)
	Versi�n  4.101.1:28/sep/2005	Corregido bug en malet�n debido al comienzo de la implementaci�n de la zona de env�o de trabajos a los profesores. (23559 l�neas)
	Versi�n  4.102:  29/sep/2005	Se sustituye el chat por la pizarra-chat. (23560 l�neas)
	Versi�n  4.102.1:30/sep/2005	A�adidos mensajes de ayuda en la edici�n de grupos de alumnos. (23566 l�neas)
	Versi�n  5.0:    30/sep/2005	Cambio de la festividades para al curso 2005-2006.
					Nuevo programa de procesamiento de fotograf�as, mejorado por Daniel Jes�s Calandria Hern�ndez. (23565 l�neas)
	Versi�n  5.1:    03/oct/2005	Los profesores pueden eliminar hebras completas de los foros de la asignatura, y el administrador tambi�n hebras de los foros comunes.
					Corregido bug en el programa de procesamiento de fotograf�as. (23724 l�neas)
	Versi�n  5.1.1:  04/oct/2005	Ampliados campos de edici�n de temarios.
					En el temario de pr�cticas aparece un enlace a la elecci�n de grupos si hay grupos de pr�cticas y el alumno no est� dado de alta en ninguno.
					Subido tama�o m�ximo de archivo para recepci�n a 32 MiB. (23746 l�neas)
	Versi�n  5.2:    05/oct/2005	En la lista de alumnos se a�aden dos columnas con los grupos de teor�a y pr�cticas a los que est� apuntado.
					Corregido peque�o bug en selecci�n de fechas de convocatoria de examen. (23841 l�neas)
	Versi�n  5.2.1:  05/oct/2005	Se muestra la cabecera original de los ficheros index.html o index.htm de enlaces, FAQ, etc. enviados al servidor.
					Numerosos cambios internos. (23872 l�neas)
	Versi�n  5.2.2:  14/oct/2005	La longitud m�nima del ID cambia a 5 n�meros o a una letra m�s 5 n�meros. (23876 l�neas)
	Versi�n  5.2.3:  14/oct/2005	Corregido bug en lectura de mensajes dentro de una hebra de foro. (23888 l�neas)
	Versi�n  5.2.4:  17/oct/2005	Contin�a la implementaci�n de la zona de env�o de trabajos a los profesores. (23917 l�neas)
	Versi�n  5.2.5:  17/oct/2005	Contin�a la implementaci�n de la zona de env�o de trabajos a los profesores. (24062 l�neas)
	Versi�n  5.2.6:  18/oct/2005	Contin�a la implementaci�n de la zona de env�o de trabajos a los profesores. (24083 l�neas)
	Versi�n  5.2.7:  18/oct/2005	Contin�a la implementaci�n de la zona de env�o de trabajos a los profesores. (24470 l�neas)
	Versi�n  5.2.8:  19/oct/2005	Contin�a la implementaci�n de la zona de env�o de trabajos a los profesores. (24537 l�neas)
	Versi�n  5.2.9:  19/oct/2005	Contin�a la implementaci�n de la zona de env�o de trabajos a los profesores. (24611 l�neas)
	Versi�n  5.2.10: 19/oct/2005	Contin�a la implementaci�n de la zona de env�o de trabajos a los profesores.
					Cambios internos relativos a los temarios de teor�a y pr�cticas. (24361 l�neas)
	Versi�n  5.3:    19/oct/2005	Se resalta in menu la supracci�n de la que depende la subacci�n actual.
					Terminada la implementaci�n de la zona de env�o de trabajos a los profesores a falta de activar permisos. (24556 l�neas)
	Versi�n  5.3.1:  20/oct/2005	En la zona de env�o de trabajos de alumnos es posible crear un fichero zip para descarga de todos los trabajos. (24736 l�neas)
	Versi�n  5.3.2:  21/oct/2005	Cambio interno en la creaci�n del fichero zip para descarga de todos los trabajos. (24679 l�neas)
	Versi�n  5.4:    21/oct/2005	Activados los permisos para el acceso de profesores y alumnos a las nuevas opciones de env�o de trabajos. (24752 l�neas)
	Versi�n  5.4.1:  21/oct/2005	Nueva opci�n para dar de baja simult�nea a varios alumnos. (24782 l�neas)
	Versi�n  5.4.2:  24/oct/2005	En aquellas opciones de la pesta�a "Usuarios" en las que se necesita el ID de otro usuario, se pasa ahora por una pantalla intermedia para pedirlo. (24871 l�neas)
	Versi�n  5.4.3:  25/oct/2005	Eliminada temporalmente la red neuronal de reconocimiento de caras porque fallaba demasiado.
					A�adida una nueva categor�a en la clasificaci�n de las asignaturas: el tipo de titulaci�n. (24904 l�neas)
	Versi�n  5.4.4:  27/oct/2005	Mejoras internas en la selecci�n de grupos. (24941 l�neas)
	Versi�n  5.4.5:  28/oct/2005	Los profesores pueden apuntarse a varios grupos de alumnos.(25153 l�neas)
	Versi�n  5.4.6:  29/oct/2005	En la lista de selecci�n de grupos aparecen por defecto los grupos a los que el usuario est� apuntado. (25035 l�neas)
	Versi�n  5.4.7:  01/nov/2005	Mejoras internas relativas a los grupos. (25009 l�neas)
	Versi�n  5.4.8:  02/nov/2005	Modificaciones en las tablas de la base de datos para los grupos de usuarios. (25011 l�neas)
	Versi�n  5.4.9:  03/nov/2005	Si no se ha enviado la fotograf�a, se presenta un mensaje indic�ndolo.
					En el uso de SWAD se presenta el n�mero de asignaturas con profesores y el n�mero de asignaturas con alumnos. (25044 l�neas)
	Versi�n  5.4.10: 04/nov/2005	Si un alumno no ha enviado la fotograf�a, no puede ver la orla. (25045 l�neas)
	Versi�n  5.4.11: 06/nov/2005	Si un alumno no ha enviado la fotograf�a no puede acceder a ciertaos servicios. (25071 l�neas)
	Versi�n  5.5:    07/nov/2005	Acceso directo a una asignatura mediante el m�todo GET.
					Cambios en la localizaci�n de algunos directorios p�blicos en el servidor. (25102 l�neas)
	Versi�n  5.5.1:  08/nov/2005	Cambios en la localizaci�n de algunos directorios p�blicos en el servidor.
					Modificado el c�digo de acceso directo a la asignatura.
					Ejemplo de acceso directo: https://swad.ugr.es/swad?CodAsg=II21EC1 (25086 l�neas)
	Versi�n  5.5.2:  08/nov/2005	Cambios internos relativos a la fecha y hora actuales. (25098 l�neas)
	Versi�n  5.5.3:  09/nov/2005	Mensajes de error m�s espec�ficos cuando el nombre de un archivo o carpeta no es v�ido. (25109 l�neas)
	Versi�n  5.5.4:  09/nov/2005	Cambios en la comprobaci�n de si una p�gina recibida es HTML o ZIP. (25097 l�neas)
	Versi�n  5.6:    09/nov/2005	Cada estudiante tiene un n�mero de accesos m�ximo a las opciones no permitidas a invitados mientras no env�e la foto. (25179 l�neas)
	Versi�n  5.6.1:  10/nov/2005	Mensaje indicando el software necesario para ejecutar la pizarra/chat. (25185 l�neas)
	Versi�n  5.6.2:  11/nov/2005	Corregido bug en edici�n de temarios de teor�a y pr�cticas. (25183 l�neas)
	Versi�n  5.6.3:  14/nov/2005	Comienza la implementaci�n de la creaci�n de tipos de grupos. (25252 l�neas)
	Versi�n  5.6.4:  14/nov/2005	Contin�a la implementaci�n de la creaci�n de tipos de grupos. (25689 l�neas)
	Versi�n  5.6.5:  15/nov/2005	Contin�a la implementaci�n de la creaci�n de tipos de grupos. (26518 l�neas)
	Versi�n  5.6.6:  16/nov/2005	Contin�a la implementaci�n de la creaci�n de tipos de grupos. (26859 l�neas)
	Versi�n  5.6.7:  16/nov/2005	Contin�a la implementaci�n de la creaci�n de tipos de grupos. (27588 l�neas)
	Versi�n  5.6.8:  17/nov/2005	Contin�a la implementaci�n de la creaci�n de tipos de grupos. (28058 l�neas)
	Versi�n  5.7:    17/nov/2005	Terminada la implementaci�n de la creaci�n de tipos de grupos. (28079 l�neas)
	Versi�n  5.7.1:  18/nov/2005	Eliminado el c�digo de gesti�n de grupos antiguo, que ya no es necesario. (25990 l�neas)
	Versi�n  5.7.2:  20/nov/2005	Corregidos permisos de usuarios en la creaci�n de grupos.
					Eliminadas columnas ya innecesarias en la tabla de grupos. (25989 l�neas)
	Versi�n  5.7.3:  21/nov/2005	Corregido bug en adscripci�n a grupos. (26046 l�neas)
	Versi�n  5.7.4:  22/nov/2005	A�adidos nuevos tipos de ficheros que se pueden subir al servidor. (26048 l�neas)
	Versi�n  5.7.5:  24/nov/2005	Desdoblado el usuario "invitado" en "desconocido" (no identificado) e "invitado" (identificado, pero no perteneciente a la asignatura)
					La pesta�a personal est� disponible para un invitado a�n cuando est� en una asignatura a la que no pertenece. (26061 l�neas)
	Versi�n  5.8:    24/nov/2005	Cuando se conecta un usuario le aparece su pesta�a "Personal" antes de seleccionar la asignatura. (26069 l�neas)
	Versi�n  5.8.1:  25/nov/2005	Ahora un usuario puede acceder a sus mensajes recibidos directamente aun cuando no haya seleccionado asignatura.
					Actualizados enlaces a departamentos. (26058 l�neas)
	Versi�n  5.8.2:  28/nov/2005	Corregido bug en creaci�n de grupos. (26064 l�neas)
	Versi�n  5.8.3:  28/nov/2005	Corregido bug en lectura del c�digo de asignatura cuando no se ha seleccionado ninguna y se intenta enviar un archivo. (26066 l�neas)
	Versi�n  5.8.4:  28/nov/2005	A�adidos accesos al registro incluso cuando no se ha seleccionado una asignatura. (26073 l�neas)
	Versi�n  5.8.5:  29/nov/2005	Corregido problema en las estad�sticas de acceso por asignaturas cuando el c�digo de una asignatura no existe.
					Corregido bug cuando existe un tipo de grupo de adscripci�n obligatoria pero no hay ning�n grupo de ese tipo. (26064 l�neas)
	Versi�n  5.9:    29/nov/2005	Nuevos foros para cada titulaci�n: de profesores, y de profesores y alumnos. (26172 l�neas)
	Versi�n  5.9.1:  29/nov/2005	Peque�os cambios en el listado de foros disponibles. (26139 l�neas)
	Versi�n  5.9.2:  30/nov/2005	Peque�o cambio en el listado de foros disponibles. (26140 l�neas)
	Versi�n  5.9.3:  01/dic/2005	Peque�o cambio en el listado de foros disponibles.
					Subida la anchura de las convocatorias de examen de 440 a 500 p�xeles. (26142 l�neas)
	Versi�n  5.10:   01/dic/2005	A�adido campo en las fichas de los alumnos de observaciones para todas las asignaturas. (26281 l�neas)
	Versi�n  5.10.1: 02/dic/2005	Cambios en la presentaci�n de la cabecera de las convocatorias de examen. (26286 l�neas)
	Versi�n  5.10.2: 02/dic/2005	Mejoras internas en asignaci�n de memoria para datos de usuarios. (26313 l�neas)
	Versi�n  5.11:   03/dic/2005	Se sustituye la pesta�a de administraci�n de la asignatura por una de evaluaci�n.
					Reorganizaci�n de las opciones de los men�s. (26313 l�neas)
	Versi�n  5.11.1: 04/dic/2005	Las esquinas de la orla se dibujan redondeadas. (26353 l�neas)
	Versi�n  5.11.2: 05/dic/2005	Las esquinas de algunas otros rect�ngulos se dibujan redondeadas. (26398 l�neas)
	Versi�n  5.11.3: 06/dic/2005	Las esquinas de m�s rect�ngulos se dibujan redondeadas. (26398 l�neas)
	Versi�n  5.11.4: 07/dic/2005	Peque�a mejora en la presentaci�n de los temarios. (26396 l�neas)
	Versi�n  5.11.5: 07/dic/2005	Las esquinas de m�s rect�ngulos se dibujan redondeadas.
					Diversas mejoras en la presentaci�n. (26358 l�neas)
	Versi�n  5.11.6: 13/dic/2005	Mejorada la detecci�n de piel en el programa de procesamiento de fotograf�as.
					Cambios en la recepci�n de fotograf�as. (26355 l�neas)
	Versi�n  5.11.7: 14/dic/2005	Corregido bug en recepci�n de par�metros, detectado por Francisco G�mez Mula. (26375 l�neas)
	Versi�n  5.11.8: 15/dic/2005	Los destinatarios de mensajes tienen un c�digo distinto del nombre usado para las fotograf�as, para evitar que un usuario astuto pueda averiguar el nombre de la foto de usuarios con foto privada. (26378 l�neas)
	Versi�n  5.11.9: 15/dic/2005	Corregido bug en alta de varios alumnos. (26382 l�neas)
	Versi�n  5.11.10:18/dic/2005	Algunas modificaciones internas referentes a los datos de los usuarios. (26347 l�neas)
	Versi�n  5.11.11:21/dic/2005	Peque�o cambio en el formulario de edici�n de tipos de grupo. (26351 l�neas)
	Versi�n  5.11.12:28/dic/2005	Cuando un profesor vea las calificaciones, se mostrar�n las de un alumno aleatorio.
					La opci�n est� programada, pero no activada. (26457 l�neas)
	Versi�n  5.11.13:02/ene/2006	Se a�ade un marco a la foto cuando se presenta en tama�o m�ximo. (26471 l�neas)
	Versi�n  5.12:   03/ene/2006	Comienza la implementaci�n del nuevo sistema de calificaciones de distintos ex�menes. (26596 l�neas)
	Versi�n  5.12.1: 04/ene/2006	Contin�a la implementaci�n del nuevo sistema de calificaciones. (27236 l�neas)
	Versi�n  5.12.2: 05/ene/2006	Contin�a la implementaci�n del nuevo sistema de calificaciones. (27342 l�neas)
	Versi�n  5.12.3: 05/ene/2006	Terminada la implementaci�n del nuevo sistema de calificaciones.
					Eliminado el sistema antiguo. (26926 l�neas)
	Versi�n  5.12.4: 07/ene/2006	Ampliados tipos MIME permitidos en el env�o de calificaciones, y otros cambios menores. (26947 l�neas)
	Versi�n  5.12.5: 10/ene/2006	Se pregunta antes de eliminar un archivo, y otros cambios menores. (27008 l�neas)
	Versi�n  5.12.6: 11/ene/2006	Mejoras gr�ficas en la presentaci�n de estad�sticas, principalmente en la de acceso por horas.
					Compilar con: gcc swad.c rijndael.c -o swad -Wall -O2 -lmysqlclient -lz -L/usr/lib/mysql -lm (27018 l�neas)
	Versi�n  5.13:   12/ene/2006	Nueva estad�stica de accesos por intervalos de 5 minutos. (27164 l�neas)
	Versi�n  5.13.1: 13/ene/2006	Cambios en la presentaci�n de las estad�sticas de acceso cada 5 minutos. (27206 l�neas)
	Versi�n  5.13.2: 13/ene/2006	Peque�o cambio en la presentaci�n de las estad�sticas de acceso cada 5 minutos. (27236 l�neas)
	Versi�n  5.13.3: 16/ene/2006	Corregido peque�o bug en enlaces a convocatorias de examen en el calendario. (27238 l�neas)
	Versi�n  5.13.4: 17/ene/2006	Los bordes de los meses del calendario y de los accesos a servicios de la universidad se dibujan redondeados. (27269 l�neas)
	Versi�n  5.13.5: 20/ene/2006	Corregido peque�o bug en dibujo de bordes redondeados. (27275 l�neas)
	Versi�n  5.14:   24/ene/2006	A�adida nueva estad�stica: tasa de accesos por asignatura. (27407 l�neas)
	Versi�n  5.14.1: 25/ene/2006	Mejorada la estad�stica de tasa de accesos por asignatura. Optimizados algunos �ndices en la base de datos. (27457 l�neas)
	Versi�n  5.14.2: 26/ene/2006	Eliminado campo innecesario de la tabla de accesos en la base de datos. (27456 l�neas)
	Versi�n  5.14.3: 27/ene/2006	Corregido peque�o bug en listado de accesos detallados de usuarios de una asignatura. (27460 l�neas)
	Versi�n  5.15:   29/ene/2006	Nueva estad�stica de accesos por semana. (27654 l�neas)
	Versi�n  5.15.1: 31/ene/2006	Nueva estad�stica de accesos por mes. (27792 l�neas)
	Versi�n  5.15.2: 02/feb/2006	Ampliados tipos MIME permitidos en el env�o de archivos.
					Las estad�sticas vuelven a mostrarse, por omisi�n, hasta el d�a actual. (27797 l�neas)
	Versi�n  5.16:   05/feb/2006	A�adidas fichas para cada asignatura, configurables por los profesores. (28545 l�neas)
	Versi�n  5.16.1: 06/feb/2006	En el listado de fichas se muestran los nuevos campos configurables. (28758 l�neas)
	Versi�n  5.16.2: 07/feb/2006	Comienza la implementaci�n de la recepci�n de datos de los nuevos campos configurables. (28884 l�neas)
	Versi�n  5.16.3: 08/feb/2006	Terminada la recepci�n de datos de los nuevos campos configurables. (29008 l�neas)
	Versi�n  5.16.4: 16/feb/2006	Cambios gr�ficos en la presentaci�n de las fichas. (29020 l�neas)
	Versi�n  5.16.5: 17/feb/2006	A�adido campo con n�mero de filas en formulario de ficha de la asignatura. (29133 l�neas)
	Versi�n  5.17:   17/feb/2006	Terminado de implementar el sistema de fichas de asignaturas. Se ofrece a los usuarios. (29134 l�neas)
	Versi�n  5.18:   18/feb/2006	Nueva pesta�a de estad�sticas y algunas mejoras gr�ficas. (29142 l�neas)
	Versi�n  5.18.1: 18/feb/2006	A�adido formulario para seleccionar los alumnos cuyas fichas desean verse.
					Se eliminan del men� par�metros extra de la orla y del listado de alumnos. (29208 l�neas)
	Versi�n  5.18.2: 19/feb/2006	Desde la orla y desde la lista de alumnos se puede acceder a la ficha de cada alumno. (29263 l�neas)
	Versi�n  5.19:   21/feb/2006	Nuevas estad�sticas con el n�mero de usuarios distintos que han accedido. (29334 l�neas)
	Versi�n  5.19.1: 22/feb/2006	Importantes cambios en las estad�sticas. (29330 l�neas)
	Versi�n  5.19.2: 23/feb/2006	Mejoras gr�ficas en las estad�sticas.
					A�adidos nuevos tipos de ficheros permitidos. (29376 l�neas)
	Versi�n  5.19.3: 24/feb/2006	Mejora interna en la selecci�n del n�mero de alumnos por fila en la orla. (29377 l�neas)
	Versi�n  5.19.4: 24/feb/2006	Mejora en las estad�sticas de acceso por semanas. (29432 l�neas)
	Versi�n  5.19.5: 26/feb/2006	Peque�os cambios internos. (29432 l�neas)
	Versi�n  5.19.6: 28/feb/2006	Mejora gr�fica en la presentaci�n de horarios. (29466 l�neas)
	Versi�n  5.19.7: 02/mar/2006	Corregido peque�o bug de javascript en Netscape y Firefox. (29467 l�neas)
	Versi�n  5.19.8: 08/mar/2006	Aparece un icono parpadeante cuando hay mensajes nuevos. Corregido peque�o bug en Firefox. (29475 l�neas)
	Versi�n  5.20:   08/mar/2006	Cada acci�n tiene un campo booleano que indica si se debe ejecutar un lockfile o no. (29486 l�neas)
	Versi�n  5.20.1: 09/mar/2006	Al responder a un mensaje, se marca el remitente en un color diferente para que destaque m�s. (29498 l�neas)
	Versi�n  5.21:   10/mar/2006	Aparece en las fichas un campo para hacer llamadas por skype. (29541 l�neas)
	Versi�n  5.22:   11/mar/2006	En la lista de alumnos se eliminan el campo de foto p�blica y el del tama�o del malet�n.
					En la lista resumida s�lo se muestran los campos m�s importantes. (29532 l�neas)
	Versi�n  5.22.1: 11/mar/2006	En la lista de alumnos completa se muestran los campos de la fichas en la asignatura. (29582 l�neas)
	Versi�n  5.22.2: 11/mar/2006	Cambio importante en la presentaci�n de la administraci�n de descargas, malet�n y trabajos. (29581 l�neas)
	Versi�n  5.22.3: 13/mar/2006	Comienza implementaci�n de la ocultaci�n de carpetas y archivos en descargas. (29620 l�neas)
	Versi�n  5.22.4: 14/mar/2006	Contin�a la implementaci�n de la ocultaci�n de carpetas y archivos en descargas. (29983 l�neas)
	Versi�n  5.23:   14/mar/2006	Finaliza la implementaci�n de la ocultaci�n de carpetas y archivos en descargas. (29973 l�neas)
	Versi�n  5.23.1: 14/mar/2006	Corregido problema de visualizaci�n del �rbol de exploraci�n enb Firefox. (29974 l�neas)
	Versi�n  5.23.2: 15/mar/2006	Cambio de icono parpadeante de mensaje nuevo. (29975 l�neas)
	Versi�n  5.24:   15/mar/2006	Comienza la implementaci�n de copiar-pegar en la exploraci�n de archivos (propuesta de Javier Alba Tercedor) (30238 l�neas)
	Versi�n  5.24.1: 16/mar/2006	Contin�a la implementaci�n de copiar-pegar en la exploraci�n de archivos. (30469 l�neas)
	Versi�n  5.24.2: 17/mar/2006	Contin�a la implementaci�n de copiar-pegar en la exploraci�n de archivos. (30598 l�neas)
	Versi�n  5.24.3: 17/mar/2006	Contin�a la implementaci�n de copiar-pegar en la exploraci�n de archivos. (30658 l�neas)
	Versi�n  5.24.4: 17/mar/2006	Terminada la implementaci�n de copiar-pegar en la exploraci�n de archivos. (30690 l�neas)
	Versi�n  5.24.5: 21/mar/2006	Cambio en el orden de las convocatorias de examen. Ahora se muestran primero las menos futuras. (30692 l�neas)
	Versi�n  5.24.6: 28/mar/2006	Peque�a correcci�n gr�fica para Firefox en listado de �rbol de archivos. (30697 l�neas)
	Versi�n  5.24.7: 28/mar/2006	Nuevo icono para pegar archivos o carpetas. (30786 l�neas)
	Versi�n  5.24.8: 29/mar/2006	Corregido peque�o error en presentaci�n de nuevo icono para pegar archivos o carpetas.
					Corregido bug con nombres de ficheros comenzando o terminando en espacios. (30802 l�neas)
	Versi�n  5.24.9: 03/abr/2006	Subido tama�o m�ximo de archivo de 32 MiB a 48 MiB. (30803 l�neas)
	Versi�n  5.24.10:03/abr/2006	Nuevo grupo de usuarios en estad�sticas: "usuarios identificados". (30811 l�neas)
	Versi�n  5.25:   04/abr/2006	Subido cuota de descarga en asignaturas de 512 MiB a 1 GiB.
					Cambiados algunos mensajes en estad�sticas.
					Se permite que se cierren grupos de alumnos. Un alumno no podr� apuntarse ni borrarse de un grupo cerrado. (30986 l�neas)
	Versi�n  5.25.1: 05/abr/2006	Si todos los grupos de un tipo est�n cerrados o completos, y la adscripci�n a ese tipo de grupo es obligatoria, y el alumno no est� apuntado a ninguno de los grupos de ese tipo, no se le obliga a hacerlo, pues de lo contrario no podr�a acceder a otras opciones al no poder apuntarse a ninguno. (31020 l�neas)
	Versi�n  5.25.2: 06/abr/2006	Para agilizar algunas consultas en la base de datos, se a�ade un campo con el n�mero de alumnos en la tabla de grupos.
					Numerosos cambios internos en altas y bajas de usuarios. (31124 l�neas)
	Versi�n  5.25.3: 07/abr/2006	Se agilizan algunas consultas en la base de datos mediante el nuevo campo en la tabla de grupos que contiene el n�mero de alumnos.
					Cuando no hay grupos de cierto tipo abiertos con vacantes, no se muestra mensaje al alumno indicando que tiene que (o que puede) apuntarse a un grupo de ese tipo. (31202 l�neas)
	Versi�n  5.25.4: 08/abr/2006	Corregido bug en la copia de archivos (se copiaban m�s bytes de la cuenta). (31204 l�neas)
	Versi�n  5.26:   09/abr/2006	Comienza la implementaci�n de zona de archivos com�n para todos los usuarios de una asignatura. (31234 l�neas)
	Versi�n  5.26.1: 10/abr/2006	Contin�a la implementaci�n de zona de archivos com�n para todos los usuarios de una asignatura. (31634 l�neas)
	Versi�n  5.26.2: 10/abr/2006	Finaliza la implementaci�n de zona de archivos com�n para todos los usuarios de una asignatura.
					Ahora no est� permitido crear ni pegar un archivo existente en ning�n �rbol de exploraci�n. (31743 l�neas)
	Versi�n  5.27:   11/abr/2006	Comienza la implementaci�n de zona de archivos com�n para todos los usuarios de un grupo, sugerida por Daniel Calandria. (32036 l�neas)
	Versi�n  5.27.1: 12/abr/2006	Finaliza la implementaci�n de zona de archivos com�n para todos los usuarios de un grupo. (32132 l�neas)
	Versi�n  5.27.2: 17/abr/2006	Corregido c�lculo err�neo del n�m. de alumnos no apuntados a grupos de cierto tipo cuando en ese tipo est� permitido apuntarse a varios grupos. (32168 l�neas)
	Versi�n  5.28:   17/abr/2006	Nueva versi�n del programa de retoque fotogr�fico, totalmente reprogramada en C++ por Daniel Calandria.
					Comienza la implementaci�n de listado de profesores de SWAD (s�lo para administradores). (32206 l�neas)
	Versi�n  5.28.1: 18/abr/2006	Termina la implementaci�n de listado de profesores de SWAD (s�lo para administradores).
					Nueva versi�n del programa de retoque fotogr�fico. Ahora se presenta tambi�n la foto original.
					Ligeras mejoras est�ticas en marcos. (32444 l�neas)
	Versi�n  5.28.2: 19/abr/2006	Ligeras mejoras est�ticas en marcos y en exploraci�n de archivos.
					Se muestra la fecha y el tama�o de las carpetas. (32431 l�neas)
	Versi�n  5.28.3: 19/abr/2006	Codificaci�n en base 64 en lugar de 16 de los ID encriptados para que ocupen 22 caracteres en lugar de 32. (32550 l�neas)
	Versi�n  5.29:   20/abr/2006	Las sesiones se guardan en la base de datos en lugar de en ficheros. (32624 l�neas)
	Versi�n  5.29.1: 21/abr/2006	Para disminuir el tama�o de las p�ginas, y por tanto aumentar la velocidad, algunos par�metros ocultos se guardan en la base de datos en lugar de enviarlos a trav�s del formulario. (32765 l�neas)
	Versi�n  5.29.2: 21/abr/2006	Para disminuir el tama�o de las p�ginas, y por tanto aumentar la velocidad, la pesta�a siguiente no se pasa como par�metro.
					Se muestra el nombre y apellidos de un usuario al pasar sobre su foto incluso en el Firefox. (32772 l�neas)
	Versi�n  5.29.3: 24/abr/2006	Se eliminan de la base de datos los par�metros ocultos cuyas sesiones ya no existan. (32790 l�neas)
	Versi�n  5.30:   24/abr/2006	En Descargas aparecen destacados los archivos nuevos desde la �ltima vez que se entr�.
					Corregido bug en consulta de calificaciones. (32867 l�neas)
	Versi�n  5.30.1: 25/abr/2006	Aparecen destacados en todos los �rboles de exploraci�n los archivos nuevos desde la �ltima vez que se entr� y los recientes. (32912 l�neas)
	Versi�n  5.30.2: 25/abr/2006	Tambi�n aparecen destacadas las carpetas con cambios recientes. (32915 l�neas)
	Versi�n  5.30.3: 28/abr/2006	Se aceptan ID que sean n�meros y tengan una �nica letra en medio (no al final). (32931 l�neas)
	Versi�n  5.30.4: 02/may/2006	Cambiados casi todos los nombres de las acciones para homogeneizarlos.
					Cambio en la comprobaci�n de validez de un ID.
					A�adidos nuevos tipos MIME. (32949 l�neas)
	Versi�n  5.31:   02/may/2006	Los usuarios conectados aparecen en distintos colores seg�n est�n o no accediendo a la asignatura seleccionada. (32945 l�neas)
	Versi�n  5.31.1: 03/may/2006	Comienza la comprobaci�n de las filas de cabecera y pie al recibir un archivo de calificaciones. (32955 l�neas)
	Versi�n  5.32:   03/may/2006	Se averiguan las filas de cabecera y pie al recibir un archivo de calificaciones. (33032 l�neas)
	Versi�n  5.32.1: 03/may/2006	Corregido bug en lectura de ancho y alto de ciertos ficheros JPEG. (33035 l�neas)
	Versi�n  5.32.2: 06/may/2006	Corregido peque�o bug en "Mensajes recibidos". (33035 l�neas)
	Versi�n  5.32.3: 08/may/2006	Insertado anuncio del I Encuentro de Usuarios de SWAD. (33081 l�neas)
	Versi�n  5.33:   09/may/2006	A�adido campo con el c�digo de la titulaci�n en la tabla de accesos.
					Cambiado campo Fecha de tipo TIMESTAMP a FechaHora de tipo DATETIME en la tabla de accesos,
					para evitar cambios de Fecha involuntarios al actualizar la tabla.
					Nuevas estad�sticas por titulaci�n. (33188 l�neas)
	Versi�n  5.33.1: 15/may/2006	A�adido tipo MIME. (33192 l�neas)
	Versi�n  5.34:   16/may/2006	Corregido bug relacionado con los formularios en el navegador Konqueror.
					Numerosos cambios internos en el formato HTML generado en los formularios. (33097 l�neas)
	Versi�n  5.34.1: 22/may/2006	Peque�os cambios est�ticos. (33099 l�neas)
			26/may/2006	 Se celebra el I Encuentro de Usuarios de SWAD.
	Versi�n  5.34.2: 27/may/2006	Eliminado el anuncio del I Encuentro de Usuarios de SWAD. (33100 l�neas)
	Versi�n  5.34.3: 28/may/2006	Comienza la implementaci�n de adscripci�n de alumnos a grupos por parte del profesor. (33298 l�neas)
	Versi�n  5.34.4: 29/may/2006	Contin�a la implementaci�n de adscripci�n de alumnos a grupos por parte del profesor. (33412 l�neas)
	Versi�n  5.34.5: 30/may/2006	Contin�a la implementaci�n de adscripci�n de alumnos a grupos por parte del profesor. (33509 l�neas)
	Versi�n  5.34.6: 31/may/2006	Contin�a la implementaci�n de adscripci�n de alumnos a grupos por parte del profesor. (33533 l�neas)
	Versi�n  5.35:   01/jun/2006	Finalizada la implementaci�n de adscripci�n de alumnos a grupos por parte del profesor.
					Corregido peque�o bug en lista de alumnos cuando hab�a grupos y ning�n alumno se hab�a apuntado a ninguno. (33592 l�neas)
	Versi�n  5.35.1: 01/jun/2006	Cambios internos (en las consultas a la base de datos) en la presentaci�n de grupos. (32584 l�neas)
	Versi�n  5.35.2: 01/jun/2006	M�s cambios internos (en las consultas a la base de datos) en la presentaci�n de grupos. (33528 l�neas)
	Versi�n  5.35.3: 04/jun/2006	M�s cambios internos (en las consultas a la base de datos) en la presentaci�n de grupos. (33494 l�neas)
	Versi�n  5.35.4: 05/jun/2006	Comienza implementaci�n de tipos de respuesta de test num�ricos entero y real. (33680 l�neas)
	Versi�n  5.35.5: 06/jun/2006	Contin�a la implementaci�n de tipos de respuesta de test num�ricos entero y real. (33785 l�neas)
	Versi�n  5.36:   06/jun/2006	Finalizada la implementaci�n de tipos de respuesta de test num�ricos entero y real. (33839 l�neas)
	Versi�n  5.36.1: 07/jun/2006	Diversas mejoras internas en las preguntas con respuestas num�ricas entera y real. (33838 l�neas)
	Versi�n  5.36.2: 09/jun/2006	Las convocatorias de examen tambi�n aparecen resaltadas en los primeros d�as del mes siguiente. (33834 l�neas)
	Versi�n  5.36.3: 09/jun/2006	Nombres de los foros m�s breves (sugerencia de Ra�l Jim�nez Ortega). S�lo se muestran los foros accesibles.
					Se guardan en la tabla de accesos el tiempo de generaci�n y el tiempo de env�o de la p�gina. (33856 l�neas)
	Versi�n  5.37:   11/jun/2006	Se muestran estad�sticas por tiempo de generaci�n y de env�o de p�gina. (33888 l�neas)
	Versi�n  5.37.1: 12/jun/2006	A�adidos nuevos tipos MIME y extensiones de archivos. (33899 l�neas)
	Versi�n  5.37.2: 17/jun/2006	Comienza el traspaso de mensajes y foros de archivos a la base de datos. (33928 l�neas)
	Versi�n  5.37.3: 21/jun/2006	Contin�a el traspaso de mensajes y foros de archivos a la base de datos. (34315 l�neas)
	Versi�n  5.37.4: 22/jun/2006	Contin�a el traspaso de mensajes y foros de archivos a la base de datos. (34451 l�neas)
	Versi�n  5.37.5: 22/jun/2006	Contin�a el traspaso de mensajes y foros de archivos a la base de datos. (34483 l�neas)
	Versi�n  5.37.6: 23/jun/2006	Contin�a el traspaso de mensajes y foros de archivos a la base de datos. (34601 l�neas)
	Versi�n  5.37.7: 24/jun/2006	Contin�a el traspaso de mensajes y foros de archivos a la base de datos. (34700 l�neas)
	Versi�n  5.37.8: 25/jun/2006	Finaliza el traspaso de mensajes y foros de archivos a la base de datos. (34747 l�neas)
	Versi�n  5.37.9: 25/jun/2006	Comienza la implementaci�n de los mensajes y foros en la base de datos. (36312 l�neas)
	Versi�n  5.37.10:25/jun/2006	Contin�a la implementaci�n de los mensajes y foros en la base de datos. (36719 l�neas)
	Versi�n  5.37.11:26/jun/2006	Contin�a la implementaci�n de los mensajes y foros en la base de datos. (36779 l�neas)
	Versi�n  5.37.12:27/jun/2006	Contin�a la implementaci�n de los mensajes y foros en la base de datos. (36907 l�neas)
	Versi�n  5.37.13:27/jun/2006	Contin�a la implementaci�n de los mensajes y foros en la base de datos. (36873 l�neas)
	Versi�n  5.37.14:27/jun/2006	Contin�a la implementaci�n de los mensajes y foros en la base de datos. (37081 l�neas)
	Versi�n  5.37.15:28/jun/2006	Contin�a la implementaci�n de los mensajes y foros en la base de datos. (37444 l�neas)
	Versi�n  5.37.16:28/jun/2006	Contin�a la implementaci�n de los mensajes y foros en la base de datos. (37511 l�neas)
	Versi�n  5.37.17:29/jun/2006	Finaliza la implementaci�n de los mensajes y foros en la base de datos. (37530 l�neas)
	Versi�n  5.38:   29/jun/2006	Se ponen en marcha los avisos, mensajes entre usuarios y foros en la base de datos. (37601 l�neas)
	Versi�n  5.38.1: 29/jun/2006	Corregido peque�o bug al responder a un mensaje desde otra asignatura. (37602 l�neas)
	Versi�n  5.38.2: 30/jun/2006	Cuando se elimina un usuario, se eliminan sus mensajes enviados y recibidos de la base de datos. (37607 l�neas)
	Versi�n  5.39:   30/jun/2006	Las hebras de los foros se muestran paginadas. (37699 l�neas)
	Versi�n  5.39.1: 03/jul/2006	Eliminadas las funciones antiguas de tratamiento de mensajes, foros y avisos en archivos.
					Nuevas funciones temporales para eliminar los archivos con mensajes, foros y avisos.
					Eliminados todos los archivos con mensajes, foros y avisos. (34742 l�neas)
	Versi�n  5.39.2: 03/jul/2006	Eliminadas las funciones temporales para eliminar mensajes, foros y avisos en archivos. (34420 l�neas)
	Versi�n  5.39.3: 05/jul/2006	En la lista de hebras de un foro se muestran enlaces a las distintas p�ginas de mensajes de cada hebra. (34428 l�neas)
	Versi�n  5.39.4: 05/jul/2006	Eliminados los n�meros de hebra seg�n sugerencia de Ra�l Jim�nez Ortega, aunque a�n se mantienen en la base de datos. (34475 l�neas)
	Versi�n  5.39.5: 05/jul/2006	Cambios en el listado de hebras. (34491 l�neas)
	Versi�n  5.39.6: 06/jul/2006	Cambios en la escritura de fechas (aparece la cadena "Hoy"). (34511 l�neas)
	Versi�n  5.39.7: 06/jul/2006	Cambios est�ticos en el marco de la zona principal y las pesta�as. (34512 l�neas)
	Versi�n  5.40:   07/jul/2006	En la base de datos se guarda informaci�n sobre si un mensaje ha sido respondido. (34578 l�neas)
	Versi�n  5.40.1: 08/jul/2006	Aparece un s�mbolo especial en los mensajes que ya han sido respondidos. (34605 l�neas)
	Versi�n  5.40.2: 08/jul/2006	Nueva funci�n para actualizar un mensaje enviado cuando haya sido le�do.
					En los mensajes enviados y recibidos se muestra primero el m�s reciente.
					El icono de los mensajes recibidos nuevos se muestra en verde. (34647 l�neas)
	Versi�n  5.41:   08/jul/2006	Se destacan en negrita las hebras que contienen mensajes no vistos. (34630 l�neas)
	Versi�n  5.41.1: 09/jul/2006	Corregido fallo est�tico en foros detectado por Ra�l Jim�nez Ortega. (34630 l�neas)
	Versi�n  5.41.2: 10/jul/2006	Se destacan en verde los mensajes no vistos de una hebra. (34697 l�neas)
	Versi�n  5.41.3: 11/jul/2006	Insertado anuncio beca predoctoral. (34706 l�neas)
	Versi�n  5.41.4: 17/jul/2006	Corregido bug en ID encriptados de destinatarios de mensajes, detectado por el profesor Germ�n Luz�n Gonz�lez. (34722 l�neas)
	Versi�n  5.41.5: 18/jul/2006	A�adida comprobaci�n de departamentos inexistentes en fichas de profesores. (34729 l�neas)
	Versi�n  5.41.6: 19/jul/2006	Corregido error de c�lculo en presentaci�n de estad�sticas de acceso por d�as, semanas o meses cuando hay filas vac�as.
					Adem�s, ahora aparecen filas vac�as al principio y al final hasta completar el rango de fechas seleccionadas. (34801 l�neas)
	Versi�n  5.41.7: 31/jul/2006	A�adido tipo MIME.
					Se resaltan en negrita los fotos que tienen mensajes nuevos.
					Peque�o cambio en alta/baja de varios alumnos en/de un grupo. (34864 l�neas)
	Versi�n  5.41.8: 04/ago/2006	Corregido peque�o bug relacionado con el c�lculo de la diferencia entre fechas, usado en las estad�sticas. (34875 l�neas)
	Versi�n  5.41.9: 26/sep/2006	Corregido bug relacionado con la baja de todos los alumnos de una asignatura, detectado por la profesora Gracia L�pez Contreras. (34876 l�neas)
	Versi�n  5.41.10:29/sep/2006	Se permite que los ID tengan dos letras consecutivas en medio.
					Aumentados los tama�os m�ximos de archivos y las cuotas. (34880 l�neas)
	Versi�n  6.0:    01/oct/2006	Se cambia al curso 2006-2007.
					A�adidos nuevos departamentos. (34883 l�neas)
	Versi�n  6.0.1:  03/oct/2006	Comienza la implementaci�n de la elecci�n de la fuente de informaci�n de cada apartado de la asignatura. (34936 l�neas)
	Versi�n  6.1:    05/oct/2006	Un profesor puede cambiar los datos b�sicos de la ficha de un alumno. (34998 l�neas)
	Versi�n  6.1.1:  05/oct/2006	Corregido peque�o bug en presentaci�n de la orla. (34999 l�neas)
	Versi�n  6.1.2:  05/oct/2006	Contin�a la implementaci�n de la elecci�n de la fuente de informaci�n de cada apartado de la asignatura. (35124 l�neas)
	Versi�n  6.1.3:  06/oct/2006	A�adida extensi�n de archivo permitida.
					Los iconos de extensi�n de nombres de archivos se almacenan en una carpeta especial. (35129 l�neas)
	Versi�n  6.2:    06/oct/2006	Los profesores pueden enviar fotos de los alumnos.
					A�adidos nuevos tipos MIME. (35297 l�neas)
	Versi�n  6.3:    07/oct/2006	La bibliograf�a, FAQ, enlaces y evaluaci�n aparecen en el centro de la ventana principal (iframe) en lugar de en una ventana nueva. (35413 l�neas)
	Versi�n  6.3.1:  07/oct/2006	Corregido bug al recibir un ID cifrado.
					Se aceptan ID con tres letras al principio o en medio.
					Mejorado el comportamiento del cambio de altura del iframe central.
					Las p�ginas externas tambi�n se muestran en la ventana principal (iframe).
					Leves mejoras est�ticas para el navegador Opera. (35412 l�neas)
	Versi�n  6.3.2:  07/oct/2006	Contin�a la implementaci�n de la elecci�n de la fuente de informaci�n de cada apartado de la asignatura. (35440 l�neas)
	Versi�n  6.3.3:  10/oct/2006	Contin�a la implementaci�n de la elecci�n de la fuente de informaci�n de cada apartado de la asignatura. (35474 l�neas)
	Versi�n  6.3.4:  18/oct/2006	Contin�a la implementaci�n de la elecci�n de la fuente de informaci�n de cada apartado de la asignatura. (35815 l�neas)
	Versi�n  6.3.5:  18/oct/2006	Contin�a la implementaci�n de la elecci�n de la fuente de informaci�n de cada apartado de la asignatura. (35858 l�neas)
	Versi�n  6.3.6:  19/oct/2006	Contin�a la implementaci�n de la elecci�n de la fuente de informaci�n de cada apartado de la asignatura. (35997 l�neas)
	Versi�n  6.4:    19/oct/2006	Termina la implementaci�n de la elecci�n de la fuente de informaci�n de cada apartado de la asignatura. (35827 l�neas)
	Versi�n  6.5:    19/oct/2006	Eliminado el c�digo para la generaci�n de la tabla de la base de datos sobre fuentes de informaci�n de las asignaturas, ya innecesario.
					Desaparecen del men� las opciones para editar la informaci�n de la asignatura. (35769 l�neas)
	Versi�n  6.5.1:  20/oct/2006	A petici�n de algunos profesores de Farmacia, se disminuye a 10 el n�mero de columnas de la orla de env�o de mensajes.
					Se aumenta el n�mero de caracteres del nombre y apellidos de los alumnos en dicha orla. (35771 l�neas)
	Versi�n  6.5.2:  20/oct/2006	Cambio sugerido por Francisco Oca�a: los nombres de los alumnos aparecen ahora en tres filas, una para el nombre, otra para el apellido 1 y otra para el apellido 2. (35766 l�neas)
	Versi�n  6.5.3:  20/oct/2006	En las estad�sticas por titulaci�n o por asignatura, junto a las titulaciones aparece su icono. (35789 l�neas)
	Versi�n  6.5.4:  21/oct/2006	Corregido peque�o bug en lista de alumnos. (35790 l�neas)
	Versi�n  6.5.5:  21/oct/2006	La opci�n para editar la descripci�n de la asignatura se quita del men� y se integra en la de ver la descripci�n de la asignatura. (35801 l�neas)
	Versi�n  6.5.6:  22/oct/2006	La opci�n para editar el horario de la asignatura se quita del men� y se integra en la de ver el horario de la asignatura.
					Se muestra un icono en los enlaces para la vista de impresi�n. (35833 l�neas)
	Versi�n  6.5.7:  22/oct/2006	Desaparece la opci�n del men� para editar las convocatorias, y se integra en la opci�n de ver las convocatorias. (35832 l�neas)
	Versi�n  6.6:    22/oct/2006	Las descripciones de las asignaturas se guardan en la base de datos en lugar de archivos XML.
					Los diferentes tipos de informaci�n de la asignatura: descripci�n, teor�a, pr�cticas, bibliograf�a, FAQ, enlaces y sistema de evaluaci�n se pueden editar como texto. (35967 l�neas)
	Versi�n  6.6.1:  22/oct/2006	Corregido peque�o bug en presentaci�n de informaci�n de la asignatura.
					Eliminado el c�digo para la generaci�n de la tabla de la base de datos con la descripci�n de las asignaturas, ya innecesario. (35826 l�neas)
	Versi�n  6.6.2:  22/oct/2006	A�adidos enlaces a las titulaciones en las estad�sticas por asignatura y por titulaci�n. (35829 l�neas)
	Versi�n  6.6.3:  26/oct/2006	Se puede seleccionar si se muestran estad�sticas de la titulaci�n seleccionada o de todas las titulaciones. (35869 l�neas)
	Versi�n  6.6.4:  26/oct/2006	Peque�o cambio en la selecci�n de estad�sticas de una titulaci�n o de todas las titulaciones.
					A�adidos nuevos tipos MIME.
					Aumentado el tiempo tras el cual se eliminan los enlaces p�blicos a �rboles de exploraci�n (sugerido por A. F. D�az).
					Se permiten cuotas mayores de 4 GiB (se utilizan variables de 64 bits).
					Aumentada la cuota de las asignaturas. (35890 l�neas)
	Versi�n  6.6.5:  26/oct/2006	Se muestran iconos de asignatura, titulaci�n y universidad delante de los nombres de cada foro.
					Mejoras internas en consultas a la base de datos relacionadas con los foros. (35942 l�neas)
	Versi�n  6.6.6:  27/oct/2006	Aumentado el tama�o de iconos de los fotos (sugerencia de Francisco A. Oca�a). (35935 l�neas)
	Versi�n  6.6.7:  30/oct/2006	Nueva versi�n del programa de procesamiento de fotograf�as, realizada por Daniel Calandria.
					Esta versi�n realiza el histograma para la mejora de contraste teniendo en cuenta s�lo los p�xeles de piel. (35937 l�neas)
	Versi�n  6.6.8:  30/oct/2006	A�adido mensaje de ayuda relacionado con el alta de alumnos en grupos. (35940 l�neas)
	Versi�n  6.7:    01/nov/2006	Se pueden dar de baja los alumnos que no est�n en una lista. (36027 l�neas)
	Versi�n  6.7.1:  03/nov/2006	Cambiados mensajes en respuestas de test para que los alumnos no se confundan. (36029 l�neas)
	Versi�n  6.7.2:  04/nov/2006	Nuevo dise�o de las barras de las estad�sticas. (36032 l�neas)
	Versi�n  6.7.3:  06/nov/2006	Comienza la implementaci�n de un nuevo horario que permitir� clases superpuestas. (36706 l�neas)
	Versi�n  6.7.4:  06/nov/2006	Contin�a la implementaci�n de un nuevo horario que permitir� clases superpuestas. (36771 l�neas)
	Versi�n  6.7.5:  07/nov/2006	Contin�a la implementaci�n de un nuevo horario que permitir� clases superpuestas. (36852 l�neas)
	Versi�n  6.7.6:  07/nov/2006	Contin�a la implementaci�n de un nuevo horario que permitir� clases superpuestas. (36847 l�neas)
	Versi�n  6.7.7:  07/nov/2006	Termina la implementaci�n de un nuevo horario que permite clases superpuestas. (36860 l�neas)
	Versi�n  6.8:    07/nov/2006	Eliminado el c�digo de los horarios antiguos. (36231 l�neas)
	Versi�n  6.8.1:  08/nov/2006	Correcci�n de bug y cambios importantes en el c�lculo y dibujo del horario.
					Corregido dise�o de la barra de estad�sticas en tasa de accesos por asignatura. (36238 l�neas)
	Versi�n  6.8.2:  08/nov/2006	Mejoras gr�ficas en el dibujo del horario. (36240 l�neas)
	Versi�n  6.8.3:  08/nov/2006	Mejoras gr�ficas en el dibujo del horario. (36242 l�neas)
	Versi�n  6.8.4:  08/nov/2006	Nueva opci�n de la pesta�a personal para que cada usuario obtenga el horario conjunto de todas sus asignaturas. (36313 l�neas)
	Versi�n  6.9:    11/nov/2006	En el horario de la asignatura se pueden seleccionar los grupos creados por el profesor. (36422 l�neas)
	Versi�n  6.9.1:  12/nov/2006	Corregido bug en eliminaci�n de un grupo y de un tipo de grupo: hasta ahora no se eliminaban las zonas comunes de los grupos afectados.
					Mejoras en la selecci�n de grupos en el horario de la asignatura.
					En el horario de las asignaturas del usuario identificado se muestran s�lo las celdas correspondientes a sus grupos. (36447 l�neas)
	Versi�n  6.10:   14/nov/2006	A�adida nueva estad�stica en la que se muestra el uso distribuido por d�as y por horas. (36724 l�neas)
	Versi�n  6.10.1: 15/nov/2006	Mejoras gr�ficas en la estad�stica en la que se muestra el uso distribuido por d�as y por horas. (36745 l�neas)
	Versi�n  6.10.2: 21/nov/2006	En el horario se permite la posibilidad de mostrar s�lo los grupos del usuario identificado o bien todos los grupos. (36869 l�neas)
	Versi�n  6.10.3: 23/nov/2006	Cambio de nombre de un departamento. (36870 l�neas)
	Versi�n  6.10.4: 29/nov/2006	El d�a actual aparece en el calendario destacado con un marco, en lugar de un color de fondo. (36850 l�neas)
	Versi�n  6.10.5: 30/nov/2006	Se muestra un n�mero indicando la posici�n en el ranking en las estad�sticas por titulaci�n y por asignatura. (36872 l�neas)
	Versi�n  6.11:   30/nov/2006	Comienza la implementaci�n en AJAX del refresco autom�tico de los usuarios conectados. (36931 l�neas)
	Versi�n  6.11.1: 30/nov/2006	Contin�a la implementaci�n en AJAX del refresco autom�tico de los usuarios conectados. (37001 l�neas)
	Versi�n  6.11.2: 05/dic/2006	Mejoras internas en la obtenci�n y actualizaci�n de los usuarios conectados. (37044 l�neas)
	Versi�n  6.11.3: 05/dic/2006	Mejoras internas en la obtenci�n y actualizaci�n de los usuarios conectados. (37065 l�neas)
	Versi�n  6.11.4: 06/dic/2006	Mejoras internas en la obtenci�n y actualizaci�n de los usuarios conectados.
					Contin�a la implementaci�n en AJAX del refresco autom�tico de los usuarios conectados. (37125 l�neas)
	Versi�n  6.12:   06/dic/2006	Separado el programa en varios m�dulos.
					Compilar con:
					gcc swad.c swad_action.c swad_connected.c swad_string.c swad_user.c rijndael.c -o swad -Wall -O2 -lmysqlclient -lz -L/usr/lib/mysql -lm; strip swad
	Versi�n  6.12.1: 07/dic/2006	Nuevos m�dulos para las funciones comunes y para las relacionadas con la base de datos. (37904 l�neas)
					Compilar con:
					gcc swad.c swad_action.c swad_com.c swad_connected.c swad_database.c swad_string.c swad_user.c rijndael.c -o swad -Wall -O2 -lmysqlclient -lz -L/usr/lib/mysql -lm; strip swad
	Versi�n  6.12.2: 07/dic/2006	Se refrescan los usuarios conectados mediante AJAX ejecutando este mismo programa swad. (37928 l�neas)
	Versi�n  6.12.3: 08/dic/2006	Corregido bug que aparec�a al expirar la sesi�n. (37936 l�neas)
	Versi�n  6.12.4: 11/dic/2006	Corregidos bugs relacionados con los usuarios conectados en Netscape 7.2 y en MSIE 5.
					Disminuido el tiempo de refresco de usuarios conectados de 1 minuto a 30 segundos. (37915 l�neas)
	Versi�n  6.12.5: 11/dic/2006	Algunas mejoras en los scripts AJAX para el refresco de los usuarios conectados. (37860 l�neas)
	Versi�n  6.12.6: 12/dic/2006	Un usuario se elimina de la lista de conectados cuando se cierra la �ltima sesi�n de todas en las que estaba conectado simult�neamente, no cuando se cierra una sesi�n cualquiera. (37897 l�neas)
	Versi�n  6.13:   13/dic/2006	Se elimina la tabla de usuarios conectados de la base de datos y se usa s�lo la de sesiones. (37771 l�neas)
	Versi�n  6.13.1: 13/dic/2006	Las tildes no se llevan bien con AJAX en Internet Explorer 5, aunque se indique el charset correcto. Por tanto se ha eliminado el env�o de tildes. (37825 l�neas)
	Versi�n  6.13.2: 13/dic/2006	Se muestra una �nica vez cada usuario conectado, tal y como se hac�a antes de eliminar la tabla de usuarios conectados de la base de datos. (37840 l�neas)
	Versi�n  6.13.3: 14/dic/2006	Peque�o cambio en consulta en la base de datos de los usuarios conectados y otros cambios menores. (37839 l�neas)
	Versi�n  6.13.4: 14/dic/2006	Cambios en los script relacionados con AJAX y los usuarios conectados para que funcionen en Konqueror. (37846 l�neas)
	Versi�n  6.14:   14/dic/2006	De nuevo se crea una tabla de usuarios conectados, para evitar el problema siguiente: si un usuario cierra una sesi�n y siguen abiertas sesiones m�s antiguas, se mostrar�a un tiempo m�s antiguo correspondiente a esas sesiones m�s antiguas.
					Disminuido el tiempo de refresco de usuarios conectados de 30 segundos a 10 segundos. (37954 l�neas)
	Versi�n  6.14.1: 18/dic/2006	Revertido el �ltimo cambio en los script relacionados con AJAX para que no se mostraran errores en versiones muy antiguas de Netscape, ya que no funcionaban en IE5.
					Corregido peque�o bug gr�fico al mostrar el listado de preguntas de test.
					Aumentados los tama�os m�ximos de algunas matrices internas. (37956 l�neas)
	Versi�n  6.14.2: 19/dic/2006	Peque�as mejoras en la presentaci�n de usuarios conectados.
					Se elimina el par�metro con el c�digo de la asignatura de los formularios cuando no se cambia de asignatura (el c�digo se guarda junto con la sesi�n). (37949 l�neas)
	Versi�n  6.14.3: 19/dic/2006	Corregido bug detectado por Javier Fern�ndez Baldomero relacionado con los par�metros ocultos y el refresco de usuarios. (37950 l�neas)
	Versi�n  6.15:   22/dic/2006	Las sesiones se cierran autom�ticamente cuando el navegador no refresca un contador de tiempo (sugerencia de Daniel J. Calandria).
					Ahora, al poco tiempo de cerrar el navegador o abandonar la p�gina de swad, la sesi�n se cierra autom�ticamente. (37929 l�neas)
	Versi�n  6.15.1: 31/dic/2006	Mensaje de felicitaci�n del a�o 2007. (37935 l�neas)
	Versi�n  6.15.2: 02/ene/2007	Peque�a mejora est�tica al mostrar fotos de 12x16 en MSIE. (37932 l�neas)
	Versi�n  6.15.3: 10/ene/2007	Mejorado el acceso a fotograf�as p�blicas: antes un alumno o profesor no pod�a ver las fotos de otro alumno o profesor de la misma asignatura si no se encontraba en ese momento dentro de dicha asignatura. Ahora s�. (37940 l�neas)
	Versi�n  6.16:   10/ene/2007	Nuevo m�dulo interno para lo relacionado con las estad�sticas.
					Compilar con: gcc swad.c swad_action.c swad_com.c swad_connected.c swad_course.c swad_database.c swad_statistic.c swad_string.c swad_user.c rijndael.c -o swad -Wall -O2 -lmysqlclient -lz -L/usr/lib/mysql -lm
					Corregido bug in menu de titulaciones-cursos-asignaturas. (38098 l�neas)
	Versi�n  6.16.1: 11/ene/2007	Nuevas estad�sticas de acceso globales para el usuario identificado.
					Nuevas estad�sticas de acceso globales a la asignatura seleccionada. (38132 l�neas)
	Versi�n  6.16.2: 11/ene/2007	Mejora gr�fica en las estad�sticas de accesos por d�as y horas. (38202 l�neas)
	Versi�n  6.16.3: 12/ene/2007	Mejora gr�fica en las estad�sticas de accesos por d�as y horas. (38219 l�neas)
	Versi�n  6.17:   14/ene/2007	Foros y mensajes: se insertan enlaces en las cadenas que parecen una URL (http://... o https://...). (38474 l�neas)
	Versi�n  6.17.1: 14/ene/2007	Corregido peque�o bug en la inserci�n de enlaces. (38476 l�neas)
	Versi�n  6.17.2: 16/ene/2007	Aumentado el tama�o de algunos campos de la ficha. (38476 l�neas)
	Versi�n  6.17.3: 17/ene/2007	Corregido bug en los enlaces a foros en IE5, detectado por Luis Quesada Torres. (38472 l�neas)
	Versi�n  6.17.4: 17/ene/2007	Corregido bug que permit�a SQL injection. (38478 l�neas)
	Versi�n  6.17.5: 17/ene/2007	Cambiado el orden en el que aparecen listados los foros. (38476 l�neas)
	Versi�n  6.18:   17/ene/2007	Ahora se permite el acceso a los foros de una titulaci�n sin necesidad de haber seleccionado asignatura. (38579 l�neas)
	Versi�n  6.18.1: 18/ene/2007	Simplificada la comprobaci�n de qu� fotos pueden mostrarse. (38574 l�neas)
	Versi�n  6.18.2: 20/ene/2007	Corregido peque�o bug en la edici�n del sistema de evaluaci�n. (38575 l�neas)
	Versi�n  6.18.3: 22/ene/2007	Aumentado el tiempo de refresco de usuarios conectados a 30 segundos.
					Se muestra un margen con sombra en las fotos de usuarios ampliadas (38576 l�neas)
	Versi�n  6.19:   24/ene/2007	Numerosos cambios est�ticos. (38580 l�neas)
	Versi�n  6.19.1: 25/ene/2007	Corregido bug en recepci�n de archivos, detectado por el profesor Juan Carlos de Pablos. (38587 l�neas)
	Versi�n  6.19.2: 26/ene/2007	Mejorada la comprobaci�n de qu� fotos pueden mostrarse. (38586 l�neas)
	Versi�n  6.19.3: 26/ene/2007	En el zoom de las fotos se muestra el nombre. (38618 l�neas)
	Versi�n  6.19.4: 28/ene/2007	Algunos cambios est�ticos. A�adido un bot�n para cerrar la sesi�n. (38620 l�neas)
	Versi�n  6.19.5: 29/ene/2007	Aumentado el tiempo de refresco de usuarios conectados a 60 segundos. (38621 l�neas)
	Versi�n  6.19.6: 29/ene/2007	A�adidos tipos MIME y extensiones de archivos de OpenOffice.
					Adem�s de los mensajes de informaci�n y de error, se a�ade un tercer tipo de mensaje: advertencia. (38660 l�neas)
	Versi�n  6.19.7: 30/ene/2007	Se limita el tama�o de los URL mostrados en mensajes para que no aparezca la barra de desplazamiento horizontal del navegador.
					A�adidas extensiones de archivos de BibTex. (38702 l�neas)
	Versi�n  6.20:   30/ene/2007	Nuevo m�dulo interno para lo relacionado con la exploraci�n de archivos. (38777 l�neas)
					Compilar con: gcc swad.c swad_action.c swad_com.c swad_connected.c swad_course.c swad_database.c swad_file_browser.c swad_statistic.c swad_string.c swad_user.c rijndael.c -o swad -Wall -O2 -lmysqlclient -lz -L/usr/lib/mysql -lm
	Versi�n  6.20.1: 01/feb/2007	Comienza la implementaci�n de la expansi�n/contracci�n de las carpetas en los �rboles de exploraci�n de archivos. (38887 l�neas)
	Versi�n  6.20.2: 02/feb/2007	Contin�a la implementaci�n de la expansi�n/contracci�n de las carpetas en los �rboles de exploraci�n de archivos. (39270 l�neas)
	Versi�n  6.20.3: 02/feb/2007	Contin�a la implementaci�n de la expansi�n/contracci�n de las carpetas en los �rboles de exploraci�n de archivos. (39300 l�neas)
	Versi�n  6.20.4: 02/feb/2007	Contin�a la implementaci�n de la expansi�n/contracci�n de las carpetas en los �rboles de exploraci�n de archivos. (39398 l�neas)
	Versi�n  6.20.5: 03/feb/2007	Contin�a la implementaci�n de la expansi�n/contracci�n de las carpetas en los �rboles de exploraci�n de archivos. (39410 l�neas)
	Versi�n  6.20.6: 03/feb/2007	Contin�a la implementaci�n de la expansi�n/contracci�n de las carpetas en los �rboles de exploraci�n de archivos. (39435 l�neas)
	Versi�n  6.20.7: 04/feb/2007	Optimizada la eliminaci�n de ciertas entradas de las tablas de la base de datos relacionadas con la exploraci�n de archivos. (39348 l�neas)
	Versi�n  6.20.8: 04/feb/2007	Optimizado el renombramiento de ciertas entradas de las tablas de la base de datos relacionadas con la exploraci�n de archivos. (39240 l�neas)
	Versi�n  6.20.9: 04/feb/2007	Contin�a la implementaci�n de la expansi�n/contracci�n de las carpetas en los �rboles de exploraci�n de archivos. (39378 l�neas)
	Versi�n  6.21:   05/feb/2007	Termina la implementaci�n de la expansi�n/contracci�n de las carpetas en los �rboles de exploraci�n de archivos. (39370 l�neas)
	Versi�n  6.21.1: 05/feb/2007	Mejoras gr�ficas en los �rboles de exploraci�n de archivos. (39372 l�neas)
	Versi�n  6.21.2: 05/feb/2007	Comienza el cambio hacia iconos estilo Nuvola. (39458 l�neas)
	Versi�n  6.21.3: 07/feb/2007	Termina el cambio hacia iconos estilo Nuvola. (39485 l�neas)
	Versi�n  6.22:   07/feb/2007	Disponibles para todos los usuarios los nuevos iconos estilo Nuvola. (39439 l�neas)
	Versi�n  6.22.1: 07/feb/2007	Cambio de orden de algunas opciones de los men�s. A�adidos nuevos tipos de archivos. (39435 l�neas)
	Versi�n  6.22.2: 07/feb/2007	En la parte principal aparece un icono y un t�tulo asociados a la acci�n actual. (39445 l�neas)
	Versi�n  6.22.3: 08/feb/2007	Se limita la longitud de enlaces (se puede pulsar sobre ellos) y firmas en los avisos. (39467 l�neas)
	Versi�n  6.22.4: 08/feb/2007	Nuevo dise�o de los men�s de opciones. (39517 l�neas)
	Versi�n  6.22.5: 09/feb/2007	Cambiados los tipos de letra para Linux. Corregida errata en foros. (39518 l�neas)
	Versi�n  6.23:   11/feb/2007	SWAD pasa a un nuevo servidor. (39532 l�neas)
	Versi�n  6.23.1: 13/feb/2007	Cambios internos relacionados con los listados de datos y fichas de usuarios. (39712 l�neas)
	Versi�n  6.24:   14/feb/2007	Se saltan los comentarios HTML en los archivos con tablas de calificaciones. (39664 l�neas)
	Versi�n  6.24.1: 14/feb/2007	Optimizaciones internas en la gesti�n de los archivos con tablas de calificaciones. (39546 l�neas)
	Versi�n  6.24.2: 17/feb/2007	Los tiempos de generaci�n y env�o de p�gina se muestran en segundos, milisegundos o microsegundos seg�n convenga. (39564 l�neas)
	Versi�n  6.24.3: 18/feb/2007	Se puede eliminar la foto personal o la de otro usuario. (39646 l�neas)
	Versi�n  6.24.4: 19/feb/2007	M�nimo cambio en script que ajusta el tama�o del iframe dedicado a FAQ, enlaces, etc.
					Corregido bug en el ajuste de la acci�n actual cuando �sta es refrescar los usuarios conectados. Detectado por el alumno Juan Antonio Ruiz Torres. (39652 l�neas)
	Versi�n  6.24.5: 20/feb/2007	Un administrador puede cambiar la contrase�a de otro usuario aunque no est� situado en una asignatura de ese usuario. (39654 l�neas)
	Versi�n  6.24.6: 20/feb/2007	Aumentado tiempo de refresco de usuarios conectados a 2 minutos. (39655 l�neas)
	Versi�n  6.24.7: 20/feb/2007	El tiempo de refresco var�a en funci�n del n�mero de usuarios conectados. (39678 l�neas)
	Versi�n  6.24.8: 21/feb/2007	A�adido tipo MIME. (39680 l�neas)
	Versi�n  6.24.9: 22/feb/2007	Algunos cambios para que no haya warnings al compilar con -O3 (aunque se sigue compilando con -O2). (39690 l�neas)
					Aumentados algunos par�metros de tama�o en la configuraci�n del servidor MySQL para ganar velocidad.
	Versi�n  6.24.10:26/feb/2007	Un profesor puede darse de baja a s� mismo de una asignatura. (39701 l�neas)
	Versi�n  6.25:   26/feb/2007	Nueva opci�n para el administrador: dar de baja a los usuarios antiguos que no est�n ya en ninguna asignatura ni entran en SWAD. (39836 l�neas)
	Versi�n  6.26:   27/feb/2007	Nueva opci�n para eliminar todos los mensajes recibidos o enviados. (39923 l�neas)
	Versi�n  6.26.1: 01/mar/2007	A�adidos nuevas extensiones de archivos y nuevos tipos MIME. (39930 l�neas)
	Versi�n  6.27:   01/mar/2007	Se pueden mostrar o eliminar los mensajes recibidos o enviados desde una asignatura concreta. (40107 l�neas)
	Versi�n  6.27.1: 01/mar/2007	Por omisi�n aparecen los mensajes enviados o recibidos desde cualquier asignatura. (40086 l�neas)
	Versi�n  6.27.2: 02/mar/2007	A�adido tipo MIME.
					La lista de asignaturas para la selecci�n de mensajes enviados o recibidos se toma de los propios mensajes, no de la lista de asignaturas actuales. (40136 l�neas)
	Versi�n  6.27.3: 02/mar/2007	Cambios en la tabla de la base de datos que contiene los datos principales de los usuarios. (40156 l�neas)
	Versi�n  6.27.4: 04/mar/2007	El nombre p�blico de la foto de cada usuario se almacena en un campo de la base de datos para su uso en el nuevo chat. (40254 l�neas)
	Versi�n  6.27.5: 05/mar/2007	El nombre p�blico de la foto de cada usuario cambia en cada env�o de foto para que no haya necesidad de recargar la p�gina debido a la cach� del navegador. (40284 l�neas)
	Versi�n  6.27.6: 06/mar/2007	El nombre p�blico de la foto de cada usuario se genera cada vez pseudoaleatoriamente en lugar de hacerlo a partir del ID.
					Se elimina el antiguo enlace p�blico a la foto al recibir una nueva. (40295 l�neas)
	Versi�n  6.27.7: 06/mar/2007	Se cifran los identificadores de sesi�n.
					Se usa el est�ndar base64url descrito en el documento RFC 4648 The Base16, Base32, and Base64 Data Encodings, http://tools.ietf.org/html/rfc4648. (40312 l�neas)
	Versi�n  6.27.8: 07/mar/2007	Transici�n suave entre p�ginas en IE similar a la de Firefox. (40315 l�neas)
	Versi�n  6.27.9: 07/mar/2007	Se cambia el separador ";" de destinatarios de correo por ",", debido a que el Thunderbird no adminte los ";". Detectado por Francisco Pelayo.
					Cambio interno en la presentaci�n de usuarios conectados.
					Renombramiento de variables globales. (40317 l�neas)
	Versi�n  6.27.10:07/mar/2007	Las contrase�as se almacenan cifradas en base64url en lugar de base16.
					El m�dulo swad_com pasa a llamarse swad_cryptography (por crytography).
					Compilar con gcc swad.c swad_action.c swad_connected.c swad_course.c swad_cryptography.c swad_database.c swad_file_browser.c swad_statistic.c swad_string.c swad_user.c rijndael.c -o swad -Wall -O2 -lmysqlclient -lz -L/usr/lib64/mysql -lm -s (40236 l�neas)
	Versi�n  6.27.11:08/mar/2007	Corregido peque�o bug reciente relacionado con el refresco de usuarios conectados tras expirar la sesi�n. (40238 l�neas)
	Versi�n  6.27.12:08/mar/2007	Si el navegador no admite iframes, la informaci�n s�lo se muestra en una ventana nueva. (40265 l�neas)
	Versi�n  6.28:   08/mar/2007	Comienza la implementaci�n de la administraci�n de titulaciones. (40296 l�neas)
	Versi�n  6.28.1: 09/mar/2007	Nuevos iconos estilo Nuvola en las pesta�as. (40296 l�neas)
	Versi�n  6.28.2: 12/mar/2007	Enlace a art�culo sobre Netiquette en foros y mensajes. (40316 l�neas)
	Versi�n  6.28.3: 16/mar/2007	A partir de ahora, los profesores ya pueden dar de alta y eliminar a otros profesores en sus asignaturas si estos �ltimos ya eran profesores en SWAD. (40359 l�neas)
	Versi�n  6.28.4: 19/mar/2007	Se fuerza a usar contrase�as m�s seguras. (40428 l�neas)
	Versi�n  6.29:   23/mar/2007	Se pone en marcha el nuevo chat realizado por Carlos Moreno Mu�oz y Ana Bel�n Cara Carmona. (40524 l�neas)
	Versi�n  6.29.1: 26/mar/2007	Modificaci�n en la llamada al nuevo chat. (40530 l�neas)
	Versi�n  6.29.2: 29/mar/2007	A�adidos nuevos tipos de archivos. (40534 l�neas)
	Versi�n  6.30:   10/abr/2007	Orla de profesores de SWAD. (40610 l�neas)
	Versi�n  6.30.1: 11/abr/2007	Orla de profesores de una asignatura, una titulaci�n o SWAD. (40663 l�neas)
	Versi�n  6.30.2: 12/abr/2007	A�adidos dos nuevos tipos de archivos por sugerencia de Joaqu�n Derrac Rus.
					Enlace a ayuda sobre el explorador de archivos. (40677 l�neas)
	Versi�n  6.30.3: 12/abr/2007	Se utiliza un Makefile para la compilaci�n. (40679 l�neas)
	Versi�n  6.31:   17/abr/2007	Comienza la implementaci�n de mensajes a usuarios de otras asignaturas usando sus ID o apodos. (40795 l�neas)
	Versi�n  6.31.1: 17/abr/2007	Nueva versi�n del programa de procesamiento de fotograf�as realizada por Daniel Calandria que implementa la detecci�n de rostros. (40860 l�neas)
	Versi�n  6.31.2: 18/abr/2007	Contin�a la implementaci�n de mensajes a usuarios de otras asignaturas usando sus ID o apodos. (40929 l�neas)
	Versi�n  6.31.3: 20/abr/2007	Contin�a la implementaci�n de mensajes a usuarios de otras asignaturas usando sus ID o apodos. (41005 l�neas)
	Versi�n  6.31.4: 20/abr/2007	Finaliza la implementaci�n de mensajes a usuarios de otras asignaturas usando sus ID o apodos. (41006 l�neas)
	Versi�n  6.31.5: 20/abr/2007	S�lo se permiten comas entre destinatarios de un mensaje, no puntos y comas, por simplicidad y para no tener problemas con caracteres del tipo &xx; (41007 l�neas)
	Versi�n  6.31.6: 25/abr/2007	Cambiados algunos iconos del men� de usuarios.
					Mejorados mensajes de posibilidad u obligatoriedad de adscripci�n a grupos. (40991 l�neas)
	Versi�n  6.31.7: 25/abr/2007	Nuevos m�dulos para generar la foto media y mediana de un conjunto de fotos, realizados por Daniel Calandria.
					Comienza la implementaci�n de la foto media y mediana de un conjunto de usuarios. (40952 l�neas)
	Versi�n  6.31.8: 25/abr/2007	Contin�a la implementaci�n de la foto media y mediana de un conjunto de usuarios. (41022 l�neas)
	Versi�n  6.31.9: 02/may/2007	Contin�a la implementaci�n de la foto media y mediana de un conjunto de usuarios. (41354 l�neas)
	Versi�n  6.31.10:02/may/2007	Contin�a la implementaci�n de la foto media y mediana de un conjunto de usuarios.
					Es necesario imprimir algo en la salida est�ndar cada cierto tiempo para evitar timeout del HTTP. (41360 l�neas)
	Versi�n  6.31.11:03/may/2007	Finaliza la implementaci�n de la foto media y mediana de un conjunto de usuarios. (41505 l�neas)
	Versi�n  6.31.12:03/may/2007	Bajo las fotos de la orla de las titulaciones se muestra el n�mero de alumnos de la titulaci�n. (41651 l�neas)
	Versi�n  6.31.13:04/may/2007	Se puede elegir si el tama�o de las fotos de la orla de las titulaciones es proporcional al n�mero de alumnos, proporcional al n�mero de fotos, proporcional al porcentaje de fotos, o fijo. (41752 l�neas)
	Versi�n  6.31.14:05/may/2007	Corregido bug en la recepci�n de mensajes internos.
					Nueva versi�n del programa de procesamiento de fotograf�as (realizado por Daniel Calandria), que corrige bugs y mejora la velocidad y el uso de memoria. (41757 l�neas)
	Versi�n  6.31.15:08/may/2007	Se permite abrir varias salas del nuevo chat desarrollado por Ana Bel�n Cara Carmona y Carlos Moreno Mu�oz. (41805 l�neas)
	Versi�n  6.31.16:10/may/2007	Nueva versi�n del programa de procesamiento de fotograf�as (realizado por Daniel Calandria), que mejora el reconocimiento y muestra las fotograf�as originales con las caras reconocidas destacadas. (41793 l�neas)
	Versi�n  6.31.17:15/may/2007	Al llamar al nuevo chat se abre una nueva ventana para cada sala. (41792 l�neas)
	Versi�n  6.32:   19/may/2007	A�adidas nuevas salas de chat. Aparecen listadas en el mismo formato que los foros.
					Junto a cada sala de chat aparece el n�mero de usuarios conectados a esa sala. (41888 l�neas)
	Versi�n  6.32.1: 21/may/2007	El administrador puede ver todas las salas de chat que tienen usuarios conectados. (41939 l�neas)
	Versi�n  6.32.2: 22/may/2007	Cuando un usario se identifica, aparece un enlace a env�o de contrase�a si la contrase�a actual no es suficientemente segura, y un enlace a env�o de fotograf�a si el usuario no dispone de fotograf�a. (41949 l�neas)
	Versi�n  6.32.3: 25/may/2007	A�adido tipo MIME. Cambiado icono de pizarra. (41949 l�neas)
	Versi�n  6.33:   28/may/2007	Cambio en la forma de almacenar los descriptores de test en la base de datos. (42031 l�neas)
	Versi�n  6.33.1: 29/may/2007	Comienza el cambio en la forma de acceder a los descriptores de test en la base de datos. (42033 l�neas)
	Versi�n  6.33.2: 04/jun/2007	Contin�a el cambio en la forma de acceder a los descriptores de test en la base de datos. (42151 l�neas)
	Versi�n  6.33.3: 05/jun/2007	Finaliza el cambio en la forma de acceder a los descriptores de test en la base de datos. (42066 l�neas)
	Versi�n  6.34:   05/jun/2007	Se permite prohibir el acceso a preguntas de test con ciertos descriptores.
					En todos los enlaces a formularios aparece un acr�nimo. (42241 l�neas)
	Versi�n  6.35:   06/jun/2007	A�adido formulario para configurar los n�meros m�nimo, por defecto y m�ximo de preguntas en los ex�menes de autoevaluaci�n. (42416 l�neas)
	Versi�n  6.35.1: 07/jun/2007	Se usan los n�meros m�nimo, por defecto y m�ximo de preguntas en la presentaci�n de ex�menes de autoevaluaci�n. (42467 l�neas)
	Versi�n  6.36:   12/jun/2007	A�adido formulario para configurar el tipo de realimentaci�n que se dar� al alumno en los ex�menes de autoevaluaci�n. (42656 l�neas)
	Versi�n  6.37:   14/jun/2007	Se permiten preguntas de test de elecci�n con las opciones desordenadas. (42789 l�neas)
	Versi�n  6.37.1: 14/jun/2007	El profesor puede configurar en cada pregunta de elecci�n entre varias opciones si se pueden desordenar las opciones. (42867 l�neas)
	Versi�n  6.37.2: 15/jun/2007	Mejoras internas en los test de autoevaluaci�n. (42875 l�neas)
	Versi�n  6.37.3: 21/jun/2007	Corregido peque�o bug en selecci�n de grupos. (42876 l�neas)
	Versi�n  6.38:   21/jun/2007	Se incorpora la nueva pizarra creada por Ana Bel�n Cara Carmona y Carlos Moreno Mu�oz, y se elimina la anterior. (42704 l�neas)
	Versi�n  6.39:   26/jun/2007	Nuevo tipo de respuesta de test: texto. (42919 l�neas)
	Versi�n  6.39.1: 26/jun/2007	Eliminado el l�mite que hab�a en el n�mero de test que puede realizar un alumno en cada asignatura. (42908 l�neas)
	Versi�n  6.39.2: 04/jul/2007	Aumentado el tiempo que se guardan las carpetas expandidas de un mes a dos meses. (42909 l�neas)
	Versi�n  6.39.3: 07/jul/2007	Al pulsar sobre un tipo de fuente de informaci�n de una asignatura, dicho tipo se selecciona directamente sin tener que pulsar en un bot�n de env�o del formulario. (42903 l�neas)
	Versi�n  6.39.4: 12/jul/2007	Aumentadas cuotas. (42902 l�neas)
	Versi�n  6.39.5: 16/jul/2007	Corregido peque�o bug en ordenaci�n de lista de usuarios. (42904 l�neas)
	Versi�n  6.40:   02/ago/2007	A�adidas estad�sticas por acci�n. (42961 l�neas)
	Versi�n  6.40.1: 03/sep/2007	Corregido bug en consulta de estad�sticas de acceso detalladas a una asignatura, detectado por Fco. Javier Fern�ndez Baldomero. (42964 l�neas)
	Versi�n  6.40.2: 17/sep/2007	La gr�fica de estad�sticas de acceso por d�as y horas puede mostrarse en tonos de gris. (42988 l�neas)
	Versi�n  6.40.3: 24/sep/2007	Corregido bug en presentaci�n de horarios. (43005 l�neas)
	Versi�n  6.41:   25/sep/2007	El administrador puede ocultar mensajes de foros que no cumplan las normas de netiquette. (43261 l�neas)
	Versi�n  6.41.1: 26/sep/2007	Los profesores tambi�n pueden ocultar mensajes del foro de la asignatura.
					Corregido bug que ten�a lugar cuando se intentaba eliminar un mensaje ya eliminado. (43311 l�neas)
	Versi�n  6.41.2: 30/sep/2007	Corregido bug en lista de asignaturas al ver los mensajes recibidos o enviados. (43362 l�neas)
	Versi�n  7.0:    01/oct/2007	Se cambia al curso 2007-2008. (43362 l�neas)
	Versi�n  7.0.1:  03/oct/2007	Algunas mejoras gr�ficas en edici�n de grupos. A�adido tipo de archivo. (43398 l�neas)
	Versi�n  7.0.2:  13/oct/2007	A�adidos dos tipos MIME de archivos. Cambio en un mensaje de error interno relacionado con las fotograf�as. (43418 l�neas)
	Versi�n  7.0.3:  15/oct/2007	Cambio est�tico en la eliminaci�n de avisos. (43419 l�neas)
	Versi�n  7.0.4:  16/oct/2007	Las zonas comunes de grupos pasan de la pesta�a Usuarios a la pesta�a Asignatura. (43420 l�neas)
	Versi�n  7.1:    16/oct/2007	Si un usuario est� apuntado a un �nico grupo, accede directamente a la zona com�n de ese grupo. (43493 l�neas)
	Versi�n  7.1.1:  17/oct/2007	Las zonas comunes de la asignatura y de los grupos son accesibles desde una �nica opci�n del men�.
					Si no hay grupos en la asignatura o el usuario no est� apunto a ning�n grupo, se accede directamente a la zona com�n de la asignatura.
					A�adido tipo MIME. (43513 l�neas)
	Versi�n  7.1.2:  18/oct/2007	Cambios en la forma de seleccionar las zonas comunes de asignatura y grupos. (43523 l�neas)
	Versi�n  7.1.3:  18/oct/2007	Se recuerda (almacen�ndolo en la base de datos) el grupo de la zona com�n m�s recientemente visitado. (43524 l�neas)
	Versi�n  7.2:    19/oct/2007	Se permiten zonas de descarga por grupos adem�s de la zona de la asignatura. (43924 l�neas)
	Versi�n  7.2.1:  19/oct/2007	Al crear un nuevo grupo, por defecto estar� cerrado.
					Siempre se pide confirmaci�n en la eliminaci�n de un grupo. (43928 l�neas)
	Versi�n  7.2.2:  20/oct/2007	Cambios en la tabla de la base de datos de los grupos de las asignaturas relacionado con los grupos abiertos/cerrados. (43934 l�neas)
	Versi�n  7.3:    20/oct/2007	Pueden habilitarse/inhabilitarse las zonas comunes de cada grupo. (44066 l�neas)
	Versi�n  7.4:    22/oct/2007	Se permiten zonas de calificaciones por grupos. (44454 l�neas)
	Versi�n  7.4.1:  23/oct/2007	Mejora en el refresco mediante AJAX de los usuarios conectados, con la ayuda de Jos� Carlos Calvo Tudela. (44461 l�neas)
	Versi�n  7.4.2:  23/oct/2007	El reloj junto al mes actual pasa de mostrar la hora del cliente a mostrar la hora del servidor. (44451 l�neas)
	Versi�n  7.4.3:  24/oct/2007	Corregido bug en el portapapeles al copiar de una zona de grupo de una asignatura y pegar en otra asignatura (detectado por Francisco A. Oca�a Lara). (44452 l�neas)
	Versi�n  7.4.4:  24/oct/2007	Mejoras internas relacionadas con las zonas de grupos y con el portapapeles. (44510 l�neas)
	Versi�n  7.4.5:  24/oct/2007	Corregido bug en la zona de trabajos de la asignatura (detectado por Mar�a Dolores Ruiz L�pez). (44517 l�neas)
	Versi�n  7.4.6:  24/oct/2007	Cambios internos en la baja de todos los alumnos de una asignatura. (44528 l�neas)
	Versi�n  7.5:    24/oct/2007	El c�digo javascript se mueve a un archivo aparte. (44358 l�neas)
	Versi�n  7.5.1:  25/oct/2007	Se permiten ID con letra al final, aunque internamente se siguen usando y almacenando sin ella. (44389 l�neas)
	Versi�n  7.5.2:  26/oct/2007	Comienza la migraci�n a XHTML 1.0 Transitional.
					Se a�ade el atributo align="left" a todas las celdas que no ten�an alineaci�n para que se vea bien en IE6. (44399 l�neas)
	Versi�n  7.5.3:  27/oct/2007	Contin�a la migraci�n a XHTML 1.0 Transitional. Eliminados todos los <nobr>. (44410 l�neas)
	Versi�n  7.5.4:  28/oct/2007	Contin�a la migraci�n a XHTML 1.0 Transitional. Cambios en la presentaci�n gr�fica de las pesta�as. (44407 l�neas)
	Versi�n  7.5.5:  29/oct/2007	Contin�a la migraci�n a XHTML 1.0 Transitional. (44482 l�neas)
	Versi�n  7.5.6:  30/oct/2007	Contin�a la migraci�n a XHTML 1.0 Transitional. Casi todas las p�ginas generadas se validan correctamente. (44436 l�neas)
	Versi�n  7.5.7:  30/oct/2007	Mejoras en algunos iconos. (44445 l�neas)
	Versi�n  7.5.8:  31/oct/2007	Contin�a la migraci�n a XHTML 1.0 Transitional. Los enlaces (a href="...") a archivos en la zona de descargas se escriben sustituyendo "%20" en lugar de cada espacio. (44494 l�neas)
	Versi�n  7.5.9:  31/oct/2007	Contin�a la migraci�n a XHTML 1.0 Transitional.
					Corregido problema de ordenaci�n en espa�ol detectado por Luis E. Hueli Amador. (44495 l�neas)
	Versi�n  7.5.10: 31/oct/2007	Reemplazados todos los <BR> por <br /> en los mensajes en la base de datos.
					Todas las p�ginas generadas validan XHTML 1.0 Transitional, exceptuando dos detalles:
					- & deber�a ser sustituido por &amp; en todas las URL.
					- los caracteres entre &#128; y &#159 deber�an ser sustituidos por sus equivalentes UNICODE. (44503 l�neas)
	Versi�n  7.5.11: 01/nov/2007	Cambios en algunas tablas de la base de datos.
					Cambios en los formularios para a�adir campos a las fichas, tipos de grupo y grupos. (44497 l�neas)
	Versi�n  7.6:    03/nov/2007	Comienza la edici�n de tipos de titulaciones. (45236 l�neas)
	Versi�n  7.6.1:  03/nov/2007	Termina la edici�n de tipos de titulaciones. (45321 l�neas)
	Versi�n  7.6.2:  04/nov/2007	Comienza la edici�n de titulaciones. (45465 l�neas)
	Versi�n  7.6.3:  04/nov/2007	Contin�a la edici�n de titulaciones. (45745 l�neas)
	Versi�n  7.6.4:  05/nov/2007	Contin�a la edici�n de titulaciones. (45885 l�neas)
	Versi�n  7.6.5:  05/nov/2007	Contin�a la edici�n de titulaciones. (45800 l�neas)
	Versi�n  7.6.6:  05/nov/2007	Contin�a la edici�n de titulaciones. (46035 l�neas)
	Versi�n  7.6.7:  06/nov/2007	Contin�a la edici�n de titulaciones. (46220 l�neas)
	Versi�n  7.6.8:  07/nov/2007	Todos los identificadores de acciones aparecen ahora en ingl�s. (46220 l�neas)
	Versi�n  7.6.9:  07/nov/2007	Termina la edici�n de titulaciones. (46383 l�neas)
	Versi�n  7.6.10: 07/nov/2007	Los campos ID pasan de VARCHAR(16) a CHAR(16) en todas las tablas excepto la tabla de log. (46384 l�neas)
	Versi�n  7.6.11: 08/nov/2007	Optimizaci�n en consultas a la base de datos en los listados de mensajes. (46441 l�neas)
	Versi�n  7.6.12: 08/nov/2007	Los campos con c�digo de asignatura pasan de VARCHAR(16) a CHAR(16) en todas las tablas excepto la tabla de log. (46442 l�neas)
	Versi�n  7.6.13: 08/nov/2007	M�s optimizaciones en consultas a la base de datos en los listados de mensajes. (46480 l�neas)
	Versi�n  7.6.14: 08/nov/2007	Cambios internos en los listados de mensajes. (46476 l�neas)
	Versi�n  7.6.15: 08/nov/2007	Cambios internos en la edici�n de preguntas de test. (46482 l�neas)
	Versi�n  7.6.16: 09/nov/2007	Cambios en la censura de mensajes: ahora, cuando se censura un mensaje, no se muestra el autor ni el asunto. (46490 l�neas)
	Versi�n  7.7:    09/nov/2007	Comienza la edici�n de asignaturas. (46439 l�neas)
	Versi�n  7.7.1:  10/nov/2007	Contin�a la edici�n de asignaturas. (46710 l�neas)
	Versi�n  7.7.2:  11/nov/2007	Los campos VARCHAR(16) en la tabla de log pasan a CHAR(16). (46711 l�neas)
	Versi�n  7.7.3:  11/nov/2007	Contin�a la edici�n de asignaturas. (46954 l�neas)
	Versi�n  7.7.4:  12/nov/2007	Optimizaciones en las consultas de mensajes recibidos y enviados.
					Contin�a la edici�n de asignaturas. (47091 l�neas)
	Versi�n  7.8:    13/nov/2007	Para mejorar la protecci�n de datos, un usuario debe confirmar si desea que se le d� de alta en una asignatura. Sugerido por Jos� Luis Bernier. (47417 l�neas)
	Versi�n  7.8.1:  13/nov/2007	Los alumnos pueden darse de baja de una asignatura. (47427 l�neas)
	Versi�n  7.8.2:  13/nov/2007	Contin�a la edici�n de asignaturas. (47441 l�neas)
	Versi�n  7.8.3:  14/nov/2007	Aparecen visibles el nombre y apellidos de los usuarios que a�n no han confirmado su alta en una asignatura. (47390 l�neas)
	Versi�n  7.8.4:  14/nov/2007	Contin�a la edici�n de asignaturas. (47507 l�neas)
	Versi�n  7.8.5:  14/nov/2007	En el listado de alumnos se indica mediante negritas si cada alumno ha confirmado su alta en las asignatura. (47513 l�neas)
	Versi�n  7.8.6:  18/nov/2007	En el listado de alumnos se indica mediante un icono si cada alumno ha confirmado su alta en las asignatura. (47516 l�neas)
	Versi�n  7.8.7:  19/nov/2007	Contin�a la edici�n de asignaturas. (47577 l�neas)
	Versi�n  7.8.8:  19/nov/2007	Contin�a la edici�n de asignaturas. (47616 l�neas)
	Versi�n  7.8.9:  20/nov/2007	Contin�a la edici�n de asignaturas. (47730 l�neas)
	Versi�n  7.8.10: 20/nov/2007	Contin�a la edici�n de asignaturas. (47809 l�neas)
	Versi�n  7.8.11: 21/nov/2007	Comienza la implementaci�n de dos opciones para importar las titulaciones y las asignaturas a la base de datos. (47842 l�neas)
	Versi�n  7.8.12: 23/nov/2007	A�adida extensi�n de archivo y nuevos tipos MIME. (47852 l�neas)
	Versi�n  7.9:    23/nov/2007	A�adido formulario para cambiar la respuesta secreta usada para recordar la contrase�a. (47997 l�neas)
	Versi�n  7.9.1:  24/nov/2007	Cuando un usuario olvida su contrase�a, se le ofrece una opci�n para ver su pregunta y su respuesta secreta. (48156 l�neas)
	Versi�n  7.9.2:  24/nov/2007	Puesta en marcha de la respuesta secreta. (48130 l�neas)
	Versi�n  7.9.3:  26/nov/2007	Finaliza la importaci�n de titulaciones. (48221 l�neas)
	Versi�n  7.9.4:  26/nov/2007	Finaliza la importaci�n de asignaturas. (48416 l�neas)
	Versi�n  7.9.5:  27/nov/2007	Comienza el paso al nuevo sistema de c�digos num�ricos de titulaciones y asignaturas. (48440 l�neas)
	Versi�n  7.9.6:  28/nov/2007	Cambio interno: movidas muchas funciones de usuarios de swad.c a swad_user.c. (48457 l�neas)
	Versi�n  7.9.7:  28/nov/2007	Cambio interno: movidas m�s funciones de usuarios de swad.c a swad_user.c.
					Cambio interno: nuevo m�dulo swad_timetable (time table) para los horarios. (48531 l�neas)
	Versi�n  7.9.8:  28/nov/2007	En la lista de datos completos de usuarios la primera columna ser� el ID. (48556 l�neas)
	Versi�n  7.9.9:  28/nov/2007	Contin�a el paso al nuevo sistema de c�digos num�ricos de titulaciones y asignaturas. (48591 l�neas)
	Versi�n  7.9.10: 29/nov/2007	Contin�a el paso al nuevo sistema de c�digos num�ricos de titulaciones y asignaturas. (48610 l�neas)
	Versi�n  7.9.11: 29/nov/2007	Contin�a el paso al nuevo sistema de c�digos num�ricos de titulaciones y asignaturas. (48632 l�neas)
	Versi�n  7.9.12: 30/nov/2007	Peque�o cambio en elecci�n de grupos de cara a la validaci�n XHTML. (48633 l�neas)
	Versi�n  7.9.13: 30/nov/2007	Corregido bug en convocatorias detectado por el profesor Luis E. Hueli Amador.
					Cambios internos en la recepci�n de algunos par�metros. (48679 l�neas)
	Versi�n  7.9.14: 01/dic/2007	Cambios internos en la recepci�n de algunos par�metros.
					Movidas funciones de sesiones de swad.c a swad_session.c (48788 l�neas)
	Versi�n  7.9.15: 01/dic/2007	Donde se mostraba el n�mero de conectados, ahora se muestra cu�ntas sesiones hay abiertas y cu�ntos usuarios conectados. (48804 l�neas)
	Versi�n  7.9.16: 02/dic/2007	Los usuarios conectados se desglosan en alumnos y profesores conectados. (48868 l�neas)
	Versi�n  7.9.17: 02/dic/2007	Contin�a el paso al nuevo sistema de c�digos num�ricos de titulaciones y asignaturas. (48863 l�neas)
	Versi�n  7.9.18: 03/dic/2007	Corregido peque�o bug en inserci�n autom�tica de enlaces en URL. (48864 l�neas)
	Versi�n  7.9.19: 06/dic/2007	Se permite que un ID acabe en dos letras. En ese caso, se eliminan las dos letras finales. (48869 l�neas)
	Versi�n  7.9.20: 06/dic/2007	A�adido anuncio de seminario-taller sobre swad. (48903 l�neas)
	Versi�n  7.10:   09/dic/2007	A�adido c�digo de asignatura completo a tabla de asignaturas en la base de datos. (48938 l�neas)
	Versi�n  7.10.1: 09/dic/2007	Contin�a el paso al nuevo sistema de c�digos num�ricos de titulaciones y asignaturas. (49067 l�neas)
	Versi�n  7.10.2: 10/dic/2007	Contin�a el paso al nuevo sistema de c�digos num�ricos de titulaciones y asignaturas. (49191 l�neas)
	Versi�n  7.10.3: 10/dic/2007	Contin�a el paso al nuevo sistema de c�digos num�ricos de titulaciones y asignaturas. (49289 l�neas)
	Versi�n  7.10.4: 10/dic/2007	Contin�a el paso al nuevo sistema de c�digos num�ricos de titulaciones y asignaturas. (49411 l�neas)
	Versi�n  7.11:   10/dic/2007	Eliminado el almacenamiento interno de las titulaciones y las asignaturas dentro de este programa. Ahora s�lo se almacenan en la base de datos. El programa ha pasado de 2253208 bytes a 738792 bytes. (49408 l�neas)
	Versi�n  7.11.1: 10/dic/2007	Eliminadas las funciones dedicadas a la importaci�n de titulaciones y asignaturas. (49246 l�neas)
	Versi�n  7.11.2: 10/dic/2007	Eliminados c�digos antiguos de asignaturas de las tablas de sesiones, de conectados y de portapapeles. (49229 l�neas)
	Versi�n  7.11.3: 11/dic/2007	Eliminados c�digos antiguos de asignaturas de la tabla de horarios de asignaturas. (49242 l�neas)
	Versi�n  7.11.4: 11/dic/2007	Eliminados c�digos antiguos de asignaturas de la tabla de asignaturas-usuarios. (49243 l�neas)
	Versi�n  7.11.5: 11/dic/2007	Eliminados c�digos antiguos de asignaturas de la tabla de archivos comunes.
					Se cambian todas las columnas Path en las tablas de la base de datos para que sean sensibles a may�sculas/min�sculas, para corregir un bug en archivos comunes detectado por Francisco Jos� Garz�n Palomares. (49249 l�neas)
	Versi�n  7.11.6: 11/dic/2007	Eliminados c�digos antiguos de asignaturas de la tabla de tipos de grupo en cada asignatura. (49247 l�neas)
	Versi�n  7.11.7: 11/dic/2007	Eliminados c�digos antiguos de asignaturas de la tabla de archivos de calificaciones. (49246 l�neas)
	Versi�n  7.11.8: 11/dic/2007	Eliminados c�digos antiguos de asignaturas de la tabla de campos de fichas de asignaturas. (49246 l�neas)
	Versi�n  7.11.9: 11/dic/2007	Eliminados c�digos antiguos de asignaturas de la tabla de carpetas expandidas. (49244 l�neas)
	Versi�n  7.11.10:11/dic/2007	Eliminados c�digos antiguos de asignaturas de la tabla de descargas ocultas. (49244 l�neas)
	Versi�n  7.11.11:12/dic/2007	Eliminados c�digos antiguos de asignaturas de las tres tablas relacionadas con los test de autoevaluaci�n. (49242 l�neas)
	Versi�n  7.11.12:12/dic/2007	Eliminados c�digos antiguos de asignaturas de las tablas de convocatorias de examen y de informaci�n de las asignaturas. (49238 l�neas)
	Versi�n  7.11.13:12/dic/2007	Eliminados c�digos antiguos de asignaturas de las tablas de mensajes y avisos. (49234 l�neas)
	Versi�n  7.11.14:12/dic/2007	Eliminados c�digos antiguos de asignaturas de las tablas de hebras y de log. (49221 l�neas)
	Versi�n  7.12:   14/dic/2007	Eliminados c�digos antiguos de asignaturas de las salas de chat.
					Se usan nuevos c�digos num�ricos de titulaci�n y asignatura en los men�s. (49315 l�neas)
	Versi�n  7.12.1: 14/dic/2007	Corregido bug en asignaci�n de cuatrimestres de las asignaturas. (49319 l�neas)
	Versi�n  7.12.2: 14/dic/2007	Cambios internos relacionados con la selecci�n de asignaturas. (49288 l�neas)
	Versi�n  7.12.3: 14/dic/2007	Eliminados c�digos antiguos de asignaturas de la tabla de estad�sticas de titulaciones. (49286 l�neas)
	Versi�n  7.12.4: 15/dic/2007	M�s cambios internos relacionados con c�digos antiguos de titulaciones y asignaturas. (49267 l�neas)
	Versi�n  7.13:   15/dic/2007	Se refresca mediante AJAX el n�mero de mensajes nuevos.
					Eliminada la posibilidad de enviar mensajes desde los usuarios conectados. (49296 l�neas)
	Versi�n  7.13.1: 16/dic/2007	Eliminados campos antiguos de titulaci�n y asignatura de la tabla de log. (49290 l�neas)
	Versi�n  7.14:   16/dic/2007	Movidos todos los directorios antiguos de asignaturas a los nuevos directorios correspondientes a los c�digos num�ricos de las asignaturas. (49310 l�neas)
	Versi�n  7.14.1: 18/dic/2007	Corregido bug en men� de asignaturas, detectado por Eva Mart�nez Ortigosa. (49316 l�neas)
	Versi�n  7.15:   18/dic/2007	Nuevo campo en las fichas para el g�nero, usado para que el programa muestre mensajes no sexistas. (49373 l�neas)
	Versi�n  7.15.1: 19/dic/2007	Corregido peque�o bug en creaci�n de nuevos usuarios detectado por Gabriel Mart�nez Fern�ndez. (49374 l�neas)
	Versi�n  7.15.2: 19/dic/2007	Eliminado el c�digo para los cambios de directorios de asignaturas al nuevo sistema. (49277 l�neas)
	Versi�n  7.15.3: 20/dic/2007	Los administradores pasan a ser superusuarios y se crea un nuevo de tipo de usuario administrador de titulaciones. (49280 l�neas)
	Versi�n  7.15.4: 20/dic/2007	Se avisa a los usuarios que deben cambiar los antiguos enlaces con el par�metro CodAsg por los nuevos con el par�metro CrsCod. (49298 l�neas)
	Versi�n  7.15.5: 21/dic/2007	Corregido bug en el par�metro CrsCod informado por Jos� Luis Ar�stegui.
					Cambios relacionados con los usuarios administradores. (49354 l�neas)
	Versi�n  7.15.6: 22/dic/2007	Corregido bug en alta de profesores informado por Yolanda Arag�n.
					Cambios relacionados con las estad�sticas de acceso. (49386 l�neas)
	Versi�n  7.15.7: 24/dic/2007	Se puede acceder a men�s dentro de una titulaci�n sin elegir asignatura. (49389 l�neas)
	Versi�n  7.15.8: 24/dic/2007	Los tama�os de las fotos en la orla de titulaciones dependen ahora del n�mero de p�xeles en lugar del ancho. (48387 l�neas)
	Versi�n  7.15.9: 07/ene/2008	Cambios relacionados con el renombramiento de asignaturas. (49406 l�neas)
	Versi�n  7.15.10:07/ene/2008	Corregido bug en la creaci�n de una nueva asignatura. (49407 l�neas)
	Versi�n  7.15.11:08/ene/2008	Corregido bug en la creaci�n de una nueva titulaci�n.
					Modificaci�n en la estad�stica de n�mero total de titulaciones. (49442 l�neas)
	Versi�n  7.15.12:10/ene/2008	Es obligatorio especificar el g�nero en la ficha personal. (49443 l�neas)
	Versi�n  7.16:   10/ene/2008	Se limita el intervalo entre dos accesos consecutivos de un alumno al test de una asignatura. (49550 l�neas)
	Versi�n  7.17:   10/ene/2008	Para disminuir el tama�o de la p�gina de edici�n de titulaciones, �sta opci�n se desglosa en dos: edici�n de tipos de titulaci�n y edici�n de titulaciones. (49570 l�neas)
	Versi�n  7.18:   12/ene/2008	Nueva opci�n para buscar asignaturas. (49751 l�neas)
	Versi�n  7.18.1: 14/ene/2008	Mejora en la b�squeda de asignaturas: se buscan las asignaturas que contengan todas las palabras buscadas. (49803 l�neas)
	Versi�n  7.19:   14/ene/2008	Se pueden buscar asignaturas a partir del profesor que las imparte. (49871 l�neas)
	Versi�n  7.19.1: 15/ene/2008	Se pueden buscar asignaturas de la titulaci�n actual. (49932 l�neas)
	Versi�n  7.20:   16/ene/2008	Se almacena en una tabla el estado de cada test de autoevaluaci�n, de modo que un test no pueda ser respondido m�s de una vez. (50074 l�neas)
	Versi�n  7.20.1: 17/ene/2008	En la edici�n de titulaciones no se muestra el c�digo antiguo de la titulaci�n. (50068 l�neas)
	Versi�n  7.20.2: 17/ene/2008	En la edici�n de asignaturas no se muestra el c�digo antiguo de la asignatura.
					Eliminaci�n de algunos c�digos antiguos de titulaciones y asignaturas en la base de datos. (49959 l�neas)
	Versi�n  7.21:   17/ene/2008	En los listados de asignaturas aparece el n�mero de alumnos y el de profesores.
					Cambios internos en la forma de consultar el n�mero de alumnos de una asignatura. (49951 l�neas)
	Versi�n  7.21.1: 17/ene/2008	Para aligerar la edici�n de titulaciones, s�lo aparecen las titulaciones del tipo seleccionado. (49966 l�neas)
	Versi�n  7.21.2: 18/ene/2008	Cambios internos en la edici�n de titulaciones y asignaturas. (49971 l�neas)
	Versi�n  7.22:   21/ene/2008	La lista de profesores es visible por los profesores y se muestra, al igual que la orla de profesores, restringida a la asignatura, a la titulaci�n o a toda la instituci�n. (49981 l�neas)
	Versi�n  7.23:   21/ene/2008	A�adida opci�n para listar los administradores de titulaciones. (50168 l�neas)
	Versi�n  7.24:   21/ene/2008	A�adida opci�n para a�adir un administrador a la titulaci�n actual. (50308 l�neas)
	Versi�n  7.25:   22/ene/2008	A�adida opci�n para eliminar un administrador de la titulaci�n actual. (50594 l�neas)
	Versi�n  7.26:   22/ene/2008	Cambio interno: los punteros a las funciones de cada acci�n se sit�an en la tabla de acciones. (50083 l�neas)
	Versi�n  7.26.1: 23/ene/2008	Algunos cambios relacionados con los administradores de titulaciones. (50093 l�neas)
	Versi�n  7.26.2: 23/ene/2008	Los superusuarios est�n dados de alta en la tabla de administradores de titulaciones como administradores de todas las titulaciones. (50126 l�neas)
	Versi�n  7.27:   24/ene/2008	Un usuario puede elegir entre identificarse como usuario normal (alumno o profesor) o como administrador o superusuario (si ello es posible). (50218 l�neas)
	Versi�n  7.27.1: 24/ene/2008	Numerosos cambios relacionados con los administradores de titulaciones. (50188 l�neas)
	Versi�n  7.28:   24/ene/2008	Eliminada la estad�stica de accesos disribuidos por asignatura, y divididos por el n� de usuarios actuales en cada asignatura.
					Se muestran estad�sticas de acceso de administradores. (50066 l�neas)
	Versi�n  7.28.1: 25/ene/2008	En la edici�n de titulaciones, el n�mero de asignaturas de una titulaci�n se calcula cada vez en lugar de almacenarse en la tabla de titulaciones. (50021 l�neas)
	Versi�n  7.28.2: 25/ene/2008	Mejoras en la eliminaci�n de un tipo de titulaci�n. (50052 l�neas)
	Versi�n  7.28.3: 25/ene/2008	Mejoras en la eliminaci�n de una titulaci�n.
					Numerosos cambios internos en la edici�n de titulaciones y asignaturas. (50171 l�neas)
	Versi�n  7.28.4: 28/ene/2008	Se muestran menos mensajes en las altas y bajas de varios alumnos.
					Eliminado el campo de la tabla de log que indicaba si en un acceso el usuario pertenec�a o no a la asignatura. (50173 l�neas)
	Versi�n  7.28.5: 28/ene/2008	Mejoras en la eliminaci�n de una asignatura. (50186 l�neas)
	Versi�n  7.28.6: 29/ene/2008	Mejoras en la eliminaci�n de una asignatura. (50278 l�neas)
	Versi�n  7.28.7: 29/ene/2008	Mejora en b�squeda de asignaturas.
					Mejoras en la eliminaci�n de una asignatura. (50364 l�neas)
	Versi�n  7.28.8: 29/ene/2008	Al crear o editar una asignatura, se comprueba que el curso es v�lido dentro de su titulaci�n. (50393 l�neas)
	Versi�n  7.28.9: 29/ene/2008	Cambios en los permisos de acceso a los foros por parte de administradores.
					Corregido bug en lista de destinatarios de mensajes entre usuarios. (50352 l�neas)
	Versi�n  7.28.10:30/ene/2008	En la lista de administradores se muestran las titulaciones administradas. (50495 l�neas)
	Versi�n  7.28.11:30/ene/2008	A�adidos permisos para que los administradores den de alta/baja a otros usuarios.
					Peque�os cambios en los listados de datos de usuarios. (50494 l�neas)
	Versi�n  7.29:   30/ene/2008	Los administradores s�lo pueden cambiar una asignatura a otra de las titulaciones que administran, no a cualquier titulaci�n. (50595 l�neas)
	Versi�n  7.29.1: 30/ene/2008	Optimizaci�n en la obtenci�n de los datos de una titulaci�n. (50524 l�neas)
	Versi�n  7.29.2: 30/ene/2008	Optimizaci�n en la obtenci�n de los datos de una asignatura.
					Mejoras en listado de foros disponibles y de salas de chat disponibles. (50519 l�neas)
	Versi�n  7.29.3: 30/ene/2008	Corregido bug relacionado con el refresco del enlace a los mensajes nuevos. (50525 l�neas)
	Versi�n  7.29.4: 30/ene/2008	Mejora en edici�n de asignaturas. (50523 l�neas)
	Versi�n  7.29.5: 30/ene/2008	En la b�squeda de asignaturas se permite dejar los dos campos (asignatura y profesor) en blanco. Ese ese caso se buscan todas las asignaturas. (50521 l�neas)
	Versi�n  7.29.6: 31/ene/2008	Los administradores tienen permiso para acceder a listas de profesores.
					Corregido error de HTML en formulario de cambio de tipo de usuario.
					Por defecto una nueva pregunta de tipo test es de elecci�n �nica. (50539 l�neas)
	Versi�n  7.30:   01/feb/2008	Corregido bug en el c�lculo del n�mero de semanas de un a�o y del n�mero de semana correspondiente a una fecha.
					El antiguo m�dulo swad_course pasa a llamarse swad_degree.
					Creado un nuevo m�dulo swad_date para las funciones relacionadas con fechas. (50624 l�neas)
	Versi�n  7.30.1: 01/feb/2008	Corregido bug al cambiar una asignatura de curso. (50625 l�neas)
	Versi�n  7.31:   01/feb/2008	En la lista de profesores se destacan aquellos que han confirmado su alta en al menos una de las asignaturas en las que est�n dados de alta dentro del rango de asignaturas seleccionado para el listado. (50654 l�neas)
	Versi�n  7.31.1: 01/feb/2008	En la lista de profesores se destacan aquellos que han confirmado su alta en todas las asignaturas en las que est�n dados de alta dentro del rango de asignaturas seleccionado para el listado. (50658 l�neas)
	Versi�n  7.31.2: 03/feb/2008	Corregido bug en comprobaci�n de n�mero de preguntas en la evaluaci�n de un test, detectado por Juan Antonio Aldea Armenteros.
					Mejora en dise�o de los formularios de estad�sticas. (50738 l�neas)
	Versi�n  7.31.3: 05/feb/2008	Varias funciones relacionadas con fechas se han movido al m�dulo de fechas.
					La fecha inicial de las estad�sticas de acceso pasa del 13/12/2004 al 01/01/2005. (50738 l�neas)
	Versi�n  7.31.4: 06/feb/2008	A�adida extensi�n de archivo.
					Corregido peque�o bug en edici�n de asignaturas.
					Corregido peque�o bug en listado de profesores en ninguna asignatura. (50754 l�neas)
	Versi�n  7.31.5: 07/feb/2008	A�adido anuncio de seminario-taller sobre swad. (50758 l�neas)
	Versi�n  7.31.6: 07/feb/2008	Corregido bug en formulario de creaci�n de asignatura.
					Peque�o cambio en listado de datos de usuarios. (50759 l�neas)
	Versi�n  7.32:   09/feb/2008	Cuando se busca un profesor sin rellenar el campo asignatura, se listan todas las asignaturas impartidas por ese profesor. (50892 l�neas)
	Versi�n  7.32.1: 10/feb/2008	Mejora en el listado de profesores encontrados en la opci�n de b�squeda. Ahora aparece, en cada una de las asignaturas de un profesor, un icono indicando si ha aceptado el alta. (50943 l�neas)
	Versi�n  7.32.2: 15/feb/2008	La acci�n "mostrar mensajes recibidos" se desdobla en una acci�n para el men� principal y otra para la cabecera superior.
					Cambios internos en funciones de rango de usuarios en estad�sticas. (50963 l�neas)
	Versi�n  7.32.3: 16/feb/2008	En la opci�n "Estad�sticas > Uso de SWAD" se muestra el n�mero total de asignaturas. (50990 l�neas)
	Versi�n  7.32.4: 19/feb/2008	La creaci�n de archivos zip con trabajos de alumnos se realiza con la m�s baja prioridad. (50991 l�neas)
	Versi�n  7.32.5: 20/feb/2008	Corregido peque�o bug en realizaci�n de ex�menes de autoevaluaci�n. (50996 l�neas)
	Versi�n  7.32.6: 25/feb/2008	SWAD se integra en el CEVUG. Se insertan enlaces al CEVUG. (50998 l�neas)
	Versi�n  7.32.7: 27/feb/2008	Corregido bug en cambio de apodo. (50995 l�neas)
	Versi�n  7.32.8: 28/feb/2008	Cambio en mensaje de error cuando un profesor no puede cambiar la contrase�a de un alumno por no haber aceptado el alta. (51001 l�neas)
	Versi�n  7.32.9: 29/feb/2008	Se obliga a los usuarios a rellenar su correo electr�nico en la ficha. (51007 l�neas)
	Versi�n  7.32.10:01/mar/2008	Corregido bug en estad�sticas de acceso por d�as. (51023 l�neas)
	Versi�n  7.33:   01/mar/2008	Antes se mostraban preguntas de test con alg�n descriptor oculto, siempre que tuvieran al menos un descriptor permitido. Ahora no se muestra una pregunta de test si tiene alguno de sus descriptores oculto. (51038 l�neas)
	Versi�n  7.33.1: 02/mar/2008	Mejora interna en consulta a base de datos para generar ex�menes test de autoevaluaci�n. (51059 l�neas)
	Versi�n  7.34:   02/mar/2008	Reordenaci�n de las opciones de la pesta�a "Titulaci�n". (51060 l�neas)
	Versi�n  7.35:   05/mar/2008	Comienza la implementaci�n de env�o de un correo electr�nico en caso de olvido de contrase�a, gracias al script programado por Antonio F. D�az Garc�a en Python. (51144 l�neas)
	Versi�n  7.35.1: 06/mar/2008	Termina la implementaci�n de env�o de la contrase�a por correo electr�nico. (51138 l�neas)
	Versi�n  7.35.2: 07/mar/2008	Los administradores ya pueden ver la orla de alumnos de asignaturas que no sean la suya. (51144 l�neas)
	Versi�n  7.36:   08/mar/2008	Al listar los foros se indica el n�mero de hebras y mensajes de cada foro. (51243 l�neas)
	Versi�n  7.37:   08/mar/2008	Al listar los mensajes de un foro se muestra el n�mero de mensajes totales de cada usuario en ese foro.
					Eliminado el campo de �ndice de hebra, que ya no se usaba, de la tabla de hebras. (51203 l�neas)
	Versi�n  7.38:   08/mar/2008	Nuevo m�dulo swad_message para todas las funciones relacionadas con mensajes. (51254 l�neas)
	Versi�n  7.38.1: 08/mar/2008	Cambios internos relacionados con variables globales. (51239 l�neas)
	Versi�n  7.39:   10/mar/2008	Se permiten administradores globales. (51246 l�neas)
	Versi�n  7.39.1: 10/mar/2008	Los tel�fonos pasan de 9 d�gitos a 16, ya que el profesor Manuel C�ceres indica que tienen nuevos tel�fonos de 9 d�gitos m�s una extensi�n de 5 d�gitos. (51249 l�neas)
	Versi�n  7.40:   10/mar/2008	Los mensajes de los foros se quitan de la tabla messages (general para todos los mensajes y muy grande) y se pasan a la tabla forum_post (m�s peque�a), para ganar velocidad. (51315 l�neas)
	Versi�n  7.40.1: 11/mar/2008	Corregido bug relacionado con la eliminaci�n de foros al borrar asignaturas y titulaciones. (51348 l�neas)
	Versi�n  7.41:   11/mar/2008	Los avisos se quitan de la tabla messages (general para todos los mensajes y muy grande) y se pasan a la tabla msg_notices (m�s peque�a), para ganar velocidad. (51348 l�neas)
	Versi�n  7.42:   12/mar/2008	Eliminados definitivamente los c�digos antiguos de asignaturas. (51173 l�neas)
	Versi�n  7.42.1: 12/mar/2008	Los administradores de todas las titulaciones pueden actuar como administradores aunque no haya titulaci�n seleccionada, y como tales, pueden cambiarle la contrase�a a cualquier alumno o profesor. (51211 l�neas)
	Versi�n  7.42.2: 26/mar/2008	En cada hebra del foro se muestra el n�mero de lectores distintos de esa hebra. (51250 l�neas)
	Versi�n  7.42.3: 26/mar/2008	En cada hebra del foro se muestra el n�mero de mensajes totales y el n�mero de escritores distintos de esa hebra. (51328 l�neas)
	Versi�n  7.43:   31/mar/2008	Se pueden mover hebras de un foro a otro. (51773 l�neas)
	Versi�n  7.44:   01/abr/2008	A�adidos dos foros sobre SWAD: uno para todos los usuarios y otro para profesores. (51879 l�neas)
	Versi�n  7.44.1: 01/abr/2008	Mejoras en la paginaci�n de hebras relacionadas con el movimiento de hebras. (51898 l�neas)
	Versi�n  7.44.2: 02/abr/2008	Mejoras internas en consulta de hebras de un foro. (51927 l�neas)
	Versi�n  7.44.3: 03/abr/2008	Numerosos cambios en los enlaces a los foros. (51879 l�neas)
	Versi�n  7.44.4: 04/abr/2008	Cambios internos en algunas variables globales. (51879 l�neas)
	Versi�n  7.44.5: 04/abr/2008	Nuevo m�dulo swad_course para todas las funciones relacionadas con la gesti�n de informaci�n de la asignatura seleccionada. (51947 l�neas)
	Versi�n  7.44.5: 04/abr/2008	Nuevo m�dulo swad_course para todas las funciones relacionadas con la gesti�n de informaci�n de la asignatura seleccionada. (51947 l�neas)
	Versi�n  7.45:   04/abr/2008	Los apodos se almacenan en la tabla de datos de los usuarios. (51896 l�neas)
	Versi�n  7.46:   04/abr/2008	Los datos de los profesores se almacenan en la tabla de datos de los usuarios. (51862 l�neas)
	Versi�n  7.47:   04/abr/2008	Los datos de los alumnos se almacenan en la tabla de datos de los usuarios. (51751 l�neas)
	Versi�n  7.47.1: 04/abr/2008	Renombrados los campos de la tabla de la base de datos que almacena los datos de los usuarios. (51752 l�neas)
	Versi�n  7.48:   08/abr/2008	Pesta�as y men�s en 6 idiomas: alem�n, ingl�s, espa�ol, franc�s, italiano y portugu�s. (52442 l�neas)
	Versi�n  7.48.1: 08/abr/2008	Cambios relacionados con la selecci�n del idioma. (52068 l�neas)
	Versi�n  7.48.2: 08/abr/2008	Se puede seleccionar el idioma aunque no haya un usuario identificado. (52137 l�neas)
	Versi�n  7.48.3: 08/abr/2008	Traducidos algunos botones y mensajes. (52203 l�neas)
	Versi�n  7.48.4: 09/abr/2008	Cambios en la traducci�n al italiano propuestos por Nicola Comunale. (52204 l�neas)
	Versi�n  7.48.5: 09/abr/2008	Se muestran estad�sticas del n�mero de usuarios que ha elegido cada idioma. (52266 l�neas)
	Versi�n  7.48.6: 09/abr/2008	Se limita la longitud de nombre y apellidos al mostrar el autor de un mensaje o una hebra. (52267 l�neas)
	Versi�n  7.49:   10/abr/2008	Delante de cada hebra en la que el usuario ha escrito mensajes aparece su foto. (52306 l�neas)
	Versi�n  7.49.1: 10/abr/2008	Peque�as mejoras gr�ficas en los foros.
					Eliminado c�digo antiguo relacionado con el horario. (52274 l�neas)
	Versi�n  7.49.2: 10/abr/2008	Corregido peque�o bug relacionado con el n�mero de mensajes escritos por el usuario en cada hebra.
					Corregido peque�o bug relacionado con acr�nimos en los enlaces de formularios. (52285 l�neas)
	Versi�n  7.49.3: 14/abr/2008	Mejorado mensaje de error relacionado con la tabla de mensajes.
					Insertados dos anuncios de seminarios de swad para profesores. (52314 l�neas)
	Versi�n  7.49.4: 19/abr/2008	Importante optimizaci�n del c�digo de la funci�n encargada de convertir cadenas de caracteres.
					Eliminados anuncios de seminarios. (52319 l�neas)
	Versi�n  7.49.5: 19/abr/2008	Renombrados los campos de la tabla de la base de datos que almacena los usuarios en cada asignatura. (52316 l�neas)
	Versi�n  7.50:   19/abr/2008	La �ltima pesta�a accedida se guarda en la tabla de datos del usuario, no en la tabla de usuarios en cada asignatura. De este modo, ahora no cambia la pesta�a accedida al cambiar de asignatura. (52318 l�neas)
	Versi�n  7.50.1: 20/abr/2008	Corregido bug en lista de alumnos detectado por Pablo J. G�mez L�pez y Luis E. Hueli Amador.
					Movidas varias funciones relacionadas con cadenas de caracteres al m�dulo swad_string. (52272 l�neas)
	Versi�n  7.50.2: 20/abr/2008	Renombrados los campos de las tablas de la base de datos que almacenan los tipos de grupo, los grupos en cada asignatura, y los usuarios en cada grupo. (52268 l�neas)
	Versi�n  7.50.3: 20/abr/2008	Renombradas algunas tablas la base de datos. (52269 l�neas)
	Versi�n  7.51:   21/abr/2008	Comienza la implementaci�n de la identificaci�n de usuarios mediante un c�digo num�rico. (52305 l�neas)
	Versi�n  7.51.1: 22/abr/2008	Contin�a la implementaci�n de la identificaci�n de usuarios mediante un c�digo num�rico.
					Corregido bug relacionado con la eliminaci�n de los mensajes nuevos de un usuario. (52323 l�neas)
	Versi�n  7.51.2: 22/abr/2008	Contin�a la implementaci�n de la identificaci�n de usuarios mediante un c�digo num�rico. (52328 l�neas)
	Versi�n  7.51.3: 22/abr/2008	Corregido importante bug en eliminaci�n de avisos. (52326 l�neas)
	Versi�n  7.51.4: 24/abr/2008	Corregido bug al presentar el enunciado de los test de autoevaluaci�n. (52328 l�neas)
	Versi�n  7.51.5: 27/abr/2008	Contin�a la implementaci�n de la identificaci�n de usuarios mediante un c�digo num�rico. (52327 l�neas)
	Versi�n  7.51.6: 27/abr/2008	Contin�a la implementaci�n de la identificaci�n de usuarios mediante un c�digo num�rico. (52312 l�neas)
	Versi�n  7.51.7: 28/abr/2008	Contin�a la implementaci�n de la identificaci�n de usuarios mediante un c�digo num�rico. (52365 l�neas)
	Versi�n  7.51.8: 28/abr/2008	Contin�a la implementaci�n de la identificaci�n de usuarios mediante un c�digo num�rico. (52376 l�neas)
	Versi�n  7.51.9: 28/abr/2008	Contin�a la implementaci�n de la identificaci�n de usuarios mediante un c�digo num�rico. (52380 l�neas)
	Versi�n  7.51.10:28/abr/2008	Contin�a la implementaci�n de la identificaci�n de usuarios mediante un c�digo num�rico. (52391 l�neas)
	Versi�n  7.51.11:28/abr/2008	Contin�a la implementaci�n de la identificaci�n de usuarios mediante un c�digo num�rico. (52392 l�neas)
	Versi�n  7.51.12:28/abr/2008	Contin�a la implementaci�n de la identificaci�n de usuarios mediante un c�digo num�rico. (52392 l�neas)
	Versi�n  7.51.13:28/abr/2008	Contin�a la implementaci�n de la identificaci�n de usuarios mediante un c�digo num�rico. (52392 l�neas)
	Versi�n  7.51.14:28/abr/2008	Contin�a la implementaci�n de la identificaci�n de usuarios mediante un c�digo num�rico. (52391 l�neas)
	Versi�n  7.51.15:28/abr/2008	Contin�a la implementaci�n de la identificaci�n de usuarios mediante un c�digo num�rico. (52424 l�neas)
	Versi�n  7.51.16:29/abr/2008	Contin�a la implementaci�n de la identificaci�n de usuarios mediante un c�digo num�rico. (52475 l�neas)
	Versi�n  7.51.17:29/abr/2008	Contin�a la implementaci�n de la identificaci�n de usuarios mediante un c�digo num�rico. (52476 l�neas)
	Versi�n  7.51.18:29/abr/2008	Comienza el cambio de par�metro oculto con el ID de otro usuario a c�digo de ese usuario. (52535 l�neas)
	Versi�n  7.51.19:29/abr/2008	Finaliza el cambio de par�metro oculto con el ID de otro usuario a c�digo de ese usuario. (52502 l�neas)
	Versi�n  7.51.20:01/may/2008	M�s cambios internos relacionados con los ID de los usuarios. (52416 l�neas)
	Versi�n  7.51.21:01/may/2008	M�s cambios internos relacionados con los ID de los usuarios. (52305 l�neas)
	Versi�n  7.51.22:02/may/2008	M�s cambios internos relacionados con los ID de los usuarios. (52307 l�neas)
*/
#endif
