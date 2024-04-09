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
)

set_target_properties(ProbabilitiesLab3 PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)


find_package(Matplot++ REQUIRED)
target_link_libraries(ProbabilitiesLab3 PUBLIC Matplot++::matplot)


