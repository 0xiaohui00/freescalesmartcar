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

// ·������

#include "includes.h"


INT8U PathMemState = RECORD;

#define UP 2
#define DOWN 0
#define ZERO 1

#define POSI 0
#define NEGA 1

INT16U lastDir = UP, nowDir;
INT8U lastValue = 90;
INT8U nowSigned, lastSigned = POSI;
INT8U valve = 10; /* ��ֵ */

INT16U newXPointDist;
INT8U newXPointValue;
INT8U newXPointType;
INT16U lastXPointDist;

INT16U edgeStartPointDist, edgeEndPointDist;

#define MAX 1
#define MIN 2

#define WAIT 1
#define GET_X 2

INT8U nowState = WAIT;
INT8U lastState = WAIT;

#define LEFT 1
#define RIGHT 2

struct _PATHINFO {
    INT16U startDist;
    INT16U endDist;

    INT16U startEdgeDist;
    INT16U endEdgeDist;

    INT8U type;

    INT8U extremum;
}
pathInfo[200];


INT8U pathInfoN = 0;

void PrintPath() {
    INT16U i;
    for (i = 0;i < pathInfoN;i++) {
        prints("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               i, pathInfo[i].startDist, pathInfo[i].endDist,
               pathInfo[i].startEdgeDist, pathInfo[i].endEdgeDist,
               pathInfo[i].type, pathInfo[i].extremum
              );
    }
}

void Process(INT16U i, INT8U value) {
    INT8U X_POINT = FALSE;

    if (lastValue > value) {
        nowDir = UP;
    } else if ( lastValue < value) {
        nowDir = DOWN;
    } else {
        nowDir = lastDir;
    }

    if (value >= 90) {
        nowSigned = POSI;
    } else {
        nowSigned = NEGA;
    }
    // �жϷ��ŵı仯

    if (nowSigned != lastSigned) {
        X_POINT = TRUE;
    }

    // �жϷ���ı仯
    if ( (lastDir >= 1) ^ (nowDir > 1) ) {
        X_POINT = TRUE;
    }

    // �ҷ�ֵ��Ե�ĵ�
    if (lastState == WAIT) {
        if ( (lastValue < 90 + valve && value >= 90 + valve) ||
                (lastValue > 90 - valve && value <= 90 - valve) ) {
            edgeStartPointDist = i;
        }
    } else {
        if ( (lastValue > 90 + valve && value <= 90 + valve) ||
                (lastValue < 90 - valve && value >= 90 - valve) ) {
            edgeEndPointDist = i;
        }
    }


    // ����ҵ���ֵ��
    if (X_POINT) {
        newXPointDist = i;
        newXPointValue = value;

        // ���ϴ��ڵȴ���ֵ
        if (lastState == WAIT) {

            // ����˴δ����˷�ֵ (��)
            if (newXPointValue > 90 + valve) {
                lastState = GET_X;
                pathInfo[pathInfoN].startEdgeDist = edgeStartPointDist;
                pathInfo[pathInfoN].type = RIGHT;
                pathInfo[pathInfoN].extremum = newXPointValue;
                pathInfo[pathInfoN].startDist = lastXPointDist;
            }
            // ����˴δ����˷�ֵ (��)
            else if (newXPointValue < 90 - valve) {
                lastState = GET_X;
                pathInfo[pathInfoN].startEdgeDist = edgeStartPointDist;
                pathInfo[pathInfoN].type = LEFT;
                pathInfo[pathInfoN].extremum = newXPointValue;
                pathInfo[pathInfoN].startDist = lastXPointDist;
            }
        }
        // ����ϴ��Ѿ����ڼ�ֵ��
        else if (lastState == GET_X) {
            // ���С���˷�ֵ, �������һ���ж�
            if (newXPointValue <= 90 + valve && newXPointValue >= 90 - valve) {
                pathInfo[pathInfoN].endEdgeDist = edgeEndPointDist;
                pathInfo[pathInfoN].endDist = newXPointDist;
                lastState = WAIT;
                pathInfoN++;
            }
            // ������ڷ�ֵ, ��ˢ��ߵ� (��)
            else if (newXPointValue > 90 + valve) {
                if (pathInfo[pathInfoN].extremum < newXPointValue) {
                    pathInfo[pathInfoN].extremum = newXPointValue;
                }
            }
            // ������ڷ�ֵ, ��ˢ��͵� (��)
            else if (newXPointValue < 90 - valve) {
                if (pathInfo[pathInfoN].extremum > newXPointValue) {
                    pathInfo[pathInfoN].extremum = newXPointValue;
                }
            }
        }

        // �����ϴ�ֵ
        lastXPointDist = newXPointDist;
    }

    lastDir = nowDir;
    lastValue = value;
    lastSigned = nowSigned;

}


/******************************************************************************************/
// play

INT16U firstLoopStartDist;
INT16U secondLoopStartDist;

INT8U nowSpeedChgN = 0;

INT8U pathPlayN = 0;
struct _SPEED_CHG_POINTS {
    INT16U dist;
    INT16U maxSpd;
    INT16U minSpd;
}
SpeedChgPoints[200];

INT8U SpeedChgN = 1;



INT16U XSpeed = 300;

INT16U BendMaxSpeedBase = 110;
INT16U SingleBendMaxSpeedBase = 125;

INT16U needUpdateDist = 0;
// �����������ٶ�
INT16U GetBendMaxSpeed(INT8U x) {
    INT16U tspd;
    if ( x > 90 ) {
        tspd = (130 - x)  * 30 / 40 + BendMaxSpeedBase;
    } else {
        tspd = (x - 30) * 30 / 40 + BendMaxSpeedBase;
    }
    return (tspd > XSpeed)?(XSpeed):tspd;
}

INT16U GetSingleBendMaxSpeed(INT8U x, INT16U arcLength) {
    INT16U tspd;

    if (arcLength > 45) return GetBendMaxSpeed(x);

    if ( x > 90 ) {
        tspd = (130 - x)  * 33 / 40 + SingleBendMaxSpeedBase;
    } else {
        tspd = (x - 30) * 33 / 40 + SingleBendMaxSpeedBase;
    }


    return (tspd > XSpeed)?(XSpeed):tspd;
}

INT16U ErrorLength = 30;
INT16U AccLength = 15;

INT16U EnterLength = 5;
INT16U OutLength = 57;
INT16U SpeedUpLength = 5;
INT16U SpeedDownLength = 2;

// ��һ������ֱ���ϵ���С�ٶ�
INT16U MSpeed = 130;

// ���ݳ��ȵõ�ֱ��������ٶ�(length Ӧ����1m)
INT16U GetStraightMaxSpeed(INT16U length) {
    if (length > 90 /* 4m */) {
        return XSpeed;
    } else if ( length < 10) {
        return MSpeed;
    } else {
        return MSpeed + (XSpeed - MSpeed) * (length - 5) / 75;
    }
}
// ��������ٶȼ�����ʵļ��پ���
INT16U MaxStraightSpdDwnLength = 40;
INT16U MinStraightSpdDwnLength = 18;
INT16U GetStraightSpdDwnLength(INT16U maxspeed, INT16U bendspeed) {
    INT16U dspeed = (maxspeed > bendspeed)?(maxspeed - bendspeed):(1);
    return MinStraightSpdDwnLength +
           (MaxStraightSpdDwnLength - MinStraightSpdDwnLength) * dspeed / 170;

}


// ֱ��������
INT16U StrightErrorLength = 2;

// ��̸�����ʻ����
INT16U ShortestHighSpeedLength = 14;


void CalcStraightSpeed(INT16U StartDist, INT16U length, INT16U BendSpeed) {
    INT16U strMaxSpeed, strSpdDwnLength;
    // ֱ��̫�̲��˸�����ʻ
    if (length < ShortestHighSpeedLength) {
        SpeedChgPoints[SpeedChgN].dist = StartDist;
        SpeedChgPoints[SpeedChgN].maxSpd = BendSpeed;
        SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN - 1].minSpd;
        if (SpeedChgPoints[SpeedChgN].maxSpd <= SpeedChgPoints[SpeedChgN].minSpd)
            SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN].maxSpd;

        SpeedChgN++;

        SpeedChgPoints[SpeedChgN].dist = StartDist + length;
        SpeedChgPoints[SpeedChgN].maxSpd = BendSpeed;
        SpeedChgPoints[SpeedChgN].minSpd = BendSpeed;
        if (SpeedChgPoints[SpeedChgN].maxSpd <= SpeedChgPoints[SpeedChgN].minSpd)
            SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN].maxSpd;

        SpeedChgN++;
    } else {
        // ���Ը�����ʻ
        // Ԥ����
        strMaxSpeed = GetStraightMaxSpeed(length);
        strSpdDwnLength = GetStraightSpdDwnLength(strMaxSpeed, BendSpeed);


        if (length > 2 * StrightErrorLength) {
            // (1) ��ȫ����
            SpeedChgPoints[SpeedChgN].dist = StartDist;
            SpeedChgPoints[SpeedChgN].maxSpd = SpeedChgPoints[SpeedChgN - 1].maxSpd;
            SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN - 1].minSpd;
            SpeedChgN++;
            if (SpeedChgPoints[SpeedChgN].maxSpd <= SpeedChgPoints[SpeedChgN].minSpd)
                SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN].maxSpd;

            if ( length > 2 * StrightErrorLength + strSpdDwnLength) {

                // (2) ���ٵ�
                SpeedChgPoints[SpeedChgN].dist = StartDist + StrightErrorLength;
                SpeedChgPoints[SpeedChgN].maxSpd = strMaxSpeed;
                SpeedChgPoints[SpeedChgN].minSpd = strMaxSpeed;
                SpeedChgN++;
                if (SpeedChgPoints[SpeedChgN].maxSpd <= SpeedChgPoints[SpeedChgN].minSpd)
                    SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN].maxSpd;


                // (3) ���ٽ�����
                SpeedChgPoints[SpeedChgN].dist = StartDist + length - strSpdDwnLength;
                SpeedChgPoints[SpeedChgN].maxSpd = strMaxSpeed;
                SpeedChgPoints[SpeedChgN].minSpd = strMaxSpeed;
                SpeedChgN++;
                if (SpeedChgPoints[SpeedChgN].maxSpd <= SpeedChgPoints[SpeedChgN].minSpd)
                    SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN].maxSpd;

                // (4) ���ٵ� + ��ȫ����
                SpeedChgPoints[SpeedChgN].dist = StartDist + length - strSpdDwnLength - StrightErrorLength;
                SpeedChgPoints[SpeedChgN].maxSpd = BendSpeed;
                SpeedChgPoints[SpeedChgN].minSpd = BendSpeed;
                SpeedChgN++;
                if (SpeedChgPoints[SpeedChgN].maxSpd <= SpeedChgPoints[SpeedChgN].minSpd)
                    SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN].maxSpd;
            }
        }
        // (5) ���
        SpeedChgPoints[SpeedChgN].dist = StartDist + length;
        SpeedChgPoints[SpeedChgN].maxSpd = BendSpeed;
        SpeedChgPoints[SpeedChgN].minSpd = BendSpeed;
        SpeedChgN++;
        if (SpeedChgPoints[SpeedChgN].maxSpd <= SpeedChgPoints[SpeedChgN].minSpd)
            SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN].maxSpd;

    }
}
/*

// ����ֱ�����ٶ�
void CalcStraightSpeed(INT16U StartDist, INT16U length, INT16U BendSpeed) {
    // С����Χ
    if(length < EnterLength + OutLength) {
        SpeedChgPoints[SpeedChgN].dist = StartDist;
        SpeedChgPoints[SpeedChgN].maxSpd = BendSpeed;
        SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN - 1].minSpd;
        if(SpeedChgPoints[SpeedChgN].maxSpd <= SpeedChgPoints[SpeedChgN].minSpd)
            SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN].maxSpd;

        SpeedChgN++;

        SpeedChgPoints[SpeedChgN].dist = StartDist + length;
        SpeedChgPoints[SpeedChgN].maxSpd = BendSpeed;
        SpeedChgPoints[SpeedChgN].minSpd = BendSpeed;
        if(SpeedChgPoints[SpeedChgN].maxSpd <= SpeedChgPoints[SpeedChgN].minSpd)
            SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN].maxSpd;

        SpeedChgN++;
    } else {
        SpeedChgPoints[SpeedChgN].dist = StartDist + EnterLength;
        SpeedChgPoints[SpeedChgN].maxSpd = SpeedChgPoints[SpeedChgN - 1].maxSpd;
        SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN - 1].minSpd;
        SpeedChgN++;
        if(SpeedChgPoints[SpeedChgN].maxSpd <= SpeedChgPoints[SpeedChgN].minSpd)
            SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN].maxSpd;

        // С�ڼӵ����ٵķ�Χ
        if(length < EnterLength + OutLength + SpeedUpLength + SpeedDownLength) {

            SpeedChgPoints[SpeedChgN].dist = StartDist + length / 4;
            SpeedChgPoints[SpeedChgN].maxSpd = XSpeed * (length - (EnterLength + OutLength)) / ( SpeedUpLength + SpeedDownLength);
            SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN - 1].minSpd;
            SpeedChgN++;
            if(SpeedChgPoints[SpeedChgN].maxSpd <= SpeedChgPoints[SpeedChgN].minSpd)
            SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN].maxSpd;

        }
        // �ɼӵ�����
        else {
            SpeedChgPoints[SpeedChgN].dist = StartDist + EnterLength + SpeedUpLength;
            SpeedChgPoints[SpeedChgN].maxSpd = XSpeed;
            SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN - 1].minSpd;
            SpeedChgN++;
            if(SpeedChgPoints[SpeedChgN].maxSpd <= SpeedChgPoints[SpeedChgN].minSpd)
            SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN].maxSpd;

            SpeedChgPoints[SpeedChgN].dist = StartDist + length - OutLength - SpeedDownLength;
            SpeedChgPoints[SpeedChgN].maxSpd = XSpeed;
            SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN - 1].minSpd;
            SpeedChgN++;
            if(SpeedChgPoints[SpeedChgN].maxSpd <= SpeedChgPoints[SpeedChgN].minSpd)
            SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN].maxSpd;
        }

        SpeedChgPoints[SpeedChgN].dist = StartDist + length - EnterLength;
        SpeedChgPoints[SpeedChgN].maxSpd = BendSpeed;
        SpeedChgPoints[SpeedChgN].minSpd = BendSpeed;
        SpeedChgN++;
        if(SpeedChgPoints[SpeedChgN].maxSpd <= SpeedChgPoints[SpeedChgN].minSpd)
            SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN].maxSpd;

        SpeedChgPoints[SpeedChgN].dist = StartDist + length;
        SpeedChgPoints[SpeedChgN].maxSpd = BendSpeed;
        SpeedChgPoints[SpeedChgN].minSpd = BendSpeed;
        SpeedChgN++;
        if(SpeedChgPoints[SpeedChgN].maxSpd <= SpeedChgPoints[SpeedChgN].minSpd)
            SpeedChgPoints[SpeedChgN].minSpd = SpeedChgPoints[SpeedChgN].maxSpd;

    }
}
*/
// ��������ٶ�
void CalcBendSpeed(INT16U EndDist, INT16U BendSpeed, INT16U NextBendSpeed) {
    SpeedChgPoints[SpeedChgN].dist = EndDist;
    if (NextBendSpeed > BendSpeed) {
        SpeedChgPoints[SpeedChgN].maxSpd = BendSpeed;
        SpeedChgPoints[SpeedChgN].minSpd = BendSpeed;
    } else {
        SpeedChgPoints[SpeedChgN].maxSpd = NextBendSpeed;
        SpeedChgPoints[SpeedChgN].minSpd = NextBendSpeed;
    }


    SpeedChgN++;
}

// ���㵥����ٶ�(�й������)
void CalcSingleBendSpeed(INT16U startDist, INT16U endDist, INT16U BendSpeed) {
    /*   if(endDist - startDist > 10) {
           SpeedChgPoints[SpeedChgN].dist = endDist - 10;
           SpeedChgPoints[SpeedChgN].maxSpd = BendSpeed;
           SpeedChgPoints[SpeedChgN].minSpd = BendSpeed;;
           SpeedChgN++; 
     
           SpeedChgPoints[SpeedChgN].dist = endDist;
           SpeedChgPoints[SpeedChgN].maxSpd = BendSpeed + 7;
           SpeedChgPoints[SpeedChgN].minSpd = BendSpeed + 7;
           SpeedChgN++;
       } else */{
        SpeedChgPoints[SpeedChgN].dist = endDist;
        SpeedChgPoints[SpeedChgN].maxSpd = BendSpeed;
        SpeedChgPoints[SpeedChgN].minSpd = BendSpeed;
        SpeedChgN++;
    }


}

void PathPlayProcess(INT16U dist) {
    INT16U memDist;
    INT16U straightStartDist, straightEndDist;

    if ( dist < needUpdateDist) {
        return;
    }

    // �ͼ����Ӧ�ľ���
    memDist = dist - secondLoopStartDist + firstLoopStartDist;

    // ��ֱ��
    while (pathPlayN <= pathInfoN) {

        if (pathPlayN == 0) {
            if (pathInfo[pathPlayN].startDist > memDist) {
                // �ҵ�
                break;
            } else {
                pathPlayN++;
            }
        } else {
            if (pathInfo[pathPlayN - 1].endDist <= memDist &&
                    pathInfo[pathPlayN].startDist > memDist) {
                // �ҵ�
                break;
            } else {
                pathPlayN++;
            }
        } // if pathPlayN
    } // while

    // ����ֱ��
    straightStartDist = dist;
    straightEndDist = pathInfo[pathPlayN].startDist + secondLoopStartDist - firstLoopStartDist;


    // �������
    if (pathPlayN + 1 <= pathInfoN) {
        /*******************************************************************************************/
        // ��һ���������ô?

        // ��ֱ��
        if (pathInfo[pathPlayN].endDist - pathInfo[pathPlayN + 1].startDist > 0) {
            // ����ֱ��
            CalcStraightSpeed(straightStartDist,
                              straightEndDist - straightStartDist,
                              GetSingleBendMaxSpeed(pathInfo[pathPlayN].extremum,
                                                    pathInfo[pathPlayN].endEdgeDist - pathInfo[pathPlayN].startEdgeDist));

            CalcSingleBendSpeed(pathInfo[pathPlayN].startDist + secondLoopStartDist - firstLoopStartDist,
                                pathInfo[pathPlayN].endDist + secondLoopStartDist - firstLoopStartDist,
                                GetSingleBendMaxSpeed(pathInfo[pathPlayN].extremum,
                                                      pathInfo[pathPlayN].endEdgeDist - pathInfo[pathPlayN].startEdgeDist));
            pathPlayN++;
        } else {
            // ����ֱ��
            CalcStraightSpeed(straightStartDist,
                              straightEndDist - straightStartDist,
                              GetBendMaxSpeed(pathInfo[pathPlayN].extremum));

            for (;;) {
                if (pathInfo[pathPlayN].endDist - pathInfo[pathPlayN + 1].startDist < 5) {
                    CalcBendSpeed(pathInfo[pathPlayN].endDist + secondLoopStartDist - firstLoopStartDist,
                                  GetBendMaxSpeed(pathInfo[pathPlayN].extremum),
                                  GetBendMaxSpeed(pathInfo[pathPlayN + 1].extremum));
                    pathPlayN++;
                } else {
                    CalcBendSpeed(pathInfo[pathPlayN].endDist + secondLoopStartDist - firstLoopStartDist,
                                  GetBendMaxSpeed(pathInfo[pathPlayN].extremum),
                                  GetBendMaxSpeed(pathInfo[pathPlayN].extremum));
                    pathPlayN++;
                    break;
                }
            }
        }
        /*******************************************************************************************/
    } else {
        // �����, �����?
    }

    needUpdateDist = pathInfo[pathPlayN - 1].endDist - firstLoopStartDist + secondLoopStartDist;
}


void PathPlayer(INT16U dist) {
    INT16U i;
    for (i = nowSpeedChgN;i < SpeedChgN - 1;i++) {
        if ( dist > SpeedChgPoints[i].dist && dist <= SpeedChgPoints[i+1].dist ) {
            /********************************************************************/
            if (SpeedChgPoints[i].maxSpd >= SpeedChgPoints[i + 1].maxSpd) {
                maxSpeed = ( SpeedChgPoints[i+1].dist - dist ) *
                           ( SpeedChgPoints[i].maxSpd - SpeedChgPoints[i+1].maxSpd) / (SpeedChgPoints[i+1].dist - SpeedChgPoints[i].dist) + SpeedChgPoints[i+1].maxSpd;
            } else {
                maxSpeed = ( dist - SpeedChgPoints[i].dist ) *
                           ( SpeedChgPoints[i+1].maxSpd - SpeedChgPoints[i].maxSpd) / (SpeedChgPoints[i+1].dist - SpeedChgPoints[i].dist) + SpeedChgPoints[i].maxSpd;
            }

            if (SpeedChgPoints[i].minSpd >= SpeedChgPoints[i + 1].minSpd) {
                minSpeed = ( SpeedChgPoints[i+1].dist - dist ) *
                           ( SpeedChgPoints[i].minSpd - SpeedChgPoints[i+1].minSpd) / (SpeedChgPoints[i+1].dist - SpeedChgPoints[i].dist) + SpeedChgPoints[i+1].minSpd;
            } else {
                minSpeed = ( dist - SpeedChgPoints[i].dist ) *
                           ( SpeedChgPoints[i+1].minSpd - SpeedChgPoints[i].minSpd) / (SpeedChgPoints[i+1].dist - SpeedChgPoints[i].dist) + SpeedChgPoints[i].minSpd;
            }

            if (maxSpeed < minSpeed ) {
                minSpeed = maxSpeed;
            }

            return;
            /********************************************************************/
        }
    }

}

void StartPathPlay(void) {
    firstLoopStartDist = StartLineDist[0];
    secondLoopStartDist = StartLineDist[1];

    nowSpeedChgN = 0;

    pathPlayN = 0;
    SpeedChgPoints[0].dist = secondLoopStartDist;
    SpeedChgPoints[0].maxSpd = 110;
    SpeedChgPoints[0].minSpd = 80;

    SpeedChgN = 1;

    //ErrorLength = 30;
    //AccLength = 75;

    //XSpeed = 200;

    needUpdateDist = 0;

    PathMemState = PLAY;
}
