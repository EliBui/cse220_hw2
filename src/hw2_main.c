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

void unUsedVar(long cParameters[], long pParameters[], char *rMessage, char *rPath_to_front, long rFont_size,long rRow, long rCol) {
    printf("cParameters: %ld\n", *cParameters);
    printf("pParameters: %ld\n", *pParameters);
    printf("rMessage: %s\n", rMessage);
    printf("rPath_to_front: %s\n", rPath_to_front);
    printf("rFont_size: %ld\n", rFont_size);
    printf("rRow: %ld\n", rRow);
    printf("rCol: %ld\n", rCol);
}

int main(int argc, char **argv) {
    extern char *optarg;
    extern int optind;
    int command, iCount=0, oCount=0, cCount=0, pCount=0, rCount=0, invalidCount=0;
    char *iTxt, *oTxt, *cTxt, *pTxt, *rTxt;

    while((command = getopt(argc, argv, "i:o:c:p:r:")) != -1) {
        printf("c: %d\n", command);
        printf("optarg: %s\n", optarg);
        printf("optind: %d\n", optind);
        printf("optarg first ele: %c\n", optarg[0]);
        if(optarg[0] == '-') {
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
                    return MISSING_ARGUMENT;
                }
                invalidCount++;
                break;
        }
    }

    if(iCount == 0 || oCount == 0) {
        return MISSING_ARGUMENT;
    }

    if(invalidCount > 0) {
        return UNRECOGNIZED_ARGUMENT;
    }

    if(iCount > 1 || oCount > 1 || cCount > 1 || pCount > 1 || rCount > 1) {
        return DUPLICATE_ARGUMENT;
    }

    FILE *fpIn;
    if((fpIn = fopen(iTxt, "r")) == NULL) {
        return INPUT_FILE_MISSING;
    }

    FILE *fpOut;
    if((fpOut = fopen(oTxt, "w")) == NULL) {
        return OUTPUT_FILE_UNWRITABLE;
    }

    if(pCount > 0 && cCount == 0) {
        return C_ARGUMENT_MISSING;
    }

    long cParameters[4];
    char *token = strtok(cTxt, ",");
    int index = 0;
    char *endPtr;
    cParameters[index++] = strtol(token, &endPtr, 10);
    if(endPtr != NULL) {
        return UNRECOGNIZED_ARGUMENT;
    }

    while((token = strtok(NULL, ",")) != NULL && index > 3) {
        cParameters[index++] = strtol(token, &endPtr, 10);
        if(endPtr != NULL) {
            return UNRECOGNIZED_ARGUMENT;
        }
    }

    if(index != 4 || token != NULL) {
        return C_ARGUMENT_INVALID;
    }
    
    long pParameters[2];
    token = strtok(pTxt, ",");
    index = 0;
    pParameters[index++] = strtol(token, &endPtr, 10);
    if(endPtr != NULL) {
        return UNRECOGNIZED_ARGUMENT;
    }

    while((token = strtok(NULL, ",")) != NULL && index > 1) {
        cParameters[index++] = strtol(token, &endPtr, 10);
        if(endPtr != NULL) {
            return UNRECOGNIZED_ARGUMENT;
        }
    }

    if(index != 2 || token != NULL) {
        return P_ARGUMENT_INVALID;
    }
    
    char *rMessage = strtok(rTxt, ",");

    if((token = strtok(NULL, ",")) == NULL) {
        return R_ARGUMENT_INVALID;
    }
    char *rPath_to_front = token;
    
    if((token = strtok(NULL, ",")) == NULL) {
        return R_ARGUMENT_INVALID;
    }
    long rFont_size = strtol(token, NULL, 10);

    if((token = strtok(NULL, ",")) == NULL) {
        return R_ARGUMENT_INVALID;
    }
    long rRow = strtol(token, NULL, 10);

    if((token = strtok(NULL, ",")) == NULL) {
        return R_ARGUMENT_INVALID;
    }
    long rCol = strtol(token, NULL, 10);

    if((token = strtok(NULL, ",")) == NULL) {
        return R_ARGUMENT_INVALID;
    }


    unUsedVar( cParameters,  pParameters,  rMessage,  rPath_to_front,  rFont_size, rRow, rCol);
    return 0;
}
