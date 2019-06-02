DeleteAccount(playerid, const accountName[])
{
    inline const OnDeleteAccount() {
        if (!cache_num_rows()) {
            SendErrorMsgF(playerid, "The account "C_GREY"('%s') "C_WHITE"you are trying to delete does not exist in the database", accountName);
            return 0;
        } else {
            new
                acc_id;

            cache_get_value_name_int(0, F_ACCOUNTID, acc_id);

            mysql_tquery(GetMySQLHandle(), sprintf("DELETE FROM "T_ACCOUNT" WHERE "F_ACCOUNTID" = %i", acc_id));

            SendServerMsgF(playerid, "The account "C_GREY"(Nane: '%s' - accountID: %i) has been successfully deleted!", accountName, acc_id);
        }
        return 1;
    }
    MySQL_TQueryInline(
        GetMySQLHandle(),
        using inline OnDeleteAccount,
        "SELECT "F_USERNAME", "F_ACCOUNTID" FROM "T_ACCOUNT" WHERE "F_USERNAME" = '%e' LIMIT 1", accountName
    );
    return 1;
}

PromptLogin(playerid) {
    new string[512];
    inline prompt_log_Response(ply_id, dialogid, response, listitem, string:inputtext[]) {
        #pragma unused ply_id, dialogid, listitem
        if (!response)
            return Kick(playerid);

        LoadAccount(playerid, inputtext);
    }

    format(string, sizeof(string), "\
        "C_WHITE"Welcome back!. This account "C_GREEN"%p "C_WHITE"is registered, type your password below to login.\n\n\
        "C_RED"NOTE: "C_WHITE"If you are not the owner of this account please re-connect with a different name.\n\nEnjoy! - "SV_NAME" Staff Team :)", playerid);
    Dialog_ShowCallback(playerid, using inline prompt_log_Response, DIALOG_STYLE_PASSWORD, "Please login...", string, "Login", "Leave");
    return 1;
}

GetPlayerNameFromAccountID(accountid, plyName[MAX_PLAYER_NAME])
{

    new
        qString[64];

    mysql_format(GetMySQLHandle(), qString, sizeof(qString), "SELECT "F_USERNAME" FROM "T_ACCOUNT" WHERE "F_ACCOUNTID" = %i", accountid);
    new Cache: res = mysql_query(GetMySQLHandle(), qString, true);

    new
        ret_name[24];
    if(cache_num_rows())
        cache_get_value_index(0, 0, ret_name);
    else
        ret_name = "N/A";

    strcpy(plyName, ret_name, MAX_PLAYER_NAME);

    cache_delete(res);
}