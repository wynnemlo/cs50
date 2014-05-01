/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <stdint.h>

// define type BYTE
typedef uint8_t BYTE;

// prototype
bool jpg_or_not(BYTE buffer[]);

int main(int argc, char* argv[])
{
    // open card file
    FILE* file = fopen("card.raw", "r");
    
    // declare variables
    BYTE buffer[512];
    int filename = 0;
    char title[8];
    bool jpg_open = false;
    FILE* img = fopen("000.jpg", "w");
    
    // keep reading 512 bytes into buffer until end of card
    while (fread(&buffer, 1, 512, file) == 512)
    {
        // check if start of new jpg
        if (jpg_or_not(buffer))
        {
            if (jpg_open)
            {
                fclose(img);
            }
            // print jpg name
            sprintf(title, "%03i.jpg", filename);
            filename++;
            
            // create jpg file
            FILE* img = fopen(title, "w");
            
            // write 512 bytes into new jpg file
            fwrite(&buffer, 1, 512, img);
            
            // indicate jpg file is open
            jpg_open = true;
        }
        else
        {
            // if a jpg is already open
            if (jpg_open)
            {
                // write 512 bytes into new jpg file
                fwrite(&buffer, 1, 512, img);
            }
        }
    }
     
    // close card file
    fclose(file);
    
    // that's all folks
    return 0;
}

// checks if the buffer is a start of a jpg
bool jpg_or_not(BYTE buffer[])
{
    if ((buffer[0] == 0xff &&
         buffer[1] == 0xd8 &&
         buffer[2] == 0xff &&
         buffer[3] == 0xe0) ||
        (buffer[0] == 0xff &&
         buffer[1] == 0xd8 &&
         buffer[2] == 0xff &&
         buffer[3] == 0xe1))
    {
        return true;
    }
    else
    {
       return false;
    }
}
