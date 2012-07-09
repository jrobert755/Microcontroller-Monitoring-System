<?php
	global $UpdateSensorFunctions;
	global $NewReadingsFunctions;
	global $DownloadReadingsFunctions;
	global $Sensor;
	$UpdateSensorFunctions['abc'] = "updateAnalogSensor";
	$NewReadingsFunctions['abc'] = "newAnalogReading";
	$DownloadReadingsFunctions['abc'] = "downloadAnalogReadings";
	
	function updateAnalogSensor($serial){
		global $Sensor;
		$mysqliDatabase = new mysqli($Sensor["host"], $Sensor["username"], $Sensor["password"], $Sensor["database"]);
		$query = "SELECT * FROM analogsensorconfiguration WHERE serial = '$serial'";
		$result = $mysqliDatabase->query($query);
		while($row = $result->fetch_array(MYSQLI_ASSOC)){
			/*foreach($row as $i => $j){
				echo "'$i' => '$j'\n";
			}*/
			$output_string = "";
			if($row['pollcurve']){
				$current_time = time();
				$current_spot = intval($row['currentspot']);
				$resistances = explode(",", $row['pollcurveresistances']);
				if($current_time >= $row['pollcurvestart'] && $current_time <= $row['pollcurveend'] && count($resistances > 0) && $current_spot < count($resistances)){
					$output_string .= "resistance=".$resistances[$current_spot]."\n";
					$output_string .= "readtime=".$row['pollcurvereadtime']."\n";
					$query = "UPDATE analogsensorconfiguration SET currentspot=".($current_spot + 1)." WHERE serial='$serial'";
					$mysqliDatabase->query($query);
					if(($current_spot+1) == count($resistances)){
						$query = "UPDATE analogsensorconfiguration SET pollcurve=0 WHERE serial='$serial'";
						$mysqliDatabase->query($query);
					}
				} else{
					$output_string .= "readtime=".$row['readtime']."\n";
				}
			} else{
				$output_string .= "readtime=".$row['readtime']."\n";
			}
			$output_string .= "pinstoread=".$row['pinstoread']."\n";
			echo $output_string;
		}
		$mysqliDatabase->close();
		
	}
	
	function newAnalogReading($postArray){
		if(!array_key_exists("reading", $postArray)) return 0;
		global $Sensor;
		$serial = $postArray['serial'];
		$mysqliDatabase = new mysqli($Sensor["host"], $Sensor["username"], $Sensor["password"], $Sensor["database"]);
		$query = "SELECT * FROM sensors WHERE serial='$serial'";
		$result = $mysqliDatabase->query($query);
		$row = $result->fetch_array(MYSQLI_ASSOC);
		$nickname = $row['nickname'];
		$datetime = new DateTime(null, new DateTimeZone('America/New_York'));
		$time = $datetime->format('Y-m-d H:i:s');
		$reading = $postArray['reading'];
		$pin = $postArray['pin'];
		$query = "SELECT * FROM analogsensorconfiguration WHERE serial='$serial'";
		$result = $mysqliDatabase->query($query);
		$row = $result->fetch_array(MYSQLI_ASSOC);
		$pollcurve = $row['pollcurve'];
		$query = "INSERT INTO analogsensorreadings VALUES ('$serial', '$nickname', '$time', '$reading', '$pin', '$pollcurve')";
		if(!$mysqliDatabase->query($query)){
			echo "Nope\n";
			echo $query."\n";
		}
		$mysqliDatabase->close();
		//updateAnalogSensor($serial);
	}
	
	function downloadAnalogReadings($postArray){
		global $Sensor;
		$serial = $postArray['serial'];
		$mysqliDatabase = new mysqli($Sensor["host"], $Sensor["username"], $Sensor["password"], $Sensor["database"]);
		$query = "SELECT * FROM analogsensorreadings WHERE serial='$serial'";
		if(array_key_exists('regular', $postArray) && !array_key_exists('pollcurve', $postArray) && $postArray['regular'] == "1"){
			$query .= " AND pollcurvereading='0'";
		}
		if(array_key_exists('pollcurve', $postArray) && !array_key_exists('regular', $postArray)&& $postArray['pollcurve'] == "1"){
			$query .= " AND pollcurvereading='1'";
		}
		if(!array_key_exists('regular', $postArray) && !array_key_exists('pollcurve', $postArray)){
			$query .= " AND pollcurvereading='0'";
		}
		$query .= " ORDER BY time";
		$results = $mysqliDatabase->query($query);
		$row = $results->fetch_array(MYSQLI_ASSOC);
		header('Content-type: text/csv');
		header('Content-Disposition: attachment; filename="'.$row['nickname'].'_readings.csv"');
		//flush();
		//echo "\"".$row['time']."\",".$row['reading'].",".$row['pin'].",".$row['pollcurvereading']."\r\n";
		echo "\"".$row['time']."\",".$row['reading'].",".$row['pin'].($row['pollcurvereading'] == '1' ? ",pollcurve" : "")."\r\n";
		while($row = $results->fetch_array(MYSQLI_ASSOC)){
			//echo "\"".$row['time']."\",".$row['reading'].",".$row['pin']."\r\n";
			echo "\"".$row['time']."\",".$row['reading'].",".$row['pin'].($row['pollcurvereading'] == '1' ? ",pollcurve" : "")."\r\n";
		}
		$mysqliDatabase->close();
	}
?>
