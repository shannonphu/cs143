<!DOCTYPE HTML>  
<html>
<head>
	<link rel="stylesheet" type="text/css" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
	<style>
		.error {color: #FF0000;}
	</style>
</head>
<body> 
	<?php include 'nav.php';?>
 	<?php include 'helper.php';?>

	<div class="container">
		<h2> Add an Actor</h2>
		<form id="add_actor" method="POST">
			<div class="form-group">
				<label for="first">First Name</label>
				<input type="text" name="first" class="form-control">
			</div>
			<div class="form-group">
				<label for="last">Last Name</label>
				<input type="text" name="last" class="form-control">
			</div>
			<div class="form-check">
				<label class="form-check-label">
				  <input class="form-check-input" type="radio" name="sex"  value="Male" /> Male
				</label>
			</div>
			<div class="form-group">
				<label class="form-check-label">
				  <input class="form-check-input" type="radio" name="sex"  value="Female" /> Female
				</label>
			</div>
			<div class="form-group">
				<label for="last">Date of Birth</label>
				<input type="text" name="dob" class="form-control" placeholder="ie: 1997-05-05">
			</div>
			<div class="form-group">
				<label for="last">Date of Death</label>
				<input type="text" name="dod" class="form-control" placeholder="ie: 1997-05-05">
			</div>
			<button type="submit" name="submit" id="submit-actor-btn" class="btn btn-primary">Submit</button>
		</form>
		<?php
			if(isset($_POST['submit']))
			{
			   $maxID = get_max_id('MaxPersonID');
			   $db = new mysqli('localhost', 'cs143', '', 'CS143');

			   if($db->connect_errno > 0) {
			       die('Unable to connect to database [' . $db->connect_error . ']');
			   }

			   $last = $_POST['last'];
   			   $first = $_POST['first'];
   			   $sex = $_POST['sex'];
   			   $dob = $_POST['dob'];
   			   $dod = 'null';
   			   if ($_POST['dod']) {
   			   		$dod = "'" . $_POST['dod'] . "'";
   			   }
   			   
   			   if ($last && $first && $sex && $dob) {
   			   		$query = "INSERT INTO Actor (id, last, first, sex, dob, dod) VALUES (" . $maxID . ", '" . $last . "', '" . $first . "', '" . $sex . "', '" . $dob . "', " . $dod . ");";
   			   		echo $query;
   			   		$db->query($query);
   			   		increment_max_id("MaxPersonID");

   			   		header("Location:actor.php?id=" . $maxID);
   			   } else {
   			   		echo '<p class="fail">You must fill in first name, last name, sex, and date of birth.</p>';
   			   }
			} 
		?>
	</div>

	<script type="text/javascript">
	  $(document).ready(function() {
  		    $('input[type="radio"]').not(':checked').prop("checked", true);
	  })
	</script>
</body>
</html> 