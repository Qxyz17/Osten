#include "osten/training.hpp"

namespace osten {

void NoOpOfflineStrategy::observe(const TrainingSample& sample) {
    (void)sample;
    ++samples_seen_;
}

} // namespace osten

