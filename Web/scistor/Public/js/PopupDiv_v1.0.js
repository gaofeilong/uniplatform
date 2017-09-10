/**
 * @author maxcoold
 * PopupDiv v1.0 通用弹出层插件
 */
jQuery.fn.PopupDiv = function(mods, options){
    //初始默认参数
    var PopupDivSettings = {
        modal: false,//是否需要遮罩层
        top: null,//弹出的绝对位置，默认居中
        left: null,//弹出的绝对位置，默认居中
        title: "标题信息",//标题信息
        closeTitle: false,//是否不需要标题，默认显示
        myclass: "myWrap",//自定义弹出层样式
        maxHeight: null,//自定义最大高，默认适应弹出内容（ie6不支持）
        maxWidth: null,//自定义最大宽，默认适应弹出内容（ie6不支持）
        minHeight: 100,//自定义最小高，默认适应弹出内容
        minWidth: 500,//自定义最小宽，默认适应弹出内容
        center: true,//内容是否居中
        close_btn: true,//是否启用右上的关闭按钮关闭,默认启用
        autoOpen: true,//自动弹出窗口
        PopupDivId: "myWrap",
        ajaxUrl: null,
        close_fn_before: function(){
        },
        close_fn_later: function(){
        },
        start_fn_before: function(){
        },
        start_fn_later: function(){
        },
        open_fn_before: function(){
        },
        open_fn_later: function(){
        },
        remove_fn_before: function(){
        },
        remove_fn_later: function(){
        }
    };
    if (options == null) {
        options = mods;
    }
    var container = this;
    bool = container.data("bool");
    if (bool == null) {
        bool = true;
    }
    if (options) {
        jQuery.extend(PopupDivSettings, options);
    }
    //接受参数
    return this.each(function(){
        switch (mods) {
            case 'open':
                open();
                break;
            case 'close':
                close();
                break;
            case "remove":
                remove();
                break;
            default:
                ckStart();
        }
    })
    function open(){
        container.data("val").open_fn_before();
        container.show();
        if (container.data("val").modal) {
            addOutDiv();
        }
        jQuery("#" + container.data("val").PopupDivId).fadeIn(function(){
            jQuery("#" + container.data("val").PopupDivId + " .myTi_l").width(jQuery("#" + container.data("val").PopupDivId + " .myTi").width() - jQuery("#" + container.data("val").PopupDivId + " .myTi_r").width());
           container.data("val").open_fn_later();
		   });
        
    }
    function close(){
        container.data("val").close_fn_before();
        reOutDiv();
        jQuery("#" + container.data("val").PopupDivId).hide();
        container.hide();
        container.data("bool", false);
        container.data("val").close_fn_later();
    }
    function remove(){
        container.data("val").remove_fn_before();
        reOutDiv();
        jQuery("#" + container.data("val").PopupDivId).hide();
        reWrap();
        container.hide();
        container.data("val").remove_fn_later();
        container.removeData("bool").removeData("val");
    }
    function ckStart(){
        if (bool) {
            container.data("bool", false);
            container.data("val", PopupDivSettings);
            if (container.data("val").ajaxUrl !== null) {
                jQuery.ajax({
                    type: "GET",
                    url: container.data("val").ajaxUrl,
                    success: function(mydata){
                        container.html(mydata);
                        start();
                    }
                })
            }
            else {
                start();
            }            
        }
        else {
            open();
        }
    }
    
    function start(){
        container.data("val").start_fn_before();
        addWrap();
        if (container.data("val").center) {
            container.css("margin", "0px auto");
        };
        
        jQuery("#" + container.data("val").PopupDivId).css("width", container.width());
        jQuery("#" + container.data("val").PopupDivId).css("maxHeight", container.data("val").maxHeight);
        jQuery("#" + container.data("val").PopupDivId).css("maxWidth", container.data("val").maxWidth);
        jQuery("#" + container.data("val").PopupDivId).css("minHeight", container.data("val").minHeight);
        jQuery("#" + container.data("val").PopupDivId).css("minWidth", container.data("val").minWidth);
        
        if (container.data("val").minWidth > container.width()) {
            jQuery("#" + container.data("val").PopupDivId).width(container.data("val").minWidth);
		}
        if (container.data("val").minHeight > (container.height() + jQuery("#" + container.data("val").PopupDivId + " .myTi").height())) {
        
            jQuery("#" + container.data("val").PopupDivId).height(container.data("val").minHeight + jQuery("#" + container.data("val").PopupDivId + " .myTi").height());
            
        }
        if (container.width() < jQuery("#" + container.data("val").PopupDivId).width()) {
            
            container.data("val").left == null ? jQuery("#" + container.data("val").PopupDivId).css("left", document.documentElement.scrollLeft + document.documentElement.clientWidth / 2 - jQuery("#" + container.data("val").PopupDivId).width() / 2) : jQuery("#" + container.data("val").PopupDivId).css("left", container.data("val").left);
        }
        else {
            container.data("val").left == null ? jQuery("#" + container.data("val").PopupDivId).css("left", document.documentElement.scrollLeft + document.documentElement.clientWidth / 2 - container.width() / 2) : jQuery("#" + container.data("val").PopupDivId).css("left", container.data("val").left);
        }
        if (container.height() < jQuery("#" + container.data("val").PopupDivId).height() - jQuery("#" + container.data("val").PopupDivId + " .myTi").height()) {
        
            container.data("val").top == null ? jQuery("#" + container.data("val").PopupDivId).css("top", document.documentElement.scrollTop - 70 + document.documentElement.clientHeight / 2 - jQuery("#" + container.data("val").PopupDivId).height() / 2) : $("#" + container.data("val").PopupDivId).css("top", container.data("val").top);
        }
        else {
            container.data("val").top == null ? jQuery("#" + container.data("val").PopupDivId).css("top", document.documentElement.scrollTop - 70 + document.documentElement.clientHeight / 2 - container.height() / 2) : jQuery("#" + container.data("val").PopupDivId).css("top", container.data("val").top);
        }
        
        jQuery("#" + container.data("val").PopupDivId).addClass(container.data("val").myclass);
        
        container.data("val").start_fn_later();
        if (container.data("val").autoOpen) {
            open();
        }
    }
    
    function addWrap(){
        var myWrap = document.createElement("div");
        myWrap.style.position = "absolute";
        myWrap.id = container.data("val").PopupDivId;
        myWrap.style.zIndex = 15000;
		// myWrap.style.backgroundColor = "#fff";
		 myWrap.style.border = "1px solid #ccc";
        
        var myTi = document.createElement("div");
        myTi.className = "myTi";
        var myTi_l = document.createElement("div");
        myTi_l.className = "myTi_l";
        var myTi_r = document.createElement("div");
        myTi_r.className = "myTi_r";
        
        container.wrap(myWrap);
        if (!container.data("val").closeTitle) {
            jQuery("#" + container.data("val").PopupDivId).prepend(myTi);
            myTi.appendChild(myTi_l);
            jQuery("#" + container.data("val").PopupDivId + " .myTi_l").text(container.data("val").title);
            if (container.data("val").close_btn) {
                myTi.appendChild(myTi_r);
                jQuery("#" + container.data("val").PopupDivId + " .myTi_r").attr("class", "myTi_r close_out");
                jQuery("#" + container.data("val").PopupDivId + " .myTi_r").click(function(){
                    close();jQuery('#slidercontainer2').remove();
                });
				/****改0314***/
				jQuery(".cancel").click(function(){
                    close();jQuery('#slidercontainer2').remove();
                });
				/*****/
                jQuery("#" + container.data("val").PopupDivId + " .myTi_r").hover(function(){
                    jQuery("#" + container.data("val").PopupDivId + " .myTi_r").attr("class", "myTi_r close_on");
                }, function(){
                    jQuery("#" + container.data("val").PopupDivId + " .myTi_r").attr("class", "myTi_r close_out");
                })
                //document.getElementById("myTi_l").onmousedown = function(e){
            }
            
            /***层拖动取消*/
            
            /*jQuery("#" + container.data("val").PopupDivId + " .myTi_l").get(0).onmousedown = function(e){
                e = arguments[0] || window.event;
                var x = e.layerX || e.offsetX;
                var y = e.layerY || e.offsetY;
                var thisti=document.getElementById(container.data("val").PopupDivId);
                document.onmousemove = function(e){
                    e = arguments[0] || window.event;
                    var nowx = document.documentElement.scrollLeft + (e.clientX - x);
                    var nowy = document.documentElement.scrollTop + (e.clientY - y);
                    if (nowx < 0 && nowy < 0) {
                        document.onmousemove = null;
                        document.onmousedown = null;
                    }
                    thisti.style.filter = "alpha(opacity=50)";
                    thisti.style.opacity = 0.5;
                    thisti.style.left = nowx + "px";
                    thisti.style.top = nowy + "px";
                }; *///原生js
                
                /*document.onmousemove = function(e) {
                    e = arguments[0] || window.event;
                    if (nowx < 0 && nowy < 0) {
                        document.onmousemove = null;
                        document.onmousedown = null;
                    }
                    thisti.style.filter = "alpha(opacity=50)";
                    thisti.style.opacity = 0.5;
                    var nowx          = document.documentElement.scrollLeft + (e.clientX - x);
                    var nowy          = document.documentElement.scrollTop + (e.clientY - y);
                    var bodyHeight    = document.getElementById("corage_bg").offsetHeight;
                    thisti.style.left = nowx + "px";
                    thisti.style.top  = nowy + "px";
                    if (nowx < 5 ) {
                        thisti.style.left = "5px";
                    }
                    if (nowx > screen.availWidth - thisti.offsetWidth - 260) {
                        thisti.style.left = screen.availWidth - thisti.offsetWidth - 260 + "px";
                    }
                    if (nowy < 35) {
                        thisti.style.top = "35px";
                    }
                    if (nowy > bodyHeight - thisti.offsetHeight -10) {
                        thisti.style.top = bodyHeight - thisti.offsetHeight -10 + "px";
                    }
                };//0711 修改
                
                document.onmouseup = function(){
                    document.onmousemove = null;
                    document.onmousedown = null;
                    thisti.style.filter = "alpha(opacity=100)";
                    thisti.style.opacity = 1;					
                };
            };*/
        }
    }
    function reWrap(){
        jQuery("#" + container.data("val").PopupDivId).replaceWith(container);
    }
    //添加遮罩层
    function addOutDiv(){
        var myIframe = document.createElement("iframe");
        var myDiv = document.createElement("div");
        myIframe.id = "myIframe";
        myIframe.style.height = getHeight() - 10 + "px";
        myIframe.style.width = "100%";
        
        myDiv.id = "corage_bg";
        myDiv.className = "corage_bg";
        myDiv.style.position = "absolute";
        myDiv.style.height = getHeight() + "px";
        myDiv.style.width = "100%";
       // document.body.appendChild(myIframe);
        document.body.appendChild(myDiv);//创建右边遮罩层
		
		var myIframe1 = document.createElement("iframe");
		var myDiv1 = document.createElement("div");
        myIframe1.id = "myIframe";
        myIframe1.style.height = getHeight() - 10 + "px";
        myIframe1.style.width = "100%";
        
        myDiv1.id = "corage_bg";
        myDiv1.className = "corage_bg";
        myDiv1.style.height = getHeight() - 10  + "px";
        myDiv1.style.width = "100%";
		//
		
		//顶部创建遮罩层
		//parent.header.document.body.appendChild(myIframe1);
		parent.header.document.body.appendChild(myDiv1);        
		
		var myDiv2 = document.createElement("div");
		myDiv2.id = "corage_bg";
        myDiv2.className = "corage_bg";
        myDiv2.style.height = getHeight() - 10  + "px";
        myDiv2.style.width = "110.5%";
		
		parent.left.document.body.appendChild(myDiv2);//创建左边遮罩层
        window.onresize = re_show;
		
    }
    
    //计算遮罩层的高
    function getHeight(){
        var winWidth
        if (document.documentElement.scrollHeight > document.documentElement.clientHeight) {
            winHeight = document.documentElement.scrollHeight;
        }
        else {
            winHeight = document.documentElement.clientHeight;
        }
        if (navigator.appName !== "Microsoft Internet Explorer") {
            winHeight = winHeight - 4;
        }
        else {
            winHeight = winHeight;
        }
        return winHeight;
    }
    
    //计算遮罩层的宽
    function getWidth(){
        var winWidth;
        if (document.documentElement.scrollWidth > document.documentElement.clientWidth) {
            winWidth = document.documentElement.scrollWidth;
        }
        else {
            winWidth = document.documentElement.clientWidth;
        }
        if (navigator.appName !== "Microsoft Internet Explorer") {
            winWidth = winWidth - 4;
        }
        else {
            winWidth = winWidth;
        }
        return winWidth;
    }
    
    //窗口改变时变更遮罩层的大小
    function re_show(){
    //    $("#myIframe").height(getHeight());
    //    $("#myIframe").width(getWidth());
        jQuery("#corage_bg").height(getHeight());
    //    $("#corage_bg").width(getWidth());
		
	//	parent.header.$("#myIframe").height(getHeight());
	//	parent.header.$("#myIframe").width(getWidth());
		parent.header.jQuery("#corage_bg").height(getHeight());
	//	parent.header.$("#corage_bg").width(getWidth());
		
		parent.left.jQuery("#corage_bg").height(getHeight());
	//	parent.left.$("#corage_bg").width(getWidth());
    }
    
    //去掉遮罩层的方法
    function reOutDiv(){
       // $("#myIframe").remove();
        jQuery(".corage_bg").remove();
		
		//parent.header.$("#myIframe").remove();
		parent.header.jQuery(".corage_bg").remove();
		
		parent.left.jQuery(".corage_bg").remove();
    }
    
}
