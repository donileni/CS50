SELECT avg(ALL rating) FROM ratings JOIN movies ON movie_id = movies.id WHERE movies.year = 2012;