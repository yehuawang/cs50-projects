#include <cs50.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;
#define FAT_SIZE 512

bool isJPEGheader(BYTE *buffer);
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Unable to open file\n");
        return 1;
    }

    uint8_t buffer[FAT_SIZE];
    int image_count = 0;
    FILE *img = NULL;

    while (fread(buffer, 1, FAT_SIZE, card) == FAT_SIZE)
    {
        if (isJPEGheader(buffer))
        {
            if (image_count > 0)
            {
                fclose(img);
            }
            char file_name[8];
            sprintf(file_name, "%03i.jpg", image_count);
            img = fopen(file_name, "w");
            image_count++;
        }
        if (image_count > 0)
        {
            fwrite(buffer, 1, FAT_SIZE, img);
        }
    }
    if (image_count > 0)
    {
        fclose(img);
    }
    fclose(card);
}

bool isJPEGheader(BYTE *buffer)
{
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
    {
        return true;
    }
    return false;
}
