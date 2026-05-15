cmake_minimum_required(VERSION 3.16)

project(ZMachineCore LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(ZMachineCore SHARED
  ZMachineCore_global.h
  zmachinevm.cpp
  zmachinevm.h
  zmachinememory.cpp
  zmachinememory.h
  enums.h
  zobject.h zobject.cpp
  zmachinevm_instructions.cpp
  unhandledopcodeexception.h unhandledopcodeexception.cpp
)

target_link_libraries(ZMachineCore PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(ZMachineCore PRIVATE ZMACHINECORE_LIBRARY)
