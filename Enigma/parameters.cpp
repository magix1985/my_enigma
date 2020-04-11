#include "parameters.h"
#include "ui_parameters.h"
#include "math.h"

Parameters::Parameters(QWidget *parent, QString func) :
    QDialog(parent),
    ui(new Ui::Parameters)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnWidth(0, 156);
    ui->tableWidget->setColumnWidth(1, 156);
    ui->tableWidget->setColumnWidth(2, 156);
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(OK()));
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addRow()));
    connect(ui->delButton, SIGNAL(clicked()), this, SLOT(deleteRow()));
    function = func;
    restoreParameters();
    countAndDraw();
}

Parameters::~Parameters()
{
    delete ui;
}

void Parameters::restoreParameters()
{
    int numrow = 0, k;
    QString func_copy = "";
    func_copy.append(function);

    while (func_copy.length() > 0)
    {
        ui->tableWidget->setRowCount(numrow + 1);

        for (k = 0; k < func_copy.length(); k++)
        {
            if (func_copy.at(k) == '+')
            {
                break;
            }
        }

        QTableWidgetItem * item1 = new QTableWidgetItem;
        item1->setText("sin");
        item1->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(numrow, 0, item1);

        QTableWidgetItem * item2 = new QTableWidgetItem;
        item2->setText("+");
        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(numrow, 2, item2);

        QTableWidgetItem * item3 = new QTableWidgetItem;
        item3->setText(func_copy.left(k));
        item3->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(numrow, 1, item3);

        func_copy.remove(0, k + 1);
        numrow++;
    }
}

void Parameters::OK()
{
    QTableWidgetItem *pToItem;

    function.clear();

    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        pToItem = ui->tableWidget->item(i, 1);
        function.append(pToItem->text());

        if (i < (ui->tableWidget->rowCount() - 1))
        {
            pToItem = ui->tableWidget->item((i + 1), 2);

            if ((strcmp(pToItem->text().toAscii(), "+")) == 0)
            {
                function.append('+');
            }
            if ((strcmp(pToItem->text().toAscii(), "*")) == 0)
            {
                function.append('*');
            }
        }
    }

    close();
}

void Parameters::addRow()
{
    int i = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(i + 1);

    QTableWidgetItem * item1 = new QTableWidgetItem;
    item1->setText("sin");
    item1->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(i, 0, item1);

    QTableWidgetItem * item2 = new QTableWidgetItem;

    if (ui->actionList->currentIndex() == 0)
    {
        item2->setText("+");
    }
    else
    {
        item2->setText("*");
    }

    item2->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(i, 2, item2);

    QTableWidgetItem * item3 = new QTableWidgetItem;
    item3->setText(QString::number(ui->spinBox->value()));
    item3->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(i, 1, item3);
    countAndDraw();
    repaint();
}

void Parameters::deleteRow()
{
    if (ui->tableWidget->rowCount() > 1)
    {
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());

        if (ui->tableWidget->item(0, 2)->text().length() > 0)
        {
            ui->tableWidget->item(0, 2)->setText("");
        }
    }
    else
    {
        QMessageBox::critical( this,
                               QTextCodec::codecForName("UTF-8")->toUnicode("Ошибка"),
                               QTextCodec::codecForName("UTF-8")->toUnicode("Должна быть хотя бы одна гармоника") );

    }

    countAndDraw();
    repaint();
}

void Parameters::countAndDraw()
{
    uint i, j;
    double mul_coeff;
    uint diapasone;
    int base_x = 10;
    double v_exp_coeff = 50;
    double h_exp_coeff = 38.1;
    int y_incr;

    uint cnt = ui->tableWidget->rowCount();
    diapasone = (this->width() - base_x - 10);
    y_points.resize(480, 0);
    y_points.assign(480, 0);

    for (i = 0; i < cnt; i++)
    {
        mul_coeff = ui->tableWidget->item(i, 1)->text().toDouble();

        for (j = 0; j < diapasone; j++)
        {
            y_incr = (v_exp_coeff*sin((j*mul_coeff)/h_exp_coeff));
            y_points[j] += y_incr;
        }
    }
}

void Parameters::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);

    int i;
    QPainter painter(this); //Создаём объект отрисовщика
    int base_x = 10;
    int base_y = (this->height() - 95);
    int delta = 10;
//  Устанавливаем кисть абриса
    painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));
//  Рисуем линию
    QPoint p1, p2;
    painter.setPen(QPen(Qt::lightGray, 1, Qt::SolidLine, Qt::FlatCap));

    for (i = 1; i < 6; i++)
    {
        p1.setX(base_x);
        p1.setY(base_y - i*delta);
        p2.setX(this->width() - 9);
        p2.setY(base_y - i*delta);
        painter.drawLine(p1, p2);

        p1.setX(base_x);
        p1.setY(base_y + i*delta);
        p2.setX(this->width() - 9);
        p2.setY(base_y + i*delta);
        painter.drawLine(p1, p2);
    }

    for (i = 0; i < 49; i++)
    {
        p1.setX(base_x + i*delta);
        p1.setY(base_y - 50);
        p2.setX(base_x + i*delta);
        p2.setY(base_y + 50);
        painter.drawLine(p1, p2);
    }

    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    p1.setX(base_x);
    p1.setY(base_y);
    p2.setX(this->width() - 10);
    p2.setY(base_y);
    painter.drawLine(p1, p2);

    painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap));

    for (i = 0; i < 480; i++)
    {
        painter.drawPoint(10 + i, base_y + y_points[i]/ui->tableWidget->rowCount());
        painter.drawPoint(10 + i, base_y + y_points[i]/ui->tableWidget->rowCount() + 1);
    }
}
