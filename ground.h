#ifndef GROUND_H
#define GROUND_H
#include <QVector>
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFramebufferObject>
#include <QOpenGLTexture>


class ground
{
public:
    ground();
    ground(QVector3D nw,QVector3D ne,QVector3D se,QVector3D sw);
    QVector<QVector3D> pos={};
    QVector<float> data={};
    QOpenGLBuffer *vbo;
    QOpenGLVertexArrayObject *vao;
    QOpenGLFramebufferObject *fbo;
    QOpenGLTexture *shadowtexture;


    void generatedata();

};

#endif // GROUND_H
