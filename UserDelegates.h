#ifndef USERDELEGATES_H
#define USERDELEGATES_H

#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QItemDelegate>
#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QString>
#include <QSpinBox>
#include <QComboBox>

//------------------------Study model Delegate----------------------------------
class StudyDelegate : public QItemDelegate
 {
     Q_OBJECT
 public:
     StudyDelegate(QObject* parent=0);

     QWidget *createEditor(QWidget* parent,const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;

     void setEditorData(QWidget* editor,const QModelIndex &index) const;
     void setModelData(QWidget* editor,QAbstractItemModel* model,
                       const QModelIndex &index) const;
     void updateEditorGeometry(QWidget* editor,
         const QStyleOptionViewItem &option,const QModelIndex &index) const;

 };

//----------------------Metod jobs Delegate-------------------------------------

class MetodDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    MetodDelegate(QSqlTableModel* model_ev,QSqlTableModel* model_type,QStandardItemModel* model,QObject* parent=0);

     QWidget *createEditor(QWidget* parent,const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;

     void setEditorData(QWidget* editor,const QModelIndex &index) const;
     void setModelData(QWidget* editor,QAbstractItemModel* model,
                       const QModelIndex &index) const;
     void updateEditorGeometry(QWidget* editor,
         const QStyleOptionViewItem &option,const QModelIndex &index) const;
private:
    QSqlTableModel* model_events;
    QSqlTableModel* model_report_type;
    QStandardItemModel* model_parent;
};

//---------------------Spinbox Delegate-----------------------------------------
class SpinboxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    SpinboxDelegate(QList<int> list,QObject* parent=0);

     QWidget *createEditor(QWidget* parent,const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;

     void setEditorData(QWidget* editor,const QModelIndex &index) const;
     void setModelData(QWidget* editor,QAbstractItemModel* model,
                       const QModelIndex &index) const;
     void updateEditorGeometry(QWidget* editor,
         const QStyleOptionViewItem &option,const QModelIndex &index) const;
private:
    QList<int> int_list;


};

//---------------------No Delegate-----------------------------------------
class NoDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    NoDelegate(QObject* parent=0):QItemDelegate(parent){}

     QWidget *createEditor(QWidget* parent,const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;

};




#endif // USERDELEGATES_H
