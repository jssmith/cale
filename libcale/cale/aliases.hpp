#ifndef CALE_ALIASES_HPP
#define CALE_ALIASES_HPP

#include <cstdint>
#include <limits>

namespace cale {

/// A unique record identifier into a stream.
using offset = uint64_t;

/// The offset denoting infinity.
constexpr auto infinity = std::numeric_limits<offset>::max();

} // namespace cale

#endif // CALE_ALIASES_HPP
