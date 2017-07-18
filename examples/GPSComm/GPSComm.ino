// Reed Foster
// 28 Jun 2017

#include <NMEAParser.hpp>
#include <Venus838.hpp>

#define GPS_DEBUG_BAUDRATE 9600

NMEAParser parser;
Venus838 gps(Serial1, 115200, false);

String errorcodes[5] = {"NORMAL", "NACK", "TIMEOUT", "INVALIDARG", "UNKNOWN"};

void setup()
{
    //while (!gps.available());
    //Serial.println(errorcodes[(int)gps.resetReceiver(true)]);
    //delay(1000);
    //while (!gps.available());
    //Serial.println(errorcodes[(int)gps.setBaudRate(19200, 1)]);
    //Serial.println("exiting setup");
    //Serial1.begin(9600);
    gps.setBaudRate(115200, 0);
    gps.cfgNMEA(0b0000000, 0);
    gps.cfgNMEA(NMEA_GGA, true, 0);
    gps.cfgNMEA(NMEA_GSA, true, 0);
    gps.cfgNMEA(NMEA_GSV, true, 0);
    gps.cfgNMEA(NMEA_RMC, true, 0);
    gps.setUpdateRate(20, 0);
    Serial.begin(9600);
}

void loop()
{
    bool newdata = false;
    char c;
    while (gps.available())
    {
        c = gps.read();
        //Serial.write(c);
        newdata = parser.encode(c);
    }
    if (newdata)
    {
        long latitude_u, latitude_l;
        parser.getLatitude(&latitude_u, &latitude_l);
        long longitude_u, longitude_l;
        parser.getLongitude(&longitude_u, &longitude_l);
        long altitude = parser.getAltitude();
        short pdop = parser.getPDOP();
        short hdop = parser.getHDOP();
        short vdop = parser.getVDOP();
        char numsats = parser.getNSats();
        char numsatsvisible = parser.getNSatsVisible();
        long snravg = parser.getSNR();
        char nsnr = parser.getNSNR();
        Serial.printf("%d.%07d,%d.%07d,%d,%d,%d,%d,%d,%d,%d,%d\n", latitude_u, latitude_l, longitude_u, longitude_l, altitude, pdop, hdop, vdop, numsats, numsatsvisible, snravg, nsnr);
    }
}
