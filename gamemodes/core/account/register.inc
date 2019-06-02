PromptRegister(playerid) {

    static string[512];
    inline prompt_reg_Response(ply_id, dialogid, response, listitem, string:inputtext[]) {
        #pragma unused ply_id, dialogid, listitem
        if (!response)
            return Kick(playerid);

        CreateAccount(playerid, inputtext);
    }

    format(string, sizeof(string), "\
        "C_WHITE"Welcome to "SV_NAME". This account "C_RED"%p "C_WHITE"is not registered, type your password below to register.\n\n\
        "C_RED"NOTE: "C_WHITE"Once you are registered we suggest that you "C_RED"DO NOT "C_WHITE"share your password with anyone even the server owner.\n\n\
        Once you are spawned please read "C_YELLOW"/rules, /pc and /commands "C_WHITE"to make sure you understand the server concepts.\n\nEnjoy! - "SV_NAME" Staff Team :)", playerid);
    Dialog_ShowCallback(playerid,
        using inline prompt_reg_Response,
        DIALOG_STYLE_PASSWORD, // style
        "Please register...", // title
        string, // content
        "Register", // button left
        "Leave"); // button right
    return 1;
}

CreateAccount(playerid, const password[])
{
    if (!(3 <= strlen(password) <= 20)) {
        SendErrorMsg(playerid, "Invalid length on the password. It should be between 3-20 characters" );
        PromptRegister(playerid);
        return 1;
    }
    if (isnumeric(password)) {
        SendErrorMsg(playerid, "Your password is invalid. The password should include alphabets.");
        PromptRegister(playerid);
        return 1;
    }

    new
        bool:ret;
    for(new i = 0; password[i] != EOS; ++i) {
        switch(password[i]) {
            case '0'..'9', 'A'..'Z', 'a'..'z': ret = true;
            default: ret = false;
        }
    }

    if(!ret) {
        SendErrorMsg(playerid, "Your password is invalid. Valid characters are: A-Z, a-z, 0-9.");
        PromptRegister(playerid);
        return 1;
    }


    new
        salt[MAX_HASH],
        _playerPassword[MAX_PASSWORD];

    for(new i; i < 10; i++) {
        salt[i] = random(79) + 47;
    }
    salt[10] = 0;


    SHA256_PassHash(password, salt, _playerPassword, MAX_PASSWORD);

    inline OnPlayerRegister() {
        SetPlayerAccountID(playerid, cache_insert_id());

        SendServerMsg(playerid, "You have just registered to our server! You have been automatically logged in!");
    }

    new
        playerName[MAX_PLAYER_NAME];
    GetPlayerName(playerid, playerName, sizeof(playerName));


    MySQL_PQueryInline(GetMySQLHandle(), using inline OnPlayerRegister, "INSERT INTO accounts ("FIELD_USERNAME", "FIELD_PASSWORD", "FIELD_SALT", "FIELD_REG_DATE") VALUES ('%e', '%e', '%e', CURRENT_TIMESTAMP())", playerName, _playerPassword, salt);
    return 1;
}