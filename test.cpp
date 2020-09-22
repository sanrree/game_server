#include <iostream>
#include <thread>

using namespace std;

void input(char* str){
	while(true){
	cin >> str;
	}
}

int main()
{
   char* str = new char[1000];
   str[0] = '\0';

  thread th(input,str);
   th.detach();

	while (true) {
		if(str[0] != '\0'){
			cout << str << endl;
			str[0] = '\0';
		}
    }
}