// External
#include <a_mysql>
#include <YSI_Visual\y_dialog>
#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_inline>
#include <YSI_Coding\y_hooks>
#include <sscanf2>

// Internal
#include <admin-core>
#include <admin-ban>
#include <admin-spectate>
#include <acmd-level_1>
#include <acmd-level_2>
#include <acmd-level_3>
#include <acmd-level_4>
#include <acmd-level_5>

hook OnGameModeInit() {

    // Initiliase Commands

    // Level 1
    AddAdminCommandToList(ADM_LVL_1, "/a");
    AddAdminCommandToList(ADM_LVL_1, "/acmds");
    AddAdminCommandToList(ADM_LVL_1, "/mute");
    AddAdminCommandToList(ADM_LVL_1, "/unmute");
    AddAdminCommandToList(ADM_LVL_1, "/freeze");
    AddAdminCommandToList(ADM_LVL_1, "/unfreeze");
    AddAdminCommandToList(ADM_LVL_1, "/spec");
    AddAdminCommandToList(ADM_LVL_1, "/specoff");

    // Level 2
    AddAdminCommandToList(ADM_LVL_2, "/goto");
    AddAdminCommandToList(ADM_LVL_2, "/get");

    // Level 3
    AddAdminCommandToList(ADM_LVL_3, "/ban");
    AddAdminCommandToList(ADM_LVL_3, "/unban");

    // Level 4
    AddAdminCommandToList(ADM_LVL_4, "/setvip");

    // Level 5
    AddAdminCommandToList(ADM_LVL_5, "/setadmin");
    AddAdminCommandToList(ADM_LVL_5, "/delacc");
    AddAdminCommandToList(ADM_LVL_5, "/mysqlstats");

    return 1;
}