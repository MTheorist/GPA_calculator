//add filestream EVERY-fucking-WHERE...
//check line 214
#define n_sub 3
#include <iostream>
#include <fstream>
#include <string.h>
#include <windows.h>
#include "lexicon.h"

using namespace std;

float marks[n_sub];
const int n_settings = 2;
int n_fa = 0, n_gpa = 0;
char list[n_settings][10] = { "n_fa" , "n_gpa" };

class std_details{
	string name;
	string ID;
	char sec;
	int s_class;
	public: 
		std_details(); 
		void get_details();	//function under construction...
		void ret_details(int identifier); //returns the data according to the given identifier
		void show_details();	// [MFReaper: #5]
}st_details;	// [MFReaper: #5]

class std_marks{	// removed inheritance [MFReaper: #5]
	float perc[n_sub], gpa[n_sub], cgpa;
	public:
		std_marks();
		float calc_perc(int i); //calculates the percentage in each subject
		float calc_gpa(int j); //calculates the GPA in each subject
		float calc_cgpa(); //calculates the CGPA in each subject
		void show_marks();	//[MFReaper: #5]		
}st_marks;	//[MFReaper: #5]

void check_config(); //check the config file for any updates. 
void get_marks();	//input marks of the student in total "n_sub" subjects
string assign_ID(); //assigns a unique roll number while keeping check on the used IDs
 
int main(){
	/*
	cout<<"Enter the no. of student  you want to enter the details of: ";
	cin x;
	cout<<endl;
	int x;
	*/
	check_config();
	
	// [MFReaper: #5]
	for( int i = 0; i < 1; i++ ) {
		cout << endl;
		st_details.get_details();
		get_marks();
		for( int i = 0; i < n_sub; i++ ) {
			st_marks.calc_perc( i );
			st_marks.calc_gpa( i );
		}
		st_marks.calc_cgpa();
		cout << endl;
	}
	
	cout << endl;
	st_details.show_details();
	// well...weird error check this out...show_details() ki last line run karti hai, but not the next line here
	// file streaming perfect tho
	cout << "B" << endl;
	st_marks.show_marks();
	cout << endl;
	
	//mk24.get_details(1);
	//get_marks();	
	
	/*
	for( int i = 0 ; i < n_sub ; i++ ){
		cout<<endl<<"PERC: "<<mk24.calc_perc(i);	
	}
	
	for( int i = 0 ; i < n_sub ; i++ ){
		cout<<endl<<"GPA:"<<mk24.calc_gpa(i);	
	}
	
	cout<<endl<<"CGPA: "<<mk24.calc_cgpa();	
	*/
	
	return 0;	
}

//Class Functions...

//class: student details

std_details::std_details(){
	name = "";
	sec = '\0';
	s_class = 0;	
}

void std_details::get_details( ){
	cout<<"Scholar ID: ";
	ID = assign_ID();
	cout<<ID<<endl;
	cout<<"First Name: ";
	cin>>name;
	cout<<"Section: ";
	cin>>sec;
	cin.ignore();
	cout<<"Class: ";
	cin>>s_class;
	
	// [MFReaper: #5]
	fstream fout;
	fout.open( "Files/details.dat", ios::out | ios::app | ios::binary );
	fout.write( (char*&)st_details, sizeof( std_details ) );
	fout.close();
	return;
}

//class: student marks
std_marks::std_marks(){
		int i=0;
		for( ; i< n_sub ; i++){
			perc[i] = 0;
			gpa[i] = 0;	
		}
		cgpa = 0;
}

float std_marks::calc_perc(int i){
		perc[i] = (marks[i]/n_fa)*100;
		return perc[i];
}

float std_marks::calc_gpa(int j){
		gpa[j] = (perc[j]/100)*n_gpa;
		return gpa[j];
}

float std_marks::calc_cgpa(){
		int k = 0;
		for( ; k < n_sub ; k++){
			cgpa += gpa[k];	
		}
		cgpa /= 3;
		return cgpa;
}

//Class Functions END HERE

//Program Functions...

void check_config(){		
	char word[30];
	string range;
	fstream cfg;
	cfg.open("Files/config.txt", ios::in);
	/*if(cfg.is_open()){
		cout<<"OPEN..."<<endl;	
	}*/
	while( cfg >> word ){
		if(  ( ( word[0] == '/' ) && ( word[1] == '/' ) ) || strlen( word ) ==0 ){
			continue;	
		}
		for( int i=0 ; i < n_settings ; i++ ){
		
			if( strcmp( word , list[i] )==0 ){
				//cout<<"A";	
				for( int j=0 ; j < 2 ; j++ ){
					cfg >> word;
				}
				for( int j=0 ; j < strlen(word) ; j++ ){
					if( word[j] == '[' ){

						for( int k=( j+1 ) ; k < strlen(word) ; ){
							while ( word[k] != ']' ){
								range += word[k];
								k++;
							}
							break;
						}
					}
					else{
						continue;	
					}
				}
				char*marks = &range[0];
				switch(i){
					case 0:
						n_fa = conv_to_int( marks );
						break;
						
					case 1:
						n_gpa = conv_to_int( marks );
						break;
				}
			}
		}
		range = "\0";
	}
	cfg.close();
	//cout<<n_fa<<" : "<<n_gpa;
}

void get_marks(){
	cout<<"Enter marks:- \n";
	for( int i = 0 ; i < n_sub ; i++ ){
		cin>>marks[i];
		if( ( marks[i] <= n_fa ) && ( marks[i] >= 0 ) ){
			continue;
		}
		else{
			cout<<"Marks lie out of range. \nRange: 0-"<<n_fa;
			getch();
			cout<<"\r";
			gotoXY( 0 , -2 , 1 );
			cout<<"       \n                              \n                \r";
			gotoXY( 0 , -2 , 1 );
			marks[i] = 0;
			i--;
		}
	}
	
	// [MFReaper: #5]
	for( int i = 0; i < n_sub; i++ ) {
		st_marks.calc_perc( i );
		st_marks.calc_gpa( i );
	}
	st_marks.calc_cgpa();
	fstream fout;
	fout.open( "Files/Marks.dat", ios::out | ios::app | ios::binary );
	fout.write( (char*)&st_marks, sizeof( std_marks ) );
	fout.close();
	return;
}

string assign_ID(){
	string line, line_1;
	ifstream fin, fin_1;
	ofstream fout_1;
	fin.open("Files/unique_id.txt");
	fin_1.open("Files/used_unique_id.txt");
	while(!fin.eof()){	
		fin >> line;
		fin_1 >> line_1;
		char* word = &line[0];
		char* word_1 = &line_1[0];
		if( strcmp( word , word_1 )==0 ){			
			//compares the existing text file with a list of all the unique student id
			//if the id is already in use, it assigns a different id
			//if bool holds true: return value= 0
		}
		else{
			fin.close();
			fin_1.close();
			//include the following code elsewhere...
			/*
			fout_1.open("Files/used_unique_id.txt", ios::app);
			fout_1<<line<<"\n";	
			fout_1.close();
			*/ 
			return line;
		}	
	}
}

// [MFReaper: #5]

void std_details::show_details() {
	std_details test;
	fstream fin;
	fin.open( "Files/details.dat", ios::in | ios::binary );
	while( fin.read( (char*)&test, sizeof( std_details ) ) ) {
		cout << endl << name << endl << ID << endl << sec << endl << s_class << endl;
	}
	fin.close();
	cout << "A" << endl;
	return;
}

void std_marks::show_marks() {
	cout << "C" << endl;
	std_marks test2;
	fstream fin;
	fin.open( "Files/Marks.dat", ios::in | ios::binary );
	while( fin.read( (char*)&test2, sizeof( std_marks ) ) ) {
		for( int i = 0; i < n_sub; i++ ) {
			cout << endl << endl << i << endl << perc[i] << gpa[i] << endl;
		}
		cout << cgpa;
	}
	fin.close();
	return;
}
