#include <QtTest>
#include <QCoreApplication>

#include "Tests/PubSub.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTest::qExec(new PubSubTest, argc, argv);

    return 0;
}
