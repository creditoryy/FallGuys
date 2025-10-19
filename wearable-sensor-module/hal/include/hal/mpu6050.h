// MPU6050 Hardware Abstraction Layer
// Provides interface for reading accelerometer and gyroscope data

#ifndef _MPU6050_H_
#define _MPU6050_H_

#include <stdbool.h>

// Data structure for accelerometer readings
typedef struct {
    float x;  // X-axis acceleration (m/s^2)
    float y;  // Y-axis acceleration (m/s^2)
    float z;  // Z-axis acceleration (m/s^2)
} mpu6050_accel_t;

// Data structure for gyroscope readings
typedef struct {
    float x;  // X-axis rotation (rad/s)
    float y;  // Y-axis rotation (rad/s)
    float z;  // Z-axis rotation (rad/s)
} mpu6050_gyro_t;

// Data structure for temperature reading
typedef struct {
    float celsius;  // Temperature in Celsius
} mpu6050_temp_t;

/**
 * Initialize the MPU6050 sensor
 * @return true if successful, false otherwise
 */
bool mpu6050_init(void);

/**
 * Read accelerometer data from MPU6050
 * @param accel Pointer to store accelerometer data
 * @return true if successful, false otherwise
 */
bool mpu6050_read_accel(mpu6050_accel_t *accel);

/**
 * Read gyroscope data from MPU6050
 * @param gyro Pointer to store gyroscope data
 * @return true if successful, false otherwise
 */
bool mpu6050_read_gyro(mpu6050_gyro_t *gyro);

/**
 * Read temperature from MPU6050
 * @param temp Pointer to store temperature data
 * @return true if successful, false otherwise
 */
bool mpu6050_read_temp(mpu6050_temp_t *temp);

/**
 * Read all sensor data at once (more efficient)
 * @param accel Pointer to store accelerometer data
 * @param gyro Pointer to store gyroscope data
 * @param temp Pointer to store temperature data
 * @return true if successful, false otherwise
 */
bool mpu6050_read_all(mpu6050_accel_t *accel, mpu6050_gyro_t *gyro, mpu6050_temp_t *temp);

/**
 * Cleanup and release MPU6050 resources
 */
void mpu6050_cleanup(void);

#endif // _MPU6050_H_
