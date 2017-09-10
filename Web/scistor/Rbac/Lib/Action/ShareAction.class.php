<?php
class ShareAction extends CommonAction
{
// ISCSI 操作
    public function index()
    {
        $send = array();
        $itlist = array();
        $unmaplun = array();
        $maplist = array();
        $chapusers = array();
        $allowhosts = array();
        $iqn = $_REQUEST['iqn'];
        $status = $this->exInfo(CMD_NAS_ISCSI_TARGET_LOOKUP, $send, $itlist);
        //echo "<pre>";print_r($itlist);
        if (!$status){
            $this->error("获取ISCSI页面失败: ".$recv["info"]);
        }
        //$this->exInfo(CMD_NAS_ISCSI_SHOWUNMAP, $send, $unmaplun);
        if (count($itlist['targetlist']) >= 1)
        {
            if ($iqn == "")
            {
                $send['iqn'] = $itlist['targetlist'][0];
            }
            else
            {
                $send['iqn'] = $iqn;
            }
            $iqn = $send['iqn'];
            $this->exInfo(CMD_NAS_ISCSI_CHAP_GET, $send, $chapusers);
            $this->exInfo(CMD_NAS_ISCSI_SHOWMAP, $send, $maplist);
            //$this->exInfo(CMD_NAS_ISCSI_GETALLOW, $send, $allowhosts);
        }
        //sort($maplist['lun']);
        $this->assign('iqn', $iqn);
        $this->assign('itlist', $itlist['targetlist']);
        //$this->assign('unmaplun', $unmaplun['unmaplun']);
        $this->assign('incominguser', $chapusers['incominguser']);
        $this->assign('outgoinguser', $chapusers['outgoinguser']);
        $this->assign('maplist', $maplist['lun']);
        $this->assign('today', date("Y-m"));
        $this->display();
    }
    public function AddTarget()
    {
        $iqn = $_REQUEST["iqn"];
        $data = array("iqn" => $iqn);
        //echo "<pre>";print_r($data);print_r($str);

        $status = $this->exInfo(CMD_NAS_ISCSI_TARGET_ADD, $data, $recv);
        if (!$status) {
            $this->writeLog("ISCSI","添加ISCSI-Target失败. iqn=".$iqn,"error");
            $this->ajaxReturn("/Share/index", $recv["info"], 0);  
        }
        $this->writeLog("ISCSI","添加ISCSI-Target成功. iqn=".$iqn,"info");
        $this->ajaxReturn("/Share/index&iqn=".$iqn, "添加ISCSI-Target成功!", 1);
    }
    public function DelTarget()
    {
        $iqn = $_REQUEST["iqn"];
        $data = array("iqn" => $iqn);
        //echo "<pre>";print_r($data);print_r($str);

        $status = $this->exInfo(CMD_NAS_ISCSI_TARGET_DEL, $data, $recv);
        if ($status) {
            $this->writeLog("ISCSI","删除ISCSI-Target成功. iqn=".$iqn,"info");
            $this->ajaxReturn("/Share/index", "删除ISCSI-Target ".$iqn." 成功!", 1);
        } else {
            $this->writeLog("ISCSI","添加ISCSI-Target失败. iqn=".$iqn,"error");
            $this->ajaxReturn("/Share/index&iqn=".$iqn, $recv["info"], 0);
        }
    }
    public function AddChap()
    {
        $iqn = $_REQUEST["iqn"];
        $data = array(
            "iqn" => $iqn,
            "user" => $_REQUEST["username"],
            "pwd" => $_REQUEST["passwd"],
            "type" => $_REQUEST["usertype"],
            );
        //echo "<pre>";print_r($data);print_r($str);

        $status = $this->exInfo(CMD_NAS_ISCSI_CHAP_ADD, $data, $recv);
        if ($status) {
            $this->writeLog("ISCSI","提交ISCSI-CHAP User成功. iqn=".$iqn,"info");
            $this->ajaxReturn("/Share/index&iqn=".$iqn, "提交ISCSI-CHAPUser成功!", 1);
        } else {
            $this->writeLog("ISCSI","提交ISCSI-CHAP User失败. iqn=".$iqn,"error");
            $this->ajaxReturn("/Share/index&iqn=".$iqn, $recv["info"], 0);
        }
    }
    public function DelChap()
    {
        $iqn = $_REQUEST["iqn"];
        $data = array("iqn" => $iqn);
        //echo "<pre>";print_r($data);print_r($_REQUEST);
        $data['users'] = $_REQUEST["users_del"];
        $data['usertype'] = 1;
        //echo $users_del;
        $status = $this->exInfo(CMD_NAS_ISCSI_CHAP_DEL, $data, $recv);
        if ($status) {
            $this->writeLog("ISCSI","删除ISCSI-CHAP User成功. iqn=".$iqn,"info");
            $this->ajaxReturn("/Share/index&iqn=".$iqn, "删除ISCSI-CHAP User成功!", 1);
        } else {
            $this->writeLog("ISCSI","删除ISCSI-CHAP User失败. iqn=".$iqn,"error");
            $this->ajaxReturn("/Share/index&iqn=".$iqn, $recv["info"], 0);
        }
    }
    public function AddLun()
    {
        $iqn = $_REQUEST["iqn"];
        $lun = $_REQUEST["lun"];
        $vchoice = $_REQUEST['Vchoice'];
        $data = array(
            "iqn" => $iqn,
            "lun" => array(),
        );
        $k = 0;

        for ($j = 0; $j < count($vchoice); ++$j)
        {
            for ($i = 0; $i < count($lun); $i++)
            {
                if ($lun[$i]['path'] == $vchoice[$j])
                {
                    $data['lun'][] = $lun[$i];
                }
            }
        }
//        echo "<pre>";print_r($data);

        $status = $this->exInfo(CMD_NAS_ISCSI_MAP, $data, $recv);
        if ($status) {
            $this->writeLog("ISCSI","添加ISCSI卷映射成功. iqn=".$iqn,"info");
            $this->ajaxReturn("/Share/index&iqn=".$iqn, "添加ISCSI卷映射成功!", 1);
        } else {
            $this->writeLog("ISCSI","添加ISCSI卷映射失败. iqn=".$iqn,"error");
            $this->ajaxReturn("/Share/index&iqn=".$iqn, $recv["info"], 0);
        }

    }
    public function DelLun()
    {
        $iqn = $_REQUEST["iqn"];
        $data = array("iqn" => $iqn);
        //echo "<pre>";print_r($data);print_r($str);

        $status = $this->exInfo(CMD_NAS_ISCSI_UNMAP, $data, $recv);
        if ($status) {
            $this->writeLog("ISCSI","删除ISCSI卷映射成功. iqn=".$iqn,"info");
            $this->ajaxReturn("/Share/index&iqn=".$iqn, "删除ISCSI卷映射成功!", 1);
        } else {
            $this->writeLog("ISCSI","删除ISCSI卷映射失败. iqn=".$iqn,"error");
            $this->ajaxReturn("/Share/index&iqn=".$iqn, $recv["info"], 0);
        }
    }
// FC 操作
    public function FCshare()
    {
        $tar = $_REQUEST["tar"];
        $send = array(
            "tar" => $tar,
        );
        $fcinfo = array();
        $status = $this->exInfo(CMD_NAS_FC_LOOKUP, $send, $fcinfo);
        //echo "<pre>";print_r($itlist);
        if (!$status){
            $this->error("获取FC页面失败: ".$recv["info"]);
        }
        $tar = $fcinfo['tar'];
        if ($tar == "")
        {
            $tar = $fcinfo['card'][0]['name'];
        }
        //sort($fcinfo['lun']);
        $this->assign('tar', $tar);
        $this->assign('card', $fcinfo['card']);
        $this->assign('lun', $fcinfo['lun']);
        $this->display();
    }
    public function FcMap()
    {
        //echo "<pre>";print_r($_REQUEST);
        $mapped = $_REQUEST['fcMapped'];
        $choice = $_REQUEST['choice'];
        $tar = $_REQUEST['tar'];
        //echo "<pre>";print_r($mapped);print_r($choice);print_r($newmaplun);
        $newmaplist = array();
        for ($i=0; $i<count($choice); ++$i)
        {
            $sel = $choice[$i];
            $newmaplist[$i] = $mapped[$sel];
        }
        $data = array(
            "tar" => $tar,
            "mapped" => $mapped,
            "newmap" => $newmaplist,
        );
        $recv = array();
        $status = $this->exInfo(CMD_NAS_FC_MAP, $data, $recv);
        if ($status) {
            $this->writeLog("FC","FC 卷映射成功. target=".$tar,"info");
            $this->ajaxReturn("/Share/FCshare&tar=".$tar, "FC 卷映射成功!", 1);
        } else {
            $this->writeLog("FC","FC 卷映射失败. target=".$tar,"error");
            $this->ajaxReturn("/Share/FCshare&tar=".$tar, $recv["info"], 0);
        }
        //$this->display();
    }
    public function FcUnmap()
    {
        //$this->display();
    }

// NAS share 操作
    public function NASshare()
    {
        $this->display();
    }
}
