# set the target name (the visual studio project)
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(PROJECT_NAME ${CURRENT_FOLDER_NAME})

option(IRRKLANG_USE_MP3 "Add ikpMP3.dll when on, don't when off" ON)
option(IRRKLANG_USE_FLAC "Add ikpFlac.dll when on, don't when off" ON)

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
	# 32 bits
	set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/win32/irrKlang.lib)
	set(DLL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin/win32)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
	# 64 bits
	set(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/winx64/irrKlang.lib)
	set(DLL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin/winx64)
endif()

set(PROJECT_TARGETS "")

function(addModule MODULE_NAME)
	add_library(${MODULE_NAME}_DLL SHARED IMPORTED GLOBAL)
	set_target_properties(${MODULE_NAME}_DLL PROPERTIES
		IMPORTED_LOCATION "${DLL_DIR}/${MODULE_NAME}.dll"
		IMPORTED_IMPLIB "${LIB_PATH}"
	)
	
	list(APPEND PROJECT_TARGETS ${MODULE_NAME}_DLL)
	set(PROJECT_TARGETS ${PROJECT_TARGETS} PARENT_SCOPE)
	message(STATUS "Added IrrKlang module : ${MODULE_NAME}")
endfunction()

addModule(irrKlang)

if (${IRRKLANG_USE_MP3})
	addModule(ikpMP3)
endif()

if (${IRRKLANG_USE_FLAC})
	addModule(ikpFlac)
endif()

add_library(${PROJECT_NAME} INTERFACE IMPORTED GLOBAL)
target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_TARGETS})

#set(IRRKLANG_NAME ${PROJECT_NAME} PARENT_SCOPE)
set(IRRKLANG_NAME irrKlang_DLL PARENT_SCOPE)
set(IRRKLANG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)