# Some compilation defaults specific to this directory

# The following is needed for some SSE stuff
# set (CMAKE_C_FLAGS   "-flax-vector-conversions ${CMAKE_C_FLAGS}")

include_directories (.)
#include_directories (${CMAKE_CURRENT_BINARY_DIR})
link_directories (${CMAKE_CURRENT_BINARY_DIR}/ecm)

# ugly (copied from linalg/bwc/CMakeLists.txt...)
add_executable (makefb makefb.c)
target_link_libraries (makefb utils)

add_library (trialdiv OBJECT trialdiv.cpp)

add_executable(freerel freerel.cpp fb.cpp)
target_link_libraries(freerel utils)
set_target_properties(freerel PROPERTIES COMPILE_FLAGS "${OpenMP_CXX_FLAGS}")
set_target_properties(freerel PROPERTIES LINK_FLAGS "${OpenMP_CXX_FLAGS}")

# las_core_a consists of all the object files that are necessary to build
# a las_info object. It is probably more than what would be strictly
# necessary, ideally. Everything extra is burden that we needlessly
# embark in dupsup.
set(las_core_a_sources
        las-info.cpp
        las-memory.cpp
        las-parallel.cpp
        las-sieve-shared-data.cpp
        fb.cpp
        las-choose-sieve-area.cpp
        las-dlog-base.cpp
        las-cofactor.cpp
        las-siever-config.cpp
        las-config.c
        las-norms.cpp
        las-threads.cpp
        las-auxiliary-data.cpp
        las-report-stats.cpp
        las-trialdiv.cpp
        las-bkmult.cpp
        logapprox.cpp
        las-todo-list.cpp
        las-todo-entry.cpp
        las-galois.cpp
        las-globals.cpp
        las-threads-work-data.cpp
        las-dumpfile.cpp
        las-coordinates.cpp
        las-duplicate.cpp
        las-plattice.cpp
        las-qlattice.cpp
        las-descent.cpp
        las-divide-primes.cpp
        las-sieve2357.cpp
)

set(las_core_b_sources
        las-arith.cpp
        las-descent-trees.cpp
        las-plattice.cpp
)
add_library(las_core_a OBJECT ${las_core_a_sources})
add_library(las_core_b OBJECT ${las_core_b_sources})

set(las_multi_a_sources
	bucket.cpp
	las-fill-in-buckets.cpp
	las-smallsieve.cpp
        las-cofac-standalone.cpp
        las-detached-cofac.cpp
	las-process-bucket-region.cpp
	las-output.cpp
        las-multiobj-globals.cpp
	las-unsieve.cpp
	las-unsieve-sse2.cpp
)
add_library(las_multi_a_prod OBJECT ${las_multi_a_sources})
add_library(las_multi_a_debug OBJECT ${las_multi_a_sources})
set_property(TARGET las_multi_a_debug PROPERTY COMPILE_DEFINITIONS TRACE_K=1)
add_library(las_multi_a_descent OBJECT ${las_multi_a_sources})
set_property(TARGET las_multi_a_descent PROPERTY COMPILE_DEFINITIONS DLP_DESCENT SUPPORT_LARGE_Q)

add_executable(las
    las-where-am-i-prod.cpp
    las.cpp
    $<TARGET_OBJECTS:las_multi_a_prod>
    $<TARGET_OBJECTS:las_core_b>
    $<TARGET_OBJECTS:las_core_a>
    $<TARGET_OBJECTS:trialdiv>
    )
target_link_libraries (las facul utils)
set_target_properties(las PROPERTIES LINK_FLAGS "${OpenMP_CXX_FLAGS}")

add_executable(las_tracek
    las-where-am-i-debug.cpp
    las.cpp
    $<TARGET_OBJECTS:las_multi_a_debug>
    $<TARGET_OBJECTS:las_core_b>
    $<TARGET_OBJECTS:las_core_a>
    $<TARGET_OBJECTS:trialdiv>
    )
target_link_libraries (las_tracek facul utils)
set_target_properties(las_tracek PROPERTIES LINK_FLAGS "${OpenMP_CXX_FLAGS}")
set_property(TARGET las_tracek PROPERTY COMPILE_DEFINITIONS TRACE_K=1)

add_executable(las_descent
    las-where-am-i-prod.cpp
    las.cpp
    $<TARGET_OBJECTS:las_multi_a_descent>
    $<TARGET_OBJECTS:las_core_b>
    $<TARGET_OBJECTS:las_core_a>
    $<TARGET_OBJECTS:trialdiv>
    )
target_link_libraries (las_descent facul utils)
set_target_properties(las_descent PROPERTIES LINK_FLAGS "${OpenMP_CXX_FLAGS}")
set_property(TARGET las_descent PROPERTY COMPILE_DEFINITIONS DLP_DESCENT SUPPORT_LARGE_Q)

add_executable (fb_test EXCLUDE_FROM_ALL fb.cpp)
set_property(TARGET fb_test PROPERTY COMPILE_DEFINITIONS TESTDRIVE=1)
target_link_libraries (fb_test utils)

add_subdirectory(ecm)
add_subdirectory(strategies)

add_executable(dupsup dupsup.cpp
    las-where-am-i-prod.cpp
    $<TARGET_OBJECTS:las_multi_a_descent>
    $<TARGET_OBJECTS:las_core_a>
    $<TARGET_OBJECTS:trialdiv>
    )
target_link_libraries(dupsup facul)
set_target_properties(dupsup PROPERTIES LINK_FLAGS "${OpenMP_CXX_FLAGS}")

add_executable(fake_rels EXCLUDE_FROM_ALL fake_rels.cpp
    # I don't like this situation where there's a single object file that
    # I want to pull from a greater compound.
    las-todo-entry.cpp)
target_link_libraries(fake_rels utils_with_io)

install(TARGETS makefb las las_descent freerel RUNTIME DESTINATION ${LIBSUFFIX}/sieve) 

include(CMakeLists-nodist.txt OPTIONAL)
