#include "divider.h"

Divider::Divider()
{

}

Divider::Divider(Resistor r1, Resistor r2)
{
    this->r1_value = r1.get_value_s();
    this->r2_value = r2.get_value_s();
    this->config = 0;
    set_div_ratio(r1.get_value(), r2.get_value());
    set_div_ratio_minmax(r1, r2);
}

Divider::Divider(Resistor r1, Resistor r2, Resistor r3, int _config)
{
    this->r1_value = r1.get_value_s();
    this->r2_value = r2.get_value_s();
    this->r3_value = r3.get_value_s();
    this->config = _config;
    set_div_ratio(r1.get_value(), r2.get_value(), r3.get_value());
    set_div_ratio_minmax(r1, r2, r3);
}

void Divider::set_div_ratio(int r1_value, int r2_value)
{
    this->div_ratio = float(r2_value) / float(r1_value + r2_value);
}

/// config 1: r1_1, r1_2, r2
/// config 2: r1, r2_1, r2_2
void Divider::set_div_ratio(int r1_value, int r2_value, int r3_value)
{
    if(this->config == 1)
        this->div_ratio = float(r3_value) / float(r1_value + r2_value + r3_value);
    if(this->config == 2)
        this->div_ratio = float(r2_value + r3_value) / float(r1_value + r2_value + r3_value);
}

void Divider::set_div_ratio_minmax(Resistor r1, Resistor r2)
{
    float r1_min = r1.get_value() * (1 - r1.get_tol() * 0.01);
    float r1_max = r1.get_value() * (1 + r1.get_tol() * 0.01);
    float r2_min = r2.get_value() * (1 - r2.get_tol() * 0.01);
    float r2_max = r2.get_value() * (1 + r2.get_tol() * 0.01);

    this->div_ratio_max = float(r2_max) / float(r1_min + r2_max);
    this->div_ratio_min = float(r2_min) / float(r1_max + r2_min);
}

/// config 1: r1_1, r1_2, r2
/// config 2: r1, r2_1, r2_2
void Divider::set_div_ratio_minmax(Resistor r1, Resistor r2, Resistor r3)
{
    float r1_min = r1.get_value() * (1 - r1.get_tol() * 0.01);
    float r1_max = r1.get_value() * (1 + r1.get_tol() * 0.01);
    float r2_min = r2.get_value() * (1 - r2.get_tol() * 0.01);
    float r2_max = r2.get_value() * (1 + r2.get_tol() * 0.01);
    float r3_min = r3.get_value() * (1 - r3.get_tol() * 0.01);
    float r3_max = r3.get_value() * (1 + r3.get_tol() * 0.01);

    if(this->config == 1)
    {
        this->div_ratio_max = float(r3_max) / float(r1_min + r2_min + r3_max);
        this->div_ratio_min = float(r3_min) / float(r1_max + r2_max + r3_min);
    }
    if(this->config == 2)
    {
        this->div_ratio_max = float(r2_max + r3_max) / float(r1_min + r2_max + r3_max);
        this->div_ratio_min = float(r2_min + r3_min) / float(r1_max + r2_min + r3_min);
    }
}


QString Divider::get_r1_value()
{
    return this->r1_value;
}

QString Divider::get_r2_value()
{
    return this->r2_value;
}

QString Divider::get_r3_value()
{
    return this->r3_value;
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

int Divider::get_config()
{
    return this->config;
}
