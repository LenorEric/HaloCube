//
// Created by Lenor on 2021/10/14.
//

#include <ForDemoOnly.h>

extern uint32_t EnergyRecord[30][32];

#define EnergyBulb EnergyRecord[date][0]
#define EnergyPC EnergyRecord[date][1]
#define EnergyAC EnergyRecord[date][2]
#define EnergyFridge EnergyRecord[date][3]

void summonDemoData() {
    srand((unsigned)time(NULL));
    for (uint8_t date = 0; date < 30; date++) {
        EnergyBulb = (rand() % 12 + 4) * 3600 * 18;
        EnergyPC = (rand() % 10 + 3) * 3600 * 50;
        if (date < 20)
            EnergyAC = (rand() % 10 + 14) * 3600 * 100;
        EnergyFridge = 3456000;
    }
//    EEPROM_Save_Data(1, (uint8_t *) EnergyRecord[0], sizeof(EnergyRecord[DT_TS_TODAY]) * 30);
}

