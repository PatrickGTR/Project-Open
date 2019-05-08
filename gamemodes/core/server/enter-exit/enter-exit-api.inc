// Dependencies
#include <YSI_Data\y_iterate>
#include <YSI_Coding\y_timers>
#include <streamer>

// Definitions
#define MAX_ENTRANCES               (128)
#define MAX_STORE_NAME              (30)
#define ENEX_STREAMER_IDENTIFIER    (10)

// Declarations
static
    // Entrance
    entranceStoreName[MAX_ENTRANCES][MAX_STORE_NAME],
    entranceCheckpoint[MAX_ENTRANCES],
    Float: entranceX[MAX_ENTRANCES],
    Float: entranceY[MAX_ENTRANCES],
    Float: entranceZ[MAX_ENTRANCES],
    Float: entranceAng[MAX_ENTRANCES],
    // Exit
    exitCheckpoint[MAX_ENTRANCES],
    Float: exitX[MAX_ENTRANCES],
    Float: exitY[MAX_ENTRANCES],
    Float: exitZ[MAX_ENTRANCES],
    Float: exitAng[MAX_ENTRANCES],
    exitVirtualID[MAX_ENTRANCES],
    exitInteriorID[MAX_ENTRANCES],
    // Other
    Iterator: entranceID<MAX_ENTRANCES>,
    playerEnExCoolDown[MAX_PLAYERS],
    getShopName[MAX_PLAYERS][MAX_STORE_NAME];

/*
    Function:
        CreateMapIconOnEntrance(id, iconid)
    Information:
        Attach a map icon to assigned entranceid.
*/
CreateMapIconOnEntrance(id, iconid) {

    if(!Iter_Contains(entranceID, id)) {
        return 0;
    }

    CreateDynamicMapIcon(entranceX[id], entranceY[id], entranceZ[id], iconid, -1, .streamdistance = 200.0, .style = MAPICON_GLOBAL);

    return 1;
}

/* 
    Function:
        SetEntranceName(id, const ent_name[])
    Information:
        Attach entrance name to assigned entranceid.
*/
SetEntranceName(id, const ent_name[]) {

    if(!Iter_Contains(entranceID, id)){
        return 0;
    }

    if(isnull(ent_name)) {
        printf("ERROR: Entrance Name is null.");
        return 0;
    }

    strcpy(entranceStoreName[id], ent_name, MAX_STORE_NAME);
    return 1;
}

/*
    Function:
        GetEntranceName(playerid)
    Information:
        Returns the entrance name that player is inside.
*/
GetEntranceName(playerid)
    return (!isnull(getShopName[playerid])) ? getShopName[playerid] : "None";

/*
    Function:
        CreateEntrance(Float:enx, Float:eny, Float:enz, Float:enang, Float:exx, Float:exy, Float:exz, Float:exang, vwid, intid)
    Information:
        Create an entrance with exit on specificied coordinates.
*/
CreateEntrance(Float:enx, Float:eny, Float:enz, Float:enang, Float:exx, Float:exy, Float:exz, Float:exang, vwid, intid) {

    new 
        entid = Iter_Alloc(entranceID),
        arr[2]
    ;

    arr[0] = ENEX_STREAMER_IDENTIFIER;
    arr[1] = entid;
    
    entranceCheckpoint[entid] = CreateDynamicCP(enx, eny, enz + 0.50, 1.5, .streamdistance = CHECKPOINT_STREAMDISTANCE);
    exitCheckpoint[entid] = CreateDynamicCP(exx, exy, exz + 0.50, 1.5, vwid, intid, .streamdistance = CHECKPOINT_STREAMDISTANCE);

    CreateDynamic3DTextLabel("[ENT]", COLOR_YELLOW, enx, eny, enz, 50.0, .testlos = 1, .streamdistance = TEXTLABEL_STREAMDISTANCE);
    CreateDynamic3DTextLabel("[EXT]", COLOR_YELLOW, exx, exy, exz, 50.0, .testlos = 1, .streamdistance = TEXTLABEL_STREAMDISTANCE);

    Streamer_SetArrayData(STREAMER_TYPE_CP, entranceCheckpoint[entid], E_STREAMER_EXTRA_ID, arr, sizeof(arr));
    Streamer_SetArrayData(STREAMER_TYPE_CP, exitCheckpoint[entid], E_STREAMER_EXTRA_ID, arr, sizeof(arr));

    entranceX[entid]         = enx;
    entranceY[entid]         = eny;
    entranceZ[entid]         = enz;
    entranceAng[entid]       = enang;

    exitX[entid]             = exx;
    exitY[entid]             = exy;
    exitZ[entid]             = exz;
    exitAng[entid]           = exang;
    exitVirtualID[entid]     = vwid;
    exitInteriorID[entid]    = intid;
    return entid;
}

/*
    Function: 
        GetPlayerEntranceCD(playerid)
    Information:
        Returns unix timestamp of player last entered CP
*/
GetPlayerEntranceCD(playerid) {
    return playerEnExCoolDown[playerid];
}

/*
    Function: 
        SetPlayerEntranceCD(playerid)
    Information:
        Assign unix timestamp of player last entered CP
*/
SetPlayerEntranceCD(playerid, time) {
    return playerEnExCoolDown[playerid] = time;
}

// Checkpoint Handler

hook OnPlayerEnterDynamicCP(playerid, checkpointid)
{
    new
        arr[2],
        entid
    ;

    Streamer_GetArrayData(STREAMER_TYPE_CP, checkpointid, E_STREAMER_EXTRA_ID, arr);

    entid = arr[1];

    if (arr[0] == ENEX_STREAMER_IDENTIFIER) {
        if (gettime() < GetPlayerEntranceCD(playerid)) {
            return 1;
        }
        if (checkpointid == entranceCheckpoint[entid]) {
            SetPlayerEntranceCD(playerid, gettime() + 2);

            SetPlayerVirtualWorld(playerid, exitVirtualID[entid]);
            SetPlayerInterior(playerid, exitInteriorID[entid]);

            SetPlayerPos(playerid, exitX[entid], exitY[entid], exitZ[entid]);
            SetPlayerFacingAngle(playerid, exitAng[entid]);

            SetCameraBehindPlayer(playerid);

            TogglePlayerControllable(playerid, 0);
            defer UnFreezeLoadingPlayer(playerid);

            strcpy(getShopName[playerid], entranceStoreName[entid], MAX_STORE_NAME);
        }
        if (checkpointid == exitCheckpoint[entid]) {
            SetPlayerEntranceCD(playerid, gettime() + 2);

            SetPlayerInterior(playerid, 0);
            SetPlayerVirtualWorld(playerid, 0);

            SetPlayerPos(playerid, entranceX[entid], entranceY[entid], entranceZ[entid]);
            SetPlayerFacingAngle(playerid, entranceAng[entid]);

            SetCameraBehindPlayer(playerid);

            getShopName[playerid][0] = EOS;

            
        }
    }
    return 1;
}

timer UnFreezeLoadingPlayer[1500](playerid) {
    TogglePlayerControllable(playerid, 1);
    return 1;
}

