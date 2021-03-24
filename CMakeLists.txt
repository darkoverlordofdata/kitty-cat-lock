cmake_minimum_required ( VERSION 3.1 )
###################################################################################
#
#   Build catlock
#
###################################################################################
project ( catlock VERSION 0.1.0 )

#
set ( VERSION 0.1.0 )
set ( RELEASE_NAME catlock )

#
# enable features
#
include ( CMakeToolsHelpers OPTIONAL )

#
#   application source
#
list ( APPEND C_SOURCES src/calendar/calendar.c )
list ( APPEND C_SOURCES src/application.c )
list ( APPEND C_SOURCES src/helpers.c )
list ( APPEND C_SOURCES src/main.c )
list ( APPEND C_SOURCES src/polyfill.c )
#
#   application includes
#
set ( INCLUDES "${INCLUDES};/usr/local/include" )
set ( INCLUDES "${INCLUDES};/usr/local/include/freetype2" )
set ( INCLUDES "${INCLUDES};/usr/local/include/X11" )
set ( INCLUDES "${INCLUDES};/usr/include" )
set ( INCLUDES "${INCLUDES};/usr/include/freetype2" )
set ( INCLUDES "${INCLUDES};/usr/include/X11" )

set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Os -std=c11" )
# set ( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O0 -g -std=c11" )

# links folders 
link_directories ( /usr/lib/ )

include_directories ( AFTER ${INCLUDES} )

#
#   compile ccode & link executable
#
add_executable ( ${PROJECT_NAME} ${C_SOURCES} ${RESOURCES} )
add_custom_command ( TARGET ${PROJECT_NAME} 
                   POST_BUILD COMMAND 
                   ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> .. )
                                      
target_link_libraries ( ${PROJECT_NAME} c X11 Xext Xpm Imlib2 Xft crypt  )
install ( TARGETS ${PROJECT_NAME} RUNTIME DESTINATION . )

include ( CPack )

