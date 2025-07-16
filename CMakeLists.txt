cmake_minimum_required(VERSION 3.16)
project(miseriesWorld)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(WIN32)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configurations" FORCE)
    
    foreach(config IN LISTS CMAKE_CONFIGURATION_TYPES)
        string(TOUPPER ${config} config_upper)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config_upper} ${CMAKE_BINARY_DIR}/bin/${config})
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config_upper} ${CMAKE_BINARY_DIR}/bin/${config})
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config_upper} ${CMAKE_BINARY_DIR}/lib/${config})
    endforeach()
endif()

set(SDL2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2/cmake")
set(SDL2_image_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2_image/cmake") 
set(SDL2_mixer_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2_mixer/cmake")
set(SDL2_ttf_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2_ttf/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h src/*.hpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    src
    thirdparty/SDL2/include
    thirdparty/SDL2_image/include
    thirdparty/SDL2_ttf/include
    thirdparty/SDL2_mixer/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2::SDL2
    SDL2::SDL2main 
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
)

add_library(SDL2_gfx STATIC IMPORTED)
set_target_properties(SDL2_gfx PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2/lib/x64/SDL2_gfx.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2/include"
)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_gfx)

if(WIN32)
    set(DLL_DESTINATION $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2/lib/x64/SDL2.dll"
            "${DLL_DESTINATION}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2/lib/x64/SDL2_gfx.dll"
            "${DLL_DESTINATION}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2_image/lib/x64/SDL2_image.dll"
            "${DLL_DESTINATION}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2_mixer/lib/x64/SDL2_mixer.dll"
            "${DLL_DESTINATION}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/SDL2_ttf/lib/x64/SDL2_ttf.dll"
            "${DLL_DESTINATION}"
    )
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${SOURCES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Header Files" FILES ${HEADERS})