cmake_minimum_required(VERSION 3.16)
project(rso_server_client C)

set(CMAKE_C_STANDARD 99)

add_executable(rso_server server.c server.h communication_common.h server_parse_query.h server_parse_query.c server_client_handler.h server_client_handler.c)
add_executable(rso_client client.h client.c communication_common.h)