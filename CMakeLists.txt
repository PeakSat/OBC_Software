cmake_minimum_required(VERSION 3.30)
project(OBC_software LANGUAGES C CXX)

find_package(SAMV71-DFP COMPONENTS SAMV71Q21B REQUIRED)
find_package(Harmony REQUIRED)

add_executable(OBC_software src/main.cpp)

target_link_libraries(OBC_software PRIVATE samv71-dfp::Core samv71-dfp::SAMV71Q21B::Startup samv71-dfp::SAMV71Q21B::Linker::Flash)