#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "city.h"
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "search.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}




