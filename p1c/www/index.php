<html>
<head>
	<title>Home</title>
	<link rel="stylesheet" type="text/css" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
</head>
<body> 
 	<?php include 'nav.php';?>
 	<?php include 'helper.php';?>

 	<div class="container">
	 	<?php
	 		$param = $_GET['query'];
	 		$dropdown = $_GET["director"];
	 		$dropdown1 = $_GET["actor"];
	 		if ($dropdown !== null) {
	 			echo $dropdown;
	 		}
	 		if ($dropdown1 !== null) {
	 			echo $dropdown1;
	 		}
	 		if ($param !== null) {
	 			$param = preg_replace('!\s+!', ' ', $param);
	 			$keywords = explode(' ', $param);
	 			$actor_query = "WHERE first LIKE " . '\'%' . $keywords[0] . '%\'' . "OR last LIKE " . '\'%' . $keywords[0] . '%\'';
	 			$movie_query = "WHERE title LIKE " . '\'%' . $keywords[0] . '%\'';
	 			$query = $keywords[0];
	 			if (count($keywords) > 1){
	 				for ($i = 1; $i < count($keywords); $i++) {
	 			 		$query = $query . ' ' . $keywords[$i];
	 				}
	 			}
	 			$actor_where = "";
	 			$movie_where = "";
	 			if (count($keywords) >1 ){
	 				$actor_where = "WHERE CONCAT(first, ' ', last) like '%" . $query . "%'" ;
	 				$movie_where = "WHERE title LIKE " . "'%" . $query . "%'";
	 			}
	 			else {
	 				$actor_where = $actor_query;
	 				$movie_where = $movie_query;
	 			}
	 			$actor_query = $actor_query . ';';
	 			$query = "SELECT * FROM Actor " . $actor_where;
	 			make_table_with_attr_for_query($query, ['first+last', 'dob-dod', 'sex'], ["Actors", "Years Alive", "Gender"], 'actor', 'id');

	 			$query = "SELECT * FROM Movie " . $movie_where;
	 			make_table_with_attr_for_query($query, ['title', 'year', 'rating', 'company'], ['Movies', 'Year', 'Rating', 'Company'], 'movie', 'id');
	 					echo '<form method="GET">';
	 			// make_dropdown("director", "director");
	 			// make_dropdown("actor", "actor");
	 			// echo  '<button type="submit" class="btn btn-default" id="search_btn">Submit</button>';
	 			// 		echo '</form>';
	 			// echo $_GET["director"];
	 			// echo $_GET["actor"];
	 		}
	 		else {
		 		$query = "SELECT * FROM Actor where id < 200;";
		 		make_table_with_attr_for_query($query, ['first+last', 'dob-dod', 'sex'], ["Actors", "Years Alive", "Gender"], 'actor', 'id');

	 	 		$query = "SELECT * FROM Movie where id < 200;";
	 	 		make_table_with_attr_for_query($query, ['title', 'year', 'rating', 'company'], ['Movies', 'Year', 'Rating', 'Company'], 'movie', 'id');

	 	 		$query = "SELECT * FROM Director where id < 400;";
	 	 		make_table_with_attr_for_query($query, ['first+last', 'dob-dod'], ["Director", "Years Alive"], 'director', 'id');
	 		}
	 		

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