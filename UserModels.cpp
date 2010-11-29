#include "UserModels.h"

//###################################Metod Model###############################

MetodModel::MetodModel(QSqlTableModel* model_ev,QSqlTableModel* model_type,QObject* parent):QStandardItemModel(parent)
{
    model_events=model_ev;
    model_report_type=model_type;
}

//--------------------------------------------------------------------------

bool MetodModel::setData(const QModelIndex &index,const QVariant &value,int role/*,bool nul*/)
{
    //MessageHandler::showMessage(MessageHandler::Error,"1",0);
    if(index.column()==2)
    {
        //MessageHandler::showMessage(MessageHandler::Error,QString::number(role),0);
        //int temp=value.toInt();
        //if(nul)
        int temp=value.toInt();
        if(role!=Qt::DecorationRole)
           ++temp;
        else
            role=Qt::EditRole;
      //  MessageHandler::showMessage(MessageHandler::Error,"temp "+QString::number(value.toInt()),0);
        QString res="";
        QModelIndex idx;
        //model_events->submitAll();
        int n=model_events->rowCount();
       // MessageHandler::showMessage(MessageHandler::Error,"n "+QString::number(n),0);
        bool flag=true;
        for(int i=0;i<n&&flag;++i)
        {
            idx=model_events->index(i,0);
         //   MessageHandler::showMessage(MessageHandler::Error,"idx.data "+QString::number(idx.data().toInt()),0);
            if(idx.data().toInt()==temp)
            {
               idx=model_events->index(i,1);
               res=idx.data().toString();
           //    MessageHandler::showMessage(MessageHandler::Error,"res "+res,0);
               flag=false;
            }
        }
        if(!flag)
        {
            idx=QStandardItemModel::index(index.row(),8);
            QStandardItemModel::setData(idx,temp);
            return QStandardItemModel::setData(index,QVariant(res),role);
        }
        else
            return true;
    }

    if(index.column()==5)
    {
        int temp=value.toInt();
        if(role!=Qt::DecorationRole)
           ++temp;
        else
            role=Qt::EditRole;
        QString res="";
        QModelIndex idx;
        model_report_type->submitAll();
        int n=model_report_type->rowCount();
        bool flag=true;
        for(int i=0;i<n&&flag;++i)
        {
            idx=model_report_type->index(i,0);
            if(idx.data().toInt()==temp)
            {
               idx=model_report_type->index(i,1);
               res=idx.data().toString();
               flag=false;
            }
        }
        idx=QStandardItemModel::index(index.row(),9);
        QStandardItemModel::setData(idx,temp);
        return QStandardItemModel::setData(index,QVariant(res),role);
    }
    role=Qt::EditRole;
    return QStandardItemModel::setData(index,value,role);

}
//-----------------------------------------------------------------------------
QVariant MetodModel::data(const QModelIndex &index,int role)
{
    if (index.column()==2)
    {
        QModelIndex idx=QStandardItemModel::index(index.row(),8);
        return idx.data();
    }
    if (index.column()==5)
    {
        QModelIndex idx=QStandardItemModel::index(index.row(),9);
        return idx.data();
    }
    return QStandardItemModel::data(index,role);
}
