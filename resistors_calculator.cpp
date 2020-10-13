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
    QString path = "D:\\bom.csv";
    QList<Resistor> resistors_list = read_csv(path);
    ui->events_textBrowser->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") + ": BOM file loaded from path " + path);
    float div_ratio = get_div_ratio();
    if(div_ratio > 0 && div_ratio < 1)
    {
        QList<Divider> dividers_list = get_all_dividers(
            resistors_list, ui->r_tot_min_lineEdit->text().toInt(), ui->r_tot_max_lineEdit->text().toInt());
        QVector<QPair<int,float>> empty(9,qMakePair(0,1)), best_dividers_indexes = get_best_dividers(dividers_list, div_ratio);
        if(best_dividers_indexes != empty)
        {
            QVector<Divider> best_dividers(9);
            for(int i = 0; i < 9; i++ )
            {
                best_dividers[i] = dividers_list[best_dividers_indexes[i].first];
            }
            update_resistors_tableWidget(best_dividers);
        }
    }

}

void Resistors_Calculator::update_resistors_tableWidget(QVector<Divider> new_dividers)
{
    for(int i = 0; i < 3; i++ )
    {
        ui->resistors_tableWidget->item(i, 0)->setText(new_dividers[i].get_r1_value());
        ui->resistors_tableWidget->item(i, 1)->setText(new_dividers[i].get_r2_value());
        ui->resistors_tableWidget->item(i, 3)->setText(QString::number(new_dividers[i].get_div_ratio()));
        ui->resistors_tableWidget->item(i, 4)->setText(QString::number(new_dividers[i].get_div_ratio_min()));
        ui->resistors_tableWidget->item(i, 5)->setText(QString::number(new_dividers[i].get_div_ratio_max()));
    }

    for(int i = 0; i < 3; i++ )
    {
        ui->resistors_co1_tableWidget->item(i, 0)->setText(new_dividers[3+i].get_r1_value());
        ui->resistors_co1_tableWidget->item(i, 1)->setText(new_dividers[3+i].get_r2_value());
        ui->resistors_co1_tableWidget->item(i, 2)->setText(new_dividers[3+i].get_r3_value());
        ui->resistors_co1_tableWidget->item(i, 3)->setText(QString::number(new_dividers[3+i].get_div_ratio()));
        ui->resistors_co1_tableWidget->item(i, 4)->setText(QString::number(new_dividers[3+i].get_div_ratio_min()));
        ui->resistors_co1_tableWidget->item(i, 5)->setText(QString::number(new_dividers[3+i].get_div_ratio_max()));
    }

    for(int i = 0; i < 3; i++ )
    {
        ui->resistors_co2_tableWidget->item(i, 0)->setText(new_dividers[6+i].get_r1_value());
        ui->resistors_co2_tableWidget->item(i, 1)->setText(new_dividers[6+i].get_r2_value());
        ui->resistors_co2_tableWidget->item(i, 2)->setText(new_dividers[6+i].get_r3_value());
        ui->resistors_co2_tableWidget->item(i, 3)->setText(QString::number(new_dividers[6+i].get_div_ratio()));
        ui->resistors_co2_tableWidget->item(i, 4)->setText(QString::number(new_dividers[6+i].get_div_ratio_min()));
        ui->resistors_co2_tableWidget->item(i, 5)->setText(QString::number(new_dividers[6+i].get_div_ratio_max()));
    }
}

/// returned vector contains all configurations in order 0, 1, 2
QVector<QPair<int,float>> Resistors_Calculator::get_best_dividers(QList<Divider> dividers_list, float div_ratio)
{
    // vector of best dividers pair - index, deviation
    QVector<QPair<int,float>> best_dividers(9,qMakePair(0,1));

    if(dividers_list.length() == 0)
        return best_dividers;

    for(int i = 0; i < dividers_list.length(); i++ )
    {
        float deviation = div_ratio - dividers_list[i].get_div_ratio();

        for(int j = 0; j < 3; j++ )
        {
            if(dividers_list[i].get_config() == j)
            {
                deviation_indexer(best_dividers, i, deviation, j);
            }
        }
    }

    return best_dividers;
}

void Resistors_Calculator::deviation_indexer(QVector<QPair<int,float>> &best_dividers, int index, float deviation, int config)
{
    if(abs(deviation) < abs(best_dividers[3*config].second))
    {
        best_dividers[3*config+2] = best_dividers[3*config+1];
        best_dividers[3*config+1] = best_dividers[3*config];
        best_dividers[3*config] = qMakePair(index,deviation);
    }
    else if (abs(deviation) < abs(best_dividers[1].second))
    {
        best_dividers[3*config+2] = best_dividers[3*config+1];
        best_dividers[3*config+1] = qMakePair(index,deviation);
    }
    else if (abs(deviation) < abs(best_dividers[2].second))
    {
        best_dividers[3*config+2] = qMakePair(index,deviation);
    }
}

QList<Divider> Resistors_Calculator::get_all_dividers(QList<Resistor> resistors_list, int r_tot_min, int r_tot_max)
{
    QList<Divider> dividers_list;

    for(int i = 0; i < resistors_list.length(); i++ )
    {
        for(int j = 0; j < resistors_list.length(); j++ )
        {
            // configuration with 2 resistors
            int r_tot = resistors_list[i].get_value() + resistors_list[j].get_value();
            if(( r_tot <= r_tot_max || r_tot_max <= 0) && (r_tot >= r_tot_min || r_tot_min <= 0))
            {
                Divider temp_div(resistors_list[i], resistors_list[j]);
                dividers_list.append(temp_div);
            }

            // configuration with 3 resistors
            for(int k = 0; k < resistors_list.length(); k++ )
            {
                int r_tot = resistors_list[i].get_value() + resistors_list[j].get_value() + resistors_list[k].get_value();
                if(( r_tot <= r_tot_max || r_tot_max <= 0) && (r_tot >= r_tot_min || r_tot_min <= 0))
                {
                    // add divider with 3 resistors in 1st configuration
                    Divider temp_div(resistors_list[i], resistors_list[j], resistors_list[k], 1);
                    dividers_list.append(temp_div);
                    // add divider with 3 resistors in 2nd configuration
                    Divider temp_div2(resistors_list[i], resistors_list[j], resistors_list[k], 2);
                    dividers_list.append(temp_div2);
                }
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
                    Resistor temp_res(comp_name, line.split(';').takeAt(3), line.split(';').takeAt(4).at(0)-48);
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
