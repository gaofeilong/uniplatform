//表格隔行换色,及 输入文本框的
jQuery(document).ready(function(){
    jQuery(".right_title").corner("top");
    jQuery(".table1 tr:even").not((jQuery(".top,.t_save"))) .css({
        'background' : '#F0F3F5'
    });
    jQuery("input[type='text'],input[type='password']").not(".nobg_input, .nobg_ipt").css({
        'background':'#F5F7FC',
        'border-top':'1px solid #486A83',
        'border-left':'1px solid #486A83',
        'border-right':'1px solid #8BAFC6',
        'border-bottom':'1px solid #8BAFC6'
    });
    jQuery("input[type='text'],input[type='password']").not(".nobg_input, .nobg_ipt").focus(function(){
        jQuery(this).css("background","#FFF7ED");
    });
    jQuery("input[readonly]").focus(function(){
        jQuery(this).css("background","#F5F7FC");
    });
    jQuery("input[type='text'],input[type='password']").not(".nobg_input, .nobg_ipt").blur(function(){
        jQuery(this).css("background","#F5F7FC");
    });
});

//弹出框 提交表单时，关闭遮罩层
function closeCover(){
    jQuery("#corage_bg").remove();
    
    //关闭top和left页面的遮罩层
    parent.header.jQuery("#corage_bg").remove();
    parent.left.jQuery("#corage_bg").remove();
    
    //关闭弹出框层
    jQuery(".myWrap2").css("display","none");
}

 //复选框的全选
function All(e, itemName)
{
    /*var aa = document.getElementsByName(itemName);
    for (var i=0; i<aa.length; i++) {
        aa[i].checked = e.checked; //得到全选复选框的选中状态
    }*/
    var aa = jQuery("input[name='"+itemName+"']").not("input[disabled='true']");
    var allcheck = jQuery(e).is(":checked");
    for (var i=0; i<aa.length; i++) {
        aa.eq(i).attr("checked",allcheck);
    }
}
function Item(e, allName)
{
    /*var all = document.getElementsByName(allName)[0];
    if(!e.checked){
            all.checked = false;
    } else {
        var aa = document.getElementsByName(e.name);
        for (var i=0; i<aa.length; i++) {
          if(!aa[i].checked) return;
        }
        all.checked = true;
    }*/
    var all = jQuery("input[name='"+allName+"']").eq(0);
    if(!jQuery(e).is(":checked")) {
        all.attr("checked",false);
    } else {
        var eName = jQuery(e).attr("name");
        var aa    = jQuery("input[name='"+eName+"']").not("input[disabled='true']");
        for (var i=0; i<aa.length; i++) {
          if(!aa.eq(i).is(":checked")) return;
        }
        all.attr("checked",true);
    }
}

//归档页面弹框页面中IP的全选和不选
function AllArchive(e, itemName)
{
    var aa = jQuery("."+itemName).not("input[disabled='true']");
    var allcheck = jQuery(e).is(":checked");
    for (var i=0; i<aa.length; i++) {
        aa.eq(i).attr("checked",allcheck);
    }
}
function ItemArchive(e, allName)
{
    var all = jQuery("input[name='"+allName).eq(0);
    if(!jQuery(e).is(":checked")) {
        all.attr("checked",false);
    } else {
        var eName = jQuery(e).attr("class");
        var aa    = jQuery("."+eName).not("input[disabled='true']");
        for (var i=0; i<aa.length; i++) {
          if(!aa.eq(i).is(":checked")) return;
        }
        all.attr("checked",true);
    }
}


//系统信息 
function show_hide(span_title,showDiv){
    var table = document.getElementById(showDiv);
    if(table.style.display == "none"){
        table.style.display = "block";
        jQuery(span_title).children(".show_hide").html("︽");
        jQuery(span_title).attr("title","收起");
    }else{
        table.style.display = "none";
        jQuery(span_title).children(".show_hide").html("︾");
        jQuery(span_title).attr("title","展开");
    }
}

//归档页面    特殊
function show_hide2(span_title,showDiv){
    var table = document.getElementById(showDiv);
    if(table.style.display == "none"){
        table.style.display = "block";
        jQuery(span_title).children(".colexp").html("︽");
        jQuery(span_title).attr("title","收起");
    }else{
        table.style.display = "none";
        jQuery(span_title).children(".colexp").html("︾");
        jQuery(span_title).attr("title","展开");
    }
}
//归档页面 组织父元素事件冒泡
function stopPop(e)
{
    if (e && e.stopPropagation) { //非IE浏览器 
    　　e.stopPropagation(); 
    } else {  //IE浏览器 
        window.event.cancelBubble = true; 
    } 
}

//系统时间 》自定义同步模块的显示和隐藏 ps:该功能被取缔
function showAdminSyn(adminDiv){
    var adminDiv = document.getElementById(adminDiv);
    adminDiv.style.display = "block";
}
function hideAdminSyn(adminDiv){
    var adminDiv = document.getElementById(adminDiv);
    adminDiv.style.display = "none";
}
//系统时间 》控件之间的相互影响
function synchroNow(obj){
    if(obj.checked == true){
        jQuery("#setSysTime select").not("#timezone").attr("disabled",true);
        jQuery("#setNTP").attr("disabled",false);
    }else{
        jQuery("#setSysTime select").attr("disabled",false);
        jQuery("#setNTP").attr("disabled",true);
    }
}
//系统日志 》指定时间 是否可用
function timeCss(){
    if(jQuery("#fixTime").is(":checked")){
        jQuery("#fromLogTime,#toLogTime").attr("disabled",false);
    }else{
        document.getElementById("fromLogTime").value = "";
        document.getElementById("toLogTime").value = "";
        jQuery("#fromLogTime,#toLogTime").attr("disabled",true);
    }
}
//系统日志 》指定时间输入是否合法
/* function checkDate(status){
    var mystatus = status;
    var fixTime = document.getElementById("fixTime");
    var fromTime = document.getElementById("fromLogTime");
    var toTime = document.getElementById("toLogTime");
    var ftimeArr = fromTime.value.split("-");
    var ttimeArr = toTime.value.split("-");
    var ftime = "";
    var ttime = "";

    for(var i=0;i<ftimeArr.length;i++){
        ftime += ftimeArr[i];
        ttime += ttimeArr[i];
    }
    if(fixTime.checked){
        if(fromTime.value == ""){
            alert("指定时间输入为空或不合法！");
            fromTime.focus();
            return status;
        }
        if(toTime.value == "" || ftime*1 > ttime*1){
            alert("指定时间输入为空或不合法！");
            toTime.value = "";
            toTime.focus();
            return false;
        }
    }
} */

//卷管理 》 新建卷 》是否格式化显示与隐藏
function showFomat(){
    var fomatDiv = document.getElementById("fomatDiv");
    fomatDiv.style.display = "block";
}
function hideFomat(){
    var fomatDiv = document.getElementById("fomatDiv");
    fomatDiv.style.display = "none";
}

//事件通知/UPS管理 》页面加载判断后台获取数据
jQuery(document).ready(function(){
    if(jQuery("#isEvent").val() == "1"){
        jQuery("#warn").attr("checked","checked");
    }else{
        jQuery("#coverMail").css("display","block");
    }
    if(jQuery("#verify").val() == "common"){
        jQuery("#common_way").attr("checked","checked");
    } else if (jQuery("#verify").val() == "ssl"){
        jQuery("#ssl_way").attr("checked","checked");
    }
    //ups
    if(jQuery("#isUps").val() == "1"){
        jQuery("#upsCheck").attr("checked","checked");
    }else{
        jQuery("#upsCheck").attr("checked",false);
        jQuery("#coverUps").css("display","block");
    }
    //timeCss();
});
//事件通知 》是否需要报警邮件
function checkNeedMail(obj,pDiv){
    if(obj.checked){
        /* jQuery("input").attr("disabled",false); */
        document.getElementById(pDiv).style.display = "none";
    }else{
        document.getElementById(pDiv).style.display = "block";
        /* jQuery("input").not("#warn,#submitEvent,.hidden").attr("disabled",true); */

    }
}
//事件通知 》动态添加和删除邮箱地址
function addEmail()
{
    var elevel     = "";
    var email      = document.getElementById("newmail");//页面输入邮箱地址
    var eventlevel = document.getElementsByName("eventlevel");//事件级别
    var mailcbox   = document.getElementsByName('maillist');
    var buildmail  = document.getElementsByName("eventMail[]");//页面现有的邮箱
    var matchMail  = /^\w+((-\w+)|(\.\w+))*\@[A-Za-z0-9]+((\.|-)[A-Za-z0-9]+)*\.[A-Za-z0-9]+$/;
    var rowID      = mailcbox.length + 1;//rowID是为了给每行的复选框 添加特定的ID值
    var newemail   = divideData2(email);//处理输入的邮箱地址
    var allExit    = new Array();
    
    if (email.value == "" || newemail == "") {
        alert("请输入邮箱地址！");
        email.focus();
        email.select();
        return false;
    }
    
    for (var i = 0; i < buildmail.length; i++) {
        allExit = allExit.concat(divideData2(buildmail[i]));//已存在的邮箱进行分号处理
    }
    
    for (var i = 0; i < newemail.length; i++) {
        //判断邮箱地址是否合法
        if (!matchMail.exec(newemail[i])) {
            alert("邮箱地址输入不合法！");
            email.focus();
            email.select();
            return false;
        }
        //与自身的比较
        for (var j = i+1; j < newemail.length; j++) {
            if (newemail[i] == newemail[j]) {
                alert("邮箱地址输入不合法！");
                email.focus();
                email.select();
                return false;
            }
        }
        //与存在邮箱的进行比较
        for (var k = 0; k < allExit.length; k++) {
            if (newemail[i] == allExit[k]) {
                alert("邮箱地址已经存在！");
                email.focus();
                email.select();
                return false;
            }
        }
    }
    
    var mail_table  = document.getElementById("emailTable");
    var lastnewmail = divideData(email);
    
    for (var i = 0; i < eventlevel.length; i++) {
        if (eventlevel[i].checked) {
            elevel += eventlevel[i].value + ";";
        }
    }
    if (elevel == "") {
        alert("请勾选事件级别！");
    } else {
        var mail_tr  = mail_table.insertRow();   //添加行
        var mail_td1 = mail_tr.insertCell(0);     //添加列
        mail_td1.className   = "l_b_none vmiddle";
        mail_td1.style.width = "30px";
        mail_td1.innerHTML   = '<input type="checkbox" name="maillist" id="mail' + rowID + '" onclick="Item(this,\'mailAll\')" />';
        
        var mail_td2   = mail_tr.insertCell(1);
        mail_td2.style.width = "420px";
        mail_td2.className   = "wrap";
        mail_td2.innerHTML   = '<label for="mail' +rowID + '">' 
                               +'<div class="emailVal">' + lastnewmail +'</div>'
                               +'<input type="hidden" value="' +lastnewmail+ '" class="nobg_input" name="eventMail[]" readonly /></label>';
        
        var mail_td3 = mail_tr.insertCell(2);
        mail_td3.className = "r_b_none vmiddle";
        mail_td3.innerHTML = '<label for="mail' + rowID + '">'
                             + '<div class="levelVal">' +elevel+ '</div>'
                             + '<input type="hidden" value="' +elevel+ '" class="nobg_input" name="mailLevel[]" readonly />'
                             +'<input type="hidden" value="" class="nobg_input" name="hideLevel[]" readonly /></label>';
        
        //将行号推进下一行，并清空邮件输入框
        rowID += 1;
        email.value = "";
        for (var i = 0; i < eventlevel.length; i++) {
            if( eventlevel[i].checked) {
                eventlevel[i].checked = false;
            }
        }
        email.focus();
    }
}

// 删除表格中的邮件
function delEmail()
{
    var email_table = jQuery("#emailTable");
    var checkchoice = jQuery("input[name='maillist']");
    var a = 0;
    
    for (var j = 0; j < checkchoice.length; j++) {
        if (jQuery("input[name='maillist']").eq(j).is(":checked")) {
            jQuery("input[name='maillist']").eq(j).parent("td").parent("tr").remove();
            checkchoice.length -= 1;
            j -= 1;
            a++;
        }
    }
    if (a == "0") {
        alert("未选中任何邮箱！");
    }
}

//处理输入的邮箱地址 返回字符串
function divideData(data)
{
    var newData  = data.value.split(";");
    var lastData = "";
    for (var i = 0; i < newData.length; i++) {
        if (newData[i] != "") {
            lastData += newData[i] + ";";
        }
    }
    return lastData;
}
//处理邮箱地址 不加分号 返回数组
function divideData2(data)
{
    var newData  = data.value.split(";");
    var lastData = new Array();
    for (var i = 0; i < newData.length; i++) {
        if (newData[i] != "") {
            lastData.push(newData[i]);
        }
    }
    return lastData;
}


//事件通知 》是否需要认证 的显示和隐藏
function checkConfirm(obj,pDiv){
    var pdiv = document.getElementById(pDiv);
    if(!obj.checked){
        pdiv.style.display = "none";
    }else{
        pdiv.style.display = "block";
    }
}

//系统设置》管理口令


//判断导入包是否为空
function checkUpdate(txt){
    var itxt = document.getElementById(txt);
    if(itxt.value == ""){
        alert("未选取导入包！");
        return false;
    }
}

//NAS共享 》新建/编辑共享 SMB 判断控件间的相互影响关系
function checknameless(obj,obj_c,sel){
    var check2 = document.getElementById(obj_c);
    var select2 = document.getElementById(sel);
    if(obj.checked){
        check2.disabled = false;
        select2.disabled = true;
    }else{
        check2.disabled = true;
        select2.disabled = false;
    }
}

//NAS共享 》新建/编辑 添加和删除表格记录
//将NFS中 编辑的内容添加到左边的表格中去
function addnfs(nfstable,nfsin,nfsel,nfs,nfs_tdName,nfs_tdLimit){
    var a = 1;
    var allowip = document.getElementById(nfsin);
    var sellimit = document.getElementById(nfsel);
    var allowname = document.getElementsByName(nfs + 'choice');
    var nfs_tdname = document.getElementsByName(nfs_tdName);
    
    for(var j=0; j<allowname.length; j++){
        a++;
        if(nfs_tdname[j].value == allowip.value){
            alert("不能添加相同的ip字段或组名！");
            //allowip.value = "";
            allowip.select();
            allowip.focus();
            return false;
        }
    }
    if(allowip.value == ""){
        alert("ip字段或组名不能为空值！");
        allowip.focus();
        return false;
    }
    var rowID = a;//rowID是为了给每行的 共享名称 添加特定的ID值
    nfs_table = document.getElementById(nfstable);
     
    //添加行
    var nfs_tr = nfs_table.insertRow();
    
    //添加列
    var nfs_td1=nfs_tr.insertCell(0);
    nfs_td1.innerHTML = '<input type="checkbox" name="' + nfs + 'choice" id="' + nfs + 'choice' + rowID + '" onclick="Item(this,\''+nfs+"_check'"+')" />';
     
    var nfs_td2=nfs_tr.insertCell(1);
    //nfs_td2.innerHTML = allowip.value;
    nfs_td2.innerHTML = '<label for="' + nfs + rowID + '">' + '<input type="text" value=' +allowip.value+ ' size="10" class="nobg_ipt" name="'+nfs_tdName+'" readonly />';
            
    
    var nfs_td3=nfs_tr.insertCell(2);
    //nfs_td3.innerHTML = sellimit.value + '</label>';
    nfs_td3.innerHTML = '<label for="' + nfs + rowID + '">' + '<input type="text" value=' +sellimit.value+ ' size="3" class="nobg_ipt" name="'+nfs_tdLimit+'" readonly />';
    
    //将行号推进下一行
    rowID += 1;
}
function delnfs(nfstable,nfschoice){
    var nfs_table = document.getElementById(nfstable);
    var allowname = document.getElementsByName(nfschoice);
    for(var j = 1;j <= allowname.length;j++){
        cbox = nfs_table.rows[j].childNodes[0].childNodes[0];
        if(cbox.checked){
            nfs_table.deleteRow(j--);
        }
    }
}

/*NAS管理 》NAS共享 》弹框tab切换*/
function sharewaySet(obj,showdiv){
    var a=0;
    var tab = jQuery(".gxway");
    var setdiv = jQuery(".ns_model");
    var cboxnew = document.getElementsByName("shareway");
    for( var i=0; i<tab.length; i++){
        tab[i].className = tab[i].className.replace(" nas_select","");
        setdiv[i].style.display = "none";
    }
    obj.className += " nas_select";
    document.getElementById(showdiv).style.display = "block";
    var a = jQuery(obj).text();
    if(jQuery(obj).parent("ul").parent(".newns_nav").attr("id") != "isDisabled" && jQuery(obj).children("input").is(":checked")){
        jQuery("#"+showdiv+" input,#"+showdiv+" select,#" +showdiv+ " textarea").attr("disabled",false);
    }else{
        jQuery("#"+showdiv+" input,#"+showdiv+" select,#" +showdiv+ " textarea").attr("disabled",true);
    }  
}
//弹框切换时，checkbox的所控制的层 控件可用或不可用事件
function nasTabDis(o,pdiv){
    if(o.checked == true){
        jQuery("#" +pdiv+ " input,#" +pdiv+ " select,#" +pdiv+ " textarea").attr("disabled",false);
    }else{
        jQuery("#" +pdiv+ " input,#" +pdiv+ " select,#" +pdiv+ " textarea").attr("disabled",true);
        jQuery("#" +pdiv+ " input[type='radio']").attr("checked",false);
    }
}

/*消冗文件系统*/
function showSuper(btn,showDiv){
    var superdiv = document.getElementById(showDiv);
    if(btn.value == "高级选项"){
        superdiv.style.display = "block";
        btn.value = "隐藏";
    }else{
        superdiv.style.display = "none";
        btn.value = "高级选项";
    }
}
