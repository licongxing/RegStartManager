#ifndef ADDSTART_H
#define ADDSTART_H

#include <QWidget>
#include <QDebug>
#include <windows.h>

namespace Ui {
class AddStart;
}

class AddStart : public QWidget
{
    Q_OBJECT

public:
    explicit AddStart(QWidget *parent = nullptr);
    ~AddStart();
signals:
    void addDone();
private slots:
    void on_cancelButton_clicked();

    void on_sureButton_clicked();

private:
    Ui::AddStart *ui;
    HKEY mRunKey;
};

#endif // ADDSTART_H
