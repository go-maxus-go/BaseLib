#include <iostream>

#include "Tests/PubSub.h"

#include "Obs/Obs.h"

struct Object
{
    int a = 0;
    std::string b = "wow";
};

int main(int argc, char *argv[])
{
    /*MySub sub;
    MySub2 sub2;

    PubSub::publish(MyMessage("Hello!"));
    PubSub::publish(MyRequest(99));
    PubSub::publish(777);*/

    Object obj;
    Obs::Chain<Object> chain(nullptr, &obj);
    chain->a = 5;
    chain->b = "yo";

    const Object * pObj = chain;

    return 0;
}
