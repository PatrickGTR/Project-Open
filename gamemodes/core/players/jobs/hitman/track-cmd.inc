
CMD:track(playerid, params[])
{	
	if (GetPlayerJob(playerid) != TYPE_HITMAN || GetPlayerClass(playerid) != TEAM_CIVILIAN)
		return SendErrorMsg(playerid, "You are not allowed to use this command if you are not a hitman.");

	if (isnull(params))
		return SendSyntaxMsg(playerid, "/track <playerid>");

	new targetid = strval(params);

	if (targetid == playerid)
		return SendErrorMsg(playerid, "You are not allowed to track your own self.");

	if (!IsPlayerConnected(targetid))
		return SendErrorMsg(playerid, "The player you are trying to track is not connected.");

	if (!IsPlayerSpawned(targetid))
		return SendErrorMsg(playerid, "The player you are trying to track is not spawned.");

	if (GetPlayerInterior(targetid) > 1)
	{
		SendMsgF(playerid, COLOR_WHITE, "Location of "C_GREY"%p "C_WHITE"near "C_GREY"inside an interior", targetid);
	}
	else SendMsgF(playerid, COLOR_WHITE, "Location of "C_GREY"%p "C_WHITE"near "C_GREY"%s", targetid, GetPlayerLocation(playerid));
	return true;
}	
