#ifndef USERMODELS_H
#define USERMODELS_H
#include "MainData.h"
#include "MessageHandler.h"

#include <QStandardItemModel>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QModelIndex>
#include <QString>
#include <QMap>

//-----------------Metod Model---------------------------------------------
class MetodModel : public QStandardItemModel
{
    Q_OBJECT
    public:
        MetodModel(QSqlTableModel* model_ev,QSqlTableModel* model_type,QObject* parent=0);

        bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole/*,bool nul=true*/);
        QVariant data(const QModelIndex &index,int role=Qt::DisplayRole );
    private:

        QSqlTableModel* model_events;
        QSqlTableModel* model_report_type;
};


#endif // USERMODELS_H
