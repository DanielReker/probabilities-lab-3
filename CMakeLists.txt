cmake_minimum_required (VERSION 3.12)


# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ProbabilitiesLab3")

add_executable (ProbabilitiesLab3
    "src/main.cpp"
    "src/Controller/Controller.h"
    "src/View/Histogram.h"
    "src/Model/ContinuousDistribution.h"
    "src/Model/IDistribution.h"
    "src/Model/ExponentialDistribution.h"
 "src/Model/NormalDistribution.h")

set_target_properties(ProbabilitiesLab3 PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)


# Import matplot++
find_package(Matplot++ REQUIRED)
target_link_libraries(ProbabilitiesLab3 PRIVATE Matplot++::matplot)


# Import nlohmann/json
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)
target_link_libraries(ProbabilitiesLab3 PRIVATE nlohmann_json::nlohmann_json)


# Copy config.json to bin directory
configure_file(data/config.json config.json COPYONLY)