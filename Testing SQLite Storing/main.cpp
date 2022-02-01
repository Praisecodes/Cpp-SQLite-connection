#include <iostream>
#include <sqlite3.h>
#include <string>

int StoreData(const char*, std::string, std::string);
int CreateTable(const char*);
int UpdatePassword(const char*, std::string, std::string);
int SelectData(const char*, std::string, std::string);
int SelectCallback(void*, int, char**, char**);
int PasswordCallback(void*, int, char**, char**);

std::string pword;

int main() {
	const char* Database = "Information.db";

	int choice = 0;
	std::cout << "Hello there, please what do you want to do?\n1. Open Account\n2. Change Password\n3. Retrieve your information\n";
	std::cout << "Enter your choice here[1/2/3]: ";

	std::cin >> choice;

	switch (choice) {
	case 1: {
		sqlite3* db;
		int exit = 0;
		exit = sqlite3_open(Database, &db);
		sqlite3_close(db);

		CreateTable(Database);

		std::string Name;
		std::string Password;
		std::cout << "Please enter your name here: ";
		std::cin.ignore();
		std::getline(std::cin, Name);
		std::cout << "Please enter your account password here: ";
		std::getline(std::cin, Password);
		StoreData(Database, Name, Password);
	}
		break;

	case 2: {
		std::string Name, newPassword;
		std::cout << "Please enter your name below\n";
		std::cout << "[Tip]: Enter name you used to open an account here: ";
		std::cin.ignore();
		std::getline(std::cin, Name);
		std::cout << "Enter new password: ";
		std::getline(std::cin, newPassword);
		UpdatePassword(Database, newPassword, Name);
	}
		break;

	case 3: {
		std::string Name, Password;
		std::cout << "Please enter your name below\n[TIP]: Enter name you used to open and account here: ";
		std::cin.ignore();
		std::getline(std::cin, Name);
		std::cout << "Please enter your account password here: ";
		std::getline(std::cin, Password);

		SelectData(Database, Name, Password);
	}
		break;

	default:
		std::cout << choice << " is not an option!!\n";
		system("cls");
		main();
		break;
	}

	return 0;
}

int CreateTable(const char* dir) {
	sqlite3* db;

	int exit = sqlite3_open(dir, &db);
	std::string table = "CREATE TABLE IF NOT EXISTS Information("
		"ID		INTEGER PRIMARY KEY AUTOINCREMENT, "
		"NAME	TEXT NOT NULL, "
		"PASSWORD	TEXT);";

	char* messageError;
	exit = sqlite3_exec(db, table.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		std::cout << "Error Creating Information Table!!\n";
		sqlite3_free(messageError);
	}
	else {
		std::cout << "Successfully Created Table!!\n";
	}
	sqlite3_close(db);

	return 0;
}

int UpdatePassword(const char* dir, std::string newPassword, std::string Name)
{
	sqlite3* db;

	int exit = sqlite3_open(dir, &db);

	std::string data = "UPDATE Information SET PASSWORD='" + newPassword + "' WHERE NAME = '" + Name + "';";

	char* messageError;
	exit = sqlite3_exec(db, data.c_str(), NULL, 0, &messageError);

	if (exit != SQLITE_OK) {
		std::cout << "Error Updating Your Password Comrade!!\n";
		sqlite3_free(messageError);
	}
	else {
		std::cout << "Successfully Updated Your Password to " << newPassword << "!!\n";
	}
	sqlite3_close(db);
	return 0;
}

int StoreData(const char* dir, std::string Name, std::string Password) {
	sqlite3* db;

	std::string data = "INSERT INTO Information(NAME,PASSWORD) VALUES('" + Name + "', '" + Password + "');";

	int exit = sqlite3_open(dir, &db);

	char* messageError;
	exit = sqlite3_exec(db, data.c_str(), NULL, 0, &messageError);

	if (exit != SQLITE_OK) {
		std::cout << "Error Storing Your Data To Our DataBase!!\n";
		sqlite3_free(messageError);
	}
	else {
		std::cout << "Program Success!!\nThe Information About The Name " << Name << " Has Been Stored Successfully\n";
	}
	sqlite3_close(db);

	return 0;
}

int SelectData(const char* dir, std::string Name, std::string Password) {
	sqlite3* db;

	std::string Mpword = "SELECT PASSWORD FROM Information WHERE NAME = '" + Name + "';";
	std::string Select = "SELECT * FROM Information WHERE NAME = '" + Name + "';";

	int exit = sqlite3_open(dir, &db);

	sqlite3_exec(db, Mpword.c_str(), PasswordCallback, NULL, NULL);
	if (strcmp(Password.c_str(), pword.c_str()) != 0) {
		std::cout << "Password Mismatch Comrade\nWill You Getat Please!!!\n";
	}
	else {
		sqlite3_exec(db, Select.c_str(), SelectCallback, NULL, NULL);
	}

	return 0;
}

int SelectCallback(void* NotUsed, int argv, char** argc, char** azColName) {
	for (int i = 0; i < argv; i++) {
		std::cout << azColName[i] << ": " << argc[i] << std::endl;
	}

	std::cout << std::endl;

	return 0;
}

int PasswordCallback(void* NotUsed, int argv, char** argc, char** azColName) {
	for (int i = 0; i < argv; i++) {
		pword = argc[i];
	}
	
	return 0;
}
