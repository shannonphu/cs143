<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
<script type="text/javascript" src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
<nav class="navbar navbar-default">
  <div class="container-fluid">
    <!-- Brand and toggle get grouped for better mobile display -->
    <div class="navbar-header">
      <a class="navbar-brand" href="index.php">Demo</a>
    </div>

    <!-- Collect the nav links, forms, and other content for toggling -->
    <!--<div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">-->
      <ul class="nav navbar-nav">
        <li class="dropdown">
          <a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-haspopup="true" aria-expanded="false">Add Entity<span class="caret"></span></a>
          <ul class="dropdown-menu">
            <li><a href="add_actor.php">Actor</a></li>
            <li><a href="add_director.php">Director</a></li>
            <li><a href="add_movie.php">Movie</a></li>
          </ul>
        </li>
        <li class="dropdown">
          <a href="#" class="dropdown-toggle" data-toggle="dropdown" role="button" aria-haspopup="true" aria-expanded="false">Add Relation<span class="caret"></span></a>
          <ul class="dropdown-menu">
            <li><a href="director_to_movie.php">Director/Movie Relation</a></li>
            <li><a href="actor_to_movie.php">Actor/Movie Relation</a></li>
          </ul>
        </li>
      </ul>
      <form id='search_form' class="navbar-form navbar-right" method='POST'>
        <div class="form-group">
          <input type="text" class="form-control" placeholder="Search">
        </div>
        <button type="submit" class="btn btn-default" id='search_btn'>Submit</button>
      </form>
    </div><!-- /.navbar-collapse -->
  <!-- </div>--><!-- /.container-fluid -->
</nav>
<script type="text/javascript">
  $(document).ready(function() {
    $('#search_btn').click(function(){
      $('#search_form').attr('action', 'index.php?query=' + $('input').val());
    });
  });
</script>
<style type="text/css">
  .success {
    color: green;
  }
  .fail {
    color: red;
  }
</style>