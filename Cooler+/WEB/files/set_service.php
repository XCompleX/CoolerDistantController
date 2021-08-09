<?php
$mysqli = new mysqli("localhost", "YourLogin", "YourPassword", "NameYourDatabase");
$id = $_GET['ID'];
$value = $_GET['Value'];
$result = $mysqli->query("UPDATE `Coolers` SET Value = ".$value." WHERE ID = ".$id.";");
?>