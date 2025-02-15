cmake_minimum_required(VERSION 3.31)
project(ILP VERSION 0.1.0 LANGUAGES C CXX)


# aux_source_directory(${PROJECT_SOURCE_DIR}/utils UTILS_SRC)

# file(GLOB SCRAPERS_SRC "${PROJECT_SOURCE_DIR}/scrapers/*.cpp")

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr
    GIT_TAG        dec9422db3af470641f8b0d90e4b451c4daebf64) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
FetchContent_MakeAvailable(fmt)

# FetchContent_Declare(
#     libxml2
#     GIT_REPOSITORY https://github.com/GNOME/libxml2
#     GIT_TAG        1c82bca6bd23d0f0858d7fc228ec3a91fda3e0e2)
# FetchContent_MakeAvailable(libxml2)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    SQLiteCpp
    GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp)
FetchContent_MakeAvailable(SQLiteCpp)

FetchContent_Declare(
    cli11_proj
    QUIET
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.3.2
)
FetchContent_MakeAvailable(cli11_proj)

# add_executable(ILP main.cpp ${UTILS_SRC})
add_executable(ILP main.cpp)

target_link_libraries(ILP PRIVATE cpr::cpr)
target_link_libraries(ILP PRIVATE fmt::fmt)
target_link_libraries(ILP PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(ILP PRIVATE SQLiteCpp)
target_link_libraries(ILP PRIVATE CLI11::CLI11)
# target_link_libraries(ILP PRIVATE libxml2::libxml2)

# SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O2 -Wall -g -ggdb")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
