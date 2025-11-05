set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Ensure the file exists (create an empty file if it doesn't)
execute_process(
    COMMAND ${CMAKE_COMMAND} -E touch
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)
