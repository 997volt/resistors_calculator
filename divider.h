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
    void set_div_ratio(int r1_value, int r2_value);
    void set_div_ratio_minmax(Resistor r1, Resistor r2);
    float div_ratio_min;
    float div_ratio_max;
public:
    Divider();
    Divider(Resistor r1, Resistor r2);
    QString get_r1_value();
    QString get_r2_value();
    float get_div_ratio();
    float get_div_ratio_min();
    float get_div_ratio_max();
};

#endif // DIVIDER_H
