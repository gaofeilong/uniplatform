<?php
class BasenetAction extends CommonAction
{
    public function index()
    {
        $send = array();
        $recv = array();
        if (!$this->exInfo(CMD_NET_BASE_LOOKUP, $send, $recv)) {
            $this->error("获取基础网络配置信息失败: ".$recv["info"]);
        }
        $this->assign("hostName", $recv["hostName"]);
        if ($recv["dns0"]) {
            $this->assign("dns0", $recv["dns0"]);
            if ($recv["dns1"]) {
                $this->assign("dns1", $recv["dns1"]);
            }
        }
        $this->display();
    }
    //==================================================================
    // 功能： 设置基础网络配置
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function setBasenet()
    {
        $recv = array();
        $send = array(
            "hostName" => $_POST['hostName'],
            "dns0"     => $_POST['dns0'],
            "dns1"     => $_POST['dns1'],
        );

        $ret = $this->exInfo(CMD_NET_BASE_SET, $send, $recv);
        if ($ret) {
        	$this->writeLog("NETWORK","配置基础网络配置成功","info");
            $this->ajaxReturn("/Basenet/index", "基础网络配置成功!", 1);
        } else {
        	$this->writeLog("NETWORK","配置基础网络配置失败".$recv["info"],"error");
            $this->ajaxReturn("/Basenet/index", $recv["info"], 0);
        }
    }
}
