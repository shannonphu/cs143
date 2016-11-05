-- Violates unique property of primary key id cannot insert a duplicate value into table
insert into Actor(id, last, first, sex, dob, dod) 
values (68635, 'Jiang', 'Jason', 'Male', '1995-04-01', NULL);
-- ERROR 1062 (23000): Duplicate entry '68635' fpr key 'PRIMARY'

-- Violates unique property of primary key id in Movie attempted to insert a movie in with an id the same as a preexisting entry
insert into Movie(id, title, year, rating, company)
values (4734, 'The Room', 2000, 'PG-13', 'Pixar')
-- ERROR 1062 (23000): Duplicate entry '68635' fpr key 'PRIMARY'

-- Violates unique property of primary key (mid,aid) in MovieActor an actor with id 4734 already exists in the table 
insert into MovieActor(mid, aid, role)
values (4734, 52225, 'boy')
-- ERROR 1062 (23000) Duplicate entry'4734-52225' for key 'PRIMARY'

-- Cannot add to a table with a foreign key constraint cannot add an addition movie and its genre 
insert into MovieGenre (mid, genre) values (6555, 'fan-fic');
-- ERROR 1452 (23000) Cannot add or update a child row: a foreign key constraint fails ('TEST' . 'MovieGenre', CONSTRAINT 'MovieGenre_ibfk_1' FOREIGN KEY 9'mid') REFERENCES 'Movie' ('id'))

-- Cannot insert a new value into a table with a foreign key constraint since appropriate referenced value does not exist in the other table yet
insert into MovieDirector (mid, did) values(636, 637);
-- ERROR 1452 (23000) Cannot add or update a child row: a foreign key constraint fails ('TEST' . 'MovieDirector', CONSTRAINT 'MovieDirector_ibfk_1' FOREIGN KEY 9'mid') REFERENCES 'Movie' ('id'))

-- Foreign Key Constraint Violation: cannot update a table where a key is references another table attribute
update MovieDirector set did = did + 100;
-- ERROR 1452 (23000) Cannot add or update a child row: a foreign key constraint fails ('TEST' . 'MovieDirector', CONSTRAINT 'MovieDirector_ibfk_2' FOREIGN KEY 9'mid') REFERENCES 'Director' ('id'))

-- Foreign Key Constraint Violation attempting to insert a new value into a table that references an attribute from another table which may not be defined yet
insert into MovieActor (mid, aid, role) values (64, 56, 'tree');
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`TEST`. `MovieActor`, CONSTRAINT `MovieActor_ib_fk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`))

-- Foreign Key Constraint Violation: cannot update a table where a key references another attribute in a separate table
update MovieActor set aid = aid+100;
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`TEST`. `MovieActor`, CONSTRAINT `MovieActor_ib_fk_2` FOREIGN KEY (`aid`) REFERENCES `Actor` (`id`))

-- Foreign Key Constraint Violation: cannot delete a row from table B in relation table A references Table B Cannot delete a movie that is referenced by a review
delete from Movie where rating = 'PG-13';
-- ERROR 1451 (23000): Cannot delete or update from a parent row: a foreign key constraint fails (`TEST.`MovieGenre`, CONSTRAINT `MovieGenre_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`))

-- Violates Check constraint: adding a movie with a release year in the future >2016
insert into Movie(id, title, year, rating, company) values (6548, 'merp', 3014, 'R','pixar');

-- Violates Check constraint: adding an actor whose sex is neither female nor male
insert into Actor(id, last, first, sex, dob, dod) values (6969, 'guy', 'pink', '????', '1969-04-01', NULL);

-- Violates Check constraint: adding a review whose value is not 1-5 inclusive
insert into Review(name, time, mid, rating, comment) values ('john', '1000-01-01 00:00:00', 6, 8, 'bleh');

