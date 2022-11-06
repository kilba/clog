cmake_minimum_required(VERSION 3.23.2)

project(clog)

add_executable(${PROJECT_NAME}
	src/main.c
)

target_include_directories(${PROJECT_NAME}
	PUBLIC include/
)

target_link_directories(${PROJECT_NAME}
	PRIVATE lib
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
