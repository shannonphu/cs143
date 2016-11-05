<!DOCTYPE HTML>  
<html>
<head>
	<link rel="stylesheet" type="text/css" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
</head>
<body> 
	<?php include 'nav.php';?>
 	<?php include 'helper.php';?>
	<div class="container">
		<h2>Add a Movie</h2>
		<form id="add_movie" method="POST">
			<div class="form-group">
				<label for="title">Movie Title</label>
				<input type="text" name="title" class="form-control">
			</div>
			<div class="form-group">
				<label for="year">Year</label>
				<input type="text" name="year" class="form-control">
			</div>
			<div class="col-md-3">
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="Drama" name="genre[]"/> Drama
					</label>
				</div>
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="Comedy" name="genre[]"/> Comedy
					</label>
				</div>
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="Romance" name="genre[]"/> Romance
					</label>
				</div>
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="Crime" name="genre[]"/> Crime
					</label>
				</div>
			</div>
			<div class="col-md-3">
				<div class="form-group checkbox">			  
					<label class="genre-checkbox">
					<input type="checkbox" value="Horror" name="genre[]"/> Horror
					</label>
				</div>
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="Mystery" name="genre[]"/> Mystery
					</label>
				</div>
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="Thriller" name="genre[]"/> Thriller
					</label>
				</div>
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="Action" name="genre[]"/> Action
					</label>
				</div>
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="Adventure" name="genre[]"/> Adventure
					</label>
				</div>
			</div>
			<div class="col-md-3">
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="Fantasy" name="genre[]"/> Fantasy
					</label>
				</div>			
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="Documentary" name="genre[]"/> Documentary
					</label>
				</div>
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="Family" name="genre[]"/> Family
					</label>
				</div>
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="Sci-Fi" name="genre[]"/> Sci-Fi
					</label>
				</div>
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="Animation" name="genre[]"/> Animation
					</label>
				</div>
			</div>
			<div class="col-md-3">
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="Musical" name="genre[]"/> Musical
					</label>
				</div>
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="War" name="genre[]"/> War
					</label>
				</div>
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="Western" name="genre[]"/> Western
					</label>
				</div>
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="Adult" name="genre[]"/> Adult
					</label>
				</div>
				<div class="form-group checkbox">
					<label class="genre-checkbox">
					<input type="checkbox" value="Short" name="genre[]"/> Short
					</label>
				</div>
			</div>
			<div class="form-group">
				<label for="rating" name="rating">Select a rating</label>
				<select class="form-control" name="rating">
					<option value="G">G</option>
					<option value="PG">PG</option>
					<option value="PG-13">PG-13</option>
					<option value="NC-17">NC-17</option>
					<option value="surrendere">surrendere</option>
				</select>
			</div>
			<div class="form-group">
				<label for="company">Company</label>
				<input type="text" name="company" class="form-control">
			</div>
			<button type="submit" name="submit" class="btn btn-primary">Submit</button>
		</form>
		<?php
			if(isset($_POST['submit']))
			{
			   $maxID = get_max_id('MaxMovieID');
			   $db = new mysqli('localhost', 'cs143', '', 'CS143');

			   if($db->connect_errno > 0) {
			       die('Unable to connect to database [' . $db->connect_error . ']');
			   }

			   $title = $_POST['title'];
   			   $year = $_POST['year'];
   			   $rating = $_POST['rating'];
   			   $company = $_POST['company'];
   			   $genres = $_POST['genre'];

   			   if ($title && $year && $rating && $company) {
				   $query = "INSERT INTO Movie (id, title, year, rating, company) VALUES (" . $maxID . ", '" . $title . "', '" . $year . "', '" . $rating . "', '" . $company . "');";

				   if ($db->query($query)) {
					echo '<p class="success">Successfully inserted.</p>';
				   }

				   // add genres to MovieGenre table
				   foreach($genres as $g) {
						$query = "INSERT INTO MovieGenre (mid, genre) VALUES (" . $maxID . ", '" . $g . "');";
						$db->query($query);
				   }

				   increment_max_id('MaxMovieID');
				} else {
					echo '<p class="fail">You must fill in all form parts.</p>';
				}
			} 
		?>
	</div>
</body>
</html> 