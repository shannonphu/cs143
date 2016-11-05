-- get actors full name who acted in the movie 'Die Another Day'
SELECT CONCAT(first, ' ', last) as Name
FROM Actor a, MovieActor ma, Movie m
WHERE a.id = ma.aid AND ma.mid = m.id AND m.title = 'Die Another Day';

--Get number of actors who have acted in multiple movies
SELECT COUNT(*)
FROM (SELECT aid FROM MovieActor GROUP BY aid HAVING COUNT(mid) > 1) S;

-- get full name of female actors who are still alive
SELECT CONCAT(first, ' ', last)
FROM Actor
WHERE sex = 'Female' and dod IS NULL;