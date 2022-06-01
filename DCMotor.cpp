#include "Arduino.h"
#include <DCMotor.h>

DCMotor::DCMotor(unsigned int in1, unsigned int in2, unsigned int en, bool *isMovingForward)
{
    this->in1 = in1;
    this->in2 = in2;
    this->en = en;
    this->isMovingForward = isMovingForward;

    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
}

void DCMotor::move(double speed)
{
    if (speed == 0)
    {
        this->stop();
        return;
    }
    else if (*(this->isMovingForward))
    {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        analogWrite(en, speed);
    }
    else
    {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        analogWrite(en, speed);
    }
}

void DCMotor::stop()
{
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(en, 0);
}