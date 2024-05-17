/*
 *******************************************************************************
 *                              Register Symbols                               *
 *******************************************************************************
*/

//I2C address
#define MPU_6050_I2C_ADDR 0b1101000

// BIT_ masks
#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)
#define BIT_2 (1 << 2)
#define BIT_3 (1 << 3)
#define BIT_4 (1 << 4)
#define BIT_5 (1 << 5)
#define BIT_6 (1 << 6)
#define BIT_7 (1 << 7)

// [R] Accelerometer registers (X,Y,Z)
#define REG_AX_L 0x3C
#define REG_AX_H 0x3B
#define REG_AY_L 0x3E
#define REG_AY_H 0x3D
#define REG_AZ_L 0x40
#define REG_AZ_H 0x3F

// [R] Gyroscope registers (X,Y,Z)
#define REG_GX_L 0x44
#define REG_GX_H 0x43
#define REG_GY_L 0x46
#define REG_GY_H 0x45
#define REG_GZ_L 0x48
#define REG_GZ_H 0x47

// [R+W] Accelerometer configuration register + mode masks
#define REG_A_CFG 0x1C
#define A_CFG_2G 0x0
#define A_CFG_4G BIT_3
#define A_CFG_8G BIT_4
#define A_CFG_16G (BIT_3 | BIT_4)

// [R+W] Gyroscope configuration register + mode masks
#define REG_G_CFG 0x1B
#define G_CFG_250 0x0
#define G_CFG_500 BIT_3
#define G_CFG_1000 BIT_4
#define G_CFG_2000 (BIT_3 | BIT_4)

// [R+W] Power management (1) register + mode masks
#define REG_PWR_MGMT_1 0x6B
#define PWR_MGMT_1_RESET BIT_7
#define PWR_MGMT_1_SLEEP BIT_6

// [R+W] FIFO configuration register + mode masks
#define REG_FIFO_CFG 0x23
#define FIFO_CFG_TEMP BIT_7
#define FIFO_CFG_GX BIT_6
#define FIFO_CFG_GY BIT_5
#define FIFO_CFG_GZ BIT_4
#define FIFO_CFG_AXYZ BIT_3

// [R] FIFO count registers
#define REG_FIFO_COUNT_L 0x73
#define REG_FIFO_COUNT_H 0x72

// [R/W] FIFO output register
#define REG_FIFO 0x74

// [R+W] Sample-rate divider register
#define REG_SAMPLE_RATE_DIV 0x19

// [R+W] DLFP configuration register + masks
#define REG_DLFP_CFG 0x1A
#define DLFP_CFG_FILTER_0 0x0  // A{260Hz,0.0ms} G{256Hz 0.98ms} Fs=8kHz
#define DLFP_CFG_FILTER_1 0x1  // A{184Hz,2.0ms} G{188Hz 1.9ms}  Fs=1kHz
#define DLFP_CFG_FILTER_2 0x2  // A{94Hz, 3.0ms} G{98Hz  2.8ms}  Fs=1kHz
#define DLFP_CFG_FILTER_3 0x3  // A{44Hz, 4.9ms} G{42Hz, 4.8ms}  Fs=1kHz
#define DLFP_CFG_FILTER_4 0x4  // A{21Hz, 8.5ms} G{20Hz, 8.3ms}  Fs=1kHz
#define DLFP_CFG_FILTER_5 0x5  // A{10Hz,13.8ms} G{10Hz,13.4ms}  Fs=1kHz
#define DLFP_CFG_FILTER_6 0x6  // A{ 5Hz,19.0ms} G{ 5Hz,18.6ms}  Fs=1kHz

// [R+W] Interrupt enable register + masks
#define REG_INTR_EN 0x38
#define INTR_EN_DATA_RDY BIT_0
#define INTR_EN_FIFO_OFL BIT_4

// [R+W] Interrupt configuration register + masks
#define REG_INTR_CFG 0x37
#define INTR_CFG_ACTIVE_LOW BIT_7
#define INTR_CFG_OPEN_DRAIN BIT_6
#define INTR_CFG_LATCHING BIT_5
#define INTR_CFG_ANY_CLR BIT_4
#define INTR_CFG_FSYNC_LVL BIT_3
#define INTR_CFG_FSYNC_EN BIT_2
#define INTR_CFG_FSYNC_BYPASS BIT_1

// [R] Interrupt status register
#define REG_INTR_STATUS 0x3A

// [R+W] User control register + masks
#define REG_USER_CTRL 0x6A
#define USER_CTRL_FIFO_EN BIT_6
#define USER_CTRL_FIFO_RST BIT_2
#define USER_CTRL_SIG_COND_RESET BIT_0
