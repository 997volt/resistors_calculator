#ifndef RESISTORS_CALCULATOR_H
#define RESISTORS_CALCULATOR_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QStringList>
#include <resistor.h>

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
    Resistor * resistors_search(QList<Resistor> resistors_list, float div_ratio);

private:
    Ui::Resistors_Calculator *ui;
};
#endif // RESISTORS_CALCULATOR_H
