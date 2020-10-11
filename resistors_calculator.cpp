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
    float div_ratio = get_div_ratio();
    if(div_ratio > 0 && div_ratio < 1)
    {
        QList<Divider> dividers_list = get_all_dividers(resistors_list);
        for(int i = 0; i < 3; i++ )
        {
            ui->resistors_tableWidget->item(0, i)->setText("example");
        }
    }
}

QList<Divider> Resistors_Calculator::get_best_dividers(QList<Divider> dividers_list, float div_ratio)
{
    QList<Divider> best_dividers_list;

    return best_dividers_list;
}

QList<Divider> Resistors_Calculator::get_all_dividers(QList<Resistor> resistors_list)
{
    QList<Divider> dividers_list;

    for(int i = 0; i < resistors_list.length(); i++ )
    {
        for(int j = 0; j < resistors_list.length(); j++ )
        {
            Divider temp_div(resistors_list[i], resistors_list[j]);
            dividers_list.append(temp_div);
        }
    }

    return dividers_list;
}

float Resistors_Calculator::get_div_ratio()
{
    try
    {
        return ui->divider_lineEdit->text().toFloat();
    }
    catch(...)
    {
        return 0;
    }
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


