cmake_minimum_required(VERSION 3.7)

project(tcpip_my)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/net/net
)

file (GLOB_RECURSE SOURCE_LIST  "src/*.c")

set(CMAKE_C_STANDARD 99)
find_library(PCAP_LIBRARY pcap)
if(NOT PCAP_LIBRARY)
    message(FATAL_ERROR "libpcap library not found")
endif()


add_executable(${PROJECT_NAME} ${SOURCE_LIST})

add_compile_options(-g)

target_link_libraries(${PROJECT_NAME} ${PCAP_LIBRARY})