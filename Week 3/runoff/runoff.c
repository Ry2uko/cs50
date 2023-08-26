#include <cs50.h>
#include <stdio.h>
#include <strings.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
} candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    int candidate_index = -1;
    // check if name is name of candidate
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcasecmp(name, candidates[i].name) == 0)
        {
            candidate_index = i;
            break;
        }
    }

    // record preference if valid
    if (candidate_index >= 0)
    {
        preferences[voter][rank] = candidate_index;
        return true;
    }

    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // loop through every voter's preferences
    for (int i = 0; i < voter_count; i++)
    {
        // keep looping until voter's non-eliminated top preference is found
        int curr_index = 0;
        while (true)
        {
            int candidate_index = preferences[i][curr_index];
            if (candidates[candidate_index].eliminated)
            {
                curr_index++;
            }
            else
            {
                candidates[candidate_index].votes++;
                break;
            }
        }
    }

    // if eliminated check next preference
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    float half = voter_count / 2.0;

    // check every candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // has more than half the total votes
        if (candidates[i].votes > half)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }

    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min_votes = -1;

    // check every candidate's votes
    for (int i = 0; i < candidate_count; i++)
    {
        // if already eliminated
        if (candidates[i].eliminated)
        {
            continue;
        }

        if (candidates[i].votes < min_votes || min_votes < 0)
        {
            min_votes = candidates[i].votes;
        }
    }

    return min_votes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    int tie_votes = -1;

    // check every candidate votes that are not eliminated
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated)
        {
            continue;
        }

        if (tie_votes < 0 || tie_votes == candidates[i].votes)
        {
            tie_votes = candidates[i].votes;
        }
        else
        {
            return false;
        }
    }

    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // check every non-eliminated candidate and eliminate if votes is equal to min votes
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated)
        {
            continue;
        }

        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }

    return;
}