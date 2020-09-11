#include "search.h"
#include "ui_search.h"

search::search(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::search)
{
    ui->setupUi(this);
    connect(QDialogButtonBox
}

search::~search()
{
    delete ui;
}
