#include "city.h"
#include <QOpenGLWidget>
#include <QWidget>
#include <QtWidgets/QToolTip>
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
    //connect(this,SIGNAL(sendstatusCoordination(double,double,double)),this,SLOT(ScreenToWorld(double,double,double)));
    //connect(,SIGNAL(Target(QVector3D *)),city,SLOT(recieveTarget(QVector3D *)));
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
        building[i].Normalrevise();// change cw ccw
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
//    qDebug()<<"begin"<<endl;
//    for (int i=0;i<building[0].data.length();i++) {
//       qDebug()<<building[0].data[i];
//    }
//    for(int i=0;i<building[0].h_vertex.size();i++)
//    {
//        qDebug()<<building[0].h_vertex[i].normal;
//    }
//    building[0].Normalrevise();
//qDebug()<<"after revise"<<endl;
//    building[0].GenerateData();
//    for(int i=0;i<building[0].h_vertex.size();i++)
//    {
//        qDebug()<<building[0].h_vertex[i].normal;
//    }


    //groundVertices
    //range：539071   538187   377975   377537   61.72   27.58
    m_ground=new ground(QVector3D(538097,378105,27.58),QVector3D(539900,378105,27.58),QVector3D(539900,376752,27.58),QVector3D(538097,376752,27.58));
    m_ground=new ground(QVector3D(538000,378100,27.58),QVector3D(539900,378100,27.58),QVector3D(539900,376752,27.58),QVector3D(538000,376752,27.58));

    //    m_ground=new ground();
//    m_ground->pos.push_back(QVector3D(538000,378200,27.58));
//    m_ground->pos.push_back(QVector3D(539200,378200,27.58));
//    m_ground->pos.push_back(QVector3D(539200,377300,27.58));
//    m_ground->pos.push_back(QVector3D(538000,377300,27.58));

    m_ground->generatedata();
    m_camera=new Camera();
    m_camera->Camera_pos=QVector3D(539000,376000,30);
    m_camera->Camera_front=QVector3D(0,1,0);
    m_camera->Camera_up=QVector3D(0,0,1);
    m_camera->Camera_right=QVector3D::crossProduct(m_camera->Camera_front,m_camera->Camera_up);

}



void city::initializeGL()
{
    this->initializeOpenGLFunctions();
    glViewport(0,0,this->width(),this->height());
    glClearColor(0.2f,0.3f,0.3f,1.0f);


//    build_shader=new QOpenGLShaderProgram;
//    build_shader->addShaderFromSourceFile(QOpenGLShader::Vertex,"E:/Qt/3DCity/surface.vert");
//    build_shader->addShaderFromSourceFile(QOpenGLShader::Fragment,"E:/Qt/3DCity/surface.frag");
//    build_shader->link();
//    build_shader->bind();
//    top_shader=new QOpenGLShaderProgram;
//    top_shader->addShaderFromSourceFile(QOpenGLShader::Vertex,"E:/Qt/3DCity/topsurface.vert");
//    top_shader->addShaderFromSourceFile(QOpenGLShader::Fragment,"E:/Qt/3DCity/topsurface.frag");
//    top_shader->link();
//    top_shader->bind();

    ground_shader=new QOpenGLShaderProgram;
    ground_shader->addShaderFromSourceFile(QOpenGLShader::Vertex,"E:/Qt/3DCity/ground.vert");
    ground_shader->addShaderFromSourceFile(QOpenGLShader::Fragment,"E:/Qt/3DCity/ground.frag");
    ground_shader->link();
    ground_shader->bind();
    quads_shader=new QOpenGLShaderProgram;
    quads_shader->addShaderFromSourceFile(QOpenGLShader::Vertex,"E:/Qt/3DCity/quads.vert");
    quads_shader->addShaderFromSourceFile(QOpenGLShader::Fragment,"E:/Qt/3DCity/quads.frag");
    quads_shader->link();
    quads_shader->bind();
    shadow_mapping_depth=new QOpenGLShaderProgram;
    shadow_mapping_depth->addShaderFromSourceFile(QOpenGLShader::Vertex,"E:/Qt/3DCity/shadow_mapping_depth.vert");
    shadow_mapping_depth->addShaderFromSourceFile(QOpenGLShader::Fragment,"E:/Qt/3DCity/shadow_mapping_depth.frag");
    shadow_mapping_depth->link();
    shadow_mapping_depth->bind();
    shadow_mapping=new QOpenGLShaderProgram;
    shadow_mapping->addShaderFromSourceFile(QOpenGLShader::Vertex,"E:/Qt/3DCity/shadow_mapping.vert");
    shadow_mapping->addShaderFromSourceFile(QOpenGLShader::Fragment,"E:/Qt/3DCity/shadow_mapping.frag");
    shadow_mapping->link();
    shadow_mapping->bind();
    shadow_mapping_top=new QOpenGLShaderProgram;
    shadow_mapping_top->addShaderFromSourceFile(QOpenGLShader::Vertex,"E:/Qt/3DCity/shadow_mapping_top.vert");
    shadow_mapping_top->addShaderFromSourceFile(QOpenGLShader::Fragment,"E:/Qt/3DCity/shadow_mapping_top.frag");
    shadow_mapping_top->link();
    shadow_mapping_top->bind();
    shadow_mapping_ground=new QOpenGLShaderProgram;
    shadow_mapping_ground->addShaderFromSourceFile(QOpenGLShader::Vertex,"E:/Qt/3DCity/shadow_mapping_ground.vert");
    shadow_mapping_ground->addShaderFromSourceFile(QOpenGLShader::Fragment,"E:/Qt/3DCity/shadow_mapping_ground.frag");
    shadow_mapping_ground->link();
    shadow_mapping_ground->bind();


    //vao  vbo
    for(int i=0;i<building.size()-1;i++)
    {
        building[i].init();
    }

    //m_ground
    m_ground->vao=new QOpenGLVertexArrayObject();
    m_ground->vbo=new QOpenGLBuffer();
    m_ground->vao->create();m_ground->vao->bind();
    m_ground->vbo->create();m_ground->vbo->bind();
    m_ground->vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_ground->vbo->allocate(&m_ground->data[0],m_ground->data.length()*sizeof (float));
    ground_shader->setAttributeBuffer("attrib_position",GL_FLOAT,0,3,0);
    //ground_shader->setAttributeBuffer("attrib_texcoord",GL_FLOAT,3*sizeof(float),2,sizeof (float)*5);
    ground_shader->enableAttributeArray(0);
    //ground_shader->enableAttributeArray(1);
    m_ground->vao->release(); m_ground->vbo->release();
//    //FBO
//    m_ground->fbo=new QOpenGLFramebufferObject(1024,1024,QOpenGLFramebufferObject::Depth);


//    //create a shadow texture
//      m_ground->shadowtexture=new QOpenGLTexture(QOpenGLTexture::Target2D);
//      m_ground->shadowtexture->create();
//      m_ground->shadowtexture->bind();
//      m_ground->shadowtexture->setSize(1024,1024);
//    //m_ground->shadowtexture->setData(QOpenGLTexture::RGBA,QOpenGLTexture::UInt8,);
//    m_ground->shadowtexture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToBorder);// 等于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    m_ground->shadowtexture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToBorder);//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    // set texture filtering parameters
//    m_ground->shadowtexture->setMinificationFilter(QOpenGLTexture::Linear);   //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    m_ground->shadowtexture->setMagnificationFilter(QOpenGLTexture::Linear);
//    m_ground->shadowtexture->generateMipMaps();
//    m_ground->fbo->bind();


    //m_ground->fbo->setAttachment(QOpenGLFramebufferObject::Depth);


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
/*---FBO------*/
//    GLuint depthMapFBO;
//    glGenFramebuffers(1, &depthMapFBO);
//    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

//    GLuint depthMap;
//    glGenTextures(1, &depthMap);
//    glBindTexture(GL_TEXTURE_2D, depthMap);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
//                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
//    glDrawBuffer(GL_NONE);
//    glReadBuffer(GL_NONE);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void city::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    //fly transmit to city widget

    mvMatrix.setToIdentity();
    mvMatrix.lookAt(m_camera->Camera_pos,m_camera->Camera_pos+m_camera->Camera_front,m_camera->Camera_up);
    pMatrix.setToIdentity();
    pMatrix.perspective(m_camera->Zoom,GLfloat(this->width()/this->height()),100.0f,3000.0f);

    QVector3D lightPosition=QVector3D(538000,376700,2000);
    QVector3D lightTarget=QVector3D(540000,378200,0);
    QVector3D lightDirection=(lightTarget-lightPosition).normalized();
    QVector3D lightRight=QVector3D::crossProduct(lightDirection,QVector3D(0,0,1));
    QVector3D lightUp=QVector3D::crossProduct(lightRight,lightDirection);
    lightViewMatrix.setToIdentity();
    //lightViewMatrix.lookAt(QVector3D(539000,377400,300),QVector3D(539000,377400,300)+QVector3D(1,1,1),QVector3D(0,-1,1));
    lightViewMatrix.lookAt(lightPosition,lightPosition+lightDirection,lightUp);
    lightProjectionMatrix.setToIdentity();
    lightProjectionMatrix.ortho(-1000,1000,-2000,0,1500,3200); //reference camera space


    //lightProjectionMatrix.perspective(m_camera->Zoom,this->width()/this->height(),100.0f,3000.0f);
    QMatrix4x4 lightSpaceMatrix=lightProjectionMatrix*lightViewMatrix;
    //qDebug()<<"pMatrix"<<pMatrix<<"lightProjectionMatrix"<<lightProjectionMatrix;
    fbo=new QOpenGLFramebufferObject(4096,4096,QOpenGLFramebufferObject::Depth);
    glViewport(0,0,4096,4096); //sampling density
    fbo->bind();
    //m_camera->Camera_pos=QVector3D(539000,376000,30);
    shadow_mapping_depth->bind();
    {
        GLdouble coords[30][5];
        for(int i=0;i<building.size()-1;i++)
        {
        shadow_mapping_depth->setUniformValue("lightSpaceMatrix",lightSpaceMatrix);
        //shadow_mapping_depth->setUniformValue("model",);
        building[i].vao->bind();
                       //QOpenGLVertexArrayObject::Binder vaoBind(building[i].vao);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        glDrawArrays(GL_QUADS,0,building[i].data.length()/6);
        building[i].vao->release();

        building[i].topvao->bind();
        building[i].top_shader->enableAttributeArray(0);  //
        building[i].top_shader->enableAttributeArray(1);
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
          }


        m_ground->vao->bind();
        glDrawArrays(GL_QUADS,0,m_ground->data.length()/3);
        m_ground->vao->release();
    }
   fbo->release();

    glViewport(0,0,this->width(),this->height());
    glActiveTexture(GL_TEXTURE0);
    m_texture->bind();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,fbo->texture());
   // qDebug()<<fbo->texture();
//            ground_shader->bind();
//            {
//                ground_shader->setUniformValue("proj_Matrix",pMatrix);
//                ground_shader->setUniformValue("mv_Matrix",mvMatrix);
//                ground_shader->setUniformValue("shadow2D",0);
//                m_ground->vao->bind();
//                glDrawArrays(GL_QUADS,0,m_ground->data.length()/3);
//                m_ground->vao->release();
//            }
//            ground_shader->release();

    shadow_mapping->bind();
     {
         shadow_mapping->setUniformValue("proj_Matrix",pMatrix);
         shadow_mapping->setUniformValue("mv_Matrix",mvMatrix);
         shadow_mapping->setUniformValue("lightSpaceMatrix",lightSpaceMatrix);
         shadow_mapping->setUniformValue("lightPos",lightPosition);
         shadow_mapping->setUniformValue("viewPos",m_camera->Camera_pos);
         shadow_mapping->setUniformValue("diffuseTexture",0);
         shadow_mapping->setUniformValue("shadowMap",1);
         //surround
         for(int i=0;i<building.size()-1;i++)
         {
             building[i].vao->bind();
              //QOpenGLVertexArrayObject::Binder vaoBind(building[i].vao);
              //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
              glDrawArrays(GL_QUADS,0,building[i].data.length()/6);
              building[i].vao->release();
         }
     }


     shadow_mapping_top->bind();
     {
         shadow_mapping_top->setUniformValue("proj_Matrix",pMatrix);
         shadow_mapping_top->setUniformValue("mv_Matrix",mvMatrix);
         shadow_mapping_top->setUniformValue("lightSpaceMatrix",lightSpaceMatrix);
         shadow_mapping_top->setUniformValue("lightPos",lightPosition);
         shadow_mapping_top->setUniformValue("viewPos",m_camera->Camera_pos);
         shadow_mapping_top->setUniformValue("diffuseTexture",0);
         shadow_mapping_top->setUniformValue("shadowMap",1);
         GLdouble coords[30][5];
         for(int i=0;i<building.size()-1;i++){
         building[i].topvao->bind();
          building[i].top_shader->enableAttributeArray(0);  //
          building[i].top_shader->enableAttributeArray(1);
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
         }
     }

   // glCullFace(GL_FRONT);
     shadow_mapping_ground->bind();
     {
         shadow_mapping_ground->setUniformValue("proj_Matrix",pMatrix);
         shadow_mapping_ground->setUniformValue("mv_Matrix",mvMatrix);
         shadow_mapping_ground->setUniformValue("lightSpaceMatrix",lightSpaceMatrix);
         shadow_mapping_ground->setUniformValue("lightPos",lightPosition);
         shadow_mapping_ground->setUniformValue("viewPos",m_camera->Camera_pos);
         shadow_mapping_ground->setUniformValue("diffuseTexture",0);
         shadow_mapping_ground->setUniformValue("shadowMap",1);

         //ground
         m_ground->vao->bind();

         glDrawArrays(GL_QUADS,0,m_ground->data.length()/3);
         m_ground->vao->release();
     }
 //    glCullFace(GL_BACK);



    GLint    upviewport[4]={0,0,this->width(),this->height()};
    GLdouble modelview[16];

    GLdouble projection[16];
//        glGetIntegerv(GL_VIEWPORT, upviewport);
//        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
//        glGetDoublev(GL_PROJECTION_MATRIX, projection);
//        for(int i=0;i<16;i++){
//            qDebug()<<projection[i];
//        }
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            modelview[4*i+j]=mvMatrix.column(i)[j];
        }
    }
    //output the mat4x4 modelview
//        for(int i=0;i<4;i++){
//            for(int j=0;j<4;j++){
//                qDebug()<<modelview[4*i+j];
//            }
//        }
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            projection[4*i+j]=pMatrix.column(i)[j];
        }
    }
    //output the mat4x4 projection
//        for(int i=0;i<4;i++){
//            for(int j=0;j<4;j++){
//                qDebug()<<projection[4*i+j];
//            }
//        }
//        winX = m_lastPos.x();
//        winY = this->height()-m_lastPos.y();
    winX=m_lastPos.x();
    winY=this->height()-m_lastPos.y();
    glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
    gluUnProject(winX, winY, winZ, modelview, projection, upviewport, &posX, &posY, &posZ);
    //qDebug()<<"position:"<<posX<<","<<posY<<","<<posZ;
    fbo->~QOpenGLFramebufferObject();
    glFlush();
}

void city::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    pMatrix.perspective(m_camera->Zoom,GLfloat(w)/h,100.0f,3000.0f);
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
    //emit pos_building();
    QVector3D coord=QVector3D(posX,posY,posZ);

    if(building_Search(&coord)!=-1){
        QToolTip::showText(event->globalPos(),QString::number(building_Search(&coord)));
    }
    emit sendstatusCoordination(posX,posY,posZ);

    update();
}
void city::mouseMoveEvent(QMouseEvent *event)
{

    float dx=event->x()-m_lastPos.x();
    float dy=event->y()-m_lastPos.y();
    //qDebug()<<"mouse MOVE event"<<event->x()<<event->y()<<endl;
    if(event->buttons()&Qt::LeftButton){
        m_camera->Camera_pos+=m_camera->Camera_up*dy*5;
        m_camera->Camera_pos-=m_camera->Camera_right*dx*5;
        //qDebug()<<m_camera->Camera_pos<<endl;

    }
    if(event->buttons()&Qt::RightButton){
        QVector3D front;
        m_camera->Pitch=m_camera->Pitch+dy*0.5;
        m_camera->Yaw=m_camera->Yaw+dx*0.5;
        if(m_camera->Pitch > 89.9f)
          m_camera->Pitch =  89.9f;
        if(m_camera->Pitch < -89.9f)
          m_camera->Pitch = -89.9f;
//        front.setX(cos(m_camera->Yaw*PI/180)*cos(m_camera->Pitch*PI/180));
//        front.setY(sin(m_camera->Pitch*PI/180));
//        front.setZ(sin(m_camera->Yaw*PI/180)*cos(m_camera->Pitch*PI/180));
//        front.setX(0);
//        front.setY(sin(m_camera->Pitch*PI/180));
//        front.setZ(sin(m_camera->Yaw*PI/180)*cos(m_camera->Pitch*PI/180));
        front.setX(sin(m_camera->Yaw*PI/180)*cos(m_camera->Pitch*PI/180));
        front.setY(cos(m_camera->Yaw*PI/180)*cos(m_camera->Pitch*PI/180));
        front.setZ(sin(m_camera->Pitch*PI/180));
        //C++ RAD   DEG
        m_camera->Camera_front=front.normalized();

//        m_camera->Camera_up.setY(cos(m_camera->Yaw*PI/180)*cos(m_camera->Pitch*PI/180));
//        m_camera->Camera_up.setX(-sin(m_camera->Yaw*PI/180)*cos(m_camera->Pitch*PI/180));
//        m_camera->Camera_up.setZ(sin(m_camera->Pitch*PI/180));
//        m_camera->Camera_up.normalized();
          m_camera->Camera_right=QVector3D::crossProduct(m_camera->Camera_front,QVector3D(0,0,1)).normalized();
        //cross worldup;
        m_camera->Camera_up=QVector3D::crossProduct(m_camera->Camera_right,m_camera->Camera_front).normalized();
        //m_camera->Camera_right=QVector3D::crossProduct(m_camera->Camera_rights,m_camera->Camera_front).normalized();

    }
   //qDebug()<<"camera front"<<m_camera->Camera_front<<"camera_up"<<m_camera->Camera_up<<"pitch"<<m_camera->Pitch<<
             //"yaw"<<m_camera->Yaw;

    m_lastPos=event->pos();

    //emit sendstatusCoordination(posX,posY,posZ);// emit the last point position     press mouse get current point position
    update();

}
//keypress event
void city::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_W:
            m_camera->Camera_pos.setY(m_camera->Camera_pos.y()+10);
            break;
        case Qt::Key_S:
            m_camera->Camera_pos.setY(m_camera->Camera_pos.y()-10);
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
void city::recieveTarget(QVector3D *coord)
{
    Targetcoord=new QVector3D;
    Targetcoord=coord;
    selected=building_Search(Targetcoord);
    m_camera->Camera_pos=QVector3D(Targetcoord->x(),Targetcoord->y(),600);
    m_camera->Camera_front=QVector3D(0,0,-1);
    m_camera->Camera_up=QVector3D(0,1,0);
    m_camera->Camera_right=QVector3D::crossProduct(m_camera->Camera_front,m_camera->Camera_up);
    update();
    qDebug()<<"city target position"<<*Targetcoord<<"building"<<selected;
}
int city::building_Search(QVector3D *coord)
{
    //beyond full range send message

    //search building
    int res=-1;
    for(int i=0;i<building.size()-1;i++){
        if(coord->x()<building[i].px_max&&coord->x()>building[i].px_min&&
           coord->y()<building[i].py_max&&coord->y()>building[i].py_min&&
           coord->z()<building[i].pz_max&&coord->z()>building[i].pz_min)
        {
            res=i;
        }
    }
    return res;
}
void city::recieveCamera(Camera *cam)
{
    m_camera=cam;
    update();
    //qDebug()<<m_camera->Camera_pos <<m_camera->Camera_front <<m_camera->Camera_up;
}

