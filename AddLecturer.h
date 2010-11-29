#ifndef ADDLECTURER_H
#define ADDLECTURER_H

#include "MessageHandler.h"

#include <QString>
#include <QDialog>
#include <QLayout>
#include <QPushButton>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QTableView>
#include <QList>
#include <QPair>
#include <QCloseEvent>
#include <QKeyEvent>


class AddLecturer: public QDialog
{
    Q_OBJECT
    public:
        AddLecturer(QWidget* parent);
    private:
        bool changed;
        QString name;
        QList<int> row_list;
        QList<int> del_row_list;

        QSqlRelationalTableModel* model;

        QTableView* table_view;

        QPushButton* button_revert;
        QPushButton* button_add;
        QPushButton* button_del;
        QPushButton* button_save;
        QPushButton* button_exit;

        QGridLayout* layout_button;
        QVBoxLayout* layout_main;

        void initializeModel(QSqlRelationalTableModel* model);
        QTableView* createView(QSqlRelationalTableModel* model);
        void fitWidth();
        bool checkRows();
        void removeDeleted();
        void closeEvent ( QCloseEvent * event );
        void keyPressEvent ( QKeyEvent * event );

    private slots:
        bool onSave();
        void onExit();
        void onRevert();
        void onAdd();
        void onDel();
        void onDataChanged(const QModelIndex& topLeft,const QModelIndex& bottomRight);
};


#endif // ADDLECTURER_H
