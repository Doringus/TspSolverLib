set(BINARY ${CMAKE_PROJECT_NAME})

set(HEADERS blockingqueue.h branchandbound.h matrix.h staticthreadpool.h tspsolverlib.h matrixindiceswrapper.h node.h)

add_library(${BINARY}_lib INTERFACE)
target_sources(${BINARY}_lib INTERFACE ${HEADERS})