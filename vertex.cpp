#include "vertex.h"
#include <gl/GL.h>
#include <gl/GLU.h>

vertex::vertex()
{

}
vertex::vertex(GLfloat px,GLfloat py,GLfloat pz,GLfloat nx=0,GLfloat ny=0,GLfloat nz=0,GLfloat u=0,GLfloat v=0)
{
   position.setX(px);position.setY(py);position.setZ(pz);
   normal.setX(nx);normal.setY(ny);normal.setZ(nz);
   texcoord.setX(u);texcoord.setY(v);
}
vertex::vertex(QVector3D point,QVector3D nor,QVector2D tex)
{
    position=point; normal=nor;texcoord=tex;
}
