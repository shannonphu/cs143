<html>
 <head>
  <title>MySQL Interface</title>
 </head>
 <body> 
	<p>Type an SQL query in the following box:</p>
	<p>Example: SELECT * FROM Actor WHERE id=10;</p>
	<form action="query.php" method="GET">
		<textarea cols="60" rows="8" name="expr"></textarea>
		<br />
		<input type="submit" value="Submit">
	</form>

 	<?php
  
 		 $db = new mysqli('localhost', 'cs143', '', 'CS143');

 		 if($db->connect_errno > 0) {
 		     die('Unable to connect to database [' . $db->connect_error . ']');
 		 }

 		 $query = $_GET["expr"]; // ie: "SELECT * FROM Actor WHERE id < 50";
 		 $rs = $db->query($query);

 		 // Number of tuples returned
 		 // echo 'Number returned: ' . $rs->num_rows. '<br />';

 		 // Make results table
 		 echo '<table>';

 		 // Make column headers
 		 echo '<tr id="column-head">';

 		 // Get field information for all columns
 		 $columns = $rs->fetch_fields();
 		 $column_names = [];
 		 foreach ($columns as $column) {
 		 	array_push($column_names, $column->name);
 		 	echo '<td><strong>' . $column->name . '</strong></td>';
 		 }

 		 echo '</tr>';

 		 while($row = $rs->fetch_assoc()) {
 		 	echo '<tr>';
 		 	foreach ($column_names as $key) {
 		 		$val = $row[$key];
 		 		echo '<td>';
 		 		if (!$val) {
 		 			echo "N/A";
 		 		}
 		 		else {
 		 			echo $val;
 		 		}
 		 		echo "</td>";
 		 	}
 		 	echo '</tr>';
 		 }

 		 $rs->free();

 		 $db->close();

 	?>

 	<style type="text/css">
 		td {
 			border: solid black 1px;
 			padding: 5px;
 		}
 	</style>
  </body>
 </html>