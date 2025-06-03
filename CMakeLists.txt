
cmake_minimum_required(VERSION 3.22)






set (JUCE_LIBRARY_PATH ../JUCE)

project(Otto VERSION 0.0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory(${JUCE_LIBRARY_PATH} ${CMAKE_CURRENT_BINARY_DIR}/JUCE)
add_subdirectory(libs/JUCECC)
add_subdirectory(libs/melatonin_inspector)
juce_set_aax_sdk_path(${JUCE_LIBRARY_PATH}/modules/juce_audio_plugin_client/AAX/SDK)

juce_add_plugin(Otto
    # ICON_SMALL ...
    PLUGIN_CHANNEL_CONFIGURATIONS "1, 2"  # Example: mono and stereo
    COMPANY_NAME Hippasus
    IS_SYNTH FALSE                       # Is this a synth or an effect?
    NEEDS_MIDI_INPUT FALSE               # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE              # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                 # Is this plugin a MIDI effect?
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE    # Does the editor need keyboard focus?
    PLUGIN_MANUFACTURER_CODE Hipp # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Otto # A unique four-character plugin id with exactly one upper-case character
    FORMATS AAX VST3                   # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME Otto)        # The name of the final executable, which can differ from the target name

target_sources(Otto
    PRIVATE
    Source/PluginEditor.cpp
    Source/PluginProcessor.cpp
    Source/FilterFollower.cpp)

target_compile_definitions(Otto
    PUBLIC
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0)


# juce_add_binary_data(AudioPluginData SOURCES ...)
if(MSVC)
    set_property(TARGET Otto_VST3 PROPERTY VS_DEBUGGER_COMMAND $<$<CONFIG:Debug>:../Juce/AudioPluginHost.exe>)
    set_property(TARGET Otto_VST3 PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<$<CONFIG:Debug>:${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>>)
endif()

target_link_libraries(Otto
    PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_basics
    juce::juce_gui_basics
    juce::juce_graphics
    juce::juce_events
    juce::juce_dsp
    JUCECC
    melatonin_inspector
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
