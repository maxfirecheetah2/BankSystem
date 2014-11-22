#include "sqlite3.h"
#include <iostream>
#include <stdio.h>
#include <string>
using namespace std;

#define create_client "create client"
#define add_account "add account"
#define add_money "add money"
#define view_accounts "view accounts"
#define exit "exit"

#define admin_create_client 1
#define admin_add_account 2
#define admin_exit 3
#define operator_add_money 1
#define operator_exit 2

enum roleInSystem{UNKNOWN,ADMIN,OPERATOR};
roleInSystem roleIdentified;

void createClient(){}
void addAccount(){}
void addMoney(){}
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

void operatorActions(){
	bool isExit=false;
	int numberOfOperation=0;
	while(!isExit){
		cout <<"Operations:" << endl;
		cout <<operator_add_money <<" "<< add_money << endl;
		cout <<operator_exit<<" "<< exit<<endl;
		scanf("%d",&numberOfOperation);
		
		switch(numberOfOperation){
		case operator_add_money:
			addMoney();
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
	}
}


void main(){
	roleIdentified=ADMIN;
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
}