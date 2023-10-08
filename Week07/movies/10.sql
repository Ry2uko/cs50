SELECT
  name
FROM
  (
    SELECT
      *
    FROM
      directors
    WHERE
      movie_id IN (
        SELECT
          id
        FROM
          movies
          JOIN ratings ON movies.id = ratings.movie_id
        WHERE
          rating >= 9.0
      )
  ) AS m
  JOIN people ON m.person_id = people.id;