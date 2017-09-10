<?php
class UpsAction extends CommonAction
{
    public function index()
    {
    	$this->unsetSig();   //删除保存在session里的内容
        $data = array();
        $recv = array();
        if (!$this->exInfo(CMD_SYS_UPS_LOOKUP, $send, $recv)) {
            $this->error("获取UPS信息失败: ".$recv["info"]);
        }
        $this->assign("lefttime", $recv["time"]);
        $this->assign("leftpower", $recv["power"]);
        $this->assign("isUps", $recv["state"]);

    	$this->display();
    }
    //==================================================================
    // 功能： UPS设置
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function setUps()
    {
        $str = $_POST;
        $str['upsmodel'] = $str['upsmodel'] ? 1 : 0;
        $recv = array();
        $data = array(
            "state"  => $str['upsmodel'],
            "power"  => (int)$str['leftpower'],
            "time"   => (int)$str['lefttime'],
        );
        $ret = $this->exInfo(CMD_SYS_UPS_SET, $data, $recv);
        if ($ret) {
        	$this->writeLog("SYSTEM","UPS设置成功","info");
            $this->ajaxReturn("/Ups/index", "UPS设置成功!", 1);
        } else {
        	$this->writeLog("SYSTEM","UPS设置失败".$recv["info"],"error");
            $this->ajaxReturn("/Ups/index", $recv["info"], 0);
        }
    }
}
