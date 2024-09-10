#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }

    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    char cipherLOW[26];
    char cipherUP[26];

    for (int i = 0; i < 26; i++)
    {
        if (!isalpha((argv[1][i])))
        {
            printf("Key must contain 26 characters that are alphabetical.\n");
            return 1;
        }

        /* check for duplicate keys*/
        for (int j = 0; j < i; j++)
        {
            if (argv[1][i] == argv[1][j])
            {
                printf("Key must cantain 26 distinct characters\n");
                return 1;
            }
        }

        cipherUP[i] = toupper(argv[1][i]);
        cipherLOW[i] = tolower(argv[1][i]);
    }
    string plaintext = get_string("plaintext: ");

    int n = strlen(plaintext);
    char ciphertext[n];

    for (int i = 0; i < n; i++)
    {
        char c = plaintext[i];
        if (isupper(c))
        {
            ciphertext[i] = cipherUP[c - 'A'];
        }
        else if (islower(c))
        {
            ciphertext[i] = cipherLOW[c - 'a'];
        }
        else
        {
            ciphertext[i] = c;
        }
    }

    printf("ciphertext: ");
    for (int i = 0; i < n; i++)
    {
        printf("%c", ciphertext[i]);
    }
    printf("\n");

    return 0;
}
