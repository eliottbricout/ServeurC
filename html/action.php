<?php
$nom=$_POST[nom];
$test=0;
$value="";
foreach ($_COOKIE['cookie'] as $name => $value) {
if ($name==$nom){
setcookie("cookie[$nom]","$value"."v" );
$test=strlen($value);
echo "Bonjour $nom c'est votre $test visite <br/>";
$nom="";
}
 }
if ($test===0){
echo "Bonjour $nom Bienvenue pour votre premiere visite";
setcookie("cookie[$nom]", "Vv");
}
?>
