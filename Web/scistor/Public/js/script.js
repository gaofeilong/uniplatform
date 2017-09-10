var TINY={};

function T$(i){return document.getElementById(i)}
function T$$(e,p){return p.getElementsByTagName(e)}
//该资源下载自：源码爱好者 http://www.codefans.net
TINY.table=function(){
    function sorter(n){this.n=n; this.pagesize=20; this.paginate=0}
    sorter.prototype.init=function(e,f){
        var t=ge(e), i=0; 
        this.e=e; 
        this.l=t.r.length; 
        t.a=[];
        t.h=T$$('thead',T$(e))[0].rows[0]; 
        t.w=t.h.cells.length;
        for(i;i<t.w;i++){
            var c=t.h.cells[i];
            if(c.className!='nosort'){
                c.className=this.head; 
                c.onclick=new Function(this.n+'.wk(this.cellIndex)');
            }
        }
        for(i=0;i<this.l;i++){
            t.a[i]={};
        }
        if(f!=null){
            var a=new Function(this.n+'.wk('+f+')'); 
            a();
        }
        if(this.paginate){
            this.g=1; 
            this.pages();
        }
    };
    sorter.prototype.wk=function(y){
        var t=ge(this.e), x=t.h.cells[y], i=0;
        for(i;i<this.l;i++){
            t.a[i].o=i;
            var v=t.r[i].cells[y];
            t.r[i].style.display='';
            while(v.hasChildNodes()){
                v=v.firstChild;
            }
            t.a[i].v=v.nodeValue?v.nodeValue:'';
        }
        for(i=0;i<t.w;i++){
            var c=t.h.cells[i];
            if(c.className!='nosort'){
                c.className=this.head;
            }
        }
        if(t.p==y){
            t.a.reverse();
            x.className=t.d?this.asc:this.desc;
            t.d=t.d?0:1;
        }else{
            t.p=y;
            t.a.sort(cp);
            t.d=0;
            x.className=this.asc;
        }
        var n=document.createElement('tbody');
        for(i=0;i<this.l;i++){
            var r=t.r[t.a[i].o].cloneNode(true);
            n.appendChild(r);
            r.className=i%2==0?this.even:this.odd;
            var cells=T$$('td',r);
            for(var z=0;z<t.w;z++){
                cells[z].className=y==z?i%2==0?this.evensel:this.oddsel:'';
            }
        }
        t.replaceChild(n,t.b);
        if(this.paginate){
            this.size(this.pagesize);
        }
    };
    sorter.prototype.page=function(s){
        var t=ge(this.e), i=0, l=s+parseInt(this.pagesize);
        if(l == 0){
            alert("请选择需要显示的表格的行数!");
        }else{
            if(this.currentid&&this.limitid){
                T$(this.currentid).innerHTML=this.g;
            }
            for(i;i<this.l;i++){
                t.r[i].style.display=i>=s&&i<l?'':'none';
            }
        }
    };
    sorter.prototype.move=function(d,m){
        var s=d==1?(m?this.d:this.g+1):(m?1:this.g-1);
        if(s<=this.d&&s>0){
            this.g=s;
            this.page((s-1)*this.pagesize);
        }
    };
    sorter.prototype.size=function(s){
        this.pagesize=s;
        this.g=1;
        this.pages();
        this.page(0);
        if(this.currentid&&this.limitid){
            T$(this.limitid).innerHTML=this.d;
        }
    };
    sorter.prototype.pages=function(){
        this.d=Math.ceil(this.l/this.pagesize);
    };
    function ge(e){
        var t=T$(e);
        t.b=T$$('tbody',t)[0];
        t.r=t.b.rows;
        lightRow(t.r);
        return t;
    };
    function cp(f,c){
        var g,h;
        f=g=f.v.toLowerCase(), c=h=c.v.toLowerCase();
        var i=parseFloat(f.replace(/(\$|\,)/g,'')), n=parseFloat(c.replace(/(\$|\,)/g,''));
        if(!isNaN(i)&&!isNaN(n)){
            g=i,h=n;
        }
        i=Date.parse(f);
        n=Date.parse(c);
        if(!isNaN(i)&&!isNaN(n)){
            g=i;
            h=n;
        }
        return g>h?1:(g<h?-1:0)
    };
    return{
        sorter:sorter
    }
}();

    /**
     *点击和经过表格行高亮显示
     */
function lightRow(lightRows){
    //var lightRows = document.getElementsByTagName('tr');
    for (var i = 0; i < lightRows.length; i++) {
        lightRows[i].onmouseover = function() {
            if(this.className.search(/\btrSelected\b/) == -1){
                this.className += ' hilite';
            }
        }
        lightRows[i].onmouseout = function() {
            this.className = this.className.replace('hilite', '');
        }
        lightRows[i].onclick = function(){
        /**
         *多行高亮显示
        if(this.className.search(/\btrSelected\b/) != -1){
                this.className = this.className.replace(' trSelected', '');
            }else{
                this.className += ' trSelected';
            }
        */
        //单行高亮显示
            for(var j = 0; j < lightRows.length; j++){
                lightRows[j].className = lightRows[j].className.replace(' trSelected', '');
            }
            if(this.className.search(/\btrSelected\b/) != -1){
                this.className = this.className.replace('trSelected', '');
            }else{
                this.className += ' trSelected';
            }
        }
    }
}

    /**
     *拖动表格列
     */
function MouseDownToResize(e, obj){
    obj.mouseDownX=e.clientX;
    obj.pareneTdW=obj.parentElement.offsetWidth;
    obj.pareneTableW=table.offsetWidth;
    obj.setCapture();
}
function MouseMoveToResize(e,obj){
    if(!obj.mouseDownX) return false;
    var newWidth=obj.pareneTdW*1+e.clientX*1-obj.mouseDownX;
    if(newWidth>10)
    {
    obj.parentElement.style.width = newWidth;
    //theObjTable.style.width=obj.pareneTableW*1+event.clientX*1-obj.mouseDownX;
}
}
function MouseUpToResize(obj){
    obj.releaseCapture();
    obj.mouseDownX=0;
}