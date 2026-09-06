#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>

#include "osten/types.hpp"

namespace osten {

struct MemoryItem {
    Vector vector;
    float strength = 0.0F;
    std::uint64_t step = 0;
};

class EpisodicMemory {
public:
    EpisodicMemory(std::size_t dimension, std::size_t capacity);

    void append(const Vector& vector, float strength, std::uint64_t step);
    Vector summary() const;
    void clear() noexcept;
    std::size_t size() const noexcept { return items_.size(); }
    std::size_t dimension() const noexcept { return dimension_; }

private:
    std::size_t dimension_;
    std::size_t capacity_;
    std::deque<MemoryItem> items_;
};

} // namespace osten

