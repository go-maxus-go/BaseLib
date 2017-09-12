#pragma once

#include "Dispatcher.h"

namespace PubSub {

template<typename MSG> void publish(const MSG & msg)
    { __private__::Dispatcher<MSG>::get().sendMessage(msg); }

}
