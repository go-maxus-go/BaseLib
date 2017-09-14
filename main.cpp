#include <QtTest>
#include <QCoreApplication>

#include "Tests/PubSubTest.h"
#include "Tests/Obstest.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTest::qExec(new PubSubTest, argc, argv);
    QTest::qExec(new ObsTest   , argc, argv);

    return 0;
}
