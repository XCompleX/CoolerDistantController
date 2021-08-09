<?php
$mysqli = new mysqli("localhost", "YourLogin", "YourPassword", "NameYourDatabase");
$result = $mysqli->query("SELECT * FROM `Coolers`");
$obj;
$i = 0;
foreach ($result as $row){
 	$obj[$i] = $row;
 	$i++;
}
echo json_encode($obj);
?>