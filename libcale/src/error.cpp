#include "cale/error.hpp"

namespace cale {
namespace {

const char* descriptions[] = {
  "unspecified",
  "filesystem_error",
  "invalid_offset",
};

} // namespace <anonymous>

const char* to_string(ec x) {
  auto index = static_cast<size_t>(x) - 1;
  CAF_ASSERT(index < sizeof(descriptions));
  return descriptions[index];
}

} // namespace cale
