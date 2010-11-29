#include "Addlecturer.h"

//------------------AddLecturer Dialog Implementation-----------------------------------------------------------
AddLecturer::AddLecturer(QWidget* parent):QDialog(parent,/*Qt::WShowModal|*/Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint)
{
    changed=false;
    row_list.clear();
    del_row_list.clear();
    name="Добавить/Изменить информацию о преподавателях";
    setWindowTitle(name);
    model=new QSqlRelationalTableModel;
    initializeModel(model);

    table_view=createView(model);

    button_save=new QPushButton("Сохранить",this);
    button_exit=new QPushButton("Закрыть",this);
    button_add=new QPushButton("Добавить",this);
    button_revert=new QPushButton("Отменить изменения",this);
    button_del=new QPushButton("Удалить",this);

    layout_button=new QGridLayout();
    layout_button->addWidget(button_add,0,0);
    layout_button->addWidget(button_del,0,1);
    layout_button->addWidget(button_save,1,0);
    layout_button->addWidget(button_revert,1,1);
    layout_button->addWidget(button_exit,2,0);



    layout_main=new QVBoxLayout();
    layout_main->addWidget(table_view);
    layout_main->addLayout(layout_button);

    setLayout(layout_main);
    setMinimumHeight(600);
        
    connect(button_save,SIGNAL(clicked()),this,SLOT(onSave()));
    connect(button_exit,SIGNAL(clicked()),this,SLOT(onExit()));
    connect(button_revert,SIGNAL(clicked()),this,SLOT(onRevert()));
    connect(button_add,SIGNAL(clicked()),this,SLOT(onAdd()));
    connect(button_del,SIGNAL(clicked()),this,SLOT(onDel()));
    connect(model,SIGNAL(dataChanged(const QModelIndex&,const QModelIndex&)),this,SLOT(onDataChanged(const QModelIndex&,const QModelIndex&)));
}

//--------------------------------------------------------------------
void AddLecturer::onDataChanged(const QModelIndex& topLeft,const QModelIndex& bottomRight)
{   changed=true;
    fitWidth();
}

//---------------------------------------------------------------------
bool AddLecturer::onSave()
{
    if(checkRows())
    {
        removeDeleted();
        model->submitAll();
        model->select();
        fitWidth();
        changed=false;
        row_list.clear();
        del_row_list.clear();
        return true;
    }
    else
        return false;
}
//---------------------------------------------------------------------
void AddLecturer::onRevert()
{
    model->revertAll();
    model->select();
    fitWidth();
    changed=false;
    row_list.clear();
    del_row_list.clear();
}
//---------------------------------------------------------------------
void AddLecturer::onAdd()
{
    model->insertRow(model->rowCount());
    row_list.append(model->rowCount()-1);

}
//---------------------------------------------------------------------
void AddLecturer::onDel()
{
   QItemSelectionModel* selectionModel=table_view->selectionModel();
   QModelIndexList list=selectionModel->selectedIndexes();
   QModelIndex index;
   int count=list.count()/model->columnCount();
   int id=0;
   for(int i=0;i<count;++i)
   {
       index=list[i];
       del_row_list.append(index.row());
       table_view->setRowHidden(index.row(),true);
       id=row_list.indexOf(index.row());
       if(id>=0)
           row_list.removeAt(id);
   }
   //fitWidth();
}
//---------------------------------------------------------------------
void AddLecturer::onExit()
{
    close();
}

//----------------------------------------------------------------------------
void AddLecturer::initializeModel(QSqlRelationalTableModel* model)
{
    model->setTable("professor");

    model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);


    model->setRelation(4, QSqlRelation("position","position_id","position_name"));
    model->setRelation(6, QSqlRelation("rank","rank_id","rank_name"));
    model->setRelation(7, QSqlRelation("degree","degree_id","degree_name"));
    model->setRelation(9, QSqlRelation("rate","rate_id","rate_value"));
    model->setRelation(10, QSqlRelation("depts","depts_id","depts_abr"));

    model->setHeaderData(1, Qt::Horizontal, "Имя");
    model->setHeaderData(2, Qt::Horizontal, "Отчество");
    model->setHeaderData(3, Qt::Horizontal, "Фамилия");
    model->setHeaderData(4, Qt::Horizontal, "Должность");
    model->setHeaderData(5, Qt::Horizontal, "Год избрания");
    model->setHeaderData(6, Qt::Horizontal, "Ученое звание");
    model->setHeaderData(7, Qt::Horizontal, "Ученая степень");
    model->setHeaderData(8, Qt::Horizontal, "Год повышения\nквалификации");
    model->setHeaderData(9, Qt::Horizontal, "Ставка");
    model->setHeaderData(10, Qt::Horizontal, "Кафедра");
    model->setHeaderData(11, Qt::Horizontal, "Имя\nпользователя");
    model->setHeaderData(12, Qt::Horizontal, "Пароль");
    model->setHeaderData(13, Qt::Horizontal, "Флаг\nадминистратора");

    model->select();

}

//------------------------------------------------------------------------
QTableView* AddLecturer::createView(QSqlRelationalTableModel* model)
{
     table_view = new QTableView(this);
     table_view->setModel(model);
     table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
     table_view->setItemDelegate(new QSqlRelationalDelegate(table_view));
     table_view->hideColumn(0);
     fitWidth();
     return table_view;
 }

//------------------------------------------------------------------------

void AddLecturer::fitWidth()
{
    int width=0;
    int col=model->columnCount();
    col=col-6;
    for(int i=0;i<col;++i)
    {
        table_view->resizeColumnToContents(i);
        width+=table_view->columnWidth(i);
    }
    table_view->setMinimumWidth(width);
}
//--------------------------------------------------------------------------
bool AddLecturer::checkRows()
{

    int num=row_list.count();
    if(num>0)
    {
        int i,j;
        int col=model->columnCount();
        int f1=model->fieldIndex("professor_pos_year");
        int f2=model->fieldIndex("professor_upg_year");
        int f3=model->fieldIndex("professor_admin");
        QModelIndex id;
        QString tmp;
        bool flag=true;
        bool ok=true;
        for(i=0;(i<num)&&flag;++i)
        {
            for(j=1;(j<col)&&flag;++j)
            {
                id=model->index(row_list[i],j,QModelIndex());
                tmp=id.data().toString();
                if((j==f1)||(j==f2)||(j==f3))
                    tmp.toInt(&ok);
                if(tmp.isEmpty()||!ok) flag=false;
            }
        }
        if(!flag)
        {
            QString message="Данные в ячейке ("+QString::number(row_list[i-1]+1)+",";
            message=message+QString::number(j-1)+") не введены,\nлибо введены некорректно";
            MessageHandler::showMessage(MessageHandler::Error,message,this);
            return false;
        }
   }
   return true;
}
//---------------------------------------------------------------------------
void AddLecturer::removeDeleted()
{
    int col=del_row_list.count();
    QModelIndex id;
    for(int i=0;i<col;++i)
    {
            model->removeRow(del_row_list[i]);
        }
}
//----------------------------------------------------------------------------
void AddLecturer::closeEvent ( QCloseEvent * event )
{
  if(del_row_list.count()!=0||row_list.count()!=0) changed=true;

  if(changed)
  {
        int ret=MessageHandler::showMessage(MessageHandler::CloseWindow,"Данные были изменены.<br>Сохранить?",this);
        switch (ret)
        {
            case QMessageBox::Save :
                if(onSave())
                    QDialog::closeEvent(event);
                break;
            case QMessageBox::Discard :
                model->revertAll();
                QDialog::closeEvent(event);
                break;
            case QMessageBox::Cancel:
                event->ignore();
                break;
            default:
                MessageHandler::showMessage(MessageHandler::Error,"AddLecturer: Неизвестный тип возвращаемого значения!",this);
                event->ignore();
         }
    }
    else
        event->accept();
}
//---------------------
void AddLecturer::keyPressEvent ( QKeyEvent * event )
{
    if(event->key()==Qt::Key_Escape)
        close();
    else
        QDialog::keyPressEvent(event);
}
