<?php
class NetworkAction extends CommonAction
{
    public function index()
    {
    	$this->unsetSig();   //删除保存在session里的内容
        $send = array();
        $recv = array();
        if (!$this->exInfo(CMD_NET_CARD_LOOKUP, $send, $recv)) {
            $this->error("获取网卡配置信息失败: ".$recv["info"]);
        }
            
        $netinfo = $recv["eths"];
        $bondinfo = $recv["bonds"];
        for ($i = 0;$i < count($bondinfo);$i++)
        {
           $card = $bondinfo[$i]["eths"];
           for ($j = 0;$j < count($card);$j++)
           {
                $bondinfo[$i]["card"] .= " ".$card[$j];
           }
        }
        //echo "<pre>";print_r($ip);
        $this->assign("netinfo",$netinfo);
        $this->assign("bondinfo",$bondinfo);
        $this->assign("name",$bondinfo[0]["name"]);
        $this->display();
        
    }
    //==================================================================
    // 功能：网卡配置
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function setNetwork()
    {
        $data = array(
            "name"      => $_POST["card_name"],
            "ip"        => $_POST["ipAdd"],
            "mask"      => $_POST["mask"],
            "mtu"       => (int)$_POST["mtu_set"],
            "mac"       => $_POST["netMmac"],
            "bootProto" => $_POST["config_ip"],
            "gateway"   => $_POST["geteWay"],
        );
        $recv = array();
        //echo "<pre>";print_r($data);
        $ret = $this->exInfo(CMD_NET_CARD_SET, $data, $recv);
        if ($ret) {
        	$this->writeLog("NETWORK","配置普通网卡成功","info");
            $this->ajaxReturn("/Network/index", "网卡配置成功!正在重启网络...", 1);
        } else {
        	$this->writeLog("NETWORK","配置普通网卡失败".$recv["info"],"error");
            $this->ajaxReturn("/Network/index", $recv["info"], 0);
        }
    }
    //==================================================================
    // 功能：网卡绑定
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function bondNetwork()
    {
    	$DEVICES = split(" ", $_POST["DEVICES"]);
    	for ($i = 0; $i < count($DEVICES); $i++) {
    	   if ($DEVICES[$i] != "") {
    	       $dev[] = $DEVICES[$i];
    	   }
    	}
        $data = array(
            "bootProto" => $_POST["bondip"],
            "ip"        => $_POST["ipAdd"],
            "mask"      => $_POST["mask"],
            "gateway"   => $_POST["geteWay"],
            "mode"      => (int)$_POST["bondmode"],
            "mtu"       => (int)$_POST["mtu_set"],
            "eths"      => $dev,
        );
        $recv = array();
        //echo "<pre>";print_r($data);
        $ret = $this->exInfo(CMD_NET_BOND_CREATE, $data, $recv);
        if ($ret) {
        	$this->writeLog("NETWORK","绑定网卡成功","info");
            $this->ajaxReturn("/Network/index", "网卡绑定成功!正在重启网络...", 1);
        } else {
        	$this->writeLog("NETWORK","绑定网卡失败".$recv["info"],"error");
            $this->ajaxReturn("/Network/index", $recv["info"], 0);
        }
    }
    //==================================================================
    // 功能：绑定网卡配置
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function bondWorkSet()
    {
        $data = array(
            "bootProto" => $_POST["bondiphand"],
            "name"      => $_POST["bondmtu_name"],
            "ip"        => $_POST["ipAdd"],
            "mode"      => (int)$_POST["bondmode"],
            "mtu"       => (int)$_POST["bondmtu_set"],
            "mask"      => $_POST["mask"],
            "gateway"   => $_POST["geteWay"],
            "subdev"    => split(" ", $_POST["bondDEVICES"]),
        );
        //echo "<pre>";print_r($data);
        $recv = array();
        $ret = $this->exInfo(CMD_NET_BOND_SET, $data, $recv);
        if ($ret) {
        	$this->writeLog("NETWORK","配置绑定网卡成功","info");
            $this->ajaxReturn("/Network/index", "绑定网卡配置成功!正在重启网络...", 1);
        } else {
        	$this->writeLog("NETWORK","配置绑定网卡失败".$recv["info"],"error");
            $this->ajaxReturn("/Network/index", $recv["info"], 0);
        }
    }
    //==================================================================
    // 功能：删除绑定网卡
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function bondDel()
    {
        $data = array("name" => $_REQUEST["bondname"]);
        $recv = array();
        $ret = $this->exInfo(CMD_NET_BOND_REMOVE, $data, $recv);
        if ($ret) {
        	$this->writeLog("NETWORK","解除网卡绑定成功","info");
            $this->ajaxReturn("/Network/index", "绑定网卡删除成功!正在重启网络...", 1);
        } else {
        	$this->writeLog("NETWORK","解除网卡绑定失败".$recv["info"],"error");
            $this->ajaxReturn("/Network/index", $recv["info"], 0);
        }
    }
}
