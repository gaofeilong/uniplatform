#include "Mail.h"

#include <stdio.h>
#include <string.h>

#include "Include/smtp.h"
#include "Utils/Log/Log.h"

#define FIELD "MailConf"

Mail::Mail(const string& user, const string& passwd, const string& server, 
                const vector<string>& receiver, unsigned int type, unsigned int port):
        m_User(user), m_Passwd(passwd), m_Server(server), m_Type(type), m_Port(port)
{
        m_Receiver = receiver;
}

Mail::~Mail()
{
}

int Mail::SendMail(const string& subject, const string& msg)
{
        int ret = 0;

        size_t idx = m_User.find("@");
        if (idx == string::npos) {
                LOG_ERR("domain is null!");
                return -1;
        }

        string domain = m_User.substr(idx+1);
        char msg_buf[10240] = {0};

        size_t rsize = m_Receiver.size();
        char* rctp_list[rsize+1];
        memset(rctp_list, 0, rsize+1);

        string toUser;
        for (size_t i=0; i<rsize; ++i) {
                rctp_list[i] = (char*)(m_Receiver[i].c_str());
                toUser += m_Receiver[i] + ";";
        }
        rctp_list[rsize] = NULL;

        /* init smtp */
        smtp_srv_t srv;
        smtp_init (&srv, domain.c_str(), m_User.c_str(), m_Server.c_str(), m_Port, 60);

        /* auth user */
        smtp_set_credentials (&srv, "auto", m_User.c_str(), m_Passwd.c_str(), "");

        /* ssl */
        if (m_Type == 1)  {
                smtp_ssl_opts(&srv, SMTPS, VERIFY_NONE, "", "");
        } else if (m_Type == 2) {
                smtp_ssl_opts(&srv, STARTTLS, VERIFY_NONE, "", "");
        }

        do {
                /* smtp connect */
                if (smtp_connect (&srv) == -1) {
                        puts (err_str_error());
                        LOG_ERR("smtp_connect error!");
                        ret = -1;
                        break;
                }
                /* init msg_buf */
                sprintf(msg_buf, "To: %s\nSubject: %s\nFrom: %s\n\n%s" ,
                                        toUser.c_str(), subject.c_str(), 
                                        m_User.c_str(), msg.c_str());

                /* send msg */
                if (smtp_send_msg (&srv, msg_buf, rctp_list) == -1) {
                        puts (err_str_error ());
                        LOG_ERR("smtp_send_msg error!");
                        ret = -2;
                }
        } while (false);

        /* close */
        if (smtp_close (&srv) == -1) {
                puts (err_str_error ());
        }

        /* clean up */
        smtp_cleanup (&srv);

        return ret;
}
