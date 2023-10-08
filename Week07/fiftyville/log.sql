-- took place on July 28, 2021 on Humphrey Street
-- check crime scene reports
SELECT
  *
FROM
  crime_scene_reports
WHERE
  street = "Humphrey Street";

-- crime_scene_report of id: 295 report of the theft of the CS50 duck
-- took place at 10:15am at the Humphrey Street Bakery with three witnesses
-- check interview transcripts that mentions 'bakery'
SELECT
  *
FROM
  interviews
WHERE
  transcript LIKE "%bakery%";

-- thief 1 left the bakery ten minutes of the theft (10:15am) and got away in a car
-- thief 1 also withdrawed from the ATM on Leggett Street, the morning before the theft
-- thief 1 calls thief 2 for less than a minute and said that they will take the earliest flight out of Fiftyville tomorrow, and thief 2 will purchase the flight ticket
-- get license plates that exited the bakery after around 10 minutes of the theft (10:15am)
SELECT
  license_plate
FROM
  bakery_security_logs
WHERE
  MONTH = 7
  AND DAY = 28
  AND YEAR = 2021
  AND HOUR = 10
  AND MINUTE >= 15
  AND MINUTE <= 25
  AND activity = "exit";

-- these are the license plates of the car of thief1
-- get the people who withdrawed at the ATM at Leggett Street that day
SELECT
  *
FROM
  people
WHERE
  id IN (
    SELECT
      person_id
    FROM
      atm_transactions AS atm
      JOIN bank_accounts AS bank ON atm.account_number = bank.account_number
    WHERE
      MONTH = 7
      AND DAY = 28
      AND YEAR = 2021
      AND atm_location = "Leggett Street"
      AND transaction_type = "withdraw"
  );

-- now we narrowed down to 8 people who are the possible thief 1:
-- Kenny, Iman, Benista, Taylor, Brooke, Luca, Diana, Bruce
-- Now we compare the license plates from the plates we got earlier
SELECT
  *
FROM
  people
WHERE
  id IN (
    SELECT
      person_id
    FROM
      atm_transactions AS atm
      JOIN bank_accounts AS bank ON atm.account_number = bank.account_number
    WHERE
      MONTH = 7
      AND DAY = 28
      AND YEAR = 2021
      AND atm_location = "Leggett Street"
      AND transaction_type = "withdraw"
  )
  AND license_plate IN (
    SELECT
      license_plate
    FROM
      bakery_security_logs
    WHERE
      MONTH = 7
      AND DAY = 28
      AND YEAR = 2021
      AND HOUR = 10
      AND MINUTE >= 15
      AND MINUTE <= 25
      AND activity = "exit"
  );

-- We're almost there, now we narrowed down to only 4 people:
-- Iman, Luca, Diana, Bruce
-- These are the ones who left the bakery within 10 minutes after the theft who
-- also withdrawed from the bank at Legget Street, we still can't draw conclusions here, so...
-- remember that thief 1 called thief 2 (for less than a minute) as thief 1 was leaving the bakery:
SELECT
  caller,
  receiver
FROM
  phone_calls
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 28
  AND duration <= 60;

-- these are the suspected caller and reciever, and one of these is between the thieves
-- filter the rat!
SELECT
  *
FROM
  people
WHERE
  id IN (
    SELECT
      person_id
    FROM
      atm_transactions AS atm
      JOIN bank_accounts AS bank ON atm.account_number = bank.account_number
    WHERE
      MONTH = 7
      AND DAY = 28
      AND YEAR = 2021
      AND atm_location = "Leggett Street"
      AND transaction_type = "withdraw"
  )
  AND license_plate IN (
    SELECT
      license_plate
    FROM
      bakery_security_logs
    WHERE
      MONTH = 7
      AND DAY = 28
      AND YEAR = 2021
      AND HOUR = 10
      AND MINUTE >= 15
      AND MINUTE <= 25
      AND activity = "exit"
  )
  AND phone_number IN (
    SELECT
      caller
    FROM
      phone_calls
    WHERE
      YEAR = 2021
      AND MONTH = 7
      AND DAY = 28
      AND duration <= 60
  );

-- we are now down to 2: Diana and Bruce
-- Lastly, we get the earliest flight on the day after the theft (July 29, 2021)
SELECT
  id
FROM
  flights
WHERE
  YEAR = 2021
  AND MONTH = 7
  AND DAY = 29
ORDER BY
  HOUR,
  MINUTE
LIMIT
  1;

-- the earliest flight on that day is flight id 36 on 8:20am
-- anddd we check who among the two is aboard the earliest flight the day after the theft
SELECT
  name
FROM
  people
WHERE
  id IN (
    SELECT
      person_id
    FROM
      atm_transactions AS atm
      JOIN bank_accounts AS bank ON atm.account_number = bank.account_number
    WHERE
      MONTH = 7
      AND DAY = 28
      AND YEAR = 2021
      AND atm_location = "Leggett Street"
      AND transaction_type = "withdraw"
  )
  AND license_plate IN (
    SELECT
      license_plate
    FROM
      bakery_security_logs
    WHERE
      MONTH = 7
      AND DAY = 28
      AND YEAR = 2021
      AND HOUR = 10
      AND MINUTE >= 15
      AND MINUTE <= 25
      AND activity = "exit"
  )
  AND phone_number IN (
    SELECT
      caller
    FROM
      phone_calls
    WHERE
      YEAR = 2021
      AND MONTH = 7
      AND DAY = 28
      AND duration <= 60
  )
  AND passport_number IN (
    SELECT
      passport_number
    FROM
      passengers
    WHERE
      flight_id = (
        SELECT
          id
        FROM
          flights
        WHERE
          YEAR = 2021
          AND MONTH = 7
          AND DAY = 29
        ORDER BY
          HOUR,
          MINUTE
        LIMIT
          1
      )
  );

-- There, so thief 1 is Bruce!
-- To find thief 2, we just check the receiver of the phone call:
SELECT
  name
FROM
  people
WHERE
  phone_number = (
    SELECT
      receiver
    FROM
      phone_calls
      JOIN people ON phone_calls.caller = people.phone_number
    WHERE
      YEAR = 2021
      AND MONTH = 7
      AND DAY = 28
      AND duration <= 60
      AND name = (
        SELECT
          name
        FROM
          people
        WHERE
          id IN (
            SELECT
              person_id
            FROM
              atm_transactions AS atm
              JOIN bank_accounts AS bank ON atm.account_number = bank.account_number
            WHERE
              MONTH = 7
              AND DAY = 28
              AND YEAR = 2021
              AND atm_location = "Leggett Street"
              AND transaction_type = "withdraw"
          )
          AND license_plate IN (
            SELECT
              license_plate
            FROM
              bakery_security_logs
            WHERE
              MONTH = 7
              AND DAY = 28
              AND YEAR = 2021
              AND HOUR = 10
              AND MINUTE >= 15
              AND MINUTE <= 25
              AND activity = "exit"
          )
          AND phone_number IN (
            SELECT
              caller
            FROM
              phone_calls
            WHERE
              YEAR = 2021
              AND MONTH = 7
              AND DAY = 28
              AND duration <= 60
          )
          AND passport_number IN (
            SELECT
              passport_number
            FROM
              passengers
            WHERE
              flight_id = (
                SELECT
                  id
                FROM
                  flights
                WHERE
                  YEAR = 2021
                  AND MONTH = 7
                  AND DAY = 29
                ORDER BY
                  HOUR,
                  MINUTE
                LIMIT
                  1
              )
          )
      )
  );

-- The accomplice is Robin, the one who bought the flight tickets for thief 1
-- Lastly, we get the city where the flight is heading
SELECT
  city
FROM
  airports
WHERE
  id = (
    SELECT
      destination_airport_id
    FROM
      flights
    WHERE
      YEAR = 2021
      AND MONTH = 7
      AND DAY = 29
    ORDER BY
      HOUR,
      MINUTE
    LIMIT
      1
  );

-- which is New York City!
-- And the mystery has been solved!
-- The thief who stole the cs50 duck is Bruce
-- The accomplice of the thief is Robin
-- And the city where the thief is headed to is New York City
-- Our job here is done, the authorities will do the rest.