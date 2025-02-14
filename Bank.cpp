#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

const string fileName = "clients.txt";
void exitToBankMainSecreen() {
	cout << "\nEnter any key to go to the Bank Main Secreen..";
	system("pause");
}

struct stClient {
	string accountNumber;
	short pinCode;
	string name;
	string phoneNumber;
	float balance;
	bool deleteMark = false;
};
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

string convertClientFromRecordToLine(stClient client, string deli="#//#") {
	string clientRecordInLine = client.accountNumber + deli
		+ to_string(client.pinCode) + deli
		+ client.name + deli
		+ client.phoneNumber + deli
		+ to_string(client.balance);
	return clientRecordInLine;
}
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

void loadClientRecordsFromFileToVector(vector<stClient>& clients) {
	clients.clear();

	fstream file;
	file.open(fileName, ios::in);

	if (file.is_open()) {
		string line;

		while (getline(file, line)) {
			clients.push_back(convertClientFromLineToRecord(line));
		}

		file.close();
	}
}
void storeClientRecordsFromVectorToFile(vector<stClient>& clients) {
	fstream file;
	file.open(fileName, ios::out);

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
	file.open(fileName, ios::app);

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

void showClintList(vector<stClient>& clients) {
	cout << "\t\t\t\tClient List (" << clients.size() << ") Client(s)." << endl;
	cout << "_____________________________________________________________________________________" << endl << endl;
	cout << "| " << setw(15) << left << "Account Number";
	cout << "| " << setw(10) << left << "Pic Code";
	cout << "| " << setw(30) << left << "Client Name";
	cout << "| " << setw(12) << left << "Phone";
	cout << "| " << setw(12) << left << "Balance" << endl;
	cout << "_____________________________________________________________________________________" << endl << endl;

	loadClientRecordsFromFileToVector(clients);

	for (stClient& client : clients) {
		cout << "| " << setw(15) << left << client.accountNumber;
		cout << "| " << setw(10) << left << client.pinCode;
		cout << "| " << setw(30) << left << client.name;
		cout << "| " << setw(12) << left << client.phoneNumber;
		cout << "| " << setw(12) << left << client.balance << endl;
	}
	cout << "_____________________________________________________________________________________" << endl;

	exitToBankMainSecreen();
}
void addClient(vector<stClient>& clients) {
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

	exitToBankMainSecreen();
}
void markClientToDeleteIt(string accountNumber, vector<stClient>& clients) {
	for (stClient& client : clients) {
		if (accountNumber == client.accountNumber) {
			client.deleteMark = true;
			break;
		}
	}
}
void deleteClient(vector<stClient>& clients) {
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
		storeClientRecordsFromVectorToFile(clients);

		cout << "Client Deleted Successfully!" << endl;
	}
	
	loadClientRecordsFromFileToVector(clients);

	exitToBankMainSecreen();
}
void updateClient(vector<stClient>& clients) {
	cout << "---------------------------------------" << endl;
	cout << "\t\tUpdate Clients" << endl;
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
			}
		}
		storeClientRecordsFromVectorToFile(clients);

		cout << "Client Updated Successfully!" << endl;
	}

	loadClientRecordsFromFileToVector(clients);

	exitToBankMainSecreen();
}
void findClient(vector<stClient>& clients) {
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

	exitToBankMainSecreen();
}
void exitProgram() {
	cout << "---------------------------------------" << endl;
	cout << "\t\tEnd Program" << endl;
	cout << "---------------------------------------" << endl;
}

enum enChoiceOption {showClientListOption=1, addClientOption, deleteClientOption, updateClientOption, findClientOption, exitOption};
void performOperations(short choice, vector<stClient>& clients) {
	switch (choice) {
	case enChoiceOption::showClientListOption:
		system("cls");
		showClintList(clients);
		break;
	case enChoiceOption::addClientOption:
		system("cls");
		addClient(clients);
		break;
	case enChoiceOption::deleteClientOption:
		system("cls");
		deleteClient(clients);
		break;
	case enChoiceOption::updateClientOption:
		system("cls");
		updateClient(clients);
		break;
	case enChoiceOption::findClientOption:
		system("cls");
		findClient(clients);
		break;
	case enChoiceOption::exitOption:
		system("cls");
		exitProgram();
		break;
	default:
		exitProgram();
		break;
	}
}

void BankMainScreen() {
	vector<stClient> clients;
	short choice;
	
	do {
		system("cls");
		cout << "==========================================" << endl;
		cout << "\t\tBank Main Screen" << endl;
		cout << "==========================================" << endl;
		cout << "[1] Show Client List." << endl;
		cout << "[2] Add New Client." << endl;
		cout << "[3] Delete Client." << endl;
		cout << "[4] Update Client." << endl;
		cout << "[5] Find Client." << endl;
		cout << "[6] Exit." << endl;
		cout << "==========================================" << endl << endl;


		cout << "Choose what do you want to do? [1-6]: ";
		cin >> choice;

		performOperations((enChoiceOption)choice, clients);

	} while (choice != enChoiceOption::exitOption);
}

int main() {
	BankMainScreen();
	
	return 0;
}