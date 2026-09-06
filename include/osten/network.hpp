#pragma once

#include <cstddef>
#include <random>

#include "osten/types.hpp"

namespace osten {

struct NetworkOutput {
    Vector state_delta;
    Vector action_logits;
    Vector memory_vector;
    float memory_strength = 0.0F;
    Vector goal_delta;
    float confidence = 0.0F;
};

class ForwardPolicy {
public:
    explicit ForwardPolicy(const Config& config);

    NetworkOutput forward(const Vector& observation,
                          const Vector& state,
                          const Vector& goal,
                          const Vector& memory,
                          const Vector& heartbeat) const;

private:
    Vector w1_;
    Vector b1_;
    Vector w2_;
    Vector b2_;
    Vector w_state_;
    Vector b_state_;
    Vector w_action_;
    Vector b_action_;
    Vector w_memory_;
    Vector b_memory_;
    Vector w_goal_;
    Vector b_goal_;
    Vector w_strength_;
    Vector b_strength_;
    Vector w_confidence_;
    Vector b_confidence_;
    std::size_t input_dim_;
    Config config_;
};

} // namespace osten

