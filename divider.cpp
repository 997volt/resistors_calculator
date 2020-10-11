#include "divider.h"

Divider::Divider()
{

}

Divider::Divider(Resistor r1, Resistor r2)
{
    this->r1_value = r1.get_value_s();
    this->r2_value = r2.get_value_s();
    this->div_ratio = calculate_divider(r1.get_value(), r2.get_value());
}

float Divider::calculate_divider(int r1_value, int r2_value)
{
    return float(r2_value / float( r1_value + r2_value ));
}

float Divider::get_div_ratio()
{
    return this->div_ratio;
}

QString Divider::get_r1_value()
{
    return this->r1_value;
}

QString Divider::get_r2_value()
{
    return this->r2_value;
}
