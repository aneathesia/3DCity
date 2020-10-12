#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "searchdialog.h"
#include "flycontrol.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QOpenGLWidget;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //searchDialog *sdig=NULL;
    //bool sdigCreated=false;



signals:
    void Target(QVector3D *tar);
    void add_RoutinePoint(double posx, double posy,double posz);
private slots:

    void on_actionsearch_triggered();
    void recieveData(QVector3D *coord);
    void ScreenToWorld(double posx,double posy,double posz);
    //void ScreenToWorld(double posX,double posY,double posZ);

    void on_actionroutine_triggered();

private:
    Ui::MainWindow *ui;
    QOpenGLWidget *city;

};
#endif // MAINWINDOW_H
