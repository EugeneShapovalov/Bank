#include "stdio.h"
#include "sqlite3.h"
#include "adminFunctions.h"

extern sqlite3 *dataBase;
extern char* dataBaseName;

void addAccountToClient(char* login, char* password, int pin, int accountType, int overdraft) {
	int primaryKey = -1; 
	int client_id = -1;
	int rc1;
	sqlite3 *database;
	char* insertCommand = (char*)malloc(sizeof(char)*200);
	char *zErrMsg = 0;
	sqlite3_stmt *statement;
	if (sqlite3_open(dataBaseName, &database) == SQLITE_OK) {
       const char *sql = "SELECT MAX(accountid) FROM bank_accounts";
       if (sqlite3_prepare_v2(database, sql, -1, &statement, NULL) == SQLITE_OK) {
           while (sqlite3_step(statement) == SQLITE_ROW) {
               client_id = sqlite3_column_int(statement, 0);
           }
	   }
    }
	if (sqlite3_open(dataBaseName, &database) == SQLITE_OK) {
		char *sql = (char*)malloc(sizeof(char)*200);
		sprintf(sql, "SELECT user_id FROM user where login = '%s' AND password = '%s'", login, password);
       if (sqlite3_prepare_v2(database, sql, -1, &statement, NULL) == SQLITE_OK) {
           while (sqlite3_step(statement) == SQLITE_ROW) {
               primaryKey = sqlite3_column_int(statement, 0);
           }
	   }
    }
	if (primaryKey == -1) {
		printf("Sqlite error!");
		return;
	}
	sprintf(insertCommand, "insert into bank_accounts values(%d, 0, %d, %d, %d, %d);",++client_id , pin, primaryKey, accountType, overdraft);   
	rc1 = sqlite3_exec(database, insertCommand, 0, 0, &zErrMsg);
}


void deleteAccountToClient(int id) { 
	int rc1;
	sqlite3 *database;
	char* insertCommand = (char*)malloc(sizeof(char)*200);
	char *zErrMsg = 0;
	if (sqlite3_open(dataBaseName, &database) == SQLITE_OK) {
		sprintf(insertCommand, "delete from BANK_ACCOUNTS where accountid = %d",id); 
		rc1 = sqlite3_exec(database, insertCommand, 0, 0, &zErrMsg);
		if (rc1 != SQLITE_OK) {
			printf("Sqlite error!");
		}
	}
}


void addNewClient(char* login, char* password, int role) {
	int primaryKey; 
	int rc1;
	sqlite3 *database;
	char* insertCommand = (char*)malloc(sizeof(char)*200);
	char *zErrMsg = 0;
	if (sqlite3_open(dataBaseName, &database) == SQLITE_OK) {
       const char *sql = "SELECT MAX(user_id) FROM user";
       sqlite3_stmt *statement;
       if (sqlite3_prepare_v2(database, sql, -1, &statement, NULL) == SQLITE_OK) {
           while (sqlite3_step(statement) == SQLITE_ROW) {
               primaryKey = sqlite3_column_int(statement, 0);
           }
	   }
    }
	sprintf(insertCommand, "insert into user values(%d, '%s', '%s', %d);",++primaryKey , login, password, role);   
	rc1 = sqlite3_exec(database, insertCommand, 0, 0, &zErrMsg);
}

void deleteClient(int id) {
	int rc1;
	sqlite3 *database;
	char* insertCommand = (char*)malloc(sizeof(char)*200);
	char *zErrMsg = 0;
	if (sqlite3_open(dataBaseName, &database) == SQLITE_OK) {
		sprintf(insertCommand, "delete from user where user_id = %d",id); 
		rc1 = sqlite3_exec(database, insertCommand, 0, 0, &zErrMsg);
		if (rc1 != SQLITE_OK) {
			printf("Sqlite error!");
		}
	}
}

