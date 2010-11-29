#include "LoginDialog.h"

//-----------------------------------------------
LoginDialog::LoginDialog():QDialog(0,Qt::MSWindowsFixedSizeDialogHint)
{
    name="Вход в систему";
    setWindowTitle(name);

    label_login=new QLabel("Логин:",this);
    label_password=new QLabel("Пароль:",this);

    edit_login=new QLineEdit(this);
    edit_password=new QLineEdit(this);
    edit_password->setEchoMode(QLineEdit::Password);

    button_ok=new QPushButton("ОК",this);
    button_settings=new QPushButton("Настройки",this);
    button_cancel=new QPushButton("Отмена",this);

    layout_login=new QHBoxLayout;
    layout_login->addWidget(label_login);
    layout_login->addWidget(edit_login);

    layout_password=new QHBoxLayout;
    layout_password->addWidget(label_password);
    layout_password->addWidget(edit_password);

    layout_button=new QHBoxLayout;
    layout_button->addWidget(button_ok);
    layout_button->addWidget(button_cancel);
    layout_button->addWidget(button_settings);

    layout_main=new QVBoxLayout;
    layout_main->addLayout(layout_login);
    layout_main->addLayout(layout_password);
    layout_main->addLayout(layout_button);

    setLayout(layout_main);

    connect(button_ok,SIGNAL(clicked()),this,SLOT(onOk()));
    connect(button_cancel,SIGNAL(clicked()),this,SLOT(onCancel()));
    connect(button_settings,SIGNAL(clicked()),this,SLOT(onSettings()));

}
//---------------------------------------------------------------------
void LoginDialog::onSettings()
{
    emit returnButton(SETTINGS);
}

//---------------------------------------------------------------------
void LoginDialog::onCancel()
{
    emit returnButton(CANCEL);
    close();
}

//---------------------------------------------------------------------
void LoginDialog::onOk()
{
    saveData();
    emit returnButton(OK);
}
//----------------------------------------------------------------------
void LoginDialog::saveData()
{
    settings_data* s_data=MainData::Instance()->getSettingsData();
    s_data->username=edit_login->text();
    s_data->password=edit_password->text();
}

