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
       QVector3D temp=QVector3D::normal(temp1,temp2)*(pow((-1),i)); //invert ? *pow((-1),j)
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
