#include "osten/network.hpp"

#include <stdexcept>

#include "osten/math.hpp"

namespace osten {

ForwardPolicy::ForwardPolicy(const Config& config)
    : input_dim_(config.encoder_dim + config.state_dim + config.goal_dim +
                 config.memory_dim + 4),
      config_(config) {
    std::mt19937_64 rng(config.seed);
    w1_ = math::random_weights(rng, config.hidden_dim, input_dim_);
    b1_ = math::zeros(config.hidden_dim);
    w2_ = math::random_weights(rng, config.hidden_dim, config.hidden_dim);
    b2_ = math::zeros(config.hidden_dim);
    w_state_ = math::random_weights(rng, config.state_dim, config.hidden_dim);
    b_state_ = math::zeros(config.state_dim);
    w_action_ = math::random_weights(rng, config.action_count, config.hidden_dim);
    b_action_ = math::zeros(config.action_count);
    w_memory_ = math::random_weights(rng, config.memory_dim, config.hidden_dim);
    b_memory_ = math::zeros(config.memory_dim);
    w_goal_ = math::random_weights(rng, config.goal_dim, config.hidden_dim);
    b_goal_ = math::zeros(config.goal_dim);
    w_strength_ = math::random_weights(rng, 1, config.hidden_dim);
    b_strength_ = math::zeros(1);
    w_confidence_ = math::random_weights(rng, 1, config.hidden_dim);
    b_confidence_ = math::zeros(1);
}

NetworkOutput ForwardPolicy::forward(const Vector& observation,
                                      const Vector& state,
                                      const Vector& goal,
                                      const Vector& memory,
                                      const Vector& heartbeat) const {
    if (observation.size() != config_.encoder_dim ||
        state.size() != config_.state_dim ||
        goal.size() != config_.goal_dim ||
        memory.size() != config_.memory_dim ||
        heartbeat.size() != 4) {
        throw std::invalid_argument("forward input dimensions do not match Config");
    }
    const Vector features = math::concatenate(
        {&observation, &state, &goal, &memory, &heartbeat});
    Vector hidden = math::affine(w1_, config_.hidden_dim, input_dim_, features, b1_);
    hidden = math::tanh(hidden);
    hidden = math::affine(w2_, config_.hidden_dim, config_.hidden_dim, hidden, b2_);
    hidden = math::tanh(hidden);

    NetworkOutput output;
    output.state_delta = math::scaled_tanh(
        math::affine(w_state_, config_.state_dim, config_.hidden_dim, hidden, b_state_),
        0.08F);
    output.action_logits = math::affine(
        w_action_, config_.action_count, config_.hidden_dim, hidden, b_action_);
    output.memory_vector = math::tanh(
        math::affine(w_memory_, config_.memory_dim, config_.hidden_dim, hidden, b_memory_));
    output.memory_strength = math::sigmoid(
        math::affine(w_strength_, 1, config_.hidden_dim, hidden, b_strength_)[0]);
    output.goal_delta = math::scaled_tanh(
        math::affine(w_goal_, config_.goal_dim, config_.hidden_dim, hidden, b_goal_),
        0.04F);
    output.confidence = math::sigmoid(
        math::affine(w_confidence_, 1, config_.hidden_dim, hidden, b_confidence_)[0]);
    return output;
}

} // namespace osten

