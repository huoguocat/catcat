#include "connectdialog.h"
#include "ui_connectdialog.h"
#include <QMessageBox>

ConnectDialog::ConnectDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConnectDialog)
    , pconnectInfo(nullptr)
{
    ui->setupUi(this);
    this->setStyleSheet("QDialog{background-image:url(:/photo/mid2.jpg)}");
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

ConnectInfo* ConnectDialog::GetConnectInfo() const
{
    return this->pconnectInfo;
}


void ConnectDialog::onClickConnectSlot()
{
    if(this->pconnectInfo == nullptr)
    {
        ui->LabelUserName->text();
        QString color = ui->comboBoxColor->currentText();
        int indexCur = ui->comboBoxColor->currentIndex();
        if(indexCur==0)
        {
            color = "";
        }
        this->pconnectInfo = new ConnectInfo(
            this
            , ui->lineEditUserName->text()
            , color
            , ui->lineEditIp->text()
            , ui->lineEditPort->text()
            );
        connect(this->pconnectInfo, &ConnectInfo::hasConnected, this, &ConnectDialog::Connected);
        connect(this->pconnectInfo, &ConnectInfo::hasConnectError, this, &ConnectDialog::ConnectError);
    }

    this->pconnectInfo->ConnectToServer(HumenSocket_Index);
}

void ConnectDialog::Connected()
{
    QDialog::accept();
}

void ConnectDialog::ConnectError()
{
    if(this->pconnectInfo != nullptr)
    {
        delete this->pconnectInfo;
        this->pconnectInfo = nullptr;
    }
    QMessageBox::warning(this, "error", "connect server error");
}

void ConnectDialog::onClickOutlineSlot()
{
    QDialog::accept();
}
