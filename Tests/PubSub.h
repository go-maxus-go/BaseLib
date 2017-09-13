#include <string>
#include <iostream>

#include "PubSub/Publisher.h"
#include "PubSub/Subscriber.h"

struct MyMessage
{
    MyMessage(const std::string & val) : val(val) {}
    std::string val;
};

struct MyRequest
{
    MyRequest(int val) : val(val) {}
    int val = 0;
};

class MySub : protected PubSub::Subscriber<MyMessage, int>
{
protected:
    void notify(const MyMessage & msg) override
    {
        std::cout << "Message: " << msg.val << std::endl;
    }
    void notify(const int & r) override
    {
        std::cout << "int: " << r << std::endl;
    }
};

class MySub2 : protected PubSub::Subscriber<MyRequest, int>
{
protected:
    void notify(const MyRequest & r) override
    {
        std::cout << "Request: " << r.val << std::endl;
    }
    void notify(const int & r) override
    {
        std::cout << "int: " << r << std::endl;
    }
};
