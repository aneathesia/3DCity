#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>

namespace Ui {
class searchDialog;
}

class searchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit searchDialog(QWidget *parent = nullptr);
    ~searchDialog();

private slots:
    void on_Search_clicked();

signals:
    void sendData(QVector3D*);

private:
    Ui::searchDialog *ui;
};

#endif // SEARCHDIALOG_H
