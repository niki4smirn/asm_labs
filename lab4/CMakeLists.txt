cmake_minimum_required(VERSION 3.7)

project(asm_lab)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

if (MINGW)
    set(THREADS_OPTION "-mthreads")
else (MINGW)
    set(THREADS_OPTION "-pthread")
endif (MINGW)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-no-pie")

enable_language(ASM_NASM)

add_executable(solution_tests Lab4_Smirnov_Nikita_Unix.asm tests.cpp)

target_link_libraries(solution_tests gtest_main)
