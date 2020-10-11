#include "resistor.h"

Resistor::Resistor()
{
    this->designator = "";
    this->value_s = "";
    this->value = 0;
}

Resistor::Resistor(QString _designator)
{
    this->designator = _designator;
    this->value_s = "";
    this->value = 0;
}

Resistor::Resistor(QString _designator, QString _value_s)
{
    this->designator = _designator;
    this->value_s = _value_s;
    get_value_from_string();
}

void Resistor::get_value_from_string()
{
    int r_index = this->value_s.indexOf("R");
    int k_index = this->value_s.indexOf("k");
    int m_index = this->value_s.indexOf("M");

    if (r_index != -1)
        this->value = this->value_s.left(r_index).toInt();

    else if (k_index != -1)
    {
        if (k_index != this->value_s.length()-1)
            this->value = this->value_s.left(k_index).toInt() * 1000 + this->value_s.right(k_index).toInt() * 100;
        else
            this->value = this->value_s.left(k_index).toInt() * 1000;
    }

    else if (m_index != -1)
    {
        if (k_index != this->value_s.length()-1)
            this->value = 1000 * (this->value_s.left(k_index).toInt() * 1000 + this->value_s.right(k_index).toInt() * 100);
        else
            this->value = this->value_s.left(k_index).toInt() * 1000 * 1000;
    }
}

