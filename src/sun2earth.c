#include <stdio.h>
#include <math.h>
#include "solarterms.h"




void sun2earth(int set, double AU, double clight, double JD, double pos[3], double vol[3]){

/*
******************************************************************************
计算给定历元下的太阳相对于地球的位置与速度，并考虑了光行时修正
      Input:
            (1)AU
                     double      天文单位AU，单位km
            (2)clight
                     double      光速，单位km/s
            (3)JD
                     double      历元儒略日（UTC）
            (4)set
                     int         选择是否考虑光行差，set == 1，考虑；set == 0，不考虑。

         
      Output:
            pos(x, y, z), vol(vx, vy, vz)
******************************************************************************
*/

    double TT2[2], TT, PRD[6];
    int Ntar = 11, Ncen  = 3, Nbary = 12;
    double r, DTDB1, DTDB2;
    double d2s = 86400.0;
    int i;

    utc2tdb(JD, TT2);
    TT = TT2[0] + TT2[1];
    pleph_(&TT, &Ntar, &Ncen, PRD);
    pos[0] = PRD[0]*AU;
    pos[1] = PRD[1]*AU;
    pos[2] = PRD[2]*AU;
    vol[0] = PRD[3]*AU/d2s;
    vol[1] = PRD[4]*AU/d2s;
    vol[2] = PRD[5]*AU/d2s;
    
    if(set == 1){
        r = sqrt(pos[0]*pos[0] + pos[1]*pos[1] +pos[2]*pos[2]);
        DTDB1 = 0.0;
        DTDB2 = r/clight/d2s;
        while (fabs(DTDB1 - DTDB2) > (1.0/d2s/1000.0)){
            DTDB1 = DTDB2;
            pos[0] = pos[0] - vol[0]*DTDB1*d2s;
            pos[1] = pos[1] - vol[1]*DTDB1*d2s;
            pos[2] = pos[2] - vol[2]*DTDB1*d2s;
            r = sqrt(pos[0]*pos[0] + pos[1]*pos[1] + pos[2]*pos[2]); 
            DTDB2 = r/clight/d2s;      
        }
        TT = TT - DTDB2;
        //位置是考虑光行时后太阳相对地心的位置
        pleph_(&TT, &Ntar, &Ncen, PRD);
        pos[0] = PRD[0]*AU;
        pos[1] = PRD[1]*AU;
        pos[2] = PRD[2]*AU;
        //速度是考虑光行时后的历元时刻，地球相对于太阳系质心的速度，用于后边的光行差改正
        pleph_(&TT, &Ncen, &Nbary, PRD);
        vol[0] = PRD[3]*AU/d2s;
        vol[1] = PRD[4]*AU/d2s;
        vol[2] = PRD[5]*AU/d2s;
    }

    else{
        pleph_(&TT, &Ncen, &Nbary, PRD);
        vol[0] = PRD[3]*AU/d2s;
        vol[1] = PRD[4]*AU/d2s;
        vol[2] = PRD[5]*AU/d2s;       
    }
}