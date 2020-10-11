#ifndef RESISTOR_H
#define RESISTOR_H

#include <QString>

class Resistor
{
private:
    QString designator;
    QString value_s;
    int value;
public:
    Resistor();
    Resistor(QString _designator);
    Resistor(QString _designator, QString _value_s);
    void get_value_from_string();
    int get_value();
};

#endif // RESISTOR_H






