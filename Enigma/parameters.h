#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QDialog>
#include <QTextCodec>
#include <QMessageBox>
#include <QPainter>

namespace Ui {
class Parameters;
}

class Parameters : public QDialog
{
    Q_OBJECT

public:
    explicit Parameters(QWidget *parent = 0, QString func = "");
    ~Parameters();
    void paintEvent(QPaintEvent *event);
    QString function;
    std::vector<int> y_points;

private:
    Ui::Parameters *ui;

private slots:
    void OK();
    void addRow();
    void deleteRow();
    void countAndDraw();
    void restoreParameters();
};

#endif // PARAMETERS_H
