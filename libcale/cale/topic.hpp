#ifndef CALE_TOPIC_HPP
#define CALE_TOPIC_HPP

#include <caf/typed_actor.hpp>
#include <caf/stream.hpp>

#include "cale/aliases.hpp"
#include "cale/atoms.hpp"
#include "cale/log.hpp"

namespace cale {

template <class T>
struct topic_state {
  std::unique_ptr<cale::log<T>> log;
  std::string name = "<topic>";
};

/// The topic actor interface.
template <class T>
using topic_actor = caf::typed_actor<
  typename caf::replies_to<append_atom, T>::
    template with<offset>,
  typename caf::replies_to<get_atom, offset>::
    template with<T>,
  typename caf::replies_to<stream_atom, offset, offset>::
    template with<caf::stream<T>>
>;

/// Spawns a topic actor.
/// @param self The actor handle.
/// @param name The name of the topic.
template <class Log, class T = typename Log::value_type>
typename topic_actor<T>::behavior_type
topic(typename topic_actor<T>::template stateful_pointer<topic_state<T>> self,
      std::string name) {
  using namespace caf;
  self->state.name = std::move(name);
  self->state.log = std::make_unique<Log>();
  return {
    [=](append_atom, const T& x) {
      auto result = self->state.log->append(x);
      if (result)
        self->trigger_downstreams();
      return result;
    },
    [=](get_atom, offset off) {
      return self->state.log->get(off);
    },
    [=](stream_atom, offset begin, offset end) -> stream<T> {
      return self->add_source(
        // initialize state: the stream offset in [begin, end)
        [=](offset& pos) {
          pos = begin;
        },
        // get next elements
        [=](offset& pos, downstream<T>& out, size_t possible) {
          auto available = end - pos;
          auto n = std::min(offset{possible}, available);
          if (n == 0)
            return;
          auto x = self->state.log->get(pos, pos + n);
          if (!x) {
            out.abort(self->current_sender(), x.error());
          } else {
            std::move(x->begin(), x->end(), std::back_inserter(out.buf()));
            pos += n;
          }
        },
        // check whether we reached the end
        [=](const offset& pos) {
          return pos == end;
        }
      );
    }
  };
}

} // namespace cale

#endif // CALE_TOPIC_HPP
