#ifndef CALE_ATOMS_HPP
#define CALE_ATOMS_HPP

#include <caf/atom.hpp>

namespace cale {

using caf::get_atom;
using caf::ok_atom;

using append_atom = caf::atom_constant<caf::atom("append")>;
using stream_atom = caf::atom_constant<caf::atom("stream")>;

} // namespace cale

#endif // CALE_ATOMS_HPP
