file(GLOB SOURCES *.c)

add_library(${PROJECT_NAME} ${SOURCES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include ${PCRE2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE pcre2-8)
