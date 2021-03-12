#include <ICM20948_headers/ICM20948.h>

HAL_StatusTypeDef _ICM20948_SelectUserBank(I2C_HandleTypeDef * hi2c, bool altI2CAddr, int userBankNum) {
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t writeData = userBankNum << BIT_4;
	uint8_t deviceI2CAddress = (altI2CAddr == false)? ICM20948__I2C_SLAVE_ADDRESS_1: ICM20948__I2C_SLAVE_ADDRESS_2;

	status = HAL_I2C_Mem_Write(
			hi2c,
			deviceI2CAddress << 1,
			ICM20948__USER_BANK_ALL__REG_BANK_SEL__REGISTER,
			I2C_MEMADD_SIZE_8BIT,
			&writeData,
			I2C_MEMADD_SIZE_8BIT,
			10);

	return status;
}

HAL_StatusTypeDef _ICM20948_WriteByte(I2C_HandleTypeDef * hi2c, bool altI2CAddr, uint8_t registerAddress, uint8_t writeData) {
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t deviceI2CAddress = (altI2CAddr == false)? ICM20948__I2C_SLAVE_ADDRESS_1: ICM20948__I2C_SLAVE_ADDRESS_2;

	status = HAL_I2C_Mem_Write(
			hi2c,
			deviceI2CAddress << 1,
			registerAddress,
			I2C_MEMADD_SIZE_8BIT,
			&writeData,
			I2C_MEMADD_SIZE_8BIT,
			10);

	return status;
}

HAL_StatusTypeDef _ICM20948_ReadByte(I2C_HandleTypeDef * hi2c, bool altI2CAddr, uint8_t registerAddress, uint8_t * readData) {
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t deviceI2CAddress = (altI2CAddr == false)? ICM20948__I2C_SLAVE_ADDRESS_1: ICM20948__I2C_SLAVE_ADDRESS_2;

	status = HAL_I2C_Mem_Read(
			hi2c,
			deviceI2CAddress << 1,
			registerAddress,
			I2C_MEMADD_SIZE_8BIT,
			readData,
			I2C_MEMADD_SIZE_8BIT,
			10);

	return status;
}

HAL_StatusTypeDef _AK09918_WriteByte(I2C_HandleTypeDef * hi2c, uint8_t registerAddress, uint8_t writeData) {
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(
			hi2c,
			AK09918__I2C_SLAVE_ADDRESS << 1,
			registerAddress,
			I2C_MEMADD_SIZE_8BIT,
			&writeData,
			I2C_MEMADD_SIZE_8BIT,
			10);

	return status;
}

HAL_StatusTypeDef _AK09918_ReadByte(I2C_HandleTypeDef * hi2c, uint8_t registerAddress, uint8_t * readData) {
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(
			hi2c,
			AK09918__I2C_SLAVE_ADDRESS << 1,
			registerAddress,
			I2C_MEMADD_SIZE_8BIT,
			readData,
			I2C_MEMADD_SIZE_8BIT,
			10);

	return status;
}

/*
HAL_StatusTypeDef _BrustRead(I2C_HandleTypeDef * hi2c, bool altI2CAddr, uint8_t registerAddress, uint8_t * readData, int readCount) {
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t deviceI2CAddress = (altI2CAddr == false)? ICM20948__I2C_SLAVE_ADDRESS_1: ICM20948__I2C_SLAVE_ADDRESS_2;

	status = HAL_I2C_Mem_Read(
			hi2c,
			deviceI2CAddress << 1,
			registerAddress,
			I2C_MEMADD_SIZE_8BIT,
			readData,
			I2C_MEMADD_SIZE_8BIT,
			10);

	return status;
}
*/

void ICM20948_isDeviceReady(I2C_HandleTypeDef * hi2c, GPIO_TypeDef * GPIOx, uint16_t GPIO_Pin) {
    HAL_StatusTypeDef addressStatus1 = HAL_I2C_IsDeviceReady(hi2c, ICM20948__I2C_SLAVE_ADDRESS_1 << 1, 2, 10);
    HAL_StatusTypeDef addressStatus2 = HAL_I2C_IsDeviceReady(hi2c, ICM20948__I2C_SLAVE_ADDRESS_2 << 1, 2, 10);

    if (addressStatus1 == HAL_OK) {
        HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
        HAL_Delay(1000);
        HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
    } else if (addressStatus2 == HAL_OK) {
    	HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
		HAL_Delay(1000);
		HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
    }
}

void ICM20948_init(I2C_HandleTypeDef * hi2c, bool altI2CAddr) {
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t deviceI2CAddress = (altI2CAddr == false)? ICM20948__I2C_SLAVE_ADDRESS_1: ICM20948__I2C_SLAVE_ADDRESS_2;

	status = _ICM20948_SelectUserBank(hi2c, altI2CAddr, USER_BANK_0);

	status = _ICM20948_WriteByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_0__PWR_MGMT_1__REGISTER,
			CLKSEL_AUTO_SELECT << CLKSEL_BIT);

	status = _ICM20948_SelectUserBank(hi2c, altI2CAddr, USER_BANK_2);

	status = _ICM20948_WriteByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_2__GYRO_CONFIG_1__REGISTER,
			3 << GYRO_DLPFCFG_BIT || EN_GRYO_DLPF << GYRO_FCHOICE_BIT);

	status = _ICM20948_WriteByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_2__TEMP_CONFIG__REGISTER,
			0x03);

	status = _ICM20948_WriteByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_2__GYRO_SMPLRT_DIV__REGISTER,
			0x04);

	status = _ICM20948_WriteByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_2__ACCEL_CONFIG__REGISTER,
			0x19);

	status = _ICM20948_WriteByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_2__ACCEL_SMPLRT_DIV_2__REGISTER,
			0x04);

	status = _ICM20948_SelectUserBank(hi2c, altI2CAddr, USER_BANK_0);

	status = _ICM20948_WriteByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_0__INT_PIN_CFG__REGISTER,
			0x02);

	status = _AK09918_WriteByte(hi2c, AK09916__CNTL2__REGISTER, 0x08);
}

void ICM20948_getGryoReading(I2C_HandleTypeDef * hi2c, bool altI2CAddr) {
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t readData = 0;
	uint16_t gyroX = 0;
	uint16_t gyroY = 0;
	uint16_t gyroZ = 0;

	status = _ICM20948_SelectUserBank(hi2c, altI2CAddr, USER_BANK_0);

	status = _ICM20948_ReadByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_0__GYRO_XOUT_H__REGISTER,
			&readData);

	gyroX = readData << 8;
	status = _ICM20948_ReadByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_0__GYRO_XOUT_L__REGISTER,
			&readData);

	gyroX = readData | gyroX;

	status = _ICM20948_ReadByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_0__GYRO_YOUT_H__REGISTER,
			&readData);

	gyroY = readData << 8;
	status = _ICM20948_ReadByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_0__GYRO_YOUT_L__REGISTER,
			&readData);

	gyroY = readData | gyroY;

	status = _ICM20948_ReadByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_0__GYRO_ZOUT_H__REGISTER,
			&readData);

	gyroZ = readData << 8;
	status = _ICM20948_ReadByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_0__GYRO_ZOUT_L__REGISTER,
			&readData);

	gyroZ = readData | gyroZ;

}

void ICM20948_getAccelReading(I2C_HandleTypeDef * hi2c, bool altI2CAddr) {
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t readData = 0;
	uint16_t accelX = 0;
	uint16_t accelY = 0;
	uint16_t accelZ = 0;

	status = _ICM20948_SelectUserBank(hi2c, altI2CAddr, USER_BANK_0);

	status = _ICM20948_ReadByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_0__ACCEL_XOUT_H__REGISTER,
			&readData);

	accelX = readData << 8;
	status = _ICM20948_ReadByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_0__ACCEL_XOUT_L__REGISTER,
			&readData);

	accelX = readData | accelX;

	status = _ICM20948_ReadByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_0__ACCEL_YOUT_H__REGISTER,
			&readData);

	accelY = readData << 8;
	status = _ICM20948_ReadByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_0__ACCEL_YOUT_L__REGISTER,
			&readData);

	accelY = readData | accelY;

	status = _ICM20948_ReadByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_0__ACCEL_ZOUT_H__REGISTER,
			&readData);

	accelZ = readData << 8;
	status = _ICM20948_ReadByte(
			hi2c,
			altI2CAddr,
			ICM20948__USER_BANK_0__ACCEL_ZOUT_L__REGISTER,
			&readData);

	accelZ = readData | accelZ;

}

void ICM20948_getMagReading(I2C_HandleTypeDef * hi2c) {
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t readData = 0;
	uint16_t magX = 0;
	uint16_t magY = 0;
	uint16_t magZ = 0;

	status = _AK09918_ReadByte(hi2c, AK09916__XOUT_H__REGISTER , &readData);
	magX = readData << 8;
	status = _AK09918_ReadByte(hi2c, AK09916__XOUT_L__REGISTER , &readData);
	magX |= readData;

	status = _AK09918_ReadByte(hi2c, AK09916__YOUT_H__REGISTER , &readData);
	magY = readData << 8;
	status = _AK09918_ReadByte(hi2c, AK09916__YOUT_L__REGISTER , &readData);
	magY |= readData;

	status = _AK09918_ReadByte(hi2c, AK09916__ZOUT_H__REGISTER , &readData);
	magZ = readData << 8;
	status = _AK09918_ReadByte(hi2c, AK09916__ZOUT_L__REGISTER , &readData);
	magZ |= readData;
}
