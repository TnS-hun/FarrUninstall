//-----------------------------------------------------------------------
// JrPlugin_MyPlugin
//-----------------------------------------------------------------------


//---------------------------------------------------------------------------
// Header Guard
#ifndef JrPlugin_MyPluginH
#define JrPlugin_MyPluginH
//---------------------------------------------------------------------------



//------------------------------------------------------------------------
// system includes
#include <windows.h>
#include <string>
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// includes generic plugin shell of other headers
#include "JrPlugin_GenericShell.h"
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Inlcude FARR SPECIFIC plugin func definitions (shared with host)
#include "JrPluginFuncs_FARR.h"
//-----------------------------------------------------------------------



















//-----------------------------------------------------------------------
// Plugin defines refered to in JrPlugin_GenericShell.cpp
// IMPORTANT: you need to customize there
//
#define ThisPlugin_DisplayName			"FarrUninstall"
#define ThisPlugin_VersionString		"1.00"
#define ThisPlugin_ReleaseDateString		"Jul 17th, 2007"
#define ThisPlugin_Author			"Miklos \"TnS\" Vegvari <vegvari.miklos@gmail.com>"
#define ThisPlugin_HomepageURL			""
#define ThisPlugin_UpdateURL			""
#define ThisPlugin_ShortDescription		"Uninstall programs."
#define ThisPlugin_LongDescription		"Uninstall programs from Find and Run Robot."
//
// If you support advanced options button, specify caption here, otherwise ""
#define ThisPlugin_AdvConfigString		""
//
// If you support a read me file or help file, specify caption here, otherwise ""
#define ThisPlugin_ReadMeString			"View Help File"
// Used internally to display help file
#define ThisPlugin_ReadMeFile			"Read me.txt"
//
// If this plugin has an icon (recommended), specify icon file in same dir as dll - shown in plugin list
#define ThisPlugin_IconFilename			"FarrUninstall.ico"
//
//-----------------------------------------------------------------------






















//-----------------------------------------------------------------------
// FARR specific stuff
#define ThisPlugin_FARR_DefaultAlias		"un"
#define ThisPlugin_FARR_DefaultRegex		""
#define ThisPlugin_FARR_DefaultRegexFilter	"$$1"
#define ThisPlugin_FARR_DefaultKeywordStr	""
#define ThisPlugin_FARR_DefaultScoreStr		"300"
//-----------------------------------------------------------------------






//-----------------------------------------------------------------------
// external globals defined in the .cpp
extern E_SearchStateT current_searchstate;
extern E_ResultAvailableStateT resultsavailabletype;
extern int numresultsavailable;
extern BOOL current_lockstate;
extern bool isready;
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// FARR: global callback when we want to tell host our search state has changed asynchronously
extern Fp_GlobalPluginCallback_NotifySearchStateChanged callbackfp_notifysearchstatechanged;
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// Forward declarations
void ExecuteCallback_SearchStateChanged();
BOOL DoFarrSearchBegin(const char *searchstring_lc_nokeywords);
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// Our plugin-specific functions called by generic plugin shell
BOOL MyPlugin_DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved);
//
BOOL MyPlugin_DoInit();
BOOL MyPlugin_DoShutdown();
BOOL MyPlugin_GetStrVal(const char* varname,char *destbuf, int maxlen);
BOOL MyPlugin_SetStrVal(const char* varname, void *val);
BOOL MyPlugin_SupportCheck(const char* testname, int version);
BOOL MyPlugin_DoAdvConfig();
BOOL MyPlugin_DoShowReadMe();
BOOL MyPlugin_SetState(int newstateval);
//-----------------------------------------------------------------------





//-----------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------


