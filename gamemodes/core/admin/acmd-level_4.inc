

ACMD:[4]setvip(playerid, params[])
{
    new
        targetid,
        level,
        month;

    if (isnull(params)) {
        return SendSyntaxMsg(playerid, "/setvip month | perma");
    }
    if (!strcmp(params, "month", true, 5)) {
        if(sscanf(params, "{s[6]}iii", targetid, level, month)) {
            return SendSyntaxMsg(playerid, "/setvip month <playerid> <Level> <Months>");
        }
        if(targetid == INVALID_PLAYER_ID) {
            return SendErrorMsg(playerid, "The player you are trying to set VIP is invalid");
        }
        if(GetPlayerVIPLevel(targetid) > 0) {
            return SendErrorMsg(playerid, "The player you are trying to set VIP has VIP already.");
        }
        if(!(0 <= level < 3)) {
            return SendErrorMsg(playerid, "Invalid VIP level");
        }

        GivePlayerVIP(targetid, level, 0, month);
    }
    if(!strcmp(params, "perma", true, 5)) {
        if (sscanf(params, "{s[6]}ii", targetid, level)) {
            return SendSyntaxMsg(playerid, "/setvip perma <playerid> <Level>");
        }
        if (targetid == INVALID_PLAYER_ID) {
            return SendErrorMsg(playerid, "The player you are trying to set VIP is invalid");
        }
        if (GetPlayerVIPLevel(targetid) > 0) {
            return SendErrorMsg(playerid, "The player you are trying to set VIP has VIP already.");
        }
        if (!(0 <= level < 3)) {
            return SendErrorMsg(playerid, "Invalid VIP level");
        }

        GivePlayerVIP(targetid, level, 1, 20);
    }

    SendMsgF(targetid, COLOR_LIGHTGREEN, "** VIP ** "C_WHITE"Your VIP level has been set to "C_GREY"%i "C_WHITE"by admin "C_GREY"%p(%i)", level, playerid, playerid);
    SendAdminMessageF("ADMIN "C_GREY"%p "C_WHITE"sets "C_GREY"%p "C_WHITE"VIP level to "C_GREY"%i", playerid, targetid, level);
    return true;
}

ACMD:[4]hcreate(playerid, params[])
{
    if (GetTotalHouses() == INVALID_HOUSE_ID) {
        return SendErrorMsg(playerid, "MAX_HOUSES reached its limit.");
    }

    if (isnull(params) || !IsNumeric(params)) {
        return SendSyntaxMsg(playerid, "/hcreate <House Level> (Available Levels: 1 - 10)");
    }

    new
        level = strval(params);

    if((10 < level <= 0)) {
        return SendErrorMsg(playerid, "Invalid house level, available levels are 1 -> 10");
    }

    new
        Float:houseEntX,
        Float:houseEntY,
        Float:houseEntZ,
        Float:houseEntA,
        price,
        interiorid,
        virtualid,
        Float:houseExtX,
        Float:houseExtY,
        Float:houseExtZ,
        Float:houseExtA;

    GetPlayerPos(playerid, houseEntX, houseEntY, houseEntZ);
    GetPlayerFacingAngle(playerid, houseEntA);

    GetHouseLevelInfo(level, price, houseExtX, houseExtY, houseExtZ, houseExtA, virtualid, interiorid);

    AddHouse(houseEntX, houseEntY, houseEntZ, houseEntA, houseExtX, houseExtY, houseExtZ, houseExtA, virtualid, interiorid, level, price);
    return true;
}

ACMD:[4]hdelete(playerid, params[])
{

    if (isnull(params) || !IsNumeric(params)) {
        return SendSyntaxMsg(playerid, "/hdelete <HouseID>");
    }

    new
        houseid = strval(params),
        ret = RemoveHouse(houseid);

    // if RemoveHouse returned 0, house id doesn't exist in the table.
    if(!ret) {
        SendErrorMsgF(playerid, "The house "C_GREY"(ID:%i) "C_WHITE"you are trying to delete was not found in the database.", houseid);
    } else {
        SendServerMsgF(playerid, "You successfully deleted house id "C_GREY"%i", houseid);
    }
    return true;
}