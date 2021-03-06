#ifndef KMINTAPP_HARE_HPP
#define KMINTAPP_HARE_HPP

#include "kmint/map/map.hpp"
#include "kmint/play.hpp"
#include "kmint/primitives.hpp"
#include "kmint/random.hpp"

#include <stack>

class state;
class cow;

class hare : public kmint::play::map_bound_actor {
public:
    hare(kmint::map::map_graph &g);
    void act(kmint::delta_time dt) override;
    kmint::ui::drawable const &drawable() const override { return drawable_; }
    void set_cow(cow const &c) { cow_ = &c; }
    bool incorporeal() const override { return false; }
    kmint::scalar radius() const override { return 16.0; }
    void getNewNode(kmint::map::map_graph const &g);

private:
    kmint::play::image_drawable drawable_;
    cow const *cow_;
    kmint::graph::basic_graph<kmint::map::map_node_info>::node_type* node_;

    std::stack<std::unique_ptr<state>> currentState_;
};

#endif /* KMINTAPP_HARE_HPP */