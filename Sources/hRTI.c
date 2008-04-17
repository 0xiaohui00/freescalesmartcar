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

// ʵʱ�ж� 
#include "includes.h"

//INT8U STB_Flag = FALSE;
unsigned long absoluteTime = 0;
/*************************************************************************/
/* �жϷ������ */

/*
  ��ʱ��1��ʱ���жϷ������
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
__interrupt void RTI_ISR(void) {
    absoluteTime++;
    CRGFLG = 0x80;
}


/*
   ʱ���жϳ�ʼ��
*/
#pragma CODE_SEG DEFAULT
void RTIInit(void) {
    //RTICTL = 0x1B ->> ԼΪ1ms
    /* 0x10 ʱ��ƵΪ16M/2^10 */
    RTICTL = 0x10; /* 0x1F ʱ��ƵΪ16M/16/2^10   2^10 ~= 10^3 */
    CRGINT = 0x80; /* enable RTI interrupts */
}

/*
  ��ʼʱ���ж�
*/
void StartTimeBase(void) {
    //STB_Flag = TRUE;
    absoluteTime = 0;
    RTIInit();
    EnableInterrupts;
}

/*
  �ȴ���Ӧ�ĺ���
*/
void Wait(long ms) {
    unsigned long timeout;

    /* RTICTL = 0x10ʱ�� ��16 */
    timeout = absoluteTime + ms * 16;
    while (timeout != absoluteTime) {
        __asm NOP;
    }
}

void WaitEnable(void) {
    //if(!STB_Flag) {
    StartTimeBase();
    //}
}

void WaitDisable(void) {
    CRGINT = 0x00;
}
