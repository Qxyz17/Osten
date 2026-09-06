#pragma once

#include <cstddef>
#include <string>

#include "osten/engine.hpp"

namespace osten {

EvaluationReport evaluate_baseline(OstenEngine& engine,
                                   std::size_t steps,
                                   const std::string& text = {});

} // namespace osten

