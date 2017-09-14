#pragma once

#include "Dispatcher.h"

namespace PubSub {

template<typename MSG, typename... ARGS>
class Publisher
    : protected Publisher<ARGS...>
    , protected __private__::BasePublisher<MSG>
{
public:
    template<typename T> void publish(const T & msg)
        { __private__::BasePublisher<T>::publish(msg); }
};

template<typename MSG>
class Publisher<MSG> : protected __private__::BasePublisher<MSG>
{
public:
    template<typename T> void publish(const T & msg)
        { __private__::BasePublisher<T>::publish(msg); }
};

template<typename MSG> void publish(const MSG & msg)
    { __private__::Dispatcher<MSG>::get().sendMessage(msg); }

}
