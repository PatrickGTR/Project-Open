#include <sscanf2>

#define TYPE_KMH 	(0)
#define TYPE_MPH	(1)

stock GetVehicleSpeedKMH(playerid, type)
{
	new
		Float:vx, Float:vy, Float:vz, Float:calculation, ret_speed;

 	GetVehicleVelocity(GetPlayerVehicleID(playerid), vx, vy, vz);
	calculation = (floatsqroot(((vx * vx) + (vy * vy)) + (vz * vz))* 181.5);

	if (type == TYPE_KMH) ret_speed = floatround(calculation, floatround_round);
	else if (type == TYPE_MPH) ret_speed = floatround((calculation / 1.6), floatround_round);

	return ret_speed;
}

stock IsVehicleCopCar(vehicleid)
{
    switch(GetVehicleModel(vehicleid))
    {
        case 523, 427, 490, 528, 596 .. 599, 601: return 1;
    }
    return 0;
}

stock LoadStaticVehiclesFromFile(const filename[])
{
    new
        line[55],
        File:file_ptr,
        vehicletype,
        Float:SpawnX,
        Float:SpawnY,
        Float:SpawnZ,
        Float:SpawnRot,
        col1,
        col2,
        vehicles_loaded;

    file_ptr = fopen(filename, filemode:io_read);

    if (!file_ptr) return 0;

    vehicles_loaded = 0;

    while(fread(file_ptr, line, sizeof(line)) > 0)
    {
        sscanf(line, "p<,>iffffii", vehicletype, SpawnX, SpawnY, SpawnZ, SpawnRot, col1, col2);

        if (vehicletype < 400 || vehicletype > 611) continue;

        AddStaticVehicleEx(vehicletype, SpawnX, SpawnY, SpawnZ, SpawnRot, col1, col2, (30*60)); // respawn 30 minutes
        vehicles_loaded++;
    }

    fclose(file_ptr);
    printf("Loaded %d vehicles from: %s", vehicles_loaded, filename);
    return vehicles_loaded;
}

stock IsAPlane(vehicleid)
{
    switch(GetVehicleModel(vehicleid))
    {
        case 460,464,476,511,512,513,519,520,553,577,592,593: return 1;
    }
    return 0;
}

stock IsValidNosVehicle(vehicleid)
{
    if (IsAPlane(vehicleid)) return 0;
    switch(GetVehicleModel(vehicleid))
    {
        case 581,523,462,521,463,522,461,448,468,586,
                         509,481,510,472,473,493,595,484,430,453,
                         452,446,454,590,569,537,538,570,449: return 0;
    }
    return 1;
}