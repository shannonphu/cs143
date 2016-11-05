<!DOCTYPE HTML>  
<html>
<head>
	<title>Movie</title>
	<link rel="stylesheet" type="text/css" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
</head>
 <body> 
 	<?php include 'nav.php';?>
 	<?php include 'helper.php';?>
 	<div class="container">
	 	<?php
	 		$query = "SELECT * FROM Movie where id = " . $_GET['id'];
	 		make_table_with_attr_for_query($query, ['title', 'year', 'rating', 'company'], ['Movie Title', 'Year', 'Rating', 'Company'], 'movie', 'id');

			// Get actors in this movie 
			$query = "SELECT * FROM MovieActor ma, Actor a where a.id = ma.aid and ma.mid = " . $_GET['id'];
 	 		make_table_with_attr_for_query($query, ['first+last', 'role'], ['Actor', 'Role'], 'actor', 'id');

		 	$query = "SELECT * FROM MovieDirector md, Director d where d.id = md.did and md.mid = " . $_GET['id'];
		 	make_table_with_attr_for_query($query, ['first+last'], ['Director'], 'director', 'id');

	 		$genre_query = "SELECT genre from MovieGenre where mid = " . $_GET['id'] . ";";
	 		make_generic_table_for_query($genre_query, "moviegenre");
	 		$query = "SELECT name as Reviewer,comment as Review, time as Date, rating FROM Review where mid = " . $_GET["id"] . ";";
	 		$query1 = "SELECT avg(rating) as 'Average Rating' from Review where mid = " . $_GET["id"] . ";";
	 		make_generic_table_for_query($query, "movie");
	 		make_generic_table_for_query($query1, "movie");
	 		echo "<div>";
	 		echo '<a class="btn btn-primary" href="comment.php?id=' . $_GET['id'] . '"> Add a comment </a>';
	 		echo "</div>"
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