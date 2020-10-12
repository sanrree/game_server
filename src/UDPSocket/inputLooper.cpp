#include <iostream>

class InputLooper
{
private:
    char *message_;

    bool isLooping_ = false;

public:
    InputLooperDelegate *delegate = nullptr;

protected:
    void startInputLoop(unsigned int messageSize)
    {
        if (isLooping_)
        {
            perror("Input loop alread started.");
            return;
        }

        isLooping_ = true;

        message_ = new char[messageSize];

        char msgBuffer[messageSize];

        while (isLooping_)
        {
            std::cin.getline(message_, sizeof(message_));
            if (delegate != nullptr)
            {
                (*delegate).onInputSubmit(msgBuffer);
            }
        }
    }

    void stopInputLoop()
    {
        isLooping_ = false;
        delete message_;
    }

public:
    ~InputLooper()
    {
        delete message_;
    }
};

class InputLooperDelegate
{
public:
    virtual void onInputSubmit(char *message);
};