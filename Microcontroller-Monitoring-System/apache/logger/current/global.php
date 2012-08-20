<?php
	$UpdateSensorFunctions = array();
	$NewReadingsFunctions = array();
	$DownloadOptionsFunctions = array();
	$DownloadReadingsFunctions = array();
	$ConfigureOptionsFunctions = array();
	$Sensor = array();
	
	global $Sensor;
	global $UpdateSensorFunctions;
	global $NewReadingsFunctions;
	global $DownloadReadingsFunctions;
	global $DownloadOptionsFunctions;
	global $ConfigureOptionsFunctions;
	
	$Sensor["username"] = "sensoraccess";
	$Sensor["password"] = "access755";
	$Sensor["database"] = "sensorscurrent";
	$Sensor["host"] = "localhost";
	
	require_once 'analogSensor.php';
	require_once 'temperatureSensor.php';
?>