#ifndef KMINTAPP_STATE_H
#define KMINTAPP_STATE_H

#include "kmint/primitives.hpp"

class state {
public:
    state();
    ~state() = default;
    state(const state& other) = delete;
    state(state&& other) noexcept = delete;
    state& operator=(const state& other) = delete;
    state& operator=(state&& other) noexcept = delete;

    virtual void update(kmint::delta_time deltaTime);

protected:
    kmint::delta_time elapsedTime_;
};

#endif KMINTAPP_STATE_H