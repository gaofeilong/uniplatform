<?php
class EventAction extends CommonAction
{
    public function index()
    {
        $t = M("email_config");
        $data = $t->order("level")->findall();
        if ($_SESSION["sig"] == 1){
        	$this->assign("isEvent", 1);
            $this->assign("mailServer", $_SESSION["alarm"]["mailServer"]);
            $this->assign("mail_from", $_SESSION["alarm"]["mail_from"]);
            $this->assign("mail_fromPass", $_SESSION["alarm"]["mail_fromPass"]);
            $this->assign("verify", $_SESSION["alarm"]["identy_way"]);
            $this->assign("recvive", $_SESSION["alarm"]["newmail"]);
            if ($data[0]["recv"]){
                    $this->assign("event_data", $data);
            }
        } else {
	        if (count($data) > 0) {
	            $this->assign("isEvent", $data[0]["state"]);
	            $this->assign("mailServer", $data[0]["server"]);
	            $this->assign("mail_from", $data[0]["send"]);
	            $this->assign("mail_fromPass", $data[0]["send_passwd"]);
	            $this->assign("verify", $data[0]["verify"]);
	            if ($data[0]["recv"]){
	                $this->assign("event_data", $data);
	            }
	        }
        }
        //echo "<pre>";print_r($data);
        $this->display();
    }
    //==================================================================
    // 功能： 邮件报警设置
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function setEmail()
    {
    	unset($_SESSION["sig"]);
        // 把所有报警级别相同的收件箱放在一条记录中，
        // 整理完成后最多只有 7 条记录，每条记录级别都不同
        $table = array();
    	$recvEmail = $_POST["eventMail"];
    	$recvLevel = $_POST["mailLevel"];
        for ($i = 0; $i < count($recvLevel); $i++) {
            $table[$recvLevel[$i]] .= $recvEmail[$i];
        }
        //dump($table);
        // 插入数据表
        $t = M("email_config");
        $t->delete($t->findall());
        $record = array(
            "state"       => $_POST["warn"] == "yes"? 1: 0,
            "server"      => $_POST["mailServer"],
            "send"        => $_POST["mail_from"],
            "send_passwd" => $_POST["mail_fromPass"],
            "verify"      => $_POST["identy_way"],
        );
        if (count($table) == 0)
        {
            if ($t->add($record) == 0) {
            	$this->writeLog("SYSTEM","配置报警邮件失败","error");
                $this->ajaxReturn("", "插入数据库失败!", 0);
            } 
        } else {
	        foreach (array_keys($table) as $k) {
	            $record["recv"]  = $table[$k];
	            $record["level"] = $k;
	            if ($t->add($record) == 0) {
	            	$this->writeLog("SYSTEM","配置报警邮件失败","error");
	                $this->ajaxReturn("/Event/index", "插入数据库失败!", 0);
	            }
	        }
	    }
	    $this->writeLog("SYSTEM","配置报警邮件成功","info");
	    $this->ajaxReturn("/Event/index", "报警设置成功!", 1);
    }
    
    //==================================================================
    // 功能： 用session保存测试的内容
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function save_alarm()
    {
        $alarm["mailServer"]    = $_POST['mailServer'];
        $alarm["mail_from"]     = $_POST['mail_from'];
        $alarm["mail_fromPass"] = $_POST['mail_fromPass'];
        $alarm["identy_way"]    = $_POST['identy_way'];
        $alarm["newmail"]       = $_POST['newmail'];
        
        $_SESSION["alarm"]  = $alarm;
    }
    //==================================================================
    // 功能： 邮件报警测试
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function testEmail()
    {
    	$alarm_set = $this->save_alarm();
    	$send = array(
    	    "host"     =>  $_REQUEST["mailServer"],
    	    "user"     =>  $_REQUEST["mail_from"],
    	    "password" =>  $_REQUEST["mail_fromPass"],
    	    "type"     =>  $_REQUEST["identy_way"],
    	    "recv"     =>  $_REQUEST["newmail"],
    	);
    	$ret = $this->exInfo(CMD_SYS_EMAIL_TEST, $send, $recv); 
        if (!$ret) {
        	$_SESSION["sig"] = 1;
            $this->writeLog("SYSTEM","邮件测试失败".$recv["info"],"error");
            $this->ajaxReturn("/Event/index", $recv["info"], 0);
        }
        $_SESSION["sig"] = 1;
        $this->writeLog("SYSTEM","邮件测试成功","info");
        $this->ajaxReturn("/Event/index", "邮件测试成功!", 1);
    }
}
