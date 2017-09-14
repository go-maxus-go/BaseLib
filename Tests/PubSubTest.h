#pragma once
#include <QObject>

class PubSubTest : public QObject
{
    Q_OBJECT
public:
    explicit PubSubTest(QObject * p = 0) : QObject(p) {}

private slots:
    void singleSubscription();
    void variadicSubscription();
    void unsubscription();
    void broadcasting();
};
