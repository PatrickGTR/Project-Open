


static 
    gPlayerRobbedRecently[MAX_PLAYERS];


CMD:rob(playerid, params[])
{
    new 
        targetid = GetClosestPlayerFromPlayer(playerid, 5.0);

    if (!IsPlayerConnected(targetid) || !IsPlayerSpawned(targetid))
        return SendErrorMsg(playerid, "There are no players nearby to rob.");   

    if (gettime() - gPlayerRobbedRecently[playerid] < 30)
        return SendErrorMsg(playerid, "You just robbed someone recently, please try again later");

    if (GetPlayerWallet(playerid) > 0)
    {
        SendMsgF(playerid, COLOR_ORANGE, "** ROB **: "C_WHITE"You failed to rob "C_GREY"%p (%i) "C_WHITE"because he had wallet, run away!", targetid, targetid);
        SendMsgF(targetid, COLOR_ORANGE, "** ROB **: "C_GREY"%p (%i) "C_WHITE"attempted to rob you but failed, chase him! - Wallets Left: %i", playerid, playerid, GetPlayerWallet(playerid));
        return 1;
    }

    new bool:success_rate = RandomChance(65);

    if (!success_rate)
    {
        SendMsgF(playerid, COLOR_ORANGE, "** ROB **: "C_WHITE"You failed to rob "C_GREY"%p (%i) "C_WHITE"run away!", targetid, targetid);
        SendMsgF(targetid, COLOR_ORANGE, "** ROB **: "C_GREY"%p (%i) "C_WHITE"attempted to rob you but failed, chase him!", playerid, playerid);
        return 1;
    }

    new 
        robbedcash = random(8500)+500;

    SendMsgF(playerid, COLOR_ORANGE, "** ROB **: "C_WHITE"You managed to rob "C_GREY"%p (%i) "C_WHITE"and found "C_GREEN"%m "C_WHITE"in his wallet.", targetid, targetid, robbedcash);
    SendMsgF(targetid, COLOR_ORANGE, "** ROB **: "C_WHITE"You have been robbed by "C_GREY"%p (%i) "C_WHITE"and found "C_GREEN"%m "C_WHITE"on your wallet.", playerid, playerid, robbedcash);
    
    GivePlayerTotalRob(playerid, 1);
    GivePlayerTotalBeenRob(targetid, 1);

    GivePlayerMoney(targetid, -robbedcash);
    GivePlayerMoney(playerid, robbedcash);

    gPlayerRobbedRecently[playerid] = gettime();
    return true;
}