<?php
	require_once 'global.php';
	if(array_key_exists('serial', $_POST) && $_POST['serial'] != ""){
		$serial = $_POST['serial'];
		$model = $serial[0].$serial[1].$serial[2];
		if(array_key_exists($model, $ConfigureOptionsFunctions)){
			$toCall = $ConfigureOptionsFunctions[$model];
			$toCall($_POST);
		}
	} else if(array_key_exists('nickname', $_POST)){
		$mysqliDatabase = new mysqli($Sensor["host"], $Sensor["username"], $Sensor["password"], $Sensor["database"]);
		$nickname = $_POST['nickname'];
		$query = "SELECT * FROM sensors WHERE nickname='$nickname'";
		$results = $mysqliDatabase->query($query);
		if(!$results) exit;
		$row = $results->fetch_array(MYSQLI_ASSOC);
		if(!array_key_exists('serial', $row)) exit;
		$serial = $row['serial'];
		$model = $serial[0].$serial[1].$serial[2];
		$postArray = array('serial' => $serial);
		foreach($_POST as $key => $value){
			if($key == "serial") continue;
			$postArray[$key] = $value;
		}
		if(array_key_exists($model, $ConfigureOptionsFunctions)){
			$toCall = $ConfigureOptionsFunctions[$model];
			$toCall($postArray);
		}
	}
?>