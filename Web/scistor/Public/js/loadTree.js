function dtree(){
    
    d=new dTree('d');

    d.add(0,-1,'','');

    d.add(2,0,'系统设置','','','','/scistor/Public/img/icons/sysSet_20.png','/scistor/Public/img/icons/sysSet_20.png','true');
    d.add(21,2,'系统信息','index.php?s=/Sysinfo/index','','content','/scistor/Public/img/icons/sysMsg_20.png');
    //d.add(24,2,'系统时间','index.php?s=/Systime/index','','content','/scistor/Public/img/icons/time_20.png');
    d.add(24,2,'系统时间','index.php?s=/Stime/index','','content','/scistor/Public/img/icons/time_20.png');
    d.add(25,2,'事件通知','index.php?s=/Event/index','','content','/scistor/Public/img/icons/mail_20.png');
    d.add(26,2,'管理口令','index.php?s=/Password/index','','content','/scistor/Public/img/icons/key_20.png','');
    d.add(27,2,'UPS管理','index.php?s=/Ups/index','','content','/scistor/Public/img/icons/save_20.png');
    d.add(28,2,'LICENSE','index.php?s=/License/index','','content','/scistor/Public/img/icons/license_20.png');
    //d.add(29,2,'维护与更新','index.php?s=/Update/index','','content','/scistor/Public/img/icons/update1_20.png');
    d.add(210,2,'系统日志','index.php?s=/Syslog/index','','content','/scistor/Public/img/icons/doc_20.png');

    d.add(4,0,'网络管理','','','','/scistor/Public/img/icons/network1_20.png','/scistor/Public/img/icons/network1_20.png');
    d.add(45,4,'基础网络配置','index.php?s=/Basenet/index','','content','/scistor/Public/img/icons/setPaper1_20.png');
    d.add(41,4,'网卡配置','index.php?s=/Network/index','','content','/scistor/Public/img/icons/netcard_20.png');
    //d.add(42,4,'链路聚合','index.php?s=/Bond/index','','content','/scistor/Public/img/icons/road1_20.png');
    //d.add(43,4,'FC设置','index.php?s=/Fc/index','','content','/scistor/Public/img/icons/FC_20.png');
    //d.add(44,4,'Iscsi设置','index.php?s=/Iscsi/index','','content','/scistor/Public/img/icons/ISCSI_20.png');
    
    
    /*d.add(3,0,'存储管理','','','','/scistor/Public/img/icons/diskMsg_20.png','/scistor/Public/img/icons/diskMsg_20.png');
    d.add(31,3,'物理磁盘','index.php?s=/Pd/index','','content','/scistor/Public/img/icons/disk_20.png');
    d.add(32,3,'卷组管理','index.php?s=/Vg/index','','content','/scistor/Public/img/icons/volumes_20.png');
    d.add(33,3,'卷管理','index.php?s=/Lv/index','','content','/scistor/Public/img/icons/volume_20.png');*/
//    d.add(34,3,'快照','__APP__/UpdateType/index');
    
    /*
    d.add(5,0,'消冗文件系统','');
    d.add(51,5,'单机消冗','__APP__/Index/index');
    d.add(52,5,'分布式消冗','__APP__/Index/index');

    d.add(6,0,'数据归档管理','');
    d.add(61,6,'数据归档管理','__APP__/Index/index');

    d.add(7,0,'用户管理','');
    d.add(71,7,'用户管理','__APP__/Index/index');

    d.add(8,0,'共享管理','');
    d.add(81,8,'共享管理','__APP__/Index/index');
    */
    d.add(11,0,'消冗文件系统管理','','','','/scistor/Public/img/icons/ddfs.png','/scistor/Public/img/icons/ddfs.png');
    d.add(111,11,'配置管理','index.php?s=/Ddfs/index','','content','/scistor/Public/img/icons/ddfs.png');
    d.add(112,11,'共享管理','index.php?s=/Dfshare/index','','content','/scistor/Public/img/icons/share.png');
    
    d.add(12,0,'分布式消冗文件系统','','','','/scistor/Public/img/icons/fbs_20.png','/scistor/Public/img/icons/fbs_20.png');
    d.add(121,12,'集群概况','index.php?s=/Hdfs/summary','','content','/scistor/Public/img/icons/chart_pie_20.png');
    d.add(122,12,'配置管理','index.php?s=/Hdfs/index','','content','/scistor/Public/img/icons/fbset_20.png');
    //d.add(122,12,'集群信息','index.php?s=/Ddfs/index','','content','/scistor/Public/img/icons/ddfs.png');
    
    d.add(9,0,'归档管理','','','','/scistor/Public/img/icons/guid.png','/scistor/Public/img/icons/guid.png');
    d.add(91,9,'数据归档','index.php?s=/Archive/index','','content','/scistor/Public/img/icons/guid.png');
    d.add(92,9,'归档日志','index.php?s=/Archive/archiveLog','','content','/scistor/Public/img/icons/syslog4.png');
    
    /*d.add(10,0,'NAS管理','','','','/scistor/Public/img/icons/nas.png','/scistor/Public/img/icons/nas.png');
    d.add(101,10,'共享管理','index.php?s=/Share/index','','content','/scistor/Public/img/icons/share.png');
    d.add(102,10,'用户和组','index.php?s=/Usergroup/index','','content','/scistor/Public/img/users.png');
    d.add(103,10,'服务','index.php?s=/Server/index','','content','/scistor/Public/img/icons/fuwu.png');*/
    
    
    /*
    d.add(9,0,'VTL管理','','','','/scistor/Public/img/icons/vtl_20.png','/scistor/Public/img/icons/vtl_20.png');
    d.add(91,9,'虚拟磁带库管理','right.html','','content','/scistor/Public/img/icons/tape_20.png');
    d.add(92,9,'远程复制','right.html','','content','/scistor/Public/img/icons/copy_20.png');
    d.add(93,9,'配置和报表','right.html','','content','/scistor/Public/img/icons/setPaper1_20.png');
    d.add(94,9,'授权','right.html','','content','/scistor/Public/img/icons/shouquan_20.png');
    */
    d.config.useSelection = true;//选中的节点背景色设置
        
    document.write(d);
}
