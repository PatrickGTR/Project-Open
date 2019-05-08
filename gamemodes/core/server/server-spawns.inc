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
	spawn_id,
	Float:spawn_posX,
	Float:spawn_posY,
	Float:spawn_posZ,
	Float:spawn_posA
}
static
	spawn_data[MAX_SPAWNPOINTS][SpawnPoint],
	Iterator:spawn_Index<MAX_SPAWNPOINTS>,
	lastSpawnID[MAX_PLAYERS] = -1;

/*
	Hook 
		-> Create the table in our database if it doesn't exist.
*/
hook OnGameModeInit()
{
	mysql_tquery(GetSQLHandle(), "CREATE TABLE IF NOT EXISTS "T_SPAWNPOINTS" (\
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
	new id = Iter_Free(spawn_Index);
	CreateSpawnPoint(id, X, Y, Z, A);
    
    new 
    	query[128];
   	mysql_format(GetSQLHandle(), query, sizeof(query), "INSERT INTO "T_SPAWNPOINTS" \
   		("F_SPAWN_ID", "F_SPAWN_POS_X", "F_SPAWN_POS_Y", "F_SPAWN_POS_Z", "F_SPAWN_POS_A") \
   		VALUES (%i, %0.2f, %0.2f, %0.2f, %0.2f)", id, X, Y, Z, A);
 	mysql_tquery(GetSQLHandle(), query, "", "");
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
 	if (Iter_Count(spawn_Index) == INVALID_SPAWN_ID)
 		return 0;

 	spawn_data[ID][spawn_id] = ID;
    spawn_data[ID][spawn_posX] = X;
    spawn_data[ID][spawn_posY] = Y;
    spawn_data[ID][spawn_posZ] = Z;
    spawn_data[ID][spawn_posA] = A;

    Iter_Add(spawn_Index, ID);
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

    if (!Iter_Contains(spawn_Index, spawnid))
            return 0;

    mysql_format(GetSQLHandle(), query, sizeof(query), "DELETE FROM "T_SPAWNPOINTS" WHERE "F_SPAWN_ID" = %i", spawnid);
    mysql_tquery(GetSQLHandle(), query);

	spawn_data[spawnid][spawn_id] = -1;
    spawn_data[spawnid][spawn_posX] = 
    spawn_data[spawnid][spawn_posY] = 
    spawn_data[spawnid][spawn_posZ] = 
    spawn_data[spawnid][spawn_posA] = 0.0;
    Iter_Remove(spawn_Index, spawnid);
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
		GetSQLHandle(), 
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

	new ranspawn_ID = Iter_Random(spawn_Index);
	
	if (lastSpawnID[playerid] == ranspawn_ID) {
        return SpawnPlayerToSpawnPoint(playerid);
    }
    
	SetPlayerPos(playerid, spawn_data[ranspawn_ID][spawn_posX], spawn_data[ranspawn_ID][spawn_posY], spawn_data[ranspawn_ID][spawn_posZ]);
	SetPlayerFacingAngle(playerid, spawn_data[ranspawn_ID][spawn_posA]);
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

	foreach(new i : spawn_Index) {
		format(tmp_string, sizeof(tmp_string), "ID: %i - X: %0.2f - Y: %0.2f - Z: %0.2f - A: %0.2f\n", spawn_data[i][spawn_id], spawn_data[i][spawn_posX], spawn_data[i][spawn_posY], spawn_data[i][spawn_posZ], spawn_data[i][spawn_posA]);
		strcat(string, tmp_string, sizeof(string));
	}
	if (Iter_Count(spawn_Index)) {
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
	return Iter_Contains(spawn_Index, spawnid);
}

/*
	Function:
		CountSpawnPoint(playerid)
	Information:
		Returns the total amount of spawn point created
*/
CountSpawnPoint() {
	return Iter_Count(spawn_Index);
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
	
	SetPlayerPos(playerid, spawn_data[spawnid][spawn_posX], spawn_data[spawnid][spawn_posY], spawn_data[spawnid][spawn_posZ]);
	SetPlayerFacingAngle(playerid, spawn_data[spawnid][spawn_posA]);
	return 1;
}
