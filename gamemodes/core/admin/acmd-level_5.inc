


/*
	Level 5 ------- ADMIN COMMANDS
*/

ACMD:[5]setadmin(playerid, params[])
{
    new 
        targetid,
        adminlevel;

    if (sscanf(params, "ri", targetid, adminlevel))
        return SendSyntaxMsg(playerid, "/setadmin <playerid> <AdminLevel> (level 1 'lowest' to 5 'highest')");

    if (!IsPlayerConnected(targetid))
        return SendErrorMsg(playerid, "The player you are trying to set admin is not connected.");

    if (!IsPlayerSpawned(targetid))
        return SendErrorMsg(playerid, "The player you are trying to set admin is not spawned. ");

    if (targetid == playerid)
        return SendErrorMsg(playerid, "You can not set your own admin level.");

    if (0 <= adminlevel < 5)
        return SendErrorMsg(playerid, "Invalid Admin Level, valid admin levels are 0 - 5");

    SetPlayerAdminLevel(targetid, adminlevel);
    SendMsgF(targetid, COLOR_LIGHTGREEN, "** PROMOTED ** "C_WHITE"Your admin level has been set to "C_GREY"%i "C_WHITE"by admin "C_GREY"%p(%i)", adminlevel, playerid, playerid);
	SendAdminMessageF(""C_GREY"%p "C_WHITE"sets "C_GREY"%p "C_WHITE"admin level to "C_GREY"%i", playerid, targetid, adminlevel);
    return true;
}

ACMD:[5]delacc(playerid, params[])
{
    if (isnull(params))
        return SendSyntaxMsg(playerid, "/deleteaccount <PlayerName>");

    if (IsNumeric(params))
        return SendErrorMsg(playerid, "Please insert username, not number");

    DeleteAccount(playerid, params);
    return true;
}

ACMD:[5]mysqlstats(playerid, params[])
{
    new 
        stats[150];

    mysql_stat(stats, sizeof(stats));
    Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "MySQL Status", stats, "Close", "");
    return true;
}

ACMD:[5]spawn(playerid, params[])
{
	if (isnull(params))
		return SendSyntaxMsg(playerid, "/spawn create | delete | list | goto");
	
	if (isequal(params, "create"))
	{
		if (CountSpawnPoint() == MAX_SPAWNPOINTS)
			return SendErrorMsg(playerid, "No spawn points left.");
		
		new Float:Position[ 4];

		GetPlayerPos(playerid, Position[0], Position[1], Position[2]);
		GetPlayerFacingAngle(playerid, Position[3]);

		SendServerMsgF(playerid, "Created spawnpoint at X: %0.2f, Y: %0.2f, Z: %0.2f, A: %0.2f", Position[0], Position[1], Position[2], Position[3]);

		AddSpawnPoint(Position[0], Position[1], Position[2], Position[3]);
	}
	if (strcmp(params, "delete", true, 7))
	{
		new spawnid;

		if (sscanf(params, "{s[7]}i", spawnid))
			return SendSyntaxMsg(playerid, "/spawn delete <SpawnID>");
	
		if (!RemoveSpawnPoint(spawnid))
			return SendErrorMsg(playerid, "Invalid spawn id, use /spawn list to see valid spawn ids.");

		SendServerMsgF(playerid, "You have successfully deleted Spawn ID: "C_GREY"%i", spawnid);

		RemoveSpawnPoint(spawnid);

		SendAdminMessageF(""C_GREY"%p(%i) "C_WHITE"deleted to Spawn ID: "C_GREY"%i", playerid, playerid, spawnid);
	}
	if (isequal(params, "list"))
	{
		ListSpawnPoint(playerid);
	}
	if (isequal(params, "goto"))
	{
		new spawnid;

		if (sscanf(params, "{s[5]}i", spawnid))
			return SendSyntaxMsg(playerid, "/spawn goto <SpawnID>");

		if(IsValidSpawnPoint(spawnid))
			return SendErrorMsg(playerid, "Invalid spawn id, use /spawn list to see valid spawns ids");

        SendPlayerToSpawnPoint(playerid, spawnid);

		SendServerMsgF(playerid, "You have teleported to Spawn ID: "C_GREY"%i", spawnid);
		SendAdminMessageF(""C_GREY"%p(%i) "C_WHITE"teleported to Spawn ID: "C_GREY"%i", playerid, playerid, spawnid);
	}
	return true;
}