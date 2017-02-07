#include <caf/all.hpp>

#define CAF_SUITE topic
#include <caf/test/unit_test.hpp>

#include "cale/error.hpp"
#include "cale/log.hpp"
#include "cale/memory_log.hpp"
#include "cale/topic.hpp"

using namespace caf;
using namespace cale;

namespace {

struct actor_system_fixture {
  actor_system_fixture() : system{config}, self{system} {
  }

  auto error_handler() {
    return [&](const caf::error& e) {
      CAF_FAIL(system.render(e));
    };
  }

  caf::actor_system_config config;
  caf::actor_system system;
  caf::scoped_actor self;
};

behavior accumulator(event_based_actor* self) {
  return [=](stream<int>& in) {
    return self->add_sink(
      // input stream
      in,
      // initialize state
      [](int& x) {
        x = 0;
      },
      // processing step
      [](int& x, int y) {
        x += y;
      },
      // cleanup and produce result message
      [](int& x) -> int {
        return x;
      }
    );
  };
}

} // namespace <anonymous>

CAF_TEST_FIXTURE_SCOPE(topic_tests, actor_system_fixture)

CAF_TEST(basic topic operations) {
  auto t = self->spawn(topic<memory_log<int>>, "test");
  CAF_MESSAGE("append values");
  for (auto x : {10, 11, 12, 13, 14, 15})
    self->request(t, infinite, append_atom::value, x).receive(
      [=](offset off) {
        CAF_CHECK_EQUAL(off, static_cast<offset>(x - 10));
      },
      error_handler()
    );
  CAF_MESSAGE("perform a point lookup");
  self->request(t, infinite, get_atom::value, offset{2}).receive(
    [=](int x) {
      CAF_CHECK_EQUAL(x, 12);
    },
    error_handler()
  );
  self->request(t, infinite, get_atom::value, offset{42}).receive(
    [](int) {
      CAF_FAIL("no such offset");
    },
    [](const error& e) {
      CAF_CHECK_EQUAL(e, ec::invalid_offset);
    }
  );
  CAF_MESSAGE("perform a range lookup");
  // Composition: f * g = f(g(x))
  auto pipeline = self->spawn(accumulator) * actor_cast<actor>(t);
  self->send(pipeline, stream_atom::value, offset{4}, offset{7});
  CAF_MESSAGE("appending after pipeline started");
  self->request(t, infinite, append_atom::value, 20).receive(
    [=](offset off) {
      CAF_CHECK_EQUAL(off, offset{6});
    },
    error_handler()
  );
  CAF_MESSAGE("getting pipeline result");
  self->receive(
    [](int x) {
      CAF_CHECK_EQUAL(x, 14 + 15 + 20);
    },
    error_handler()
  );
}

CAF_TEST_FIXTURE_SCOPE_END()
