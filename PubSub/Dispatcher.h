#pragma once

#include <list>
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
    void subscribe()
    {
        if (m_subscribed)
            return;
        Dispatcher<MSG>::get().registerSubscriber(this);
        m_subscribed = true;
    }
    void unsubscribe()
    {
        if (!m_subscribed)
            return;
        Dispatcher<MSG>::get().removeSubscriber(this);
        m_subscribed = false;
    }
private:
    bool m_subscribed = false;
};

template<typename MSG>
class BasePublisher
{
protected:
    BasePublisher() = default;
    ~BasePublisher() = default;
public:
    void publish(const MSG & msg)
        { Dispatcher<MSG>::get().sendMessage(msg, this); }
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
        { m_subs.push_back(sub); }
    void removeSubscriber(BaseSubscriber<MSG> * sub)
    {
        auto it = std::find(m_subs.begin(), m_subs.end(), sub);
        assert(it != m_subs.end());
        m_subs.erase(it);
    }
    void sendMessage(const MSG & msg, const BasePublisher<MSG> * pub)
    {
        assert(!m_subs.empty());
        for (auto sub: m_subs)
            if (pub != reinterpret_cast<const BasePublisher<MSG>*>(sub))
                sub->notify(msg);
    }
    void broadcastMessage(const MSG & msg)
    {
        assert(!m_subs.empty());
        for (auto sub: m_subs)
            sub->notify(msg);
    }
private:
   std::list<BaseSubscriber<MSG>*> m_subs;
};

} }
