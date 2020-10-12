#include "divider.h"

Divider::Divider()
{

}

Divider::Divider(Resistor r1, Resistor r2)
{
    this->r1_value = r1.get_value_s();
    this->r2_value = r2.get_value_s();
    set_div_ratio(r1.get_value(), r2.get_value());
    set_div_ratio_minmax(r1, r2);
}


void Divider::set_div_ratio_minmax(Resistor r1, Resistor r2)
{
    float r1_min = r1.get_value() * (1 - r1.get_tol() * 0.01);
    float r1_max = r1.get_value() * (1 + r1.get_tol() * 0.01);
    float r2_min = r2.get_value() * (1 - r2.get_tol() * 0.01);
    float r2_max = r2.get_value() * (1 + r2.get_tol() * 0.01);
    this->div_ratio_max = r2_max / (r1_min + r2_max);
    this->div_ratio_min = r2_min / (r1_max + r2_min);
}

void Divider::set_div_ratio(int r1_value, int r2_value)
{
    this->div_ratio = float(r2_value / float( r1_value + r2_value ));
}

QString Divider::get_r1_value()
{
    return this->r1_value;
}

QString Divider::get_r2_value()
{
    return this->r2_value;
}

float Divider::get_div_ratio()
{
    return this->div_ratio;
}

float Divider::get_div_ratio_min()
{
    return this->div_ratio_min;
}

float Divider::get_div_ratio_max()
{
    return this->div_ratio_max;
}
