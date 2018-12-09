#include "addstart.h"
#include "ui_addstart.h"

AddStart::AddStart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddStart)
{
    ui->setupUi(this);
    // 打开注册表句柄
    if(ERROR_SUCCESS != RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                                      "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\",
                                      0,
                                      KEY_ALL_ACCESS,&mRunKey)){
        qDebug() << "注册表打开失败";
        return;
    }
}

AddStart::~AddStart()
{
    // 关闭注册表句柄
    RegCloseKey(this->mRunKey);
    delete ui;
}

void AddStart::on_cancelButton_clicked()
{
    this->close();
}

void AddStart::on_sureButton_clicked()
{
    QString key = ui->startKey->text();
    QString value = ui->startValue->text();
    if(key.isEmpty() || (value.isEmpty())){
        qDebug() << "请正确填写!";
        return;
    }

    LSTATUS lRet;
    lRet = RegSetValueExA(mRunKey,key.toStdString().c_str(),0,REG_SZ,
                   (BYTE*)value.toStdString().c_str(),value.length()+1);
    if( lRet != ERROR_SUCCESS){
        qDebug() << "添加启动项失败";
        return;
    }

    emit addDone();
    this->close();
}
