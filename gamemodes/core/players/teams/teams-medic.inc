#include <YSI_Coding\y_hooks>
#include <YSI_Data\y_bit>

static 
            cure_Cooldown[MAX_PLAYERS],
            cure_OfferedBy[MAX_PLAYERS],
BitArray:   cure_HasBeenOffered<MAX_PLAYERS>;


hook OnPlayerConnect(playerid ){
    
    cure_Cooldown[playerid] = -1;
    Bit_Set(cure_HasBeenOffered, playerid, false);
    cure_OfferedBy[playerid] = INVALID_PLAYER_ID;
}

CMD:cure(playerid, params[]) {
    if(IsPlayerMedic(playerid))
        return SendErrorMsg(playerid, "You need to be a medic to use this command.");

    if(cure_Cooldown[playerid] < gettime())
        return SendErrorMsg(playerid, "Please wait before using the command...");

    if(isnull(params) || !IsNumeric(params))
        return SendErrorMsg(playerid, "/cure <playerid>");
    new 
        targetid = strval(params);

    if(!IsPlayerConnected(playerid))
        return SendErrorMsg(playerid, "Player is not connected");

    if(!IsPlayerInfected(targetid))
        return SendErrorMsg(playerid, "Player is not infected with any dieases.");

    SendMsgF(targetid, COLOR_GREEN, "[CURE] "C_WHITE"Paramedic "C_GREY"%p (%i) "C_WHITE"has offered to cure your disease for "C_GREEN"$5,000"C_WHITE", use /acceptcure.", playerid, playerid);
    SendMsgF(playerid, COLOR_GREEN, "[CURE] "C_WHITE"You have offered "C_GREY"%p (%i) "C_WHITE"to cure his diseases.", playerid, playerid);

    cure_Cooldown[playerid] = gettime() + 60; 
    cure_OfferedBy[targetid] = playerid;
    Bit_Set(cure_HasBeenOffered, targetid, true);
    return 1;
}

CMD:acceptcure(playerid, params) {
    
    if(!Bit_Get(cure_HasBeenOffered, playerid) || cure_OfferedBy[playerid] == INVALID_PLAYER_ID)
        return SendErrorMsg(playerid, "No one has offered to cure you.");

    if(GetPlayerMoney(playerid) < 5000)
        return SendErrorMsg(playerid, "You don't have enough money to be cured.");

    SendMsgF(playerid, COLOR_GREEN, "[CURE] "C_WHITE"%p (%i) has cured you, you're now free from diseases, stay safe!", cure_OfferedBy[playerid], cure_OfferedBy[playerid]);
    SendMsgF(playerid, COLOR_GREEN, "[CURE] "C_WHITE"%p (%i) has accepted your cure offer, take "C_GREEN"$5,000 "C_WHITE"as a reward", playerid, playerid);

    GivePlayerMoney(playerid, -5000);
    GivePlayerMoney(cure_OfferedBy[playerid], 5000);

    Bit_Set(cure_HasBeenOffered, playerid, false);
    cure_OfferedBy[playerid] = INVALID_PLAYER_ID;
    GivePlayerDisease(playerid, DISEASE_NONE);
    return 1;
}