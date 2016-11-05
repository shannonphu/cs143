<html>
<head>
	<!-- TODO set as this director's name -->
	<title>Director</title>
	<link rel="stylesheet" type="text/css" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
</head>
 <body> 
 	<?php include 'nav.php';?>
 	<?php include 'helper.php';?>

 	<div class="container">
 		<h3>Director</h3>
	 	<?php
	 		// Make actor table
	 		$query = "SELECT * FROM Director where id = " . $_GET['id'];
	 		make_table_with_attr_for_query($query, ['first+last', 'dob-dod'], ["Name", "Years Alive"], 'director', 'id');

	 		 // Make table for movies this director directed
	 		 $query = "SELECT * FROM MovieDirector md, Movie m where m.id = md.mid and md.did = " . $_GET['id'];
	 		 make_table_with_attr_for_query($query, ['title', 'year', 'rating', 'company'], ['Movie title', 'Year', 'Rating', 'Company'], 'movie', 'id');
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