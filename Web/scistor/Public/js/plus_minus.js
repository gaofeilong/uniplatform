
//数值微调框

//检查输入的数是否为整数
function checkInt(editTextV)
{
    return (editTextV == "" || isNaN(editTextV) == true);
}
//检查是否需要disabled
function checkDisabled(e, minus, letter, min)
{
    var jian      = document.getElementById(minus);
    var editTextV = document.getElementById(letter).value;
    
    if (isNaN(e.value) || e.value < min) {
        e.select();
    } 
    if (editTextV <= min) {
        jian.disabled = true;
    } else {
        jian.disabled = false;
    }
}
//自增
function plusSelf(minus, letter, min)
{
    var jian     = document.getElementById(minus);
    var editText = document.getElementById(letter);
    var myno     = editText.value*1;
    
    if (!checkInt(editText.value)) {
        myno++;
        editText.value = myno;
        jian.disabled  = false;
    } else {
        editText.value = min;
    }
}
//自减
function minusSelf(minus, letter, min)
{
    var jian     = document.getElementById(minus);
    var editText = document.getElementById(letter);
    var myno     = editText.value*1;
    
    if (!checkInt(editText.value)) {
        myno--;
        editText.value = myno;
        if (myno <= min) {
            myno = min;
            editText.value = min;
            jian.disabled  = true;
        }
    } else {
        editText.value = min;
    }
}
