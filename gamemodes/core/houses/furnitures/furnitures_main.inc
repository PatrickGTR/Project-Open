

#if defined _inc_main
	#undef _inc_main
#endif

#define TABLE_FURNITURE 		"house_furnitures"
#define FIELD_FURNITURE_INDEXID "furniture_id"
#define FIELD_FURNITURE_HOUSEID	"furniture_house_owner_id"
#define FIELD_FURNITURE_OBJID	"object_id"
#define FIELD_FURNITURE_POSX 	"pos_x"
#define FIELD_FURNITURE_POSY	"pos_y"
#define FIELD_FURNITURE_POSZ	"pos_z"
#define FIELD_FURNITURE_ROTX	"rot_x"
#define FIELD_FURNITURE_ROTY	"rot_y"
#define FIELD_FURNITURE_ROTZ	"rot_z"
#define FIELD_FURNITURE_INTID	"interior_id"
#define FIELD_FURNITURE_VWID	"virtual_world"

enum INTERIOR_EDITING_TYPE
{
	INVALID_EDITING_TYPE = -1,
	TYPE_EDITING_FURNITURE,
	TYPE_BUYING_FURNITURE,
    TYPE_DELETING_FURNITURE
}

new
    tempPlayerObjectID[MAX_PLAYERS] = {INVALID_OBJECT_ID, ...},
    tempPlayerIndexID[MAX_PLAYERS] = {-1, ...};

#include <furnitures-data>
#include <furnitures-impl>

#include <YSI_Coding\y_hooks>

hook OnGameModeInit() {
    mysql_tquery(GetSQLHandle(),
        "CREATE TABLE IF NOT EXISTS "TABLE_FURNITURE" \
        ("FIELD_FURNITURE_INDEXID" INT(11) AUTO_INCREMENT, \
        "FIELD_FURNITURE_HOUSEID" SMALLINT(6) NOT NULL, \
        "FIELD_FURNITURE_OBJID" INT(11) NOT NULL, \
        "FIELD_FURNITURE_POSX" FLOAT(6) NOT NULL, \
        "FIELD_FURNITURE_POSY" FLOAT(6) NOT NULL, \
        "FIELD_FURNITURE_POSZ" FLOAT(6) NOT NULL, \
        "FIELD_FURNITURE_ROTX" FLOAT(6) NOT NULL, \
        "FIELD_FURNITURE_ROTY" FLOAT(6) NOT NULL, \
        "FIELD_FURNITURE_ROTZ" FLOAT(6) NOT NULL, \
        "FIELD_FURNITURE_INTID" INT(11) NOT NULL, \
        "FIELD_FURNITURE_VWID" INT(11) NOT NULL, \
        PRIMARY KEY("FIELD_FURNITURE_INDEXID", "FIELD_FURNITURE_HOUSEID"), \
        FOREIGN KEY("FIELD_FURNITURE_HOUSEID") REFERENCES "TABLE_HOUSE"("FIELD_HOUSE_ID") ON DELETE CASCADE ON UPDATE CASCADE)");

    inline LoadFurnitureObjects() {
        new rows = cache_num_rows() ;
        if(rows) {
            new
                furniture_id,
                house_id,
                object_id,
                Float: pos_x,
                Float: pos_y,
                Float: pos_z,
                Float: rot_x,
                Float: rot_y,
                Float: rot_z,
                int_id,
                virtual_id
            ;

            for(new i = 0; i != rows; i++) {
                cache_get_value_name_int(i, FIELD_FURNITURE_INDEXID, furniture_id);
                cache_get_value_name_int(i, FIELD_FURNITURE_HOUSEID, house_id);
                cache_get_value_name_int(i, FIELD_FURNITURE_OBJID,   object_id);

                cache_get_value_name_float(i, FIELD_FURNITURE_POSX,  pos_x);
                cache_get_value_name_float(i, FIELD_FURNITURE_POSY,  pos_y);
                cache_get_value_name_float(i, FIELD_FURNITURE_POSZ,  pos_z);

                cache_get_value_name_float(i, FIELD_FURNITURE_ROTX,  rot_x);
                cache_get_value_name_float(i, FIELD_FURNITURE_ROTY,  rot_y);
                cache_get_value_name_float(i, FIELD_FURNITURE_ROTZ,  rot_z);

                cache_get_value_name_int(i, FIELD_FURNITURE_INTID,   int_id);
                cache_get_value_name_int(i, FIELD_FURNITURE_VWID,    virtual_id);

                CreateFurniture(furniture_id, house_id, object_id, pos_x, pos_y, pos_z, rot_x, rot_y, rot_z, virtual_id, int_id);
            }
        }
    }
    MySQL_TQueryInline(GetSQLHandle(), using inline LoadFurnitureObjects, "SELECT * FROM "TABLE_FURNITURE"");
    return 1;
}


hook OnPlayerEditDynObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz) {
    if(IsPlayerOnFurnitureMode(playerid))
    {
        switch(GetPlayerFurnitureEditingType(playerid))
        {
            case TYPE_BUYING_FURNITURE:
            {
                switch(response)
                {
                    case EDIT_RESPONSE_CANCEL:
                    {
                        DeleteTemporaryFurniture(playerid);
                    }
                    case EDIT_RESPONSE_FINAL:
                    {

                        new
                            index = tempPlayerIndexID[playerid],
                            qString[512];

                        if (GetPlayerMoney(playerid) < furnitureData[index][E_FURNITURE_PRICE])
                        {
                            SendErrorMsg(playerid, "You don't have enough money to buy this furniture.");
                            DeleteTemporaryFurniture(playerid);
                            return 1;
                        }

                        mysql_format(GetSQLHandle(), qString, sizeof(qString), "INSERT INTO "TABLE_FURNITURE" \
                                ("FIELD_FURNITURE_HOUSEID", "FIELD_FURNITURE_OBJID", "FIELD_FURNITURE_POSX", "FIELD_FURNITURE_POSY", "FIELD_FURNITURE_POSZ", \
                                "FIELD_FURNITURE_ROTX", "FIELD_FURNITURE_ROTY", "FIELD_FURNITURE_ROTZ", "FIELD_FURNITURE_INTID", "FIELD_FURNITURE_VWID") \
                            VALUES \
                            (%i, %i, %0.4f, %0.4f, %0.4f, %0.4f, %0.4f, %0.4f, %i, %i)", GetPlayerInHouseID(playerid), furnitureData[index][E_FURNITURE_OBJID], x, y, z, rx, ry, rz,
                            GetPlayerInHouseIntID(playerid), GetPlayerInHouseVWID(playerid));
                        mysql_tquery(GetSQLHandle(), qString);

                        inline LoadNewFurniture()
                        {
                            /*
                                We retrieve the last inserted ID.
                                Fetch all the fields that was in last inserted id
                                CreateFurniture based on the data that was fetched
                                from the MySQL database.
                            */
                            if(cache_num_rows())
                            {
                                new
                                    furniture_id,
                                    house_id,
                                    object_id,
                                    Float: pos_x,
                                    Float: pos_y,
                                    Float: pos_z,
                                    Float: rot_x,
                                    Float: rot_y,
                                    Float: rot_z,
                                    int_id,
                                    virtual_id
                                ;
                                cache_get_value_name_int(0, FIELD_FURNITURE_INDEXID, furniture_id);
                                cache_get_value_name_int(0, FIELD_FURNITURE_HOUSEID, house_id);
                                cache_get_value_name_int(0, FIELD_FURNITURE_OBJID,   object_id);

                                cache_get_value_name_float(0, FIELD_FURNITURE_POSX,  pos_x);
                                cache_get_value_name_float(0, FIELD_FURNITURE_POSY,  pos_y);
                                cache_get_value_name_float(0, FIELD_FURNITURE_POSZ,  pos_z);

                                cache_get_value_name_float(0, FIELD_FURNITURE_ROTX,  rot_x);
                                cache_get_value_name_float(0, FIELD_FURNITURE_ROTY,  rot_y);
                                cache_get_value_name_float(0, FIELD_FURNITURE_ROTZ,  rot_z);

                                cache_get_value_name_int(0, FIELD_FURNITURE_INTID,   int_id);
                                cache_get_value_name_int(0, FIELD_FURNITURE_VWID,    virtual_id);

                                CreateFurniture(furniture_id, house_id, object_id, pos_x, pos_y, pos_z, rot_x, rot_y, rot_z, virtual_id, int_id);
                            }
                        }
                        MySQL_TQueryInline(GetSQLHandle(), using inline LoadNewFurniture,  "SELECT * FROM "TABLE_FURNITURE" WHERE "FIELD_FURNITURE_INDEXID" =  LAST_INSERT_ID()");

                        SendServerMsg(playerid, "Object Saved [BOUGHT]");
                        GivePlayerMoney(playerid, -furnitureData[index][E_FURNITURE_PRICE]);
                        DeleteTemporaryFurniture(playerid);
                    }
                }

            }
            case TYPE_EDITING_FURNITURE:
            {
                new
                    Float: old_x,
                    Float: old_y,
                    Float: old_z,
                    Float: old_rx,
                    Float: old_ry,
                    Float: old_rz
                ;
                GetDynamicObjectPos(objectid, old_x, old_y, old_z);
                GetDynamicObjectRot(objectid, old_rx, old_ry, old_rz);

                switch(response)
                {
                    case EDIT_RESPONSE_CANCEL:
                    {
                        SetPlayerFurnitureEditingType(playerid, INVALID_EDITING_TYPE);
                        SetPlayerFurnitureMode(playerid, false);

                        // Set the object to its old position.
                        SetDynamicObjectPos(objectid, old_x, old_y, old_z);
                        SetDynamicObjectRot(objectid, old_rx, old_ry, old_rz);
                    }
                    case EDIT_RESPONSE_FINAL:
                    {
                        // Set the object to its new position.
                        SetDynamicObjectPos(objectid, x, y, z);
                        SetDynamicObjectRot(objectid, rx, ry, rz);

                        SendServerMsg(playerid, "Object Saved [EDITED]");
                        SetPlayerFurnitureEditingType(playerid, INVALID_EDITING_TYPE);
                        SetPlayerFurnitureMode(playerid, false);

                        new
                            arr[2];

                        /*
                            first cell -> house index ID
                            second cell -> furniter index ID
                        */
                        Streamer_GetArrayData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_EXTRA_ID, arr);

                        mysql_tquery(GetSQLHandle(), sprintf(
                        "UPDATE "TABLE_FURNITURE" SET \
                        "FIELD_FURNITURE_POSX" = %0.4f, \
                        "FIELD_FURNITURE_POSY" = %0.4f, \
                        "FIELD_FURNITURE_POSZ" = %0.4f, \
                        "FIELD_FURNITURE_ROTX" = %0.4f, \
                        "FIELD_FURNITURE_ROTY" = %0.4f, \
                        "FIELD_FURNITURE_ROTZ" = %0.4f WHERE "FIELD_FURNITURE_INDEXID" = %i AND "FIELD_FURNITURE_HOUSEID" = %i", x, y, z, rx, ry, rz, arr[1], arr[0]));

                    }
                }
            }
        }
    }
    return 1;
}

hook OnPlayerSelectDynObj(playerid, objectid, modelid, Float:x, Float:y, Float:z)
{
    if(IsPlayerOnFurnitureMode(playerid) && GetPlayerFurnitureEditingType(playerid) == TYPE_EDITING_FURNITURE) {
        EditDynamicObject(playerid, objectid);
    }
    else if (IsPlayerOnFurnitureMode(playerid) && GetPlayerFurnitureEditingType(playerid) == TYPE_DELETING_FURNITURE) {

        inline ConfirmFurnitureDeletion(pid, dialogid, response, listitem, string:inputtext[]) {
            #pragma unused pid, listitem, dialogid, inputtext
            if(!response)
                return ShowPlayerFurnitureDialog(playerid);

            SetPlayerFurnitureEditingType(playerid, INVALID_EDITING_TYPE);
            SetPlayerFurnitureMode(playerid, false);

            new
                arr[2];

            /*
                first cell -> house index ID
                second cell -> furniture index ID
            */
            Streamer_GetArrayData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_EXTRA_ID, arr);
            DeleteFurniture(arr[0], arr[1]);

            DestroyDynamicObject(objectid);
        }

        Dialog_ShowCallback(playerid, using inline ConfirmFurnitureDeletion, DIALOG_STYLE_MSGBOX,
            "Furniture Delete Confirmation",
            ""C_WHITE"Are you sure you want to delete this furniture?\nIt will be "C_RED"PERMANENTLY"C_WHITE" deleted.",
            "Confirm",
            "Back");
    }
    return 1;
}