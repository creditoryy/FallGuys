// MPU6050 Hardware Abstraction Layer - ESP32 Implementation
#include "hal/mpu6050.h"
#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Static MPU6050 object
static Adafruit_MPU6050 mpu;
static bool is_initialized = false;

bool mpu6050_init(void)
{
    Serial.println("MPU6050 - Initializing");
    
    if (is_initialized) {
        Serial.println("MPU6050 - Already initialized");
        return false;
    }

    // Initialize MPU6050
    if (!mpu.begin()) {
        Serial.println("MPU6050 - Failed to find sensor");
        return false;
    }

    // Configure MPU6050
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    is_initialized = true;
    Serial.println("MPU6050 - Initialized successfully");
    return true;
}

bool mpu6050_read_accel(mpu6050_accel_t *accel)
{
    if (!is_initialized || accel == NULL) {
        return false;
    }

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    accel->x = a.acceleration.x;
    accel->y = a.acceleration.y;
    accel->z = a.acceleration.z;

    return true;
}

bool mpu6050_read_gyro(mpu6050_gyro_t *gyro)
{
    if (!is_initialized || gyro == NULL) {
        return false;
    }

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    gyro->x = g.gyro.x;
    gyro->y = g.gyro.y;
    gyro->z = g.gyro.z;

    return true;
}

bool mpu6050_read_temp(mpu6050_temp_t *temp)
{
    if (!is_initialized || temp == NULL) {
        return false;
    }

    sensors_event_t a, g, t;
    mpu.getEvent(&a, &g, &t);
    
    temp->celsius = t.temperature;

    return true;
}

bool mpu6050_read_all(mpu6050_accel_t *accel, mpu6050_gyro_t *gyro, mpu6050_temp_t *temp)
{
    if (!is_initialized || accel == NULL || gyro == NULL || temp == NULL) {
        return false;
    }

    // Read all sensors at once (more efficient)
    sensors_event_t a, g, t;
    mpu.getEvent(&a, &g, &t);
    
    accel->x = a.acceleration.x;
    accel->y = a.acceleration.y;
    accel->z = a.acceleration.z;
    
    gyro->x = g.gyro.x;
    gyro->y = g.gyro.y;
    gyro->z = g.gyro.z;
    
    temp->celsius = t.temperature;

    return true;
}

void mpu6050_cleanup(void)
{
    Serial.println("MPU6050 - Cleanup");
    is_initialized = false;
}
