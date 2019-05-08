#include <YSI_Coding\y_hooks>
#include <YSI_Data\y_iterate>
#include <formatex>
#include <strlib>

#define MAX_ROBBERY_ACTOR   (50)

#define TYPE_SUCCESS        (0)
#define TYPE_FAILED         (1)
#define TYPE_UNFINISHED     (2)

#define MIN_MONEY_ROB       500
#define MAX_MONEY_ROB       5000
#define ROBBERY_WAIT_TIME   (5)

enum E_ROBBERY_DATA
{
    actor_id, 
    actor_skinID,
    Float:actor_posX,
    Float:actor_posY,
    Float:actor_posZ,
    Float:actor_posA,
    actor_VWID,
    actor_robbedRecently
}
new
    robbery_data[MAX_ROBBERY_ACTOR][E_ROBBERY_DATA],
    Iterator:robberyIndex<MAX_ROBBERY_ACTOR>;

forward OnPlayerRobbery(playerid, actorid, robbedmoney, type);
forward OnPlayerStartRobbery(playerid, actorid, robbed_recently);

hook OnPlayerConnect(playerid)
{
     EnablePlayerCameraTarget(playerid, 1);
     return Y_HOOKS_CONTINUE_RETURN_1;
}

#define KEY_AIM 128

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if ((newkeys & KEY_AIM) == KEY_AIM && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
    {
        switch(GetPlayerWeapon(playerid))
        {
            case 22 .. 33:
            {
                new actorid = GetPlayerCameraTargetActor(playerid);

                if (actorid == INVALID_ACTOR_ID)
                    return 1;

                if (gettime() - robbery_data[actorid][actor_robbedRecently] < 60 * ROBBERY_WAIT_TIME)
                {
                    OnPlayerStartRobbery(playerid, actorid, 1);
                    return 1;
                }

                if (IsPlayerLEO(playerid))
                {
                    SendErrorMsg(playerid, "You cannot rob stores when you are a cop!");
                }
                else
                {
                    OnPlayerStartRobbery(playerid, actorid, 0);
                    RunActorAnimationSequence(playerid, actorid, 0);
                    robbery_data[actorid][actor_robbedRecently] = gettime();
                }
            }
        }
    }
    return Y_HOOKS_CONTINUE_RETURN_1;
}

public OnPlayerStartRobbery(playerid, actorid, robbed_recently)
{
    if (!robbed_recently)
    {
        SendMsgAllF(COLOR_ORANGE, "** ROBBERY ** "C_GREY"%p "C_WHITE"has began robbing %s in %s.", playerid, GetEntranceName(playerid), GetPlayerLocation(playerid));
        SendMsgF(playerid, COLOR_ORANGE, "** ROBBERY ** "C_WHITE"Stay within 10 meters to complete the robbery!");
        GivePlayerWantedLevel(playerid, 2);
    }
    else
    {
        SendErrorMsgF(playerid, "The store "C_GREY"%s "C_WHITE"has been robbed recently, please try again later!", GetEntranceName(playerid));
    }
    return 1;
}

public OnPlayerRobbery(playerid, actorid, robbedmoney, type)
{
    switch(type)
    {
        case TYPE_SUCCESS:
        {
            GameTextForPlayer(playerid, sprintf("~g~+ ~w~%m", robbedmoney), 6000, 1);

            SendMsgAllF(COLOR_ORANGE, "** ROBBERY ** "C_GREY"%p "C_WHITE"has robbed "C_GREEN"%m "C_WHITE"from %s in %s.", playerid, robbedmoney, GetEntranceName(playerid), GetPlayerLocation(playerid));

            GivePlayerTotalStoreRob(playerid, 1);
            GivePlayerMoney(playerid, robbedmoney);
        }
        case TYPE_FAILED:
        {
            GameTextForPlayer(playerid, "~r~Robbery Failed", 6000, 1);

            SendClientMessage(playerid, COLOR_ORANGE, "** ROBBERY FAILED ** "C_WHITE"Cashier refused to give money");
            SendMsgAllF(COLOR_ORANGE, "** ROBBERY **  "C_GREY"%p "C_WHITE"tried to rob %s in %s but the cashier refused to give the money.", playerid, GetEntranceName(playerid), GetPlayerLocation(playerid));
        }
        case TYPE_UNFINISHED:
        {

            GameTextForPlayer(playerid, "~r~Robbery Failed", 6000, 1);

            SendClientMessage(playerid, COLOR_ORANGE, "** ROBBERY FAILED ** "C_WHITE"You left the cashiers area.");
            SendMsgAllF(COLOR_ORANGE, "** ROBBERY **  "C_GREY"%p "C_WHITE"tried to rob %s in %s but he went to far for the cashier to call 911", playerid, GetEntranceName(playerid), GetPlayerLocation(playerid));
        }
    }
    GivePlayerWantedLevel(playerid, 2);
    GivePlayerXP(playerid, 2);
    return 1;
}

CreateActorRobbery(const label[], skinid, Float:x, Float:y, Float:z, Float:a, vwid)
{
    if (Iter_Count(robberyIndex) == -1)
    {
        print("ERROR: MAX_ROBBERY_ACTOR reached, increase the limit size.");
        return -1;
    }

    new 
        findID = Iter_Free(robberyIndex);

    robbery_data[findID][actor_id] = CreateActor(skinid, x, y, z, a);
    SetActorVirtualWorld(robbery_data[findID][actor_id], vwid);
    robbery_data[findID][actor_skinID] = skinid;
    robbery_data[findID][actor_posX] = x;
    robbery_data[findID][actor_posY] = y;
    robbery_data[findID][actor_posZ] = z;
    robbery_data[findID][actor_posA] = a;

    CreateDynamic3DTextLabel(label, COLOR_YELLOW, 
        robbery_data[findID][actor_posX], robbery_data[findID][actor_posY], robbery_data[findID][actor_posZ], 50.0, .worldid = vwid, .testlos = 1, .streamdistance = 50);

    Iter_Add(robberyIndex, findID);
    return findID;
}

timer RunActorAnimationSequence[1000 * 5](playerid, actorid, animation_pattern) 
{
    switch(animation_pattern)
    {
        case 0:
        {
            ClearActorAnimations(actorid);
            ApplyActorAnimation(actorid, "SHOP", "SHP_Rob_HandsUp", 4.1, 0, 1, 1, 1, 0);
            defer RunActorAnimationSequence(playerid, actorid, 1);

            new Float:x, Float:y, Float:z;
            GetPlayerPos(playerid, x, y, z);
        
            foreach(new i : Player)
            {
                PlayerPlaySound(i, 3401, x, y, z);
            }
        }
        case 1:
        {
            if (!IsPlayerInRangeOfPoint(playerid, 10.0, robbery_data[actorid][actor_posX], robbery_data[actorid][actor_posY], robbery_data[actorid][actor_posZ]))
            {
               OnPlayerRobbery(playerid, actorid, 0, TYPE_UNFINISHED);
            }
            else 
            {
                ClearActorAnimations(actorid);
                ApplyActorAnimation(actorid, "SHOP", "SHP_Rob_GiveCash", 4.1, 0, 1, 1, 1, 0);
                defer RunActorAnimationSequence(playerid, actorid, 2);
            }
        }
        case 2:
        {
            ClearActorAnimations(actorid);
            ApplyActorAnimation(actorid, "PED", "DUCK_cower", 4.1, 1, 1, 1, 1, 1);

            new 
                success_rate = RandomChance(65);
           
            if (success_rate)
            {
                OnPlayerRobbery(playerid, actorid, (random(MAX_MONEY_ROB - MIN_MONEY_ROB) + MIN_MONEY_ROB), TYPE_SUCCESS);
            }
            else OnPlayerRobbery(playerid, actorid, 0, TYPE_FAILED);

            defer clearActorAnim(actorid);
        }
    }
}

timer clearActorAnim[1000 * 60 * ROBBERY_WAIT_TIME](actorid)
{
    ClearActorAnimations(actorid);
    foreach(new i : Player)
    {
        PlayerPlaySound(i, 0, 0, 0, 0);
    }
}
