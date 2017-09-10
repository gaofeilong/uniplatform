<?php
class CommonAction extends Action {
    private $m_Operation;

    function _initialize() {
        require 'Rbac/Common/Protocol.php';
        $this->m_Operation = $OPERATION;
    }
    //==================================================================
    // 功能：在其他页面删除保存在session里的内容
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function unsetSig()
    {
    	unset($_SESSION["sig"]);
    }
    //==================================================================
    // 功能：分页公共部分
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function getPageInfo($totalRow,$pageSize, $currentPage, $jump_page,$data){

        $totalPage = ceil($totalRow / $pageSize);
        $totalPage = $totalPage == 0 ? 1 : $totalPage;
        $first = ($currentPage - 1) * $pageSize;
        $last = min($first + $pageSize, $totalRow);
	    for ($i = $first;$i < $last;$i++)
        {
            $articles[] = $data[$i];
        }
        $this->assign("previousPage",$currentPage - 1);
        $this->assign("currentPage",$currentPage);
        $this->assign("nextPage",$currentPage + 1);
        $this->assign("lastPage",$totalPage);
        $this->assign("jump_page",$jump_page);
        return  $articles; 
    } 
    
    //==================================================================
    // 功能：分页公共部分:如果一页面存在两个分页
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function getPageInfonew($totalRow,$pageSize, $currentPage, $jump_page,$data){

        $totalPage = ceil($totalRow / $pageSize);
        $totalPage = $totalPage == 0 ? 1 : $totalPage;
        $first = ($currentPage - 1) * $pageSize;
        $last = min($first + $pageSize, $totalRow);
        for ($i = $first;$i < $last;$i++)
        {
            $articles[] = $data[$i];
        }
        $this->assign("previousPagenew",$currentPage - 1);
        $this->assign("currentPagenew",$currentPage);
        $this->assign("nextPagenew",$currentPage + 1);
        $this->assign("lastPagenew",$totalPage);
        $this->assign("jump_pagenew",$jump_page);
        return  $articles; 
    }
    //==================================================================
    // 功能：处理路径中多个斜杠的问题
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function handPath($path)
    {
    	$divide = "/";
        $data = explode("/",$path);
        for ($i = 0; $i < count($data); $i++) {
           if ($data[$i] != ""){
               $divide .= $data[$i]."/";
           }
        }
        $result = substr($divide,0,(strlen($divide) - 1));
    	return $result;
    }
    
    //==================================================================
    // 功能：封装退出，（附加提示信息）
    // 参数：1、提示信息
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    /*public function toLogin1($info){
        if(isset($_SESSION[C('USER_AUTH_KEY')])) {

            $this->assign("jumpUrl" , "__URL__/index");
            unset($_SESSION[C('USER_AUTH_KEY')]);
            unset($_SESSION);
            session_destroy();

            $this->success ($info);
            $this->redirect('Public/login');

        }else {
            $this->error('已经登出，请刷新页面！');
        }
    }*/

    public function toLogin($info)
    {
    	$this->assign("jumpUrl" , "__APP__/Public/loginout");
    	$this->error($info);
        $this->redirect("/Public/login");
    }
    //==================================================================
    // 功能：超时功能
    // 参数：1、提示信息
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function timeOut()
    {
    	$time   = time();
        if($_SESSION['logOutTime'] < $time - 60) {
            return 1;
        } else {
        	$_SESSION['logOutTime'] = $time;
        }
    }
    //==================================================================
    // 功能：单位换算
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    function format_bytes($size) {
        $dw = array('byte','KB','MB', 'GB', 'TB' , 'PB');
        for ($i = 0; $size >= 1024 && $i < 5; $i++){ 
            $size /= 1024;
        }
        return sprintf("%.2f",$size)." ".$dw[$i];
    } 
    //==================================================================
    // 功能：时间单位换算
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    function format_time($size) {
        $dw = array('秒','分钟','小时', '天');
        for ($i = 0; $size >= 24 && $i < 3; $i++){ 
            if ($i > 1) {
               $size /= 24;
            } else {
                $size /= 60;
            }
        }
        return sprintf("%.2f",$size)." ".$dw[$i];
    }
    //==================================================================
    // 功能：  将页面和后台协议交互的操作的日期、时间、模块、操作、结果、
    //      事件界别记录到数据库。
    // 作者：  作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    function writeLog($module, $desc, $level)
    {
        $logTable = M("weblog");
        $time = date("y-m-d H:i:s");
        $log = array(
            "time"        => $time,
            "module"      => $module,
            "description" => $desc,
            "loglevel"    => $level,
        );
        $logTable->add($log);
    }
    
    //==================================================================
    // 功能：  与服务器进行数据交换
    // 作者： wlj
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function exInfo($code, $send, &$recv, $errInfo)
    {
    	/* $time   = time();
        if($_SESSION['logOutTime'] < $time - 13315820035) {
            $this->toLogin("系统超时，请重新登录!");
        } else {
            $_SESSION['logOutTime'] = $time; }*/
            // set_time_limit(0);
    	    //$send['agentIp'] = C('AGENT_ADDRESS');
            // 如果send['agentIp']为空，设置为C('AGENT_ADDRESS');
    	    $send['agentIp'] = $send['agentIp'] == '' ? C('AGENT_ADDRESS') : $send['agentIp'];
            $body = json_encode($send);
            $head = pack("ccii", C('MAGIC'), C('VERSION'), $code, strlen($body));
    
            // socket communication
            $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
            if (!$socket) {
                $recv = array(
                    "info" => socket_strerror(socket_last_error()),
                );
                return false;
            }
            $ret = socket_connect($socket, C('SOCKET_ADDRESS'), C('SOCKET_PORT'));
            if (!$ret) {
                $recv = array(
                    //"info" => socket_strerror(socket_last_error()),
                    "info" => "socket connect [".$send['agentIp']."] error",
                );
                return false;
            }
            
            
            //echo "connect to ".C('SOCKET_ADDRESS').":".C('SOCKET_PORT')."<br>";
            $ret = socket_write($socket, $head, strlen($head));
            if (!$ret) {
                $recv = array(
                    "info" => socket_strerror(socket_last_error()),
                );
                return false;
            }
            if (strlen($body)) {
                $ret = socket_write($socket, $body, strlen($body));
                if (!$ret) {
                    $recv = array(
                        "info" => socket_strerror(socket_last_error()),
                    );
                    return false;
                }
            }
            //把包头和包体合在一起写入
            /*$pack = $head.$body;
            $ret = socket_write($socket, $pack, strlen($head)+strlen($body));
            if (!$ret) {
                $recv = array(
                    "info" => socket_strerror(socket_last_error()),
                );
                return false;
            }*/
            
            $rHead = socket_read($socket, strlen($head));
            if (!$rHead) {
                $recv = array(
                    "info" => socket_strerror(socket_last_error()),
                );
                return false;
            } 
    
            // char1: 'B', char2: '1', int1: CMD_OK/CMD_ERR, int2: length
            //$rHead = array_merge(unpack("c2char/i2int", $rHead));
            $rHead = unpack("c2char/i2int", $rHead);
            if ($rHead["int2"]) {
                $rBody = socket_read($socket, $rHead["int2"]);
                if (!$rBody) {
                    $recv = array(
                        "info" => socket_strerror(socket_last_error()),
                    );
                    return false;
                } 
                $rBody = json_decode($rBody, true);
            }
            socket_close($socket);     
            if ($rHead["int1"] == CMD_OK) {
                $recv = $rBody;
                return true;
            } elseif ($rHead["int1"] == CMD_ERR) {
                $recv = array(
                    "info" => $rBody["info"],
                );
                return false;
            } else {
                $recv = array(
                    "info" => "server error",
                );
                return false;
            }
    }
    
    //==================================================================
    // 功能：  与服务器进行数据交换 + 记录日志
    // 作者：  gfl
    // e-mail：flgao@scidata.com
    //==================================================================
    public function testexInfo($code, $send, &$recv, $errInfo = "")
    {
        $ret = $this->doExInfo($code, $send, $recv, $errInfo);

        $level  = $ret? "info": "error";
        $module = $this->m_Operation[$code & MODULE_MASK];
        $desc   = $this->m_Operation[$code].($ret?"成功":"失败, ".$recv["info"]);
        $this->writeLog($module, $desc, $level);

        return $ret;
    }
}
?>
