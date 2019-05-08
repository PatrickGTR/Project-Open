


ACMD:[2]goto(playerid, params[]) {
    new
        targetid,
        Float:tPos[3],
        tVWID,
        tIntID;

    if(isnull(params) || !IsNumeric(params))
        return SendSyntaxMsg(playerid, "/goto <playerid>");

    targetid = strval(params);

    if(!IsPlayerConnected(targetid))
        return SendErrorMsg(playerid, "The player you are trying to goto is invalid.");

    // gathering the targetid data
    tVWID = GetPlayerVirtualWorld(targetid);
    tIntID = GetPlayerInterior(targetid);
    GetPlayerPos(targetid, tPos[0], tPos[1], tPos[2]);

    // setting the targetid data
    SetPlayerPos(playerid, tPos[0], tPos[1], tPos[2]);
    SetPlayerVirtualWorld(playerid, tVWID);
    SetPlayerInterior(playerid, tIntID);

    SendServerMsgF(targetid, "Admin "C_GREY"%p (%i) "C_WHITE"teleported to your location.", playerid, playerid);
    SendServerMsgF(playerid, "You teleported to "C_GREY"%p (%i) "C_WHITE"location.", targetid, targetid);
    return true;
}

ACMD:[2]get(playerid, params[]) {
    new
        targetid,
        Float:tPos[3],
        tVWID,
        tIntID;

    if(isnull(params) || !IsNumeric(params))
        return SendSyntaxMsg(playerid, "/goto <playerid>");

    targetid = strval(params);

    if(!IsPlayerConnected(targetid))
        return SendErrorMsg(playerid, "The player you are trying to get is invalid.");

    // gathering the admin's data data
    tVWID = GetPlayerVirtualWorld(playerid);
    tIntID = GetPlayerInterior(playerid);
    GetPlayerPos(playerid, tPos[0], tPos[1], tPos[2]);

    // setting the targetid data
    SetPlayerPos(targetid, tPos[0], tPos[1], tPos[2]);
    SetPlayerVirtualWorld(targetid, tVWID);
    SetPlayerInterior(targetid, tIntID);

    SendServerMsgF(targetid, "Admin "C_GREY"%p (%i) "C_WHITE"teleported you to his location.", playerid, playerid);
    SendServerMsgF(playerid, "You teleported "C_GREY"%p (%i) "C_WHITE"to your location.", targetid, targetid);
    return 1;
}