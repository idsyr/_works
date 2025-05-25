cmake_minimum_required(VERSION 3.11)
project("tuimagic")

set(SRCS
	widget_header.c
	darray.c
	theme.c
	infoline.c
	infoblock.c

	conf_area.c
	conf_parser.c
	main.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${CURSES_LIBRARY})
