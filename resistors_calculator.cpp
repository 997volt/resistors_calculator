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
        QList<Divider> dividers_list = get_all_dividers(
            resistors_list, ui->r_tot_min_lineEdit->text().toInt(), ui->r_tot_max_lineEdit->text().toInt());
        QVector<QPair<int,float>> best_dividers_indexes = get_best_dividers(dividers_list, div_ratio);
        QVector<Divider> best_dividers(3);
        for(int i = 0; i < 3; i++ )
        {
            best_dividers[i] = dividers_list[best_dividers_indexes[i].first];
        }
        update_resistors_tableWidget(best_dividers);
    }
}

void Resistors_Calculator::update_resistors_tableWidget(QVector<Divider> new_dividers)
{
    for(int i = 0; i < 3; i++ )
    {
        ui->resistors_tableWidget->item(i, 0)->setText(new_dividers[i].get_r1_value());
        ui->resistors_tableWidget->item(i, 1)->setText(new_dividers[i].get_r2_value());
        ui->resistors_tableWidget->item(i, 2)->setText(QString::number(new_dividers[i].get_div_ratio()));
    }
}

QVector<QPair<int,float>> Resistors_Calculator::get_best_dividers(QList<Divider> dividers_list, float div_ratio)
{
    // vector of best dividers pair - index, deviation
    QVector<QPair<int,float>> best_dividers(3,qMakePair(0,1));

    for(int i = 0; i < dividers_list.length(); i++ )
    {
        float deviation = div_ratio - dividers_list[i].get_div_ratio();
        if(abs(deviation) < abs(best_dividers[0].second))
        {
            best_dividers[2] = best_dividers[1];
            best_dividers[1] = best_dividers[0];
            best_dividers[0] = qMakePair(i,deviation);
        }
        else if (abs(deviation) < abs(best_dividers[1].second))
        {
            best_dividers[2] = best_dividers[1];
            best_dividers[1] = qMakePair(i,deviation);
        }
        else if (abs(deviation) < abs(best_dividers[2].second))
        {
            best_dividers[2] = qMakePair(i,deviation);
        }
    }

    return best_dividers;
}

QList<Divider> Resistors_Calculator::get_all_dividers(QList<Resistor> resistors_list, int r_tot_min, int r_tot_max)
{
    QList<Divider> dividers_list;

    for(int i = 0; i < resistors_list.length(); i++ )
    {
        for(int j = 0; j < resistors_list.length(); j++ )
        {
            int r_tot = resistors_list[i].get_value() + resistors_list[j].get_value();
            if( r_tot <= r_tot_max && (r_tot >= r_tot_min || r_tot_min <= 0))
            {
                Divider temp_div(resistors_list[i], resistors_list[j]);
                dividers_list.append(temp_div);
            }
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


