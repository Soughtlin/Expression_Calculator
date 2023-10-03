#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include<QObject>
#include<QPushButton>
#include<QButtonGroup>
#include<QtDebug>
#include<QStack>
#include <QProcess>
QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();
    void init();
    void buttonSlot(QAbstractButton* btn);
    double compute(QString exp);
private:
    Ui::Calculator *ui;
    QButtonGroup* buttonGroup;
};

#endif // CALCULATOR_H
