#include "osten/evaluation.hpp"

#include <cmath>
#include <set>

#include "osten/math.hpp"

namespace osten {

EvaluationReport evaluate_baseline(OstenEngine& engine,
                                   std::size_t steps,
                                   const std::string& text) {
    EvaluationReport report;
    const std::vector<Decision> decisions = engine.run(steps, text);
    report.steps = decisions.size();
    report.survived = true;
    if (decisions.empty()) {
        return report;
    }

    std::set<std::size_t> actions;
    float memory_signal = 0.0F;
    float state_change = 0.0F;
    float continuity = 0.0F;
    float goal_signal = 0.0F;
    for (std::size_t index = 0; index < decisions.size(); ++index) {
        const Decision& decision = decisions[index];
        actions.insert(decision.action_id);
        memory_signal += decision.memory_strength;
        state_change += std::abs(decision.action_strength);
        goal_signal += 1.0F - std::min(1.0F, math::norm(decision.goal));
        if (index != 0) {
            continuity += decisions[index - 1].action_id == decision.action_id ? 1.0F : 0.0F;
        }
    }
    report.output_is_diverse = actions.size() > 1 || state_change > 0.0001F;
    report.memory_utilization = memory_signal / static_cast<float>(decisions.size());
    report.goal_attainment = goal_signal / static_cast<float>(decisions.size());
    report.self_consistency = engine.state().state.size() == engine.config().state_dim ? 1.0F : 0.0F;
    report.action_continuity = decisions.size() > 1
        ? continuity / static_cast<float>(decisions.size() - 1)
        : 1.0F;
    return report;
}

} // namespace osten
