#include "hw2.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h> 

bool processCInputs(long *cParameters, char *cTxt) {
    char *token = strtok(cTxt, ",");
    int index = 0;
    cParameters[index++] = strtol(token, NULL, 10);

    while((token = strtok(NULL, ",")) != NULL && index < 4) {
        cParameters[index++] = strtol(token, NULL, 10);
    }

    if(index != 4 || token != NULL) {
        return true;
    }
    return false;
}

bool processPInputs(long *pParameters, char *pTxt) {
    char *token = strtok(pTxt, ",");
    int index = 0;

    pParameters[index++] = strtol(token, NULL, 10);

    while((token = strtok(NULL, ",")) != NULL && index < 2) {
        pParameters[index++] = strtol(token, NULL, 10);
    }

    if(index != 2 || token != NULL) {
        return true;
    }

    return false;
}

bool processRInputs(char *rMessage, char *rPath_to_front, long rFont_size, long rRow, long rCol, char *rTxt) {
    char *token;
    rMessage = strtok(rTxt, ",");

    if((token = strtok(NULL, ",")) == NULL) {
        return true;
    }
    rPath_to_front = token;
    
    if((token = strtok(NULL, ",")) == NULL) {
        return true;
    }
    rFont_size = strtol(token, NULL, 10);

    if((token = strtok(NULL, ",")) == NULL) {
        return true;
    }
    rRow = strtol(token, NULL, 10);

    if((token = strtok(NULL, ",")) == NULL) {
        return true;
    }
    rCol = strtol(token, NULL, 10);

    if((token = strtok(NULL, ",")) != NULL) {
        return true;
    }
    (void) rMessage;
    (void) rPath_to_front;
    (void) rFont_size;
    (void) rRow;
    (void) rCol;
    return false;
}


void getCopiedRegionP3(int **copiedRegion, int imageWidth, long *cParameters, FILE *fpIn) {
    int r, g, b;
    int colIndex = 0, rowIndex = 0; //index for copiedRegion
    int row = cParameters[0], col = cParameters[1], width = cParameters[2], height = cParameters[3];
    int upperR = row + width - 1, upperC = col + height - 1; //upper bound index for row & col
    int currR = 0, currC = 0; //current index of the image
    while(fscanf(fpIn, "%d %d %d", &r, &g, &b) == 3) {
        if(currR >= row  && currR <= upperR && currC >= col && currC <= upperC) { //the pixel falls within the copy region
            copiedRegion[rowIndex][colIndex] = r;
            copiedRegion[rowIndex][colIndex+1] = g;
            copiedRegion[rowIndex][colIndex+2] = b;
            colIndex += 3;
            if(colIndex > imageWidth * 3) {
                colIndex = 0;
                rowIndex++;
            }
        }
        
        if(currR >= upperR && currC > upperC) { //we passed all pixels in the copy region
            break;
        }

        if(currC >= imageWidth) { //we reached the last pixel of that row
            currC = 0;
            currR++;
        } else {
            currC++;
        }
    }
}


int **createCopiedRegionArr(int imageWidth, int imageHeight, char fileTypeIn, long *cParameters, FILE *fpIn) {
    int **copiedRegion = (int **)malloc(imageHeight * sizeof(int));
    for(int i = 0; i < imageHeight; i++) {
        copiedRegion[i] = (int *)malloc(imageWidth * 3 * sizeof(int));
    }

    if(fileTypeIn == 'P') {
        //call function to copy from P3
        fscanf(fpIn, "%*[^\n]\n"); //skip the line 255
        getCopiedRegionP3(copiedRegion, imageWidth, cParameters, fpIn);
    } else {
        //call function to copy from SBU

    }

    return copiedRegion;
}


void ppmToArr(int **imageArr, int imageWidth, FILE *fpIn){
    int r, g, b;
    int colIndex = 0, rowIndex = 0; //index for imageArr
    while(fscanf(fpIn, "%d %d %d", &r, &g, &b) == 3) {
        imageArr[rowIndex][colIndex] = r;
        imageArr[rowIndex][colIndex+1] = g;
        imageArr[rowIndex][colIndex+2] = b;
        colIndex += 3;
        if(colIndex >= imageWidth * 3) { //reached the end of a row
            colIndex = 0;
            rowIndex++;
        }
    }
}


void sbuToArr(int **imageArr, int imageWidth, FILE *fpIn) {
    int numOfColor, r, g, b;
    fscanf(fpIn, "%d", &numOfColor);
    int colorTable[numOfColor * 3];

    for(int i = 0; i < numOfColor * 3; i += 3) {
        fscanf(fpIn, "%d %d %d", &r, &g, &b);
        colorTable[i] = r;
        colorTable[i+1] = g;
        colorTable[i+2] = b;
    }

    char star = '-';
    int num1, num2, i;
    int colIndex = 0, rowIndex = 0; //index for imageArr
    while(fscanf(fpIn, "%d", &num1) == 1 || fscanf(fpIn, "%c%d", &star, &num1) == 2) {
        if(star == '*') { //if there's a star then read the num next to that
            fscanf(fpIn, "%d", &num2);
            for(i = 0; i < num1; i++) {
                imageArr[rowIndex][colIndex] = colorTable[num2 * 3];
                imageArr[rowIndex][colIndex+1] = colorTable[num2 * 3 + 1];
                imageArr[rowIndex][colIndex+2] = colorTable[num2 * 3 + 2];
                colIndex += 3;
                if(colIndex >= imageWidth * 3) { //reached the end of a row
                    colIndex = 0;
                    rowIndex++;
                }
            }
        } else {
            imageArr[rowIndex][colIndex] = colorTable[num1 * 3];
            imageArr[rowIndex][colIndex+1] = colorTable[num1 * 3 + 1];
            imageArr[rowIndex][colIndex+2] = colorTable[num1 * 3 + 2];
            colIndex += 3;
            if(colIndex >= imageWidth * 3) { //reached the end of a row
                colIndex = 0;
                rowIndex++;
            }
        }
        star = '-';
    }
}


void arrToPPM(int **imageArr, int imageWidth, int imageHeight, FILE *fpOut) {
    fprintf(fpOut, "P3\n");
    fprintf(fpOut, "%d %d\n", imageWidth, imageHeight);
    fprintf(fpOut, "255\n");
    for(int i = 0; i < imageHeight; i++) {
        for(int j = 0; j < imageWidth * 3; j++) {
            fprintf(fpOut, "%d ", imageArr[i][j]);
        }
    }
}


void arrToSBU(int **imageArr, int imageWidth, int imageHeight, FILE *fpOut) {
    fprintf(fpOut, "SBU\n");
    fprintf(fpOut, "%d %d\n", imageWidth, imageHeight);

    //create color table
    int colorCount = 0, r = 0, g = 0, b = 0, LIA = 0;
    bool unique;
    int colorTable[imageWidth * imageHeight * 3];
    int linearImageArr[imageWidth * imageHeight * 3];
    for(int i = 0; i < imageHeight; i++) {
        for(int j = 0; j < imageWidth * 3; j += 3) {
            unique = true;
            imageArr[i][j] = r;
            imageArr[i][j+1] = g;
            imageArr[i][j+2] = b;
            linearImageArr[LIA] = r; LIA++;
            linearImageArr[LIA] = g; LIA++;
            linearImageArr[LIA] = b; LIA++;
            for(int k = 0; k < colorCount; k++) {
                if(colorTable[k*3] == r && colorTable[k*3+1] == g && colorTable[k*3+2] == b) {
                    unique = false;
                    break;
                }
            }
            if(unique) {
                colorTable[colorCount * 3] = r;
                colorTable[colorCount * 3 + 1] = g;
                colorTable[colorCount * 3 + 2] = b;
                colorCount++;
            }
        }
    }

    //print color table
    fprintf(fpOut, "%d", colorCount);
    for(int i = 0, k = 0; i < colorCount; i++) {
        fprintf(fpOut, " %d %d %d", colorTable[k], colorTable[k+1], colorTable[k+2]);
        k += 3;
    }
    fprintf(fpOut, "\n");

    //print pixels
    int duplCount;
    for(int i = 0; i < LIA; i+=3) {
        duplCount = 0;
        linearImageArr[i] = r;
        linearImageArr[i+1] = g;
        linearImageArr[i+2] = b;
        i += 3;
        while(i < LIA && linearImageArr[i] == r && linearImageArr[i+1] == g && linearImageArr[i+2] == b) {
            duplCount++;
            i += 3;
        }
        i -= 3;

        for(int j = 0; j < colorCount; j++) {
            if(colorTable[j*3] == r && colorTable[j*3+1] == g && colorTable[j*3+2] == b) {
                if(duplCount == 0) {
                    fprintf(fpOut, "%d ", j);
                } else {
                    fprintf(fpOut, "*%d %d ", duplCount, j);
                }
            }
        }
    }
}

int main(int argc, char **argv) {
    extern char *optarg;
    extern int optind;
    int command, iCount=0, oCount=0, cCount=0, pCount=0, rCount=0, invalidCount=0;
    char *iTxt = NULL, *oTxt = NULL, *cTxt = NULL, *pTxt = NULL, *rTxt = NULL;
    bool UA = false, DA = false, IFM = false, OFU = false, CAM = false, CAI = false, PAI = false, RAI = false;

    while((command = getopt(argc, argv, "i:o:c:p:r:")) != -1) {
        // printf("c: %c\n", command);
        // printf("optarg: %s\n", optarg);
        // printf("optind: %d\n", optind);
        // printf("optarg first ele: %c\n", optarg[0]);
        if(command != '?' && optarg[0] == '-') {
            //printf("1MA returned\n");
            return MISSING_ARGUMENT;
        }
        switch(command) {
            case 'i':
                iCount++;
                iTxt = optarg;
                break;
            case 'o':
                oCount++;
                oTxt = optarg;
                break;
            case 'c':
                cCount++;
                cTxt = optarg;
                break;
            case 'p':
                pCount++;
                pTxt = optarg;
                break;
            case 'r':
                rCount++;
                rTxt = optarg;
                break;
            default:
                if(optopt == 'i' || optopt == 'o' || optopt == 'c' || optopt == 'p' || optopt == 'r') {
                    //printf("2MA returned\n");
                    return MISSING_ARGUMENT;
                }
                invalidCount++;
                break;
        }
    }

    if(iCount == 0 || oCount == 0) {
        //printf("3MA returned\n");
        return MISSING_ARGUMENT;
    }

    if(invalidCount > 0) {
        UA = true;
        //printf("UA returned\n");
        return UNRECOGNIZED_ARGUMENT;
    }

    if(iCount > 1 || oCount > 1 || cCount > 1 || pCount > 1 || rCount > 1) {
        DA = true;
        //printf("DA returned\n");
        return DUPLICATE_ARGUMENT;
    }

    FILE *fpIn;
    char fileTypeIn;
    int imageWidth;
    int imageHeight;
    if((fpIn = fopen(iTxt, "r")) == NULL) {
        IFM = true;
        //printf("IFM returned\n");
        return INPUT_FILE_MISSING;
    } else {
        fscanf(fpIn, "%c", &fileTypeIn); //takes the first char of the fileType
        fscanf(fpIn, "%*[^\n]\n"); //skips the rest of the line of fileType
        fscanf(fpIn, "%d %d", &imageWidth, &imageHeight);
    }

    FILE *fpOut;
    char fileTypeOut; //stores the last character of the file type
    int i = 0;
    if((fpOut = fopen(oTxt, "w")) == NULL) {
        OFU = true;
        //printf("OFU returned\n");
        return OUTPUT_FILE_UNWRITABLE;
    } else {
        while(oTxt[i] != '\0') {
            fileTypeOut = oTxt[i];
            i++; 
        }
    }

    if(pCount > 0 && cCount == 0) {
        CAM = true;
        //printf("CAM returned\n");
        return C_ARGUMENT_MISSING;
    }

    int **copiedRegionArr = NULL;
    if(cTxt != NULL) {
        long cParameters[4];
        CAI = processCInputs(cParameters, cTxt);
        if(CAI) {
            //printf("CAI returned\n");
            return C_ARGUMENT_INVALID;
        }

        copiedRegionArr = createCopiedRegionArr(imageWidth, imageHeight, fileTypeIn, cParameters, fpIn);
        (void) copiedRegionArr;
    }
    
    
    if(pTxt != NULL) {
        long pParameters[2];
        PAI = processPInputs(pParameters, pTxt);
        if(PAI) {
            //printf("PAI returned\n");
            return P_ARGUMENT_INVALID;
        }
    }
    
    
    if(rTxt != NULL) {
        char *rMessage = "";
        char *rPath_to_front = "";
        long rFont_size = 0;
        long rRow = 0;
        long rCol = 0;
        RAI = processRInputs(rMessage, rPath_to_front, rFont_size, rRow, rCol, rTxt);
        if(RAI) {
            //printf("RAI1 returned\n");
            return R_ARGUMENT_INVALID;
        }
        FILE *fpFonts;
        if((fpFonts = fopen(rPath_to_front, "r")) == NULL) {
            //printf("RAI2 returned\n");
            return R_ARGUMENT_INVALID;
        }
    }

    int **imageArr = (int **)malloc(imageHeight * sizeof(int *)); //this will hold the entire image
    for(i = 0; i < imageWidth; i++) {
        imageArr[i] = (int *)malloc(imageWidth * 3 * sizeof(int));
    }

    fseek(fpIn, 0, SEEK_SET); //reset pointer back to beginning of file
    fscanf(fpIn, "%*[^\n]\n"); //skip the p3/sbu line
    fscanf(fpIn, "%*[^\n]\n"); //skip the dimesion line
    if(fileTypeIn == 'P') {
        fscanf(fpIn, "%*[^\n]\n"); //skip the 255 line
        ppmToArr(imageArr, imageWidth, fpIn);
    } else {
        sbuToArr(imageArr, imageWidth, fpIn);
    }
    fclose(fpIn);

    if(pCount > 0) {
        //paste copiedRegion into imageArr
    }

    if(fileTypeOut == 'm') {
        arrToPPM(imageArr, imageWidth, imageHeight, fpOut);
    } else {
        arrToSBU(imageArr, imageWidth, imageHeight, fpOut);
    }
    fclose(fpOut);

    if(UA == true) {
        //printf("UA bot returned\n");
        return UNRECOGNIZED_ARGUMENT;
    } else if(DA == true) {
        //printf("DA bot returned\n");
        return DUPLICATE_ARGUMENT;
    } else if(IFM == true) {
        //printf("IFM bot returned\n");
        return INPUT_FILE_MISSING;
    } else if(OFU == true) {
        //printf("OFU bot returned\n");
        return OUTPUT_FILE_UNWRITABLE;
    } else if(CAM == true) {
        //printf("CAM bot returned\n");
        return C_ARGUMENT_MISSING;
    } else if(CAI == true) {
        //printf("CAI bot returned\n");
        return C_ARGUMENT_INVALID;
    } else if(PAI == true) {
       // printf("PAI bot returned\n");
        return P_ARGUMENT_INVALID;
    } else if(RAI == true) {
        //printf("RAI bot returned\n");
        return R_ARGUMENT_INVALID;
    } else {
        //printf("no issue\n");
        return 0;
    }
}
