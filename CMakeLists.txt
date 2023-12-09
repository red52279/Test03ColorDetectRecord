CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

project(thread)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(OpenCV REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src/detect/include)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/detect/src SRC)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC2)
set(SRC ${SRC} ${SRC2})

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS})