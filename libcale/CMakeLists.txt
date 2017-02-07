# ----------------------------------------------------------------------------
#                                   libcale
# ----------------------------------------------------------------------------

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# TODO: uncomment once we have a binary
set(libcale_sources
  src/error.cpp
)

set(libcale_libs ${CAF_LIBRARIES})

if (CALE_ENABLE_ASSERTIONS)
  set(libcale_libs ${libcale_libs} ${Backtrace_LIBRARIES})
endif ()

add_library(libcale SHARED ${libcale_sources})
set_target_properties(libcale
  PROPERTIES
  SOVERSION ${VERSION_MAJOR}
  VERSION ${VERSION_MAJOR}.${VERSION_MINOR}
  OUTPUT_NAME cale)

target_link_libraries(libcale ${libcale_libs})

# Install libcale in PREFIX/lib and headers in PREFIX/include/cale.
install(TARGETS libcale DESTINATION lib)
install(DIRECTORY cale/
        DESTINATION include/cale FILES_MATCHING PATTERN "*.hpp")

# ----------------------------------------------------------------------------
#                                 unit tests
# ----------------------------------------------------------------------------

include_directories(
  ${CMAKE_SOURCE_DIR}/libcale
  ${CMAKE_BINARY_DIR}/libcale
  ${CMAKE_CURRENT_SOURCE_DIR}/test
  ${CMAKE_CURRENT_BINARY_DIR}/test)

set(tests
  test/log.cpp
  test/main.cpp
  test/topic.cpp
)

add_executable(cale-test ${tests})
target_link_libraries(cale-test libcale ${CAF_LIBRARIES})

#
# Generate CMake tests
#

# Helper macro to construct a CMake test from a CALE test suite.
macro(make_test suite)
  string(REPLACE " " "_" test_name ${suite})
  set(cale_test ${EXECUTABLE_OUTPUT_PATH}/cale-test)
  add_test(${test_name} ${cale_test} -v 3 -r 60 -s "^${suite}$" ${ARGN})
endmacro()

# Find all test suites.
foreach(test ${tests})
  file(STRINGS ${test} contents)
  foreach(line ${contents})
    if ("${line}" MATCHES "SUITE")
      string(REGEX REPLACE "#define CAF_SUITE \(.*\)" "\\1" suite ${line})
      list(APPEND suites ${suite})
    endif()
  endforeach()
endforeach()
list(REMOVE_DUPLICATES suites)

enable_testing()

foreach(suite ${suites})
  make_test("${suite}")
endforeach ()
