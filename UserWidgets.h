#ifndef USERWIDGETS_H
#define USERWIDGETS_H

#include <QComboBox>
#include <QStringListModel>
#include <QString>
#include <QStringList>



//##################Переопределенные классы для 1-ой страницы#############
class ComboSurname: public QComboBox
{
Q_OBJECT
Q_PROPERTY(int curIdx READ curIdx)
    public:
       ComboSurname(QWidget* parent=0):QComboBox(parent){}
       void setModel ( QAbstractItemModel * model );
       int curIdx();
    private:
       QStringListModel* st_model;



};



#endif // USERWIDGETS_H
