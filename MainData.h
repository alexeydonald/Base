#ifndef MAINDATA_H
#define MAINDATA_H

# include <QString>
# include <QSettings>
# include <QSqlDatabase>
# include <QSqlError>
# include <QSqlQuery>
# include "MessageHandler.h"

//------------------Типы возвращаемых значений для Window Manager--------------


struct current_document
{
    int di_ida;
    int di_id;
    int di_activ;
};


struct settings_data
{
    QString hostname;
    QString port;
    QString dbname;
    QString username;
    QString password;
    bool isAdmin;
    int professor_id;
};

class MainData
{
public:

    //------------------Типы возвращаемых значений для Window Manager--------------



    static MainData* Instance();
    settings_data* getSettingsData() {return &settings; }
    void setListId(int id){ list_id=id; }
    int getListId(){ return list_id; }

    bool createDBConnection();
    void closeDBConnection();
    bool checkUser();
    void saveData();
    QSqlQuery makeQuery(QString query,bool& made,bool silent=false);


private:
    MainData();

    void loadData();

    static MainData* man_ptr;

    settings_data settings;
    int  list_id;
    QString save_path;
    QSqlDatabase db;

};

#endif // MAINDATA_H
