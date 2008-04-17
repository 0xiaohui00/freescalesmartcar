/*
 * Copyright (c) 1998, Xiongfei Guo, Shanghai Second Polytechnic University 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Shanghai Second Polytechnic University nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 

/******************************************************************************
  hPWM.c
  PWM ����
******************************************************************************/
#include "includes.h"



/* ע�� *   ������Ƶ��Ϊ8MHzʱ, ��PWMCLK_PCLK1 = 0x0,ѡ��ʱ��A,
            PWMPRCLK_PCKA = 0x3,PWMһ�����ڵ���С�ֶ�Ϊһ΢��
            ����1ms = 1000,����Ƶ��24MHzʱ1msΪ 3000
*/
// ****************************************************************************
// ר�к���

// ----------------------------------------------------------------------------
// ����PWM01��,16λ
void SetPWM01(INT16U period, INT16U duty) {
    if ( duty < 3880 || duty > 5120) {

#if DEBUG == ON
        //HAVE_AN_ERROR();
#endif

        return;
    }
    PWMPER01 = period;
    PWMDTY01 = duty;
}

void InitPWM01(void) {
    PWMCTL_CON01 = 0x1;		/* ����PWM0 �� 1 */

    PWMPOL_PPOL1 = 0x1;		/* ��ʼ�Ǹ� */
    PWMCLK_PCLK1 = 0x0;   	/* ѡ��ʱ��A, һΪSA */
    PWMPRCLK_PCKA = 0x3;  	/* ��ʱ��A��Ϊԭ����8��֮1 */
    PWMCAE_CAE1 = 0x0;		/* ����� */
    PWMCNT01 = 0x0;			/* ���� PWM������,�ö��ٶ�һ�� */
    /*	PWMSCLA = ?;            ����SA��ʱ��A�ֶ���Ƶ��������SA��ʱ�� */
}

//#define StartPWM01()	{PWME_PWME1 = 1;}
//#define StopPWM01()		{PWME_PWME1 = 0;}


// ----------------------------------------------------------------------------
// ����PWM23��,16λ
void SetPWM23(INT16U period, INT16U duty) {
    if ( period < 100 ) {
        HAVE_AN_ERROR();
        return;
    }
    PWMPER23 = period;
    PWMDTY23 = duty;
}

void InitPWM23(void) {
    PWMCTL_CON23 = 0x1;		/* ����PWM 2 �� 3 */

    PWMPOL_PPOL3 = 0x0;	/* ��ʼ�ǵ� */
    PWMCLK_PCLK3 = 0x0;   /* ѡ��ʱ��B, һΪSB */
    PWMPRCLK_PCKB = 0x3;  /* ��ʱ��B��Ϊԭ����8��֮1 */
    PWMCAE_CAE3 = 0x0;		/* ����� */
    PWMCNT23 = 0x0;			/* ���� PWM������,�ö��ٶ�һ�� */
    /*	PWMSCLA = ?;            ����SA��ʱ��A�ֶ���Ƶ��������SA��ʱ�� */
}

//#define StartPWM23()	{PWME_PWME3 = 1;}
//#define StopPWM23()		{PWME_PWME3 = 0;}

//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// ����PWM45��,16λ
void SetPWM45(INT16U period, INT16U duty) {
    if ( period < 100 ) {
        HAVE_AN_ERROR();
        return;
    }
    PWMPER45 = period;
    PWMDTY45 = duty;
}

void InitPWM45(void) {
    PWMCTL_CON45 = 0x1;		/* ����PWM 4 �� 5 */

    PWMPOL_PPOL5 = 0x0;	/* ��ʼ�ǵ� */
    PWMCLK_PCLK5 = 0x0;   /* ѡ��ʱ��B, һΪSB */
    PWMPRCLK_PCKA = 0x3;  /* ��ʱ��B��Ϊԭ����8��֮1 */
    PWMCAE_CAE5 = 0x0;		/* ����� */
    PWMCNT45 = 0x0;			/* ���� PWM������,�ö��ٶ�һ�� */
    /*	PWMSCLA = ?;            ����SA��ʱ��A�ֶ���Ƶ��������SA��ʱ�� */
}

//#define StartPWM45()	{PWME_PWME5 = 1;}
//#define StopPWM45()		{PWME_PWME5 = 0;}
// ****************************************************************************
// ͨ�ú���


