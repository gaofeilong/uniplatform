<?php
class LicenseAction extends CommonAction
{
	//==================================================================
    //功能：license首页
    //作者：wwpu
    //e-mail：wwpu@scidata.com
    //==================================================================
    public function index()
    {
    	$this->unsetSig();   //删除保存在session里的内容
    	$m      = M();
        $send   = array();
        $recv   = array();
        $nodeIp = $_REQUEST["nodeIp"];
        //在页面上显示IP列表
        $totalNode = $m->query("select ip from fbs_index_node");
        if (count($totalNode) < 1) {
            $indexArr[] = C('AGENT_ADDRESS');
            $ip         = C('AGENT_ADDRESS');
            $ign        = "single";
        } else {
            for ($i = 0; $i < count($totalNode); $i++) {
               $indexArr[] = $totalNode[$i]["ip"];
            }
            $ip = $nodeIp != '' ? $nodeIp : $totalNode[0]["ip"];
            $_SESSION["nodeIp"] = $ip;
            $ign = "distribute";
        }
        
        /*数据的显示*/
        $send = array(
            "agentIp" => $ip
        );
        if (!$this->exInfo(CMD_SYS_LICENSE_LOOKUP, $send, $recv)) {
            $this->error("获取license信息失败: ".$recv["info"]);
        }
        $time = time();
        $AvailABSTime = (int)$recv["AvailABSTime"] - $time; //截止时间转换为时间戳后和现在的时间差
        $AvailTime    = $recv["AvailTime"] * 60;
        $recv["ABSQuota"] = $this->format_bytes($recv["ABSQuota"] * 1024); //总的数据量转换单位
        $recv["CurQuota"] = $this->format_bytes($recv["CurQuota"] * 1024); //当前数据量转换单位
        if (count($totalNode) < 1) {
            //single状态
            if ($recv["Activated"] == 'y') {
                $recv['leftTime'] = 'unlimit'; 
                $recv["ABSQuota"] = $recv['QuotaPoStatus'] == 'y' ? $recv["ABSQuota"] : 'unlimit';  
            } else {
                if ($recv["ABSTimePoStatus"] == 'y' && $recv["TimePoStatus"] == 'n') {
                    $recv['leftTime'] = $this->format_time($AvailABSTime);
                } elseif ($recv["ABSTimePoStatus"] == 'n' && $recv["TimePoStatus"] == 'y') {
                	$recv['leftTime'] = $this->format_time($AvailTime);
                } elseif ($recv["ABSTimePoStatus"] == 'n' && $recv["TimePoStatus"] == 'n') {
                	$recv['leftTime'] = 'unlimit'; 
                } else {
                    //处理得到最小的那个时间
                    $recv['leftTime'] = $this->format_time(min($AvailABSTime,$AvailTime)); 
                }
                $recv["ABSQuota"] = $recv['QuotaPoStatus'] == 'y' ? $recv["ABSQuota"] : 'unlimit'; 
            }
        } else {
            //分布式的状态
            if ($recv["Activated"] == 'y') {
                $recv['leftTime'] = 'unlimit'; 
                $recv["ABSQuota"] = $recv['QuotaPoStatus'] == 'y' ? $recv["ABSQuota"] : 'unlimit';
            } else {
                if ($recv["ABSTimePoStatus"] == 'y' && $recv["TimePoStatus"] == 'n') {
                    $recv['leftTime'] = $this->format_time($AvailABSTime);
                } elseif ($recv["ABSTimePoStatus"] == 'n' && $recv["TimePoStatus"] == 'y') {
                    $recv['leftTime'] = $this->format_time($AvailTime);
                } elseif ($recv["ABSTimePoStatus"] == 'n' && $recv["TimePoStatus"] == 'n') {
                    $recv['leftTime'] = 'unlimit'; 
                } else {
                    //处理得到最小的那个时间
                    $recv['leftTime'] = $this->format_time(min($AvailABSTime,$AvailTime)); 
                }
                $recv["ABSQuota"] = $recv['QuotaPoStatus'] == 'y' ? $recv["ABSQuota"] : 'unlimit'; 
            }
        }
        
    	$this->assign("totalNode",count($totalNode));
    	$this->assign("ignlic",$ign);
    	$this->assign("ipname",$ip);
    	$this->assign("indexArr",$indexArr);
    	$this->assign("licdata",$recv);
        $this->display();
    }
    //==================================================================
    //功能：license上传功能（没有成功）
    //作者：ljwang
    //e-mail：wwpu@scidata.com
    //==================================================================
    function upLicense()
    {
        import ( "@.ORG.UploadFile" );
        $upload = new UploadFile();
        $upload->maxSiae=32922;
        //$upload->allowExts = explode(',','tar');   //上传文件扩展名为tar的文件
        $upload->uploadReplace = true;
        $upload->savePath='./Public/Uploads/';
        
        if (!$upload->upload()) {
            $this->ajaxReturn("", $upload->getErrorMsg(), 0);
        } else {
            /*$uploadList = $upload->getUploadFileInfo(); 
            if($uploadList[0]['savename'] !='upgrade_nas.tar'){
                echo '上传文件必须是upgrade_nas.tar';
            } else {
                echo "上传成功！";
            }*/
            $this->ajaxReturn("", "success", 1);
        }
    	
        /*//PHP自带的函数处理上传文件
		$myfile   = $_FILES ["update"];
		$name     = $myfile["name"];
		$filename = $myfile ["tmp_name"];
		$path     = "./Public/Uploads/".$name;
		
		if (move_uploaded_file($filename,$path)) {
		    $this->ajaxReturn("", "success", 1);
		    //echo "上传成功";
		} else {
		    $this->ajaxReturn("", "fail", 0);
		    //echo "上传失败";
		}*/
    }
    //==================================================================
    //功能：license上传功能(不适用)
    //作者：ljwang
    //e-mail：wwpu@scidata.com
    //==================================================================
    public function licimport()
    {
        import ( "@.ORG.UploadFile" );
        $upload  = new UploadFile();
        $picname = $_FILES['licname']['name'];  //文件名称
        $upload->allowExts = explode(',','lic');
        $upload->uploadReplace = true;
        $upload->savePath = './Public/Uploads/';
        $licPath     = '/tmp/'.$picname;
        $m = M();
        $totalNode   = $m->query("select ip from fbs_index_node");
        $ip          = $_SESSION["nodeIp"];
        if(!$upload->Upload()) {
            //$this->ajaxReturn("/License/index", $upload->getErrorMsg(), 0);
            $this->error($upload->getErrorMsg());
        }
        else{
            $uploadList = $upload->getUploadFileInfo();
            if (count($totalNode) < 1) {
                if ($uploadList[0]['savename'] != "scigw_ddfs.lic") {
                    //$this->ajaxReturn("/License/index", "License文件名称必须是scigw_ddfs.lic", 0); 
                    $this->error("License文件名称必须是scigw_ddfs.lic");
                }
            } else {
                if ($uploadList[0]['savename'] != "scigw_".$ip."_istri.lic") {
                    //$this->ajaxReturn("/License/index", 'License文件名称必须是scigw_'.$ip.'_istri.lic', 0);
                    $this->error("License文件名称必须是scigw_{$ip}_istri.lic");
                } 
            } 
        }
        
        $ret = $this->exInfo(CMD_SYS_LICENSE_IMPORT, $data, $recv);
        if ($ret) {
            $this->writeLog("SYSTEM","License文件导入成功","info");
            //$this->ajaxReturn("/License/index", "License文件导入成功", 1);
            $this->error("License文件导入成功");
        } else {
            $this->writeLog("SYSTEM","License文件导入失败".$recv["info"],"error");
            //$this->ajaxReturn("/License/index", $recv["info"], 0);
            $this->error("License文件导入失败".$recv["info"]);
        }

    }
    //==================================================================
    //功能：license上传功能
    //作者：ljwang
    //e-mail：wwpu@scidata.com
    //==================================================================
    public function uploadFile()
    {
    	$m         = M();
        $picname   = $_FILES['mypic']['name'];  //文件名称
	    $picsize   = $_FILES['mypic']['size']; 
	    $size      = sprintf("%.2f",$picsize / 1024); //转换成kb 
        $totalNode = $m->query("select ip from fbs_index_node");
        $ip        = $_SESSION["nodeIp"];
        if (count($totalNode) < 1) {
            if ($picname != "scigw_ddfs.lic") {
                echo 'the files name must be scigw_ddfs.lic!';
                exit;
            }
        } else {
            if ($picname != "scigw_{$ip}_istri.lic") {
                echo 'the files name must be scigw_'.$ip.'_istri.lic!'; 
                exit;  
            } 
            $data = array(
                "agentIp" => $ip,
            );
        }
        //上传路径 
        $pic_path = './Public/Uploads/'.$picname;
        $LicensePath='/tmp/'.$picname; 
        $arr = array( 
            'name' =>$picname, 
            //'size' =>$size,
        );
        if(file_exists($pic_path)){
            unlink($pic_path);
        }  
        move_uploaded_file($_FILES['mypic']['tmp_name'], $pic_path);
        $ret = $this->exInfo(CMD_SYS_LICENSE_IMPORT, $data, $recv);
        if (!$ret) {
        	$this->writeLog("SYSTEM","License文件导入失败","error");
            echo "the file of License import fail:".$recv["info"];
            exit;
        }
	        $this->writeLog("SYSTEM","License文件导入成功","info");
		    echo json_encode($arr); //输出json数据 
    }
    //==================================================================
    //功能：license导出功能
    //作者：ljwang
    //e-mail：wwpu@scidata.com
    //==================================================================
    public function licexport()
    {
    	$m = M();
        $totalNode   = $m->query("select ip from fbs_index_node");
        $ip          = $_SESSION["nodeIp"];
        if (count($totalNode) < 1) {
            $filename = "scigw_ddfs.lic";
        } else {
        	$filename = "scigw_{$ip}_istri.lic";
        	$data = array(
                "agentIp" => $ip,
            );
        } 
        $filedir = './Public/Uploads/' ;
        $licPath = '/tmp/';
        
        $ret = $this->exInfo(CMD_SYS_LICENSE_EXPORT, $data, $recv);
        if (!$ret) {
        	$this->writeLog("SYSTEM","License文件导出失败".$recv["info"],"error");
            $this->error("License文件导出失败:".$recv["info"]);
        } else {
	        if(!file_exists($filedir.$filename)){
	            $this->error("{$filename}不存在!");
	        } else {
	            $sta = 0;
	            $file = fopen($filedir.$filename, "r ");
	            Header( "Content-type:   application/octet-stream ");
	            Header( "Accept-Ranges:   bytes ");
	            Header( "Accept-Length:   ".filesize($filedir.$filename));
	            Header( "Content-Disposition:   attachment;   filename= ".$filename);
	            
	            echo fread($file,filesize($filedir.$filename));
	            fclose($file);
	            $this->writeLog("SYSTEM","License文件导出成功","info");
	        }
	    }
    }
}