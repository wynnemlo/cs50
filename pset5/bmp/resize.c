/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Takes a 24-bit uncompressed BMP and resizes it by a factor of n.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    int n = atoi(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];
        
    // ensure that n is positive and less than or equal to one hundred
    if (n > 100 && n < 1)
    {
        printf("Please provide an integer between 1 and 100.");
        return 1;
    }

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // save old padding info
    int oldPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // update width and height of output header file
    LONG originalWidth = bi.biWidth;
    LONG originalHeight = bi.biHeight;
    bi.biWidth = bi.biWidth * n;
    bi.biHeight = bi.biHeight * n;
    
    // determine new padding for scanlines
    int newPadding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int paddingSize = newPadding * (abs(bi.biHeight));
    
    // update image size of output header file
    bf.bfSize = (((bi.biWidth * abs(bi.biHeight)) * 3) + paddingSize + 54);
    bi.biSizeImage = ((bi.biWidth * abs(bi.biHeight)) * 3) + paddingSize;
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    int counter = 0;
    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(originalHeight); i < biHeight; i++)
    {   
        do
        {
            // iterate over pixels in scanline
            for (int j = 0; j < originalWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                //printf("just read: %02x %02x %02x\n", triple.rgbtBlue, triple.rgbtGreen, triple.rgbtRed);

                // write RGB triple to outfile n times
                for (int k = 0; k < n; k++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    //printf("just wrote in %02x %02x %02x\n", triple.rgbtBlue, triple.rgbtGreen, triple.rgbtRed);
                }
            }
            // skip over padding, if any
            fseek(inptr, oldPadding, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < newPadding; k++)
            {
                fputc(0x00, outptr);
            }
            
            // if there are more vertical lines to repeat
            if (counter + 1 < n)
            {
                //printf("moving pointer back %i\n", ((-originalWidth * 3) - oldPadding));
                fseek(inptr, ((-originalWidth * 3) - oldPadding), SEEK_CUR);
            }
            counter++;
        }
        while (counter < n);
        counter = 0;
        // bf size = 126
        // bi size = 40
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
