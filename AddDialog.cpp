#include "AddDialog.h"

AddDialog::AddDialog(QWidget* parent):QDialog(parent,Qt::WindowMinimizeButtonHint|Qt::MSWindowsFixedSizeDialogHint/*|Qt::WShowModal*/)
{


    name="Добавить информацию в базу данных";
    setWindowTitle(name);

    item_list<<"Информация о преподавателе";

    list_widget=new QListWidget(this);
    list_widget->setSelectionMode(QAbstractItemView::SingleSelection);
    list_widget->addItems(item_list);

    button_add=new QPushButton("Добавить/Изменить",this);
    button_exit=new QPushButton("Закрыть",this);

    layout_button=new QVBoxLayout();
    layout_button->addWidget(button_add);
    layout_button->addWidget(button_exit);

    layout_main=new QHBoxLayout();
    layout_main->addWidget(list_widget);
    layout_main->addLayout(layout_button);




    setLayout(layout_main);

    connect(button_add,SIGNAL(clicked()),this,SLOT(onAdd()));
    connect(list_widget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(onClicked(QListWidgetItem*)));
    connect(button_exit,SIGNAL(clicked()),this,SLOT(onExit()));

}
//----------------------------------------------------------------------------

void AddDialog::onExit()
{
    emit returnButton(EXIT);
    close();
}
//----------------------------------------------------------------------------

void AddDialog::onAdd()
{
    QList<QListWidgetItem*> selected_item=list_widget->selectedItems();
    if(selected_item.count()!=0)
    {
        QListWidgetItem* item=selected_item.first();
        onClicked(item);
    }
}
//-----------------------------------------------------------------------------

void AddDialog::onClicked(QListWidgetItem* item)
{
   QString item_name=item->text();

    if(item_name=="Информация о преподавателе")
        emit returnButton(ADD_LECTURRER);
}

