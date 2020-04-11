#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include <QTextCodec>
#include <QMessageBox>

namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();

private:
    int focusFlag;
    int clearFlag;
    QString functext, functext_tmp;
    Ui::MainForm *ui;

private slots:
    void keyEditClicked();
    void onKeyEditFocusLost();
    void clearPlainTextEdit();
    void openParamForm();
    int  checkKey();
    void crypt();
};

#endif // MAINFORM_H
