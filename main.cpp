#include <iostream>

#include "Tests/PubSub.h"

int main(int argc, char *argv[])
{
    MySub sub;
    MySub2 sub2;

    PubSub::publish(MyMessage("Hello!"));
    PubSub::publish(MyRequest(99));
    PubSub::publish(777);
    return 0;
}
