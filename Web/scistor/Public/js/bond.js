//***数据归档页面****//
jQuery(document).ready(function(){
    //创建归档
    jQuery(".cre_document").click(function(){
    	var countNode = jQuery("#countNode").val() * 1;
    	var countData = jQuery("#countData").val() * 1;
    	if (countNode > 0 && countData < 1) {
    		alert("当前只有索引节点，请先添加数据节点再建立归档任务！");
    		return false;
    	} else {
    		publicP("new_document","newDocument","新建归档任务");
    	}
    });

    //删除归档
    jQuery(".deldocument").click(function(){
    /*对应的参数分别是，checkbox的name属性值，表格的id值，
        *提示信息，弹出框内容id值，要显示的内容class值（表格中），
        *弹出层内容id值，给弹出层id值，弹出层标题*/
        /* var a=jQuery("input[name='guidang']");
        var addTable = document.getElementById("tab_group");
        var cbox;//表格索引
        var vols_no = "";//定义删除弹出框 删除内容
        for(var j = 1;j < addTable.rows.length;j++){
            cbox = addTable.rows[j].childNodes[0].childNodes[0];
            if(cbox.checked && cbox.disabled == false){
            	vols_no += jQuery(".inputId").eq(j-1).val() + ";";
            }
        }
        jQuery("#delhidId").val(vols_no);
        
        publicPDel('guidang','tab_group','归档任务','del_document','document',"del_docu","delDocu","删除归档任务");
     */
        var a=document.getElementsByName("guidang");
        var num=0;
        var na=0;
        var vols_no = "";
        var hidId   = "";
        var arvname = "";
        for (var i=0; i<a.length; i++){
            if(a[i].checked && a[i].disabled == false){
                hidId += jQuery(".inputId").eq(i).val() + ";";
                arvname += jQuery(".arvname").eq(i).html() + ";";
                num++;
            }
        }
        jQuery("#delhidId").val(hidId);
        jQuery("#arv_name").val(arvname);
        if(num == 0){
            alert("未选中任何归档任务！");
            return false;
        }else {
            if(num == a.length){
                jQuery("#del_document").html("&nbsp;全部归档任务?");
            }else{
                for(var j = 0;j < a.length;j++){
                    if(a[j].checked && a[j].disabled == false) {
                        vols_no += "&nbsp;" + a[j].value;//将取到的归档任务名称赋给del_list
                        na++;
                        if(na%5 === 0){
                            vols_no += "<br />";
                        }
                    }
                }
                jQuery("#del_document").html(vols_no);
            }
            publicP('del_docu','delDocu','删除归档任务');
        }
    });

});

//判断select哪个选中
function archiveInfo(e_inputId,infoTd)
{
	var editstart = document.getElementById(e_inputId);
    for(var i = 0;i < editstart.options.length;i++)
    {
    	if(editstart.options[i].value == jQuery(infoTd).text())
        {
    		editstart.options[i].selected = "selected";
        }
    }
}
//归档详细信息
function infoArchive(id)
{
    var name_de = jQuery("#info_name"+id).text();
    jQuery("#detail_name").val(name_de);
    
    archiveInfo("infobe_h","#infostart_h"+id);
    archiveInfo("infobe_m","#infostart_m"+id);
    archiveInfo("infostop_ho","#infostop_h"+id);
    //archiveInfo("infostop_mi","#infostop_m"+id);
    archiveInfo("infointerv","#infointervalTime"+id);
   
    var infodatatime     = jQuery("#infodatatime"+id).text();
    jQuery("#ago_time").val(infodatatime);
    
    var infocheck = jQuery("#infocheck"+id).text();
    if(infocheck == 1){
    	document.getElementById("infocheck_y").checked = "checked";
    } else {
    	document.getElementById("infocheck_y").checked = false;
    }
    
    var infoSource = jQuery("#infoSource"+id).text();
    if(infoSource == 1){
    	document.getElementById("infodelOldFile").checked = "checked";
    } else {
    	document.getElementById("infodelOldFile").checked = false;
    } 
    
    /*ip显示**/
    checkIP(id,"Dipson","Diplist");
    /*ip显示结束**/
     
    /*过滤条件**/
    //前缀
    var infoprefix     = jQuery("#infoprefix"+id).text();
    var infoprefixType = jQuery("#infoprefixType"+id).text();
    checkFix(infoprefixType,"Depre","Dpreass","Dprefil");
    jQuery("#Dprefix").val(infoprefix);
    //后缀
    var infopostfix     = jQuery("#infopostfix"+id).text();
    var infopostfixType = jQuery("#infopostfixType"+id).text();
    checkFix(infopostfixType,"Depost","Dpostass","Dpostfil");
    jQuery("#Dpostfix").val(infopostfix);
    //文件大小
    var infosizeType    = jQuery("#infosizeType"+id).text();
    var infomaxSize     = jQuery("#infomaxSize"+id).text().split(" ");
    var infominSize     = jQuery("#infominSize"+id).text().split(" ");
    checkFix(infosizeType,"Desize","Dsizeass","Dsizefil");
    jQuery("#dmaxSize").val(infomaxSize[0]);
    jQuery("#dminSize").val(infominSize[0]);
    forinfo("DmaxSizeUnit",1,infomaxSize);
    forinfo("DminSizeUnit",1,infominSize);
    
    //路径
    var infodir        = jQuery("#infodir"+id).text();
    var infodirType    = jQuery("#infodirType"+id).text();
    if (infodirType == "2") {
     	document.getElementById("Ddirfil").checked = "checked";
     	document.getElementById("Dedir").checked = "checked";
     } else {
     	document.getElementById("Dedir").checked = false;
     }
    jQuery("#Ddir").val(infodir);
    
    /*过滤条件结束**/
    
    /*过滤条件结束**/
    
    var inpathSrc  = jQuery("#infopath"+id).children("span:even"); //获取源路径
    var inpathDest = jQuery("#infopath"+id).children("span:odd"); //获取目标路径
    //因弹框是隐藏不是消失，故每次显示归档路径时先删除之前点击按钮多显示的归档路径
    while(Table_detail.hasChildNodes()){
    	Table_detail.removeChild(Table_detail.lastChild);
    }
    for(var i = 0;i < inpathSrc.length;i++)
    {
   	 var src_path  = inpathSrc.eq(i).text();
        var dest_path = inpathDest.eq(i).text();
        //把获取到的已归档路径，显示在编辑框里
        var new_row=Table_detail.insertRow(Table_detail.rows.length);
        var new_col01=new_row.insertCell(0); 
        new_col01.innerHTML='源：<input class="s_width220new"  type="text" name="detail_archiveSrcdir[]" id="de_archiveSrcdir" value="'+src_path+'" disabled/>&nbsp;&nbsp;目标：<input type="text" name="detail_archiveDestdir[]" id="de_archiveDestdir" value="'+dest_path+'"  class="s_width220new" disabled/>';
    }
    
    publicP("detail_doc","detailDoc","数据归档详细信息");
}
//编辑、详细归档，判断Ip列表的复选框是否应该被选中
function checkIP(id,ipson,iplist)
{
    var nodesIp = jQuery(".nodesIp"+id);
    var cbox    = jQuery("."+ipson).find("input");
    var cboxall = jQuery("."+ipson).parent("div").siblings(".ipNode").children("input");
    var n = 0;
    for (var i = 0; i < cbox.length; i++) {
        //初始化复选框
        cbox.eq(i).attr("checked",false);
        cboxall.attr("checked",false);
        
        for (var j = 0; j < nodesIp.length; j++) {
            if (cbox.eq(i).val() == nodesIp.eq(j).text()) {
                cbox.eq(i).attr("checked",true);
            }
        }
    }
    var Diplist = jQuery("."+iplist);
    for (var i = 0; i < Diplist.length; i++){
        var cboxson = jQuery("."+iplist).eq(i).children("."+ipson).find("input");
        for (var j = 0; j < cboxson.length; j++) {
            if (cboxson.eq(j).is(":checked")) {
                n++;
            }
            if (n == cboxson.length) {
                jQuery("."+iplist).eq(i).siblings(".ipNode").find("input").attr("checked",true);
            } else {
                jQuery("."+iplist).eq(i).siblings(".ipNode").find("input").attr("checked",false);
            }
        }
        n = 0;
    }
}

//编辑、详细归档，判断前缀后缀的复选框是否应该选中
function checkFix(infoType,ectype,typeAss,typeFil)
{
	if (infoType == "1") {
     	document.getElementById(typeAss).checked = "checked";
     	document.getElementById(ectype).checked = "checked";
     	
     } else if (infoType == "2") {
     	document.getElementById(typeFil).checked = "checked";
     	document.getElementById(ectype).checked = "checked";
     	
     } else {
     	document.getElementById(ectype).checked = false;
     	document.getElementById(typeAss).checked = false;
     	document.getElementById(typeFil).checked = false;
     }
}
//编辑归档信息
function editArc(id)
{
	 var doc_name = jQuery("#info_name"+id).text();
	    jQuery("#doc_name").val(doc_name);
     
	 archiveInfo("editbe_h","#infostart_h"+id);
	 archiveInfo("editbe_m","#infostart_m"+id);
	 archiveInfo("editstop_ho","#infostop_h"+id);
	 //archiveInfo("editstop_mi","#infostop_m"+id);
	 archiveInfo("editinterv","#infointervalTime"+id);
	 archiveInfo("editinterv","#infointervalTime"+id);
     
     var editinfodatatime     = jQuery("#infodatatime"+id).text();
     jQuery("#edit_letter").val(editinfodatatime);
     
     var infocheck = jQuery("#infocheck"+id).text();
     if(infocheck == 1){
     	document.getElementById("editcheck_y").checked = "checked";
     } else {
    	 document.getElementById("editcheck_y").checked = false;
     }
     
     var infoSource = jQuery("#infoSource"+id).text();
     if(infoSource == 1){
     	document.getElementById("editSource").checked = "checked";
     } else {
    	 document.getElementById("editSource").checked = false;
     }
     
     /*ip显示**/
     checkIP(id,"Eipson","Eiplist");
     /*ip显示结束**/
     
     /*过滤条件**/
     //前缀
     var infoprefix     = jQuery("#infoprefix"+id).text();
     var infoprefixType = jQuery("#infoprefixType"+id).text();
     checkFix(infoprefixType,"Ecpre","Epreass","Eprefil");
     jQuery("#Eprefix").val(infoprefix);
     //后缀
     var infopostfix     = jQuery("#infopostfix"+id).text();
     var infopostfixType = jQuery("#infopostfixType"+id).text();
     checkFix(infopostfixType,"Ecpost","Epostass","Epostfil");
     jQuery("#Epostfix").val(infopostfix);
     //文件大小
     var infosizeType    = jQuery("#infosizeType"+id).text();
     var infomaxSize     = jQuery("#infomaxSize"+id).text().split(" ");
     var infominSize     = jQuery("#infominSize"+id).text().split(" ");
     checkFix(infosizeType,"Ecsize","Esizeass","Esizefil");
     jQuery("#EmaxSize").val(infomaxSize[0]);
     jQuery("#EminSize").val(infominSize[0]);
     forinfo("EmaxSizeUnit",1,infomaxSize);
     forinfo("EminSizeUnit",1,infominSize);
     
     //路径
     var infodir        = jQuery("#infodir"+id).text();
     var infodirType    = jQuery("#infodirType"+id).text();
     if (infodirType == "2") {
      	document.getElementById("Edirfil").checked = "checked";
      	document.getElementById("Ecdir").checked = "checked";
      } else {
      	document.getElementById("Ecdir").checked = false;
      }
     jQuery("#Edir").val(infodir);
     
     /*过滤条件结束**/
     
     var hidId = jQuery("#hidId"+id).val();
     jQuery("#hideId").val(hidId);
     
     var inpathSrc  = jQuery("#infopath"+id).children("span:even"); 
     var inpathDest = jQuery("#infopath"+id).children("span:odd");
     //因弹框是隐藏不是消失，故每次显示归档路径时先删除之前点击按钮多显示的归档路径
     while(Table_edit.hasChildNodes()){
    	 Table_edit.removeChild(Table_edit.lastChild);
     }
     
     //获取到当前操作的名称id值
     var rowID   = Table_edit.insertRow(Table_edit.length);
     var rowIDtd = rowID.insertCell(0);
     rowIDtd.innerHTML = '<td><input type="hidden" value="'+id+'" id="pathId" /></td>';
     
     for(var i = 0;i < inpathSrc.length;i++)
     {
    	 var src_path  = inpathSrc.eq(i).text();
         var dest_path = inpathDest.eq(i).text();
         //把获取到的已归档路径，显示在编辑框里
         var new_row=Table_edit.insertRow(Table_edit.rows.length);
         var new_col01=new_row.insertCell(0); 
         var new_col02=new_row.insertCell(1);
         new_col01.innerHTML='源：<input class="s_width220new"  type="text" name="edit_archiveSrcdir[]" id="archiveSrcdir" value="'+src_path+'" disabled />&nbsp;&nbsp;目标：<input type="text" name="edit_archiveDestdir[]" id="archiveDestdir" value="'+dest_path+'"  class="s_width220new" disabled /> ';
         new_col02.innerHTML='&nbsp;';
     }
     
     publicP("edit_doc","eidtDoc","编辑归档任务");
}
//添加源路径存放位置
function build_row(){
    var add_Dir=document.getElementsByName("archiveSrcdir[]");
    var add_Dest=document.getElementsByName("archiveDestdir[]");
    for(var i = 0;i < add_Dir.length; i++){
        if(add_Dir[i].value == ""){
            alert("源路径不能为空！");
            add_Dir[i].focus();
            return false;
        }
    }

    for(var i = 0;i < add_Dest.length; i++){
        if(add_Dest[i].value == ""){
            alert("目标路径不能为空！");
            add_Dest[i].focus();
            return false;
        }
    }
    
    var e = event.srcElement ? event.srcElement.parentNode : event.target.parentNode;//js三目运算
     //表格图片片段
    var row = e.parentNode.sectionRowIndex;
      // alert(row);
       //return;
     //图片判断结束
    var len = Table1.rows.length;
    var new_row=Table1.insertRow(Table1.rows.length);
    var new_col01=new_row.insertCell(0);
    var new_col02=new_row.insertCell(1);
    new_col01.innerHTML='源：<input class="s_width220new"  type="text" name="archiveSrcdir[]" id="archiveSrcdir" value="" />&nbsp;&nbsp;目标：<input type="text" name="archiveDestdir[]" id="archiveDestdir" value=""  class="s_width220new" />';
    new_col02.innerHTML='<img src="/scistor/Public/img/clear.gif" align="absmiddle" style="cursor:pointer;margin:5px 0px 0px 3px;" onclick = "delete_row()" title="删除数据路径对"/>';

    var j = add_Dir.length - 1; //得到新增的input，以便获得焦点
    add_Dir[j].focus();
}

function delete_row()   
{
    var arc_dir=document.getElementsByName("archiveSrcdir[]");
    var arc_dest=document.getElementsByName("archiveDestdir[]");
    var e = event.srcElement ? event.srcElement.parentNode : event.target.parentNode;//js三目运算
    if(arc_dir.length > 1){
        var row  = e.parentNode.sectionRowIndex;
        var cell = e.cellIndex;
        if(row < 0){
              alert("操作无效");
            return;
        }
        //如果为空并且count>1删除该编辑框时不需要提示
        var con = 1;
        /*if(arc_dir[row].value != ""){
            con = window.confirm("您确定要删除吗？");
        }
        if(arc_dir[row].value == "" && arc_dest[row].value != ""){
            con = window.confirm("您确定要删除吗？");
        }*/
        if(con == 1){
            if(e.tagName == "TD"){
                Table1.deleteRow(row - 0);    
            }
        }
    } else {
        alert("不能全部删除");
    }
}



//编辑归档添加路径
function editbuild_row(){
    var edit_Dir=document.getElementsByName("edit_archiveSrcdir[]");
    var edit_Dest=document.getElementsByName("edit_archiveDestdir[]");
    for(var i = 0;i < edit_Dir.length; i++){
        if(edit_Dir[i].value == ""){
            alert("源路径不能为空！");
            edit_Dir[i].focus();
            return false;
        }
    }

    for(var i = 0;i < edit_Dest.length; i++){
        if(edit_Dest[i].value == ""){
            alert("目标路径不能为空！");
            edit_Dest[i].focus();
            return false;
        }
    }
    
    var e = event.srcElement ? event.srcElement.parentNode : event.target.parentNode;//js三目运算
     //表格图片片段
    var row = e.parentNode.sectionRowIndex;
      // alert(row);
       //return;
     //图片判断结束
    var len = Table_edit.rows.length;
    var new_row=Table_edit.insertRow(Table_edit.rows.length);
    var new_col01=new_row.insertCell(0);
    var new_col02=new_row.insertCell(1);
    new_col01.innerHTML='源：<input class="s_width220new"  type="text" name="edit_archiveSrcdir[]" id="archiveSrcdir" value="" />&nbsp;&nbsp;目标：<input type="text" name="edit_archiveDestdir[]" id="archiveDestdir" value=""  class="s_width220new" />';
    new_col02.innerHTML='<img src="/scistor/Public/img/clear.gif" align="absmiddle" style="cursor:pointer;margin:5px 0px 0px 3px;" onclick = "editdelete_row()" title="删除数据路径对"/>';

    var j = edit_Dir.length - 1; //得到新增的input，以便获得焦点
    edit_Dir[j].focus();
}

function editdelete_row()   
{
    var edit_Dir=document.getElementsByName("edit_archiveSrcdir[]");
    var edit_dest=document.getElementsByName("edit_archiveDestdir[]");
    var e = event.srcElement ? event.srcElement.parentNode : event.target.parentNode;//js三目运算
    if(edit_Dir.length > 1){
        var row  = e.parentNode.sectionRowIndex;
        var cell = e.cellIndex;
        if(row < 0){
              alert("操作无效");
            return;
        }
        //如果为空并且count>1删除该编辑框时不需要提示
        var con = 1;
        /* if(edit_Dir[row].value != ""){
            con = window.confirm("您确定要删除吗？");
        }
        if(edit_Dir[row].value == "" && edit_dest[row].value != "" ){
            con = window.confirm("您确定要删除吗？");
        } */
        if(con == 1){
            if(e.tagName == "TD"){
                Table_edit.deleteRow(row - 0);    
            }
        }
    } else {
        alert("不能全部删除");
    }
}

//***网卡配置页面****//

jQuery(document).ready(function(){

    //绑定网卡
    jQuery(".bind_newvols").click(function(){
        var a=jQuery("input[name='choice']");
        var addTable = document.getElementById("tablevols");
        var cbox;
        var num=0;
        var na = 0;
        var vols_no = "";
        var vols_noDiv = ""
        for (var i=0; i<a.length; i++){
            if(a[i].checked && a[i].disabled == false){
                num++;
            }
        }
        if(num < 2){
            alert("绑定网卡至少需要2块连接网卡！");
            return false;
        }else{
            for(var j = 1;j < addTable.rows.length;j++){
                cbox = addTable.rows[j].childNodes[0].childNodes[0];
                if(cbox.checked && cbox.disabled == false) {
                    vols_no += " " + jQuery(".document").eq(j-1).html();//将取到的网卡号的值赋给vols_no
                    vols_noDiv += " " + jQuery(".document").eq(j-1).html();//将取到的网卡号的值赋给div
                    na++;
                    if(na%5 === 0){
                        vols_noDiv += "<br />";
                    }
                }
            }
            jQuery("#DEVICES").val(vols_no);
            jQuery("#devicesDiv").html(vols_noDiv);
    
            publicP("card_bond","cardBond","网卡绑定");
        }
    });
    
    //删除绑定网卡
    jQuery(".bond_remove").click(function(){
        var delbondname = jQuery(this).parent().parent().children()[0];
        var delname = jQuery(delbondname).text();
        jQuery("#del_bondname").html(delname);
        
        publicP("del_bond","delBond","删除绑定网卡");
    });
});

//网卡配置
function netWorkSet(id)
{
    var name       = jQuery("#workName"+id).text();
    var mtu_set    = jQuery("#mtu_size"+id).text();
    var card_setIp = jQuery("#networkIp"+id).text();
    var mask_set   = jQuery("#networkMask"+id).text();
    var geteWayset = jQuery("#networkgate"+id).text();
    var bootProto  = jQuery("#bootState"+id).text();
    var netMmac    = jQuery("#netMmac"+id).text();
    
    jQuery("#mtu_name").val(name);
    jQuery("#nobondMtu").val(mtu_set);
    jQuery("#cardSetIp").val(card_setIp);
    jQuery("#cardSetmask").val(mask_set);
    jQuery("#cardSetgeteWay").val(geteWayset);
    jQuery("#netMmac").val(netMmac);
    jQuery("#netId").val(id);
    
    if(bootProto == "none"){
    	document.getElementById("cardhand_ip").checked = true;
    	jQuery(".netcard_edit").attr("disabled",false);
    } else {
    	document.getElementById("cardauto_ip").checked = true;
    	jQuery(".netcard_edit").attr("disabled",true);
    }
    
    
    publicP("configcard","configCard","网卡配置");
}

//绑定网卡配置
function config_bond(id)
{
	var name = jQuery("#bondName"+id).text();
    jQuery("#bond_name").val(name);
    var devi = jQuery("#con_devi"+id).text();
    jQuery("#con_devi").val(devi);
    
    var bondbootProto  = jQuery("#bondPro"+id).text();
    var bondmode       = jQuery("#bondMode"+id).text();
    
    var bondcard_setIp = jQuery("#bondcard_ip"+id).text();
    var bondmask_set   = jQuery("#bondmask"+id).text();
    var bondgeteWayset = jQuery("#bondgeteWay"+id).text();
    
    jQuery("#bondSetIp").val(bondcard_setIp);
    jQuery("#bondSetmask").val(bondmask_set);
    jQuery("#bondSetgeteWay").val(bondgeteWayset);
    
    var mode_set = document.getElementById("bondmodeset");
    
    for(var i = 0;i < mode_set.options.length;i++)
    {
    	if(mode_set.options[i].value == bondmode)
        {
    		mode_set.options[i].selected = "selected";
        }
    }
    if(bondbootProto == "none"){
    	document.getElementById("hand_set").checked = true;
    	jQuery(".hand_bondcard").attr("disabled",false);
    } else {
    	document.getElementById("auto_set").checked = true;
    	jQuery(".hand_bondcard").attr("disabled",true);
    }
    
    publicP("bondcon_card","bondconCard","绑定网卡配置");
}

function net_radio()
{
    var edit_card = jQuery("input[name='config_ip']");
    if(edit_card[0].checked){
        jQuery(".netcard_edit").attr("disabled",true);
        jQuery(".netcard_edit").val("");
    } else {
        jQuery(".netcard_edit").attr("disabled",false);
        jQuery(".netcard_edit").val("");
    }
}

function bondip_set()
{
    var bond_card = jQuery("input[name='bondip']");
    if(bond_card[0].checked){
        jQuery(".hand_bondip").attr("disabled",true);
        jQuery(".hand_bondip").val("");
    } else {
        jQuery(".hand_bondip").attr("disabled",false);
        jQuery(".hand_bondip").val("");
    }
}

function bondcard_pei()
{
    var bond_card = jQuery("input[name='bondiphand']");
    if(bond_card[0].checked){
        jQuery(".hand_bondcard").attr("disabled",true);
        jQuery(".hand_bondcard").val("");
    } else {
        jQuery(".hand_bondcard").attr("disabled",false);
        jQuery(".hand_bondcard").val("");
    }
}

//***用户组页面****//
jQuery(document).ready(function(){

    //新建用户组
    jQuery(".cre_group").click(function(){
        publicP("new_group","newGroup","新建组");
    });
    
    //编辑配额
    jQuery(".quota_edit").click(function(){
        var gr_name = jQuery(this).parent().parent().children()[0];
        var name = jQuery(gr_name).text();
        jQuery("#group_name").val(name);
        
        publicP("quo_edit","newEdit","编辑配额");
    })
    /*
    //删除用户组
    jQuery(".delgroup").click(function(){
        publicPDel('grouplist','tab_group','任何用户组','del_group','groupname',"del_grp","delvololume","删除用户组");
    });
    */
});

//***用户组页面****//
function show_this()
{
    var radio_vali = document.getElementsByName("validate");
    if(radio_vali[0].checked){
        jQuery("#vali_AD").css("display","none");
        jQuery("#vali_NIS").css("display","none");
    }
    if(radio_vali[1].checked){
        jQuery("#vali_AD").css("display","none");
        jQuery("#vali_NIS").css("display","none");
    }
    if(radio_vali[2].checked){
        jQuery("#vali_AD").css("display","block");
        jQuery("#vali_NIS").css("display","none");
    }
    if(radio_vali[3].checked){
        jQuery("#vali_AD").css("display","none");
        jQuery("#vali_NIS").css("display","block");
    }
}
function groupbuild_row(){
    var gshare_na=document.getElementsByName("gshare_name[]");
    var gkuota_num=document.getElementsByName("gkuota[]");
    for(var i = 0;i < gshare_na.length; i++){
        if(gshare_na[i].value == ""){
            alert("共享名称不能为空！");
            gshare_na[i].focus();
            return false;
        }
    }

    for(var i = 0;i < gkuota_num.length; i++){
        if(gkuota_num[i].value == ""){
            alert("配额量不能为空！");
            gkuota_num[i].focus();
            return false;
        }
    }
    
    var e = event.srcElement ? event.srcElement.parentNode : event.target.parentNode;//js三目运算
     //表格图片片段
    var row = e.parentNode.sectionRowIndex;
      // alert(row);
       //return;
     //图片判断结束
    var len = Table_group.rows.length;
    var new_row=Table_group.insertRow(Table_group.rows.length);
    var new_col01=new_row.insertCell(0);
    var new_col02=new_row.insertCell(1);
    new_col01.innerHTML='共享名称：<input class="s_width220new"  type="text" name="gshare_name[]" id="gshare_name" value="" />&nbsp;&nbsp;配额量：<input type="text" name="gkuota[]" id="gkuota" value=""  class="s_width110new" />';
    new_col02.innerHTML='<img src="/scistor/Public/img/clear.gif" align="absmiddle" style="cursor:pointer;margin:5px 0px 0px 3px;" onclick = "groupdelete_row()" title="删除配额"/>';

    var j = gshare_na.length - 1; //得到新增的input，以便获得焦点
    gshare_na[j].focus();
}

function groupdelete_row()   
{
    var gshare_na=document.getElementsByName("gshare_name[]");
    var gkuota_num=document.getElementsByName("gkuota[]");
    var e = event.srcElement ? event.srcElement.parentNode : event.target.parentNode;//js三目运算
    if(gshare_na.length > 1){
        var row  = e.parentNode.sectionRowIndex;
        var cell = e.cellIndex;
        if(row < 0){
              alert("操作无效");
            return;
        }
        //如果为空并且count>1删除该编辑框时不需要提示
        var con = 1;
        if(gshare_na[row].value != ""){
            con = window.confirm("您确定要删除吗？");
        }
        if(gshare_na[row].value == "" && gkuota_num[row].value != ""){
            con = window.confirm("您确定要删除吗？");
        }
        if(con == 1){
            if(e.tagName == "TD"){
                Table_group.deleteRow(row - 0);    
            }
        }
    } else {
        alert("不能全部删除");
    }
}

//***用户页面****//
//创建用户
jQuery(document).ready(function(){

    //新建用户
    jQuery(".cre_user").click(function(){
        publicP("new_user","newUser","新建用户");
    });
    
    //编辑用户
    jQuery(".user_edit").click(function(){
        
        var uname = jQuery(this).parent().parent().children()[0];
        var name = jQuery(uname).text();
        jQuery("#user_name").val(name);
        
        publicP("edit_user","editUser","编辑用户");
    
    });
    /*
    //删除用户
    jQuery(".deluser").click(function(){
        publicPDel('userlist','tab_group','任何用户','del_user','username',"del_user","delVololume","删除用户");
    });
    */
});
function edit_user()
{
    var edit_che = jQuery("input[name='edit_user']");
    if(edit_che[0].checked){
        jQuery(".edit_pwd").attr("disabled",false);
    } else {
        jQuery(".edit_pwd").attr("disabled",true);
    }
    
    if(edit_che[1].checked){
        jQuery(".edit_qu").attr("disabled",false);
    } else {
        jQuery(".edit_qu").attr("disabled",true);
    }
}
function userbuild_row(){
    var ushare_na=document.getElementsByName("ushare_name[]");
    var ukuota_num=document.getElementsByName("ukuota[]");
    for(var i = 0;i < ushare_na.length; i++){
        if(ushare_na[i].value == ""){
            alert("共享名称不能为空！");
            ushare_na[i].focus();
            return false;
        }
    }

    for(var i = 0;i < ukuota_num.length; i++){
        if(ukuota_num[i].value == ""){
            alert("配额量不能为空！");
            ukuota_num[i].focus();
            return false;
        }
    }
    
    var e = event.srcElement ? event.srcElement.parentNode : event.target.parentNode;//js三目运算
     //表格图片片段
    var row = e.parentNode.sectionRowIndex;
      // alert(row);
       //return;
     //图片判断结束
    var len = Table_user.rows.length;
    var new_row=Table_user.insertRow(Table_user.rows.length);
    var new_col01=new_row.insertCell(0);
    var new_col02=new_row.insertCell(1);
    new_col01.innerHTML='共享名称：<input class="s_width220new edit_qu"  type="text" name="ushare_name[]" id="ushare_name" value="" />&nbsp;&nbsp;配额量：<input type="text" name="ukuota[]" id="ukuota" value=""  class="s_width110new edit_qu" />';
    new_col02.innerHTML='<img src="/scistor/Public/img/clear.gif" align="absmiddle" style="cursor:pointer;margin:5px 0px 0px 3px;" onclick = "userdelete_row()" title="删除配额"/>';

    var j = ushare_na.length - 1; //得到新增的input，以便获得焦点
    ushare_na[j].focus();
}

function userdelete_row()   
{
    var ushare_na=document.getElementsByName("ushare_name[]");
    var ukuota_num=document.getElementsByName("ukuota[]");
    var e = event.srcElement ? event.srcElement.parentNode : event.target.parentNode;//js三目运算
    if(ushare_na.length > 1){
        var row  = e.parentNode.sectionRowIndex;
        var cell = e.cellIndex;
        if(row < 0){
              alert("操作无效");
            return;
        }
        //如果为空并且count>1删除该编辑框时不需要提示
        var con = 1;
        if(ushare_na[row].value != ""){
            con = window.confirm("您确定要删除吗？");
        }
        if(ushare_na[row].value == "" && ukuota_num[row].value != ""){
            con = window.confirm("您确定要删除吗？");
        }
        if(con == 1){
            if(e.tagName == "TD"){
                Table_user.deleteRow(row - 0);    
            }
        }
    } else {
        alert("不能全部删除");
    }
}
//***用户验证页面****//
function localset_valida()
{
    var local_valiset = jQuery("input[name='local_validate']");
    if(local_valiset[0].checked){
        jQuery(".local_valiset").attr("disabled",false);
        //$("#ldap_ab").attr("disabled",true);
    } else {
        jQuery(".local_valiset").attr("disabled",true);
        //$("#ldap_ab").attr("disabled",false);
    }
}
function set_validaldap()
{
    var ldap_valiset = jQuery("input[name='ldap_validate']");
    if(ldap_valiset[0].checked){
        jQuery(".ldap_valiset").attr("disabled",false);
        //$("#local_ab").attr("disabled",true);
    } else {
        jQuery(".ldap_valiset").attr("disabled",true);
        //$("#local_ab").attr("disabled",false);
    }
}

//存储管理——物理磁盘
function jumpPage(Action)
{
    var page = jQuery("#gopage").val();
    var totalpage = jQuery("#pagelimit").text();
    if(page == "" || page > totalpage || page < 1){
        alert("输入指定页为空或不合法！");
        //jQuery("#gopage").val("");
        jQuery("#gopage").select();
        jQuery("#gopage").focus();
        return false;
    } else {
        document.getElementById("jump_img").href = "index.php?s=/"+Action+"/index/jumpPage/"+page;
    }
}

function jumpPagenew(Action)
{
    var page = jQuery("#gopagenew").val();
    var totalpage = jQuery("#pagelimitnew").text();
    if(page == "" || page > totalpage || page < 1){
        alert("输入指定页为空或不合法！");
        //jQuery("#gopagenew").val("");
        jQuery("#gopagenew").select();
        jQuery("#gopagenew").focus();
        return false;
    } else {
        document.getElementById("jump_imgnew").href = "index.php?s=/"+Action+"/index/jumpPagenew/"+page;
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
        if(a[i].checked && a[i].disabled == false){
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
                if(cbox.checked && cbox.disabled == false){
                    vols_no += "&nbsp;" + jQuery("."+tdClass).eq(j-1).html();
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
   
//***分布式系统管理页面****//
function hdfsType()
{
	var newType = document.getElementById("newType");
	var newWay = document.getElementById("newWay");
    if(newType.selectedIndex == 1){
    	//document.getElementById("indexName").style.display = "";
    	document.getElementById("hdfsWay").style.display = "none";
    	document.getElementById("indexsinIp").style.display = "";
    	document.getElementById("startIp").style.display = "none";
        document.getElementById("endIp").style.display = "none";
    }
    if(newType.selectedIndex == 0){
        for(var i = 0;i < newWay.length;i++)
        {
        	if(newWay.options[i].value == "single")
            {
        		newWay.options[i].selected = "selected";
            }
        }
        document.getElementById("hdfsWay").style.display = "";
        //document.getElementById("indexName").style.display = "none";
        document.getElementById("indexsinIp").style.display = "";
        document.getElementById("startIp").style.display = "none";
        document.getElementById("endIp").style.display = "none";
    }
}
function hdfsneWay()
{
	var newWay = document.getElementById("newWay");
	if(newWay.selectedIndex == 0){
        document.getElementById("indexsinIp").style.display = "";
        document.getElementById("startIp").style.display = "none";
        document.getElementById("endIp").style.display = "none";
    }
    if(newWay.selectedIndex == 1){
        document.getElementById("indexsinIp").style.display = "none";
        document.getElementById("startIp").style.display = "";
        document.getElementById("endIp").style.display = "";
    }
}

//分布式系统首页，删除功能
function indexDelbtn()
{
	var dataCount  = jQuery("#dataCount").val() * 1;
	var is_sel     = jQuery("#is_sel").val();
	var indexState = jQuery("#indexState").val();
	var num = 0;
    var datalist = "";
    var a=jQuery("input[name='choice']");
    for (var i=0; i<a.length; i++){
        if(a[i].checked && a[i].disabled == false){
        	datalist += jQuery(".dataIplist").eq(i).html() + ";";
        	num++;
        }
    }
    if(num == 0){
        alert("未选中任何节点!");
        return false;
    } else {
		if (dataCount < 1) {
			window.location='index.php?s=/Hdfs/delList/id/1/is_sel/'+is_sel;
		} else {
			var indexid = jQuery("#hidIndexip").val();
			var num = 0;
		    var datalist = "";
		    var nodearrIp = jQuery("#nodearrIp").children("span");
		    for (var i=0; i<a.length; i++){
		        if (a[i].checked && a[i].disabled == false){
		        	//是否在数据归档中
		        	for (var j = 0; j < nodearrIp.length; j++) {
			        	if (jQuery(".dataIplist").eq(i).html() == nodearrIp.eq(j).text()) {
			        		alert("请先删除该节点  "+jQuery(".dataIplist").eq(i).html()+" 的归档任务！");
			        		return false;
			        	}
		        	}
		        	datalist += jQuery(".dataIplist").eq(i).html() + ";";
		        	num++;
		        }
		    }
		    
		    window.location='index.php?s=/Hdfs/delList/id/0/datalist/'+datalist+'/indexid/'+indexid;
		}
    }
}


function selIndex(index)
{
    var name = jQuery(index).val();
    //alert(name);
    location.href = "index.php?s=/Hdfs/index/indexname/"+name;
}

function hdfsnewIP()
{
	var ipaddr = document.getElementsByName("ipaddr[]");
    for(var i = 0;i < ipaddr.length; i++){
        if(ipaddr[i].value == ""){
            alert("IP地址不能为空！");
            ipaddr[i].focus();
            return false;
        }
    }
    var e = event.srcElement ? event.srcElement.parentNode : event.target.parentNode;//js三目运算
     //表格图片片段
    var row = e.parentNode.sectionRowIndex;
     //图片判断结束
    var len = ipTab.rows.length;
    var new_row=ipTab.insertRow(len);
    var new_col01=new_row.insertCell(0);
    var new_col02=new_row.insertCell(1);
    new_col01.innerHTML='<input type="text" value="" name="ipaddr[]" id="ipaddr" class="s_width110new"/>';
    new_col02.innerHTML='<img src="/scistor/Public/img/clear.gif" align="absmiddle" class="imgdelIp" onclick = "hdfsdelIP()()" title="删除IP"/>';

    var j = ipaddr.length - 1; //得到新增的input，以便获得焦点
    ipaddr[j].focus();
}
function hdfsdelIP()
{
	var ipaddr=document.getElementsByName("ipaddr[]");
	    var e = event.srcElement ? event.srcElement.parentNode : event.target.parentNode;//js三目运算
    if(ipaddr.length > 1){
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
            	ipTab.deleteRow(row - 0);    
            }
        }
    } else {
    	alert("不能全部删除");
    }
}
//索引配置文件，添加索引存储路径
function hdfsindex_row()
{
    var dataPath = document.getElementsByName("dataindexPath[]");
    for(var i = 0;i < dataPath.length; i++){
        if(dataPath[i].value == ""){
            alert("索引存储路径不能为空！");
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
    var len       = dataindexTab.rows.length;
    var new_row   = dataindexTab.insertRow(len);
    var new_col01 = new_row.insertCell(0);
    var new_col02 = new_row.insertCell(1);
    new_col01.innerHTML='<input type="text" name="dataindexPath[]" id="dataindexPath" class="s_width250new"/>';
    new_col02.innerHTML='<img src="/scistor/Public/img/clear.gif" align="absmiddle" class="img_delete1" onclick = "hdfsindexdel_row()" title="删除索引存储路径"/>';

    var j = dataPath.length - 1; //得到新增的input，以便获得焦点
    dataPath[j].focus();
}

function hdfsindexdel_row()
{
    var deldataPath=document.getElementsByName("dataindexPath[]");
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
                dataindexTab.deleteRow(row - 0);    
            }
        }
    } else {
        alert("不能全部删除");
    }
}
//测试页面
/* function test_over()
{
    var testlastData = jQuery("#testlastData").val();
    var ign = jQuery("#dataIgn").val();
    if (ign == 1) {
        document.getElementById("text_config").disabled = false;
        if (testlastData == "waiting" || testlastData == "testing") {
            document.getElementById("test_pre").disabled = true;
            document.getElementById("text_config").disabled = true;
        } else {
            document.getElementById("test_pre").disabled = false;
            document.getElementById("text_config").disabled = false;
            clearInterval();
        }
    } else {
        document.getElementById("text_config").disabled = true;
    }
} */
function test_over()
{
    var nodes = jQuery(".nodeStatus");
    var refresh = false;
    document.getElementById("test_pre").disabled = true;
    document.getElementById("text_config").disabled = true;
    
    if (nodes.length == 0) {
        refresh = true;
    } else {
        for (var i = 0; i < nodes.length; i++) {
            if ( nodes.eq(i).html() == "testing" || nodes.eq(i).html() == "waiting") {
                refresh = true;
            }
        }
    }
    if (refresh == true) {
        setTimeout("reflashPage()",2000);
    } else {
        document.getElementById("test_pre").disabled = false;;
        for (var i = 0; i < nodes.length; i++) {
            if (nodes.eq(i).html() == "ok") {
                document.getElementById("text_config").disabled = false;
            }
        }
    }
}
function reflashPage(){
    location.href="index.php?s=/Hdfs/displayData";
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
