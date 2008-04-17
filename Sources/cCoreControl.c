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

// ����ʶ��

#include "includes.h"

// �й��ٶȵĲ���
INT16S lastTgtSpeed = 100;
INT16S tgtSpeed = 100;
INT16U maxSpeed = 110;
INT16U minSpeed = 80;

// ���ƾ���Ϊ ������
INT16U DistLimit = 0;

// ���Ժ���Եĺ�����ֵ
INT16U wir[nIR];
INT16U irValue[nIR];

// �ڰ׵Ĳο�ֵ
INT16U whiteAvg[nIR], blackAvg[nIR];

// �м����
INT16U minTmp[nIR][10], maxTmp[nIR][10];

// ÿ�μ�⵽��IRλ��
INT16U position;

// ���Ŀ��λ��
INT8U servoTgtAngle;

INT16U tmir;

// ʮ�ֺͽ���ļ�¼
INT8U crsN,crsType[10];
INT16U crsDist[10];

INT16U StartLineDist[4];
INT8U CrossLineN = 0, RampDistN = 0;
INT16U CrossLineDist[20];
INT16U RampDist[10];
// ��ǰ���ڵ�Ȧ
INT8U nowLoop;

// ���ڼ��㵥λ�Ƕȶ���սǵ�ƽ��ֵ
INT32U PerDistSrvTotal;
INT16U PerDistSrvN;

#define swap(x, y) {(x) = (x) ^ (y);(y) = (x) ^ (y);(x) = (x) ^ (y);}


// IR �������� �ֱ���PTH, PORTE & PORTA ��ֵ
const INT8U irSendArr[nIR][3] = {
#if nIR == 12
                                    {0, 0, PORTA_BIT6_MASK},
                                    {0, 0, PORTA_BIT7_MASK},
                                    {PTH_PTH3_MASK, 0, 0},
                                    {PTH_PTH2_MASK, 0, 0},
                                    {PTH_PTH1_MASK, 0, 0},
                                    {PTH_PTH0_MASK, 0, 0},
                                    {0, PORTE_BIT3_MASK, 0},
                                    {0, PORTE_BIT2_MASK, 0},
                                    {0, 0, PORTA_BIT2_MASK},
                                    {0, 0, PORTA_BIT3_MASK},
                                    {0, 0, PORTA_BIT5_MASK},
                                    {0, 0, PORTA_BIT4_MASK}
#elif nIR == 8
                                    {PTH_PTH3_MASK, 0, 0},
                                    {PTH_PTH2_MASK, 0, 0},
                                    {PTH_PTH1_MASK, 0, 0},
                                    {PTH_PTH0_MASK, 0, 0},
                                    {0, PORTE_BIT3_MASK, 0},
                                    {0, PORTE_BIT2_MASK, 0},
                                    {0, 0, PORTA_BIT2_MASK},
                                    {0, 0, PORTA_BIT3_MASK},

#endif
                                };

// IR ���չ����з�ʽ
const INT8U irRecvArr[nIR] = {
#if nIR == 12
                                 11,//0
                                 10,
                                 12,
                                 4,
                                 15,
                                 7,
                                 14,//6
                                 6,
                                 13,
                                 5,
                                 3,
                                 2
#elif nIR == 8
                                 12,
                                 14,//64,
                                 15,
                                 13,
                                 4,
                                 6,
                                 7,
                                 5
#endif
                             };

const INT8U irRevPair[nIR / 2][4] = {
#if nIR == 12
                                        {5, 0, 7, 11},
                                        {10, 1, 3, 10},
                                        {7 ,2, 6, 12},
                                        {3, 8, 4, 13},
                                        {9, 4, 5, 15},
                                        {11, 6, 2, 14}
#elif nIR == 8
                                        {7,3,5,13},
                                        {4,0,4,12},
                                        {5,1,6,14},
                                        {6,2,7,15}

#endif
                                    };


const INT8U ir_position[nIR] = {
#if nIR == 8
                                   38,
                                   63,
                                   88,
                                   113,
                                   138,
                                   163,
                                   188,
                                   213
#elif nIR == 12
                                   15,
                                   35,
                                   55,
                                   75,
                                   95,
                                   115,
                                   135,
                                   155,
                                   175,
                                   195,
                                   215,
                                   235
#endif
                               };

const INT8U speed_arr[251] = {
                                 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0, 0, 1, 1, 1, 1,
                                 1, 2, 2, 2, 2, 2, 3, 3,
                                 3, 4, 4, 4, 5, 5, 5, 6,
                                 6, 6, 7, 7, 8, 8, 8, 9,
                                 9, 10, 10, 11, 11, 11, 12, 12,
                                 13, 13, 14, 14, 15, 15, 16, 16,
                                 17, 17, 18, 18, 19, 19, 20, 20,
                                 21, 21, 22, 22, 23, 23, 24, 24,
                                 25, 25, 26, 26, 27, 27, 28, 28,
                                 28, 29, 29, 30, 30, 31, 31, 31,
                                 32, 32, 33, 33, 33, 34, 34, 34,
                                 35, 35, 35, 36, 36, 36, 37, 37,
                                 37, 37, 37, 38, 38, 38, 38, 38,
                                 39, 39, 39, 39, 39, 39, 39, 39,
                                 39, 39, 39, 39, 39, 39, 39, 39,
                                 39, 39, 39, 39, 39, 39, 39, 39,
                                 39, 38, 38, 38, 38, 38, 37, 37,
                                 37, 37, 37, 36, 36, 36, 35, 35,
                                 35, 34, 34, 34, 33, 33, 33, 32,
                                 32, 31, 31, 31, 30, 30, 29, 29,
                                 28, 28, 28, 27, 27, 26, 26, 25,
                                 25, 24, 24, 23, 23, 22, 22, 21,
                                 21, 20, 20, 19, 19, 18, 18, 17,
                                 17, 16, 16, 15, 15, 14, 14, 13,
                                 13, 12, 12, 11, 11, 11, 10, 10,
                                 9, 9, 8, 8, 8, 7, 7, 6,
                                 6, 6, 5, 5, 5, 4, 4, 4,
                                 3, 3, 3, 2, 2, 2, 2, 2,
                                 1, 1, 1, 1, 1, 0, 0, 0,
                                 0, 0, 0, 0, 0, 0, 0, 0,
                                 0, 0, 0
                             };


const INT8U servo_turn_arr[251] = {
                                      51, 51, 51, 51, 51, 51, 51, 51,
                                      51, 51, 51, 51, 51, 51, 51, 51,
                                      51, 51, 51, 51, 51, 51, 51, 51,
                                      51, 52, 52, 52, 52, 52, 52, 52,
                                      52, 52, 52, 52, 53, 53, 53, 53,
                                      53, 53, 53, 53, 54, 54, 54, 54,
                                      54, 54, 54, 55, 55, 55, 55, 55,
                                      56, 56, 56, 56, 56, 57, 57, 57,
                                      57, 58, 58, 58, 58, 59, 59, 59,
                                      60, 60, 60, 61, 61, 61, 62, 62,
                                      63, 63, 63, 64, 64, 65, 65, 66,
                                      66, 67, 67, 68, 68, 69, 69, 70,
                                      71, 71, 72, 72, 73, 74, 74, 75,
                                      76, 77, 77, 78, 79, 80, 80, 81,
                                      82, 83, 84, 84, 85, 86, 87, 87,
                                      88, 89, 89, 89, 89, 89, 90, 90,
                                      91, 92, 92, 93, 94, 95, 96, 97,
                                      98, 99, 100, 101, 101, 102, 103, 104,
                                      105, 106, 106, 107, 108, 109, 109, 110,
                                      111, 111, 112, 113, 113, 114, 114, 115,
                                      115, 116, 116, 117, 117, 118, 118, 119,
                                      119, 119, 120, 120, 121, 121, 121, 122,
                                      122, 122, 122, 123, 123, 123, 124, 124,
                                      124, 124, 125, 125, 125, 125, 125, 126,
                                      126, 126, 126, 126, 127, 127, 127, 127,
                                      127, 127, 127, 127, 128, 128, 128, 128,
                                      128, 128, 128, 128, 128, 128, 129, 129,
                                      129, 129, 129, 129, 129, 129, 129, 129,
                                      129, 129, 129, 129, 129, 129, 129, 129,
                                      129, 129, 129, 129, 129, 129, 129, 129,
                                      129, 129, 129, 129, 129, 129, 129, 129,
                                      129, 129, 129
                                  };

// ʶ����������һЩ����
INT16U tBlState;
INT16U tv;
INT16U ttotal,total;
INT16U minIRv, minIRn;
INT16U minIRv2, minIRn2; /* �ڶ�С */
INT16U maxIRv;
INT16U last_position = 90;

INT32U tdist;

// ���ߵ�״̬
#define NORMAL 0        /* ��ͨ���� */
#define LOST 1          /* �Ҳ��� */
#define START 2         /* ��ʼ�� */
#define CROSS 3         /* ʮ�ֽ��� */

INT16U blStateArr[4];   /* ״̬���� */
INT16U blState;         /* ״̬������� */
INT16U tw, tb;
void GetBlackAndWhite(void) {
    INT16U i, j;


    for (i = 0;i < nIR;i++) {
        whiteAvg[i] = 0;
        blackAvg[i] = 0;
        for (j = 0;j < 10;j++) {
            maxTmp[i][j] = 0;
            minTmp[i][j] = 1023;
        }
    }

    for (;;) {


        PORTB = 0x55;


        if (!PTIP_PTIP0) {
            // ����
            for (i = 0;i < nIR;i++) {
                tw = 0;
                tb = 0;
                for (j = 0;j < 10;j++) {
                    if (maxTmp[i][j] != 0) {
                        tw++;
                        whiteAvg[i] += maxTmp[i][j];
                    }
                }
                whiteAvg[i] /= tw;
                for (j = 0;j < 10;j++) {
                    if (minTmp[i][j] != 1023) {
                        tb++;
                        blackAvg[i] += minTmp[i][j];
                    }
                }
                blackAvg[i] /= tb;

            }

            /** ����ֵ���ֹ����� **/

            //whiteAvg[3] -= 36;
            //whiteAvg[2] -= 10;
            //whiteAvg[6] -= 10;

            return;
        }


        // �ɼ�����ֵ
        for (i = 0;i < nIR / 2;i++) {
            irSendDouble(irRevPair[i][0], irRevPair[i][1]);

            for (j = 0;j < 100;j++);

            ReadADCDouble(irRevPair[i][2],irRevPair[i][3], &wir[irRevPair[i][0]], &wir[irRevPair[i][1]]);

        }

        irSendAllStop();

        // �������С
        for (i = 0;i < nIR;i++) {
            for (j = 0;j < 10;j++) {
                if (wir[i] > maxTmp[i][j]) {
                    maxTmp[i][j] = wir[i];
                    break;
                }
            }
            for (j = 0;j < 10;j++) {
                if (wir[i] < minTmp[i][j]) {
                    minTmp[i][j] = wir[i];
                    break;
                }
            }

        }
    }
}


// ���ٺ� ���ٰ�
struct _SPC_PNT {
    INT32U allWhite;
    INT32U allBlack;
    INT32U likeStart;
    INT16U dist;
}
spcPnts[30] = {{0,0,0,0}};
INT8U spcPntsN = 0;

INT16U nBlack, nWhite;

// ����ǲ���ͬһ����, �������¿�һ��
void NewSpecialPoint(void) {
    if (GetDistance() - spcPnts[spcPntsN].dist > 8) {
        if (spcPntsN > 28) return;
        spcPntsN++;
        spcPnts[spcPntsN].allWhite = 0;
        spcPnts[spcPntsN].allBlack = 0;
        spcPnts[spcPntsN].likeStart = 0;
        spcPnts[spcPntsN].dist = GetDistance();
    }
}

void ProcessSpecialPoint(void) {
    INT16U centerMax1,centerMax2, sideMax1,sideMax2;
    INT16U i,j;
    nBlack = nWhite = 0;

    // ��·�м�ô
    if (LastSrvAvg > 115 || LastSrvAvg < 75)
        return;

    // �ж���
    for (i = 0;i < nIR;i++) {
        if (wir[i] > whiteAvg[i] + (1023 - whiteAvg[i]) / 3) {
            nWhite++;
        }
    }// for

    if ( nWhite > 5 && blState == 1) {
        NewSpecialPoint();
        spcPnts[spcPntsN].allWhite++;
    }


    // �жϳ�����
    centerMax1 = centerMax2 = sideMax1 = sideMax2 = 0;
#if nIR == 8
    for (j = 2;j <= 5;j++) {
#elif nIR == 12
    for (j = 4;j <= 7;j++) {
#endif
        if (centerMax1 < irValue[j]) {
            centerMax2 = centerMax1;
            centerMax1 = irValue[j];
        }
    }

#if nIR == 8
    for (j = 6;j <= 9;j++) {
        if (sideMax1 < irValue[j % 8]) {
            sideMax2 = sideMax1;
            sideMax1 = irValue[j % 8];
        }
    }
#elif nIR == 12
    for (j = 6;j <= 9;j++) {
        if (sideMax1 < irValue[j % 8 + 2]) {
            sideMax2 = sideMax1;
            sideMax1 = irValue[j % 8 + 2];
        }
    }
#endif

    if (sideMax1 + sideMax2 + 20 < centerMax1 + centerMax2 && blState > 1) {
        NewSpecialPoint();
        spcPnts[spcPntsN].likeStart++;
    }

    // �жϽ�����
    for (i = 0;i < nIR;i++) {
        if (irValue[i] < 40) {
            nBlack++;
        }
    }
    if (nBlack >= 8 && blState > 1) {
        NewSpecialPoint();
        spcPnts[spcPntsN].allBlack++;
    }
}

void PrintProcSpcResult(void) {
    INT8U i;
    prints("total = %d\n", spcPntsN);
    for (i = 1;i <= spcPntsN;i++) {
        prints("w = %d\tb = %d\ts = %d\tdist = %d\n",
               (INT16U)spcPnts[i].allWhite, (INT16U)spcPnts[i].allBlack, (INT16U)spcPnts[i].likeStart, spcPnts[i].dist);
    }
}




void ProcStartLine(void) {
    nowLoop++;
    switch (nowLoop) {
    case 1:
        PORTB = ~0x22;

        StartLineDist[0] = GetDistance();
        break;
    case 2:
        StartLineDist[1] = GetDistance();

        PORTB = 0xA5;

        // ����ڶ�Ȧ��ͣ��
        DistLimit = ( GetDistance() + (StartLineDist[1] - StartLineDist[0]) ) / 25 + 4;
        /** �����ﴥ��·������ **/
        StartPathPlay();

        //maxSpeed = 110;
        //minSpeed = 75;


        break;
    case 3:
        // ������Ȧ��ͣ��
        StartLineDist[2] = GetDistance();

        PORTB = 0x81;
        //DistLimit = GetDistance() / 25 + 2;
        break;
    default:
        break;
    }
}
void ProcRamp(void) {
    PORTB = 0x77;

    RampDist[RampDistN] = GetDistance();
    RampDistN++;

}
void ProcCrossLine(void) {
    PORTB = ~PORTB;

    CrossLineDist[CrossLineN] = GetDistance();
    CrossLineN++;
}

// �Ѿ������ĵ�
INT8U chkedPntsN = 0;
INT16U lastAllWhiteDist = 0;
void CheckNewPoint(void) {
    // ����ǲ�����û�д�����µ�
    if (chkedPntsN < spcPntsN && spcPnts[spcPntsN].dist + 4 < GetDistance() ) {
        if (lastAllWhiteDist == 0 || lastAllWhiteDist + 70 < GetDistance()) {
            if (spcPnts[spcPntsN].allWhite > 0) {
                // �����µ�
                //PORTB = ~(1);
                lastAllWhiteDist = GetDistance();

                ProcRamp();

            }

            if (spcPnts[spcPntsN].allBlack > 0 || spcPnts[spcPntsN].likeStart > 0) {
                // �˴� ��3 ��ΪallBlack ������ likeStart ��ܶ�
                if (spcPnts[spcPntsN].allBlack > spcPnts[spcPntsN].likeStart * 3) {
                    // �����
                    //PORTB = ~PORTB;
                    ProcCrossLine();

                } else {
                    // ������
                    //PORTB = 0x9;
                    ProcStartLine();


                }
            }
        }
        chkedPntsN++;
    }
}

INT16U xo = 100;
INT16U CenterOffset(INT16U bl) {
    if ( bl >= xo ) {
        return 125 + ( bl - xo ) * 125 / (250 - xo);
    } else {
        return 125 - (xo - bl) * 125 / xo;
    }
}

// ���Ľ���
void CoreControl(void) {
    INT16U i = 0, j;
    INT8U tpls = 0;
//    INT16U tmp;

    // ���ڷ��͵�״̬, �ʹ�����
    //INT8U sciState = 0;//, sciChar;

    StartSpeeder();
    ClearDistanceCounter();

    MotorControlInit();

    crsN = 0;
    nowLoop = 0;


    FOREVER() {
        //PORTB = ~PORTB;

        // �ж�С�����Ƿ���, �������˵�
        if (!PTIP_PTIP0) {
            WaitEnable();
            Wait(20);
            if (!PTIP_PTIP0) {
                StopRun();
                StartMenu();
                WaitSmallButtonPress();
                Wait(1500);
            }
        }


        // �ɼ�����ֵ
        for (i = 0;i < nIR / 2;i++) {
            irSendDouble(irRevPair[i][0], irRevPair[i][1]);

            for (j = 0;j < 100;j++);

            ReadADCDouble(irRevPair[i][2],irRevPair[i][3], &wir[irRevPair[i][0]], &wir[irRevPair[i][1]]);
        }

        // ����ֵ��һ��
        for (i = 0;i < nIR;i++) {
            if (whiteAvg[i] < wir[i]) {
                irValue[i] = 100;
            } else if (blackAvg[i] > wir[i]) {
                irValue[i] = 0;
            } else {
                irValue[i] = (100 * (wir[i] - blackAvg[i])) / ( whiteAvg[i] - blackAvg[i]);
            }
        }

        /** ����ֵ���ֹ����� **/
        //irValue[2] = irValue[2] * 100 / 94;
        //irValue[3] = irValue[3] * 100 / 86;
        //irValue[6] = irValue[6] * 100 / 94;


        // �����
        minIRv = 100;
        maxIRv = 0;
        ttotal = 0;
        for (i = 0;i < nIR;i++) {
            if ( minIRv > irValue[i] ) {
                minIRn2 = minIRn;
                minIRn = i;
                minIRv2 = minIRv;
                minIRv = irValue[i];
            }

            if ( maxIRv < irValue[i] ) {
                maxIRv = irValue[i];
            }

            ttotal+= irValue[i];
        }
        total = ttotal;



        // �����ȷλ��
        if ( minIRn <= (nIR - 1 - 1) && minIRn >= 1) {
            position = ir_position[minIRn - 1] + irValue[minIRn - 1] * IR_SPACE_BETWEEN / (irValue[minIRn - 1] + irValue[minIRn + 1]);
        } else if (minIRn == 0) {
            if (irValue[1] >= 90) {
                position = ir_position[0] - (IR_SPACE_BETWEEN / 2) * irValue[0] / (irValue[0] + irValue[1]);
            } else {
                position = ir_position[0] + (IR_SPACE_BETWEEN / 2) * irValue[0] / (irValue[0] + irValue[1]);
            }
        } else {
            if (irValue[nIR - 1 - 1] >= 92/** TODO ��������ô? **/) {
                position = ir_position[nIR - 1] + (IR_SPACE_BETWEEN / 2) * irValue[nIR - 1] / (irValue[nIR - 1 - 1] + irValue[nIR - 1]);
            } else {
                position = ir_position[nIR - 1] - (IR_SPACE_BETWEEN / 2) * irValue[nIR - 1] / (irValue[nIR - 1 - 1] + irValue[nIR - 1]);
            }
        }

        // �Ŵ�, ������ǰ������֮���ֵ��Ӧ����15

        if ( ((position > last_position)?(position - last_position):(last_position - position)) > 60) {
            if (nowLoop == 1) {
                if (GetDistance() - lastAllWhiteDist > 50)
                    position = last_position;
            } else if (nowLoop == 2) {
                if (GetDistance() - (StartLineDist[1] - StartLineDist[0]) - lastAllWhiteDist > 50)
                    position = last_position;
            }

        }


        // ��ģ�����жϸ���·�����
        /** TODO ����������Ȩֵ�����Ե���(ͨ������), ���������������� **/
        //���                  minIRv              minIRv2             maxIRv              total
        blStateArr[NORMAL]  =   (100 - minIRv) +    (100 - minIRv2) +   maxIRv          +   total / nIR;
        blStateArr[LOST]    =   minIRv         +    minIRv2         +   maxIRv          +   (100 - total / nIR);
        blStateArr[CROSS]   =   (100 - minIRv) +    (100 - minIRv2) +   (100 - maxIRv)  +   (100 - total / nIR);
        blStateArr[START]   =   (100 - minIRv) +    (100 - minIRv2) +   maxIRv          +   (100 - total / nIR);

        // �����п��ܵ����
        blState = NORMAL;
        for (i = 1;i < 4;i++) {
            if (blStateArr[blState] < blStateArr[i]) {
                blState = i;
            }
        }

        tBlState = blState;


        // �����жϽ��������Ӧ
        if ( blState != NORMAL) {


            ProcessSpecialPoint();
            if (GetDistance() - lastAllWhiteDist < 50) {
                last_position = position;
            } else {
                position = last_position;
            }


        } else {
            last_position = position;
        }

        // �ó����ת��
#if nIR == 8
        // ����ǰ��ƥ��
        if (position > 125) {
            position = 125 + (position - 125) * 220 / 175;
        } else {
            position = 125 - (125 - position) * 220 / 175;
        }
#endif
        /** TODO ʹ��һ��������������Ϊ����ṩת�� **/
        servoTgtAngle = (INT8U)PosToAgl(position);

        //�ۼӶ���Ƕ�
        if (PerDistSrvTotal < 0xFFFFFF && PerDistSrvN < 0xFFF0 && servoTgtAngle < 130 && servoTgtAngle > 50 ) {
            PerDistSrvTotal += servoTgtAngle;
            PerDistSrvN++;
        }


        // ��ת�ٿ���
        tgtSpeed = (INT16S)(minSpeed + (maxSpeed - minSpeed) * (speed_arr[position] ) / 40);


        if (tgtSpeed < 50 || tgtSpeed > XSpeed) {
            tgtSpeed = lastTgtSpeed;
        } else {
            lastTgtSpeed = tgtSpeed;
        }


        // ת�����
        ServoControl();

        // ת�����, �����ƾ����
        if (DistLimit != 0 && DistLimit * 25 < GetDistance() ) {
            StopRun();
        } else {
            MotorControl();
        }

        tdist = GetDistance();

        /*if(SCI0SR1 & 0x80) {
            switch(sciState) {
                case 0:
                    sciChar = 0xFE;
                    break;
                case 1:
                    sciChar = (INT8U)position;
                    break;
                case 2:
                    sciChar = (INT8U)servoTgtAngle;
                    break;
                case 3:
                    sciChar = ((INT8U)((3927 * 4) / GetSpeed())  * _RTI_P )& 0xFF;
                    break;
                case 4:
                    sciChar = (INT8U)(GetDistance());
                default:  
                    break;            
            }
            sciState = (INT16U)(sciState + 1) % 5;
            SCI0DRL = sciChar;
        } */
        ///////////////////////////////////////////////////////////////////////
    }

}

void StartCore(void) {
    PORTB = 0xAA;
    DDRB = 0xFF;

    //  ��ʼ��С����
    DDRP_DDRP0 = 0;
    PPSP_PPSP0 = 0;
    PERP_PERP0 = 1;

    /********************************/
    /* ��ʼ�� */
    InitServo();
    StartServo();
    SetServo(90);

    InitSpeeder();

    irInit();

    InitADC();

    InitSCI0();

    WaitEnable();

    /*******************************/

    GetBlackAndWhite();

    PORTB = 0x55;

    Wait(1500);

    PORTB = 0xAA;

    while (PTIP_PTIP0);

    PORTB = 0x5A;

    Wait(1000);

    PORTB = 0xA5;

    CoreControl();

}
