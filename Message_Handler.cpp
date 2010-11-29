# include "MessageHandler.h"

//----------------------------------------------------------------
MessageHandler::MessageHandler()
{}

//----------------------------------------------------------------
int MessageHandler::showMessage(int type,QString message,QWidget* parent)
{
	QMessageBox msgBox(parent);	
	int ret=-1;
	switch (type)
	{
		case Debug:
			msgBox.setWindowTitle("�������");
			msgBox.setText(message);
                        ret=msgBox.exec();
			break;
		case Error:
                        msgBox.setWindowTitle("������");
                        msgBox.setText(message);
                        ret=msgBox.exec();
			break;
		case Message:
                        break;
                case CloseWindow:
                        msgBox.setWindowTitle("������� ����");
                        msgBox.setText(message);
                        msgBox.setStandardButtons(QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
                        msgBox.setDefaultButton(QMessageBox::Save);
                        ret=msgBox.exec();
                        break;
                case ExitApp:
                        msgBox.setWindowTitle("������� ����");
                        msgBox.setText(message);
                        msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
                        msgBox.setDefaultButton(QMessageBox::Cancel);
                        ret=msgBox.exec();
                        break;
		default:
			msgBox.setWindowTitle("������");
			msgBox.setText("�������� ��� ���������");
			ret=msgBox.exec();
			break;
	}
	return ret;
}
