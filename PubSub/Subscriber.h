#pragma once

#include "Dispatcher.h"

namespace PubSub {

template<typename MSG, typename... ARGS>
class Subscriber
    : protected Subscriber<ARGS...>
    , protected __private__::BaseSubscriber<MSG>
{
public:
    template<typename T> void subscribe()
        { __private__::BaseSubscriber<T>::subscribe(); }
    template<typename T> void unsubscribe()
        { __private__::BaseSubscriber<T>::unsubscribe(); }
    void subscribeAll()
    {
        subscribe<MSG>();
        Subscriber<ARGS...>::subscribeAll();
    }
    void unsubscribeAll()
    {
        unsubscribe<MSG>();
        Subscriber<ARGS...>::unsubscribeAll();
    }
};

template<typename MSG>
class Subscriber<MSG> : protected __private__::BaseSubscriber<MSG>
{
public:
    template<typename T> void subscribe()
        { __private__::BaseSubscriber<T>::subscribe(); }
    template<typename T> void unsubscribe()
        { __private__::BaseSubscriber<T>::unsubscribe(); }
    void subscribeAll()
        { subscribe<MSG>(); }
    void unsubscribeAll()
        { unsubscribe<MSG>(); }
};

}
