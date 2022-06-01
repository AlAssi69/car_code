#ifndef DCMotor_H
#define DCMotor_H

class DCMotor
{
private:
    unsigned int in1, in2, en; // pin numbers
    bool *isMovingForward;     // motor's status

public:
    DCMotor(unsigned int, unsigned int, unsigned int, bool *); // constructor
    void move(double);                                         // moves the motor at some given speed
    void stop();                                               // stops the motor
};
#endif // DCMotor_H