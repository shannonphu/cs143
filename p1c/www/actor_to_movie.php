<!DOCTYPE HTML>  
<html>
<head>
	<link rel="stylesheet" type="text/css" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
</head>
<body>
	<?php include 'nav.php';?>
	<?php include 'helper.php';?>
	<div class="container">
		<form id="add-comment" method="POST">
			<h2>Add an Actor to a Movie</h2>
			<h3>Actor</h3>
			<?php make_dropdown("actor", "actor"); ?>
			<h3>Movie</h3>
			<?php make_dropdown("movie", "movie"); ?>
			<div class="form-group">
				<label for="first">Role</label>
				<input type="text" name="role" class="form-control">
			</div>
			<button type="submit" name="submit" id="submit-actor-btn" class="btn btn-primary">Submit</button>	
		</form>
	<?php
		if (isset($_POST["submit"])){
			$actor = explode(' ', $_POST["actor"]);
			$first_name = $actor[0];
			$last_name = $actor[1];
			$movie = $_POST["movie"];
			$role = $_POST["role"];

			if (!$role) {
				echo '<p class="fail">You must fill in the role field.</p>';
				return;
			}

			// echo $first_name;
			// echo "<br>";
			// echo $last_name;
			// echo "<br>";
			// echo $role;
			// echo "<br>";
			// echo $movie;
			// echo "<br>";
			$mid_query = "SELECT id from Movie where title = '" . $movie . "';";
			$aid_query = "SELECT id from Actor where first = '"  . $first_name  . "' AND last = '" . $last_name . "';";
			$db = new mysqli('localhost', 'cs143', '', 'CS143');
			if($db->connect_errno > 0) {
			    die('Unable to connect to database [' . $db->connect_error . ']');
			}
			$rs = $db->query($mid_query);
			$mid = $rs->fetch_assoc()['id'];
			$rs = $db->query($aid_query);
			$aid = $rs->fetch_assoc()['id'];
			$query = "INSERT into MovieActor (mid, aid,  role) VALUES (" . $mid . ", " . $aid . ", '" .$role . "');";
			// echo $aid;
			// echo "<br>";
			// echo $mid;
			// echo "<br>";
			// echo $role;
			if ($db->query($query) !==0) {
				echo '<p class="success">Submission success!</p>';
			}
		}
	?>
	</div>

</body>
</html>