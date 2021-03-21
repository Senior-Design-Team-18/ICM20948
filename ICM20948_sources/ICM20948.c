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

HAL_StatusTypeDef _ICM20948_BrustRead(I2C_HandleTypeDef * hi2c, bool altI2CAddr, uint8_t startAddress, uint8_t * readData, uint16_t amountOfRegistersToRead) {
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t deviceI2CAddress = (altI2CAddr == false)? ICM20948__I2C_SLAVE_ADDRESS_1: ICM20948__I2C_SLAVE_ADDRESS_2;

	status = HAL_I2C_Mem_Read(
			hi2c,
			deviceI2CAddress << 1,
			startAddress,
			I2C_MEMADD_SIZE_8BIT,
			readData,
			amountOfRegistersToRead,
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

HAL_StatusTypeDef _AK09918_BrustRead(I2C_HandleTypeDef * hi2c, bool altI2CAddr, uint8_t startAddress, uint8_t * readData, uint16_t amountOfRegistersToRead) {
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Read(
			hi2c,
			AK09918__I2C_SLAVE_ADDRESS << 1,
			startAddress,
			I2C_MEMADD_SIZE_8BIT,
			readData,
			amountOfRegistersToRead,
			10);

	return status;
}

uint8_t ICM20948_IsI2cAddress1(I2C_HandleTypeDef * hi2c) {
	HAL_StatusTypeDef addressStatus = HAL_I2C_IsDeviceReady(hi2c, ICM20948__I2C_SLAVE_ADDRESS_1 << 1, 2, 10);

	if (addressStatus == HAL_OK) {
		return 1;
	}
	
	return 0;
}

uint8_t ICM20948_IsI2cAddress2(I2C_HandleTypeDef * hi2c) {
	HAL_StatusTypeDef addressStatus = HAL_I2C_IsDeviceReady(hi2c, ICM20948__I2C_SLAVE_ADDRESS_2 << 1, 2, 10);

	if (addressStatus == HAL_OK) {
		return 1;
	}
	
	return 0;
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
			3 << GYRO_DLPFCFG_BIT | EN_GRYO_DLPF << GYRO_FCHOICE_BIT);

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

void ICM20948_getGryoReading(I2C_HandleTypeDef * hi2c, bool altI2CAddr, uint8_t selectGyroSensitivity) {
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t readData[6];
	int16_t gyroX;
	int16_t gyroY;
	int16_t gyroZ;

	status = _ICM20948_SelectUserBank(hi2c, altI2CAddr, USER_BANK_0);

	status = _ICM20948_BrustRead(hi2c, altI2CAddr, ICM20948__USER_BANK_0__GYRO_XOUT_H__REGISTER, readData, 6);

	gyroX = readData[0]<<8|readData[1];
	gyroY = readData[2]<<8|readData[3];
	gyroZ = readData[4]<<8|readData[5];

	switch (selectGyroSensitivity) {
		case 0:
			gyroX /= 131;
			gyroY /= 131;
			gyroZ /= 131;
			break;
		case 1:
			gyroX /= 65.5;
			gyroY /= 65.5;
			gyroZ /= 65.5;
			break;
		case 2:
			gyroX /= 32.8;
			gyroY /= 32.8;
			gyroZ /= 32.8;
			break;
		case 3:
			gyroX /= 16.4;
			gyroY /= 16.4;
			gyroZ /= 16.4;
			break;
	}
}

void ICM20948_getAccelReading(I2C_HandleTypeDef * hi2c, bool altI2CAddr, uint8_t selectAccelSensitivity) {
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t readData[6];
	int16_t accelX;
	int16_t accelY;
	int16_t accelZ;

	status = _ICM20948_SelectUserBank(hi2c, altI2CAddr, USER_BANK_0);

	status = _ICM20948_BrustRead(hi2c, altI2CAddr, ICM20948__USER_BANK_0__ACCEL_XOUT_H__REGISTER, readData, 6);

	accelX = readData[0]<<8|readData[1];
	accelY = readData[2]<<8|readData[3];
	accelZ = readData[4]<<8|readData[5];

	switch (selectAccelSensitivity) {
		case 0:
			accelX /= 16384;
			accelY /= 16384;
			accelZ /= 16384;
			break;
		case 1:
			accelX /= 8192;
			accelY /= 8192;
			accelZ /= 8192;
			break;
		case 2:
			accelX /= 4096;
			accelY /= 4096;
			accelZ /= 4096;
			break;
		case 3:
			accelX /= 2048;
			accelY /= 2048;
			accelZ /= 2048;
			break;
	}

}

void ICM20948_getMagReading(I2C_HandleTypeDef * hi2c, bool altI2CAddr) {
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t readData[6];
	int16_t magX;
	int16_t magY;
	int16_t magZ;

	status = _AK09918_BrustRead(hi2c, altI2CAddr, AK09916__XOUT_H__REGISTER, readData, 6);

	magX = readData[0]<<8|readData[1];
	magY = readData[2]<<8|readData[3];
	magZ = readData[4]<<8|readData[5];

	magX /= 4900;
	magY /= 4900;
	magZ /= 4900;

}
