#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include "MessageHandler.h"

#include <QDialog>
#include <QListWidget>
#include <QLayout>
#include <QPushButton>
#include <QString>
#include <QStringList>

class AddDialog: public QDialog
{
    Q_OBJECT
    public:
        enum add_dialog_ret{ADD_LECTURRER,EXIT};

        AddDialog(QWidget* parent);
    private:
        QString name;
        QStringList item_list;

        QHBoxLayout* layout_main;
        QVBoxLayout* layout_button;
        QListWidget* list_widget;
        QPushButton* button_add;
        QPushButton* button_exit;


     signals:
        void returnButton(int);

     private slots:

         void onExit();
         void onAdd();
         void onClicked(QListWidgetItem* item);


};









#endif // ADDDIALOG_H
