#include "osten/memory.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace osten {

EpisodicMemory::EpisodicMemory(std::size_t dimension, std::size_t capacity)
    : dimension_(dimension), capacity_(capacity) {
    if (dimension_ == 0 || capacity_ == 0) {
        throw std::invalid_argument("memory dimension and capacity must be positive");
    }
}

void EpisodicMemory::append(const Vector& vector, float strength, std::uint64_t step) {
    if (vector.size() != dimension_) {
        throw std::invalid_argument("memory vector has an unexpected size");
    }
    MemoryItem item;
    item.vector = vector;
    item.strength = std::clamp(strength, 0.0F, 1.0F);
    item.step = step;
    for (float& value : item.vector) {
        value = std::clamp(value, -1.0F, 1.0F);
    }
    if (items_.size() == capacity_) {
        items_.pop_front();
    }
    items_.push_back(std::move(item));
}

Vector EpisodicMemory::summary() const {
    Vector result(dimension_, 0.0F);
    if (items_.empty()) {
        return result;
    }

    float total = 0.0F;
    const float denominator = static_cast<float>(std::max<std::size_t>(1, items_.size() - 1));
    for (std::size_t index = 0; index < items_.size(); ++index) {
        const float recency = 0.5F + 0.5F * static_cast<float>(index) / denominator;
        const float weight = items_[index].strength * recency;
        total += weight;
        for (std::size_t dimension = 0; dimension < dimension_; ++dimension) {
            result[dimension] += items_[index].vector[dimension] * weight;
        }
    }
    if (total > 0.000001F) {
        for (float& value : result) {
            value = std::clamp(value / total, -1.0F, 1.0F);
        }
    }
    return result;
}

void EpisodicMemory::clear() noexcept {
    items_.clear();
}

} // namespace osten
