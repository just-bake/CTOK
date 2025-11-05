include(FetchContent)

FetchContent_Declare(
    pcre2
    GIT_REPOSITORY https://github.com/PhilipHazel/pcre2.git
    GIT_TAG        pcre2-10.47
)

FetchContent_MakeAvailable(pcre2)
