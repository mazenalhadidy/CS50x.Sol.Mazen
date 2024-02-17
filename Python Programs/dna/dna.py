import csv
import sys


def main():
    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Wrong Number of Entries.")
        return

    # TODO: Read database file into a variable
    file = open(sys.argv[1], "r")
    reader = csv.DictReader(file)
    database = []
    count = 0
    for row in reader:
        database.append({})
        for num in range(len(row.keys())):
            key = list(row.keys())
            key = key[num]
            database[count][key] = row[key]
        count = count + 1

    file.close()

    # TODO: Read DNA sequence file into a variable
    dnafile = open(sys.argv[2], "r")
    dnatxt = dnafile.read()
    dnafile.close()

    # TODO: Find longest match of each STR in DNA sequence
    dataBkeys = list(database[0].keys())
    strN = dataBkeys[1:]
    repeats = {}
    for strr in strN:
        repeats[strr] = longest_match(dnatxt, strr)

    # TODO: Check database for matching profiles
    for person in database:
        num = 0
        check = 0
        for num in range(len(repeats)):
            if repeats[dataBkeys[num + 1]] != int(person[dataBkeys[num + 1]]):
                check = 1
                break
        if check == 0 and num == len(repeats) - 1:
            print(person["name"])
            return

    print("No Match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):
        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:
            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
