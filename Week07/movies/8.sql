SELECT
  name
FROM
  (
    SELECT
      *
    FROM
      stars
    WHERE
      movie_id = (
        SELECT
          id
        FROM
          movies
        WHERE
          title = 'Toy Story'
      )
  ) AS m
  JOIN people ON m.person_id = people.id;