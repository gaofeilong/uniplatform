<?php
class LvAction extends CommonAction
{
    public function index()
    {
    	$this->unsetSig();   //删除保存在session里的内容
        $send = array();
        $lvlist = array();
        $vglist = array();
        if (!$this->exInfo(CMD_STORAGE_LV_LOOKUP, $send, $lvlist)) {
            $this->error("获取逻辑卷列表失败: ".$vglist["info"]);
        }
        if (!$this->exInfo(CMD_STORAGE_VG_LOOKUP, $send, $vglist)) {
            $this->error("获取卷组列表失败: ".$vglist["info"]);
        }
        $lvlist = $lvlist["lvlist"];
        $vglist = $vglist["vglist"];
        for ($i=0; $i<count($lvlist); ++$i)
        {
            $arr = split("/", $lvlist[$i]['lvpath']);
            for ($j=0; $j<count($vglist); ++$j)
            {
                if ($arr[2] == $vglist[$j]['name'])
                {
                    $lvlist[$i]['vgfree'] = $vglist[$j]['free'];
                    break;
                }
                $lvlist[$i]['vgfree'] = 0;
            }
            $lvlist[$i]['name'] = $arr[3];
            $lvlist[$i]['vgname'] = $arr[2];
        }
        $this->assign("lvInfo",$lvlist);
        $this->assign("vgInfo",$vglist);
        $this->display();
    }
    //==================================================================
    // 功能： 卷扩容
    // 作者： ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function lvExtend()
    {
        $arr = $_REQUEST;
        $data = array(
            "lv_name"      => $arr["lv_name"],
            "usable_space" => $arr["usable_space"],
            "extendspace"  => $arr["extendspace"]
        );
        //echo "<pre>";
        //print_r($data);
        $status = $this->exInfo(CMD_STORAGE_LV_EXTEND, $data, $recv);
        if ($status) {
            $this->writeLog("LV","扩展逻辑卷成功. lv=".$arr["lv_name"],"info");
            $this->ajaxReturn("/Lv/index", "卷扩容成功!", 1);
        } else {
            $this->writeLog("LV","扩展逻辑卷失败. lv=".$arr["lv_name"],"error");
            $this->ajaxReturn("/Lv/index", $recv["info"], 0);
        }
    }
    //==================================================================
    // 功能： 新建卷
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function lvCreate()
    {
        $arr = $_REQUEST;
        $data = array(
            "lvgroup"   => $arr["lvgroup"],
            "lvnewname" => $arr["lvnewname"],
            "size"   => $arr["lvolume"]*1,
            "format"  => $arr["format"]
        );
        //echo "<pre>";
        //print_r($data);
        $status = $this->exInfo(CMD_STORAGE_LV_CREATE, $data, $recv);
        if ($status) {
            $this->writeLog("LV","创建逻辑卷成功. lv=".$arr["lvnewname"]." vg=".$arr["lvgroup"],"info");
            $this->ajaxReturn("/Lv/index", "新建卷成功!", 1);
        } else {
            $this->writeLog("LV","创建逻辑卷失败. lv=".$arr["lvnewname"]." vg=".$arr["lvgroup"],"error");
            $this->ajaxReturn("/Lv/index", $recv["info"], 0);
        }
    }
    //==================================================================
    // 功能： 删除卷
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function delLv()
    {
        $list = $_REQUEST["choice"];
        $data = array();//"lvlist" => $str);//explode(";",substr($str,0,strlen($str)-1));
        for ($i=0; $i<count($list); ++$i)
        {
            $data["lvlist"][$i]['path'] = $list[$i];
        }
        //echo "<pre>";print_r($data);print_r($str);

        $status = $this->exInfo(CMD_STORAGE_LV_REMOVE, $data, $recv);
        if ($status) {
            $this->writeLog("LV","删除逻辑卷成功. ","info");
            $this->ajaxReturn("/Lv/index", "卷删除成功!", 1);
        } else {
            $this->writeLog("LV","删除逻辑卷失败. ","error");
            $this->ajaxReturn("/Lv/index", $recv["info"], 0);
        }
    }
}