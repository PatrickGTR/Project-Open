// Dependencies
#include <YSI_Visual\y_dialog>
#include <YSI_Coding\y_hooks>
#include <YSI_Data\y_iterate>

// Definitions
#define MAX_SPAWNPOINTS (10)
#define INVALID_SPAWN_ID (-1)

#define T_SPAWNPOINTS 		"Spawns"
#define F_SPAWN_ID 			"SpawnID"
#define F_SPAWN_POS_X 		"SpawnX"
#define F_SPAWN_POS_Y 		"SpawnY"
#define F_SPAWN_POS_Z 		"SpawnZ"
#define F_SPAWN_POS_A 		"SpawnA"

// Declarations
enum SpawnPoint
{
	E_SPAWN_ID,
	Float:E_SPAWN_X,
	Float:E_SPAWN_Y,
	Float:E_SPAWN_Z,
	Float:E_SPAWN_A
}
static
	spawnData[MAX_SPAWNPOINTS][SpawnPoint],
	Iterator:spawnIndex<MAX_SPAWNPOINTS>,
	lastSpawnID[MAX_PLAYERS] = -1;

/*
	Hook
		-> Create the table in our database if it doesn't exist.
*/
hook OnGameModeInit()
{
	mysql_tquery(GetMySQLHandle(), "CREATE TABLE IF NOT EXISTS "T_SPAWNPOINTS" (\
		"F_SPAWN_ID" TINYINT(3) NOT NULL DEFAULT 0,\
		"F_SPAWN_POS_X" FLOAT NOT NULL,\
		"F_SPAWN_POS_Y" FLOAT NOT NULL,\
		"F_SPAWN_POS_Z" FLOAT NOT NULL,\
		"F_SPAWN_POS_A"	FLOAT NOT NULL)");

	LoadSpawns();
}

/*
	Function:
		AddSpawnPoint(Float:X, Float:Y, Float:Z, Float:A)
	Information:
		Creates and saves the spawnpoint to the database with specified coordinates.
*/
AddSpawnPoint(Float:X, Float:Y, Float:Z, Float:A)
{
	new
		id = Iter_Free(spawnIndex);
	CreateSpawnPoint(id, X, Y, Z, A);

    new
    	query[128];
   	mysql_format(GetMySQLHandle(), query, sizeof(query), "INSERT INTO "T_SPAWNPOINTS" \
   		("F_SPAWN_ID", "F_SPAWN_POS_X", "F_SPAWN_POS_Y", "F_SPAWN_POS_Z", "F_SPAWN_POS_A") \
   		VALUES (%i, %0.2f, %0.2f, %0.2f, %0.2f)", id, X, Y, Z, A);
 	mysql_tquery(GetMySQLHandle(), query, "", "");
    return 1;
}

/*
	Function:
		CreateSpawnPoint(ID, Float:X, Float:Y, Float:Z, Float:A)
	Information:
		Create spawnpoint on specified coordinates, this function should only be used under AddSpawnPoint,
		if you want to create a new spawnpoint and save it to database function AddSpawnPoint should be used.
*/
CreateSpawnPoint(ID, Float:X, Float:Y, Float:Z, Float:A)
{
 	if (Iter_Count(spawnIndex) == INVALID_SPAWN_ID)
 		return 0;

 	spawnData[ID][E_SPAWN_ID] = ID;
    spawnData[ID][E_SPAWN_X] = X;
    spawnData[ID][E_SPAWN_Y] = Y;
    spawnData[ID][E_SPAWN_Z] = Z;
    spawnData[ID][E_SPAWN_A] = A;

    Iter_Add(spawnIndex, ID);
    return 1;
}

/*
	Function:
		RemoveSpawnPoint(spawnid)
	Information:
		Removes the spawnpoint from the server and delete it from the database based on spawnid given.
*/
RemoveSpawnPoint(spawnid)
{
    new
    	query[70];

    if (!Iter_Contains(spawnIndex, spawnid))
            return 0;

    mysql_format(GetMySQLHandle(), query, sizeof(query), "DELETE FROM "T_SPAWNPOINTS" WHERE "F_SPAWN_ID" = %i", spawnid);
    mysql_tquery(GetMySQLHandle(), query);

	spawnData[spawnid][E_SPAWN_ID] = -1;
    spawnData[spawnid][E_SPAWN_X] =
    spawnData[spawnid][E_SPAWN_Y] =
    spawnData[spawnid][E_SPAWN_Z] =
    spawnData[spawnid][E_SPAWN_A] = 0.0;
    Iter_Remove(spawnIndex, spawnid);
    return 1;
}

/*
	Function:
		LoadSpawns()
	Information:
		Retrieves coordinates from the database and create the spawnpoint in-game.
		If there is no data to be retrieved, we will create 10 default spawnpoints to avoid run-time error.
*/
LoadSpawns()
{
	inline OnLoadSpawnPoint() {
		new
			_spawnID,
			Float:_spawnX,
			Float:_spawnY,
			Float:_spawnZ,
			Float:_spawnAng
		;

		if (cache_num_rows()) {
			for(new i = 0; i < cache_num_rows(); ++i) {
				cache_get_value_name_int 	(i, F_SPAWN_ID, 	_spawnID);
				cache_get_value_name_float 	(i, F_SPAWN_POS_X, 	_spawnX);
				cache_get_value_name_float 	(i, F_SPAWN_POS_Y, 	_spawnY);
				cache_get_value_name_float 	(i, F_SPAWN_POS_Z, 	_spawnZ);
				cache_get_value_name_float 	(i, F_SPAWN_POS_A, 	_spawnAng);
				CreateSpawnPoint(_spawnID, _spawnX, _spawnY, _spawnZ, _spawnAng);
			}
		} else {
			AddSpawnPoint(-1935.0742, 678.1586, 46.5625, 356.4240 );
			AddSpawnPoint(-1922.5177, 680.0504, 46.5625, 2.6907   );
			AddSpawnPoint(-1934.6843, 264.8631, 41.0469, 276.9846 );
			AddSpawnPoint(-2022.3842, 155.8002, 28.8359, 266.6432 );
			AddSpawnPoint(-2314.1555, -169.0953, 35.3203, 178.2457);
			AddSpawnPoint(-2126.3633, -383.9755, 35.3359, 2.5950  );
			AddSpawnPoint(-2720.4807, -317.9581, 7.8438, 41.5845  );
			AddSpawnPoint(-2521.2214, -621.2564, 132.7300, 1.2376 );
			AddSpawnPoint(-1928.7382, -790.2328, 32.1506, 273.2949);
			AddSpawnPoint(-1953.3707, 1339.3734, 7.1875, 174.4267 );
		}
	}
	MySQL_TQueryInline(
		GetMySQLHandle(),
		using inline OnLoadSpawnPoint,
		"SELECT * FROM  "T_SPAWNPOINTS""
	);
}

/*
	Function:
		SpawnPlayerToSpawnPoint(playerid)
	Information:
		Spawn the player to a randomised spawn point, same spawn could not happen twice in a row after death.
*/
SpawnPlayerToSpawnPoint(playerid) {

	new ranspawn_ID = Iter_Random(spawnIndex);

	if (lastSpawnID[playerid] == ranspawn_ID) {
        return SpawnPlayerToSpawnPoint(playerid);
    }

	SetPlayerPos(playerid, spawnData[ranspawn_ID][E_SPAWN_X], spawnData[ranspawn_ID][E_SPAWN_Y], spawnData[ranspawn_ID][E_SPAWN_Z]);
	SetPlayerFacingAngle(playerid, spawnData[ranspawn_ID][E_SPAWN_A]);
	SetCameraBehindPlayer(playerid);
	lastSpawnID[playerid] = ranspawn_ID;
    return 1;
}

/*
	Function:
		ListSpawnPoint(playerid)
	Information:
		List the spawn point to the spcecified playerid in a dialog form.
*/
ListSpawnPoint(playerid)
{
	new
		tmp_string[128],
		string[512];

	foreach(new i : spawnIndex) {
		format(tmp_string, sizeof(tmp_string), "ID: %i - X: %0.2f - Y: %0.2f - Z: %0.2f - A: %0.2f\n", spawnData[i][E_SPAWN_ID], spawnData[i][E_SPAWN_X], spawnData[i][E_SPAWN_Y], spawnData[i][E_SPAWN_Z], spawnData[i][E_SPAWN_A]);
		strcat(string, tmp_string, sizeof(string));
	}
	if (Iter_Count(spawnIndex)) {
		Dialog_Show( playerid, DIALOG_STYLE_LIST, " Spawn Points", string, "Close", "" );
	} else {
		Dialog_Show( playerid, DIALOG_STYLE_LIST, " Spawn Points", "There are no  Spawn created, create one to let players spawn.", "Close", "" );
	}
	return 1;
}

/*
	Function:
		IsValidSpawnPoint(playerid)
	Information:
		Check if spawn point is valid
*/
IsValidSpawnPoint(spawnid) {
	return Iter_Contains(spawnIndex, spawnid);
}

/*
	Function:
		CountSpawnPoint(playerid)
	Information:
		Returns the total amount of spawn point created
*/
CountSpawnPoint() {
	return Iter_Count(spawnIndex);
}

/*
	Function:
		SendPlayerToSpawnPoint(playerid, spawnid)
	Information:
		Teleports the player to spawnpoint.
*/
SendPlayerToSpawnPoint(playerid, spawnid) {

	if(!IsValidSpawnPoint(spawnid)) {
		return 0;
	}

	SetPlayerPos(playerid, spawnData[spawnid][E_SPAWN_X], spawnData[spawnid][E_SPAWN_Y], spawnData[spawnid][E_SPAWN_Z]);
	SetPlayerFacingAngle(playerid, spawnData[spawnid][E_SPAWN_A]);
	return 1;
}
