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
    bool * destroyed = nullptr;
    Object * setNumber(int v)
    {
        m_state << v;
        number = v;
        return this;
    }
    Object * setName(const std::string & v)
    {
        m_state << v;
        name = v;
        return this;
    }
    const State & state() const { return m_state; }
    ~Object() { if (destroyed) *destroyed = true; }
private:
    State m_state;
};

using ObjectObs = Obs::Obs<Object>;
struct Controller : ObjectObs
{
    Controller() : ObjectObs(true) {}
};

struct Observer : Obs::Obs<Object>
{
};

void ObsTest::singleOwnerAddChangeRemove()
{
    Controller ctrl;
    Observer obs;

    obs.add()->setName("name");
    obs.change()->setNumber(42);
    QCOMPARE(obs.object()->state(), State() << "name" << 42);

    bool isDestroyed = false;
    const_cast<Object*>(obs.object())->destroyed = &isDestroyed;
    obs.remove(obs.object());
    QCOMPARE(isDestroyed, true);
}
