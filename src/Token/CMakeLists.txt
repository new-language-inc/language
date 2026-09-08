add_library(token STATIC token.cpp token.hpp)
target_include_directories(token PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(token PUBLIC cxx_std_20)