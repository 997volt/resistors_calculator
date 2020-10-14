#ifndef RESISTOR_H
#define RESISTOR_H

#include <QString>

class Resistor
{

private:

    QString designator;
    QString value_s;
    int value;
    int tol;

    void get_value_from_string();
    void parse_other_formats();

public:

    Resistor();
    Resistor(QString _designator, QString _value_s);
    Resistor(QString _designator, QString _value_s, int _tol);

    int get_value();
    QString get_value_s();
    int get_tol();
};

#endif // RESISTOR_H

