/*
    Normal
        - SendErrorMsg(playerid, string[]);
        - SendSyntaxMsg(playerid, string[]);
        - SendServerMsg(playerid, string[]);
    Formattted
        - SendErrorMsgF(playerid, string[]);
        - SendSyntaxMsgF(playerid, string[]);
        - SendServerMsgF(playerid, string[]);
*/

#include <YSI_Coding\y_va>

static
    out[144];


stock SendErrorMsg(playerid, const string[]) return SendMsgF(playerid, COLOR_RED, "[ERROR]: "C_WHITE"%s", string);
stock SendSyntaxMsg(playerid, const string[]) return SendMsgF(playerid, COLOR_YELLOW, "[USAGE]: "C_WHITE"%s", string);
stock SendServerMsg(playerid, const string[]) return SendMsgF(playerid, COLOR_GREY, "[SERVER]: "C_WHITE"%s", string);

stock SendErrorMsgF(playerid, const fmat[], {Float,_}:...)
{
    formatex(out, sizeof(out), fmat, ___(2));
    return SendErrorMsg(playerid, out);
}

stock SendServerMsgF(playerid, const fmat[], {Float,_}:...)
{
    formatex(out, sizeof(out), fmat, ___(2));
    return SendServerMsg(playerid, out);
}



/*
    Normal Messaging
        - SendMsgF(playerid, colour, fmat, {Float,_}:...);
*/

stock SendMsgF(playerid, colour, const fmat[], {Float,_}:...)
{
    formatex(out, sizeof(out), fmat, ___(3));
    return SendClientMessage(playerid, colour, out);
}

stock SendMsgAllF(colour, const fmat[], {Float,_}:...)
{
    formatex(out, sizeof(out), fmat, ___(2));
    return SendClientMessageToAll(colour, out);
}