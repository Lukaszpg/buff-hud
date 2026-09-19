cmake_minimum_required(VERSION 3.29)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    if(WIN32)
        project(BuffPanel VERSION 1.0.0 LANGUAGES CXX RC)
    else()
        project(BuffPanel VERSION 1.0.0 LANGUAGES CXX)
    endif()
elseif(WIN32)
    enable_language(RC)
endif()

if(NOT TARGET D2RLPlugin::D2RLPlugin AND NOT TARGET D2RLPluginV4::D2RLPluginV4)
    foreach(BUFF_PANEL_SDK IN ITEMS
        "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/PluginSDK"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/PluginSDK-V4"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/PluginSDK-v4"
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/PluginSDK")
        if(EXISTS "${BUFF_PANEL_SDK}/CMakeLists.txt"
                AND EXISTS "${BUFF_PANEL_SDK}/include/D2RLPlugin/version.h")
            file(READ "${BUFF_PANEL_SDK}/include/D2RLPlugin/version.h" SDK_VERSION_HEADER)
            if(SDK_VERSION_HEADER MATCHES "D2RL_PLUGIN_ABI_VERSION[ \t]+4([^0-9]|$)"
                    AND SDK_VERSION_HEADER MATCHES "D2RL_SDK_VERSION[ \t]+\"0[.]2[.][0-9]+\"")
                add_subdirectory("${BUFF_PANEL_SDK}" "${CMAKE_CURRENT_BINARY_DIR}/_deps/PluginSDK")
                break()
            endif()
        endif()
    endforeach()
endif()
if(NOT TARGET D2RLPlugin::D2RLPlugin AND NOT TARGET D2RLPluginV4::D2RLPluginV4)
    find_package(D2RLPlugin 0.2.0 CONFIG QUIET)
endif()
if(TARGET D2RLPlugin::D2RLPlugin)
    set(BUFF_PANEL_SDK_TARGET D2RLPlugin::D2RLPlugin)
elseif(TARGET D2RLPluginV4::D2RLPluginV4)
    set(BUFF_PANEL_SDK_TARGET D2RLPluginV4::D2RLPluginV4)
else()
    message(FATAL_ERROR "Buff Panel requires official D2RLoader PluginSDK 0.2.x (plugin ABI 4). Update Suite third_party/PluginSDK.")
endif()

file(READ
    "${CMAKE_CURRENT_SOURCE_DIR}/runtime-data/data/global/excel/d2rloader/buff-panel/buff-hud.txt"
    BUFF_PANEL_DEFAULT_BUFF_HUD)
configure_file(src/systems/buff_tracker/default_buff_hud_table.hpp.in
    "${CMAKE_CURRENT_BINARY_DIR}/generated/default_buff_hud_table.hpp" @ONLY)

file(READ
    "${CMAKE_CURRENT_SOURCE_DIR}/runtime-data/data/global/ui/layouts/buff-panel/BuffHudhd.json"
    BUFF_PANEL_LAYOUT)
set(BUFF_PANEL_CHUNK 4096)
string(LENGTH "${BUFF_PANEL_LAYOUT}" BUFF_PANEL_LAYOUT_LENGTH)
set(BUFF_PANEL_LAYOUT_LITERALS "")
set(BUFF_PANEL_OFFSET 0)
while(BUFF_PANEL_OFFSET LESS BUFF_PANEL_LAYOUT_LENGTH)
    math(EXPR BUFF_PANEL_REMAINING "${BUFF_PANEL_LAYOUT_LENGTH} - ${BUFF_PANEL_OFFSET}")
    if(BUFF_PANEL_REMAINING GREATER BUFF_PANEL_CHUNK)
        set(BUFF_PANEL_THIS_CHUNK ${BUFF_PANEL_CHUNK})
    else()
        set(BUFF_PANEL_THIS_CHUNK ${BUFF_PANEL_REMAINING})
    endif()
    string(SUBSTRING "${BUFF_PANEL_LAYOUT}" ${BUFF_PANEL_OFFSET}
        ${BUFF_PANEL_THIS_CHUNK} BUFF_PANEL_LAYOUT_PART)
    string(APPEND BUFF_PANEL_LAYOUT_LITERALS
        "R\"buffpaneljson(${BUFF_PANEL_LAYOUT_PART})buffpaneljson\"\n")
    math(EXPR BUFF_PANEL_OFFSET "${BUFF_PANEL_OFFSET} + ${BUFF_PANEL_THIS_CHUNK}")
endwhile()
configure_file(src/systems/buff_hud/buff_hud_layout.hpp.in
    "${CMAKE_CURRENT_BINARY_DIR}/generated/buff_hud_layout.hpp" @ONLY)

add_library(buff_panel SHARED
    src/plugin.cpp
    src/core/services.cpp
    src/core/hook_registry.cpp
    src/core/stat_list_post_bus.cpp
    src/core/stat_read_bus.cpp
    src/core/buff_display_bus.cpp
    src/systems/buff_tracker/buff_tracker.cpp
    src/systems/buff_hud/buff_hud.cpp
    src/systems/buff_hud/icon_frame_backend.cpp
    src/systems/buff_hud/skill_icon_resolver.cpp
)
if(WIN32)
    target_sources(buff_panel PRIVATE src/plugin.rc)
endif()
target_include_directories(buff_panel PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_BINARY_DIR}/generated")
target_link_libraries(buff_panel PRIVATE ${BUFF_PANEL_SDK_TARGET})
target_compile_features(buff_panel PRIVATE cxx_std_20)
# Windows.h defines min/max as function-like macros unless NOMINMAX is set.
# Without this, PluginSDK item.h and our numeric_limits<>::min/max calls
# are mangled by the preprocessor (MSVC C4003/C2589/C2059).
# Apply to *every* Buff Panel translation unit before it includes Windows.h.
if(WIN32)
    target_compile_definitions(buff_panel PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
endif()
set_target_properties(buff_panel PROPERTIES OUTPUT_NAME "d2rl-buff-panel")
if(MSVC)
    target_compile_options(buff_panel PRIVATE /W4 /permissive- /utf-8)
endif()
