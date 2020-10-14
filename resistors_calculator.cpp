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
    QList<Resistor> resistors_list;
    float div_ratio;
    int r_tot_min, r_tot_max;
    get_ui_input(resistors_list, div_ratio, r_tot_min, r_tot_max);

    if(check_input(resistors_list.length(), div_ratio))
    {
        QList<Divider> dividers_list;
        if(get_all_dividers(dividers_list, resistors_list, r_tot_min, r_tot_max))
        {
            // vector of best dividers pair - index, deviation
            QVector<QPair<int,float>> best_dividers_indexes(9,qMakePair(0,1));
            get_best_dividers(best_dividers_indexes, dividers_list, div_ratio);

            QVector<Divider> best_dividers(9);
            for(int i = 0; i < 9; i++ )
            {
                best_dividers[i] = dividers_list[best_dividers_indexes[i].first];
            }
            update_resistors_tableWidget(best_dividers);
        }
    }
}

void Resistors_Calculator::get_ui_input(QList<Resistor> &resistors_list, float &div_ratio, int &r_tot_min, int &r_tot_max)
{
    try
    {
        QString bom_path = get_bom_location();
        resistors_list = read_csv(bom_path);
        div_ratio = ui->divider_lineEdit->text().toFloat();
        r_tot_min = ui->r_tot_min_lineEdit->text().toInt();
        r_tot_max = ui->r_tot_max_lineEdit->text().toInt();

        add_log("User input loaded, BOM file location = " + bom_path + ", found " + QString::number(resistors_list.length()) + " resistors");
    }
    catch (const std::exception& e)
    {
        add_log("User input loading error - " + QString(e.what()));
    }
}

bool Resistors_Calculator::check_input(int resistors_list_length, float div_ratio)
{
    if(div_ratio <= 0 && div_ratio >= 1)
    {
        add_log("Program stopped - div_ratio out of range (0;1)");
        return false;
    }
    else if (resistors_list_length < 1)
    {
        add_log("Program stopped - no resistors found in BOM file");
        return false;
    }

    return true;
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


QList<Resistor> Resistors_Calculator::read_csv(QString path)
{
    QList<Resistor> resistors_list;
    QFile file(path);

    try
    {
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
    catch (...)
    {
        add_log("Error loading BOM file");
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

void Resistors_Calculator::add_log(QString to_log)
{
    ui->events_textBrowser->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") + ": " + to_log);
}


/// returned vector contains all configurations in order 0, 1, 2
void Resistors_Calculator::get_best_dividers(QVector<QPair<int,float>> &best_dividers_indexes, QList<Divider> dividers_list, float div_ratio)
{
    try
    {
        for(int i = 0; i < dividers_list.length(); i++ )
        {
            float deviation = div_ratio - dividers_list[i].get_div_ratio();

            for(int j = 0; j < 3; j++ )
            {
                if(dividers_list[i].get_config() == j)
                {
                    deviation_indexer(best_dividers_indexes, i, deviation, j);
                }
            }
        }
        add_log("Best dividers found for " + QString::number(div_ratio));
    }
    catch (const std::exception& e)
    {
        add_log("Error finding best dividers - " + QString(e.what()));
    }
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

bool Resistors_Calculator::get_all_dividers(QList<Divider> &dividers_list, QList<Resistor> resistors_list, int r_tot_min, int r_tot_max)
{
    try
    {
        for(int i = 0; i < resistors_list.length(); i++ )
        {
            for(int j = 0; j < resistors_list.length(); j++ )
            {
                // configuration with 2 resistors
                if(check_r_tot(r_tot_min, r_tot_max, resistors_list[i].get_value(), resistors_list[j].get_value(), 0))
                    dividers_list.append(Divider(resistors_list[i], resistors_list[j]));

                // configuration with 3 resistors
                for(int k = 0; k < resistors_list.length(); k++ )
                {
                    if(check_r_tot(r_tot_min, r_tot_max, resistors_list[i].get_value(), resistors_list[j].get_value(), resistors_list[k].get_value()))
                    {
                        // add divider with 3 resistors in 1st configuration                     ;
                        dividers_list.append(Divider(resistors_list[i], resistors_list[j], resistors_list[k], 1));
                        // add divider with 3 resistors in 2nd configuration
                        dividers_list.append(Divider(resistors_list[i], resistors_list[j], resistors_list[k], 2));
                    }
                }
            }
        }
        add_log("Created " + QString::number(dividers_list.length()) + " dividers" );
        return true;
    }
    catch (const std::exception& e)
    {
        add_log("Error calculating dividers - " + QString(e.what()));
    }
    return false;
}

bool Resistors_Calculator::check_r_tot(int r_tot_min, int r_tot_max, int r1_value, int r2_value, int r3_value)
{
    int r_tot = r1_value + r2_value + r3_value;
    if(( r_tot <= r_tot_max || r_tot_max <= 0) && (r_tot >= r_tot_min || r_tot_min <= 0))
        return true;

    return false;
}
