cmake_minimum_required(VERSION 3.18.4 FATAL_ERROR)

set(CMAKE_CXX_STANDARD           11)
set(CMAKE_CXX_STANDARD_REQUIRED  YES)
set(CMAKE_SKIP_INSTALL_RULES     TRUE)

#
# NOTE: If you're building int2ssl using add_subdirectory() command,
#       see bottom of the file for the list of exported variables
#

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build (default: Debug)")
option(INT2SSL_BINARY        "If disabled, building int2ssl executable will be skipped" ON)
option(INT2SSL_BINARY_STATIC "If enabled, int2ssl will be built as static executable (UNIX only)" OFF)
option(INT2SSL_STRICT        "If enabled, compiler warnings are promoted to errors (UNIX only)" OFF)

project(int2ssl
    DESCRIPTION  "Script decompiler for Fallout 1 / Fallout 2 / sfall scripts"
    HOMEPAGE_URL "https://github.com/sfall-team/int2ssl/"
    LANGUAGES    CXX)

#
# Misc
#

# Base targets names
set(INT2SSL_BIN ${PROJECT_NAME})
set(INT2SSL_LIB ${INT2SSL_BIN}-lib)

include(CheckCXXCompilerFlag)
include(CheckLinkerFlag)

# int2ssl_build_option( target type guard required flag var )
#   target ...... name of the the target
#   type ........ string describing which CMake functions should be used to test and add <flag> to <target>
#                 "compile" ... test: check_cxx_compiler_flag()  add: target_compile_options()
#                 "link" ...... test: check_linker_flag()        add: target_link_options()
#   guard .....,. name of variable used to test if given flag should be tested
#                 if variable resolves to FALSE, function does nothing
#   required .... name of variable used to test if given flag is mandatory
#                 if flag is not found and variable resolves to TRUE, configuration stops with error message
#   flag, var ... see CMake documentation
#                 https://cmake.org/cmake/help/v3.18/module/CheckCXXCompilerFlag.html
#                 https://cmake.org/cmake/help/v3.18/module/CheckLinkerFlag.html
function(int2ssl_build_option target type guard required flag var)
    if(NOT ${guard})
        return()
    endif()

    if("${type}" STREQUAL "compile")
        check_cxx_compiler_flag(${flag} ${var})
    elseif("${type}" STREQUAL "link")
        check_linker_flag(CXX ${flag} ${var})
    else()
        message(AUTHOR_WARNING "Unknown flag type: ${type}")
        message(AUTHOR_WARNING "Valid flag types: compile, link")
        return()
    endif()

    if(${var})
        if("${type}" STREQUAL "compile")
            target_compile_options(${target} PRIVATE ${flag})
        elseif("${type}" STREQUAL "link")
            target_link_options(${target} PRIVATE ${flag})
        endif()
    else()
        if(${required})
            message(FATAL_ERROR "Required ${type} flag ${flag} not found")
            return()
        endif()
    endif()
endfunction()

# Add out-of-source build directory to git ignore list
if(NOT "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" AND NOT EXISTS "${PROJECT_BINARY_DIR}/.gitignore")
    file(GENERATE OUTPUT "${PROJECT_BINARY_DIR}/.gitignore" CONTENT "*")
endif()

#
# Library
#

add_library(${INT2SSL_LIB} STATIC)
target_sources(${INT2SSL_LIB}
    PUBLIC
        FalloutScript.h

    PRIVATE
        ${CMAKE_CURRENT_LIST_FILE}

        FalloutScript.cpp
        FalloutScriptDecompile.cpp
        FalloutScriptDefObject.cpp
        FalloutScriptDump.cpp
        FalloutScriptStore.cpp
        Namespace.cpp
        Namespace.h
        Node.cpp
        Node.h
        ObjectAttributes.h
        Opcode.cpp
        Opcode.h
        OpcodeAttributes.cpp
        ProcTable.cpp
        ProcTable.h
        StartupCode.cpp
        StartupCode.h
        Utility.cpp
        Utility.h

        Hacks/CMap.h
)

#
# Library test
# Used to check if external applications are able to safely link library target
#

if(NOT "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    file(GENERATE OUTPUT "${PROJECT_BINARY_DIR}/${INT2SSL_LIB}-test.cpp" CONTENT "#include <FalloutScript.h>\nint main(){ CFalloutScript script; return 0; }\n")
    add_executable(${INT2SSL_LIB}-test EXCLUDE_FROM_ALL "${PROJECT_BINARY_DIR}/${INT2SSL_LIB}-test.cpp")
    target_include_directories(${INT2SSL_LIB}-test PRIVATE "${CMAKE_CURRENT_LIST_DIR}")
    target_link_libraries(${INT2SSL_LIB}-test PRIVATE ${INT2SSL_LIB})
endif()

#
# Executable (optional)
#

if(INT2SSL_BINARY)
    add_executable(${INT2SSL_BIN} "")
    target_sources(int2ssl
        PRIVATE
            XGetopt.h
            XGetopt.cpp

            main.cpp
            main.h
    )
    target_link_libraries(${INT2SSL_BIN} PRIVATE ${INT2SSL_LIB})
endif()

#
# Compilation flags
# Flags for all targets
#
# https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html
# https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2010/fwkeyyhe(v=vs.100)
# https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-alphabetically/ (latest VS)
#

get_property(INT2SSL_TARGETS DIRECTORY "${CMAKE_CURRENT_LIST_DIR}" PROPERTY BUILDSYSTEM_TARGETS)
foreach(target IN ITEMS ${INT2SSL_TARGETS})
    int2ssl_build_option(${target}     "compile" UNIX           FALSE -Os              INT2SSL_COMPILE_FLAG_Os)
#   int2ssl_build_option(${target}     "compile" UNIX           FALSE -s               INT2SSL_COMPILE_FLAG_s)

    int2ssl_build_option(${target}     "compile" MSVC           FALSE /options:strict  INT2SSL_COMPILE_FLAG_options_strict)
    int2ssl_build_option(${target}     "compile" MSVC           FALSE /Os              INT2SSL_COMPILE_FLAG_Os)
    int2ssl_build_option(${target}     "compile" MSVC           FALSE /permissive-     INT2SSL_COMPILE_FLAG_permissive_)

    int2ssl_build_option(${target}     "compile" UNIX           FALSE -Wall            INT2SSL_COMPILE_FLAG_Wall)
    int2ssl_build_option(${target}     "compile" UNIX           FALSE -Wextra          INT2SSL_COMPILE_FLAG_Wextra)
    int2ssl_build_option(${target}     "compile" UNIX           FALSE -Wpedantic       INT2SSL_COMPILE_FLAG_Wpedantic)
#   int2ssl_build_option(${target}     "compile" UNIX           FALSE -Wold-style-cast INT2SSL_COMPILE_FLAG_Wold_style_cast)
#   int2ssl_build_option(${target}     "compile" UNIX           FALSE -Wuseless-cast   INT2SSL_COMPILE_FLAG_Wuseless-cast)
    if(UNIX)
        int2ssl_build_option(${target} "compile" INT2SSL_STRICT TRUE  -Werror          INT2SSL_COMPILE_FLAG_Werror)
    endif()

#   int2ssl_build_option(${target}     "compile" MSVC           FALSE /W4              INT2SSL_COMPILE_FLAG_W4)
endforeach()

#
# Compilation flags
# Flags for executable only
#

if(UNIX)
    # According to internet, only first flag is required to build static binary in most cases; other two mmight be needed only in edge cases
    int2ssl_build_option(${INT2SSL_BIN} "link" INT2SSL_BINARY_STATIC TRUE  -static           INT2SSL_LINK_FLAG_static)
    int2ssl_build_option(${INT2SSL_BIN} "link" INT2SSL_BINARY_STATIC FALSE -static-libgcc    INT2SSL_LINK_FLAG_static_libgcc)
    int2ssl_build_option(${INT2SSL_BIN} "link" INT2SSL_BINARY_STATIC FALSE -static-libstdc++ INT2SSL_LINK_FLAG_static_libstdcxx)
endif()

#
# Export variables
# Set possibly useful variables when project is built via add_subdirectory() command
#
get_directory_property(INT2SSL_HAS_PARENT PARENT_DIRECTORY)
if(INT2SSL_HAS_PARENT) # if(NOT PROJECT_IS_TOP_LEVEL) -- added in CMake v3.21
    set(INT2SSL_LIBRARY_TARGET ${INT2SSL_LIB}     PARENT_SCOPE)
    set(INT2SSL_ALL_TARGETS    ${INT2SSL_TARGETS} PARENT_SCOPE)
endif()
