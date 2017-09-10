   
//***分布式系统管理页面****//
function hdfsType()
{
	var newType = document.getElementById("newType");
	var newWay = document.getElementById("newWay");
    if(newType.selectedIndex == 0){
    	//document.getElementById("indexName").style.display = "";
    	document.getElementById("hdfsWay").style.display = "none";
    	document.getElementById("indexsinIp").style.display = "";
    	document.getElementById("startIp").style.display = "none";
        document.getElementById("endIp").style.display = "none";
    }
    if(newType.selectedIndex == 1){
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

/***分布式系统首页
 * ，删除功能***/
function indexDelbtn()
{
	var dataCount  = jQuery("#dataCount").val() * 1;
	var is_sel     = jQuery("#is_sel").val();
	var indexState = jQuery("#indexState").val();
	if (dataCount == 0) {
		if (indexState == 'stopped') {
			window.location='index.php?s=/Hdfs/delList/id/1/is_sel/'+is_sel;
		} else {
			document.getElementById("delState").disabled = true;
		}
	} else {
		var indexid = jQuery("#hidIndexip").val();
		var a=jQuery("input[name='choice']");
		var num = 0;
	    var datalist = "";
	    for (var i=0; i<a.length; i++){
	        if(a[i].checked && a[i].disabled == false){
	        	datalist += jQuery(".dataIplist").eq(i).html() + ";";
	        	num++;
	        }
	    }
	    if(num == 0){
	        alert("未选中任何节点!");
	        return false;
	    }else{
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

/***添加节点页面
 ***/
//根据节点数判断是否能添加数据节点
function nodeHave()
{
    var countnode  = parseInt(jQuery("#countIndex").val());
    var typeSelect = document.getElementById("newType");
    
    if (countnode < 1) {
        typeSelect.remove(typeSelect.selectedIndex);
        jQuery("#hdfsWay,#startIp,#endIp").remove();
    }
}
//新建节点提交
function checkNodeIp()
{
    var nodeIpList = jQuery("#NodeIpList").children("span");
    var nodeType   = jQuery("#newType").val();//添加类型
    var nodeWay    = jQuery("#newWay").val();//添加方式
    var ipaddr     = jQuery("#ipaddr");//索引IP
    var startIp    = jQuery("#startIpVal");//起始IP
    var endIp      = jQuery("#endIpVal");//结束IP
    var password   = jQuery("#rootPwd").val();//密码
    var pwd_patt   = /^(\w){6,20}$/;//密码匹配
    
    if (nodeType == "index" || (nodeType == "data" && nodeWay == "single")) {
        if (!limitIp(ipaddr.val(),"ipaddr","输入索引IP不合法！")) {  //判断IP输入是否合法
            return false;
        } else {
            for (var i = 0; i < nodeIpList.length; i++) {
                if (ipaddr.val() == nodeIpList.eq(i).text()) {  //判断IP输入是否重复
                    showNoticeMsg("ipaddr","该IP已经存在!");
                    return false;
                }
            }
        }
    } else {
        if (!limitIp(startIp.val(),"startIpVal","起始IP不合法！") || !limitIp(endIp.val(),"endIpVal","结束IP不合法！")) {
            return false;
        } else {
            if (!limitIpRange()) {
                return false;
            }
            if (delIpDot(startIp.val())*1 >= delIpDot(endIp.val())*1) { //起始IP不得大于或等于结束IP
                showNoticeMsg("startIpVal","起始IP不能大于或等于结束IP！");
                return false;
            } else {
                for (var i = startIp.val().split(".")[3]; i <= endIp.val().split(".")[3]; i++) {
                    for (var j = 0; j < nodeIpList.length; j++) {
                        if (concatIp(startIp.val(),i) == nodeIpList.eq(j).text()) {
                            showNoticeMsg("startIpVal","该批IP地址中与已有IP地址重复！");
                            return false;
                        }
                    }
                }
            }
        }
    }
    /*if (!pwd_patt.exec(password)) {
        showNoticeMsg("rootPwd","密码输入不合法");
        return false;
    }*/
}

//处理IP，去除“.”
function delIpDot(ipVal)
{
    var newIp  = ipVal.split(".");
    var lastIp = "";
    
    for (var i = 0;i < newIp.length; i++) {
        lastIp += newIp[i];
    }
    return lastIp;
}
//限制起始IP和结束IP的范围
function limitIpRange()
{
    var startIp    = document.getElementById("startIpVal").value.split(".");
    var endIp      = document.getElementById("endIpVal").value.split(".");
    var startIpN   = "";
    var endIpN     = "";
    var lastSField = "";
    var lastEField = "";
    
    for (var i = 0;i < startIp.length - 1; i++) {
        startIpN += startIp[i];
        endIpN   += endIp[i];
        lastSField = startIp[startIp.length -1];
        lastEField = endIp[endIp.length -1];
    }
    if (startIpN != endIpN) {
        showNoticeMsg("startIpVal","起始IP与结束IP前三个字段必须保持一致！");
        return false;
    } else {
        return true;
    }
}
//得到ip前面三个的字段 和最后一个字段，并将之拼接起来
function concatIp(ipVal, lastField)
{
    var newIp = ipVal.split(".");
    var ipField3 = "";
    
    for (var i = 0;i < newIp.length - 1; i++) {
        ipField3 += newIp[i] + ".";
    }
    lastIp = ipField3 + lastField;
    return lastIp;
}

/***配置管理首页
 ***/
function stateDel()
{
    var countnode = parseInt(jQuery("#countIndex").val());
    var datacount = parseInt(jQuery("#dataCount").val());
    var ign       = jQuery("#clusterign").val();
    var state     = jQuery("#indexState").val();
    if (ign == 1) {
    	document.getElementById("startCluster").disabled = true;
    	document.getElementById("stopCluster").disabled = true;
    } else {
    	document.getElementById("startCluster").disabled = false;
        document.getElementById("stopCluster").disabled = false;
    }

    if (countnode < 1) {
    	document.getElementById("editCluster").disabled = true;
    	document.getElementById("startCluster").disabled = true;
        document.getElementById("stopCluster").disabled = true;
    } else {
    	if (datacount < 1) {
            if (state == 'stopped') {
                document.getElementById("startCluster").disabled = false;
                document.getElementById("stopCluster").disabled = true;
            } else if (state == 'running') {
                document.getElementById("startCluster").disabled = true;
                document.getElementById("stopCluster").disabled = false;
            } else {
            	document.getElementById("startCluster").disabled = true;
                document.getElementById("stopCluster").disabled = true;
            }
        } else {
            var status = jQuery(".status_get");
            var a = jQuery("input[name='choice']");
            for (var i = 0; i < status.length; i++){
                if(status.eq(i).html() == "ready"){
                    a.eq(i).attr("disabled",false);    
                } else {
                    if (status.eq(i).html() == "stopped" && state == 'running') {
                        a.eq(i).attr("disabled",false);
                    } else {
                        a.eq(i).attr("disabled",true);
                    }
                }
            }
        }
    }
    
}
function showhideData()
{
    var indexCbox = document.getElementById("indexCbox");
    if (!indexCbox.checked || indexCbox.disabled) {
        jQuery("#indexRootSet").css({"display":"none"}).find("input").attr("disabled",true);
        jQuery("#isDedupPic").attr("disabled",true);
    } else {
        jQuery("#indexRootSet").css({"display":"block"}).find("input").attr("disabled",false);
        jQuery("#isDedupPic").attr("disabled",false);
    }
}
function hdfsSet()
{
    var indexIp = jQuery("#is_sel").val();
    jQuery("#indexNodeIp").val(indexIp);
    var isdisabled = document.getElementById("dataCount");
    var stateIndex = document.getElementById("indexState").value;
    //因弹框是隐藏不是消失，故每次显示归档路径时先删除之前点击按钮多显示的归档路径
    while(dataindexTab.hasChildNodes()){
        dataindexTab.removeChild(dataindexTab.lastChild);
    }
    //当已经配置过路径后，则在弹框中显示路径
    var idxrootpath = jQuery("#idxrootpath").val();
    var indexpath   = idxrootpath.split(";");
    for(var i = 0;i < indexpath.length;i++)
    {
       //把获取到的已归档路径，显示在编辑框里
       var new_row=dataindexTab.insertRow(dataindexTab.rows.length);
       var new_col01=new_row.insertCell(0); 
       var new_col02=new_row.insertCell(1);
       new_col01.innerHTML='<input type="text" name="indexRootPath[]" id="indexRootPath" class="s_width250new" value="'+indexpath[i]+'" />';
       new_col02.innerHTML='<img src="__PUBLIC__/img/clear.gif" class="hdfs_delete" align="" title="删除索引存储路径" onclick="hdfsindexdel_row()"/><br>';
    }
    
    if (stateIndex != "ready") {
        jQuery("input[name='indexRootPath[]']").attr("disabled",true);
        jQuery("input[name='indexRootPath[]']").parent().siblings().find(".hdfs_delete").css("visibility","hidden");
    } else {
    	jQuery("input[name='indexRootPath[]']").attr("disabled",false);
        jQuery("input[name='indexRootPath[]']").parent().siblings().find(".hdfs_delete").css("visibility","");
    }
    if (isdisabled.value == 0) {
        //document.getElementById("indexCbox").disabled = true;
    } else {
        var status = jQuery(".hdfs_status");
        var num = 0;
        for (var i = 0; i < status.length; i++) {
            if (status.eq(i).children(".status_con").text() == "ready") {
                num++;
            }
        }
        if (num !=0 && num == status.length) {
            //document.getElementById("indexCbox").disabled = false;
        } else {
            //document.getElementById("indexCbox").disabled = true;
        }
    }
    
    showhideData();
	publicP("configcolony","configcolonyDiv","集群配置");
}

//单个索引配置
function indexPerSet(e)
{
	var indexIp = jQuery("#is_sel").val();
    jQuery("#indexaloneIp").val(indexIp);
    //因弹框是隐藏不是消失，故每次显示归档路径时先删除之前点击按钮多显示的归档路径
    while(perindexTab.hasChildNodes()){
    	perindexTab.removeChild(perindexTab.lastChild);
    }
    //当已经配置过路径后，则在弹框中显示路径
    var idxrootpath = jQuery("#idxrootpath").val();
    var indexpath   = idxrootpath.split(";");
    for(var i = 0;i < indexpath.length;i++)
    {
       //把获取到的已归档路径，显示在编辑框里
       var new_row = perindexTab.insertRow(perindexTab.rows.length);
       var new_col01 = new_row.insertCell(0); 
       var new_col02 = new_row.insertCell(1);
       new_col01.innerHTML='<input type="text" name="indexNodePath[]" id="indexNodePath" class="s_width250new" value="'+indexpath[i]+'" />';
       new_col02.innerHTML='<img src="__PUBLIC__/img/clear.gif" class="hdfs_delete" align="" title="删除索引存储路径" onclick="hdfsindexPerdel_row(event)"/><br>';
    }
    
    var status = jQuery(e).parent("td").siblings().find(".status_con");
    publicP("indexPerSet","indexPerSetDiv","索引配置");
    if (status.text() != "ready") {
        jQuery("#isDedupPicPer,input[name='indexNodePath[]']").attr("disabled",true);
        jQuery("#indexPerSet").find(".hdfs_delete").css("visibility","hidden");
    } else {
        jQuery("#isDedupPicPer,input[name='indexNodePath[]']").attr("disabled",false);
        jQuery("#indexPerSet").find(".hdfs_delete").css("visibility","");
    }
}

//单个索引添加索引根目录
function hdfsindexPer_row(e)
{
    var dataPath = document.getElementsByName("indexNodePath[]");
    for(var i = 0;i < dataPath.length; i++){
        if(dataPath[i].value == ""){
            alert("索引存储根目录不能为空！");
            dataPath[i].focus();
            return false;
        }
    }
    var len       = perindexTab.rows.length;
    var new_row   = perindexTab.insertRow(len);
    var new_col01 = new_row.insertCell(0);
    var new_col02 = new_row.insertCell(1);
    new_col01.innerHTML='<input type="text" name="indexNodePath[]" id="indexNodePath" class="s_width250new" value="" />';
    new_col02.innerHTML='<img src="__PUBLIC__/img/clear.gif" class="hdfs_delete" align="" title="删除索引存储路径" onclick="hdfsindexPerdel_row(event)"/>';

    var j = dataPath.length - 1; //得到新增的input，以便获得焦点
    dataPath[j].focus();
}

//索引配置文件，删除索引存储路径
function hdfsindexPerdel_row()
{
    publicdel_row("indexNodePath[]",perindexTab);
}

function dataSet(e,id)
{
    var dataIP = jQuery(e).parent().siblings(".dataIplist").text();
    jQuery("#datanodesIP").val(dataIP);
    var status = jQuery(e).parent().siblings(".hdfs_status");

    //因弹框是隐藏不是消失，故每次显示归档路径时先删除之前点击按钮多显示的归档路径
    while(condata_tab.hasChildNodes()){
        condata_tab.removeChild(condata_tab.lastChild);
    }
    //页面返回数据在弹框中
    var idxpath    = jQuery("#idxpath"+id).text();
    var mtpath     = jQuery("#mtpath"+id).text();
    var dbpathlist = jQuery("#datapath"+id).text();
    jQuery("#indexPathNode").val(idxpath);
    jQuery("#datametaPath").val(mtpath);
    dbpath = dbpathlist.split(";");

    for(var i = 0;i < dbpath.length;i++)
    {
       //把获取到的已归档路径，显示在编辑框里
       var new_row=condata_tab.insertRow(condata_tab.rows.length);
       var new_col01=new_row.insertCell(0); 
       var new_col02=new_row.insertCell(1);
       new_col01.innerHTML='<input type="text" name="dataPathNode[]" id="dataPathNode" class="s_width250new" value="'+dbpath[i]+'" />';
       new_col02.innerHTML='<img src="__PUBLIC__/img/clear.gif" class="hdfs_delete" align="absmiddle" title="删除数据存储路径" onclick="condatadel_row()"/><br>';
    }
    
    if (status.children(".status_con").text() == "ready") {
        jQuery("#datametaPath,#indexPathNode").attr("disabled",false).removeClass("iptreadonly");
        jQuery("#configdata").find("input[name='dataPathNode[]']").attr("disabled",false).removeClass("iptreadonly");
        jQuery("#configdata").find(".hdfs_delete").css("visibility","");
        jQuery("#isDedupPicNode").attr("disabled",false);
    } else {
        jQuery("#datametaPath,#indexPathNode").attr("disabled",true).addClass("iptreadonly");
        jQuery("#datametaPath,#indexPathNode").focus(function(){
            jQuery(this).css("background","#F5F7FC");
        });
        jQuery("#configdata").find("input[name='dataPathNode[]']").attr("disabled",true).addClass("iptreadonly");
        jQuery("#configdata").find(".hdfs_delete").css("visibility","hidden");
        jQuery("#isDedupPicNode").attr("disabled",true);
    }
    
	publicP("configdata","configData","节点配置");
}
function configNode()
{
    var identify = jQuery("#identify").val();
    if (identify == "index") {
        document.getElementById("indexConfig").style.display = "";
        document.getElementById("dataConfig").style.display = "none";
        document.getElementById("hdfs_superDiv").style.display = "none";
    } else {
        document.getElementById("indexConfig").style.display = "none";
        document.getElementById("dataConfig").style.display = "";
    }
}

//单独配置添加路径
function dataAdd_row()
{
	var dataPath = document.getElementsByName("dataPathNode[]");
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
    var len       = condata_tab.rows.length;
    var new_row   = condata_tab.insertRow(len);
    var new_col01 = new_row.insertCell(0);
    var new_col02 = new_row.insertCell(1);
    new_col01.innerHTML='<input type="text" name="dataPathNode[]" id="dataPathNode" class="s_width250new"/>';
    new_col02.innerHTML='<img src="/scistor/Public/img/clear.gif" align="absmiddle" class="img_delete1" onclick = "condatadel_row()" title="删除索引存储路径"/>';

    var j = dataPath.length - 1; //得到新增的input，以便获得焦点
    dataPath[j].focus();
}
//单独配置删除
function condatadel_row()
{
	publicdel_row("dataPathNode[]",condata_tab);
}
//索引配置文件，添加索引存储路径
function hdfsindex_row()
{
    var dataPath = document.getElementsByName("indexRootPath[]");
    for(var i = 0;i < dataPath.length; i++){
        if(dataPath[i].value == ""){
            alert("索引存储根目录不能为空！");
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
    new_col01.innerHTML='<input type="text" name="indexRootPath[]" id="indexRootPath" class="s_width250new"/>';
    new_col02.innerHTML='<img src="/scistor/Public/img/clear.gif" align="absmiddle" class="img_delete1" onclick = "hdfsindexdel_row()" title="删除索引存储路径"/>';

    var j = dataPath.length - 1; //得到新增的input，以便获得焦点
    dataPath[j].focus();
}

function hdfsindexdel_row()
{
	publicdel_row("indexRootPath[]",dataindexTab);
}



//数据配置文件，添加数据存储路径
function hdfsData_row()
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
    var len       = dataconTab.rows.length;
    var new_row   = dataconTab.insertRow(len);
    var new_col01 = new_row.insertCell(0);
    var new_col02 = new_row.insertCell(1);
    new_col01.innerHTML='<input type="text" name="dataPath[]" id="dataPath" class="s_width250new"/>';
    new_col02.innerHTML='<img src="/scistor/Public/img/clear.gif" align="absmiddle" class="img_delete1" onclick = "hdfsDatadel_row()" title="删除数据存储路径"/>';

    var j = dataPath.length - 1; //得到新增的input，以便获得焦点
    dataPath[j].focus();
}

function hdfsDatadel_row()
{
	publicdel_row("dataPath[]",dataconTab);
}

/*function publicAdd_row(name,text,pubTab,pathName,pathId,className,del_row)
{
    var addPath = document.getElementsByName(name);
    for(var i = 0;i < addPath.length; i++){
        if(addPath[i].value == ""){
            alert(text);
            addPath[i].focus();
            return false;
        }
    }
    var e = event.srcElement ? event.srcElement.parentNode : event.target.parentNode;//js三目运算
    //表格图片片段
    var row = e.parentNode.sectionRowIndex;
     // alert(row);
      //return;
    //图片判断结束
    var len       = pubTab.rows.length;
    var new_row   = pubTab.insertRow(len);
    var new_col01 = new_row.insertCell(0);
    var new_col02 = new_row.insertCell(1);
    new_col01.innerHTML='<input type="text" name="'+pathName+'" id="'+pathId+'" class="'+className+'"/>';
    new_col02.innerHTML='<img src="/scistor/Public/img/clear.gif" align="absmiddle" class="img_delete1" onclick = "'+del_row+'" title="删除数据存储路径"/>';

    var j = addPath.length - 1; //得到新增的input，以便获得焦点
    addPath[j].focus();
}*/
function publicdel_row(name,delconTab)
{
    var delPath=document.getElementsByName(name);
    var e = event.srcElement ? event.srcElement.parentNode : event.target.parentNode;//js三目运算
    if(delPath.length > 1){
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
                delconTab.deleteRow(row - 0);    
            }
        }
    } else {
        alert("不能全部删除");
    }
}

/***配置管理首页提交js
 ***/
function startNodes()
{
    var dataCount = jQuery("#dataCount").val();
    var delnodes = jQuery("#is_sel").val();
	closeCover();
    createCorage();
    //window.location = "index.php?s=/Hdfs/startIndex/nodes/"+delnodes;
    ThinkAjax.send("index.php?s=/Hdfs/startIndex/",'ajax=1&nodes='+delnodes,pwdHandle,"pN_info");
}
function startNodealone(id)
{
    var delnodes = jQuery("#nodeIp"+id).text();
    closeCover();
    createCorage();
    //window.location = "index.php?s=/Hdfs/startData/nodes/"+delnodes;
    ThinkAjax.send("index.php?s=/Hdfs/startData/",'ajax=1&nodes='+delnodes,pwdHandle,"pN_info");
}
function stopNodes()
{
	var delnodes = jQuery("#is_sel").val();
    closeCover();
    createCorage();
    //window.location = "index.php?s=/Hdfs/stopIndex/nodes/"+delnodes;
    ThinkAjax.send("index.php?s=/Hdfs/stopIndex/",'ajax=1&nodes='+delnodes,pwdHandle,"pN_info");
}
function stopNodealone(id)
{
    var delnodes = jQuery("#nodeIp"+id).text();
    closeCover();
    createCorage();
    //window.location = "index.php?s=/Hdfs/stopData/nodes/"+delnodes;
    ThinkAjax.send("index.php?s=/Hdfs/stopData/",'ajax=1&nodes='+delnodes,pwdHandle,"pN_info");
}
//删除剩余的数据Id
function delNodealone(id)
{
    var is_sel = jQuery("#is_sel").val();
	var nodeId = jQuery("#nodeId"+id).text();
	//alert(is_sel);alert(nodeId);
    closeCover();
    createCorage();
    //window.location = "index.php?s=/Hdfs/delId/nodeId/"+nodeId+"/nodeIp/"+nodeIp;
    ThinkAjax.send("index.php?s=/Hdfs/delId/",'ajax=1&nodeId='+nodeId+'&nodeIp='+is_sel,pwdHandle,"pN_info");
}
function repairData(id)
{
	var repairNode = jQuery("#nodeIp"+id).text();
    closeCover();
    createCorage();
    //window.location = "index.php?s=/Hdfs/repairData/nodes/"+repairNode;
    ThinkAjax.send("index.php?s=/Hdfs/repairData/",'ajax=1&nodes='+repairNode,pwdHandle,"pN_info");
}
function repairIndex()
{
	var repairNode = jQuery("#is_sel").val();
    closeCover();
    createCorage();
    //window.location = "index.php?s=/Hdfs/repairIndex/nodes/"+repairNode;
    ThinkAjax.send("index.php?s=/Hdfs/repairIndex/",'ajax=1&nodes='+repairNode,pwdHandle,"pN_info");
}
function dataConfig()
{
    
	//document.getElementById("dataconForm").action = "index.php?s=/Hdfs/configData";
    //document.getElementById("dataconForm").submit();
    closeCover();
    createCorage();
    set_ajax('dataconForm','index.php?s=/Hdfs/configData',pwdHandle,"pN_info");
}
function indexConfig()
{
    
    //document.getElementById("indexconForm").action = "index.php?s=/Hdfs/configIndex";
    //document.getElementById("indexconForm").submit();
    closeCover();
    createCorage();
    set_ajax('indexconForm','index.php?s=/Hdfs/configIndex',pwdHandle,"pN_info");
}

function picidxconfig()
{
	//document.getElementById("indexPerForm").action = "index.php?s=/Hdfs/indexPerconfig";
    //document.getElementById("indexPerForm").submit();
    closeCover();
    createCorage();
    set_ajax('indexPerForm','index.php?s=/Hdfs/indexPerconfig',pwdHandle,"pN_info");
}
function startCluster() 
{
	var indexIp = jQuery("#is_sel").val(); 
	var dataIp  = jQuery(".dataIplist");
	var dataIplist = new Array();
	for (var i = 0; i < dataIp.length; i++) {
		dataIplist.push(dataIp.eq(i).text());
    }
	closeCover();
    createCorage();
    //window.location = "index.php?s=/Hdfs/startCluster/indexIp/"+indexIp+"/nodes/"+dataIplist;
    ThinkAjax.send("index.php?s=/Hdfs/startCluster/",'ajax=1&indexIp='+indexIp+'&nodes='+dataIplist,pwdHandle,"pN_info");
}
function stopCluster()
{
	var indexIp = jQuery("#is_sel").val(); 
    var dataIp  = jQuery(".dataIplist");
    var dataIplist = new Array();
    for (var i = 0; i < dataIp.length; i++) {
        dataIplist.push(dataIp.eq(i).text());
    }
    closeCover();
    createCorage();
    //window.location = "index.php?s=/Hdfs/stopCluster/indexIp/"+indexIp+"/nodes/"+dataIplist;
    ThinkAjax.send("index.php?s=/Hdfs/stopCluster/",'ajax=1&indexIp='+indexIp+'&nodes='+dataIplist,pwdHandle,"pN_info"); 
}

