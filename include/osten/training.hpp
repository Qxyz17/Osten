#pragma once

#include <cstddef>

#include "osten/types.hpp"

namespace osten {

struct TrainingSample {
    Vector state;
    Vector action;
    Vector next_state;
    float reward = 0.0F;
};

class TrainingStrategy {
public:
    virtual ~TrainingStrategy() = default;
    virtual void observe(const TrainingSample& sample) = 0;
    virtual std::size_t samples_seen() const noexcept = 0;
};

class NoOpOfflineStrategy final : public TrainingStrategy {
public:
    void observe(const TrainingSample& sample) override;
    std::size_t samples_seen() const noexcept override { return samples_seen_; }

private:
    std::size_t samples_seen_ = 0;
};

} // namespace osten

