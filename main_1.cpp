//complete code in line 395
#define n_sub 3										//number of subjects can be changed according to need without affecting the finctionality of the program
#include <iostream>
#include <fstream>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include "lexicon.h"

using namespace std;

float marks[n_sub], marks_1[n_sub];
const int n_settings = 2;								//number of settings (variables, passwords...etc.) stored in the config.txt file
int tm_fa = 0, tm_gpa = 0;
int g_pos_file = 0, p_pos_file = 0;						//used for seek[g/p] and tell[g/p] functions to maniplate file
char list[n_settings][10] = { "tm_fa" , "tm_gpa" };		//setting currently available in config.txt
char* title = "GPA CALCULATOR";							//title of the program
char* exit_statement = "//-----CLOSING PROGRAM-----//";
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
		int show_details( char* );	//extracts the data from the file and displays it
		char* getID();				//returns the ID assigned to the student
}w_det, r_det;						//w_det: write details TO file; r_det:read details FROM file

class std_marks{
	char ID[ID_len];	
	float perc[n_sub], gpa[n_sub], cgpa;
	public:
		std_details temp;
		std_marks();
		float calc_perc( int ); 	//calculates the percentage in each subject
		float calc_gpa( int ); 		//calculates the GPA in each subject
		float calc_cgpa(); 			//calculates the CGPA in each subject
		void add_marks();			//stores the entered marks into the .bin file
		int show_marks( char* );	//extracts the data from the file and displays it
		char* getID();
}w_marks, r_marks;					//w_marks: writes marks To file; r_marks: read marks FROM file

void check_config(); 				//checks the config file for any updates. 
void teacher( int );				//for when the user is a teacher
void student( void );				//for when the user is a student
void get_marks();					//input marks of the student in total "n_sub" subjects
void write_data();					//write the data of the human and also the marks
int read_data( char* , int );		//read the data of the human and also the marks
void wrong_choice( void );			//when user inputs a wrong choice [ PS: the code is very short but it was used multiple times ]
void exit_animation( void );		//super sexy way to slide ino DM's... JK animation to exit the program

//---------------MAIN PROGRAM---------------//
 
int main(){
	char category;
	COORD pos;
	check_config();
	char temp_title[5000];
	strcpy( temp_title, ascii_heading( title ) );
	rand_animation( temp_title, 't', 500 , 1);
	RETRY:
	pos.X = getX();
	pos.Y = getY();
	cout<<endl<<endl;
	cout<<"Enter Category:- \n";
	cout<< "\t1.Teacher \n\t2.Student \n:: ";
	category = getch();
	cout<<category<<endl;
	if ( category == '1' ){
		COORD pos_1;
		char choice;
		clean_slate( pos.X , pos.Y );
		cout<<endl<<endl;
		typeout( "//------TEACHER------//" , 't' , 1000);
		cout<<endl<<endl;
		RETRY_1:
		pos_1.X = getX();
		pos_1.Y = getY();
		cout<<"Enter the operation you would like to perform:- \n";
		cout<<"1.Enter new records \n2.View/Modify Previous Record \n3.Exit Program \n::";
		choice = getch();
		cout<<choice<<endl;
		if( choice == '1' ){
			clean_slate( pos_1.X , pos_1.Y );
			cout<<endl<<endl;
			teacher(1);
			exit_animation();	
		}
		else if( choice == '2' ){
			clean_slate( pos_1.X , pos_1.Y );
			cout<<endl<<endl;
			teacher(2);
			exit_animation();	
		}
		else if( choice == '3' ){
			exit_animation();
		}
		else{
			clean_slate( pos_1.X , pos_1.Y );
			wrong_choice();	
			goto RETRY_1;
		}	
	}
	else if( category == '2' ){
		COORD pos_1;
		char choice;
		clean_slate( pos.X , pos.Y );
		cout<<endl<<endl;
		typeout( "//------STUDENT------//" , 't' , 1000);
		cout<<endl<<endl;
		RETRY_2:
		pos_1.X = getX();
		pos_1.Y = getY();
		cout<<"Enter the operation you would like to perform:- \n";
		cout<<"1.View your Record \n2.Exit Program \n::";
		choice = getch();
		cout<<choice<<endl;
		if( choice == '1' ){
			clean_slate( pos_1.X , pos_1.Y );
		//	cout<<"__A__"<<endl;
			student();
		//	cout<<"__B__"<<endl;
			exit_animation();
		}
		else if( choice == '2' ){
			exit_animation();	
		}
		else{
			clean_slate( pos_1.X , pos_1.Y );
			wrong_choice();	
			goto RETRY_2;
		}
	}
	else{
		clean_slate( pos.X , pos.Y );
		wrong_choice();
		goto RETRY;	
	}
	
//	cout << endl;
//	cout <<"Test output 1"<<endl;
//	cout <<"Test output 2"<<endl;

//	cout <<"Test output 3"<<endl;
	return 0;	
}

//---------------MAIN PROGRAM ENDS HERE---------------//

//---------------CLASS FUNCTIONS---------------//

//class: std_details [student details]

char* std_details::getID() {
	return ID;
}

std_details::std_details(){
	for(int i=0 ; i<name_len ; i++ ){
		name[i] = '\0';	
	}
	for(int i=0 ; i<ID_len ; i++){
		ID[i] = '\0';	
	}
	sec = '\0';
	s_class = 0;
	return;
}

char* std_details::assign_ID(){
	// input: "Files/unique_id.txt"
	// dat: "Files/details.dat"
	int counter = 0;
	char id[ID_len];
	fstream fin_txt, fin_dat;
	fin_txt.open( "Files/unique_id.txt", ios::in );
	while( fin_txt >> id ) {
		if( !read_data( id, 0 ) ) {
			strcmp( ID, id );
			break;
		}
	}
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
//		cout<<"Copy successful."<<endl;
		p_pos_file = fout.tellp();
	}
	fout.close();
	return;
}

int std_details::show_details( char* id ) {
	if( strcmp( r_det.ID, id ) == 0 ) {
		cout<< "Name: " << r_det.name << endl;
		cout<< "ID: " << r_det.ID << endl;
		cout<< "Class: " << r_det.s_class << endl;
		cout<< "Section: " << r_det.sec << endl;
		return 1;
	}
//	cout<<g_pos_file<<endl;
	return 0;
}

//class: std_marks [student marks]

char* std_marks::getID() {
	return ID;
}

std_marks::std_marks(){
		for(int i=0 ; i< n_sub ; i++){
			perc[i] = 0;
			gpa[i] = 0;	
		}
		cgpa = 0;
		strcpy( ID , temp.assign_ID() );
		//cout<<"----------"<<ID<<"----------"<<endl; 
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
		fout.write( (char*)marks, sizeof( marks ) );
//		cout<<"Copy successful."<<endl;
	}
	fout.close();
	return;
}

 int std_marks::show_marks( char* id ) {
	if( strcmp( r_marks.ID , id ) == 0 ){
		cout<<"Marks:- \n";
		for( int i = 0; i < n_sub; i++ ) {
			marks_1[i] = (r_marks.perc[i] * tm_fa)/100;
			cout<<"\tSubject "<< (i+1) << ": " << marks_1[i] << endl;
		}
		cout<<"Percentage:- \n";
		for( int i = 0; i < n_sub; i++ ) {
			cout<<"\tSubject "<< (i+1) << ": " << r_marks.perc[i] << endl;
		}
		cout<<"GPA:- \n";
		for( int i = 0; i < n_sub; i++ ) {
			cout<<"\tSubject "<< (i+1) << ": " << r_marks.gpa[i] << endl;
		}
		cout <<"CGPA: " << r_marks.cgpa << endl;
		return 1;
	}
	return 0;
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

void teacher( int op ){
	if( op == 1 ){
		int n_std;
		cout<<"Enter the number of students you want to enter the details of: ";
		cin >> n_std;
		for( int i=0 ; i<n_std ; i++){
			w_det.enter_details();
			get_marks();
			write_data();
		}
	}
	if( op == 2 ){
		char id[ID_len];
		char choice;
		
		
		strcpy( id, "\0" );
		COORD pos;
		pos.X = getX();
		pos.Y = getY();
		cout<<"Enter the assigned ID of the student: ";
		cin>>id;
		while( !read_data( id, 1 ) ) {
			
			cout<<"The desired record either doesn't exist or the given ID is invalid.";
			cout<<"\nPress any key to continue.";
			getch();
			clean_slate( pos.X , pos.Y );
		}
		RETRY:
			pos.X = getX();
			pos.Y = getY();
		cout<<"Would you like to modify the data [y/n]: ";
		choice = getch();
		cout<<choice;
		if( islower( choice ) ){
			choice = toupper( choice );	
		}
		if ( choice == 'Y' || choice == 'N' ){
			
		}
		else{
			clean_slate( pos.X , pos.Y );
			wrong_choice();
			goto RETRY;	
		}
	}
	return;
}

void student(){
	char id[ID_len];
	strcpy( id, "\0" );
	COORD pos;
	pos.X = getX();
	pos.Y = getY();
	cout<<"Enter your ID: ";
	cin>>id;
	while( !read_data( id, 1 ) ) {
		cout<<"The desired record either doesn't exist or the given ID is invalid.";
		cout<<"\nPress any key to continue.";
		getch();
		clean_slate( pos.X , pos.Y );
	}
	
	return;
}

void get_marks(){
	char temp[20];
	cin.ignore();
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

void write_data() {
	w_det.add_details();
	w_marks.add_marks();
	//std_details();
	//std_marks();
	return;
}

int read_data( char* id, int show_data ) {
	fstream fin;
	fin.open( "Files/details.dat", ios::in | ios::binary );
	while( fin ) {
		fin.read( (char*)&r_det, sizeof( std_details ) );
		fin.read( (char*)&r_marks, sizeof( std_marks ) );
		fin.read( (char*)&marks, sizeof( marks ) );
		if( strcmp( id, r_det.getID() ) == 0 ) {
			if( show_data ) {
				r_det.show_details( id );
				r_marks.show_marks( id );
			}
			fin.close();
			return 1;
		}
	}
	fin.close();
	return 0;
}

void wrong_choice(){
	COORD pos;
	pos.X = getX();
	pos.Y = getY();
	cout<<endl<<endl;
	cout<<"Wrong input. Please enter a valid choice.";
	cout<<"\nPress any key to retry...";
	getch();
	clean_slate( pos.X , pos.Y );
	return;	
}

void exit_animation(){
	console_cursor(0);
	COORD pos;
	gotoXY( center_align( strlen( exit_statement ) ), 2, 1 );
	pos.X = getX();
	pos.Y = getY();
	typeout(exit_statement , 't' , 1000);
	Sleep(500);
	for(int i=0 ; i < 2 ; i++ ){
		clean_slate( pos.X , pos.Y );
		Sleep(100);
		cout<<exit_statement;
		if( i <= 1 ){
			Sleep(200);	
		}
	}
	exit(0);
}

//---------------PROGRAM FUNCTIONS END HERE---------------//
