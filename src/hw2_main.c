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

// void unUsedVar(long cParameters[], long pParameters[], char *rMessage, char *rPath_to_front, long rFont_size,long rRow, long rCol) {
//     printf("cParameters: %ld\n", *cParameters);
//     printf("pParameters: %ld\n", *pParameters);
//     printf("rMessage: %s\n", rMessage);
//     printf("rPath_to_front: %s\n", rPath_to_front);
//     printf("rFont_size: %ld\n", rFont_size);
//     printf("rRow: %ld\n", rRow);
//     printf("rCol: %ld\n", rCol);
// }

bool processCInputs(long *cParameters, char *cTxt) {
    char *token = strtok(cTxt, ",");
    int index = 0;
    cParameters[index++] = strtol(token, NULL, 10);

    while((token = strtok(NULL, ",")) != NULL && index > 3) {
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

    while((token = strtok(NULL, ",")) != NULL && index > 1) {
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

    if((token = strtok(NULL, ",")) == NULL) {
        return true;
    }
    (void) rMessage;
    (void) rPath_to_front;
    (void) rFont_size;
    (void) rRow;
    (void) rCol;
    return false;
}

int main(int argc, char **argv) {
    extern char *optarg;
    extern int optind;
    int command, iCount=0, oCount=0, cCount=0, pCount=0, rCount=0, invalidCount=0;
    char *iTxt, *oTxt, *cTxt, *pTxt, *rTxt;
    bool UA = false, DA = false, IFM = false, OFU = false, CAM = false, CAI = false, PAI = false, RAI = false;

    while((command = getopt(argc, argv, "i:o:c:p:r:")) != -1) {
        // printf("c: %d\n", command);
        // printf("optarg: %s\n", optarg);
        // printf("optind: %d\n", optind);
        // printf("optarg first ele: %c\n", optarg[0]);
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
        UA = true;
        return UNRECOGNIZED_ARGUMENT;
    }

    if(iCount > 1 || oCount > 1 || cCount > 1 || pCount > 1 || rCount > 1) {
        DA = true;
        return DUPLICATE_ARGUMENT;
    }

    FILE *fpIn;
    if((fpIn = fopen(iTxt, "r")) == NULL) {
        IFM = true;
        return INPUT_FILE_MISSING;
    }

    FILE *fpOut;
    if((fpOut = fopen(oTxt, "w")) == NULL) {
        printf("%s\n", oTxt);
        OFU = true;
        return OUTPUT_FILE_UNWRITABLE;
    }

    if(pCount > 0 && cCount == 0) {
        CAM = true;
        return C_ARGUMENT_MISSING;
    }

    long cParameters[4];
    if(cTxt != NULL) {
        CAI = processCInputs(cParameters, cTxt);
        if(CAI) {
            return C_ARGUMENT_INVALID;
        }
    }
    
    long pParameters[2];
    if(pTxt != NULL) {
        PAI = processPInputs(pParameters, pTxt);
        if(PAI) {
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
            return R_ARGUMENT_INVALID;
        }
        FILE *fpFonts;
        if((fpFonts = fopen(oTxt, "r")) == NULL) {
            return R_ARGUMENT_INVALID;
        }
    }


    //unUsedVar( cParameters,  pParameters,  rMessage,  rPath_to_front,  rFont_size, rRow, rCol);
    
    if(UA == true) {
        return UNRECOGNIZED_ARGUMENT;
    } else if(DA == true) {
        return DUPLICATE_ARGUMENT;
    } else if(IFM == true) {
        return INPUT_FILE_MISSING;
    } else if(OFU == true) {
        return OUTPUT_FILE_UNWRITABLE;
    } else if(CAM == true) {
        return C_ARGUMENT_MISSING;
    } else if(CAI == true) {
        return C_ARGUMENT_INVALID;
    } else if(PAI == true) {
        return P_ARGUMENT_INVALID;
    } else if(RAI == true) {
        return R_ARGUMENT_INVALID;
    } else {
        return 0;
    }
}
