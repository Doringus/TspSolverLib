include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

set(BINARY ${CMAKE_PROJECT_NAME}_tst)

set(SOURCES main.cpp matrix/matrixtst.cpp bb/reduceplainarraytst.cpp bb/reducematrixtst.cpp
        bb/getminexcepttst.cpp bb/penaltiestst.cpp bb/branchingtst.cpp bb/solvertst.cpp)

add_executable(${BINARY} ${SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

#include_directories(${PROJECT_SOURCE_DIR}/src)
target_link_libraries(${BINARY} PUBLIC gtest gmock)
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib)