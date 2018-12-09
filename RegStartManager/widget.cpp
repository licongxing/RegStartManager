#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 设置表格
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setColumnWidth(0, 100);
    ui->tableWidget->setColumnWidth(1, 200);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    //遍历 注册表下的启动项，这里我们将枚举注册表中的
    //"HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run"子键下的键值项
    if(ERROR_SUCCESS != RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                                      "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\",
                                      0,
                                      KEY_ALL_ACCESS,&mRunKey)){
        qDebug() << "注册表打开失败";
        return;
    }

    refreshTable();
}

void Widget::refreshTable(){
    // 清空表格
    int row = ui->tableWidget->rowCount();
    for( int i = 0; i < row; i++){
        ui->tableWidget->removeRow(0);
    }

    unsigned long i = 0;
    char name[1024] = {0};
    BYTE data[1024] = {0};
    unsigned long nameLen = 1024;
    unsigned long dataLen = 1024;//DWORD = unsigned long
//    ui->tableWidget->
    LSTATUS lRet;
    while(true){
        lRet = RegEnumValueA(mRunKey,i,name,&nameLen,nullptr,nullptr,data,&dataLen);
        if(lRet ==  ERROR_NO_MORE_ITEMS)
            break;
        qDebug() << "i = " << i;

        ui->tableWidget->insertRow(0);
        ui->tableWidget->setItem(0,0,new QTableWidgetItem(name));
        ui->tableWidget->setItem(0,1,new QTableWidgetItem((char*)data));
        i++;
    }
}

Widget::~Widget()
{
    // 关闭注册表句柄
    RegCloseKey(mRunKey);
    delete ui;
}

void Widget::on_exitButton_clicked()
{
    this->close();
}

void Widget::on_addButton_clicked()
{
    mAddStart = new AddStart();
    connect(mAddStart,&AddStart::addDone,this,[=](){
        this->refreshTable();
    });
    mAddStart->show();
}

void Widget::on_delButton_clicked()
{
    int row = this->ui->tableWidget->currentRow();
    QString key = this->ui->tableWidget->item(row,0)->text();
    LSTATUS lRet;
    lRet = RegDeleteValueA(mRunKey,key.toStdString().c_str());
    if(ERROR_SUCCESS != lRet){
        qDebug() << "删除启动项失败";
        return;
    }
    refreshTable();
}
