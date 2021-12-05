find_package(GTest REQUIRED)
include(GoogleTest)

set(${GTEST_SHUFFLE} 1)

add_executable(test1 AlgebraicTest.cpp)
target_link_libraries(test1 algebraic GTest::GTest GTest::Main)
include_directories(${PROJECT_SOURCE_DIR}/lib ${GTEST_INCLUDE_DIRS})

gtest_discover_tests(test1 EXTRA_ARGS ${GTEST_SHUFFLE} 1 "--gtest_shuffle")
