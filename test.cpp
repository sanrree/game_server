// Example program
#include <iostream>
#include <cstring>
#include <thread>

void change(char* str){
    	char sta[10] = "adasdas";
    	strncpy(str,sta,10);
}

int main()
{
	char arr[10];
  
  	std::thread t(change,arr);
	t.detach();

while(1){
  	std::cout << "Hello, " << arr;
}

}
