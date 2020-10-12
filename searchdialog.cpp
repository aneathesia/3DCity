#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QVector3D>
#include <QDebug>

searchDialog::searchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::searchDialog)
{
    ui->setupUi(this);
}

searchDialog::~searchDialog()
{
    delete ui;

}

void searchDialog::on_Search_clicked()
{
    QVector3D *coord=new QVector3D();
    coord->setX(ui->coordx->text().toFloat());
    coord->setY(ui->coordy->text().toFloat());
    coord->setZ(ui->coordz->text().toFloat());
    qDebug()<<*coord;
    emit sendData(coord);
}
