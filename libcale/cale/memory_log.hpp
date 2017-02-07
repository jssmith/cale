#ifndef CALE_MEMORY_LOG_HPP
#define CALE_MEMORY_LOG_HPP

#include <algorithm>
#include <vector>

#include "cale/aliases.hpp"
#include "cale/error.hpp"
#include "cale/log.hpp"

namespace cale {

/// An unbounded in-memory log.
template <class T>
class memory_log : public log<T> {
public:
  caf::expected<offset> append(const T& x) override {
    xs_.push_back(x);
    return xs_.size() - 1;
  }

  caf::expected<T> get(offset off) const override {
    if (off < xs_.size())
      return xs_[off];
    return ec::invalid_offset;
  }

  caf::expected<std::vector<T>> get(offset begin, offset end) const override {
    CAF_ASSERT(begin < end);
    if (end > xs_.size())
      return ec::invalid_offset;
    if (begin == 0 && end == xs_.size())
      return xs_;
    std::vector<T> result;
    result.reserve(end - begin);
    std::copy(xs_.begin() + begin, xs_.begin() + end,
              std::back_inserter(result));
    return result;
  }

private:
  std::vector<T> xs_;
};

} // namespace cale

#endif // CALE_LOG_HPP
