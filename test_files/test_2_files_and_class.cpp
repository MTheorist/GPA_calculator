#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

class test{
	int number;
	char name[10];
	public:
		void enter_details(void);
		void add_details();	
		void show_details();	// MFReaper: function to show details saved in the file
}c1,c2;	// MFReaper: added another to show results

int main(){
	char word[30];
	//c1.enter_details();
	//cout<<"RETURNED";
	//c1.add_details();
	system("cls");	
	
	// MFReaper: This is only for text streams
	// MFReaper: Binary files u must specify the data itself. See new code
	/*
	db.open("test_case.dat", ios::in | ios::binary);
	db >> word;
	cout<<word;
	*/
	c2.show_details();
	
	
	return 0;
}

void test::enter_details(){
	cout<<"number: ";
	cin>>number;
	cout<<"Name: ";
	cin>>name;	
}

void test::add_details(){
	fstream db;
	db.open("test_case.dat", ios::out | ios::app | ios::binary);
	
	// MFReaper: Rewrote code. included indentation. No issues.
	if ( db ) {
		db.write( (char*)&c1, sizeof( test ) );
		cout << "File Opened";
	}
	db.close();
	return;
}

void test::show_details() {
	ifstream db;
	db.open("test_case.dat", ios::in | ios::binary);
	while( db.read( (char*)&c2, sizeof( test ) ) ) {	// MFReaper: reading the data from the file
		// MFReaper: the data is now saved in c2 automatically
		cout << "number: " << c2.number << endl;	
		cout << "name: " << c2.name << endl << endl;
	}
	return;
}
