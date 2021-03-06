#include "F28x_Project.h"
#include "math.h"
#define fc 5000

#define DEAD_TIME_RISE 200
#define DEAD_TIME_FALL 200
#define pi 3.14159265359

float32 va[512], vb[512], vc[512], ia[101], ib[101], ic[101], EPWM1_CMPA = 0, EPWM2_CMPA = 0, EPWM3_CMPA = 0, EPWM4_CMPA = 0, EPWM5_CMPA = 0, EPWM6_CMPA = 0,EPWM7_CMPA = 0,EPWM8_CMPA = 0,EPWM9_CMPA = 0,EPWM10_CMPA = 0,EPWM11_CMPA = 0,EPWM12_CMPA = 0, Adc_results_vc1 = 0, Adc_results_vc2 = 0, Adc_results_ia = 0, Adc_results_ib = 0, Adc_results_ic = 0;
Uint16 index = 0, count = 0;
float32 t = 0, m = 0,TIMER_PERIOD=0,fc=0,i=0,tc=0,fs=0;

void Initialize_PWM1(void);
void Initialize_PWM2(void);
void Initialize_PWM3(void);
void Initialize_PWM4(void);
void Initialize_PWM5(void);
void Initialize_PWM6(void);
void Initialize_PWM7(void);
void Initialize_PWM8(void);
void Initialize_PWM9(void);
void Initialize_PWM10(void);
void Initialize_PWM11(void);
void Initialize_PWM12(void);
void Initialize_adc(void);
void GPIO_select(void);
__interrupt void epwm1_isr(void);

void main(void)
{

    InitSysCtrl();

    InitEPwmGpio();

    DINT;

    InitPieCtrl();

    IER = 0x0000;
    IFR = 0x0000;

    InitPieVectTable();

    EALLOW;
    // This is needed to write to EALLOW protected registers
    PieVectTable.EPWM1_INT = &epwm1_isr;

    EDIS;
    // This is needed to disable write to EALLOW protected registers

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

/////////////////////////////////////////////////Calculate Reference Signals/////////////////////////////////////////////////////////////////////////////////
    t = 0;
       fs=50;
       fc=10000;
       m =0.8 ;
       TIMER_PERIOD= (100e6/(2*fc))/2;
       i = fc/fs;
       tc= (1/(float32)fc);
       for (index = 0; index <= i; index++)
       {
           va[index] = (m *2*cos(100* pi * t)) + 2;
           vb[index] = (m * 2* cos(100 * pi * t - 2 * pi / 3)) + 2;
           vc[index] = (m * 2*cos(100 * pi * t - 4 * pi / 3)) + 2;
           t= index *tc;
       }
       index = 0;
       t = 0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Initialize_PWM1();
    Initialize_PWM2();
    Initialize_PWM3();
    Initialize_PWM4();
    Initialize_PWM5();
    Initialize_PWM6();
    Initialize_PWM7();
    Initialize_PWM8();
    Initialize_PWM9();
    Initialize_PWM10();
    Initialize_PWM11();
    Initialize_PWM12();


    Initialize_adc();
    GPIO_select();

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

    IER |= M_INT3;

    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;

    EINT;
    ERTM;
    // Enable Global real-time interrupt DBGM
//    while(1)
//    {
//        GpioDataRegs.GPASET.bit.GPIO20 = 1;
//    }
}

void Initialize_PWM1(void)
{

    // TBPRD Register
    EPwm1Regs.TBPRD = TIMER_PERIOD;
    EPwm1Regs.TBCTR = 0x0000;
    EPwm1Regs.TBPHS.bit.TBPHS = 0x0000;

    // TBCTL (Time-Base Control)
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;
    EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm1Regs.TBCTL.bit.SYNCOSEL = 0x1; //Generate Sync Signal for EPWM2...

    // CMPCTL (Compare Control)
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    // CMPA Register
    EPwm1Regs.CMPA.bit.CMPA = 0;

    // AQCTL Register
    EPwm1Regs.AQCTL.bit.LDAQAMODE = 0x0;
    EPwm1Regs.AQCTL.bit.LDAQBMODE = 0x0;
    EPwm1Regs.AQCTL.bit.SHDWAQAMODE = 0x1;
    EPwm1Regs.AQCTL.bit.SHDWAQBMODE = 0x1;

// AQCTLA and AQCTLB (Action Qualifier Control)
    EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm1Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm1Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm1Regs.AQCTLA.bit.CBD = AQ_NO_ACTION;

    EPwm1Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
    EPwm1Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm1Regs.AQCTLB.bit.CAD = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm1Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;

// DBCTL (Dead-Band Control)
/*    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwm1Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm1Regs.DBCTL.bit.HALFCYCLE = 0x0;

// DBRED Register
    EPwm1Regs.DBRED.bit.DBRED = DEAD_TIME_RISE;

// DBFED Register
    EPwm1Regs.DBFED.bit.DBFED = DEAD_TIME_FALL; */

// EPWM Interrupt
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
    EPwm1Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;

}

void Initialize_PWM2(void)
{

    // TBPRD Register
    EPwm2Regs.TBPRD = TIMER_PERIOD;
    EPwm2Regs.TBCTR = 0x0000;
    EPwm2Regs.TBPHS.bit.TBPHS = TIMER_PERIOD/2;

    // TBCTL (Time-Base Control)
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    EPwm2Regs.TBCTL.bit.SYNCOSEL = 0x0;
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm2Regs.TBCTL.bit.PHSDIR = 0x0; //Count down after Synchronization.

    // CMPCTL (Compare Control)
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    // CMPA Register
    EPwm2Regs.CMPA.bit.CMPA = 0;

    // AQCTL Register
    EPwm2Regs.AQCTL.bit.LDAQAMODE = 0x0;
    EPwm2Regs.AQCTL.bit.LDAQBMODE = 0x0;
    EPwm2Regs.AQCTL.bit.SHDWAQAMODE = 0x1;
    EPwm2Regs.AQCTL.bit.SHDWAQBMODE = 0x1;

// AQCTLA and AQCTLB (Action Qualifier Control)
    EPwm2Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm2Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm2Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm2Regs.AQCTLA.bit.CBD = AQ_NO_ACTION;

    EPwm2Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
    EPwm2Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
    EPwm2Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm2Regs.AQCTLB.bit.CAD = AQ_CLEAR;
    EPwm2Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm2Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;

// DBCTL (Dead-Band Control)
 /*   EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwm2Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm2Regs.DBCTL.bit.HALFCYCLE = 0x0;

// DBRED Register
    EPwm2Regs.DBRED.bit.DBRED = DEAD_TIME_RISE;

// DBFED Register
    EPwm2Regs.DBFED.bit.DBFED = DEAD_TIME_FALL; */

// EPWM2 SOCA Enable
    EPwm2Regs.ETSEL.bit.SOCAEN = 1;
    EPwm2Regs.ETSEL.bit.SOCASEL = 1;
    EPwm2Regs.ETPS.bit.SOCAPRD = 1;
}

void Initialize_PWM3(void)
{

    // TBPRD Register
    EPwm3Regs.TBPRD = TIMER_PERIOD;
    EPwm3Regs.TBCTR = 0x0000;
    EPwm3Regs.TBPHS.bit.TBPHS = TIMER_PERIOD;

    // TBCTL (Time-Base Control)
    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    EPwm3Regs.TBCTL.bit.SYNCOSEL = 0x0;
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm3Regs.TBCTL.bit.PHSDIR = 0x0; //Count down after Synchronization.

    // CMPCTL (Compare Control)
    EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    // CMPA Register
    EPwm3Regs.CMPA.bit.CMPA = 0;

    // AQCTL Register
    EPwm3Regs.AQCTL.bit.LDAQAMODE = 0x0;
    EPwm3Regs.AQCTL.bit.LDAQBMODE = 0x0;
    EPwm3Regs.AQCTL.bit.SHDWAQAMODE = 0x1;
    EPwm3Regs.AQCTL.bit.SHDWAQBMODE = 0x1;

// AQCTLA and AQCTLB (Action Qualifier Control)
    EPwm3Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm3Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm3Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm3Regs.AQCTLA.bit.CBD = AQ_NO_ACTION;

    EPwm3Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
    EPwm3Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
    EPwm3Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm3Regs.AQCTLB.bit.CAD = AQ_CLEAR;
    EPwm3Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm3Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;

// DBCTL (Dead-Band Control)
 /*   EPwm3Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm3Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwm3Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm3Regs.DBCTL.bit.HALFCYCLE = 0x0;

// DBRED Register
    EPwm3Regs.DBRED.bit.DBRED = DEAD_TIME_RISE;

// DBFED Register
    EPwm3Regs.DBFED.bit.DBFED = DEAD_TIME_FALL; */
}

void Initialize_PWM4(void)
{

    // TBPRD Register
    EPwm4Regs.TBPRD = TIMER_PERIOD;
    EPwm4Regs.TBCTR = 0x0000;
    EPwm4Regs.TBPHS.bit.TBPHS =TIMER_PERIOD/2;

    // TBCTL (Time-Base Control)
    EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm4Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    EPwm4Regs.TBCTL.bit.SYNCOSEL = 0x0;
    EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm4Regs.TBCTL.bit.PHSDIR = 0x1; //Count up after Synchronization.


    // CMPCTL (Compare Control)
    EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    // CMPA Register
    EPwm4Regs.CMPA.bit.CMPA = 0;

    // AQCTL Register
    EPwm4Regs.AQCTL.bit.LDAQAMODE = 0x0;
    EPwm4Regs.AQCTL.bit.LDAQBMODE = 0x0;
    EPwm4Regs.AQCTL.bit.SHDWAQAMODE = 0x1;
    EPwm4Regs.AQCTL.bit.SHDWAQBMODE = 0x1;

// AQCTLA and AQCTLB (Action Qualifier Control)
    EPwm4Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm4Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm4Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm4Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm4Regs.AQCTLA.bit.CBD = AQ_NO_ACTION;

    EPwm4Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
    EPwm4Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
    EPwm4Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm4Regs.AQCTLB.bit.CAD = AQ_CLEAR;
    EPwm4Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm4Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;

// DBCTL (Dead-Band Control)
/*    EPwm4Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm4Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwm4Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm4Regs.DBCTL.bit.HALFCYCLE = 0x0;

// DBRED Register
    EPwm4Regs.DBRED.bit.DBRED = DEAD_TIME_RISE;

// DBFED Register
    EPwm4Regs.DBFED.bit.DBFED = DEAD_TIME_FALL; */
}


/*------------------------------PWM PHA B--------------------------------------------*/

void Initialize_PWM5(void)
{

    // TBPRD Register
    EPwm5Regs.TBPRD = TIMER_PERIOD;
    EPwm5Regs.TBCTR = 0x0000;
    EPwm5Regs.TBPHS.bit.TBPHS = 0x0000;

    // TBCTL (Time-Base Control)
    EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm5Regs.TBCTL.bit.PHSEN = TB_DISABLE;
    EPwm5Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm5Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm5Regs.TBCTL.bit.SYNCOSEL = 0x1; //Generate Sync Signal for EPWM2...

    // CMPCTL (Compare Control)
    EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    // CMPA Register
    EPwm5Regs.CMPA.bit.CMPA = 0;

    // AQCTL Register
    EPwm5Regs.AQCTL.bit.LDAQAMODE = 0x0;
    EPwm5Regs.AQCTL.bit.LDAQBMODE = 0x0;
    EPwm5Regs.AQCTL.bit.SHDWAQAMODE = 0x1;
    EPwm5Regs.AQCTL.bit.SHDWAQBMODE = 0x1;

// AQCTLA and AQCTLB (Action Qualifier Control)
    EPwm5Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm5Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm5Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm5Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm5Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm5Regs.AQCTLA.bit.CBD = AQ_NO_ACTION;

    EPwm5Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
    EPwm5Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
    EPwm5Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm5Regs.AQCTLB.bit.CAD = AQ_CLEAR;
    EPwm5Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm5Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;

// DBCTL (Dead-Band Control)
/*    EPwm5Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm5Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwm5Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm5Regs.DBCTL.bit.HALFCYCLE = 0x0;

// DBRED Register
    EPwm5Regs.DBRED.bit.DBRED = DEAD_TIME_RISE;

// DBFED Register
    EPwm5Regs.DBFED.bit.DBFED = DEAD_TIME_FALL; */

// EPWM Interrupt
    EPwm5Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
    EPwm5Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm5Regs.ETPS.bit.INTPRD = ET_1ST;

}

void Initialize_PWM6(void)
{

    // TBPRD Register
    EPwm6Regs.TBPRD = TIMER_PERIOD;
    EPwm6Regs.TBCTR = 0x0000;
    EPwm6Regs.TBPHS.bit.TBPHS = TIMER_PERIOD/2;

    // TBCTL (Time-Base Control)
    EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm6Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm6Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    EPwm6Regs.TBCTL.bit.SYNCOSEL = 0x0;
    EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm6Regs.TBCTL.bit.PHSDIR = 0x0; //Count down after Synchronization.

    // CMPCTL (Compare Control)
    EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    // CMPA Register
    EPwm6Regs.CMPA.bit.CMPA = 0;

    // AQCTL Register
    EPwm6Regs.AQCTL.bit.LDAQAMODE = 0x0;
    EPwm6Regs.AQCTL.bit.LDAQBMODE = 0x0;
    EPwm6Regs.AQCTL.bit.SHDWAQAMODE = 0x1;
    EPwm6Regs.AQCTL.bit.SHDWAQBMODE = 0x1;

// AQCTLA and AQCTLB (Action Qualifier Control)
    EPwm6Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm6Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm6Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm6Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm6Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm6Regs.AQCTLA.bit.CBD = AQ_NO_ACTION;

    EPwm6Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
    EPwm6Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
    EPwm6Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm6Regs.AQCTLB.bit.CAD = AQ_CLEAR;
    EPwm6Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm6Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;

// DBCTL (Dead-Band Control)
 /*   EPwm6Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm6Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwm6Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm6Regs.DBCTL.bit.HALFCYCLE = 0x0;

// DBRED Register
    EPwm6Regs.DBRED.bit.DBRED = DEAD_TIME_RISE;

// DBFED Register
    EPwm6Regs.DBFED.bit.DBFED = DEAD_TIME_FALL; */

// EPWM6 SOCA Enable
    EPwm6Regs.ETSEL.bit.SOCAEN = 1;
    EPwm6Regs.ETSEL.bit.SOCASEL = 1;
    EPwm6Regs.ETPS.bit.SOCAPRD = 1;
}

void Initialize_PWM7(void)
{

    // TBPRD Register
    EPwm7Regs.TBPRD = TIMER_PERIOD;
    EPwm7Regs.TBCTR = 0x0000;
    EPwm7Regs.TBPHS.bit.TBPHS = TIMER_PERIOD;

    // TBCTL (Time-Base Control)
    EPwm7Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm7Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm7Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    EPwm7Regs.TBCTL.bit.SYNCOSEL = 0x0;
    EPwm7Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm7Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm7Regs.TBCTL.bit.PHSDIR = 0x0; //Count down after Synchronization.

    // CMPCTL (Compare Control)
    EPwm7Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm7Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm7Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm7Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    // CMPA Register
    EPwm7Regs.CMPA.bit.CMPA = 0;

    // AQCTL Register
    EPwm7Regs.AQCTL.bit.LDAQAMODE = 0x0;
    EPwm7Regs.AQCTL.bit.LDAQBMODE = 0x0;
    EPwm7Regs.AQCTL.bit.SHDWAQAMODE = 0x1;
    EPwm7Regs.AQCTL.bit.SHDWAQBMODE = 0x1;

// AQCTLA and AQCTLB (Action Qualifier Control)
    EPwm7Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm7Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm7Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm7Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm7Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm7Regs.AQCTLA.bit.CBD = AQ_NO_ACTION;

    EPwm7Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
    EPwm7Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
    EPwm7Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm7Regs.AQCTLB.bit.CAD = AQ_CLEAR;
    EPwm7Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm7Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;

// DBCTL (Dead-Band Control)
 /*   EPwm7Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm7Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwm7Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm7Regs.DBCTL.bit.HALFCYCLE = 0x0;

// DBRED Register
    EPwm7Regs.DBRED.bit.DBRED = DEAD_TIME_RISE;

// DBFED Register
    EPwm7Regs.DBFED.bit.DBFED = DEAD_TIME_FALL; */
}

void Initialize_PWM8(void)
{

    // TBPRD Register
    EPwm8Regs.TBPRD = TIMER_PERIOD;
    EPwm8Regs.TBCTR = 0x0000;
    EPwm8Regs.TBPHS.bit.TBPHS =TIMER_PERIOD/2;

    // TBCTL (Time-Base Control)
    EPwm8Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm8Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm8Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    EPwm8Regs.TBCTL.bit.SYNCOSEL = 0x0;
    EPwm8Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm8Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm8Regs.TBCTL.bit.PHSDIR = 0x1; //Count up after Synchronization.


    // CMPCTL (Compare Control)
    EPwm8Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm8Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm8Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm8Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    // CMPA Register
    EPwm8Regs.CMPA.bit.CMPA = 0;

    // AQCTL Register
    EPwm8Regs.AQCTL.bit.LDAQAMODE = 0x0;
    EPwm8Regs.AQCTL.bit.LDAQBMODE = 0x0;
    EPwm8Regs.AQCTL.bit.SHDWAQAMODE = 0x1;
    EPwm8Regs.AQCTL.bit.SHDWAQBMODE = 0x1;

// AQCTLA and AQCTLB (Action Qualifier Control)
    EPwm8Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm8Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm8Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm8Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm8Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm8Regs.AQCTLA.bit.CBD = AQ_NO_ACTION;

    EPwm8Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
    EPwm8Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
    EPwm8Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm8Regs.AQCTLB.bit.CAD = AQ_CLEAR;
    EPwm8Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm8Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;

// DBCTL (Dead-Band Control)
/*  EPwm8Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm8Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwm8Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm8Regs.DBCTL.bit.HALFCYCLE = 0x0;

// DBRED Register
    EPwm8Regs.DBRED.bit.DBRED = DEAD_TIME_RISE;

// DBFED Register
    EPwm8Regs.DBFED.bit.DBFED = DEAD_TIME_FALL; */
}
/*---------------------------------PWM PHA C--------------------------*/

void Initialize_PWM9(void)
{

    // TBPRD Register
    EPwm9Regs.TBPRD = TIMER_PERIOD;
    EPwm9Regs.TBCTR = 0x0000;
    EPwm9Regs.TBPHS.bit.TBPHS = 0x0000;

    // TBCTL (Time-Base Control)
    EPwm9Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm9Regs.TBCTL.bit.PHSEN = TB_DISABLE;
    EPwm9Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    EPwm9Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm9Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm9Regs.TBCTL.bit.SYNCOSEL = 0x1; //Generate Sync Signal for EPWM2...

    // CMPCTL (Compare Control)
    EPwm9Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm9Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm9Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm9Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    // CMPA Register
    EPwm9Regs.CMPA.bit.CMPA = 0;

    // AQCTL Register
    EPwm9Regs.AQCTL.bit.LDAQAMODE = 0x0;
    EPwm9Regs.AQCTL.bit.LDAQBMODE = 0x0;
    EPwm9Regs.AQCTL.bit.SHDWAQAMODE = 0x1;
    EPwm9Regs.AQCTL.bit.SHDWAQBMODE = 0x1;

// AQCTLA and AQCTLB (Action Qualifier Control)
    EPwm9Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm9Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm9Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm9Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm9Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm9Regs.AQCTLA.bit.CBD = AQ_NO_ACTION;

    EPwm9Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
    EPwm9Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
    EPwm9Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm9Regs.AQCTLB.bit.CAD = AQ_CLEAR;
    EPwm9Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm9Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;

// DBCTL (Dead-Band Control)
/*  EPwm9Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm9Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwm9Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm9Regs.DBCTL.bit.HALFCYCLE = 0x0;

// DBRED Register
    EPwm9Regs.DBRED.bit.DBRED = DEAD_TIME_RISE;

// DBFED Register
    EPwm9Regs.DBFED.bit.DBFED = DEAD_TIME_FALL; */

// EPWM Interrupt
    EPwm9Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
    EPwm9Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm9Regs.ETPS.bit.INTPRD = ET_1ST;

}

void Initialize_PWM10(void)
{

    // TBPRD Register
    EPwm10Regs.TBPRD = TIMER_PERIOD;
    EPwm10Regs.TBCTR = 0x0000;
    EPwm10Regs.TBPHS.bit.TBPHS = TIMER_PERIOD/2;

    // TBCTL (Time-Base Control)
    EPwm10Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm10Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm10Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    EPwm10Regs.TBCTL.bit.SYNCOSEL = 0x0;
    EPwm10Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm10Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm10Regs.TBCTL.bit.PHSDIR = 0x0; //Count down after Synchronization.

    // CMPCTL (Compare Control)
    EPwm10Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm10Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm10Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm10Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    // CMPA Register
    EPwm10Regs.CMPA.bit.CMPA = 0;

    // AQCTL Register
    EPwm10Regs.AQCTL.bit.LDAQAMODE = 0x0;
    EPwm10Regs.AQCTL.bit.LDAQBMODE = 0x0;
    EPwm10Regs.AQCTL.bit.SHDWAQAMODE = 0x1;
    EPwm10Regs.AQCTL.bit.SHDWAQBMODE = 0x1;

// AQCTLA and AQCTLB (Action Qualifier Control)
    EPwm10Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm10Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm10Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm10Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm10Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm10Regs.AQCTLA.bit.CBD = AQ_NO_ACTION;

    EPwm10Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
    EPwm10Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
    EPwm10Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm10Regs.AQCTLB.bit.CAD = AQ_CLEAR;
    EPwm10Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm10Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;

// DBCTL (Dead-Band Control)
 /* EPwm10Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm10Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwm10Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm10Regs.DBCTL.bit.HALFCYCLE = 0x0;

// DBRED Register
    EPwm10Regs.DBRED.bit.DBRED = DEAD_TIME_RISE;

// DBFED Register
    EPwm10Regs.DBFED.bit.DBFED = DEAD_TIME_FALL; */

// EPWM10 SOCA Enable
    EPwm10Regs.ETSEL.bit.SOCAEN = 1;
    EPwm10Regs.ETSEL.bit.SOCASEL = 1;
    EPwm10Regs.ETPS.bit.SOCAPRD = 1;
}

void Initialize_PWM11(void)
{

    // TBPRD Register
    EPwm11Regs.TBPRD = TIMER_PERIOD;
    EPwm11Regs.TBCTR = 0x0000;
    EPwm11Regs.TBPHS.bit.TBPHS = TIMER_PERIOD;

    // TBCTL (Time-Base Control)
    EPwm11Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm11Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm11Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    EPwm11Regs.TBCTL.bit.SYNCOSEL = 0x0;
    EPwm11Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm11Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm11Regs.TBCTL.bit.PHSDIR = 0x0; //Count down after Synchronization.

    // CMPCTL (Compare Control)
    EPwm11Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm11Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm11Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm11Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    // CMPA Register
    EPwm11Regs.CMPA.bit.CMPA = 0;

    // AQCTL Register
    EPwm11Regs.AQCTL.bit.LDAQAMODE = 0x0;
    EPwm11Regs.AQCTL.bit.LDAQBMODE = 0x0;
    EPwm11Regs.AQCTL.bit.SHDWAQAMODE = 0x1;
    EPwm11Regs.AQCTL.bit.SHDWAQBMODE = 0x1;

// AQCTLA and AQCTLB (Action Qualifier Control)
    EPwm11Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm11Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm11Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm11Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm11Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm11Regs.AQCTLA.bit.CBD = AQ_NO_ACTION;

    EPwm11Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
    EPwm11Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
    EPwm11Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm11Regs.AQCTLB.bit.CAD = AQ_CLEAR;
    EPwm11Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm11Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;

// DBCTL (Dead-Band Control)
 /* EPwm11Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm11Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwm11Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm11Regs.DBCTL.bit.HALFCYCLE = 0x0;

// DBRED Register
    EPwm11Regs.DBRED.bit.DBRED = DEAD_TIME_RISE;

// DBFED Register
    EPwm11Regs.DBFED.bit.DBFED = DEAD_TIME_FALL; */
}

void Initialize_PWM12(void)
{

    // TBPRD Register
    EPwm12Regs.TBPRD = TIMER_PERIOD;
    EPwm12Regs.TBCTR = 0x0000;
    EPwm12Regs.TBPHS.bit.TBPHS =TIMER_PERIOD/2;

    // TBCTL (Time-Base Control)
    EPwm12Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
    EPwm12Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm12Regs.TBCTL.bit.PRDLD = TB_SHADOW;
    EPwm12Regs.TBCTL.bit.SYNCOSEL = 0x0;
    EPwm12Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm12Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm12Regs.TBCTL.bit.PHSDIR = 0x1; //Count up after Synchronization.


    // CMPCTL (Compare Control)
    EPwm12Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm12Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm12Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm12Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

    // CMPA Register
    EPwm12Regs.CMPA.bit.CMPA = 0;

    // AQCTL Register
    EPwm12Regs.AQCTL.bit.LDAQAMODE = 0x0;
    EPwm12Regs.AQCTL.bit.LDAQBMODE = 0x0;
    EPwm12Regs.AQCTL.bit.SHDWAQAMODE = 0x1;
    EPwm12Regs.AQCTL.bit.SHDWAQBMODE = 0x1;

// AQCTLA and AQCTLB (Action Qualifier Control)
    EPwm12Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm12Regs.AQCTLA.bit.PRD = AQ_NO_ACTION;
    EPwm12Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm12Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm12Regs.AQCTLA.bit.CBU = AQ_NO_ACTION;
    EPwm12Regs.AQCTLA.bit.CBD = AQ_NO_ACTION;

    EPwm12Regs.AQCTLB.bit.ZRO = AQ_CLEAR;
    EPwm12Regs.AQCTLB.bit.PRD = AQ_NO_ACTION;
    EPwm12Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm12Regs.AQCTLB.bit.CAD = AQ_CLEAR;
    EPwm12Regs.AQCTLB.bit.CBU = AQ_NO_ACTION;
    EPwm12Regs.AQCTLB.bit.CBD = AQ_NO_ACTION;

// DBCTL (Dead-Band Control)
/*  EPwm12Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    EPwm12Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    EPwm12Regs.DBCTL.bit.IN_MODE = DBA_ALL;
    EPwm12Regs.DBCTL.bit.HALFCYCLE = 0x0;

// DBRED Register
    EPwm12Regs.DBRED.bit.DBRED = DEAD_TIME_RISE;

// DBFED Register
    EPwm12Regs.DBFED.bit.DBFED = DEAD_TIME_FALL; */
}
void GPIO_select(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX2.all = 0x00000000;
    GpioCtrlRegs.GPADIR.all = 0xFFFFFFFF;

    //////
    GpioCtrlRegs.GPAGMUX2.bit.GPIO16 = 0x1; //EPWM9A
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0x1; //EPWM9A
    GpioCtrlRegs.GPAGMUX2.bit.GPIO18 = 0x1; //EPWM10A
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0x1; //EPWM10A
    GpioCtrlRegs.GPAGMUX2.bit.GPIO20 = 0x1; //EPWM11A
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0x1; //EPWM11A
    GpioCtrlRegs.GPAGMUX2.bit.GPIO22 = 0x1; //EPWM12A
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0x1; //EPWM12A

    ////
    EDIS;
    //GpioDataRegs.GPACLEAR.bit.GPIO16 = 1;
    //GpioDataRegs.GPASET.bit.GPIO16 = 1;
}

void Initialize_adc(void)
{
    EALLOW;
    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1; //Interrupt pulse generation occurs at the end of the conversion

    AdcaRegs.ADCCTL2.bit.PRESCALE = 7; //ADCCLK = Input Clock/4.5
    AdcbRegs.ADCCTL2.bit.PRESCALE = 7;
    AdccRegs.ADCCTL2.bit.PRESCALE = 7;
    AdcdRegs.ADCCTL2.bit.PRESCALE = 7;

    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 7; //ADCTRIG7-ePWM2, ADCSOCA
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 7;
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = 7;
    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL = 7;

    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 7;
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = 7;
    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL = 7;
    AdcbRegs.ADCSOC3CTL.bit.TRIGSEL = 7;

    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = 7;
    AdccRegs.ADCSOC1CTL.bit.TRIGSEL = 7;
    AdccRegs.ADCSOC2CTL.bit.TRIGSEL = 7;
    AdccRegs.ADCSOC3CTL.bit.TRIGSEL = 7;

    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = 7;
    AdcdRegs.ADCSOC1CTL.bit.TRIGSEL = 7;
    AdcdRegs.ADCSOC2CTL.bit.TRIGSEL = 7;
    AdcdRegs.ADCSOC3CTL.bit.TRIGSEL = 7;

    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 3;
    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 3;
    AdcaRegs.ADCSOC2CTL.bit.CHSEL = 3;
    AdcaRegs.ADCSOC3CTL.bit.CHSEL = 3;

    AdcbRegs.ADCSOC0CTL.bit.CHSEL = 0;
    AdcbRegs.ADCSOC1CTL.bit.CHSEL = 0;
    AdcbRegs.ADCSOC2CTL.bit.CHSEL = 0;
    AdcbRegs.ADCSOC3CTL.bit.CHSEL = 0;

    AdccRegs.ADCSOC0CTL.bit.CHSEL = 2;
    AdccRegs.ADCSOC1CTL.bit.CHSEL = 2;
    AdccRegs.ADCSOC2CTL.bit.CHSEL = 2;
    AdccRegs.ADCSOC3CTL.bit.CHSEL = 2;

    AdcdRegs.ADCSOC0CTL.bit.CHSEL = 0;
    AdcdRegs.ADCSOC1CTL.bit.CHSEL = 0;
    AdcdRegs.ADCSOC2CTL.bit.CHSEL = 0;
    AdcdRegs.ADCSOC3CTL.bit.CHSEL = 0;

    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1; // Power on ADC core
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1; // ADCINT1 is enabled
    AdcaRegs.ADCINTSEL1N2.bit.INT1CONT = 0; // No further ADCINT1 is generated until ADCINT1 flag is cleared by the user
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 3; // EOC3 is trigger for ADCINT1
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; // Clear the flag bit in ADCINTFLG Register

    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 127;  // Acquisition window 640ns
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = 127;
    AdcaRegs.ADCSOC2CTL.bit.ACQPS = 127;
    AdcaRegs.ADCSOC3CTL.bit.ACQPS = 127;

    AdcbRegs.ADCSOC0CTL.bit.ACQPS = 127;
    AdcbRegs.ADCSOC1CTL.bit.ACQPS = 127;
    AdcbRegs.ADCSOC2CTL.bit.ACQPS = 127;
    AdcbRegs.ADCSOC3CTL.bit.ACQPS = 127;

    AdccRegs.ADCSOC0CTL.bit.ACQPS = 127;
    AdccRegs.ADCSOC1CTL.bit.ACQPS = 127;
    AdccRegs.ADCSOC2CTL.bit.ACQPS = 127;
    AdccRegs.ADCSOC3CTL.bit.ACQPS = 127;

    AdcdRegs.ADCSOC0CTL.bit.ACQPS = 127;
    AdcdRegs.ADCSOC1CTL.bit.ACQPS = 127;
    AdcdRegs.ADCSOC2CTL.bit.ACQPS = 127;
    AdcdRegs.ADCSOC3CTL.bit.ACQPS = 127;
    EDIS;
}

__interrupt void epwm1_isr(void)
{
/*    GpioDataRegs.GPASET.bit.GPIO20 = 1;
    ////////Calculate Offset////////////////////
    if(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 1)
    {
        Adc_results_vc1 = (AdcaResultRegs.ADCRESULT0 + AdcaResultRegs.ADCRESULT1 + AdcaResultRegs.ADCRESULT2 + AdcaResultRegs.ADCRESULT3)/4;
        Adc_results_vc1 = (307.1277 * (Adc_results_vc1/4096)-2);

        Adc_results_vc2 = (AdcbResultRegs.ADCRESULT0 + AdcbResultRegs.ADCRESULT1 + AdcbResultRegs.ADCRESULT2 + AdcbResultRegs.ADCRESULT3)/4;
        Adc_results_vc2 = (329.25* (Adc_results_vc2/4096)-7.81);

        Adc_results_ia = (AdccResultRegs.ADCRESULT0 + AdccResultRegs.ADCRESULT1 + AdccResultRegs.ADCRESULT2 + AdccResultRegs.ADCRESULT3)/4;
        Adc_results_ia = (3.3 * (Adc_results_ia/4096));

        Adc_results_ib = (AdcdResultRegs.ADCRESULT0 + AdcdResultRegs.ADCRESULT1 + AdcdResultRegs.ADCRESULT2 + AdcdResultRegs.ADCRESULT3)/4;
        Adc_results_ib = (3.3 * (Adc_results_ib/4096));

        ia[count] = Adc_results_ia;
        ib[count] = Adc_results_ib;
        ic[count] = - Adc_results_ia - Adc_results_ib;
        if(count == 100)
        {
            count = 0;
        }
        else
        {
            count++;
        }

        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    }*/

    ///////////////////////////////////////////
    if ((va[index] >= 0) && (va[index] <4))
    {
        EPWM1_CMPA = TIMER_PERIOD*(va[index]/4);
        EPWM2_CMPA = TIMER_PERIOD*(va[index]/4);
        EPWM3_CMPA = TIMER_PERIOD*(va[index]/4);
        EPWM4_CMPA = TIMER_PERIOD*(va[index]/4);
    }

    if ((vb[index] >= 0) && (vb[index] <4))
        {
            EPWM5_CMPA = TIMER_PERIOD*(vb[index]/4);
            EPWM6_CMPA = TIMER_PERIOD*(vb[index]/4);
            EPWM7_CMPA = TIMER_PERIOD*(vb[index]/4);
            EPWM8_CMPA = TIMER_PERIOD*(vb[index]/4);
        }
    if ((vc[index] >= 0) && (vc[index] <4))
          {
              EPWM9_CMPA = TIMER_PERIOD*(vc[index]/4);
              EPWM10_CMPA = TIMER_PERIOD*(vc[index]/4);
              EPWM11_CMPA = TIMER_PERIOD*(vc[index]/4);
              EPWM12_CMPA = TIMER_PERIOD*(vc[index]/4);
          }


    EPwm1Regs.CMPA.bit.CMPA = EPWM1_CMPA;
    EPwm2Regs.CMPA.bit.CMPA = EPWM2_CMPA;
    EPwm3Regs.CMPA.bit.CMPA = EPWM3_CMPA;
    EPwm4Regs.CMPA.bit.CMPA = EPWM4_CMPA;
    EPwm5Regs.CMPA.bit.CMPA = EPWM5_CMPA;
    EPwm6Regs.CMPA.bit.CMPA = EPWM6_CMPA;
    EPwm7Regs.CMPA.bit.CMPA = EPWM7_CMPA;
    EPwm8Regs.CMPA.bit.CMPA = EPWM8_CMPA;
    EPwm9Regs.CMPA.bit.CMPA = EPWM9_CMPA;
    EPwm10Regs.CMPA.bit.CMPA = EPWM10_CMPA;
    EPwm11Regs.CMPA.bit.CMPA = EPWM11_CMPA;
    EPwm12Regs.CMPA.bit.CMPA = EPWM12_CMPA;

    if (index == 100)
    {
        index = 0;
    }
    else
    {
        index++;
    }
   /* EPwm1Regs.CMPA.bit.CMPA = 7500;
    EPwm2Regs.CMPA.bit.CMPA = 7500;
    EPwm3Regs.CMPA.bit.CMPA = 7500;
    EPwm4Regs.CMPA.bit.CMPA = 7500;
    */
    EPwm1Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    GpioDataRegs.GPACLEAR.bit.GPIO20 = 1;
}

