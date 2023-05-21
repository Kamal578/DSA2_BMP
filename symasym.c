#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "image.c"

int main(int argc, char** argv){
    /*
    Potential command line arguments:
        -o <output file name>   
        -size <size of the image>
        -h <horizontal symmetry>
        -v <vertical symmetry>
        -a <assymentric image>
        -s <symmetric image>
        -n <number of images to be generated>
        -seed <seed for random number generator>
    */

    char szOutputFilename[200] = "";
    int nSize = DEFAULT_SIZE;
    int nThickness = DEFAULT_THICKNESS;
    EDirectionsSymmetry nDirectionSymmetry = SH;
    int nNumberOfImages = 1;
    int nSeed = (int)time(NULL); 
    bool bSpecifySeed = false;
    char szSeed[100];
    
    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i], "-o") == 0){
            strcpy(szOutputFilename, argv[i+1]);
            strcat(szOutputFilename, ".bmp");
        }
        else if (strcmp(argv[i], "-size") == 0){
            nSize = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "-n") == 0){
            nNumberOfImages = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "-seed") == 0){
            bSpecifySeed = true;
            nSeed = atoi(argv[i+1]);
            strcpy(szSeed, argv[i+1]);
        }
        else if (strcmp(argv[i], "-ah") == 0){
            nDirectionSymmetry = AH;
        }
        else if (strcmp(argv[i], "-av") == 0){
            nDirectionSymmetry = AV;
        }
        else if (strcmp(argv[i], "-sh") == 0){
            nDirectionSymmetry = SH;
        }
        else if (strcmp(argv[i], "-sv") == 0){
            nDirectionSymmetry = SV;
        }
    }
    

    // Generating n images
    for (int i = 0; i < nNumberOfImages; i++){
        // if seed is not specified, the seed is the current time
        if (!bSpecifySeed){
            // Generate a random seed
            nSeed = (int)time(NULL) + i;
        }
        else{
            nSeed = atoi(szSeed) + i;
            if (nSeed % 4 == 0) nDirectionSymmetry = SV;
            else if (nSeed % 4 == 1) nDirectionSymmetry = AV;
            else if (nSeed % 4 == 2) nDirectionSymmetry = SH;
            else if (nSeed % 4 == 3) nDirectionSymmetry = AH;
        }

        srand(nSeed);

        char szOutputSuffix[10];

        switch (nDirectionSymmetry){
            case SV:
                strcpy(szOutputSuffix, "sv");
                break;
            case AV:
                strcpy(szOutputSuffix, "av");
                break;
            case SH:
                strcpy(szOutputSuffix, "sh");
                break;
            case AH:
                strcpy(szOutputSuffix, "ah");
                break;
        }

        char szOutputFilenameWithSuffix[200];
        strcpy(szOutputFilenameWithSuffix, szOutputFilename);
        strcat(szOutputFilenameWithSuffix, szOutputSuffix);
        // add seed to the filename
        char szSeed[100];
        sprintf(szSeed, "%d", nSeed);
        strcat(szOutputFilenameWithSuffix, szSeed);
        strcat(szOutputFilenameWithSuffix, ".bmp");

        // Generating the image
        // Initialize the image with SImageHeader SImageInit(int nWidth, int nHeight, int nBitsPerPixel)

        SImageHeader sImageHeader = SImageInit(nSize, nSize, 3);

        // Generate pixels with int* ppCreatePixels(int nSize, int nThickness, EDirectionsSymmetry nDirectionSymmetry);

        int* pPixels = ppCreatePixels(nSize, nThickness, nDirectionSymmetry);

        // SImage createImage(int nWidth, int nHeight, int nBitsPerPixel, int* pPixels, EDirectionsSymmetry nDirectionSymmetry, int nThickness);

        SImage sImage = createImage(nSize, nSize, 3, pPixels, nDirectionSymmetry, nThickness);

        // Write the image to a file with void saveImage(SImage image, char* szFileName)

        saveImage(sImage, szOutputFilenameWithSuffix);

        // free the memory

        free(pPixels);
        free(sImage.pPixels);

        
    }

}