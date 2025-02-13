/**
 * @file icm20948.h
 * @author Vaibhav Bansal
 * @brief This file contains a useful c translation of the datasheet register map of ICM20948
 * @version 0.1
 * @date 2021-11-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef _ICM20948_H_
#define _ICM20948_H_

#include <stdint.h>
#include <stm32g0xx_hal.h>

#define SLAVE_ADDRESS 		(0x69 << 1)
#define WHO_AM_I			0xEA
#define MAG_I2C_ADDR 		0x0C
#define MAG_WHO_AM_I 		0x09
#define SOFT_RESET      	1 << 7
#define MOTION_INT_EN		1 << 3
#define DRDY_INT_EN			1 << 0
#define ACC_SAMPLING_RATE	21
#define GYRO_SAMPLING_RATE	21

#define MAG_SOFT_RESET		1 << 0
#define ACC_FULLSCALE_2g	0 << 1
#define ACC_FULLSCALE_4g	1 << 1
#define ACC_FULLSCALE_8g	2 << 1
#define ACC_FULLSCALE_16g	3 << 1

#define GYRO_FULLSCALE_250dps	0 << 1
#define GYRO_FULLSCALE_500dps	1 << 1
#define GYRO_FULLSCALE_1000dps	2 << 1
#define GYRO_FULLSCALE_2500dps	3 << 1

#define FCHOICE_EN				1 << 0
#define FCHOICE_DIS				0 << 0

#define DLPFCFG_0		0 << 3
#define DLPFCFG_1		1 << 3
#define DLPFCFG_2		2 << 3
#define DLPFCFG_3		3 << 3
#define DLPFCFG_4		4 << 3
#define DLPFCFG_5		5 << 3
#define DLPFCFG_6		6 << 3
#define DLPFCFG_7		7 << 3

#define ENABLE_WOM		3 << 0
#define SET_WOM_THRESHOLD	50

#define SLEEP			0x41
#define NORMAL			0x01

#define MAG_CONTINUOUS_MEAUREMENT_MODE0		1 << 0
#define I2C_SLAVE_0_EN						1 << 7
#define I2C_SLAVE_READ_BYTES				1 << 0
#define ICM_AUX_I2C_MST_EN					1 << 5
#define ICM_I2C_MST_TRNSTN_BW_SLAVE			1 << 4
#define ICM_I2C_MST_CLK_FRQ_345v6KHz		7 << 0

typedef enum RegisterMap
{
	REG_BANK_SEL = 0x7F,
	AGB0_REG_WHO_AM_I = 0x00,
	AGB0_REG_USER_CTRL = 0x03,
	AGB0_REG_LP_CONFIG = 0x05,
	AGB0_REG_PWR_MGMT_1,
	AGB0_REG_PWR_MGMT_2,
	AGB0_REG_INT_PIN_CONFIG = 0x0F,
	AGB0_REG_INT_ENABLE,
	AGB0_REG_INT_ENABLE_1,
	AGB0_REG_INT_ENABLE_2,
	AGB0_REG_INT_ENABLE_3,
	AGB0_REG_I2C_MST_STATUS = 0x17,
	AGB0_REG_INT_STATUS = 0x19,
	AGB0_REG_INT_STATUS_1,
	AGB0_REG_INT_STATUS_2,
	AGB0_REG_INT_STATUS_3,
	AGB0_REG_DELAY_TIMEH = 0x28,
	AGB0_REG_DELAY_TIMEL,
	AGB0_REG_ACCEL_XOUT_H = 0x2D,
	AGB0_REG_ACCEL_XOUT_L,
	AGB0_REG_ACCEL_YOUT_H,
	AGB0_REG_ACCEL_YOUT_L,
	AGB0_REG_ACCEL_ZOUT_H,
	AGB0_REG_ACCEL_ZOUT_L,
	AGB0_REG_GYRO_XOUT_H,
	AGB0_REG_GYRO_XOUT_L,
	AGB0_REG_GYRO_YOUT_H,
	AGB0_REG_GYRO_YOUT_L,
	AGB0_REG_GYRO_ZOUT_H,
	AGB0_REG_GYRO_ZOUT_L,
	AGB0_REG_TEMP_OUT_H, 
	AGB0_REG_TEMP_OUT_L,
	AGB0_REG_EXT_SLV_SENS_DATA_00,
	AGB0_REG_EXT_SLV_SENS_DATA_01,
	AGB0_REG_EXT_SLV_SENS_DATA_02,
	AGB0_REG_EXT_SLV_SENS_DATA_03,
	AGB0_REG_EXT_SLV_SENS_DATA_04,
	AGB0_REG_EXT_SLV_SENS_DATA_05,
	AGB0_REG_EXT_SLV_SENS_DATA_06,
	AGB0_REG_EXT_SLV_SENS_DATA_07,
	AGB0_REG_EXT_SLV_SENS_DATA_08,
	AGB0_REG_EXT_SLV_SENS_DATA_09,
	AGB0_REG_EXT_SLV_SENS_DATA_10,
	AGB0_REG_EXT_SLV_SENS_DATA_11,
	AGB0_REG_EXT_SLV_SENS_DATA_12,
	AGB0_REG_EXT_SLV_SENS_DATA_13,
	AGB0_REG_EXT_SLV_SENS_DATA_14,
	AGB0_REG_EXT_SLV_SENS_DATA_15,
	AGB0_REG_EXT_SLV_SENS_DATA_16,
	AGB0_REG_EXT_SLV_SENS_DATA_17,
	AGB0_REG_EXT_SLV_SENS_DATA_18,
	AGB0_REG_EXT_SLV_SENS_DATA_19,
	AGB0_REG_EXT_SLV_SENS_DATA_20,
	AGB0_REG_EXT_SLV_SENS_DATA_21,
	AGB0_REG_EXT_SLV_SENS_DATA_22,
	AGB0_REG_EXT_SLV_SENS_DATA_23,
	AGB0_REG_FIFO_EN_1 = 0x66,
	AGB0_REG_FIFO_EN_2,
	AGB0_REG_FIFO_MODE,
	AGB0_REG_FIFO_COUNT_H = 0x70,
	AGB0_REG_FIFO_COUNT_L,
	AGB0_REG_FIFO_R_W,
	AGB0_REG_DATA_RDY_STATUS = 0x74,
	AGB0_REG_FIFO_CFG = 0x76,
	AGB0_REG_MEM_START_ADDR 	= 0x7C,		// Hmm, Invensense thought they were sneaky not listing these locations on the datasheet...
	AGB0_REG_MEM_R_W 			= 0x7D,		// These three locations seem to be able to access some memory within the device
	AGB0_REG_MEM_BANK_SEL 		= 0x7E,		// And that location is also where the DMP image gets loaded
	AGB0_REG_REG_BANK_SEL = 0x7F,
	AGB1_REG_SELF_TEST_X_GYRO = 0x02,
	AGB1_REG_SELF_TEST_Y_GYRO,
	AGB1_REG_SELF_TEST_Z_GYRO,
	AGB1_REG_SELF_TEST_X_ACCEL = 0x0E,
	AGB1_REG_SELF_TEST_Y_ACCEL,
	AGB1_REG_SELF_TEST_Z_ACCEL,
	AGB1_REG_XA_OFFS_H = 0x14,
	AGB1_REG_XA_OFFS_L,
	AGB1_REG_YA_OFFS_H = 0x17,
	AGB1_REG_YA_OFFS_L,
	AGB1_REG_ZA_OFFS_H = 0x1A,
	AGB1_REG_ZA_OFFS_L,
	AGB1_REG_TIMEBASE_CORRECTION_PLL = 0x28,
	AGB1_REG_REG_BANK_SEL = 0x7F,

	// Bank 2
	AGB2_REG_GYRO_SMPLRT_DIV = 0x00,
	AGB2_REG_GYRO_CONFIG_1,
	AGB2_REG_GYRO_CONFIG_2,
	AGB2_REG_XG_OFFS_USRH,
	AGB2_REG_XG_OFFS_USRL,
	AGB2_REG_YG_OFFS_USRH,
	AGB2_REG_YG_OFFS_USRL,
	AGB2_REG_ZG_OFFS_USRH,
	AGB2_REG_ZG_OFFS_USRL,
	AGB2_REG_ODR_ALIGN_EN,
	AGB2_REG_ACCEL_SMPLRT_DIV_1 = 0x10,
	AGB2_REG_ACCEL_SMPLRT_DIV_2,
	AGB2_REG_ACCEL_INTEL_CTRL,
	AGB2_REG_ACCEL_WOM_THR,
	AGB2_REG_ACCEL_CONFIG_1,
	AGB2_REG_ACCEL_CONFIG_2,
	AGB2_REG_FSYNC_CONFIG = 0x52,
	AGB2_REG_TEMP_CONFIG,
	AGB2_REG_MOD_CTRL_USR,
	AGB2_REG_REG_BANK_SEL = 0x7F,

	// Bank 3
	AGB3_REG_I2C_MST_ODR_CONFIG = 0x00,
	AGB3_REG_I2C_MST_CTRL,
	AGB3_REG_I2C_MST_DELAY_CTRL,
	AGB3_REG_I2C_SLV0_ADDR,
	AGB3_REG_I2C_SLV0_REG,
	AGB3_REG_I2C_SLV0_CTRL,
	AGB3_REG_I2C_SLV0_DO,
	AGB3_REG_I2C_SLV1_ADDR,
	AGB3_REG_I2C_SLV1_REG,
	AGB3_REG_I2C_SLV1_CTRL,
	AGB3_REG_I2C_SLV1_DO,
	AGB3_REG_I2C_SLV2_ADDR,
	AGB3_REG_I2C_SLV2_REG,
	AGB3_REG_I2C_SLV2_CTRL,
	AGB3_REG_I2C_SLV2_DO,
	AGB3_REG_I2C_SLV3_ADDR,
	AGB3_REG_I2C_SLV3_REG,
	AGB3_REG_I2C_SLV3_CTRL,
	AGB3_REG_I2C_SLV3_DO,
	AGB3_REG_I2C_SLV4_ADDR,
	AGB3_REG_I2C_SLV4_REG,
	AGB3_REG_I2C_SLV4_CTRL,
	AGB3_REG_I2C_SLV4_DO,
	AGB3_REG_I2C_SLV4_DI,
	AGB3_REG_REG_BANK_SEL = 0x7F,
	
	// Magnetometer
	M_REG_WIA2 = 0x01,
	M_REG_ST1 = 0x10,
	M_REG_HXL,
	M_REG_HXH,
	M_REG_HYL,
	M_REG_HYH,
	M_REG_HZL,
	M_REG_HZH,
	M_REG_ST2,
	M_REG_CNTL2 = 0x31,
	M_REG_CNTL3,
	M_REG_TS1,
	M_REG_TS2,
}ICM_20948_Reg_Addr_e;	// These enums are not needed for the user, so they stay in this scope (simplifies naming among other things)

typedef enum BANK
{
	BANK_0 = 0x00,
	BANK_1 = 0x10,
	BANK_2 = 0x20,
	BANK_3 = 0x30,
}BANK_t;

typedef enum
{
	disable,
	enable,
}enable_t;

/**
 * @brief Class containing APIs for configuring the ICM20948
 * 
 */
class ICM20948
{
	private:
		int16_t Calax = 0, Calay = 0, Calaz = 0, Calgx = 0, Calgy = 0, Calgz = 0;
		void selectBank(uint8_t bank);
		bool validateWhoAmI();
		bool validateWHOAmIMag();
		void softReset();
		void enableWOM(uint8_t data);
		void enableSleep(uint8_t data);
		void MagSoftReset();
		void ExternalMasterInit(void);
		void ExternalMasterWrite(uint8_t slaveAddress, uint8_t regAddr, uint8_t data);
		void ExternalMasterRead(uint8_t slaveAddress, uint8_t regAddr, uint8_t *data);
		uint8_t ReadMagStatus1(void);
		uint8_t ReadMagStatus2(void);
	public:
		ICM20948();
		void Init();
		void setAccConfig(uint8_t data);
		void setGyroConfig(uint8_t data);
		void ReadAccData(int16_t *ax, int16_t *ay, int16_t *az);
		void ReadGyroData(int16_t *gx, int16_t *gy, int16_t *gz);
		void ReadMagData(int16_t *mx, int16_t *my, int16_t *mz);
		void MagControl2Setting(uint8_t data);
		void MotionInterrupt(uint8_t data);
		void DrdyInterrupt(uint8_t data);
		uint8_t returnINTStatus();
		void enableSelfTestAcc();
		void enableSelfTestGyro();
		void setGyroSamplingRate(uint8_t data);
		void setAccelSamplingRate(uint8_t data);
		void setWOMThreshold(uint8_t data);
		void SetCalibratedOffsets(int16_t ax, int16_t ay, int16_t az, int16_t gx, int16_t gy, int16_t gz);
		void Datalogging(bool enable);
};
#endif /* _ICM_20948_REGISTERS_H_ */
