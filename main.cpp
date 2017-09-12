#include <typeinfo>
#include <iostream>
#include <cassert>
#include <deque>

template<typename MSG> class Dispatcher;
template<typename MSG> class Publisher;

template<typename MSG>
class BaseSubscriber
{
    friend class Dispatcher<MSG>;
protected:
    BaseSubscriber()
    {
        Dispatcher<MSG>::get().registerSubscriber(this);
    }
    virtual void notify(const MSG &) = 0;
};

template<typename MSG>
class Dispatcher
{
    Dispatcher() {}
    Dispatcher(const Dispatcher &) = delete;
    Dispatcher(Dispatcher &&) = delete;
    void operator=(const Dispatcher &) = delete;
    void operator=(Dispatcher &&) = delete;
public:
    static Dispatcher & get()
    {
        static Dispatcher instance;
        return instance;
    }
    void registerSubscriber(BaseSubscriber<MSG> * sub)
    {
        m_subs.push_back(sub);
    }
    void sendMessage(const MSG & msg)
    {
        for (auto sub: m_subs)
            sub->notify(msg);
    }

private:
   std::deque<BaseSubscriber<MSG>*> m_subs;
};

template<typename MSG>
class Publisher
{
public:
    void publish(const MSG & msg)
    {
        Dispatcher<MSG>::get().sendMessage(msg);
    }
};

template<typename MSG, typename... ARGS>
class Subscriber
    : protected Subscriber<ARGS...>
    , protected BaseSubscriber<MSG>
{
};

template<typename MSG>
class Subscriber<MSG> : protected BaseSubscriber<MSG> {};

struct MyMessage
{
    MyMessage(int val) : val(val) {}
    int val = 0;
};

struct MyRequest
{
    MyRequest(int val) : val(val) {}
    int val = 0;
};

class MySub : protected Subscriber<MyMessage, MyRequest>
{
protected:
    void notify(const MyMessage & msg) override
    {
        std::cout << "Message: " << msg.val << std::endl;
    }
    void notify(const MyRequest & r) override
    {
        std::cout << "Request: " << r.val << std::endl;
    }
};

int main(int argc, char *argv[])
{
    MySub sub;
    Publisher<MyMessage>().publish(MyMessage(42));
    Publisher<MyMessage>().publish(MyMessage(99));
    return 0;
}
