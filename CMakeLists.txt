cmake_minimum_required(VERSION 2.8)
project(GLDEMO)

find_package(Qt4 COMPONENTS QtCore QtGui QtOpenGL)
find_package(OpenGL)
include(${QT_USE_FILE})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
enable_testing(true)

# Macro for creating a test. Saves us having to write all this
# garbage every time we want to add a simple Qt test.
MACRO (add_qt_test testname testsrc)
   SET (test_${testname}_SRCS ${testsrc})
   #qt4_automoc(${test_${testname}_SRCS})
   add_executable(test_${testname} ${test_${testname}_SRCS} ${test_additional})
   target_link_libraries(test_${testname} ${QT_QTCORE_LIBRARY}
                         ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY}
                         ${QT_QTLOCATION_LIBRARY})
   ADD_TEST(test_${testname} test_${testname})
ENDMACRO (add_qt_test)

include_directories(${GLDEMO_SOURCE_DIR})
include_directories(${OPENGL_INCLUDE_DIRS})

set(SOURCES
    main.cpp
)

set(HEADERS
)

set(MOC_HEADERS
)

set(UI_SOURCES
)

include(${GLDEMO_SOURCE_DIR}/Math/CMakeLists.txt)
include(${GLDEMO_SOURCE_DIR}/Renderer/CMakeLists.txt)
include(${GLDEMO_SOURCE_DIR}/Scene/CMakeLists.txt)

qt4_wrap_ui(UIC_SOURCES ${UI_SOURCES})

add_executable(gldemo ${SOURCES} ${HEADERS} ${MOC_SOURCES} ${UIC_SOURCES} ${RESOURCES})
target_link_libraries(gldemo ${QT_LIBRARIES} ${OPENGL_LIBRARIES})

