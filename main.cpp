#include <iostream>

#include "Tests/PubSub.h"

#include "Obs/Obs.h"

struct Object
{
    int a = 0;
    std::string b = "wow";
};

struct Controller : protected Obs::Obs<Object>
{
    Controller() : Obs::Obs<Object>(true) {}
};

struct Observer : Obs::Obs<Object>
{
};

void obs_add()
{
    Controller ctrl;
    Observer obs;
    obs.add();
    assert(obs.objects().size() == 1);
    std::cout << obs.object()->a << " " << obs.object()->b;
}

int main(int argc, char *argv[])
{
    /*MySub sub;
    MySub2 sub2;

    PubSub::publish(MyMessage("Hello!"));
    PubSub::publish(MyRequest(99));
    PubSub::publish(777);*/

    obs_add();

    return 0;
}
