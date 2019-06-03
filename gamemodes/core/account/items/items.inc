#include <a_mysql>
#include <YSI_Coding\y_inline>
#include <YSI_Coding\y_hooks>

#include <strlib>

#define TABLE_ITEM_TYPE            	"item_type"
#define FIELD_ITEMID               	"item_id"
#define FIELD_ITEMNAME             	"item_name"

#define TABLE_ITEMPLAYERS          	"item_players"
#define FIELD_FK_USERID            	"fk_account_id"
#define FIELD_FK_ITEMID            	"fk_item_id"
#define FIELD_VALUE              	"value"

enum _: E_MYSQL_ITEM_ID
{
    E_WEED_ITEMID = 0,       // 00
    E_CRACK_ITEMID,          // 01
    E_ROPE_ITEMID,           // 02
    E_PICKLOCK_ITEMID,       // 03
    E_SCISSORS_ITEMID,       // 04
    E_CONDOM_ITEMID,         // 05
    E_WALLET_ITEMID,         // 06
    E_C4_ITEMID              // 07
}

enum E_PLAYER_ITEMS
{
    E_PLAYER_WEED,
    E_PLAYER_CRACK,
    E_PLAYER_ROPE,
    E_PLAYER_PICKLOCK,
    E_PLAYER_SCISSORS,
    E_PLAYER_CONDOM,
    E_PLAYER_WALLET,
    E_PLAYER_C4
}
static
    playerItems[MAX_PLAYERS][E_PLAYER_ITEMS]
;

hook OnGameModeInit()
{
	mysql_tquery(GetMySQLHandle(), "CREATE TABLE IF NOT EXISTS "TABLE_ITEM_TYPE" (\
		"FIELD_ITEMID" SMALLINT(6) NOT NULL, \
		"FIELD_ITEMNAME" VARCHAR(12) NOT NULL, \
		PRIMARY KEY("FIELD_ITEMID"))");

	inline checkItemRows()
	{
		new rows = 0;
		cache_get_value_index_int(0, 0, rows);

		if(!rows)
		{
			mysql_tquery(GetMySQLHandle(), "INSERT INTO "TABLE_ITEM_TYPE" ("FIELD_ITEMID", "FIELD_ITEMNAME") \
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
	MySQL_TQueryInline(GetMySQLHandle(), using inline checkItemRows, "SELECT COUNT("FIELD_ITEMID") FROM "TABLE_ITEM_TYPE"");

	mysql_tquery(GetMySQLHandle(), "CREATE TABLE IF NOT EXISTS "TABLE_ITEMPLAYERS"(\
		"FIELD_FK_USERID" INT(11) NOT NULL, \
		"FIELD_FK_ITEMID" SMALLINT(6) NOT NULL, \
		"FIELD_VALUE" SMALLINT(6) NOT NULL, \
        PRIMARY KEY ("FIELD_FK_USERID", "FIELD_FK_ITEMID"), \
        FOREIGN KEY ("FIELD_FK_USERID") REFERENCES "TABLE_ACCOUNT"("FIELD_ACCOUNTID") ON DELETE CASCADE ON UPDATE CASCADE, \
        FOREIGN KEY ("FIELD_FK_ITEMID") REFERENCES "TABLE_ITEM_TYPE"("FIELD_ITEMID") ON DELETE CASCADE ON UPDATE CASCADE)");
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
				cache_get_value_name_int(i, FIELD_FK_ITEMID, item_id);
				cache_get_value_name_int(i, FIELD_VALUE, item_val);

				switch(item_id)
				{
					case E_WEED_ITEMID:
						playerItems[playerid][E_PLAYER_WEED] = item_val;
					case E_CRACK_ITEMID:
						playerItems[playerid][E_PLAYER_CRACK] = item_val;
					case E_ROPE_ITEMID:
						playerItems[playerid][E_PLAYER_ROPE] = item_val;
					case E_PICKLOCK_ITEMID:
						playerItems[playerid][E_PLAYER_PICKLOCK] = item_val;
					case E_SCISSORS_ITEMID:
						playerItems[playerid][E_PLAYER_SCISSORS] = item_val;
					case E_CONDOM_ITEMID:
						playerItems[playerid][E_PLAYER_CONDOM] = item_val;
					case E_WALLET_ITEMID:
						playerItems[playerid][E_PLAYER_WALLET] = item_val;
					case E_C4_ITEMID:
						playerItems[playerid][E_PLAYER_C4] = item_val;
				}
			}

		}
	}

	MySQL_TQueryInline(
        GetMySQLHandle(),
        using inline OnPlayerItemsLoad,
        "SELECT p."FIELD_FK_ITEMID", p."FIELD_VALUE", t."FIELD_ITEMID" FROM \
		"TABLE_ITEMPLAYERS" as p INNER JOIN "TABLE_ITEM_TYPE" as t \
		on p."FIELD_FK_ITEMID" = t."FIELD_ITEMID" \
		WHERE p."FIELD_FK_USERID" = %i", GetPlayerAccountID(playerid)
    );
}

/*
    [USAGE]

    SetPlayerWeed(playerid, amount)
    GivePlayerWeed(playerid, amount)
    GetPlayerWeed(playerid)
*/

SetPlayerWeed(playerid, amount, item_id = E_WEED_ITEMID)
{
    mysql_tquery(GetMySQLHandle(), sprintf("INSERT INTO "TABLE_ITEMPLAYERS" ("FIELD_FK_USERID", "FIELD_FK_ITEMID", "FIELD_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "FIELD_VALUE" = %i", GetPlayerAccountID(playerid), item_id, amount, amount));
    return playerItems[playerid][E_PLAYER_WEED] = amount;
}

GivePlayerWeed(playerid, amount)
    return SetPlayerWeed(playerid, GetPlayerWeed(playerid) + amount);
GetPlayerWeed(playerid)
    return playerItems[playerid][E_PLAYER_WEED];

/*
    [USAGE]

    SetPlayerCrack(playerid, amount)
    GivePlayerCrack(playerid, amount)
    GetPlayerCrack(playerid)
*/

SetPlayerCrack(playerid, amount, item_id = E_CRACK_ITEMID)
{
    mysql_tquery(GetMySQLHandle(), sprintf("INSERT INTO "TABLE_ITEMPLAYERS" ("FIELD_FK_USERID", "FIELD_FK_ITEMID", "FIELD_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "FIELD_VALUE" = %i", GetPlayerAccountID(playerid), item_id, amount, amount));
    return playerItems[playerid][E_PLAYER_CRACK] = amount;
}

GivePlayerCrack(playerid, amount)
    return SetPlayerCrack(playerid, GetPlayerCrack(playerid) + amount);

GetPlayerCrack(playerid)
    return playerItems[playerid][E_PLAYER_CRACK];

/*
    [USAGE]

    SetPlayerRope(playerid, amount)
    GivePlayerRope(playerid, amount)
    GetPlayerRope(playerid)
*/

SetPlayerRope(playerid, amount, item_id = E_ROPE_ITEMID)
{
    mysql_tquery(GetMySQLHandle(), sprintf("INSERT INTO "TABLE_ITEMPLAYERS" ("FIELD_FK_USERID", "FIELD_FK_ITEMID", "FIELD_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "FIELD_VALUE" = %i", GetPlayerAccountID(playerid), item_id, amount, amount));
    return playerItems[playerid][E_PLAYER_ROPE] = amount;
}

GivePlayerRope(playerid, amount)
    return SetPlayerRope(playerid, GetPlayerRope(playerid) + amount);
GetPlayerRope(playerid)
    return playerItems[playerid][E_PLAYER_ROPE];

/*
    [USAGE]

    SetPlayerCondom(playerid, amount)
    GivePlayerCondom(playerid, amount)
    GetPlayerCondom(playerid)
*/

SetPlayerCondom(playerid, amount, item_id = E_CONDOM_ITEMID)
{
    mysql_tquery(GetMySQLHandle(), sprintf("INSERT INTO "TABLE_ITEMPLAYERS" ("FIELD_FK_USERID", "FIELD_FK_ITEMID", "FIELD_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "FIELD_VALUE" = %i", GetPlayerAccountID(playerid), item_id, amount, amount));
    return playerItems[playerid][E_PLAYER_CONDOM] = amount;
}

GivePlayerCondom(playerid, amount)
    return SetPlayerCondom(playerid, GetPlayerCondom(playerid) + amount);
GetPlayerCondom(playerid)
    return playerItems[playerid][E_PLAYER_CONDOM];

/*
    [USAGE]

    SetPlayerWallet(playerid, amount)
    GivePlayerWallet(playerid, amount)
    GetPlayerWallet(playerid)
*/
SetPlayerWallet(playerid, amount, item_id = E_WALLET_ITEMID)
{
    mysql_tquery(GetMySQLHandle(), sprintf("INSERT INTO "TABLE_ITEMPLAYERS" ("FIELD_FK_USERID", "FIELD_FK_ITEMID", "FIELD_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "FIELD_VALUE" = %i", GetPlayerAccountID(playerid), item_id, amount, amount));
    return playerItems[playerid][E_PLAYER_WALLET] = amount;
}

GivePlayerWallet(playerid, amount)
    return SetPlayerWallet(playerid, GetPlayerWallet(playerid) + amount);

GetPlayerWallet(playerid)
    return playerItems[playerid][E_PLAYER_WALLET];

/*
    [USAGE]

    SetPlayerPickLock(playerid, amount)
    GivePlayerPickLock(playerid, amount)
    GetPlayerPickLock(playerid)
*/
SetPlayerPickLock(playerid, amount, item_id = E_PICKLOCK_ITEMID)
{
    mysql_tquery(GetMySQLHandle(), sprintf("INSERT INTO "TABLE_ITEMPLAYERS" ("FIELD_FK_USERID", "FIELD_FK_ITEMID", "FIELD_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "FIELD_VALUE" = %i", GetPlayerAccountID(playerid), item_id, amount, amount));
    return playerItems[playerid][E_PLAYER_PICKLOCK] = amount;
}

GivePlayerPickLock(playerid, amount)
    return SetPlayerPickLock(playerid, GetPlayerPickLock(playerid) + amount);
GetPlayerPickLock(playerid)
    return playerItems[playerid][E_PLAYER_PICKLOCK];


/*
    [USAGE]

    SetPlayerScissors(playerid, amount)
    GivePlayerScissors(playerid, amount)
    GetPlayerScissors(playerid)
*/
SetPlayerScissors(playerid, amount, item_id = E_SCISSORS_ITEMID)
{
    mysql_tquery(GetMySQLHandle(), sprintf("INSERT INTO "TABLE_ITEMPLAYERS" ("FIELD_FK_USERID", "FIELD_FK_ITEMID", "FIELD_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "FIELD_VALUE" = %i", GetPlayerAccountID(playerid), item_id, amount, amount));
    return playerItems[playerid][E_PLAYER_SCISSORS] = amount;
}

GivePlayerScissors(playerid, amount)
    return SetPlayerScissors(playerid, GetPlayerPickLock(playerid) + amount);

GetPlayerScissors(playerid)
    return playerItems[playerid][E_PLAYER_SCISSORS];

/*
    [USAGE]

    SetPlayerC4(playerid, amount)
    GivePlayerC4(playerid, amount)
    GetPlayerC4(playerid)
*/
SetPlayerC4(playerid, amount, item_id = E_C4_ITEMID)
{
    mysql_tquery(GetMySQLHandle(), sprintf("INSERT INTO "TABLE_ITEMPLAYERS" ("FIELD_FK_USERID", "FIELD_FK_ITEMID", "FIELD_VALUE") \
       VALUES (%i, %i, %i) ON DUPLICATE KEY UPDATE "FIELD_VALUE" = %i", GetPlayerAccountID(playerid), item_id, amount, amount));
    return playerItems[playerid][E_PLAYER_C4] = amount;
}

GivePlayerC4(playerid, amount)
    return SetPlayerC4(playerid, GetPlayerC4(playerid) + amount);

GetPlayerC4(playerid)
    return playerItems[playerid][E_PLAYER_C4];


#include <items-shop>