############# PROJECT SETTINGS #############
############################################

cmake_minimum_required(VERSION 3.25)

# This tells cmake we have goodies in the /cmake folder
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include (PamplejuceVersion)
include (CPM) # Modern concise way to add dependencies to your project
include(PamplejuceMacOS) # Configures universal binaries and decides which version of macOS to support
include(JUCEDefaults) # Couple tweaks that IMO should be JUCE defaults

set(CMAKE_UNITY_BUILD OFF)

# This is the internal name of the project and the name of JUCE's shared code target
# Note: This cannot have spaces (it may be 2024, but you can't have it all!)
# Worry not, JUCE's PRODUCT_NAME can have spaces (and is what DAWs display)
set(PROJECT_NAME "Particules")

# Worry not, JUCE's PRODUCT_NAME can have spaces (and is what DAWs will display)
# You can also just have it be the same thing as PROJECT_NAME
# You may want to append the major version on the end of this (and PROJECT_NAME) ala:
#   set(PROJECT_NAME "MyPlugin_v${MAJOR_VERSION}")
# Doing so enables major versions to show up in IDEs and DAWs as separate plugins
# allowing you to change parameters and behavior without breaking existing user projects
set(PRODUCT_NAME "Particules")

set(COMPANY_NAME "KaiMing") # Used for the MacOS bundle name and Installers

set(BUNDLE_ID "com.kaiming.particules") # Used for the MacOS bundle identifier (and signing)

set(FORMATS Standalone AU VST3 AUv3) # Valid choices: AAX Unity VST VST3 AU AUv3 Standalone

# Force JUCE to work with sccache
# instruct MSVC to embed debug info (/Z7) instead of emitting a .pdb
set(CMAKE_POLICY_DEFAULT_CMP0141        NEW      CACHE STRING "" FORCE)
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT Embedded CACHE STRING "" FORCE)
#set(CMAKE_BUILD_TYPE RelWithDebInfo) # doesnt work on visual studio 

project(${PROJECT_NAME} VERSION ${CURRENT_VERSION})


############## MACOS LINK-TIME OPTIMIZATION FIX ##############
##############################################################
# Bypass the Xcode 15 ld-prime linker bug when using LTO with Static Libraries
if(APPLE)
    execute_process(COMMAND ld -ld_classic -v
                    RESULT_VARIABLE LD_CLASSIC_SUPPORTED
                    OUTPUT_QUIET ERROR_QUIET)
                    
    if(LD_CLASSIC_SUPPORTED EQUAL 0)
        message(STATUS "macOS: Using ld_classic linker to prevent LTO crashes")
        add_link_options(-Wl,-ld_classic)
    endif()
endif()


############## JUCE ##############
##################################

# JUCE is setup as a submodule in the /JUCE folder
# Locally, you must run `git submodule update --init --recursive` once
# and later `git submodule update --remote --merge` to keep it up to date
# On Github Actions, this is done as a part of actions/checkout
add_subdirectory(JUCE)

############# MODULES #############
###################################

add_subdirectory(modules/clap-juce-extensions EXCLUDE_FROM_ALL) # Add CLAP format

# Add any other modules you want modules here, before the juce_add_plugin call
# juce_add_module(modules/my_module)

add_subdirectory (modules/melatonin_inspector)

# Define an option to turn profiling on/off
option(PARTICULES_ENABLE_PROFILING "Enable Tracy Profiler" OFF)

if(PARTICULES_ENABLE_PROFILING)
    add_subdirectory(modules/tracy)
endif()

############# RESOURCES #############
#####################################

file(GLOB_RECURSE ResourcesFiles CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/*")

list(FILTER ResourcesFiles EXCLUDE REGEX "/\\.DS_Store$")

juce_add_binary_data(BinaryDataAssets SOURCES ${ResourcesFiles})
set_target_properties(BinaryDataAssets PROPERTIES POSITION_INDEPENDENT_CODE TRUE)


############# PLUGIN TARGET #############
#########################################

# See `docs/CMake API.md` in the JUCE repo for all config options
juce_add_plugin("${PROJECT_NAME}"
    IS_SYNTH TRUE # it supress audio input busses
    NEEDS_MIDI_INPUT TRUE
    #ICON_BIG "${CMAKE_CURRENT_SOURCE_DIR}/packaging/icon.png"
    COMPANY_NAME "${COMPANY_NAME}"
    BUNDLE_ID "${BUNDLE_ID}"
    COPY_PLUGIN_AFTER_BUILD FALSE # On MacOS, plugin is copied to /Users/yourname/Library/Audio/Plug-Ins/
    # First character MUST be uppercase for AU formats
    PLUGIN_MANUFACTURER_CODE KAIM # A four-character plugin id
    # Note: this must have at least one upper-case character
    PLUGIN_CODE P001 # A unique four-character plugin id
    FORMATS "${FORMATS}"
    # The name of your final executable
    # This is how it's listed in the DAW
    # This can be different from PROJECT_NAME and can have spaces!
    # You might want to use v${MAJOR_VERSION} here once you go to v2...
    PRODUCT_NAME "${PRODUCT_NAME}"
    IS_STANDALONE TRUE)

juce_generate_juce_header(${PROJECT_NAME})

clap_juce_extensions_plugin(TARGET "${PROJECT_NAME}" CLAP_ID "${BUNDLE_ID}" CLAP_FEATURES audio-effect)

############# GUI APP TARGET #############
##########################################

set(PROJECT_GUI_APP_NAME "GuiApp")

juce_add_gui_app(${PROJECT_GUI_APP_NAME} PRODUCT_NAME ${PROJECT_GUI_APP_NAME}) # doesnt create any entry point

set_target_properties(${PROJECT_GUI_APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/gui") 


############### BASE LIB ###############
########################################

# add whats common to Core and Gui lib here

add_library(Base STATIC)

set_target_properties(Base PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

target_include_directories(Base PUBLIC  "${CMAKE_CURRENT_SOURCE_DIR}/source")

target_compile_definitions(Base PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    PARTICULES_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
    PARTICULES_VERSION="${CMAKE_PROJECT_VERSION}"
)

# cmake lose gtk lib on linux
if(LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-x11-3.0)
    target_include_directories(Base PUBLIC ${GTK3_INCLUDE_DIRS})
endif()

target_link_libraries(Base PUBLIC 
    juce::juce_core
    juce::juce_audio_formats # audio format manager
    juce::juce_audio_utils # audio thumbnail
)

file(GLOB_RECURSE BaseSources CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/utils/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/framework/state/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/framework/state/*.h"
) 

target_sources(Base PUBLIC 
    ${BaseSources}
    "${CMAKE_CURRENT_SOURCE_DIR}/source/framework/core/PluginParams.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/framework/core/Core.h"
)


############# CORE LIB #############
####################################

add_library(Core STATIC)

set_target_properties(Core PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

target_compile_features(Core PUBLIC cxx_std_20)

target_include_directories(Core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source")

file(GLOB_RECURSE CoreSources CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/dsp/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/dsp/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/framework/audio/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/framework/audio/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/framework/bridge/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/framework/bridge/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/utils/*.h"
)

#list(FILTER CoreSources EXCLUDE REGEX 
#    "/bridge/"
#    "GuiTypes.h")

#message(STATUS "=== SHARED_SOURCES ===")
#foreach(file ${CoreSources})
#    message(STATUS "  ${file}")
#endforeach()

target_sources(Core PUBLIC 
    ${CoreSources}  
    "${CMAKE_CURRENT_SOURCE_DIR}/source/framework/core/PluginTypes.h"
)

target_link_libraries(Core PUBLIC
    Base                          # dsp depend de juce::Random pour le random
    juce::juce_dsp
    juce::juce_audio_basics
    juce::juce_audio_processors) # pas de apvts dans dsp mais dans framework


# injection of Tracy in the Core lib
if(PARTICULES_ENABLE_PROFILING)
    target_link_libraries(Core PUBLIC TracyClient)
    target_compile_definitions(Core PUBLIC TRACY_ENABLE)
    target_include_directories(Core PUBLIC "${CMAKE_SOURCE_DIR}/modules/tracy/public")
endif()

########### GUI LIB ############
################################

add_library(Gui STATIC)
 
set_target_properties(Gui PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

target_compile_features(Gui PUBLIC cxx_std_20)

target_include_directories(Gui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source")

# gui depend de audio processor et de juce core pour juce::string
file(GLOB_RECURSE GuiSources CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/source/gui/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/gui/*.h"
)

target_sources(Gui PRIVATE 
    ${GuiSources}
    "${CMAKE_CURRENT_SOURCE_DIR}/source/framework/core/GuiTypes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/framework/bridge/FromUI.h"
)

target_link_libraries(Gui PUBLIC
    Base
    BinaryDataAssets
    melatonin_inspector

    juce::juce_gui_basics
    juce::juce_graphics
    juce::juce_events
    juce::juce_audio_processors) # obglié car on a des slider attachment de APVTS

########### SHAREDCODE ##########
#################################

# This lets us use our code in both the JUCE targets and our Test target
# Without running into ODR violations
add_library(SharedCode INTERFACE)

include(SharedCodeDefaults) # Enable fast math, C++20 and a few other target defaults

#file(GLOB_RECURSE SourceFiles CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h")
#target_sources(SharedCode PRIVATE ${SourceFiles})
#target_compile_features(SharedCode PUBLIC cxx_std_20) 
target_include_directories(SharedCode INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/source")

# This is where you can set preprocessor definitions for JUCE and your plugin
target_compile_definitions(SharedCode INTERFACE
    #JUCE_WEB_BROWSER=0  # If you set this to 1, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    #JUCE_USE_CURL=0     # If you set this to 1, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    #JUCE_VST3_CAN_REPLACE_VST2=0
    # JUCE_DISPLAY_SPLASH_SCREEN=0 # Uncomment if you are paying for a an Indie/Pro license or releasing under GPLv3
    CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}" # lets the app known if we're Debug or Release
    VERSION="${CURRENT_VERSION}"
    PRODUCT_NAME_WITHOUT_VERSION="${PROJECT_NAME}") # JucePlugin_Name is for some reason doesn't use the nicer PRODUCT_NAME

# Link to any other modules you added (with juce_add_module) here!
# Usually JUCE modules must have PRIVATE visibility
# See https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md#juce_add_module
# However, with Pamplejuce, you'll link modules to SharedCode with INTERFACE visibility
# This allows the JUCE plugin targets and the Tests target to link against it
target_link_libraries(SharedCode INTERFACE
    Core
    Gui
    
    juce::juce_audio_processors
    juce::juce_audio_devices
    juce::juce_audio_utils  

    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

target_sources(SharedCode INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/source/framework/PluginCore.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/framework/PluginCore.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/PluginEditor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/PluginEditor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/PluginProcessor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/PluginProcessor.h"
)

########## PLUGIN SOURCES ##########
####################################

#target_sources(${PROJECT_NAME} PRIVATE

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    JUCE_ASIO=1
    BUILDING_PLUGIN_APP=1
    BUILDING_GUI_APP=0
    PROJECT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SharedCode
    Gui)
    #BinaryDataAssets
    #melatonin_inspector)

    #juce::juce_audio_utils # transitively included by Core
    #juce::juce_audio_formats
    #juce::juce_audio_processors

    #juce::juce_recommended_config_flags
    #juce::juce_recommended_lto_flags
    #juce::juce_recommended_warning_flags)
    
# target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

########### GUI APP SOURCE ############
#######################################

# target_include_directories(${PROJECT_GUI_APP_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/source")

target_sources(${PROJECT_GUI_APP_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/source/gui_app/GuiMain.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/gui_app/GuiAppEditor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/gui_app/GuiAppEditor.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/gui_app/GuiAppProcessor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/gui_app/GuiAppProcessor.h") # need EngineState and UIState

target_compile_features(${PROJECT_GUI_APP_NAME} PRIVATE cxx_std_20)

target_link_libraries(${PROJECT_GUI_APP_NAME} PRIVATE
    Gui
    BinaryDataAssets

    juce::juce_audio_processors

    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags)

target_compile_definitions(${PROJECT_GUI_APP_NAME} PRIVATE
    BUILDING_PLUGIN_APP=0
    BUILDING_GUI_APP=1
    )


############## BUILD TOOLS ##############
#########################################

option(BUILD_TOOLS "Build visualization tools" OFF)

if(BUILD_TOOLS)
    add_executable(drawEnvelopeSVG "${CMAKE_CURRENT_SOURCE_DIR}/source/tools/DrawEnvelopeSVG.cpp")
    target_link_libraries(drawEnvelopeSVG PRIVATE Core)
    file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/resources/envelopes")
    #set_target_properties(drawEnvelopeSVG PROPERTIES)
    #RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/resources/envSvg"
    add_custom_command(TARGET drawEnvelopeSVG POST_BUILD
        COMMAND $<TARGET_FILE:drawEnvelopeSVG>
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/resources/envelopes"
        COMMENT "Generating SVG envelopes in resources...")
    set_target_properties(drawEnvelopeSVG PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/resources/envelopes"
    ) 
endif()

############# PCH ##############
################################

#target_precompile_headers(${PROJECT_NAME} PRIVATE 
#    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/source/framework/pch/pch_plugin.h>")

#target_precompile_headers(${PROJECT_GUI_APP_NAME} PRIVATE
#    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/source/framework/pch/pch_gui.h>")

############ OPTIONAL  ############
###################################

include(PamplejuceIPP) # IPP support, comment out to disable
include(Tests)
include(Benchmarks)
include(GitHubENV) # Output some config for CI (like our PRODUCT_NAME)
