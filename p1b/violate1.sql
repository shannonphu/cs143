-- 3 primary key contraints --
CREATE TABLE Actor(
	id int,
	last varchar(20),
	first varchar(20),
	sex varchar(6),
	dob date,
	dod date,
	--Each Actor id should be unique)
	PRIMARY KEY(id),
);

CREATE TABLE Movie(
	id int, 
	title varchar(100), 
	year int, 
	rating varchar(10), 
	company varchar(50),
	--Each movie id should be unique
	PRIMARY KEY(id)
);

CREATE TABLE MovieActor(
	mid int,
	aid int,
	role varchar(50),
	--Each actor can only have one role in a given movie
	PRIMARY KEY(mid, aid),
);



-- 6 referentails integrity constraints -- 
movie actor -> actor 
review -> movie 
moviedirector -> movie 
actor -> director 
movie -> movie genre 

CREATE TABLE MovieDirector(
	--A movie director will have an accompnaying director 
	mid int REFERENCES Movie(id),
	--Every director has a director bio
	did int REFERENCES Director(id)
);


CREATE TABLE Movie(
	-- each movie has a genre
	id int REFERENCES MovieGenre(id), 
	title varchar(100), 
	year int, 
	rating varchar(10), 
	company varchar(50)
); 


CREATE TABLE Review(
	name varchar(20),
	time timestamp,
	--A review has a corresponding movie that it is reviewing 
	mid int REFERENCES Movie(id),
	rating int,
	comment varchar(500)
);

CREATE TABLE MovieActor(
	-- A movie actor has to act in a movie
	mid int REFERENCES Movie(id),
	-- a movie actor can be a director probably doesn't work
	aid int REFERENCES Actor(id)
	role varchar(50),
);

--Check Constraints 

CREATE TABLE Review(
	name varchar(20),
	time timestamp,
	mid int,
	rating int,
	comment varchar(500),
	-- ratings are from one to five stars 
	CHECK (rating >0 and rating < 6)
);

CREATE TABLE Actor(
	id int,
	last varchar(20),
	first varchar(20),
	sex varchar(6),
	dob date,
	dod date,
	CHECK (sex='Male' OR sex='Female')
);

CREATE TABLE Movie(
	id int, 
	title varchar(100), 
	year int, 
	rating varchar(10), 
	company varchar(50),
	CHECK (year < 2017)
);







