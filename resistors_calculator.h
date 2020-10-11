#ifndef RESISTORS_CALCULATOR_H
#define RESISTORS_CALCULATOR_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QStringList>
#include <resistor.h>
#include <divider.h>

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
    QString get_bom_location();
    QList<Resistor> read_csv(QString path);
    void on_calculate_pushButton_clicked();
    float get_div_ratio();
    QList<Divider> get_all_dividers(QList<Resistor> resistors_list, int r_tot_min, int r_tot_max);
    QVector<QPair<int,float>> get_best_dividers(QList<Divider> dividers_list, float div_ratio);
    void update_resistors_tableWidget(QVector<Divider> new_dividers);

private:
    Ui::Resistors_Calculator *ui;
};
#endif // RESISTORS_CALCULATOR_H
