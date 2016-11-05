<html>
<head>
	<!-- TODO set as this actor's name -->
	<title>Actor</title>
	<link rel="stylesheet" type="text/css" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
</head>
 <body> 
 	<?php include 'nav.php';?>
 	<?php include 'helper.php';?>

 	<div class="container">
		<h3>Actors</h3>

	 	<?php
	 		// Make actor table
	 		$query = "SELECT * FROM Actor where id = " . $_GET['id'];
	 		make_table_with_attr_for_query($query, ['first+last', 'dob-dod', 'sex'], ["Name", "Years Alive", "Gender"], 'actor', 'id');

	 		 // Make actor to movie table
	 		 $query = "SELECT * FROM MovieActor ma, Movie m where m.id = ma.mid and aid = " . $_GET['id'];
	 		 make_table_with_attr_for_query($query, ['title', 'role', 'year', 'rating', 'company'], ['Title', 'Role', 'Year', 'Rating', 'Company'], 'movie', 'mid');
	 	?>
 	</div>

 	<script type="text/javascript">
 		$(document).ready(function($) {
 		    $(".clickable-row").click(function() {
 		        window.document.location = $(this).data("href");
 		    });
 		});
 	</script>
  </body>
 </html>