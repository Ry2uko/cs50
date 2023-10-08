SELECT
  AVG(rating)
FROM
  movies
  JOIN ratings ON movies.id = ratings.movie_id
WHERE
  YEAR = 2012;