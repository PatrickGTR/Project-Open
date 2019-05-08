// WEED
CMD:plantweed(playerid, params[])
{

	if (GetPlayerJob(playerid) != TYPE_DRUGDEALER || GetPlayerClass(playerid) != TEAM_CIVILIAN) {
		return SendErrorMsg(playerid, "Only drug dealers can use this command");
    }

	new
		Float:x,
		Float:y,
		Float:z
    ;

	GetPlayerPos(playerid, x, y, z);
	CreatePlayerWeed(playerid, x, y, z);
    return 1;
}

CMD:harvestweed(playerid, params[])
{
	if (GetPlayerJob(playerid) != TYPE_DRUGDEALER || GetPlayerClass(playerid) != TEAM_CIVILIAN) {
        return SendErrorMsg(playerid, "Only drug dealers can use this command");
    }

	new 
        i = GetClosestWeedID(playerid)
    ;

	if (i == -1) {
		return SendErrorMsg(playerid, "You are not close to any of your weed plant!");
    }
	
    if (GetWeedPercent(playerid, i) < 100) {
		return SendErrorMsg(playerid, "Unable to harvest cannabis plant, it hasn't fully grown yet!");
    }

	ResetWeedData(playerid, i);
	ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 0 , 0, 0, 0, 0, 1);

	GivePlayerWeed(playerid, 1);
	SendClientMessage(playerid, COLOR_GREEN, "** WEED PICKUP ** "C_WHITE"You have harvested 10 grams of weed.");
	return 1;
}
