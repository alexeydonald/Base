#include "PlanDialog.h"

PlanDialog::PlanDialog(QWidget* parent,bool cr,int doc_id,bool accept):QDialog(parent,Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint)
{
    this->parent=parent;
    document_id=doc_id;
    di_id=document_id;
    create=cr;
    create2=cr;
    create3=cr;
    create4=cr;
    page_counter=0;
    name="Индивидуальный план работы преподавателя на учебный год";
    setWindowTitle(name);

    button_savePage=new QPushButton("Сохранить",this);
    button_revertPage=new QPushButton("Отменить",this);

    button_saveAll=new QPushButton("Сохранить",this);
    button_revertAll=new QPushButton("Отменить",this);
    button_export=new QPushButton("Экспортировать",this);

    button_exit=new QPushButton("Закрыть",this);

    button_prev=new QPushButton("Предыдущая страница",this);
    button_next=new QPushButton("Следующая стрница",this);

    if(accept)
    {
        button_accept=new QPushButton("Принять",this);
        button_reject=new QPushButton("Отклонить",this);
    }

    layout_button=new QHBoxLayout;
    layout_button->addStretch(1);
    layout_button->addWidget(button_prev);
    layout_button->addStretch(1);
    layout_button->addWidget(button_next);
    layout_button->addStretch(1);


    layout_page=new QVBoxLayout;
    layout_page->addWidget(button_savePage);
    layout_page->addWidget(button_revertPage);

    layout_all=new QVBoxLayout;
    layout_all->addWidget(button_saveAll);
    layout_all->addWidget(button_revertAll);
    layout_all->addWidget(button_export);

    group_page=new QGroupBox("Страница",this);
    group_page->setLayout(layout_page);

    group_all=new QGroupBox("Документ",this);
    group_all->setLayout(layout_all);

    layout_group=new QVBoxLayout;
    layout_group->addWidget(group_page);
    layout_group->addWidget(group_all);
    layout_group->addStretch(1);
    if(accept)
    {
        layout_group->addWidget(button_accept);
        layout_group->addWidget(button_reject);
        layout_group->addStretch(1);
    }
    layout_group->addWidget(button_exit);
    layout_group->addStretch(1);

    widget_data=new QStackedWidget(this);
    widget_data->addWidget(make1());
    widget_data->addWidget(make2());
    widget_data->addWidget(make3());
    widget_data->addWidget(make4());

    layout_top=new QHBoxLayout;
    layout_top->addWidget(widget_data);
    layout_top->addLayout(layout_group);


    layout_main=new QVBoxLayout;
    layout_main->addLayout(layout_top);
    layout_main->addLayout(layout_button);

    setLayout(layout_main);

    connect(button_saveAll,SIGNAL(clicked()),this,SLOT(onSaveAll()));
    connect(button_revertAll,SIGNAL(clicked()),this,SLOT(onRevertAll()));
    connect(button_savePage,SIGNAL(clicked()),this,SLOT(onSavePage()));
    connect(button_revertPage,SIGNAL(clicked()),this,SLOT(onRevertPage()));
    connect(button_next,SIGNAL(clicked()),this,SLOT(onNext()));
    connect(button_prev,SIGNAL(clicked()),this,SLOT(onPrev()));
    connect(button_exit,SIGNAL(clicked()),this,SLOT(onExit()));

    if(accept)
    {
        connect(button_accept,SIGNAL(clicked()),this,SLOT(onAccept()));
        connect(button_reject,SIGNAL(clicked()),this,SLOT(onReject()));
    }

    settings_data* st=MainData::Instance()->getSettingsData();
    admin_flag=st->isAdmin;
    if(!admin_flag)
    {
        create=true;
        create2=true;
        create3=true;
        create4=true;

        QString str="select count( `di_ida` ) as `ctr` from `docs_ind` ";
        bool made=false;
        QSqlQuery query=MainData::Instance()->makeQuery(str,made);
        if(made)
        {
            document_id=query.value(0).toInt()+1;
        }


    }
}
//----------------------------------------------------------------------
void PlanDialog::onAccept()
{
    emit returnButton(ACCEPT);
}
//----------------------------------------------------------------------
void PlanDialog::onReject()
{
    emit returnButton(REJECT);
}
//----------------------------------------------------------------------
void PlanDialog::onNext()
{
    int n=widget_data->count();
    ++page_counter;
    if(page_counter>n-1)
        page_counter=0;
    widget_data->setCurrentIndex(page_counter);
}
//-----------------------------------------------------------------------
void PlanDialog::onPrev()
{
    --page_counter;
    if(page_counter<0)
        page_counter=widget_data->count()-1;
    widget_data->setCurrentIndex(page_counter);
}

//----------------------------------------------------------------------
void PlanDialog::onSaveAll()
{
    savePage1();
    savePage2();
    savePage3();
    savePage4();
    plan_changed=false;
}
//----------------------------------------------------------------------
void PlanDialog::onRevertAll()
{
    revertPage1();
    revertPage2();
    revertPage3();
    revertPage4();
    plan_changed=false;
}
//-----------------------------------------------------------------------
void PlanDialog::onExit()
{
    close();
}
//-----------------------------------------------------------------------
void PlanDialog::onSavePage()
{
    if(admin_flag)
    {
    int idx=widget_data->currentIndex();
    switch(idx)
    {
        case 0:
            if(create)
            {
                saveEmpty2();
                saveEmpty3();
                saveEmpty4();
            }
            savePage1();
            break;
        case 1:
            if(create2)
            {
                saveEmpty1();
                saveEmpty3();
                saveEmpty4();
            }
                savePage2();
            break;
        case 2:
            if(create3)
            {
                saveEmpty1();
                saveEmpty2();
                saveEmpty4();
            }
            savePage3();
            break;
        case 3:
            if(create4)
            {
                saveEmpty1();
                saveEmpty2();
                saveEmpty3();
            }
            savePage4();
            break;
        default:
            MessageHandler::showMessage(MessageHandler::Error,"Неверный индекс",this);

    }
    }
    else
        onSaveAll();
    plan_changed=false;
}
//-----------------------------------------------------------------------
void PlanDialog::onRevertPage()
{
    int idx=widget_data->currentIndex();
    switch(idx)
    {
        case 0:
            revertPage1();
            break;
        case 1:
            revertPage2();
            break;
        case 2:
            revertPage3();
            break;
        case 3:
            revertPage4();
            break;
        default:
            MessageHandler::showMessage(MessageHandler::Error,"Неверный индекс",this);

    }
    plan_changed=false;
}
//----------------------------------------------------------------------------
void PlanDialog::closeEvent ( QCloseEvent * event )
{
  ListDialog* list_dlg;
  if(plan_changed)
  {
        int ret=MessageHandler::showMessage(MessageHandler::CloseWindow,"Данные были изменены.<br>Сохранить?",this);
        switch (ret)
        {
            case QMessageBox::Save :
                onSaveAll();
                list_dlg=static_cast<ListDialog*>(parent);
                list_dlg->onRenew();
                QDialog::closeEvent(event);
                break;
            case QMessageBox::Discard :
                list_dlg=static_cast<ListDialog*>(parent);
                list_dlg->onRenew();
                QDialog::closeEvent(event);
                break;
            case QMessageBox::Cancel:
                event->ignore();
                break;
            default:
                MessageHandler::showMessage(MessageHandler::Error,"PlanDialog: Неизвестный тип возвращаемого значения!",this);
                event->ignore();
         }
    }
    else
    {
        list_dlg=static_cast<ListDialog*>(parent);
        list_dlg->onRenew();
        event->accept();
    }
}
//---------------------
void PlanDialog::keyPressEvent ( QKeyEvent * event )
{
    if(event->key()==Qt::Key_Escape)
        close();
    else
        QDialog::keyPressEvent(event);
}



//######################################Станица 1########################
//-----------------------------------------------------------------------

QWidget* PlanDialog::make1()
{


    label_header1=new QLabel("Индивидуальный план работы преподавателя на",this);
    label_header1->setAlignment(Qt::AlignHCenter);
    label_header2=new QLabel("учебный год",this);
    label_faculty=new QLabel("Факультет: ",this);
    label_faculty_data=new QLabel(this);
    label_fio=new QLabel("Имя Отчество Фамилия: ",this);
    label_table=new QLabel("I. Учебная работа.");
    label_table->setAlignment(Qt::AlignHCenter);
    label_itog=new QLabel("Итого: ",this);
    label_itog->setAlignment(Qt::AlignHCenter);


    edit_year=new QLineEdit(this);
    edit_year->setInputMask("D999/D999");
    combo_fio=new ComboSurname(this);

    list_itog_header<<"Количество часов\nпо плану\nза I семестр";
    list_itog_header<<"Количество часов\nпо плану\nза II семестр";
    list_itog_header<<"Количество часов\nпо плану\nвсего";
    list_itog_header<<"Количество часов\nвыполнено\nза I семестр";
    list_itog_header<<"Количество часов\nвыполнено\nза II семестр";
    list_itog_header<<"Количество часов\nвыполнено\nвсего";

    table_itog=new QTableWidget(1,6,this);
    for (int i=0;i<6;++i)
        table_itog->setItem(0,i,new QTableWidgetItem(""));
    table_itog->setHorizontalHeaderLabels(list_itog_header);

    fitTableWidth(table_itog);

    int height=table_itog->rowHeight(0);
    height+=table_itog->horizontalHeader()->height()+35;

    table_itog->setFixedHeight(height);
    table_itog->verticalHeader()->hide();
    table_itog->setEnabled(false);

    model_study=new QStandardItemModel;
    initializeStudyModel(model_study);
    if(create) addDocsClassesRow(model_study);

    model_init_data=new QStandardItemModel;
    initializeInitDataModel(model_init_data);
    if(create) addDocsIndRow(model_init_data);



    mapper_init_data=mapInitDataModel(model_init_data);
    view_study=createStudyView(model_study);

    layout_year=new QHBoxLayout;
    layout_year->addStretch(1);
    layout_year->addWidget(edit_year);
    layout_year->addWidget(label_header2);
    layout_year->addStretch(1);

    layout_faculty=new QHBoxLayout;
    layout_faculty->addStretch(1);
    layout_faculty->addWidget(label_faculty);
    layout_faculty->addWidget(label_faculty_data);
    layout_faculty->addStretch(1);

    layout_fio=new QHBoxLayout;
    layout_fio->addStretch(1);
    layout_fio->addWidget(label_fio);
    layout_fio->addWidget(combo_fio);
    layout_fio->addStretch(1);

    layout_page1=new QVBoxLayout;

    layout_page1->addWidget(label_header1);
    layout_page1->addLayout(layout_year);
    layout_page1->addLayout(layout_faculty);
    layout_page1->addLayout(layout_fio);
    layout_page1->addWidget(label_table);
    layout_page1->addWidget(view_study);
    layout_page1->addWidget(label_itog);
    layout_page1->addWidget(table_itog);


    connect(edit_year,SIGNAL(textChanged(QString)),this,SLOT(onYearChanged(QString)));
    connect(model_study,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(onModelChanged(QStandardItem*)));
    connect(combo_fio,SIGNAL(currentIndexChanged(int)),this,SLOT(onCombo(int)));


    plan_changed=false;

    settings_data* st=MainData::Instance()->getSettingsData();
 
    if(!st->isAdmin)
    {
        combo_fio->setEnabled(false);
        edit_year->setEnabled(false);
        view_study->setItemDelegate(new NoDelegate(view_study));
    }
    widget_page1=new QWidget(this);
    widget_page1->setLayout(layout_page1);
    return widget_page1;
}
//-----------------------------------------------------------------------
void PlanDialog::onMapperChanged(int )
{
    QModelIndex idx=model_init_data->index(0,3);
    //MessageHandler::showMessage(MessageHandler::Error,"!!!",this);
    int ind=idx.data().toInt()-1;
    combo_fio->setCurrentIndex(ind);
}
//-----------------------------------------------------------------------
void PlanDialog::onYearChanged(QString str)
{ plan_changed=true;}
//-----------------------------------------------------------------------
void PlanDialog::fitTableWidth(QTableWidget* table)
{
    int count=table->columnCount();
    int width=0;
    for(int i=0;i<count;++i)
         width+=table->columnWidth(i);
    table->setFixedWidth(width+5);
}
//----------------------------------------------------------------------
void PlanDialog::onModelChanged(QStandardItem* item)
{
    plan_changed=true;
    if(item->column()==4||item->column()==5||
       item->column()==7||item->column()==8)
    countItog(item->column(),item->row());
}
//-----------------------------------------------------------------------
void PlanDialog::countItog(int col,int row)
{
    if(col==4||col==5)
    {
        int itog=0;
        QModelIndex idx=model_study->index(row,4);
        itog+=idx.data().toInt();

        idx=model_study->index(row,5);
        itog+=idx.data().toInt();
        idx=model_study->index(row,6);
        int delta_all=idx.data().toInt();
        model_study->setData(idx,QVariant(itog));

        QTableWidgetItem* itm=table_itog->item(0,2);
        QVariant val=itm->data(Qt::DisplayRole);
        int itog_col=itm->data(Qt::DisplayRole).toInt();
        itog_col=itog_col-delta_all+itog;
        itm->setData(Qt::DisplayRole,QVariant(itog_col));
    }
    if(col==7||col==8)
    {
        int itog=0;
        QModelIndex idx=model_study->index(row,7);
        itog+=idx.data().toInt();
        idx=model_study->index(row,8);
        itog+=idx.data().toInt();
        idx=model_study->index(row,9);
        int delta_all=idx.data().toInt();
        model_study->setData(idx,QVariant(itog));

        QTableWidgetItem* itm=table_itog->item(0,5);
        int itog_col=itm->data(Qt::DisplayRole).toInt();
        itog_col=itog_col-delta_all+itog;
        itm->setData(Qt::DisplayRole,QVariant(itog_col));
    }

    countColItog(col);

}
//-----------------------------------------------------------------------
void PlanDialog::countColItog(int col)
{
    int n=model_study->rowCount();
    QModelIndex idx;
    int itog=0;
    for(int i=0;i<n;++i)
    {
        idx=model_study->index(i,col);
        itog+=idx.data().toInt();
    }
    QTableWidgetItem* itm=table_itog->item(0,col-4);
    itm->setData(Qt::DisplayRole,QVariant(itog));
}
//-----------------------------------------------------------------------
void PlanDialog::onCombo(int )
{
    plan_changed=true;
    mapper_init_data->submit();
    int row=model_init_data->rowCount()-1;
    QModelIndex idx=model_init_data->index(row,3,QModelIndex());


    QString str=idx.data().toString();
    QSqlQuery query;
    QString q_str="select `depts_abr`";
            q_str+=" from `professor`,`depts`";
            q_str+=" where `professor_id`="+str;
            q_str+=" and `depts_id`=`professor_dept`";
   bool made=false;
   query=MainData::Instance()->makeQuery(q_str,made);
   if(made)
   {
       str=query.value(0).toString();
       label_faculty_data->setText(str);
   }

}
//-----------------------------------------------------------------------
void PlanDialog::addDocsIndRow(QStandardItemModel* model_i)
{
    //Добавляем запись в таблицу docs_ind
    QSqlQuery query;
    QString str="select count( `di_ida` ) as `ctr` from `docs_ind` ";
    bool made=false;
    query=MainData::Instance()->makeQuery(str,made);
    if(made)
         document_id=query.value(0).toInt()+1;
    di_id=document_id;

    model_i->insertRow(model_i->rowCount());
    QModelIndex idx=model_i->index(model_i->rowCount()-1,5);

    QDateTime tm=QDateTime::currentDateTime();
    QString str_time=tm.toString("yyyy-MM-dd HH::mm::ss");

    model_i->setData(idx,QVariant(str_time));

    idx=model_i->index(model_i->rowCount()-1,0);
    model_i->setData(idx,QVariant(document_id));

    idx=model_i->index(model_i->rowCount()-1,1);
    model_i->setData(idx,QVariant(di_id));

    idx=model_i->index(model_i->rowCount()-1,2);
    model_i->setData(idx,1);
}
//-----------------------------------------------------------------------------
void PlanDialog::addDocsClassesRow(QStandardItemModel* model_st)
{
    //Добавляем запись в таблицу docs_classes
    QModelIndex idx;

    QString str="select `ct_id`,`ct_name` from `class_type` where 1";
    bool made=false;
    QSqlQuery query=MainData::Instance()->makeQuery(str,made);
    if(made)
    {
        int n=query.size();
        model_st->insertRows(model_st->rowCount(),n);
        int tmp_column=model_st->columnCount()-1;
        for(int i=0;i<n;++i)
        {
            idx=model_st->index(i,2);
            model_st->setData(idx,query.value(1));

            idx=model_st->index(i,1);
            model_st->setData(idx,QVariant(document_id));

            idx=model_st->index(i,tmp_column);
            model_st->setData(idx,query.value(0));

            query.next();
        }
    }
}
//-----------------------------------------------------------------------
void PlanDialog::initializeStudyModel(QStandardItemModel* model_study)
{
    QString str="select * from `docs_classes` where `dc_docs_id`="+QString::number(document_id);
    bool made=false;
    QSqlQuery query=MainData::Instance()->makeQuery(str,made,create);
    if(made)
    {
        int num_cols=0;
        while (query.value(num_cols).isValid()) ++num_cols;
        ++num_cols;
        model_study->setColumnCount(num_cols);
        QModelIndex idx;
        int n=query.size();
        query.first();
        for(int j=0;j<n;++j)
        {
            model_study->insertRow(model_study->rowCount());
            for(int i=0;i<num_cols;++i)
            {
                if(i==2)
                {
                    idx=model_study->index(model_study->rowCount()-1,num_cols-1);
                    model_study->setData(idx,query.value(i));
                }
                else
                {
                    idx=model_study->index(model_study->rowCount()-1,i);
                    model_study->setData(idx,query.value(i));
                }
             }
            query.next();
        }
        str="select `ct_name` from `class_type`";
        QSqlQuery query=MainData::Instance()->makeQuery(str,made,create);
        if(made&&(query.size()==model_study->rowCount()))
        {
            int k;
            for(k=0;k<query.size();++k)
            {
                idx=model_study->index(k,2);
                model_study->setData(idx,query.value(0));
                query.next();
            }
        }
    }
    else
        model_study->setColumnCount(11);
    model_study->setHeaderData(2, Qt::Horizontal, "Вид занятий");
    model_study->setHeaderData(3, Qt::Horizontal, "Факультет,\nпоток,\nгруппа");
    model_study->setHeaderData(4, Qt::Horizontal, "Количество часов\nпо плану\nза I семестр");
    model_study->setHeaderData(5, Qt::Horizontal, "Количество часов\nпо плану\nза II семестр");
    model_study->setHeaderData(6, Qt::Horizontal, "Количество часов\nпо плану\nвсего");
    model_study->setHeaderData(7, Qt::Horizontal, "Количество часов\nвыполнено\nза I семестр");
    model_study->setHeaderData(8, Qt::Horizontal, "Количество часов\nвыполнено\nза II семестр");
    model_study->setHeaderData(9, Qt::Horizontal, "Количество часов\nвыполнено\nвсего");
}
//------------------------------------------------------------------------
QTableView* PlanDialog::createStudyView(QStandardItemModel  * model,bool make)
{
    if(make)
    {
        view_study=new QTableView(this);
        view_study->setModel(model_study);
        view_study->setItemDelegate(new StudyDelegate(view_study));
    }

    view_study->hideColumn(0);
    view_study->hideColumn(1);
    view_study->hideColumn(model_study->columnCount()-1);
    if(make) fitWidth(view_study,2,9,45);
    view_study->setMinimumHeight(350);

    for(int i=4;i<=9;++i)
        countColItog(i);


    return view_study;
}
//------------------------------------------------------------------------
void PlanDialog::initializeInitDataModel(QStandardItemModel* model_init_data)
{
    QSqlQuery query;
    QModelIndex idx;
    QString q_str="select * from `docs_ind` where `di_ida`="+QString::number(document_id);
    bool made=false;
    query=MainData::Instance()->makeQuery(q_str,made,create);
    if(made)
    {
        model_init_data->setRowCount(1);
        int i=0;
        while (query.value(i).isValid())
        {
            model_init_data->insertColumn(i);
            idx=model_init_data->index(0,i);
            ++i;
        }
    }
    else
        model_init_data->setColumnCount(6);
}
//------------------------------------------------------------------------
QDataWidgetMapper* PlanDialog::mapInitDataModel(QStandardItemModel* model_init_data,bool make)
{
    if(make)
    {
        mapper_init_data=new QDataWidgetMapper(this);
        prof_table = new QSqlTableModel(this);
        prof_table->setTable("professor");
        prof_table->select();
        combo_fio->setModel(prof_table);
        connect(mapper_init_data,SIGNAL(currentIndexChanged(int )),this,SLOT(onMapperChanged(int)));
    }
    mapper_init_data->setModel(model_init_data);
    mapper_init_data->addMapping(edit_year, 4);
    mapper_init_data->addMapping(combo_fio, 3,"curIdx");
    mapper_init_data->toLast();
    if(!create) onCombo(0);
    return mapper_init_data;

}
//------------------------------------------------------------------------
void PlanDialog::fitWidth(QTableView* view,int x1,int x2,int delta)
{
    int width=0;
    for(int i=x1;i<=x2;++i)
         width+=view->columnWidth(i);
    view->setMinimumWidth(width+delta);
}
//------------------------------------------------------------------------
void PlanDialog::saveStudy(QStandardItemModel* model_st)
{
    int n=model_st->rowCount();
    QSqlQuery query;
    bool ok=true;
    QString q_str;
    q_str="insert into `docs_classes`(`dc_id`,`dc_docs_id`,`dc_class_type`,`dc_fac`,`dc_hour_plan_1`,`dc_hour_plan_2`,`dc_hour_plan`,`dc_hour_perf_1`,`dc_hour_perf_2`,`dc_hour_perf`) values (''";
    QModelIndex  idx;
    for(int i=0;i<n&&ok;++i)
    {
        QString str=q_str;
        str+=",'";
        str=str+QString::number(document_id);
        str+="','";
        idx=model_st->index(i,model_study->columnCount()-1);
        str+=QString::number(idx.data().toInt());
        str+="'";
        for(int j=3;j<=9;++j)
        {
            idx=model_st->index(i,j);
            str+=",";
            if(j==3)
            {
                str+="'";
                str+=idx.data().toString();
                str+="'";
            }
            else
            {
                str+="'";
                str+=QString::number(idx.data().toInt());
                str+="'";
            }
        }
        str+=")";
        query.clear();
        ok=query.exec(str);
        if(!ok)
            MessageHandler::showMessage(MessageHandler::Error,"SaveStudy::Ошибка при записи данных в БД",this);
    }
}
//-----------------------------------------------------------------------------
void PlanDialog::saveInitData(QStandardItemModel* model_i)
{
    QSqlQuery query;
    QString q_str="insert into `docs_ind`(`di_ida`,`di_id`,`di_activ`,`di_professor`,`di_year`,`di_create_time`) values (";
    int n=model_i->columnCount();
    QModelIndex idx;
    q_str+="'";
    q_str+=QString::number(document_id);
    q_str+="'";
    for(int i=1;i<n;++i)
    {
        idx=model_i->index(0,i);
        q_str+=",'";

        if(i>3)
          q_str+=idx.data().toString();
        else
          q_str+=QString::number(idx.data().toInt());
        q_str+="'";
    }
    q_str+=")";
    bool ok=query.exec(q_str);
    if(!ok)
        MessageHandler::showMessage(MessageHandler::Error,"saveInitData::Ошибка при записи данных в БД",this);

    if(!admin_flag)
    {
        q_str="update `docs_ind` set `di_activ`=2 where `di_ida`="+QString::number(di_id);
        ok=query.exec(q_str);
        if(!ok)
            MessageHandler::showMessage(MessageHandler::Error,"saveInitData(!admin_flag)::Ошибка при записи данных в БД",this);
    }
}
//-----------------------------------------------------------------------------
void PlanDialog::updateStudy()
{
    int n=model_study->rowCount();
    int col=model_study->columnCount();
    QModelIndex idx;
    QString str;
    QSqlQuery query;
    QStringList str_list;
    bool ok=true;
    str_list<<"`dc_class_type`="<<"`dc_fac`="<<"`dc_hour_plan_1`=";
    str_list<<"`dc_hour_plan_2`="<<"`dc_hour_plan`=";
    str_list<<"`dc_hour_perf_1`="<<"`dc_hour_perf_2`="<<"`dc_hour_perf`=";
    for(int i=0;i<n&&ok;++i)
    {
        str="update `docs_classes` set ";

        for(int j=2;j<col-1;++j)
        {
            if(j!=2)
               str+=",";
            str+=str_list.value(j-2);
            str+="'";

            idx=model_study->index(i,j);
            if(j==2)
            {
                idx=model_study->index(i,col-1);
                str+=QString::number(idx.data().toInt());
            }
            else
            {
                if(j==3)
                   str+=idx.data().toString();
                else
                   str+=QString::number(idx.data().toInt());
            }

            str+="'";
        }

        idx=model_study->index(i,0);
        str=str+" where `dc_id`="+QString::number(idx.data().toInt());
        ok=query.exec(str);
        if(!ok)
            MessageHandler::showMessage(MessageHandler::Error,"Ошибка при изменении данных в БД",this);
    }


}
//-----------------------------------------------------------------------------
void PlanDialog::updateInitData()
{
    int cols=model_init_data->columnCount();
    QModelIndex idx;
    QString str="update `docs_ind` set ";
    QStringList str_list;
    str_list<<"`di_ida`="<<"`di_id`="<<"`di_activ`=";
    str_list<<"`di_professor`="<<"`di_year`=";
    for(int i=0;i<cols-1;++i)
    {
        if(i!=0)
            str+=",";
        str+=str_list.value(i);
        str+="'";

        idx=model_init_data->index(0,i);
        if(i==2||i==4)
            str+=idx.data().toString();
        else
            str+=QString::number(idx.data().toInt());

        str+="'";
    }

    str=str+" where `di_ida`="+QString::number(document_id);

    QSqlQuery query;
    bool ok=query.exec(str);
    if(!ok)
            MessageHandler::showMessage(MessageHandler::Error,"Ошибка при изменении данных в БД",this);


}
//-----------------------------------------------------------------------------
void  PlanDialog::savePage1()
{
    if(create)
    {
        saveInitData(model_init_data);
        saveStudy(model_study);
        create=false;
        revertPage1();
    }
    else
    {
        updateStudy();
        updateInitData();
    }
}


//-----------------------------------------------------------------------------
void PlanDialog::revertPage1()
{
    model_init_data->clear();
    initializeInitDataModel(model_init_data);
    if(create) addDocsIndRow(model_init_data);

    model_study->clear();
    initializeStudyModel(model_study);
    if(create) addDocsClassesRow(model_study);
    mapInitDataModel(model_init_data,false);
    if(create) addDocsIndRow(model_init_data);
    createStudyView(model_study,false);
    for(int i=4;i<=9;++i)
        countColItog(i);
}
//-----------------------------------------------------------------------------
void PlanDialog::saveEmpty1()
{
    QStandardItemModel model_i;
    initializeInitDataModel(&model_i);
    addDocsIndRow(&model_i);
    saveInitData(&model_i);

    QStandardItemModel model_s;
    initializeStudyModel(&model_s);
    addDocsClassesRow(&model_s);
    saveStudy(&model_s);
    create=false;
}


//################################Страница 2###################################

QWidget* PlanDialog::make2()
{
    max_rows_umr=35;
    de_type_umr="1";

    label_header=new QLabel("II. Методическая работа.\n1. Учебно-методическая работа.");
    label_header->setAlignment(Qt::AlignHCenter);

    model_events=new QSqlTableModel;
    model_events->setTable("events");
    model_events->select();

    model_report_type=new QSqlTableModel;
    model_report_type->setTable("report_type");
    model_report_type->select();



    model_umr=new MetodModel(model_events,model_report_type);
    initializeMrModel(model_umr,de_type_umr,max_rows_umr,create2);

    view_umr=new QTableView(this);
    createMrView(view_umr,model_umr);

    layout_page2=new QVBoxLayout;
    layout_page2->addWidget(label_header);
    layout_page2->addWidget(view_umr);

    connect(model_umr,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(onModelChanged(QStandardItem*)));

    widget_page2=new QWidget(this);
    widget_page2->setLayout(layout_page2);
    return widget_page2;
}

//----------------------------------------------------------------------------
void PlanDialog::initializeMrModel(MetodModel* model_mr,QString de_type,int max_rows,bool silent)
{
    QString str="select * from `docs_events` where `de_docs_id`="+QString::number(document_id);
    str=str+" and `de_type`="+de_type+" order by `de_id` asc";
    bool made=false;
    QSqlQuery query=MainData::Instance()->makeQuery(str,made,silent);
    if(made)
    {
        int n=query.size();
        query.first();
        int num_cols=0;
        while(query.value(num_cols).isValid()) ++num_cols;
        model_mr->setColumnCount(num_cols+2);
        model_mr->setRowCount(n);
        QModelIndex idx;
        bool ok=true;
        for(int i=0;i<n&&ok;++i)
        {
            for(int j=0;j<num_cols;++j)
            {
                idx=model_mr->index(i,j);
                model_mr->setData(idx,query.value(j),Qt::DecorationRole);
            }
            ok=query.next();
        }
    }
    else
    {
        model_mr->setColumnCount(10);
        model_mr->setRowCount(max_rows);
    }

    model_mr->setHeaderData(2, Qt::Horizontal, "Наименование мероприятий,\nзаданий,\nработ");
    model_mr->setHeaderData(3, Qt::Horizontal, "Сроки за\nI семестр");
    model_mr->setHeaderData(4, Qt::Horizontal, "Сроки за\nII семестр");
    model_mr->setHeaderData(5, Qt::Horizontal, "Форма отчетности");
    model_mr->setHeaderData(6, Qt::Horizontal, "Отметка о выполнении\n№ и дата\nдокумента,\nфиксирующего\nвыполнение");
}
//----------------------------------------------------------------------------
void PlanDialog::createMrView(QTableView* view_mr, MetodModel* model_mr,bool make)
{
    if(make)
    {
        view_mr->setModel(model_mr);

        delegate_metod=new MetodDelegate(model_events,model_report_type,model_mr,view_mr);
        view_mr->setItemDelegate(delegate_metod);
    }
    view_mr->hideColumn(0);
    view_mr->hideColumn(1);
    view_mr->hideColumn(7);
    view_mr->hideColumn(8);
    view_mr->hideColumn(9);
    view_mr->horizontalHeader()->resizeSection(2,165);
    view_mr->horizontalHeader()->resizeSection(3,100);
    view_mr->horizontalHeader()->resizeSection(4,100);
    view_mr->horizontalHeader()->resizeSection(5,120);
    view_mr->horizontalHeader()->resizeSection(6,140);
    fitWidth(view_study,2,6,45);
}
//-----------------------------------------------------------------------------
void PlanDialog::saveMr(MetodModel* model,QString de_type)
{

    int n=model->rowCount();
    int cols=model->columnCount();

    QModelIndex idx;
    QSqlQuery query;
    bool ok=true;
    QString str;
    for(int i=0;i<n&&ok;++i)
    {
        query.clear();
        str="insert into `docs_events` (`de_id`,`de_docs_id`,`de_event_id`,`de_period_1`,`de_period_2`,`de_report_type`,`de_done`,`de_type`) values (''";
        str=str+",'"+QString::number(document_id)+"'";
        for(int j=2;j<cols-3;++j)
        {
            idx=model->index(i,j);
            str+=",'";
            if(j==2||j==5)
                str+=QString::number(model->data(idx).toInt());
            else
                str+=model->data(idx).toString();
            str+="'";
        }
        str=str+",'"+de_type+"')";
        ok=query.exec(str);
        if(!ok)
            MessageHandler::showMessage(MessageHandler::Error,"saveMr::Ошибка при записи данных в БД",this);
    }
}
//----------------------------------------------------------------------------------
void PlanDialog::updateMr(MetodModel* model)
{
    int n=model->rowCount();
    int cols=model->columnCount();

    QModelIndex idx;
    QSqlQuery query;
    bool ok=true;
    QString str;
    int j;
    QStringList list_str;
    list_str<<"`de_event_id`="<<",`de_period_1`=";
    list_str<<",`de_period_2`="<<",`de_report_type`=";
    list_str<<",`de_done`="<<"`de_id`=";
    for(int i=0;i<n&&ok;++i)
    {
        query.clear();
        str="update `docs_events` set ";
        for(j=2;j<cols-3;++j)
        {
            str+=list_str.value(j-2);
            str+="'";
            idx=model->index(i,j);
            if(j==2||j==5)
                str+=QString::number(model->data(idx).toInt());
            else
                str+=model->data(idx).toString();
            str+="'";
        }
        str=str+" where "+list_str.value(j-2);
        idx=model->index(i,0);
        str+=QString::number(model->data(idx).toInt());
        ok=query.exec(str);
        if(!ok)
             MessageHandler::showMessage(MessageHandler::Error,"updateMr::Ошибка при записи данных в БД",this);
     }
}

//---------------------------------------------------------------------------
void PlanDialog::savePage2()
{
    if(create2)
    {
        saveMr(model_umr,de_type_umr);
        create2=false;
        revertPage2();

    }
    else
        updateMr(model_umr);
}
//----------------------------------------------------------------------------
void PlanDialog::revertPage2()
{
    model_umr->clear();
    initializeMrModel(model_umr,de_type_umr,max_rows_umr,create2);
    createMrView(view_umr,model_umr,false);

}
//----------------------------------------------------------------------------
void PlanDialog::saveEmpty2()
{
    MetodModel model(0,0);
    initializeMrModel(&model,de_type_umr,max_rows_umr,create2);
    saveMr(&model,de_type_umr);
    create2=false;
}

//###############################Страница 3####################################
QWidget* PlanDialog::make3()
{
    max_rows_nmr=19;
    max_rows_omr=9;
    max_rows_gnir=5;

    de_type_nmr="2";
    de_type_omr="3";
    de_type_gnir="4";


    label_nmr=new QLabel("II. Методическая работа\n2. Научно-методическая работа.", this);
    label_nmr->setAlignment(Qt::AlignHCenter);

    label_omr=new QLabel("3. Организационно-методическая работа.",this);
    label_omr->setAlignment(Qt::AlignHCenter);

    label_gnir=new QLabel("III. Госбюджетная научно-исследовательская работа.",this);
    label_gnir->setAlignment(Qt::AlignHCenter);

    model_nmr=new MetodModel(model_events,model_report_type);
    initializeMrModel(model_nmr,de_type_nmr,max_rows_nmr,create3);

    view_nmr=new QTableView(this);
    createMrView(view_nmr,model_nmr);

    model_omr=new MetodModel(model_events,model_report_type);
    initializeMrModel(model_omr,de_type_omr,max_rows_omr,create3);

    view_omr=new QTableView(this);
    createMrView(view_omr,model_omr);

    model_gnir=new MetodModel(model_events,model_report_type);
    initializeMrModel(model_gnir,de_type_gnir,max_rows_gnir,create3);

    view_gnir=new QTableView(this);
    createMrView(view_gnir,model_gnir);

    layout_page3=new QVBoxLayout;
    layout_page3->addStretch(1);
    layout_page3->addWidget(label_nmr);
    layout_page3->addWidget(view_nmr);
    layout_page3->addStretch(1);
    layout_page3->addWidget(label_omr);
    layout_page3->addWidget(view_omr);
    layout_page3->addStretch(1);
    layout_page3->addWidget(label_gnir);
    layout_page3->addWidget(view_gnir);

    connect(model_nmr,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(onModelChanged(QStandardItem*)));
    connect(model_omr,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(onModelChanged(QStandardItem*)));
    connect(model_gnir,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(onModelChanged(QStandardItem*)));

    widget_page3=new QWidget(this);
    widget_page3->setLayout(layout_page3);
    return widget_page3;
}
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
void PlanDialog::savePage3()
{
    if(create3)
    {
        saveMr(model_nmr,de_type_nmr);
        saveMr(model_omr,de_type_omr);
        saveMr(model_gnir,de_type_gnir);
        create3=false;
        revertPage3();
    }
    else
    {
        updateMr(model_nmr);
        updateMr(model_omr);
        updateMr(model_gnir);
    }
}
//----------------------------------------------------------------------------
void PlanDialog::revertPage3()
{
    model_nmr->clear();
    model_omr->clear();
    model_gnir->clear();

    initializeMrModel(model_nmr,de_type_nmr,max_rows_nmr,create3);
    createMrView(view_nmr,model_nmr,false);

    initializeMrModel(model_omr,de_type_omr,max_rows_omr,create3);
    createMrView(view_omr,model_omr,false);

    initializeMrModel(model_gnir,de_type_gnir,max_rows_gnir,create3);
    createMrView(view_gnir,model_gnir,false);
}
//----------------------------------------------------------------------------
void PlanDialog::saveEmpty3()
{
    MetodModel model(0,0);
    initializeMrModel(&model,de_type_nmr,max_rows_nmr,create3);
    saveMr(&model,de_type_nmr);

    model.clear();
    initializeMrModel(&model,de_type_omr,max_rows_omr,create3);
    saveMr(&model,de_type_omr);

    model.clear();
    initializeMrModel(&model,de_type_gnir,max_rows_gnir,create3);
    saveMr(&model,de_type_gnir);

    create3=false;
}

//#################################Страница 4#################################

QWidget* PlanDialog::make4()
{
    max_rows_train=13;
    max_rows_dis=2;
    max_rows_science=5;

    max_cols_train=6;
    max_cols_dis=7;
    max_cols_science=7;

    label_train=new QLabel("IV. Работа по воспитанию студентов.",this);
    label_train->setAlignment(Qt::AlignHCenter);

    QString head="V. Работа, не включаемая в шестимесяный рабочий день.\n";
    head+="1. Работа над докторской(кандидатской) диссертацией.";
    label_dis=new QLabel(head,this);
    label_dis->setAlignment(Qt::AlignHCenter);

    label_science=new QLabel("2. Хоздоговорная научно-исследовательская работа.");
    label_science->setAlignment(Qt::AlignHCenter);

    model_train=new QStandardItemModel;
    initializeOtherModel(model_train,"docs_train","dt_docs_id",max_cols_train,max_rows_train,create4);
    view_train=new QTableView;
    createTrainView(view_train,model_train);

    model_dis=new QStandardItemModel;
    initializeOtherModel(model_dis,"docs_dis","dd_docs_id",max_cols_dis,max_rows_dis,create4);
    view_dis=new QTableView;
    createDisView(view_dis,model_dis);

    model_science=new QStandardItemModel;
    initializeOtherModel(model_science,"docs_science","ds_docs_id",max_cols_science,max_rows_science,create4);
    view_science=new QTableView;
    createDisView(view_science,model_science);

    resizeTrainHeader();

    QVBoxLayout* layout_page4=new QVBoxLayout;
    layout_page4->addStretch(1);
    layout_page4->addWidget(label_train);
    layout_page4->addWidget(view_train);
    layout_page4->addStretch(1);
    layout_page4->addWidget(label_dis);
    layout_page4->addWidget(view_dis);
    layout_page4->addStretch(1);
    layout_page4->addWidget(label_science);
    layout_page4->addWidget(view_science);

    connect(model_train,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(onModelChanged(QStandardItem*)));
    connect(model_science,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(onModelChanged(QStandardItem*)));
    connect(model_dis,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(onModelChanged(QStandardItem*)));

    widget_page4=new QWidget(this);
    widget_page4->setLayout(layout_page4);

    return widget_page4;
}
//----------------------------------------------------------------------------
void PlanDialog::initializeOtherModel(QStandardItemModel* model,QString table,QString srch,int max_cols,int max_rows,bool silent)
{
    QString str="select * from `"+table+"` where `"+srch+"`="+QString::number(document_id);

    bool made=false;
    QSqlQuery query=MainData::Instance()->makeQuery(str,made,silent);
    if(made)
    {
        int n=query.size();
        query.first();
        int num_cols=max_cols;

        model->setColumnCount(num_cols);
        model->setRowCount(n);
        QModelIndex idx;
        bool ok=true;
        for(int i=0;i<n&&ok;++i)
        {
            for(int j=0;j<num_cols;++j)
            {
                idx=model->index(i,j);
                model->setData(idx,query.value(j));
            }
            ok=query.next();
        }
    }
    else
    {
        model->setColumnCount(max_cols);
        model->setRowCount(max_rows);
    }
}
//---------------------------------------------------------------------------
void PlanDialog::createTrainView(QTableView* view,QStandardItemModel* model,bool made)
{
    if(made)
          view->setModel(model);
    view->hideColumn(0);
    view->hideColumn(1);

    model->setHeaderData(2, Qt::Horizontal, "");
    model->setHeaderData(3, Qt::Horizontal, "");
    model->setHeaderData(4, Qt::Horizontal, "");
    model->setHeaderData(5, Qt::Horizontal, "");
}
//----------------------------------------------------------------------------
 void PlanDialog::createDisView(QTableView* view,QStandardItemModel* model,bool made)
 {
    if(made)
    {
        view->setModel(model);
        QList<int> list;
        list<<3<<4<<5;
        view->setItemDelegate(new SpinboxDelegate(list,view));
    }
    view->hideColumn(0);
    view->hideColumn(1);

    model->setHeaderData(2, Qt::Horizontal, "Наименование темы\nразделы диссертационной работы,\nвыполняемые в отчетном году");
    model->setHeaderData(3, Qt::Horizontal, "Выполнение \nк началу учебного года,%");
    model->setHeaderData(4, Qt::Horizontal, "Выполнение \nк концу учебного года\nпо плану,%");
    model->setHeaderData(5, Qt::Horizontal, "Выполнение \nк концу учебного года\nвыполнено,%");
    model->setHeaderData(6, Qt::Horizontal, "Примечание");

    view->horizontalHeader()->resizeSection(2,220);
    view->horizontalHeader()->resizeSection(3,170);
    view->horizontalHeader()->resizeSection(4,145);
    view->horizontalHeader()->resizeSection(5,145);
    view->horizontalHeader()->resizeSection(6,80);

    fitWidth(view,2,6,40);

 }
//-----------------------------------------------------------------------------
 void PlanDialog::createScienceView(QTableView* view,QStandardItemModel* model,bool made)
 {
    if(made)
    {
        view->setModel(model);
        QList<int> list;
        list<<5;
        view->setItemDelegate(new SpinboxDelegate(list,view));
    }
    view->hideColumn(0);
    view->hideColumn(1);

    model->setHeaderData(2, Qt::Horizontal, "Наименование темы");
    model->setHeaderData(3, Qt::Horizontal, "Плановые сроки\nвыполнения всей\nтемы,\nНачало");
    model->setHeaderData(4, Qt::Horizontal, "Плановые сроки\nвыполнения всей\nтемы,\nКонец");
    model->setHeaderData(5, Qt::Horizontal, "Объем на\nтекущий год,\n(тыс.руб)");
    model->setHeaderData(6, Qt::Horizontal, "Примечание");

    view->horizontalHeader()->resizeSection(2,220);
    view->horizontalHeader()->resizeSection(3,170);
    view->horizontalHeader()->resizeSection(4,145);
    view->horizontalHeader()->resizeSection(5,145);
    view->horizontalHeader()->resizeSection(6,80);

    fitWidth(view,2,6,40);
 }
 //---------------------------------------------------------------------------
void PlanDialog::saveOther(QStandardItemModel* model,QString table,QString values)
{

    int n=model->rowCount();
    int cols=model->columnCount();

    QModelIndex idx;
    QSqlQuery query;
    bool ok=true;

    QString q_str="insert into `"+table+"` ("+values;
    q_str+=") values (''";
    q_str=q_str+",'"+QString::number(document_id)+"'";

    QString str;
    for(int i=0;i<n&&ok;++i)
    {
        query.clear();
        str=q_str;
        for(int j=2;j<cols;++j)
        {
            idx=model->index(i,j);
            str+=",'";
            str+=model->data(idx).toString();
            str+="'";
        }
        str+=")";
        ok=query.exec(str);
        if(!ok)
            MessageHandler::showMessage(MessageHandler::Error,"saveOther::Ошибка при записи данных в БД",this);
    }
}
//----------------------------------------------------------------------------
void PlanDialog::updateOther(QStandardItemModel* model,QString table,QStringList list_str)
{
    int n=model->rowCount();
    int cols=model->columnCount();

    QModelIndex idx;
    QSqlQuery query;
    bool ok=true;
    QString str;
    QString q_str="update `"+table+"` set ";
    int j;
    for(int i=0;i<n&&ok;++i)
    {
        query.clear();
        str=q_str;
        for(j=2;j<cols;++j)
        {
            str+=list_str.value(j-2);
            str+="'";
            idx=model->index(i,j);
            str+=model->data(idx).toString();
            str+="'";
        }
        str=str+" where "+list_str.value(j-2);
        idx=model->index(i,0);
        str+=QString::number(model->data(idx).toInt());
        ok=query.exec(str);
        if(!ok)
             MessageHandler::showMessage(MessageHandler::Error,"updateOther::Ошибка при записи данных в БД",this);
     }
}


//----------------------------------------------------------------------------
void PlanDialog::savePage4()
{
    if(create4)
    {
        QString values="`dt_id`,`dt_docs_id`,`dt_field_1`,`dt_field_2`,`dt_field_3`,`dt_field_4`";
        saveOther(model_train,"docs_train",values);

        values="`ds_id`,`ds_docs_id`,`ds_topic`,`ds_date_start`,`ds_date_end`,`ds_volume`,`ds_comments`";
        saveOther(model_science,"docs_science",values);

        values="`dd_id`,`dd_docs_id`,`dd_topic`,`dd_perf_start`,`dd_perf_end_plan`,`dd_perf_end`,`dd_comments`";
        saveOther(model_dis,"docs_dis",values);

        create4=false;
        revertPage4();
    }
    else
    {
        QStringList list;

        list<<"`dt_field_1`="<<",`dt_field_2`="<<",`dt_field_3`="<<",`dt_field_4`=";
        list<<"`dt_id`=";
        updateOther(model_train,"docs_train",list);

        list.clear();
        list<<"`ds_topic`="<<",`ds_date_start`="<<",`ds_date_end`=";
        list<<",`ds_volume`="<<",`ds_comments`="<<"`ds_id`=";
        updateOther(model_science,"docs_science",list);

        list.clear();
        list<<"`dd_topic`="<<",`dd_perf_start`="<<",`dd_perf_end_plan`=";
        list<<",`dd_perf_end`="<<",`dd_comments`="<<"`dd_id`=";
        updateOther(model_dis,"docs_dis",list);
    }
}
//---------------------------------------------------------------------------
void PlanDialog::saveEmpty4()
{
    QStandardItemModel model;

    initializeOtherModel(&model,"docs_train","dt_docs_id",max_cols_train,max_rows_train,create4);
    QString values="`dt_id`,`dt_docs_id`,`dt_field_1`,`dt_field_2`,`dt_field_3`,`dt_field_4`";
    saveOther(&model,"docs_train",values);

    model.clear();
    initializeOtherModel(&model,"docs_science","ds_docs_id",max_cols_science,max_rows_science,create4);

    values="`ds_id`,`ds_docs_id`,`ds_topic`,`ds_date_start`,`ds_date_end`,`ds_volume`,`ds_comments`";
    saveOther(&model,"docs_science",values);

    model.clear();
    initializeOtherModel(&model,"docs_dis","dd_docs_id",max_cols_dis,max_rows_dis,create4);
    values="`dd_id`,`dd_docs_id`,`dd_topic`,`dd_perf_start`,`dd_perf_end_plan`,`dd_perf_end`,`dd_comments`";
    saveOther(&model,"docs_dis",values);

    create4=false;
}
//---------------------------------------------------------------------------
void PlanDialog::revertPage4()
{
    model_train->clear();
    model_science->clear();
    model_dis->clear();

    initializeOtherModel(model_train,"docs_train","dt_docs_id",max_cols_train,max_rows_train,create4);
    createTrainView(view_train,model_train);


    initializeOtherModel(model_dis,"docs_dis","dd_docs_id",max_cols_dis,max_rows_dis,create4);
    createDisView(view_dis,model_dis);

    initializeOtherModel(model_science,"docs_science","ds_docs_id",max_cols_science,max_rows_science,create4);
    createDisView(view_science,model_science);

    resizeTrainHeader();
}

//---------------------------------------------------------------------------
 void PlanDialog::resizeTrainHeader()
 {
    int total=0;
    for(int i=2;i<=6;++i)
         total+=view_dis->horizontalHeader()->sectionSize(i);
    int size=total/4-5;
    for(int j=2;j<=5;++j)
        view_train->horizontalHeader()->resizeSection(j,size);
 }


