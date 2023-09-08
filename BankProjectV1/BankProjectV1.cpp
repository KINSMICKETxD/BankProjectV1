#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <cctype>
#include <fstream>
using namespace std;

enum userChoices {showClients =1,AddClient,DeleteClient,UpdateClient,FindClient,Exit};
const string fileName = "clients.txt";
struct stClient
{
    string accountNumber;
    string pinCode;
    string clientName;
    string phoneNumber;
    int balance;
    bool markForDelete = false;
};
userChoices readUserChoice() {
    short choice;
    cout << "Choose what do you  want to do? [1 to 6]?";
    cin >> choice;
    return (userChoices)choice;
}
void printMenuScreen() {
    system("cls");
    cout << "=============================================";
    cout << "\n              Main Menu Screen";
    cout << "\n=============================================";
    cout << "\n\t[1] Show Clients List.";
    cout << "\n\t[2] Add New Client.";
    cout << "\n\t[3] Delete Client.";
    cout << "\n\t[4] Update Client Info.";
    cout << "\n\t[5] Find Client.";
    cout << "\n\t[6] Exit.";
    cout << "\n=============================================" << endl;
}
void printCLientsListHeader(int clientsNumber) {
    cout << "\n                                   Client list (" << clientsNumber << ") client(s).";
    cout << "\n__________________________________________________________________________________________________________";
    cout << "\n\n|" << setw(20) << left << " Account Number";
    cout << "|" << setw(15) << left << " Pin Code";
    cout << "|" << setw(40) << left << " Client Name";
    cout << "|" << setw(15) << left << " Phone";
    cout << "|" << setw(15) << left << " Balance";
    cout << "\n__________________________________________________________________________________________________________";
}
void printAClient(stClient client) {
    cout << "\n\n|" << setw(20) << left << client.accountNumber;
    cout << "|" << setw(15) << left << client.pinCode;
    cout << "|" << setw(40) << left << client.clientName;
    cout << "|" << setw(15) << left << client.phoneNumber;
    cout << "|" << setw(15) << left << client.balance;
}
vector<string> splitString(string s1, string delim) {
    vector<string> vResult;
    short pos = 0;
    string word;
    while ((pos = s1.find(delim)) != std::string::npos) {
        word = s1.substr(0, pos);
        if (word != "") {
            vResult.push_back(word);
        }
        s1.erase(0, pos + delim.length());
    }
    if (s1 != "") {
        vResult.push_back(s1);
    }
    return vResult;
}
stClient LineDataToClientRecord(string line) {
    stClient client;
    vector<string> vLine = splitString(line, "#//#");
    client.accountNumber = vLine[0];
    client.pinCode = vLine[1];
    client.clientName = vLine[2];
    client.phoneNumber = vLine[3];
    client.balance = stoi(vLine[4]);
    return client;
}
void showClientsList(vector<stClient> vClients) {
    system("cls");
    printCLientsListHeader(vClients.size());
    for (stClient c : vClients) {
        printAClient(c);
    }
    cout << "\n\n___________________________________________________________________________________________________________";
    cout << "\nPress any key to return to main menu\n";
    system("pause>0");
}
vector<stClient> LoadClientsDataFromFile(string fileName) {
    stClient client;
    vector<stClient> vClients;
    string line;
    fstream file(fileName, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            client = LineDataToClientRecord(line);
            vClients.push_back(client);
        }
        file.close();
    }
    return vClients;
}
bool findClientByAccountNumber(string accountNumber,stClient& client,vector<stClient> vClients) {
    for (stClient c : vClients) {
        if (c.accountNumber == accountNumber) {
            client = c;
            return true;
        }
    }
    return false;
}
string convertAClientToDataLine(stClient client,string delim) {
    string s;
    s += client.accountNumber + delim + client.pinCode + delim + client.clientName + delim +
        client.phoneNumber + delim + to_string(client.balance);
    return s;
}
stClient readAClientRecord(vector<stClient> vClients) {
    stClient client;
    system("cls");
    cout << "________________________________";
    cout << "\n\n     Add New Client Screen";
    cout << "\n________________________________";
    cout << "\nAdding New Client:";
    cout << "\n\nEnter Account Number ? ";
    getline(cin >> ws, client.accountNumber);
    while (findClientByAccountNumber(client.accountNumber, client, vClients)) {
        cout << "\nClient with [" << client.accountNumber << "] already exists,Enter Another Account Number ?";
        getline(cin >> ws, client.accountNumber);
    }
    cout << "\nEnter PinCode? ";
    getline(cin, client.pinCode);
    cout << "\nEnter Name? ";
    getline(cin, client.clientName);
    cout << "\nEnter Phone? ";
    getline(cin, client.phoneNumber);
    cout << "\nEnter AccountBalance? ";
    cin >> client.balance;
    cout << "Client Added Successfully";
    return client;
}
void saveAClientToFile(string line) {
    fstream file(fileName, ios::out | ios::app);
    if (file.is_open()) {
        file << line << endl;
        cout << "Client added successfully, ";

        file.close();
    }
}
void AddANewClient(vector<stClient> vClients) {
    stClient client = readAClientRecord(vClients);
    saveAClientToFile(convertAClientToDataLine(client,"#//#"));
}
void AddClients(vector<stClient>& clients) {
    char answer = 'n';
    do {
        system("cls");
        AddANewClient(clients);
        clients = LoadClientsDataFromFile(fileName);
        cout << "Do you want to add more clients y/n?";
        cin >> answer;
    } while (toupper(answer) == 'Y');
    cout << "\n\npress any key to return to main menu\n";
    system("pause>0");
}
string readAccountNumber() {
    string accountNumber;
    cout << "Please enter account Number? ";
    getline(cin >> ws, accountNumber);
    return accountNumber;
}
void markClientForDeletion(vector<stClient>& vCLients, string accountNumber) {
    for (stClient& c : vCLients) {
        if (c.accountNumber == accountNumber) {
            c.markForDelete = true;
            break;
        }
    }
}
void saveClientsToFile(vector<stClient>& vClients) {
    fstream file(fileName, ios::out);
    if (file.is_open()) {
        for (stClient& client : vClients) {
            if (client.markForDelete == false) {
                file << convertAClientToDataLine(client, "#//#") << endl;
            }
        }
        file.close();
    }
}
void printClientDetails(stClient client) {
    cout << "\nThe following are the client details :";
    cout << "\n________________________________________";
    cout << "\nAccount Number :" << client.accountNumber;
    cout << "\nPin Code       :" << client.pinCode;
    cout << "\nName           :" << client.clientName;
    cout << "\nPhone          :" << client.phoneNumber;
    cout << "\nAccount balance:" << client.balance;
    cout << "\n_______________________________________";
}
bool deleteClientByAccountNumber(vector<stClient>& vClients) {
    stClient client;
    char answer = 'n';
    system("cls");
    cout << "__________________________";
    cout << "\n   Delete Client Screen";
    cout << "\n__________________________\n\n";
    string accountNumber = readAccountNumber();
    if (!findClientByAccountNumber(accountNumber, client, vClients)) {
        cout << "Client with account Number (" << accountNumber << ") is not found!" << endl;
        cout << "\n\n Press Any Key to return to main menu\n";
        system("pause>0");
        return false;
    }
    else {
        printClientDetails(client);
        cout << "\n\nAre you sure you want to delete this client? y/n";
        cin >> answer;
        if (toupper(answer) == 'Y') {
            markClientForDeletion(vClients, accountNumber);
            saveClientsToFile(vClients);
            vClients = LoadClientsDataFromFile(fileName);
            cout << "\n\nClient Deleted successfully.";
            cout << "\n\n Press Any Key to return to main menu\n";
            system("pause>0");
            return true;
        }
    }
}
stClient updateClient(stClient client) {
    stClient newClient;
    newClient.accountNumber = client.accountNumber;
    cout << "\nEnter PinCode?";
    getline(cin >> ws, newClient.pinCode);
    cout << "\nEnter Name? ";
    getline(cin, newClient.clientName);
    cout << "Enter Phone? ";
    getline(cin, newClient.phoneNumber);
    cout << "Enter AccountBalance?";
    cin >> newClient.balance;
    cout << "\n\n Press Any Key to return to Main Menu\n";
    system("pause>0");
    return newClient;
}
bool updateClientByAccountNumber(vector<stClient>& vClients) {
    stClient client;
    char answer = 'n';
    system("cls");
    cout << "_________________________________";
    cout << "\n\n   Update Client Info Screen";
    cout << "\n_________________________________\n\n";
    string accountNumber = readAccountNumber();
    if (findClientByAccountNumber(accountNumber, client, vClients)) {
        printClientDetails(client);
        cout << "\n\nAre you sure you want to update this client? y/n? ";
        cin >> answer;
        if (toupper(answer) == 'Y') {
            for (stClient& c : vClients) {
                if (c.accountNumber == accountNumber) {
                    c = updateClient(client);
                    break;
                }
            }
            saveClientsToFile(vClients);
            cout << "\n\nClient Updated successfully\n";
            return true;
        }
    }
    else {
        cout << "client with accountNumber (" << accountNumber << ") is not found!";
        return false;
    }
}
bool findAClient(vector<stClient> vClient) {
    stClient client;
    system("cls");
    cout << "_________________________________";
    cout << "\n\n       Find Client Screen";
    cout << "\n_________________________________\n\n";
    string accountNumber = readAccountNumber();
    if (findClientByAccountNumber(accountNumber, client, vClient)) {
        printClientDetails(client);
        cout << "\n\n Press any key to return to main Menue\n";
        system("pause>0");
        return true;
    }
    else {
        cout << "Client with account Number (" << accountNumber << ") is not found!";
        cout << "\n\nPress any keyt to return to main Menue\n";
        system("pause>0");
        return false;
    }
}
void printProgramEnd() {
    system("cls");
    cout << "\n__________________________\n\n";
    cout << "      Program Ends :-)\n";
    cout << "__________________________";
    system("pause>0");
}


int main()
{
    vector<stClient> vClients = LoadClientsDataFromFile(fileName);
    while (true) {
        printMenuScreen();
        userChoices userChoice = readUserChoice();
        switch (userChoice)
        {
        case userChoices::showClients:
            showClientsList(vClients);
            break;
        case userChoices::AddClient:
            AddClients(vClients);
            break;
        case userChoices::UpdateClient:
            updateClientByAccountNumber(vClients);
            break;
        case userChoices::DeleteClient:
            deleteClientByAccountNumber(vClients);
            break;
        case userChoices::FindClient:
            findAClient(vClients);
            break;
        case userChoices::Exit:
            printProgramEnd();
            break;
        }
    }
}