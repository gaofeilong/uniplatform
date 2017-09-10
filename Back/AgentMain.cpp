/*******************************************************************************
 *
 * @ File Name  : AgentMain.cpp
 * @ Date       : 2013-03-04
 * @ Author     : wangbo
 * @ Description: 代理端入口程序
 * @ History    : 创建 2013-03-04
 *
 * ****************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "Include/Types.h"
#include "Include/Protocol.h"
#include "Utils/Log/Log.h"
#include "Utils/Lightev/TCPServer.h"
#include "Agent/Istri/IstriMgr.h"
#include "Agent/Frame/AgentFactory.h"

using boost::ref;
using boost::bind;
using boost::thread;

int main(int argc, char** argv)
{
        int nodaemon = 0, logToStdout = 0;
        for (int i=1; i<argc; ++i)
        {
                if (argv[i] && !*argv[i]) ;
                else if (argv[i] && '-'==*argv[i] && 'f'==*(argv[i]+1) && !*(argv[i]+2)) nodaemon = 1;
                else if (argv[i] && '-'==*argv[i] && 'o'==*(argv[i]+1) && !*(argv[i]+2)) logToStdout = 1;
        }
        nodaemon || daemon(1, 1);
        if (logToStdout)
        {
                Log::Init("/dev/stdout");
        }

        IstriMgr istriMgr;
        thread t1(bind(&IstriMgr::UpdateNodeInfo, ref(istriMgr)));

        AgentFactory factory;
        TCPServer* server = new TCPServer(&factory, AGENT_PORT, THREAD_COUNT);

        server->SetMagic('B');
        server->SetFcntlOpt(F_SETFD, 1);
        int ret = server->Start();
        if (ret < 0) {
                perror("TCPServer start failed ");
        }
}
