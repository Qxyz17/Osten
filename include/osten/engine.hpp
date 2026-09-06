#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include "osten/encoder.hpp"
#include "osten/memory.hpp"
#include "osten/network.hpp"
#include "osten/training.hpp"
#include "osten/types.hpp"

namespace osten {

class OstenEngine {
public:
    explicit OstenEngine(Config config = {});

    void reset();
    Decision tick(const std::string& text = {});
    std::vector<Decision> run(std::size_t steps, const std::string& text = {});

    const EngineState& state() const noexcept { return state_; }
    const EpisodicMemory& memory() const noexcept { return memory_; }
    const Config& config() const noexcept { return config_; }

private:
    Vector heartbeat() const;

    Config config_;
    CharacterNGramEncoder encoder_;
    ForwardPolicy policy_;
    EpisodicMemory memory_;
    EngineState state_;
};

} // namespace osten

