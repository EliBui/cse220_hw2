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

int main(int argc, char **argv) {
    extern char *optarg;
    extern int optind;
    int command, iCount=0, oCount=0, cCount=0, pCount=0, rCount=0, invalidCount=0;
    char *iTxt, *oTxt, *cTxt, *pTxt, *rTxt;
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
    if((fpIn = fopen(iTxt, "r")) == NULL) {
        IFM = true;
        //printf("IFM returned\n");
        return INPUT_FILE_MISSING;
    }

    FILE *fpOut;
    if((fpOut = fopen(oTxt, "w")) == NULL) {
        OFU = true;
        //printf("OFU returned\n");
        return OUTPUT_FILE_UNWRITABLE;
    }

    if(pCount > 0 && cCount == 0) {
        CAM = true;
        //printf("CAM returned\n");
        return C_ARGUMENT_MISSING;
    }

    long cParameters[4];
    if(cTxt != NULL) {
        CAI = processCInputs(cParameters, cTxt);
        if(CAI) {
            //printf("CAI returned\n");
            return C_ARGUMENT_INVALID;
        }
    }
    
    long pParameters[2];
    if(pTxt != NULL) {
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
            printf("RAI1 returned\n");
            return R_ARGUMENT_INVALID;
        }
        FILE *fpFonts;
        if((fpFonts = fopen(rPath_to_front, "r")) == NULL) {
            printf("RAI2 returned\n");
            return R_ARGUMENT_INVALID;
        }
    }
    
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
        printf("RAI bot returned\n");
        return R_ARGUMENT_INVALID;
    } else {
        printf("no issue\n");
        return 0;
    }
}
