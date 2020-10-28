#ifndef ROUTE_H
#define ROUTE_H
#include <QVector>
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>


class route
{
public:
    route();
    QVector<QVector3D> pos={};
    QVector<float> data={};
    QOpenGLBuffer *vbo;
    QOpenGLVertexArrayObject *vao;
    QOpenGLShaderProgram *route_shader;
    void generatedata();
    void init(QString shadername);
    void init();
};

#endif // ROUTE_H
