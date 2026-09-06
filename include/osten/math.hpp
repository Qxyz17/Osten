#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <random>
#include <stdexcept>
#include <vector>

#include "osten/types.hpp"

namespace osten::math {

inline void require_size(const Vector& value, std::size_t expected, const char* name) {
    if (value.size() != expected) {
        throw std::invalid_argument(std::string(name) + " has an unexpected size");
    }
}

inline bool finite(const Vector& value) {
    return std::all_of(value.begin(), value.end(), [](float item) {
        return std::isfinite(item);
    });
}

inline float dot(const Vector& left, const Vector& right) {
    if (left.size() != right.size()) {
        throw std::invalid_argument("dot product dimensions do not match");
    }
    float result = 0.0F;
    for (std::size_t index = 0; index < left.size(); ++index) {
        result += left[index] * right[index];
    }
    return result;
}

inline Vector add(const Vector& left, const Vector& right) {
    if (left.size() != right.size()) {
        throw std::invalid_argument("vector dimensions do not match");
    }
    Vector result(left.size(), 0.0F);
    for (std::size_t index = 0; index < left.size(); ++index) {
        result[index] = left[index] + right[index];
    }
    return result;
}

inline Vector concatenate(std::initializer_list<const Vector*> values) {
    Vector result;
    for (const Vector* value : values) {
        result.insert(result.end(), value->begin(), value->end());
    }
    return result;
}

inline Vector affine(const Vector& weights, std::size_t rows, std::size_t columns,
                     const Vector& input, const Vector& bias) {
    require_size(weights, rows * columns, "weights");
    require_size(input, columns, "input");
    require_size(bias, rows, "bias");
    Vector result(rows, 0.0F);
    for (std::size_t row = 0; row < rows; ++row) {
        float value = bias[row];
        for (std::size_t column = 0; column < columns; ++column) {
            value += weights[row * columns + column] * input[column];
        }
        result[row] = value;
    }
    return result;
}

inline Vector tanh(const Vector& value) {
    Vector result(value.size(), 0.0F);
    std::transform(value.begin(), value.end(), result.begin(), [](float item) {
        return std::tanh(item);
    });
    return result;
}

inline float sigmoid(float value) {
    const float bounded = std::clamp(value, -30.0F, 30.0F);
    return 1.0F / (1.0F + std::exp(-bounded));
}

inline Vector scaled_tanh(const Vector& value, float scale) {
    Vector result = tanh(value);
    for (float& item : result) {
        item *= scale;
    }
    return result;
}

inline Vector clip_finite(const Vector& value, float limit) {
    Vector result(value.size(), 0.0F);
    for (std::size_t index = 0; index < value.size(); ++index) {
        const float item = value[index];
        if (std::isnan(item)) {
            result[index] = 0.0F;
        } else if (std::isinf(item)) {
            result[index] = item > 0.0F ? limit : -limit;
        } else {
            result[index] = std::clamp(item, -limit, limit);
        }
    }
    return result;
}

inline float norm(const Vector& value) {
    return std::sqrt(std::max(0.0F, dot(value, value)));
}

inline Vector zeros(std::size_t size) {
    return Vector(size, 0.0F);
}

inline Vector random_weights(std::mt19937_64& rng, std::size_t rows, std::size_t columns) {
    const float scale = std::sqrt(2.0F / static_cast<float>(rows + columns));
    std::normal_distribution<float> distribution(0.0F, scale);
    Vector weights(rows * columns, 0.0F);
    for (float& item : weights) {
        item = distribution(rng);
    }
    return weights;
}

inline std::size_t argmax(const Vector& values) {
    if (values.empty()) {
        throw std::invalid_argument("argmax requires a non-empty vector");
    }
    return static_cast<std::size_t>(
        std::distance(values.begin(), std::max_element(values.begin(), values.end())));
}

} // namespace osten::math

