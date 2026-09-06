#include "osten/engine.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

#include "osten/math.hpp"

namespace osten {

OstenEngine::OstenEngine(Config config)
    : config_(config),
      encoder_(config.encoder_dim),
      policy_(config),
      memory_(config.memory_dim, config.memory_capacity),
      state_{math::zeros(config.state_dim), math::zeros(config.goal_dim), 0} {}

void OstenEngine::reset() {
    state_.state = math::zeros(config_.state_dim);
    state_.goal = math::zeros(config_.goal_dim);
    state_.step = 0;
    memory_.clear();
}

Vector OstenEngine::heartbeat() const {
    const float phase = static_cast<float>(state_.step);
    const float scale = static_cast<float>(std::max<std::size_t>(1, config_.memory_capacity));
    float state_norm = math::norm(state_.state);
    state_norm /= std::sqrt(static_cast<float>(config_.state_dim));
    return {std::sin(phase * 0.17F), std::cos(phase * 0.17F),
            std::min(1.0F, static_cast<float>(state_.step) / scale), state_norm};
}

Decision OstenEngine::tick(const std::string& text) {
    const std::string bounded_text =
        text.substr(0, std::min(text.size(), config_.max_text_length));
    const Vector observation = encoder_.encode(bounded_text);
    const Vector pulse = heartbeat();
    const NetworkOutput output = policy_.forward(
        observation, state_.state, state_.goal, memory_.summary(), pulse);

    Vector next_state = math::add(state_.state, output.state_delta);
    Vector next_goal = math::add(state_.goal, output.goal_delta);
    state_.state = math::clip_finite(next_state, config_.state_limit);
    state_.goal = math::clip_finite(next_goal, config_.goal_limit);
    ++state_.step;
    memory_.append(output.memory_vector, output.memory_strength, state_.step);

    const std::size_t action_id = math::argmax(output.action_logits);
    Decision decision;
    decision.step = state_.step;
    decision.action_id = action_id;
    decision.action_strength = std::tanh(output.action_logits[action_id]);
    decision.action_logits = output.action_logits;
    decision.state_delta = output.state_delta;
    decision.state = state_.state;
    decision.goal_delta = output.goal_delta;
    decision.goal = state_.goal;
    decision.memory_strength = output.memory_strength;
    decision.memory_size = memory_.size();
    decision.confidence = output.confidence;
    decision.heartbeat = pulse;
    return decision;
}

std::vector<Decision> OstenEngine::run(std::size_t steps, const std::string& text) {
    std::vector<Decision> decisions;
    decisions.reserve(steps);
    for (std::size_t index = 0; index < steps; ++index) {
        decisions.push_back(tick(text));
    }
    return decisions;
}

} // namespace osten

