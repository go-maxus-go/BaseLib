#include "PubSub.h"
#include <QTest>

#include "PubSub/Publisher.h"
#include "PubSub/Subscriber.h"

#include <string>
#include <cstddef>

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

struct Publisher : PubSub::Publisher<int, std::string>
{
};

struct Subscriber : PubSub::Subscriber<std::string, int>
{
    void notify(const std::string & n) override { state << n; }
    void notify(const int         & n) override { state << n; }
    State state;
};

struct SubscriberInt : PubSub::Subscriber<int>
{
    void notify(const int & n) override { state << n; }
    State state;
};

struct SubscriberStr : PubSub::Subscriber<std::string>
{
    void notify(const std::string & n) override { state << n; }
    State state;
};

using PubInt = PubSub::Publisher<int>;
using PubStr = PubSub::Publisher<std::string>;

void PubSubTest::singleSubscription()
{
    Publisher  pub;
    Subscriber sub;
    State      s;

    pub.PubInt::publish(42);
    pub.PubInt::publish(69);
    pub.PubStr::publish("Hello");
    pub.PubInt::publish(0);

    QCOMPARE(sub.state, s << 42 << 69 << "Hello" << 0);
}
void PubSubTest::variadicSubscription()
{
    /*Publisher  pub;
    Subscriber sub;
    State      stt;

    pub.publish(42); stt << 42;
    {
        SubscriberInt subInt;
        State         sttInt;
        pub.publish(77); stt << 77; sttInt << 77;
        pub.publish("hi"); stt << "hi";
        {
            SubscriberStr subStr;
            State         sttStr;
            pub.publish("yo"); stt << "yo"; sttStr << "yo";
            pub.publish(11); stt << 11; sttInt << 11;
            QCOMPARE(subStr.state, sttStr);
        }
        pub.publish(777); stt << 777; sttInt << 777;
        pub.publish("qq"); stt << "qq";
        QCOMPARE(subInt.state, sttInt);
    }
    pub.publish(69); stt << 69;
    pub.publish("thx"); stt << "thx";
    QCOMPARE(sub.state, stt);*/
}
void PubSubTest::unsubscription()
{

}
void PubSubTest::broadcasting()
{

}
