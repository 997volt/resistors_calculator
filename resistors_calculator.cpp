#include "resistors_calculator.h"
#include "ui_resistors_calculator.h"

Resistors_Calculator::Resistors_Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Resistors_Calculator)
{
    ui->setupUi(this);
}

Resistors_Calculator::~Resistors_Calculator()
{
    delete ui;
}

void Resistors_Calculator::on_calculate_pushButton_clicked()
{
    QList<Resistor> resistors_list = read_csv("D:\\bom.csv");
    float div_ratio = 0.5;
    Resistor * divider_resistors = resistors_search(resistors_list, div_ratio);
    int i = 0;
}

Resistor * Resistors_Calculator::resistors_search(QList<Resistor> resistors_list, float div_ratio)
{
    Resistor* divider_resistors = new Resistor[2];
    float best_div_ratio = 10;
    float temp_div_ratio;
    for(int i = 0; i < resistors_list.length(); i++ )
    {
        for(int j = 0; j < resistors_list.length(); j++ )
        {
            temp_div_ratio = float(resistors_list[j].get_value()) / float( resistors_list[i].get_value() + resistors_list[j].get_value() );
            if(abs(div_ratio - temp_div_ratio) < abs(best_div_ratio - div_ratio))
            {
                best_div_ratio = temp_div_ratio;
                divider_resistors[0] = resistors_list[i];
                divider_resistors[1] = resistors_list[j];
            }
        }
    }

    return divider_resistors;
}

QList<Resistor> Resistors_Calculator::read_csv(QString path)
{
    QList<Resistor> resistors_list;
    QFile file(path);

    if(file.exists()){
        if(file.open(QIODevice::ReadOnly))
        {
            while (!file.atEnd())
            {
                QByteArray line = file.readLine();
                line.replace(QByteArray("\",\""), QByteArray(";"));
                line.replace(QByteArray("\""), QByteArray(""));
                QString comp_name = line.split(';').first();
                if(comp_name.at(0) == "R")
                {
                    Resistor temp_res(comp_name, line.split(';').takeAt(3));
                    resistors_list.append(temp_res);
                }
            }
        }
    }
    return resistors_list;
}

QString Resistors_Calculator::get_bom_location()
{
    QFileDialog dialog(this);
    QStringList fileNames;
    QString bom_path = "";
    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();
        bom_path = fileNames[0];
    }
    return bom_path;
}


