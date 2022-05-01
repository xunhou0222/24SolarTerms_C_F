#include <stdio.h>
#include "solarterms.h"




void utc2tdb(double JD, double TT[2]){

    double UTC[2], TAI[2];
    int i,j;

    split_(&JD, UTC);

    i = iauUtctai(UTC[0], UTC[1], &TAI[0], &TAI[1]);
    if (i == 0){
        j = iauTaitt(TAI[0], TAI[1], &TT[0], &TT[1]);
        if (j != 0)
            printf("%s","Errors occured while Converting TAI TO TT!");
    }
    else 
        printf("%s","Errors occured while Converting UTC TO TAI!");

   
}