// Dependencies
#include <a_mysql>
#include <YSI_Visual\y_dialog>
#include <YSI_Coding\y_hooks>

#define YSI_MALLOC_SECURE
#include <YSI_Coding\y_malloc>


// Table and Fields Constants
#define TABLE_SPAWNPOINTS 		"Spawns"
#define FIELD_SPAWN_ID 			"SpawnID"
#define FIELD_SPAWN_POS_X 		"SpawnX"
#define FIELD_SPAWN_POS_Y 		"SpawnY"
#define FIELD_SPAWN_POS_Z 		"SpawnZ"
#define FIELD_SPAWN_POS_A 		"SpawnA"

// constant
const
	INVALID_SPAWN_ID = -1;

new
    MAX_SPAWNPOINTS = 0;

// Declarations
static enum e_SPAWN_DATA
{
	Alloc: e_SPAWN_ID,
	Alloc: e_SPAWN_X, // should be Float:
	Alloc: e_SPAWN_Y, // should be Float:
	Alloc: e_SPAWN_Z, // should be Float:
	Alloc: e_SPAWN_A, // should be Float:
	Alloc: e_SPAWN_CREATED // should be bool:
}
static
	spawn_Data[e_SPAWN_DATA],
	spawn_Count = 0, // start from 0
	lastSpawnID[MAX_PLAYERS] = INVALID_SPAWN_ID;

hook OnGameModeInit()
{
	// Create the table in our database if it doesn't exist.
	mysql_tquery(GetMySQLHandle(), "CREATE TABLE IF NOT EXISTS "TABLE_SPAWNPOINTS" (\
		"FIELD_SPAWN_ID" TINYINT(3) NOT NULL DEFAULT 0,\
		"FIELD_SPAWN_POS_X" FLOAT NOT NULL,\
		"FIELD_SPAWN_POS_Y" FLOAT NOT NULL,\
		"FIELD_SPAWN_POS_Z" FLOAT NOT NULL,\
		"FIELD_SPAWN_POS_A"	FLOAT NOT NULL)");
	LoadSpawns();
}


hook OnPlayerConnect(playerid) {
	// set letSpawnID[playerid] to INVALID_SPAWN_ID just to init the variable back to default.
	lastSpawnID[playerid] = INVALID_SPAWN_ID;


	AllocateSpawnMemory();
}

AllocateSpawnMemory() {

	// called on fresh start.
	if (MAX_SPAWNPOINTS == 0)
    {
		MAX_SPAWNPOINTS = 15;
        spawn_Data[e_SPAWN_ID]      = calloc(MAX_SPAWNPOINTS);
        spawn_Data[e_SPAWN_X]       = calloc(MAX_SPAWNPOINTS);
        spawn_Data[e_SPAWN_Y]       = calloc(MAX_SPAWNPOINTS);
        spawn_Data[e_SPAWN_Z]       = calloc(MAX_SPAWNPOINTS);
        spawn_Data[e_SPAWN_A]       = calloc(MAX_SPAWNPOINTS);
        spawn_Data[e_SPAWN_CREATED] = calloc(MAX_SPAWNPOINTS);
    }
    else
    {
		MAX_SPAWNPOINTS *= 2;
        spawn_Data[e_SPAWN_ID]      = realloc(spawn_Data[e_SPAWN_ID], MAX_SPAWNPOINTS);
        spawn_Data[e_SPAWN_X]       = realloc(spawn_Data[e_SPAWN_X], MAX_SPAWNPOINTS);
        spawn_Data[e_SPAWN_Y]       = realloc(spawn_Data[e_SPAWN_Y], MAX_SPAWNPOINTS);
        spawn_Data[e_SPAWN_Z]       = realloc(spawn_Data[e_SPAWN_Z], MAX_SPAWNPOINTS);
        spawn_Data[e_SPAWN_A]       = realloc(spawn_Data[e_SPAWN_A], MAX_SPAWNPOINTS);
        spawn_Data[e_SPAWN_CREATED] = realloc(spawn_Data[e_SPAWN_CREATED], MAX_SPAWNPOINTS);
    }

    // Go up to the internal limit - may as well.
    MAX_SPAWNPOINTS = Malloc_SlotSize(spawn_Data[e_SPAWN_CREATED]);
	return MAX_SPAWNPOINTS;
}

/*
	Function:
		CountSpawnPoint()
	Information:
		Returns the total amount of spawn point created
*/
CountSpawnPoint() {
	return spawn_Count;
}

/*
	Function:
		IsValidSpawnPoint(spawnid)
	Information:
		Check if spawn point is valid
*/
bool:IsValidSpawnPoint(spawnid) {
	return bool:mget(spawn_Data[e_SPAWN_CREATED], spawnid);
}

/*
	Function:
		GetSpawnNextSlot()
	Information:
		Retrieves the unused slot.
*/
GetSpawnFreeID() {
	// loop through all the valid spawnpoint
	for(new i = 0, j = MAX_SPAWNPOINTS; i < j; i++) {
		// next invalid ID = free slot.
		if(!IsValidSpawnPoint(i)) {
			// return the slot found.
			return i;
		}
	}
	// if no valid spawn point found we return INVALID_SPAWN_ID
	return INVALID_SPAWN_ID;
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
		id = GetSpawnFreeID();

	if(id == INVALID_SPAWN_ID) {
		new
			size = AllocateSpawnMemory();
		printf("ERROR: No more slots left, allocating more slot to MAX_SPAWNPOINTS, new size: %i", size);
		return;
	}

	id = GetSpawnFreeID();
	CreateSpawnPoint(id, X, Y, Z, A);

    new
    	query[128];
   	mysql_format(GetMySQLHandle(), query, sizeof(query), "INSERT INTO "TABLE_SPAWNPOINTS" \
   		("FIELD_SPAWN_ID", "FIELD_SPAWN_POS_X", "FIELD_SPAWN_POS_Y", "FIELD_SPAWN_POS_Z", "FIELD_SPAWN_POS_A") \
   		VALUES (%i, %0.2f, %0.2f, %0.2f, %0.2f)", id, X, Y, Z, A);
 	mysql_tquery(GetMySQLHandle(), query, "", "");
}

/*
	Function:
		CreateSpawnPoint(ID, Float:X, Float:Y, Float:Z, Float:A)
	Information:
		Create spawnpoint on specified coordinates, this function should only be used under AddSpawnPoint,
		if you want to create a new spawnpoint and save it to database function AddSpawnPoint should be used.
*/
CreateSpawnPoint(spawnid, Float:X, Float:Y, Float:Z, Float:A)
{
	mset(spawn_Data[e_SPAWN_ID], spawnid, spawnid);
	mset(spawn_Data[e_SPAWN_X], spawnid, _:X);
	mset(spawn_Data[e_SPAWN_Y], spawnid, _:Y);
	mset(spawn_Data[e_SPAWN_Z], spawnid, _:Z);
	mset(spawn_Data[e_SPAWN_A], spawnid, _:A);
	mset(spawn_Data[e_SPAWN_CREATED], spawnid, bool:true);
	return spawn_Count ++;
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

    if (!IsValidSpawnPoint(spawnid))
            return INVALID_SPAWN_ID;

	// execute DELETE statement
    mysql_format(GetMySQLHandle(), query, sizeof(query), "DELETE FROM "TABLE_SPAWNPOINTS" WHERE "FIELD_SPAWN_ID" = %i", spawnid);
    mysql_tquery(GetMySQLHandle(), query);

	// set the value to invalid
	mset(spawn_Data[e_SPAWN_ID], spawnid, INVALID_SPAWN_ID);
	mset(spawn_Data[e_SPAWN_X], spawnid, _:0.0);
	mset(spawn_Data[e_SPAWN_Y], spawnid, _:0.0);
	mset(spawn_Data[e_SPAWN_Z], spawnid, _:0.0);
	mset(spawn_Data[e_SPAWN_A], spawnid, _:0.0);
	mset(spawn_Data[e_SPAWN_CREATED], spawnid, bool:false);
    return spawn_Count --;
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
			Float:_spawnAng,
			rows
		;

		cache_get_row_count(rows);
		if (rows) {
			for(new i = 0; i < rows; ++i) {
				cache_get_value_name_int 	(i, FIELD_SPAWN_ID, 	_spawnID);
				cache_get_value_name_float 	(i, FIELD_SPAWN_POS_X, 	_spawnX);
				cache_get_value_name_float 	(i, FIELD_SPAWN_POS_Y, 	_spawnY);
				cache_get_value_name_float 	(i, FIELD_SPAWN_POS_Z, 	_spawnZ);
				cache_get_value_name_float 	(i, FIELD_SPAWN_POS_A, 	_spawnAng);
				CreateSpawnPoint(_spawnID, _spawnX, _spawnY, _spawnZ, _spawnAng);
			}
		// if no rows to load from the database, we create 10 default spawn to avoid player from spawning in blueberry farms.
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
		"SELECT * FROM  "TABLE_SPAWNPOINTS""
	);
}

/*
	Function:
		SpawnPlayerToSpawnPoint(playerid)
	Information:
		Spawn the player to a randomised spawn point, same spawn could not happen twice in a row after death.
*/
SpawnPlayerToSpawnPoint(playerid) {

	new spawnid = random(CountSpawnPoint());

	if (lastSpawnID[playerid] == spawnid) {
        return SpawnPlayerToSpawnPoint(playerid);
    }


	SetPlayerPos(playerid,
		Float:mget(spawn_Data[e_SPAWN_X], spawnid),
		Float:mget(spawn_Data[e_SPAWN_Y], spawnid),
		Float:mget(spawn_Data[e_SPAWN_Z], spawnid)
	);
	SetPlayerFacingAngle(playerid, Float:mget(spawn_Data[e_SPAWN_A], spawnid));
	SetCameraBehindPlayer(playerid);
	lastSpawnID[playerid] = spawnid;
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

	for (new i = 0, j = CountSpawnPoint(); i < j; i++) {
		format(tmp_string, sizeof(tmp_string), "ID: %i - X: %0.2f - Y: %0.2f - Z: %0.2f - A: %0.2f\n",
			mget(spawn_Data[e_SPAWN_ID], i),
			Float:mget(spawn_Data[e_SPAWN_X], i),
			Float:mget(spawn_Data[e_SPAWN_Y], i),
			Float:mget(spawn_Data[e_SPAWN_Z], i),
			Float:mget(spawn_Data[e_SPAWN_A], i)
		);
		strcat(string, tmp_string, sizeof(string));
	}
	if (CountSpawnPoint()) {
		Dialog_Show(playerid, DIALOG_STYLE_LIST, " Spawn Points", string, "Close", "" );
	} else {
		Dialog_Show(playerid, DIALOG_STYLE_LIST, " Spawn Points", "There are no  Spawn created, create one to let players spawn.", "Close", "");
	}
	return 1;
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

	SetPlayerPos(playerid,
		Float:mget(spawn_Data[e_SPAWN_X], spawnid),
		Float:mget(spawn_Data[e_SPAWN_Y], spawnid),
		Float:mget(spawn_Data[e_SPAWN_Z], spawnid)
	);
	SetPlayerFacingAngle(playerid, Float:mget(spawn_Data[e_SPAWN_A], spawnid));
	SetCameraBehindPlayer(playerid);
	return 1;
}
