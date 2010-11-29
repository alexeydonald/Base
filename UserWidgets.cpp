#include "UserWidgets.h"

//#######################################Класс ComboSurname#####################

void ComboSurname::setModel(QAbstractItemModel* model)
{

     int rw=model->rowCount();
     QModelIndex idx;
     QString result;
     int num;
     QStringList list;
     for(int i=0;i<rw;++i)
     {
        result="";
        idx=model->index(i,0,QModelIndex());
        num=idx.data().toInt();
        result+=QString::number(num);
        for(int j=1;j<4;++j)
        {
            idx=model->index(i,j,QModelIndex());
            result=result+" "+idx.data().toString();
        }
        list<<result;
     }
     st_model=new QStringListModel(list,this);
     QComboBox::setModel(st_model);
}
//------------------------------------------------------------------------

int ComboSurname::curIdx()
{
    QString str=QComboBox::currentText();
    int pos=str.indexOf(" ");
    str=str.left(pos);
    return str.toInt();
}





