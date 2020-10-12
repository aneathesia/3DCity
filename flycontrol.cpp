#include "flycontrol.h"
#include "ui_flycontrol.h"
#include <QDebug>
#include <QTimer>


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

    }
    else
        disconnect(parent(),SIGNAL(add_RoutinePoint(double,double,double)),this,SLOT(add_point(double,double,double)));

}

void flycontrol::on_Fly_clicked()
{


    if(data.size()>1){

        flycam->Camera_pos=data[0];
        //normalize
        flycam->Camera_front=(data[1]-data[0]).normalized();
        flycam->Camera_right=QVector3D::crossProduct(flycam->Camera_front,QVector3D(0,0,-1)).normalized();
        flycam->Camera_up=QVector3D::crossProduct(flycam->Camera_front,flycam->Camera_right).normalized();
    }
    timer=new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCamera()));
    timer->start(40);
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
        }
        else{
            qDebug()<<data.size()<<PID;
            flycam->Camera_pos=data[PID+1];
            flycam->Camera_front=(data[PID+2]-data[PID+1]).normalized();
//            flycam->Camera_right=QVector3D::crossProduct(flycam->Camera_front,QVector3D(0,0,1)).normalized();
//            flycam->Camera_up=QVector3D::crossProduct(flycam->Camera_front,flycam->Camera_right).normalized();
            flycam->Camera_up=QVector3D(0,0,1);
            flycam->Camera_right=QVector3D::crossProduct(flycam->Camera_front,flycam->Camera_up);
            PID++;
            //camera_pos+=data2-data1/fps
        }
    }
    else{
            flycam->Camera_pos=flycam->Camera_pos+(data[PID+1]-data[PID])/fps;
    }

//    qDebug()<<(data[PID+1]-data[PID])/fps;
//    qDebug()<<"position"<<flycam->Camera_pos<<"front"<<flycam->Camera_front<<
//              "right"<<flycam->Camera_right<<"up"<<flycam->Camera_up;
//    //qDebug()<<fps;
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
