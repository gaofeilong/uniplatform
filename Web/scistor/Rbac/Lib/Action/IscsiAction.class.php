<?php
class IscsiAction extends CommonAction
{
    public function index()
    {
    	$this->unsetSig();   //删除保存在session里的内容
    	$send = array();
        $recv = array();
        /*$iscsiinfo = $this->exInfo(CMD_NET_ISCSI_LOOKUP, $send, $recv);
        if ($iscsiinfo){
            $this->display();
        } else {
            $this->error("获取ISCSI页面失败: ".$recv["info"]);
        }*/
        $this->display();
    }
    //==================================================================
    // 功能： 设置ISCSI配置
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function iscsiSet()
    {
    	$data = array(
            "iscsiSet" => $_POST["Target_check"]
        );
        echo "<pre>";print_r($data);
        $recv = array();
        /*$status = $this->exInfo(CMD_NET_ISCSI_SET, $data, $recv);
        if ($status) {
            $this->writeLog("NETWORK","配置ISCSI服务成功","info");
            $this->ajaxReturn("/Iscsi/index", "ISCSI设置成功!", 1);
        } else {
            $this->writeLog("NETWORK","配置ISCSI服务失败".$recv["info"],"error");
            $this->ajaxReturn("/Iscsi/index", $recv["info"], 0);
        }*/
    }
}