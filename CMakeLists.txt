cmake_minimum_required(VERSION 3.10)

project(blackbox_part2)

# ========== COMMON DATA TYPES ===========

set(COMMON_DATA_INC_DIR ${CMAKE_BINARY_DIR}/../common)
set(COMMON_DATA_SRC_DIR ${CMAKE_BINARY_DIR}/../common)

set(COMMON_DATA_SOURCES ${COMMON_DATA_SRC_DIR}/blackbox_data_xdr.c # auto-gen file
                        ${COMMON_DATA_SRC_DIR}/logger_data_xdr.c # auto-gen file
                        ${COMMON_DATA_SRC_DIR}/progress_bar.c)

#  ============== SERVER =================

message("Compiling server source codes...")

set(SERVER_EXC_NAME "server_main")

set(SERVER_DIR ${CMAKE_BINARY_DIR}/../blackbox-runner-server)

set(SERVER_BIN_DIR ${SERVER_DIR}/bin)
set(SERVER_INC_DIR ${SERVER_DIR}/include)
set(SERVER_SRC_DIR ${SERVER_DIR}/src)

set(SERVER_SOURCES ${SERVER_SRC_DIR}/logger.c
                   ${SERVER_SRC_DIR}/blackbox_receive.c
                   ${SERVER_SRC_DIR}/ip_addr_utility.c
                   ${SERVER_SRC_DIR}/blackbox_data_proc.c
                   ${SERVER_SRC_DIR}/blackbox_data_svc.c # auto-gen file
                   ${SERVER_SRC_DIR}/popen3.c
                   ${SERVER_SRC_DIR}/logger_data_clnt.c # auto-gen file
                   ${COMMON_DATA_SOURCES})

include_directories(${SERVER_INC_DIR})
include_directories(${COMMON_DATA_INC_DIR}) # include for rpc

add_executable(${SERVER_EXC_NAME} ${SERVER_SOURCES})

set_target_properties(${SERVER_EXC_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${SERVER_BIN_DIR}
)

#  ============== CLIENT =================

message("Compiling client source codes...")

set(CLIENT_EXC_NAME "client_main")

set(CLIENT_DIR ${CMAKE_BINARY_DIR}/../blackbox-provider-client)

set(CLIENT_BIN_DIR ${CLIENT_DIR}/bin)
set(CLIENT_INC_DIR ${CLIENT_DIR}/include)
set(CLIENT_SRC_DIR ${CLIENT_DIR}/src)

set(CLIENT_SOURCES ${CLIENT_SRC_DIR}/main.c
                   ${CLIENT_SRC_DIR}/blackbox_send.c
                   ${CLIENT_SRC_DIR}/logger.c
                   ${CLIENT_SRC_DIR}/blackbox_data_clnt.c # auto-gen file
                   ${COMMON_DATA_SOURCES})

include_directories(${CLIENT_INC_DIR})
include_directories(${COMMON_DATA_INC_DIR}) # include for rpc

add_executable(${CLIENT_EXC_NAME} ${CLIENT_SOURCES})

set_target_properties(${CLIENT_EXC_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CLIENT_BIN_DIR}
)

#  ============== LOGGER =================

message("Compiling logger source codes...")

set(LOGGER_EXC_NAME "logger_main")

set(LOGGER_DIR ${CMAKE_BINARY_DIR}/../logger-server)

set(LOGGER_BIN_DIR ${LOGGER_DIR}/bin)
set(LOGGER_INC_DIR ${LOGGER_DIR}/include)
set(LOGGER_SRC_DIR ${LOGGER_DIR}/src)

set(LOGGER_SOURCES ${LOGGER_SRC_DIR}/logger_server.c
                   ${LOGGER_SRC_DIR}/logger_data_proc.c
                   ${LOGGER_SRC_DIR}/logger_data_svc.c # auto-gen file
                   ${COMMON_DATA_SOURCES})

include_directories(${LOGGER_INC_DIR})
include_directories(${COMMON_DATA_INC_DIR}) # include for rpc

add_executable(${LOGGER_EXC_NAME} ${LOGGER_SOURCES})

set_target_properties(${LOGGER_EXC_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${LOGGER_BIN_DIR}
)
