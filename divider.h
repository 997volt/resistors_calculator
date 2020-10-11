#ifndef DIVIDER_H
#define DIVIDER_H

#include <QString>
#include <resistor.h>

class Divider
{
private:
    QString r1_value;
    QString r2_value;
    float div_ratio;
    float calculate_divider(int r1_value, int r2_value);
public:
    Divider();
    Divider(Resistor r1, Resistor r2);
};

#endif // DIVIDER_H
