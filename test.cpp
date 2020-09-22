#include <thread>
#include <iostream>

void run(int a){
    std::cout << "Hahoe";
}

int main()
{
    std::thread t1(run,5);
    t1.join();
    return 0;
}