#ifndef FLYCONTROL_H
#define FLYCONTROL_H

#include <QDialog>
#include <QStandardItemModel>
#include <QVector>
#include <QVector3D>
#include "camera.h"

namespace Ui {
class flycontrol;
}

class flycontrol : public QDialog
{
    Q_OBJECT

public:
    explicit flycontrol(QWidget *parent = nullptr);
    ~flycontrol();
    QStandardItemModel *model;
    Camera *flycam;
    float fps=60.0;
    int PID=0;
    QTimer *timer;
signals:
    void transmitCamera(Camera *cam);
private slots:
    void updateCamera();
    void on_FlyForward_clicked();
    void add_point(double posx,double posy,double posz);

    void on_addFlyPoint_clicked(bool checked);


    void on_Fly_clicked();

    void on_FlyPause_clicked();

private:
    Ui::flycontrol *ui;
    QVector<QVector3D> data={};
};

#endif // FLYCONTROL_H
