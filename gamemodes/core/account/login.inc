DeleteAccount(playerid, const username[])
{
    inline const OnDeleteAccount() {
        if (!cache_num_rows()) {
            SendErrorMsgF(playerid, "The account "C_GREY"('%s') "C_WHITE"you are trying to delete does not exist in the database", username);
            return 0;
        } else {
            new
                acc_id;

            cache_get_value_name_int(0, FIELD_ACCOUNTID, acc_id);

            mysql_tquery(GetMySQLHandle(), sprintf("DELETE FROM "TABLE_ACCOUNT" WHERE "FIELD_ACCOUNTID" = %i", acc_id));

            SendServerMsgF(playerid, "The account "C_GREY"(Nane: '%s' - accountID: %i) has been successfully deleted!", username, acc_id);
        }
        return 1;
    }
    MySQL_TQueryInline(
        GetMySQLHandle(),
        using inline OnDeleteAccount,
        "SELECT "FIELD_USERNAME", "FIELD_ACCOUNTID" FROM "TABLE_ACCOUNT" WHERE "FIELD_USERNAME" = '%e' LIMIT 1", username
    );
    return 1;
}

PromptLogin(playerid, const salt[], const password[]) {

    // y_inline fix
    // this issue: https://github.com/pawn-lang/YSI-Includes/issues/249
    new
        string[512],
        fix_salt[MAX_HASH],
        fix_password[MAX_PASSWORD];

    // copy to local
    strcpy(fix_salt, salt, MAX_HASH);
    strcpy(fix_password, password, MAX_PASSWORD);


    inline PromptLoginResponse(ply_id, dialogid, response, listitem, string:inputtext[]) {
        // we don't need these
        #pragma unused ply_id, dialogid, listitem

        // if user does not respond, kick him.
        if (!response) {
            Kick(playerid);
            return;
        }

        // if user does respond, take their password, saved salt and saved password.
        LoadAccount(playerid, inputtext, fix_salt, fix_password);
    }

    format(string, sizeof(string), "\
        "C_WHITE"Welcome back!. This account "C_GREEN"%p "C_WHITE"is registered, type your password below to login.\n\n\
        "C_RED"NOTE: "C_WHITE"If you are not the owner of this account please re-connect with a different name.\n\nEnjoy! - "SV_NAME" Staff Team :)", playerid);

    Dialog_ShowCallback(playerid,
        using inline PromptLoginResponse, // Handler
        DIALOG_STYLE_PASSWORD,            // Style
        "Please login...",                // Title
        string,                           // Content
        "Login",                          // Button Left
        "Leave");                         // Button Right
}

GetPlayerNameFromAccountID(accountid, plyName[MAX_PLAYER_NAME])
{

    new
        qString[64];

    mysql_format(GetMySQLHandle(), qString, sizeof(qString), "SELECT "FIELD_USERNAME" FROM "TABLE_ACCOUNT" WHERE "FIELD_ACCOUNTID" = %i", accountid);
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