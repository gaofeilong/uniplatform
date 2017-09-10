function set_ajax(formId,URL,pwdHandle,resultId)
{
    ThinkAjax.sendForm(formId,URL,pwdHandle,resultId);
}
function pwdHandle(data,status){
    if (status == "1") {
        setTimeout("window.location = 'index.php?s="+data+"';removeCorage()",2000);
    } else {
        removeCorage();
    }
}
function pwdSet(data,status)
{
    if (status == "1") {
        setTimeout("window.location = 'index.php?s=/Password/index';removeCorage();",2000);
    } else if (status == "3") {
        showNoticeMsg("password","原口令输入错误！");
        //createCorageRight();
        removeCorage();
    } else {
    /*else {
    	createCorageRight();
    }*/
        removeCorage();
    }
}

function NasTimeout(data,status)
{
    if (data == "")
    {
        data = "/Share/index";
    }
    if (status == "1") {
        setTimeout("window.location = 'index.php?s="+data+"'",1000);
    }
    removeCorage();
}
function userSet(data,status)
{
    if (status == "1") {
        setTimeout("window.location = 'index.php?s="+data+"'",1000);
    }
    if (status == "2") {
        showNoticeMsg("","密码不一致！");
    }
    removeCorage();
}

//输入有误时，提示框出现
function showNoticeMsg(getMsg,error)
{
  jQuery("#pN_info").css({"display":"block"}).addClass("pN_red");
  jQuery("#pN_info").html(error);
  //jQuery("#"+getMsg).val("");
  jQuery("#"+getMsg).select();
  jQuery("#"+getMsg).focus();
}
//公共获取名称函数
function checkBoxDel(checkboxNaem,tableId,tip,spanCon,tdClass)
{
    var a=jQuery("input[name='"+checkboxNaem+"']");
    var addTable = document.getElementById(tableId);
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
        alert(tip);
        return false;
    } else {
        for(var j = 1;j < addTable.rows.length;j++){
            cbox = addTable.rows[j].childNodes[0].childNodes[0];
            if(cbox.checked){
                vols_no += jQuery("."+tdClass).eq(j-1).html() + ";";
            }
        }
        jQuery("#"+spanCon).val(vols_no);
    }
}

function showMsg(getMsg,error)
{
  alert(error);
  //jQuery("#"+getMsg).val("");
  jQuery("#"+getMsg).select();
  jQuery("#"+getMsg).focus();
}

//公共IP的限制
function limitIp(data,getMsg,error)
{
    var patt_ip = /^(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9])\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[0-9])$/;
    if (!patt_ip.exec(data)) {
        showNoticeMsg(getMsg,error);
                return false;
    } else {
        return true;
    }
}
//公共IP的限制
function limitIpnew(data,getMsg,error)
{
    var patt_ip = /^(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9])\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[0-9])$/;
    if (!patt_ip.exec(data)) {
        showMsg(getMsg,error);
        return false;
    } else {
        return true;
    }
}
//公共邮箱的限制
function limitEmail(data,getMsg,error)
{
    var email_patt = /^(\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*)$/;
    if (!email_patt.exec(data)) {
        showNoticeMsg(getMsg,error);
        return false;
    } else {
        return true;
    }
}
//名称限制
function limitName(name, error)
{
    var name_patt = /^[\.\w]{1,20}$/;
    return name_patt.exec(name);
}

function IsDirRepeat(d1, d2)
{
    var divide = "/";
    var split1 = d1.split(divide);
    var split2 = d2.split(divide);

    var dir1 = "", dir2 = ""; //modify by pww;
    for (var i = 0; i < split1.length; i++) {
        if (split1[i]) {
            dir1 += split1[i] + "/";
        }
    }
    for (var i = 0; i < split2.length; i++) {
        if (split2[i]) {
            dir2 += split2[i] + "/";
        }
    }
    return (dir1.indexOf(dir2) == 0 || dir2.indexOf(dir1) == 0 || dir1 == "" || dir2 == "")? true: false; //modify by pww
}

//管理口令 提交表单
function checkPass()
{
    var admin = document.getElementById("admin");
    var password = document.getElementById("password");
    var newpass = document.getElementById("newPass");
    var newpassok = document.getElementById("newPassOk");
    var pwd_patt = /^(\w){6,20}$/;

    if (!pwd_patt.exec(password.value)) {
        showNoticeMsg("password","原口令输入不合法！");
        return false;
    } else if (!pwd_patt.exec(newpass.value)) {
        showNoticeMsg("newPass","口令输入不合法！");
        jQuery("#newPassOk").val("");
        return false;
    } else if (password.value == newpass.value) {
        showNoticeMsg("newPass","新口令和原口令不能重复！");
        jQuery("#newPassOk").val("");
        return false;
    } else if (newpass.value != newpassok.value) {
        showNoticeMsg("newPass","两次口令不一致！");
        jQuery("#newPassOk").val("");
        return false;
    }
    //ThinkAjax.sendForm(表单ID,URL,回调函数,信息显示的地方);
    createCorage();
    set_ajax('passForm','index.php?s=/Password/setpwd',pwdSet,"pN_info");
    // document.getElementById("passForm").action = "index.php?s=/Password/setpwd";
    // document.getElementById("passForm").submit();
}

//事件通知 提交表单
function checkEvent()
{
    var mailServer    = jQuery("#mailServer").val();
    var mail_from     = jQuery("#mail_from").val();
    var mail_fromPass = jQuery("#mail_fromPass").val();
    var identyWay     = document.getElementsByName("identy_way");//认证方式
    var exitMail      = document.getElementsByName("eventMail[]");//已存在的邮箱
    var pwd_patt      = /^(\w){6,20}$/;
    var isCheck       = document.getElementById("warn");
    if (isCheck.checked) {  
        if (!limitIp(mailServer,"mailServer","邮件服务器不合法！") ||　
            !limitEmail(mail_from,"mail_from","发件箱输入不合法！")) {
            return false;
        }
        if (mail_fromPass == "") {
        	showNoticeMsg("mail_fromPass","发件箱密码不能为空！");
            return false;
        }
    }
    if (!exitMail.length == 0) {
        var acheck = 0;
        for (var i = 0; i < identyWay.length; i++) {
            if (identyWay[i].checked) {
                acheck++;
            }
        }
        if (acheck < 1) {
            alert("请选择认证方式！");
            return false;
        }
    }
    //document.getElementById("eventForm").action = "index.php?s=/Event/setEmail";
    //document.getElementById("eventForm").submit();
    createCorage();
    set_ajax('eventForm','index.php?s=/Event/setEmail',pwdHandle,"pN_info");
}
//测试邮件
function testEmail()
{
	var mailServer    = jQuery("#mailServer").val();
    var mail_from     = jQuery("#mail_from").val();
    var mail_fromPass = jQuery("#mail_fromPass").val();
    var newEmail      = jQuery("#newmail").val()+";";
    var identyWay     = document.getElementsByName("identy_way");//认证方式
    var pwd_patt      = /^(\w){6,20}$/;
    var isCheck       = document.getElementById("warn");
    if (isCheck.checked) {  
        if (!limitIp(mailServer,"mailServer","邮件服务器不合法！") ||　
            !limitEmail(mail_from,"mail_from","发件箱输入不合法！")) {
            return false;
        }
        if (!pwd_patt.exec(mail_fromPass)) {
        	showNoticeMsg("mail_fromPass","发件箱密码不合法！");
            return false;
        }
        var acheck = 0;
        for (var i = 0; i < identyWay.length; i++) {
            if (identyWay[i].checked) {
                acheck++;
            }
        }
        if (acheck < 1) {
            alert("请选择认证方式！");
            return false;
        }
        var recv = newEmail.split(";");
        for (var i = 0; i < recv.length; i++) {
        	if (recv[i] != "") {
        		if (!limitEmail(recv[i],"newmail","邮件地址不合法！")) {
                    return false;
                }
        	}
        }
    }
    createCorage();
   //document.getElementById("eventForm").action = "index.php?s=/Event/testEmail";
   //document.getElementById("eventForm").submit();
   set_ajax('eventForm','index.php?s=/Event/testEmail',pwdHandle,"pN_info");
}

function timeSelect(name,date)
{
    for (var i = 0;i < name.options.length;i++) {
        if (name.options[i].value == date) {
            name.options[i].selected = "selected";
        }
    }
}
//刚打开页面时时间的显示
function setTime()
{
    var webHour = jQuery("#webHour").val();
    var webMin  = jQuery("#webMin").val();
    var webSec  = jQuery("#webSec").val();
    
    //时间的显示
    var hour = document.getElementById("hour");
    var minute = document.getElementById("minute");
    var second = document.getElementById("second");
    timeSelect(hour,webHour);
    timeSelect(minute,webMin);
    timeSelect(second,webSec);
    
}
//系统时间 提交表单
function checkTime(){
    var ntp = jQuery("#setNTP").val();

    // 如果选中同步时间，那么要求输入NTP服务器的合法IP地址
    // 如果不选中同步，那么NTP服务器的IP地址可以为空或者合法的IP地址
    if (document.getElementById("nowSyn").checked) {
        if (limitIp(ntp,"setNTP","NTP时间服务器输入不合法！")) {
            createCorage()
            set_ajax('setSysTime','index.php?s=/Stime/setTime',pwdHandle,"pN_info");
        }
    } else {
        if (ntp == "" || limitIp(ntp,"setNTP","NTP时间服务器输入不合法！")) {
            createCorage()
            set_ajax('setSysTime','index.php?s=/Stime/setTime',pwdHandle,"pN_info");
        }
    }
}
/*遮住右边内容的遮罩层*/
function createCorageRight()
{
    var divcorage = document.createElement("div");
    divcorage.className = "corageRight";
    document.body.appendChild(divcorage);
}
function removeCorageRight()
{
    jQuery(".corageRight").remove();
}
/*遮住整体页面的遮罩层*/
function createCorage(){
    //创建页面遮罩层
    var divcorage = document.createElement("div");
    divcorage.id = "corage_data";
    divcorage.className = "corage_data";
    document.body.appendChild(divcorage);
    
    //创建顶部遮罩层
    var topcorage = document.createElement("div");
    topcorage.id = "corage_data";
    topcorage.className = "corage_data";
    parent.header.document.body.appendChild(topcorage);
    
    //创建左边遮罩层
    var leftcorage = document.createElement("div");
    leftcorage.id = "corage_data";
    leftcorage.className = "corage_data";
    parent.left.document.body.appendChild(leftcorage);
    
    jQuery("#pN_info").removeClass("pN_red");//去除提示信息的红色样式，避免给“数据处理”等造成影响
}
function removeCorage(){
    var corage = document.getElementById("corage_data");
    document.body.removeChild(corage);
    
    //var corageLeft = parent.left.document.getElementById("corage_data");
    parent.left.jQuery(".corage_data").remove();
    
    //var corageTop = parent.header.document.getElementById("corage_data");
    parent.header.jQuery(".corage_data").remove();
}

/*
 *UPS设置
 */
function Ups_set()
{
    var electric = document.getElementById("letter").value*1;
    var lastTime = document.getElementById("lefttime").value;
    var ele_patt = /^([1-9]?\d|100)$/;
    var time_patt = /^[0-9]{1,4}$/;
    var upsCheck = document.getElementById("upsCheck");
    if (upsCheck.checked) {
        if (electric < 0 || electric > 100 || isNaN(electric)) {
            showNoticeMsg("letter","剩余电量输入不合法!");
            return false;
        } else if (!time_patt.exec(lastTime)) {
            showNoticeMsg("lefttime","剩余时间输入不合法!");
            return false;
        }
    }
    createCorage();
    set_ajax('upsForm','index.php?s=/Ups/setUps',pwdHandle,"pN_info");
}

/*
 * License页面
 */


/*
 * 基础网络配置
 */
function Base_set()
{
    var hostName = document.getElementById("hostName");
    var dnsFirst = document.getElementById("dnsFirst");
    var dnsReserve = document.getElementById("dnsReserve");
    var dnsPatt  = /^(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9])\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[0-9])$/;
    if (!limitName(hostName.value)) {
        showNoticeMsg("hostName","主机名输入不合法!");
        return false;
    }
    if (!dnsPatt.exec(dnsFirst.value) && dnsFirst.value != "") {
        showNoticeMsg("dnsFirst","首选DNS输入不合法!");
        return false;
    } 
    if (!dnsPatt.exec(dnsReserve.value) && dnsReserve.value != "") {
        showNoticeMsg("dnsReserve","备用DNS输入不合法!");
        return false;
    }
    if (dnsFirst.value == "" && dnsReserve.value != "") {
        showNoticeMsg("dnsFirst","优先配置首选DNS!");
        return false;
    }

    //document.getElementById("baseNetForm").action = "index.php?s=/Basenet/setBasenet";
    //document.getElementById("baseNetForm").submit();
    createCorage();
    set_ajax('baseNetForm','index.php?s=/Basenet/setBasenet',pwdHandle,"pN_info");
}
/*
 * 网卡配置页面
*/
function idbondcard()
{
	var name = jQuery("#netBondName").val();
	if (name == "") {
		document.getElementById("Bond_set").disabled = false;
	} else {
		document.getElementById("Bond_set").disabled = true;
	}
}
function setNetwork()
{
	
	/*var bondIpList  = jQuery(".bondIpList"); //绑定网卡的IP
	var netId       = jQuery("#netId").val();
	var ipList      = jQuery(".ipList");*/
	
    var mtu            = jQuery("#nobondMtu").val();
    var cardSetIp      = jQuery("#cardSetIp").val();
    var cardSetmask    = jQuery("#cardSetmask").val();
    var cardSetgeteWay = jQuery("#cardSetgeteWay").val();
    var patt_mtu = /^\d+$/;
    if (mtu < 0 || mtu > 1500 || !patt_mtu.exec(mtu)) {
        showMsg("nobondMtu","MTU输入不合法！");
        return false;
    }
    if (document.getElementById("cardhand_ip").checked == true) {
    	if (!limitIpnew(cardSetIp,"cardSetIp","IP地址输入不合法！") || !limitIpnew(cardSetmask,"cardSetmask","子网掩码输入不合法！") ) {
            return false;
        } else if (cardSetgeteWay != '') {
        	if (!limitIpnew(cardSetgeteWay,"cardSetgeteWay","网关输入不合法！")) {
        		return false;
        	}
        }
    	/*//和其他网卡的IP比较
    	for (var i = 0; i < ipList.length; i++) {
    		if (i+1 != netId) {
    			if (ipList.eq(i).text() == cardSetIp) {
        			alert("IP地址已经存在");
        			jQuery("#cardSetIp").select();
        			return false;
        		}
    		}
    	}
    	//和绑定网卡的IP比较
    	for (var i = 0; i < bondIpList.length; i++) {
    		if (bondIpList.eq(i).text() == cardSetIp) {
    			alert("IP地址已经存在");
    			jQuery("#cardSetIp").select();
    			return false;
    		}
    	}*/
    }
    
    document.getElementById("mtu_name").disabled = false;
    //document.getElementById("setNetfrm").action = "index.php?s=/Network/setNetwork";
    //document.getElementById("setNetfrm").submit();
    closeCover();
    createCorage();
    set_ajax('setNetfrm','index.php?s=/Network/setNetwork',pwdHandle,"pN_info");
}
//绑定网卡
function bondNetwork()
{
    var bondmtu    = jQuery("#setbondmtu").val();
    var setIp      = jQuery("#bondcardIp").val();
    var setmask    = jQuery("#bondcardmask").val();
    var setgeteWay = jQuery("#bondcardga").val();
    var patt_mtu   = /^\d+$/;
    if (bondmtu < 0 || bondmtu > 1500 || !patt_mtu.exec(bondmtu)) {
        showMsg("setbondmtu","MTU输入不合法！");
        return false;
    }
    
    if (document.getElementById("hand_setip").checked == true) {
    	if (!limitIpnew(setIp,"bondcardIp","IP地址输入不合法！") || !limitIpnew(setmask,"bondcardmask","子网掩码输入不合法！")) {
            return false;
        } else if (setgeteWay != '') {
        	if (!limitIpnew(setgeteWay,"bondcardga","网关输入不合法！")) {
        		return false;
        	}
        }
    }
    
    //document.getElementById("bondNetfrm").action = "index.php?s=/Network/bondNetwork";
    //document.getElementById("bondNetfrm").submit();
    closeCover();
    createCorage();
    set_ajax('bondNetfrm','index.php?s=/Network/bondNetwork',pwdHandle,"pN_info");
}
//绑定网卡的配置
function bondworkSet()
{
    var bondmtuset  = jQuery("#bondmtu_set").val();
    var bondIp      = jQuery("#bondSetIp").val();
    var bondmask    = jQuery("#bondSetmask").val();
    var bondgeteWay = jQuery("#bondSetgeteWay").val();
    var patt_mtu    = /^\d+$/;
    if (bondmtuset < 0 || bondmtuset > 1500 || !patt_mtu.exec(bondmtuset)) {
        showMsg("setbondmtu","MTU输入不合法！");
        return false;
    }
    
    if (document.getElementById("hand_set").checked == true) {
    	if (!limitIpnew(bondIp,"bondSetIp","IP地址输入不合法！") || !limitIpnew(bondmask,"bondSetmask","子网掩码输入不合法！")) {
            return false;
        } else if (bondgeteWay != '') {
        	if (!limitIpnew(bondgeteWay,"bondSetgeteWay","网关输入不合法！")) {
        		return false;
        	}
        }
    }
    
    document.getElementById("con_devi").disabled = false;
    document.getElementById("bond_name").disabled = false;
    //document.getElementById("bondWorkSet").action = "index.php?s=/Network/bondWorkSet";
    //document.getElementById("bondWorkSet").submit();
    closeCover();
    createCorage();
    set_ajax('bondWorkSet','index.php?s=/Network/bondWorkSet',pwdHandle,"pN_info");
}
function bondDel()
{
    var bondDelname = jQuery("#del_bondname").text();
    //document.getElementById("bondDelfrm").action = "index.php?s=/Network/bondDel/bondname/"+bondDelname;
    //document.getElementById("bondDelfrm").submit();
    closeCover();
    createCorage();
    ThinkAjax.send("index.php?s=/Network/bondDel/",'ajax=1&bondname='+bondDelname,pwdHandle,"pN_info");
}

/*
 * 卷管理页面
*/
function lvExtend()
{
    //document.getElementById("lv_name").disabled = false;
    //document.getElementById("lvExtendfrm").action = "index.php?s=/Lv/lvExtend";
    //document.getElementById("lvExtendfrm").submit();
    var exspace = jQuery("#letter").val();
    var allspace = jQuery("#usable_space").val();
    if (allspace*1 < exspace*1)
    {
         alert("不能超过可扩展上限。");
         return false;
    }
    closeCover();
    createCorage();
    set_ajax('lvExtendfrm','index.php?s=/Lv/lvExtend',lvDataSet,"pN_info");
}
function lvCreate()
{
    var lvname = jQuery("#lvnewname");
    if (lvname.val() == "") {
        alert("卷名称不能为空！");
        lvname.focus();
        return false;
    }
    //document.getElementById("lvcreatefrm").action = "index.php?s=/Lv/lvCreate";
    //document.getElementById("lvcreatefrm").submit();
    closeCover();
    createCorage();
    set_ajax('lvcreatefrm','index.php?s=/Lv/lvCreate',pwdHandle,"pN_info");
}
function delLv()
{
    //document.getElementById("delLvfrm").action = "index.php?s=/Lv/delLv";
    //document.getElementById("delLvfrm").submit();
    closeCover();
    createCorage();
    set_ajax('lvListForm','index.php?s=/Lv/delLv',pwdHandle,"pN_info");
}
/*
 * FC设置页面
*/
function FCset()
{
    document.getElementById("FCfrm").action = "index.php?s=/Fc/fcSet";
    document.getElementById("FCfrm").submit();
    //createCorage();
    //set_ajax('FCfrm','index.php?s=/Fc/fcSet',pwdHandle,"pN_info");
}
/*
 * ISCSI设置页面
*/
function setIscsi()
{
    document.getElementById("iscsiForm").action = "index.php?s=/Iscsi/iscsiSet";
    document.getElementById("iscsiForm").submit();
    //createCorage();
    //set_ajax('iscsiForm','index.php?s=/ISCSI/iscsiSet',pwdHandle,"pN_info");
}


/*
 * 卷组页面
*/
function vgExent(){
    checkBoxDel("ku_choice","lvexentTab","未选中任何物理卷!","hidLvexName","lvexentName")
    document.getElementById("vgExentForm").action = "index.php?s=/Vg/vgExent";
    document.getElementById("vgExentForm").submit();
    closeCover();
    //createCorage();
    //set_ajax('vgExentForm','index.php?s=/Vg/vgExent',vgDataSet,"pN_info");
}
function vgCreate()
{
    checkBoxDel("wuli_choice","vgCreateTab","未选中任何物理卷!","hidvgCreate","vgCreName")
    document.getElementById("vgCreForm").action = "index.php?s=/Vg/vgCreate";
    document.getElementById("vgCreForm").submit();
    closeCover();
    //createCorage();
    //set_ajax('vgCreForm','index.php?s=/Vg/vgCreate',vgDataSet,"pN_info");
}
function vgDelete()
{
    document.getElementById("vgDelForm").action = "index.php?s=/Vg/vgDel";
    document.getElementById("vgDelForm").submit();
    closeCover();
    //createCorage();
    //set_ajax('vgDelForm','index.php?s=/Vg/vgDel',vgDataSet,"pN_info");
}
/*
 * 数据归档
*/
//新建归档
function createArchive(){
    var newdocName = document.getElementById("archive_name");//获取归档任务名称
    var docSrcdir = document.getElementsByName("archiveSrcdir[]");//获取源目标路径
    var docDectdir = document.getElementsByName("archiveDestdir[]");//获取目标路径
    var docDay = document.getElementById("letter");
    var starthour = document.getElementById("starttime_hour").value;
    var startminute = document.getElementById("starttime_minute").value;
    var stophour = document.getElementById("stoptime_hour").value;
    var allArvName = jQuery(".document");
    var datatime = document.getElementById("letter");
    var allSrcDirList = new Array();

    if (datatime.value < 1) {
        alert("归档数据时间非法");
        datatime.select();
        return false;
    }

    if (!limitName(newdocName.value)) {
        alert("归档任务名称不合法！");
        newdocName.select();
        newdocName.focus();
        return false;
    }
    for (var i = 0; i < allArvName.length; i++) {
        if (newdocName.value == allArvName.eq(i).text()) {
            alert("归档任务名称已经存在！");
            newdocName.select();
            newdocName.focus();
            return false;
        }
    }
    //IP列表节点不能为空
    var ipNode = jQuery("#newIp").find(".ipson").children(":checkbox");
    var ipnum  = 0;
    if (ipNode.length > 0){
        for (var i = 0; i < ipNode.length; i++) {
            if (ipNode.is(":checked")) {
                ipnum++;
            }
        }
        if (ipnum == 0) {
            alert("请选择节点IP！");
            return false;
        }
    }
    
    //判断源和目标路径是否为空
    for (var i = 0; i < docSrcdir.length; i++) {
    	if (docSrcdir[i].value == "") {
            alert("源路径不能为空！");
            docSrcdir[i].focus();
            return false;
         }
         if (docDectdir[i].value == "") {
            alert("目标路径不能为空！");
            docDectdir[i].focus();
            return false;
         }
    }
    
    var pathPairList = jQuery(".infopathMsg").children("span:even"); //源路径
    //var goalPairList = jQuery(".infopathMsg").children("span:odd");  //目标路径
    for (var i = 0; i < pathPairList.length; i++) {
        allSrcDirList.push(pathPairList.eq(i).text()); //已经添加的源路径
    }
    

    for (var i = 0; i < docSrcdir.length; i++) {
        // 与自身源路径是否存在重复、子路径
        for (var k = i + 1; k < docSrcdir.length; k++) {
            if (IsDirRepeat(docSrcdir[i].value, docSrcdir[k].value)) {
                alert("源路径不合法: " + docSrcdir[k].value);
                docSrcdir[k].select();
                docSrcdir[k].focus();
                return false;
            } 
        }
        // 与新添加目标路径是否存在重复、子路径
        for (var k = 0; k < docDectdir.length; k++) {
            if (IsDirRepeat(docSrcdir[i].value, docDectdir[k].value)) {
                if (i <= k) {
                    alert("目标路径不合法: " + docDectdir[k].value);
                } else {
                    alert("源路径不合法: " + docSrcdir[i].value);
                }
                docSrcdir[i].select();
                docSrcdir[i].focus();
                return false;
            } 
        }
        
        // 当为单机归档时，与其他归档任务源路径是否存在重复、子路径
        if (document.getElementById("archiveIpnode")) {
            for (var j = 0; j < allSrcDirList.length; j++) {
                if (IsDirRepeat(allSrcDirList[j], docSrcdir[i].value)) {
                    alert("源路径已经存在: " + docSrcdir[i].value);
                    docSrcdir[i].select();
                    docSrcdir[i].focus();
                    return false;
                }
            }
        }
    }
        
    //判断前缀后缀
	var licbox = jQuery("#new_document").find(".gxway").children("input");
    if (licbox.eq(0).is(":checked")) {
        var prefixType = licbox.eq(0).parents(".newns_nav").children(".ns_model").eq(0).find("input[type='radio']");
        var preVal     = licbox.eq(0).parents(".newns_nav").children(".ns_model").eq(0).find("textarea");
        if (prefixType.eq(0).is(":checked") || prefixType.eq(1).is(":checked")) {
            if (!prepost(preVal.val())) {
                alert("前缀表达式输入不合法！");
                jumpShowError("new_document", licbox.eq(0), "prefixdiv");
                preVal.select();
                return false;
            }
        } else {
            alert("前缀没有选择指定或过滤！");
            jumpShowError("new_document", licbox.eq(0), "prefixdiv");
            prefixType.eq(0).focus();
            return false;
        }
    }
    if (licbox.eq(1).is(":checked")) {
        var postfixType = licbox.eq(1).parents(".newns_nav").children(".ns_model").eq(1).find("input[type='radio']");
        var postVal     = licbox.eq(1).parents(".newns_nav").children(".ns_model").eq(1).find("textarea");
        if (postfixType.eq(0).is(":checked") || postfixType.eq(1).is(":checked")) {
            if (!prepost(postVal.val())) {
                alert("后缀表达式输入不合法！");
                jumpShowError("new_document", licbox.eq(1), "postfixdiv");
                postVal.select();
                return false;
            }
        } else {
            alert("后缀没有选择指定或过滤！");
            jumpShowError("new_document", licbox.eq(1), "postfixdiv");
            postfixType.eq(0).focus();
            return false;
        }
    }
    if (licbox.eq(2).is(":checked")) {
        var sizeType = licbox.eq(2).parents(".newns_nav").children(".ns_model").eq(2).find("input[type='radio']");
        var sizeVal  = licbox.eq(2).parents(".newns_nav").children(".ns_model").eq(2).find("input[type='text']");
        var mapSize  = /^[0-9]+$/;
        var minSizeUnit = document.getElementById("minSizeUnit");
        var maxSizeUnit = document.getElementById("maxSizeUnit");
        if (sizeType.eq(0).is(":checked") || sizeType.eq(1).is(":checked")) {
            for (var i = 0; i < sizeVal.length; i++) {
                if (!mapSize.exec(sizeVal.eq(i).val())) {
                    alert("文件大小请输入数字！");
                    jumpShowError("new_document", licbox.eq(2), "sizetypediv");
                    sizeVal.eq(i).select();
                    return false;
                }
            }
            if (!compareSize(sizeVal, minSizeUnit, maxSizeUnit)) {
                return false;
            }
        } else {
            alert("文件大小没有选择指定或过滤！");
            jumpShowError("new_document", licbox.eq(2), "sizetypediv");
            sizeType.eq(0).focus();
            return false;
        }
    }
    if (licbox.eq(3).is(":checked")) {
        var dirType = licbox.eq(3).parents(".newns_nav").children(".ns_model").eq(3).find("input[type='radio']");
        var dirVal  = licbox.eq(3).parents(".newns_nav").children(".ns_model").eq(3).find("textarea");
        var mapDir  = /^\/[\.;\/\w]+$/;
        if (dirType.eq(0).is(":checked")) {
            if (!mapDir.exec(dirVal.val())) {
                alert("目录表达式输入不合法！");
                jumpShowError("new_document", licbox.eq(3), "dirdiv");
                dirVal.select();
                return false;
            }
        } else {
            alert("目录没有勾选中过滤！");
            jumpShowError("new_document", licbox.eq(3), "dirdiv");
            dirType.eq(0).focus();
            return false;
        }
    }

    closeCover();
    createCorage();
    if (document.getElementById("archiveIpnode")) {
    	document.getElementById("archiveIpnode").disabled = false;
    }
    //document.getElementById("createDocuForm").action = "index.php?s=/Archive/createArchive";
    //document.getElementById("createDocuForm").submit();
    set_ajax('createDocuForm','index.php?s=/Archive/createArchive',pwdHandle,"pN_info");
}
//比较文件最大最小值
function compareSize(sizeVal,min, max)
{
    var minVal = sizeVal.eq(0).val()*(Math.pow(1024, min.selectedIndex));
    var maxVal = sizeVal.eq(1).val()*(Math.pow(1024, max.selectedIndex));
    var flag = true;
    //alert(minVal +"min --- max"+ maxVal)
    if (sizeVal.eq(0).val() != 0 && sizeVal.eq(1).val() != 0 && minVal > maxVal) {
        alert("文件最小值不得大于最大值！");
        sizeVal.eq(0).select();
        flag = false;
    }
    return flag;
}
function prepost(prepostVal)
{
    var mapString = /^[.;\w]+$/ ;
    return mapString.exec(prepostVal);
}
//当判断前缀后缀的输入是否合法时，跳转到输入不合法的层
function jumpShowError(belongDiv, obj, showdiv)
{   
    var licbox = jQuery("#"+ belongDiv).find(".gxway");
    var setdiv = jQuery("#"+ belongDiv).find(".ns_model");
    for( var i=0; i<licbox.length; i++){
        licbox[i].className = licbox[i].className.replace(" nas_select","");
        setdiv[i].style.display = "none";
    }
    obj.parent("li").addClass("nas_select");
    obj.attr("checked",true);
    document.getElementById(showdiv).style.display = "block";
}

function editArchive(){
    var docSrcdir = document.getElementsByName("edit_archiveSrcdir[]");//获取源目标路径
    var docDestdir = document.getElementsByName("edit_archiveDestdir[]");//获取目标路径
    var starthour = document.getElementById("editbe_h").value;
    var startminute = document.getElementById("editbe_m").value;
    var stophour = document.getElementById("editstop_ho").value;
    var pathId = document.getElementById("pathId").value;
    var datatime = document.getElementById("edit_letter");
    var allSrcDirList = new Array();

    if (datatime.value < 1) {
        alert("归档数据时间非法");
        datatime.select();
        return false;
    }
    
    //IP列表节点不能为空
    var ipNode = jQuery("#editIp").find(".ipson").children(":checkbox");
    var ipnum  = 0;
    if (ipNode.length > 0){
        for (var i = 0; i < ipNode.length; i++) {
            if (ipNode.is(":checked")) {
                ipnum++;
            }
        }
        if (ipnum == 0) {
            alert("请选择节点IP！");
            return false;
        }
    }
    
    //判断源和目标路径是否为空
    for (var i = 0; i < docSrcdir.length; i++) {
    	if (docSrcdir[i].value == "") {
            alert("源路径不能为空！");
            docSrcdir[i].focus();
            return false;
         }
         if (docDestdir[i].value == "") {
            alert("目标路径不能为空！");
            docDestdir[i].focus();
            return false;
         }
    }

    var pathPairList = jQuery(".infopathMsg").children("span:even").not(".spanEven" + pathId);
    for (var i = 0; i < pathPairList.length; i++) {
        allSrcDirList.push(pathPairList.eq(i).text());
    }

    for (var i = 0; i < docSrcdir.length; i++) {
    	// 与新添加源路径是否存在重复、子路径
        for (var k = i + 1; k < docSrcdir.length; k++) {
            if (IsDirRepeat(docSrcdir[i].value, docSrcdir[k].value)) {
                alert("源路径不合法: " + docSrcdir[k].value);
                docSrcdir[k].select();
                docSrcdir[k].focus();
                return false;
            } 
        }
        
        // 与新添加目标路径是否存在重复、子路径
        for (var k = 0; k < docDestdir.length; k++) {
            if (IsDirRepeat(docSrcdir[i].value, docDestdir[k].value)) {
                if (i <= k) {
                    alert("目标路径不合法: " + docDestdir[k].value);
                } else {
                    alert("源路径不合法: " + docSrcdir[i].value);
                }
                docSrcdir[i].select();
                docSrcdir[i].focus();
                return false;
            } 
        }
        
        // 当为单机归档时，与其他归档任务源路径是否存在重复、子路径
        if (document.getElementById("editarcIpnode")) {
            for (var j = 0; j < allSrcDirList.length; j++) {
                if (IsDirRepeat(allSrcDirList[j], docSrcdir[i].value)) {
                    alert("源路径已经存在: " + docSrcdir[i].value);
                    docSrcdir[i].select();
                    docSrcdir[i].focus();
                    return false;
                }
            }
        }
    }
    
    //前缀后缀
    var licbox = jQuery("#edit_doc").find(".gxway").children("input");
    if (licbox.eq(0).is(":checked")) {
        var prefixType = licbox.eq(0).parents(".newns_nav").children(".ns_model").eq(0).find("input[type='radio']");
        var preVal = licbox.eq(0).parents(".newns_nav").children(".ns_model").eq(0).find("textarea");
        if (prefixType.eq(0).is(":checked") || prefixType.eq(1).is(":checked")) {
            if (!prepost(preVal.val())) {
                alert("前缀表达式输入不合法！");
                jumpShowError("edit_doc", licbox.eq(0), "Eprefixdiv");
                preVal.select();
                return false;
            }
        } else {
            alert("前缀没有选择指定或过滤！");
            jumpShowError("edit_doc", licbox.eq(0), "Eprefixdiv");
            prefixType.eq(0).focus();
            return false;
        }
    }
    if (licbox.eq(1).is(":checked")) {
        var postfixType = licbox.eq(1).parents(".newns_nav").children(".ns_model").eq(1).find("input[type='radio']");
        var postVal = licbox.eq(1).parents(".newns_nav").children(".ns_model").eq(1).find("textarea");
        if (postfixType.eq(0).is(":checked") || postfixType.eq(1).is(":checked")) {
            if (!prepost(postVal.val())) {
                alert("后缀表达式输入不合法！");
                jumpShowError("edit_doc", licbox.eq(1), "Epostfixdiv");
                postVal.select();
                return false;
            }
        } else {
            alert("后缀没有选择指定或过滤！");
            jumpShowError("edit_doc", licbox.eq(1), "Epostfixdiv");
            postfixType.eq(0).focus();
            return false;
        }
    }
    if (licbox.eq(2).is(":checked")) {
        var sizeType = licbox.eq(2).parents(".newns_nav").children(".ns_model").eq(2).find("input[type='radio']");
        var sizeVal  = licbox.eq(2).parents(".newns_nav").children(".ns_model").eq(2).find("input[type='text']");
        var mapSize  = /^[0-9]+$/;
        var minSizeUnit = document.getElementById("EminSizeUnit");
        var maxSizeUnit = document.getElementById("EmaxSizeUnit");
        if (sizeType.eq(0).is(":checked") || sizeType.eq(1).is(":checked")) {
            for (var i = 0; i < sizeVal.length; i++) {
                if (!mapSize.exec(sizeVal.eq(i).val())) {
                    alert("文件大小请输入数字！");
                    jumpShowError("edit_doc", licbox.eq(2), "Esizetypediv");
                    sizeVal.eq(i).select();
                    return false;
                }
            }
            if (!compareSize(sizeVal, minSizeUnit, maxSizeUnit)) {
                return false;
            }
        } else {
            alert("文件大小没有选择指定或过滤！");
            jumpShowError("edit_doc", licbox.eq(2), "Esizetypediv");
            sizeType.eq(0).focus();
            return false;
        }
    }
    if (licbox.eq(3).is(":checked")) {
        var dirType = licbox.eq(3).parents(".newns_nav").children(".ns_model").eq(3).find("input[type='radio']");
        var dirVal  = licbox.eq(3).parents(".newns_nav").children(".ns_model").eq(3).find("textarea");
        var mapDir  = /^\/[\.;\/\w]+$/;
        if (dirType.eq(0).is(":checked")) {
            if (!mapDir.exec(dirVal.val())) {
                alert("目录表达式输入不合法！");
                jumpShowError("edit_doc", licbox.eq(3), "Edirdiv");
                dirVal.select();
                return false;
            }
        } else {
            alert("目录没有勾选中过滤！");
            jumpShowError("edit_doc", licbox.eq(3), "Edirdiv");
            dirType.eq(0).focus();
            return false;
        }
    }
    
    //document.getElementById("editDocForm").action = "index.php?s=/Archive/editArchive";
    //document.getElementById("editDocForm").submit();
    if (document.getElementById("editarcIpnode")) {
    	document.getElementById("editarcIpnode").disabled = false;
    }
    closeCover();
    createCorage();
    set_ajax('editDocForm','index.php?s=/Archive/editArchive',pwdHandle,"pN_info");
}
function delArchive(){
    //document.getElementById("delArchiveForm").action = "index.php?s=/Archive/delArchive";
    //document.getElementById("delArchiveForm").submit();
    closeCover();
    createCorage();
    set_ajax('delArchiveForm','index.php?s=/Archive/delArchive',pwdHandle,"pN_info");
}
//启动服务
function startArch(id)
{
	var nodesIp   = jQuery(".nodesIp"+id);
	var info_name = jQuery("#info_name"+id).text();
    var nodes = new Array();
    for (var i = 0; i < nodesIp.length; i++) {
    	nodes.push(nodesIp.eq(i).text());
    }
    
    var startId       = jQuery("#hidId"+id).val();
    //alert(startId);
    var startTime_h   = jQuery("#infostart_h"+id).text();
    var startTime_m   = jQuery("#infostart_m"+id).text();
    var startTime     = startTime_h+":"+startTime_m;
    var startInterval = jQuery("#infointervalTime"+id).text();
    closeCover();
    createCorage();
    //window.location = "index.php?s=/Archive/startService/startId/"+startId+"/startTime/"+startTime+"/startInterval/"+startInterval+"/nodes/"+nodes+"/arvname/"+info_name;
    ThinkAjax.send("index.php?s=/Archive/startService/",'ajax=1&startId='+startId+'&startTime='+startTime+'&startInterval='+startInterval+'&nodes='+nodes+'&arvname='+info_name,pwdHandle,"pN_info");
}

function archiveHandle(data,status){
	setTimeout("window.location = 'index.php?s=/Archive/index';removeCorage()",2000);
}

function startAlone(idalone,id)
{
	var nodesIps = jQuery("#nodesIps"+id+idalone).text();
	var info_name = jQuery("#info_name"+id).text();
	var startId       = jQuery("#hidId"+id).val();
	//alert(nodesIps);
    var startTime_h   = jQuery("#infostart_h"+id).text();
    var startTime_m   = jQuery("#infostart_m"+id).text();
    var startTime     = startTime_h+":"+startTime_m;
    var startInterval = jQuery("#infointervalTime"+id).text();
    closeCover();
    //window.location = "index.php?s=/Archive/startService/startId/"+startId+"/startTime/"+startTime+"/startInterval/"+startInterval+"/nodes/"+nodesIps+"/arvname/"+info_name;
    createCorage();
	ThinkAjax.send("index.php?s=/Archive/startService/",'ajax=1&startId='+startId+'&startTime='+startTime+'&startInterval='+startInterval+'&nodes='+nodesIps+'&arvname='+info_name,pwdHandle,"pN_info");
}
//停止服务
function stopArch(id)
{
	var nodesIp = jQuery(".nodesIp"+id);
	var info_name = jQuery("#info_name"+id).text();
    var nodes = new Array()
    for (var i = 0; i < nodesIp.length; i++) {
    	nodes.push(nodesIp.eq(i).text());
    }
    var stopId       = jQuery("#hidId"+id).val();
    //alert(stopId);
    var stopTime_h   = jQuery("#infostart_h"+id).text();
    var stopTime_m   = jQuery("#infostart_m"+id).text();
    var stopTime     = stopTime_h+":"+stopTime_m;
    var stopInterval = jQuery("#infointervalTime"+id).text();
    //alert(stopId);alert(stopTime);alert(stopInterval);
    closeCover();
    //window.location = "index.php?s=/Archive/stopService/stopId/"+stopId+"/stopTime/"+stopTime+"/stopInterval/"+stopInterval+"/nodes/"+nodes+"/arvname/"+info_name;
    createCorage();
    ThinkAjax.send("index.php?s=/Archive/stopService/",'ajax=1&stopId='+stopId+'&stopTime='+stopTime+'&stopInterval='+stopInterval+'&nodes='+nodes+'&arvname='+info_name,pwdHandle,"pN_info");
}
function stopAlone(idalone,id)
{
	var nodesIps     = jQuery("#nodesIps"+id+idalone).text();
	var info_name = jQuery("#info_name"+id).text();
	var stopId       = jQuery("#hidId"+id).val();
	//alert(nodesIps);
    var stopTime_h   = jQuery("#infostart_h"+id).text();
    var stopTime_m   = jQuery("#infostart_m"+id).text();
    var stopTime     = stopTime_h+":"+stopTime_m;
    var stopInterval = jQuery("#infointervalTime"+id).text();
    //alert(stopId);alert(stopTime);alert(stopInterval);
    closeCover();
    //window.location = "index.php?s=/Archive/stopService/stopId/"+stopId+"/stopTime/"+stopTime+"/stopInterval/"+stopInterval+"/nodes/"+nodesIps+"/arvname/"+info_name;
    createCorage();
	ThinkAjax.send("index.php?s=/Archive/stopService/",'ajax=1&stopId='+stopId+'&stopTime='+stopTime+'&stopInterval='+stopInterval+'&nodes='+nodesIps+'&arvname='+info_name,pwdHandle,"pN_info");
}
/*
 * DDFS页面
*/
function pathHand()
{
    //var dataPath = jQuery("#dataPath").val();
    var dataPath = jQuery("input[name='dataPath[]']").eq(0).val();
    while (dataPath[dataPath.length - 1] == "/") {
        dataPath = dataPath.substr(0, dataPath.length - 1);
    }
    var metaPath = dataPath + '/metaPath';
    var mapPath  =  dataPath + '/mapPath';
    var biPath   =  dataPath + '/biPath';
    jQuery("#metaPath").val(metaPath);
    jQuery("#mapPath").val(mapPath);
    jQuery("#biPath").val(biPath);
    if (dataPath == "") {
        jQuery("#metaPath").val("");
        jQuery("#mapPath").val("");
        jQuery("#biPath").val("");
    }
}
//公共处理提交的路径函数，把多余的斜线去掉
function managePath(path)
{
    var divide = "/";
    var data = path.split("/");
    for (var i = 0; i < data.length; i++) {
       if (data[i] != ""){
           divide += data[i] + "/";
       }
    }
    var len = divide.length;
    var result = divide.substr(0,(len - 1));
    return result;
}

//新建ddfs
function createDdfs()
{
    var mountPoint    = document.getElementById("mountPoint");    //挂载点路径
    var dataRoad      = document.getElementsByName("dataPath[]"); //数据存储路径
    var metaPath      = document.getElementById("metaPath");      //源数据存储路径
    var mapPath       = document.getElementById("mapPath");       //数据索引
    var biPath        = document.getElementById("biPath");        //二级块索引
    var bucketCount   = document.getElementById("bucketCount");   //分级索引 桶数量
    var minSpace      = document.getElementById("minSpace");      //最小预留空间
    var fsBucketCount = document.getElementById("fsBucketCount"); //文件系统设置 桶数量
    var mmapSize      = document.getElementById("mmapSize");      //内存映射大小
    var allExistMp    = jQuery(".mount_point");
    var allExistData  = jQuery(".allDataPath").children("span");
    var allExistPath  = new Array();
    var num = 0;
    if (document.getElementById("mountWay").value == "adfs") {
        num = 1;
    }

    for (var i = 0; i < allExistMp.length; i++) {
        allExistPath.push(allExistMp.eq(i).text());
    }
    for (var i = 0; i < allExistData.length; i++) {
        allExistPath.push(allExistData.eq(i).text());
    }

    //数据存储路径的比较
    var dataP = new Array();
    /*挂载点路径的处理*/
    if (mountPoint.value == "") {
        alert("挂载点路径不能为空！");
        if (num == 1) {
            dataRoad[0].select();
        } else {
            mountPoint.focus();
        }
        return false;
    }
    //判断数据路径是否为空
    for (var j = 0; j < dataRoad.length; j++) {
        if (dataRoad[j].value == '') {
            alert("数据存储路径不能为空！ ");
            dataRoad[j].focus();
            return false;
        }
    }
    /*if (mapPath.value == "") {
        alert("数据索引路径不能为空！");
        mapPath.focus();
        return false;
    }
    if (biPath.value == "") {
        alert("二级块索引路径不能为空！");
        biPath.focus();
        return false;
    }
    if (bucketCount.value == "" || isNaN(bucketCount.value) ) {
        alert("分级索引中的桶数量不能为空或输入不合法！");
        //bucketCount.value = "";
        bucketCount.select();
        bucketCount.focus();
        return false;
    }
    if (minSpace.value == "" || isNaN(minSpace.value) || minSpace.value < 10) {
        alert("最小预留空间不能为空或输入不合法！");
        minSpace.value = "";
        minSpace.focus();
        return false;
    }
    if (fsBucketCount.value == "" || isNaN(fsBucketCount.value)) {
        alert("文件系统设置中的桶数量不能为空或输入不合法！");
        //fsBucketCount.value = "";
        fsBucketCount.select();
        fsBucketCount.focus();
        return false;
    }
    if (mmapSize.value == "" || isNaN(mmapSize.value)) {
        alert("内存映射大小不能为空或输入不合法！");
        //mmapSize.value = "";
        mmapSize.select();
        mmapSize.focus();
        return false;
    }*/
    
    // 将所有已经存在的挂载点、数据路径放在数组allExistPath中
    // 将新建挂载点的数据路径放在数组B中，新建挂载点路径为mountPoint
    // 和已经存在的比较
    for (var i = 0; i < allExistPath.length; i++) {
        // 挂载点
        /**if (IsDirRepeat(allExistPath[i], mountPoint.value)) {
            alert("挂载点路径不合法: " + mountPoint.value);
            mountPoint.select();
            mountPoint.focus();
            return false;
        }**只判断挂载点是否相同*/
        if (allExistPath[i] == mountPoint.value) {
            alert("挂载点路径不合法！"+ mountPoint.value + "\n如果选择的是adfs系统，请重新输入数据存储路径！" );
            if (num == 1) {
                dataRoad[0].select();
            } else {
                mountPoint.select();
                mountPoint.focus();
            }
            return false;
        }
        // 数据路径
        for (var j = num; j < dataRoad.length; j++) {
            if (IsDirRepeat(allExistPath[i], dataRoad[j].value)) {
                alert("数据路径不合法: " + dataRoad[j].value);
                dataRoad[j].select();
                dataRoad[j].focus();
                return false;
            }
        }
    }
    // 和自己比较
    for (var i = num; i < dataRoad.length; i++) {
        // 挂载点和数据路径比较
        if (IsDirRepeat(dataRoad[i].value, mountPoint.value)) {
            alert("数据路径不合法: " + dataRoad[i].value);
            dataRoad[i].select();
            dataRoad[i].focus();
            return false;
        }
    }
    for (var i = 0; i < dataRoad.length; i++) {
        // 数据路径和数据路径比较
        for (var j = i + 1; j < dataRoad.length; j++) {
            if (IsDirRepeat(dataRoad[i].value, dataRoad[j].value)) {
                alert("数据路径不合法: " + dataRoad[j].value);
                dataRoad[j].select();
                dataRoad[j].focus();
                return false;
            }
        }
    }
    
    //document.getElementById("newddfsForm").action = "index.php?s=/Ddfs/createMountpoint";
    //document.getElementById("newddfsForm").submit();
    closeCover();
    createCorage();
    set_ajax('newddfsForm','index.php?s=/Ddfs/createMountpoint',pwdHandle,"pN_info");
}
//编辑ddfs
function editMountpoint()
{
	//var e_allExistData = jQuery(".allDataPath").children("span");
    var dataRoad       = document.getElementsByName("e_dataPath[]"); //数据存储路径
    var e_allExistMp   = jQuery(".mount_point");
    var ddfsId         = document.getElementById("ddfsId").value;
    //判断数据路径是否为空
    for (var j = 0; j < dataRoad.length; j++) {
        if (dataRoad[j].value == '') {
            alert("数据存储路径不能为空！ ");
            dataRoad[j].focus();
            return false;
        }
    }
    
    var num = 0;//该num用于判断第一个数据路径是不是能与挂载点路径互为子目录;
    if (document.getElementById("e_mountWay").value == "adfs") {
        num = 1;
    }
    var e_allExistPath = new Array(); //已经存在的数据路径和挂载点路径
    //已存在的除自己本身的
	var e_allExistNot1 = jQuery(".allDataPath").children("span").not(".spanPath" + ddfsId);
    
    for (var i = 0; i < e_allExistMp.length; i++) {
    	e_allExistPath.push(e_allExistMp.eq(i).text());
    }
    
    for (var i = 0; i < e_allExistNot1.length; i++) {
    	e_allExistPath.push(e_allExistNot1.eq(i).text());
    }
    
    //数据路径和已经添加的数据路径比较
    for (var i = 0; i < e_allExistPath.length; i++) {
        for (var j = num; j < dataRoad.length; j++) {
            if (IsDirRepeat(e_allExistPath[i], dataRoad[j].value)) {
                alert("数据路径不合法: " + dataRoad[j].value);
                //dataRoad[j].value = '';
                dataRoad[j].select();
                dataRoad[j].focus();
                return false;
            }
        }
    }
    // 数据路径和新添加的数据路径比较
    for (var i = 0; i < dataRoad.length; i++) {
        for (var j = i + 1; j < dataRoad.length; j++) {
            if (IsDirRepeat(dataRoad[i].value, dataRoad[j].value)) {
                alert("数据路径不合法: " + dataRoad[j].value);
                dataRoad[j].select();
                dataRoad[j].focus();
                return false;
            }
        }
    }
	
    //document.getElementById("editddfsForm").action = "index.php?s=/Ddfs/editMountpoint";
    //document.getElementById("editddfsForm").submit();
    closeCover();
    createCorage();
    set_ajax('editddfsForm','index.php?s=/Ddfs/editMountpoint',pwdHandle,"pN_info");
}
//挂载
function mount(id)
{
	var systemType    = jQuery("#systemType").val();
    var firstDataPath = jQuery("#firstDataPath"+id).val();
    var mountconfig   = jQuery("#ddfsconfig"+id).val();
    var mount_point   = jQuery("#mount_point"+id).text();
    closeCover();
    createCorage();
    ThinkAjax.send("index.php?s=/Ddfs/mount/",'ajax=1&mountconfig='+mountconfig+'&mount_point='+mount_point+'&firstDataPath='+firstDataPath+'&systemType='+systemType,pwdHandle,"pN_info");
}
//卸载
function umount(id)
{
	var systemType    = jQuery("#systemType").val();
    var firstDataPath = jQuery("#firstDataPath"+id).val();
    var umountconfig = jQuery("#ddfsconfig"+id).val();
    var umount_point = jQuery("#mount_point"+id).text();
    closeCover();
    createCorage();
    ThinkAjax.send("index.php?s=/Ddfs/umount/",'ajax=1&umountconfig='+umountconfig+'&umount_point='+umount_point+'&firstDataPath='+firstDataPath+'&systemType='+systemType,pwdHandle,"pN_info");
}
//修复
function fix(id)
{
    var fixconfig = jQuery("#ddfsconfig"+id).val();
    var fix_point = jQuery("#mount_point"+id).text();
    closeCover();
    createCorage();
    ThinkAjax.send("index.php?s=/Ddfs/fix/",'ajax=1&fixconfig='+fixconfig+'&fix_point='+fix_point,pwdHandle,"pN_info");
}
//删除挂载点
function del_Ddfs()
{
    //document.getElementById("delMountForm").action = "index.php?s=/Ddfs/delMountpoint";
    //document.getElementById("delMountForm").submit();
    closeCover();
    createCorage();
    set_ajax('delMountForm','index.php?s=/Ddfs/delMountpoint',pwdHandle,"pN_info");
}

/**
 * HDFS模块***/
//删除数据节点
function dataDel()
{
	//document.getElementById("dataDelfrm").action = "index.php?s=/Hdfs/dataDel";
    //document.getElementById("dataDelfrm").submit();
    closeCover();
    createCorage();
    set_ajax('dataDelfrm','index.php?s=/Hdfs/dataDel',pwdHandle,"pN_info");
}
//删除索引节点
function indexDel()
{
	//document.getElementById("indexDelfrm").action = "index.php?s=/Hdfs/indexDel";
    //document.getElementById("indexDelfrm").submit();
    closeCover();
    createCorage();
    set_ajax('indexDelfrm','index.php?s=/Hdfs/indexDel',pwdHandle,"pN_info");
}

/***NAS
 ***/
// 选择 iscsi-target iqn
function SelIqn()
{
    var iqn = document.getElementById("is_sel").value;
    //ThinkAjax.send("index.php?s=/Share/index/",'ajax=1&iqn='+iqn,NasTimeout,"pN_info");
    closeCover();
    window.location = "index.php?s=/Share/index&iqn="+iqn;
}
function AddTarget()
{
    var iqn = document.getElementById('newIqnPrefix').innerHTML + document.getElementById('newTargetName').value;
    closeCover();
    ThinkAjax.send("index.php?s=/Share/AddTarget",'ajax=1&iqn='+iqn,NasTimeout,"pN_info");
}
function DelTarget()
{
    var iqn = document.getElementById('is_sel').value;
    closeCover();
    ThinkAjax.send("index.php?s=/Share/DelTarget",'ajax=1&iqn='+iqn,NasTimeout,"pN_info");
}
function AddChap()
{
    closeCover();
    set_ajax('newIsUForm',"index.php?s=/Share/AddChap",NasTimeout,"pN_info");
}
function DelChap()
{
    var delChap = jQuery("#delIsU_list").text();
    closeCover();
    set_ajax('delIsUForm',"index.php?s=/Share/DelChap",NasTimeout,"pN_info");
}
function EditChap()
{
    closeCover();
    set_ajax('editIsUForm',"index.php?s=/Share/AddChap",NasTimeout,"pN_info");
}
function AddItLun()
{
    closeCover();
    //document.getElementById("itLunForm").action = "index.php?s=/Share/AddLun";
    //document.getElementById("itLunForm").submit();
    set_ajax('itLunForm',"index.php?s=/Share/AddLun",NasTimeout,"pN_info");
}
function FcLunMap(tar)
{
    closeCover();
    //document.getElementById("fcLunFrom").action = "index.php?s=/Share/FcMap&tar="+tar;
    //document.getElementById("fcLunFrom").submit();
    set_ajax('fcLunFrom',"index.php?s=/Share/FcMap&tar="+tar,NasTimeout,"pN_info");
}
function SelFcTar()
{
    var tar = document.getElementById("fctarget").value;
    //ThinkAjax.send("index.php?s=/Share/index/",'ajax=1&iqn='+iqn,NasTimeout,"pN_info");
    closeCover();
    window.location = "index.php?s=/Share/FCshare&tar="+tar;
}

function SetAuth()
{
    closeCover();
    set_ajax('authForm',"index.php?s=/Usergroup/SetAuth",NasTimeout,"pN_info");
}
function GroupAdd()
{
    closeCover();
    set_ajax('groupAddForm',"index.php?s=/Usergroup/groupadd",NasTimeout,"pN_info");
}
function GroupDel()
{
    closeCover();
    set_ajax('glistForm',"index.php?s=/Usergroup/groupdel",NasTimeout,"pN_info");
}
function UserAdd()
{
    closeCover();
    set_ajax('userAddForm',"index.php?s=/Usergroup/useradd",userSet,"pN_info");
}
function UserDel()
{
    closeCover();
    set_ajax('ulistForm',"index.php?s=/Usergroup/userdel",NasTimeout,"pN_info");
}
