#define S_FUNCTION_NAME  FOCSF
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "FOC.h"

PI_str D_PI  = {0};
PI_str Q_PI  = {0};
PI_str Spd_PI  = {0};

uint8_t Spd_Tick = 0;

/* 模块初始化函数 */
static void mdlInitializeSizes(SimStruct *S)
{
    D_PI.ui  = 0;
    Q_PI.ui  = 0;
    Spd_PI.ui  = 0;

    Spd_Tick = 0;
    /* 设置参数数量 */
    ssSetNumSFcnParams(S, 1);

    ssSetSFcnParamTunable(S, 0, 1);

    /* 设置输入端口数量 */
    if (!ssSetNumInputPorts(S, 5)) return;

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

    /* 设置输出端口数量 */
    if (!ssSetNumOutputPorts(S, 18)) return;

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
    
    real_T* iNp = (real_T*) ssGetRunTimeParamInfo(S, 0)->data;

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

    D_PI.Kp     = 1103;
    D_PI.Ki     = 44;
    D_PI.Max    = 32767;
    
    Q_PI.Kp     = 1103;
    Q_PI.Ki     = 44;
    Q_PI.Max    = 32767;
    
    Spd_PI.Kp     = 6064;
    Spd_PI.Ki     = 381;
    Spd_PI.Max    = 20971;

    /* 调用函数接口 */
    FOC(&D_PI,   &Q_PI,  &Spd_PI,    &Spd_Tick,
        *iTheta, *iNp,    oSinTheta,  oCosTheta, 
        *iId,    *iIq,    oUx,        oUy,
         oU1,     oU2,    oU3,        oSector,
         oCCRa,   oCCRb,  oCCRc,
        *iIa,    *iIc,    oIx,        oIy,
         oId,     oIq,    oUd,        oUq,
         oSpd);
}

/* 用于存储全局变量和运行时参数，在确定端口的宽度和采样时间后调用 */
#define MDL_SET_WORK_WIDTHS
#if defined(MDL_SET_WORK_WIDTHS) && defined(MATLAB_MEX_FILE)
static void mdlSetWorkWidths(SimStruct *S)
{
     /* 设置运行时参数的数量 */
    if (!ssSetNumRunTimeParams(S, 1)) return;

    /* 注册参数 */
    ssRegDlgParamAsRunTimeParam(S, 0,  0,  "Np",   ssGetDataTypeId(S, "double"));
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
