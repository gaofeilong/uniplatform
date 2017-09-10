<?php
class VgAction extends CommonAction
{
    public function index()
    {
    	$this->unsetSig();   //删除保存在session里的内容
    	$send = array();
        $recv = array();
        /*if (!$this->exInfo(CMD_STORAGE_VG_LOOKUP, $send, $recv)) {
            $this->error("获取卷组管理页面失败: ".$recv["info"]);
        }
        $this->assign("lvInfo",$recv);*/
        $this->display();
    }
    
    //==================================================================
    // 功能： 新建卷组
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function vgCreate()
    {
        $str = $_REQUEST["hidvgCreate"];
        $lvname = explode(";",substr($str,0,strlen($str)-1));
        $data = array(
            "lvname"    => $lvname,
            "vol_level" => $_POST["vol_level"],
            "bandwidth" => $_POST["bandwidth"],
            "vgName"    => $_POST["vgName"],
        );
        echo "<pre>";print_r($data);
        /*$status = $this->exInfo(CMD_STORAGE_LV_CREATE, $data, $recv);
        if ($status) {
            $this->ajaxReturn("", "新建卷组成功!", 1);
        } else {
            $this->ajaxReturn("", $recv["info"], 0);
        }*/
    }
    
    //==================================================================
    // 功能： 卷组扩容
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function vgExent()
    {
    	$str = $_REQUEST["lvexentName"];
    	$data = explode(";",substr($str,0,strlen($str)-1));
        echo "<pre>";print_r($data);
        /*$status = $this->exInfo(CMD_STORAGE_VG_EXTEND, $data, $recv);
        if ($status) {
            $this->ajaxReturn("", "卷组扩容成功!", 1);
        } else {
            $this->ajaxReturn("", $recv["info"], 0);
        }*/
    }
    //==================================================================
    // 功能： 卷组删除
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function vgDel()
    {
        $str = $_REQUEST["vgDelname"];
        $start = strrpos($str,";");
        $data = explode(";",substr($str,0,strlen($str)-1));
        echo "<pre>";print_r($data);
        /*$status = $this->exInfo(CMD_STORAGE_VG_REMOVE, $data, $recv);
        if ($status) {
            $this->ajaxReturn("", "卷组删除成功!", 1);
        } else {
            $this->ajaxReturn("", $recv["info"], 0);
        }*/
    }
}
