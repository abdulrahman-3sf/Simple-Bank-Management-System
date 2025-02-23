#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;


const string clientsFileName = "clients.txt";
const string usersFileName = "users.txt";
void exitBack(string msg = "Bank Main Secreen..") {
	cout << "\nEnter any key to go to the " + msg;
	system("pause");
}
void noAccess() {
	cout << "---------------------------------------" << endl;
	cout << "Access Denied,\nYou don't have Premission to do this,\nPlease contact with the admin." << endl;
	cout << "---------------------------------------" << endl << endl;

	exitBack();
}
void login();

struct stClient {
	string accountNumber;
	short pinCode;
	string name;
	string phoneNumber;
	float balance;
	bool deleteMark = false;
};
struct stUser {
	string username;
	short password;
	short permission;
	bool deleteMark = false;
};

vector<string> splitString(string statement, string deli = " ") {
	vector<string> splitedString;
	string currentWord = "";
	int pos = 0;

	while ((pos = statement.find(deli)) != string::npos) {
		currentWord = statement.substr(0, pos);
		if (currentWord != "")
			splitedString.push_back(currentWord);
		statement.erase(0, pos + deli.length());
	}
	if (statement != "")
		splitedString.push_back(statement);

	return splitedString;
}

// --------------------------- Screens Functions ---------------------------

// Main Functions
string readAccountNumber() {
	string accountNumber;

	cout << "Client Account Number: ";
	getline(cin >> ws, accountNumber);

	return accountNumber;
}
void readClientWithoutAccountNumber(stClient& client) {
	cout << "Client Pin Code: ";
	cin >> client.pinCode;

	cout << "Client Name: ";
	cin.ignore();
	getline(cin, client.name);

	cout << "Client phoneNumber: ";
	getline(cin, client.phoneNumber);

	cout << "Client Balance: ";
	cin >> client.balance;
}
void printClient(stClient client) {
	cout << "\nClient Details:" << endl;
	cout << "---------------------------------" << endl;
	cout << setw(18) << left << "Account Number: " << client.accountNumber << endl;
	cout << setw(18) << left << "Pin Cide: " << client.pinCode << endl;
	cout << setw(18) << left << "Name: " << client.name << endl;
	cout << setw(18) << left << "Phone Number: " << client.phoneNumber << endl;
	cout << setw(18) << left << "Account Balance: " << client.balance << endl;
	cout << "---------------------------------" << endl << endl;
}

string convertClientFromRecordToLine(stClient client, string deli = "#//#") {
	string clientRecordInLine = client.accountNumber + deli
		+ to_string(client.pinCode) + deli
		+ client.name + deli
		+ client.phoneNumber + deli
		+ to_string(client.balance);
	return clientRecordInLine;
}
stClient convertClientFromLineToRecord(string clientRecordInLine, string deli = "#//#") {
	stClient client;
	vector<string> clientDataInVector = splitString(clientRecordInLine, deli);

	client.accountNumber = clientDataInVector[0];
	client.pinCode = stoi(clientDataInVector[1]);
	client.name = clientDataInVector[2];
	client.phoneNumber = clientDataInVector[3];
	client.balance = stof(clientDataInVector[4]);

	return client;
}

void loadClientsRecordsFromFileToVector(vector<stClient>& clients) {
	clients.clear();

	fstream file;
	file.open(clientsFileName, ios::in);

	if (file.is_open()) {
		string line;

		while (getline(file, line)) {
			clients.push_back(convertClientFromLineToRecord(line));
		}

		file.close();
	}
}
void storeClientsRecordsFromVectorToFile(vector<stClient>& clients) {
	fstream file;
	file.open(clientsFileName, ios::out);

	if (file.is_open()) {
		for (stClient& client : clients) {
			if (client.deleteMark == false)
				file << convertClientFromRecordToLine(client) << endl;
		}

		file.close();
	}
}
void addClientRecordsFromVectorToFile(stClient client, vector<stClient>& clients) {
	fstream file;
	file.open(clientsFileName, ios::app);

	if (file.is_open()) {
		file << convertClientFromRecordToLine(client) << endl;

		file.close();
	}
}

bool findClientByAccountNumber(string accountNumber, vector<stClient>& clients, stClient& client) {
	for (stClient& clientInClients : clients) {
		if (accountNumber == clientInClients.accountNumber) {
			client = clientInClients;
			return true;
		}
	}
	return false;
}
void showClintList(vector<stClient>& clients, stUser currentUser) {
	if ((currentUser.permission & 1) == 0) {
		noAccess();
		return;
	}
	
	cout << "\t\t\t\tClient List (" << clients.size() << ") Client(s)." << endl;
	cout << "_____________________________________________________________________________________" << endl << endl;
	cout << "| " << setw(15) << left << "Account Number";
	cout << "| " << setw(10) << left << "Pic Code";
	cout << "| " << setw(30) << left << "Client Name";
	cout << "| " << setw(12) << left << "Phone";
	cout << "| " << setw(12) << left << "Balance" << endl;
	cout << "_____________________________________________________________________________________" << endl << endl;

	loadClientsRecordsFromFileToVector(clients);

	for (stClient& client : clients) {
		cout << "| " << setw(15) << left << client.accountNumber;
		cout << "| " << setw(10) << left << client.pinCode;
		cout << "| " << setw(30) << left << client.name;
		cout << "| " << setw(12) << left << client.phoneNumber;
		cout << "| " << setw(12) << left << client.balance << endl;
	}
	cout << "_____________________________________________________________________________________" << endl;

	exitBack();
}
void addClient(vector<stClient>& clients, stUser currentUser) {
	if ((currentUser.permission & 2) == 0) {
		noAccess();
		return;
	}

	cout << "---------------------------------------" << endl;
	cout << "\t\tAdd Clients" << endl;
	cout << "---------------------------------------" << endl << endl;

	stClient client;
	string accountNumber;
	char moreClients = 'n';

	do {
		accountNumber = readAccountNumber();

		while (findClientByAccountNumber(accountNumber, clients, client)) {
			cout << "Client with [" << accountNumber << "] already exists, Enter another ";
			accountNumber = readAccountNumber();
		}

		client.accountNumber = accountNumber;
		readClientWithoutAccountNumber(client);

		clients.push_back(client);
		addClientRecordsFromVectorToFile(client, clients);

		cout << "\nClient Added Successfully, do you want to add more clients? (y/n): ";
		cin >> moreClients;
	} while (tolower(moreClients) == 'y');

	exitBack();
}
void markClientToDeleteIt(string accountNumber, vector<stClient>& clients) {
	for (stClient& client : clients) {
		if (accountNumber == client.accountNumber) {
			client.deleteMark = true;
			break;
		}
	}
}
void deleteClient(vector<stClient>& clients, stUser currentUser) {
	if ((currentUser.permission & 4) == 0) {
		noAccess();
		return;
	}

	cout << "---------------------------------------" << endl;
	cout << "\t\tDelete Client" << endl;
	cout << "---------------------------------------" << endl << endl;

	stClient client;
	string accountNumber = readAccountNumber();

	while (!findClientByAccountNumber(accountNumber, clients, client)) {
		cout << "Client with [" << accountNumber << "] dosen't exists, Enter another ";
		accountNumber = readAccountNumber();
	}

	printClient(client);

	char answerOfDelete = 'n';
	cout << "Are you sure that you want to delete this client? (y/n): ";
	cin >> answerOfDelete;

	if (tolower(answerOfDelete) == 'y') {
		markClientToDeleteIt(accountNumber, clients);
		storeClientsRecordsFromVectorToFile(clients);

		cout << "Client Deleted Successfully!" << endl;
	}
	
	loadClientsRecordsFromFileToVector(clients);

	exitBack();
}
void updateClient(vector<stClient>& clients, stUser currentUser) {
	if ((currentUser.permission & 8) == 0) {
		noAccess();
		return;
	}

	cout << "---------------------------------------" << endl;
	cout << "\t\tUpdate Client" << endl;
	cout << "---------------------------------------" << endl << endl;

	stClient client;
	string accountNumber = readAccountNumber();

	while (!findClientByAccountNumber(accountNumber, clients, client)) {
		cout << "Client with [" << accountNumber << "] dosen't exists, Enter another ";
		accountNumber = readAccountNumber();
	}

	printClient(client);

	char answerOfUpdate = 'n';
	cout << "Are you sure that you want to update this client? (y/n): ";
	cin >> answerOfUpdate;

	if (tolower(answerOfUpdate) == 'y') {
		for (stClient& clientInClients : clients) {
			if (accountNumber == clientInClients.accountNumber) {
				readClientWithoutAccountNumber(clientInClients);
				break;
			}
		}
		storeClientsRecordsFromVectorToFile(clients);

		cout << "Client Updated Successfully!" << endl;
	}

	loadClientsRecordsFromFileToVector(clients);

	exitBack();
}
void findClient(vector<stClient>& clients, stUser currentUser) {
	if ((currentUser.permission & 16) == 0) {
		noAccess();
		return;
	}

	cout << "---------------------------------------" << endl;
	cout << "\t\tFind Client" << endl;
	cout << "---------------------------------------" << endl << endl;

	stClient client;
	string accountNumber = readAccountNumber();
	
	while (!findClientByAccountNumber(accountNumber, clients, client)) {
		cout << "Client with [" << accountNumber << "] dosen't exists, Enter another ";
		accountNumber = readAccountNumber();
	}

	printClient(client);

	exitBack();
}
void exitProgram() {
	cout << "---------------------------------------" << endl;
	cout << "\t\tEnd Program" << endl;
	cout << "---------------------------------------" << endl;
}

// Transactions Functions
bool depositMoneyToClientByAccountNumber(string accountNumber, vector<stClient>& clients, float amount) {
	char answer = 'n';
	cout << "Are you sure that you want to perform this transaction? (y/n): ";
	cin >> answer;

	if (tolower(answer) == 'y') {
		for (stClient& clientInClients : clients) {
			if (accountNumber == clientInClients.accountNumber) {
				clientInClients.balance += amount;
				storeClientsRecordsFromVectorToFile(clients);
				cout << "Transaction Done Seccessfully!, The New Balance is: " << clientInClients.balance << endl;
				return true;
			}
		}
	}
	return false;
}
void depositMoneyToClient(vector<stClient>& clients) {
	cout << "---------------------------------------" << endl;
	cout << "\t\tDeposit Money" << endl;
	cout << "---------------------------------------" << endl << endl;

	stClient client;
	string accountNumber = readAccountNumber();

	while (!findClientByAccountNumber(accountNumber, clients, client)) {
		cout << "Client with [" << accountNumber << "] dosen't exists, Enter another ";
		accountNumber = readAccountNumber();
	}

	printClient(client);

	float depositAmount;
	cout << "Enter deposit amount: ";
	cin >> depositAmount;

	depositMoneyToClientByAccountNumber(accountNumber, clients, depositAmount);

	loadClientsRecordsFromFileToVector(clients);

	exitBack("Transactions Secreen..");
}
void withdrawMoneyFromClient(vector<stClient>& clients) {
	cout << "---------------------------------------" << endl;
	cout << "\t\tWithdraw Money" << endl;
	cout << "---------------------------------------" << endl << endl;

	stClient client;
	string accountNumber = readAccountNumber();

	while (!findClientByAccountNumber(accountNumber, clients, client)) {
		cout << "Client with [" << accountNumber << "] dosen't exists, Enter another ";
		accountNumber = readAccountNumber();
	}

	printClient(client);

	float withdrawAmount;
	cout << "Enter withdraw amount: ";
	cin >> withdrawAmount;

	while (withdrawAmount > client.balance) {
		cout << "Amount Exceeds the Balance, you can withdraw up to " << client.balance << " only!" << endl;
		cout << "Enter withdraw amount: ";
		cin >> withdrawAmount;
	}

	depositMoneyToClientByAccountNumber(accountNumber, clients, withdrawAmount * -1);

	loadClientsRecordsFromFileToVector(clients);

	exitBack("Transactions Secreen..");
}
void showTotalBalancesClient(vector<stClient>& clients) {
	cout << "\t\t\t\tClient List (" << clients.size() << ") Client(s)." << endl;
	cout << "_____________________________________________________________________________________" << endl << endl;
	cout << "| " << setw(15) << left << "Account Number";
	cout << "| " << setw(30) << left << "Client Name";
	cout << "| " << setw(12) << left << "Balance" << endl;
	cout << "_____________________________________________________________________________________" << endl << endl;

	loadClientsRecordsFromFileToVector(clients);

	float totalBalance = 0;
	for (stClient& client : clients) {
		totalBalance += client.balance;
		cout << "| " << setw(15) << left << client.accountNumber;
		cout << "| " << setw(30) << left << client.name;
		cout << "| " << setw(12) << left << client.balance << endl;
	}
	cout << "_____________________________________________________________________________________" << endl;

	cout << "\n\t\t\t\t" << "  Total Balances = " << totalBalance << endl;

	exitBack("Transactions Secreen..");
}

// Mange Users Functions
stUser readUser() {
	stUser user;

	cout << "Enter Username: ";
	cin >> user.username;
	cout << "Enter Password: ";
	cin >> user.password;

	return user;
}
short permissionsValue() {
	char giveAccess = 'n';
	short permissions = 0;

	cout << "\nDo you want to give full access? (y/n): ";
	cin >> giveAccess;

	if (tolower(giveAccess) == 'y') {
		return -1;
	}
	else {
		cout << "\nDo you want to give access to: ";

		cout << "\nShow Users List? (y/n): ";
		cin >> giveAccess;
		if (giveAccess == 'y')
			permissions = (permissions | 1);
		giveAccess = 'n';

		cout << "Add New User? (y/n): ";
		cin >> giveAccess;
		if (giveAccess == 'y')
			permissions = (permissions | 2);
		giveAccess = 'n';

		cout << "Delete User? (y/n): ";
		cin >> giveAccess;
		if (giveAccess == 'y')
			permissions = (permissions | 4);
		giveAccess = 'n';

		cout << "Update User? (y/n): ";
		cin >> giveAccess;
		if (giveAccess == 'y')
			permissions = (permissions | 8);
		giveAccess = 'n';

		cout << "Find User? (y/n): ";
		cin >> giveAccess;
		if (giveAccess == 'y')
			permissions = (permissions | 16);
		giveAccess = 'n';

		cout << "Transactions? (y/n): ";
		cin >> giveAccess;
		if (giveAccess == 'y')
			permissions = (permissions | 32);
		giveAccess = 'n';

		cout << "Manage Users? (y/n): ";
		cin >> giveAccess;
		if (giveAccess == 'y')
			permissions = (permissions | 64);

		return permissions;
	}
}
void printUser(stUser user) {
	cout << "\nUser Details:" << endl;
	cout << "---------------------------------" << endl;
	cout << setw(18) << left << "Username: " << user.username << endl;
	cout << setw(18) << left << "Password: " << user.password << endl;
	cout << setw(18) << left << "Permission: " << user.permission << endl;
	cout << "---------------------------------" << endl << endl;
}

string convertUserFromRecordToLine(stUser user, string deli = "#//#") {
	return user.username + deli + to_string(user.password) + deli + to_string(user.permission);
}
stUser convertUserFromLineToRecord(string userRecordInLine, string deli = "#//#") {
	stUser user;
	vector<string> userDateInVector = splitString(userRecordInLine, deli);

	user.username = userDateInVector[0];
	user.password = stoi(userDateInVector[1]);
	user.permission = stoi(userDateInVector[2]);

	return user;
}

void loadUsersRecordsFromFileToVector(vector<stUser>& users) {
	users.clear();

	fstream file;
	file.open(usersFileName, ios::in);

	if (file.is_open()) {
		string line;

		while (getline(file, line)) {
			users.push_back(convertUserFromLineToRecord(line));
		}

		file.close();
	}
}
void storeUsersRecordsFromVctorToFile(vector<stUser>& users) {
	fstream file;
	file.open(usersFileName, ios::out);

	if (file.is_open()) {
		for (stUser& user : users) {
			if (user.deleteMark == false)
				file << convertUserFromRecordToLine(user) << endl;
		}

		file.close();
	}
}
void addUserRecordFromVectorToFile(stUser user, vector<stUser>& users) {
	fstream file;
	file.open(usersFileName, ios::app);

	if (file.is_open()) {
		file << convertUserFromRecordToLine(user) << endl;

		file.close();
	}
}

bool findUserByUsernameAndPassword(stUser user, vector<stUser>& users) {
	for (stUser& userInUsers : users) {
		if (user.username == userInUsers.username && user.password == userInUsers.password)
			return true;
	}
	return false;
}
bool findUserByUsername(string username, vector<stUser>& users, stUser& user) {
	for (stUser& userInUsers : users) {
		if (username == userInUsers.username) {
			user = userInUsers;
			return true;
		}
	}
	return false;
}
void showUsersList(vector<stUser>& users) {
	cout << "\t\t\t\tUsers List (" << users.size() << ") User(s)." << endl;
	cout << "_____________________________________________________________________________________" << endl << endl;
	cout << "| " << setw(15) << left << "Username";
	cout << "| " << setw(10) << left << "Password";
	cout << "| " << setw(10) << left << "Permissions" << endl;
	cout << "_____________________________________________________________________________________" << endl << endl;

	loadUsersRecordsFromFileToVector(users);

	for (stUser& user : users) {
		cout << "| " << setw(15) << left << user.username;
		cout << "| " << setw(10) << left << user.password;
		cout << "| " << setw(10) << left << user.permission << endl;
	}
	cout << "_____________________________________________________________________________________" << endl;

	exitBack("Mange Users Screen..");
}
void addUser(vector<stUser>& users) {
	cout << "---------------------------------------" << endl;
	cout << "\t\tAdd Users" << endl;
	cout << "---------------------------------------" << endl << endl;

	stUser user;
	char moreUsers = 'n';

	do {
		cout << "Enter Username: ";
		cin >> user.username;

		while (findUserByUsername(user.username, users, user)) {
			cout << "User with [" << user.username << "] already exists, Enter another Username: ";
			cin >> user.username;
		}

		cout << "Enter Password: ";
		cin >> user.password;
		
		user.permission = permissionsValue();
		
		addUserRecordFromVectorToFile(user, users);

		cout << "\nUser Added Successfully, do you want to add more Users? (y/n): ";
		cin >> moreUsers;
	} while (tolower(moreUsers) == 'y');

	exitBack("Mange Users Screen..");
}
void markUserToDeleteIt(stUser user, vector<stUser>& users) {
	for (stUser& userInUsers : users) {
		if (user.username == userInUsers.username) {
			userInUsers.deleteMark = true;
			break;
		}
	}
}
void deleteUser(vector<stUser>& users) {
	cout << "---------------------------------------" << endl;
	cout << "\t\tDelete User" << endl;
	cout << "---------------------------------------" << endl << endl;

	stUser user;
	cout << "Enter Username: ";
	cin >> user.username;

	if (user.username == "admin" || user.username == "Admin") {
		cout << "You can not delete this user!";
		exitBack("Mange Users Screen..");
		return;
	}

	while (!findUserByUsername(user.username, users, user)) {
		cout << "Client with [" << user.username << "] dosen't exists, Enter another Username: ";
		cin >> user.username;
	}

	printUser(user);

	char answerOfDelete = 'n';
	cout << "Are you sure that you want to delete this user? (y/n): ";
	cin >> answerOfDelete;

	if (tolower(answerOfDelete) == 'y') {
		markUserToDeleteIt(user, users);
		storeUsersRecordsFromVctorToFile(users);

		cout << "User Deleted Successfully!" << endl;
	}

	loadUsersRecordsFromFileToVector(users);

	exitBack("Mange Users Screen..");
}
void updateUser(vector<stUser>& users) {
	cout << "---------------------------------------" << endl;
	cout << "\t\tUpdate User" << endl;
	cout << "---------------------------------------" << endl << endl;

	stUser user;
	cout << "Enter Username: ";
	cin >> user.username;

	while (!findUserByUsername(user.username, users, user)) {
		cout << "Client with [" << user.username << "] dosen't exists, Enter another Username: ";
		cin >> user.username;
	}

	printUser(user);

	char answerOfUpdate = 'n';
	cout << "Are you sure that you want to update this user? (y/n): ";
	cin >> answerOfUpdate;

	if (tolower(answerOfUpdate) == 'y') {
		for (stUser& userInUsers : users) {
			if (user.username == userInUsers.username) {
				cout << "Enter Password: ";
				cin >> userInUsers.password;

				userInUsers.permission = permissionsValue();
				break;
			}
		}

		storeUsersRecordsFromVctorToFile(users);

		cout << "User Updated Successfully!" << endl;
	}

	loadUsersRecordsFromFileToVector(users);

	exitBack("Mange Users Screen..");
}
void findUser(vector<stUser>& users) {
	cout << "---------------------------------------" << endl;
	cout << "\t\tFind User" << endl;
	cout << "---------------------------------------" << endl << endl;

	stUser user;
	cout << "Enter Username: ";
	cin >> user.username;

	while (!findUserByUsername(user.username, users, user)) {
		cout << "Client with [" << user.username << "] dosen't exists, Enter another Username: ";
		cin >> user.username;
	}

	printUser(user);

	exitBack("Mange Users Screen..");
}

// --------------------------- Screens ---------------------------

// Transactions Screen
enum enTransactionsOption { deposit = 1, withdraw, totalBalances, transToMainScreen};
void performTransactionsOperations(short choice, vector<stClient>& clients) {
	switch (choice) {
	case enTransactionsOption::deposit:
		system("cls");
		depositMoneyToClient(clients);
		break;
	case enTransactionsOption::withdraw:
		system("cls");
		withdrawMoneyFromClient(clients);
		break;
	case enTransactionsOption::totalBalances:
		system("cls");
		showTotalBalancesClient(clients);
	}
}
void TransactionsScreen(vector<stClient>& clients, stUser currentUser) {
	if ((currentUser.permission & 32) == 0) {
		noAccess();
		return;
	}

	short choice;

	do {
		system("cls");
		cout << "==========================================" << endl;
		cout << "\t\tTransactions Screen" << endl;
		cout << "==========================================" << endl;
		cout << "[1] Deposit." << endl;
		cout << "[2] Withdraw." << endl;
		cout << "[3] Total Balances." << endl;
		cout << "[4] Main Screen." << endl;
		cout << "==========================================" << endl << endl;


		cout << "Choose what do you want to do? [1-4]: ";
		cin >> choice;

		performTransactionsOperations((enTransactionsOption)choice, clients);

	} while (choice != enTransactionsOption::transToMainScreen);
}

// Mange Users Screen
enum enMangeUsersOption { showUsersListOption = 1, addUserOption, deleteUserOption, updateUserOption, findUserOption, mangeToMainScreen};
void performManageUsersOperations(short choice, vector<stUser>& users) {
	switch (choice) {
	case enMangeUsersOption::showUsersListOption:
		system("cls");
		showUsersList(users);
		break;
	case enMangeUsersOption::addUserOption:
		system("cls");
		addUser(users);
		break;
	case enMangeUsersOption::deleteUserOption:
		system("cls");
		deleteUser(users);
		break;
	case enMangeUsersOption::updateUserOption:
		system("cls");
		updateUser(users);
		break;
	case enMangeUsersOption::findUserOption:
		system("cls");
		findUser(users);
		break;
	}
}
void ManageUsersScreen(stUser currentUser) {
	if ((currentUser.permission & 64) == 0) {
		noAccess();
		return;
	}

	vector<stUser> users;
	short choice;

	do {
		loadUsersRecordsFromFileToVector(users);

		system("cls");
		cout << "==========================================" << endl;
		cout << "\t\tManage Users Screen" << endl;
		cout << "==========================================" << endl;
		cout << "[1] List Users." << endl;
		cout << "[2] Add New User." << endl;
		cout << "[3] Delete User." << endl;
		cout << "[4] Update User." << endl;
		cout << "[5] Find User." << endl;
		cout << "[6] Main Screen." << endl;
		cout << "==========================================" << endl << endl;


		cout << "Choose what do you want to do? [1-6]: ";
		cin >> choice;

		performManageUsersOperations((enMangeUsersOption)choice, users);

	} while (choice != enMangeUsersOption::mangeToMainScreen);
}


// Main Screen
enum enChoiceOption { showClientListOption = 1, addClientOption, deleteClientOption, updateClientOption, findClientOption, transactionsOption, mangeUsersOption, logoutOption };
void performOperations(short choice, vector<stClient>& clients, stUser currentUser) {
	switch (choice) {
	case enChoiceOption::showClientListOption:
		system("cls");
		showClintList(clients, currentUser);
		break;
	case enChoiceOption::addClientOption:
		system("cls");
		addClient(clients, currentUser);
		break;
	case enChoiceOption::deleteClientOption:
		system("cls");
		deleteClient(clients, currentUser);
		break;
	case enChoiceOption::updateClientOption:
		system("cls");
		updateClient(clients, currentUser);
		break;
	case enChoiceOption::findClientOption:
		system("cls");
		findClient(clients, currentUser);
		break;
	case enChoiceOption::transactionsOption:
		system("cls");
		TransactionsScreen(clients, currentUser);
		break;
	case enChoiceOption::mangeUsersOption:
		system("cls");
		ManageUsersScreen(currentUser);
		break;
	case enChoiceOption::logoutOption:
		system("cls");
		login();
		break;
	default:
		exitProgram();
		break;
	}
}
void BankMainScreen(stUser currrentUser) {
	vector<stClient> clients;
	short choice;

	do {
		loadClientsRecordsFromFileToVector(clients);

		system("cls");
		cout << "==========================================" << endl;
		cout << "\t\tBank Main Screen" << endl;
		cout << "==========================================" << endl;
		cout << "[1] Show Client List." << endl;
		cout << "[2] Add New Client." << endl;
		cout << "[3] Delete Client." << endl;
		cout << "[4] Update Client." << endl;
		cout << "[5] Find Client." << endl;
		cout << "[6] Transactions." << endl;
		cout << "[7] Manage Users." << endl;
		cout << "[8] Logout." << endl;
		cout << "==========================================" << endl << endl;


		cout << "Choose what do you want to do? [1-8]: ";
		cin >> choice;

		performOperations((enChoiceOption)choice, clients, currrentUser);

	} while (choice != enChoiceOption::logoutOption);
}

// LoginScreen
void loginLogo() {
	system("cls");

	cout << "---------------------------------------" << endl;
	cout << "\t\tLogIn Screen" << endl;
	cout << "---------------------------------------" << endl << endl;
}
void login() {
	vector<stUser> users;
	loadUsersRecordsFromFileToVector(users);

	stUser user;

	loginLogo();

	cout << "Enter Username: ";
	cin >> user.username;
	cout << "Enter Password: ";
	cin >> user.password;

	while (!findUserByUsernameAndPassword(user, users)) {
		loginLogo();
		cout << "Invalid username/password!" << endl;

		cout << "Enter Username: ";
		getline(cin >> ws, user.username);
		cout << "Enter Password: ";
		cin >> user.password;
	}

	findUserByUsername(user.username, users, user);

	BankMainScreen(user);
}

int main() {
	login();

	return 0;
}