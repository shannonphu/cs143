CREATE TABLE Movie(
	id int NOT NULL,
	title varchar(100), 
	year int, 
	rating varchar(10), 
	company varchar(50),
	-- Each movie id should be unique
	PRIMARY KEY(id),
	CHECK(year < 2017)
) ENGINE=INNODB;

CREATE TABLE Actor(
	id int,
	last varchar(20),
	first varchar(20),
	sex varchar(6),
	dob date,
	dod date,
	-- Each Actor id should be unique)
	PRIMARY KEY(id),
	CHECK(sex='Male' OR sex='Female')
) ENGINE=INNODB;

CREATE TABLE Director(
	id int NOT NULL,
	last varchar(20),
	first varchar(20),
	dob date,
	dod date,
	PRIMARY KEY (id)
) ENGINE=INNODB;

CREATE TABLE MovieGenre(
	mid int,
	genre varchar(20),
	-- each movie has a genre
	FOREIGN KEY (mid) REFERENCES Movie(id)
) ENGINE=INNODB;

CREATE TABLE MovieDirector(
	mid int,
	did int,
	-- A movie director will have an accompnaying director 
	FOREIGN KEY (mid) REFERENCES Movie(id),
	-- Every director has a director bio
	FOREIGN KEY (did) REFERENCES Director(id)
) ENGINE = INNODB;

CREATE TABLE MovieActor(
	mid int,
	aid int,
	role varchar(50),
	PRIMARY KEY(mid, aid),
	-- A movie actor has to act in a movie
	FOREIGN KEY (mid) REFERENCES Movie(id),
	-- An actor has an actor page
	FOREIGN KEY (aid) REFERENCES Actor(id)
) ENGINE = INNODB;

CREATE TABLE Review(
	name varchar(20),
	time timestamp,
	mid int,
	rating int,
	comment varchar(500),
	-- A review has a corresponding movie that it is reviewing 
	FOREIGN KEY (mid) REFERENCES Movie(id),
	-- ratings are from one to five stars 
	CHECK(rating >0 and rating < 6)
) ENGINE=INNODB;

CREATE TABLE MaxPersonID(
	id int
) ENGINE=INNODB;

CREATE TABLE MaxMovieID(
	id int
) ENGINE = INNODB;

