#ifndef GLDEMO_CAMERA_H
#define GLDEMO_CAMERA_H

#include "Math/vector3.h"
#include "spatialentity.h"

namespace GLDemo
{

  /**
   * \brief Class representing a camera in a scene.
   */
  class Camera : public SpatialEntity
  {
  public:
      Camera();
      Camera(const Vector3f& eye, const Vector3f& up, const Vector3f& lookAt);

      void setCameraView(const Vector3f& eye, const Vector3f& up, const Vector3f& lookAt);
      void setCameraView(const Vector3f& eye, const Vector3f& view, const Vector3f& up, const Vector3f& right);

      void  setNearPlaneDistance(float near) { m_near = near; }
      float getNearPlaneDistance() const     { return m_near; }

      void  setFarPlaneDistance(float far) { m_far = far; }
      float getFarPlaneDistance() const    { return m_far; }

      void  setFieldOfView(float fov) { m_fov = fov; }
      float getFieldOfView() const    { return m_fov; }

      void calcWorldVectors(Vector3f& position, Vector3f& view, Vector3f& up, Vector3f& right);

      /**
       * \return The location that the camera is currently looking at.
       */
      const Vector3f& getLookAt() const { return m_lookAt; }

      virtual Camera* clone() const;

  private:
      float    m_near;
      float    m_far;
      float    m_fov;
      Vector3f m_lookAt;
  };

}

#endif
