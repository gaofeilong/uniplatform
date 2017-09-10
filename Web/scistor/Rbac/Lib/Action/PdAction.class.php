<?php
class PdAction extends CommonAction
{
    public function index()
    {
    	$this->unsetSig();   //删除保存在session里的内容
        $data = array(
            0 =>array(
                "caowei" => "1",
                "model"  => "EXT1",
                "wwn"    => "11111",
                "size"   => "1258",
                "vg"     => "Physical Volume",
                "raid"   => "RAID1",
                "speed"  => "70",
                "status" => "0",
            ),
            1 =>array(
                "caowei" => "2",
                "model"  => "EXT1",
                "wwn"    => "2222",
                "size"   => "1258",
                "vg"     => "Physical Volume",
                "raid"   => "RAID2",
                "speed"  => "70",
                "status" => "1",
            ),
            2 =>array(
                "caowei" => "3",
                "model"  => "EXT1",
                "wwn"    => "333333",
                "size"   => "1258",
                "vg"     => "Physical Volume",
                "raid"   => "RAID3",
                "speed"  => "70",
                "status" => "0",
            ),
            3 =>array(
                "caowei" => "4",
                "model"  => "EXT1",
                "wwn"    => "444444444",
                "size"   => "1258",
                "vg"     => "Physical Volume",
                "raid"   => "RAID4",
                "speed"  => "70",
                "status" => "1",
            ),
            4 =>array(
                "caowei" => "5",
                "model"  => "EXT1",
                "wwn"    => "55555555",
                "size"   => "1258",
                "vg"     => "Physical Volume",
                "raid"   => "RAID5",
                "speed"  => "70",
                "status" => "0",
            ),
            5 =>array(
                "caowei" => "6",
                "model"  => "EXT1",
                "wwn"    => "66666666",
                "size"   => "1258",
                "vg"     => "Physical Volume",
                "raid"   => "RAID6",
                "speed"  => "70",
                "status" => "1",
            )
        );
        
        $recv = array();
        $send = array();
        $info = array();
        /*if (!$this->exInfo(CMD_STORAGE_DISK_LOOKUP, $send, $recv)) {
            $this->error("获取物理磁盘信息失败: ".$recv["info"]);
        }*/   
        
        $jump_page = $_GET["jumpPage"];
        $currentPage=$_GET["currentPage"];
        $currentPage = $currentPage? $currentPage: 1;
        //判断是否输入跳转页
        $currentPage = $jump_page == ""? $currentPage: $jump_page;
        $pageSize=4;
        $totalRow = count($data);
        $artInfo = $this->getPageInfo($totalRow,$pageSize,$currentPage,$jump_page,$data);
        
        $this->assign("data",$artInfo);
        $this->display();
    }
}