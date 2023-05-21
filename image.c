#include "image.h"

SImageHeader SImageInit(int nWidth, int nHeight, int nBitsPerPixel){
    /*
    *   Initialize the header of the image
    *   Input: nSize - size of the image
    *   Output: header - the header of the image
    */
    SImageHeader iHeader;
    iHeader.szType[0] = 'B';
    iHeader.szType[1] = 'M';
    iHeader.nSizeOfFile = BMP_HEADER_SIZE + nWidth * nHeight * nBitsPerPixel; // Size of the file in bytes
    iHeader.nReserverForApp = 0; // Reserved for application. Always 0 for our needs
    iHeader.nStartOfImage = BMP_HEADER_SIZE; // Offset to start of image data in bytes. 54 is the size of the header
    iHeader.nSizeOfHeader = DIB_HEADER_SIZE; // Size of DIB header in bytes
    iHeader.nWidth = nWidth; // Width of image in pixelss (square)
    iHeader.nHeight = nHeight; // Height of image in pixelss (square)
    iHeader.nNumberOfColorPlanes = 1; // Number of color planes. Always 1
    iHeader.nNumberOfBitsPerPixels = 8*nBitsPerPixel; // Number of bits per pixel
    iHeader.nCompression = 0; // Compression method being used (0 = none)
    iHeader.nSizeOfData = nWidth * nHeight * nBitsPerPixel; // Size of the raw bitmap data in bytes (including padding). 3 bytes per pixels (RGB)
    iHeader.nResolutionHorizontal = 0; // Horizontal resolution of the image (pixels per meter). Not necessary for current needs
    iHeader.nResolutionVertical = 0; // Vertical resolution of the image (pixels per meter). Not necessary for current needs
    iHeader.nColorsInPalette = 0; // Number of colors in the palette. Not necessary for current needs
    iHeader.nImportantColors = 0; // Number of important colors used (0 = all). Not necessary for current needs
    return iHeader;
}

int* ppCreatePixels(int nSize, int nThickness, EDirectionsSymmetry nDirectionSymmetry){
    /*
    *   Create the pixels of the image
    *   Input:  nSize - size of the image
    *           nThickness - thickness of the line
    *           nDirectionSymmetry - the direction of the symmetry
    *   Output: pPixels - the pixels of the image
    */

    int *pPixels = (int *)malloc(nSize * 2 * sizeof(int)); // 2 * nSize because we need to store the pixels of the other half of the image
    if (pPixels == NULL){ // check if memory allocation succeeded
        printf("Error allocating memory for pixels\n");
        exit(1);
    }
    int nStartPoint = rand() % nSize; // random start point
    pPixels[0] = nStartPoint; // set the first pixel
    for (int i = 1; i < nSize; i++){ // create the pixels
        int nLast = pPixels[i-1];
        int nNext = nLast - nThickness + rand() % (2 * nThickness + 1);
        if (nNext < 0) nNext = 0;
        else if (nNext >= nSize)nNext = nSize - 1;
        pPixels[i] = nNext;
    }
    if (nDirectionSymmetry == SH || nDirectionSymmetry == SV){ // symmetrical
        // copy the pixels to the other half of the image
        for (int i = 0; i < nSize / 2; i++) pPixels[nSize - i - 1] = pPixels[i]; 
    }
    return pPixels;
}

SImage createImage(int nWidth, int nHeight, int nBitsPerPixel, int* pPixels, EDirectionsSymmetry nDirectionSymmetry, int nThickness){
    /*
    *   Create the image
    *   Input:  nWidth - width of the image
    *           nHeight - height of the image
    *           nBitsPerPixel - bits per pixel
    *           pPixels - the pixels of the image
    *           nDirectionSymmetry - the direction of the symmetry
    *   Output: image - the image
    */
    SImage image;
    image.sHeader = SImageInit(nWidth, nHeight, nBitsPerPixel); // initialize the header
    image.pPixels = (unsigned char *)malloc(image.sHeader.nSizeOfData); // allocate memory for the pixels
    if (image.pPixels == NULL){ // check if memory allocation succeeded
        printf("Error allocating memory for pixels\n");
        exit(1);
    }

    // getting direction

    bool bIsHorizontal = (nDirectionSymmetry == AH || nDirectionSymmetry == SH); // horizontal
    bool bIsVertical = (nDirectionSymmetry == AV || nDirectionSymmetry == SV); // vertical

    // Fill image with white 
    for (int i = 0; i < image.sHeader.nSizeOfData; i++) image.pPixels[i] = WHITE;

    // Drawing the line
    // unnecessary for current needs, since we are generating square images
    int nEnd = (nWidth == nHeight) ? nWidth : nHeight; // the end of the line

    for (int i = 0; i < nEnd; i++){
        for (int j = 0; j < nThickness; j++){

            int nChecker =  ((pPixels[i] + j) >= nEnd) ? nEnd-1 : pPixels[i] + j;
            int nXaxis =  (bIsHorizontal) ? i : nChecker;
            int nYaxis =  (bIsVertical) ? i : nChecker;
            int nIndex = nYaxis * nEnd * nBitsPerPixel + nXaxis * nBitsPerPixel; // index of the pixel

            // set the pixel to black
            image.pPixels[nIndex] = BLACK;
            image.pPixels[nIndex + 1] = BLACK;
            image.pPixels[nIndex + 2] = BLACK;
        }
    }
    return image;
}

void saveImage(SImage image, char* szFileName){
    /*
    *   Save the image
    *   Input:  image - the image
    *           szFileName - the name of the file
    *   Output: -
    */
    FILE *fp = fopen(szFileName, "wb"); // open the file
    if (fp == NULL){ // check if file opening succeeded
        printf("Error opening file\n");
        exit(1);
    }
    fwrite(&image.sHeader, sizeof(SImageHeader), 1, fp); // write the header
    fwrite(image.pPixels, sizeof(unsigned char), image.sHeader.nSizeOfData, fp); // write the pixels
    fclose(fp); // close the file
}
