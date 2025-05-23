#ifndef STM1_H
#define STM1_H

class STM1 {
private:
    

public:
    STM1();
    double getLuchtkwaliteit();
    void handleTemperatureRGB(double temp);
private:
    double temperatuur;
    double co2;
    double luchtvochtigheid;



};


#endif