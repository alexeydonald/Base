#include "ListDialog.h"

ListDialog::ListDialog():QDialog(0,Qt::WindowMinimizeButtonHint)
{
    settings_data* s_data=MainData::Instance()->getSettingsData();
    name="Документы пользователя "+s_data->username;

    setWindowTitle(name);
    label_header=new QLabel("Список индивидуальных\nпланов работ",this);
    label_header->setAlignment(Qt::AlignHCenter);

    button_open=new QPushButton("Открыть",this);
    if(s_data->isAdmin)
    {
        button_add=new QPushButton("Добавить",this);
        button_create=new QPushButton("Создать",this);
        button_delete=new QPushButton("Удалить",this);
    }
    button_export=new QPushButton("Экспортировать",this);
    button_renew=new QPushButton("Обновить",this);
    button_exit=new QPushButton("Выход",this);

    layout_button=new QVBoxLayout();
    layout_button->addWidget(button_open);
    if(s_data->isAdmin)
    {
        layout_button->addWidget(button_add);
        layout_button->addWidget(button_create);
        layout_button->addWidget(button_delete);
    }
    layout_button->addWidget(button_export);
    layout_button->addWidget(button_renew);
    layout_button->addWidget(button_exit);

    table_view=new QTableView(this);

    layout_table=new QVBoxLayout();
    layout_table->addWidget(label_header);
    layout_table->addWidget(table_view);

    layout_main=new QHBoxLayout();
    layout_main->addLayout(layout_table);
    layout_main->addLayout(layout_button);

    setLayout(layout_main);

    QString q_str;
    connect(button_open,SIGNAL(clicked()),this,SLOT(onOpen()));
    connect(table_view,SIGNAL(doubleClicked(const QModelIndex )),this,SLOT(onClicked(const QModelIndex)));
    connect(button_exit,SIGNAL(clicked()),this,SLOT(onExit()));
    connect(button_renew,SIGNAL(clicked()),this,SLOT(onRenew()));
    connect(button_export,SIGNAL(clicked()),this,SLOT(onExport()));
    if(s_data->isAdmin)
    {
        connect(button_create,SIGNAL(clicked()),this,SLOT(onCreate()));
        connect(button_add,SIGNAL(clicked()),this,SLOT(onAdd()));
        connect(button_delete,SIGNAL(clicked()),this,SLOT(onDel()));
        query_string="select * from `docs_ind` where 1";
    }
    else
    {
        query_string="select * from `docs_ind` where `di_activ`=1 and `di_professor`="+QString::number(s_data->professor_id);
    }

    model=new QStandardItemModel;
    model_prof=new QSqlTableModel;

    initializeListModel(model,model_prof,query_string);
    initializeListView(table_view,model);

}

//-----------------------------------------------------------------------------
void ListDialog::initializeListModel(QStandardItemModel* model,QSqlTableModel* model_prof,QString q_str)
{
    model_prof->setTable("professor");
    model_prof->select();

    QSqlQuery query;
    bool made;
    query=MainData::Instance()->makeQuery(q_str,made,true);
    if(made)
    {
        QModelIndex idx;
        QString tmp;
        int int_tmp;
        bool flag;
        int n=query.size();
        int num_cols=6;
        model->setColumnCount(num_cols);
        model->setRowCount(n);
        bool ok=query.first();
        for(int i=0;i<n&&ok;++i)
        {


            idx=model->index(i,1);
            model->setData(idx,query.value(4));

            int_tmp=query.value(2).toInt();
            if(int_tmp==1)
                tmp="Активно";
            else
                tmp="Изменен";
            idx=model->index(i,2);
            model->setData(idx,QVariant(tmp));

            idx=model->index(i,3);
            model->setData(idx,query.value(0));

            idx=model->index(i,4);
            model->setData(idx,query.value(1));

            idx=model->index(i,5);
            model->setData(idx,QVariant(int_tmp));

            int id_tmp;
            int id=query.value(3).toInt();

            flag=true;
            int j;
            for(j=0;j<model_prof->rowCount()&&flag;++j)
            {

                idx=model_prof->index(j,0);
                id_tmp=idx.data().toInt();
                if(id==id_tmp)
                    flag=false;
            }
            if(flag==false)
            {
                --j;
                idx=model_prof->index(j,1);
                tmp=idx.data().toString();
                idx=model_prof->index(j,2);
                tmp=tmp+" "+idx.data().toString();
                idx=model_prof->index(j,3);
                tmp=tmp+" "+idx.data().toString();
                idx=model->index(i,0);
                model->setData(idx,QVariant(tmp));
            }
            ok=query.next();
        }
    }

    model->setHeaderData(0, Qt::Horizontal, "ФИО преподавателя");
    model->setHeaderData(1, Qt::Horizontal, "Год");
    model->setHeaderData(2, Qt::Horizontal, "Статус");
}
//-----------------------------------------------------------------
void ListDialog::initializeListView(QTableView* table_view,QStandardItemModel* model)
{
    table_view->setModel(model);
    table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_view->setSelectionMode(QAbstractItemView::SingleSelection);
    table_view->setMinimumHeight(300);
    fitWidth(table_view,0,3,50);
    table_view->setItemDelegate(new NoDelegate(table_view));
    table_view->hideColumn(3);
    table_view->hideColumn(4);
    table_view->hideColumn(5);
}
//-----------------------------------------------------------------
void ListDialog::fitWidth(QTableView* view,int x1,int x2,int delta)
{
    int width=0;
    for(int i=x1;i<=x2;++i)
    {
        table_view->resizeColumnToContents(i);
        width+=view->columnWidth(i);
    }
    view->setMinimumWidth(width+delta);
}
//------------------------------------------------------------------
current_document ListDialog::selectedDocument()
{
   QItemSelectionModel* selectionModel=table_view->selectionModel();
   QModelIndexList list=selectionModel->selectedIndexes();
   current_document doc;
   if(!list.isEmpty())
   {
       QModelIndex index;

       index=list[3];
       doc.di_ida=index.data().toInt();

       index=list[4];
       doc.di_id=index.data().toInt();

       index=list[5];
       doc.di_activ=index.data().toInt();
   }
   else
    doc.di_ida=-1;
   return doc;
}
//------------------------------------------------------------------
bool ListDialog::selectedStatus()
{
   QItemSelectionModel* selectionModel=table_view->selectionModel();
   QModelIndexList list=selectionModel->selectedIndexes();
   QModelIndex index;



   index=list[4];
   int di_id=index.data().toInt();

   index=list[5];
   if(index.data().toInt()!=1)
        return false;
   else
   {
        int n=model->rowCount();
        int counter=0;
        for(int i=0;i<n;++i)
        {
            index=model->index(i,4);
            if(index.data().toInt()==di_id)
            {
                ++counter;
                if(counter==2)
                    return true;
            }
        }
   }
   return false;
}

//------------------------------------------------------------------
void ListDialog::onAdd()
{
    emit returnButton(ADD);
}


//-------------------------------------------------------------------

void ListDialog::onExit()
{
    close();
}

//-------------------------------------------------------------------

void ListDialog::onCreate()
{
    emit returnButton(CREATE);
}

//---------------------------------------------------------------------
void ListDialog::onDel()
{
    emit returnButton(DELETE);
}
//---------------------------------------------------------------------
void ListDialog::onOpen()
{
    emit returnButton(OPEN);
}
//----------------------------------------------------------------------
void ListDialog::onRenew()
{
    model->clear();
    initializeListModel(model,model_prof,query_string);
    initializeListView(table_view, model);
}
//-----------------------------------------------------------------------
void ListDialog::onClicked(const QModelIndex& idx)
{
    onOpen();
}
//-----------------------------------------------------------------------
void ListDialog::closeEvent ( QCloseEvent * event )
{
   int ret=MessageHandler::showMessage(MessageHandler::ExitApp,"Вы желаете выйти?",this);
    if(ret==QMessageBox::Ok)
        event->accept();
    else
        event->ignore();
}
//----------------------------------------------------------------------
void ListDialog::keyPressEvent (QKeyEvent * event)
{
    if(event->key()==Qt::Key_Escape)
        close();
    else
        QDialog::keyPressEvent(event);
}
//-----------------------------------------------------------------------
void ListDialog::onExport()

{ emit returnButton(EXPORT); }
