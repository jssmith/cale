#ifndef CALE_ERROR_HPP
#define CALE_ERROR_HPP

#include <caf/error.hpp>
#include <caf/make_message.hpp>

namespace cale {

using caf::error;

/// CALE's error codes.
enum class ec : uint8_t {
  /// The unspecified default error code.
  unspecified = 1,
  /// An error while accessing the filesystem.
  filesystem_error,
  /// A topic has no message for the requested offset.
  invalid_offset,
};

/// @relates ec
const char* to_string(ec x);

/// @relates ec
template <class... Ts>
error make_error(ec x, Ts&&... xs) {
  return error{static_cast<uint8_t>(x), caf::atom("cale"),
               caf::make_message(std::forward<Ts>(xs)...)};
}

} // namespace cale

#endif

