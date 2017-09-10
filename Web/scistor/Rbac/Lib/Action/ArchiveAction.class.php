<?php
class ArchiveAction extends CommonAction
{
	public $pageSize = 11;
    public function index()
    {
    	$this->unsetSig();   //删除保存在session里的内容
        // 页面下拉框初始化
        $time_hour     = array("00","01","02","03","04","05","06", 
                               "07","08","09","10","11","12","13",
                               "14","15","16","17","18","19","20",
                               "21","22","23",);
        $time_minute   = array("00","10","20","30","40","50");
        $time_duration = array("1","2","3","4","5","6","7","8","9",
                               "10","11","12","13","14","15","16",
                               "17","18","19","20","21","22","23",);
        $interval_time = array("1","2","3","4","5","6","7","8","9");
        $this->assign("time_hour",$time_hour);
        $this->assign("time_minute",$time_minute);
        $this->assign("time_duration",$time_duration);
        $this->assign("interval_time",$interval_time);

        // 查询数据库归档任务记录，记录所有任务的ID
        $index = array();
        $archiveTable = M();
        $data = $archiveTable->query("select * from archive order by id");

        foreach (array_keys($data) as $k) {
            $index[] = (int)$data[$k]["id"];
        }
        // 将ID传给服务器，获取对应的归档任务的运行状态，记录到数组 $data 中
        $recv = array();
        $send = array("configId" => $index);
        if (!$this->exInfo(CMD_ARCHIVE_LOOKUP, $send, $recv)) {
            $this->error("获取归档信息失败: ".$recv["info"]);
        }
        foreach (array_keys($recv) as $k) {
            for ($i = 0; $i < count($data); $i++) {
                if ((int)$data[$i]["id"] == $k) {
                	$startTime                = explode(":",$recv[$k]["startTime"]);
                    $data[$i]["state"]        = $recv[$k]["state"];
                    $data[$i]["start_hour"]   = $startTime[0];
                    $data[$i]["start_minute"] = $startTime[1];
                    $data[$i]["duration"]     = $recv[$k]["duration"];
                    $data[$i]["dataTime"]     = $recv[$k]["dataTime"];
                    $data[$i]["intervalTime"] = $recv[$k]["intervalTime"];
                    $data[$i]["check"]        = $recv[$k]["check"];
                    $data[$i]["deleteSource"] = $recv[$k]["deleteSource"];
                    $data[$i]["path"]         = $archiveTable->query("select * from archive_path_pair where config_id = {$k} order by src_path");
                    $data[$i]["nodes"]        = $recv[$k]["nodes"];
                    $data[$i]["prefix"]       = $recv[$k]["prefix"];
                    $data[$i]["prefixType"]   = $recv[$k]["prefixType"];
                    $data[$i]["postfix"]      = $recv[$k]["postfix"];
                    $data[$i]["postfixType"]  = $recv[$k]["postfixType"];
                    $data[$i]["sizeType"]     = $recv[$k]["sizeType"];
                    $data[$i]["maxSize"]      = $this->formatN($recv[$k]["maxSize"]);
                    $data[$i]["minSize"]      = $this->formatN($recv[$k]["minSize"]);
                    $data[$i]["dir"]          = $recv[$k]["dir"];
                    $data[$i]["dirType"]      = $recv[$k]["dirType"];
                    $data[$i]["isModify"]     = $recv[$k]["isModify"];
                    $data[$i]["delSig"]       = 0;
                    $data[$i]["stopSig"] = $data[$i]["startSig"] = 1;
	                for ($j = 0; $j < count($recv[$k]["nodes"]); $j++) {
	                	if ($recv[$k]["nodes"][$j]['state'] != 'stop') {
	                		$data[$i]["delSig"] = 1;
	                	}
	                	if ($recv[$k]["nodes"][$j]['state'] != 'idle') {
	                	    $data[$i]["stopSig"] = 0;
	                	}
	                    if ($recv[$k]["nodes"][$j]['state'] != 'stop') {
                            $data[$i]["startSig"] = 0;
                        } 
	                }
                }
            }
        }
        /** 获取IP节点列表*/
        $ipDate    = $archiveTable->query("select a.ip aip,b.ip bip from fbs_index_node a, fbs_data_node b where a.id = b.indexid");
        $countnode = $archiveTable->query("select count(*) from fbs_index_node");
        $countdata = $archiveTable->query("select count(*) from fbs_data_node");
        $newArr = array();
        $newData = array();
        for ($i = 0; $i < count($ipDate); $i++) {
            $newArr[$ipDate[$i]['aip']][] = $ipDate[$i]['bip']; 
        }
        $arrKey = array_keys($newArr);
        for ($i = 0; $i < count($newArr); $i++) {
            $newData[$i]['index'] = $arrKey[$i];
            $newData[$i]['data'] = $newArr[$arrKey[$i]];
        }
        //echo "<pre>";print_r($recv);
        //dump($newData);
        $this->assign("ign",$ign);
        $this->assign("countnode",$countnode[0]["count(*)"]);
        $this->assign("countdata",$countdata[0]["count(*)"]);
        $this->assign("newData",$newData);
        $this->assign("archive_data",$data);
        $this->display();
    }
    //==================================================================
    // 功能：单位换算
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    function formatN($size) {
        $dw = array('byte','KB','MB', 'GB', 'TB' , 'PB');
        for ($i = 0; $size >= 1024 && $i < 5; $i++){ 
            $size /= 1024;
        }
        return $size." ".$dw[$i];
    }
    //==================================================================
    // 功能： 从MB等单位换算到字节
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    function formatNumber($unit,$size)
    {
        if ($unit == 'BT') {
           $size = $size;
        } elseif ($unit == 'KB') {
           $size *= 1024;
        } elseif ($unit == 'MB') {
           $size *= 1048576;
        } elseif ($unit == 'GB') {
           $size *= 1073741824;
        }
        return $size;
    }
    //==================================================================
    // 功能： 新建归档
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function createArchive()
    {
    	$archiveSrcdir  = $_POST["archiveSrcdir"];
    	$archiveDestdir = $_POST["archiveDestdir"];
    	$arvname        = $_POST["archive_name"];
    	$maxSize        = "".$this->formatNumber($_POST["maxSizeUnit"],$_POST["maxSize"]);
    	$minSize        = "".$this->formatNumber($_POST["minSizeUnit"],$_POST["minSize"]);
    	for ($i = 0;$i < count($archiveSrcdir);$i++) {
    	   $archiveSrcdir[$i] = $this->handPath($archiveSrcdir[$i]);
    	}
        for ($i = 0;$i < count($archiveDestdir);$i++) {
           $archiveDestdir[$i] = $this->handPath($archiveDestdir[$i]);
        }
    	
    	$recv = array();
    	$send = array(
            "name"          => $arvname,
            "startTime"     => $_POST["starttime_hour"].":".$_POST["starttime_minute"],
            "duration"      => (int)$_POST["stoptime_hour"],
            "intervalTime"  => (int)$_POST["intervaltime"],
            "dataTime"      => (int)$_POST["modifyTime"],
            "check"         => ($_POST["newcheck"] == "1")? 1: 0,
            "deleteSource"  => ($_POST["deleteSource"] == "1")? 1: 0,
            "src"           => $archiveSrcdir,
            "dest"          => $archiveDestdir,
            "ip"            => C('AGENT_ADDRESS'),
            "nodes"         => $_POST["archiveNodes"],
    	    "dir"           => $_POST["dir"],
    	    "dirType"       => $_POST["dirType"] == '' ? 0 : (int)$_POST["dirType"],
    	    "maxSize"       => $maxSize,
    	    "minSize"       => $minSize,
    	    "sizeType"      => $_POST["sizeType"] == ''  ? 0 : (int)$_POST["sizeType"],
    	    "postfix"       => $_POST["postfix"],
    	    "postfixType"   => $_POST["postfixType"] == '' ? 0 : (int)$_POST["postfixType"],
    	    "prefix"        => $_POST["prefix"],
    	    "prefixType"    => $_POST["prefixType"] == ''  ? 0 : (int)$_POST["prefixType"],
        );
        $ret = $this->exInfo(CMD_ARCHIVE_CREATE, $send, $recv); 
        if (!$ret) {
        	$this->writeLog("ARCHIVE","新建归档任务".$arvname."失败".$recv["info"],"error");
            $this->ajaxReturn("/Archive/index", $recv["info"], 0);
        }
        $this->writeLog("ARCHIVE","新建归档任务".$arvname."成功","info");
        $this->ajaxReturn("/Archive/index", "新建归档任务成功!", 1);
    }

    //==================================================================
    // 功能： 编辑归档任务
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function editArchive()
    {
        $edit_archiveSrcdir  = $_POST["edit_archiveSrcdir"];
        $edit_archiveDestdir = $_POST["edit_archiveDestdir"];
        $maxSize        = "".$this->formatNumber($_POST["maxSizeUnit"],$_POST["maxSize"]);
        $minSize        = "".$this->formatNumber($_POST["minSizeUnit"],$_POST["minSize"]);
        for ($i = 0;$i < count($edit_archiveSrcdir);$i++)
        {
           $edit_archiveSrcdir[$i] = $this->handPath($edit_archiveSrcdir[$i]);
        }
        for ($i = 0;$i < count($edit_archiveDestdir);$i++)
        {
           $edit_archiveDestdir[$i] = $this->handPath($edit_archiveDestdir[$i]);
        }
        $nodes   = $_POST["archiveNodes"];
        $arvname = $_POST["doc_name"];
        $send = array(
            "configId"      => (int)$_POST["hideId"],
            "name"          => $arvname,
            "startTime"     => $_POST["starttime_hour"].":".$_POST["starttime_minute"],
            "duration"      => (int)$_POST["stoptime_hour"],
            "intervalTime"  => (int)$_POST["intervaltime"],
            "dataTime"      => (int)$_POST["modifyTime"],
            "check"         => ($_POST["eidtcheck"] == "1")? 1: 0,
            "deleteSource"  => ($_POST["editSource"] == "1")? 1: 0,
            "src"           => $edit_archiveSrcdir,
            "dest"          => $edit_archiveDestdir,
            "ip"            => C('AGENT_ADDRESS'),
            "nodes"         => $_POST["archiveNodes"],
            "dir"           => $_POST["dir"],
            "dirType"       => $_POST["dirType"] == '' ? 0 : (int)$_POST["dirType"],
            "maxSize"       => $maxSize,
            "minSize"       => $minSize,
            "sizeType"      => $_POST["sizeType"] == ''  ? 0 : (int)$_POST["sizeType"],
            "postfix"       => $_POST["postfix"],
            "postfixType"   => $_POST["postfixType"] == '' ? 0 : (int)$_POST["postfixType"],
            "prefix"        => $_POST["prefix"],
            "prefixType"    => $_POST["prefixType"] == ''  ? 0 : (int)$_POST["prefixType"],
        );
        if (!$this->exInfo(CMD_ARCHIVE_SET, $send, $recv)) {
        	$this->writeLog("ARCHIVE","编辑归档任务".$arvname."失败".$recv["info"],"error");
            $this->ajaxReturn("/Archive/index", $recv["info"], 0);
        }
        $this->writeLog("ARCHIVE","编辑归档任务".$arvname."成功","info");
        $this->ajaxReturn("/Archive/index", "编辑归档任务成功!", 1);
    }


    //==================================================================
    // 功能： 删除归档任务
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function delArchive()
    {
        /* 获取归档ID */
        $recv = array();
        $str  = $_POST["delhidId"];
        $name = $_POST["arv_name"];
        $id      = explode(";",substr($str,0,strlen($str)-1));
        $arvname = substr($name,0,strlen($name) - 1); 
        for ($i = 0; $i < count($id); $i++)
        {
            $id[$i] = (int)$id[$i];
        }
        
        $send = array(
            "configId" => $id,
        );
        if (!$this->exInfo(CMD_ARCHIVE_REMOVE, $send, $recv)) {
        	$this->writeLog("ARCHIVE","删除归档任务".$arvname."失败".$recv["info"],"error");
            $this->ajaxReturn("/Archive/index", $recv["info"], 0);
        }
        $this->writeLog("ARCHIVE","删除归档任务".$arvname."成功","info");
        $this->ajaxReturn("/Archive/index", "删除归档服务成功!", 1);
    }

    //==================================================================
    // 功能： 启动归档服务
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function startService()
    {
        $recv     = array();
    	$nodes    = $_REQUEST["nodes"];
    	$arvname  = $_REQUEST['arvname'];
    	$nodesArr = split(",",$nodes);
    	$startId  = $_REQUEST["startId"];
    	$send = array (
    	   "configId"       => $startId,
           "intervalTime"   => $_REQUEST["startInterval"],
           "startTime"      => $_REQUEST["startTime"],
    	);
    	$m = M();
        $errInfo = '';
        $errorIgn = 0;
        $dataid  = 0;
    	for ($i = 0; $i < count($nodesArr); $i++) {
	    	$send["agentIp"] = $nodesArr[$i];
	    	//单机启动归档
	    	if ($nodesArr[$i] == '127.0.0.1') {
	    	    $dataid = -1;
	    	    if (!$this->exInfo(CMD_ARCHIVE_SERVICE_START, $send, $recv)) {
                    $errorIgn    = 1;
                    $errorInfo[] = $nodesArr[$i].":".$recv['info'];
                    $m->query("update node_archive_state set is_error = 1 where data_node_id = {$dataid} and archive_id = {$startId}");
                } else {
                    $m->query("update node_archive_state set is_error = 0 where data_node_id = {$dataid} and archive_id = {$startId}");
                }
	    	} else {
		    	$id = $m->query("select id from fbs_data_node where ip = '$nodesArr[$i]'");
		    	$dataid = (int)$id[0]["id"];
		    	//分布式启动归档
	            $dataState = $m->query("select state from fbs_data_node where ip = '$nodesArr[$i]'");
	            if ($dataState[0]['state'] != 'running') {
	                $errorInfo[] = "启动".$nodesArr[$i]."归档失败,节点服务未启动";
	                $errorIgn    = 1;
	            } else {
	                if (!$this->exInfo(CMD_ARCHIVE_SERVICE_START, $send, $recv)) {
	                    $errorIgn    = 1;
	                    $errorInfo[] = $nodesArr[$i].":".$recv['info'];
	                    $m->query("update node_archive_state set is_error = 1 where data_node_id = {$dataid} and archive_id = {$startId}");
	                } else {
	                    $m->query("update node_archive_state set is_error = 0 where data_node_id = {$dataid} and archive_id = {$startId}");
	                }
	            }
	    	}
    	}
    	$info = implode(";",$errorInfo);
    	$list = implode(";",$nodesArr);
        if ($errorIgn == 0) {
            $this->writeLog("ARCHIVE","启动归档任务".$arvname."成功({$list})","info");
            $this->ajaxReturn("/Archive/index", "启动归档成功!", 1);
        } 
        $this->writeLog("ARCHIVE","启动归档任务".$arvname."失败({$list})".$info,"error");
        $this->ajaxReturn("/Archive/index", $info, 0);
    }

    //==================================================================
    // 功能： 停止归档服务
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function stopService()
    {
        $recv     = array();
    	$nodes    = $_REQUEST["nodes"];
    	$arvname  = $_REQUEST['arvname'];
        $nodesArr = split(",",$nodes);
        $stopId   = $_REQUEST["stopId"];
        $send = array (
           "configId"       => $stopId,
           "intervalTime"   => $_REQUEST["stopInterval"],
           "startTime"      => $_REQUEST["stopTime"],
        );
        $m = M();
        //$errInfo = '';
        $dataid  = 0;
        for ($i = 0; $i < count($nodesArr); $i++) {
            $send["agentIp"] = $nodesArr[$i];
            if ($nodesArr[$i] == '127.0.0.1') {
                $dataid = -1;
            } else {
	            $id = $m->query("select id from fbs_data_node where ip = '$nodesArr[$i]'");
	            $dataid = (int)$id[0]["id"];
            }
            if (!$this->exInfo(CMD_ARCHIVE_SERVICE_STOP, $send, $recv)) {
                $errorInfo[] = $nodesArr[$i] .":".$recv['info'];
                $m->query("update node_archive_state set is_error=1 where data_node_id = {$dataid} and archive_id = {$stopId}");
            } else {
                $m->query("update node_archive_state set is_error=0 where data_node_id = {$dataid} and archive_id = {$stopId}");
            }
        }
        $info = implode(";",$errorInfo);
        $list = implode(";",$nodesArr);
        if ($recv["info"] == "") {
            $this->writeLog("ARCHIVE","停止归档任务".$arvname."成功({$list})","info");
            $this->ajaxReturn("/Archive/index", "停止归档成功!", 1);
        } 
        $this->writeLog("ARCHIVE","停止归档任务".$arvname."失败({$list})".$info,"error");
        $this->ajaxReturn("/Archive/index", $info, 0);
    }

    //==================================================================
    // 功能： 获取目录
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function getDir()
    {
        $path = $_REQUEST["path"];
        $strdata = array(
          'path' => $path,
        );
        $user = 1;
        //$jsoninfo = $this->exchangeInfo(0x91010001 , $strdata , $user);
        //echo json_encode($jsoninfo);
    }
    
    public function archiveLog()
    {
        $this->display();
    }
    public function templates()
    {
        $currentPage = isset($_GET['page'])?$_GET['page']:1;
        /*获取内容*/
        $log         = M("archive_log");
        $startDate   = $_REQUEST['startDate'];
        $endDate     = $_REQUEST['endDate'];
        $statuslink  = $_REQUEST['statuslink']? $_REQUEST['statuslink'] : "link";
        $statuserror = $_REQUEST['statuserror']? $_REQUEST['statuserror'] : "error";
        //级别分开
        if($startDate == '') {
            $startDate = '2000-01-01 00:00:00';
            $endDate   = '2150-01-01 00:00:00';
        }else{
            $startDate = $_REQUEST['startDate'].' 00:00:00';
            $endDate   = $_REQUEST['endDate'].' 23:59:59';
        }
        
        $_SESSION['b_date']      = $startDate;
        $_SESSION['e_date']      = $endDate;
        $_SESSION['statuserror'] = $statuserror;
        $_SESSION['statuslink']  = $statuslink;
        /*获取内容结束*/
        //echo "startDate:".$_COOKIE['b_date']."**endDate:".$_COOKIE['e_date']."***module:".$_COOKIE['module']."**type:".$type."**keyword:".$_COOKIE['keyword']."**levelinfo:".$_COOKIE['levelinfo']."**levelwarning:".$_COOKIE['levelwarning']."**levelerror:".$_COOKIE['levelerror'];
        $total   = $log->query("select count(*) from archive_log where (state = '{$statuserror}' or state = '{$statuslink}') and (arvtime >= '{$startDate}' and arvtime <= '{$endDate}')");
        $first = ($currentPage - 1) * $this->pageSize;
        $pageSize = $this->pageSize;
        $content = $log->query("select * from archive_log where (state = '{$statuserror}' or state = '{$statuslink}') and (arvtime >= '{$startDate}' and arvtime <= '{$endDate}') order by arvtime desc limit {$first},{$pageSize}");
        //总页数
        $totalPage = ceil($total[0]["count(*)"]/$this->pageSize);
        if ($currentPage >= $totalPage){
            $currentPage = $totalPage;
        }
        if ($total[0]["count(*)"] == 0) {
            $currentPage = 1;
            $totalPage = 1;
        }
        for ($i = 0; $i < count($content); $i++) {
            $content[$i]["size"] = $this->format_bytes($content[$i]["size"]);
        }
        $this->assign('currentPage',$currentPage);
        $this->assign('totalPage',$totalPage);
        $this->assign('dateInfo',$content);
        $this->display();
    }
    //==================================================================
    // 功能：归档日志导出
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function archdown_log()
    {
        Header("Content-type:application/vnd.ms-excel");    
        Header("Content-Disposition:attachment;filename=log_".date("Ymd").".csv");    
    
        $model = M();
        
        $startDate    = $_SESSION['b_date'];
        $endDate      = $_SESSION['e_date'];
        $statuserror  = $_SESSION['statuserror'];
        $statuslink   = $_SESSION['statuslink'];
        
        //查询结果后 数据导出
        $ReportArr = $model->query("select * from archive_log where (state = '{$statuserror}' or state = '{$statuslink}') and (arvtime >= '{$startDate}' and arvtime <= '{$endDate}') order by arvtime desc ");
        for ($i = 0; $i < count($ReportArr); $i++) {
            $ReportArr[$i]["size"] = $this->format_bytes($ReportArr[$i]["size"]);
        }
        $len = count($ReportArr);
        $texcel = "编号,任务ID,任务名称,IP,时间,归档源,归档目标,文件大小,状态\n";
        
        for ($i = 0; $i < $len; $i++) {
            $texcel .= ($i+1).",";
            $texcel .= $ReportArr[$i]['config_id'].",";
            $texcel .= $ReportArr[$i]['arvname'].",";
            $texcel .= $ReportArr[$i]['ip'].",";
            $texcel .= $ReportArr[$i]['arvtime'].",";
            $texcel .= $ReportArr[$i]['src'].",";
            $texcel .= $ReportArr[$i]['dest'].",";
            $texcel .= $ReportArr[$i]['size'].",";
            if ($ReportArr[$i]['state'] == "link") {
                $texcel .= "成功,";
            } else {
                $texcel .= "失败,";
            }
            $texcel .= "\n";
        }
        //用的utf-8 最后转换一个编码为gb
        $texcel = iconv("utf-8","gb2312",$texcel);
       
        echo $texcel;
    }
    
}
