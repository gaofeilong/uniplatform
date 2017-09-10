/***
    日期三联动****/
	var year = document.getElementById("year");
    var month = document.getElementById("month");
    var day = document.getElementById("day");
function YYYYMMDDstart(){
    MonHead = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31];
    var sYear = document.getElementById("sYear");
    var sMonth = document.getElementById("sMonth");
    var sDay = document.getElementById("sDay");
    
    //先给年 下拉框赋内容
    var y = sYear.value;
    for (var i = (y*1-30); i < (y*1+30); i++){ //以今年为准，前30年，后30年
        document.setSysTime.year.options.add(new Option(i, i));
    }
    //赋月份的下拉框
    for (var i = 1; i < 13; i++){
        if(i<10){
            document.setSysTime.month.options.add(new Option("0"+i, "0"+i));
        }else{
            document.setSysTime.month.options.add(new Option(i, i));
        }
    }
    document.setSysTime.year.value = y*1;
    document.setSysTime.month.value = sMonth.value;
    var n = MonHead[sMonth.value*1 - 1];
    if (sMonth.value*1 == 0  && IsPinYear(y*1)){ 
        n++;
    }
    writeDay(n); //赋日期下拉框
    document.setSysTime.day.value = sDay.value;
}
if(document.attachEvent){
    window.attachEvent("onload", YYYYMMDDstart);
}
else{
    window.addEventListener('load', YYYYMMDDstart, false);
}
function YYYYDD(str){ //年发生变化时日期发生变化(主要是判断闰平年)
    var MMvalue = document.setSysTime.month.options[document.setSysTime.month.selectedIndex].value;
    if (MMvalue == ""){ var e = document.setSysTime.day; optionsClear(e); return;}
    var n = MonHead[MMvalue - 1];
    if (MMvalue ==2 && IsPinYear(str)){
        n++;
    }
    writeDay(n)
}
function MMDD(str){  //月发生变化时日期联动
    var YYYYvalue = document.setSysTime.year.options[document.setSysTime.year.selectedIndex].value;
    if (YYYYvalue == ""){ var e = document.setSysTime.day; optionsClear(e); return;}
    var n = MonHead[str - 1];
    if (str ==2 && IsPinYear(YYYYvalue)){
        n++;
    }
    writeDay(n)
}
function writeDay(n){  //据条件写日期的下拉框
    var e = document.setSysTime.day; optionsClear(e);
    for (var i=2; i<(n+1); i++){
        if(i<10){
            e.options.add(new Option("0"+i, "0"+i));
        }else{
            e.options.add(new Option(i, i));
        }
    }
}
function IsPinYear(year){//判断是否闰平年
    return(0 == year%4 && (year%100 !=0 || year%400 == 0));
}
function optionsClear(e){
    e.options.length = 1;
}