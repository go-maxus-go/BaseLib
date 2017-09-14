#pragma once

#include <set>
#include <cassert>

namespace PubSub { namespace __private__ {

template<typename MSG> class Dispatcher;

template<typename MSG>
class BaseSubscriber
{
protected:
    BaseSubscriber() { subscribe(); }
    ~BaseSubscriber() { unsubscribe(); }
public:
    virtual void notify(const MSG &) = 0;
protected:
    void subscribe  () { Dispatcher<MSG>::get().  registerSubscriber(this); }
    void unsubscribe() { Dispatcher<MSG>::get().unregisterSubscriber(this); }
};

template<typename MSG>
class BasePublisher
{
protected:
    BasePublisher() = default;
    ~BasePublisher() = default;
public:
    void publish(const MSG & msg)
        { Dispatcher<MSG>::get().sendMessage(msg); }
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
        { if (!isRegistered(sub)) m_subs.insert(sub); }
    void unregisterSubscriber(BaseSubscriber<MSG> * sub)
        { if (isRegistered(sub)) m_subs.erase(sub); }
    bool isRegistered(BaseSubscriber<MSG> * sub)
        { return m_subs.find(sub) != m_subs.cend(); }
    void sendMessage(const MSG & msg)
    {
        assert(!m_subs.empty());
        for (auto sub: m_subs)
            sub->notify(msg);
    }
private:
   std::set<BaseSubscriber<MSG>*> m_subs;
};

} }
