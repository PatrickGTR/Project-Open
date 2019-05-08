static
	plyObjectC4[MAX_PLAYERS],
	plyAlreadyPlantedC4[MAX_PLAYERS]
;


CMD:plantc4(playerid, params[])
{
	if (GetPlayerC4(playerid) <= 0)
		return SendErrorMsg(playerid, "You do not have C4, how is that possible to plant one?");

	if (plyAlreadyPlantedC4[playerid])
		return SendErrorMsg(playerid, "You already planted a C4, /detonate to blow the bank safe or jail cell.");
	new
		Float:ply_posX,
		Float:ply_posY,
		Float:ply_posZ
	;
	GetPlayerPos(playerid, ply_posX, ply_posY, ply_posZ);

	plyObjectC4[playerid] = CreateDynamicObject(1252, ply_posX, ply_posY, ply_posZ-1, 90, 0, 0);

	ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.1, 0, 1, 1, 0, 5, 1);
	plyAlreadyPlantedC4[playerid] = true;
	return true;
}

CMD:detonate(playerid, params[])
{
	if (!plyAlreadyPlantedC4[playerid])
		return SendErrorMsg(playerid, "You did not plant any C4 how is that possible to detonate?");

	new
		Float:ply_posX,
		Float:ply_posY,
		Float:ply_posZ
	;
	GetDynamicObjectPos(plyObjectC4[playerid], ply_posX, ply_posY, ply_posZ);

	ExplodeBankDoors(playerid);
	ExplodeCellDoors(playerid);

	CreateExplosion(ply_posX, ply_posY, ply_posZ, 0, 5.0);
	DestroyDynamicObject(plyObjectC4[playerid]);
	plyObjectC4[playerid] = INVALID_OBJECT_ID;
	plyAlreadyPlantedC4[playerid] = false;
	GivePlayerWantedLevel(playerid, 4, "Terrorism", true);
	return true;
}
