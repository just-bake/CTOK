file(GLOB SOURCES *.c)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}_test ${SOURCES})
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})

add_test(NAME TokenizerBasic COMMAND test_tokenizer)
