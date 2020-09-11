#ifndef CAMERA_H
#define CAMERA_H
#include <QVector3D>
#include <QMouseEvent>
#define PI 3.1415926

//const float YAW=-90.0f;
//const float PITCH=0.0f;
//const float ROLL=0.0f;
//const float SPEED=2.5f;
//const float SENSITIVITY=0.1f;
//const float ZOOM=45.0f;

class Camera
{
public:
    Camera();
    Camera(QVector3D Cam_pos,QVector3D Cam_front,QVector3D Cam_up);
    QVector3D Camera_pos;
    QVector3D Camera_front;
    QVector3D Camera_up;
    QVector3D Camera_right;
    QVector3D WorldUp;
    //Euler Angles
    float Yaw=-90.0f;
    float Pitch=0.0f;
    float Roll;
    //camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom=45.0f;
    Camera(float posX,float posY,float posZ,float upX,float upY,float upZ,float yaw,float pitch,float roll);
    QMatrix4x4 GetViewMatrix();
    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessMouseScroll(float yoffset);

private:
    void updateCameraVectors();

};

#endif // CAMERA_H
