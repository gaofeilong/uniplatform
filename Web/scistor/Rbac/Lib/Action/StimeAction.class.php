<?php
class StimeAction extends CommonAction
{
    public function index()
    {
    	$this->unsetSig();
        $send = array();
        $recv = array();
        if (!$this->exInfo(CMD_SYS_TIME_LOOKUP, $send, $recv)) {
            $this->error("获取系统时间配置失败: ".$recv["info"]);
        }
        $time_hour=array("00","01","02","03","04","05","06","07","08","09","10","11","12"
                         ,"13","14","15","16","17","18","19","20","21","22","23",);
        $time_in=array("00","01","02","03","04","05","06","07","08","09","10","11","12"
                         ,"13","14","15","16","17","18","19","20","21","22","23","24","25"
                         ,"26","27","28","29","30","31","32","33","34","35","36","37","38"
                         ,"39","40","41","42","43","44","45","46","47","48","49","50","51"
                         ,"52","53","54","55","56","57","58","59");
        $webTime = explode(" ",$recv["time"]);
        $webDate = explode("-",$webTime[0]);
        $webHour = explode(":",$webTime[1]);
        //dump($recv);
        
        $this->assign("timeInfo",$recv);
        $this->assign("webDate",$webDate);
        $this->assign("webHour",$webHour);
        $this->assign("time_hour",$time_hour);
        $this->assign("time_sel",$time_in);
        $this->display();
        
    }
    //==================================================================
    // 功能： 系统时间设置
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function setTime()
    {
        $str = $_POST;
        $str['synchro'] = $str['synchro'] ? 1 : 0;
        if($str['synchro'] == 0){
            $data = array (
                'time' => $str['year']."-".$str['month']."-".$str['day']." ".$str['hour'].":".$str['minute'].":".$str['second'],
                'zone' => $str['timezone'],
                'sync' => $str['synchro'],
                'ntp'  => $str['ntp_serve'],
            );
        } else {
            $data = array (
                'zone' => $str['timezone'],
                'sync' => $str['synchro'],
                'ntp' => $str['ntp_serve'],
            );
        }
        $recv = array();
        $status = $this->exInfo(CMD_SYS_TIME_SET, $data, $recv);
        if ($status) {
            $this->writeLog("SYSTEM","系统时间设置成功","info");
            $this->ajaxReturn("/Stime/index", "系统时间设置成功!", 1);
        } else {
            $this->writeLog("SYSTEM","系统时间设置失败".$recv["info"],"error");
            $this->ajaxReturn("/Stime/index", $recv["info"], 0);
        }
    }


}
