
cmake_minimum_required(VERSION 3.22)

file(GLOB_RECURSE SOURCES
    "Processors/*.cpp"
    "UIComponents/*.cpp"
    "Utils/*.cpp"
)

add_library(JUCECC STATIC
    ${SOURCES}
)

target_include_directories(JUCECC PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Processors
    ${CMAKE_CURRENT_SOURCE_DIR}/UIComponents
    ${CMAKE_CURRENT_SOURCE_DIR}/Utils
)

target_link_libraries(JUCECC PRIVATE
    juce::juce_core
    juce::juce_gui_basics
    juce::juce_audio_basics
)
