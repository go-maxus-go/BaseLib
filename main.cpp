#include <typeinfo>
#include <iostream>
#include <map>
#include <cassert>

template<typename MSG> class Dispatcher;
template<typename MSG> class Publisher;
template<typename MSG> class BaseSubscriber
{
    friend class Dispatcher<MSG>;
protected:
    virtual void notify(const MSG &) = 0;
};

class Message {};

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
        const std::string name = typeid(MSG).name();
        m_msg2sub.insert(std::make_pair(name, sub));
    }
    void sendMessage(const MSG & msg)
    {
        const std::string name = typeid(msg).name();
        if (auto count = m_msg2sub.count(name))
        {
            auto it = m_msg2sub.find(name);
            for (size_t i = 0; i < count; ++i, ++it)
                it->second->notify(msg);
        }
    }

private:
   std::multimap<std::string, BaseSubscriber<MSG> *> m_msg2sub;
};

template<typename MSG>
class Publisher
{
public:
    Publisher() {}

public:
    void publish(const MSG & msg)
    {
        Dispatcher<MSG>::get().sendMessage(msg);
    }
};

template<typename MSG>
class Subscriber : protected BaseSubscriber<MSG>
{
protected:
    Subscriber()
        { Dispatcher<MSG>::get().registerSubscriber(this); }
    virtual void notify(const MSG &) = 0;
};

struct MyMessage : Message
{
    MyMessage(int val) : val(val) {}
    int val = 0;
};

class MySub : protected Subscriber<MyMessage>
{
protected:
    void notify(const MyMessage & msg) override
    {
        std::cout << msg.val << std::endl;
    }
};

int main(int argc, char *argv[])
{
    MySub sub;
    Publisher<MyMessage>().publish(MyMessage(42));
    return 0;
}
