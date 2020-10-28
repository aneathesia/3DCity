#include "flycontrol.h"
#include "ui_flycontrol.h"
#include <QDebug>
#include <QTimer>
#include <QMessageBox>


flycontrol::flycontrol(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::flycontrol)
{
    ui->setupUi(this);
    //data input and show
    model = new QStandardItemModel;

    this->ui->tableView->setModel(model);

    this->model->setHorizontalHeaderItem(0,new QStandardItem("PID"));
    this->model->setHorizontalHeaderItem(1,new QStandardItem("Coord_X"));
    this->model->setHorizontalHeaderItem(2,new QStandardItem("Coord_Y"));
    this->model->setHorizontalHeaderItem(3,new QStandardItem("Coord_Z"));
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //context
//    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
//    ui->tableView->setColumnWidth(0,30);
//    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    flycam= new Camera();
}

flycontrol::~flycontrol()
{
    delete ui;
}

void flycontrol::on_FlyForward_clicked()
{
    //turn to last point

}
void flycontrol::add_point(double posx,double posy,double posz)
{

    {
        data.push_back(QVector3D(posx,posy,posz+5.0));
        emit route(data);
    } //avoid flying on the floor
    //qDebug()<<data.size();
    for(int i =0 ; i<data.size();i++){
       // qDebug()<<data[i];
        this->model->setItem(i,0,new QStandardItem(QString::number(i)));
        this->model->setItem(i,1,new QStandardItem(QString::number(data[i].x())));
        this->model->setItem(i,2,new QStandardItem(QString::number(data[i].y())));
        this->model->setItem(i,3,new QStandardItem(QString::number(data[i].z())));
    }
 }

void flycontrol::on_addFlyPoint_clicked(bool checked)
{
    qDebug()<<checked;
    //ui->addFlyPoint->setDown(true);

    //ui->addFlyPoint->setAutoExclusive(true);
    if(checked){
        //ui->addFlyPoint->setStyleSheet("background-color:rgb(175,0,0)");

        connect(parent(),SIGNAL(add_RoutinePoint(double,double,double)),this,SLOT(add_point(double,double,double)));
        //connect(this,SIGNAL(route(QVector<QVector3D>)),parent(),SLOT(paintroute(QVector<QVector3D>)));
    }
    else
        disconnect(parent(),SIGNAL(add_RoutinePoint(double,double,double)),this,SLOT(add_point(double,double,double)));
        //disconnect(this,SIGNAL(route(QVector<QVector3D>)),parent(),SLOT(paintroute(QVector<QVector3D>)));

}

void flycontrol::on_Fly_clicked()
{


    if(data.size()>1){

        flycam->Camera_pos=data[0];
        //normalize
        flycam->Camera_front=(data[1]-data[0]).normalized();
        flycam->Camera_right=QVector3D::crossProduct(flycam->Camera_front,QVector3D(0,0,-1)).normalized();
        flycam->Camera_up=QVector3D::crossProduct(flycam->Camera_front,flycam->Camera_right).normalized();

        timer=new QTimer;
        connect(timer, SIGNAL(timeout()), this, SLOT(updateCamera()));
        //connect(timer, SIGNAL(timeout()), this, SLOT(camera_turn(QVector3D,QVector3D)));
        timer->start(40);
    }
    else{

    }
    //qobject_cast<QTimer *> (sender())
    //flycam->Camera_pos+=flycam->Camera_front*fps;  //25fps
    //timer->stop();

}
void flycontrol::updateCamera()
{

    if(flycam->Camera_pos.distanceToPoint(data[PID+1])<(data[PID+1]-data[PID]).distanceToPoint(QVector3D(0,0,0))/fps)
        {
        if((data.size()-1)==(PID+1)){
            qDebug()<<data.size()<<PID;
            timer->stop();
            PID=0;
        }
        else{
            //qDebug()<<data.size()<<PID;
            //qDebug()<<"start"<<flycam->Camera_front<<"end"<<(data[PID+2]-data[PID+1]).normalized();


           // qDebug()<<flycam->Camera_front<<(data[PID+2]-data[PID+1]).normalized();
//            qDebug()<<(flycam->Camera_front.x()-(data[PID+2]-data[PID+1]).normalized().x())<0.000001;
//            qDebug()<<(flycam->Camera_front.y()-(data[PID+2]-data[PID+1]).normalized().y())<0.000001;
//            qDebug()<<(flycam->Camera_front.z()-(data[PID+2]-data[PID+1]).normalized().z())<0.000001;

            //qDebug()<<(data[PID+2]-data[PID+1]-flycam->Camera_front).distanceToPoint(QVector3D(0,0,0))<0.000001);

            if(stage==25)
            {
                flycam->Camera_pos=data[PID+1];
                flycam->Camera_front=(data[PID+2]-data[PID+1]).normalized();
//            flycam->Camera_right=QVector3D::crossProduct(flycam->Camera_front,QVector3D(0,0,1)).normalized();
//            flycam->Camera_up=QVector3D::crossProduct(flycam->Camera_front,flycam->Camera_right).normalized();
                flycam->Camera_up=QVector3D(0,0,1);
                flycam->Camera_right=QVector3D::crossProduct(flycam->Camera_front,flycam->Camera_up);
                PID++;
                stage=0;
            }
            else
            {
                QVector3D start = (data[PID+1]-data[PID]).normalized();
                QVector3D  end = (data[PID+2]-data[PID+1]).normalized();
                flycam->Camera_front = (start + (end - start)*stage/ 25).normalized();
                //qDebug()<<"stage"<< stage << "camera.front"<<flycam->Camera_front;
                stage++;
            }
            //camera_pos+=data2-data1/fps
        }
    }
    else{
            flycam->Camera_pos=flycam->Camera_pos+(data[PID+1]-data[PID])/fps;


    }

//    qDebug()<<(data[PID+1]-data[PID])/fps;
//    qDebug()<<"position"<<flycam->Camera_pos<<"front"<<flycam->Camera_front<<
//              "right"<<flycam->Camera_right<<"up"<<flycam->Camera_up;
//    qDebug()<<fps;
    emit transmitCamera(flycam);

}


void flycontrol::on_FlyPause_clicked()
{
    if(timer->isActive()) {
        timer->stop();
    }
    else
        timer->start(40);
}

void flycontrol::on_SpeedUp_clicked()
{
    fps=fps-5;
    update();
}

void flycontrol::on_SpeedDown_clicked()
{
    fps=fps+5;
    update();
}

//void flycontrol::camera_turn(QVector3D start, QVector3D end,int stage)
//{

//    QVector3D Vturn=end-start;
//    qDebug()<<"Vturn"<<Vturn;
//    //QVector3D lookat_position = start +Vturn / 15;  //角度旋转次数
//    for(int i=0;i<61;i++){
//        flycam->Camera_front = (start + Vturn * i / 60).normalized();
//        flycam->Camera_up=QVector3D(0,0,1);
//        flycam->Camera_right =QVector3D::crossProduct(flycam->Camera_front,flycam->Camera_up);
//        qDebug()<<"camera_position"<<flycam->Camera_pos<<"camera_front"<<flycam->Camera_front;
//        //connect(timer, SIGNAL(timeout()), this, SLOT(updateCamera()));
//        emit transmitCamera(flycam);

//        break;
//    }

//}
