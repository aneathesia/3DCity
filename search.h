#ifndef SEARCH_H
#define SEARCH_H

#include <QDialog>

namespace Ui {
class search;
}

class search : public QDialog
{
    Q_OBJECT

public:
    explicit search(QWidget *parent = nullptr);
    ~search();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::search *ui;
};

#endif // SEARCH_H
