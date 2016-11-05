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
			<h2>Add a Director to a Movie</h2>
			<h3>Director</h3>
			<?php
				make_dropdown("director", "director");
			?>

			<h3>Movie</h3>
			<?php
				make_dropdown("movie", "movie");
			?>
			<button type="submit" name="submit" id="submit-actor-btn" class="btn btn-primary">Submit</button>
		</form>
		<br>
		<?php
			if (isset($_POST["submit"])){
				$director = explode(' ', $_POST["director"]);
				$movie = $_POST["movie"];
				$first_name = $director[0];
				$last_name = $director[1];

				$mid_query = "SELECT id from Movie where title = '" . $movie . "';";
				$did_query = "SELECT id from Director where first = '"  . $first_name  . "' AND last = '" . $last_name . "';";

				$db = new mysqli('localhost', 'cs143', '', 'CS143');
				if($db->connect_errno > 0) {
				    die('Unable to connect to database [' . $db->connect_error . ']');
				}
				$rs = $db->query($mid_query);
				$mid = $rs->fetch_assoc()['id'];
				$rs = $db->query($did_query);
				$did = $rs->fetch_assoc()['id'];
				$query = "INSERT into MovieDirector (mid, did) VALUES (" . $mid . ", " . $did . ");";

				if ($db->query($query) === TRUE) {
					echo '<p class="success">Success</p>';
				} else {
					echo '<p class="fail">Failure to add relation.</p>';
				}
			}
		?>
	</div>
</body>
</html>