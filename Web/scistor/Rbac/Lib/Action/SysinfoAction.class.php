<?php
class SysinfoAction extends CommonAction
{
    public function index()
    {
    	$this->unsetSig();   //删除保存在session里的内容
    	$send = array();
    	$recv = array();
        $ret = $this->exInfo(CMD_SYS_INFO_LOOKUP, $send, $recv);
        if (!$this->exInfo(CMD_SYS_INFO_LOOKUP, $send, $recv)) {
            $this->error("获取系统信息失败: ".$recv["info"]);
        }
        for ($i = 0;$i < count($recv["disk"]);$i++)
        {
            $recv["disk"][$i]["size"] = $this->format_bytes($recv["disk"][$i]["size"]);
        }
        for ($i = 0;$i < count($recv["cpu"]);$i++)
        {
            $recv["cpu"][$i]["cache"] = $this->format_bytes($recv["cpu"][$i]["cache"]);
        }
        $recv["mem"]["size"] = $this->format_bytes($recv["mem"]["size"]);
        
        $this->assign("sysInfo",$recv);
        $this->display();
    }
}
