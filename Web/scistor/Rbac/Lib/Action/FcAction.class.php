<?php
class FcAction extends CommonAction
{
    public function index()
    {
    	$this->unsetSig();   //删除保存在session里的内容
        $send = array();
        $recv = array();
        
        /*if (!$this->exInfo(CMD_NET_FC_LOOKUP, $send, $recv)) {
            $this->error("获取Fc信息失败: ".$recv["info"]);
        }*/
        $this->display();
    }
    //==================================================================
    // 功能： 设置FC配置
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function fcSet()
    {
        $arr = $_POST;
        $data = array(
            "fcset" => $arr["Target_cho"]
        );
        echo "<pre>";print_r($data);
        $recv = array();
        /*$status = $this->exInfo(CMD_NET_FC_SET, $data, $recv);
        if ($status) {
            $this->writeLog("NETWORK","配置FC服务成功","info");
            $this->ajaxReturn("/Fc/index", "FC设置成功!", 1);
        } else {
            $this->writeLog("NETWORK","配置FC服务失败".$recv["info"],"error");
            $this->ajaxReturn("/Fc/index", $recv["info"], 0);
        }*/
    }
}