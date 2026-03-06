// MUHAMMAD USMAN
// DSALA-2025S-CS-48
// LAB 02
// Mini Database Engine
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;

class Column{
public:
    string name;
    string type;
    unsigned int constraints;
    
    Column(string n,string t,unsigned int c){
        name=n;
        type=t;
        constraints=c;
    }
};

class Row{
public:
    vector<string> values;
};

class Table{
public:
    string tableName;
    vector<Column> columns;
    vector<Row*> rows;
    
    Table(string name){
        tableName=name;
    }
    
    ~Table(){
        for(int i=0;i<rows.size();i++){
            delete rows[i];
        }
        rows.clear();
    }
    
    bool insertRow(vector<string> vals){
        if(vals.size()!=columns.size()){
            cout<<"Error: Wrong number of values"<<endl;
            return false;
        }
        
        for(int i=0;i<columns.size();i++){
            if((columns[i].constraints & 2) && vals[i].empty()){
                cout<<"Error: "<<columns[i].name<<" cannot be NULL"<<endl;
                return false;
            }
            
            if(columns[i].constraints & 1){
                if(vals[i].empty()){
                    cout<<"Error: Primary key cannot be NULL"<<endl;
                    return false;
                }
                for(int j=0;j<rows.size();j++){
                    if(rows[j]->values[i]==vals[i]){
                        cout<<"Error: Duplicate primary key"<<endl;
                        return false;
                    }
                }
            }
        }
        
        Row* newRow=new Row();
        newRow->values=vals;
        rows.push_back(newRow);
        return true;
    }
    
    void selectAll(){
        for(int i=0;i<columns.size();i++){
            cout<<columns[i].name<<"\t";
        }
        cout<<endl;
        
        for(int i=0;i<rows.size();i++){
            for(int j=0;j<rows[i]->values.size();j++){
                cout<<rows[i]->values[j]<<"\t";
            }
            cout<<endl;
        }
    }
    
    void saveToFile(string filename){
        ofstream file(filename);
        file<<"TABLE "<<tableName<<endl;
        
        for(int i=0;i<columns.size();i++){
            file<<columns[i].name<<" "<<columns[i].type<<" "<<columns[i].constraints<<endl;
        }
        
        file<<"DATA"<<endl;
        for(int i=0;i<rows.size();i++){
            for(int j=0;j<rows[i]->values.size();j++){
                file<<rows[i]->values[j];
                if(j<rows[i]->values.size()-1) file<<" ";
            }
            file<<endl;
        }
        file.close();
        cout<<"Saved to file"<<endl;
    }
    
    void loadFromFile(string filename){
        ifstream file(filename);
        if(!file.is_open()){
            cout<<"Error: Cannot open file"<<endl;
            return;
        }
        
        for(int i=0;i<rows.size();i++){
            delete rows[i];
        }
        rows.clear();
        columns.clear();
        
        char buffer[256];
        string line;
        
        file>>buffer>>tableName;
        file.ignore();
        
        while(getline(file,line)){
            if(line=="DATA") break;
            
            char name[64],type[32];
            unsigned int cons;
            sscanf(line.c_str(),"%s %s %u",name,type,&cons);
            columns.push_back(Column(name,type,cons));
        }
        
        while(getline(file,line)){
            if(line.empty()) continue;
            
            Row* newRow=new Row();
            char* token=strtok((char*)line.c_str()," ");
            while(token){
                newRow->values.push_back(token);
                token=strtok(NULL," ");
            }
            rows.push_back(newRow);
        }
        
        file.close();
        cout<<"Loaded from file"<<endl;
    }
};

int main(){
    vector<Table*> tables;
    Table* currentTable=NULL;
    
    cout<<"=================================="<<endl;
    cout<<"  Mini Database Engine"<<endl;
    cout<<"=================================="<<endl;
    cout<<"Commands:"<<endl;
    cout<<"  1. CREATE TABLE"<<endl;
    cout<<"  2. INSERT"<<endl;
    cout<<"  3. SELECT"<<endl;
    cout<<"  4. SAVE"<<endl;
    cout<<"  5. LOAD"<<endl;
    cout<<"  6. EXIT"<<endl;
    cout<<"=================================="<<endl;
    
    while(true){
        cout<<"\nEnter command (1-6): ";
        int choice;
        cin>>choice;
        cin.ignore();
        
        if(choice==1){
            string tableName;
            cout<<"Table name: ";
            cin>>tableName;
            
            Table* newTable=new Table(tableName);
            
            int numCols;
            cout<<"Number of columns: ";
            cin>>numCols;
            
            for(int i=0;i<numCols;i++){
                string name,type;
                int cons;
                
                cout<<"Column "<<(i+1)<<" name: ";
                cin>>name;
                cout<<"Column "<<(i+1)<<" type (int/string): ";
                cin>>type;
                cout<<"Constraints (0=None, 1=PK, 2=NotNull, 4=Unique): ";
                cin>>cons;
                
                newTable->columns.push_back(Column(name,type,cons));
            }
            
            tables.push_back(newTable);
            currentTable=newTable;
            cout<<"Table created!"<<endl;
        }
        else if(choice==2){
            if(!currentTable){
                cout<<"No table selected"<<endl;
                continue;
            }
            
            vector<string> values;
            cout<<"Enter "<<currentTable->columns.size()<<" values:"<<endl;
            
            for(int i=0;i<currentTable->columns.size();i++){
                string val;
                cout<<currentTable->columns[i].name<<": ";
                cin>>val;
                values.push_back(val);
            }
            
            if(currentTable->insertRow(values)){
                cout<<"Record inserted!"<<endl;
            }
        }
        else if(choice==3){
            if(!currentTable){
                cout<<"No table selected"<<endl;
                continue;
            }
            currentTable->selectAll();
        }
        else if(choice==4){
            if(!currentTable){
                cout<<"No table selected"<<endl;
                continue;
            }
            string filename;
            cout<<"Filename: ";
            cin>>filename;
            currentTable->saveToFile(filename);
        }
        else if(choice==5){
            string filename;
            cout<<"Filename: ";
            cin>>filename;
            
            Table* newTable=new Table("temp");
            newTable->loadFromFile(filename);
            tables.push_back(newTable);
            currentTable=newTable;
        }
        else if(choice==6){
            break;
        }
    }
    
    for(int i=0;i<tables.size();i++){
        delete tables[i];
    }
    
    return 0;
}
