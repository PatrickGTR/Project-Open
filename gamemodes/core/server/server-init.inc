#include <a_mysql>
#include <YSI_Coding\y_hooks>

new 
    MySQL:sql_handle;

#define sql_server      "127.0.0.1"
#define sql_username    "root"
#define sql_password    ""
#define sql_database    "cnr"

hook OnScriptInit() {
	new MySQLOpt: sql_option_id = mysql_init_options();

	mysql_set_option(sql_option_id, AUTO_RECONNECT, true);

    sql_handle = mysql_connect(sql_server, sql_username, sql_password, sql_database, sql_option_id);

    mysql_log(ALL);

    if (sql_handle == MYSQL_INVALID_HANDLE || mysql_errno(sql_handle) != 0) {
		print("MySQL connection failed. Server is shutting down.");
		SendRconCommand("exit");
		return 1;
	}
    
    print("MySQL connection is successful.");
    return 1;
}

hook OnGameModeExit() {
    mysql_close(sql_handle);
    return 1;
}

MySQL:GetSQLHandle() return sql_handle;