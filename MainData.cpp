#include "MainData.h"

MainData* MainData::man_ptr=0;

//--------------------------------------------------------
MainData::MainData()
{
    list_id=0;

    settings.dbname="";
    settings.hostname="";
    settings.password="";
    settings.port="";
    settings.username="";
    settings.isAdmin=false;
    save_path=".";
    loadData();

}
//--------------------------------------------------------

MainData* MainData::Instance()
{
    if(man_ptr==0)
        man_ptr=new MainData();
    return man_ptr;
}

//--------------------------------------------------------
void MainData::saveData()
{
    QSettings::setPath(QSettings::IniFormat,QSettings::UserScope,save_path);
    QSettings saver(QSettings::IniFormat,QSettings::UserScope,"MyOrg","MyApp");
    saver.setValue("/connection/hostname",settings.hostname);
    saver.setValue("/connection/port",settings.port);
    saver.setValue("/connection/dbname",settings.dbname);
}
//----------------------------------------------------------
void MainData::loadData()
{
    QSettings::setPath(QSettings::IniFormat,QSettings::UserScope,save_path);
    QSettings saver(QSettings::IniFormat,QSettings::UserScope,"MyOrg","MyApp");
    settings.hostname=saver.value("/connection/hostname").toString();
    settings.port=saver.value("/connection/port").toString();
    settings.dbname=saver.value("/connection/dbname").toString();
}
//----------------------------------------------------------
bool MainData::createDBConnection()
{
   settings_data* sd=MainData::Instance()->getSettingsData();
   db=QSqlDatabase::addDatabase("QMYSQL");
   db.setHostName(sd->hostname);
   db.setDatabaseName(sd->dbname);
   db.setUserName("root");
   db.setPassword("");
   db.setPort(sd->port.toInt());
   if(!db.open())
   {
       QSqlError err=db.lastError();
       MessageHandler::showMessage(MessageHandler::Error,err.databaseText(),0);
       return false;
   }
   return true;
}
//-------------------------------------------------------------
bool MainData::checkUser()
{
    settings_data* sd=MainData::Instance()->getSettingsData();
    QSqlQuery query;
    QString q_str="select `professor_admin`,`professor_id` from `professor` where `professor_login`='"+sd->username;
    q_str+="' and `professor_pass`='"+sd->password+"'";
    bool made=false;
    query=makeQuery(q_str,made);
    if(made)
    {
        QVariant adm = query.value(0);
        sd->isAdmin=adm.toBool();
        sd->professor_id=query.value(1).toInt();
        return true;
    }
    else
        return false;
}
//-------------------------------------------------------------
void MainData::closeDBConnection()
{
    db.close();
}
//--------------------------------------------------------------

QSqlQuery MainData::makeQuery(QString q_str,bool& made,bool silent)
{
    QSqlQuery query;
    try
    {
        query.exec(q_str);
        if(query.isActive())
        {

             if(query.size()==0) throw "Неверный логин/пароль";
             if(!query.first()) throw "Ошибка при запросе данных из БД";
             made=true;
             return query;
        }
        else
            throw "Не могу выполнить запрос к БД";
    }
    catch(QString message)
    {

        if(!silent) MessageHandler::showMessage(MessageHandler::Error,message,0);
        made=false;
        query.clear();
    }
    catch(...)
    {
        if(!silent) MessageHandler::showMessage(MessageHandler::Error,"Ошибка при работе с БД\n"+q_str,0);
        made=false;
        query.clear();
    }


 }


