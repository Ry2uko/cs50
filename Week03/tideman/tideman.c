#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // get candidate_index and check if valid
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            // update ranks
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{

    // update preferences
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = 1; j < candidate_count; j++)
        {
            if (i == j)
            {
                continue;
            }
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i == j)
            {
                continue;
            }

            // ith candidate has more votes than jth candidate
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count += 1;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // get each pair's strength of victory
    int pair_strengths[pair_count];

    for (int i = 0; i < pair_count; i++)
    {
        int pair_winner = pairs[i].winner;
        int pair_loser = pairs[i].loser;

        for (int j = 0; j < candidate_count; j++)
        {
            if (j == pair_winner)
            {
                int winner_votes = preferences[pair_winner][pair_loser];
                int loser_votes = preferences[pair_loser][pair_winner];
                pair_strengths[i] = winner_votes - loser_votes;
            }
        }
    }

    // Bubble sort pairs by strength
    int r_pointer = pair_count - 2;
    int l_pointer = 0;
    int swapped = 0;

    while (true)
    {
        if (l_pointer > r_pointer)
        {
            if (swapped == 0)
            {
                break;
            }
            swapped = 0;
            l_pointer = 0;
        }

        pair curr_pair = pairs[l_pointer];
        int curr_strength = pair_strengths[l_pointer];
        int next_strength = pair_strengths[l_pointer + 1];

        // Decreasing order (<), Increasing order (>)
        if (curr_strength < next_strength)
        {
            // update both pair_strengths and pairs
            pair_strengths[l_pointer] = next_strength;
            pair_strengths[l_pointer + 1] = curr_strength;

            pairs[l_pointer] = pairs[l_pointer + 1];
            pairs[l_pointer + 1] = curr_pair;

            swapped = 1;
            if (l_pointer == r_pointer)
            {
                r_pointer -= 1;
            }
        }

        l_pointer += 1;
    }

    return;
}

// https://stackoverflow.com/a/63015680
// Recursive function to check if entry makes a circle
bool makes_circle(int cycle_start, int loser)
{
    if (loser == cycle_start)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[loser][i])
        {
            if (makes_circle(cycle_start, i))
            {
                return true;
            }
        }
    }

    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!makes_circle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

// Print the winner of the election
void print_winner(void)
{
    // determine winner (not in loser in pair)
    for (int i = 0; i < candidate_count; i++)
    {
        int found = 0;

        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                found = 1;
                break;
            }
        }

        if (found)
        {
            continue;
        }

        printf("%s\n", candidates[i]);
        break;
    }

    return;
}