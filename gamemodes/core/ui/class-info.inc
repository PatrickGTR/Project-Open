#include <YSI_Coding\y_hooks>

static 
    PlayerText:InfoHead,
    PlayerText:ClassInfo;

hook OnPlayerConnect(playerid)
{
    InfoHead = CreatePlayerTextDraw     (playerid, 320, 305, "_");
    PlayerTextDrawLetterSize            (playerid, InfoHead, 0.376998, 1.699555);
    PlayerTextDrawTextSize              (playerid, InfoHead, 866.000000, 336.622436);
    PlayerTextDrawAlignment             (playerid, InfoHead, 2);
    PlayerTextDrawColor                 (playerid, InfoHead, -1);
    PlayerTextDrawUseBox                (playerid, InfoHead, true);
    PlayerTextDrawBoxColor              (playerid, InfoHead, 80);
    PlayerTextDrawSetShadow             (playerid, InfoHead, 0);
    PlayerTextDrawSetOutline            (playerid, InfoHead, 1);
    PlayerTextDrawBackgroundColor       (playerid, InfoHead, TD_OUTLINE_COLOUR);
    PlayerTextDrawFont                  (playerid, InfoHead, 2);
    PlayerTextDrawSetProportional       (playerid, InfoHead, 1);

    ClassInfo = CreatePlayerTextDraw    (playerid, 320, 326, "_");
    PlayerTextDrawLetterSize            (playerid, ClassInfo, 0.280499, 1.425778);
    PlayerTextDrawTextSize              (playerid, ClassInfo, 53.500000, 336.622314);
    PlayerTextDrawAlignment             (playerid, ClassInfo, 2);
    PlayerTextDrawColor                 (playerid, ClassInfo, -1);
    PlayerTextDrawUseBox                (playerid, ClassInfo, true);
    PlayerTextDrawBoxColor              (playerid, ClassInfo, 80);
    PlayerTextDrawSetShadow             (playerid, ClassInfo, 0);
    PlayerTextDrawSetOutline            (playerid, ClassInfo, 1);
    PlayerTextDrawBackgroundColor       (playerid, ClassInfo, TD_OUTLINE_COLOUR);
    PlayerTextDrawFont                  (playerid, ClassInfo, 1);
    PlayerTextDrawSetProportional       (playerid, ClassInfo, 1);
}

hook OnPlayerSpawn(playerid)
{
    HideClassTextdraw(playerid);
}

HideClassTextdraw(playerid)
{
    PlayerTextDrawHide(playerid, InfoHead);
    PlayerTextDrawHide(playerid, ClassInfo);
    return 1;
}

ShowClassTextdraw(playerid, teamcolor, const teamname[], const teaminfo[])
{
    PlayerTextDrawSetString(playerid, InfoHead, teamname);
    PlayerTextDrawColor(playerid, InfoHead, teamcolor);

    PlayerTextDrawSetString(playerid, ClassInfo, teaminfo);
    
    PlayerTextDrawShow(playerid, InfoHead);
    PlayerTextDrawShow(playerid, ClassInfo);
    return 1;
}