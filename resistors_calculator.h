#ifndef RESISTORS_CALCULATOR_H
#define RESISTORS_CALCULATOR_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QStringList>
#include <resistor.h>
#include <divider.h>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class Resistors_Calculator; }
QT_END_NAMESPACE

class Resistors_Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Resistors_Calculator(QWidget *parent = nullptr);
    ~Resistors_Calculator();

private slots:
    void on_calculate_pushButton_clicked();
    void get_ui_input(QList<Resistor> &resistors_list, float &div_ratio, int &r_tot_min, int &r_tot_max);
    bool check_input(int resistors_list_length, float div_ratio);
    void update_resistors_tableWidget(QVector<Divider> new_dividers);

    QString get_bom_location();
    QList<Resistor> read_csv(QString path);
    void add_log(QString to_log);

    bool get_all_dividers(QList<Divider> &dividers_list, QList<Resistor> resistors_list, int r_tot_min, int r_tot_max);
    void get_best_dividers(QVector<QPair<int,float>> &best_dividers_indexes, QList<Divider> dividers_list, float div_ratio);
    void deviation_indexer(QVector<QPair<int,float>> &best_dividers, int index, float deviation, int config);
    bool check_r_tot(int r_tot_min, int r_tot_max, int r1_value, int r2_value, int r3_value);

private:
    Ui::Resistors_Calculator *ui;
};
#endif // RESISTORS_CALCULATOR_H
