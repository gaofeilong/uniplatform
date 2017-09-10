<?php
class UpdateAction extends CommonAction
{
    public function index()
    {
    	$this->unsetSig();   //删除保存在session里的内容
        $this->display();
    }
    public function update()
    {
        header ( "content-type:text/html;charset=utf-8" );
        $myfile   = $_FILES ["updatefile"];
        $name     = $myfile["name"];
        $filename = $myfile ["tmp_name"];
        $path     = "./Public/Update/".$name;
        
        if (move_uploaded_file($filename,$path)) {
            $this->ajaxReturn("", "上传成功!", 1);
            //echo "上传成功";
        } else {
            $this->ajaxReturn("", "上传失败!", 0);
            //echo "上传失败";
        }
    }
    
    public function updateFile()
    {
        $picname = $_FILES['mypic']['name']; 
        $picsize = $_FILES['mypic']['size']; 
        $size = sprintf("%.2f",$picsize / 1024); //转换成kb 
        if ($picname != "") { 
            /*if ($picsize > 512000) { //限制上传大小 
                echo '图片大小不能超过500k'; 
                exit; 
            }*/ 
            $type = strstr($picname, '.'); //限制上传格式 
            if ($type != ".gif" && $type != ".jpg") { 
                echo 'the file type error!'; 
                exit; 
            }
            //$rand = rand(100, 999); 
            //$pics = date("YmdHis") . $rand . $type; //命名图片名称
            //上传路径 
            $pic_path = './Public/Update/'. $picname; 
            move_uploaded_file($_FILES['mypic']['tmp_name'], $pic_path); 
        }
        $arr = array( 
            'name' =>$picname, 
            'size' =>$size,
        );  
        echo json_encode($arr); //输出json数据 
    }
}