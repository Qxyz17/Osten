#include <cmath>
#include <iostream>
#include <stdexcept>

#include "osten/encoder.hpp"
#include "osten/engine.hpp"

namespace {

void require(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void test_encoder() {
    osten::CharacterNGramEncoder encoder(32);
    const auto left = encoder.encode("hello Osten");
    const auto right = encoder.encode("different input");
    require(left.size() == 32, "encoder dimension");
    require(left != right, "encoder must represent observations");
}

void test_autonomous_ticks() {
    osten::OstenEngine engine;
    const auto decisions = engine.run(32);
    require(decisions.size() == 32, "tick count");
    require(engine.memory().size() == engine.config().memory_capacity / 2, "memory capacity behavior");
    require(decisions.back().step == 32, "step counter");
    bool changed = false;
    for (float value : decisions.back().state) {
        changed = changed || std::abs(value) > 0.0F;
    }
    require(changed, "autonomous state must change without input");
}

void test_reproducible() {
    osten::OstenEngine left;
    osten::OstenEngine right;
    const auto left_run = left.run(8, "same observation");
    const auto right_run = right.run(8, "same observation");
    require(left_run.size() == right_run.size(), "run size");
    for (std::size_t index = 0; index < left_run.size(); ++index) {
        require(left_run[index].action_id == right_run[index].action_id,
                "fixed seed must be reproducible");
        require(left_run[index].state == right_run[index].state,
                "state must be reproducible");
    }
}

} // namespace

int main() {
    try {
        test_encoder();
        test_autonomous_ticks();
        test_reproducible();
        std::cout << "all tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "test failure: " << error.what() << '\n';
        return 1;
    }
}

