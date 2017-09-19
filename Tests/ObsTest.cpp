#include "ObsTest.h"
#include <QTest>

#include "Obs/Obs.h"

#include <string>
#include <vector>

struct Object
{
    int number = 0;
    std::string name;
    bool * destroyed = nullptr;
    ~Object()
        { if (destroyed) *destroyed = true; }
};

void ObsTest::oneObjectOneObserverBasicOperations()
{
    Obs::Controller<Object> ctrl;
    Obs::View<Object> view;
    QCOMPARE(view.object(), nullptr);
    QVERIFY(view.objects().empty());
    // add
    auto obj = view.add();
    QCOMPARE(view.object(), obj);
    QCOMPARE(view.objects().size(), 1U);
    // change
    auto copy = *obj;
    copy.name = "name";
    copy.number = 42;
    view.change(obj, copy);
    QCOMPARE(obj->name, copy.name);
    QCOMPARE(obj->number, copy.number);
    // remove
    bool isDestroyed = false;
    const_cast<Object*>(obj)->destroyed = &isDestroyed;
    view.remove(obj);
    QCOMPARE(isDestroyed, true);
    QCOMPARE(view.object(), nullptr);
    QVERIFY(view.objects().empty());
}
void ObsTest::manyObjectsOneObserver()
{
    Obs::Controller<Object> ctrl;
    Obs::View<Object> v1;
    Obs::View<Object> v2;

    auto o1 = v1.add();
    v2.observe(o1);
    QCOMPARE(v1.object(), v2.object());

    auto copy = *o1;
    copy.name = "name";
    copy.number = 42;
    v2.change(o1, copy);
    QCOMPARE(v1.object()->name, copy.name);
    QCOMPARE(v1.object()->number, copy.number);

    v2.remove(o1);
}

void ObsTest::oneObjectManyObservers()
{
}
void ObsTest::maneObjectsManyObservers()
{
}
