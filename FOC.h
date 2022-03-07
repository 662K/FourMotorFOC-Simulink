#ifndef __FOC_H__
#define __FOC_H__

#include <math.h>
#include <stdint.h>

typedef struct{
    int16_t Kp;
    int16_t Ki;
    int16_t Max;
    int32_t up;
    int32_t ui;
    int16_t Target;
    int16_t Present;
    int32_t Out_temp;
}PI_str;

typedef struct{
    uint8_t Spd_Tick;
    uint32_t Theta_temp;
    int16_t PresentSpd;
    int16_t TargetSpd;

    uint32_t Theta;
    uint8_t Np;

    int16_t SinTheta;
    int16_t CosTheta; 

    int16_t TargetId;
    int16_t TargetIq;

    int16_t Ux;       
    int16_t Uy;

    int16_t U1;    
    int16_t U2;   
    int16_t U3;       

    uint8_t Sector;

    uint16_t CCRa;
    uint16_t CCRb; 
    uint16_t CCRc;

    int16_t Ia;
    int16_t Ic;

    int16_t Ix;       
    int16_t Iy;

    int16_t PresentId;
    int16_t PresentIq;

    int16_t PresentUd;
    int16_t PresentUq;

    int16_t TargetUd;
    int16_t TargetUq;

    uint8_t Mode;
}DataIO_str;

#define PI acos(-1)

extern uint32_t GetTheta(double iTheta);
extern int16_t GetCur(double iI);
extern void FOC(PI_str* D_PI, PI_str* Q_PI, PI_str* Spd_PI, DataIO_str* DataIO);

#endif
