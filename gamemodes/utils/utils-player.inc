#include <YSI_Data\y_iterate>

stock IsPlayerNearPlayer(playerid, targetid, Float:radius = 2.50) {
    new
        Float: x, Float: y, Float: z;
    GetPlayerPos(targetid, x, y, z);

    if (IsPlayerInRangeOfPoint(playerid, radius, x, y, z)) {
        return 1;
    }
    return 0;
}

PreloadAnimLib(playerid, const animlib[]) {
    return ApplyAnimation(playerid, animlib, "null", 0,0, 0,0, 0,0); 
}

stock GetClosestPlayerFromPlayer(playerid, Float:range = 10000.0) {
    new
        Float:x,
        Float:y,
        Float:z;

    GetPlayerPos(playerid, x, y, z);

    return GetClosestPlayerFromPoint(x, y, z, range, playerid);
}

stock GetClosestPlayerFromPoint(Float:x, Float:y, Float:z, &Float:lowestdistance = 10000.0, exceptionid = INVALID_PLAYER_ID)
{
    new
        Float:px,
        Float:py,
        Float:pz,
        Float:distance,
        closestplayer = -1;

    foreach(new i : Player)
    {
        if (i == exceptionid)
            continue;

        GetPlayerPos(i, px, py, pz);

        distance = Distance(px, py, pz, x, y, z);

        if (distance < lowestdistance)
        {
            lowestdistance = distance;
            closestplayer = i;
        }
    }
    return closestplayer;
}

stock IsPlayerSpawned(playerid) {
    new ply_state = GetPlayerState(playerid);
    return (ply_state != PLAYER_STATE_NONE && ply_state != PLAYER_STATE_WASTED && ply_state != PLAYER_STATE_SPAWNED);
}

stock IsValidPlayerID(playerid)
    return (0 <= playerid < MAX_PLAYERS);

stock IsValidMoneyAmount(amount)
    return (0 <= amount < 9999999);

stock GetPlayerSpeed(playerid) {
    new Float:velocity[4];
    GetPlayerVelocity(playerid,velocity[0], velocity[1], velocity[2]);
    velocity[3] = floatsqroot(floatpower(floatabs(velocity[0]), 2.0) + floatpower(floatabs(velocity[1]), 2.0) + floatpower(floatabs(velocity[2]), 2.0)) * 179.28625;
    return floatround(velocity[3]);
}

stock GetPlayerPacketloss(playerid, &Float:packetloss) {
    
    if(!IsPlayerConnected(playerid)) {
        return 0;
    }

    new nstats[400+1], nstats_loss[20], start, end;
    GetPlayerNetworkStats(playerid, nstats, sizeof(nstats));

    start = strfind(nstats,"packetloss",true);
    end = strfind(nstats,"%",true,start);

    strmid(nstats_loss, nstats, start+12, end, sizeof(nstats_loss));
    packetloss = floatstr(nstats_loss);
    return 1;
}