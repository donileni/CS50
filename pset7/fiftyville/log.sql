-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Starting by checking the crime description based on the date and location of the crime.
SELECT description
FROM crime_scene_reports
WHERE year = 2021
AND month = 7
AND day = 28
AND street = "Humphrey Street";
-- Based on the description, we get a time (10:15) and a mention of the bakey.

-- Before proceeding, we need some more infromation. So, let us take a clsoser look at the interviews from the same day.
SELECT transcript
FROM interviews
WHERE year = 2021
AND month = 7
AND day = 28;
-- This reveals a bunch of things: 1) the theif left the bakery parking lot after the crime, 2) the theif took out money from an ATM on Legget Street before the crime, 3) they called someone
-- and talked for less than one minute, and 5) they planned on leaving Fiftyvill the day after the crime took place.

-- So,let us look at the bakery logs between 10:10 and 10:20 to see what cars based on their license plates that left the bakery during this interval.
SELECT activity,license_plate
FROM bakery_security_logs
WHERE year = 2021
AND month = 7
AND day = 28
AND hour = 10
AND minute BETWEEN 10 and 20
AND activity = "exit";
-- Running the command above privided five plates, which is our current suspects: 5P2BI95, 94KL13X, 6P58WS2, 4328GD8, G412CB7.

-- Next, we look up who is the owner of the plates.
SELECT DISTINCT(name)
FROM people
JOIN bakery_security_logs
ON bakery_security_logs.license_plate = people.license_plate
WHERE people.license_plate
IN (SELECT license_plate
FROM bakery_security_logs
WHERE year = 2021
AND month = 7
AND day = 28
AND hour = 10
AND minute BETWEEN 10 and 20
AND activity = "exit");
-- Running this gives us the name of our suspects: Vanessa, Bruce, Barry, Luca, Sofia.

-- Let us see if any of these people left town. To begin with, lets get the airport ID and name of Fiftyville's airport.
SELECT full_name, id
FROM airports
WHERE city = "Fiftyville";
-- Gives us name: "Fiftyville Regional Airport", id: "8".

-- Next, let us get a list of all the IDs and the destination airports' IDs of all the flights leaving the Fiftyville Regional Airport the day after the crime. Let us also sort by time, to get the
-- earliest flight that day.
SELECT id, destination_airport_id
FROM flights
WHERE origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville")
AND year = 2021
AND month = 7
AND day = 29
ORDER BY hour ASC, minute ASC;
-- This gives us a list of 5 filights: 36(4), 43(1), 23(11), 53(9), and 18(4), where the earliest flight has the id 36 and the destination id = 4. As such, let us asume that our suspect is on this flight

-- Let us check if any of our suspects were on this flights by checking if they are passengers.
SELECT name
FROM people
JOIN passengers
ON passengers.passport_number = people.passport_number
JOIN flights
ON flights.id = passengers.flight_id
WHERE flights.id = 36
AND people.name IN ("Vanessa", "Bruce", "Barry", "Luca", "Sofia");
-- This narrows down or suspects to Sofia, Bruce, and Luca.

-- Let us also get the city name based on the destination ID so we can see where the suspect fled to.
SELECT city
FROM airports
WHERE id = 4;
-- This gives us the answer New York City, meaning that our suspect fled to this city.

-- Let us now see if any of our suspects withdrew money from the ATM in question.
SELECT name
FROM people
JOIN bank_accounts
ON bank_accounts.person_id = people.id
JOIN atm_transactions
ON atm_transactions.account_number = bank_accounts.account_number
WHERE atm_transactions.year = 2021
AND atm_transactions.month = 7
AND atm_transactions.day = 28
AND people.name IN ("Sofia", "Bruce", "Luca")
AND atm_transactions.transaction_type = "withdraw"
AND atm_transactions.atm_location = "Leggett Street";
-- This leaves us with two suspects: Bruce and Luca

-- Lastly, the last clue is the phone call. So, let us chekc if either of our suspects made a phone call before the day with a duration less than 60 seconds.
SELECT name
FROM people
JOIN phone_calls
ON phone_calls.caller = people.phone_number
WHERE phone_calls.year = 2021
AND phone_calls.month = 7
AND phone_calls.day = 28
AND phone_calls.duration < 60
AND people.name IN ("Bruce", "Luca");
-- This leaves us with Bruce, which is our final suspect.

-- Lastyl, let us check the name of the person to whom Bruce called to get the accomplice.
SELECT name
FROM people
JOIN phone_calls
ON phone_calls.receiver = people.phone_number
WHERE phone_calls.caller = (SELECT people.phone_number FROM people WHERE name = "Bruce")
AND phone_calls.year = 2021
AND phone_calls.month = 7
AND phone_calls.day = 28
AND phone_calls.duration < 60;
-- The receiver of the phone call is Robin and therefore the accomplice. 