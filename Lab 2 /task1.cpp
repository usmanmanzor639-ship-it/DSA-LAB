// MUHAMMAD USMAN
// DSALA-2025S-CS-48
// LAB 02
// Secure File-Based Banking System
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
using namespace std;

class Account{
protected:
    int id;
    string name;
    double balance;
    unsigned int perms;
    vector<double> trans;
public:
    Account(int i,string n,double b,unsigned int p){
        id=i;
        name=n;
        balance=b;
        perms=p;
    }
    virtual ~Account(){}
    virtual string getType()=0;
    int getId(){return id;}
    double getBalance(){return balance;}
    bool canWithdraw(){return (perms & 1)!=0;}
    bool canDeposit(){return (perms & 2)!=0;}
    bool canTransfer(){return (perms & 4)!=0;}
    virtual void deposit(double amt){
        if(!canDeposit()){
            cout<<"No deposit permission"<<endl;
            return;
        }
        balance+=amt;
        trans.push_back(amt);
        cout<<"Deposited: "<<amt<<" Balance: "<<balance<<endl;
    }
    virtual void withdraw(double amt){
        if(!canWithdraw()){
            cout<<"No withdraw permission"<<endl;
            return;
        }
        if(amt>balance){
            cout<<"Insufficient balance"<<endl;
            return;
        }
        balance-=amt;
        trans.push_back(-amt);
        cout<<"Withdrawn: "<<amt<<" Balance: "<<balance<<endl;
    }
    void show(){
        cout<<"Type: "<<getType()<<" ID: "<<id<<" Name: "<<name<<" Balance: "<<balance<<endl;
    }
    virtual void saveToFile(ofstream& f){
        f<<"ACCOUNT "<<getType()<<endl;
        f<<id<<" "<<name<<" "<<balance<<" "<<perms<<endl;
        f<<"TRANSACTIONS"<<endl;
        for(int i=0;i<(int)trans.size();i++){
            f<<trans[i]<<endl;
        }
    }
    virtual void loadFromFile(ifstream& f){
        string line;
        getline(f,line);
        if(line=="TRANSACTIONS"){
            while(getline(f,line)){
                if(line.empty()) break;
                if(line.find("ACCOUNT")==0) break;
                trans.push_back(atof(line.c_str()));
            }
        }
    }
    void monthly(double m[12]){
        for(int i=0;i<12;i++) m[i]=0;
        int month=0;
        for(int i=0;i<(int)trans.size();i++){
            if(i%10==0 && i>0) month++;
            if(month>=12) month=11;
            m[month]+=trans[i];
        }
    }
};

class SavingsAccount:public Account{
public:
    SavingsAccount(int i,string n,double b,unsigned int p):Account(i,n,b,p){}
    string getType(){return "Savings";}
};

class CurrentAccount:public Account{
public:
    CurrentAccount(int i,string n,double b,unsigned int p):Account(i,n,b,p){}
    string getType(){return "Current";}
};

int main(){
    vector<Account*> accounts;
    cout<<"============================="<<endl;
    cout<<"   Banking System"<<endl;
    cout<<"============================="<<endl;
    while(true){
        cout<<endl;
        cout<<"1. Create Account"<<endl;
        cout<<"2. Deposit"<<endl;
        cout<<"3. Withdraw"<<endl;
        cout<<"4. Show Accounts"<<endl;
        cout<<"5. Save to File"<<endl;
        cout<<"6. Load from File"<<endl;
        cout<<"7. Exit"<<endl;
        cout<<endl;
        cout<<"Choose option: ";
        int opt;
        cin>>opt;
        cin.ignore();
        cout<<endl;
        if(opt==1){
            int id,type,perm;
            string name;
            double bal;
            cout<<"Enter Account ID: ";
            cin>>id;
            cin.ignore();
            cout<<"Enter Name: ";
            getline(cin,name);
            cout<<"Enter Initial Balance: ";
            cin>>bal;
            cout<<"Account Type (1=Savings, 2=Current): ";
            cin>>type;
            cout<<"Permissions (1=Withdraw, 2=Deposit, 4=Transfer): ";
            cin>>perm;
            if(type==1){
                accounts.push_back(new SavingsAccount(id,name,bal,perm));
            }else{
                accounts.push_back(new CurrentAccount(id,name,bal,perm));
            }
            cout<<endl<<"Account created successfully!"<<endl;
        }
        else if(opt==2){
            int id;
            double amt;
            cout<<"Enter Account ID: ";
            cin>>id;
            cout<<"Enter Amount: ";
            cin>>amt;
            bool found=false;
            for(int i=0;i<(int)accounts.size();i++){
                if(accounts[i]->getId()==id){
                    accounts[i]->deposit(amt);
                    found=true;
                    break;
                }
            }
            if(!found){
                cout<<"Account not found!"<<endl;
            }
        }
        else if(opt==3){
            int id;
            double amt;
            cout<<"Enter Account ID: ";
            cin>>id;
            cout<<"Enter Amount: ";
            cin>>amt;
            bool found=false;
            for(int i=0;i<(int)accounts.size();i++){
                if(accounts[i]->getId()==id){
                    accounts[i]->withdraw(amt);
                    found=true;
                    break;
                }
            }
            if(!found){
                cout<<"Account not found!"<<endl;
            }
        }
        else if(opt==4){
            if(accounts.size()==0){
                cout<<"No accounts found!"<<endl;
            }else{
                cout<<"All Accounts:"<<endl;
                cout<<"============================="<<endl;
                for(int i=0;i<(int)accounts.size();i++){
                    accounts[i]->show();
                }
                cout<<"============================="<<endl;
            }
        }
        else if(opt==5){
            string file;
            cout<<"Enter filename: ";
            cin>>file;
            ofstream f(file);
            for(int i=0;i<(int)accounts.size();i++){
                accounts[i]->saveToFile(f);
            }
            f.close();
            cout<<"File saved successfully!"<<endl;
        }
        else if(opt==6){
            string file;
            cout<<"Enter filename: ";
            cin>>file;
            ifstream f(file);
            if(!f.is_open()){
                cout<<"File not found!"<<endl;
                continue;
            }
            for(int i=0;i<(int)accounts.size();i++){
                delete accounts[i];
            }
            accounts.clear();
            string line;
            while(getline(f,line)){
                if(line.find("ACCOUNT")==0){
                    char buf[256];
                    strcpy(buf,line.c_str());
                    char* tok=strtok(buf," ");
                    tok=strtok(NULL," ");
                    string type=tok;
                    int id;
                    char name[100];
                    double bal;
                    unsigned int perm;
                    f>>id>>name>>bal>>perm;
                    f.ignore();
                    Account* acc=NULL;
                    if(type=="Savings"){
                        acc=new SavingsAccount(id,name,bal,perm);
                    }else if(type=="Current"){
                        acc=new CurrentAccount(id,name,bal,perm);
                    }
                    if(acc!=NULL){
                        acc->loadFromFile(f);
                        accounts.push_back(acc);
                    }
                }
            }
            f.close();
            cout<<"File loaded successfully! Total accounts: "<<accounts.size()<<endl;
        }
        else if(opt==7){
            cout<<"Exiting... Thank you!"<<endl;
            break;
        }
        else{
            cout<<"Invalid option! Please choose 1-7"<<endl;
        }
    }
    for(int i=0;i<(int)accounts.size();i++){
        delete accounts[i];
    }
    return 0;
}
