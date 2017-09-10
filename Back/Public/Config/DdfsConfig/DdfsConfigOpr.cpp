/* 
 * @ Date        : 2012-09-04
 * @ Auther      : wangbo
 * @ Description : 对DDFS配置文件进行操作(读,写)
 * @ Email       : wang.bo@scidata.cn
 *
 * @ History     : 2012-09-04 创建
 *                 2012-10-10 完成功能
 *
 */

#include "DdfsConfigOpr.h"

#include <stdio.h>
#include "Utils/Log/Log.h"
#include "Include/Types.h"
#include "Utils/Config/IniConfig.h"
#include "Utils/Common/CommonFunc.h"

DdfsConfigOpr::DdfsConfigOpr(const string& configPath):
        ConfigOpr<struct _DdfsConfig>(configPath)
{
}

DdfsConfigOpr::~DdfsConfigOpr()
{
}

int DdfsConfigOpr::Read(struct _DdfsConfig &ddfsConfig)
{
        int ret = 0;
        IniConfig ini(m_ConfigPath);
        ret = ini.Open();
        if (ret < 0) {
                LOG_ERR("Open Error! configPath=%s\n", m_ConfigPath.c_str());
                return -1;
        }
        /* FileSystem */
        string FS = "FileSystem";
        ini.GetVal(FS, "delayedCloseTime", ddfsConfig.delayedCloseTime);
        ini.GetVal(FS, "maxDelayedCloseNum", ddfsConfig.maxDelayedCloseNum);
        ini.GetVal(FS, "raChunkCnt", ddfsConfig.raChunkCnt);
        ini.GetVal(FS, "raStrategy", ddfsConfig.raStrategy);
        ini.GetVal(FS, "bucketCount", ddfsConfig.fsBucketCount);
        ini.GetVal(FS, "mmapSize", ddfsConfig.mmapSize);
        ini.GetVal(FS, "mountPoint", ddfsConfig.mountPoint);
        /* dpl */
        string DPL = "Dpl";
        ini.GetVal(DPL, "dplCpuNum", ddfsConfig.dplCpuNum);
        ini.GetVal(DPL, "dplTimeOutSec", ddfsConfig.dplTimeOutSec);
        ini.GetVal(DPL, "dplCapacity", ddfsConfig.dplCapacity);
        ini.GetVal(DPL, "dplReadMode", ddfsConfig.dplReadMode);
        /* lds */
        string LDS = "Lds";
        ini.GetVal(LDS, "bucketCount", ddfsConfig.ldsBucketCount);
        ini.GetVal(LDS, "minSpace", ddfsConfig.minSpace);
        ini.GetVal(LDS, "fileTypeCount", ddfsConfig.fileTypeCount);
        ini.GetVal(LDS, "biPath", ddfsConfig.biPath);
        /* plds */
        string PLDS = "Plds";
        ini.GetVal(PLDS, "blockSize", ddfsConfig.blockSize);
        ini.GetVal(PLDS, "chunkSize", ddfsConfig.chunkSize);
        ini.GetVal(PLDS, "dedupOption", ddfsConfig.dedupOption);
        ini.GetVal(PLDS, "crcOption", ddfsConfig.crcOption);
        ini.GetVal(PLDS, "byteCmpOption", ddfsConfig.byteCmpOption);
        ini.GetVal(PLDS, "hashType", ddfsConfig.hashType);
        ini.GetVal(PLDS, "compressType", ddfsConfig.compressType);
        ini.GetVal(PLDS, "mapPath", ddfsConfig.mapPath);
        ini.GetVal(PLDS, "metaPath", ddfsConfig.metaPath);
        /* 数据路径可以为多个 */
        int cnt = 1;
        char buf[BUF1K];
        string dataPath;
        while (true) {
                sprintf(buf, "dataPath%d", cnt);
                ret = ini.GetVal(PLDS, buf, dataPath);
                if (ret < 0) {
                        break;
                }
                AppendBias(dataPath);
                ddfsConfig.dataPathList.push_back(dataPath);
                ++cnt;
        }

        return 0;
}

int DdfsConfigOpr::Write(const struct _DdfsConfig &ddfsConfig)
{
        int ret = 0;
        IniConfig ini(m_ConfigPath);
        ret = ini.Open();
        if (ret < 0) {
                LOG_ERR("Open Error! configPath=%s\n", m_ConfigPath.c_str());
                return -1;
        }
        /* FileSystem */
        string FS = "FileSystem";
        if (!ddfsConfig.delayedCloseTime.empty()) {
                ini.SetVal(FS, "delayedCloseTime", ddfsConfig.delayedCloseTime);
        }
        if (!ddfsConfig.maxDelayedCloseNum.empty()) {
                ini.SetVal(FS, "maxDelayedCloseNum", ddfsConfig.maxDelayedCloseNum);
        }
        if (!ddfsConfig.raChunkCnt.empty()) {
                ini.SetVal(FS, "raChunkCnt", ddfsConfig.raChunkCnt);
        }
        if (!ddfsConfig.raStrategy.empty()) {
                ini.SetVal(FS, "raStrategy", ddfsConfig.raStrategy);
        }
        if (!ddfsConfig.fsBucketCount.empty()) {
                ini.SetVal(FS, "bucketCount", ddfsConfig.fsBucketCount);
        }
        if (!ddfsConfig.mmapSize.empty()) {
                ini.SetVal(FS, "mmapSize", ddfsConfig.mmapSize);
        }
        if (!ddfsConfig.mountPoint.empty()) {
                ini.SetVal(FS, "mountPoint", ddfsConfig.mountPoint);
        }
        /* dpl */
        string DPL = "Dpl";
        if (!ddfsConfig.dplCpuNum.empty()) {
                ini.SetVal(DPL, "dplCpuNum", ddfsConfig.dplCpuNum);
        }
        if (!ddfsConfig.dplTimeOutSec.empty()) {
                ini.SetVal(DPL, "dplTimeOutSec", ddfsConfig.dplTimeOutSec);
        }
        if (!ddfsConfig.dplCapacity.empty()) {
                ini.SetVal(DPL, "dplCapacity", ddfsConfig.dplCapacity);
        }
        if (!ddfsConfig.dplReadMode.empty()) {
                ini.SetVal(DPL, "dplReadMode", ddfsConfig.dplReadMode);
        }
        /* lds */
        string LDS = "Lds";
        if (!ddfsConfig.ldsBucketCount.empty()) {
                ini.SetVal(LDS, "bucketCount", ddfsConfig.ldsBucketCount);
        }
        if (!ddfsConfig.minSpace.empty()) {
                ini.SetVal(LDS, "minSpace", ddfsConfig.minSpace);
        }
        if (!ddfsConfig.fileTypeCount.empty()) {
                ini.SetVal(LDS, "fileTypeCount", ddfsConfig.fileTypeCount);
        }
        if (!ddfsConfig.biPath.empty()) {
                ini.SetVal(LDS, "biPath", ddfsConfig.biPath);
        }

        /* plds */
        string PLDS = "Plds";
        if (!ddfsConfig.blockSize.empty()) {
                ini.SetVal(PLDS, "blockSize", ddfsConfig.blockSize);
        }
        if (!ddfsConfig.chunkSize.empty()) {
                ini.SetVal(PLDS, "chunkSize", ddfsConfig.chunkSize);
        }
        if (!ddfsConfig.dedupOption.empty()) {
                ini.SetVal(PLDS, "dedupOption", ddfsConfig.dedupOption);
        }
        if (!ddfsConfig.crcOption.empty()) {
                ini.SetVal(PLDS, "crcOption", ddfsConfig.crcOption);
        }
        if (!ddfsConfig.byteCmpOption.empty()) {
                ini.SetVal(PLDS, "byteCmpOption", ddfsConfig.byteCmpOption);
        }
        if (!ddfsConfig.hashType.empty()) {
                ini.SetVal(PLDS, "hashType", ddfsConfig.hashType);
        }
        if (!ddfsConfig.compressType.empty()) {
                ini.SetVal(PLDS, "compressType", ddfsConfig.compressType);
        }
        if (!ddfsConfig.mapPath.empty()) {
                ini.SetVal(PLDS, "mapPath", ddfsConfig.mapPath);
        }
        if (!ddfsConfig.metaPath.empty()) {
                ini.SetVal(PLDS, "metaPath", ddfsConfig.metaPath);
        }
        /* 数据路径可以为多个 */
        int cnt = 1;
        char buf[BUF1K];
        string dataPath;
        for (size_t i=0; i<ddfsConfig.dataPathList.size(); ++i, ++cnt) {
                string tStr = ddfsConfig.dataPathList[i];
                sprintf(buf, "dataPath%d", cnt);
                ini.GetVal(PLDS, buf, dataPath);
                if (dataPath != tStr && !tStr.empty()) {
                        ini.SetVal(PLDS, buf, tStr);
                }
        }

        ret = ini.Close();
        if (ret < 0) {
                LOG_ERR("ini close! path=%s\n", m_ConfigPath.c_str());
                ret = -2;
        }
        return ret;
}
