qt4_add_resources(RESOURCES ${GLDEMO_SOURCE_DIR}/Renderer/shaders.qrc)

list(APPEND HEADERS
    ${GLDEMO_SOURCE_DIR}/Renderer/glwidget.h
    ${GLDEMO_SOURCE_DIR}/Renderer/glwidgetimpl.h
    ${GLDEMO_SOURCE_DIR}/Renderer/glrenderer.h
    ${GLDEMO_SOURCE_DIR}/Renderer/glutils.h
    ${GLDEMO_SOURCE_DIR}/Renderer/renderer.h
    ${GLDEMO_SOURCE_DIR}/Renderer/shader.h
    ${GLDEMO_SOURCE_DIR}/Renderer/lambertshader.h
)

list(APPEND MOC_HEADERS
    ${GLDEMO_SOURCE_DIR}/Renderer/glwidget.h
    ${GLDEMO_SOURCE_DIR}/Renderer/glwidgetimpl.h
)

list(APPEND SOURCES
    ${GLDEMO_SOURCE_DIR}/Renderer/glwidget.cpp
    ${GLDEMO_SOURCE_DIR}/Renderer/glwidgetimpl.cpp
    ${GLDEMO_SOURCE_DIR}/Renderer/glrenderer.cpp
    ${GLDEMO_SOURCE_DIR}/Renderer/glutils.cpp
    ${GLDEMO_SOURCE_DIR}/Renderer/shader.cpp
    ${GLDEMO_SOURCE_DIR}/Renderer/lambertshader.cpp
)

