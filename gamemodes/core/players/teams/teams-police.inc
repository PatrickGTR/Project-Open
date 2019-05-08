#include <YSI_Data\y_bit>

static
    BitArray:gPlayerHasTicket<MAX_PLAYERS>,
    gPlayerCuffEscapeDelay[MAX_PLAYERS];

CMD:arrest(playerid, params[])
{

    if (!IsPlayerLEO(playerid))
        return SendErrorMsg(playerid, "You are not allowed to use this command. You not in Law Enforcement");

    if (isnull(params))
        return SendSyntaxMsg(playerid, "/ar <playerid>");

    new targetid = strval(params);

    if (targetid == playerid)
       return SendErrorMsg(playerid, "You are not allowed to arrest your own self.");

    if (!IsPlayerConnected(targetid))
       return SendErrorMsg(playerid, "The player you are trying to arrest is not connected.");

    if (!IsPlayerSpawned(targetid))
       return SendErrorMsg(playerid, "The player you are trying to arrest is not spawned.");

    if (GetPlayerSpecialAction(targetid) != SPECIAL_ACTION_CUFFED)
       return SendErrorMsg(playerid, "The player you are trying to arrest is not cuffed.");

    if (!IsPlayerNearPlayer(playerid, targetid, 4.0))
        return SendErrorMsg(playerid, "The player you are trying to arrest is not near you.");

    if (IsPlayerInAnyVehicle(targetid))
        return SendErrorMsg(playerid, "Its impossible to arrest someone whilst inside a vehicle.");

    if (GetPlayerWantedLevel(targetid) <= 5)
        return SendErrorMsg(playerid, "Its impossible to arrest this person, he's wanted level is too low, /ticket him/her.");

    SetPlayerSpecialAction(targetid, SPECIAL_ACTION_NONE);

    new
        jailtime = 0,
        pWantedLVL = GetPlayerWantedLevel(targetid);

    if (5 <= pWantedLVL <= 15)
        jailtime = 120;
    else
        jailtime = pWantedLVL * 2;

    GivePlayerTotalArrest(playerid, 1);
    GivePlayerTotalBeenArrest(targetid, 1);

    SendMsgAllF(COLOR_YELLOW, "** JAIL SENTENCE ** "C_GREY"%p (%i) "C_WHITE"sent "C_GREY"%p (%i) "C_WHITE"to jail for "C_GREY"%i "C_WHITE"seconds.", playerid, playerid, targetid, targetid, jailtime);

    SendPlayerToJail(targetid, jailtime);
    return 1;
}

CMD:loc(playerid, params[])
{
    if (!IsPlayerLEO(playerid))
        return SendErrorMsg(playerid, "You are not allowed to use this command. You not in Law Enforcement");

    if (isnull(params))
        return SendSyntaxMsg(playerid, "/loc <playerid>");

    new targetid = strval(params);

    if (targetid == playerid)
        return SendErrorMsg(playerid, "You are not allowed to locate your own self.");

    if (!IsPlayerConnected( targetid))
        return SendErrorMsg(playerid, "The player you are trying to locate is not connected.");

    if (!IsPlayerSpawned(targetid))
        return SendErrorMsg(playerid, "The player you are trying to locate is not spawned.");

    if (GetPlayerInterior(targetid) > 1) {
        SendMsgF(playerid, COLOR_BLUE, "Location of "C_GREY"%p (%i) "C_BLUE"near "C_GREY"inside an interior", targetid, targetid);
    }
    else {
        SendMsgF(playerid, COLOR_BLUE, "Location of "C_GREY"%p (%i) "C_BLUE"near "C_GREY"%s", targetid, targetid, GetPlayerLocation(playerid));
    }
    return true;
}

CMD:pullover(playerid, params[])
{
    if (!IsPlayerLEO(playerid))
        return SendErrorMsg(playerid, "You are not allowed to use this command. You not in Law Enforcement");

    if (isnull(params))
        return SendSyntaxMsg(playerid, "/pullover <playerid>");

    new targetid = strval(params);

    if (targetid == playerid)
        return SendErrorMsg(playerid, "You are not allowed to pullover your own self.");

    if (!IsPlayerConnected(targetid))
        return SendErrorMsg(playerid, "The player you are trying to pullover is not connected.");

    if (!IsPlayerSpawned(targetid))
        return SendErrorMsg(playerid, "The player you are trying to pullover is not spawned.");

    if (!IsPlayerNearPlayer(playerid, targetid, 4.0))
        return SendErrorMsg(playerid, "The player you are trying to pullover is not near you.");

    SendMsgF(playerid, COLOR_BLUE, "[PULLOVER]: "C_WHITE"You have told %p (%i) to pull over on the side of the road.", targetid, targetid);
    SendMsgF(targetid, COLOR_BLUE, "[PULLOVER]: "C_WHITE"You have been told by %p (%i) to pull over on the side of the road.", playerid, playerid);
    return true;
}


CMD:breakcuff(playerid, params[])
{
    if (GetPlayerSpecialAction(playerid) != SPECIAL_ACTION_CUFFED && !IsPlayerCuffed(playerid))
        return SendErrorMsg(playerid, "You are not cuffed, how is that possible?");

    if (gettime() - gPlayerCuffEscapeDelay[playerid] < 60)
        return SendErrorMsg(playerid, "You just used this command recently, try again later.");

    if (GetPlayerPickLock(playerid) < 1)
        return SendErrorMsg(playerid, "You need a picklock to escape from cuffs.");


    new bool:success_rate = RandomChance(65);
    if (!success_rate)
    {
        gPlayerCuffEscapeDelay[playerid] = gettime();
        SendServerMsg(playerid, "You failed to escape from cuffs");
        return 1;
    }

    gPlayerCuffEscapeDelay[playerid] = gettime();
    SendServerMsg(playerid, "You have managed to picklock the cuffs! Run away from the police!");

    GivePlayerTotalCuffEscape(playerid, 1);

    SetPlayerCuffed(playerid, false);
    return true;
}


CMD:cuff(playerid, params[])
{
    if (!IsPlayerLEO(playerid))
        return SendErrorMsg(playerid, "You are not allowed to use this command. You are not a cop/army/cia.");

    if (isnull(params))
        return SendSyntaxMsg(playerid, "/cuff <playerid>");

    new targetid = strval(params);

    if (targetid == playerid)
        return SendErrorMsg(playerid, "You are not allowed to cuff your own self.");

    if (!IsPlayerConnected(targetid))
        return SendErrorMsg(playerid, "The player you are trying to cuff is not connected.");

    if (!IsPlayerSpawned(targetid))
        return SendErrorMsg(playerid, "The player you are trying to cuff is not spawned.");

    if (!IsPlayerNearPlayer(playerid, targetid, 4.0))
        return SendErrorMsg(playerid, "The player you are trying to cuff is not near you.");

    if (GetPlayerSpecialAction(targetid) == SPECIAL_ACTION_CUFFED && IsPlayerCuffed(targetid))
        return SendErrorMsg(playerid, "The player you are trying to cuff is already cuffed.");

    if (IsPlayerInAnyVehicle(targetid))
        return SendErrorMsg(playerid, "Its impossible to cuff someone whilst inside a vehicle.");

    SetPlayerCuffed(targetid, true);

    SendServerMsgF(playerid, "You successfully cuffed "C_GREY"%p[%i]"C_WHITE". Arrest him before he breaks the cuffs.", targetid, targetid);
    SendServerMsgF(targetid, "You have been cuffed by "C_GREY"%p[%i]"C_WHITE". Break the cuffs before he/she arrest you.", playerid, playerid);
    return 1;
}

CMD:uncuff(playerid, params[])
{
    if (!IsPlayerLEO(playerid))
        return SendErrorMsg(playerid, "You are not allowed to use this command. You are not a cop/army/cia.");

    if (isnull(params))
        return SendSyntaxMsg(playerid, "/uncuff <playerid>");

    new targetid = strval(params);

    if (targetid == playerid)
        return SendErrorMsg(playerid, "You are not allowed to uncuff your own self.");

    if (!IsPlayerConnected(targetid))
        return SendErrorMsg(playerid, "The player you are trying to uncuff is not connected.");

    if (!IsPlayerSpawned(targetid))
        return SendErrorMsg(playerid, "The player you are trying to uncuff is not spawned.");

    if (!IsPlayerNearPlayer(playerid, targetid, 4.0))
        return SendErrorMsg(playerid, "The player you are trying to uncuff is not near you.");

    if (GetPlayerSpecialAction(targetid) != SPECIAL_ACTION_CUFFED && !IsPlayerCuffed(targetid))
        return SendErrorMsg(playerid, "The player you are trying to uncuff does not have cuffs behind his back.");

    if (IsPlayerInAnyVehicle(targetid))
        return SendErrorMsg(playerid, "Its impossible to uncuff someone whilst inside a vehicle.");

    SetPlayerCuffed(targetid, true);

    SendServerMsgF(playerid, "You successfully uncuffed "C_GREY"%p"C_WHITE".", targetid);
    SendServerMsgF(targetid, "You have been uncuffed by "C_GREY"%p. "C_WHITE"You are free to go!", playerid);
    return 1;
}

CMD:payticket(playerid, params[])
{
    if (!Bit_Get(gPlayerHasTicket, playerid))
        return SendErrorMsg(playerid, "No one offered you a ticket.");

    SendMsgF(playerid, COLOR_BLUE, "** TICKET ** "C_WHITE"You have paid "C_GREEN"$5,000 "C_WHITE"for your ticket, you are now clean.");
    SetPlayerWantedLevel(playerid, 0);
    GivePlayerMoney(playerid, -5000);
    Bit_Vet(gPlayerHasTicket, playerid);
    return true;
}

CMD:ticket(playerid, params[])
{
    if (!IsPlayerLEO(playerid))
        return SendErrorMsg(playerid, "You are not allowed to use this command. You are not a cop/army/cia.");

    new
        targetid = GetClosestPlayerFromPlayer(playerid, 5.0);

    if (targetid == INVALID_PLAYER_ID || GetPlayerWantedLevel(targetid) == 0)
        return SendErrorMsg(playerid, "No ticketable players nearby.");

    if (GetPlayerWantedLevel(targetid) >= 5)
        return SendErrorMsg(playerid, "This player wanted level is too high, arrest him!");

    SendMsgF(targetid, COLOR_BLUE, "** TICKET ** "C_GREY"%p (%i) "C_WHITE"issued you a ticket, use "C_GREY"/payticket "C_WHITE"to pay your ticket.", playerid, playerid);
    SendMsgF(playerid, COLOR_BLUE, "** TICKET ** "C_WHITE"You have issued "C_GREY"%p (%i) "C_WHITE"a ticket, hopefully he pays.", targetid, targetid);
    Bit_Let(gPlayerHasTicket, targetid);
    return true;
}


