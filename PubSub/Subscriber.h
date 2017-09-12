#pragma once

#include "Dispatcher.h"

namespace PubSub {

template<typename MSG, typename... ARGS>
class Subscriber
    : protected Subscriber<ARGS...>
    , protected __private__::BaseSubscriber<MSG>
{
protected:
    using __private__::BaseSubscriber<MSG>::notify;
};

template<typename MSG>
class Subscriber<MSG> : protected __private__::BaseSubscriber<MSG> {};

}
