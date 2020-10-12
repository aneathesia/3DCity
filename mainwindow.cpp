#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "city.h"
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QStatusBar>
#include "searchdialog.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this,SIGNAL(Target(QVector3D *)),ui->openGLWidget,SLOT(recieveTarget(QVector3D *)));
    connect(ui->openGLWidget,SIGNAL(sendstatusCoordination(double,double,double)),this,SLOT(ScreenToWorld(double,double,double)));
    //connect(ui->openGLWidget,SIGNAL(sendstatusCoordination(double,double,double),flycontrol,SLOT(add_point(double,double,double)));
}



MainWindow::~MainWindow()
{
    delete ui;

}





void MainWindow::on_actionsearch_triggered()
{
    //judge generate already?
        searchDialog *sdig=new searchDialog(this);
        connect(sdig,SIGNAL(sendData(QVector3D*)),this,SLOT(recieveData(QVector3D *)));
        sdig->show();

}

void MainWindow::recieveData(QVector3D *coord)
{
    QVector3D *SearchTarget=coord;
    qDebug()<<"target position"<<*SearchTarget;
    emit Target(SearchTarget);
}
void MainWindow::ScreenToWorld(double posx,double posy,double posz)
{
    QString mes="  ";
    //qDebug()<<"output coordination"<<posx<<posy<<posz;

    mes=mes+"X:"+QString::number(posx)+" Y:"+QString::number(posy)+" Z:"+QString::number(posz);
    this->statusBar()->setSizeGripEnabled(false);
    this->statusBar()->showMessage(mes,3000);
//    ui->statusbar->setSizeGripEnabled(false);
//    ui->statusbar->showMessage(mes,3000);
    //mousepress add point
    //qDebug()<<Qt::LeftButton;
        emit add_RoutinePoint(posx,posy,posz);
}

void MainWindow::on_actionroutine_triggered()
{
    flycontrol  *flycon=new flycontrol(this);
    flycon->show();
    connect(flycon,SIGNAL(transmitCamera(Camera*)),ui->openGLWidget,SLOT(recieveCamera(Camera*)));
}
