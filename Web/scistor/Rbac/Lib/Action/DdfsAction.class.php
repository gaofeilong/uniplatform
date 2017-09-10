<?php
class DdfsAction extends CommonAction
{
    //==================================================================
    // 功能：消冗文件系统页面
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function index()
    {
    	$this->unsetSig();   //删除保存在session里的内容
        $recv = array();
        $send = array();
        $info = array();
        $m    = M();
        $ddfs = $m->query("select * from ddfsmp");
        $adfs = $m->query("select * from adfsmp");
        if (count($ddfs) > 0) {
            if (!$this->exInfo(CMD_MP_LIST, $send, $recv)) {
                $this->error("获取挂载点信息失败: ".$recv["info"]);
            }
            $system = 'ddfs';
        } elseif (count($adfs) > 0) {
            if (!$this->exInfo(0x000400A0, $send, $recv)) {
                $this->error("获取挂载点信息失败: ".$recv["info"]);
            }
            $recv = $recv['adfsList'];
            $system = 'adfs';
        } elseif(count($ddfs) > 0 && count($adfs) > 0) {
            $this->error("获取挂载点信息失败: 文件系统类型冲突");
        }
        //查找被共享的路径
        $mp = $m->query("select path from nfsserver");
        for ($i = 0; $i < count($mp); $i++) {
        	$mpArr[] = $mp[$i]['path'];
        }
        for ($i = 0;$i < count($recv);$i++) {
        	//分为adfs和ddfs两种
            if ($system == 'ddfs') {
	        	if ($recv[$i]["total"] * 1 == 0) {
	        	   $ratio = '0.00';
	        	} elseif ($recv[$i]["ratio"] * 1 == 0) {
	               $ratio = '1.00';
	            } elseif ($recv[$i]["ratio"] * 1 >= 99.99) {
	        	   $ratio = '9999.00';
	        	} else {
	        	   $ratio = sprintf("%.2f",100 / (100 - $recv[$i]["ratio"] * 1));
	            }
	            $info[$i]["ratio"]      = $ratio;
	            $info[$i]["total"]      = $recv[$i]["total"];
                $info[$i]["real"]       = $recv[$i]["real"];
                $info[$i]["dataPath"]   = $recv[$i]["dataPath"];
                $info[$i]["configPath"] = $recv[$i]["configPath"];
            } else {
                $info[$i]["ratio"]  = $recv[$i]["ratio"];
                $info[$i]["total"]  = $this->format_bytes($recv[$i]["total"]);
                $info[$i]["real"]   = $this->format_bytes($recv[$i]["real"]);
                $info[$i]["firstDataPath"] = $recv[$i]["firstDataPath"];
                $info[$i]["dataPath"][]    = $recv[$i]["firstDataPath"];
                for ($j = 0; $j < count($recv[$i]['otherDataPath']); $j++) {
                    $info[$i]["dataPath"][] = $recv[$i]["otherDataPath"][$j];
                }
            }
            $info[$i]["mountPoint"] = $recv[$i]["mountPoint"];
            $info[$i]["capacity"]   = $this->format_bytes($recv[$i]["capacity"] * 1);
            $info[$i]["left"]       = $this->format_bytes($recv[$i]["left"] * 1);
            $info[$i]["state"]      = $recv[$i]["state"];
            $info[$i]["json_recv"]  = json_encode($recv[$i]);
            //查找mp路径是否被建立共享
            if (in_array($recv[$i]['mountPoint'],$mpArr)) {
                $info[$i]["nfsState"] = $recv[$i]["nfsState"];   
            } else {
                $info[$i]["nfsState"] = 'noPath';
            }
        }
        $fbsnode = $m->query("select * from fbs_index_node");
        $this->assign("fbsnode",count($fbsnode));
        $this->assign("ddfs_data",$info);
        $this->assign("system",$system);
        $this->display();
    }

    //==================================================================
    // 功能：新建挂载点
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function createMountpoint()
    {
        $recv = array();
        $mountPoint = $this->handPath($_POST["mountPoint"]);
        $metaPath   = $this->handPath($_POST["metaPath"]);
        $mapPath    = $this->handPath($_POST["mapPath"]);
        $biPath     = $this->handPath($_POST["biPath"]);
        $dataPath   = $_POST["dataPath"];
        $mountWay   = $_POST["mountWay"];
        for ($i = 0;$i < count($dataPath);$i++) {
           $dataPath[$i] = $this->handPath($dataPath[$i]);
        }
        $errorMark = '';    //标示错误的信息
        if ($mountWay == 'ddfs') {
	        $send = array(
	            "mountPoint"         => $mountPoint,
	            "dataPath"           => $dataPath,
	            "metaPath"           => $metaPath,
	            "mapPath"            => $mapPath,
	            "biPath"             => $biPath,
	            /*"bucketCount"        => $_POST["bucketCount"],
	            "minSpace"           => $_POST["minSpace"],
	            "fileTypeCount"      => $_POST["fileTypeCount"],
	            "blockSize"          => $_POST["blockSize"],
	            "chunkSize"          => $_POST["chunkSize"],
	            //"byteDedup"        => $_POST["byteDedup"],
	            "hashType"           => $_POST["hashType"],
	            "dedupOption"        => $_POST["dedupOption"],
	            "crcOption"          => $_POST["crcOption"],
	            "byteCmpOption"      => $_POST["byteCmpOption"],
	            "compressType"       => $_POST["compressType"],
	            "dplCpuNum"          => $_POST["dplCpuNum"],
	            "dplTimeOutSec"      => $_POST["dplTimeOutSec"],
	            "dplCapacity"        => $_POST["dplCapacity"],
	            "fsBucketCount"      => $_POST["fsBucketCount"],
	            "mmapSize"           => $_POST["mmapSize"],
	            "raStrategy"         => $_POST["raStrategy"],
	            "raChunkCnt"         => $_POST["raChunkCnt"],
	            "dplReadMode"        => $_POST["dplReadMode"],
	            "delayedCloseTime"   => $_POST["delayedCloseTime"],
	            "maxDelayedCloseNum" => $_POST["maxDelayedCloseNum"],*/
	        );
	        if (!$this->exInfo(CMD_MP_CREATE, $send, $recv)) {          
	            $errorMark = "error";
	        }
        } else {
        	$firstDb = $dataPath[0];
        	$dbPath  = array_shift($dataPath);
            $send = array(
                "firstDataPath" => $firstDb,
                "otherDataPath" => $dataPath,
            );
            if (!$this->exInfo(0x000400B0, $send, $recv)) {          
                $errorMark = "error";
            }
        }
        if ($errorMark == 'error') {          
            $this->writeLog("DDFS","创建消冗挂载点失败".$recv["info"],"error");
            $this->ajaxReturn("/Ddfs/index", $recv["info"], 0);
        }
        $this->writeLog("DDFS","创建消冗挂载点成功","info");
        $this->ajaxReturn("/Ddfs/index", "创建消冗挂载点成功!", 1);
    }

    //==================================================================
    // 功能：编辑挂载点
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function editMountpoint()
    {
        $recv = array();
        $mountPoint = $this->handPath($_POST["mountPoint"]);
        $metaPath   = $this->handPath($_POST["metaPath"]);
        $mapPath    = $this->handPath($_POST["mapPath"]);
        $biPath     = $this->handPath($_POST["biPath"]);
        $dataPath   = $_POST["e_dataPath"];
        $mountWay   = $_POST["mountWay"];
        $errorMark  = '';
        for ($i = 0;$i < count($dataPath);$i++) {
           $dataPath[$i] = $this->handPath($dataPath[$i]);
        }
        if ($mountWay == 'ddfs') {
	        $send = array(
	            "configPath"         => $_POST["configPath"],
	            "mountPoint"         => $mountPoint,
	            "dataPath"           => $dataPath,
	            "metaPath"           => $metaPath,
	            "mapPath"            => $mapPath,
	            "biPath"             => $biPath,
	            /*"bucketCount"        => $_POST["bucketCount"],
	            "minSpace"           => $_POST["minSpace"],
	            "fileTypeCount"      => $_POST["fileTypeCount"],
	            "blockSize"          => $_POST["blockSize"],
	            "chunkSize"          => $_POST["chunkSize"],
	            //"byteDedup"        => $_POST["byteDedup"],
	            "hashType"           => $_POST["hashType"],
	            "dedupOption"        => $_POST["dedupOption"],
	            "crcOption"          => $_POST["crcOption"],
	            "byteCmpOption"      => $_POST["byteCmpOption"],
	            "compressType"       => $_POST["compressType"],
	            "dplCpuNum"          => $_POST["dplCpuNum"],
	            "dplTimeOutSec"      => $_POST["dplTimeOutSec"],
	            "dplCapacity"        => $_POST["dplCapacity"],
	            "fsBucketCount"      => $_POST["fsBucketCount"],
	            "mmapSize"           => $_POST["mmapSize"],
	            "raStrategy"         => $_POST["raStrategy"],
	            "raChunkCnt"         => $_POST["raChunkCnt"],
	            "dplReadMode"        => $_POST["dplReadMode"],
	            "delayedCloseTime"   => $_POST["delayedCloseTime"],
	            "maxDelayedCloseNum" => $_POST["maxDelayedCloseNum"],*/
	        );
            if (!$this->exInfo(CMD_MP_SET, $send, $recv)) {          
                $errorMark = "error";
            }
        } else {
            $firstDb = $dataPath[0];
            $dbPath  = array_shift($dataPath);
            $send = array(
                "firstDataPath"    => $firstDb,
                "allOtherDataPath" => $dataPath,
            );
            if (!$this->exInfo(0x000400D0, $send, $recv)) {          
                $errorMark = "error";
            }
        }
        //echo "<pre>";print_r($send); 
        if ($errorMark == "error") {
            $this->writeLog("DDFS","编辑消冗挂载点失败".$recv["info"],"error");
            $this->ajaxReturn("/Ddfs/index", $recv["info"], 0);
        }
        $this->writeLog("DDFS","编辑消冗挂载点成功","info");
        $this->ajaxReturn("/Ddfs/index", "编辑消冗挂载点成功!", 1);
    }

    //==================================================================
    // 功能：挂载点详细信息
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function infoMountpoint()
    {
        $recv = array();
        $send = array("id" => $_POST["hideId"]);
        if (!$this->exInfo(CMD_MP_DETAIL, $send, $recv)) {
            $this->ajaxReturn("", $recv["info"], 0);
        }

    }

    //==================================================================
    // 功能：删除挂载点
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function delMountpoint()
    {
        $recv   = array();
        $str    = $_POST["delconfig"];
        $config = explode(";",substr($str,0,strlen($str)-1));
        $mount  = $_POST["delmount_point"];
        $system = $_POST["systemType"];
        $mnt    = explode(";",substr($mount,0,strlen($mount)-1));
        $errorMark = '';
        for ($i = 0; $i < count($mnt); $i++) {
            $mp  = $mnt[$i]; 
	        $mount_point_bias = $mp."/";
	        $pathTable = M();
	        $arcdest   = $pathTable->query("select archive_id, archive_state, dest_path from node_archive_state nas, archive_path_pair app where nas.archive_id = app.config_id;");
	        foreach ($arcdest as $arc) {
	            //如果找到
	            if ($arc['dest_path'] == $mp || substr($arc['dest_path'],0,strlen($mount_point_bias)) == $mount_point_bias) {
	                $this->writeLog("DDFS","删除消冗挂载点失败,请先删除归档任务","error");
	                $this->ajaxReturn("/Ddfs/index", "删除消冗挂载点失败,请先删除归档任务", 0);
	            }
	        }
        }
        if ($system == 'ddfs') {
            $send = array(
                "configPath" => $config,
            );
	        if (!$this->exInfo(CMD_MP_REMOVE, $send, $recv)) {
	            $errorMark = 'error';
	        }
        } else {
            $send = array(
                "firstDataPath" => $config,
            );
            if (!$this->exInfo(0x000400C0, $send, $recv)) {
                $errorMark = 'error';
            }
        }
        if ($errorMark == 'error') {
            $this->writeLog("DDFS","删除消冗挂载点失败".$recv["info"],"error");
            $this->ajaxReturn("/Ddfs/index", $recv["info"], 0);
        }
        $this->writeLog("DDFS","删除消冗挂载点成功","info");
        $this->ajaxReturn("/Ddfs/index", "删除消冗挂载点成功!", 1);
    }

    //==================================================================
    // 功能：挂载
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function mount()
    {
        $recv = array();
        $systemType = $_REQUEST["systemType"];
        $mp         = $_REQUEST["mount_point"];
        $mount_point_bias = $mp."/";
        $pathTable = M();
        $errorMark = '';
        $arcdest   = $pathTable->query("select archive_id, archive_state, dest_path from node_archive_state nas, archive_path_pair app where nas.archive_id = app.config_id;");
        foreach ($arcdest as $arc) {
            //如果找到
            //if (strpos($arc['dest_path'],$mp) == 0 && $arc['archive_state'] != 'stop') {
            if (($arc['dest_path'] == $mp || substr($arc['dest_path'],0,strlen($mount_point_bias)) == $mount_point_bias) && $arc['archive_state'] != 'stop') {
                $this->writeLog("DDFS","挂载消冗挂载点失败,归档服务未停止","error");
                $this->ajaxReturn("/Ddfs/index", "挂载消冗挂载点失败,归档服务未停止", 0);
            }
        }
        if ($systemType == 'ddfs') {
            $send = array(
	            "mountPoint" => $_REQUEST["mount_point"],
	            "configPath" => $_REQUEST["mountconfig"],
	        ); 
            if (!$this->exInfo(CMD_MP_MOUNT, $send, $recv)) {
                $errorMark = 'error';
            }
        } else {
            $send = array(
                "firstDataPath" => $_REQUEST["firstDataPath"],
            );
            if (!$this->exInfo(0x000400E0, $send, $recv)) {
                $errorMark = 'error';
            }
        }
        
        if ($errorMark == 'error') {
            $this->writeLog("DDFS","挂载消冗挂载点失败".$recv["info"],"error");
            $this->ajaxReturn("/Ddfs/index", $recv["info"], 0);
        }
        $this->writeLog("DDFS","挂载消冗挂载点成功","info");
        $this->ajaxReturn("/Ddfs/index", "挂载消冗挂载点成功!", 1);
    }

    //==================================================================
    // 功能：卸载
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function umount()
    {
        $recv = array();
        $systemType = $_REQUEST["systemType"];
        
        //处理挂载点路径，如果最后一个字符不是/ 则在字符串后追加/     
        $mp = $send["mountPoint"];
        $mount_point_bias = $mp."/";
        $pathTable = M();
        $errorMark = '';
        $arcdest   = $pathTable->query("select archive_id, archive_state, dest_path from node_archive_state nas, archive_path_pair app where nas.archive_id = app.config_id;");
        foreach ($arcdest as $arc) {
        	//如果找到
            if (($arc['dest_path'] == $mp || substr($arc['dest_path'],0,strlen($mount_point_bias)) == $mount_point_bias) && $arc['archive_state'] != 'stop') {
                $this->writeLog("DDFS","卸载消冗挂载点失败,归档服务未停止","error");
                $this->ajaxReturn("/Ddfs/index", "卸载消冗挂载点失败,归档服务未停止", 0);
            }
        }
        $dest_path = $pathTable->query("select config_id,dest_path from archive_path_pair where instr(dest_path, '{$mount_point_bias}') = 1 or dest_path = '{$mp}'");
        
        if ($systemType == 'ddfs') {
            $send = array(
	            "mountPoint" => $_REQUEST["umount_point"],
	            "configPath" => $_REQUEST["umountconfig"],
	        ); 
            if (!$this->exInfo(CMD_MP_UMOUNT, $send, $recv)) {
                $errorMark = 'error';
            }
        } else {
            $send = array(
                "firstDataPath" => $_REQUEST["firstDataPath"],
            );
            if (!$this->exInfo(0x000400F0, $send, $recv)) {
                $errorMark = 'error';
            }
        }
        if ($errorMark == 'error') {
            $this->writeLog("DDFS","卸载消冗挂载点失败".$recv["info"],"error");
            $this->ajaxReturn("/Ddfs/index", $recv["info"], 0);
        } else  {
            $this->writeLog("DDFS","卸载消冗挂载点成功","info");
            $this->ajaxReturn("/Ddfs/index", "卸载消冗挂载点成功!", 1);
        }
    }

    //==================================================================
    // 功能：修复
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function fix()
    {
        $recv = array();
        $send = array(
            "mountPoint" => $_REQUEST["fix_point"],
            "configPath" => $_REQUEST["fixconfig"],
        );
        if (!$this->exInfo(CMD_MP_FIX, $send, $recv)) {
        	$this->writeLog("DDFS","修复消冗挂载点失败".$recv["info"],"error");
            $this->ajaxReturn("/Ddfs/index", $recv["info"], 0);
        }
        $this->writeLog("DDFS","修复消冗挂载点成功","info");
        $this->ajaxReturn("/Ddfs/index", "修复消冗挂载点成功!", 1);
    }
}
