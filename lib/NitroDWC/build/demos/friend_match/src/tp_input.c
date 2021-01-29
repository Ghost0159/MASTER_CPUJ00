#include <nitro.h>
#include "tp_input.h"

static TPData raw_point;
static TPData disp_point;
static TPCalibrateParam calibrate;

static char point_judge(int x, int y);

void tp_init(){
    TP_Init();
    // get CalibrateParameter from FlashMemory
    if (!TP_GetUserInfo(&calibrate))
    {
        OS_Panic("FATAL ERROR: can't read UserOwnerInfo\n");
    }
    else
    {
        OS_Printf("Get Calibration Parameter from NVRAM\n");
    }

    TP_SetCalibrateParam(&calibrate);
}

char tp_input(){
    static char tp_data = 0;

    while (TP_RequestRawSampling(&raw_point) != 0)
    {
    };
    TP_GetCalibratedPoint(&disp_point, &raw_point);

    //タッチペン位置の取得
    if (disp_point.touch)
    {
        switch (disp_point.validity)
        {
        case TP_VALIDITY_VALID:
            if(tp_data == 0){
                tp_data = point_judge(disp_point.x, disp_point.y);
                return tp_data;
            }
            break;
        case TP_VALIDITY_INVALID_X:
            break;
        case TP_VALIDITY_INVALID_Y:
            break;
        case TP_VALIDITY_INVALID_XY:
            break;
        }
    }
    else{
        tp_data = 0;
    }
    return 0;
}

static char point_judge(int x, int y){
    char i, j;

    // 1～9の入力
    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            if(82+i*32 < x && 82+(i+1)*32 > x){
                if(136-j*32 > y && 136-(j+1)*32 < y){
                    return (char)(i+1+j*3+48);
                }
            }
        }
    }
    if(136 < y && 168 > y){
        if(82 < x && 114 > x)// 0
            return '0';
        else if(114 < x && 146 > x)// <--
            return 1;
        else if(146 < x && 178 > x)// -->
            return 2;
    }
    
    return 0;

}
