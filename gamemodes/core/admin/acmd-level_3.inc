
/* 
     TODO: Revise this command for which admin level.
ACMD:[3]agivemoney(playerid, params[])
{
    new 
        targetid, 
        money;     

    if(sscanf(params, "ri", targetid, money))
        return SendSyntaxMsg(playerid, "/agivemoney <playerid> <Amount>");

    if(targetid == INVALID_PLAYER_ID)
        return SendErrorMsg(playerid, "The player you are trying to give money is not online.");

    if(!IsPlayerSpawned(targetid))
        return SendErrorMsg(playerid, "The player you are trying to give money is not spawned.");

    if(targetid == playerid)
        return SendErrorMsg(playerid, "You are not allowed to give your self money. (To avoid abuse)");

    if(!IsValidMoneyAmount(money))
        return SendErrorMsg(playerid, "Invalid amount of money 1 - 99999999");

    GivePlayerMoney(targetid, money);

    SendServerMsgF(playerid, "You gave "C_GREY"%p(%i) "C_WHITE"an amount of "C_GREEN"%m", targetid, targetid, money);
    SendServerMsgF(targetid, "Admin "C_GREY"%p(%i) "C_WHITE"gave you an amount of "C_GREEN"%m", playerid, playerid, money);
    
    SendAdminMessageF(""C_GREY"%p(%i) "C_WHITE"gave "C_GREY"%p(%i) "C_WHITE"an amount of "C_GREEN"%m", playerid, playerid, targetid, targetid, money);
    return true;
}

ACMD:[3]agivescore(playerid, params[])
{
    new 
        targetid, 
        score;

    if(sscanf(params, "ri", targetid, score))
        return SendSyntaxMsg(playerid, "/agivemoney <playerid> <Amount>");

    if(targetid == INVALID_PLAYER_ID)
        return SendErrorMsg(playerid, "The player you are trying to give score is not online.");

    if(!IsPlayerSpawned(targetid))
        return SendErrorMsg(playerid, "The player you are trying to give score is not spawned.");

    if(targetid == playerid)
        return SendErrorMsg(playerid, "You are not allowed to give your self score. (To avoid abuse)");

    SetPlayerScore(targetid, GetPlayerScore(targetid) + score);
    
    SendServerMsgF(playerid, "You gave %p(%i) %i score.", targetid, targetid, score);
    SendServerMsgF(targetid, "Admin "C_GREY"%p(%i) "C_WHITE"gave you %i score.", playerid, playerid, score);
    
    SendAdminMessageF(""C_GREY"%p(%i) "C_WHITE"gave "C_GREY"%p(%i) "C_WHITE"%i score", playerid, playerid, targetid, targetid, score);
    return true;
} */

ACMD:[3]ban(playerid, params[])
{
    if (isnull(params) || IsNumeric(params))
        return SendClientMessage(playerid, COLOR_YELLOW, "/ban hour | month | year");


    new 
        targetid, reason[MAX_BAN_REASON], duration;

    if (!strcmp(params, "hour", true, 4))
    {
        if (sscanf(params, "{s[5]}ris[50]", targetid, duration, reason))
            return SendSyntaxMsg(playerid, "/ban hour <playerid> <duration> <reason>");
        if (!(IsPlayerConnected(targetid)))
            return SendSyntaxMsg(playerid, "The player you are trying to ban is not connected.");
        if (strlen(reason) > MAX_BAN_REASON)
            return SendSyntaxMsg(playerid, "The lenght of the reason is too long, maximum characters is 32");
        
        AddPlayerBan(playerid, targetid, reason, 0, duration);
    }
    if (!strcmp(params, "month", true, 5))
    {
        if (sscanf(params, "{s[6]}ris[50]", targetid, duration, reason))
            return SendSyntaxMsg(playerid, "/ban month <playerid> <duration> <reason>");
        if (!(IsPlayerConnected(targetid)))
            return SendSyntaxMsg(playerid, "The player you are trying to ban is not connected.");
        if (strlen(reason) > MAX_BAN_REASON)
            return SendSyntaxMsg(playerid, "The lenght of the reason is too long, maximum characters is 32");

        AddPlayerBan(playerid, targetid, reason, 1, duration);
    }
    if (!strcmp(params, "year", true, 4)) 
    {
        if (sscanf(params, "{s[5]}ris[50]", targetid, duration, reason))
            return SendSyntaxMsg(playerid, "/ban year <playerid> <duration> <reason>");
        if (!(IsPlayerConnected(targetid)))
            return SendSyntaxMsg(playerid, "The player you are trying to ban is not connected.");
        if (strlen(reason) > MAX_BAN_REASON)
            return SendSyntaxMsg(playerid, "The lenght of the reason is too long, maximum characters is 32");

        AddPlayerBan(playerid, targetid, reason, 2, duration);
    }

    SendMsgF(playerid, COLOR_RED, "[BANNED]: "C_WHITE"You have been banned by admin "C_GREY"%p "C_WHITE"for "C_GREY"%s", playerid, reason);
    return 1;
}

ACMD:[3]unban(playerid, params[])
{
    if (isnull(params) || strval(params))
        return SendSyntaxMsg(playerid, "/unban <playername>");

    if (strlen(params) > MAX_PLAYER_NAME)
        return SendErrorMsg(playerid, "MAX_PLAYER_NAME = 24, type the name again!");
 
    RemovePlayerBan(params, playerid);
    return 1;
}