#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <vector>
#include <fstream>
using namespace std;



struct stUser {
    string UserName;
    string PassWord;
    int Permission;
    bool MarkForDelete = false;
};
enum enTransactionsMenueOptions {
    eDepositTransaction = 1, eWithdrawTransaction = 2,
    eListOfBalance = 3, GoBackTomainMenue = 4
};


enum ePermissionToMaineMenue {
    pAll = -1, pListClient = 1, pAddClient = 2, pUpdateClient = 4, pDeleteClient = 8, pFindClient = 16, pTransactionOption = 32,
    pManageUser = 64
};
enum eManageUserOption {
    eListUser = 1, eAddUser = 2, eFindUser = 3, eUdateUser = 4, eDeleteUser = 5, eGoToMainMenu = 6
};
enum eMainMenueOption {
    eShowScreenList = 1, eAddClients = 2, eFindClient = 3
    , eUpdataClient = 4, eDeleteClient = 5, TransationExtantion = 6, eManageUser = 7,
    Loginout = 8
};
string const AddDataClient = "Client.txt";
string const UsersNameFile = "User.txt";

stUser CurrentUser; 
struct StClients {
    string AccountNumber = "";
    string PinCode = "";
    string Name = "";
    string Phone = "";
    double AccountBalance;
    bool MarkForDelete = false; // flag to delete Account 
};

void ShowMainMenue();
void ShowTransactionMenue();
void ShowManageUserMenue();
void GoBackToMainMenue();
void Login();
vector <string> SplitString(string st, string Delim) {

    short pos = 0;
    short Counter = 0;
    string sWord;
    vector <string> vString;
    while ((pos = st.find(Delim)) != (std::string::npos)) {
        sWord = st.substr(0, pos);
        if (sWord != "") {
            vString.push_back(sWord);
        }
        st.erase(0, pos + Delim.length());
    }
    if (sWord != "") {
        vString.push_back(st);
    }
    return vString;
}
string TransfromStructToOneLineString(StClients Client, string Separator = "#//#") {
    string OneLineRecord = "";
    OneLineRecord += Client.AccountNumber + Separator;
    OneLineRecord += Client.PinCode + Separator;
    OneLineRecord += Client.Name + Separator;
    OneLineRecord += Client.Phone + Separator;
    OneLineRecord += to_string(Client.AccountBalance);
    return OneLineRecord;
}
string TransfromStructUserToOneLineString(stUser User, string Separator = "#//#") {
    string OneLineRecord = "";
    OneLineRecord += User.UserName   + Separator;
    OneLineRecord += User.PassWord   + Separator;
    OneLineRecord += to_string(User.Permission);
    
    return OneLineRecord;
}
StClients TransformOneLineToStruct(string Line) {
    StClients Clinet;
    vector <string> vClient = SplitString(Line, "#//#");
    Clinet.AccountNumber = vClient[0];
    Clinet.PinCode = vClient[1];
    Clinet.Name = vClient[2];
    Clinet.Phone = vClient[3];
    Clinet.AccountBalance = stod(vClient[4]);

    return Clinet;
}
stUser TransformOneLineToStructUser(string Line) {
    stUser User;
    vector <string> vUser = SplitString(Line, "#//#");
    User.UserName = vUser[0];
    User.PassWord = vUser[1];
    User.Permission = stoi(vUser[2]);
   
  

    return User;
}
vector <StClients> LoadCientFileToVector(string FileName) {
    vector <StClients>vClient;
    string Line;
    StClients Client;
    fstream Myfile;
    Myfile.open(FileName, ios::in);//ReadMode 
    if (Myfile.is_open()) {
        while (getline(Myfile, Line)) {//Read File Line By Line AND Save it In the String 
            //1 Load one Line in the to File to Struct 
            Client = TransformOneLineToStruct(Line);
            //2 Load Client Struct in the vector USING While Loop     
            vClient.push_back(Client);
        }
        Myfile.close();
    }
    //3 return the vector 
    return vClient;
}
vector <stUser> LoadUserFileToVector(string FileName) {
    vector <stUser>vUser;
    string Line;
    stUser User;
    fstream Myfile;
    Myfile.open(FileName, ios::in);//ReadMode 
    if (Myfile.is_open()) {
        while (getline(Myfile, Line)) {//Read File Line By Line AND Save it In the String 
            //1 Load one Line in the to File to Struct 
            User = TransformOneLineToStructUser(Line);
            //2 Load Client Struct in the vector USING While Loop     
            vUser.push_back(User);
        }
        Myfile.close();
    }
    //3 return the vector 
    return vUser;
}
bool FindUserByUserNameAmdPassword(string Username, string Password, stUser& User) {
    vector <stUser> vUser = LoadUserFileToVector(UsersNameFile);
    for (stUser U : vUser) {
        if (U.UserName == Username && U.PassWord == Password) {
            User = U;
            return true;
        }
        
    }
    return false;
}
bool CheckAccessPermission(ePermissionToMaineMenue Permission) {
    if (CurrentUser.Permission == ePermissionToMaineMenue::pAll) {
        
        return true;
        
    }
    if ((Permission & CurrentUser.Permission) == Permission)
        return true;
    else
        return false;
}
void AccessDeniedScreen() {
    system("cls");
    cout << "\n\t==================================================\n";
    cout << "\tAccess Denied Don't Have Permission to this  Option \n\tContact your Administrator  ";
    cout << "\n\t==================================================\n";
}
void  PrintLineOfDataClient(StClients Client) {
    cout << "| " << left << setw(15) << Client.AccountNumber;
    cout << "| " << left << setw(10) << Client.PinCode;
    cout << "| " << left << setw(40) << Client.Name;
    cout << "| " << left << setw(30) << Client.Phone;
    cout << "| " << left << setw(20) << Client.AccountBalance;

}
void  PrintLineOfDataUser(stUser User) {
    cout << "| " << left << setw(15) << User.UserName;
    cout << "| " << left << setw(10) << User.PassWord;
    cout << "| " << left << setw(40) << User.Permission;
    

}
void ShowListClientScreen() {
    if (!CheckAccessPermission(ePermissionToMaineMenue::pListClient)) {
        AccessDeniedScreen();
        GoBackToMainMenue();
        return;
    }
    cout << "____________________________________\n";
    cout << "\tShow Clients List ";
    cout << "\n___________________________________\n";
    vector<StClients>vClient = LoadCientFileToVector(AddDataClient);
    cout << "\t\t\t Clients Number " << vClient.size() << " Client(s)\n\n";
    cout << "________________________________________________________";
    cout << "______________________________________________________\n";

    cout << "| " << setw(15) << left << "Account Number ";
    cout << "| " << setw(10) << left << "PIN Code ";
    cout << "| " << setw(40) << left << "Name ";
    cout << "| " << setw(30) << left << "Phone ";
    cout << "| " << setw(20) << left << "Account Balance";
    cout << endl;
    if (vClient.size() == 0) {
        cout << "\nYou Dont Have Any Client In the System\n";
    }
    else
        for (StClients C : vClient) {
            PrintLineOfDataClient(C);
            cout << endl;
        }
    cout << "________________________________________________________";
    cout << "___________________________________________________________________\n";



}
void ShowListUserScreen() {
    cout << "____________________________________\n";
    cout << "\tShow Clients List ";
    cout << "\n___________________________________\n";
    vector<stUser>vUser = LoadUserFileToVector(UsersNameFile);
    cout << "\t\t\t Clients Number " << vUser.size() << " User(s)\n\n";
    cout << "________________________________________________________";
    cout << "______________________________________________________\n";

    cout << "| " << setw(15) << left << "User Name  ";
    cout << "| " << setw(10) << left << "Password ";
    cout << "| " << setw(40) << left << "Permission";
  
    cout << endl;
    if (vUser.size() == 0) {
        cout << "\nYou Dont Have Any User In the System\n";
    }
    else
        for (stUser C : vUser) {
            PrintLineOfDataUser(C);
            cout << endl;
        }
    cout << "________________________________________________________";
    cout << "___________________________________________________________________\n";



}
bool IsExisteAccountNumber(string AccountNumber, string FileName) {
    vector<StClients> vClient;
    fstream Myfile;
    Myfile.open(FileName, ios::in);//Read Only Mode
    if (Myfile.is_open()) {
        string Line;
        StClients Client;
        while (getline(Myfile, Line)) {//read the file line by line 
            Client = TransformOneLineToStruct(Line);
            if (Client.AccountNumber == AccountNumber) {
                return true;
                Myfile.close();
            }
            vClient.push_back(Client);
        }
        Myfile.clear();
    }
    return false;
}
bool IsExisteUserName(string UserName, string FileName) {
    vector<stUser> vUser;
    fstream Myfile;
    Myfile.open(FileName, ios::in);//Read Only Mode
    if (Myfile.is_open()) {
        string Line;
        stUser User;
        while (getline(Myfile, Line)) {//read the file line by line 
            User = TransformOneLineToStructUser(Line);
            if (User.UserName == UserName) {
                return true;
                Myfile.close();
            }
            vUser.push_back(User);
        }
        Myfile.clear();
    }
    return false;
}
StClients ReadClientData() {
    StClients Client;
    cout << "Please Enter Client Details :\n";
    cout << "Account Number  : ";
    getline(cin >> ws, Client.AccountNumber);
    while (IsExisteAccountNumber(Client.AccountNumber, AddDataClient)) {
        cout << "\nAccount number [" << Client.AccountNumber << "Is All Ready Exist ";
        cout << ", Enter other Account Number ";
        getline(cin >> ws, Client.AccountNumber);
    }
    cout << "PIN Code        : ";
    getline(cin, Client.PinCode);
    cout << "Name            : ";
    getline(cin, Client.Name);
    cout << "Phone            : ";
    getline(cin, Client.Phone);
    cout << "Account Balance  : ";
    cin >> Client.AccountBalance;
    return Client;
}
int PermissionToSet() {
    int Permission = 0;
    char answer = 'n';

    cout << "\nDo you Want to Give all full Access(n/y)\n";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        return  -1;
    }
    cout << "\nList Client (n/y) ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        Permission += ePermissionToMaineMenue::pListClient;
    }
    cout << "\nAdd Client (n/y) ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        Permission += ePermissionToMaineMenue::pAddClient;
    }
    cout << "\nUpdate Client (n/y) ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        Permission += ePermissionToMaineMenue::pUpdateClient;
    }
    cout << "\nDelete Client (n/y) ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        Permission += ePermissionToMaineMenue::pDeleteClient;
    }
    cout << "\nFind Client (n/y) ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        Permission += ePermissionToMaineMenue::pFindClient;
    }
    cout << "\nTransaction Option  (n/y) ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        Permission += ePermissionToMaineMenue::pTransactionOption;

    }
    cout << "\nMange User Option (n/y) ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        Permission += ePermissionToMaineMenue::pManageUser;

    }
    return Permission;
}
stUser ReadUserData() {
    stUser User;
    cout << "Please Enter User Details :\n";
    cout << "Enter User Name  : ";
    getline(cin >> ws,User.UserName);
    while (IsExisteUserName(User.UserName, UsersNameFile)) {
        cout << "\nUser Name [" << User.UserName << "Is All Ready Exist ";
        cout << ", Enter other User Name ";
        getline(cin >> ws, User.UserName);
    }
    cout << "Enter Password        : ";
    getline(cin >> ws , User.PassWord);
    cout << "Permission            : ";
    User.Permission = PermissionToSet();
    return User; 
}
void AddClientsToFile(string Line, string FileName) {
    fstream Myfile;
    Myfile.open(FileName, ios::out | ios::app);
    if (Myfile.is_open()) {
        Myfile << Line << endl;
        Myfile.close();
    }
}
void AddUserToFile(string Line, string FileName) {
    fstream Myfile;
    Myfile.open(FileName, ios::out | ios::app);
    if (Myfile.is_open()) {
        Myfile << Line << endl;
        Myfile.close();
    }
}


string ReadUserName() {
    string User; 
    cout << "Please Enter UserName you looking for ?\n";
    cin >> User;
    return User; 

}
void AddUser() {
    
    stUser User;
    User = ReadUserData();
    AddUserToFile(TransfromStructUserToOneLineString(User), UsersNameFile);
}

void AddClient() {
    StClients Client;
    Client = ReadClientData();
    AddClientsToFile(TransfromStructToOneLineString(Client), AddDataClient);
}
void AddNewUser() {
    char Again = 'n';
    do {

        cout << "Add New User:  \n";
        AddUser();
        cout << "\nClient Added Successfully , Do you want More Client (n/y)?\n";
        cin >> Again;


    } while (tolower(Again) == 'y');
}
void ShowAddUserScreen() {
    cout << "____________________________________\n";
    cout << "\t Add User  ";
    cout << "\n___________________________________\n";
    AddNewUser();
}
void AddNewClient() {
    char Again = 'n';
    do {

        cout << "Add New Client:  \n";
        AddClient();
        cout << "\nClient Added Successfully , Do you want More Client (n/y)?\n";
        cin >> Again;


    } while (tolower(Again) == 'y');
}

void ShowAddClientsScreen() {
    if (!CheckAccessPermission(ePermissionToMaineMenue::pAddClient)) {
        AccessDeniedScreen();
        GoBackToMainMenue();
        return;
    }
    cout << "____________________________________\n";
    cout << "\t Add Clients  ";
    cout << "\n___________________________________\n";
    AddNewClient();
}
string  ReadAccoutnNumber() {
    string AccountNumber = "";
    cout << "Please Enter the Account Number you Looking For ?\n";
    cin >> AccountNumber;
    return AccountNumber;
}
void PrintFileCard(StClients Clients) {
    cout << "\nClient Details: \n";
    cout << "Account Number      :" << Clients.AccountNumber << endl;
    cout << "PIN Code            :" << Clients.PinCode << endl;
    cout << "Name                :" << Clients.Name << endl;
    cout << "Phone               :" << Clients.Phone << endl;
    cout << "Account Balance     :" << Clients.AccountBalance << endl;
}
void PrintUserInfo(stUser User) {
    cout << "\nUser Details: \n";
    cout << "User Name      :" << User.UserName << endl;
    cout << "Password           :" << User.PassWord << endl;
    cout << "Permission             :" << User.Permission << endl;
  
}
bool FindClientByAccountNumber(string AccountNumber, vector<StClients>vClient, StClients& Client) {

    for (StClients& C : vClient) {
        if (C.AccountNumber == AccountNumber) {
            //Dont Forget
            Client = C;
            /////////////
            return true;
        }
    }
    return false;
}
bool FindUserByUserName(string UserName, vector<stUser>vUser, stUser&User) {

    for (stUser& C : vUser) {
        if (C.UserName ==UserName) {
            //Dont Forget
            User = C;
            /////////////
            return true;
        }
    }
    return false;
}
void ShowFindUserScreen() {
    cout << "____________________________________\n";
    cout << "\t Find User   ";
    cout << "\n___________________________________\n";
    stUser User;
    string UserName = ReadUserName();
    vector <stUser>vUser = LoadUserFileToVector(UsersNameFile);
    if (FindUserByUserName(UserName, vUser, User)) {
        PrintUserInfo( User);
    }
    else { 
        cout << "User ( " << UserName << " ) Dont Found ! \n";
    }


}

void ShowFindClientScreen() {
    if (!CheckAccessPermission(ePermissionToMaineMenue::pFindClient)) {
        AccessDeniedScreen();
        GoBackToMainMenue();
        return;
    }
    cout << "____________________________________\n";
    cout << "\t Find Client   ";
    cout << "\n___________________________________\n";
    StClients Client;
    string AccountNumber = ReadAccoutnNumber();
    vector <StClients>vClient = LoadCientFileToVector(AddDataClient);
    if (FindClientByAccountNumber(AccountNumber, vClient, Client)) {
        PrintFileCard(Client);
    }
    else {
        cout << "Account ( " << AccountNumber << " ) Dont Found ! \n";
    }


}
StClients ModifyClient(string AccountNumber) {
    StClients Client;
    cout << "\nUpdate Client Data? \n";
    Client.AccountNumber = AccountNumber;
    cout << "PIN Code         : ";
    getline(cin >> ws, Client.PinCode);
    cout << "Name             : ";
    getline(cin, Client.Name);
    cout << "Phone            : ";
    getline(cin, Client.Phone);
    cout << "Account Balance  : ";
    cin >> Client.AccountBalance;
    return Client;
}
stUser ModifyUser(string UserName) {
    stUser User;
    cout << "\nUpdate Client Data? \n";
    User.UserName = UserName;
    cout << "\nPassword         : ";
    getline(cin >> ws, User.PassWord);
    cout << "\nPermission  : ";
    User.Permission=PermissionToSet();
    return User;
}
vector <StClients>  SavaNewDataToFile(string FileName, vector<StClients>vClient) {
    string Line;
    fstream Myfile;
    Myfile.open(FileName, ios::out);//OverWrite 
    if (Myfile.is_open()) {
        for (StClients C : vClient) {
            if (C.MarkForDelete == false) {
                Line = TransfromStructToOneLineString(C);
                Myfile << Line << endl;

            }
        }
        Myfile.close();
    }
    return vClient;
}
vector <stUser> SavaNewDataUserToFile(string FileName, vector<stUser>vUser) {
    string Line;
    fstream Myfile;
    Myfile.open(FileName, ios::out);//OverWrite 
    if (Myfile.is_open()) {
        for (stUser C : vUser) {
            if (C.MarkForDelete == false) {
                Line = TransfromStructUserToOneLineString(C);
                Myfile << Line << endl;

            }
        }
        Myfile.close();
    }
    return vUser;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <StClients>& vClient) {
    StClients Client;
    char answer = 'n';
    if (FindClientByAccountNumber(AccountNumber, vClient, Client)) {
        PrintFileCard(Client);
        cout << "\nDo you want To Update Data Client (N/y)?\n";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            for (StClients& C : vClient) {
                if (C.AccountNumber == AccountNumber) {
                    C = ModifyClient(AccountNumber);
                    break;
                }
            }

            SavaNewDataToFile(AddDataClient, vClient);

            cout << "\nUpdate Client ,Successful.\n";
            return true;
        }
    }
    else {
        cout << "Account Number (" << AccountNumber << ") Dont Fount!\n";
        return false;

    }
}
bool UpdateUserByUserName(string UserName, vector <stUser>& vUser) {
    stUser User;
    if (UserName == "Admin") {
        cout << "You Can't Delete the Account ";
        return false; 
    }
    char answer = 'n';
    if (FindUserByUserName(UserName, vUser, User)) {
        PrintUserInfo(User);
        cout << "\nDo you want To Update Data User (N/y)?\n";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            for (stUser& C : vUser) {
                if (C.UserName == UserName) {
                    C = ModifyUser(UserName);
                    break;
                }
            }

            SavaNewDataUserToFile(UsersNameFile, vUser);

            cout << "\nUpdate User ,Successful.\n";
            return true;
        }
    }
    else {
        cout << "User Name  (" << UserName << ") Dont Fount!\n";
        return false;

    }
}
void ShowUpdateClientsScreen() {
    if (!CheckAccessPermission(ePermissionToMaineMenue::pUpdateClient)) {
        AccessDeniedScreen();
        GoBackToMainMenue();
        return;
    }
    cout << "____________________________________\n";
    cout << "\tUpdate Client    ";
    cout << "\n___________________________________\n";
    string AccountNumber = ReadAccoutnNumber();
    vector<StClients>vClient = LoadCientFileToVector(AddDataClient);
    UpdateClientByAccountNumber(AccountNumber, vClient);


}
void ShowUpdateUserScreen() {
    cout << "____________________________________\n";
    cout << "\tUpdate User    ";
    cout << "\n___________________________________\n";
    string UserName = ReadUserName();
    vector<stUser>vUser = LoadUserFileToVector(UsersNameFile);
    UpdateUserByUserName(UserName, vUser);


}
bool MarkDeDelete(vector<StClients>& vClient, string AccountNumber) {
    for (StClients& C : vClient) {
        if (C.AccountNumber == AccountNumber) {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}
bool MarkToDeleteUser(vector<stUser>& vUser, string UserName) {
    for (stUser& C : vUser) {
        if (C.UserName == UserName) {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}
bool DeleteClientByAccountNumber(string AccountNumber, vector <StClients>& vClient) {
    char answer = 'n';
    StClients Client;
    if (FindClientByAccountNumber(AccountNumber, vClient, Client)) {
        PrintFileCard(Client);
        cout << "\nDo you Want to Delete this Account (N/Y)?\n";
        cin >> answer;
        if (answer == 'Y' || answer == 'y') {
            MarkDeDelete(vClient, AccountNumber);
            SavaNewDataToFile(AddDataClient, vClient);
            // Refrash Data
            vClient = LoadCientFileToVector(AddDataClient);// will Send a New Vector file BY ref because we Delete the Data Client 
            cout << "Delete Client , Successful!\n";
            return true;
        }
    }
    else {
        cout << "Account ( " << AccountNumber << " ) Dont Found ! \n";
        return false;
    }

}
bool DeleteUserByUserName(string UserName, vector <stUser>& vUser) {
    char answer = 'n';
    stUser User;
    if (UserName == "Admin") {
        cout << "\nYou Can't Delete this User .\n";
        return false;
    }
    if (FindUserByUserName(UserName, vUser, User)) {
        PrintUserInfo(User);
        cout << "\nDo you Want to Delete this User (N/Y)?\n";
        cin >> answer;
        if (answer == 'Y' || answer == 'y') {
            MarkToDeleteUser(vUser, UserName);
            SavaNewDataUserToFile(UsersNameFile, vUser);
            // Refrash Data
            vUser = LoadUserFileToVector(UsersNameFile);// will Send a New Vector file BY ref because we Delete the Data Client 
            cout << "Delete User , Successful!\n";
            return true;
        }
    }
    else {
        cout << "User ( " << UserName << " ) Dont Found ! \n";
        return false;
    }

}
void ShowDeleteClientScreen() {
    if (!CheckAccessPermission(ePermissionToMaineMenue::pUpdateClient)) {
        AccessDeniedScreen();
        GoBackToMainMenue();
        return;
    }
    cout << "____________________________________\n";
    cout << "\tDelete Client    ";
    cout << "\n___________________________________\n";
    vector <StClients>vClient = LoadCientFileToVector(AddDataClient);
    string AccountNumber = ReadAccoutnNumber();
    DeleteClientByAccountNumber(AccountNumber, vClient);

}
void ShowDeleteUserScreen() {
    cout << "____________________________________\n";
    cout << "\tDelete User   ";
    cout << "\n___________________________________\n";
    vector <stUser>vUser = LoadUserFileToVector(UsersNameFile);
    string UserName = ReadUserName();
    DeleteUserByUserName(UserName, vUser);

}
void ShowExitScreen() {
    cout << "____________________________________\n";
    cout << "\tExit     ";
    cout << "\n___________________________________\n";

}





bool AddDepositTransactionByAccountNumber(string AccountNumber, vector <StClients>& vClient) {
    StClients Client;
    char answer = 'n';
    double Deposit = 0;
    double TotalOfBalance = 0;
    if (FindClientByAccountNumber(AccountNumber, vClient, Client)) {
        PrintFileCard(Client);
        cout << "\nPlease Enter Deposit Amount ";
        cin >> Deposit;
        for (StClients& C : vClient) {
            if (C.AccountNumber == AccountNumber) {
                C.AccountBalance += Deposit;
                TotalOfBalance = C.AccountBalance;
                break;
            }




        }
        cout << "\nAre You sure to preform this transaction  y / n  ?";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            SavaNewDataToFile(AddDataClient, vClient);
        }
        cout << "\nYour Balance Now " << TotalOfBalance << endl;
        return true;
    }
    else {
        cout << "Account Number (" << AccountNumber << ") Dont exist !\n";
        return false;

    }
}



void ShowDepositTransactionScreen() {
    cout << "____________________________________\n";
    cout << "\tDeposit Transaction    ";
    cout << "\n___________________________________\n";
    string AccountNumber = ReadAccoutnNumber();
    vector<StClients>vClient = LoadCientFileToVector(AddDataClient);
    AddDepositTransactionByAccountNumber(AccountNumber, vClient);
    cout << "\nTransaction Successful \n";


}
bool WithdrawTransactionByAccountNumber(string AccountNumber, vector <StClients>& vClient) {
    StClients Client;
    char answer = 'n';
    double NewBalance = 0;
    double DrawNumber = 0;
    if (FindClientByAccountNumber(AccountNumber, vClient, Client)) {
        PrintFileCard(Client);
        cout << "\nPlease Enter Withdraw Amount ";
        cin >> DrawNumber;
        while (DrawNumber > Client.AccountBalance) {
            cout << "\nAmount Exceeds the balance , you can withdraw down to ";
            cout << Client.AccountBalance;
            cout << "\nPlease Enter Another Amount :";
            cin >> DrawNumber;
        }
        for (StClients& C : vClient) {
            if (C.AccountNumber == AccountNumber) {

                C.AccountBalance += (DrawNumber * -1);// when we use -1 will doing withdraw function 
                NewBalance = C.AccountBalance;
                break;
            }




        }
        cout << "\nAre You sure to preform this transaction  y / n  ?";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            SavaNewDataToFile(AddDataClient, vClient);
        }
        cout << "\nYour Balance New " << NewBalance << endl;
        return true;
    }
    else {
        cout << "Account Number (" << AccountNumber << ") Dont exist !\n";
        return false;

    }
}
void ShowWithdrawTransaction() {
    cout << "____________________________________\n";
    cout << "\tWithdraw Transaction    ";
    cout << "\n___________________________________\n";
    string AccountNumber = ReadAccoutnNumber();
    vector<StClients>vClient = LoadCientFileToVector(AddDataClient);
    WithdrawTransactionByAccountNumber(AccountNumber, vClient);
    cout << "\nTransaction Successful \n";
}
void  PrintLineOfDataBalanceClient(StClients Client) {
    cout << "| " << left << setw(15) << Client.AccountNumber;
    cout << "| " << left << setw(40) << Client.Name;
    cout << "| " << left << setw(20) << Client.AccountBalance;

}
void ShowBalanceScreen() {
    cout << "____________________________________\n";
    cout << "\tBalance Transaction List    ";
    cout << "\n___________________________________\n";
    vector<StClients>vClient = LoadCientFileToVector(AddDataClient);
    double TotalOfBalance = 0;
    cout << "\t\t\t Clients Number " << vClient.size() << " Client(s)\n\n";
    cout << "________________________________________________________";
    cout << "______________________________________________________\n";

    cout << "| " << setw(15) << left << "Account Number ";
    cout << "| " << setw(40) << left << "Name ";

    cout << "| " << setw(20) << left << "Account Balance";
    cout << endl;
    if (vClient.size() == 0) {
        cout << "\nYou Dont Have Any Client In the System\n";
    }
    else
        for (StClients C : vClient) {
            PrintLineOfDataClient(C);
            TotalOfBalance += C.AccountBalance;
            cout << endl;
        }
    cout << "________________________________________________________";
    cout << "___________________________________________________________________\n";
    cout << "\tthe Total of Balances is " << TotalOfBalance << endl;
}

void GoBackToMainMenue() {
    cout << "\n\nPress Any Key To go Back To Main Menue...";
    system("pause>0");
    ShowMainMenue();

}
short ReadTransactionMenue() {
    short Choice = 0;
    cout << "\nChoose the Option you want [1, 4] ? ";
    cin >> Choice;
    return Choice;
}
void GoBackToTransactionMenue() {
    cout << "\n\nPress Any Key To go Back To Transaction Menu...";
    system("pause>0");
    ShowTransactionMenue();

}
void GoBackToMangeUserMenue() {
    cout << "\n\nPress Any Key To go Back To manage User Menu...";
    system("pause>0");
    ShowManageUserMenue();

}


short ReadMangeUserMenue() {
    short Choice = 0;
    cout << "\nChoose the Option you want [1, 6] ? ";
    cin >> Choice;
    return Choice;
}









































short ReadMainMenue() {
    short Choice = 0;
    cout << "\nChoose the Option you want [1, 8] ? ";
    cin >> Choice;
    return Choice;
}
void PerfromTranactionsMenueOption(enTransactionsMenueOptions TransactionMenueOption)
{
    switch (TransactionMenueOption)
    {
    case enTransactionsMenueOptions::eDepositTransaction:
    {
        system("cls");
        ShowDepositTransactionScreen();
        GoBackToTransactionMenue();
        break;
    }

    case enTransactionsMenueOptions::eWithdrawTransaction:
    {
        system("cls");
        ShowWithdrawTransaction();
        GoBackToTransactionMenue();
        break;
    }


    case enTransactionsMenueOptions::eListOfBalance:
    {
        system("cls");
        ShowBalanceScreen();
        GoBackToTransactionMenue();
        break;
    }


    case enTransactionsMenueOptions::GoBackTomainMenue:
    {

        ShowMainMenue();

    }
    }

}



void ShowTransactionMenue() {
    if (!CheckAccessPermission(ePermissionToMaineMenue::pTransactionOption))
    {
        AccessDeniedScreen();
        GoBackToMainMenue();
        return;
    }
    system("cls");
    cout << "\t\t\t____________________________\n";
    cout << "\t\t\t   Transaction Menue Screen ";
    cout << "\n\t\t\t___________________________\n";
    cout << "\t\t\t[1] Deposit Transaction  \n";
    cout << "\t\t\t[2] Withdraw Transaction   \n";
    cout << "\t\t\t[3] List Of Balance   \n";
    cout << "\t\t\t[4] Go back to Main Menue \n";
    PerfromTranactionsMenueOption((enTransactionsMenueOptions)ReadTransactionMenue());

}
void PerformManageUserManue(eManageUserOption ManageUserOption) {
    switch (ManageUserOption) {
    case eManageUserOption::eListUser:
    {
        system("cls");
        ShowListUserScreen();
        GoBackToMangeUserMenue();
        break;
    }
    case eManageUserOption::eAddUser:
    {
        system("cls");
        ShowAddUserScreen();
        GoBackToMangeUserMenue();
        break;
    }
    case eManageUserOption::eFindUser:
    {
        system("cls");
        ShowFindUserScreen();
        GoBackToMangeUserMenue();
    }
    case eManageUserOption::eUdateUser:
    {
        system("cls");
        ShowUpdateUserScreen();
        GoBackToMangeUserMenue();

    }
    case eManageUserOption::eDeleteUser:
    {
        system("cls");
        ShowDeleteUserScreen();
        GoBackToMangeUserMenue();

    }
    case eManageUserOption::eGoToMainMenu:
    {
        system("cls");
        ShowMainMenue();
        break;
    }
    }
}
void ShowManageUserMenue() {
    if (!CheckAccessPermission(ePermissionToMaineMenue::pManageUser))
    {
        AccessDeniedScreen();
        GoBackToMainMenue();
        return;
    }

    system("cls");
    cout << "\t\t\t____________________________\n";
    cout << "\t\t\t   Manage User Screen Menue ";
    cout << "\n\t\t\t___________________________\n";
    cout << "\t\t\t[1] Show List User  \n";
    cout << "\t\t\t[2] Add User  \n";
    cout << "\t\t\t[3] Find User   \n";
    cout << "\t\t\t[4] Updata User  \n";
    cout << "\t\t\t[5] Delete User   \n";
    cout << "\t\t\t[6] Go To Main Menue  \n";
    PerformManageUserManue((eManageUserOption)ReadMangeUserMenue());
}
void PrefromMainMenue(eMainMenueOption MainMenueOption) {
    switch (MainMenueOption) {
    case eMainMenueOption::eShowScreenList:
    {
        system("cls");
        ShowListClientScreen();
        GoBackToMainMenue();
        break;
    }
    case eMainMenueOption::eAddClients:
    {
        system("cls");
        ShowAddClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case eMainMenueOption::eFindClient:
    {
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;
    }
    case eMainMenueOption::eUpdataClient:
    {
        system("cls");
        ShowUpdateClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case eMainMenueOption::eDeleteClient:
    {
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;
    }
    case eMainMenueOption::TransationExtantion:
    {
        system("cls");
        ShowTransactionMenue();
        GoBackToMainMenue();
        break;

    }
    case eMainMenueOption::eManageUser:
    {
        system("cls");
        ShowManageUserMenue();
        GoBackToMainMenue();
        break;
    }
    case eMainMenueOption::Loginout:
    {
        system("cls");
        Login();
        break;
    }
    }

}


void ShowMainMenue() {
    system("cls");
    cout << "\t\t\t____________________________\n";
    cout << "\t\t\t   Main Menue Screen ";
    cout << "\n\t\t\t___________________________\n";
    cout << "\t\t\t[1] Show Screen List Clients  \n";
    cout << "\t\t\t[2] Add Clients  \n";
    cout << "\t\t\t[3] Find Client   \n";
    cout << "\t\t\t[4] Updata Client Data \n";
    cout << "\t\t\t[5] Delete Client   \n";
    cout << "\t\t\t[6] Transaction Menue   \n";
    cout << "\t\t\t[7] Manage User  \n";
    cout << "\t\t\t[8] Login out \n";
    PrefromMainMenue((eMainMenueOption)ReadMainMenue());
}


bool LoadUserInfo(string UserName, string Password) {
    if (FindUserByUserNameAmdPassword(UserName, Password, CurrentUser))
        return true;
    else
        return false;
}
void Login() {
    string UserName, Password;
    bool LoginFailed = false; 
    do {
        system("cls");
        cout << "---------------------------------------------\n";
        cout << "\tLogin";
        cout << "\n--------------------------------------------\n";
        if (LoginFailed) {
            cout << "Invalid User Name / Password \n";

        }
        cout << "Enter User Name:   \n";
        cin >> UserName;
        cout << "Enter Password:    \n";
        cin >> Password; 
        LoginFailed = (!LoadUserInfo(UserName, Password));

    } while (LoginFailed);

    ShowMainMenue();
}

int main() {
    Login();


    return 0;


}