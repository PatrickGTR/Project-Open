
#define TABLE_HOUSE             "houses"
#define FIELD_HOUSE_ID          "h_ID"
#define FIELD_HOUSE_INTERIOR    "h_fk_intID"
#define FIELD_HOUSE_OWNERID     "h_ownerID"
#define FIELD_HOUSE_DESCRIPTION "h_description"
#define FIELD_HOUSE_PRICE       "h_price"
#define FIELD_HOUSE_CASH_STORED "h_cashstored"
#define FIELD_HOUSE_ENT_X       "h_entX"
#define FIELD_HOUSE_ENT_Y       "h_entY"
#define FIELD_HOUSE_ENT_Z       "h_entZ"
#define FIELD_HOUSE_ENT_A       "h_entA"
#define FIELD_HOUSE_VWID        "h_vwID"

#define TABLE_HOUSEINT          "house_interiors"
#define FIELD_HOUSE_LEVEL       "h_intLevel"
#define FIELD_HOUSE_INFO        "h_intInfo"
#define FIELD_HOUSE_EXT_X       "h_intX"
#define FIELD_HOUSE_EXT_Y       "h_intY"
#define FIELD_HOUSE_EXT_Z       "h_intZ"
#define FIELD_HOUSE_EXT_A       "h_intA"
#define FIELD_HOUSE_INTID       "h_intID"

#define HOUSE_UPGRADE_PRICE     (500000)

#define H_LEVEL1_PRICE          (250000)
#define H_LEVEL2_PRICE          (500000)
#define H_LEVEL3_PRICE          (750000)
#define H_LEVEL4_PRICE          (1000000)
#define H_LEVEL5_PRICE          (1500000)
#define H_LEVEL6_PRICE          (1750000)
#define H_LEVEL7_PRICE          (2000000)
#define H_LEVEL8_PRICE          (2500000)
#define H_LEVEL9_PRICE          (2750000)
#define H_LEVEL10_PRICE         (3900000)

enum E_HOUSE_INT_DATA
{
    H_INTLEVEL,
    H_INTINFO,
    H_INTID,
    H_INTX,
    H_INTY,
    H_INTZ,
    H_INTA
}

enum E_HOUSE_DATA
{
    H_DESC[MAX_H_DESCRIPTION],
    H_ID,
    H_OWNERID,
    H_PRICE,
    H_LEVEL,
    H_CASHSTORED,

    Float:ENT_POSX,
    Float:ENT_POSY,
    Float:ENT_POSZ,
    Float:ENT_POSA,

    Float:EXT_POSX,
    Float:EXT_POSY,
    Float:EXT_POSZ,
    Float:EXT_POSA,

    EXT_VWID,
    EXT_INTID,

    // DATA DOES NOT SAVE //
    ENT_CP,
    EXT_CP,
    Text3D:ENT_LABEL,
    Text3D:EXT_LABEL
}

static
        houseInteriorData[MAX_HOUSE_INTERIORS][E_HOUSE_INT_DATA],
        houseData[MAX_HOUSES][E_HOUSE_DATA];


/*
    Function:
        GetHouseEntranceCP(houseid)
    Information:
        Returns the id of the entrance checkpoint from CreateDynamicCheckpoint.
*/
GetHouseEntranceCP(houseid) {
    if(houseid == INVALID_HOUSE_ID){
        return 0;
    }
    return houseData[houseid][ENT_CP];
}

/*
    Function:
        GetHouseExitCP(houseid)
    Information:
        Returns the id of the exit checkpoint from CreateDynamicCheckpoint.
*/
GetHouseExitCP(houseid) {
    if(houseid == INVALID_HOUSE_ID){
        return 0;
    }
    return houseData[houseid][EXT_CP];
}

/*
    Function:
        GetHouseInteriorLevel(houseid)
    Information:
        Returns the interior level of specified houseid.
*/
GetHouseInteriorLevel(houseid) {
    if(houseid == INVALID_HOUSE_ID){
            return 0;
    }
    return houseData[houseid][H_LEVEL];
}

/*
    Function:
        GetHouseOwnerID(houseid)
    Information:
        Returns the ownerid (accountid) of the specified houseid.
*/
GetHouseOwnerID(houseid) {

    if(houseid == INVALID_HOUSE_ID){
        return 0;
    }

    return houseData[houseid][H_OWNERID];
}

/*
    Function:
        GetHouseExitPos(houseid, &Float: x, &Float: y, &Float: z, &Float: ang)
    Information:
        Returns the coordinates of the house exit checkpoint
*/
GetHouseExitPos(houseid, &Float: x, &Float: y, &Float: z, &Float: ang) {
    if(houseid == INVALID_HOUSE_ID){
        return 0;
    }

    x = houseData[houseid][EXT_POSX];
    y = houseData[houseid][EXT_POSY];
    z = houseData[houseid][EXT_POSZ];
    ang = houseData[houseid][EXT_POSA];
    return 1;
}

/*
    Function:
        GetHouseEntrancePos(houseid, &Float: x, &Float: y, &Float: z, &Float: ang)
    Information:
        Returns the coordinates of the house entrance checkpoint
*/
GetHouseEntrancePos(houseid, &Float: x, &Float: y, &Float: z, &Float: ang) {
    if(houseid == INVALID_HOUSE_ID){
        return 0;
    }

    x = houseData[houseid][ENT_POSX];
    y = houseData[houseid][ENT_POSY];
    z = houseData[houseid][ENT_POSZ];
    ang = houseData[houseid][ENT_POSA];
    return 1;
}

LoadHouseInteriors()
{
    inline OnHouseInteriorLoad()
    {
        new
            rows = cache_num_rows();

        if(rows)
        {
            for(new i = 0; i < rows; i++)
            {
                cache_get_value_name_int    (i, FIELD_HOUSE_LEVEL, houseInteriorData[i][H_INTLEVEL]);
                cache_get_value_name        (i, FIELD_HOUSE_INFO,  houseInteriorData[i][H_INTINFO], MAX_H_DESCRIPTION);
                cache_get_value_name_int    (i, FIELD_HOUSE_EXT_X, houseInteriorData[i][H_INTX]);
                cache_get_value_name_int    (i, FIELD_HOUSE_EXT_Y, houseInteriorData[i][H_INTY]);
                cache_get_value_name_int    (i, FIELD_HOUSE_EXT_Z, houseInteriorData[i][H_INTZ]);
                cache_get_value_name_int    (i, FIELD_HOUSE_EXT_A, houseInteriorData[i][H_INTA]);
                cache_get_value_name_int    (i, FIELD_HOUSE_INTID, houseInteriorData[i][H_INTID]);
            }
        }
    }
    MySQL_TQueryInline(GetSQLHandle(), using inline OnHouseInteriorLoad, "SELECT * FROM "TABLE_HOUSEINT"");
}

LoadHouses()
{
    Iter_Init(houseIndex);
    inline OnHouseLoad()
    {
        new
            Cache:res = cache_save(),
            rows = cache_num_rows();

        if (rows)
        {
            new
                id,
                ownerID,
                description[MAX_H_DESCRIPTION],
                price,
                level,
                Float:entposX,
                Float:entposY,
                Float:entposZ,
                Float:entposA,
                Float:extposX,
                Float:extposY,
                Float:extposZ,
                Float:extposA,
                virtualid,
                interiorid;

            for(new i = 0; i < rows; i++)
            {
                cache_get_value_name_int    (i, FIELD_HOUSE_ID, id);

                cache_get_value_name_int    (i, "ownerid", ownerID);

                cache_get_value_name        (i, FIELD_HOUSE_DESCRIPTION, description, MAX_H_DESCRIPTION);

                cache_get_value_name_int    (i, FIELD_HOUSE_PRICE, price);
                cache_get_value_name_int    (i, FIELD_HOUSE_INTERIOR, level);

                cache_get_value_name_int    (i, FIELD_HOUSE_CASH_STORED, houseData[id][H_CASHSTORED]);

                cache_get_value_name_float  (i, FIELD_HOUSE_ENT_X, entposX);
                cache_get_value_name_float  (i, FIELD_HOUSE_ENT_Y, entposY);
                cache_get_value_name_float  (i, FIELD_HOUSE_ENT_Z, entposZ);
                cache_get_value_name_float  (i, FIELD_HOUSE_ENT_A, entposA);

                cache_get_value_name_float  (i, FIELD_HOUSE_EXT_X, extposX);
                cache_get_value_name_float  (i, FIELD_HOUSE_EXT_Y, extposY);
                cache_get_value_name_float  (i, FIELD_HOUSE_EXT_Z, extposZ);
                cache_get_value_name_float  (i, FIELD_HOUSE_EXT_A, extposA);

                cache_get_value_name_int    (i, FIELD_HOUSE_VWID,  virtualid);
                cache_get_value_name_int    (i, FIELD_HOUSE_INTID, interiorid);

                CreateHouse(id, ownerID, description, entposX, entposY, entposZ, entposA, extposX, extposY, extposZ, extposA, virtualid, interiorid, level, price);

                cache_set_active(res);
            }
            cache_delete(res);
        }
    }
    MySQL_TQueryInline(
        GetSQLHandle(), 
        using inline OnHouseLoad, 
        "SELECT *, IFNULL(h1."FIELD_HOUSE_OWNERID", -1) \
            AS ownerid FROM "TABLE_HOUSE" AS h1 LEFT JOIN "TABLE_HOUSEINT" AS h2 \
        ON \
            h1."FIELD_HOUSE_INTERIOR" = h2."FIELD_HOUSE_LEVEL""
    );
}

UpdateHouse(houseid, newownerID, const newdescription[])
{
    new
        string[128 + MAX_PLAYER_NAME + MAX_H_DESCRIPTION];

    if (houseid == INVALID_HOUSE_ID)
        return 0;

    if(newownerID != INVALID_HOUSE_ID)
    {
        houseData[houseid][H_OWNERID] = newownerID;
        strcpy(houseData[houseid][H_DESC], newdescription, MAX_H_DESCRIPTION);

        mysql_format(GetSQLHandle(), string, sizeof(string),
        "UPDATE "TABLE_HOUSE" SET "FIELD_HOUSE_OWNERID" = %i, "FIELD_HOUSE_DESCRIPTION" = '%e', "FIELD_HOUSE_INTERIOR" = %i WHERE "FIELD_HOUSE_ID" = %i",
        newownerID, newdescription, houseData[houseid][H_LEVEL], houseid);
        mysql_tquery(GetSQLHandle(), string);
    }

    new
        name_from_id[24];

    GetPlayerNameFromAccountID(houseData[houseid][H_OWNERID], name_from_id);

    format(string, sizeof(string),
        "HouseID: "C_WHITE"%i\n\
        "C_ORANGE"Description: "C_WHITE"%s\n\
        "C_ORANGE"Owner: "C_WHITE"%s\n\
        "C_ORANGE"Level: "C_WHITE"%i\n\
        "C_ORANGE"Price: "C_WHITE"%m",
        houseData[houseid][H_ID], houseData[houseid][H_DESC],
        "test",
        houseData[houseid][H_LEVEL],
        houseData[houseid][H_PRICE]);

    UpdateDynamic3DTextLabelText(houseData[houseid][ENT_LABEL], COLOR_ORANGE, string);
    return 1;
}

AddHouse(Float:EntX, Float:EntY, Float:EntZ, Float:EntA, Float:ExtX, Float:ExtY, Float:ExtZ, Float:ExtA, VirtualID, InteriorID, level = 1, price = 500000)
{
    new
        string[356],
        id = Iter_Free(houseIndex);

    mysql_format(GetSQLHandle(), string, sizeof(string),
        "INSERT INTO "TABLE_HOUSE" (\
        "FIELD_HOUSE_ID", \
        "FIELD_HOUSE_OWNERID", \
        "FIELD_HOUSE_DESCRIPTION", \
        "FIELD_HOUSE_PRICE", \
        "FIELD_HOUSE_INTERIOR", \
        "FIELD_HOUSE_CASH_STORED", \
        "FIELD_HOUSE_ENT_X", \
        "FIELD_HOUSE_ENT_Y", \
        "FIELD_HOUSE_ENT_Z", \
        "FIELD_HOUSE_ENT_A", \
        "FIELD_HOUSE_VWID") \
        VALUES (%i, NULL, 'For Sale', %i, %i, 0, %0.4f, %0.4f, %0.4f, %0.4f, %i)",
        id, price, level, EntX, EntY, EntZ, EntA, VirtualID);
    mysql_tquery(GetSQLHandle(), string);

    CreateHouse(id, INVALID_OWNER_ID, "For Sale", EntX, EntY, EntZ, EntA, ExtX, ExtY, ExtZ, ExtA, VirtualID, InteriorID, level, price);
}

RemoveHouse(houseid, deleterid = INVALID_PLAYER_ID)
{
    inline OnHouseRemove()
    {
        if (cache_num_rows())
        {
            mysql_tquery(GetSQLHandle(), sprintf("DELETE FROM "TABLE_HOUSE" WHERE "FIELD_HOUSE_ID" = %i", houseid));

            if (deleterid != INVALID_PLAYER_ID)
            {
                SendServerMsgF(deleterid, "You successfully deleted house id "C_GREY"%i", houseid);
            }

            DestroyDynamicCP(houseData[houseid][ENT_CP]), DestroyDynamicCP(houseData[houseid][EXT_CP]);
            DestroyDynamic3DTextLabel(houseData[houseid][ENT_LABEL]), DestroyDynamic3DTextLabel(houseData[houseid][EXT_LABEL]);

            Streamer_RemoveArrayData(STREAMER_TYPE_CP, houseData[houseid][ENT_CP], E_STREAMER_EXTRA_ID, houseid);
            Streamer_RemoveArrayData(STREAMER_TYPE_CP, houseData[houseid][EXT_CP], E_STREAMER_EXTRA_ID, houseid);

            Iter_Remove(houseIndex, houseid);
        }
        else    
        {
            if (deleterid != INVALID_PLAYER_ID)
            {
                SendErrorMsgF(deleterid, "The house "C_GREY"(ID:%i) "C_WHITE"you are trying to delete was not found in the database.", houseid);
            }
        }
    }
    MySQL_TQueryInline(GetSQLHandle(), using inline OnHouseRemove, "SELECT "FIELD_HOUSE_ID" FROM "TABLE_HOUSE" WHERE "FIELD_HOUSE_ID" = %i", houseid);
}

CreateHouse(houseid, ownerID, const description[], Float:EntX, Float:EntY, Float:EntZ, Float:EntA, Float:ExtX, Float:ExtY, Float:ExtZ, Float:ExtA, VirtualID, InteriorID, level , price)
{
    new
        arr[2],
        string[128 + MAX_PLAYER_NAME + MAX_H_DESCRIPTION];


    new
        name_from_id[24];

    GetPlayerNameFromAccountID(ownerID, name_from_id);


    format(string, sizeof(string),
        "HouseID: "C_WHITE"%i\n\
        "C_ORANGE"Description: "C_WHITE"%s\n\
        "C_ORANGE"Owner: "C_WHITE"%s\n\
        "C_ORANGE"Level: "C_WHITE"%i\n\
        "C_ORANGE"Price: "C_WHITE"%m",
        houseid,
        description,
        (ownerID == INVALID_OWNER_ID) ? ("None") : (name_from_id),
        level,
        price);

    houseData[houseid][ENT_LABEL] = CreateDynamic3DTextLabel(string, COLOR_ORANGE, EntX, EntY, EntZ, 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1, 25.0);
    houseData[houseid][EXT_LABEL] = CreateDynamic3DTextLabel("House Exit", COLOR_ORANGE, ExtX, ExtY, ExtZ, 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, VirtualID, InteriorID, -1, 25.0);
    houseData[houseid][ENT_CP] = CreateDynamicCP(EntX, EntY, EntZ, 1.0, -1, -1, -1, 25.0);
    houseData[houseid][EXT_CP] = CreateDynamicCP(ExtX, ExtY, ExtZ, 1.0, VirtualID, InteriorID, -1, 25.0);


    arr[0] = HOUSES_STREAMER_IDENTIFIER;
    arr[1] = houseid;

    Streamer_SetArrayData(STREAMER_TYPE_CP, houseData[houseid][ENT_CP], E_STREAMER_EXTRA_ID, arr);
    Streamer_SetArrayData(STREAMER_TYPE_CP, houseData[houseid][EXT_CP], E_STREAMER_EXTRA_ID, arr);

    strcpy(houseData[houseid][H_DESC], description, MAX_H_DESCRIPTION);

    houseData[houseid][H_ID]           = houseid;
    houseData[houseid][H_OWNERID]      = ownerID;
    houseData[houseid][H_PRICE]        = price;
    houseData[houseid][H_LEVEL]        = level;
    houseData[houseid][EXT_VWID]       = VirtualID;
    houseData[houseid][EXT_INTID]      = InteriorID;

    houseData[houseid][ENT_POSX]       = EntX;
    houseData[houseid][ENT_POSY]       = EntY;
    houseData[houseid][ENT_POSZ]       = EntZ;
    houseData[houseid][ENT_POSA]       = EntA;

    houseData[houseid][EXT_POSX]       = ExtX;
    houseData[houseid][EXT_POSY]       = ExtY;
    houseData[houseid][EXT_POSZ]       = ExtZ;
    houseData[houseid][EXT_POSA]       = ExtA;

    Iter_Add(houseIndex, houseid);
}

GetPlayerInHouseIntID(playerid)
{
    new
        arr[2];

    Streamer_GetArrayData(STREAMER_TYPE_CP, ply_houseCheckpointID[playerid], E_STREAMER_EXTRA_ID, arr);
    return houseData[arr[1]][EXT_INTID];
}

GetPlayerInHouseVWID(playerid)
{
    new
        arr[2];

    Streamer_GetArrayData(STREAMER_TYPE_CP, ply_houseCheckpointID[playerid], E_STREAMER_EXTRA_ID, arr);
    return houseData[arr[1]][EXT_VWID];
}

GetPlayerInHouseID(playerid)
{
    new
        arr[2];

    Streamer_GetArrayData(STREAMER_TYPE_CP, ply_houseCheckpointID[playerid], E_STREAMER_EXTRA_ID, arr);
    return houseData[arr[1]][H_ID];
}

GetHouseLevelInfo(level, &price, &Float:ExtX, &Float:ExtY, &Float:ExtZ, &Float:ExtA, &vw, &intid)
{
    if (level == 0)
        level = 1;

    if (level > 10)
        level = 10;

    switch(level)
    {
        case 1:     price = H_LEVEL1_PRICE;
        case 2:     price = H_LEVEL2_PRICE;
        case 3:     price = H_LEVEL3_PRICE;
        case 4:     price = H_LEVEL4_PRICE;
        case 5:     price = H_LEVEL5_PRICE;
        case 6:     price = H_LEVEL6_PRICE;
        case 7:     price = H_LEVEL7_PRICE;
        case 8:     price = H_LEVEL8_PRICE;
        case 9:     price = H_LEVEL9_PRICE;
        case 10:    price = H_LEVEL10_PRICE;
    }

    ExtX = houseInteriorData[level -1][H_INTX];
    ExtY = houseInteriorData[level -1][H_INTY];
    ExtZ = houseInteriorData[level -1][H_INTZ];
    ExtA = houseInteriorData[level -1][H_INTA];
    intid = houseInteriorData[level - 1][H_INTID];
    vw = 1 + Iter_Count(houseIndex);
    return true;
}
