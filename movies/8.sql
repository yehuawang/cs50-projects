SELECT name FROM people WHERE people.id IN (SELECT stars.person_id FROM stars WHERE stars.movie_id = (SELECT movies.id FROM movies WHERE movies.title = 'Toy Story'));
