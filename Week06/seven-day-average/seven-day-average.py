import csv
import requests


def main():
    # Read NYTimes Covid Database
    download = requests.get(
        "https://raw.githubusercontent.com/nytimes/covid-19-data/master/us-states.csv"
    )
    decoded_content = download.content.decode("utf-8")
    file = decoded_content.splitlines()
    reader = csv.DictReader(file)

    # Construct 14 day lists of new cases for each states
    new_cases = calculate(reader)

    # Create a list to store selected states
    states = []
    print("Choose one or more states to view average COVID cases.")
    print("Press enter when done.\n")

    while True:
        state = input("State: ")
        if state in new_cases:
            states.append(state)
        if len(state) == 0:
            break

    print(f"\nSeven-Day Averages")

    # Print out 7-day averages for this week vs last week
    comparative_averages(new_cases, states)


# TODO: Create a dictionary to store 14 most recent days of new cases by state
def calculate(reader):
    new_cases = {}
    prev_cases = {}

    for row in reader:
        state, cases = row["state"], int(row["cases"])

        if state not in new_cases:
            new_cases[state] = []

        if state not in prev_cases:
            prev_cases[state] = cases
        else:
            curr_cases = cases - prev_cases[state]
            if len(new_cases[state]) >= 14:
                new_cases[state].pop()

            new_cases[state].append(curr_cases)
            prev_cases[state] = cases

    return new_cases


# TODO: Calculate and print out seven day average for given state
def comparative_averages(new_cases, states):
    for state in states:
        week_1 = round(sum(new_cases[state][:7]) / 7)
        week_2 = round(sum(new_cases[state][7:]) / 7)
        try:
            percentage = ((week_2 - week_1) / week_1) * 100
        except ZeroDivisionError:
            percentage = 0

        if percentage >= 0:
            print(
                f"{state} had a 7-day average of {week_2} and a increase of {percentage:.1f}%."
            )
        else:
            print(
                f"{state} had a 7-day average of {week_2} and a decrease of {percentage:.1f}%."
            )

    return


main()
