#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "searchdialog.h"

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
    searchDialog *sdig;



signals:
    void Target(QVector3D *tar);
private slots:

    void on_actionsearch_triggered();
    void recieveData(QVector3D *coord);

private:
    Ui::MainWindow *ui;
    QOpenGLWidget *city;

};
#endif // MAINWINDOW_H
