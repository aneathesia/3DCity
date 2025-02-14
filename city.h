#ifndef CITY_H
#define CITY_H
#include <Windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QWidget>
#include<QOpenGLWidget>
#include "QOpenGLFunctions_4_2_Core"
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include "house.h"
#include "camera.h"
#include "ground.h"
#include "route.h"

class city : public QOpenGLWidget,protected QOpenGLFunctions_4_2_Core
{
    Q_OBJECT
public:
    explicit city(QWidget *parent = nullptr);
    int building_Search(QVector3D *coord);

protected:
    void paintGL() override;
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    //mouse event
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event)override;
signals:
    void sendstatusCoordination(double posX,double posY,double posZ);
public slots:
    //keypress event
    void keyPressEvent(QKeyEvent *event) override;
    void recieveTarget(QVector3D *coord);
    void recieveCamera(Camera *cam);
    void paintroute(QVector<QVector3D> route);




private:

    int ourTextureLoc;
    GLfloat  winX, winY, winZ;
    GLdouble posX, posY, posZ;
    Camera *m_camera;
    ground *m_ground;
    route *m_route;
    QMatrix4x4 mMatrix;
    QMatrix4x4 vMatrix;
    QMatrix4x4 pMatrix;
    QMatrix4x4 mvMatrix;
    QMatrix3x3 NormalMatrix;
    QMatrix4x4 lightProjectionMatrix;
    QMatrix4x4 lightViewMatrix;
    QMatrix4x4 OrthoMatrix;
    QVector<house> building;

    QOpenGLShaderProgram *ground_shader;
    QOpenGLShaderProgram *quads_shader;
    QOpenGLShaderProgram *shadow_mapping_depth;
    QOpenGLShaderProgram *shadow_mapping;
    QOpenGLShaderProgram *shadow_mapping_top;
    QOpenGLShaderProgram *shadow_mapping_ground;
    QOpenGLTexture *m_texture;
    QOpenGLFramebufferObject *fbo;
    //mouse relate
    QPoint m_lastPos;
    QVector3D *Targetcoord;
    int selected=-1;

};

#endif // CITY_H
