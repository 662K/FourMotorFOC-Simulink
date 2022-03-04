#include "FOC.h"

uint32_t GetTheta(double iTheta){
    return iTheta / (2 * PI) * (1 << 20);
}

uint32_t GetThetaE(uint32_t Theta, uint8_t Np){
    return ((Theta * Np) & (0xFFFFF)) >> 3;
}

void Cordic(uint32_t ThetaE, int16_t* SinTheta, int16_t* CosTheta){
    int32_t SinTheta_temp = (1 << 15) * sin(1.0 * ThetaE / (1 << 17) * 2 * PI);
    int32_t CosTheta_temp = (1 << 15) * cos(1.0 * ThetaE / (1 << 17) * 2 * PI);

    if(SinTheta_temp > 32767)
        *SinTheta = 32767;
    else if(SinTheta_temp < -32767)
        *SinTheta = -32767;
    else
        *SinTheta = SinTheta_temp;

    if(CosTheta_temp > 32767)
        *CosTheta = 32767;
    else if(CosTheta_temp < -32767)
        *CosTheta = -32767;
    else
        *CosTheta = CosTheta_temp;
}

void InvPark(int16_t Ud, int16_t Uq, int16_t SinTheta, int16_t CosTheta, int16_t* Ux, int16_t* Uy){
    int32_t Ux_temp = (CosTheta * Ud - SinTheta * Uq) >> 15;
    int32_t Uy_temp = (SinTheta * Ud + CosTheta * Uq) >> 15;

    if(Ux_temp > 32767)
        *Ux = 32767;
    else if(Ux_temp < -32767)
        *Ux = -32767;
    else
        *Ux = Ux_temp;

    if(Uy_temp > 32767)
        *Uy = 32767;
    else if(Uy_temp < -32767)
        *Uy = -32767;
    else
        *Uy = Uy_temp;
}

void InvClarke(int16_t Ux, int16_t Uy, int16_t* U1, int16_t* U2, int16_t* U3){
    int32_t U1_temp =  Uy;
    int32_t U2_temp = ( Ux * 28378 - Uy * 16384) >> 15;
    int32_t U3_temp = (-Ux * 28378 - Uy * 16384) >> 15;

    if(U1_temp > 32767)
        *U1 = 32767;
    else if(U1_temp < -32767)
        *U1 = -32767;
    else
        *U1 = U1_temp;

    if(U2_temp > 32767)
        *U2 = 32767;
    else if(U2_temp < -32767)
        *U2 = -32767;
    else
        *U2 = U2_temp;

    if(U3_temp > 32767)
        *U3 = 32767;
    else if(U3_temp < -32767)
        *U3 = -32767;
    else
        *U3 = U3_temp;
}

uint8_t GetSector(int16_t U1, int16_t U2, int16_t U3){
    uint8_t N = ((U3 >= 0) << 2) | ((U2 >= 0) << 1) | ((U1 >= 0) << 0);
    uint8_t Sector;

    switch (N){
    case 3:
        Sector = 1;
        break;
    case 1:
        Sector = 2;
        break;
    case 5:
        Sector = 3;
        break;
    case 4:
        Sector = 4;
        break;
    case 6:
        Sector = 5;
        break;
    case 2:
        Sector = 6;
        break;
    }

    return Sector;
}

void GetCCR(int16_t U1, int16_t U2, int16_t U3, uint8_t Sector, uint16_t* CCRa, uint16_t* CCRb, uint16_t* CCRc){
    int16_t Ux, Uy;
    uint32_t Ta_temp, Tb_temp, Tc_temp;
    uint16_t Ta, Tb, Tc;

    switch(Sector){
    case 1: Ux =  U2; Uy =  U1; break;
    case 2: Ux = -U2; Uy = -U3; break;
    case 3: Ux =  U1; Uy =  U3; break;
    case 4: Ux = -U1; Uy = -U2; break;
    case 5: Ux =  U3; Uy =  U2; break;
    case 6: Ux = -U3; Uy = -U1; break;
    default:Ux =  Ux; Uy =  Uy; break;
	}

    Ta_temp = 32768 - Ux - Uy;
    Tb_temp = 32768 + Ux - Uy;
    Tc_temp = 32768 + Ux + Uy;

    if(Ta_temp > 65535)
        Ta = 65535;
    else if(Ta_temp < 0)
        Ta = 0;
    else
        Ta = Ta_temp;

    if(Tb_temp > 65535)
        Tb = 65535;
    else if(Tb_temp < 0)
        Tb = 0;
    else
        Tb = Tb_temp;

    if(Tc_temp > 65535)
        Tc = 65535;
    else if(Tc_temp < 0)
        Tc = 0;
    else
        Tc = Tc_temp;

    switch(Sector){
    case 1: *CCRa = Ta;   *CCRb = Tb;   *CCRc = Tc;   break;
    case 2: *CCRa = Tb;   *CCRb = Ta;   *CCRc = Tc;   break;
    case 3: *CCRa = Tc;   *CCRb = Ta;   *CCRc = Tb;   break;
    case 4: *CCRa = Tc;   *CCRb = Tb;   *CCRc = Ta;   break;
    case 5: *CCRa = Tb;   *CCRb = Tc;   *CCRc = Ta;   break;
    case 6: *CCRa = Ta;   *CCRb = Tc;   *CCRc = Tb;   break;
    }
}

void GetCur(double iIa, double iIc, int16_t* Ia, int16_t* Ic){
    int16_t Ia_temp = iIa * 2048 / 5;
    int16_t Ic_temp = iIc * 2048 / 5;

    if(Ia_temp > 2047)
        *Ia = 2047;
    else if(Ia_temp < -2047)
        *Ia = -2047;
    else
        *Ia = Ia_temp;

    if(Ic_temp > 2047)
        *Ic = 2047;
    else if(Ic_temp < -2047)
        *Ic = -2047;
    else
        *Ic = Ic_temp;
}

void Clarke(int16_t Ia, int16_t Ic, int16_t* Ix, int16_t* Iy){
    int16_t Ix_temp = Ia;
    int16_t Iy_temp = -((Ia + (Ic *2))*2365) >> 12;

    if(Ix_temp > 2047)
        *Ix = 2047;
    else if(Ix_temp < -2047)
        *Ix = -2047;
    else
        *Ix = Ix_temp;

    if(Iy_temp > 2047)
        *Iy = 2047;
    else if(Iy_temp < -2047)
        *Iy = -2047;
    else
        *Iy = Iy_temp;
}

void Park(int16_t Ix, int16_t Iy, int16_t SinTheta, int16_t CosTheta, int16_t* Id, int16_t* Iq){
    int32_t Id_temp = (SinTheta * Iy + CosTheta * Ix) >> 11;
	int32_t Iq_temp = (CosTheta * Iy - SinTheta * Ix) >> 11;

    if(Id_temp > 32767)
        *Id = 32767;
    else if(Id_temp < -32767)
        *Id = -32767;
    else
        *Id = Id_temp;

    if(Iq_temp > 32767)
        *Iq = 32767;
    else if(Iq_temp < -32767)
        *Iq = -32767;
    else
        *Iq = Iq_temp;
}

double CurPID(PI_str* pPI, int16_t Target, int16_t Present){
    int32_t Error = Target - Present;
    uint8_t ui_flag = !(((pPI->Out_temp > pPI->Max) || (pPI->Out_temp < -pPI->Max)) && (pPI->Out_temp * Error >= 0));
    
    pPI->up = pPI->Kp * Error;
    pPI->ui = pPI->ui + pPI->Ki * Error * ui_flag;
    
    pPI->Out_temp = (pPI->up + pPI->ui) >> 9;
    
    double PIout = 0;
    
    if(pPI->Out_temp > pPI->Max)
        PIout = pPI->Max;
    else if(pPI->Out_temp < -pPI->Max)
        PIout = -pPI->Max;
    else 
        PIout = pPI->Out_temp;
    
    return PIout;
}

double SpdPID(PI_str* pPI, int16_t Target, int16_t Present){
    int32_t Error = Target - Present;
    uint8_t ui_flag = !(((pPI->Out_temp > pPI->Max) || (pPI->Out_temp < -pPI->Max)) && (pPI->Out_temp * Error >= 0));
    
    pPI->up = pPI->Kp * Error;
    pPI->ui = pPI->ui + pPI->Ki * Error * ui_flag;
    
    pPI->Out_temp = (pPI->up + pPI->ui) >> 11;
    
    double PIout = 0;
    
    if(pPI->Out_temp > pPI->Max)
        PIout = pPI->Max;
    else if(pPI->Out_temp < -pPI->Max)
        PIout = -pPI->Max;
    else 
        PIout = pPI->Out_temp;
    
    return PIout;
}

void Spd_Timer(uint8_t* Spd_Tick){
    if(*Spd_Tick < 9)
        *Spd_Tick += 1;
    else
        *Spd_Tick = 0;
}

int32_t GetSpd(uint32_t Theta, uint8_t Spd_Tick){
    static uint32_t Theta_temp = 0;
    static int32_t Speed = 0;

    if(Spd_Tick == 0){
        Speed = (Theta - Theta_temp) & 0xFFFFF;
        Theta_temp = Theta;
    }

    return Speed;
}

void FOC(PI_str* D_PI,   PI_str* Q_PI,  PI_str* Spd_PI,    uint8_t* Spd_Tick,
         double  iTheta, double  iNp,   double* oSinTheta, double*  oCosTheta, 
         double  iId,    double  iIq,   double* oUx,       double*  oUy,
         double* oU1,    double* oU2,   double* oU3,       double*  oSector,
         double* oCCRa,  double* oCCRb, double* oCCRc,
         double  iIa,    double  iIc,   double* oIx,       double*  oIy,
         double* oId,    double* oIq,   double* oUd,       double*  oUq,
         double* oSpd){

    uint32_t Theta = GetTheta(iTheta); uint8_t  Np = (uint8_t)iNp;
    int16_t  TargetId = (int16_t)iId; int16_t  TargetIq = (int16_t)iIq;

    int16_t  SinTheta, CosTheta;
    int16_t  Ia, Ic;
    int16_t  Ix, Iy;
    int16_t  PresentId, PresentIq;
    int16_t  Ud, Uq;
    int16_t  Ux, Uy;
    int16_t  U1, U2, U3;
    int8_t   Sector;
    uint16_t CCRa, CCRb, CCRc;

    Spd_Timer(Spd_Tick);

    uint32_t ThetaE = GetThetaE(Theta, Np);
    GetCur(iIa, iIc, &Ia, &Ic);
    int32_t Speed = GetSpd(Theta, *Spd_Tick);

    Cordic(ThetaE, &SinTheta, &CosTheta);
    Clarke(Ia, Ic, &Ix, &Iy);
    Park(Ix, Iy, SinTheta, CosTheta, &PresentId, &PresentIq);

    Ud = CurPID(D_PI, TargetId, PresentId);
    Uq = CurPID(Q_PI, TargetIq, PresentIq);

    InvPark(Ud, Uq, SinTheta, CosTheta, &Ux, &Uy);
    InvClarke(Ux, Uy, &U1, &U2, &U3);
    Sector = GetSector(U1, U2, U3);
    GetCCR(U1, U2, U3, Sector, &CCRa, &CCRb, &CCRc);

    *oSinTheta = SinTheta; *oCosTheta = CosTheta;
    *oUx = Ux; *oUy = Uy;
    *oU1 = U1; *oU2 = U2; *oU3 = U3;
    *oSector = Sector;
    *oCCRa = CCRa; *oCCRb = CCRb; *oCCRc = CCRc;
    *oIx = Ix; *oIy = Iy;
    *oId = PresentId; *oIq = PresentIq;
    *oUd = Ud; *oUq = Uq;
    *oSpd = Speed;
}
