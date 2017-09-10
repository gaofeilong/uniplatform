#ifndef _DDFS_CONFIG_OPR_H_
#define _DDFS_CONFIG_OPR_H_

#include <string>
#include <vector>
#include "Public/Config/ConfigOpr.h"

using std::string;
using std::vector;

struct _DdfsConfig {
        _DdfsConfig()
        {
        }
        /* FileSystem */
        string delayedCloseTime;      //延迟关闭的时间，单位:s，设置范围30-90,设置为0 表示关闭此功能
        string maxDelayedCloseNum;    //设置最大可以延迟关闭的文件的数目；当延迟关闭功能没有打开时，这个选项无效
        string raChunkCnt;            //预读窗口Chunk个数
        string raStrategy;            //预读算法
        string fsBucketCount;         //桶的数量，单位：MB默认值：256MB
        string mmapSize;              //内存映射大小，单位MB 默认1024
        string mountPoint;            //文件系统挂载时生成的路径
        /* lds */
        string ldsBucketCount;        //桶的数量通常,(数据量->桶数量),(1TB->16MB),(4TB->64MB),单位：MB默认值：64MB
        string minSpace;              //预留最小空间,单位GB,最小10GB
        string fileTypeCount;         //blockIndex级别,默认：1
        string biPath;                //blockIndex存储路径
        /* dpl */
        string dplCpuNum;             //默认0
        string dplTimeOutSec;         //默认2
        string dplCapacity;           //默认4
        string dplReadMode;           //流线先中读的模式:0:串行1:并行，默认为0
        /* plds */
        string byteDedup;             //压缩算法
        string blockSize;             //单位MB范围[1-128]MB默认：16,示例：blockSize=16
        string chunkSize;             //单位KB范围[32-128]KB,默认：64,示例：chunkSize=64
        string dedupOption;           //是否消冗，1表示开启，0表示关闭默认：1(开启),示例：dedupOption=1
        string crcOption;             //是否开启crc校验，1表示开启，0表示关闭默认：1(开启),示例：crcOption=1
        string byteCmpOption;         //是否开启字节级比较，1表示开启，0表示关闭默认：1(开启),示例：byteCmpOption=1
        string hashType;              //哈希方法，目前使用Sha1，默认为1
        string compressType;          //压缩方法,0代表QuickLZ,1代表Z,2代表Snappy,3代表Lzma,默认使用2
        string mapPath;               //数据索引存储路径，推荐配置与索引路径相同
        string metaPath;              //元数据存储路径，推荐配置与索引路径相同
        vector<string> dataPathList;  //数据存储路径，以卷的形式添加，动态追加路径，暂时不支持删除路径
};


class DdfsConfigOpr : public ConfigOpr<struct _DdfsConfig> {
public:
        DdfsConfigOpr(const string& configPath);
        ~DdfsConfigOpr();

public:
        int  Read(struct _DdfsConfig& ddfsConfig);
        int  Write(const struct _DdfsConfig& ddfsConfig);
};

#endif //_DDFS_CONFIG_OPR_H_
