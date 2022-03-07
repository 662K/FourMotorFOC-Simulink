#include "FOC.h"

uint32_t GetTheta(double iTheta){
    return (uint32_t)(iTheta * (1 << 20) / (2 * PI));
}

uint32_t GetThetaE(uint32_t Theta, uint8_t Np){
    return ((Theta * Np) & (0xFFFFF)) >> 3;
}

void Cordic(uint32_t ThetaE, int16_t* SinTheta, int16_t* CosTheta){
    int32_t SinTheta_temp = (int32_t)((1 << 15) * sin(1.0 * ThetaE / (1 << 17) * 2 * PI));
    int32_t CosTheta_temp = (int32_t)((1 << 15) * cos(1.0 * ThetaE / (1 << 17) * 2 * PI));

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

int16_t GetCur(double iI){
    int16_t I_temp = (int16_t)(iI * 2048 / 5);

    if(I_temp > 2047)
        I_temp = 2047;
    else if(I_temp < -2047)
        I_temp = -2047;
    else
        I_temp = I_temp;
    
    return I_temp;
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

int16_t CurPID(PI_str* pPI, int16_t Target, int16_t Present){
    int32_t Error = Target - Present;
    uint8_t ui_flag = !(((pPI->Out_temp > pPI->Max) || (pPI->Out_temp < -pPI->Max)) && (pPI->Out_temp * Error >= 0));
    
    pPI->up = pPI->Kp * Error;
    pPI->ui = pPI->ui + pPI->Ki * Error * ui_flag;
    
    pPI->Out_temp = (pPI->up + pPI->ui) >> 9;
    
    int16_t PIout = 0;
    
    if(pPI->Out_temp > pPI->Max)
        PIout = pPI->Max;
    else if(pPI->Out_temp < -pPI->Max)
        PIout = -pPI->Max;
    else 
        PIout = pPI->Out_temp;
    
    return PIout;
}

int16_t SpdPID(PI_str* pPI, int16_t Target, int16_t Present){
    int32_t Error = Target - Present;
    uint8_t ui_flag = !(((pPI->Out_temp > pPI->Max) || (pPI->Out_temp < -pPI->Max)) && (pPI->Out_temp * Error >= 0));
    
    pPI->up = pPI->Kp * Error;
    pPI->ui = pPI->ui + pPI->Ki * Error * ui_flag;
    
    pPI->Out_temp = (pPI->up + pPI->ui) >> 11;
    
    int16_t PIout = 0;
    
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

void GetSpd(uint32_t Theta, uint32_t* Theta_Pre, uint8_t Spd_Tick, int16_t* Speed, int16_t* Speed_Pre, int16_t Alpha){
    if(Spd_Tick == 0){
        int32_t Speed_temp = (Theta - *Theta_Pre);
        Speed_temp = ((32767 - Alpha) * (*Speed_Pre) + Alpha * (int16_t)Speed_temp) / 32767;

        if(Speed_temp > 32767)
            *Speed = 32767;
        else if(Speed_temp < -32767)
            *Speed = -32767;
        else
            *Speed = Speed_temp;
            
        *Theta_Pre = Theta;
        *Speed_Pre = *Speed;
    }
}

void FOC(PI_str* D_PI, PI_str* Q_PI, PI_str* Spd_PI, DataIO_str* DataIO){
    Spd_Timer(&(DataIO->Spd_Tick));

    uint32_t ThetaE = GetThetaE(DataIO->Theta, DataIO->Np);
    
    GetSpd(DataIO->Theta, &DataIO->Theta_Pre, DataIO->Spd_Tick, &DataIO->PresentSpd, &DataIO->PresentSpd_Pre, DataIO->Alpha);
    Cordic(ThetaE, &DataIO->SinTheta, &DataIO->CosTheta);
    Clarke(DataIO->Ia, DataIO->Ic, &DataIO->Ix, &DataIO->Iy);
    Park(DataIO->Ix, DataIO->Iy, DataIO->SinTheta, DataIO->CosTheta, &DataIO->PresentId, &DataIO->PresentIq);

    if(DataIO->Mode == 0){
        InvPark(DataIO->TargetUd, DataIO->TargetUq, DataIO->SinTheta, DataIO->CosTheta, &DataIO->Ux, &DataIO->Uy);
        InvClarke(DataIO->Ux, DataIO->Uy, &DataIO->U1, &DataIO->U2, &DataIO->U3);
        DataIO->Sector = GetSector(DataIO->U1, DataIO->U2, DataIO->U3);
        GetCCR(DataIO->U1, DataIO->U2, DataIO->U3, DataIO->Sector, &DataIO->CCRa, &DataIO->CCRb, &DataIO->CCRc);
    }
    else if(DataIO->Mode == 1){
        DataIO->PresentUd = CurPID(D_PI, DataIO->TargetId, DataIO->PresentId);
        DataIO->PresentUq = CurPID(Q_PI, DataIO->TargetIq, DataIO->PresentIq);

        InvPark(DataIO->PresentUd, DataIO->PresentUq, DataIO->SinTheta, DataIO->CosTheta, &DataIO->Ux, &DataIO->Uy);
        InvClarke(DataIO->Ux, DataIO->Uy, &DataIO->U1, &DataIO->U2, &DataIO->U3);
        DataIO->Sector = GetSector(DataIO->U1, DataIO->U2, DataIO->U3);
        GetCCR(DataIO->U1, DataIO->U2, DataIO->U3, DataIO->Sector, &DataIO->CCRa, &DataIO->CCRb, &DataIO->CCRc);
    }
    else if(DataIO->Mode == 2){
        if(DataIO->Spd_Tick == 0){
            DataIO->TargetId = 0;
            DataIO->TargetIq = SpdPID(Spd_PI, DataIO->TargetSpd, DataIO->PresentSpd);
        }

        DataIO->PresentUd = CurPID(D_PI, DataIO->TargetId, DataIO->PresentId);
        DataIO->PresentUq = CurPID(Q_PI, DataIO->TargetIq, DataIO->PresentIq);

        InvPark(DataIO->PresentUd, DataIO->PresentUq, DataIO->SinTheta, DataIO->CosTheta, &DataIO->Ux, &DataIO->Uy);
        InvClarke(DataIO->Ux, DataIO->Uy, &DataIO->U1, &DataIO->U2, &DataIO->U3);
        DataIO->Sector = GetSector(DataIO->U1, DataIO->U2, DataIO->U3);
        GetCCR(DataIO->U1, DataIO->U2, DataIO->U3, DataIO->Sector, &DataIO->CCRa, &DataIO->CCRb, &DataIO->CCRc);
    }
}
