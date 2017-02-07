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
    return off_++;
  }

  caf::expected<T> get(offset off) const override {
    if (off < off_)
      return xs_[off];
    return ec::invalid_offset;
  }

  caf::expected<std::vector<T>> get(offset begin, offset end) const override {
    CAF_ASSERT(begin < end);
    auto n = end - begin;
    if (n > off_)
      return ec::invalid_offset;
    std::vector<T> result;
    result.reserve(end - begin);
    std::copy_n(xs_.begin() + begin, n, std::back_inserter(result));
    return result;
  }

private:
  offset off_ = 0;
  std::deque<T> xs_;
};

} // namespace cale

#endif // CALE_LOG_HPP
