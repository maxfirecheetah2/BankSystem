#include "sqlite3.h"
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

#define create_client "create client"
#define add_account "add account"
#define add_money "add money"
#define withdraw_money "withdraw money"
#define view_accounts "view accounts"
#define exit "exit"

#define admin_create_client 1
#define admin_add_account 2
#define admin_exit 3
#define operator_add_money 1
#define operator_withdraw_money 2
#define operator_exit 3

enum roleInSystem{UNKNOWN,ADMIN,OPERATOR};
roleInSystem roleIdentified;

int currentBalance;
int currentTransaction;
int totalTransaction;
int monthlyQuota;
void createClient(){}
void addAccount(){}
int balance = 0;
int addMoney(){
	balance=0;
	printf("Enter sum: ");
	scanf("%d", &balance);
	int fee = (currentTransaction + 1 - totalTransaction) * monthlyQuota;
	if(currentTransaction + 1 > totalTransaction && balance + currentBalance - fee < 0){
		cout << "It is not possible to commit the transaction";
		return -1;
	}
	currentTransaction++;
	if(currentTransaction > totalTransaction){
		balance = balance + currentBalance - fee;
	} else{
		balance = balance + currentBalance;
	}
	return 0;
}
int withdrawMoney(){
	balance=0;
	printf("Enter sum: ");
	scanf("%d", &balance);
	int fee = (currentTransaction + 1 - totalTransaction) * monthlyQuota;
	if(currentTransaction + 1 > totalTransaction && currentBalance  - balance - fee < 0){
		cout << "It is not possible to commit the transaction";
		return -1;
	}
	currentTransaction++;
	if(currentTransaction > totalTransaction){
		balance = currentBalance - balance - fee;
	} else{
		balance = currentBalance - balance;
	}
	return 0;
}

void viewAccounts(){}

void adminActions(){
	bool isExit=false;
	int numberOfOperation=0;
	while(!isExit){
		cout << "Operations:" << endl;
		cout <<admin_create_client<<" "<< create_client << endl;
		cout <<admin_add_account<<" "<< add_account << endl;
		cout <<admin_exit<<" "<< exit<<endl;
	
		cout << "Please, enter a number of operation." << endl;
		scanf("%d",&numberOfOperation);
		
		switch(numberOfOperation){
		case admin_create_client:
			createClient();
			break;
		case admin_add_account:
			addAccount();
			break;
		case admin_exit:
			cout<<"Good bye!"<<endl;
			roleIdentified=UNKNOWN;
			isExit=true;
			break;
		default:
			cout << "Unknown operation.Please, try again." << endl;
			break;
		}
	}
}
static int showList(void *data, int argc, char **argv, char **azColName){
	for(int i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

static int getCurrentBalance(void *data, int argc, char **argv, char **azColName){
	currentBalance=atoi(argv[0]);
	currentTransaction=atoi(argv[1]);
	return 0;
}
static int setSystemConfig(void *data, int argc, char **argv, char **azColName){
	totalTransaction=atoi(argv[0]);
	monthlyQuota=atoi(argv[1]);
	return 0;
}
void operatorActions(){
	sqlite3 *conn;
	char *zErrMsg;
	const char* data;
	int rc;
	int idClient=0;
	int idAccount=0;
	zErrMsg=0;
	
	data = "Callback function called\n";
	rc = sqlite3_open("12.db",&conn);
	if(rc){
		printf("Error");
		sqlite3_close(conn);
		return;
	}
	//system data
	rc = sqlite3_exec(conn,"select total_transactions, monthly_quota from system_config",setSystemConfig, (void*)data, &zErrMsg);
	if(rc){
		cout<<"Error!"<<endl;
		sqlite3_close(conn);
		return;
	}

	char bufferSelect[500];
	sprintf(bufferSelect,"select client_id,first_name, last_name from client");
	rc = sqlite3_exec(conn,bufferSelect,showList, (void*)data, &zErrMsg);
	if(rc){
		cout<<"Error!"<<endl;
		sqlite3_close(conn);
		return;
	}

	cout<<"Enter id of client"<<endl;
	scanf("%d",&idClient);

	char bufferAccount[500];
	sprintf(bufferAccount,"select account_id,balance, current_transactions from account where client_id='%d'",idClient);
	rc = sqlite3_exec(conn,bufferAccount,showList, (void*)data, &zErrMsg);
	if(rc){
		cout<<"Error!"<<endl;
		sqlite3_close(conn);
		return;
	}

	cout<<"Enter id of client`s account"<<endl;
	scanf("%d",&idAccount);

	char bufferGetBalance[500];
	sprintf(bufferGetBalance,"select balance, current_transactions from account where account_id='%d'",idAccount);
	rc = sqlite3_exec(conn,bufferGetBalance,getCurrentBalance, (void*)data, &zErrMsg);
	if(rc){
		cout<<"Error!"<<endl;
		sqlite3_close(conn);
		return;
	}

	int numberOfOperation=0;
	bool isExit=false;

	cout <<"Operations:" << endl;
	cout <<operator_add_money <<" "<< add_money << endl;
	cout <<operator_withdraw_money <<" "<< withdraw_money<< endl;
	cout <<operator_exit<<" "<< exit<<endl;
	scanf("%d",&numberOfOperation);
	int result = 0;
	switch(numberOfOperation){
	case operator_add_money:
		result = addMoney();
		break;
	case operator_withdraw_money:
		result = withdrawMoney();
		break;
	case operator_exit:
		cout<<"Good bye!"<<endl;
		roleIdentified=UNKNOWN;
		isExit=true;
		break;
	default:
		cout << "Unknown operation.Please, try again." << endl;
		break;
	}
	//------------
	if(!isExit && result == 0){
		char bufferUpdate[500];
		sprintf(bufferUpdate,"update account set balance='%d', current_transactions='%d' where account_id='%d'",balance, currentTransaction, idAccount);
		int rc = sqlite3_exec(conn,bufferUpdate,NULL,NULL,NULL);
		if(rc){
			printf("Error");
		}else{
			cout<<"Operation is executed successfully!"<<endl;
		}
	}
	sqlite3_close(conn);
	//--------------------
	
}


void main(){
	roleIdentified=OPERATOR;
	switch(roleIdentified){
	case(ADMIN):
		adminActions();
		break;
	case(OPERATOR):
		operatorActions();
		break;
	case(UNKNOWN):
		cout<<"ERROR!You are not registred in system!"<<endl;
		break;
	}
	system("pause");
	
	//bla
}
