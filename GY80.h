#ifndef GY80_H
#define GY80_H

#include <Arduino.h>
#include <Wire.h>

#define GY80_dev_m 0x1E
#define GY80_dev_a 0x53
#define GY80_dev_g 0x69
#define GY80_dev_p 0x77

#define GY80_m_reg_cfgA 0x00
#define GY80_m_reg_cfgB 0x01
#define GY80_m_reg_mode 0x02
#define GY80_m_reg_data 0x03
#define GY80_m_mode_continous 0x00
#define GY80_m_mode_single 0x01
#define GY80_m_mode_idle 0x03
#define GY80_m_scale_0_88 0x00
#define GY80_m_scale_1_3 0x01
#define GY80_m_scale_1_9 0x02
#define GY80_m_scale_2_5 0x03
#define GY80_m_scale_4_0 0x04
#define GY80_m_scale_4_7 0x05
#define GY80_m_scale_5_6 0x06
#define GY80_m_scale_8_1 0x07

#define GY80_g_reg_ctrl1 0x20
#define GY80_g_reg_ctrl2 0x21
#define GY80_g_reg_ctrl3 0x22
#define GY80_g_reg_ctrl4 0x23
#define GY80_g_reg_ctrl5 0x24
#define GY80_g_reg_datax 0x28
#define GY80_g_reg_datay 0x2A
#define GY80_g_reg_dataz 0x2C
#define GY80_g_scale_250 0x00
#define GY80_g_scale_500 0x10
#define GY80_g_scale_2000 0x30

#define GY80_a_reg_pwrctrl 0x2D
#define GY80_a_reg_data 0x32
#define GY80_a_reg_format 0x31
#define GY80_a_reg_bw 0x2C
#define GY80_a_bw_3200 0b1111 // 1600Hz Bandwidth   140µA IDD
#define GY80_a_bw_1600 0b1110 //  800Hz Bandwidth    90µA IDD
#define GY80_a_bw_800  0b1101 //  400Hz Bandwidth   140µA IDD
#define GY80_a_bw_400  0b1100 //  200Hz Bandwidth   140µA IDD
#define GY80_a_bw_200  0b1011 //  100Hz Bandwidth   140µA IDD
#define GY80_a_bw_100  0b1010 //   50Hz Bandwidth   140µA IDD
#define GY80_a_bw_50   0b1001 //   25Hz Bandwidth    90µA IDD
#define GY80_a_bw_25   0b1000 // 12.5Hz Bandwidth    60µA IDD
#define GY80_a_bw_12_5 0b0111 // 6.25Hz Bandwidth    50µA IDD
#define GY80_a_bw_6_25 0b0110 // 3.13Hz Bandwidth    45µA IDD
#define GY80_a_bw_3_13 0b0101 // 1.56Hz Bandwidth    40µA IDD
#define GY80_a_bw_1_56 0b0100 // 0.78Hz Bandwidth    34µA IDD
#define GY80_a_bw_0_78 0b0011 // 0.39Hz Bandwidth    23µA IDD
#define GY80_a_bw_0_39 0b0010 // 0.20Hz Bandwidth    23µA IDD
#define GY80_a_bw_0_20 0b0001 // 0.10Hz Bandwidth    23µA IDD
#define GY80_a_bw_0_10 0b0000  // 0.05Hz Bandwidth    23µA IDD (default value)
#define GY80_a_scale 0.004
#define GY80_a_scale_2 0x00
#define GY80_a_scale_4 0x01
#define GY80_a_scale_8 0x02
#define GY80_a_scale_16 0x03



struct GY80_scaled
{
    float a_x;
    float a_y;
    float a_z;

    float m_x;
    float m_y;
    float m_z;

    float g_x;
    float g_y;
    float g_z;

    float p;
    float t;
};

struct GY80_raw
{
    int16_t a_x;
    int16_t a_y;
    int16_t a_z;

    int16_t m_x;
    int16_t m_y;
    int16_t m_z;

    int16_t g_x;
    int16_t g_y;
    int16_t g_z;

    uint32_t p;
    uint16_t t;
};

struct GY80_single_raw
{
    int16_t x;
    int16_t y;
    int16_t z;
};

struct GY80_single_scaled
{
    float x;
    float y;
    float z;
};

struct GY80_p_calibration_type
{
    int ac1;
    int ac2;
    int ac3;
    unsigned int ac4;
    unsigned int ac5;
    unsigned int ac6;
    int b1;
    int b2;
    int mb;
    int mc;
    int md;
    long b5;
};


class GY80
{
public:
    GY80();

    void begin();

    GY80_raw read_raw();
    GY80_scaled read_scaled();

    //magnetometer HMC5883L
    void m_set_mode(uint8_t mode);
    void m_set_scale(uint8_t scale);
    GY80_single_raw m_read_raw();
    GY80_single_scaled m_read_scaled();
    //accelerometer
    void a_set_scale(uint8_t scale);
    void a_set_bw(uint8_t bw);
    GY80_single_raw a_read_raw();
    GY80_single_scaled a_read_scaled();
    //gyro
    void g_set_scale(uint8_t scale);
    GY80_single_raw g_read_raw();
    GY80_single_scaled g_read_scaled();
    //pressure
    uint32_t p_read_raw();
    float p_read_scaled();
    float p_read_altitude();
    //temperature
    uint16_t t_read_raw();
    float t_read_scaled();




protected:
    void m_init();
    void a_init();
    void g_init();
    void p_init();

    void write(uint8_t device, uint8_t address, uint8_t data);
    uint8_t* read(uint8_t device, uint8_t address, uint8_t length);

private:
    float m_scale;
    float m_scale_values[8];
    float m_scale_z;
    float m_scale_values_z[8];
    float g_scale;
    float g_scale_values[3];
    unsigned char p_oss;
    GY80_p_calibration_type p_calib;
};


#endif // GY80_H
