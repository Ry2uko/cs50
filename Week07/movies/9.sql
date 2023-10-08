SELECT DISTINCT
  (name)
FROM
  (
    SELECT
      person_id
    FROM
      stars
    WHERE
      movie_id IN (
        SELECT
          id
        FROM
          movies
        WHERE
          YEAR = 2004
      )
  ) AS m
  JOIN people ON m.person_id = people.id
ORDER BY
  birth;