#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <windows.h>
#include <QDebug>
#include "addstart.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    void refreshTable();

private slots:
    void on_exitButton_clicked();

    void on_addButton_clicked();

    void on_delButton_clicked();

private:
    Ui::Widget *ui;
    HKEY mRunKey;
    AddStart* mAddStart;
};

#endif // WIDGET_H
