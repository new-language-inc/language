add_library(lexer STATIC lexer.cpp lexer.hpp)
target_include_directories(lexer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(lexer PUBLIC cxx_std_20)
target_link_libraries(lexer PRIVATE token)