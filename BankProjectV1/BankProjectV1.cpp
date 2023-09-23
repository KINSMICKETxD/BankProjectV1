#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <cctype>
#include <fstream>
#include <cmath>
using namespace std;

enum userChoices {showClients =1,AddClient=2,DeleteClient=3,UpdateClient=4,
    FindClient=5,transations=6,ManageUsers=7,Logout=8};
enum transationUserChoices {Deposit=1,Withdraw,TotalBalances,MainMenue};
enum ManageUsersChoices {ListUsers=1,AddNewUser,DeleteUser,UpdateUser,FindUser,MainMenueFromMainMenue};
const string fileName = "clients.txt";
const string usersFile = "Users.txt";
struct stClient
{
    string accountNumber;
    string pinCode;
    string clientName;
    string phoneNumber;
    int balance;
    bool markForDelete = false;
};
struct stUser {
    string userName;
    string passWord;
    short permissions;
    bool deletionFlag = false;
};
void performeUserChoice(short permissions, userChoices userChoice);
void showMainMenueScreen(short permissions);
void login();
void showManageUsersScreen(short permissions);
stUser readFullUser();
vector<stUser> loadUserFromFile(string fileName);
void printUsersList();
void showDeleUsersMenue();
void showUpdateUsersScreen();
void showFindUserScreen();
void goBackToTransationMenue(short permissions, vector<stClient> vClients);
userChoices readUserChoice() {
    short choice;
    cout << "Choose what do you  want to do? [1 to 8]?";
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
    cout << "\n\t[6] Transactions.";
    cout << "\n\t[7] Manage Users.";
    cout << "\n\t[8] Logout.";
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
    printCLientsListHeader(vClients.size());
    for (stClient c : vClients) {
        printAClient(c);
    }
    cout << "\n\n___________________________________________________________________________________________________________";
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
    cout << "__________________________";
    cout << "\n   Delete Client Screen";
    cout << "\n__________________________\n\n";
    string accountNumber = readAccountNumber();
    if (!findClientByAccountNumber(accountNumber, client, vClients)) {
        cout << "Client with account Number (" << accountNumber << ") is not found!" << endl;
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
    return newClient;
}
bool updateClientByAccountNumber(vector<stClient>& vClients) {
    stClient client;
    char answer = 'n';
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
    cout << "_________________________________";
    cout << "\n\n       Find Client Screen";
    cout << "\n_________________________________\n\n";
    string accountNumber = readAccountNumber();
    if (findClientByAccountNumber(accountNumber, client, vClient)) {
        printClientDetails(client);
        return true;
    }
    else {
        cout << "Client with account Number (" << accountNumber << ") is not found!";
        return false;
    }
}
void goBackToMainMenue(short permissions) {
    cout << "\n\nPress any key to go back to main Menue....";
    system("pause>0");
    showMainMenueScreen(permissions);
}
void printTransactionsScreen() {
    system("cls");
    cout << "=============================================";
    cout << "\n        Transactions Menu Screen";
    cout << "\n=============================================";
    cout << "\n\t[1] Deposit.";
    cout << "\n\t[2] withdraw.";
    cout << "\n\t[3] total Balance.";
    cout << "\n\t[4] main menue.";
    cout << "\n=============================================" << endl;
}
int readDeposit() {
    int deposit;
    cout << "\n\nPlease enter deposit amount?\n";
    cin >> deposit;
    return deposit;
}
void printDepositScreen() {
    cout << "\n_______________________\n";
    cout << "\n     Deposite Screen\n";
    cout << "\n_______________________\n";
}
void printWithdrawScreen() {
    cout << "\n_______________________\n";
    cout << "\n   withdraw Screen\n";
    cout << "\n_______________________\n";
}
void deposite(short transactionType,vector<stClient>& vClients) {
    string accountNumber;
    char answer = 'n';
    if (transactionType == 1) {
        printDepositScreen();
    }
    else {
        printWithdrawScreen();
    }
    accountNumber = readAccountNumber();
    int depositAmount;
    stClient client;
    if (findClientByAccountNumber(accountNumber, client, vClients)) {
        printClientDetails(client);
        depositAmount = readDeposit();
        cout << "\n\nAre you sure you want to performe this transaction y/n ? ";
        cin >> answer;
        if (toupper(answer) == 'Y') {
            client.balance += (transactionType* depositAmount);
            for (stClient& c : vClients) {
                if (c.accountNumber == client.accountNumber) {
                    c.balance = client.balance;
                }
            }
            saveClientsToFile(vClients);
            cout << "\n\n  done ! your balance now is " << client.balance << endl;
        }
    }
}
void printBalancesHeader(int totalBalances) {
    cout << "\n                                  Balances list (" << totalBalances << ") client(s).";
    cout << "\n__________________________________________________________________________________________________________";
    cout << "\n\n|" << setw(20) << left << " Account Number";
    cout << "|" << setw(40) << left << " Client Name";
    cout << "|" << setw(15) << left << " Balance";
    cout << "\n__________________________________________________________________________________________________________";
}
void printABalance(stClient client) {
    cout << "\n\n|" << setw(20) << left << client.accountNumber;
    cout << "|" << setw(40) << left << client.clientName;
    cout << "|" << setw(15) << left << client.balance<<"\n";
}
void printBalances(vector<stClient>& vClients) {
    int totalBalances = 0;
    for (stClient& client : vClients) {
        printABalance(client);
        totalBalances += client.balance;
    }
    cout << "\n\n___________________________________________________________________________________________________________";
    cout << "\n\n                                   totalBalances = " << totalBalances<<"\n";
}
void showBalancesList() {
    vector<stClient> vClients = LoadClientsDataFromFile(fileName);
    printBalancesHeader(vClients.size());
    printBalances(vClients);
}
void printProgramEnd() {
    system("cls");
    cout << "\n__________________________\n\n";
    cout << "      Program Ends :-)\n";
    cout << "__________________________";
}
transationUserChoices readTransationChoice() {
    short choice = 0;
    cout << "Choose what do you want to do [1 to 4] ? ";
    cin >> choice;
    return (transationUserChoices)choice;
}
void transactionmenu(short permissions,vector<stClient>& vClients) {
    while (true) {
        printTransactionsScreen();
        transationUserChoices userChoice = readTransationChoice();
        switch (userChoice)
        {
        case Deposit:
            system("cls");
            deposite(1,vClients);
            goBackToTransationMenue(permissions,vClients);
            break;
        case Withdraw:
            system("cls");
            deposite(-1,vClients);
            goBackToTransationMenue(permissions,vClients);
            break;
        case TotalBalances:
            system("cls");
            showBalancesList();
            goBackToTransationMenue(permissions,vClients);
            break;
        case MainMenue:
            showMainMenueScreen(permissions);
            break;
        default:
            break;
        }
    }
}
void goBackToTransationMenue(short permissions, vector<stClient> users) {
    cout << "\n\nPress any key to go back to Transation Menue....";
    system("pause>0");
    transactionmenu(permissions, users);
}
void showMainMenueScreen(short permissions) {
    printMenuScreen();
    userChoices userChoice = readUserChoice();
    performeUserChoice(permissions,userChoice);
}
void goBackToLoginScreen() {
    login();
}
void printAccessDeniedHeader(short permissions) {
    cout << "\n_______________________________________\n";
    cout << "\nAccess Denied,\n";
    cout << "You Don't have Permission to do this,\n";
    cout << "Please contact your Admin.\n";
    cout << "________________________________________\n";
    goBackToMainMenue(permissions);
}
void checkIfUserHaveAccess(short permissions,userChoices userChoice) {
    if (permissions == -1) {
        return;
    }
    short number = 1;
    short i = 1;
    while (i<short(userChoice)) {
        number *= 2;
        i++;
    }
    short result = permissions & number;
    if (result != number) {
        printAccessDeniedHeader(permissions);
        goBackToMainMenue(permissions);
    }
}
void performeUserChoice(short permissions,userChoices userChoice) {
    vector<stClient> vClients = LoadClientsDataFromFile(fileName);
    switch (userChoice)
    {
    case showClients:
        system("cls");
        checkIfUserHaveAccess(permissions, userChoice);
        showClientsList(vClients);
        goBackToMainMenue(permissions);
        break;

    case AddClient:
        system("cls");
        checkIfUserHaveAccess(permissions, userChoice);
        AddANewClient(vClients);
        goBackToMainMenue(permissions);

    case DeleteClient:
        system("cls");
        checkIfUserHaveAccess(permissions, userChoice);
        deleteClientByAccountNumber(vClients);
        goBackToMainMenue(permissions);

    case UpdateClient:
        system("cls");
        checkIfUserHaveAccess(permissions, userChoice);
        updateClientByAccountNumber(vClients);
        goBackToMainMenue(permissions);

    case FindClient:
        system("cls");
        checkIfUserHaveAccess(permissions, userChoice);
        findAClient(vClients);
        goBackToMainMenue(permissions);

    case transations:
        system("cls");
        checkIfUserHaveAccess(permissions, userChoice);
        transactionmenu(permissions, vClients);
        goBackToMainMenue(permissions);
    case ManageUsers:
        system("cls");
        checkIfUserHaveAccess(permissions, userChoice);
        showManageUsersScreen(permissions);
        goBackToMainMenue(permissions);

    case Logout:
        system("cls");
        login();
    }
}
void printUsersListHeader(int usersNumber) {
    cout << "\n                                   Client list (" << usersNumber << ") client(s).";
    cout << "\n__________________________________________________________________________________________________________";
    cout << "\n\n|" << setw(20) << left << " User Name";
    cout << "|" << setw(15) << left << " PassWord";
    cout << "|" << setw(40) << left << " Permissions";
    cout << "\n__________________________________________________________________________________________________________";
}
void printAddNewUserScreenHeader() {
    cout << "\n______________________________\n";
    cout << "\n     Add New User Screen\n";
    cout << "______________________________\n";
    cout << "Adding New User:\n\n";
}
short buildPermissionsCode() {
    short permissions = 0;
    char answer = 'n';
    cout << "\nshow Clients List y/n? ";
    cin >> answer;
    if (toupper(answer) == 'Y') {
        permissions = permissions | 1;
    }
    cout << "Add New Client y/n? ";
    cin >> answer;
    if (toupper(answer) == 'Y') {
        permissions = permissions | 2;
    }
    cout << "Delete Client y/n? ";
    cin >> answer;
    if (toupper(answer) == 'Y') {
        permissions = permissions | 4;
    }   
    cout << "Update Clients y/n ? ";
    cin >> answer;
    if (toupper(answer) == 'Y') {
        permissions = permissions | 8;
    }
    cout << "Find Client? y/n? ";
    cin >> answer;
    if (toupper(answer) == 'Y') {
        permissions = permissions | 16;
    }
    cout << "Transaction? y/n? ";
    cin >> answer;
    if (toupper(answer) == 'Y') {
        permissions = permissions | 32;
    }
    cout << "Manage Users? y/n? ";
    cin >> answer;
    if (toupper(answer) == 'Y') {
        permissions = permissions | 64;
    }
    return permissions;
}
void givePermissionsToUser(stUser& user) {
    short permissions = 0;
    char giveFullAccess = 'n';
    cout << "Do you want to give full access? y/n? ";
    cin >> giveFullAccess;
    if (toupper(giveFullAccess) == 'Y') {
        user.permissions = -1;
    }
    else {
        cout << "Do you want to give access to :\n";
        user.permissions = buildPermissionsCode();
    }
}
bool findUserByUserName(vector<stUser> vUsers, string userName, stUser& user) {
    for (stUser u : vUsers) {
        if (u.userName == userName) {
            user = u;
            return true;
        }
    }
    return false;
}
string userToString(stUser user) {
    string userString;
    userString = user.userName + "#//#" + user.passWord + "#//#" + to_string(user.permissions);
    return userString;
}
bool saveUserToFile(stUser user) {
    fstream file(usersFile, ios::app |ios::out);
    string userString = userToString(user);
    if (file.is_open()) {
        file << userString << endl;
        cout << "User Added successfully,";
        file.close();
        return true;
    }
    else {
        return false;
    }

}
stUser readNewUser() {
    vector<stUser> vUsers = loadUserFromFile("Users.txt");
    stUser user;
    printAddNewUserScreenHeader();
    cout << "Enter Username? ";
    getline(cin >> ws, user.userName);
    while (findUserByUserName(vUsers, user.userName, user)) {
        cout << "\nUser with [" << user.userName << "] already exists,Enter another Username?";
        getline(cin >> ws, user.userName);
    }
    cout << "Enter Password? ";
    getline(cin >> ws, user.passWord);
    return user;

}
void addNewUser() {
    stUser user = readNewUser();
    givePermissionsToUser(user);
    saveUserToFile(user);       
}
void showAddNewUserScreen() {
    char answer = 'n';
    printAddNewUserScreenHeader();
    do {
        addNewUser();
        cout << "Do you want to Add more users ? y/n? ";
        cin >> answer;
    }while (toupper(answer) != 'Y');
}
void goBackToManageMenue(short permissions) {
    cout << "\n\nPress any key to back to manage Users menue....";
    system("pause>0");
    showManageUsersScreen(permissions);
}
void performeManageUsersScreenChoices(short permissions,ManageUsersChoices userChoice) {
    switch (userChoice)
    {
    case ListUsers:
        system("cls");
        printUsersList();
        goBackToManageMenue(permissions);
        break;
    case AddNewUser:
        system("cls");
        addNewUser();
        goBackToManageMenue(permissions);
        break;
    case DeleteUser:
        system("cls");
        showDeleUsersMenue();
        goBackToManageMenue(permissions);
        break;
    case UpdateUser:
        system("cls");
        showUpdateUsersScreen();
        goBackToManageMenue(permissions);
        break;
    case FindUser:
        system("cls");
        showFindUserScreen();
        goBackToManageMenue(permissions);
        break;
    case MainMenueFromMainMenue:
        showMainMenueScreen(permissions);
    default:
        break;
    }
}
void printManageUsersScreenHeader() {
    system("cls");
    cout << "=============================================";
    cout << "\n        Manage Users Menu Screen";
    cout << "\n=============================================";
    cout << "\n\t[1] List Users.";
    cout << "\n\t[2] Add New User.";
    cout << "\n\t[3] Delete User.";
    cout << "\n\t[4] Update User.";
    cout << "\n\t[5] Find User.";
    cout << "\n\t[6] Main menue.";
    cout << "\n=============================================" << endl;
}
void printAUser(stUser user) {
    cout << "\n\n|" << setw(20) << left << user.userName;
    cout << "|" << setw(15) << left << user.passWord;
    cout << "|" << setw(40) << left << user.permissions;
}
void printUsersList() {
    vector<stUser> vUsers;
    vUsers = loadUserFromFile(usersFile);
    printUsersListHeader(vUsers.size());
    for (stUser user: vUsers) {
        printAUser(user);
        cout << "\n";
    }
    cout << "\n___________________________________________________________________________________________________________";
}
ManageUsersChoices readManageUsersChoice() {
    short choice;
    cout << "Choose what do you want to do? [1 to 6]?";
    cin >> choice;
    return (ManageUsersChoices)choice;
}
void printUserDetails(stUser user) {
    cout << "\n\nThe following are the user details :";
    cout << "\n________________________________________";
    cout << "\nUserName     :" << user.userName;
    cout << "\npassWord     :" << user.passWord;
    cout << "\nPermission   :" << user.permissions;
    cout << "\n_______________________________________\n\n";
}
void printDeleteUserScreenHeader() {
    system("cls");
    cout << "__________________________";
    cout << "\n   Delete Client Screen";
    cout << "\n__________________________\n\n";
}
void markUserForDeletion(string userName,vector<stUser>& users) {
    for (stUser& u : users) {
        if (u.userName == userName) {
            u.deletionFlag = true;
            break;
        }
    }
}
void saveAllUsersInFile(vector<stUser> users, string usersFile) {
    string userString;
    fstream file(usersFile, ios::out);
    if (file.is_open()) {
        for (stUser u : users) {
            if (!u.deletionFlag) {
                file << userToString(u) << endl;
            }
        }
    }
}
void printUpdateUsersScreenHeader() {
    system("cls");
    cout << "____________________________";
    cout << "\n   Update Users Screen";
    cout << "\n____________________________\n\n";
}
void printFinduserScreenheader() {
    cout << "____________________________";
    cout << "\n   Find Users Screen";
    cout << "\n____________________________\n\n";
}
void showFindUserScreen() {
    printFinduserScreenheader();
    vector<stUser> vUsers = loadUserFromFile(usersFile);
    string userName;
    stUser user;
    cout << "Please Enter Username? ";
    getline(cin >> ws, userName);
    if (findUserByUserName(vUsers, userName, user)) {
        printUserDetails(user);
    }
    else {
        cout << "User with username (" << userName << ") is not found \n";
    }
}
void showUpdateUsersScreen() {
    printUpdateUsersScreenHeader();
    vector<stUser> vUsers = loadUserFromFile(usersFile);
    string userName;
    char answer = 'n';
    stUser user;
    cout << "Please enter userName? ";
    getline(cin >> ws, userName);
    if(findUserByUserName(vUsers, userName, user)) {
        printUserDetails(user);
        cout << "\nAre you sure you want to update this User? y/n ?";
        cin >> answer;
        if (toupper(answer) == 'Y') {
            cout << "Enter passWord? ";
            getline(cin >> ws, user.passWord);
            givePermissionsToUser(user);
            for (stUser& u : vUsers) {
                if (u.userName == user.userName) {
                    u = user;
                }
            }
            saveAllUsersInFile(vUsers, usersFile);
            cout << "\nUpdate Done successfully!\n";
        }
    }
    else {
        cout << "Client with username (" << userName << ") is not found ! \n";
    }
}
void showDeleUsersMenue() {
    printDeleteUserScreenHeader();
    vector<stUser> vUsers = loadUserFromFile(usersFile);
    string userName;
    char answer = 'n';
    stUser user;
    cout << "\nPlease enter username? ";
    getline(cin >> ws, userName);
    if (findUserByUserName(vUsers, userName, user)) {
        printUserDetails(user);
        cout << "Are you sure you want to delete this User? y/n? ";
        cin >> answer;
        if (toupper(answer) == 'Y') {
            markUserForDeletion(userName,vUsers);
            saveAllUsersInFile(vUsers, usersFile);
            vUsers = loadUserFromFile(usersFile);
            cout << "\nDeletion done successfully!\n\n";
        }
    }
    else {
        cout << "User with UserName (" << userName << ") is not found!\n\n";
    }
}
void showManageUsersScreen(short permissions) {
    printManageUsersScreenHeader();
    performeManageUsersScreenChoices(permissions,readManageUsersChoice());
}
void printLoginScreenHeader() {
    cout << "_______________________\n";
    cout << "   Login Screen\n";
    cout << "_______________________\n";
}
stUser readFullUser() {
    stUser user;
    cout << "Enter Username? ";
    getline(cin >> ws, user.userName);
    cout << "Enter Password? ";
    getline(cin >> ws, user.passWord);
    return user;
}
stUser recordToUser(string record) {
    stUser user;
    vector<string> vLine = splitString(record, "#//#");
    user.userName = vLine[0];
    user.passWord = vLine[1];
    user.permissions = stoi(vLine[2]);
    return user;
}
vector<stUser> loadUserFromFile(string fileName) {
    vector<stUser> myUsers;
    string line;
    stUser user;
    fstream myFile(fileName, ios::in);
    if (myFile.is_open()) {
        while (getline(myFile, line)) {
            user = recordToUser(line);
            myUsers.push_back(user);
        }
    }
    myFile.close();
    return myUsers;
}
bool isUserExist(stUser& user) {
    vector<stUser> users = loadUserFromFile("Users.txt");
    for (stUser u : users) {
        if (u.passWord == user.passWord && u.userName == user.userName) {
            user = u;
            return true;
        }
    }
    return false;
}
void login() {
    printLoginScreenHeader();
    stUser user;
    user = readFullUser();
    while (!isUserExist(user)) {
        system("cls");
        printLoginScreenHeader();
        cout << "Invalid Username/passWord!\n";
        user = readFullUser();
    }
    cout << user.permissions << endl;
    showMainMenueScreen(user.permissions);
}
int main()
{
    login();
}