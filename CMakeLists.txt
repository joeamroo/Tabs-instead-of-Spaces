cmake_minimum_required(VERSION 3.16)

project(TabsInsteadOfSpaces)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT DEFINED ENV{LLVM_HOME})
    message(FATAL_ERROR "$LLVM_HOME is not defined")
else()
    message(STATUS "$LLVM_HOME found: $ENV{LLVM_HOME}")
    set(LLVM_HOME $ENV{LLVM_HOME} CACHE PATH "Root of LLVM installation")
    set(LLVM_LIB ${LLVM_HOME}/lib)
    set(LLVM_DIR ${LLVM_LIB}/cmake/llvm)
    set(LLVM_BUILD $ENV{LLVM_BUILD} CACHE PATH "Root of LLVM build")

    # Find LLVM package
    find_package(LLVM REQUIRED CONFIG)

    # Include LLVM and Clang directories
    include_directories(${LLVM_INCLUDE_DIRS})
    link_directories(${LLVM_LIBRARY_DIRS})

    # Find Clang-Tidy
    if (TARGET ClangTidy)
        include(${LLVM_DIR}/ClangTidy.cmake)
    else()
        message(FATAL_ERROR "ClangTidy target not found!")
    endif()

    # Add the custom clang-tidy module
    add_clang_tidy_module(TabsInsteadOfSpacesModule
      TabsInsteadOfSpacesCheck.cpp
      TabsInsteadOfSpacesModule.cpp
    )

    target_link_libraries(TabsInsteadOfSpacesModule
        LLVMSupport
        clangAST
        clangBasic
        clangFrontend
        clangTooling
    )
endif()
