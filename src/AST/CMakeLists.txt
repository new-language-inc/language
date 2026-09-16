add_library(ast STATIC ast.cpp ast.hpp)
target_include_directories(ast PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(ast PUBLIC cxx_std_20)
target_link_libraries(ast PUBLIC token)
