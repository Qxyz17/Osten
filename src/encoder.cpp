#include "osten/encoder.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace osten {
namespace {

std::uint64_t fnv1a(const std::string& value) {
    std::uint64_t hash = 1469598103934665603ULL;
    for (unsigned char character : value) {
        hash ^= character;
        hash *= 1099511628211ULL;
    }
    return hash;
}

std::string normalize(const std::string& input) {
    std::string result;
    result.reserve(input.size());
    bool previous_space = false;
    for (unsigned char character : input) {
        const bool space = character <= 32U;
        if (space) {
            if (!previous_space) {
                result.push_back(' ');
            }
        } else {
            result.push_back(static_cast<char>(character));
        }
        previous_space = space;
    }
    if (!result.empty() && result.back() == ' ') {
        result.pop_back();
    }
    return result;
}

} // namespace

CharacterNGramEncoder::CharacterNGramEncoder(std::size_t dimension,
                                             std::size_t min_n,
                                             std::size_t max_n)
    : dimension_(dimension), min_n_(min_n), max_n_(max_n) {
    if (dimension_ == 0 || min_n_ == 0 || max_n_ < min_n_) {
        throw std::invalid_argument("invalid character n-gram encoder configuration");
    }
}

Vector CharacterNGramEncoder::encode(const std::string& text) const {
    Vector result(dimension_, 0.0F);
    const std::string value = normalize(text);
    if (value.empty()) {
        return result;
    }

    const std::string padded = "^" + value + "$";
    std::size_t count = 0;
    for (std::size_t n = min_n_; n <= max_n_; ++n) {
        if (n > padded.size()) {
            break;
        }
        for (std::size_t index = 0; index + n <= padded.size(); ++index) {
            const std::string gram = padded.substr(index, n);
            const std::uint64_t hash = fnv1a(gram);
            const std::size_t bucket = static_cast<std::size_t>(hash % dimension_);
            result[bucket] += (hash & 1ULL) != 0ULL ? 1.0F : -1.0F;
            ++count;
        }
    }

    if (count != 0) {
        const float scale = 1.0F / std::sqrt(static_cast<float>(count));
        for (float& item : result) {
            item *= scale;
        }
    }
    float norm = 0.0F;
    for (float item : result) {
        norm += item * item;
    }
    norm = std::sqrt(norm);
    if (norm > 1.0F) {
        for (float& item : result) {
            item /= norm;
        }
    }
    return result;
}

} // namespace osten

