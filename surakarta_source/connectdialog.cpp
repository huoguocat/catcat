#include "connectdialog.h"
#include "ui_connectdialog.h"
#include <QMessageBox>

// ConnectDialog 构造函数，初始化 UI 和成员变量
ConnectDialog::ConnectDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConnectDialog)
    , pconnectInfo(nullptr)
{
    ui->setupUi(this);
    // 设置对话框的背景图片
    this->setStyleSheet("QDialog{background-image:url(:/photo/mid2.jpg)}");
}

// ConnectDialog 析构函数，释放 UI 资源
ConnectDialog::~ConnectDialog()
{
    delete ui;
}

// 获取 ConnectInfo 对象指针
ConnectInfo* ConnectDialog::GetConnectInfo() const
{
    return this->pconnectInfo;
}

// 处理连接按钮点击事件的槽函数
void ConnectDialog::onClickConnectSlot()
{
    if(this->pconnectInfo == nullptr)
    {
        // 获取用户名和颜色选择
        ui->LabelUserName->text();
        QString color = ui->comboBoxColor->currentText();
        int indexCur = ui->comboBoxColor->currentIndex();
        if(indexCur == 0)
        {
            color = "";
        }
        // 创建 ConnectInfo 对象
        this->pconnectInfo = new ConnectInfo(
            this,
            ui->lineEditUserName->text(),
            color,
            ui->lineEditIp->text(),
            ui->lineEditPort->text()
            );
        // 连接信号和槽
        connect(this->pconnectInfo, &ConnectInfo::hasConnected, this, &ConnectDialog::Connected);
        connect(this->pconnectInfo, &ConnectInfo::hasConnectError, this, &ConnectDialog::ConnectError);
    }

    // 尝试连接到服务器
    this->pconnectInfo->ConnectToServer(HumenSocket_Index);
}

// 处理连接成功的槽函数
void ConnectDialog::Connected()
{
    // 关闭对话框并接受
    QDialog::accept();
}

// 处理连接错误的槽函数
void ConnectDialog::ConnectError()
{
    if(this->pconnectInfo != nullptr)
    {
        // 删除 ConnectInfo 对象并重置指针
        delete this->pconnectInfo;
        this->pconnectInfo = nullptr;
    }
    // 显示错误消息
    QMessageBox::warning(this, "error", "connect server error");
}

// 处理断开连接按钮点击事件的槽函数
void ConnectDialog::onClickOutlineSlot()
{
    // 关闭对话框并接受
    QDialog::accept();
}
