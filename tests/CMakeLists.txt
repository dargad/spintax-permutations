set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

if(Boost_FOUND)
    enable_testing()

    file(GLOB TEST_SRCS ./*.cpp)
    set(TEST_SRCS ${TEST_SRCS})

    include_directories(${PROJECT_SOURCE_DIR}/src)
    add_executable(tests EXCLUDE_FROM_ALL ${TEST_SRCS})
    target_link_libraries(tests
                          spintax
                          ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                          )
    add_test(NAME test0 COMMAND tests test0.txt 16 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests/data)
    add_test(NAME test1 COMMAND tests test1.txt 80 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests/data)
    add_test(NAME test2 COMMAND tests test2.txt 40600 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests/data)
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS tests)

endif()

