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

#define PI acos(-1)

extern void FOC(PI_str* D_PI,   PI_str* Q_PI,  PI_str* Spd_PI,    uint8_t* Spd_Tick,
                double  iTheta, double  iNp,   double* oSinTheta, double*  oCosTheta, 
                double  iId,    double  iIq,   double* oUx,       double*  oUy,
                double* oU1,    double* oU2,   double* oU3,       double*  oSector,
                double* oCCRa,  double* oCCRb, double* oCCRc,
                double  iIa,    double  iIc,   double* oIx,       double*  oIy,
                double* oId,    double* oIq,   double* oUd,       double*  oUq,
                double* oSpd);

#endif
