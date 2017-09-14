#include "PubSubTest.h"
#include <QTest>

#include "PubSub/Publisher.h"
#include "PubSub/Subscriber.h"

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

std::string operator ""_s(const char * str, std::size_t length)
    { return std::move(std::string(str, length)); }

void PubSubTest::singleSubscription()
{
    PubSub::Publisher<int, std::string> pub;
    Subscriber sub;
    State      s;

    pub.publish(42);
    pub.publish(69);
    pub.publish("Hello"_s);
    pub.publish(0);

    QCOMPARE(sub.state, s << 42 << 69 << "Hello" << 0);
}
void PubSubTest::variadicSubscription()
{
    PubSub::Publisher<int, std::string> pub;
    Subscriber sub;
    State      stt;

    pub.publish(42); stt << 42;
    {
        SubscriberInt subInt;
        State         sttInt;
        pub.publish(77); stt << 77; sttInt << 77;
        pub.publish("hi"_s); stt << "hi"_s;
        {
            SubscriberStr subStr;
            State         sttStr;
            pub.publish("yo"_s); stt << "yo"_s; sttStr << "yo"_s;
            pub.publish(11); stt << 11; sttInt << 11;
            QCOMPARE(subStr.state, sttStr);
        }
        pub.publish(777); stt << 777; sttInt << 777;
        pub.publish("qq"_s); stt << "qq"_s;
        QCOMPARE(subInt.state, sttInt);
    }
    pub.publish(69); stt << 69;
    pub.publish("thx"_s); stt << "thx"_s;
    QCOMPARE(sub.state, stt);
}
void PubSubTest::unsubscription()
{
    PubSub::Publisher<int, std::string> pub;
    Subscriber    sub;
    SubscriberInt subInt;
    SubscriberStr subStr;
    State stt;
    State sttInt;
    State sttStr;

    { auto v = 42; pub.publish(v); stt << v; sttInt << v; }
    { auto v = "hi"_s; pub.publish(v); stt << v; sttStr << v; }
    sub.unsubscribe<std::string>();
    { auto v = 77; pub.publish(v); stt << v; sttInt << v; }
    { auto v = "y0"_s; pub.publish(v); sttStr << v; }
    sub.unsubscribe<int>();
    { auto v = 69; pub.publish(v); sttInt << v; }
    { auto v = "opa"_s; pub.publish(v); sttStr << v; }
    sub.subscribeAll();
    { auto v = 123; pub.publish(v); stt << v; sttInt << v; }
    { auto v = "cpp"_s; pub.publish(v); stt << v; sttStr << v; }
    subStr.unsubscribe<std::string>();
    { auto v = "smth"_s; pub.publish(v); stt << v; }
    subStr.subscribe<std::string>();
    { auto v = "dada"_s; pub.publish(v); stt << v; sttStr << v; }
    subInt.unsubscribeAll();
    { auto v = 321; pub.publish(v); stt << v; }

    QCOMPARE(sub.state, stt);
    QCOMPARE(subInt.state, sttInt);
    QCOMPARE(subStr.state, sttStr);
}
void PubSubTest::broadcasting()
{
    struct PubSubber : PubSub::Publisher<int>, PubSub::Subscriber<int>
    {
        State s;
        void notify(const int & v) override { s << v; }
    } pubSubber;
    SubscriberInt subInt;

    pubSubber.unsubscribe<int>();
    pubSubber.publish(1);
    pubSubber.subscribe<int>();
    PubSub::Publisher<int>().publish(2);
    PubSub::publish<int>(3);

    QCOMPARE(pubSubber.s , State() << 2 << 3);
    QCOMPARE(subInt.state, State() << 1 << 2 << 3);
}
