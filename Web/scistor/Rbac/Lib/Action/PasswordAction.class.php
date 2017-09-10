<?php
class PasswordAction extends CommonAction
{
    public function index()
    {
    	$this->unsetSig();   //删除保存在session里的内容
        $table = M("user");
        $username = $table->where("id=1")->getField("username");
        $password = $table->where("id=1")->getField("password");
        $this->assign("username", $username);
        $this->assign("password", $password);
        $this->display();
    }
    //==================================================================
    // 功能： 设置管理口令
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function setpwd()
    {
        $webName = $_POST['username'];
        $webPass = $_POST['password'];
        $webNewPass = $_POST['newPass'];

        $table = M("user");
        $username = $table->where("id=1")->getField("username");
        $password = $table->where("id=1")->getField("password");

        if (md5($webPass) != $password) {
            $this->ajaxReturn("", "原口令输入错误", 3);
        } 
        if ($table->where("id=1")->setField("password", md5($webNewPass)) == 1) {
        	$this->writeLog("SYSTEM","配置密码成功","info");
            $this->ajaxReturn("", "配置密码成功!", 1);
        } else {
        	$this->writeLog("SYSTEM","配置密码失败","error");
            $this->ajaxReturn("", "配置密码失败!", 0);
        }
    }
}
