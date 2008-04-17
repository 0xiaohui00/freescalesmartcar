// Test IR Seniors

#include "includes.h"



/*
void Test_GetBlackAndWhite(void) {
    INT16U i, j;
    INT16U tw, tb;

    PORTB = 0xAA;
    DDRB = 0xFF;

    DDRP_DDRP0 = 0;
	PPSP_PPSP0 = 0;
	PERP_PERP0 = 1;

    for(i = 0;i < 12;i++) {
        whiteAvg[i] = 0;
        blackAvg[i] = 0;
        for(j = 0;j < 10;j++) {
            maxTmp[i][j] = 0;
            minTmp[i][j] = 1024;
        }
    }

    irInit();

    InitADC();


    for(;;) {


        PORTB = 0x55;


        if(!PTIP_PTIP0){
            // ����
            for(i = 0;i < nIR;i++) {
                tw = 0;tb = 0;
                for(j = 0;j < 10;j++) {
                    if(maxTmp[i][j] != 0) {
                        tw++;
                        whiteAvg[i] += maxTmp[i][j];
                    }
                }
                whiteAvg[i] /= tw;
                for(j = 0;j < 10;j++) {
                    if(minTmp[i][j] != 1024) {
                        tb++;
                        blackAvg[i] += minTmp[i][j];
                    }
                }
                blackAvg[i] /= tb;

            }

            return;
        }

        // Ϊ�˲鿴�ж�ʱ��
        //PTP_PTP0 = !PTP_PTP0;


        // �ɼ�����ֵ
        for(i = 0;i < nIR / 2;i++) {
            irSendDouble(irRevPair[i][0], irRevPair[i][1]);

            for(j = 0;j < 500;j++);

            ReadADCDouble(irRevPair[i][2],irRevPair[i][3], &wir[irRevPair[i][0]], &wir[irRevPair[i][1]]);
        }

        // �������С
        for(i = 0;i < nIR;i++) {
            for(j = 0;j < 10;j++) {
                if(wir[i] > maxTmp[i][j]) {
                    maxTmp[i][j] = wir[i];
                    break;
                }
            }
            for(j = 0;j < 10;j++) {
                if(wir[i] < minTmp[i][j]) {
                    minTmp[i][j] = wir[i];
                    break;
                }
            }

        }
    }
}
*/


/*
// ת�����
void Test_TurnServo() {
   // INT16U agl;
    //watch = (INT8U)agl;
    SetServo((INT8U)servoTgtAngle);
    //servoCurrentAngle = (INT8U)agl;
    //FrontRun(motorV);
}

*/
/*****************************************************************************/
// ������

//INT16U crsN = 0;

/*
void Test_IR_GetBL(void) {
    INT16U i = 0, j;
    INT8U tpls = 0;
    // ���ڷ��͵�״̬, �ʹ�����
    //INT8U sciState = 0;//, sciChar;

    InitSpeeder();
    StartSpeeder();
    ClearDistanceCounter();


    Test_MotorCtrlInit();


    PORTB = 0xAA;
    DDRB = 0xFF;

    // ������?
    PTP_PTP0 = 1;
    DDRP_DDRP0 = 1;

    crsN = 0;
    nowLoop = 0;

    irInit();

    InitADC();

    InitSCI0();


    FOREVER() {
            // �ж�С�����Ƿ���, �������˵�
            if(!PTIP_PTIP0){
                WaitEnable();
                Wait(20);
                if(!PTIP_PTIP0) {
                    StopRun();
                    StartMenu();
                    WaitSmallButtonPress();
                    Wait(1500);
                }
                //WaitDisable();
            }

            // Ϊ�˲鿴�ж�ʱ��
            //PTP_PTP0 = !PTP_PTP0;


            // �ɼ�����ֵ
            for(i = 0;i < nIR / 2;i++) {
                irSendDouble(irRevPair[i][0], irRevPair[i][1]);

                for(j = 0;j < 100;j++);

                ReadADCDouble(irRevPair[i][2],irRevPair[i][3], &wir[irRevPair[i][0]], &wir[irRevPair[i][1]]);
            }

            // ����ֵ��һ��
            for(i = 0;i < nIR;i++) {
                if(whiteAvg[i] < wir[i]) {
                    irValue[i] = 100;
                } else if(blackAvg[i] > wir[i]) {
                    irValue[i] = 0;
                } else {
                    irValue[i] = (100 * (wir[i] - blackAvg[i])) / ( whiteAvg[i] - blackAvg[i]);
                }
            }

            // �����
            minIRv = 100;
            maxIRv = 0;
            ttotal = 0;
            for(i = 0;i < nIR;i++) {
                if( minIRv > irValue[i] ) {
                    minIRn2 = minIRn;
            		minIRn = i;
                    minIRv2 = minIRv;
            		minIRv = irValue[i];
                }

                if( maxIRv < irValue[i] ) {
                    maxIRv = irValue[i];
                }

                ttotal+= irValue[i];
		    }
		    total = ttotal;



            // �����ȷλ��
            if( minIRn <= 10 && minIRn >= 1) {
        		position = ir_position[minIRn - 1] + irValue[minIRn - 1] * 40 / (irValue[minIRn - 1] + irValue[minIRn + 1]);
        	} else if (minIRn == 0) {
        	    if(irValue[1] >= 90) {
        	        position = ir_position[0] - 20 * irValue[0] / (irValue[0] + irValue[1]);
        	    } else {
        	        position = ir_position[0] + 20 * irValue[0] / (irValue[0] + irValue[1]);
        	    }
        	} else {
        	    if(irValue[10] >= 92) {
        	        position = ir_position[11] + 20 * irValue[11] / (irValue[10] + irValue[11]);
        	    } else {
        	        position = ir_position[11] - 20 * irValue[11] / (irValue[10] + irValue[11]);
        	    }
        	}


        	// ��ģ�����жϸ���·�����
        	// TODO ����������Ȩֵ�����Ե���(ͨ������), ����������������
            //���                  minIRv              minIRv2             maxIRv              total
        	blStateArr[NORMAL]  =   (100 - minIRv) +    (100 - minIRv2) +   maxIRv          +   total / 12;
        	blStateArr[LOST]    =   minIRv         +    minIRv2         +   maxIRv          +   (100 - total / 12);
        	blStateArr[CROSS]   =   (100 - minIRv) +    (100 - minIRv2) +   (100 - maxIRv)  +   (100 - total / 12);
        	blStateArr[START]   =   (100 - minIRv) +    (100 - minIRv2) +   maxIRv          +   (100 - total / 12);

    	    // �����п��ܵ����
    	    blState = NORMAL;
        	for(i = 1;i < 4;i++) {
        	    if(blStateArr[blState] < blStateArr[i]) {
        	        blState = i;
        	    }
        	}

        	tBlState = blState;


        	// �����жϽ��������Ӧ
        	if( blState != NORMAL) {
        	    position = last_position;


        	    // ���������ʮ�ֻ������
        	    //    ��ʵ���������,������ʮ�ֵ�ʱ��ͬʱ���ܻ���ֽ���
        	    //    �����ֽ���ʱ�������ʮ��

        	    if(blState > 1 && crsN < 10) {
        	        if(crsN == 0) {
        	            crsType[crsN] = START;
        	            crsDist[crsN] = GetDistance();
        	            PORTB = 0xCC;
        	            crsN = 1;
        	            nowLoop = 1;
        	        } else {
        	            // ���������û�н���,���¼,���һ�����г�����������
        	            if(GetDistance() - crsDist[crsN - 1] > 75) {
                            PORTB = ~PORTB;
        	                crsType[crsN] = CROSS;
        	                crsDist[crsN] = GetDistance();
        	                crsN++;
        	            }
        	        }
        	        if(crsN == 3) {
        	            nowLoop = 2;

        	            PORTB = 0xA5;
        	        }
        	    }
        	} else {
                last_position = position;

                //�ۼӶ���Ƕ�
                if(PerDistSrvTotal < 0xFFFFFFF && PerDistSrvN < 0xFFF0) {
                    PerDistSrvTotal += servoTgtAngle;
                    PerDistSrvN++;
                }
        	}


            // �ó����ת��
            // TODO ʹ��һ��������������Ϊ����ṩת��
            servoTgtAngle = (INT8U)PosToAgl(position);

            // ��ת�ٿ���
            tgtSpeed = minSpeed + (maxSpeed - minSpeed) * (speed_arr[position] ) / 40;

            // ת�����
            Test_TurnServo();

            // ת�����, �����ƾ����
            if(DistLimit == 0) {
                Test_MotorCtrl();
            } else {
                // ����Ϊ��λ
                if(DistLimit * 25 >= GetDistance() ) {
                    Test_MotorCtrl();
                } else {
                    StopRun();
                }

            }

            tv = position;

            tdist = GetDistance();


    ///////////////////////////////////////////////////////////////////////
    }//forever
}
*/
/*
void TestIR_Senior(void) {

    PORTB = 0xAA;
    DDRB = 0xFF;

    //  ��ʼ��С����
    DDRP_DDRP0 = 0;
	PPSP_PPSP0 = 0;
	PERP_PERP0 = 1;

    IR_State = 0;

    InitServo();
    StartServo();
    SetServo(90);

    WaitEnable();

    Test_GetBlackAndWhite();

    PORTB = 0x55;

    Wait(1500);

    PORTB = 0xAA;

    while(PTIP_PTIP0);

    PORTB = 0x5A;

    Wait(1000);

    PORTB = 0xA5;

    Test_IR_GetBL();


}
*/
/******************************************************************************
�����C++����
-------------------------------------------------------------------------------
#include <cstdlib>
#include <iostream>
#include <math.h>

using namespace std;

#define PI 3.1415926
#define T 19.5f
double f(double x) {
    //int y;
    if(x > 0) {
        return  20.0f * (cos(((x + 3.0f) / 125.0f * PI + PI)) + 1.0f);
    } else {
        return  19.5f * (cos((x - 3.0f) / 125.0f * PI) - 1.0f);
    }
}
int main(int argc, char *argv[])
{
    float i = 0;
    float a = 130;
    unsigned int e = 0;
    cout << e << (int)e ;


    cout << "const INT8U servo_turn_arr[251] = {";
    for(i = 0;i <= 250;i++){
        if(i) cout << ", ";
        if(!(((int)i)%8)) cout << endl << "\t";

        //
        cout << (int) (f(i - 125) + 90);
        //
    }
    cout << endl << "};" << endl;
    system("PAUSE");
    return 0;
}
-------------------------------------------------------------------------------
******************************************************************************/
// �������ݲɼ�����
// ����ͷ���°ڶ���ʱ��, �������ֵ�ı仯
/*
void TestIR_Collection() {
    //INT8U i;

    INT16U j;

    //INT16U minIRn, minIRv;


    InitServo();
    StartServo();
    SetServo(90);

    InitSCI0();
    prints("IR Data\n");

    StartTimeBase();

    PORTB = 0xAA;
    DDRB = 0xFF;

    DDRA = 0xFF;
    irInit();

    InitADC();
    for(;;) {

        PORTA_BIT5 = 1;

        for(j = 0;j < 1000;j++);//Wait(1);

        wir[0] = ReadADC(11);


        PORTA_BIT5 = 0;

        PORTA_BIT4 = 1;

        for(j = 0;j < 1000;j++);//Wait(1);

        wir[1] = ReadADC(10);

        PORTA_BIT4 = 0;

    }


}*/

void TestIR_Collection() {
    INT8U i;

    INT16U j;

    INT16U minIRn, minIRv;


    InitServo();
    StartServo();
    SetServo(90);

    InitSCI0();
    prints("IR Data\n");

    StartTimeBase();

    PORTB = 0xAA;
    DDRB = 0xFF;

    irInit();

    InitADC();

    for (;;) {
        //for(x = 0;x < 1000;x++){
        //PORTB_BIT0 = ~PORTB_BIT0;
        for (i = 0;i < 12;i++) {
            //irSendDouble(irRevPair[i][0], irRevPair[i][1]);

            //for(j = 0;j < 100;j++);

            //ReadADCDouble(irRevPair[i][2],irRevPair[i][3], &wir[irRevPair[i][0]], &wir[irRevPair[i][1]]);


            irSend(i);


            for (j = 0;j < 1000;j++);//Wait(1);

            wir[i] = ReadADC(irRecvArr[i]);


            //PkgIR(i, wir[i]);
            //wh[i] = (INT8U)(wir[i] >> 8);
            //wl[i] = (INT8U)(wir[i] % 0xFF);
        }


        // �����
        minIRv = 0x1FF;
        for (i = 0;i < nIR;i++) {
            if ( minIRv> wir[i] ) {
                minIRn = i;
                minIRv = wir[i];
            }
            prints("%d\t", wir[i]);
        }
        prints("\n");
        tmir = minIRn;


        //}
    }

    //PkgEnd();
}
/*
// ����ͬʱ����������
void TestIR_Double(void) {
    INT16U i;

    INT16U j, m,n;

    StartTimeBase();

    PORTB = 0xAA;
    DDRB = 0xFF;

    irInit();

    InitADC();

    for(;;) {
        for(i = 0;i < 5000;i++) {
            irSendDouble(0,5);
            for(j = 0;j < 1000;j++);

            ReadADCDouble(irRecvArr[5],irRecvArr[0], &m, &n);
            wir[5] = m;
            wir[0] = n;

            //wh[i] = (INT8U)(wir[i] >> 8);
            //wl[i] = (INT8U)(wir[i] % 0xFF);
        }
        for(i = 0;i < 5000;i++) {
            irSendDouble(0,7);
            for(j = 0;j < 1000;j++);

            ReadADCDouble(irRecvArr[7],irRecvArr[0], &m, &n);
            wir[7] = m;
            wir[0] = n;

            //wh[i] = (INT8U)(wir[i] >> 8);
            //wl[i] = (INT8U)(wir[i] % 0xFF);
        }
        for(i = 0;i < 5000;i++) {
            //for(i = 0;i < nIR;i++) {
                irSend(0);
                for(j = 0;j < 1000;j++);//Wait(1);
                wir[0] = ReadADC(irRecvArr[0]);
              //  wh[i] = (INT8U)(wir[i] >> 8);
              //  wl[i] = (INT8U)(wir[i] % 0xFF);
            //}
        }
    }
}
*/
