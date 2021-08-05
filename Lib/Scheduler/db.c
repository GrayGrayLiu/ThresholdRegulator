//
// Created by wirano on 2021/8/5.
//

#include "db.h"
#include "UsartScreen.h"
#include "easyflash.h"

extern ScreenSt Screen;

uint8_t ef_key[7][10]={"white","black","red","green","blue","yellow","mix"};
db_data data;

void db_task(void){
    if(Screen.WriteThreshold == 1){
        Screen.WriteThreshold = 0;

        data.HL = Screen.HL;
        data.HA = Screen.HA;
        data.HB = Screen.HB;
        data.LL = Screen.LL;
        data.LA = Screen.LA;
        data.LB = Screen.LB;

        if(ef_set_env_blob(ef_key[Screen.WriteColor],&data,sizeof(data)) == 0){
            UsartScreenWriteSuccess();
        }
    }

    if(Screen.ReadThreshold == 1)
    {
        Screen.ReadThreshold = 0;

        if(ef_get_env_blob(ef_key[Screen.ReadColor],&data,sizeof(data),NULL) == 0){
            SendScreenThreshold(Screen.ReadColor,data.LL,data.HL,data.LA,data.HA,data.LB,data.HB);
        }
    }
}
