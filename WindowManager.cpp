#include "WindowManager.h"

WindowManager* WindowManager::man_ptr=0;


//-------------------------------------------------------
WindowManager::WindowManager():QWidget()
{
    login_ptr=new LoginDialog();
    login_ptr->show();

    settings_ptr=0;
    connect(login_ptr,SIGNAL(returnButton(int)),this,SLOT(onLogin(int)));
 }
//-------------------------------------------------------
WindowManager* WindowManager::Instance()
{
    if(man_ptr==0)
      man_ptr=new WindowManager();
    return man_ptr;
}

//-------------------------------------------------------
void WindowManager::onLogin(int ret)
{
  switch (ret)
  {
    case LoginDialog::OK:


        bool ok=MainData::Instance()->createDBConnection();
        bool usr=MainData::Instance()->checkUser();
        if (ok&&usr)
        {
            list_ptr=new ListDialog();
            list_ptr->show();
            login_ptr->close();
            connect(list_ptr,SIGNAL(returnButton(int)),this,SLOT(onList(int)));
        }
        if(!usr)
            MainData::Instance()->closeDBConnection();
        break;
    case LoginDialog::CANCEL:
        MainData::Instance()->saveData();
        break;
    case LoginDialog::SETTINGS:
        settings_ptr=new SettingsDialog(login_ptr);
        settings_ptr->show();
        break;
    default:
        MessageHandler::showMessage(MessageHandler::Error,"LoginDialog: Неизвестный тип возвращаемого значения!",this);
    }
}
//----------------------------------------------------------------------------
void WindowManager::onList(int ret)
{
    switch(ret)
    {
            case ListDialog::OPEN:
                cur_doc=list_ptr->selectedDocument();
                if(cur_doc.di_ida!=-1)
                {
                    bool accept=list_ptr->selectedStatus();
                    plan_ptr=new PlanDialog(list_ptr,false,cur_doc.di_ida,accept);
                    connect(plan_ptr,SIGNAL(returnButton(int)),this,SLOT(onPlanDialog(int)));
                    plan_ptr->show();
                }
                break;
            case ListDialog::ADD:
                add_ptr=new AddDialog(list_ptr);
                connect(add_ptr,SIGNAL(returnButton(int)),this,SLOT(onAddDialog(int)));
                add_ptr->show();
                break;
            case ListDialog::CREATE:
                plan_ptr=new PlanDialog(list_ptr,true);
                plan_ptr->show();
                break;
            case ListDialog::DELETE:
                cur_doc=list_ptr->selectedDocument();
                if(cur_doc.di_activ==1)
                {
                    QString str="select * from `docs_ind` where `di_id`=";
                    str+=QString::number(cur_doc.di_id);
                    QSqlQuery query;
                    bool ok=query.exec(str);
                    if(!ok)
                        MessageHandler::showMessage(MessageHandler::Error,"onList::Ошибка при работе с БД\n"+str,this);
                    else
                    {
                        if(query.size()==2)
                        {
                            onPlanDialog(PlanDialog::ACCEPT);
                            list_ptr->onRenew();
                        }
                        if(query.size()==1)
                        {
                            deleteDocument(cur_doc.di_ida);
                            list_ptr->onRenew();
                        }
                    }

                }

                break;
            case ListDialog::RENEW:
                list_ptr->onRenew();
                break;
            case ListDialog::EXPORT:
                break;
            case ListDialog::EXIT:
                break;
            default:
                MessageHandler::showMessage(MessageHandler::Error,"ListDialog: Неизвестный тип возвращаемого значения!",this);
     }
}
//----------------------------------------------------------------------------
void WindowManager::onPlanDialog(int ret)
{
    QString str;
    QSqlQuery query;
     switch(ret)
    {
        case PlanDialog::ACCEPT:
            str="select `di_ida` from `docs_ind` where ";
            str+="`di_id`="+QString::number(cur_doc.di_id)+" and ";
            str+="`di_activ`=2";

            bool made;
            query=MainData::Instance()->makeQuery(str,made);
            if(made)
            {
                int kill_ida=query.value(0).toInt();
                deleteDocument(kill_ida);
            }


            updateItems("docs_ind","di_ida",QString::number(cur_doc.di_id),"di_ida",QString::number(cur_doc.di_ida));
            updateItems("docs_events","de_docs_id",QString::number(cur_doc.di_id),"de_docs_id",QString::number(cur_doc.di_ida));
            updateItems("docs_classes","dc_docs_id",QString::number(cur_doc.di_id),"dc_docs_id",QString::number(cur_doc.di_ida));
            updateItems("docs_train","dt_docs_id",QString::number(cur_doc.di_id),"dt_docs_id",QString::number(cur_doc.di_ida));
            updateItems("docs_science","ds_docs_id",QString::number(cur_doc.di_id),"ds_docs_id",QString::number(cur_doc.di_ida));
            updateItems("docs_dis","dd_docs_id",QString::number(cur_doc.di_id),"dd_docs_id",QString::number(cur_doc.di_ida));


            break;

        case PlanDialog::REJECT:
            deleteDocument(cur_doc.di_ida);
            updateItems("docs_ind","di_activ","1","di_ida",QString::number(cur_doc.di_id));
            plan_ptr->close();


            break;
        default:
            MessageHandler::showMessage(MessageHandler::Error,"PlanDailog: Неизвестный тип возвращаемого значения!",this);

    }


}
//----------------------------------------------------------------------------
void WindowManager::updateItems(QString table,QString field,QString value,QString where_f,QString value_f)
{
    QString str="update `"+table+"` set `"+field+"`="+value;
    str=str+" where `"+where_f+"`="+value_f;
    QSqlQuery query;
    bool ok=query.exec(str);
    if(!ok)
        MessageHandler::showMessage(MessageHandler::Error,"updateItems::Ошибка при работе с БД\n"+str,this);
}

void WindowManager::deleteDocument(int kill_ida)
{
         deleteItems("docs_ind","di_ida",kill_ida);
         deleteItems("docs_events","de_docs_id",kill_ida);
         deleteItems("docs_classes","dc_docs_id",kill_ida);
         deleteItems("docs_train","dt_docs_id",kill_ida);
         deleteItems("docs_science","ds_docs_id",kill_ida);
         deleteItems("docs_dis","dd_docs_id",kill_ida);
}
//----------------------------------------------------------------------------
void WindowManager::deleteItems(QString table,QString field, int ida)
{
    QString str="delete from `"+table+"` where `"+field+"`="+QString::number(ida);
    QSqlQuery query;
    bool ok=query.exec(str);
    if(!ok)
        MessageHandler::showMessage(MessageHandler::Error,"deleteItem::Ошибка при работе с БД\n"+str,this);
}
//-----------------------------------------------------------------------------

void WindowManager::onAddDialog(int ret)
{
    switch(ret)
    {
        case AddDialog::ADD_LECTURRER:
            lecturer_ptr=new AddLecturer(add_ptr);
            lecturer_ptr->show();
            break;
        case AddDialog::EXIT:
            break;
        default:
            MessageHandler::showMessage(MessageHandler::Error,"AddDialog: Неизвестный тип возвращаемого значения!",this);

    }


}







