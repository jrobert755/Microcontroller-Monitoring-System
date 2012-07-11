<?php
	global $UpdateSensorFunctions;
	global $NewReadingsFunctions;
	global $DownloadReadingsFunctions;
	global $Sensor;
	$UpdateSensorFunctions['bcd'] = "updateTemperatureSensor";
	$NewReadingsFunctions['bcd'] = "newTemperatureReading";
	$DownloadReadingsFunctions['bcd'] = "downloadTemperatureReadings";
	$DownloadOptionsFunctions['bcd'] = "temperatureDownloadOptions";
	
	function updateTemperatureSensor($serial){
		global $Sensor;
		$mysqliDatabase = new mysqli($Sensor["host"], $Sensor["username"], $Sensor["password"], $Sensor["database"]);
		$query = "SELECT * FROM analogsensorconfiguration WHERE serial = '$serial'";
		$result = $mysqliDatabase->query($query);
		while($row = $result->fetch_array(MYSQLI_ASSOC)){
			$readtime = $row['readtime'];
			$degrees = $row['degrees'];
			$output_string = "readtime='$readtime'\ndegrees='$degrees'\n";
			echo $output_string;
		}
		$mysqliDatabase->close();
	}
	
	function newTemperatureReading($postArray){
		global $Sensor;
		$reading = $postArray['reading'];
		$degrees = $postArray['degrees'];
		$serial = $postArray['serial'];
		$mysqliDatabase = new mysqli($Sensor["host"], $Sensor["username"], $Sensor["password"], $Sensor["database"]);
		$query = "SELECT * FROM sensors WHERE serial='$serial'";
		$result = $mysqliDatabase->query($query);
		$row = $result->fetch_array(MYSQLI_ASSOC);
		$nickname = $row['nickname'];
		$datetime = new DateTime(null, new DateTimeZone('America/New_York'));
		$time = $datetime->format('Y-m-d H:i:s');
		$query = "INSERT INTO temperaturesensorreadings VALUES ('$serial', '$nickname', '$time', '$reading', '$degrees')";
		$result = $mysqliDatabase->query($query);
		$mysqliDatabase->close();
		updateTemperatureSensor($serial);
	}
?>