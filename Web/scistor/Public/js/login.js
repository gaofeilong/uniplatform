function set_ajax(formId,URL,pwdHandle,resultId)
{
    ThinkAjax.sendForm(formId,URL,pwdHandle,resultId);
}
function pwdHandle(data,status){
    if(status == 0){
        document.getElementById(data).focus();
        document.getElementById(data).value = "";
        document.getElementById("password").value = "";
        document.getElementById("verifycode").value = "";
        fleshVerify();
    }
    if(status == 3){
        document.getElementById(data).focus();
        document.getElementById(data).value = "";
        document.getElementById("verifycode").value = "";
        fleshVerify();
    }
    if(status == 2){
        document.getElementById("password").focus();
        document.getElementById(data).value = "";
        document.getElementById("password").value = "";
        fleshVerify();
    }
    if(status == 1){
        window.location = 'index.php/Index/index';
    }
}
function checkForm(){
    /*var user = document.getElementById("user");
    var pass = document.getElementById("password");
    var verifycode = document.getElementById("verifycode");
    var error = document.getElementById("errorMsg");
    var pwdPatt = /^(\w){6,20}$/;
    var errorMsg = new Array("请输入用户名！","请输入密码！");
    if(user.value == ""){
        showErrorMsg(error,"用户名输入不合法！", user);
        fleshVerify();
        return false;
    } else if (!pwdPatt.exec(pass.value)){
        showErrorMsg(error, "密码输入不合法！", pass);
        fleshVerify();
        return false;
    }*/
    set_ajax('loginfrm','index.php?s=/Public/check',pwdHandle,"errorMsg");
}
function showErrorMsg(error, errorMsg, textFocus){
    error.innerHTML = errorMsg;
    error.style.visibility = "visible";
    error.style.color = "red";
    textFocus.value="";
    textFocus.focus();
}
function Userfocus()
{
    document.getElementById("user").focus();
}
function fleshVerify()
{
    document.getElementById("imgId").src="index.php?s=/Public/verify/id/"+new Date();
}
function keydown(e)
{
    var e = e || event;
    if (e.keyCode==13)
    {
    	checkForm();
    }
}