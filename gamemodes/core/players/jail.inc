static const Float:jailCoordinate[][] =
{
//  X          Y         Z         Angle
    {215.6494, 110.1481, 999.0156, 359.7295},
    {219.5778, 110.1481, 999.0156, 359.7295},
    {223.5972, 110.1481, 999.0156, 359.7295},
    {227.4928, 110.1481, 999.0156, 359.7295}
};

static
    playerTotalTime[MAX_PLAYERS],
    Timer:playerJailTime[MAX_PLAYERS];

SetPlayerJailTime(playerid, jailtime)
{
    if(jailtime == 0) {
        stop playerJailTime[playerid];
    }

    return playerTotalTime[playerid] = jailtime;
}

IsPlayerInJail(playerid)
    return (playerTotalTime[playerid] >= 1) ? (1) : (0);



SendPlayerToJail(playerid, jailtime)
{
    new
        rand = random(sizeof(jailCoordinate));

    ResetPlayerWeapons(playerid);

    SetPlayerVirtualWorld(playerid, 10);
    SetPlayerInterior(playerid, 10);

    SetPlayerPos(playerid, jailCoordinate[rand][0], jailCoordinate[rand][1], jailCoordinate[rand][2]);
    SetPlayerFacingAngle(playerid, jailCoordinate[rand][3]);
    SetCameraBehindPlayer(playerid);

    playerTotalTime[playerid] =  jailtime;
    playerJailTime[playerid] = repeat UpdateJailTime(playerid);
    return 1;
}

timer UpdateJailTime[1000](playerid)
{
    if (playerTotalTime[playerid] > 0)
    {
        -- playerTotalTime[playerid];
    }
    else
    {
        SendServerMsg(playerid, "You have been released from San Fierro jail, you are free!");
        SendMsgAllF(COLOR_YELLOW, "[JAIL SENTENCE]: "C_GREY"%p (%i) "C_WHITE"has been released from San Fierro jail he/she served his time.", playerid, playerid);

        SetPlayerPos(playerid, 217.8541, 117.9176, 999.0156);
        SetPlayerFacingAngle(playerid, 3.7652);
        SetCameraBehindPlayer(playerid);
        stop playerJailTime[playerid];
    }
}