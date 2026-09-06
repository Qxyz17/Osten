#pragma once

#include <cstddef>
#include <string>

#include "osten/types.hpp"

namespace osten {

class CharacterNGramEncoder {
public:
    explicit CharacterNGramEncoder(std::size_t dimension = 64,
                                   std::size_t min_n = 1,
                                   std::size_t max_n = 3);

    Vector encode(const std::string& text) const;
    std::size_t dimension() const noexcept { return dimension_; }

private:
    std::size_t dimension_;
    std::size_t min_n_;
    std::size_t max_n_;
};

} // namespace osten

