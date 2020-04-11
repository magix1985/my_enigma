#include "mainform.h"
#include "ui_mainform.h"
#include "parameters.h"
#include "math.h"

MainForm::MainForm(QWidget * parent) : QWidget(parent), ui(new Ui::MainForm)
{
    ui->setupUi(this);
    ui->keyEdit->setFont(QFont("Verdana", 8, QFont::Normal));
    ui->keyEdit->setStyleSheet("color: gray;");
    ui->plainTextEdit->setStyleSheet("color: gray;");
    ui->plainTextEdit->setText(QTextCodec::codecForName("UTF-8")->toUnicode("Введите текст..."));

    connect(ui->keyEdit, SIGNAL(lostFocus()), this, SLOT(onKeyEditFocusLost()));
    connect(ui->keyEdit, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(keyEditClicked()));
    connect(ui->plainTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(clearPlainTextEdit()));
    connect(ui->paramButton, SIGNAL(clicked()), this, SLOT(openParamForm()));

    functext = "1";
    focusFlag = 0;
    clearFlag = 1;
    ui->keyEdit->setCursorPosition(12);
}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::keyEditClicked()
{
    if (focusFlag)
    {
        if (ui->keyEdit->text().contains(QTextCodec::codecForName("UTF-8")->toUnicode("Введите ключ шифрования")))
        {
            ui->keyEdit->clear();
            ui->keyEdit->setStyleSheet("color: black;");
        }
    }

    focusFlag = 1;
}

void MainForm::onKeyEditFocusLost()
{
    if (ui->keyEdit->text().length() == 0)
    {
        focusFlag = 0;
        ui->keyEdit->setStyleSheet("color: gray;");
        ui->keyEdit->setText(QTextCodec::codecForName("UTF-8")->toUnicode("Введите ключ шифрования"));
        focusFlag = 0;
        ui->keyEdit->setCursorPosition(12);
    }
}

void MainForm::clearPlainTextEdit()
{
    if (clearFlag)
    {
        clearFlag = 0;
        ui->plainTextEdit->clear();
    }

    ui->plainTextEdit->setStyleSheet("color: darkblue;");
}

int MainForm::checkKey()
{
    int i, j;
    QString keystr = ui->keyEdit->text();

    for (i = 0; i < keystr.length(); i++)
    {
        for (j = 0; j < 10; j++)
        {
            if (keystr[i].toAscii() == (j + 48))
            {
                break;
            }
        }

        if (j == 10)
        {
            return 0;
        }
    }

    return 1;
}

void MainForm::openParamForm()
{
    Parameters parForm(this, functext);
    parForm.setModal(true);
    parForm.exec();
    functext.clear();
    functext.append(parForm.function);
}

void MainForm::crypt()
{
    QString plain, alphcopy1;
    QString crypt = "";
    QString symb;
    char alph1[] = ".<>,[]{}АБВГДЕЁЖЗИЙ®©КЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя0123456789-!?:;ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz@#$%^&*()_=+~/\\|\"";
    int i, j, k, flsum, result;
    int shift = 0;
    int alphlen;
    double coeff, key, wavefunc;
    int offset;

    plain = ui->plainTextEdit->toPlainText();
    plain.replace(QString(' '), QString(QTextCodec::codecForName("UTF-8")->toUnicode("®")));
    plain.replace(QString('\n'), QString(QTextCodec::codecForName("UTF-8")->toUnicode("©")));
    alphcopy1 = QTextCodec::codecForName("UTF-8")->toUnicode(alph1);
    alphlen = alphcopy1.length();
    key = ui->keyEdit->text().toDouble();

    result = checkKey();

    if (result)
    {
//      Шифруемся или дешифруемся
        if (plain.length() > 0)
        {
//          Гаммирование
            for (i = 0; i < plain.length(); i++)
            {
                for (j = 0; j < alphlen; j++)
                {
                    if (plain.mid(i, 1) == alphcopy1.mid(j, 1))
                    {
                        wavefunc = 1;
                        key += plain.length();
                        functext_tmp = functext;

//                      Волновая функция
                        while (functext_tmp.length() > 0)
                        {
                            flsum = 0;

                            for (k = 0; k < functext_tmp.length(); k++)
                            {
                                if (functext_tmp.at(k) == '+')
                                {
                                    flsum = 1;
                                    break;
                                }
                                if (functext_tmp.at(k) == '*')
                                {
                                    break;
                                }
                            }

                            coeff = functext_tmp.left(k).toDouble();

                            if (flsum)
                                wavefunc += sin(coeff * key);
                            else
                                wavefunc *= sin(coeff * key);

                            functext_tmp.remove(0, k + 1);
                        }

                        shift = alphlen * wavefunc;

//                      Прямой или обратный сдвиг (шифрование/дешифрование)
                        if (ui->encryptButton == (QPushButton *)sender())
                        {
                            offset = (abs(shift) % alphlen);
                            crypt.append(alphcopy1.mid((j + offset) % alphlen, 1));
                        }
                        if (ui->decryptButton == (QPushButton *)sender())
                        {
                            offset = alphlen - (abs(shift) % alphlen);
                            symb = alphcopy1.mid((j + offset) % alphlen, 1);

                            if (symb == QString(QTextCodec::codecForName("UTF-8")->toUnicode("®")))
                                crypt.append(" ");
                            else if (symb == QString(QTextCodec::codecForName("UTF-8")->toUnicode("©")))
                                crypt.append("\n");
                            else
                                crypt.append(symb);
                        }

                        break;
                    }
                }
            }
        }

        ui->plainTextEdit->setText(crypt);
    }
    else
    {
        QMessageBox::critical( this,
                               QTextCodec::codecForName("UTF-8")->toUnicode("Ошибка"),
                               QTextCodec::codecForName("UTF-8")->toUnicode("Введите числовой ключ") );
    }
}
