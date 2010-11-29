#include "SettingsDialog.h"

//--------------------------------------------------------

SettingsDialog::SettingsDialog(QWidget* parent):QDialog(parent,Qt::MSWindowsFixedSizeDialogHint)
{
    name="Настройки подключения к БД";
    setWindowTitle(name);

    label_address=new QLabel("Адрес сервера: ",this);
    label_port=new QLabel("Порт:                 ",this);
    label_dbname=new QLabel("Имя БД:             ",this);

    edit_address=new QLineEdit(this);
    edit_port=new QLineEdit(this);
    edit_dbname=new QLineEdit(this);

    settings_data* s_data=MainData::Instance()->getSettingsData();
    edit_address->setText(s_data->hostname);
    edit_port->setText(s_data->port);
    edit_dbname->setText(s_data->dbname);

    button_ok=new QPushButton("ОК",this);
    button_cancel=new QPushButton("Отмена",this);

    layout_address=new QHBoxLayout;
    layout_address->addWidget(label_address);
    layout_address->addWidget(edit_address);

    layout_port=new QHBoxLayout;
    layout_port->addWidget(label_port);
    layout_port->addWidget(edit_port);

    layout_dbname=new QHBoxLayout;
    layout_dbname->addWidget(label_dbname);
    layout_dbname->addWidget(edit_dbname);

    layout_button=new QHBoxLayout;
    layout_button->addWidget(button_ok);
    layout_button->addWidget(button_cancel);

    layout_main=new QVBoxLayout;
    layout_main->addLayout(layout_address);
    layout_main->addLayout(layout_port);
    layout_main->addLayout(layout_dbname);
    layout_main->addLayout(layout_button);

    setLayout(layout_main);

    connect(button_ok,SIGNAL(clicked()),this,SLOT(onOk()));
    connect(button_cancel,SIGNAL(clicked()),this,SLOT(onCancel()));
}
//-----------------------------------------------------------------------
void SettingsDialog::onOk()
{
    saveData();
    close();
}

//------------------------------------------------------------------------
void SettingsDialog::onCancel()
{   close(); }

//-------------------------------------------------------------------------
void SettingsDialog::saveData()
{
    settings_data* s_data=MainData::Instance()->getSettingsData();
    s_data->hostname=edit_address->text();
    s_data->port=edit_port->text();
    s_data->dbname=edit_dbname->text();
 }


