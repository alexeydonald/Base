#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

# include <QMap>
# include <QString>
# include "LoginDialog.h"
# include "MainData.h"
# include "ListDialog.h"
# include "AddDialog.h"
# include "MessageHandler.h"
# include "AddLecturer.h"
# include "PlanDialog.h"
#include "settingdlg.h"


class WindowManager:public QWidget
{
 Q_OBJECT
 public:

    static WindowManager* Instance();

//---------Функции получения данных----------------
    LoginDialog* getLoginPtr() { return login_ptr; }
    ListDialog* getListPtr() { return list_ptr; }
    PlanDialog* getPlanPtr() { return plan_ptr; }
    SettingDlg* getSettingsPtr() {return settings_ptr; }


 private:

    current_document cur_doc;
    static WindowManager* man_ptr;
    WindowManager();
    void deleteItems(QString table,QString field, int ida);
    void deleteDocument(int kill_id);
    void updateItems(QString table,QString field,QString value,QString where_f,QString value_f);

//----------Указатели на основные оконные классы----
    LoginDialog* login_ptr;
    ListDialog* list_ptr;
    PlanDialog* plan_ptr;
    SettingDlg* settings_ptr;
    AddDialog* add_ptr;
    AddLecturer* lecturer_ptr;


 private slots:
    void onLogin(int ret);
    void onList(int ret);
    void onAddDialog(int ret);
    void onPlanDialog(int ret);
};






#endif // WINDOWMANAGER_H
