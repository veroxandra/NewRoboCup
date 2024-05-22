cmake_minimum_required(VERSION 3.14)
project(runTests)

# C++20 como estándar
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Configuración de Minimal-Socket
set(BUILD_MinimalCppSocket_SAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    min_sock
    GIT_REPOSITORY https://github.com/andreacasalino/Minimal-Socket
    GIT_TAG master
)
FetchContent_MakeAvailable(min_sock)

# Configuración de GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Habilitar testing
enable_testing()

# Incluir directorios necesarios
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)

# Definir el ejecutable de pruebas
add_executable(
  runTests
  main.cpp
  funciones_test.cpp
)

# Vincular bibliotecas
target_link_libraries(
  runTests
  GTest::gtest_main
  MinimalSocket
)

# Para Windows: evitar la sobrescritura de la configuración del compilador/enlazador del proyecto principal
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(GoogleTest)
gtest_discover_tests(runTests)
