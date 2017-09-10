<?php
class SyslogAction extends CommonAction
{
    public $pageSize = 11;
    public function index()
    {
        $this->unsetSig();   //删除保存在session里的内容
        $this->display();
    }
    public function templates()
    {
        $currentPage = isset($_GET['page'])?$_GET['page']:1;
        /*获取内容*/
        $log     = M("weblog");
        $startDate = $_REQUEST['startDate'];
        $endDate   = $_REQUEST['endDate'];
        $module    = $_REQUEST["logModel"];
        $type      = $_REQUEST["logType"];
        $keyword   = $_REQUEST["logkeyword"];
        //级别分开
        $levelinfo      = $_REQUEST["levelinfo"]? $_REQUEST["levelinfo"]: "info";
        $levelwarning   = $_REQUEST["levelwarning"]? $_REQUEST["levelwarning"]: "warning";
        $levelerror     = $_REQUEST["levelerror"]? $_REQUEST["levelerror"]: "error";
        
        if($startDate == '') {
            $startDate = '2000-01-01 00:00:00';
            $endDate   = '2150-01-01 00:00:00';
        }else{
            $startDate = $_REQUEST['startDate'].' 00:00:00';
            $endDate   = $_REQUEST['endDate'].' 23:59:59';
        }
        
        $_SESSION['b_date']       = $startDate;
        $_SESSION['e_date']       = $endDate;
        $_SESSION['module']       = $module;
        $_SESSION['levelinfo']    = $levelinfo;
        $_SESSION['levelwarning'] = $levelwarning;
        $_SESSION['levelerror']   = $levelerror;
        $_SESSION['keyword']      = $keyword;
        $_SESSION['logType']      = $type;
        
        //echo "startDate:".$_COOKIE['b_date']."**endDate:".$_COOKIE['e_date']."***module:".$_COOKIE['module']."**type:".$type."**keyword:".$_COOKIE['keyword']."**levelinfo:".$_COOKIE['levelinfo']."**levelwarning:".$_COOKIE['levelwarning']."**levelerror:".$_COOKIE['levelerror'];
        $content = $log->query("select * from weblog where module like '%{$module}%' and description like '%{$keyword}%' and (loglevel = '{$levelinfo}' or loglevel = '{$levelwarning}' or loglevel = '{$levelerror}') and (time >= '{$startDate}' and time <= '{$endDate}') order by time desc ");
        //dump($log->getLastSql());
        
        /*获取内容结束*/
        $first = ($currentPage - 1) * $this->pageSize;
        //总页数
        $totalPage = ceil(count($content)/$this->pageSize);
        if ($currentPage >= $totalPage){
            $currentPage = $totalPage;
        }
        if (count($content) == 0) {
            $currentPage = 1;
            $totalPage = 1;
        }
        $cont = array_slice($content,$first,$this->pageSize);
        $this->assign('currentPage',$currentPage);
        $this->assign('totalPage',$totalPage);
        $this->assign('dateInfo',$cont);
        $this->display();
    }
    //==================================================================
    // 功能：日志导出
    // 作者：ljwang
    // e-mail：ljwang@scidata.com
    //==================================================================
    public function down_log()
    {
        Header("Content-type:application/vnd.ms-excel");    
        Header("Content-Disposition:attachment;filename=log_".date("Ymd").".csv");    
        
        $model = M();
        
        $startDate    = $_SESSION['b_date'];
        $endDate      = $_SESSION['e_date'];
        $module       = $_SESSION['module'];
        $levelinfo    = $_SESSION['levelinfo'];
        $levelwarning = $_SESSION['levelwarning'];
        $levelerror   = $_SESSION['levelerror'];
        $keyword      = $_SESSION['keyword'];
        $type         = $_SESSION['logType'];
        
        //查询结果后 数据导出
        $ReportArr = $model->query("select * from weblog where module like '%{$module}%' and description like '%{$keyword}%' and (loglevel = '{$levelinfo}' or loglevel = '{$levelwarning}' or loglevel = '{$levelerror}') and (time >= '{$startDate}' and time <= '{$endDate}') order by time desc ");
        $len = count($ReportArr);
        $texcel = "编号,时间,级别,模块,内容\n";
        
        for ($i = 0; $i < $len; $i++) {
        $texcel .= ($i+1).",";
        $texcel .= $ReportArr[$i]['time'].",";
        $texcel .= $ReportArr[$i]['loglevel'].",";
        $texcel .= $ReportArr[$i]['module'].",";
        $info    = $ReportArr[$i]['description'];

        $info = str_replace('\r\n',' ',$info);
        $info = str_replace('\n',' ',$info);
        $info = str_replace(",","，",$info);
        
        $texcel .= $info.",";
        $texcel .= "\n";
        }
        //用的utf-8 最后转换一个编码为gb
        $texcel = iconv("utf-8","gb2312",$texcel);
        
        echo $texcel;
    }
}
