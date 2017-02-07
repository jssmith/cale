#ifndef CALE_LOG_HPP
#define CALE_LOG_HPP

#include <caf/expected.hpp>
#include <caf/typed_actor.hpp>

#include "cale/aliases.hpp"
#include "cale/atoms.hpp"

namespace cale {

/// The abstract base class for log implementations.
template <class T>
class log {
public:
  using value_type = T;

  /// Appends an element to the back.
  /// @param x The element to append.
  /// @returns The new offset of *x*.
  virtual caf::expected<offset> append(const T& x) = 0;

  /// Retrieves an element.
  /// @param off The offset to retrieve.
  /// @returns The element at offset *off*.
  virtual caf::expected<T> get(offset off) const = 0;

  /// Retrieves a range of elements.
  /// @param begin The first offset to retrieve.
  /// @param end The offset one-past-the last element to retrieve.
  /// @returns The elements in *[first, last)*.
  /// @pre `begin < end`
  virtual caf::expected<std::vector<T>> get(offset begin, offset end) const = 0;
};

} // namespace cale

#endif // CALE_LOG_HPP
