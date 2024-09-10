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

/* customized variables*/
int weight[MAX * (MAX - 1) / 2] = {0};

/* customized functions*/
int find_max_weight_index(void);
bool check_for_cycle(pair new_edge_added);
bool DFS_walk(int start_node_index, char colour[]);


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
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] ++;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int k = 0; //the pairs index.

    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[k].winner = i;
                pairs[k].loser = j;
                weight[k] = preferences[i][j] - preferences[j][i];
                pair_count++;
            }
            else if (preferences[j][i] > preferences[i][j])
            {
                pairs[k].winner = j;
                pairs[k].loser = i;
                weight[k] = preferences[j][i] - preferences[i][j];
                pair_count++;
            }
            //when tie, does not count that pair.
            k++;
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair pair_sorted[pair_count];
    for (int i = 0; i < pair_count; i++)
    {
        int index = find_max_weight_index();
        pair_sorted[i].winner = pairs[index].winner;
        pair_sorted[i].loser = pairs[index].loser;
    }

    for (int i = 0; i < pair_count; i++)
    {
        pairs[i].winner = pair_sorted[i].winner;
        pairs[i].loser = pair_sorted[i].loser;
        /* copies back sorted value to the original pairs */
    }
    return;
}

/* customized function */
int find_max_weight_index(void)
{
    int max_weight = weight[0];
    int max_weight_index = 0;
    for (int i = 0; i < pair_count; i++)
    {
        if (weight[i] > max_weight)
        {
            max_weight = weight[i];
            max_weight_index = i;
        }
    }

    weight[max_weight_index] = 0;
    return max_weight_index;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;

        if (check_for_cycle(pairs[i]))
        {
            //remove this pair if cycle is found.
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }

    return;
}

bool check_for_cycle(pair new_edge_added)
{
    /* white: 'w', gray: 'g', black: 'b', colour contains nodes as all candidates */
    char colour[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        colour[i] = 'w';
    }

    /* assumes the graph before inserting new edge does not contains a cycle, then only need to check the new tree containing the node where edge is added */

    int start_node_index = new_edge_added.winner;

    /* start DFS walk on this node */

    return DFS_walk(start_node_index, colour);
}

bool DFS_walk(int start_node_index, char colour[])
{
    colour[start_node_index] = 'g';

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[start_node_index][i] == true)
        {
            if (colour[i] == 'g')
            {
                return true; //cycle found
            }
            else if (colour[i] == 'w')
            {
                return DFS_walk(i, colour);
            }
            //else when black do nothing just return
        }
    }
    colour[start_node_index] = 'b';
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    /* initially everybudy wins, change index to 0 if we found it has an arrow pointing to it.*/
    int winner_index[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        winner_index[i] = 0;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        for(int j = 0; j < candidate_count; j++)
        {
            if (locked[i][j] == true)
            {
                winner_index[i] += 1;
            }
        }
    }

    int max = 0;
    for (int k = 0; k < candidate_count; k++)
    {
        if (winner_index[k] > max)
        {
            max = winner_index[k];
        }
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (winner_index[i] == max)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}
