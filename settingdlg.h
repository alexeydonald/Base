#ifndef SETTINGDLG_H
#define SETTINGDLG_H

#include <QDialog>
#include "MainData.h"

namespace Ui {
    class SettingDlg;
}

class SettingDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDlg(QWidget *parent = 0);
    ~SettingDlg();

private:
    Ui::SettingDlg *ui;


private slots:
    void on_Cancel_clicked();
    void on_ok_clicked();

};

#endif // SETTINGDLG_H
