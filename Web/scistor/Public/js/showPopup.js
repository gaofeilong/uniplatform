/**卷组管理***/
jQuery(document).ready(function(){
    //卷组扩容
    jQuery(".extends").click(function(){
        publicP('extend_vols','extend','卷组扩容');
    });
    
    //新建卷组
    jQuery(".build_newvols").click(function(){
        publicP('new_vols','newvol','新建卷组');
    });
    
    //删除卷组
    jQuery(".delvols").click(function(){
    	var a=jQuery("input[name='choice']");
        var addTable = document.getElementById("tablevols");
        var cbox;//表格索引
        var vols_no = "";//定义删除弹出框 删除内容
        for(var j = 1;j < addTable.rows.length;j++){
            cbox = addTable.rows[j].childNodes[0].childNodes[0];
            if(cbox.checked && cbox.disabled == false){
            	vols_no += jQuery(".volsname").eq(j-1).html() + ";";
            }
        }
        jQuery("#vgDelname").val(vols_no);
        publicPDel('choice','tablevols','卷组','vols_no','volsname','del_vols','delvololume','删除卷组');
    });
 
    //新建卷
    jQuery(".build_newvol").click(function(){
        publicP('new_vol','newvol','新建卷');
    });
    
    //删除卷
    jQuery(".delvol").click(function(){
    	var a=jQuery("input[name='choice[]']");
        var addTable = document.getElementById("tablevol");
        var cbox;//表格索引
        var vols_no = "";//定义删除弹出框 删除内容
        for(var j = 1;j < addTable.rows.length;j++){
            cbox = addTable.rows[j].childNodes[0].childNodes[0];
            if(cbox.checked && cbox.disabled == false){
            	vols_no += jQuery(".volname").eq(j-1).html() + ";";
            }
        }
        jQuery("#lvDelname").val(vols_no);
    	
        publicPDel('choice[]','tablevol','逻辑卷','volv_del','volname','del_vol','delvolume','删除卷');
    });

/***NAS共享管理 》NAS共享*****/
    //共享目录详细信息
    jQuery(".ns_more").click(function(){
        publicP('nsMore','nas_more','共享目录详细信息');
    });
    
    //新建共享
    jQuery(".build_newns").click(function(){
        publicP('new_ns','new_nsModel','新建NAS共享');
    });
    /*
    //编辑共享
    jQuery(".ns_edit").click(function(){
        publicP('edit_ns','edit_nsModel','编辑NAS共享');
    });
    */
    
    //删除共享
    jQuery(".delns").click(function(){
        var a=jQuery("input[name='choice']");
        var addTable = document.getElementById("nsTable");
        var cbox;
        var num=0;
        var na=0;
        var vols_no = "";
        for (var i=0; i<a.length; i++){
            if(a[i].checked){
                num++;
            }
        }
        if(num == 0){
            alert("未选中任何NAS共享目录！");
            return false;
        }else {
            if(num == a.length){
                jQuery("#delNs_list").html("&nbsp;全部NAS共享目录?");
            }else{
                for(var j = 1;j < addTable.rows.length;j++){
                    cbox = addTable.rows[j].childNodes[0].childNodes[0];
                    if(cbox.checked){
                        vols_no += "&nbsp;" + jQuery(".nsConList").eq(j-1).html() + "&nbsp;<br />";//将取到的共享目录的内容赋给nsConList
                    }
                }
                jQuery("#delNs_list").html(vols_no);
            }
            publicP('del_ns','del_nsModel','删除NAS共享目录');
        }
    });
    
    /***NAS共享管理 》FC共享*****/
    //新建FC导出卷
    jQuery(".build_newfc").click(function(){
        publicP('new_fc','newFC','新建FC导出卷');
    });
    
    //删除FC导出卷
    jQuery(".delfc").click(function(){
        publicPDel('choice','fc_table','FC导出卷','delFC_list','lv','del_fc','delvololume','删除FC导出卷');
        
    });
    
    /*******NAS共享 》ISCSI共享*******/
    //新建ISCSI target
    jQuery(".build_is").click(function(){
        publicP('new_is','newIs','新建ISCSI');
    });
    
    //删除ISCSI target
    jQuery(".delIs").click(function(){
        //var sel = document.getElementById("select_info_is_sel").innerHTML;
        var sel = document.getElementById("is_sel").value;
        jQuery("#delIs_list").html(sel);
        publicP('del_is','delete_is','删除ISCSI');
    });
    
    //<!--新建ISCSI 用户-->
    jQuery(".build_isU").click(function(){
        publicP('new_isU','newIsU','新建 ISCSI CHAP 用户');
    });
    
    //删除ISCSI 用户
    jQuery(".delIsU").click(function(){
    /*对应的参数分别是，checkbox的name属性值，表格的id值，
        *提示信息，弹出框内容id值，要显示的内容class值（表格中），
        *弹出层内容id值，给弹出层id值，弹出层标题*/
        var a=jQuery("input[name='Uchoice']");
        var addTable = document.getElementById("is_user");
        var cbox;//表格索引
        var vols_no = "";//定义删除弹出框 删除内容
        for(var j = 1;j < addTable.rows.length;j++){
            cbox = addTable.rows[j].childNodes[0].childNodes[0];
            if(cbox.checked && cbox.disabled == false){
                vols_no += cbox.value + ";";
            }
        }
        //alert(vols_no);
        jQuery("#users_del").val(vols_no);

        publicPDel('Uchoice','is_user','iscsi用户','delIsU_list','isUser','del_isU','delete_isU','删除ISCSI用户');
    });
    
    //新建 iscsi卷映射关系
    jQuery(".build_isV").click(function(){
        publicP('new_isV','newIsV','新建ISCSI映射');
    });
    
    //删除 iscsi卷映射关系
    jQuery(".delIsV").click(function(){
        publicPDel('Vchoice','is_vol','iscsi卷映射','delIsV_list','isVol','del_isV','delete_isV','删除ISCSI卷映射');
        
    });
    
    /******消冗文件系统弹出框*********/
    
    //删除挂载点
    jQuery(".delddfs").click(function(){
    	
    	var a=jQuery("input[name='choice']");
        var addTable = document.getElementById("serviceTable");
        var cbox;//表格索引
        var vols_no     = "";//定义删除弹出框 删除内容
        var mount_point = "";
        var systemType  = jQuery("#systemType").val();
        for(var j = 1;j < addTable.rows.length;j++){
            cbox = addTable.rows[j].childNodes[0].childNodes[0];
            if(cbox.checked && cbox.disabled == false){
            	if (systemType == 'ddfs') {
            		vols_no += jQuery(".ddfsconfig").eq(j-1).val() + ";";
            	} else {
            		vols_no += jQuery(".firstDataPath").eq(j-1).val() + ";";
            	}
            	mount_point += jQuery(".mount_point").eq(j-1).text() + ";";
            }
        }
        
        jQuery("#delSystemType").val(systemType);
        jQuery("#delconfig").val(vols_no);
        jQuery("#delmount_point").val(mount_point);
    	
        publicPDel('choice','serviceTable','挂载点','delMount_list','mount_point','del_ddfs','delDdfs','删除挂载点');
        
    });
});
//新建挂载点
function addDdfsnode()
{
	var fbsnode = jQuery("#fbsnode").val() * 1;
    var sysType = document.getElementById("systemType");//判断 系统类型;
    var selType = document.getElementById("mountWay");
	if (fbsnode < 1) {
        mountDdfs("mountWay","superBtn","superDiv");
		publicP('new_ddfs','newDdfs','新建挂载点');
        if (sysType.value == "adfs") {
            selType.value = "adfs";
            selType.disabled = true;
        } else if (sysType.value == "ddfs") {
            selType.value = "ddfs";
            selType.disabled = true;
        }

    } else {
    	alert("无法添加单机消冗系统，已经配置分布式节点！");
    }
}
function mountDdfs(sel, btn, sdiv)
{
    var mountWay = document.getElementById(sel);
    var delImg;
    
    if(sel == "mountWay") {
        delImg = jQuery("#dataPathTab").find("img").eq(0);
        mpath  = jQuery("#mountPoint");
        tip    = jQuery("#adfsTip");
    }
    if (mountWay.value == "adfs") {
        document.getElementById(btn).disabled = true;
        document.getElementById(btn).value = "高级选项";
        showSuper(btn, sdiv);
        if (sel == "mountWay") {
            tip.css("display","");
            delImg.css("visibility","hidden");
            mpath.attr("disabled",true);
        }
    } else {
        document.getElementById(btn).disabled = false;
        if (sel == "mountWay") {
            tip.css("display","none");
            delImg.css("visibility","visible");
            mpath.attr("disabled",false);
        }
    } 
}

/**卷管理*****/
//卷扩容
function popupExtendLv(lvpath, size){
    //var lv_name = jQuery(this).parent().parent().children()[0];
    //var name = jQuery(lv_name).text();
    //var vgfree = jQuery("#vgfree"+line).val();
    jQuery("#lv_name").val(lvpath);
    jQuery("#usable_space").val(size);
    publicP('extend_div','extend','卷扩容');
}
function popupFormat(lvpath, size){
    //var lv_name = jQuery(this).parent().parent().children()[0];
    //var name = jQuery(lv_name).text();
    //var vgfree = jQuery("#vgfree"+line).val();
    jQuery("#lv_name").val(lvpath);
    jQuery("#usable_space").val(size);
    publicP('extend_div','extend','卷扩容');
}
function popupEditChap(user, pwd, usertype)
{
    jQuery("#editUserName").val(user);
    jQuery("#editUserPass").val(pwd);
    jQuery("#editUserVPass").val(pwd);
    jQuery("#usertype").val(usertype);
    publicP('edit_isU','editIsU','修改ISCSI CHAP 用户密码');
}
function popupItLun()
{
    //publicPDel('choice','fc_table','FC导出卷','delFC_list','lv','del_fc','delvololume','删除FC导出卷');
}
function popupGroupDel()
{
    publicPDel('gcheck[]','tab_group','任何用户组','del_group','groupname',"del_grp","delvololume","删除用户组");
}
function popupUserDel()
{
    publicPDel('ucheck[]','tab_group','任何用户','del_user','username',"del_user","delVololume","删除用户");
}



//***DdFs页面****//
function Ddfsbuild_row()
{
    var dataPath = document.getElementsByName("dataPath[]");
    for(var i = 0;i < dataPath.length; i++){
        if(dataPath[i].value == ""){
            alert("数据存储路径不能为空！");
            dataPath[i].focus();
            return false;
        }
    }
    
    var e = event.srcElement ? event.srcElement.parentNode : event.target.parentNode;//js三目运算
     //表格图片片段
    var row = e.parentNode.sectionRowIndex;
      // alert(row);
       //return;
     //图片判断结束
    var len = dataPathTab.rows.length;
    var new_row=dataPathTab.insertRow(dataPathTab.rows.length);
    var new_col01=new_row.insertCell(0);
    var new_col02=new_row.insertCell(1);
    new_col01.innerHTML='<input type="text" name="dataPath[]" id="dataPath" class="s_width250new" onkeyup="pathHand();dpFirstVal(\'mountPoint\')"/>';
    new_col02.innerHTML='<img src="/scistor/Public/img/clear.gif" align="absmiddle" class="img_delete1" onclick = "Ddfsdelete_row()" title="删除数据存储路径"/>';

    var j = dataPath.length - 1; //得到新增的input，以便获得焦点
    dataPath[j].focus();
}
function Ddfsdelete_row()
{
    var deldataPath=document.getElementsByName("dataPath[]");
    var e = event.srcElement ? event.srcElement.parentNode : event.target.parentNode;//js三目运算
    if(deldataPath.length > 1){
        var row  = e.parentNode.sectionRowIndex;
        var cell = e.cellIndex;
        if(row < 0){
              alert("操作无效");
            return;
        }
        //如果为空并且count>1删除该编辑框时不需要提示
        var con = 1;
        if(con == 1){
            if(e.tagName == "TD"){
                dataPathTab.deleteRow(row - 0);
            }
        }
        pathHand();
    } else {
        alert("不能全部删除");
    }
}
function forinfo(typeId,json_name,json_type)
{
    var Type = document.getElementById(typeId);
    for(var i = 0;i < Type.options.length;i++)
    {
        if(Type.options[i].value == json_type[json_name])
        {
            Type.options[i].selected = "selected";
        }
    }
}
//编辑挂载点
function edit_Ddfs(id)
{
    var recv_info = jQuery("#json_recv"+id).text();
    var json_recv = eval('('+recv_info+')');
    
    jQuery("#e_mountPoint").val(json_recv['mountPoint']);
    jQuery("#e_mountPoint").attr("disabled","disabled");
    
    jQuery("#e_metaPath").val(json_recv['metaPath']);
    
    jQuery("#e_mapPath").val(json_recv['mapPath']);
    
    jQuery("#e_biPath").val(json_recv['biPath']);
    
    jQuery("#e_bucketCount").val(json_recv['bucketCount']);
    
    jQuery("#e_minSpace").val(json_recv['minSpace']);
    
    forinfo("e_fileTypeCount","fileTypeCount",json_recv);
    
    forinfo("e_blockSize","blockSize",json_recv);
    
    forinfo("e_chunkSize","chunkSize",json_recv);
    
    forinfo("e_dedupOption","dedupOption",json_recv);
    
    forinfo("e_crcOption","crcOption",json_recv);
    
    forinfo("e_byteCmpOption","byteCmpOption",json_recv);
    
    forinfo("e_hashType","hashType",json_recv);
    
    forinfo("e_compressType","compressType",json_recv);
    
    //forinfo("e_byteDedup","byteDedup",json_recv);
    //jQuery("#e_byteDedup").attr("disabled","disabled");
    
    forinfo("e_dplCpuNum","dplCpuNum",json_recv);
    
    forinfo("e_dplTimeOutSec","dplTimeOutSec",json_recv);
    
    forinfo("e_dplReadMode","dplReadMode",json_recv);
    
    forinfo("e_dplCapacity","dplCapacity",json_recv);
    
    jQuery("#e_fsBucketCount").val(json_recv['fsBucketCount']);
    jQuery("#e_mmapSize").val(json_recv['mmapSize']);
    
    forinfo("e_raStrategy","raStrategy",json_recv);
    //jQuery("#e_raSize").val(json_recv['raSize']);
    jQuery("#e_raChunkCnt").val(json_recv['raChunkCnt']);
    jQuery("#e_maxDelayedCloseNum").val(json_recv['maxDelayedCloseNum']);
    jQuery("#e_delayedCloseTime").val(json_recv['delayedCloseTime']);
    
    var editddfs_hid = jQuery("#ddfsconfig"+id).val();
    jQuery("#editddfs_hid").val(editddfs_hid);
    
    //显示多条数据路径
    var editdata = jQuery("#pathInfo"+id).children("span");
    //因弹框是隐藏不是消失，故每次显示归档路径时先删除之前点击按钮多显示的归档路径
    while(editPathTab.hasChildNodes()){
        editPathTab.removeChild(editPathTab.lastChild);
    }
    for(var i = 0;i < editdata.length;i++)
    {
        var path_data = editdata.eq(i).text();
        //把获取到的已归档路径，显示在编辑框里
        var new_row=editPathTab.insertRow(editPathTab.rows.length);
        var new_col01=new_row.insertCell(0);
        new_col01.innerHTML='<input type="text" name="e_dataPath[]" id="e_dataPath" class="s_width250new" value="'+path_data+'" readonly/>'+'<input type="hidden" value="'+id+'" id="ddfsId" />';
    }
    publicP('edit_ddfs','editDdfs','编辑挂载点');
    mountDdfs("e_mountWay", "e_superBtn","e_superDiv");
}
//编辑页面添加路径
function editDdfs_row()
{
    var e_dataPath = document.getElementsByName("e_dataPath[]");
    for(var i = 0;i < e_dataPath.length; i++){
        if(e_dataPath[i].value == ""){
            alert("数据存储路径不能为空！");
            e_dataPath[i].focus();
            return false;
        }
    }
    
    var e = event.srcElement ? event.srcElement.parentNode : event.target.parentNode;//js三目运算
     //表格图片片段
    var row = e.parentNode.sectionRowIndex;
      // alert(row);
       //return;
     //图片判断结束
    var len = editPathTab.rows.length;
    var new_row=editPathTab.insertRow(editPathTab.rows.length);
    var new_col01=new_row.insertCell(0);
    var new_col02=new_row.insertCell(1);
    new_col01.innerHTML='<input type="text" name="e_dataPath[]" id="dataPath" class="s_width250new" onkeyup="pathHand()"/>';
    new_col02.innerHTML='<img src="/scistor/Public/img/clear.gif" align="absmiddle" class="img_delete1" onclick = "e_delDdfs()" title="删除数据存储路径"/>';

    var j = e_dataPath.length - 1; //得到新增的input，以便获得焦点
    e_dataPath[j].focus();
}
//编辑页面删除路径
function e_delDdfs()
{
    var e_deldataPath=document.getElementsByName("e_dataPath[]");
    var e = event.srcElement ? event.srcElement.parentNode : event.target.parentNode;//js三目运算
    var row  = e.parentNode.sectionRowIndex;
    var cell = e.cellIndex;
    if(row < 0){
          alert("操作无效");
        return;
    }
    //如果为空并且count>1删除该编辑框时不需要提示
    var con = 1;
    if(con == 1){
        if(e.tagName == "TD"){
            editPathTab.deleteRow(row - 0);    
        }
    }
}
function e_pathHand()
{
    var e_dataPath = jQuery("#e_dataPath").val();
    var e_metaPath = e_dataPath + '/metaPath';
    var e_mapPath  =  e_dataPath + '/mapPath';
    var e_biPath   =  e_dataPath + '/biPath';
    jQuery("#metaPath").val(e_metaPath);
    jQuery("#mapPath").val(e_mapPath);
    jQuery("#biPath").val(e_biPath);
    if(e_dataPath == ""){
        jQuery("#metaPath").val("");
        jQuery("#mapPath").val("");
        jQuery("#biPath").val("");
    }
}
//详细信息挂载点
function detail_Ddfs(id)
{
    var d_recvinfo = jQuery("#json_recv"+id).text();
    var d_jsonrecv = eval('('+d_recvinfo+')');
    
    jQuery("#d_mountPoint").val(d_jsonrecv['mountPoint']);
    jQuery("#d_metaPath").val(d_jsonrecv['metaPath']);
    jQuery("#d_mapPath").val(d_jsonrecv['mapPath']);
    jQuery("#d_biPath").val(d_jsonrecv['biPath']);
    jQuery("#d_bucketCount").val(d_jsonrecv['bucketCount']);
    jQuery("#d_minSpace").val(d_jsonrecv['minSpace']);
    
    jQuery("#d_dplReadMode").val(d_jsonrecv['dplReadMode']);
    
    forinfo("d_fileTypeCount","fileTypeCount",d_jsonrecv);
    forinfo("d_blockSize","blockSize",d_jsonrecv);
    forinfo("d_chunkSize","chunkSize",d_jsonrecv);
    forinfo("d_fileTypeCount","fileTypeCount",d_jsonrecv);
    forinfo("d_dedupOption","dedupOption",d_jsonrecv);
    forinfo("d_crcOption","crcOption",d_jsonrecv);
    forinfo("d_byteCmpOption","byteCmpOption",d_jsonrecv);
    forinfo("d_hashType","hashType",d_jsonrecv);
    forinfo("d_compressType","compressType",d_jsonrecv);
    //forinfo("d_byteDedup","byteDedup",d_jsonrecv);
    forinfo("d_dplCpuNum","dplCpuNum",d_jsonrecv);
    forinfo("d_dplTimeOutSec","dplTimeOutSec",d_jsonrecv);
    forinfo("d_dplCapacity","dplCapacity",d_jsonrecv);
    
    forinfo("d_raStrategy","raStrategy",d_jsonrecv);
    jQuery("#d_raChunkCnt").val(d_jsonrecv['raChunkCnt']);
    jQuery("#d_maxDelayedCloseNum").val(d_jsonrecv['maxDelayedCloseNum']);
    jQuery("#d_delayedCloseTime").val(d_jsonrecv['delayedCloseTime']);
    
    jQuery("#d_fsBucketCount").val(d_jsonrecv['fsBucketCount']);
    jQuery("#d_mmapSize").val(d_jsonrecv['mmapSize']);
    //jQuery("#d_raSize").val(d_jsonrecv['raSize']);
    
    
    
    
    //显示多条数据路径
    var detaildata = jQuery("#pathInfo"+id).children("span");
    //因弹框是隐藏不是消失，故每次显示归档路径时先删除之前点击按钮多显示的归档路径
    while(detailPathTab.hasChildNodes()){
        detailPathTab.removeChild(detailPathTab.lastChild);
    }
    for(var i = 0;i < detaildata.length;i++)
    {
        var path_data = detaildata.eq(i).text();
        //把获取到的已归档路径，显示在编辑框里
        var new_row=detailPathTab.insertRow(detailPathTab.rows.length);
        var new_col01=new_row.insertCell(0);
        new_col01.innerHTML='<input type="text" name="d_dataPath[]" id="d_dataPath" class="s_width250new" value="'+path_data+'" disabled/>'+'<input type="hidden" value="'+id+'" id="ddfsId" />';
    }
    
    mountDdfs("d_mountWay", "d_superBtn", "d_superDiv");
    publicP('show_ddfs','detailDdfs','挂载点详细信息');
    jQuery("#detailDdfs input").not(".cancel").attr("readonly",true);
    jQuery("#detailDdfs select").attr("disabled",true);
    jQuery("#detailDdfs input[type='button']").not(".cancel,#d_superBtn").css("display","none");
}
// adfs 由挂载点路径得出数据存储路径
function dpFirstVal(mp)
{
    if (document.getElementById("mountWay").value == "adfs") {
        var mpVal    = document.getElementsByName("dataPath[]")[0].value;    
        while (mpVal[mpVal.length - 1] == "/") {
            mpVal = mpVal.substr(0, mpVal.length - 1);
        }
        
        var mpArr = mpVal.split("/");
        var dpObj = document.getElementById(mp);
        var dpVal = "";
        
        for (var i = 0; i < mpArr.length; i++) {
            dpVal += mpArr[i] +"/"; 
        }
        dpObj.value = dpVal + "mp";
    }
    
}

//公共删除弹框模块，适用于输出内容5个换行
function publicPDel(checkbox0,table0,tip,spanCon,tdClass,pdiv,pid,ptitle){
    var a=jQuery("input[name='"+checkbox0+"']");
    var addTable = document.getElementById(table0);
    var cbox;//表格索引
    var num=0;//当num=0时，不会删除弹出对话层
    var na = 0;//当na=5时，可换行
    var vols_no = "";//定义删除弹出框 删除内容
    for (var i=0; i<a.length; i++){
        if(a[i].checked){
            num++;
        }
    }
    if(num == 0){
        alert("未选中任何" +tip+ "!");
        return false;
    }else{
        if(num == a.length){
            jQuery("#"+spanCon).html("&nbsp;全部" +tip+ "?");
        }else{
            for(var j = 1;j < addTable.rows.length;j++){
                cbox = addTable.rows[j].childNodes[0].childNodes[0];
                if(cbox.checked){
                    vols_no += jQuery("."+tdClass).eq(j-1).html() + "&nbsp;";
                    na++;
                    if(na%5 === 0){
                        vols_no += "<br />";
                    }
                }
            }
            jQuery("#"+spanCon).html(vols_no);
        }
        publicP(pdiv,pid,ptitle);
    }
}

//公共弹框模块
function publicP(pdiv,pid,ptitle){
    jQuery("."+pdiv).PopupDiv({
        PopupDivId: pid,
        myclass: "myWrap2",
        title: ptitle,
        modal: 'true',
        minWidth:'400'
        });
    jQuery(".myWrap2").css("background","#5D7183").corner("cc:#DFE8F7");
}

//Hdfs页面
function indexCreate()
{
    document.getElementById("hdfsWay").style.display = "";
    document.getElementById("indexsinIp").style.display = "";
    document.getElementById("indexName").style.display = "none";
    jQuery(".indexHide").css("display","none");

    publicP('hdfscreate','newhdfs','新建');
}
    