#ifndef _MAIL_H_
#define _MAIL_H_

#include <vector>
#include <string>

using std::vector;
using std::string;

class Mail {
public:
        Mail(const string& user, const string& passwd, const string& server, 
                   const vector<string>& receiver, unsigned int type, unsigned int port);
        ~Mail();

public:
        int SendMail(const string& subject, const string& msg);

private:
        string         m_User;
        string         m_Passwd;
        string         m_Server;
        vector<string> m_Receiver;
        unsigned int   m_Type;
        unsigned int   m_Port;
};

#endif //_MAIL_H_
