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

class city : public QOpenGLWidget,protected QOpenGLFunctions_4_2_Core
{
    Q_OBJECT
public:
    explicit city(QWidget *parent = nullptr);

protected:
    void paintGL() override;
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    //mouse event
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event)override;

public slots:
    //keypress event
    void keyPressEvent(QKeyEvent *event) override;


private:
    int mMatrixLoc;
    int vMatrixLoc;
    int pMatrixLoc;
    int mvMatrixLoc;
    int lightColorLoc;
    int lightPositionLoc;
    int EyeDirectionLoc;
    int NormalMatrixLoc;
    int ourTextureLoc;
    Camera *m_camera;
    QMatrix4x4 mMatrix;
    QMatrix4x4 vMatrix;
    QMatrix4x4 pMatrix;
    QMatrix4x4 mvMatrix;
    QMatrix3x3 NormalMatrix;
    QVector<house> building;
    QOpenGLShaderProgram *build_shader;  //build_shader need to set in the CLASS house ?
    QOpenGLShaderProgram *top_shader;
    QOpenGLTexture *m_texture;
    //mouse relate
    QPoint m_lastPos;
};

#endif // CITY_H
