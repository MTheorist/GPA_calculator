//add filestream EVERY-fucking-WHERE...
//check line 214
//http://www.cplusplus.com/forum/beginner/76800/
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
	string ID;
	string name;
	int s_class;
	char sec;
	public:
		static int n_var;
		std_details(); 
		void get_details(void);		//function under construction...
		void save_details(void);	//streams all the user input data into the database file
		void show_details(void);	//outputs the data saved in the file
} sr_det, sw_det;

class std_marks{
	float perc[n_sub], gpa[n_sub], cgpa;
	public:
		static int n_var;
		std_marks();
		float calc_perc(int i);	//calculates the percentage in each subject
		float calc_gpa(int j);	//calculates the GPA in each subject
		float calc_cgpa(void);	//calculates the CGPA in each subject
		void save_marks(void);	//streams all the user input data into the database file
		void show_marks(void);	//outputs the data saved in the file		
} sr_mks, sw_mks;

void check_config(void);	//check the config file for any updates. 
void get_marks(void);		//input marks of the student in total "n_sub" subjects
string assign_ID();			//assigns a unique roll number while keeping check on the used IDs

//---------------MAIN PROGRAM---------------//
 
int main(){
	/*
	cout<<"Enter the no. of student  you want to enter the details of: ";
	cin x;
	cout<<endl;
	int x;
	*/
	check_config();
	sw_det.get_details();
	get_marks();	
	
	for( int i = 0 ; i < n_sub ; i++ ){
		cout<<endl<<"PERC: "<<sw_mks.calc_perc(i);	
	}
	
	for( int i = 0 ; i < n_sub ; i++ ){
		cout<<endl<<"GPA:"<<sw_mks.calc_gpa(i);	
	}
	
	cout<<endl<<"CGPA: "<<sw_mks.calc_cgpa();	
	
	sw_det.save_details();
	sw_mks.save_marks();
//	getch();
//	system("cls");
//	sr_det.show_details();
//	sr_mks.show_marks();
	
	return 0;	
}

//---------------CLASS FUNCTIONS---------------//

//class: std_details

int std_details::n_var = 4;

std_details::std_details(){
	name = "";
	sec = '\0';
	s_class = 0;	
}

void std_details::get_details(){
	cout<<"Scholar ID: ";
	ID = assign_ID();
	cout<<ID<<endl;
	cout<<"First Name: ";
	cin>>name;
	cout<<"Class: ";
	cin>>s_class;
	cout<<"Section: ";
	cin>>sec;
	return;
}

void std_details::save_details(){
	ofstream db;
	db.open("Files/data.txt", ios::out | ios::app);
	db << sw_det.ID << "\t|\t";
	db << name << "\t|\t";
	db << s_class << "\t|\t";
	db << sec <<"\t|\t";
	db.close();
	return;	
}

//class: std_marks

int std_marks::n_var = 3;

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

void std_marks::save_marks(){
	ofstream db;
	db.open("Files/data.txt", ios::out | ios::app);
	for(int i=0 ; i < n_sub ; i++){
		db << marks[i] << " ";
	}
	db << "\t|\t";
	for(int i=0 ; i < n_sub ; i++){
		db << perc[i] << " ";
	}
	db << "\t|\t";
	for(int i=0 ; i < n_sub ; i++){
		db << gpa[i] << " ";
	}
	db << "\t|\t";
	db << cgpa << "\n";
	db.close();
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
		cout<< "\tSubject " << ( i+1 ) << ": ";
		cin>>marks[i];
		if( ( marks[i] <= n_fa ) && ( marks[i] >= 0 ) ){
			continue;
		}
		else{
			cout<<"\tMarks lie out of range. \n\tRange: 0-"<<n_fa;
			getch();
			cout<<"\r";
			gotoXY( 0 , -2 , 1 );
			cout<<"             \n                                      \n                        \r";
			gotoXY( 0 , -2 , 1 );
			marks[i] = 0;
			i--;
		}
	}	
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
