#include "Math/vector3.h"
#include "Math/matrix3.h"
#include "Math/matrix4.h"
#include "camera.h"
#include "transformation.h"

namespace GLDemo
{
    /**
     * Constructs a new camera at the default location.
     */
    Camera::Camera() :
        SpatialEntity(),
        m_near(0.01f),
        m_far(1000.0f),
        m_fov(25.0f)
    {
        setCameraView(Vector3f(0.0f, 0.0f, 5.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.0f, 0.0f, 0.0f));
    }


    /**
     * \param eye      The desired position of the camera
     * \param up       The desired up vector of the camera
     * \param lookAt   The desired location that the camera should point towards
     *
     * Constructs a new camera with the initial orientation computed from the input vectors.
     */
    Camera::Camera(const Vector3f& eye, const Vector3f& up, const Vector3f& lookAt) :
        SpatialEntity(),
        m_near(0.01f),
        m_far(1000.0f),
        m_fov(25.0f)
    {
        setCameraView(eye, up, lookAt);
    }


    /**
     * \param eye      The desired position of the camera
     * \param up       The desired up vector of the camera
     * \param lookAt   The desired location that the camera should point towards
     *
     * Orients the camera using the input vectors, computing the view and right vectors
     * using the \a lookAt vector.
     */
    void Camera::setCameraView(const Vector3f& eye, const Vector3f& up, const Vector3f& lookAt)
    {
        Vector3f camView = (eye - lookAt).unitVector();
        Vector3f camRight = up.unitCross(camView);
        Vector3f camUp = camView.unitCross(camRight);
        m_lookAt = lookAt;

        Matrix3f m;
        m(0,0) = camRight.x();
        m(1,0) = camRight.y();
        m(2,0) = camRight.z();
        m(0,1) = camUp.x();
        m(1,1) = camUp.y();
        m(2,1) = camUp.z();
        m(0,2) = camView.x();
        m(1,2) = camView.y();
        m(2,2) = camView.z();

        getLocalTransformation().setRotation(m);
        getLocalTransformation().getTranslation().x() = -(camRight.dot(eye));
        getLocalTransformation().getTranslation().y() = -(camUp.dot(eye));
        getLocalTransformation().getTranslation().z() = -(camView.dot(eye));
        updateGeometricState(0.0, true);
    }


    /**
     * \param eye      The desired position of the camera
     * \param camView  The desired view vector of the camera
     * \param camUp    The desired up vector of the camera
     * \param camRight The desired right vector of the camera
     *
     * Orients the camera using the input vectors.
     */
    void Camera::setCameraView(const Vector3f& eye, const Vector3f& camView, const Vector3f& camUp, const Vector3f& camRight)
    {
        Matrix3f m;
        m(0,0) = camRight.x();
        m(1,0) = camRight.y();
        m(2,0) = camRight.z();
        m(0,1) = camUp.x();
        m(1,1) = camUp.y();
        m(2,1) = camUp.z();
        m(0,2) = -camView.x();
        m(1,2) = -camView.y();
        m(2,2) = -camView.z();

        getLocalTransformation().setRotation(m);
        getLocalTransformation().getTranslation().x() = -(camRight.dot(eye));
        getLocalTransformation().getTranslation().y() = -(camUp.dot(eye));
        getLocalTransformation().getTranslation().z() = -(camView.dot(eye));
        updateGeometricState(0.0, true);
    }


    /**
     * \param position  The eye position of the camera in world coordinates.
     * \param view      The view vector of the camera (forward) in world coordinates.
     * \param up        The up vector in world coordinates.
     * \param right     The right (across) vector in world coordinates.
     */
    void Camera::calcWorldVectors(Vector3f& position, Vector3f& view, Vector3f& up, Vector3f& right)
    {
        const Transformation& world = getWorldTransformation();
        Matrix3f rotation = world.getRotation();
        position = rotation * -world.getTranslation();
        view = -Vector3f(rotation(0,2), rotation(1,2), rotation(2,2));
        up = Vector3f(rotation(0,1), rotation(1,1), rotation(2,1));
        right = Vector3f(rotation(0,0), rotation(1,0), rotation(2,0));
        /*
        position = world.apply(Vector3f(0,0,0));
        view = (world.apply(Vector3f(0,0,-1)) - position).unitVector();
        up = (world.apply(Vector3f(0,1,0)) - position).unitVector();
        right = (world.apply(Vector3f(1,0,0)) - position).unitVector();
        */
    }


    /**
     * \return A new copy of this camera object.
     */
    Camera* Camera::clone() const
    {
        return new Camera(*this);
    }

}

