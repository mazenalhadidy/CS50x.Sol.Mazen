-- Keep a log of any SQL queries you execute as you solve the mystery.
.table -- i want to see the important tables for the problem
.schema crime_scene_reports -- since the only information i have is about it
SELECT description FROM crime_scene_reports WHERE year = "2021" AND month LIKE "%7" AND day = "28" AND street = "Humphrey Street";
    -- (OK) the theft took place at 10:15 am in a bakery, so i also need bakery_security_logs table.
    -- (OK) interviews were performed with witnesses, so i also need interviews table.
.schema interviews -- let's see what they said before going into other information
SELECT name, transcript FROM interviews WHERE year = "2021" AND month LIKE "%7" AND day = "28";
    -- | Ruth    | (OK) Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
    -- | Eugene  | I don't know the thief's name, but it was someone I recognized. (OK)> Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
    -- | Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. <(OK) The thief then asked the person on the other end of the phone to purchase the flight ticket.
.schema bakery_security_logs -- let's extract information about Ruth's given data.
SELECT activity, license_plate, minute FROM bakery_security_logs WHERE year = "2021" AND month LIKE "%7" AND day = "28" AND hour = "10" AND minute BETWEEN 15 AND 25;
    -- Plate | Minute
    -- 5P2BI95  16
    -- 94KL13X  18
    -- 6P58WS2  18
    -- 4328GD8  19
    -- G412CB7  20
    -- L93JTIZ  21
    -- 322W7JE  23
    -- 0NTHK55  23
.schema atm_transactions -- let's extract information about Eugene's data
SELECT account_number, amount FROM atm_transactions WHERE year = "2021" AND month LIKE "%7" AND day = "28" AND atm_location = "Leggett Street";
    -- Acc. No. | Amount
    -- 28500762  48
    -- 28296815  20
    -- 76054385  60
    -- 49610011  50
    -- 16153065  80
    -- 86363979  10
    -- 25506511  20
    -- 81061156  30
    -- 26013199  35
.schema phone_calls
SELECT duration FROM phone_calls; -- to see the unit of time used and found it was "seconds".
SELECT caller, receiver FROM phone_calls WHERE year = "2021" AND month LIKE "%7" AND day = "28" AND duration BETWEEN 0 AND 60;
    --           caller | receiver
    -- | (130) 555-0289 | (996) 555-8899 |
    -- | (499) 555-9472 | (892) 555-8872 |
    -- | (367) 555-5533 | (375) 555-8161 |
    -- | (609) 555-5876 | (389) 555-5198 |
    -- | (499) 555-9472 | (717) 555-1342 |
    -- | (286) 555-6063 | (676) 555-6554 |
    -- | (770) 555-1861 | (725) 555-3243 |
    -- | (031) 555-6622 | (910) 555-3251 |
    -- | (826) 555-1652 | (066) 555-9701 |
    -- | (338) 555-6650 | (704) 555-2131 |
.schema flights -- to extract information from Raymond's data
.schema airports -- i found that it needs origin_airport_id so i need to get it first
SELECT id, abbreviation, full_name FROM airports WHERE city = "Fiftyville"; -- id was 8;
SELECT id, destination_airport_id FROM flights WHERE year = "2021" AND month LIKE "%7" AND day = "29" AND origin_airport_id = "8" ORDER BY hour, minute;
    -- earliest flight was to destination_airport_id = 4, and flight id was 36
SELECT abbreviation, full_name, city FROM airports WHERE id = "4";
    -- the theif went to LGA (LaGuardia Airport) New York City
.schema passengers -- to get passport_number of people on that flight
SELECT passport_number FROM passengers WHERE flight_id = "36";
    -- Passport Number
    -- | 7214083635 |
    -- | 1695452385 |
    -- | 5773159633 |
    -- | 1540955065 |
    -- | 8294398571 |
    -- | 1988161715 |
    -- | 9878712108 |
    -- | 8496433585 |
.schema bank_accounts
SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE year = "2021" AND month LIKE "%7" AND day = "28" AND atm_location = "Leggett Street");
    -- to find person_id of all bank clients who made the ATM transactions

SELECT id, name, phone_number, passport_number, license_plate FROM people WHERE id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE year = "2021" AND month LIKE "%7" AND day = "28" AND atm_location = "Leggett Street")) AND phone_number IN (SELECT caller FROM phone_calls WHERE year = "2021" AND month LIKE "%7" AND day = "28" AND duration BETWEEN 0 AND 60) AND passport_number IN (SELECT passport_number FROM passengers WHERE flight_id = "36") AND license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = "2021" AND month LIKE "%7" AND day = "28" AND hour = "10" AND minute BETWEEN 15 AND 25);
    -- this is a query to find the information of the person who fulfills the intersection of all the criteria presented above
    -- the thief's name was Bruce, id = 686048, phone_number is 5773159633 and license_plate is 94KL13X

SELECT id, name, phone_number FROM people WHERE phone_number IN (SELECT receiver FROM phone_calls WHERE year = "2021" AND month LIKE "%7" AND day = "28" AND duration BETWEEN 0 AND 60 AND caller IN (SELECT phone_number FROM people WHERE id = "686048"));
    -- this is a query to find the information about the thief's accomplice by bringing the receiver's phone number to the call conducted by the number of the thief.
    -- accomplice id is 863300 and his name is Robin