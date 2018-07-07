#define n_sub 3
#include <iostream>
#include <fstream>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "lexicon.h"

using namespace std;

float marks[n_sub];
const int n_settings = 2;								//number of settings (variables, passwords...etc.) stored in the config.txt file
int tm_fa = 0, tm_gpa = 0;
int g_pos_file = 0, p_pos_file = 0;						//used for seek[g/p] and tell[g/p] functions to maniplate file
char list[n_settings][10] = { "tm_fa" , "tm_gpa" };		//setting currently available in config.txt
const int name_len = 30;
const int ID_len = 10;

class std_details{
	char name[name_len];
	char ID[ID_len];
	int s_class;
	char sec;
	public: 
		std_details();
		char* assign_ID(); 			//assigns a unique roll number while keeping check on the used IDs 
		void enter_details();		//function under construction...
		void add_details();			//stores the entered details into the .bin file
		void show_details(char*);	//extracts the data from the file and displays it
}w_det, r_det;						//w_det: write details TO file; r_det:read details FROM file

class std_marks{	
	float perc[n_sub], gpa[n_sub], cgpa;
	public:
		std_marks();
		float calc_perc(int i); 	//calculates the percentage in each subject
		float calc_gpa(int j); 		//calculates the GPA in each subject
		float calc_cgpa(); 			//calculates the CGPA in each subject
		void add_marks();			//stores the entered marks into the .bin file
		void show_marks();			//extracts the data from the file and displays it
}w_marks, r_marks;					//w_marks: writes marks To file; r_marks: read marks FROM file

void check_config(); 				//checks the config file for any updates. 
void get_marks();					//input marks of the student in total "n_sub" subjects

//---------------MAIN PROGRAM---------------//
 
int main(){
	check_config();
//	w_det.enter_details();
	cout << endl;
//	w_det.add_details();
//	r_det.show_details( "A1" );
//	cout <<"Test output 1"<<endl;
//	r_marks.show_marks();
	get_marks();
//	cout <<"Test output 2"<<endl;	
//	w_marks.add_marks();
//	cout <<"Test output 3"<<endl;
	return 0;	
}

//---------------CLASS FUNCTIONS---------------//

//class: std_details [student details]

std_details::std_details(){
	for(int i=0 ; i<name_len ; i++ ){
		name[i] = '\0';	
	}
	for(int i=0 ; i<ID_len ; i++){
		ID[i] = '\0';	
	}
	sec = '\0';
	s_class = 0;	
}

char* std_details::assign_ID(){
	// input: "Files/unique_id.txt"
	// dat: "Files/details.dat"
	char id[ID_len];
	fstream fin_txt, fin_dat;
	fin_txt.open( "Files/unique_id.txt", ios::in );
	fin_dat.open( "Files/details.dat", ios::in | ios::binary );
	do {
		fin_txt >> id;
	}while( fin_dat.read( (char*)&r_det, sizeof( std_details ) ) );
//	cout << id << endl;
	fin_dat.close();
	fin_txt.close();
	return id;
}

void std_details::enter_details( ){
	cout<<"Scholar ID: ";
	strcpy( ID, assign_ID() );
	cout<<ID<<endl;
	cout<<"First Name: ";
	cin>>name;
	cout<<"Class: ";
	cin>>s_class;
	cout<<"Section: ";
	cin>>sec;
	return;
}

void std_details::add_details(){
	fstream fout;
	fout.open( "Files/details.dat", ios::out | ios::app | ios::binary );
	
	if( fout.is_open() ){
		fout.write( (char*)&w_det, sizeof( std_details ) );
		cout<<"Copy successful."<<endl;
		p_pos_file = fout.tellp();
	}
	fout.close();
	return;
}

void std_details::show_details( char* id ) {
	ifstream fin;
	fin.open( "Files/details.dat", ios::in | ios::binary );
	//fin.seekg(0);
	
	
	while( fin.read( (char*)&r_det, sizeof( std_details ) ) ) {
		/*
		cout<< "Name: " << r_det.name << endl;
		cout<< "ID: " << r_det.ID << endl;
		cout<< "Class: " << r_det.s_class << endl;
		cout<< "Section: " << r_det.sec << endl;
		g_pos_file = fin.tellg();
		*/
		if( strcmp( r_det.ID, id ) == 0 ) {
			cout<< "Name: " << r_det.name << endl;
			cout<< "ID: " << r_det.ID << endl;
			cout<< "Class: " << r_det.s_class << endl;
			cout<< "Section: " << r_det.sec << endl;
			g_pos_file = fin.tellg();
			break;
		}
	}
	cout<<g_pos_file<<endl;
	fin.close();
	return;
}

//class: std_marks [student marks]

std_marks::std_marks(){
		int i=0;
		for( ; i< n_sub ; i++){
			perc[i] = 0;
			gpa[i] = 0;	
		}
		cgpa = 0;
}

float std_marks::calc_perc(int i){
		perc[i] = (marks[i]/tm_fa)*100;
		return perc[i];
}

float std_marks::calc_gpa(int j){
		gpa[j] = (perc[j]/100)*tm_gpa;
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

void std_marks::add_marks(){
	fstream fout;
	fout.open( "Files/details.dat", ios::out | ios::app | ios::binary );
	fout.seekp( p_pos_file , ios_base::beg );
	if( fout.is_open() ){
		fout.write( (char*)&w_marks, sizeof( std_marks ) );
		cout<<"Copy successful."<<endl;
	}
	fout.close();
	return;
}

void std_marks::show_marks() {
	ifstream fin;
	fin.open( "Files/details.dat", ios::in | ios::binary );
	fin.seekg(g_pos_file, ios_base::beg);
	while( fin.read( (char*)&r_marks, sizeof( std_marks ) ) ) {
		cout<<"Percentage:- \n";
		for( int i = 0; i < n_sub; i++ ) {
			cout<<"\tSubject "<< (i+1) << ": " << r_marks.perc[i] << endl;
		}
		cout<<"GPA:- \n";
		for( int i = 0; i < n_sub; i++ ) {
			cout<<"\tSubject "<< (i+1) << ": " << r_marks.gpa[i] << endl;
		}
		cout <<"CGPA: " << r_marks.cgpa << endl;
	}
	fin.close();
	return;
}

//---------------CLASS FUNCTIONS END HERE---------------//

//---------------PROGRAM FUNCTIONS---------------//

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
						tm_fa = conv_to_int( marks );
						break;
						
					case 1:
						tm_gpa = conv_to_int( marks );
						break;
				}
			}
		}
		range = "\0";
	}
	cfg.close();
	//cout<<tm_fa<<" : "<<tm_gpa;
}

void get_marks(){
	char temp[20];
//	cin.ignore();
	cout<<"Enter marks:- \n";
	for( int i = 0 ; i < n_sub ; i++ ){
		COORD pos;
		cout<<"\tSubject "<< (i+1) << ": ";
		pos.X = getX();
		pos.Y = getY();
		cin.getline( temp, 20 );
		// work with getline
		for( int j = 0; j < strlen(temp); j++ ) {
			if( isalpha( temp[i] ) ) {
				cout << "\tInvalid Marks\n\tPress any key to retry\n";
				getch();
				clean_slate( pos.X, pos.Y );
				marks[i] = 0;
				cin.getline( temp , 20 );
				j = 0;
			}
			marks[i] = conv_to_int( temp );
			while( !( ( marks[i] <= tm_fa ) && ( marks[i] >= 0 ) ) ) {
				cout << "\tInvalid Marks\n\tPress any key to retry\n";
				getch();
				clean_slate( pos.X, pos.Y );
				marks[i] = 0;
				cin.getline( temp , 20 );
				j = 0;
				//cin >> marks[i];
			}
		}
	}
	
	/*for( int i = 0 ; i < n_sub ; i++ ){
		cout<<endl<<"PERC: "<<w_marks.calc_perc(i);	
	}
	
	for( int i = 0 ; i < n_sub ; i++ ){
		cout<<endl<<"GPA:"<<w_marks.calc_gpa(i);	
	}
	cout<<endl<<"CGPA: "<<w_marks.calc_cgpa();*/
	cout<<"Percentage:- \n";
		for( int i = 0; i < n_sub; i++ ) {
			cout<<"\tSubject "<< (i+1) << ": " << w_marks.calc_perc(i) << endl;
		}
		cout<<"GPA:- \n";
		for( int i = 0; i < n_sub; i++ ) {
			cout<<"\tSubject "<< (i+1) << ": " << w_marks.calc_gpa(i) << endl;	
		}
		cout <<"CGPA: " << w_marks.calc_cgpa() << endl;
	/*fstream fout;
	fout.open( "Files/Marks.bin", ios::out | ios::app | ios::binary );
	fout.write( (char*)&st_marks, sizeof( std_marks ) );
	fout.close();*/
	return;
}

//---------------PROGRAM FUNCTIONS END HERE---------------//
