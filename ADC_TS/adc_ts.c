

#include <stdio.h>
#include "adc_ts.h"
#include "s3c24xx.h"
#include "serial.h"

//ADCCON
#define PRESCALE_DIS		(0 << 14)
#define PRESCALE_EN         (1 << 14)
#define PRSCVL(x)           ((x) << 6)
#define ADC_INPUT(x)        ((x) << 3)
#define ADC_START           (1 << 0)
#define ADC_ENDCVT          (1 << 15)

//ADCTSC
#define UD_SEN          (1 << 8)
#define DOWN_INT        (UD_SEN*0)
#define UP_INT          (UD_SEN*1)
#define YM_SEN          (1 << 7)
#define YM_HIZ          (YM_SEN*0)
#define YM_GND          (YM_SEN*1)
#define YP_SEN          (1 << 6)
#define YP_EXTVLT       (YP_SEN*0)
#define YP_AIN          (YP_SEN*1)
#define XM_SEN          (1 << 5)
#define XM_HIZ          (XM_SEN*0)
#define XM_GND          (XM_SEN*1)
#define XP_SEN          (1 << 4)
#define XP_EXTVLT       (XP_SEN*0)
#define XP_AIN          (XP_SEN*1)
#define XP_PULL_UP      (1 << 3)
#define XP_PULL_UP_EN   (XP_PULL_UP*0)
#define XP_PULL_UP_DIS  (XP_PULL_UP*1)
#define AUTO_PST        (1 << 2)
#define CONVERT_MAN     (AUTO_PST*0)
#define CONVERT_AUTO    (AUTO_PST*1)
#define XP_PST(x)       (x << 0)

#define NOP_MODE        0
#define X_AXIS_MODE     1
#define Y_AXIS_MODE     2
#define WAIT_INT_MODE   3

/* 设置进入等待中断模式，XP_PU,XP_Dis,XM_Dis,YP_Dis,YM_En
 * (1)对于S3C2410，位[8]只能为0，所以只能使用下面的wait_down_int，
 *    它既等待Pen Down中断，也等待Pen Up中断
 * (2)对于S3C2440，位[8]为0、1时分别表示等待Pen Down中断或Pen Up中断
 */
/* 进入"等待中断模式"，等待触摸屏被按下 */
#define wait_down_int() { ADCTSC = DOWN_INT | XP_PULL_UP_EN | \
                          XP_AIN | XM_HIZ | YP_AIN | YM_GND | \
                          XP_PST(WAIT_INT_MODE); }
/* 进入"等待中断模式"，等待触摸屏被松开 */
#define wait_up_int()   { ADCTSC = UP_INT | XP_PULL_UP_EN | XP_AIN | XM_HIZ | \
                          YP_AIN | YM_GND | XP_PST(WAIT_INT_MODE); }

/* 进入自动(连续) X/Y轴坐标转换模式 */
#define mode_auto_xy()  { ADCTSC = CONVERT_AUTO | XP_PULL_UP_DIS | XP_PST(NOP_MODE); }


extern void (*isr_handle_array[50])(void);


/*
* 使用查询的方式读取AD值
*/
static int ReadAdc(int ch)
{
	ADCCON = PRESCALE_EN | PRSCVL(49) | ADC_INPUT(ch);
	
	ADCTSC &= ~(1<<2);
	
	ADCCON |= ADC_START;
	
	while (!(ADCCON & ADC_ENDCVT));
	
	return (ADCDAT0 & 0x3FF);
}

/*
* 测试ADC
*/
void Test_Adc(void)
{
	float volt0, volt1;
	int t0, t1;
	
	printf("Measuring the voltage of AIN0 and AIN1, press any key to exit\n\r");
	while (!awaitkey(0))
	{
		volt0 = ((float)ReadAdc(0)*3.3)/1024.0;
		volt1 = ((float)ReadAdc(1)*3.3)/1024.0;
		t0 = (volt0 - (int)volt0) * 1000;
		t1 = (volt1 - (int)volt1) * 1000;
		printf("AIN0 = %d.%-3dV		AIN1 = %d%-3dV\r",(int)volt0, t0, (int)volt1, t1);
	}
	printf("\n");
}

static void Isr_Tc(void)
{
	if (ADCDAT0 & 0x8000)
	{
		printf("Stylus Up!!\n\r");
		wait_down_int();
	}
	else
	{
		printf("Stylus Down: ");
		mode_auto_xy();
		
		ADCCON |= ADC_START;
	}
	
	SUBSRCPND |= BIT_SUB_TC;
	SRCPND |= BIT_ADC;
	INTPND |= BIT_ADC;
}

static void Isr_Adc(void)
{
	printf("sdata = %4d, ydata = %4d\r\n",(int)(ADCDAT0 & 0x3FF), (int)(ADCDAT1 & 0x3FF));
	
	if ((GSTATUS1 == 0x32410000) || (GSTATUS1 == 0x32410002))
	{
		wait_down_int();
	}
	else
	{
		wait_up_int();
	}
	
	SUBSRCPND |= BIT_SUB_ADC;
	SRCPND |= BIT_ADC;
	INTPND |= BIT_ADC;
}

void AdcTsIntHandle(void)
{
	if (SUBSRCPND & BIT_SUB_TC)
	{
		Isr_Tc();
	}
	if (SUBSRCPND & BIT_SUB_ADC)
	{
		Isr_Adc();
	}
}

void Test_Ts(void)
{
	isr_handle_array[ISR_ADC_OFT] = AdcTsIntHandle;
	INTMSK &= ~BIT_ADC;
	INTSUBMSK &= ~(BIT_SUB_TC);
	INTSUBMSK &= ~(BIT_SUB_ADC);
	
	ADCCON = PRESCALE_EN | PRSCVL(49);
	
	ADCDLY = 50000;
	
	wait_down_int();
	
	printf("Touch the screem to test, press any key to exit\n\r");
	getc();
	
	INTSUBMSK |= BIT_SUB_TC;
	INTSUBMSK |= BIT_SUB_ADC;
	INTMSK |= BIT_ADC;
}
