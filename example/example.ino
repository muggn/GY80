#include <Wire.h>
#include <GY80.h>

GY80 sensor = GY80(); //create GY80 instance

void setup()
{
    // initialize serial communication at 115200 bits per second:
    Serial.begin(115200);
    sensor.begin();       //initialize sensors
}


void loop()
{
    GY80_scaled val = sensor.read_scaled();       //get values from all sensors
    // print out values

    Serial.print("Mag:");                         //magnetometer values
    Serial.print(val.m_x,2);
    Serial.print(',');
    Serial.print(val.m_y,2);
    Serial.print(',');
    Serial.print(val.m_z,2);
    Serial.print(' ');
    Serial.print("Acc:");                         //accelerometer values
    Serial.print(val.a_x,3);
    Serial.print(',');
    Serial.print(val.a_y,3);
    Serial.print(',');
    Serial.print(val.a_z,3);
    Serial.print(' ');
    Serial.print("Gyro:");                        //gyroscope values
    Serial.print(val.g_x,1);
    Serial.print(',');
    Serial.print(val.g_y,1);
    Serial.print(',');
    Serial.print(val.g_z,1);
    Serial.print(' ');
    Serial.print("P:");                           //pressure values
    Serial.print(val.p,5);
    Serial.print(' ');
    Serial.print("T:");                           //temperature values
    Serial.println(val.t,1);


    delay(250);        // delay in between reads for stability
}



