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
    AddAdminCommandToList(E_ADM_LVL_1, "/a");
    AddAdminCommandToList(E_ADM_LVL_1, "/acmds");
    AddAdminCommandToList(E_ADM_LVL_1, "/mute");
    AddAdminCommandToList(E_ADM_LVL_1, "/unmute");
    AddAdminCommandToList(E_ADM_LVL_1, "/freeze");
    AddAdminCommandToList(E_ADM_LVL_1, "/unfreeze");
    AddAdminCommandToList(E_ADM_LVL_1, "/spec");
    AddAdminCommandToList(E_ADM_LVL_1, "/specoff");

    // Level 2
    AddAdminCommandToList(E_ADM_LVL_2, "/goto");
    AddAdminCommandToList(E_ADM_LVL_2, "/get");

    // Level 3
    AddAdminCommandToList(E_ADM_LVL_3, "/ban");
    AddAdminCommandToList(E_ADM_LVL_3, "/unban");

    // Level 4
    AddAdminCommandToList(E_ADM_LVL_4, "/setvip");

    // Level 5
    AddAdminCommandToList(E_ADM_LVL_5, "/setadmin");
    AddAdminCommandToList(E_ADM_LVL_5, "/delacc");
    AddAdminCommandToList(E_ADM_LVL_5, "/mysqlstats");

    return 1;
}