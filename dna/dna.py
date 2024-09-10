import csv
import sys

def main():
    if len(sys.argv) != 3:
        print("missing command-line argument!\nIntended usage: ./dna.py database.cvs sequence.txt")
        sys.exit(1)

    database = []
    dna_keys = []

    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        for person in reader:
            database.append(person)

    for key in database[0].keys():
        if key != "name":
            dna_keys.append(key)

    dna_sequence = ""
    with open(sys.argv[2]) as file:
        dna_sequence = file.read()

    longest_match_profile = dict()
    for subsequence in dna_keys:
        longest_match_profile[subsequence] = str(longest_match(dna_sequence, subsequence))

    for person in database:
        if perfect_match(person, longest_match_profile, dna_keys) == True:
            print(person["name"])
            break
    else:
        print("No match")


def perfect_match(person, longest_match_profile, dna_keys):
    for key in dna_keys:
        if person[key] != longest_match_profile[key]:
            return False
    return True


def longest_match(sequence, subsequence):
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    for i in range(sequence_length):
        count = 0

        while True:
            start = i + count * subsequence_length
            end = start + subsequence_length

            if sequence[start:end] == subsequence:
                count += 1
            else:
                break
                
        longest_run = max(longest_run, count)

    return longest_run


main()
