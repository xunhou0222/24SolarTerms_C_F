#include <stdio.h>
#include <math.h>
#include <string.h>
#include "solarterms.h"




int main(){

    int i, j, k = 0;    //计数器
    int set = 1;    //是否考虑光行时，set ==1，考虑；set == 0，不考虑
    double PI = acos(-1.0);

// const_所需参数，以及待读取参数
    char NAM[300][6];
    double VAL[300] = {0}, SSS[3] = {0};
    int N;
    double AU, clight; 

//设置起始日期以及推算范围
    const int year = 2022, month = 1, day =1;
    const float predict = 366.0, step = 365.0/360.0/10.0;
    double JD02[2], JD0, JD, TT[2];

//光行差改正
    double r_sun[3] = {0}, r_sun2[3] = {0}, v_obs[3] = {0}, r, bm1;

//进动章动修正
    double R_bpn[3][3] = {0};

//黄赤交角，赤道系到黄道系的变换矩阵，视黄经
    double alpha, R_matrix[3][3], lambda, delta; 

//二十四节气
    double date[24] = {0}, angle[24] = {0}; 

    double tan_lambda;
    

    printf("%s%d%s\n","Broadcast the 24 solar terms for ", year, "!      By Tingwei Jiang, UCAS, SHAO.");


//读取常数的值
    const_(NAM[0], VAL, SSS, &N);  
    for (i = 0; i<N; i++){
        if (strncmp(NAM[i], "AU    ", 6) == 0){
            AU = VAL[i];
            break;
        }
        else if (strncmp(NAM[i], "CLIGHT ", 6) == 0)
            clight = VAL[i];
    }


// 将起始日期转化为儒略日，并判断是否超出历表的历元范围 
    i = iauCal2jd(year, month, day, &JD02[0], &JD02[1]);
    if (i == 0)
        JD0 = JD02[0] + JD02[1];
    else{
        printf("%s\n","ERROR! Calculation of JD for the initial epoch failed!");
        return 1;
    }

    if ( (JD0 < SSS[0]) || (JD0 + predict > SSS[1]) ){
        printf("%s\n","The time interval of prediction is beyond the limit!");
        return 1;
    }


//循环推算
    JD = JD0;
    while (JD <= JD0 + predict){
        
    //该历元下的瞬时平黄赤交角alpha
        utc2tdb(JD, TT); 
        alpha = iauObl06(TT[0], TT[1]);
        R_matrix[0][0] = 1.0;
        R_matrix[0][1] = 0.0;
        R_matrix[0][2] = 0.0;
        R_matrix[1][0] = 0.0;
        R_matrix[1][1] = cos(alpha);
        R_matrix[1][2] = sin(alpha);
        R_matrix[2][0] = 0.0;
        R_matrix[2][1] = -sin(alpha);
        R_matrix[2][2] = cos(alpha);



    //光行时修正之后，太阳相对于地球的位置，以及地心相对于太阳系质心的速度（为光行差修正做准备）
        sun2earth(set, AU, clight, JD, r_sun, v_obs);

    //光行差修正
        //自然方向的单位矢量
        r = sqrt( r_sun[0]*r_sun[0] + r_sun[1]*r_sun[1] + r_sun[2]*r_sun[2] );
        r_sun[0] = r_sun[0]/r; 
        r_sun[1] = r_sun[1]/r;
        r_sun[2] = r_sun[2]/r;
        //地心相对于太阳系质心的约化速度矢量，以光速c为单位
        v_obs[0] = v_obs[0]/clight;
        v_obs[1] = v_obs[1]/clight;
        v_obs[2] = v_obs[2]/clight;
        //洛伦兹因子
        bm1 = sqrt(1 - v_obs[0]*v_obs[0] - v_obs[1]*v_obs[1] - v_obs[2]*v_obs[2]);
        //本征方向的单位矢量，存放于r_sun2
        iauAb(r_sun, v_obs, r, bm1, r_sun2);
              
    //进动章动修正
    iauPnm06a(TT[0], TT[1], R_bpn);
        for (i=0; i<3; i++){
            r_sun[i] = 0;
            for (j=0; j<3; j++){
                r_sun[i] += R_bpn[i][j]*r_sun2[j];
            }    
        }  
    
    //将坐标系转换至黄道坐标系
        for (i=0; i<3; i++){
            r_sun2[i] = 0;
            for (j=0; j<3; j++){
                r_sun2[i] += R_matrix[i][j]*r_sun[j];
            }    
        }  

    //求视黄经
        tan_lambda = r_sun2[1]/r_sun2[0];        
        if (r_sun2[0] >= 0 && r_sun2[1] >= 0)
            lambda = atan(tan_lambda)/PI*180.0;
        else if (r_sun2[0] < 0 && r_sun2[1] >= 0)
            lambda = atan(tan_lambda)/PI*180.0 + 180.0;
        else if (r_sun2[0] <0 && r_sun2[1] < 0)
            lambda = atan(tan_lambda)/PI*180.0 + 180.0;
        else 
            lambda = atan(tan_lambda)/PI*180.0 + 360.0;

    // //筛选二十四节气
        j = termscheck(&k, JD, lambda, date, angle);
        if (j == 0){
            delta =  ( (int)(lambda/15) + 1.0 - lambda/15.0 )*15.0;
            if (delta > 0.000001)
                JD = JD + delta*step;
            else 
                JD = JD + step/3600.0/1000.0; 
        }
        else
            JD = JD + 13.0;

    }

    //按日期顺序，打印输出二十四节气    
    termsprinter(date, angle);

        
    getchar();

    return 0;
}