cmake_minimum_required(VERSION 3.15)
project(skifteskift VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE from three directories up
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../JUCE ${CMAKE_BINARY_DIR}/JUCE)

# Enable JUCE_ENABLE_MODULE_SOURCE_GROUPS
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

# Create the plugin target
juce_add_plugin(skifteskift
    VERSION "1.0.0"
    COMPANY_NAME "dwrth"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE sK1f
    PLUGIN_CODE 5kiF
    FORMATS AU VST3
    PRODUCT_NAME "skifteskift"
)

# Generate JuceHeader.h
juce_generate_juce_header(skifteskift)

# Add source files
target_sources(skifteskift
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
)

# Link to JUCE modules
target_compile_definitions(skifteskift
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

# Link required JUCE modules
target_link_libraries(skifteskift
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_audio_plugin_client
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)