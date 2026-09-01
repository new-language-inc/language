enable_testing()
add_executable(language_tests test_main.cpp test_lexer.cpp)
target_link_libraries(language_tests PRIVATE lexer token)
target_compile_features(language_tests PRIVATE cxx_std_20)

add_test(NAME language_tests COMMAND language_tests)
