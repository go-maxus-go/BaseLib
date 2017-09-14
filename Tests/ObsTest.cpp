#include "ObsTest.h"
#include <QTest>

#include "Obs/Obs.h"

#include <string>

struct State
{
    State & operator <<(int val)
        { data += std::to_string(val) + " "; return *this; }
    State & operator <<(const std::string & val)
        { data += val + " "; return *this; }
    bool operator ==(const State & rhs) const
        { return this == &rhs ? true : data == rhs.data; }
private:
    std::string data;
};

struct Object
{
    int number = 0;
    std::string name;
    Object * setNumber(int v)
    {
        if (m_state)
            *m_state << v;
        number = v;
        return this;
    }
    Object * setName(const std::string & v)
    {
        if (m_state)
            *m_state << v;
        name = v;
        return this;
    }
    Object * setState(State * state)
    {
        m_state = state;
        *m_state << "state setted";
        return this;
    }
    ~Object() { if (m_state) *m_state << "deleted";}
private:
    State * m_state = nullptr;
};

struct Controller : Obs::Obs<Object>
{
    Controller() : Obs::Obs<Object>(true) {}
};

struct Observer : Obs::Obs<Object>
{
};

void ObsTest::singleOwnerAddChangeRemove()
{
    Controller ctrl;
    Observer obs;
    State state;

    obs.add()->setState(&state);
    obs.change()->setName("name")->setNumber(42);
    obs.remove(obs.object());

    QCOMPARE(state, State() << "state setted" << "name" << 42 << "deleted");
}
