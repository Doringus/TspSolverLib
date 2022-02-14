set(BINARY ${CMAKE_PROJECT_NAME})

set(HEADERS threading/blockingqueue.h bb/branchandbound.h matrix.h threading/staticthreadpool.h
        tspsolverlib.h bb/matrixindiceswrapper.h bb/node.h ga/crossovers.h)

add_library(${BINARY}_lib INTERFACE)
target_sources(${BINARY}_lib INTERFACE ${HEADERS})