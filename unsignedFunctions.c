#include "stdio.h"
#include "sqlite3.h"
#include <malloc.h>
#include <string.h>
#include "unsignedFunctions.h"



int unauthorizedRole() {
	menu(MAIN_MENU_ITEM_NUM, mainMenuItem, mainMenuFunctions);
	return 0;
}

int error() {
	printf("loggin error\n");
	return 0;
}

static int fillResult(void *NotUsed, int argc, char **argv, char **azColName) {
	resultPassword = (char*)malloc(sizeof(char)* 32);

	if (argv[0]) {
		sprintf(resultPassword, "%s", argv[0]);
	}
	else {
		resultPassword = NULL;
	}

	if (argv[1]) {
		resultRole = argv[1][0];
	}
	else {
		resultPassword = NULL;
	}
	return 0;
}

char authentication(char* login, char* password) {
	char getUserSelect[255];
	char* errorMessage = 0;
	sprintf(getUserSelect, "SELECT password, role FROM user WHERE login = '%s'", login);
	if (sqlite3_exec(dataBase, getUserSelect, fillResult, 0, &errorMessage) != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", errorMessage);
		sqlite3_free(errorMessage);
	}

	if (!strcmp(resultPassword, password)) {
		return resultRole;
	}
	return 'n';
}

void* authorization() {
	printf("Enter login: ");
	gets(login);
	printf("Enter password: ");
	gets(password);
	userRole = authentication(login, password);
	for (int i = 0; i < ROLES_NUMBER; ++i) {
		if (userRole == roles[i]) {
			return rolesFunctions[i];
		}
	}

	return *error;
}

void* showBankInfo() {
	printf("LLC \"Bank T\"\n");
	printf("There will be some information about bank.\n");
	return 0;
}

void* registerNewUser() {
	return 0;
}

void* logIn() {
	void(*userFunction) (void);
	userFunction = (void(*)()) authorization();
	userFunction();
	return 0;
}


void menu(int itemNum, char* menuItem[], void*(*menuFunctions[])()) {
	int command = -1;
	char buffer[100];
	void* (*function) (void);
	int(*userFunction) (void);
	while (command) {
		printf("Input command:\n");
		printf("  0. Exit\n");
		for (int i = 0; i < itemNum; ++i) {
			printf("  %d. %s\n", i + 1, menuItem[i]);
		}
		scanf("%d", &command);
		gets(buffer);
		if (command > 0 && command <= itemNum) {
			function = menuFunctions[command - 1];
			function();
		} else if (command != 0) {
			printf("Invalid command.\n");
		}
	}
}


