/**
 * @file icm20948.cpp
 * @author Vaibhav Bansal
 * @brief Source file containing APIs for handling IMU configurations
 * @version 0.1
 * @date 2021-11-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "i2c.h"
#include "icm20948.h"

I2CHandler i2c1;

/**
 * @brief Construct a new ICM20948::ICM20948 object
 * 
 */
ICM20948::ICM20948()
{
	i2c1.I2CInit();
}

/**
 * @brief Select the Register Bank
 * 
 * @param bank 
 */
void ICM20948::selectBank(uint8_t bank)
{
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, REG_BANK_SEL, &bank, 1);
}

/**
 * @brief Validate the ICM20948
 * 
 * @return true 
 * @return false 
 */
bool ICM20948::validateWhoAmI(void)
{
	selectBank(BANK_0);
	uint8_t data;
	i2c1.I2CRead_Data(SLAVE_ADDRESS, AGB0_REG_WHO_AM_I, &data, 1);
	return data == WHO_AM_I?true:false;
}

/**
 * @brief Softreset the device
 * 
 */
void ICM20948::softReset()
{
	selectBank(BANK_0);
	uint8_t data = SOFT_RESET;
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB0_REG_PWR_MGMT_1, &data, 1);
	//HAL_Delay(500);
	for(uint32_t i=0;i<100000;i++);
}

/**
 * @brief Enable/Disable Motion Interrupt
 * 
 * @param data 
 */
void ICM20948::MotionInterrupt(uint8_t data)
{
	selectBank(BANK_0);
	uint8_t intStatus = (data == enable)? MOTION_INT_EN : 0;
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB0_REG_INT_ENABLE, &intStatus, 1);
}

/**
 * @brief Enable/Disable Data ready interrupt
 * 
 * @param data 
 */
void ICM20948::DrdyInterrupt(uint8_t data)
{
	selectBank(BANK_0);
	uint8_t intStatus = (data == enable)? DRDY_INT_EN : 0;
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB0_REG_INT_ENABLE_1, &intStatus, 1);
}

/**
 * @brief Return Interrupt Status
 * 
 * @return uint8_t 
 */
uint8_t ICM20948::returnINTStatus()
{
	selectBank(BANK_0);
	uint8_t intStatus_1 = 0, intStatus_2 = 0;
	i2c1.I2CRead_Data(SLAVE_ADDRESS, AGB0_REG_INT_STATUS, &intStatus_1, 1);
	i2c1.I2CRead_Data(SLAVE_ADDRESS, AGB0_REG_INT_STATUS_1, &intStatus_2, 1);
	return (intStatus_1 | intStatus_2);
}

/**
 * @brief Read Accelerometer x,y,z axis raw data
 * 
 * @param ax 
 * @param ay 
 * @param az 
 */
void ICM20948::ReadAccData(int16_t *ax, int16_t *ay, int16_t *az)
{
	selectBank(BANK_0);
	uint8_t ax_h, ax_l, ay_h, ay_l, az_h, az_l;
	i2c1.I2CRead_Data(SLAVE_ADDRESS, AGB0_REG_ACCEL_XOUT_H, &ax_h, 1);
	i2c1.I2CRead_Data(SLAVE_ADDRESS, AGB0_REG_ACCEL_XOUT_L, &ax_l, 1);
	i2c1.I2CRead_Data(SLAVE_ADDRESS, AGB0_REG_ACCEL_YOUT_H, &ay_h, 1);
	i2c1.I2CRead_Data(SLAVE_ADDRESS, AGB0_REG_ACCEL_YOUT_L, &ay_l, 1);
	i2c1.I2CRead_Data(SLAVE_ADDRESS, AGB0_REG_ACCEL_ZOUT_H, &az_h, 1);
	i2c1.I2CRead_Data(SLAVE_ADDRESS, AGB0_REG_ACCEL_ZOUT_L, &az_l, 1);

	*ax = ax_h << 8 | ax_l;
	*ay = ay_h << 8 | ay_l;
	*az = az_h << 8 | az_l;

	if(*ax > 0x8000)
		*ax = *ax - 0xFFFF;

	if(*ay > 0x8000)
		*ay = *ay - 0xFFFF;

	if(*ay > 0x8000)
		*ay = *ay - 0xFFFF;

	*ax = *ax - Calax;
	*ay = *ay - Calay;
	*az = *az - Calaz;
	//printf(" accx = %d, accy = %d, accz = %d\n", *ax, *ay, *az);
}

/**
 * @brief Read gyroscope x,y,z axis raw data
 * 
 * @param gx 
 * @param gy 
 * @param gz 
 */
void ICM20948::ReadGyroData(int16_t *gx, int16_t *gy, int16_t *gz)
{
	selectBank(BANK_0);
	uint8_t gx_h, gx_l, gy_h, gy_l, gz_h, gz_l;
	i2c1.I2CRead_Data(SLAVE_ADDRESS, AGB0_REG_GYRO_XOUT_H, &gx_h, 1);
	i2c1.I2CRead_Data(SLAVE_ADDRESS, AGB0_REG_GYRO_XOUT_L, &gx_l, 1);
	i2c1.I2CRead_Data(SLAVE_ADDRESS, AGB0_REG_GYRO_YOUT_H, &gy_h, 1);
	i2c1.I2CRead_Data(SLAVE_ADDRESS, AGB0_REG_GYRO_YOUT_L, &gy_l, 1);
	i2c1.I2CRead_Data(SLAVE_ADDRESS, AGB0_REG_GYRO_ZOUT_H, &gz_h, 1);
	i2c1.I2CRead_Data(SLAVE_ADDRESS, AGB0_REG_GYRO_ZOUT_L, &gz_l, 1);

	*gx = gx_h << 8 | gx_l;
	*gy = gy_h << 8 | gx_l;
	*gz = gz_h << 8 | gz_l;

	if(*gx > 0x8000)
		*gx = *gx - 0xFFFF;

	if(*gy > 0x8000)
		*gy = *gy - 0xFFFF;

	if(*gy > 0x8000)
		*gy = *gy - 0xFFFF;

	*gx = *gx - Calgx;
	*gy = *gy - Calgy;
	*gz = *gz - Calgz;
	//printf(" gyrox = %d, gyroy = %d, gyroz = %d\n", *gx, *gy, *gz);
}

/**
 * @brief Initialise the IMU with default configuration
 * 
 */
void ICM20948::Init(void)
{
	//check device communication
	if(validateWhoAmI() == false)
		{
			printf("device detection fail, check connection or slave address\n");
			return;
		}

	printf("Device Found ICM20948\n");
	// soft reset the device
	softReset();

	//disable sleep mode
	enableSleep(disable);
	
	for(uint32_t i=0;i<10000;i++);

	//initialise external master for magnetometer
	ExternalMasterInit();

	//softreset magnetometer
	MagSoftReset();
	
	for(uint32_t i=0;i<10000;i++);

	//set accelerometer configuration
	setAccConfig(ACC_FULLSCALE_4g | FCHOICE_EN | DLPFCFG_3);

	//set gyroscope configuration
	setGyroConfig(GYRO_FULLSCALE_500dps | FCHOICE_EN | DLPFCFG_3);

	//set Wake on Motion Threshold
	setWOMThreshold(SET_WOM_THRESHOLD);

	//set Gyroscope Sampling Rate
	setGyroSamplingRate(GYRO_SAMPLING_RATE);

	//set Acccelerometer Sampling Rate
	setAccelSamplingRate(ACC_SAMPLING_RATE);

	
	for(uint32_t i=0;i<50000;i++);
	//Check magnetometer
	if(validateWHOAmIMag() == false)
	{
		printf("Mangneto meter detection Fail\n");
		return;
	}
	printf("Magneto Meter Detected\n");
	//Configure Magnetometer
	MagControl2Setting(MAG_CONTINUOUS_MEAUREMENT_MODE0);

	//uint8_t data = 1 << 7;
	//i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB0_REG_INT_PIN_CONFIG, &data, 1);

	//Enable WOM Interrupt
	enableWOM(enable);
}

/**
 * @brief Enable/Disbale IMU Sleep
 * 
 * @param data 
 */
void ICM20948::enableSleep(uint8_t data)
{
	selectBank(BANK_0);
	uint8_t mode = (data == enable)? SLEEP : NORMAL;
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB0_REG_PWR_MGMT_1, &mode, 1);
}

void ICM20948::enableSelfTestAcc(void)
{
	//self-test = self test output with self test enable - self test putput with self test not enable
}

void ICM20948::enableSelfTestGyro(void)
{
	//self-test = self test output with self test enable - self test putput with self test not enable
}

/**
 * @brief Set Gyro Sampling rate
 * 
 * @param data 
 */
void ICM20948::setGyroSamplingRate(uint8_t data)
{
	selectBank(BANK_2);
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB2_REG_GYRO_SMPLRT_DIV, &data, 1);
}
/**
 * @brief Set Accel Sampling rate
 * 
 * @param data 
 */
void ICM20948::setAccelSamplingRate(uint8_t data)
{
	selectBank(BANK_2);
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB2_REG_ACCEL_SMPLRT_DIV_2, &data, 1);
}

/**
 * @brief Set Accel configuration
 * 
 * @param data 
 */
void ICM20948::setAccConfig(uint8_t data)
{
	printf("Accelerometer config  = %02x\n",data);
	selectBank(BANK_2);
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB2_REG_ACCEL_CONFIG_1, &data, 1);
}

/**
 * @brief Set Gyro Configuration
 * 
 * @param data 
 */
void ICM20948::setGyroConfig(uint8_t data)
{
	printf("Gyroscope config  = %02x\n",data);
	selectBank(BANK_2);
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB2_REG_GYRO_CONFIG_1, &data, 1);
}

/**
 * @brief Enable/Disable WOM Interrupt
 * 
 * @param data 
 */
void ICM20948::enableWOM(uint8_t data)
{
	selectBank(BANK_2);
	uint8_t womEn = (data == enable)? ENABLE_WOM : 0;
	i2c1.I2CWrite_Data(SLAVE_ADDRESS,AGB2_REG_ACCEL_INTEL_CTRL, &womEn, 1);
}

/**
 * @brief Set WOM Threshold
 * 
 * @param data 
 */
void ICM20948::setWOMThreshold(uint8_t data)
{
	selectBank(BANK_2);
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB2_REG_ACCEL_WOM_THR, &data, 1);
}

/**
 * @brief Softreset Magnetometer
 * 
 */
void ICM20948::MagSoftReset()
{
	ExternalMasterWrite(MAG_I2C_ADDR, M_REG_CNTL3, MAG_SOFT_RESET);
}

/**
 * @brief Init External master in IMU
 * 
 */
void ICM20948::ExternalMasterInit(void)
{
	selectBank(BANK_3);
	uint8_t data= (ICM_I2C_MST_TRNSTN_BW_SLAVE | ICM_I2C_MST_CLK_FRQ_345v6KHz) ;
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB3_REG_I2C_MST_CTRL, &data, 1);
	selectBank(BANK_0);
	data = ICM_AUX_I2C_MST_EN;
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB0_REG_USER_CTRL, &data, 1);
}

/**
 * @brief Write data to Magnetometer through external master
 * 
 * @param slaveAddr 
 * @param regAddr 
 * @param data 
 */
void ICM20948::ExternalMasterWrite(uint8_t slaveAddr, uint8_t regAddr, uint8_t data)
{
	selectBank(BANK_3);
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB3_REG_I2C_SLV0_ADDR, &slaveAddr, 1);
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB3_REG_I2C_SLV0_REG, &regAddr, 1);
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB3_REG_I2C_SLV0_DO, &data, 1);
	uint8_t datactrl = ( I2C_SLAVE_0_EN | I2C_SLAVE_READ_BYTES );
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB3_REG_I2C_SLV0_CTRL, &datactrl, 1);
}

/**
 * @brief Read data from Magnetometer through external master
 * 
 * @param slaveAddr 
 * @param regAddr 
 * @param data 
 */
void ICM20948::ExternalMasterRead(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data)
{
	selectBank(BANK_3);
	slaveAddr |= 0x80;
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB3_REG_I2C_SLV0_ADDR, &slaveAddr, 1);
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB3_REG_I2C_SLV0_REG, &regAddr, 1);
	uint8_t ctrl = ( I2C_SLAVE_0_EN | I2C_SLAVE_READ_BYTES );
	i2c1.I2CWrite_Data(SLAVE_ADDRESS, AGB3_REG_I2C_SLV0_CTRL, &ctrl, 1);
	//HAL_Delay(2);
	for(uint32_t i=0;i<2000;i++);
	selectBank(BANK_0);
	i2c1.I2CRead_Data(SLAVE_ADDRESS, AGB0_REG_EXT_SLV_SENS_DATA_00, data, 1);
	//printf("Data received from i2c master for magneto = %d\n",data[0]);
}

/**
 * @brief Check Magnetometer
 * 
 * @return true 
 * @return false 
 */
bool ICM20948::validateWHOAmIMag()
{
	uint8_t dataWhoamI;
	ExternalMasterRead(MAG_I2C_ADDR, M_REG_WIA2, &dataWhoamI);
	return dataWhoamI == MAG_WHO_AM_I? true : false;
}

/**
 * @brief Return status of STATUS1 Register
 * 
 * @return uint8_t 
 */
uint8_t ICM20948::ReadMagStatus1(void)
{
	uint8_t status1;
	ExternalMasterRead(MAG_I2C_ADDR, M_REG_ST1, &status1);
	return status1;
}

/**
 * @brief Return status of STATUS2 Register
 * 
 * @return uint8_t 
 */
uint8_t ICM20948::ReadMagStatus2(void)
{
	uint8_t status2;
	ExternalMasterRead(MAG_I2C_ADDR, M_REG_ST2, &status2);
	return status2;
}

/**
 * @brief Read Magnetometer data
 * 
 * @param mx 
 * @param my 
 * @param mz 
 */
void ICM20948::ReadMagData(int16_t *mx, int16_t *my, int16_t *mz)
{
	uint8_t mx_h, mx_l, my_h, my_l, mz_h, mz_l;

	if((ReadMagStatus1() & 0x01) != 0x01)
		{
			printf("New Data stream is not arrived\n");
		}

	if(((ReadMagStatus2() >> 3) & 0x01) != 0x00)
		{
			printf("Data Stream Invalid\n");
			*mx = 0xFFFF;
			*my = 0xFFFF;
			*mz = 0xFFFF;
			return;
		}
	ExternalMasterRead(MAG_I2C_ADDR, M_REG_HXL, &mx_l);
	ExternalMasterRead(MAG_I2C_ADDR, M_REG_HXH, &mx_h);
	ExternalMasterRead(MAG_I2C_ADDR, M_REG_HYL, &my_l);
	ExternalMasterRead(MAG_I2C_ADDR, M_REG_HYH, &my_h);
	ExternalMasterRead(MAG_I2C_ADDR, M_REG_HZL, &mz_l);
	ExternalMasterRead(MAG_I2C_ADDR, M_REG_HZH, &mz_h);

	*mx = mx_h << 8 | mx_l;
	if(*mx > 0x8000)
			*mx = *mx - 0x8000;

	*my = my_h << 8 | mx_l;
	if(*my > 0x8000)
			*my = *my - 0x8000;

	*mz = mz_h << 8 | mz_l;
	if(*mz > 0x8000)
			*mz = *mz - 0x8000;

	MagControl2Setting(MAG_CONTINUOUS_MEAUREMENT_MODE0);
}

/**
 * @brief Set Magnetometer settings
 * 
 * @param data 
 */
void ICM20948::MagControl2Setting(uint8_t data)
{
	ExternalMasterWrite(MAG_I2C_ADDR, M_REG_CNTL2, data);
	//HAL_Delay(5);
	for(uint32_t i=0;i<5000;i++);
	data = 0;
	ExternalMasterRead(MAG_I2C_ADDR, M_REG_CNTL2, &data);

}

/**
 * @brief Set Accel, Gyro all axis calibration offsets
 * 
 * @param ax 
 * @param ay 
 * @param az 
 * @param gx 
 * @param gy 
 * @param gz 
 */
void ICM20948::SetCalibratedOffsets(int16_t ax, int16_t ay, int16_t az, int16_t gx, int16_t gy, int16_t gz)
{
	Calax = ax;
	Calay = ay;
	Calaz = az;
	Calgx = gx;
	Calgy = gy;
	Calgz = gz;
}
