#include <a_mysql>
#include <YSI_Coding\y_inline>
#include <YSI_Coding\y_hooks>

#include <strlib>

#define T_ITEM_TYPE            	"item_type"
#define F_ITEMID               	"item_id"
#define F_ITEMNAME             	"item_name"

#define T_ITEMPLAYERS          	"item_players"
#define F_FK_USERID            	"fk_user_id"
#define F_FK_ITEMID            	"fk_item_id"
#define F_VALUE              	"value"

enum _: E_MYSQL_ITEM_ID
{
    WEED_ITEM_ID = 0,       // 00
    CRACK_ITEM_ID,          // 01
    ROPE_ITEM_ID,           // 02
    PICKLOCK_ITEM_ID,       // 03
    SCISSORS_ITEM_ID,       // 04
    CONDOM_ITEM_ID,         // 05
    WALLET_ITEM_ID,         // 06
    C4_ITEM_ID              // 07
}

enum E_PLAYER_ITEMS
{
    P_WEED,
    P_CRACK,
    P_ROPE,
    P_PICKLOCK,
    P_SCISSORS,
    P_CONDOM,
    P_WALLET,
    P_C4
}
static 
    playerItems[MAX_PLAYERS][E_PLAYER_ITEMS]
;

hook OnGameModeInit()
{
	mysql_tquery(GetSQLHandle(), "CREATE TABLE IF NOT EXISTS "T_ITEM_TYPE" (\
		"F_ITEMID" SMALLINT(6) NOT NULL, \
		"F_ITEMNAME" VARCHAR(12) NOT NULL, \
		PRIMARY KEY("F_ITEMID"))");

	inline checkItemRows()
	{
		new rows = 0;
		cache_get_value_index_int(0, 0, rows);

		if(!rows)
		{
			mysql_tquery(GetSQLHandle(), "INSERT INTO "T_ITEM_TYPE" ("F_ITEMID", "F_ITEMNAME") \
			VALUES \
			(0, 'weed'), \
			(1, 'crack'), \
			(2, 'rope'), \
			(3, 'picklock'), \
			(4, 'scissors'), \
			(5, 'condom'), \
			(6, 'wallet'), \
			(7, 'c4')");
		}
    }
	MySQL_TQueryInline(GetSQLHandle(), using inline checkItemRows, "SELECT COUNT("F_ITEMID") FROM "T_ITEM_TYPE"");

	mysql_tquery(GetSQLHandle(), "CREATE TABLE IF NOT EXISTS "T_ITEMPLAYERS"(\
		"F_FK_USERID" INT(11) NOT NULL, \
		"F_FK_ITEMID" SMALLINT(6) NOT NULL, \
		"F_VALUE" SMALLINT(6) NOT NULL, \
        PRIMARY KEY ("F_FK_USERID", "F_FK_ITEMID"), \
        FOREIGN KEY ("F_FK_USERID") REFERENCES "T_ACCOUNT"("F_ACCOUNTID") ON DELETE CASCADE ON UPDATE CASCADE, \
        FOREIGN KEY ("F_FK_ITEMID") REFERENCES "T_ITEM_TYPE"("F_ITEMID") ON DELETE CASCADE ON UPDATE CASCADE)");
}


hook OnPlayerLogin(playerid)
{
	inline OnPlayerItemsLoad()
	{
		new
			rows = cache_num_rows();
		if (rows)
		{
			new
				item_id = 0,
				item_val = 0;


			for(new i = 0; i < rows; i++)
			{
				cache_get_value_name_int(i, F_FK_ITEMID, item_id);
				cache_get_value_name_int(i, F_VALUE, item_val);

				switch(item_id)
				{
					case WEED_ITEM_ID:
						playerItems[playerid][P_WEED] = item_val;
					case CRACK_ITEM_ID:
						playerItems[playerid][P_CRACK] = item_val;
					case ROPE_ITEM_ID:
						playerItems[playerid][P_ROPE] = item_val;
					case PICKLOCK_ITEM_ID:
						playerItems[playerid][P_PICKLOCK] = item_val;
					case SCISSORS_ITEM_ID:
						playerItems[playerid][P_SCISSORS] = item_val;
					case CONDOM_ITEM_ID:
						playerItems[playerid][P_CONDOM] = item_val;
					case WALLET_ITEM_ID:
						playerItems[playerid][P_WALLET] = item_val;
					case C4_ITEM_ID:
						playerItems[playerid][P_C4] = item_val;
				}
			}

		}
	}

	MySQL_TQueryInline(
        GetSQLHandle(), 
        using inline OnPlayerItemsLoad, 
        "SELECT p."F_FK_ITEMID", p."F_VALUE", t."F_ITEMID" FROM \
		"T_ITEMPLAYERS" as p INNER JOIN "T_ITEM_TYPE" as t \
		on p."F_FK_ITEMID" = t."F_ITEMID" \
		WHERE p."F_FK_USERID" = %i", GetPlayerAccountID(playerid)
    );
}

/*
    [USAGE]
    
    SetPlayerWeed(playerid, amount)
    GivePlayerWeed(playerid, amount)
    GetPlayerWeed(playerid)
*/

SetPlayerWeed(playerid, amount, item_id = WEED_ITEM_ID)
{   
    mysql_tquery(GetSQLHandle(), sprintf("INSERT INTO "T_ITEMPLAYERS" ("F_FK_USERID", "F_FK_ITEMID", "F_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "F_VALUE" = %i", GetPlayerAccountID(playerid), item_id, amount, amount));
    return playerItems[playerid][P_WEED] = amount;
}

GivePlayerWeed(playerid, amount)
    return SetPlayerWeed(playerid, GetPlayerWeed(playerid) + amount);
GetPlayerWeed(playerid)
    return playerItems[playerid][P_WEED];

/*
    [USAGE]
    
    SetPlayerCrack(playerid, amount)
    GivePlayerCrack(playerid, amount)
    GetPlayerCrack(playerid)
*/

SetPlayerCrack(playerid, amount, item_id = CRACK_ITEM_ID)
{
    mysql_tquery(GetSQLHandle(), sprintf("INSERT INTO "T_ITEMPLAYERS" ("F_FK_USERID", "F_FK_ITEMID", "F_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "F_VALUE" = %i", GetPlayerAccountID(playerid), item_id, amount, amount));
    return playerItems[playerid][P_CRACK] = amount;
}

GivePlayerCrack(playerid, amount)
    return SetPlayerCrack(playerid, GetPlayerCrack(playerid) + amount);

GetPlayerCrack(playerid)
    return playerItems[playerid][P_CRACK];

/*
    [USAGE]
    
    SetPlayerRope(playerid, amount)
    GivePlayerRope(playerid, amount)
    GetPlayerRope(playerid)
*/

SetPlayerRope(playerid, amount, item_id = ROPE_ITEM_ID)
{
    mysql_tquery(GetSQLHandle(), sprintf("INSERT INTO "T_ITEMPLAYERS" ("F_FK_USERID", "F_FK_ITEMID", "F_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "F_VALUE" = %i", GetPlayerAccountID(playerid), item_id, amount, amount));
    return playerItems[playerid][P_ROPE] = amount;
}

GivePlayerRope(playerid, amount)
    return SetPlayerRope(playerid, GetPlayerRope(playerid) + amount);
GetPlayerRope(playerid)
    return playerItems[playerid][P_ROPE];

/*
    [USAGE]
    
    SetPlayerCondom(playerid, amount)
    GivePlayerCondom(playerid, amount)
    GetPlayerCondom(playerid)
*/

SetPlayerCondom(playerid, amount, item_id = CONDOM_ITEM_ID)
{
    mysql_tquery(GetSQLHandle(), sprintf("INSERT INTO "T_ITEMPLAYERS" ("F_FK_USERID", "F_FK_ITEMID", "F_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "F_VALUE" = %i", GetPlayerAccountID(playerid), item_id, amount, amount));
    return playerItems[playerid][P_CONDOM] = amount;
}

GivePlayerCondom(playerid, amount)
    return SetPlayerCondom(playerid, GetPlayerCondom(playerid) + amount);
GetPlayerCondom(playerid)
    return playerItems[playerid][P_CONDOM];

/*
    [USAGE]
    
    SetPlayerWallet(playerid, amount)
    GivePlayerWallet(playerid, amount)
    GetPlayerWallet(playerid)
*/
SetPlayerWallet(playerid, amount, item_id = WALLET_ITEM_ID)
{
    mysql_tquery(GetSQLHandle(), sprintf("INSERT INTO "T_ITEMPLAYERS" ("F_FK_USERID", "F_FK_ITEMID", "F_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "F_VALUE" = %i", GetPlayerAccountID(playerid), item_id, amount, amount));
    return playerItems[playerid][P_WALLET] = amount;
}

GivePlayerWallet(playerid, amount)
    return SetPlayerWallet(playerid, GetPlayerWallet(playerid) + amount);

GetPlayerWallet(playerid)
    return playerItems[playerid][P_WALLET];

/*
    [USAGE]
    
    SetPlayerPickLock(playerid, amount)
    GivePlayerPickLock(playerid, amount)
    GetPlayerPickLock(playerid)
*/
SetPlayerPickLock(playerid, amount, item_id = PICKLOCK_ITEM_ID)
{
    mysql_tquery(GetSQLHandle(), sprintf("INSERT INTO "T_ITEMPLAYERS" ("F_FK_USERID", "F_FK_ITEMID", "F_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "F_VALUE" = %i", GetPlayerAccountID(playerid), item_id, amount, amount));
    return playerItems[playerid][P_PICKLOCK] = amount;
}

GivePlayerPickLock(playerid, amount)
    return SetPlayerPickLock(playerid, GetPlayerPickLock(playerid) + amount);
GetPlayerPickLock(playerid)
    return playerItems[playerid][P_PICKLOCK];


/*
    [USAGE]

    SetPlayerScissors(playerid, amount)
    GivePlayerScissors(playerid, amount)
    GetPlayerScissors(playerid)
*/
SetPlayerScissors(playerid, amount, item_id = SCISSORS_ITEM_ID)
{
    mysql_tquery(GetSQLHandle(), sprintf("INSERT INTO "T_ITEMPLAYERS" ("F_FK_USERID", "F_FK_ITEMID", "F_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "F_VALUE" = %i", GetPlayerAccountID(playerid), item_id, amount, amount));
    return playerItems[playerid][P_SCISSORS] = amount;
}

GivePlayerScissors(playerid, amount)   
    return SetPlayerScissors(playerid, GetPlayerPickLock(playerid) + amount);

GetPlayerScissors(playerid)
    return playerItems[playerid][P_SCISSORS];

/* 
    [USAGE]

    SetPlayerC4(playerid, amount)
    GivePlayerC4(playerid, amount)
    GetPlayerC4(playerid)
*/
SetPlayerC4(playerid, amount, item_id = C4_ITEM_ID)
{
    mysql_tquery(GetSQLHandle(), sprintf("INSERT INTO "T_ITEMPLAYERS" ("F_FK_USERID", "F_FK_ITEMID", "F_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "F_VALUE" = %i", GetPlayerAccountID(playerid), item_id, amount, amount));
    return playerItems[playerid][P_C4] = amount;
}

GivePlayerC4(playerid, amount)
    return SetPlayerC4(playerid, GetPlayerC4(playerid) + amount);

GetPlayerC4(playerid)
    return playerItems[playerid][P_C4];


#include <items-shop>