cmake_minimum_required(VERSION 3.3)
project(eulersmethod)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)

set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)

include_directories(
        ${COMMON_INCLUDES}
        ${OPENGL_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLUT_INCLUDE_DIRS}
)


set(LINK_LIBS ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

file(GLOB SRCS ${PROJECT_SOURCE_DIR}/mathplopp/*.*)

add_library(matplotpp STATIC ${SRCS})

target_include_directories(matplotpp PUBLIC ${COMMON_INCLUDES})

target_link_libraries(matplotpp LINK_PUBLIC ${LINK_LIBS})

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
install(TARGETS matplotpp DESTINATION lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)
add_executable(eulersmethod ${SOURCE_FILES})
target_include_directories(eulersmethod PUBLIC ${COMMON_INCLUDES})

target_link_libraries(eulersmethod matplotpp)