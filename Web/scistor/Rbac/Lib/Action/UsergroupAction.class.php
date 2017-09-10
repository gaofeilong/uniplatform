<?php
class UsergroupAction extends CommonAction
{
    public function index()
    {
        $this->unsetSig();   //删除保存在session里的内容
        $status = $this->exInfo(CMD_NAS_AUTH_GET, $send, $recv);
        //echo "<pre>";print_r($itlist);
        if (!$status){
            $this->error("获取认证信息失败: ".$recv["info"]);
        }
        $this->assign('usertype', $recv['usertype']);
        $this->assign('domainname', $recv['domainname']);
        $this->assign('serverip', $recv['serverip']);
        $this->display();
    }
    public function SetAuth()
    {
        $send = array();
        $send['usertype'] = $_REQUEST["usertype"];
        if ($usertype > 2 )
        {
            $send['domainname'] = $_REQUEST['domainname'];
            $send['serverip'] = $_REQUEST['serverip'];
            $send['passwd'] = $_REQUEST['passwd'];
        }

        $status = $this->exInfo(CMD_NAS_AUTH_SET, $send, $recv);
        if (!$status)
        {
            $this->writeLog("AUTH","设置验证方式失败. type=".$send['usertype'],"error");
            $this->ajaxReturn("/Usergroup/index", $recv["info"], 0);  
        } else
        {
            $this->writeLog("AUTH","设置验证方式成功. type=".$send['usertype'],"info");
            $this->ajaxReturn("/Usergroup/index", "设置验证方式成功!", 1);
        }
    }
    public function grouplist()
    {
        $send = array();
        $status = $this->exInfo(CMD_NAS_AUTH_GROUP_GET, $send, $recv);
        //$this->assign('ulist', $recv['ulist']);
        $this->assign('glist', $recv['glist']);
        $this->assign('usertype', $recv['type']);
        $this->display();
    }
    public function groupadd()
    {
        $send = array(
            'glist' => $_REQUEST['glist'],
            //'type' => $_REQUEST['usertype'],
        );
        $status = $this->exInfo(CMD_NAS_AUTH_GROUP_ADD, $send, $recv);
        if (!$status)
        {
            $this->writeLog("AUTH","添加组失败. ","error");
            $this->ajaxReturn("/Usergroup/grouplist", $recv["info"], 0);  
        } else
        {
            $this->writeLog("AUTH","添加组成功. ","info");
            $this->ajaxReturn("/Usergroup/grouplist", "添加组成功!", 1);
        }
    }
    public function groupdel()
    {
        $glist = $_REQUEST['gcheck'];
        $send = array();
        for ($i=0; $i<count($glist); ++$i)
        {
            $send['glist'][$i]['gname'] = $glist[$i];
        }
        $status = $this->exInfo(CMD_NAS_AUTH_GROUP_DEL, $send, $recv);
        if (!$status)
        {
            $this->writeLog("AUTH","删除组失败. ","error");
            $this->ajaxReturn("/Usergroup/grouplist", $recv["info"], 0);  
        } else
        {
            $this->writeLog("AUTH","删除组成功. ","info");
            $this->ajaxReturn("/Usergroup/grouplist", "删除组成功!", 1);
        }
    }
    public function userlist()
    {
        $send = array();
        $status = $this->exInfo(CMD_NAS_AUTH_USER_GET, $send, $recv);
        $this->assign('ulist', $recv['ulist']);
        $this->assign('glist', $recv['glist']);
        $this->assign('usertype', $recv['type']);
        $this->display();
    }
    public function useradd()
    {
        $send = array(
            'ulist' => $_REQUEST['ulist'],
            //'type' => $_REQUEST['usertype'],
        );
        for ($i=0; $i<count($send['ulist']); ++$i)
        {
            if ($_REQUEST['passwd1'][$i] == $_REQUEST['passwd2'][$i])
            {
                $send['ulist'][$i]['pwd'] = $_REQUEST['passwd1'][$i];
            }
            else
            {
                $this->ajaxReturn("/Usergroup/userlist", $send['ulist'][$i]['uname']."两次密码输入不一致", 2);
            }
        }
        $status = $this->exInfo(CMD_NAS_AUTH_USER_ADD, $send, $recv);
        if (!$status)
        {
            $this->writeLog("AUTH","添加用户失败. ","error");
            $this->ajaxReturn("/Usergroup/userlist", $recv["info"], 0);
        } else
        {
            $this->writeLog("AUTH","添加用户成功. ","info");
            $this->ajaxReturn("/Usergroup/userlist", "添加用户成功!", 1);
        }
    }
    public function userdel()
    {
        $ulist = $_REQUEST['ucheck'];
        $send = array();
        for ($i=0; $i<count($ulist); ++$i)
        {
            $send['ulist'][$i]['uname'] = $ulist[$i];
        }
        $status = $this->exInfo(CMD_NAS_AUTH_USER_DEL, $send, $recv);
        if (!$status)
        {
            $this->writeLog("AUTH","删除用户失败. ","error");
            $this->ajaxReturn("/Usergroup/userlist", $recv["info"], 0);  
        } else
        {
            $this->writeLog("AUTH","删除用户成功. ","info");
            $this->ajaxReturn("/Usergroup/userlist", "删除用户成功!", 1);
        }
    }
    public function groupquota()
    {
        $this->display();
    }
    public function userquota()
    {
        $this->display();
    }
}