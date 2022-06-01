#ifndef Helper_H
#define Helper_H

class Helper
{
public:
    void digitalWrites(uint8_t[], uint8_t); // same as digitalWrite but with an array as input pins
    void pinModes(uint8_t[], uint8_t);      // same as pinMode but with an array as input pins
    void plot(String, float, bool = false); // plot a value versus time using Serial Plotter
};
#endif // Helper_H