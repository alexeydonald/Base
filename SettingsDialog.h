#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

# include <QLineEdit>
# include <QLabel>
# include <QLayout>
# include <QPushButton>
# include <QString>
# include <QDialog>
# include <QFileDialog>

#include "MainData.h"


class SettingsDialog: public QDialog
{
Q_OBJECT
public:
    SettingsDialog(QWidget* parent);
private:
    QString name;

    QLabel* label_address;
    QLabel* label_port;
    QLabel* label_dbname;

    QLineEdit* edit_address;
    QLineEdit* edit_port;
    QLineEdit* edit_dbname;

    QPushButton* button_ok;
    QPushButton* button_cancel;

    QHBoxLayout* layout_address;
    QHBoxLayout* layout_port;
    QHBoxLayout* layout_dbname;
    QHBoxLayout* layout_button;

    QVBoxLayout* layout_main;

    void saveData();

private slots:
    void onCancel();
    void onOk();

};



#endif // SETTINGSDIALOG_H
