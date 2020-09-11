#ifndef HOUSE_H
#define HOUSE_H
#include <QOpenGLFunctions_4_2_Core>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vertex.h>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>


class house
{
public:
    house();
    house(vertex *ptr);
    house(QVector3D *position);
    void housepaint();
    int houseID;
    void addPoint(float px, float py,float pz);
    QVector<vertex> h_vertex;
    QVector<float> data={};
    QVector<float> topdata={};
    void caculateNormal();
    void caculateTexcoord();
    void GenerateData();//control the data pattern
    QOpenGLBuffer *vbo;
    QOpenGLVertexArrayObject *vao;
    QOpenGLBuffer *topvbo;
    QOpenGLVertexArrayObject *topvao;
    float px_max=0,px_min=600000,py_max=0,py_min=500000,pz_max=0,pz_min=100;
    bool select=false;
private:
    int vertex_num=0;


};

#endif // HOUSE_H
