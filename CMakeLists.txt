# Dynamic compile-commands.json generation CMake config.
# 
# Copyright (c) 2018 Duality Blockchain Solutions Developers
#
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
project(dynamic LANGUAGES CXX C)

cmake_minimum_required(VERSION 3.7)

enable_language(CXX)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy" DOC "Path to clang-tidy executable")

if(NOT CLANG_TIDY_EXE)
  message(ERROR "clang-tidy not found.")
else()
  message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
  set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=-*,clang-analyzer-*,-clang-analyzer-cplusplus*")
endif()

file(GLOB DYNAMIC_SOURCES
  src/dynamic-config.h
  src/*.h
  src/*.cpp
  src/bdap/*.h
  src/bdap/*.cpp
  src/compat/*.h
#  src/compat/*.cpp
  src/consensus/*.h
  src/consensus/*.cpp
  src/crypto/*.h
  src/crypto/*.cpp
  src/crypto/argon2d/*.h
  src/crypto/argon2d/*.c
  src/crypto/argon2gpu/*.h
  src/crypto/argon2gpu/*.cpp
#  src/crypto/argon2gpu/cuda/*.h
#  src/crypto/argon2gpu/cuda/*.cpp
#  src/crypto/argon2gpu/opencl/*.h
#  src/crypto/argon2gpu/opencl/*.cpp
  src/crypto/blake2/*.h
  src/crypto/blake2/*.c
  src/fluid/*.h
  src/fluid/*.cpp
  src/miner/*.h
  src/miner/*.cpp
  src/miner/internal/*.h
  src/miner/internal/*.cpp
  src/policy/*.h
  src/policy/*.cpp
  src/primitives/*.h
  src/primitives/*.cpp
  src/script/*.h
  src/script/*.cpp
  src/secp256k1
  src/support/*.h
  src/support/*.cpp
  src/support/allocators/*.h
  src/wallet/*.h
  src/wallet/*.cpp
  src/zmq/*.h
  src/zmq/*.cpp
)

list(FILTER DYNAMIC_SOURCES EXCLUDE REGEX "src/*_test.cpp$")

find_package(Boost REQUIRED COMPONENTS filesystem program_options thread chrono locale)

include_directories(${Boost_INCLUDE_DIRS}
  src/
  src/config/
  src/leveldb/include
  src/leveldb/helpers/memenv
  src/obj
  src/secp256k1/include
  src/univalue/include
)

add_compile_definitions(HAVE_CONFIG_H)

add_executable(dynamicd    src/dynamicd.cpp    ${DYNAMIC_SOURCES})
add_executable(dynamic-tx  src/dynamic-tx.cpp  ${DYNAMIC_SOURCES})
add_executable(dynamic-cli src/dynamic-cli.cpp ${DYNAMIC_SOURCES})

set_target_properties(dynamicd    PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
