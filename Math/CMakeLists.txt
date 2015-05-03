
list(APPEND HEADERS
    ${GLDEMO_SOURCE_DIR}/Math/mathdefs.h
    ${GLDEMO_SOURCE_DIR}/Math/matrix2.h
    ${GLDEMO_SOURCE_DIR}/Math/matrix3.h
    ${GLDEMO_SOURCE_DIR}/Math/matrix4.h
    ${GLDEMO_SOURCE_DIR}/Math/matrixn.h
    ${GLDEMO_SOURCE_DIR}/Math/vector2.h
    ${GLDEMO_SOURCE_DIR}/Math/vector3.h
    ${GLDEMO_SOURCE_DIR}/Math/vector4.h
    ${GLDEMO_SOURCE_DIR}/Math/vectorn.h
)

include(${GLDEMO_SOURCE_DIR}/Math/Tests/CMakeLists.txt)

