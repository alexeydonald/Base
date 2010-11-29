#ifndef PLANDIALOG_H
#define PLANDIALOG_H

#include "ListDialog.h"
#include "MessageHandler.h"
#include "MainData.h"
#include "UserWidgets.h"
#include "UserModels.h"
#include "UserDelegates.h"

#include <QLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QList>
#include <QDialog>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QStringList>
#include <QDataWidgetMapper>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlQuery>
#include <QDateTime>
#include <QStackedWidget>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QCloseEvent>
#include <QKeyEvent>


class PlanDialog: public QDialog
{
    Q_OBJECT
    public:
        PlanDialog(QWidget* parent,bool create,int doc_id=0,bool accept=false);
        enum plan_return{ACCEPT,REJECT};
    private:
        //---------------����� ����������-------------------------
        QString name;
        QWidget* parent;
        int field_index;
        bool create;
        int page_counter;
        int di_id;
        int doc_status;
        bool admin_flag;
        bool plan_changed;
        //---------------����� �������----------------------------
        QGroupBox* group_page;
        QGroupBox* group_all;

        QPushButton* button_savePage;
        QPushButton* button_revertPage;
        QPushButton* button_saveAll;
        QPushButton* button_revertAll;
        QPushButton* button_exit;
        QPushButton* button_export;

        QPushButton* button_accept;
        QPushButton* button_reject;

        QPushButton* button_next;
        QPushButton* button_prev;

        QStackedWidget* widget_data;
        QHBoxLayout* layout_top;

        QVBoxLayout* layout_all;
        QVBoxLayout* layout_group;
        QVBoxLayout* layout_page;

        QHBoxLayout* layout_button;
        QVBoxLayout* layout_main;
        //-----------������ �����----------------------------------
        void fitWidth(QTableView* view,int x1,int x2,int delta);
        void closeEvent ( QCloseEvent * event );
        void keyPressEvent ( QKeyEvent * event );


        //--------------������ 1-�� ��������--------------------------
        QStandardItemModel * model_study;
        QStandardItemModel* model_init_data;
        int document_id;
        QDataWidgetMapper* mapper_init_data;
        QStringList list_itog_header;
        QSqlTableModel *prof_table;

        //-----------������� 1-�� ��������
        QLabel* label_header1;
        QLabel* label_header2;
        QLabel* label_table;
        QLabel* label_faculty;
        QLabel* label_faculty_data;
        QLabel* label_fio;
        QLabel* label_itog;

        QLineEdit* edit_year;
        ComboSurname* combo_fio;
        QTableView* view_study;
        QTableWidget* table_itog;

        QHBoxLayout* layout_year;
        QHBoxLayout* layout_faculty;
        QHBoxLayout* layout_fio;
        QVBoxLayout* layout_page1;
        QWidget* widget_page1;

        //----------------������ 1-�� ��������------------------------

        QWidget* make1();
        void initializeStudyModel(QStandardItemModel * model_study);
        QTableView* createStudyView(QStandardItemModel * model_study,bool make=true);
        void initializeInitDataModel(QStandardItemModel* model_init_data);
        QDataWidgetMapper* mapInitDataModel(QStandardItemModel* model_init_data,bool make=true);

        void addDocsClassesRow(QStandardItemModel* model);
        void addDocsIndRow(QStandardItemModel* model);
        void fitTableWidth(QTableWidget* table);
        void countItog(int col,int row);
        void countColItog(int col);
        void saveStudy(QStandardItemModel* model);
        void updateStudy();
        void saveInitData(QStandardItemModel* model);
        void updateInitData();
        void savePage1();
        void revertPage1();
        void saveEmpty1();

        //-----------------������ 2-�� ��������-------------------------
        bool create2;
        QString de_type_umr;
        MetodModel* model_umr;
        QSqlTableModel* model_events;
        QSqlTableModel* model_report_type;
        int max_rows_umr;
        MetodDelegate* delegate_metod;
        //-----------------������� 2-�� ��������-------------------------
        QLabel* label_header;
        QTableView* view_umr;
        QVBoxLayout* layout_page2;

        QWidget* widget_page2;
        //------------------������ 2-�� ��������-------------------------
        QWidget* make2();

        void initializeMrModel(MetodModel* model_mr,QString de_type,int max_rows,bool silent);
        void createMrView(QTableView* view_mr, MetodModel* model_mr,bool make=true);
        void saveMr(MetodModel* model,QString de_type);
        void updateMr(MetodModel* model);
        void savePage2();
        void revertPage2();
        void saveEmpty2();

        //----------------������ 3-�� ��������------------------------------
        bool create3;
        int max_rows_nmr;
        int max_rows_omr;
        int max_rows_gnir;

        QString de_type_nmr;
        QString de_type_omr;
        QString de_type_gnir;

        MetodModel* model_nmr;
        MetodModel* model_omr;
        MetodModel* model_gnir;
        //-----------------������� 3-�� ��������----------------------------
        QLabel* label_nmr;
        QLabel* label_omr;
        QLabel* label_gnir;

        QTableView* view_nmr;
        QTableView* view_omr;
        QTableView* view_gnir;

        QVBoxLayout* layout_page3;
        QWidget* widget_page3;

        QWidget* make3();
        //-----------------������ 3-�� ��������-----------------------------
        void savePage3();
        void revertPage3();
        void saveEmpty3();


        //-----------------������ 4-�� ��������-----------------------------

        int max_rows_train;
        int max_rows_dis;
        int max_rows_science;

        int max_cols_train;
        int max_cols_dis;
        int max_cols_science;

        QStandardItemModel* model_train;
        QStandardItemModel* model_dis;
        QStandardItemModel* model_science;

        bool create4;

        //-----------------������� 4-�� ��������----------------------------
        QLabel* label_train;
        QLabel* label_dis;
        QLabel* label_science;

        QTableView* view_train;
        QTableView* view_dis;
        QTableView* view_science;

        QVBoxLayout* layout_page4;
        QWidget* widget_page4;

        //--------------������ 4-�� ��������---------------------------------
        void initializeOtherModel(QStandardItemModel* model,QString table,QString srch,int max_cols,int max_rows,bool silent);

        void createTrainView(QTableView* view,QStandardItemModel* model,bool made=true);
        void createDisView(QTableView* view,QStandardItemModel* model,bool made=true);
        void createScienceView(QTableView* view,QStandardItemModel* model,bool made=true);

        void resizeTrainHeader();
        void saveOther(QStandardItemModel* model,QString table,QString list);
        void updateOther(QStandardItemModel* model,QString table,QStringList list_str);
        void savePage4();
        void saveEmpty4();
        void revertPage4();

        QWidget* make4();

private slots:
        //-----------------����� �����----------------------------------
        void onSaveAll();
        void onRevertAll();
        void onSavePage();
        void onRevertPage();
        void onNext();
        void onPrev();
        void onExit();
        void onModelChanged(QStandardItem* item);
        void onAccept();
        void onReject();

        //-----------------����� 1-�� ��������--------------------------
        void onCombo(int idx);
        void onYearChanged(QString str);
        void onMapperChanged(int index);

 signals:
         void returnButton(int);

 };

#endif // PLANDIALOG_H
