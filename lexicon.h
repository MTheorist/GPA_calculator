#ifndef LEXICON_H	// include guard (to ensure that robot.h is not declared twice
#define LEXICON_H

#include <iostream>
#include <conio.h>	
#include <windows.h>
#include <math.h>
using namespace std;

// Constants //

#define MAX_SIZE 5000

// Special Keys Defined //

#define KEY_BKSP 8;
#define KEY_TAB 9;
#define KEY_ENTER 13;
#define KEY_ESC 27;
#define KEY_SPC 32;
#define KEY_DEL 127;
#define KEY_UP 72;
#define KEY_DOWN 80;
#define KEY_LEFT 75;
#define KEY_RIGHT 77;


// CLASSES //


class WINDOWS {
	private:
		HANDLE console;
		CONSOLE_SCREEN_BUFFER_INFO screen;
		CONSOLE_CURSOR_INFO cursor;
		COORD pos;
		
		void LoadData();
	public:
		WINDOWS();
		int CursorStatus();
		void CursorStatus( int );
		int ScreenLen();
		int getX();
		int getY();
		void gotoXY( int, int, int );
		void CleanSlate( int, int );
}CONSOLE;


class qStr {
	private:
		// members
		char dat[MAX_SIZE];
		
		// functions
		
		void TestCheck();
		void ClearString( char* );
		void AppendString( char*, char* );
		int StringLen( char* );
		void StringCopy( char*, char* );
		bool StringCompare( char*, char* );
		char* CutString( char*, int, int );
		int CenterAlignLen( int, int );
		
		qStr &CreateParagraph( void );
		
	public:
		// members
		class PC {
			private:
				// members
				
				
				// functions
				
				
			public:
				// members
				bool Switch;
				int margin;
				int max_line_len;
				bool center_align;
				
				// functions
				PC();
				
		};
		PC para;
		
		/*
		class AC {
			private:
				// members
				
				
				// functions
				
				
			public:
				// members
				bool Switch;
				int margin;
				int max_line_len;
				bool center_align;
				
				// functions
				AC();
				
		};
		AC animation;
		*/
		
		/*
		class EC {
			private:
				// members
				
				
				// functions
				
				
			public:
				// members
				bool Switch;
				int margin;
				int max_line_len;
				bool center_align;
				
				// functions
				EC();
				
		};
		EC encryption;
		*/
		
		// functions
		qStr();
		
		friend ostream &operator<<( ostream&, qStr& );
		friend istream &operator>>( istream&, qStr& );
		
		qStr &operator+( qStr& );
		qStr &operator+( char* );
		void operator=( char* );
		void operator=( qStr& );
		bool operator==( qStr& );
		bool operator==( char* );
		bool operator!=( qStr& );
		bool operator!=( char* );
		qStr &operator()( int, int );
		char &operator[]( int );
		
		char* ReturnAsStringArray( void );
		int Len( void );
		
};


// FUNCTIONs/OPERATORs PROTOTYPES //

char* operator+( char*, qStr& );
bool operator==( char*, qStr& );
bool operator!=( char*, qStr& );


// WINDOWS Class Functions //	// the same old one, or new depending when u last used this file. ( no changes towards 3.0.0 changes )

void WINDOWS::LoadData() {
	console = GetStdHandle( STD_OUTPUT_HANDLE );
	GetConsoleScreenBufferInfo( console, &screen );
	GetConsoleCursorInfo( console, &cursor );
	return;
}


WINDOWS::WINDOWS() {
	LoadData();
	pos.X = 0;
	pos.Y = 0;
}

int WINDOWS::CursorStatus() {
	LoadData();
	return (int)cursor.bVisible;
}

void WINDOWS::CursorStatus( int SetTo ) {
	LoadData();
	cursor.bVisible = SetTo;
	SetConsoleCursorInfo( console, &cursor );
	return;
}


int WINDOWS::ScreenLen() {
	LoadData();
	return screen.dwMaximumWindowSize.X;
}


int WINDOWS::getX() {
	LoadData();
	return screen.dwCursorPosition.X;
}


int WINDOWS::getY() {
	LoadData();
	return screen.dwCursorPosition.Y;
}


void WINDOWS::gotoXY( int X_coord, int Y_coord, int relative_movement = 0 ) {
	LoadData();
	COORD pos_temp;
	if( relative_movement ) {
		pos_temp.X = getX() + X_coord;
		pos_temp.Y = getY() + Y_coord;
	}
	else {
		pos_temp.X = X_coord;
		pos_temp.Y = Y_coord;
	}
	SetConsoleCursorPosition( console, pos_temp );
	return;
}


void WINDOWS::CleanSlate( int X_coord, int Y_coord ) {
	LoadData();
	
	int counter = 0;
	COORD pos_orig;
	pos_orig.X = getX();
	pos_orig.Y = getY();
	
	pos.X = X_coord;
	pos.Y = Y_coord;
	
	if( pos.Y == pos_orig.Y ) {
		pos.X = X_coord;
		while( pos.X != pos_orig.X ) {
			counter++;
			pos.X++;
		}
	}
	else {
		for( int i = X_coord; i < ScreenLen(); i++ ) {
			counter++;
		}
	}
	
	pos.X = 0;
	pos.Y += 1;
	
	while( pos_orig.Y - pos.Y  >= 1 ) {
		for( int i = 0; i < ScreenLen(); i++ ) {
			counter++;
		}
		pos.Y++;
	}
	
	while( pos.X != pos_orig.X ) {
		counter++;
		pos.X++;
	}
	
	char output_clear[counter];
	
	for( int i = 0; i < ( sizeof( output_clear ) / sizeof( char ) ); i++ ) {
		output_clear[i] = ' ';
	}
	
	gotoXY( X_coord, Y_coord, 0 );
	
	cout.write( output_clear, sizeof( output_clear ) );
	
	gotoXY( X_coord, Y_coord, 0 );

	return;
}

// <-----WINDOWS CLASS------>	//



// qStr Class Functions //

// Constructor

qStr::qStr() {
	ClearString( dat );
	return;
}

void	// setting value of object string
qStr::operator=( qStr &object ) {
	*this = object.ReturnAsStringArray();
	return;
}

void	// giving value of object string to another
qStr::operator=( char* input ) {
	this->ClearString( this->dat );
	this->StringCopy( this->dat, input );
	return;
}

qStr&	// adding string data from object 1 and object 2
qStr::operator+( qStr &object ) {
	char temp[MAX_SIZE];
	ClearString( temp );
	StringCopy( temp, this->dat );
	AppendString( temp, object.ReturnAsStringArray() );
	qStr ob;	ob = temp;
	return ob;
}

qStr&	// adding string data from an object and a string
qStr::operator+( char* input) {
	qStr temp[3];
	temp[0] = this->dat;
	temp[1] = input;
	temp[2] = temp[0] + temp[1];
	return temp[2];
}

bool	// comparing string data of two objects
qStr::operator==( qStr &object ) {
	if( this->StringCompare( this->dat, object.ReturnAsStringArray() ) )	return true;
	else	return false;
}

bool	// comparing string data in an object and another string
qStr::operator==( char* string ) {
	if( this->StringCompare( this->dat, string ) )	return true;
	else	return false;
}

bool	// comparing for inequality of string data of two objects
qStr::operator!=( qStr &object ) {
	if( *this == object )	return false;
	else	return true;
}

bool	// comparing for inequality of string data in an object and another string
qStr::operator!=( char* string ) {
	if( *this == string )	return false;
	else	return true;
}

qStr	// cutting the string data
&qStr::operator()( int pos_beg, int pos_end ) {
	qStr temp;
	temp = CutString( this->dat, pos_beg, pos_end );
	return temp;
}

char // returning the character stored, like an array
&qStr::operator[]( int index ) {
	return this->dat[index];
}

void	// function to see data stored in object string
qStr::TestCheck() {
	cout.write( dat, StringLen( dat ) );	cout << "//";	exit( 0 );
	return;
}

int	// string length
qStr::StringLen( char* string ) {
	int ctr = 0;
	while( !( ( string[ctr] == '\0' ) || ( ctr == MAX_SIZE ) ) )	ctr++;
	if( ctr == MAX_SIZE )	return -1;
	else	return ctr;
}

int // length of string data memeber
qStr::Len( void ) {
	return StringLen( this->dat );
}

void	// removing all previously stored data in a string
qStr::ClearString( char* string ) {
	for( int i = 0; i < MAX_SIZE; i++ )	string[i] = '\0';
	return;
}

void	// adding a string behind another string
qStr::AppendString( char* source_string, char* string_to_add ) {
	int s_orig = StringLen( source_string );
	int s_sec = StringLen( string_to_add );
	
	for( int i = s_orig; i <= ( s_orig + s_sec ); i++ )	source_string[i] = string_to_add[i - s_orig];
	
	return;
}

void	// copying a string into another array
qStr::StringCopy( char* destination, char* source_string ) {
	ClearString( destination );
	
	int len = StringLen( source_string );
	for( int i = 0; i <= len; i++ )	destination[i] = source_string[i];
	
	return;
}

char*	// return the string saved in object
qStr::ReturnAsStringArray( void ) {
	return this->dat;
}

bool	// check if the strings hold the same values
qStr::StringCompare( char* string_1, char* string_2 ) {
	int ctr = 0;
	for( int i = 0; i < StringLen( string_1 ); i++ ) {
		if( string_1[i] == string_2[i] )	ctr++;
		else	break;
	}
	if( ( ctr == StringLen( string_1 ) ) && ( ctr == StringLen( string_2 ) ) )	return true;
	else	return false;
}

char*	// Cut a string
qStr::CutString( char* string, int start, int end ) {
	if( start < 0 )	start = 0;
	if( end > StringLen( string ) )	end = StringLen( string );
	
	char temp[MAX_SIZE];
	ClearString( temp );
	for( int i = start; i <= end; i++ ){
		temp[i - start] = string[i];
		temp[i - start + 1] = '\0';		
	}
	
	return temp;
}

int	// get number of empty spaces for being center aligned
qStr::CenterAlignLen( int string_len, int console_len ) {
	if( string_len % 2 == 0 )
		if( console_len % 2 == 1 )	console_len--;
	else {
		if( console_len % 2 == 1 )	{ console_len--; string_len--; }
		else	string_len--;		
	}
	return ( console_len / 2 ) - ( string_len / 2 );
}


qStr	// Return a paragraph, indented and with margins, maybe center aligned
&qStr::CreateParagraph() {
	qStr output, input, temp;
	output = "\0";	temp = "\0";
	input = dat;
	
	int console_len = CONSOLE.ScreenLen();
	COORD pos;	pos.X = CONSOLE.getX();	pos.Y = CONSOLE.getY();
	int prev_pos = 0;
	int x_end = para.margin + para.max_line_len;
	if( x_end > CONSOLE.ScreenLen() )	x_end = CONSOLE.ScreenLen();
		
	for( int i = 0; i <= input.Len(); i++, pos.X++ ) {
		if( pos.X < para.margin )
			while( pos.X != para.margin ) {
				output = output + " ";
				pos.X++;
			}
		if( pos.X == x_end || input[i] == '\n' || input[i] == '\0' ) {
			while( !( input[i] == '\n' || input[i] == '\0' || input[i] == ' ' || i == prev_pos ) )	i--;
			if( i == prev_pos )	{
				output = input;
				break;
			}
			if( para.center_align ) {
				pos.X = para.margin;
				int l = CenterAlignLen( ( i-1 ) - prev_pos + 1,  x_end - para.margin ) + para.margin;
				while( pos.X < l ) {
					output = output + " ";
					pos.X++;
				}		
			}
			output = output + input( prev_pos, i - 1 );
			output = output + "\n";
			prev_pos = i + 1;	i = i + 2;
			pos.X = 0;
		}
	}
	
	return output;
}


// qStr::PC Class //


// Constructor
qStr::PC::PC() {
	Switch = true;
	margin = 0;
	
	max_line_len = CONSOLE.ScreenLen();
	
	center_align = false;
	return;
}

// GLOBAL FUNCTIONs/OPERATORs //

istream&	// cascading input operators
operator>>( istream &input, qStr &str ) {
	char in[MAX_SIZE];
	input.getline( in, MAX_SIZE );
	str.StringCopy( str.dat, in );
	return input;
}

ostream& // cascading output operators
operator<<( ostream &output, qStr &str ) {
	qStr temp;
	if( str.para.Switch )	temp = str.CreateParagraph();
	output.write( temp.ReturnAsStringArray(), temp.Len() );
	return output;
}

char* // adding string and data from an object
operator+( char* string, qStr &object ) {
	qStr temp;
	temp = string;
	temp = temp + object;
	return temp.ReturnAsStringArray();
}

bool	// comparing a string and a string data saved in an object
operator==( char* string, qStr &object ) {
	if( object == string )	return true;
	else	return false;
}

bool	// comparing a string and a string data saved in an object for inequality
operator!=( char* string, qStr &object ) {
	if( object != string )	return true;
	else	return false;
}






#endif LEXICON_H
