#include "UserDelegates.h"

//######################Study Model Delegate####################################

 StudyDelegate::StudyDelegate(QObject* parent):QItemDelegate(parent)
 {}
 //-----------------------------------------------------------------------------
  QWidget* StudyDelegate::createEditor(QWidget* parent,
     const QStyleOptionViewItem &option ,
     const QModelIndex &index ) const
 {
      if(index.column()==3)
         return QItemDelegate::createEditor(parent,option,index);
      if(index.column()==4||
         index.column()==5||
         index.column()==7||
         index.column()==8)
      {
        QSpinBox* editor=new QSpinBox(parent);
        editor->setMinimum(0);
        editor->setMaximum(365);
        return editor;
      }
      return 0;
 }
 //-----------------------------------------------------------------------------
 void StudyDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
 {
     if(index.column()==3)
         QItemDelegate::setEditorData(editor,index);
     if(index.column()==4||
         index.column()==5||
         index.column()==7||
         index.column()==8)
      {
        int value=index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox=static_cast<QSpinBox*>(editor);
        spinBox->setValue(value);
      }
}
//------------------------------------------------------------------------------
void StudyDelegate::setModelData(QWidget* editor,QAbstractItemModel* model,
                                    const QModelIndex &index) const
 {
     if(index.column()==3)
         QItemDelegate::setModelData(editor,model,index);
     if(index.column()==4||
         index.column()==5||
         index.column()==7||
         index.column()==8)
      {
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        int value = spinBox->value();
        model->setData(index, QVariant(value), Qt::EditRole);
      }
 }
//------------------------------------------------------------------------------
void StudyDelegate::updateEditorGeometry(QWidget* editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
 {
     editor->setGeometry(option.rect);
 }

//######################Metod Jobs Delegate####################################

 MetodDelegate::MetodDelegate(QSqlTableModel* model_ev,QSqlTableModel* model_type,QStandardItemModel* model,QObject* parent):QItemDelegate(parent)
 {
    model_parent=model;

    model_events=model_ev;
    model_report_type=model_type;

 }
 //-----------------------------------------------------------------------------
  QWidget* MetodDelegate::createEditor(QWidget* parent,
     const QStyleOptionViewItem &option ,
     const QModelIndex &index ) const
 {
      if(index.column()==3||
         index.column()==4||
         index.column()==6)
         return QItemDelegate::createEditor(parent,option,index);
      if(index.column()==2)
      {
        QComboBox* combo=new QComboBox(parent);
        combo->setEditable(true);
        combo->setModel(model_events);
        combo->setModelColumn(1);
        return combo;
      }
      if(index.column()==5)
      {
        QComboBox* combo=new QComboBox(parent);
        combo->setEditable(true);
        combo->setModel(model_report_type);
        combo->setModelColumn(1);
        return combo;

      }
      return 0;
 }
 //-----------------------------------------------------------------------------
 void MetodDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
 {
      if(index.column()==3||
         index.column()==4||
         index.column()==6)
         return QItemDelegate::setEditorData(editor,index);
      if(index.column()==2)
      {
        QModelIndex idx=model_parent->index(index.row(),8);
        int i=idx.data().toInt();
        QComboBox* combo=static_cast<QComboBox*>(editor);
        combo->setCurrentIndex(i);
      }
      if(index.column()==5)
      {
        QModelIndex idx=model_parent->index(index.row(),9);
        int i=idx.data().toInt();
        QComboBox* combo=static_cast<QComboBox*>(editor);
        combo->setCurrentIndex(i);
      }
}
//------------------------------------------------------------------------------
void MetodDelegate::setModelData(QWidget* editor,QAbstractItemModel* model,
                                    const QModelIndex &index) const
 {
     if(index.column()==3||
         index.column()==4||
         index.column()==6)
         return QItemDelegate::setModelData(editor,model,index);

      if(index.column()==2||index.column()==5)
      {
        QComboBox* combo=static_cast<QComboBox*>(editor);
        int idx=combo->currentIndex();
        model->setData(index,QVariant(idx),Qt::EditRole);
      }
}
//------------------------------------------------------------------------------
void MetodDelegate::updateEditorGeometry(QWidget* editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
 {
     editor->setGeometry(option.rect);
 }

//###################################SpinBox Delegate##########################
 SpinboxDelegate::SpinboxDelegate(QList<int> list,QObject* parent):QItemDelegate(parent)
 {
    int_list=list;
 }
 //-----------------------------------------------------------------------------
  QWidget* SpinboxDelegate::createEditor(QWidget* parent,
     const QStyleOptionViewItem &option ,
     const QModelIndex &index ) const
 {
      if(int_list.contains(index.column()))
      {

        QSpinBox* editor=new QSpinBox(parent);
        editor->setMinimum(0);
        return editor;
      }
      return QItemDelegate::createEditor(parent,option,index);
 }
 //-----------------------------------------------------------------------------
 void SpinboxDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
 {
     if(int_list.contains(index.column()))
      {
        int value=index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox=static_cast<QSpinBox*>(editor);
        spinBox->setValue(value);
     }
     QItemDelegate::setEditorData(editor,index);
}
//------------------------------------------------------------------------------
void SpinboxDelegate::setModelData(QWidget* editor,QAbstractItemModel* model,
                                    const QModelIndex &index) const
 {
     if(int_list.contains(index.column()))
      {
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        int value = spinBox->value();
        model->setData(index, QVariant(value), Qt::EditRole);
      }
      QItemDelegate::setModelData(editor,model,index);
 }
//------------------------------------------------------------------------------
void SpinboxDelegate::updateEditorGeometry(QWidget* editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
 {
     editor->setGeometry(option.rect);
 }
//########################################No Delegate###########################


QWidget * NoDelegate::createEditor(QWidget* parent,const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    return 0;
}
