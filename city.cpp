#include "city.h"
#include <QOpenGLWidget>
#include <QWidget>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFunctions_4_2_Core>
#include <QMouseEvent>
#include "house.h"
#include "camera.h"
#include "stb_image.h"
#define PI 3.1415926

/*-------tessellation callback-------*/
void CALLBACK beginCallback(GLenum which){
    glBegin(which);
}
void CALLBACK endCallback(void){
    glEnd();
}
void CALLBACK errorCallback(GLenum errorCode){
    const GLubyte *estring;
    estring=gluErrorString(errorCode);
    exit(0);
}
void CALLBACK vertexCallback(GLvoid *data)
{
    GLdouble *ptr = (GLdouble*)data;
    glVertex3dv(ptr);
    //glTexCoord2d(*(ptr+3),*(ptr+4));
    //qDebug()<<"uv:"<<*(ptr+3)<<","<<*(ptr+4);
}


city::city(QWidget *parent) : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    //data import
    QFile housefile("C:\\Users\\yy\\Desktop\\house\\house.txt");
    if(!housefile.open(QIODevice::ReadOnly|QIODevice::Text))//open specific file
    {
        qDebug()<<"file is failed to open "<<endl;
    }
    else
        qDebug()<<"open complete"<<endl;
    QTextStream txtInput(&housefile);
    //QTextStream cin(stdin,QIODevice::ReadOnly);
    //QTextStream cout(stdout,QIODevice::WriteOnly);
    txtInput.setRealNumberPrecision(2);
    //housefile.readLine();
    int count=-1;
    // string str;
    while(!txtInput.atEnd()){

        QString line=housefile.readLine();
        //qDebug()<<line<<line.length()<<endl;

        if(line.length()<10)
        {
            if(line=="0\n")
            {
                continue;
            }
            else
            {
                count++;
                int temp=line.toInt();
                house *a=new house();
                building.push_back(*a);
                building[count].houseID=temp;
            }
        }
        else
        {
            QStringList strlist=line.split(" ");
            //str=strlist[0].toStdString();

            float input_x=strlist[0].toFloat();
            if(input_x>building[count].px_max){building[count].px_max=input_x;}
            if(input_x<building[count].px_min){building[count].px_min=input_x;}
            float input_y=strlist[1].toFloat();
            if(input_y>building[count].py_max){building[count].py_max=input_y;}
            if(input_y<building[count].py_min){building[count].py_min=input_y;}
            float input_z=strlist[2].toFloat();
            if(input_z>building[count].pz_max){building[count].pz_max=input_z;}
            if(input_z<building[count].pz_min){building[count].pz_min=input_z;}
            float input_zz=strlist[3].toFloat();
            if(input_zz>building[count].pz_max){building[count].pz_max=input_zz;}
            if(input_zz<building[count].pz_min){building[count].pz_min=input_zz;}
            building[count].addPoint(input_x,input_y,input_z);
            building[count].addPoint(input_x,input_y,input_zz);
        }
        //data input complete but the decimal need to solve
    }
    //building range
    qDebug()<<building[0].px_max<<" "<<building[0].px_min;
    //input data generate;
    for(int i=0;i<building.size()-1;i++)
    {
        building[i].caculateNormal(); //data need to be caculated before generate
        building[i].caculateTexcoord(); //generate the texcoord
        building[i].GenerateData();  //change data generate methods

    }
//    for (int i=0;i<building[121].data.length();i++) {
//       qDebug()<<building[121].data[i];
//    }
// change normal vertex  test
//    for(int i=27;i<52;i=i+6){
//        building[121].data[i]=-1;
//        building[121].data[i+1]=0;
//    }
//    for (int i=0;i<building[121].data.length();i++) {
//       qDebug()<<building[121].data[i];
//    }

    m_camera=new Camera();
    m_camera->Camera_pos=QVector3D(538800,377450,1000);
    m_camera->Camera_front=QVector3D(0,0,-1);
    m_camera->Camera_up=QVector3D(0,1,0);
    m_camera->Camera_right=QVector3D::crossProduct(m_camera->Camera_front,m_camera->Camera_up);

}



void city::initializeGL()
{
    this->initializeOpenGLFunctions();
    glViewport(0,0,this->width(),this->height());
    glClearColor(0.2f,0.3f,0.3f,1.0f);


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
    mvMatrixLoc=build_shader->uniformLocation("mv_Matrix");
    pMatrixLoc=build_shader->uniformLocation("proj_Matrix");
    lightColorLoc=build_shader->uniformLocation("lightColor");
    lightPositionLoc=build_shader->uniformLocation("lightPosition");
    EyeDirectionLoc=build_shader->uniformLocation("EyeDirection");
    NormalMatrixLoc=build_shader->uniformLocation("NormalMatrix");
    ourTextureLoc=build_shader->uniformLocation("ourTexture");
    NormalMatrix=mvMatrix.normalMatrix();
    //qDebug()<<mvMatrix.normalMatrix();
    //ourTextureLoc=build_shader->uniformLocation("ourTexture");

//    int topmvMatrixLoc=top_shader->uniformLocation("mv_Matrix");
//    int toppMatrixLoc=top_shader->uniformLocation("proj_Matrix");
//    int toplightColorLoc=top_shader->uniformLocation("lightColor");
//    int toplightDirectionLoc=top_shader->uniformLocation("lightDirection");
//    int topHalfVectorLoc=top_shader->uniformLocation("HalfVector");
//    int topNormalMatrixLoc=top_shader->uniformLocation("NormalMatrix");
//    int topourTextureLoc=top_shader->uniformLocation("ourTexture");

/*    top_shader->setUniformValue(toppMatrixLoc,pMatrix);
    top_shader->setUniformValue(topmvMatrixLoc,mvMatrix);
    top_shader->setUniformValue(toplightDirectionLoc,QVector3D(1,1,1));
    top_shader->setUniformValue(toplightColorLoc,QVector3D(1.0,1.0,1.0));
    top_shader->setUniformValue(topHalfVectorLoc,QVector3D(0,0,1));
     QMatrix3x3 NormalMatrix=mvMatrix.normalMatrix();
    top_shader->setUniformValue(topNormalMatrixLoc,NormalMatrix); */ //NormalMatrix need


    //vao  vbo
    for(int i=0;i<building.size()-1;i++){
        building[i].vao=new QOpenGLVertexArrayObject;
        building[i].vbo=new QOpenGLBuffer;
        building[i].vao->create();building[i].vao->bind();
        building[i].vbo->create();building[i].vbo->bind();
        building[i].vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
        building[i].vbo->allocate(&building[i].data[0],building[i].data.length()*sizeof (float));
        //m_vbo->write(0,&building[1].data[0],building[1].data.length());
        build_shader->setAttributeBuffer("attrib_position",GL_FLOAT,0,3,sizeof (float)*6);
        build_shader->setAttributeBuffer("attrib_normal",GL_FLOAT,3*sizeof(float),3,sizeof (float)*6);
//        build_shader->setAttributeBuffer(0,GL_FLOAT,0,3,sizeof (float)*6);
//        build_shader->setAttributeBuffer(1,GL_FLOAT,3,3,sizeof (float)*6);
        build_shader->enableAttributeArray(0);
        build_shader->enableAttributeArray(1);
        /*---top  surface----*/
        building[i].topvao=new QOpenGLVertexArrayObject;
        building[i].topvbo=new QOpenGLBuffer;
        building[i].topvao->create();building[i].topvao->bind();
        building[i].topvbo->create();building[i].topvbo->bind();
        building[i].topvbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
        building[i].topvbo->allocate(&building[i].topdata[0],building[i].topdata.length()*sizeof (float));
        //m_vbo->write(0,&building[1].data[0],building[1].data.length());
        top_shader->setAttributeBuffer("attrib_position",GL_FLOAT,0,3,sizeof (float)*5);
        top_shader->setAttributeBuffer("attrib_texcoord",GL_FLOAT,3*sizeof(float),2,sizeof (float)*5);
        top_shader->enableAttributeArray(0);
        top_shader->enableAttributeArray(1);
        building[i].vao->release();building[i].vbo->release();build_shader->release();
        building[i].topvao->release();building[i].topvbo->release();top_shader->release();
    }

    /*---top surface----*/


/*-------------------texture----------*/
//    int width,height,nrChannels;
//    unsigned char* image_data=stbi_load("C:\\Users\\yy\\Desktop\\house\\container.jpg",&width,&height,&nrChannels,0);

//    glGenTextures(1,&texture);
//    glBindTexture(GL_TEXTURE_2D,texture);
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//        //glPixelStoref(
//        if(image_data){
//           glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,image_data);
//           glGenerateMipmap(GL_TEXTURE_2D);
//           glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
//          glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
//          glEnable(GL_TEXTURE_GEN_S);
//           glEnable(GL_TEXTURE_GEN_T);

//           qDebug()<<"success to load texture"<<"width,height"<<width<<height<<nrChannels<<endl;
//        }
//        else {
//            qDebug()<<"fail to load texture"<<endl;
//        }
//    stbi_image_free(image_data);

    m_texture=new QOpenGLTexture(QImage("C:\\Users\\yy\\Desktop\\house\\container.jpg"), QOpenGLTexture::GenerateMipMaps); //直接生成绑定一个2d纹理, 并生成多级纹理MipMaps
    if(!m_texture->isCreated()){
        qDebug() << "Failed to load texture";
    }
    // set the texture wrapping parameters
    m_texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);// 等于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    m_texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    m_texture->setMinificationFilter(QOpenGLTexture::Linear);   //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->generateMipMaps();

}

void city::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    //setting camera
    //can inherit in class camera ;

    //glBindTexture(GL_TEXTURE_2D,texture);//
    /*----top surface----*/
   // top_shader->bind();

    GLdouble coords[30][5]; //house 261 vertex size >20
    for(int i=0;i<building.size()-1;i++){

        mvMatrix.setToIdentity();
        mvMatrix.lookAt(m_camera->Camera_pos,m_camera->Camera_pos+m_camera->Camera_front,m_camera->Camera_up);
        pMatrix.setToIdentity();
        pMatrix.perspective(m_camera->Zoom,GLfloat(this->width())/this->height(),1.0f,1000.0f);

        //qDebug()<<"lookAt process"<<mvMatrix<<endl;

        build_shader->bind();
        {
            glActiveTexture(GL_TEXTURE0);
            m_texture->bind();
            build_shader->setUniformValue(pMatrixLoc,pMatrix);
            build_shader->setUniformValue(mvMatrixLoc,mvMatrix);
            build_shader->setUniformValue(ourTextureLoc,0);
            //qDebug()<<"sourround"<<mvMatrix<<endl;
            //build_shader->setUniformValue(lightDirectionLoc,QVector3D(0,0,1));
            build_shader->setUniformValue(lightColorLoc,QVector3D(1.0,1.0,1.0));
            //build_shader->setUniformValue(HalfVectorLoc,QVector3D(1,1,1)+m_camera->Camera_pos);
            //build_shader->setUniformValue(NormalMatrixLoc,mvMatrix.normalMatrix());
            build_shader->setUniformValue(lightPositionLoc,QVector3D(540000,380000,1000));
            build_shader->setUniformValue(EyeDirectionLoc,m_camera->Camera_front);
            building[i].vao->bind();
            //QOpenGLVertexArrayObject::Binder vaoBind(building[i].vao);
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glDrawArrays(GL_QUADS,0,building[i].data.length()/6);
            building[i].vao->release();

            m_texture->release();
        }
        build_shader->release();




        //top_shader->bind();
        {
            m_texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);// 等于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            m_texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // set texture filtering parameters
            m_texture->setMinificationFilter(QOpenGLTexture::Linear);   //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
            m_texture->generateMipMaps();
            glActiveTexture(0);
            m_texture->bind();
            int topmvMatrixLoc=top_shader->uniformLocation("mv_Matrix");
            int toppMatrixLoc=top_shader->uniformLocation("proj_Matrix");
            int toplightColorLoc=top_shader->uniformLocation("lightColor");
            int toplightDirectionLoc=top_shader->uniformLocation("lightDirection");
            int topHalfVectorLoc=top_shader->uniformLocation("HalfVector");
            top_shader->setUniformValue(toppMatrixLoc,pMatrix);
            top_shader->setUniformValue(topmvMatrixLoc,mvMatrix);
            //qDebug()<<"topsurface"<<mvMatrix<<endl;
            top_shader->setUniformValue(toplightDirectionLoc,QVector3D(0,0,1));
            top_shader->setUniformValue(toplightColorLoc,QVector3D(1.0,1.0,1.0));
            top_shader->setUniformValue(topHalfVectorLoc,QVector3D(1,1,1)+m_camera->Camera_pos);
            top_shader->setUniformValue("ourTexture",0);
            top_shader->setUniformValue("NormalMatrix",mvMatrix.normalMatrix());
            //QOpenGLFunctions_4_2_Core::glUseProgram(top_shader->programId());

            building[i].topvao->bind();
            top_shader->enableAttributeArray(0);  //
            top_shader->enableAttributeArray(1);
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            if(building[i].topdata.length()>25){

                GLUtesselator *tobj=gluNewTess();
                gluTessCallback(tobj, GLU_TESS_BEGIN,  (void (CALLBACK*)()) beginCallback);
                gluTessCallback(tobj, GLU_TESS_END,    (void (CALLBACK*)()) endCallback);
                gluTessCallback(tobj, GLU_TESS_VERTEX, (void (CALLBACK*)()) vertexCallback);
                gluTessCallback(tobj, GLU_TESS_ERROR,  (void (CALLBACK*)()) errorCallback);



                for(int vID=0;vID<building[i].topdata.length()/5;vID++){
                        coords[vID][0]=building[i].topdata[5*vID];
                        coords[vID][1]=building[i].topdata[5*vID+1];
                        coords[vID][2]=building[i].topdata[5*vID+2];
                        coords[vID][3]=building[i].topdata[5*vID+3];
                        coords[vID][4]=building[i].topdata[5*vID+4];
                   }
                gluTessNormal(tobj,0.0,0.0,1.0);
                gluTessBeginPolygon(tobj, &building[i].topdata[0]);                   // with NULL data
                    gluTessBeginContour(tobj);
                    for(int vID=0;vID<building[i].topdata.length()/5;vID++){
                        gluTessVertex(tobj,coords[vID],coords[vID]);
                       }
                    //qDebug()<<tobj<<endl;
                    gluTessEndContour(tobj);
                 gluTessEndPolygon(tobj);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                glDrawArrays(GL_QUADS,0,building[i].topdata.length()/5);
            }
            building[i].topvao->release();
            m_texture->release();
        }
        top_shader->release();

    }
    //glFlush();

}

void city::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    pMatrix.perspective(m_camera->Zoom,GLfloat(w)/h,1.0f,1000.0f);
}
//wheel event
void city::wheelEvent(QWheelEvent *event)
{
    m_camera->Zoom=m_camera->Zoom-event->delta()*0.1;
    if(m_camera->Zoom<0.0)m_camera->Zoom=45.0f;
    update();

}
//mouse event
void city::mousePressEvent(QMouseEvent *event)
{
    m_lastPos=event->pos();
    //qDebug()<<"mouse position"<<event->pos();
}
void city::mouseMoveEvent(QMouseEvent *event)
{

    float dx=event->x()-m_lastPos.x();
    float dy=event->y()-m_lastPos.y();
    //qDebug()<<"mouse MOVE event"<<endl;
    if(event->buttons()&Qt::LeftButton){
        m_camera->Camera_pos+=m_camera->Camera_up*dy*5;
        m_camera->Camera_pos+=m_camera->Camera_right*dx*5;
        //qDebug()<<m_camera->Camera_pos<<endl;

    }
    if(event->buttons()&Qt::RightButton){
        QVector3D front;
        m_camera->Pitch=m_camera->Pitch+dy*0.5;
        m_camera->Yaw=m_camera->Yaw+dx*0.5;
        if(m_camera->Pitch > 89.0f)
          m_camera->Pitch =  89.0f;
        if(m_camera->Pitch < -89.0f)
          m_camera->Pitch = -89.0f;
        front.setX(cos(m_camera->Yaw*PI/180)*cos(m_camera->Pitch*PI/180));
        front.setY(sin(m_camera->Pitch*PI/180));
        front.setZ(sin(m_camera->Yaw*PI/180)*cos(m_camera->Pitch*PI/180));
        //C++ RAD   DEG
        m_camera->Camera_front=front.normalized();
        m_camera->Camera_right=QVector3D::crossProduct(m_camera->Camera_front,QVector3D(0,1,0)).normalized();
        //cross worldup;
        m_camera->Camera_up=QVector3D::crossProduct(m_camera->Camera_right,m_camera->Camera_front).normalized();
    }

    m_lastPos=event->pos();

    update();

}
//keypress event
void city::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_W:
            m_camera->Camera_pos+=10*m_camera->Camera_up;
            break;
        case Qt::Key_S:
            m_camera->Camera_pos-=10*m_camera->Camera_up;
            break;
        case Qt::Key_A:
            m_camera->Camera_pos.setX(m_camera->Camera_pos.x()-10);
            break;
        case Qt::Key_D:
            m_camera->Camera_pos.setX(m_camera->Camera_pos.x()+10);
            break;
        case Qt::Key_Q:
            m_camera->Camera_pos.setZ(m_camera->Camera_pos.z()-10);
            break;
        case Qt::Key_E:
            m_camera->Camera_pos.setZ(m_camera->Camera_pos.z()+10);
            break;
        default:
            break;
    }
    update();
}
