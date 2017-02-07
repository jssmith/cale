#define CAF_SUITE log
#include <caf/test/unit_test.hpp>

#include "cale/memory_log.hpp"

using namespace caf;
using namespace cale;

namespace {

struct memory_log_fixture {
  memory_log_fixture() {
    for (auto x : {42, 43, 44, 45, 46}) {
      auto off = log.append(x);
      CAF_REQUIRE(off);
      CAF_CHECK_EQUAL(*off, static_cast<offset>(x - 42));
    }
  }

  memory_log<int> log;
};

} // namespace <anonymous>

CAF_TEST_FIXTURE_SCOPE(memory_log_tests, memory_log_fixture)

CAF_TEST(memory log - get) {
  CAF_MESSAGE("invalid offset");
  auto x = log.get(7);
  CAF_REQUIRE(!x);
  CAF_CHECK_EQUAL(x, ec::invalid_offset);
  x = log.get(3);
  CAF_REQUIRE(x);
  CAF_CHECK_EQUAL(x, 42 + 3);
}

CAF_TEST(memory log - range get) {
  CAF_MESSAGE("out of bounds access");
  auto xs = log.get(2, 10);
  CAF_REQUIRE(!xs);
  CAF_CHECK_EQUAL(xs, ec::invalid_offset);
  xs = log.get(1, 5);
  CAF_REQUIRE(xs);
  CAF_CHECK_EQUAL(*xs, (std::vector<int>{43, 44, 45, 46}));
}

CAF_TEST_FIXTURE_SCOPE_END()
