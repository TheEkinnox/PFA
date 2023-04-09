# set the target name (the visual studio project)
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(EXE_NAME ${CURRENT_FOLDER_NAME})
add_executable(${EXE_NAME})


###############################
#                             #
# Sources                     #
#                             #
###############################

file(GLOB_RECURSE PROJECT_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.inl

	${CMAKE_CURRENT_SOURCE_DIR}/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/*.cc
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.cxx
	${CMAKE_CURRENT_SOURCE_DIR}/*.c++

	${CMAKE_CURRENT_SOURCE_DIR}/*.txt
	${CMAKE_CURRENT_SOURCE_DIR}/*.md)

target_sources(${EXE_NAME} PRIVATE ${PROJECT_FILES})
target_include_directories(${EXE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_FILES})


###############################
#                             #
# Projet Modern openGL        #
#                             #
###############################

target_link_libraries(${EXE_NAME} PRIVATE ${LIBGL_TARGETS})
target_include_directories(${EXE_NAME} PRIVATE ${LIBGL_INCLUDE_DIRS})


###############################
#                             #
# Projet LibMath              #
#                             #
###############################

target_link_libraries(${EXE_NAME} PRIVATE ${LIBMATH_NAME})
target_include_directories(${EXE_NAME} PRIVATE ${LIBMATH_INCLUDE_DIR})

###############################
#                             #
# Copy the necessary dlls     #
#                             #
###############################

add_custom_command(TARGET ${EXE_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${EXE_NAME}> $<TARGET_FILE_DIR:${EXE_NAME}>
  COMMAND_EXPAND_LISTS
)
