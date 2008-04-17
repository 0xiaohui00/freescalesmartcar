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
 

// �������� (Hall Velocity Measurement)
// ͨ����ʱȡ������������ֵ,�����ĳ���ǰ�ٶ�
// ���ɼ������һ���ٶȷ���SpeedTimeH �� SpeedTimeL��

#include "includes.h"

// ���ټ�����,�����εõ��������ĺ�������
INT16U carSpeedCounter[4];
INT16U carSpeedArrN = 0;
unsigned long lastAbsoluteTime;
//INT16U last_TC2, last_tc2_counter;

extern INT32U PerDistSrvTotal;
extern INT16U PerDistSrvN;

#define SRV_LEFT 0
#define SRV_CENTER 1
#define SRV_RIGHT 2

// ������
#define DIS(x,y) ( ( (x) > (y) ) ? ( (x) - (y) ) : ( (y) - (x) ))

INT8U LastSrvAvg = 90;

INT16U PerDistSrvAvg = 90;
INT16U LastSrvState = SRV_CENTER;


// ����תȦ��������·��
//#define ClearDistanceCounter() {PAC = 0;}
//#define GetDistance() (PAC)
//#define Distance PAC

INT16U GetSpeed(void) {
    //�����û��ʼ��,������
    if (GetDistance() < 8)
        return 0xFFFF;

    return  carSpeedCounter[0] +
            carSpeedCounter[1] +
            carSpeedCounter[2] +
            carSpeedCounter[3];
}

void ProcessSpeed_FUNC(void) {
    carSpeedCounter[carSpeedArrN = (carSpeedArrN + 1) & 0x3] = (INT16U)(absoluteTime - lastAbsoluteTime);
    lastAbsoluteTime = absoluteTime;

    /******************************************************/
    /** TODO ·������ļ������ **/
    /* ����ǰ��Ŀ�ʼ�µ� */
    if (PerDistSrvTotal != 0 && PerDistSrvN != 0) {
        PerDistSrvAvg = (INT8U)(PerDistSrvTotal / PerDistSrvN);

        // �ų�����
        if (DIS(PerDistSrvAvg, LastSrvAvg) > 30) {
            PerDistSrvAvg = LastSrvAvg;
        }

        if ((PerDistSrvAvg > 50 && PerDistSrvAvg < 130) ) {
            LastSrvAvg = (INT8U)PerDistSrvAvg;
        }



    } else {
        PerDistSrvAvg = LastSrvAvg;
    }



    // ���͵�������
    if (SCI0SR1 & 0x80) {
        SCI0DRL = (INT8U)((INT16S)((3927 * 4 * 4) / GetSpeed() * _RTI_P / 4));
        //(INT8U)PerDistSrvAvg;
    }

    /** ·��������ڵ� **/
    Process(GetDistance(), (INT8U)PerDistSrvAvg);

    // ·��������ڵ�
    if (PathMemState == PLAY) {
        PathPlayProcess(GetDistance());
        PathPlayer(GetDistance());
    }

    // �ж������
    CheckNewPoint();

    PerDistSrvTotal = 0;
    PerDistSrvN = 0;
    /******************************************************/
}

// ��ʼ��������
void InitSpeeder(void) {
    // ��ʼ�������������
    // WaitDisable();
    WaitEnable();

    InitPA();
    PAI_FUNC = ProcessSpeed_FUNC;

    lastAbsoluteTime = 0;

    // ��λ����ֵ
    ClearDistanceCounter();

}





