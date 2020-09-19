#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "city.h"
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "searchdialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this,SIGNAL(Target(QVector3D *)),ui->openGLWidget,SLOT(recieveTarget(QVector3D *)));
}



MainWindow::~MainWindow()
{
    delete ui;
}





void MainWindow::on_actionsearch_triggered()
{
    //judge generate already?
        sdig=new searchDialog;
        connect(sdig,SIGNAL(sendData(QVector3D*)),this,SLOT(recieveData(QVector3D *)));
        sdig->show();
}

void MainWindow::recieveData(QVector3D *coord)
{
    QVector3D *SearchTarget=coord;
    qDebug()<<"target position"<<*SearchTarget;
    emit Target(SearchTarget);
}
