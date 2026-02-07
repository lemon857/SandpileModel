cmake_minimum_required(VERSION 3.50 FATAL_ERROR)

set(PROJECT_NAME SandpileModel)

project(${PROJECT_NAME})

set(INCLUDES
  include/SandpileModel/utils.h
  include/SandpileModel/bmp_writer.h
  include/SandpileModel/dmatrix.h
  include/SandpileModel/sandpile.h
  include/SandpileApplication/SandpileApp.h
  include/SandpileApplication/BitmapRGB.h
)

set(SOURCES
  src/utils.cpp
  src/bmp_writer.cpp
  src/sandpile.cpp
  src/SandpileApp.cpp
  src/BitmapRGB.cpp
)

add_executable(${PROJECT_NAME} ${INCLUDES} ${SOURCES} src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE include)

add_subdirectory(external/CppEngine)
target_link_libraries(${PROJECT_NAME} EngineCore)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
          
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/systemres $<TARGET_FILE_DIR:${PROJECT_NAME}>/systemres)
