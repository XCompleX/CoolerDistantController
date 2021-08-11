<?php
$mysqli = new mysqli("localhost", "YourLogin", "YourPassword", "NameYourDatabase");
$id = $_POST['ID'];
$value = $_POST['Value'];
$result = $mysqli->query("UPDATE `Coolers` SET Value = ".$value." WHERE ID = ".$id.";");
?>