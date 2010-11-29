#ifndef LISTDIALOG_H
#define LISTDIALOG_H

# include <QPushButton>
# include <QLayout>
# include <QString>
# include <QDialog>
# include <QLabel>
# include <QSqlQuery>
# include <QTableView>
# include <QStandardItemModel>
# include <QSqlTableModel>
#include <QCloseEvent>

# include "MainData.h"
# include "UserDelegates.h"

class ListDialog:public QDialog
{
    Q_OBJECT
    public:
        enum list_dialog_ret{OPEN,ADD,CREATE,DELETE,RENEW,EXPORT,EXIT};

        ListDialog();
        current_document selectedDocument();
        bool selectedStatus();
    private:
        QString name;
        QStandardItemModel* model;
        QSqlTableModel* model_prof;
        QString query_string;

        QLabel* label_header;

        QTableView* table_view;

        QPushButton* button_open;
        QPushButton* button_add;
        QPushButton* button_create;
        QPushButton* button_delete;
        QPushButton* button_renew;
        QPushButton* button_export;
        QPushButton* button_exit;

        QVBoxLayout* layout_table;
        QVBoxLayout* layout_button;
        QHBoxLayout* layout_main;

        void fitWidth(QTableView* view,int x1,int x2,int delta);
        void initializeListModel(QStandardItemModel* model,QSqlTableModel* model_prof,QString q_str);
        void initializeListView(QTableView* table_view,QStandardItemModel* model);
        void closeEvent ( QCloseEvent * event );
        void keyPressEvent (QKeyEvent * event);
   signals:
        void returnButton(int ret);

   public slots:
        void onRenew();

   private slots:
        void onOpen();
        void onExit();
        void onCreate();
        void onAdd();
        void onDel();
        void onClicked(const QModelIndex& idx);
        void onExport();
  };



#endif // LISTDIALOG_H

