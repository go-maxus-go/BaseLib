#pragma once

#include "Dispatcher.h"

namespace PubSub {

template<typename MSG, typename ... ARGS>
class Publisher
        : public Publisher<ARGS...>
        , public __private__::BasePublisher<MSG>
{
public:
    using __private__::BasePublisher<ARGS...>::publish;
};

template<typename MSG>
class Publisher<MSG> : public __private__::BasePublisher<MSG>
{
};

template<typename MSG> void broadcast(const MSG & msg)
    { __private__::Dispatcher<MSG>::get().sendMessage(msg); }

}
