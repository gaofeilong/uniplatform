#include <time.h>
#include <stdio.h>
#include <errno.h>

#include "Scanner.h"
#include "FilterFactory.h"
#include "Utils/Log/Log.h"
#include "Utils/Common/DirFileOpr.h"

Scanner::Scanner(const struct _FilterArg &config): m_Config(config)
{
}

Scanner::~Scanner()
{
}

int Scanner::Scan(const string &scanPath, set<string> &fileSet)
{
        int ret;
        DirFileOpr dfOpr;
        set<string> tmpFileSet;
        FilterFactory filterFactory;
        BOOSTPTR<Filter> filterPtr = filterFactory.CreateFilter(m_Config);
         
        ret = dfOpr.GetAllFileSet(scanPath, tmpFileSet);
        if (ret < 0) {
                LOG_ERR("travel directory %s error", scanPath.c_str());
                return -1;
        }
        LOG_DBG("%d files in path: %s", tmpFileSet.size(), scanPath.c_str());

        fileSet.clear();
        set<string>::iterator ssIt;
        for (ssIt = tmpFileSet.begin(); ssIt != tmpFileSet.end(); ++ssIt) {
                LOG_DBG("file befor filter: %s", ssIt->c_str());
                if (filterPtr->DoFilter(*ssIt)) {
                        LOG_DBG("file after filter: %s", ssIt->c_str());
                        LOG_DBG("========================");
                        fileSet.insert(*ssIt);
                }
        }
        return 0;
}
