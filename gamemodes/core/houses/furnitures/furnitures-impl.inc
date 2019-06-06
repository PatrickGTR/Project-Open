

#include <YSI_Data\y_bit>

static
	plyCurrentChoice[MAX_PLAYERS],
	BitArray: playerModeFurniture<MAX_PLAYERS>,
	INTERIOR_EDITING_TYPE:g_playerEditingType[MAX_PLAYERS] = {INVALID_EDITING_TYPE, ...};

INTERIOR_EDITING_TYPE:GetPlayerFurnitureEditingType(playerid) {
	return g_playerEditingType[playerid];
}

INTERIOR_EDITING_TYPE:SetPlayerFurnitureEditingType(playerid, INTERIOR_EDITING_TYPE:type) {
	return g_playerEditingType[playerid] = type;
}

IsPlayerOnFurnitureMode(playerid) {
	return Bit_Get(playerModeFurniture, playerid);
}

SetPlayerFurnitureMode(playerid, bool:edit_state) {
	Bit_Set(playerModeFurniture, playerid, edit_state);
}

ShowPlayerFurniturTypeseDialog(playerid, page) {
	new
		tmp_string[128],
		big_string[2048 * 2]
	;

	plyCurrentChoice[playerid] = page;

 	inline DialogFurniture(ply_id, dialogid, response, listitem, string:inputtext[]) {
    	#pragma unused ply_id, dialogid, inputtext

		if(!response)
			return ShowPlayerFurnitureDialog(playerid);
		new
			index = 0;

		for(new i = 0; i < sizeof(furnitureData); i++) {
			if (furnitureData[i][E_FURNITURE_TYPE] != plyCurrentChoice[playerid])
				continue;

			if (furnitureData[i][E_FURNITURE_LIST_ID] != listitem)
				continue;

			index = i;
			break;
		}
		PlayerBuyFurniture(playerid, index);
		plyCurrentChoice[playerid] = -1;
	}

	strcat(big_string, "Furniture Name\tFurniture Price\n");
	for(new i = 0; i != sizeof(furnitureData); i++) {
		if(furnitureData[i][E_FURNITURE_TYPE] == page) {
			format(tmp_string, sizeof(tmp_string), ""C_WHITE"%s\t"C_GREEN"%m\n", furnitureData[i][E_FURNITURE_NAME], furnitureData[i][E_FURNITURE_PRICE]);
			strcat(big_string, tmp_string);
		}
	}
	Dialog_ShowCallback(playerid, using inline DialogFurniture,
		DIALOG_STYLE_TABLIST_HEADERS,
		"Furniture Shop - Choose Furniture",
		big_string,
		"Select",
		"Back");
	return 1;
}

ShowPlayerFurnitureDialog(playerid) {
    inline DialogFurnitureType(ply_id, dialogid, response, listitem, string:inputtext[]) {
        #pragma unused ply_id, dialogid, inputtext

        if (!response)
        	return SendServerMsg(playerid, "Furniture Type Dialog, closed.");

        ShowPlayerFurniturTypeseDialog(playerid, listitem);
    }

    Dialog_ShowCallback(playerid, using inline DialogFurnitureType,
        DIALOG_STYLE_LIST,
        "Furniture Type",
        "Appliances\nBathroom\nBedroom\nCarpet\nKitchen\nTables\nChairs\nPoster/Frames\nStorage\nPlants\nTrash\nDoor/Gates\nWalls\nDecors",
        "Select", "Close");

    return 1;
}

CreateFurniture(furnitureID, houseID, objectID, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, vw, int) {
  	new
  		tmp_objectid = CreateDynamicObject(objectID, x, y, z, rx, ry, rz, vw, int),
  		arr[2];

    arr[0] = houseID;
    arr[1] = furnitureID;

	return Streamer_SetArrayData(STREAMER_TYPE_OBJECT, tmp_objectid, E_STREAMER_EXTRA_ID, arr);
}

DeleteFurniture(objectid, houseid, furnitureid) {

	Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_EXTRA_ID, houseid);
	mysql_tquery(GetMySQLHandle(),
		sprintf("DELETE FROM "TABLE_FURNITURE" WHERE "FIELD_FURNITURE_INDEXID" = %i AND "FIELD_FURNITURE_HOUSEID" = %i", furnitureid, houseid));
	return furnitureid;
}

DeleteTemporaryFurniture(playerid) {
	DestroyDynamicObject(tempPlayerObjectID[playerid]);

	tempPlayerObjectID[playerid] = INVALID_OBJECT_ID;
	SetPlayerFurnitureEditingType(playerid, INVALID_EDITING_TYPE);
	tempPlayerIndexID[playerid] = -1;

	SetPlayerFurnitureMode(playerid, false);
}

PlayerBuyFurniture(playerid, index) {
	new
        Float: x,
        Float: y,
        Float: z,
	    Float: a;

	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, a);

	SetPlayerFurnitureEditingType(playerid, TYPE_BUYING_FURNITURE);
	tempPlayerObjectID[playerid] = CreateDynamicObject(furnitureData[index][E_FURNITURE_OBJID], x + 2.0 * floatsin(-a, degrees), y + 2.0 * floatcos(-a, degrees), z + 1.0, 0.0, 0.0, a, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid));
    tempPlayerIndexID[playerid] = index;

	SetPlayerFurnitureMode(playerid, true);

	SendMsgF(playerid, COLOR_YELLOW, "** FURNITURE ** "C_WHITE"You are now looking at furniture "C_GREY"%s"C_WHITE". This item costs "C_GREEN"%m", furnitureData[index][E_FURNITURE_NAME], furnitureData[index][E_FURNITURE_PRICE]);
	SendMsgF(playerid, COLOR_YELLOW, "** FURNITURE ** "C_WHITE"Use your cursor to control the editor interface. Click the floppy disk to save changes.");
    EditDynamicObject(playerid, tempPlayerObjectID[playerid]);
}