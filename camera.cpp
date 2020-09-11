#include "camera.h"
#include <QMatrix4x4>

Camera::Camera():Camera_front(0.0,0.0,-1.0)
{

}
Camera::Camera(QVector3D Cam_pos,QVector3D Cam_front,QVector3D Cam_up)
{
    Camera_pos=Cam_pos;
    Camera_front=Cam_front;
    Camera_up=Cam_up;
}
Camera::Camera(float posX,float posY,float posZ,float upX,float upY,float upZ,float yaw,float pitch,float roll)
{

}
QMatrix4x4 Camera::GetViewMatrix()
{
    //return QMatrix4x4;
    QMatrix4x4 *res=new QMatrix4x4();
    res->lookAt(Camera_pos,Camera_pos+Camera_front,Camera_up);
    return *res;
}
void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{

}
void Camera::ProcessMouseScroll(float yoffset)
{

}
void Camera::updateCameraVectors()
{

}
