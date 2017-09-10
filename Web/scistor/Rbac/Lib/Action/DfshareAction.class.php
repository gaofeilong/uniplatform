<?php
class DfshareAction extends CommonAction
{
    public function index()
    {
        $set = $this->exInfo(0x00090010, $send, $recv);
        if (!$set) {
            $this->error("获取共享管理信息失败: ".$recv["info"]);
        }
        
		$nfsList = $recv['nfsList'];
		$umountSig = 1;
		$mpState = 'mounted';
		$nfsState  = 'stopped';
        for ($i = 0; $i < count($nfsList); $i++) {
        	$nfsList[$i]["mountSig"] = 1;
            $nfsList[$i]["umountSig"] = 1;
            for ($j = 0; $j < count($nfsList[$i]['client']); $j++) {
               $nfsList[$i]['strIp'] .=  $nfsList[$i]['client'][$j]['ip'].";";
               if ($nfsList[$i]['client'][$j]['state'] != "mounted") {
                   $nfsList[$i]["mountSig"] = 0;
               }
               if ($nfsList[$i]['client'][$j]['state'] != "umounted") {
                   $nfsList[$i]["umountSig"] = 0;
                   $umountSig = 0;
               }
            }
            if ($nfsList[$i]['state'] == 'running') {
                $nfsState  = 'running';
            }
            if ($nfsList[$i]['mpState'] == 'umounted') {
                $mpState  = 'umounted';
            }
            $nfsList[$i]["client_num"] = count($nfsList[$i]['client']); 
        }
        //dump($nfsList);
        $m  = M();
        $ddfs_mp = $m->query("select mp from ddfsmp");
        $adfs_mp = $m->query("select firstdatapath from adfsmp");
        $countmp = 0;
        if (count($ddfs_mp) > 0) {
            $mp      = $ddfs_mp;
            $countmp = count($ddfs_mp);
        } elseif (count($adfs_mp) > 0) {
            for ($i = 0; $i < count($adfs_mp); $i++) {
            	$str = $adfs_mp[$i]['firstdatapath'];
            	if (substr($str, strlen($str) - 1 ,1) == '/') {
                    $mp[$i]['mp'] = $adfs_mp[$i]['firstdatapath']."mp";
            	} else {
            	    $mp[$i]['mp'] = $adfs_mp[$i]['firstdatapath']."/mp";
            	}
            }
            $countmp = count($adfs_mp);
        }
        $this->assign("mp",$mp);
        $this->assign("mplnum",$countmp);
        $this->assign("nfsList",$nfsList);
        $this->assign("nfsCount",count($nfsList));
        $this->assign("umountSig",$umountSig);
        $this->assign("nfsState",$nfsState);
        $this->assign("mpState",$mpState);
        $this->assign("serverIP",$_SERVER["SERVER_ADDR"]);
        $this->display();    	
    }
    //==================================================================
    // 功能： 添加共享
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function addShare() 
    {
    	$data = $_REQUEST;
    	$name = $data["shareName"];
        if ($data["addIpWay"] == "single") {
            for ($i = 0; $i < count($data['ipName']); $i++) {
                $ipArr[$i]['ip']       = $data['ipName'][$i];
                $ipArr[$i]['password'] = $data['ipPass'][$i] == '' ? 'noPassword' : $data['ipPass'][$i];
            }
        } else {
            $startIp     = $data["startIp"];
            $endIp       = $data["endIp"];
                
            $half_q      = substr($startIp,0,strrpos($startIp,".") + 1);
            $half_satrt  = substr($startIp,strrpos($startIp,".") + 1);
            $half_end    = substr($endIp,strrpos($endIp,".") + 1);
            for ($i = $half_satrt; $i <= $half_end; $i++) {
                $vpIp[] = $half_q.$i; //所有的IP在一个数组中
            }
            for ($i = 0; $i < count($vpIp); $i++) {
                $ipArr[$i]['ip']       = $vpIp[$i];
                $ipArr[$i]['password'] = $data['ipPwd'] == "" ? "noPassword" : $data['ipPwd'];
            }
        }
        $send = array(
            'client'    => $ipArr,
            'name'      => $data['shareName'],
            'mp'        => $data['clientMountPath'],
            'parameter' => $data['shareParameter'],
            'path'      => $data['sharePath']
        );
        $set = $this->exInfo(0x00090020, $send, $recv);
        if (!$set) {
        	$this->writeLog("DDFS", $recv['info'],"error");
            $this->ajaxReturn("/Dfshare/index", $recv['info'], 0);
        } 
        $this->writeLog("DDFS","共享服务{$name}添加成功".$recv['info'],"info");
        $this->ajaxReturn("/Dfshare/index/", "共享服务{$name}添加成功".$recv['info'], 1);
    }
    //==================================================================
    // 功能： 编辑共享
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function editShare() 
    {
        $data  = $_REQUEST;
        $ipArr = array();
        $name  = $data["shareName"];
        if ($data["e_addIpWay"] == "single") {
        	if ($data['e_ipName'][0] != '') {
	            for ($i = 0; $i < count($data['e_ipName']); $i++) {
	                $ipArr[$i]['ip']       = $data['e_ipName'][$i];
	                $ipArr[$i]['password'] = $data['e_ipPass'][$i];
	            }
            }
        } else {
        	if ($data["e_startIp"] != '') {
	            $startIp     = $data["e_startIp"];
	            $endIp       = $data["e_endIp"];
	                
	            $half_q      = substr($startIp,0,strrpos($startIp,".") + 1);
	            $half_satrt  = substr($startIp,strrpos($startIp,".") + 1);
	            $half_end    = substr($endIp,strrpos($endIp,".") + 1);
	            for ($i = $half_satrt; $i <= $half_end; $i++) {
	                $vpIp[] = $half_q.$i; //所有的IP在一个数组中
	            }
	            for ($i = 0; $i < count($vpIp); $i++) {
	                $ipArr[$i]['ip']       = $vpIp[$i];
	                $ipArr[$i]['password'] = $data['e_ipPwd'];
	            }
        	}
        }
        $send = array(
            'newClient' => $ipArr,
            'mp'        => $data['clientMountPath'],
            'name'      => $data['shareName'],
            'parameter' => $data['shareParameter'],
            'path'      => $data['sharePath']
        );
        $set = $this->exInfo(0x00090040, $send, $recv);
        if (!$set) {
            $this->writeLog("DDFS", $recv['info'],"error");
            $this->ajaxReturn("/Dfshare/index", $recv['info'], 0);
        } 
        $this->writeLog("DDFS","共享服务{$name}编辑成功".$recv['info'],"info");
        $this->ajaxReturn("/Dfshare/index/", "共享服务{$name}编辑成功".$recv['info'], 1);
    }
    //==================================================================
    // 功能：删除共享
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function delShare()
    {
    	$delname = $_REQUEST['delSNameVal'];
    	$shname  = substr($delname,0,strlen($delname) - 1);
    	$name    = explode(";",$shname);
    	$send = array(
            'name' => $name
        );
        $set = $this->exInfo(0x00090030, $send, $recv);
        if (!$set) {
            $this->writeLog("DDFS"," 共享服务{$shname}删除失败".$recv['info'],"error");
            $this->ajaxReturn("/Dfshare/index", $recv['info'], 0);
        } 
        $this->writeLog("DDFS","共享服务{$shname}删除成功","info");
        $this->ajaxReturn("/Dfshare/index/", "共享服务{$shname}删除成功", 1);
    }
    //==================================================================
    // 功能： 启动服务
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function startServer()
    {
        $set = $this->exInfo(0x00090050, $send, $recv);
        if (!$set) {
            $this->writeLog("DDFS","共享服务启动失败".$recv['info'],"error");
            $this->ajaxReturn("/Dfshare/index", $recv['info'], 0);
        } 
        $this->writeLog("DDFS","共享服务启动成功","info");
        $this->ajaxReturn("/Dfshare/index", "共享服务启动成功", 1); 
    }
    //==================================================================
    // 功能：停止服务
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function stopServer()
    {
        $set = $this->exInfo(0x00090060, $send, $recv);
        if (!$set) {
            $this->writeLog("DDFS","共享服务停止失败".$recv['info'],"error");
            $this->ajaxReturn("/Dfshare/index", $recv['info'], 0);
        } 
        $this->writeLog("DDFS","共享服务停止成功","info");
        $this->ajaxReturn("/Dfshare/index", "共享服务停止成功", 1); 
    }
    //==================================================================
    // 功能： 挂载客户端
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function mountClient() 
    {
    	$ip        = $_REQUEST["ip"];
    	$shareName = $_REQUEST['shareName'];
        $send = array(
            'client' => $ip,
            'name'   => $shareName
        );
        $set = $this->exInfo(0x00090070, $send, $recv);
        if (!$set) {
            $this->writeLog("DDFS","客户端{$ip}挂载失败".$recv['info'],"error");
            $this->ajaxReturn("/Dfshare/index", $recv['info'], 0);
        } 
        $this->writeLog("DDFS","客户端{$ip}挂载成功","info");
        $this->ajaxReturn("/Dfshare/index", "客户端{$ip}挂载成功", 1);
    }
    //==================================================================
    // 功能：卸载客户端
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function umountClient() 
    {
        $ip        = $_REQUEST["ip"];
        $shareName = $_REQUEST['shareName'];
        $send = array(
            'client' => $ip,
            'name'   => $shareName
        );
        $set = $this->exInfo(0x00090080, $send, $recv);
        if (!$set) {
            $this->writeLog("DDFS","客户端{$ip}卸载失败".$recv['info'],"error");
            $this->ajaxReturn("/Dfshare/index", $recv['info'], 0);
        } 
        $this->writeLog("DDFS","客户端{$ip}卸载成功","info");
        $this->ajaxReturn("/Dfshare/index", "客户端{$ip}卸载成功", 1);
    }
    //==================================================================
    // 功能： 挂载所有客户端
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function monutAll()
    {
        $nodes     = $_REQUEST["nodes"];
        $shareName = $_REQUEST['shareName'];
        $nodesArr  = split(",",$nodes);
        $send = array (
            'client' => $nodesArr,
            'name'   => $shareName
        );
        $set = $this->exInfo(0x000900A0, $send, $recv);
        if ($recv['info'] != '') {
            $this->writeLog("DDFS", $recv['info'],"error");
            $this->ajaxReturn("/Dfshare/index", $recv['info'], 0);
        } 
        $this->writeLog("DDFS","共享服务".$shareName."挂载成功","info");
        $this->ajaxReturn("/Dfshare/index", "共享服务".$shareName."挂载成功", 1);
    }
    //==================================================================
    // 功能： 卸载所有客户端
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function umonutAll()
    {
        $nodes     = $_REQUEST["nodes"];
        $shareName = $_REQUEST['shareName'];
        $nodesArr  = split(",",$nodes);
        $send = array (
            'client' => $nodesArr,
            'name'   => $shareName
        );
        $set = $this->exInfo(0x000900B0, $send, $recv);
        if ($recv['info'] != '') {
            $this->writeLog("DDFS", $recv['info'],"error");
            $this->ajaxReturn("/Dfshare/index", $recv['info'], 0);
        } 
        $this->writeLog("DDFS","共享服务".$shareName."卸载成功","info");
        $this->ajaxReturn("/Dfshare/index", "共享服务".$shareName."卸载成功", 1);
    }
    //==================================================================
    // 功能：删除客户端节点
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function delClient() 
    {
        $ip        = $_REQUEST["ip"];
        $shareName = $_REQUEST['shareName'];
        $send = array (
            'client' => $ip,
            'name'   => $shareName
        );
        $set = $this->exInfo(0x00090090, $send, $recv);
        if (!$set) {
            $this->writeLog("DDFS","客户端{$ip}删除失败".$recv['info'],"error");
            $this->ajaxReturn("/Dfshare/index", $recv['info'], 0);
        } 
        $this->writeLog("DDFS","客户端{$ip}删除成功","info");
        $this->ajaxReturn("/Dfshare/index", "客户端{$ip}删除成功", 1);
    }
}
