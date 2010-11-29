# include <qapplication.h>
# include <qtextcodec.h>
# include "MessageHandler.h"
# include "WindowManager.h"
# include <QtSql>




int main (int argc, char* argv[])
{

	QApplication app (argc,argv);
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
        WindowManager::Instance();
	
        return app.exec();
}

