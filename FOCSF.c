#define S_FUNCTION_NAME  FOCSF
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "FOC.h"

PI_str D_PI  = {0};
PI_str Q_PI  = {0};
PI_str Spd_PI  = {0};
DataIO_str DataIO = {0};

/* 模块初始化函数 */
static void mdlInitializeSizes(SimStruct *S)
{
    memset(&D_PI, 0, sizeof(D_PI));
    memset(&Q_PI, 0, sizeof(Q_PI));
    memset(&Spd_PI, 0, sizeof(Spd_PI));

    memset(&DataIO, 0, sizeof(DataIO));
    /* 设置参数数量 */
    ssSetNumSFcnParams(S, 8);

    ssSetSFcnParamTunable(S, 0, 1);
    ssSetSFcnParamTunable(S, 1, 1);
    ssSetSFcnParamTunable(S, 2, 1);
    ssSetSFcnParamTunable(S, 3, 1);
    ssSetSFcnParamTunable(S, 4, 1);
    ssSetSFcnParamTunable(S, 5, 1);
    ssSetSFcnParamTunable(S, 6, 1);
    ssSetSFcnParamTunable(S, 7, 1);

    /* 设置输入端口数量 */
    if (!ssSetNumInputPorts(S, 9)) return;

    /* 配置输入端口 */
    ssSetInputPortDataType(S, 0, SS_DOUBLE);   
    ssSetInputPortWidth(S, 0, 1);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortRequiredContiguous(S, 0, 1);

    ssSetInputPortDataType(S, 1, SS_DOUBLE);   
    ssSetInputPortWidth(S, 1, 1);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    ssSetInputPortRequiredContiguous(S, 1, 1);	

    ssSetInputPortDataType(S, 2, SS_DOUBLE);   
    ssSetInputPortWidth(S, 2, 1);
    ssSetInputPortDirectFeedThrough(S, 2, 1);
    ssSetInputPortRequiredContiguous(S, 2, 1);	

    ssSetInputPortDataType(S, 3, SS_DOUBLE);   
    ssSetInputPortWidth(S, 3, 1);
    ssSetInputPortDirectFeedThrough(S, 3, 1);
    ssSetInputPortRequiredContiguous(S, 3, 1);	

    ssSetInputPortDataType(S, 4, SS_DOUBLE);   
    ssSetInputPortWidth(S, 4, 1);
    ssSetInputPortDirectFeedThrough(S, 4, 1);
    ssSetInputPortRequiredContiguous(S, 4, 1);

    ssSetInputPortDataType(S, 5, SS_DOUBLE);   
    ssSetInputPortWidth(S, 5, 1);
    ssSetInputPortDirectFeedThrough(S, 5, 1);
    ssSetInputPortRequiredContiguous(S, 5, 1);	

    ssSetInputPortDataType(S, 6, SS_DOUBLE);   
    ssSetInputPortWidth(S, 6, 1);
    ssSetInputPortDirectFeedThrough(S, 6, 1);
    ssSetInputPortRequiredContiguous(S, 6, 1);	

    ssSetInputPortDataType(S, 7, SS_DOUBLE);   
    ssSetInputPortWidth(S, 7, 1);
    ssSetInputPortDirectFeedThrough(S, 7, 1);
    ssSetInputPortRequiredContiguous(S, 7, 1);	

    ssSetInputPortDataType(S, 8, SS_DOUBLE);   
    ssSetInputPortWidth(S, 8, 1);
    ssSetInputPortDirectFeedThrough(S, 8, 1);
    ssSetInputPortRequiredContiguous(S, 8, 1);	

    /* 设置输出端口数量 */
    if (!ssSetNumOutputPorts(S, 19)) return;

    /* 配置输出端口 */
    ssSetOutputPortDataType(S, 0, SS_DOUBLE);
    ssSetOutputPortWidth(S, 0, 1);
    
    ssSetOutputPortDataType(S, 1, SS_DOUBLE);
    ssSetOutputPortWidth(S, 1, 1);

    ssSetOutputPortDataType(S, 2, SS_DOUBLE);
    ssSetOutputPortWidth(S, 2, 1);
    
    ssSetOutputPortDataType(S, 3, SS_DOUBLE);
    ssSetOutputPortWidth(S, 3, 1);

    ssSetOutputPortDataType(S, 4, SS_DOUBLE);
    ssSetOutputPortWidth(S, 4, 1);

    ssSetOutputPortDataType(S, 5, SS_DOUBLE);
    ssSetOutputPortWidth(S, 5, 1);

    ssSetOutputPortDataType(S, 6, SS_DOUBLE);
    ssSetOutputPortWidth(S, 6, 1);

    ssSetOutputPortDataType(S, 7, SS_DOUBLE);
    ssSetOutputPortWidth(S, 7, 1);

    ssSetOutputPortDataType(S, 8, SS_DOUBLE);
    ssSetOutputPortWidth(S, 8, 1);

    ssSetOutputPortDataType(S, 9, SS_DOUBLE);
    ssSetOutputPortWidth(S, 9, 1);

    ssSetOutputPortDataType(S, 10, SS_DOUBLE);
    ssSetOutputPortWidth(S, 10, 1);

    ssSetOutputPortDataType(S, 11, SS_DOUBLE);
    ssSetOutputPortWidth(S, 11, 1);

    ssSetOutputPortDataType(S, 12, SS_DOUBLE);
    ssSetOutputPortWidth(S, 12, 1);

    ssSetOutputPortDataType(S, 13, SS_DOUBLE);
    ssSetOutputPortWidth(S, 13, 1);

    ssSetOutputPortDataType(S, 14, SS_DOUBLE);
    ssSetOutputPortWidth(S, 14, 1);

    ssSetOutputPortDataType(S, 15, SS_DOUBLE);
    ssSetOutputPortWidth(S, 15, 1);

    ssSetOutputPortDataType(S, 16, SS_DOUBLE);
    ssSetOutputPortWidth(S, 16, 1);

    ssSetOutputPortDataType(S, 17, SS_DOUBLE);
    ssSetOutputPortWidth(S, 17, 1);

    ssSetOutputPortDataType(S, 18, SS_DOUBLE);
    ssSetOutputPortWidth(S, 18, 1);
}

/* 模块采样时间初始化函敿 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
	/* 设置采样时间为从连接的端口继承采样时间 */
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
	/* 设置采样偏移时间 */
    ssSetOffsetTime(S, 0, FIXED_IN_MINOR_STEP_OFFSET);
}

/* 模块输出函数 */
static void mdlOutputs(SimStruct *S, int_T tid){
    /* 获取参数/输入/输出的数据 */
    real_T* iTheta = (real_T*) ssGetInputPortSignal(S, 0);
    real_T* iId = (real_T*) ssGetInputPortSignal(S, 1);
    real_T* iIq = (real_T*) ssGetInputPortSignal(S, 2);
    real_T* iIa = (real_T*) ssGetInputPortSignal(S, 3);
    real_T* iIc = (real_T*) ssGetInputPortSignal(S, 4);
    real_T* iUd = (real_T*) ssGetInputPortSignal(S, 5);
    real_T* iUq = (real_T*) ssGetInputPortSignal(S, 6);
    real_T* iMode = (real_T*) ssGetInputPortSignal(S, 7);
    real_T* iSpd = (real_T*) ssGetInputPortSignal(S, 8);
    
    real_T* iNp = (real_T*) ssGetRunTimeParamInfo(S, 0)->data;
    real_T* CurKp = (real_T*) ssGetRunTimeParamInfo(S, 1)->data;
    real_T* CurKi = (real_T*) ssGetRunTimeParamInfo(S, 2)->data;
    real_T* VolMax = (real_T*) ssGetRunTimeParamInfo(S, 3)->data;
    real_T* SpdKp = (real_T*) ssGetRunTimeParamInfo(S, 4)->data;
    real_T* SpdKi = (real_T*) ssGetRunTimeParamInfo(S, 5)->data;
    real_T* CurMax = (real_T*) ssGetRunTimeParamInfo(S, 6)->data;
    real_T* Alpha = (real_T*) ssGetRunTimeParamInfo(S, 7)->data;

    real_T* oSinTheta = (real_T*) ssGetOutputPortSignal(S, 0);
    real_T* oCosTheta = (real_T*) ssGetOutputPortSignal(S, 1);
    real_T* oUx = (real_T*) ssGetOutputPortSignal(S, 2);
    real_T* oUy = (real_T*) ssGetOutputPortSignal(S, 3);
    real_T* oU1 = (real_T*) ssGetOutputPortSignal(S, 4);
    real_T* oU2 = (real_T*) ssGetOutputPortSignal(S, 5);
    real_T* oU3 = (real_T*) ssGetOutputPortSignal(S, 6);
    real_T* oSector = (real_T*) ssGetOutputPortSignal(S, 7);
    real_T* oCCRa = (real_T*) ssGetOutputPortSignal(S, 8);
    real_T* oCCRb = (real_T*) ssGetOutputPortSignal(S, 9);
    real_T* oCCRc = (real_T*) ssGetOutputPortSignal(S, 10);
    real_T* oIx = (real_T*) ssGetOutputPortSignal(S, 11);
    real_T* oIy = (real_T*) ssGetOutputPortSignal(S, 12);
    real_T* oId = (real_T*) ssGetOutputPortSignal(S, 13);
    real_T* oIq = (real_T*) ssGetOutputPortSignal(S, 14);
    real_T* oUd = (real_T*) ssGetOutputPortSignal(S, 15);
    real_T* oUq = (real_T*) ssGetOutputPortSignal(S, 16);
    real_T* oSpd = (real_T*) ssGetOutputPortSignal(S, 17);
    real_T* oTest = (real_T*) ssGetOutputPortSignal(S, 18);

    D_PI.Kp    = *CurKp;
    D_PI.Ki    = *CurKi;
    D_PI.Max   = *VolMax;
    
    Q_PI.Kp    = *CurKp;
    Q_PI.Ki    = *CurKi;
    Q_PI.Max   = *VolMax;
    
    Spd_PI.Kp  = *SpdKp;
    Spd_PI.Ki  = *SpdKi;
    Spd_PI.Max = *CurMax;

    DataIO.Mode = (uint8_t)(*iMode);

    DataIO.Theta = GetTheta(*iTheta); 
    DataIO.Np = (uint8_t)(*iNp);
    DataIO.Ia = GetCur(*iIa);
    DataIO.Ic = GetCur(*iIc);

    DataIO.Alpha = (uint16_t)(*Alpha);

    if(DataIO.Mode == 0){
        DataIO.TargetUd = (int16_t)(*iUd);
        DataIO.TargetUq = (int16_t)(*iUq);
    }
    else if(DataIO.Mode == 1){
        DataIO.TargetId = (int16_t)(*iId);
        DataIO.TargetIq = (int16_t)(*iIq);
    }
    else if(DataIO.Mode == 2){
        DataIO.TargetSpd = (int32_t)(*iSpd);
    }
    
    /* 调用函数接口 */
    FOC(&D_PI, &Q_PI, &Spd_PI, &DataIO);

    *oSinTheta = DataIO.SinTheta;
    *oCosTheta = DataIO.CosTheta;
    *oUx = DataIO.Ux;
    *oUy = DataIO.Uy;
    *oU1 = DataIO.U1;
    *oU2 = DataIO.U2;
    *oU3 = DataIO.U3;
    *oSector = DataIO.Sector;
    *oCCRa = DataIO.CCRa;
    *oCCRb = DataIO.CCRb;
    *oCCRc = DataIO.CCRc;
    *oIx = DataIO.Ix;
    *oIy = DataIO.Iy;
    *oId = DataIO.PresentId;
    *oIq = DataIO.PresentIq;
    *oUd = DataIO.PresentUd;
    *oUq = DataIO.PresentUq;
    *oSpd = DataIO.PresentSpd;
    *oTest = DataIO.Test;
}

/* 用于存储全局变量和运行时参数，在确定端口的宽度和采样时间后调用 */
#define MDL_SET_WORK_WIDTHS
#if defined(MDL_SET_WORK_WIDTHS) && defined(MATLAB_MEX_FILE)
static void mdlSetWorkWidths(SimStruct *S)
{
     /* 设置运行时参数的数量 */
    if (!ssSetNumRunTimeParams(S, 8)) return;

    /* 注册参数 */
    ssRegDlgParamAsRunTimeParam(S, 0,  0,  "Np",     ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S, 1,  1,  "CurKp",  ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S, 2,  2,  "CurKi",  ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S, 3,  3,  "VolMax", ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S, 4,  4,  "SpdKp",  ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S, 5,  5,  "SpdKi",  ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S, 6,  6,  "CurMax", ssGetDataTypeId(S, "double"));
    ssRegDlgParamAsRunTimeParam(S, 7,  7,  "Alpha",  ssGetDataTypeId(S, "double"));
}
#endif

/* 模块结束函数 */
static void mdlTerminate(SimStruct *S)
{
    
}

#ifdef MATLAB_MEX_FILE
/* 被Mex-file编译 */
#include "simulink.c"
#else
/* 不被Mex-file编译 */
#include "cg_sfun.h"
#endif
