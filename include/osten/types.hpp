#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace osten {

using Vector = std::vector<float>;

struct Config {
    std::size_t encoder_dim = 64;
    std::size_t state_dim = 32;
    std::size_t goal_dim = 16;
    std::size_t memory_dim = 32;
    std::size_t hidden_dim = 96;
    std::size_t action_count = 8;
    std::size_t memory_capacity = 64;
    std::uint64_t seed = 17;
    float state_limit = 1.0F;
    float goal_limit = 1.0F;
    std::size_t max_text_length = 2048;
};

struct EngineState {
    Vector state;
    Vector goal;
    std::uint64_t step = 0;
};

struct Decision {
    std::uint64_t step = 0;
    std::size_t action_id = 0;
    float action_strength = 0.0F;
    Vector action_logits;
    Vector state_delta;
    Vector state;
    Vector goal_delta;
    Vector goal;
    float memory_strength = 0.0F;
    std::size_t memory_size = 0;
    float confidence = 0.0F;
    Vector heartbeat;
};

struct EvaluationReport {
    std::uint64_t steps = 0;
    bool survived = false;
    bool output_is_diverse = false;
    float memory_utilization = 0.0F;
    float goal_attainment = 0.0F;
    float self_consistency = 0.0F;
    float action_continuity = 0.0F;
};

} // namespace osten
