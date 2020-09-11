#ifndef VERTEX_H
#define VERTEX_H
#include <QOpenGLFunctions_4_2_Core>
#include <QVector3D>
#include <QVector2D>



class vertex
{
public:
    vertex();
    vertex(GLfloat px,GLfloat py,GLfloat pz,GLfloat nx,GLfloat ny,GLfloat nz,GLfloat u,GLfloat v);
    vertex(QVector3D point,QVector3D normal,QVector2D tex);
    int vertexID;

    QVector3D position=QVector3D(0,0,0);
    QVector3D normal=QVector3D(0,0,0);
    QVector2D texcoord=QVector2D(0,0);

};

#endif // VERTEX_H
