cmake_minimum_required(VERSION 3.16)
project(rso_server_client C)

set(CMAKE_C_STANDARD 99)

add_executable(rso_server_client main.c server.c server.h client.h client.c communication_common.h server_parse_and_handle_query.h server_parse_and_handle_query.c)