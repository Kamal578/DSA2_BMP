#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define u32 uint32_t
#define u16 uint16_t

#define BMP_HEADER_SIZE 54
#define DIB_HEADER_SIZE 40

#define BLACK 0
#define WHITE 255

#define DEFAULT_SIZE 32
#define DEFAULT_THICKNESS 3

typedef enum {
    AH = 0, // ASSYMETRIC HORIZONTAL
    SH = 1, // SYMMETRIC HORIZONTAL
    AV = 2, // ASSYMETRIC VERTICAL
    SV = 3  // SYMMETRIC VERTICAL
} EDirectionsSymmetry;

typedef enum {false, true} bool;

#pragma pack(push, 1) // exact fit - no padding
typedef struct{
    // BMP Header
    char szType[2]; // "BM"
    u32 nSizeOfFile; // size of file in bytes
    u32 nReserverForApp; // reserved for application. always 0
    u32 nStartOfImage; // offset to start of image data in bytes

    // DIB Header
    u32 nSizeOfHeader; // size of DIB header in bytes
    u32 nWidth; // width of image in pixels
    u32 nHeight; // height of image in pixels
    u16 nNumberOfColorPlanes; // number of color planes. always 1
    u16 nNumberOfBitsPerPixels; // number of bits per pixel

    // Additional information (not necessary for current needs)
    u32 nCompression; // compression method being used (0 = none)
    u32 nSizeOfData; // size of the raw bitmap data in bytes (including padding)
    u32 nResolutionHorizontal; // horizontal resolution of the image (pixel per meter)
    u32 nResolutionVertical; // vertical resolution of the image (pixel per meter)
    u32 nColorsInPalette; // number of colors in the palette 
    u32 nImportantColors; // number of important colors used (0 = all)
} SImageHeader;
#pragma pack(pop) // back to whatever the previous packing mode was

#pragma pack(push, 1) // exact fit - no padding
typedef struct{
    unsigned char* pPixels;
    SImageHeader sHeader;
} SImage;

// Function prototypes
SImageHeader SImageInit(int nWidth, int nHeight, int nBitsPerPixel);
int* ppCreatePixels(int nSize, int nThickness, EDirectionsSymmetry nDirectionSymmetry);
SImage createImage(int nWidth, int nHeight, int nBitsPerPixel, int* pPixels, 
EDirectionsSymmetry nDirectionSymmetry, int nThickness);
void saveImage(SImage image, char* szFileName);

#endif