<?php
// +----------------------------------------------------------------------
// | ThinkPHP [ WE CAN DO IT JUST THINK IT ]
// +----------------------------------------------------------------------
// | Copyright (c) 2009 http://thinkphp.cn All rights reserved.
// +----------------------------------------------------------------------
// | Licensed ( http://www.apache.org/licenses/LICENSE-2.0 )
// +----------------------------------------------------------------------
// | Author: liu21st <liu21st@gmail.com>
// +----------------------------------------------------------------------

class PublicAction extends CommonAction 
{
    public function login()
    {
    	$this->unsetSig();   //删除保存在session里的内容
        $this->display();
    }
    public function index(){
        //如果通过认证跳转到首页
        redirect(__APP__);
    }
    public function check()
    {
    	$user = M("user");
    	$uInfo = $user->findAll();
    	
    	$userName = $_REQUEST["user"];
    	$password = $_REQUEST["password"];
    	$verify = $_REQUEST["verifycode"];
        if ($userName != $uInfo[0]["username"]){
            $this->ajaxReturn('user',"用户名不存在",0);
        } elseif (md5($password) != $uInfo[0]["password"]) {
            $this->ajaxReturn('password',"密码错误",3);
        } elseif ($_SESSION['verify'] != md5(trim($verify))) {
            $this->ajaxReturn('verifycode',"验证码错误或为空",2);
        } 
        
        $time = time();
        $_SESSION['logOutTime'] = $time;
        $this->ajaxReturn('',"登录成功！",1);
    }
    
    public function loginout()
    {
        $this->redirect("/Public/login");
    }
    
    //随机生成验证码
    public function verify()
    {
        $type =  isset($_GET['type'])?$_GET['type']:'gif';
        import("@.ORG.Image");
        Image::buildImageVerify(4,1,$type);
    }
}
?>