#pragma once
#include <QObject>

class ObsTest : public QObject
{
    Q_OBJECT
public:
    explicit ObsTest(QObject * p = 0) : QObject(p) {}

private slots:
    void oneObjectOneObserverBasicOperations();
    void manyObjectsOneObserver();
    void oneObjectManyObservers();
    void maneObjectsManyObservers();
};
