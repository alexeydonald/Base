#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

# include <QString>
# include <QLayout>
# include <QPushButton>
# include <QLineEdit>
# include <QLabel>
# include <QDialog>

# include "MainData.h"
class LoginDialog:public QDialog
{
Q_OBJECT
public:
    enum login_dialog_ret{OK,CANCEL,SETTINGS};

    LoginDialog();
private:
    QString name;

    QLabel* label_login;
    QLabel* label_password;

    QLineEdit* edit_login;
    QLineEdit* edit_password;

    QPushButton* button_ok;
    QPushButton* button_settings;
    QPushButton* button_cancel;

    QHBoxLayout* layout_login;
    QHBoxLayout* layout_password;
    QHBoxLayout* layout_button;

    QVBoxLayout* layout_main;

    void saveData();
//-----------------------------------------------
signals:
    void returnButton(int ret);

private slots:
    void onCancel();
    void onOk();
    void onSettings();
};

#endif // LOGINDIALOG_H
