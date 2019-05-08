#include <YSI_Coding\y_hooks>
#include <strlib>

static
    PlayerText: plyWantedLevel[MAX_PLAYERS];

hook OnPlayerConnect(playerid)
{
    plyWantedLevel[playerid] = CreatePlayerTextDraw   (playerid,600.000000, 109.000000, "7");
    PlayerTextDrawAlignment                 (playerid, plyWantedLevel[playerid], 2);
    PlayerTextDrawBackgroundColor           (playerid, plyWantedLevel[playerid], TD_OUTLINE_COLOUR);
    PlayerTextDrawFont                      (playerid, plyWantedLevel[playerid], 1);
    PlayerTextDrawLetterSize                (playerid, plyWantedLevel[playerid], 0.230000, 0.899999);
    PlayerTextDrawColor                     (playerid, plyWantedLevel[playerid], -1872621313);
    PlayerTextDrawSetOutline                (playerid, plyWantedLevel[playerid], 1);
    PlayerTextDrawSetProportional           (playerid, plyWantedLevel[playerid], 1);
    PlayerTextDrawSetSelectable             (playerid, plyWantedLevel[playerid], 0);
}

hook OnPlayerDisconnect(playerid, reason)
{
    PlayerTextDrawHide(playerid, plyWantedLevel[playerid]);
}

UpdatedPlayerWantedLevelTD(playerid, level)
{
    if(level == 0)
    {
        PlayerTextDrawHide(playerid, plyWantedLevel[playerid]);
    }
    else
    {
        PlayerTextDrawHide(playerid, plyWantedLevel[playerid]);
        PlayerTextDrawShow(playerid, plyWantedLevel[playerid]);
    }
    PlayerTextDrawSetString(playerid, plyWantedLevel[playerid], sprintf("%i", level));
}

stock ALS_SetPlayerWantedLevel(playerid, level)
{
    UpdatedPlayerWantedLevelTD(playerid, level);
    // TOODO: save wanted level when function is called
    //UpdatePlayerWantedLevelMySQL(playerid, level);
    return SetPlayerWantedLevel(playerid, level);
}
#if defined _ALS_SetPlayerWantedLevel
    #undef SetPlayerWantedLevel
#else
    #define _ALS_SetPlayerWantedLevel
#endif

#define SetPlayerWantedLevel ALS_SetPlayerWantedLevel