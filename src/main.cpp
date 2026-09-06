#include <cstdlib>
#include <iostream>
#include <string>

#include "osten/engine.hpp"
#include "osten/evaluation.hpp"

int main(int argc, char** argv) {
    std::size_t steps = 8;
    std::string text;
    if (argc >= 2) {
        steps = static_cast<std::size_t>(std::strtoull(argv[1], nullptr, 10));
    }
    if (argc >= 3) {
        text = argv[2];
    }

    osten::OstenEngine engine;
    const auto decisions = engine.run(steps, text);
    for (const auto& decision : decisions) {
        std::cout << "step=" << decision.step
                  << " action=" << decision.action_id
                  << " strength=" << decision.action_strength
                  << " confidence=" << decision.confidence
                  << " memory=" << decision.memory_size << '\n';
    }
    const auto report = osten::evaluate_baseline(engine, 16, text);
    std::cout << "evaluation survived=" << report.survived
              << " diverse=" << report.output_is_diverse
              << " memory_utilization=" << report.memory_utilization
              << " action_continuity=" << report.action_continuity << '\n';
    return 0;
}

