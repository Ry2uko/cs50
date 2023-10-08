SELECT
  title,
  rating
FROM
  movies
  JOIN ratings ON movies.id = ratings.movie_id
WHERE
  YEAR = 2010
ORDER BY
  rating DESC,
  title;