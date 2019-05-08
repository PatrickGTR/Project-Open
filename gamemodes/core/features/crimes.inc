#include <YSI_Coding\y_hooks>

hook OnPlayerStateChange(playerid, newstate, oldstate)
{
    if (newstate == PLAYER_STATE_DRIVER) {
        new vehicleid = GetPlayerVehicleID(playerid);
        if (IsVehicleCopCar(vehicleid) && GetPlayerClass(playerid) == TEAM_CIVILIAN) {
            SendMsgF(playerid, COLOR_GOLD, "** VEHICLE THEFT ** "C_WHITE"You have stolen a "C_GREY"%v "C_WHITE"from San Fierro Police Department.", GetVehicleModel(vehicleid));
            GivePlayerWantedLevel(playerid, 2, "Vehicle Theft", true);
        }
    }
    return 1;
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    if (killerid != INVALID_PLAYER_ID) {
        if (GetPlayerClass(killerid) == TEAM_CIVILIAN && !IsPlayerLEO(killerid)) {
            GivePlayerWantedLevel(killerid, 4, "Murder", true);
        }
    }
    return 1;    
}

GivePlayerWantedLevel(playerid, level, const crimecommitted[] = EOS, bool:show_msg = false)
{
    SetPlayerWantedLevel(playerid, GetPlayerWantedLevel(playerid) + level);
    UpdatePlayerColour(playerid);

    if(show_msg) {
        SendMsgF(playerid, COLOR_GOLD, "** CRIME ** "C_WHITE"Your wanted level has increased to "C_GREY"%i - "C_WHITE"Crime Committed: "C_GREY"%s", GetPlayerWantedLevel(playerid), crimecommitted);
    }
}

UpdatePlayerColour(playerid)
{
    new
        ply_wantedlevel = GetPlayerWantedLevel(playerid);
    if (ply_wantedlevel == 0)
    {
        /*
        TODO: Implement gang sytem
        if(IsPlayerInGang(playerid)){
            SetPlayerGangColour(playerid);
        }
        else { */
            SetPlayerColor(playerid, COLOR_WHITE);
        //}
    }
    else if (0 < ply_wantedlevel < 4)
    {
        SetPlayerColor(playerid, COLOR_YELLOW);
    }
    else if (4 <= ply_wantedlevel <= 12)
    {
        SetPlayerColor(playerid, COLOR_ORANGE);
    }
    else if (12 < ply_wantedlevel)
    {
        SetPlayerColor(playerid, COLOR_RED);
    }
    return 1;
}