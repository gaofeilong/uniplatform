/*******************************************************************************
 *
 * @ File Name  : 
 * @ Date       : 
 * @ Author     : 
 * @ Description: 
 * @ History    : 
 *
 * ****************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "Include/Types.h"
#include "Include/Protocol.h"
#include "Agent/Istri/IstriMgr.h"
#include "Utils/Lightev/TCPServer.h"
#include "Server/Frame/ServerFactory.h"

using boost::ref;
using boost::bind;
using boost::thread;

int main()
{
        daemon(1, 0);
        ServerFactory factory;
        TCPServer* server = new TCPServer(&factory, SERVER_PORT, THREAD_COUNT);

        IstriMgr istriMgr;
        thread t1(bind(&IstriMgr::UpdateNodeState, ref(istriMgr)));

        server->SetMagic('B');
        server->SetFcntlOpt(F_SETFD, 1);
        int ret = server->Start();
        if (ret < 0) {
                perror("TCPServer start failed ");
        }
}
