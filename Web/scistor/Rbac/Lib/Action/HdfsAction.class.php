<?php
class HdfsAction extends CommonAction
{
	private function reset()
	{
	   unset($_SESSION["IP"]);
	}
    //==================================================================
    //功能：分布式集群概况页面
    //作者：ljwang
    //e-mail：wwpu@scidata.com
    //==================================================================
    public function summary()
    {
    	$this->unsetSig();   //删除保存在session里的内容
    	$index     = M();
    	/**消冗率的计算 **/
    	$dataSum = $index->query("select sum(total),sum(`real`) from fbs_data_node");
    	//$realInt  = $this->format_bytes($dataSum[0]["sum(`real`)"]);
    	$totalInt = $this->format_bytes($dataSum[0]["sum(total)"]);
    	$realInt  = $dataSum[0]["sum(`real`)"];
        $total    = $dataSum[0]["sum(total)"];
        $ratioClu = sprintf("%.2f",$total / $realInt);
        if ($total == 0) {
            $ratioClu = '0.00';
        } elseif ((int)$ratioClu > 9999) {
            $ratioClu = '9999.00';
        } else {
            $ratioClu = $ratioClu;
        }
    	
    	/**索引节点状态的计算 **/
    	$totalState = $index->query("select count(*) from fbs_index_node");
    	$runState   = $index->query("select count(*) from fbs_index_node where state = 'running'");
    	$errorState = $index->query("select count(*) from fbs_index_node where state = 'error'");
    	$stopState  = $index->query("select count(*) from fbs_index_node where state = 'stopped'");
    	$lostState  = $index->query("select count(*) from fbs_index_node where state = 'lost'");
    	$readyState = $index->query("select count(*) from fbs_index_node where state = 'ready'");
    	$allI       = $totalState[0]["count(*)"];
    	$readyI     = $readyState[0]["count(*)"];
    	$runI       = $runState[0]["count(*)"];
    	$errorI     = $errorState[0]["count(*)"];
    	$stopI      = $stopState[0]["count(*)"];
    	$lostI      = $lostState[0]["count(*)"];
    	
    	
    	/**数据节点状态的计算 **/
    	$datatotal = $index->query("select count(*) from fbs_data_node");
        $datarun   = $index->query("select count(*) from fbs_data_node where state = 'running'");
        $dataerror = $index->query("select count(*) from fbs_data_node where state = 'error'");
        $datastop  = $index->query("select count(*) from fbs_data_node where state = 'stopped'");
        $datalost  = $index->query("select count(*) from fbs_data_node where state = 'lost'");
        $dataready = $index->query("select count(*) from fbs_data_node where state = 'ready'");
    	$allD      = $datatotal[0]["count(*)"];
    	$readyD    = $dataready[0]["count(*)"];
        $runD      = $datarun[0]["count(*)"];
        $errorD    = $dataerror[0]["count(*)"];
        $stopD     = $datastop[0]["count(*)"];
        $lostD     = $datalost[0]["count(*)"];
    	//dump($allD);dump($runD);dump($errorD);dump($stopD);dump($lostD);
        
        $this->assign("allI",$allI);
        $this->assign("readyI",$readyI);
    	$this->assign("runI",$runI);
    	$this->assign("errorI",$errorI);
    	$this->assign("stopI",$stopI);
    	$this->assign("lostI",$lostI);
    	$this->assign("allD",$allD);
    	$this->assign("readyD",$readyD);
    	$this->assign("runD",$runD);
    	$this->assign("errorD",$errorD);
    	$this->assign("stopD",$stopD);
    	$this->assign("lostD",$lostD);
    	$this->assign("ratio",$ratioClu);
    	$this->assign("realInt",$realInt);
    	$this->assign("totalInt",$totalInt);
        $this->display();
    }
	//==================================================================
    // 功能： 分布式配置管理页面
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function index()
    {
    	unset($_SESSION["data"]);
    	$this->unsetSig();   //删除保存在session里的内容
    	$index    = M();
        $id       = 0;
        $state    = '';
    	$indexname = $_REQUEST["indexname"];
    	
        $indexData  = $index->query("select * from fbs_index_node order by id");
        for ($i = 0; $i < count($indexData); $i++) {
            $indexArr[] = $indexData[$i]["ip"];
        }
        for ($i = 0; $i < count($indexData); $i++) {
           if ($indexData[$i]["ip"] == $indexname) {
                $id = $indexData[$i]["id"];
                $_SESSION["state"] = $indexData[$i]["state"];
                break;
           } else {
                $id = $indexData[0]["id"];
           }
        }
        $dataArr    = $index->query("select * from fbs_data_node where indexid = {$id}");
        $state      = $index->query("select state from fbs_index_node where id = {$id}");
        $stateArr[] = $state[0]["state"];
        $_SESSION["indexId"] = $id;
        //把索引节点下的数据节点相应的数据求和，修改索引节点的数据
        $dataSum = $index->query("select sum(`capacity`),sum(`used`),sum(`left`),sum(total),sum(`real`) from fbs_data_node where indexid = {$id}");
        //dump($dataSum);
        $capacity  = $dataSum[0]["sum(`capacity`)"];
        $used      = $dataSum[0]["sum(`used`)"];
        $left      = $dataSum[0]["sum(`left`)"];
        $total     = $dataSum[0]["sum(total)"];
        $real      = $dataSum[0]["sum(`real`)"];
        $ratio     = sprintf("%.2f",$total / $real);
        if (count($dataArr) < 1) { 
            $index->query("update fbs_index_node set capacity2 = 0, used2 = 0, left2 = 0,total = 0,`real` = 0, ratio = 1.00 where id = {$id}");
        } else {
            //dump($capacity);dump($used);dump($left);dump($total);dump($real);dump($ratio);
            $ret = $index->query("update fbs_index_node set capacity2 = {$capacity}, used2 = {$used}, left2 = {$left},total = {$total},`real` = {$real}, ratio = {$ratio} where id = {$id}");
        }
        //在页面上显示集群的数据
        $new = $index->query("select * from fbs_index_node where id = {$id}");
        $countIndex = $index->query("select count(*) from fbs_index_node");
        $newIndex = $new[0];
        $newRatio = $newIndex['total'] / $newIndex['real'];
        //换算单位
        $newIndex["capacity1"] = $this->format_bytes($newIndex["capacity1"]);
        $newIndex["used1"]     = $this->format_bytes($newIndex["used1"]);
        $newIndex["left1"]     = $this->format_bytes($newIndex["left1"]);
        $newIndex["capacity2"] = $this->format_bytes($newIndex["capacity2"]);
        $newIndex["used2"]     = $this->format_bytes($newIndex["used2"]);
        $newIndex["left2"]     = $this->format_bytes($newIndex["left2"]);
        $newIndex["total"]     = $this->format_bytes($newIndex["total"]);
        $newIndex["real"]      = $this->format_bytes($newIndex["real"]);
        if ($new[0]["total"] == 0) {
            $newIndex["ratio"] = '0.00';
        } elseif ($newRatio > 9999) {
        	$newIndex["ratio"] = '9999.00';
        } else {
            $newIndex["ratio"] = sprintf("%.2f",$newRatio);
        }
        for ($i = 0; $i < count($dataArr); $i++) {
        	$dataRatio = $dataArr[$i]["total"] / $dataArr[$i]["real"];
            if ($dataArr[$i]["total"] == 0) {
               $dataArr[$i]["ratio"] = '0.00';
            } elseif ($dataRatio >= 9999) {
               $dataArr[$i]["ratio"] = '9999.00';
            } else {
               $dataArr[$i]["ratio"] = sprintf("%.2f",$dataRatio);
            }
            $dataArr[$i]["capacity"] = $this->format_bytes($dataArr[$i]["capacity"]);
            $dataArr[$i]["used"]     = $this->format_bytes($dataArr[$i]["used"]);
            $dataArr[$i]["left"]     = $this->format_bytes($dataArr[$i]["left"]);
            $dataArr[$i]["total"]    = $this->format_bytes($dataArr[$i]["total"]);
            $dataArr[$i]["real"]     = $this->format_bytes($dataArr[$i]["real"]);
	        //把数据节点的状态循环放入stateArr数组中
	        $stateArr[] = $dataArr[$i]["state"];
        }
        $ign = 1; 
        for ($i = 0; $i < count($stateArr); $i++) {
            if ($stateArr[$i] != 'ready') {
                $ign = 0;                   //不是所有节点的状态都为ready时，标识为0
            } 
        }
        
        //查询归档id，节点是否可删除
        $m = M();
        $dataId = $m->query("select data_node_id from node_archive_state");
        for ($i = 0; $i < count($dataId); $i ++) {
        	$nodeIp      = $m->query("select ip from fbs_data_node where id = {$dataId[$i]['data_node_id']}");
            $nodearrIp[] = $nodeIp[0]['ip'];
        }
        $archive = $m->query("select count(*) from archive");
        //查找挂载点表中是否存在数据,如果存在则不能添加分布式索引
        $ddfs = $index->query("select * from ddfsmp");
        $adfs = $index->query("select * from adfsmp");
        if (count($ddfs) > 0) {
            $dataNum = count($ddfs);
        } elseif (count($adfs) > 0) {
            $dataNum = count($adfs);
        }
        $this->assign("nodearrIp",$nodearrIp);
        $this->assign("ign",$ign);
        $this->assign("newIndex",$newIndex);
        $this->assign("countIndex",count($indexData));
        $this->assign("indexId",$id);
        $this->assign("dataCount",count($dataArr));
    	$this->assign("dataArr",$dataArr);
    	$this->assign("indexArr",$indexArr);
    	$this->assign("indexName",$indexname);
    	$this->assign("ddfscount",$dataNum);
    	$this->assign("archive",$archive[0]['count(*)']);
        $this->display();
    }
    //==================================================================
    // 功能： 分布式添加节点页面
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function addNode()
    {
    	$m = M();
        $arrcount   = array();
    	$countIndex = $m->query("select count(*) from fbs_index_node");
    	$arrdata    = $m->query("select ip from fbs_data_node");
        $arrIndex   = $m->query("select ip from fbs_index_node");
        for ($i = 0; $i < count($arrIndex); $i++) {
            $arrcount[] = $arrIndex[$i]["ip"];
        }
        for ($i = 0; $i < count($arrdata); $i++) {
            $arrcount[] = $arrdata[$i]["ip"];
        }
        
        $data = $_SESSION["data"];
        $this->assign("serverAdd",$_SERVER["SERVER_ADDR"]);
    	$this->assign("data",$data);
    	$this->assign("arrcount",$arrcount);
    	$this->assign("countIndex",$countIndex[0]["count(*)"]);
        $this->display();
    }
    //==================================================================
    // 功能： 分布式连接测试页面
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function testAdd()
    {
    	$m    = M();
    	$data = $_REQUEST;
    	$password         = $data["password"] == '' ? 'noPassword' : $data["password"];
    	$_SESSION["data"] = $data;
    	$_SESSION["type"] = $data["newType"];
    	$_SESSION["way"]  = $data["newWay"];
    	$_SESSION["password"] = $password;
        if ($data["newType"] == "index") {
            $indexIp = $data["ipaddr"];
        } else {
        	if ($data["newWay"] == "single") {
        		$ipArr[]     = $data["ipaddr"];
        	} else {
	        	$startIp     = $data["strtIp"];
	            $endIp       = $data["endIp"];
	            
	            $half_q      = substr($startIp,0,strrpos($startIp,".") + 1);
	            $half_satrt  = substr($startIp,strrpos($startIp,".") + 1);
	            $half_end    = substr($endIp,strrpos($endIp,".") + 1);
	            for ($i = $half_satrt; $i <= $half_end; $i++) {
                    $ipArr[] = $half_q.$i; //所有的IP在一个数组中
                }
        	}
        }
        $_SESSION["idexIp"] = $indexIp;
        $_SESSION["iparr"]  = $ipArr;
        $m->query("truncate fbs_connection_test");
        if ($data["newType"] == "index") {
            $indexSend = array(
	            "connectIp" => $indexIp,
	            "password"  => $password,
	            "user"      => "root"
	        );
	        $indexSet = $this->exInfo(0x02080010, $indexSend, $indexrecv);
        } else {
            $dataSend = array(
                "connectIp" => $ipArr,
                "indexId"   => (string)$_SESSION["indexId"],
                "password"  => $password,
                "user"      => "root"
            );
            $dataSet = $this->exInfo(0x02080020, $dataSend, $datarecv);
        }
        
        $this->testData();
        $this->display();
    }
    //==================================================================
    // 功能： 页面刚打开时调用的函数
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function testData()
    {
        //查询数据库，显示测试结果
        $test = M("fbs_connection_test");
        $test_data = $test->select();
        $m   = M();
        $ign = 0;
        $indexIp = $_SESSION["idexIp"];
        $ipArr   = $_SESSION["iparr"];
        if ($_SESSION["type"] == "index") {
           $idxstate = $m->query("select state from fbs_connection_test where ip = '{$indexIp}'");
           if ($idxstate[0]["state"] == 'ok') {
               $ign = 1;
           }
        } else {
            for ($i = 0; $i < count($ipArr); $i++) {
                $datastate = $m->query("select state from fbs_connection_test where ip = '{$ipArr[$i]}'");
                if ($datastate[0]["state"] == 'ok') {
                    $ign = 1;                   //当所有的节点都测试失败时，安装按钮不可用 ，只要有ok的状态则可用
                } 
            }
        }
        $this->assign("ign",$ign);
        $this->assign("testlastData",$test_data[count($test_data) -1]["state"]);
        $this->assign("test_data",$test_data);
    }
    //==================================================================
    // 功能： 前台js循环调用的函数
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function displayData()
    {
    	$this->testData();
        $this->display("testAdd");
    }
    //==================================================================
    // 功能： 配置页面
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function configNode()
    {
    	$newType = $_SESSION["type"];
    	$this->assign("newType",$newType);
        $this->display();
    }
    //==================================================================
    // 功能： 分布式安装
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function installNode()
    {
        /*安装节点页面的信息*/
        //测试成功的ip
        $test = M();
        $test_ip = $test->query("select ip from fbs_connection_test where state = 'ok'");
        $test->query("truncate fbs_install");
        $recv = array();
        if ($_SESSION["type"] == "data") {
            for ($i = 0; $i < count($test_ip); $i++) {
                $ipArr[] = $test_ip[$i]["ip"];
            }
            $datasend = array(
               "indexId"   => (string)$_SESSION["indexId"],
               "installIp" => $ipArr,
               "password"  => $_SESSION["password"],
               "user"      => "root"
            );
            $dataret = $this->exInfo(0x02080040, $datasend, $datarecv);
            $list = implode(";",$ipArr); 
        } else {
        	$indexIp = $test_ip[0]["ip"];
            $indexsend = array(
               "installIp"    => $indexIp,
               "password"     => $_SESSION["password"],
               "user"         => "root"
            );
            $indexret = $this->exInfo(0x02080030, $indexsend, $indexrecv);
        }
        //dump($indexsend);dump($datasend);
        $this->reset();
        $this->installNodeData();
        $this->display();
    }
    
    //==================================================================
    // 功能： 分布式安装-config
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function installhh()
    {
    	/*交互配置信息*/
        $dataindexPath = $_REQUEST["dataindexPath"];
        $metaPath      = $_REQUEST["metaPath"];
        $biPath        = $_REQUEST["biPath"];
        $idxRootPath   = $_REQUEST["idxRootPath"];
        
        /*安装节点页面的信息*/
        //测试成功的ip
        $test = M();
        $test_ip = $test->query("select ip from fbs_connection_test where state = 'ok'");
        for ($i = 0; $i < count($test_ip); $i++) {
            $ipArr[] = $test_ip[$i]["ip"];
        }
        
        $recv = array();
        if ($_SESSION["type"] == "data") {
            $instalsend = array(
	           "biPath"    => $biPath,
	           "dataPath"  => $dataindexPath,
               "indexId"   => "1",
	           "installIp" => $ipArr,
	           "metaPath"  => $metaPath,
	           "password"  => $_SESSION["password"],
	           "user"      => "root"
	        );
        } else {
            $instalsend = array(
               "idxRootPath"  => $idxRootPath,
               "installIp"    => $ipArr,
               "password"     => $_SESSION["password"],
               "user"         => "root"
            );
        }
        
        $instalret = $this->exInfo(0x02080040, $instalsend, $instalrecv);
        
        $this->installNodeData();
        $this->display();
    }
    //==================================================================
    // 功能：页面刚打开时调用的函数
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function installNodeData()
    {
        //从数据库中查询所需内容
        $test = M("fbs_install");
        $data = $test->select();
        for ($i = 0; $i < count($data); $i++) {
            $ip       = $data[$i]["ip"];
            $state    = $data[$i]["state"];
            $progress = $data[$i]["progress"];
            $type     = $data[$i]["type"] == "index" ? "索引" : "数据";
            
            if (empty($_SESSION["IP"][$ip])) {
                $_SESSION["IP"][$ip] = FALSE;      // 没有记过日志
            }
            
            // 如果安装出错，并且没有记过日志，需要记录安装出错的日志
            if ($state == "error" && FALSE == $_SESSION["IP"][$ip]) {
                $this->writeLog("ISTRI","安装".$type."节点".$data[$i]["ip"]."失败".$data[$i]["description"],"error");
                $_SESSION["IP"][$ip] = TRUE;
            } elseif ($state == "ok" && $progress == "100" && FALSE == $_SESSION["IP"][$ip]) {
                $this->writeLog("ISTRI","安装".$type."节点".$data[$i]["ip"]."成功","info");
                $_SESSION["IP"][$ip] = TRUE;
            } 
        }
        $m       = M();
        $idxId   = (string)$_SESSION["indexId"];
        $idxIp   = $m->query("select ip from fbs_index_node where id = '{$idxId}'");
        $this->assign("idxIp",$idxIp[0]['ip']);
        $this->assign("lastData",$data[count($data) -1]["progress"]);
        $this->assign("laststate",$data[count($data) -1]["state"]);
        $this->assign("installData",$data);
    }
    
    //==================================================================
    // 功能：  前台js循环调用的函数
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function installDis()
    {
        $this->installNodeData();
        $this->display("installNode"); 
    }
    //==================================================================
    // 功能：删除确认页面
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function delList()
    {
    	$id       = $_REQUEST["id"];
    	$is_sel   = $_REQUEST["is_sel"];
    	$data     = $_REQUEST["datalist"];
    	$indexid  = $_REQUEST["indexid"];
    	$datalist = explode(";",substr($data,0,strlen($data)-1));
    	$_SESSION["idsgn"]    = $id;
    	
    	if ($id == NULL) {
    	   $m  = M();
            $test = $m->query("select ip,type from fbs_connection_test");
            for ($i = 0; $i < count($test); $i++) {
                $ipArr[] = $test[$i]["ip"];
                if ($test[$i]["type"] == 'index') {
                    $_SESSION["indexIp"]  = $test[$i]["ip"];
                    $_SESSION["idsgn"]    = 1;
                } else {
                	$idxid = $m->query("select indexid from fbs_data_node where ip = '".$test[$i]["ip"]."'");
                    $_SESSION["indexid"]  = $idxid[0]["indexid"];
                	$_SESSION["datalist"] = $ipArr;
                    $_SESSION["idsgn"]    = 0;
                }
            }
            $this->assign("datalist",$ipArr);
    	} else {
	    	if ($id == 0) {
	    	    $_SESSION["indexid"]  = $indexid;
	    	    $_SESSION["datalist"] = $datalist;
	    	    $this->assign("datalist",$datalist);
	    	    $this->assign("indexid",$indexid);
	    	} elseif ($id == 1) {
	    	    $_SESSION["indexIp"]  = $is_sel;
	    	    $indexlist[] = $is_sel;
	    	    $this->assign("datalist",$indexlist);
	    	} 
    	}
    	$this->assign("id",$id);
        $this->display();
    }
    //==================================================================
    // 功能： 删除页面刚打开时调用的函数
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function deltestData()
    {
        //查询数据库，显示测试结果
        $test = M("fbs_connection_test");
        $test_data = $test->select();
        $m   = M();
        $ign = 0;
        $idsgn    = $_SESSION["idsgn"];
        $datalist = $_SESSION["datalist"];
        $indexIp  = $_SESSION["indexIp"];
        $ipArr    = $datalist;
        if ($idsgn == 1) {
           $idxstate = $m->query("select state from fbs_connection_test where ip = '{$indexIp}'");
           if ($idxstate[0]["state"] == 'ok') {
               $ign = 1;
           }
        } else {
            for ($i = 0; $i < count($ipArr); $i++) {
                $datastate = $m->query("select state from fbs_connection_test where ip = '{$ipArr[$i]}'");
                if ($datastate[0]["state"] == 'ok') {
                    $ign = 1;                   //只要有节点状态为ok时可用
                } 
            }
        }
        $this->assign("ign",$ign);
        $this->assign("testlastData",$test_data[count($test_data) -1]["state"]);
        $this->assign("test_data",$test_data);
    }
    //==================================================================
    // 功能： 删除前台js循环调用的函数
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function delrefreshData()
    {
        $this->deltestData();
        $this->display("removetest");
    }
    //==================================================================
    // 功能： 分布式删除节点时测试页面
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function removetest()
    {
        $m        = M();
        $idsgn    = $_SESSION["idsgn"];
        $datalist = $_SESSION["datalist"];
        $indexid  = $_SESSION["indexid"];
        $is_sel   = $_SESSION["indexIp"];
        $password = $_REQUEST["password"] == '' ? 'noPassword' : $_REQUEST["password"];
        $_SESSION['delpwd'] = $password;
        $m->query("truncate fbs_connection_test");
        if ($idsgn == 1) {
            $indexSend = array(
                "connectIp" => $is_sel,
                "password"  => $password,
                "user"      => "root"
            );
            $indexSet = $this->exInfo(0x02080010, $indexSend, $indexrecv);
        } else {
            $dataSend = array(
                "connectIp" => $datalist,
                "indexId"   => $indexid,
                "password"  => $password,
                "user"      => "root"
            );
            $dataSet = $this->exInfo(0x02080020, $dataSend, $datarecv);
        }
        
        $this->deltestData();
        $this->display();
    }
    //==================================================================
    // 功能：删除节点
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function removeNode()
    {
    	$m        = M();
        $test_ip  = $m->query("select ip from fbs_connection_test where state = 'ok'");
        $idsgn    = $_SESSION["idsgn"];
        $indexid  = $_SESSION["indexid"];
        $password = $_SESSION['delpwd'];
        $m->query("truncate fbs_remove");
        if ($idsgn == 0) {
            for ($i = 0; $i < count($test_ip); $i++) {
                $ipArr[] = $test_ip[$i]["ip"];
            }
            $datasend = array(
	            "indexId"  => $indexid,
	            "removeIp" => $ipArr,
                "password" => $password,
                "user"     => "root",
	        );
	       $datadel = $this->exInfo(0x02080060, $datasend, $datarecv);
	       $list = implode(";",$ipArr); 
        } else {
        	$indexIp = $test_ip[0]["ip"];
            $indexsend = array(
                "removeIp" => $indexIp,
                "password" => $password,
                "user"     => "root",
            );
            $indexdel = $this->exInfo(0x02080050, $indexsend, $indexrecv);
        } 
        //dump($datasend);dump($indexsend);
        $this->reset();
        $this->dateNodes();
        $this->display();
    }
    
    //==================================================================
    // 功能：删除节点的数据查询
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function dateNodes()
    {
        //从数据库中查询所需内容
        $test = M("fbs_remove");
        $data = $test->select();
        for ($i = 0; $i < count($data); $i++) {
        	$ip       = $data[$i]["ip"];
        	$state    = $data[$i]["state"];
            $progress = $data[$i]["progress"];
            $type     = $data[$i]["type"] == "index" ? "索引" : "数据";
            if (empty($_SESSION["IP"][$ip])) {
            	$_SESSION["IP"][$ip] = FALSE;      // 没有记过日志
            }
            
            // 如果删除出错，并且没有记过日志，需要记录删除出错的日志
            if ($state == "error" && FALSE == $_SESSION["IP"][$ip]) {
            	$this->writeLog("ISTRI","删除".$type."节点".$data[$i]["ip"]."失败".$data[$i]["description"],"error");
            	$_SESSION["IP"][$ip] = TRUE;
            } elseif ($state == "ok" && $progress == "100" && FALSE == $_SESSION["IP"][$ip]) {
                $this->writeLog("ISTRI","删除".$type."节点".$data[$i]["ip"]."成功","info");
                $_SESSION["IP"][$ip] = TRUE;
            } 
        }
        $m        = M();
        $indexid  = $_SESSION["indexid"];
        $idxIp    = $m->query("select ip from fbs_index_node where id = '{$indexid}'");
        $this->assign("idxIp",$idxIp[0]["ip"]);
        $this->assign("lastdel",$data[count($data) -1]["progress"]);
        $this->assign("laststate",$data[count($data) -1]["state"]);
        $this->assign("delData",$data);
    }
    //==================================================================
    // 功能：  前台删除页面js循环调用的函数
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function removeDis()
    {
        $this->dateNodes();
        $this->display("removeNode"); 
    }
    //==================================================================
    // 功能：  删除节点剩余的Id
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    function delId()
    {
        $nodeId = $_REQUEST["nodeId"];
        $nodeIp = $_REQUEST["nodeIp"];
        $send = array(
            "agentIp" => $nodeIp,
            "id"      => $nodeId,
        );
        if (!$this->exInfo(0x00080080, $send, $recv)) {
            $this->writeLog("ISTRI","删除节点ID失败".$recv["info"],"error");
            $this->ajaxReturn("/Hdfs/index", $recv["info"], 0);
        }
        $this->writeLog("ISTRI","删除节点ID成功","info");
        $this->ajaxReturn("/Hdfs/index", "删除节点ID成功!", 1);
    }
    
    //==================================================================
    // 功能：  启动索引节点
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function startIndex()
    {
    	$m    = M();
        $recv = array();
        $node = $_REQUEST["nodes"];
        $send = array (
            "agentIp" => $node
        );
        $ret = $this->exInfo(0x00080020, $send, $recv);
        if (!$ret) {
        	$this->writeLog("ISTRI","启动索引节点{$node}失败".$recv["info"],"error");
            $this->ajaxReturn("/Hdfs/index", $recv["info"], 0);
        }
        $m->query("update fbs_index_node set state = 'running' where ip = '{$node}'");
        $this->writeLog("ISTRI","启动索引节点{$node}成功","info");
        $this->ajaxReturn("/Hdfs/index/indexname/".$node, "索引节点启动成功!", 1);
    }
    //==================================================================
    // 功能：  启动数据节点
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function startData()
    {
        $recv     = array();
        $data     = M();
        $idxnode  = $_REQUEST["idxnode"];
        $node     = $_REQUEST["nodes"];
        $send = array (
            "agentIp" => $node
        );
        //dump($send);
        
        $ret = $this->exInfo(0x00080030, $send, $recv);
        if (!$ret) {
            $this->writeLog("ISTRI","启动数据节点{$node}失败".$recv["info"],"error");
            $this->ajaxReturn("/Hdfs/index", $recv["info"], 0);
        } 
        $data->query("update fbs_data_node set state = 'running' where ip = '{$node}'");
        $this->writeLog("ISTRI","启动数据节点{$node}成功","info");
        $this->ajaxReturn("/Hdfs/index/indexname/".$idxnode, "启动数据节点成功!", 1);
    }
    //==================================================================
    // 功能： 停止索引节点
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function stopIndex()
    {
    	$m    = M();
        $recv = array();
        $node = $_REQUEST["nodes"];
        $send = array (
            "agentIp" => $node
        );
        //dump($send);
        
        $ret = $this->exInfo(0x00080040, $send, $recv);
        if (!$ret) {
            $this->writeLog("ISTRI","停止索引节点{$node}失败".$recv["info"],"error");
            $this->ajaxReturn("/Hdfs/index", $recv["info"], 0);
        } 
        $m->query("update fbs_index_node set state = 'stopped' where ip = '{$node}'");
        $this->writeLog("ISTRI","停止索引节点{$node}成功","info");
        $this->ajaxReturn("/Hdfs/index/indexname/".$node, "停止索引节点成功!", 1);
    }
    
    //==================================================================
    // 功能： 停止数据节点
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function stopData()
    {
        $recv = array();
        $data = M();
        $node = $_REQUEST["nodes"];
        $idxnode  = $_REQUEST["idxnode"];
        $send = array (
            "agentIp" => $node
        );
        $dataId = $data->query("select id from fbs_data_node where ip = '{$node}'");
        $id     = $dataId[0]['id'];
        $stopMark = 0; //标识是不是停止状态
        $arcState = $data->query("select archive_state from node_archive_state where data_node_id  = {$id}");
        for ($i = 0; $i < count($arcState); $i++) {
            if ($arcState[$i]['archive_state'] != 'stop') {
                $stopMark = 1;
            }
        }
        if ( $arcState == NULL || $stopMark == 0) {
	        $ret = $this->exInfo(0x00080050, $send, $recv);
	        if (!$ret) {
	            $this->writeLog("ISTRI","停止数据节点{$node}失败".$recv["info"],"error");
	            $this->ajaxReturn("/Hdfs/index", $recv["info"], 0);
	        } 
	        $data->query("update fbs_data_node set state = 'stopped' where ip = '{$node}'");
	        $this->writeLog("ISTRI","停止数据节点{$node}成功","info");
	        $this->ajaxReturn("/Hdfs/index/indexname/".$idxnode, "停止数据节点成功!", 1); 
        } else {
            $this->writeLog("ISTRI","停止数据节点{$node}失败,归档服务已启动","error");
            $this->ajaxReturn("/Hdfs/index", "停止数据节点{$node}失败,归档服务已启动", 0);
        } 
    }
    //==================================================================
    // 功能： 修复索引节点
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function repairIndex()
    {
        $recv = array();
        $data = M();
        $node = $_REQUEST["nodes"];
        $send = array (
            "agentIp" => $node
        );
        //dump($send);
        
        $ret = $this->exInfo(0x00080090, $send, $recv);
        if (!$ret) {
            $this->writeLog("ISTRI","修复索引节点{$node}失败".$recv["info"],"error");
            $this->ajaxReturn("/Hdfs/index", $recv["info"], 0);
        } 
        $data->query("update fbs_index_node set state = 'stopped' where ip = '{$node}'");
        $this->writeLog("ISTRI","修复索引节点{$node}成功","info");
        $this->ajaxReturn("/Hdfs/index/indexname/".$node, "修复索引节点成功!", 1);   
    }
    //==================================================================
    // 功能： 修复数据节点
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function repairData()
    {
        $recv = array();
        $data = M();
        $node = $_REQUEST["nodes"];
        $idxnode  = $_REQUEST["idxnode"];
        $send = array (
            "agentIp" => $node
        );
        //dump($send);
        
        $ret = $this->exInfo(0x000800A0, $send, $recv);
        if (!$ret) {
            $this->writeLog("ISTRI","数据节点{$node}修复失败".$recv["info"],"error");
            $this->ajaxReturn("/Hdfs/index", $recv["info"], 0);
        } 
        $data->query("update fbs_data_node set state = 'stopped' where ip = '{$node}'");
        $this->writeLog("ISTRI","数据节点{$node}修复成功","info");
        $this->ajaxReturn("/Hdfs/index/indexname/".$idxnode, "数据节点修复成功!", 1);    
    }
    //==================================================================
    // 功能： 配置数据节点
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function configData()
    {
    	$m = M();
    	$dataIp    = $_REQUEST["datanodesIP"];
        $dataPath  = $_REQUEST["dataPathNode"];
        $metaPath  = $_REQUEST["metaPath"];
        $indexPath = $_REQUEST["indexPath"];
        $idxnode   = $_REQUEST["idxaloneIp"];
        $imgDedup  = $_REQUEST["isDedupPicNode"] == '1' ? '1' : '0';
        $isphoto   = $_REQUEST["isDedupPicNode"] == '1' ? 'yes' : 'no';
        $state     = $m->query("select state from fbs_data_node where ip = '{$dataIp}'");
        $isfirst   = $state[0]["state"];
        $dbpath    = implode(";",$dataPath);
        //第一次编辑
        if ($isfirst == "ready") {
            $send = array(
	           "agentIp"     => $dataIp,
               "isFirstEdit" => "yes",
	           "indexPath"   => $indexPath,
	           "metaPath"    => $metaPath,
	           "allDataPath" => $dataPath,
	           "imgDedup"    => $imgDedup,
	        ); 
	        $ret = $this->exInfo(0x00080070, $send, $recv);
	        if (!$ret) {
	            $this->writeLog("ISTRI","配置数据节点{$dataIp}失败".$recv["info"],"error");
	            $this->ajaxReturn("/Hdfs/index", $recv["info"], 0);
	        }
	        $m->query("update fbs_data_node set state='stopped', dbpath = '{$dbpath}', mtpath = '{$metaPath}', idxpath = '{$indexPath}', isphoto = '{$isphoto}'  where ip = '{$dataIp}'");
	        $this->writeLog("ISTRI","配置数据节点{$dataIp}成功","info");
	        $this->ajaxReturn("/Hdfs/index/indexname/".$idxnode, "数据节点配置成功!", 1);
	        
        } else {
            $path = $m->query("select dbpath from fbs_data_node where ip = '{$dataIp}'");
            //把从数据库中查出的旧的路径分割成一个数组
            $oldPath = explode(";",$path[0]["dbpath"]);
            $send = array(
               "agentIp"     => $dataIp,
               "isFirstEdit" => "no",
               "oldDataPath" => $oldPath,
               "allDataPath" => $dataPath,
            );

           $ret = $this->exInfo(0x00080070, $send, $recv);
	            if (!$ret) {
	            $this->writeLog("ISTRI","配置数据节点{$dataIp}失败".$recv["info"],"error");
	            $this->ajaxReturn("/Hdfs/index", $recv["info"], 0);
            }
            $m->query("update fbs_data_node set dbpath = '{$dbpath}'  where ip = '{$dataIp}'");
	        $this->writeLog("ISTRI","配置数据节点{$dataIp}成功","info");
	        $this->ajaxReturn("/Hdfs/index/indexname/".$idxnode, "配置数据节点成功!", 1);
        }
        //dump($dbpath);
       
    }
    //==================================================================
    // 功能： 配置集群
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function configIndex()
    {
        $m = M();
        $indexIp       = $_REQUEST["indexNodeIp"];
        $indexRootPath = $_REQUEST["indexRootPath"];
        $dataPath      = $_REQUEST["dataPath"];
        $metaPath      = $_REQUEST["metaPath"];
        $indexPath     = $_REQUEST["indexPath"];
        $indexCbox     = $_REQUEST["indexCbox"];
        $imgDedup      = $_REQUEST["isDedupPic"] == '1' ? '1' : '0';
        $isphoto       = $_REQUEST["isDedupPic"] == '1' ? 'yes' : 'no';
        $info          = $m->query("select state,id from fbs_index_node where ip = '{$indexIp}'");
        $isfirst       = $info[0]["state"];
        $indexId       = $info[0]["id"];
        $ipList        = $m->query("select ip from fbs_data_node where indexid = {$indexId}");
        $errorInfo = array();
        $rootPath  = implode(";",$indexRootPath);  //把新添加的路径结合成字符串
        
        if ($isfirst == "ready") {
           $send = array(
               "agentIp"        => $indexIp,
               "isFirstEdit"    => "yes",
               "allIdxRootPath" => $indexRootPath,
           ); 
           $ret = $this->exInfo(0x00080060, $send, $recv);
	       if (!$ret) {
	           $errorInfo[] = $indexIp.":".$recv["info"];
	       } else {
               $m->query("update fbs_index_node set state='stopped',idxrootpath = '{$rootPath}' where ip = '{$indexIp}'");
	       }
           //dump($m->getlastSql()); 
        } else {
            $path    = $m->query("select idxrootpath from fbs_index_node where ip = '{$indexIp}'");
            $oldPath = explode(";",$path[0]["idxrootpath"]);
            $send = array(
                "agentIp"        => $indexIp,
                "isFirstEdit"    => "no",
                "oldIdxRootPath" => $oldPath,
                "allIdxRootPath" => $indexRootPath,
            ); 
	        $ret = $this->exInfo(0x00080060, $send, $recv);
	        if (!$ret) {
	            $errorInfo[] = $indexIp.":".$recv["info"];
	        } else {
	           $m->query("update fbs_index_node set idxrootpath = '{$rootPath}'  where ip = '{$indexIp}'");
	        }
        }
        
        if ($indexCbox == "1") {
	       for ($i = 0; $i < count($ipList); $i++) {
	            $datasend = array(
	               "agentIp"     => $ipList[$i]["ip"],
	               "isFirstEdit" => "yes",
	               "allDataPath" => $dataPath,
	               "metaPath"    => $metaPath,
	               "indexPath"   => $indexPath,
	               "imgDedup"    => $imgDedup,
	           );
	           $dataret = $this->exInfo(0x00080070, $datasend, $datarecv);
	           $newDataPath = implode(";",$dataPath);
	           if (!$dataret) {
                   $errorInfo[] = $ipList[$i]["ip"].":".$datarecv["info"];
               } else {
                   $m->query("update fbs_data_node set state='stopped',dbpath = '{$newDataPath}', mtpath = '{$metaPath}', idxpath = '{$indexPath}', isphoto = '{$isphoto}'  where ip = '{$ipList[$i]['ip']}'"); 
               }
	           //dump($m->getlastSql()); 
	       }
        }
        
        $info = implode(";",$errorInfo);
        if ($recv["info"] == "" && $datarecv["info"] == "") {
            $this->writeLog("ISTRI","配置集群{$indexIp}成功","info");
            $this->ajaxReturn("/Hdfs/index/indexname/".$indexIp, "节点配置成功!", 1);
        } 
        $this->writeLog("ISTRI","配置集群{$indexIp}失败".$info,"error");
        $this->ajaxReturn("/Hdfs/index", $info, 0);
    }
    //==================================================================
    // 功能： 单独配置索引节点
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function indexPerconfig()
    {
        $m = M();
        $indexIp       = $_REQUEST["indexaloneIp"];
        $indexRootPath = $_REQUEST["indexNodePath"];
        $isDedupPicPer = $_REQUEST["isDedupPicPer"] == '1' ? 'yes' : 'no';
        $info          = $m->query("select state,id from fbs_index_node where ip = '{$indexIp}'");
        $isfirst       = $info[0]["state"];
        $indexId       = $info[0]["id"];
        $ipList        = $m->query("select ip from fbs_data_node where indexid = {$indexId}");
        $errorInfo = array();
        $rootPath  = implode(";",$indexRootPath);  //把新添加的路径结合成字符串
        
        if ($isfirst == "ready") {
           $send = array(
               "agentIp"        => $indexIp,
               "isFirstEdit"    => "yes",
               "allIdxRootPath" => $indexRootPath,
           ); 
           $ret = $this->exInfo(0x00080060, $send, $recv);
           if (!$ret) {
               $errorInfo[] = $indexIp.":".$recv["info"];
           } else {
               $m->query("update fbs_index_node set state='stopped',idxrootpath = '{$rootPath}' where ip = '{$indexIp}'");
           }
           //dump($m->getlastSql()); 
        } else {
            $path    = $m->query("select idxrootpath from fbs_index_node where ip = '{$indexIp}'");
            $oldPath = explode(";",$path[0]["idxrootpath"]);
            $send = array(
                "agentIp"        => $indexIp,
                "isFirstEdit"    => "no",
                "oldIdxRootPath" => $oldPath,
                "allIdxRootPath" => $indexRootPath,
            ); 
            $ret = $this->exInfo(0x00080060, $send, $recv);
            if (!$ret) {
                $errorInfo[] = $indexIp.":".$recv["info"];
            } else {
               $m->query("update fbs_index_node set idxrootpath = '{$rootPath}'  where ip = '{$indexIp}'");
            }
        }
        $info = implode(";",$errorInfo);
        if ($recv["info"] == "") {
            $this->writeLog("ISTRI","配置索引节点{$indexIp}成功","info");
            $this->ajaxReturn("/Hdfs/index/indexname/".$indexIp, "配置索引节点成功!", 1);
        } 
        $this->writeLog("ISTRI","配置索引节点{$indexIp}失败".$info,"error");
        $this->ajaxReturn("/Hdfs/index", $info, 0);
    }
    //==================================================================
    // 功能： 启动集群
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function startCluster()
    {
    	$m         = M();
    	$recv      = array();
    	$datarecv  = array();
        $indexIp   = $_REQUEST["indexIp"];
        $nodes     = $_REQUEST["nodes"];
        $dataIp    = explode(",",$nodes);
        $errorInfo = array();
        $indexsend = array(
            "agentIp" => $indexIp
        );
        //发送启动索引
        $indexState = $m->query("select state, id from fbs_index_node where ip = '{$indexIp}'");
        $indexId    = $indexState[0]["id"];
        $countData  = $m->query("select * from fbs_data_node where indexid = {$indexId}");
        if ($indexState[0]["state"] == "stopped") {
	        $indexret = $this->exInfo(0x00080020, $indexsend, $recv);
	        if (!$indexret) {
	            $errorInfo[] = $indexIp.":".$recv["info"];
	        } else {
	           $m->query("update fbs_index_node set state='running' where ip = '{$indexIp}'");
	        }
        }
        if (count($countData) > 0) {
	        //循环发送启动数据节点
	        for ($i = 1; $i < count($dataIp); $i++) {
	            $datasend = array(
	                "agentIp" => $dataIp[$i]
	            );
	            $dataState = $m->query("select state from fbs_data_node where ip = '{$dataIp[$i]}'");
	            if ($dataState[0]["state"] == "stopped") {
	                $dataret = $this->exInfo(0x00080030, $datasend, $datarecv);
	                if (!$dataret) {
	                	$error[]     = $datarecv["info"];
		                $errorInfo[] = $dataIp[$i].":".$datarecv["info"];
		            } else {
		               $m->query("update fbs_data_node set state='running' where ip = '{$dataIp[$i]}'");
		            }
	            }
	            //dump($datasend);
	        }
        }
        $info = implode(";",$errorInfo);
        if ($recv["info"] == "" && count($error) == 0) {
            $this->writeLog("ISTRI","启动集群{$indexIp}成功","info");
            $this->ajaxReturn("/Hdfs/index/indexname/".$indexIp, "启动集群成功!", 1);
        } 
        $this->writeLog("ISTRI","启动集群{$indexIp}失败".$info,"error");
        $this->ajaxReturn("/Hdfs/index", $info, 0);
        //dump($indexsend);
    }
    //==================================================================
    // 功能： 停止集群
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function stopCluster()
    {
        $m         = M();
        $indexIp   = $_REQUEST["indexIp"];
        $nodes     = $_REQUEST["nodes"];
        $dataIp    = explode(",",$nodes);
        $errorInfo = array();
        $indexsend = array(
            "agentIp" => $indexIp
        );
        $errormark = 0;
        //发送停止索引
        $indexState = $m->query("select state,id from fbs_index_node where ip = '{$indexIp}'");
        $indexId    = $indexState[0]["id"];
        if ($indexState[0]["state"] == "running") {
            $Indexret = $this->exInfo(0x00080040, $indexsend, $recv);
	        if (!$Indexret) {
	        	$errormark = 1;
	            $errorInfo[] = $indexIp.":".$recv["info"];
	        } else {
	           $m->query("update fbs_index_node set state='stopped' where ip = '{$indexIp}'");
	        }
        }
        $countData    = $m->query("select * from fbs_data_node where indexid = {$indexId}");
        if (count($countData) > 0) {
	        //循环发送停止数据节点
	        for ($i = 1; $i < count($dataIp); $i++) {
	            $datasend = array(
	                "agentIp" => $dataIp[$i]
	            );
	            $dataState = $m->query("select state from fbs_data_node where ip = '{$dataIp[$i]}'");
	                        
	            $dataId   = $m->query("select id from fbs_data_node where ip = '{$dataIp[$i]}'");
		        $id       = $dataId[0]['id'];
		        $stopMark = 0;
		        $arcState = $m->query("select archive_state from node_archive_state where data_node_id  = {$id}");
		        for ($j = 0; $j < count($arcState); $j++) {
		            if ($arcState[$j]['archive_state'] != 'stop') {
		                $stopMark = 1;
		            }
		        }
		        if ( $arcState == NULL || $stopMark == 0) {
		            if ($dataState[0]["state"] == "running") {
		                $dataret = $this->exInfo(0x00080050, $datasend, $datarecv);
			            if (!$dataret) {
			            	$errormark = 1;
			                $errorInfo[] = $dataIp[$i].":".$datarecv["info"];
			            } else {
			               $m->query("update fbs_data_node set state='stopped' where ip = '{$dataIp[$i]}'");
			            }
			        }
		        } else {
		        	$errormark = 1;
		            $errorInfo[] = "停止节点".$dataIp[$i]."失败,归档服务已启动";  
		        }
	        }
        }
        $info = implode(";",$errorInfo);
        if ($errormark == 0) {
            $this->writeLog("ISTRI","停止集群{$indexIp}成功","info");
            $this->ajaxReturn("/Hdfs/index/indexname/".$indexIp, "停止集群成功!", 1);
        } 
        $this->writeLog("ISTRI","停止集群{$indexIp}失败".$info,"error");
        $this->ajaxReturn("/Hdfs/index", $info, 0);
        //dump($indexsend);
    }
}