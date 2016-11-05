<!DOCTYPE HTML>  
<html>
<head>
	<link rel="stylesheet" type="text/css" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
</head>
<body>
 	<?php include 'nav.php';?>
 	<?php include 'helper.php';?>
 	<div class="container">
 	<h2>Add a comment</h2>
 	<form id="add-comment" method="POST">
 		<div class="form-group">
 			<label for="first">Your Name</label>
 			<input type="text" name="name" class="form-control">
 		</div>
 		<div class="form-group">
 			<label for="sel1">Select a score</label>
 			<select name="rating" class="form-control">
 				<option>1</option>
 				<option>2</option>
 				<option>3</option>
 				<option>4</option>
 				<option>5</option>
 			</select>
 		</div>
 		<div class="form-group">
 			<label for="first">Comment</label>
 			<textarea class="form-control" name="comment" rows="5" id="comment"></textarea>
 		</div>
 		<!-- <input type="submit" value="DONE"> -->
 		<button type="submit" name="submit" id="submit-actor-btn" class="btn btn-primary">Submit</button>
 	</form>
 		<?php
 		if(isset($_POST["submit"])) {
 			$db = new mysqli('localhost', 'cs143', '', 'CS143');

 			if($db->connect_errno > 0) {
 			    die('Unable to connect to database [' . $db->connect_error . ']');
 			}
 			$rating = $_POST["rating"];
 			$comment = $_POST["comment"];
 			$id = $_GET["id"];
 			$name = $_POST["name"];
 			// echo $rating;
 			$time =  date('Y-m-d h:i:s');
 			$query = "INSERT INTO Review (name, time, mid, rating, comment) VALUES (" . "'" . $name . "', "  . "'" . $time . "', " . $id . ", " . $rating . ", '" . $comment . "')";
 			// echo $query;
 			if ($db->query($query)) {
 				echo "success";
 			}
 			else {
 				echo "failure";
 			}
 		}
 		php?>
</body>
</html>