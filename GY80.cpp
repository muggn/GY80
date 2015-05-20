#include <Arduino.h>
#include "GY80.h"

GY80::GY80()
{
    m_scale = 1;
    //a_scale = 0;
    g_scale = 1;
    g_scale_z = 1;
    m_scale_values[0] = 0;
    m_scale_values[1] = 100/1100;
    m_scale_values[2] = 100/855;
    m_scale_values[3] = 100/670;
    m_scale_values[4] = 100/450;
    m_scale_values[5] = 100/400;
    m_scale_values[6] = 100/330;
    m_scale_values[7] = 100/230;
    m_scale_values_z[0] = 0;
    m_scale_values_z[1] = 100/980;
    m_scale_values_z[2] = 100/760;
    m_scale_values_z[3] = 100/600;
    m_scale_values_z[4] = 100/400;
    m_scale_values_z[5] = 100/355;
    m_scale_values_z[6] = 100/295;
    m_scale_values_z[7] = 100/205;
    g_scale_values[0] = 0.007629;
    g_scale_values[1] = 0.015259;
    g_scale_values[2] = 0.061035;
    p_oss = 0;
}



void GY80::begin()
{
    Wire.begin();
    m_init();
    a_init();
    g_init();
    p_init();
}

GY80_raw GY80::read_raw()
{
    GY80_single_raw single;
    GY80_raw raw;

    single = m_read_raw();
    raw.m_x = single.x;
    raw.m_y = single.y;
    raw.m_z = single.z;

    single = g_read_raw();
    raw.g_x = single.x;
    raw.g_y = single.y;
    raw.g_z = single.z;

    single = a_read_raw();
    raw.a_x = single.x;
    raw.a_y = single.y;
    raw.a_z = single.z;

    raw.p = p_read_raw();
    raw.t = t_read_raw();

    return raw;
}
GY80_scaled GY80::read_scaled()
{
    GY80_single_scaled single;
    GY80_scaled scaled;

    single = m_read_scaled();
    scaled.m_x = single.x;
    scaled.m_y = single.y;
    scaled.m_z = single.z;

    single = g_read_scaled();
    scaled.g_x = single.x;
    scaled.g_y = single.y;
    scaled.g_z = single.z;

    single = a_read_scaled();
    scaled.a_x = single.x;
    scaled.a_y = single.y;
    scaled.a_z = single.z;

    scaled.p = p_read_scaled();
    scaled.t = t_read_scaled();

    return scaled;
}
//######################################################### MAGNETOMETER CODE
void GY80::m_init()
{
    m_set_scale(GY80_m_scale_8_1);
    m_set_mode(GY80_m_mode_continous);
}

GY80_single_raw GY80::m_read_raw()
{
    uint8_t* buffer = read(GY80_dev_m,GY80_m_reg_data, 6);
    GY80_single_raw raw;
    raw.x = (buffer[0] << 8) | buffer[1];
    raw.z = (buffer[2] << 8) | buffer[3];
    raw.y = (buffer[4] << 8) | buffer[5];
    return raw;
}

GY80_single_scaled GY80::m_read_scaled()
{
    GY80_single_raw raw = m_read_raw();
    GY80_single_scaled scaled;
    scaled.x = raw.x * m_scale;
    scaled.y = raw.y * m_scale;
    scaled.z = raw.z * m_scale_z;
    return scaled;
}

void GY80::m_set_scale(uint8_t scale)
{
    scale &= 0x07; //only lower 3 bits
    m_scale = m_scale_values[scale];
    m_scale_z = m_scale_values_z[scale];
    write(GY80_dev_m, GY80_m_reg_cfgB, (scale<<5));
}

void GY80::m_set_mode(uint8_t mode)
{
    write(GY80_dev_m, GY80_m_reg_mode, mode);
}

//######################################################### ACCELEROMETER CODE
void GY80::a_init()
{
    write(GY80_dev_a, GY80_a_reg_pwrctrl, 0x00);
	write(GY80_dev_a, GY80_a_reg_pwrctrl, 0x10);
	write(GY80_dev_a, GY80_a_reg_pwrctrl, 0x08);
    a_set_scale(GY80_a_scale_16);
    a_set_bw(GY80_a_bw_12_5);
}

void GY80::a_set_scale(uint8_t scale)
{
    uint8_t format = *read(GY80_dev_a,GY80_a_reg_format,1);

    scale &= 0x03;
    format &= ~0x0F;
    format |= scale;
    format |= 0x08;

    write(GY80_dev_a,GY80_a_reg_format, format);
}

void GY80::a_set_bw(uint8_t bw)
{
    bw &= 0x0F;
    write(GY80_dev_a,GY80_a_reg_bw,bw);
}

GY80_single_raw GY80::a_read_raw()
{
    GY80_single_raw raw;
    uint8_t * buf;
    buf = read(GY80_dev_a,GY80_a_reg_data,6); //read the acceleration data from the ADXL345

	// each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
	// thus we are converting both bytes in to one int
	raw.x = (((int16_t)buf[1]) << 8) | buf[0];
	raw.y = (((int16_t)buf[3]) << 8) | buf[2];
	raw.z = (((int16_t)buf[5]) << 8) | buf[4];
    return raw;
}

GY80_single_scaled GY80::a_read_scaled()
{
    GY80_single_scaled scaled;
    GY80_single_raw raw = a_read_raw();

    scaled.x = raw.x * GY80_a_scale;
    scaled.y = raw.y * GY80_a_scale;
    scaled.z = raw.z * GY80_a_scale;
    return scaled;
}
//######################################################### GYROMETER CODE
void GY80::g_init()
{
    write(GY80_dev_g, GY80_g_reg_ctrl1, 0x0F); //power on, enable x,y,z
    write(GY80_dev_g, GY80_g_reg_ctrl2, 0x00);
    write(GY80_dev_g, GY80_g_reg_ctrl3, 0x00);
    write(GY80_dev_g, GY80_g_reg_ctrl5, 0x00);
    g_set_scale(GY80_g_scale_2000);
}

void GY80::g_set_scale(uint8_t scale)
{
    switch(scale)
    {
    case GY80_g_scale_250:
        g_scale = g_scale_values[0];
        break;
    case GY80_g_scale_500:
        g_scale = g_scale_values[1];
        break;
    case GY80_g_scale_2000:
        g_scale = g_scale_values[2];
        break;
    default:
        scale = GY80_g_scale_2000;
        g_scale = g_scale_values[2];
        break;
    }
    write(GY80_dev_g, GY80_g_reg_ctrl4, scale);
}

GY80_single_raw GY80::g_read_raw()
{
    uint8_t buffer[6];

    for(uint8_t i = 0; i<6;i++)
    {
        buffer[i] = *read(GY80_dev_g,GY80_g_reg_datax+i, 1);
    }
    GY80_single_raw raw;
    raw.x = (buffer[1] << 8) | buffer[0];
    raw.y = (buffer[3] << 8) | buffer[2];
    raw.z = (buffer[5] << 8) | buffer[4];
    return raw;
}

GY80_single_scaled GY80::g_read_scaled()
{
    GY80_single_raw raw = g_read_raw();
    GY80_single_scaled scaled;
    scaled.x = raw.x * g_scale;
    scaled.y = raw.y * g_scale;
    scaled.z = raw.z * g_scale;
    return scaled;
}

//######################################################### BAROMETER CODE

void GY80::p_init()
{
    //Wire.begin();
    uint8_t * ptr;

    ptr = read(GY80_dev_p,0xAA,22);
    p_calib.ac1 = (ptr[0]<<8)|ptr[1];
    p_calib.ac2 = (ptr[2]<<8)|ptr[3];
    p_calib.ac3 = (ptr[4]<<8)|ptr[5];
    p_calib.ac4 = (ptr[6]<<8)|ptr[7];
    p_calib.ac5 = (ptr[8]<<8)|ptr[9];
    p_calib.ac6 = (ptr[10]<<8)|ptr[11];
    p_calib.b1 = (ptr[12]<<8)|ptr[13];
    p_calib.b2 = (ptr[14]<<8)|ptr[15];
    p_calib.mb = (ptr[16]<<8)|ptr[17];
    p_calib.mc = (ptr[18]<<8)|ptr[19];
    p_calib.md = (ptr[20]<<8)|ptr[21];

    t_read_scaled();
}

uint32_t GY80::p_read_raw()
{

    uint8_t *buffer;
    uint32_t up = 0;

    // Write 0x34+(OSS<<6) into register 0xF4
    // Request a pressure reading w/ oversampling setting
    write(GY80_dev_p, 0xF4, (0x34 + (p_oss<<6)));

    // Wait for conversion, delay time dependent on OSS
    delay(2 + (3<<p_oss));

    // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
    buffer = read(GY80_dev_p,0xF6,3);

    up = (((unsigned long) buffer[0] << 16) | ((unsigned long) buffer[1] << 8) | (unsigned long) buffer[2]) >> (8-p_oss);

    return up;
}

float GY80::p_read_scaled()
{
    long x1, x2, x3, b3, b6, p;
    unsigned long b4, b7;

    uint32_t up = p_read_raw();

    b6 = p_calib.b5 - 4000;
    // Calculate B3
    x1 = (p_calib.b2 * (b6 * b6)>>12)>>11;
    x2 = (p_calib.ac2 * b6)>>11;
    x3 = x1 + x2;
    b3 = (((((long)p_calib.ac1)*4 + x3)<<p_oss) + 2)>>2;

    // Calculate B4
    x1 = (p_calib.ac3 * b6)>>13;
    x2 = (p_calib.b1 * ((b6 * b6)>>12))>>16;
    x3 = ((x1 + x2) + 2)>>2;
    b4 = (p_calib.ac4 * (unsigned long)(x3 + 32768))>>15;

    b7 = ((unsigned long)(up - b3) * (50000>>p_oss));
    if (b7 < 0x80000000)
        p = (b7<<1)/b4;
    else
        p = (b7/b4)<<1;

    x1 = (p>>8) * (p>>8);
    x1 = (x1 * 3038)>>16;
    x2 = (-7357 * p)>>16;
    p += (x1 + x2 + 3791)>>4;

    float temp = ((float)p)/100000.0;
    return temp;
}

float GY80::p_read_altitude()
{
    float A = p_read_scaled()/1.01325;
    float B = 1/5.25588;
    float C = pow(A,B);
    C = 1 - C;
    C = C /0.0000225577;

    return C;
}

uint16_t GY80::t_read_raw()
{
    uint16_t ut;
    uint8_t *buffer;

    // Write 0x2E into Register 0xF4
    // This requests a temperature reading
    write(GY80_dev_p, 0xF4, 0x2E);

    // Wait at least 4.5ms
    delay(5);

    buffer = read(GY80_dev_p,0xF6,2);
    // Read two bytes from registers 0xF6 and 0xF7
    ut = (buffer[0]<<8)|buffer[1];
    return ut;
}

float GY80::t_read_scaled()
{
    uint16_t ut = t_read_raw();
    long x1, x2;

    x1 = (((long)ut - (long)p_calib.ac6)*(long)p_calib.ac5) >> 15;
    x2 = ((long)p_calib.mc << 11)/(x1 + p_calib.md);
    p_calib.b5 = x1 + x2;

    float temp = ((p_calib.b5 + 8)>>4);
    temp = temp /10;

    return temp;
}

//######################################################### I2C CODE
void GY80::write(uint8_t device, uint8_t address, uint8_t data)
{
    Wire.beginTransmission(device);
    Wire.write(address);
    Wire.write(data);
    Wire.endTransmission();
}

uint8_t* GY80::read(uint8_t device, uint8_t address, uint8_t length)
{
    Wire.beginTransmission(device);
    Wire.write(address);
    Wire.endTransmission();

    //Wire.beginTransmission(device);
    Wire.requestFrom(device, length);
    uint8_t buffer[length];
    while(Wire.available()<length);
    if(Wire.available() == length)
    {
        for(uint8_t i = 0; i < length; i++)
        {
            buffer[i] = Wire.read();
        }
    }
    Wire.endTransmission();

    return buffer;
}
