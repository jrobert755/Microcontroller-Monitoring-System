<?php
	global $UpdateSensorFunctions;
	global $NewReadingsFunctions;
	global $DownloadReadingsFunctions;
	global $DownloadOptionsFunctions;
	global $Sensor;
	$UpdateSensorFunctions['abc'] = "updateAnalogSensor";
	$NewReadingsFunctions['abc'] = "newAnalogReading";
	$DownloadReadingsFunctions['abc'] = "downloadAnalogReadings";
	$DownloadOptionsFunctions['abc'] = "analogDownloadOptions";
	$ConfigureOptionsFunctions['abc'] = "analogConfigureOptions";
	
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
		$set = false;
		for($i = 0; $i < 6; $i++){
			$string_i = "pin".$i;
			if(array_key_exists($string_i, $postArray) && $postArray[$string_i] == "1"){
				if(!$set){
					$set = true;
					$query .= " AND (pin='$i'";
				} else $query .= " OR pin='$i'";
			}
		}
		if($set) $query .= ")";
		$query .= " ORDER BY time";
		//echo $query;
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
	
	function analogDownloadOptions($postArray){
		echo '<form action="./current/downloadreadings.php" method="post">';
		echo 'Only need one:<br />';
		echo 'Serial: <input type="text" name="serial" /><br />';
		echo 'Nickname: <input type="text" name="nickname" /><br />';
		echo '<input type="checkbox" name="regular" value="1" /> Regular data?<br />';
		echo '<input type="checkbox" name="pollcurve" value="1" /> Poll curve data?<br />';
		echo '<br />';
		echo '<input type="checkbox" name="pin0" value="1" /> Pin 0<br />';
		echo '<input type="checkbox" name="pin1" value="1" /> Pin 1<br />';
		echo '<input type="checkbox" name="pin2" value="1" /> Pin 2<br />';
		echo '<input type="checkbox" name="pin3" value="1" /> Pin 3<br />';
		echo '<input type="checkbox" name="pin4" value="1" /> Pin 4<br />';
		echo '<input type="checkbox" name="pin5" value="1" /> Pin 5<br />';
		echo '<input type="submit" value="Submit" />';
		echo '</form>';
	}
	
	function analogConfigureOptions($postArray){
		echo '<form action="./current/configure.php" method="post">';
		echo 'Only need one:<br />';
		echo 'Serial: <input type="text" name="serial" /><br />';
		echo 'Nickname: <input type="text" name="nickname" /><br />';
		echo '<input type="checkbox" name="pollcurve" value="1" /> Do a poll curve?<br />';
		echo 'Poll curve start: <input type="text" name="pollcurvestart" value="MM/DD/YYYY HH:MM:SS"/><br />';
		echo 'Poll curve end: <input type="text" name="pollcurveend" value="MM/DD/YYYY HH:MM:SS"/><br />';
		echo 'Enter resistances separated by a comma, no spaces.<br />';
		echo 'Poll curve resistances: <input type="text" name="pollcurveend" /><br />';
		echo '<br />';
		echo '<input type="submit" value="Submit" />';
		echo '</form>';
	}
?>
