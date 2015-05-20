# GY80 All-in-one Library

## Documentation

Copy Folder to Arduino/libraries

Include:

	#include <GY80.h>

Create the GY80 class (here named sensor):
	
	GY80 sensor = GY80();

For initialization call once:

	sensor.begin();

To get values from all sensors:

	GY80_scaled values = sensor.read_scaled;
	
See below for the GY80_scaled type.

### Measuring functions:

    GY80_scaled read_scaled();
	returns scaled values from all sensors
    GY80_single_scaled m_read_scaled();
	returns scaled values from magnetometer in microTesla [µT]
    GY80_single_scaled a_read_scaled();
	returns scaled accelerometer values in G, 1G = 9.81 m/s2
    GY80_single_scaled g_read_scaled();
	returns scaled gyroscope values in dps, 6dps = 1rpm
    float p_read_scaled();
	returns pressure value in bar
    float p_read_altitude();
	returns altitude in m - not accurate, better used only for measuring
	altitude differences and over a short time as air pressure changes 
	with time
    float t_read_scaled();
	get temperature value in degree celsius
	
### Measuring functions [raw values]:

    GY80_raw read_raw();
	returns raw values from all sensors
    GY80_single_raw m_read_raw();
	returns raw values from magnetometer
    GY80_single_raw a_read_raw();
	returns raw accelerometer values
    GY80_single_raw g_read_raw();
	returns raw gyroscope values 
    uint32_t p_read_raw();
	returns raw pressure value
    uint16_t t_read_raw();
	get raw temperature value

### Functions for configuration:

    void m_set_scale(uint8_t scale);
	set magnetometer full scale range [values in µT]
		GY80_m_scale_88
		GY80_m_scale_130
		GY80_m_scale_190
		GY80_m_scale_250
		GY80_m_scale_400
		GY80_m_scale_470
		GY80_m_scale_560
		GY80_m_scale_810 <-default

    void a_set_scale(uint8_t scale);
	set accelerometer full scale range in G
		GY80_a_scale_2
		GY80_a_scale_4
		GY80_a_scale_8
		GY80_a_scale_16	<-default

    void a_set_bw(uint8_t bw);
	set measuring frequency, can be:
		GY80_a_bw_3200 	// 1600Hz Bandwidth   140µA IDD
		GY80_a_bw_1600 	//  800Hz Bandwidth    90µA IDD
		GY80_a_bw_800  	//  400Hz Bandwidth   140µA IDD
		GY80_a_bw_400  	//  200Hz Bandwidth   140µA IDD
		GY80_a_bw_200  	//  100Hz Bandwidth   140µA IDD
		GY80_a_bw_100  	//   50Hz Bandwidth   140µA IDD
		GY80_a_bw_50   	//   25Hz Bandwidth    90µA IDD
		GY80_a_bw_25   	// 12.5Hz Bandwidth    60µA IDD
		GY80_a_bw_12_5 	// 6.25Hz Bandwidth    50µA IDD (default value)
		GY80_a_bw_6_25 	// 3.13Hz Bandwidth    45µA IDD
		GY80_a_bw_3_13 	// 1.56Hz Bandwidth    40µA IDD
		GY80_a_bw_1_56 	// 0.78Hz Bandwidth    34µA IDD
		GY80_a_bw_0_78 	// 0.39Hz Bandwidth    23µA IDD
		GY80_a_bw_0_39 	// 0.20Hz Bandwidth    23µA IDD
		GY80_a_bw_0_20 	// 0.10Hz Bandwidth    23µA IDD
		GY80_a_bw_0_10 	// 0.05Hz Bandwidth    23µA IDD

    void g_set_scale(uint8_t scale);
	sets gyroscpe scale in dps, can be:
		GY80_g_scale_250
		GY80_g_scale_500
		GY80_g_scale_2000


### Struct types
	
	GY80_scaled
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
	
	GY80_raw
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

	
	GY80_single_raw
	    int16_t x;
	    int16_t y;
	    int16_t z;
	
	GY80_single_scaled
	    float x;
	    float y;
	    float z;

## Warning!
I have never tested if the values given from this lib are correct and/or precise!!
This program is a free software and there is NO WARRANTY.
No restrictions on use. You can use, modify and redistribute it for
personal, non-profit or commercial use UNDER YOUR RESPONSIBILITY.
