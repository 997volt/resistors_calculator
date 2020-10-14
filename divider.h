#ifndef DIVIDER_H
#define DIVIDER_H

#include <QString>
#include <resistor.h>

class Divider
{
private:
    QString r1_value;
    QString r2_value;
    QString r3_value;
    float div_ratio;
    float div_ratio_min;
    float div_ratio_max;
    int config;

    void set_div_ratio(int r1_value, int r2_value);
    void set_div_ratio(int r1_value, int r2_value, int r3_value);
    void set_div_ratio_minmax(Resistor r1, Resistor r2);
    void set_div_ratio_minmax(Resistor r1, Resistor r2, Resistor r3);

public:
    Divider();
    Divider(Resistor r1, Resistor r2);
    Divider(Resistor r1, Resistor r2, Resistor r3, int _config);

    QString get_r1_value();
    QString get_r2_value();
    QString get_r3_value();
    float get_div_ratio();
    float get_div_ratio_min();
    float get_div_ratio_max();
    int get_config();
};

#endif // DIVIDER_H
