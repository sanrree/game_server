#include <iostream>
#include "InputLooperDelegate.cpp"

class InputLooper
{
private:
    bool isLooping_ = false;

public:
    InputLooperDelegate *delegate = nullptr;

    void startInputLoop(unsigned int messageSize)
    {
        if (isLooping_)
        {
            perror("Input loop alread started.");
            return;
        }

        isLooping_ = true;

        char message[messageSize];

        while (isLooping_)
        {
            std::cin.getline(message, sizeof(message));
            if (delegate != nullptr)
            {
                (*delegate).onInputSubmit(message);
            }
        }
    }

    void stopInputLoop()
    {
        isLooping_ = false;
    }
};