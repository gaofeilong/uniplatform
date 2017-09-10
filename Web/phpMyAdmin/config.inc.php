<?php
//if($_SERVER['REMOTE_ADDR'] <> '127.0.0.1')
//{
//	echo '<meta http-equiv="Content-Type" content="text/html; charset=gb2312"><script>alert("���IP�������¼��");window.close();</script>';
//	exit();
//}

//$cfg['blowfish_secret'] = 'ba17c1ec07d65003';  // use here a value of your choice
//$i=0;
//$i++;
//$cfg['Servers'][$i]['auth_type']     = 'cookie';

$i=0;
$i++;
$cfg['Servers'][$i]['host'] = '127.0.0.1';
$cfg['Servers'][$i]['user']          = 'root';
$cfg['Servers'][$i]['password']      = ''; // use here your password
?>

