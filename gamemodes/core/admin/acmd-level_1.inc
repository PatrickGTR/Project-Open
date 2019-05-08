


/*
	LEVEL 1 ------- ADMIN COMMANDS
*/

ACMD:[1]spec(playerid, params[]) {
    if(isnull(params) || !IsNumeric(params))
        return SendSyntaxMsg(playerid, "/spec <playerid>");

    new 
        targetid = strval(params);

    if(targetid == playerid)
        return SendErrorMsg(playerid, "You can not spectate yourself, dummy.");

    if(targetid == INVALID_PLAYER_ID)
        return SendErrorMsg(playerid, "Invalid playerid");

    SpectateSaveData(playerid);
    TogglePlayerSpectating(playerid, true);
    SpectatePlayer(playerid, targetid);
    return 1;
}

ACMD:[1]specoff(playerid, params[]) {
    if(!IsPlayerSpectating(playerid))
        return SendErrorMsg(playerid, "You are not in spectator mode.");

    TogglePlayerSpectating(playerid, false);
    return 1;
}


ACMD:[1]duty(playerid, params[])
{
    SetPlayerOnDuty(playerid, (!IsAdminOnDuty(playerid)) ? true : false);

    SendServerMsgF(playerid, (IsAdminOnDuty(playerid)) ? "You are now on duty!" : "You are now off duty!");
    return 1;
}

ACMD:[1]a(playerid, params[])
{
    if (isnull(params))
        return SendSyntaxMsg(playerid, "/a <Message>");

    foreach(new i : Player)
    {
        if(GetPlayerAdminLevel(playerid) <= 0) continue;

        SendMsgF(i, COLOR_LIGHTGREEN, " ** ADMIN CHAT ** "C_GREY"%p[%i] -"C_WHITE" %s", playerid, playerid, params);
    }
    return 1;
}

ACMD:[1]kick(playerid, params[])
{
    new
        targetid,
        reason[60];

    if (sscanf(params, "rs[60]", targetid, reason))
        return SendSyntaxMsg(playerid, "/kick <playerid> <Reason>");

    if (!IsPlayerConnected(targetid))
        return SendErrorMsg(playerid, "The player you are trying to kick is not connected.");

    if (!IsPlayerSpawned(targetid))
        return SendErrorMsg(playerid, "The player you are trying to kick is not spawned.");

    SendServerMsgF(playerid, "You kicked "C_GREY"%p(%i) "C_WHITE"for %s", targetid, targetid, reason);
    SendServerMsgF(playerid, "You have been kicked by admin "C_GREY"%p(%i) "C_WHITE"for %s", playerid, playerid, reason);
    SendMsgAllF(COLOR_LIGHTGREEN, "** KICK ** "C_GREY"%p(%i) "C_FUCHSIA"kicked "C_GREY"%p(%i) "C_FUCHSIA"for %s", playerid, playerid, targetid, targetid, reason);
    return true;
}

ACMD:[1]freeze(playerid, params[])
{
    new
        targetid;

    if (isnull(params) && !IsNumeric(params))
        return SendSyntaxMsg(playerid, "/freeze <playerid>");

    targetid = strval(params);

    if (!IsPlayerConnected(targetid))
        return SendErrorMsg(playerid, "The player you are trying to freeze is not connected.");

    if (!IsPlayerSpawned(targetid))
        return SendErrorMsg(playerid, "The player you are trying to freeze is not spawned.");

    TogglePlayerControllable(playerid, 0);

    SendServerMsgF(playerid, "You froze "C_GREY"%p(%i)", targetid, targetid);
    SendServerMsgF(playerid, "You have been frozen by admin "C_GREY"%p(%i)", playerid, playerid);
    SendMsgAllF(COLOR_LIGHTGREEN, "** FREEZE ** "C_GREY"%p(%i) "C_FUCHSIA"frozen "C_GREY"%p(%i)", playerid, playerid, targetid, targetid);
    return true;
}

ACMD:[1]unfreeze(playerid, params[])
{
    new
        targetid;

    if (isnull(params) || !IsNumeric(params))
        return SendSyntaxMsg(playerid, "/freeze <playerid>");

    targetid = strval(params);

    if (!IsPlayerConnected(targetid))
        return SendErrorMsg(playerid, "The player you are trying to unfreeze is not connected.");

    if (!IsPlayerSpawned(targetid))
        return SendErrorMsg(playerid, "The player you are trying to unfreeze is not spawned.");

    TogglePlayerControllable(playerid, 1);

    SendServerMsgF(playerid, "You unfrozen "C_GREY"%p(%i)", targetid, targetid);
    SendServerMsgF(playerid, "You have been unfrozen by admin "C_GREY"%p(%i)", playerid, playerid);
    SendMsgAllF(COLOR_LIGHTGREEN, "** FREEZE ** "C_GREY"%p(%i) "C_FUCHSIA"unfrozen "C_GREY"%p(%i)", playerid, playerid, targetid, targetid);
    return true;
}

ACMD:[1]mute(playerid, params[])
{
    new
        targetid,
        duration;

    if (sscanf(params, "ri", targetid, duration))
        return SendSyntaxMsg(playerid, "/mute <playerid> <Seconds>");

    if (!IsPlayerConnected(targetid))
        return SendErrorMsg(playerid, "The player you are trying to mute is not connected.");

    if (!IsPlayerSpawned(targetid))
        return SendErrorMsg(playerid, "The player you are trying to mute is not spawned");

    TogglePlayerMute(targetid, true, duration);
    SendServerMsgF(playerid, "You have been muted by admin "C_GREY"%p(%i)", playerid, playerid);
    SendServerMsgF(playerid, "You muted "C_GREY"%p(%i) "C_WHITE"for "C_GREY"%i "C_WHITE"seconds.", targetid, targetid, duration);
    SendMsgAllF(COLOR_LIGHTGREEN, "** MUTE ** "C_GREY"%p(%i) "C_FUCHSIA"muted "C_GREY"%p(%i) "C_FUCHSIA"for %i seconds ", playerid, playerid, targetid, targetid, duration);
    return true;
}

ACMD:[1]unmute(playerid, params[])
{
    new
        targetid;

    if (isnull(params) || !IsNumeric(params))
        return SendSyntaxMsg(playerid, "/unmute <playerid>");

    targetid = strval(params);

    if (!IsPlayerConnected(targetid))
        return SendErrorMsg(playerid, "The player you are trying to unmute is not connected.");

    if (!IsPlayerSpawned(targetid))
        return SendErrorMsg(playerid, "The player you are trying to unmute is not spawned");

    TogglePlayerMute(targetid, false);
    SendServerMsgF(playerid, "You have been unmuted by admin "C_GREY"%p(%i)", playerid, playerid);
    SendServerMsgF(playerid, "You unmuted "C_GREY"%p(%i)", targetid, targetid);
 	SendMsgAllF(COLOR_LIGHTGREEN, "** MUTE ** "C_GREY"%p(%i) "C_FUCHSIA"unmuted "C_GREY"%p(%i)", playerid, playerid, targetid, targetid);
    return true;
}

ACMD:[1]acmds(playerid, params[])
{
	new
		tmp_string[1024];

	//strcat(tmp_string, "/a [message] - Staff chat channel");

	if (GetPlayerAdminLevel(playerid) >= ADM_LVL_5)
	{
		strcat(tmp_string, "\n\n"C_YELLOW"(level 5)"C_WHITE"\n");
		strcat(tmp_string, adminCommands[4]);
	}
	if (GetPlayerAdminLevel(playerid) >= ADM_LVL_4)
	{
		strcat(tmp_string, "\n\n"C_YELLOW"(level 4)"C_WHITE"\n");
		strcat(tmp_string, adminCommands[3]);
	}
	if (GetPlayerAdminLevel(playerid) >= ADM_LVL_3)
	{
		strcat(tmp_string, "\n\n"C_YELLOW"(level 3)"C_WHITE"\n");
		strcat(tmp_string, adminCommands[2]);
	}
	if (GetPlayerAdminLevel(playerid) >= ADM_LVL_2)
	{
		strcat(tmp_string, "\n\n"C_YELLOW"(level 2)"C_WHITE"\n");
		strcat(tmp_string, adminCommands[1]);
	}
	if (GetPlayerAdminLevel(playerid) >= ADM_LVL_1)
	{
		strcat(tmp_string, ""C_YELLOW"(level 1)"C_WHITE"\n");
		strcat(tmp_string, adminCommands[0]);
	}

	Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "Admin Commands List", tmp_string, "Close", "");
	return 1;
}
