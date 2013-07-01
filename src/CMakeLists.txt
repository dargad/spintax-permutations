set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS program_options)

set(LIB_SRCS spintax.cpp errors.cpp)
set(SRCS main.cpp)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_library(spintax ${LIB_SRCS})
    add_executable(spintax-permutations ${SRCS})
    target_link_libraries(spintax-permutations ${Boost_LIBRARIES} spintax)
endif()

