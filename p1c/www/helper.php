<?php

	// create drop down menus with all movie titles, actor names, or director names
	// call make_dropdown with either "actor", "movie", or "director" as a string input

	function make_dropdown($type, $key) {
		$db = new mysqli('localhost', 'cs143', '', 'CS143');
		$key_name = "";
		if($db->connect_errno > 0) {
		    die('Unable to connect to database [' . $db->connect_error . ']');
		}
		$query = "";
		if (strcmp($type, "movie") === 0) {
			//$query = "SELECT concat(title, ' (', year, ')') as movie_name from Movie order by title;";
			$query = "SELECT title as movie_name from Movie order by title;";
			$key_name = "movie_name";
		}
		elseif (strcmp($type,"actor") === 0) {
			$query = "SELECT concat(first, ' ', last) as actor_name from Actor order by first";
			$key_name = "actor_name";
		}
		else {
			$query = "SELECT concat(first, ' ', last) as director_name from Director order by first;";
			$key_name = "director_name";
		}

		$rs = $db->query($query);
		// echo '<form method="GET">';
		echo '<div class="form-group">';
		echo '<label for="sel1" name="thing">Select list (select one):</label>';
		echo '<select class="form-control" name="' . $key . '">';
		while ($row  = $rs->fetch_assoc()){
			$val = $row[$key_name];
			if (!$val){
				continue;
			}
			else {
				echo '<option>' . $val . '</option>';
			}
		}
		echo '</select>';
		echo '</div>';
		// echo '</form>';

		$rs->free();

		$db->close();
	}

	function make_generic_table_for_query($query, $redirect_type) {
		$db = new mysqli('localhost', 'cs143', '', 'CS143');

		if($db->connect_errno > 0) {
		    die('Unable to connect to database [' . $db->connect_error . ']');
		}

		$rs = $db->query($query);

		// Number of tuples returned
		// echo 'Number returned: ' . $rs->num_rows. '<br />';

		// Make results table
		echo '<table class="table table-hover table-responsive">';

		// Make column headers
		echo '<tr class="column-head">';

		// Get field information for all columns
		$columns = $rs->fetch_fields();
		$column_names = [];
		foreach ($columns as $column) {
			if ($column->name == 'id') {
				continue;
			}
			array_push($column_names, $column->name);
			echo '<td><strong>' . $column->name . '</strong></td>';
		}

		echo '</tr>';

		if ($rs->num_rows == 0) {
			echo '<td>No information available.</td>';
			$rs->free();
			$db->close();
			return;
		}

		while($row = $rs->fetch_assoc()) {
			echo '<tr class="clickable-row" data-href="' . $redirect_type . '.php?id=' . $row['id'] . '">';
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
	}

	function make_table_with_attr_for_query($query, $column_names, $column_headers, $redirect_type, $redirect_id_type) {
		$db = new mysqli('localhost', 'cs143', '', 'CS143');

		if($db->connect_errno > 0) {
		    die('Unable to connect to database [' . $db->connect_error . ']');
		}
		

		$rs = $db->query($query);


		// Number of tuples returned
		// echo 'Number returned: ' . $rs->num_rows. '<br />';

		// Make results table
		echo '<table class="table table-hover table-responsive">';

		// Make column headers
		echo '<tr class="column-head">';

		// Get field information for all columns
		foreach ($column_headers as $title) {
			echo '<td><strong>' . $title . '</strong></td>';
		}

		echo '</tr>';

		if ($rs->num_rows == 0) {
			echo '<td>No information available.</td>';
			$rs->free();
			$db->close();
			return;
		}

		while($row = $rs->fetch_assoc()) {
			echo '<tr class="clickable-row" data-href="' . $redirect_type . '.php?id=' . $row[$redirect_id_type] . '"' . '>';

			foreach ($column_names as $key) {
				if ($key == 'first+last') {
					$val = $row['first'] . ' ' . $row['last'];
				} elseif ($key == 'dob-dod') {
					$date_birth = date_create($row['dob']);
					$date_death = 'Still Alive';
					if ($row['dod']) {
						$date_death = date_format(date_create($row['dod']), "m/d/Y");
					}

					$val = date_format($date_birth, "m/d/Y") . ' - ' . $date_death;
				} else {
					$val = $row[$key];
				}
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
	}

	function check_if_id_exists($table, $id) {
		$db = new mysqli('localhost', 'cs143', '', 'CS143');

		if($db->connect_errno > 0) {
		    die('Unable to connect to database [' . $db->connect_error . ']');
		}

		$rs = $db->query("SELECT * FROM " . $table . " where id=" . $id . ";");
		return $rs->num_rows != 0;
	}

	function increment_max_id($table) {
		$db = new mysqli('localhost', 'cs143', '', 'CS143');

		if($db->connect_errno > 0) {
		    die('Unable to connect to database [' . $db->connect_error . ']');
		}

		$query = "UPDATE " . $table . " SET id=id+1;";
		$db->query($query);
	}

	function get_max_id($table) {
		$db = new mysqli('localhost', 'cs143', '', 'CS143');

		if($db->connect_errno > 0) {
		    die('Unable to connect to database [' . $db->connect_error . ']');
		}

		$rs = $db->query("SELECT id FROM " . $table . ";");
		$row = $rs->fetch_assoc()['id'];

		return $row;
	}
?>