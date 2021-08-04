//
// Created by Lenor on 2021/7/25.
//

#include "MPU6050.h"

extern I2C_HandleTypeDef hi2c2;

HAL_StatusTypeDef MPU6050_I2C_Write(uint16_t MemAddress, uint8_t *pData, uint16_t Size) {
    while ((&hi2c2)->State != HAL_I2C_STATE_READY);
    return HAL_I2C_Mem_Write(&hi2c2, MPU6050_ADDRESS << 1, MemAddress, I2C_MEMADD_SIZE_8BIT, pData, Size, 100);
}

HAL_StatusTypeDef MPU6050_I2C_Read(uint16_t MemAddress, uint8_t *pData, uint16_t Size) {
    while ((&hi2c2)->State != HAL_I2C_STATE_READY);
    return HAL_I2C_Mem_Read(&hi2c2, (MPU6050_ADDRESS << 1) | 1, MemAddress, I2C_MEMADD_SIZE_8BIT, pData, Size, 100);
}

uint8_t MPU6050_init() {
    uint8_t data2send, data4read = 0;
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 0);
    HAL_Delay(50);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, 1);
    HAL_Delay(50);
    data2send = 0x80;
    HAL_StatusTypeDef status = MPU6050_I2C_Write(MPU_PWR_MGMT1_REG, &data2send, 1);
    if (status == HAL_BUSY){
        printf("HAL_BUSY \r\n");
        return 1;
    } else if (status == HAL_ERROR){
        printf("HAL_ERROR \r\n");
        return 2;
    }else if (status){
        printf("Unexpected Error \r\n");
        return 3;
    }
    HAL_Delay(10);
    data2send = 0x00;
    MPU6050_I2C_Write(MPU_PWR_MGMT1_REG, &data2send, 1);
    HAL_Delay(10);
    data2send = 0 << 3;
    MPU6050_I2C_Write(MPU_GYRO_CFG_REG, &data2send, 1);
    data2send = 0 << 3;
    MPU6050_I2C_Write(MPU_ACCEL_CFG_REG, &data2send, 1);
    uint8_t rate = 40;
    data2send = 1000 / rate - 1;
    MPU6050_I2C_Write(MPU_SAMPLE_RATE_REG, &data2send, 1);
    rate >>= 1;
    if (rate >= 188)data2send = 1;
    else if (rate >= 98)data2send = 2;
    else if (rate >= 42)data2send = 3;
    else if (rate >= 20)data2send = 4;
    else if (rate >= 10)data2send = 5;
    else data2send = 6;
    MPU6050_I2C_Write(MPU_CFG_REG, &data2send, 1);
    data2send = 0x00;
    MPU6050_I2C_Write(MPU_INT_EN_REG, &data2send, 1);
    data2send = 0x00;
    MPU6050_I2C_Write(MPU_USER_CTRL_REG, &data2send, 1);
    data2send = 0x00;
    MPU6050_I2C_Write(MPU_FIFO_EN_REG, &data2send, 1);
    data2send = 0x80;
    MPU6050_I2C_Write(MPU_INTBP_CFG_REG, &data2send, 1);
    MPU6050_I2C_Read(MPU_DEVICE_ID_REG, &data4read, 1);
    if (data4read == MPU6050_ADDRESS) {
        data2send = 0x01;
        MPU6050_I2C_Write(MPU_PWR_MGMT1_REG, &data2send, 1);
        data2send = 0x00;
        MPU6050_I2C_Write(MPU_PWR_MGMT2_REG, &data2send, 1);
        data2send = 1000 / rate - 1;
        MPU6050_I2C_Write(MPU_SAMPLE_RATE_REG, &data2send, 1);
        rate >>= 1;
        if (rate >= 188)data2send = 1;
        else if (rate >= 98)data2send = 2;
        else if (rate >= 42)data2send = 3;
        else if (rate >= 20)data2send = 4;
        else if (rate >= 10)data2send = 5;
        else data2send = 6;
        MPU6050_I2C_Write(MPU_CFG_REG, &data2send, 1);
    } else
        return 1;
    return 0;
}

void DMPGetDirection(uint8_t *direction) {
    float pitch = 0, roll = 0, yaw = 0;
    mpu_dmp_get_data(&pitch, &roll, &yaw);
    direction[0] = 0;
    direction[1] = 0;
}

void DirGetDirection(uint8_t *direction) {
    int16_t accX = 0, accY = 0, accZ = 0, g = 0, gsnb = 0;
    uint8_t data4read;
    if (MPU6050_I2C_Read(MPU_ACCEL_XOUTH_REG, &data4read, 1))
        while (MPU6050_init());
    accX = data4read << 8;
    MPU6050_I2C_Read(MPU_ACCEL_XOUTL_REG, &data4read, 1);
    accX |= data4read;
    MPU6050_I2C_Read(MPU_ACCEL_YOUTH_REG, &data4read, 1);
    accY = data4read << 8;
    MPU6050_I2C_Read(MPU_ACCEL_YOUTL_REG, &data4read, 1);
    accY |= data4read;
    MPU6050_I2C_Read(MPU_ACCEL_ZOUTH_REG, &data4read, 1);
    accZ = data4read << 8;
    MPU6050_I2C_Read(MPU_ACCEL_ZOUTL_REG, &data4read, 1);
    accZ |= data4read;
    ///Transform reference system
    accX = -accX;
    accY = -accY;
    if (abs(accX) > 18918 || abs(accY) > 18918 || abs(accZ) > 18918)
        return;
    if (accZ <= 0)
        return;
    g = sqrt(accX * accX + accY * accY + accZ * accZ);
    gsnb = sqrt(accX * accX + accY * accY);
    direction[0] = accY>0?acos((double) accX / gsnb) * 40.5845f:255-acos((double) accX / gsnb) * 40.5845f;
    direction[1] = acos((double) accZ / g) * 180 / 3.14f;
}

inline void GetDirection(uint8_t *direction) {
#ifdef MPU6050_DMP_ON
    DMPGetDirection(direction);
#else
    DirGetDirection(direction);
#endif
}
