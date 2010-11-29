#include "settingdlg.h"
#include "ui_settingdlg.h"

SettingDlg::SettingDlg(QWidget *parent) :
    QDialog(parent,Qt::MSWindowsFixedSizeDialogHint),
    ui(new Ui::SettingDlg)
{

    ui->setupUi(this);
    setWindowTitle("Настройки подключения к БД");
   // ui->edit_address->setText("localhost");
    //ui->edit_dbname->setText("db_msius");
    settings_data* s_data=MainData::Instance()->getSettingsData();
    ui->edit_address->setText(s_data->hostname);
    ui->edit_port->setText(s_data->port);
    ui->edit_dbname->setText(s_data->dbname);
}

SettingDlg::~SettingDlg()
{
    delete ui;
}


void SettingDlg::on_ok_clicked()
{
    settings_data* s_data=MainData::Instance()->getSettingsData();
    s_data->hostname=ui->edit_address->text();
    s_data->port=ui->edit_port->text();
    s_data->dbname=ui->edit_dbname->text();
    close();
}

void SettingDlg::on_Cancel_clicked()
{
     close();
}
