SELECT AVG(ratings.rating) AS "Average Rating of Movies in 2012" FROM movies, ratings WHERE ratings.movie_id = movies.id AND movies.year = "2012";