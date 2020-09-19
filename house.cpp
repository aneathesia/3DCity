#include "house.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <QOpenGLFunctions_4_2_Core>
#include <QOpenGLFunctions>



house::house()
{

}
house::house(vertex *ptr)
{

}
void house::caculateNormal()
{
    //if(this.h_vertex.empty())
    int index=h_vertex.length();
    for(int i=0;i<index-2;i++){
       QVector3D temp1=this->h_vertex[i].position-this->h_vertex[i+1].position;
       QVector3D temp2=this->h_vertex[i].position-this->h_vertex[i+2].position;
       QVector3D temp=QVector3D::normal(temp1,temp2); //invert ? *pow((-1),j)
       this->h_vertex[i].normal=temp;
    }
    h_vertex[index-1].normal=h_vertex[1].normal;
    h_vertex[index-2].normal=h_vertex[0].normal;// begin end
}
void house::caculateTexcoord()
{
    int index=this->vertex_num;
    for(int i=0;i<index;i++){
       this->h_vertex[i].texcoord.setX((this->h_vertex[i].position.x()-this->px_min)/(this->px_max-this->px_min));
       this->h_vertex[i].texcoord.setY((this->h_vertex[i].position.y()-this->py_min)/(this->py_max-this->py_min));
    }

}
void house::GenerateData()  //need to control the CCW or CW
{
    data={};
    /*__normal  x y compare___*/
    if(abs(h_vertex[0].normal.y())>abs(h_vertex[0].normal.x())){
        for(int i=0;i<h_vertex.size()-2;i=i+2){
               data.push_back(h_vertex[i+0].position.x());
               data.push_back(h_vertex[i+0].position.y());
               data.push_back(h_vertex[i+0].position.z());
               data.push_back(h_vertex[i].normal.x());
               data.push_back(h_vertex[i].normal.y());
               data.push_back(h_vertex[i].normal.z());
               data.push_back(h_vertex[i+1].position.x());
               data.push_back(h_vertex[i+1].position.y());
               data.push_back(h_vertex[i+1].position.z());
               data.push_back(h_vertex[i].normal.x());
               data.push_back(h_vertex[i].normal.y());
               data.push_back(h_vertex[i].normal.z());
               data.push_back(h_vertex[i+3].position.x());
               data.push_back(h_vertex[i+3].position.y());
               data.push_back(h_vertex[i+3].position.z());
               data.push_back(h_vertex[i].normal.x());
               data.push_back(h_vertex[i].normal.y());
               data.push_back(h_vertex[i].normal.z());
               data.push_back(h_vertex[i+2].position.x());
               data.push_back(h_vertex[i+2].position.y());
               data.push_back(h_vertex[i+2].position.z());
               data.push_back(h_vertex[i].normal.x());
               data.push_back(h_vertex[i].normal.y());
               data.push_back(h_vertex[i].normal.z());
           }
       // qDebug()<<data.length()<<endl;
    }
    else{
        for(int i=h_vertex.size()-4;i>=0;i=i-2){
               data.push_back(h_vertex[i+0].position.x());
               data.push_back(h_vertex[i+0].position.y());
               data.push_back(h_vertex[i+0].position.z());
               data.push_back(h_vertex[i].normal.x());
               data.push_back(h_vertex[i].normal.y());
               data.push_back(h_vertex[i].normal.z());
               data.push_back(h_vertex[i+1].position.x());
               data.push_back(h_vertex[i+1].position.y());
               data.push_back(h_vertex[i+1].position.z());
               data.push_back(h_vertex[i].normal.x());
               data.push_back(h_vertex[i].normal.y());
               data.push_back(h_vertex[i].normal.z());
               data.push_back(h_vertex[i+3].position.x());
               data.push_back(h_vertex[i+3].position.y());
               data.push_back(h_vertex[i+3].position.z());
               data.push_back(h_vertex[i].normal.x());
               data.push_back(h_vertex[i].normal.y());
               data.push_back(h_vertex[i].normal.z());
               data.push_back(h_vertex[i+2].position.x());
               data.push_back(h_vertex[i+2].position.y());
               data.push_back(h_vertex[i+2].position.z());
               data.push_back(h_vertex[i].normal.x());
               data.push_back(h_vertex[i].normal.y());
               data.push_back(h_vertex[i].normal.z());
           }
        //qDebug()<<"tranverse data.length"<<data.length();
    }
    /*----TopData-------*/
    for(int i=0;i<h_vertex.size()-1;i=i+2){
       topdata.push_back(h_vertex[i].position.x());
       topdata.push_back(h_vertex[i].position.y());
       topdata.push_back(h_vertex[i].position.z());
       topdata.push_back(h_vertex[i].texcoord.x());
       topdata.push_back(h_vertex[i].texcoord.y());
    }
}
void house::addPoint(float px, float py,float pz){
    int index=this->vertex_num;
    vertex *a=new vertex();
    a->position.setX(px);
    a->position.setY(py);
    a->position.setZ(pz);
    h_vertex.push_back(*a);
    vertex_num++;
}
void house::init()
{
    build_shader=new QOpenGLShaderProgram;
    build_shader->addShaderFromSourceFile(QOpenGLShader::Vertex,"E:/Qt/3DCity/surface.vert");
    build_shader->addShaderFromSourceFile(QOpenGLShader::Fragment,"E:/Qt/3DCity/surface.frag");
    build_shader->link();
    build_shader->bind();
    top_shader=new QOpenGLShaderProgram;
    top_shader->addShaderFromSourceFile(QOpenGLShader::Vertex,"E:/Qt/3DCity/topsurface.vert");
    top_shader->addShaderFromSourceFile(QOpenGLShader::Fragment,"E:/Qt/3DCity/topsurface.frag");
    top_shader->link();
    top_shader->bind();

    vao= new QOpenGLVertexArrayObject;
    vbo= new QOpenGLBuffer;
    vao->create();vbo->create();
    vao->bind();vbo->bind();
    vbo->allocate(&data[0],data.length()*sizeof (float));
    vao=new QOpenGLVertexArrayObject;
    vbo=new QOpenGLBuffer;
    vao->create();vao->bind();
    vbo->create();vbo->bind();
    vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo->allocate(&data[0],data.length()*sizeof (float));
    //m_vbo->write(0,&building[1].data[0],building[1].data.length());
    build_shader->setAttributeBuffer("attrib_position",GL_FLOAT,0,3,sizeof (float)*6);
    build_shader->setAttributeBuffer("attrib_normal",GL_FLOAT,3*sizeof(float),3,sizeof (float)*6);
//        build_shader->setAttributeBuffer(0,GL_FLOAT,0,3,sizeof (float)*6);
//        build_shader->setAttributeBuffer(1,GL_FLOAT,3,3,sizeof (float)*6);
    build_shader->enableAttributeArray(0);
    build_shader->enableAttributeArray(1);
    /*---top  surface----*/
    topvao=new QOpenGLVertexArrayObject;
    topvbo=new QOpenGLBuffer;
    topvao->create();topvao->bind();
    topvbo->create();topvbo->bind();
    topvbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    topvbo->allocate(&topdata[0],topdata.length()*sizeof (float));
    //m_vbo->write(0,&building[1].data[0],building[1].data.length());
    top_shader->setAttributeBuffer("attrib_position",GL_FLOAT,0,3,sizeof (float)*5);
    top_shader->setAttributeBuffer("attrib_texcoord",GL_FLOAT,3*sizeof(float),2,sizeof (float)*5);
    top_shader->enableAttributeArray(0);
    top_shader->enableAttributeArray(1);
    vao->release();vbo->release();build_shader->release();
    topvao->release();topvbo->release();top_shader->release();

}
void house::draw()
{


}
void house::Normalrevise()
{
   //surround surface cw ccw judge
    //找边界点 前后两节点 连接算向量积

    double d=0.0;
    for(int i=0;i<h_vertex.size()-2;i=i+2){
        d+=-0.5*((h_vertex[i+2].position.y()+h_vertex[i].position.y())*
                (h_vertex[i+2].position.x()-h_vertex[i].position.x()));
        //qDebug()<<h_vertex[i+2].position<<"  "<<h_vertex[i].position;
    }
    //qDebug()<<d;
    if(d<0)
    {
        for(int i=0;i<h_vertex.size();i++){
            h_vertex[i].normal=-h_vertex[i].normal;
        }
    }

}
