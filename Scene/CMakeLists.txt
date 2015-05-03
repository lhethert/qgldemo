
list(APPEND HEADERS
    ${GLDEMO_SOURCE_DIR}/Scene/camera.h
    ${GLDEMO_SOURCE_DIR}/Scene/controller.h
    ${GLDEMO_SOURCE_DIR}/Scene/cubemesh.h
    ${GLDEMO_SOURCE_DIR}/Scene/elementlist.h
    ${GLDEMO_SOURCE_DIR}/Scene/helpers.h
    ${GLDEMO_SOURCE_DIR}/Scene/mesh.h
    ${GLDEMO_SOURCE_DIR}/Scene/meshinstance.h
    ${GLDEMO_SOURCE_DIR}/Scene/object.h
    ${GLDEMO_SOURCE_DIR}/Scene/scene.h
    ${GLDEMO_SOURCE_DIR}/Scene/scenenode.h
    ${GLDEMO_SOURCE_DIR}/Scene/spatialentity.h
    ${GLDEMO_SOURCE_DIR}/Scene/transformation.h
    ${GLDEMO_SOURCE_DIR}/Scene/vertex.h
)

list(APPEND MOC_HEADERS
    ${GLDEMO_SOURCE_DIR}/Scene/transformationcontroller.h
)


list(APPEND SOURCES
    ${GLDEMO_SOURCE_DIR}/Scene/camera.cpp
    ${GLDEMO_SOURCE_DIR}/Scene/controller.cpp
    ${GLDEMO_SOURCE_DIR}/Scene/cubemesh.cpp
    ${GLDEMO_SOURCE_DIR}/Scene/mesh.cpp
    ${GLDEMO_SOURCE_DIR}/Scene/meshinstance.cpp
    ${GLDEMO_SOURCE_DIR}/Scene/object.cpp
    ${GLDEMO_SOURCE_DIR}/Scene/scenenode.cpp
    ${GLDEMO_SOURCE_DIR}/Scene/spatialentity.cpp
    ${GLDEMO_SOURCE_DIR}/Scene/transformation.cpp
)
