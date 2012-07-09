<?php
	require_once 'global.php';
	if(array_key_exists('serial', $_POST)){
		$serial = $_POST['serial'];
		$model = $serial[0].$serial[1].$serial[2];
		if(array_key_exists($model, $UpdateSensorFunctions)){
			$toCall = $UpdateSensorFunctions[$model];
			$toCall($serial);
		}
	}
?>