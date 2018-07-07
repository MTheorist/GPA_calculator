#include <iostream>
#include <fstream>

using namespace std;

int main(){
	int i = 0;
	char word[30];
	ifstream test;
	test.open("test_1.txt", ios::in);
	while(	test >> word ){
		//cout<<word<<endl;
		cout<<i++;	
	}
	test.close();
}	
