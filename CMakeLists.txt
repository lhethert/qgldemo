cmake_minimum_required(VERSION 3.0)
project(GLDEMO)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5OpenGL)
find_package(Qt5Test)
find_package(OpenGL)
set(QT_LIBRARIES "Qt5::Core;Qt5::Widgets;Qt5::OpenGL")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
enable_testing(true)

# Macro for creating a test. Saves us having to write all this
# garbage every time we want to add a simple Qt test.
MACRO (add_qt_test testname testsrc)
   SET (test_${testname}_SRCS ${testsrc})
   add_executable(test_${testname} ${test_${testname}_SRCS} ${test_additional})
   target_link_libraries(test_${testname} gllib ${QT_LIBRARIES} Qt5::Test)
   ADD_TEST(test_${testname} test_${testname})
ENDMACRO (add_qt_test)

include_directories(${GLDEMO_SOURCE_DIR})
include_directories(${OPENGL_INCLUDE_DIRS})

set(SOURCES
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

qt5_wrap_ui(UIC_SOURCES ${UI_SOURCES})

# Create a separate library for our core things, so that we can link against it for testing.
add_library(gllib ${SOURCES} ${HEADERS} ${MOC_SOURCES} ${UIC_SOURCES} ${RESOURCES})
target_link_libraries(gllib ${QT_LIBRARIES} ${OPENGL_LIBRARIES})

# Our executable links against our library, but also directly includes the resources.
add_executable(gldemo main.cpp ${RESOURCES})
target_link_libraries(gldemo gllib)
