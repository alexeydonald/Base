# ifndef MESSAGE_HANDLER_H
# define MESSAGE_HANDLER_H

# include <qmessagebox.h>
# include <qstring.h>


class MessageHandler
{
public:
	static int showMessage(int type,QString message,QWidget* parent);
        enum message_type {Debug,Error,Message,CloseWindow,ExitApp};
private:
	MessageHandler();
	
};

# endif

