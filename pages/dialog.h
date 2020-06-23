#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <API/api.h>

class dataMaintainance;


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog( dataMaintainance* DM, API* api, QWidget *parent = nullptr);
    ~Dialog();
    void init(QString pattern);

private slots:

    void on_Pattern_lineEdit_textEdited(QString s);

    void on_DFtool_button_clicked();
    void on_DBtool_button_clicked();
    void on_LFtool_button_clicked();
    void on_LBtool_button_clicked();

    void on_DF_lineEdit_textEdited(QString s);
    void on_DB_lineEdit_textEdited(QString s);
    void on_LF_lineEdit_textEdited(QString s);
    void on_LB_lineEdit_textEdited(QString s);

    void on_Pimage_toolButton_clicked();
    void on_Mimage_toolButton_clicked();
    void on_Pimage_lineEdit_textEdited(QString s);
    void on_Mimage_lineEdit_textEdited(QString s);

    void on_ConfirmButton_clicked();
    void on_CancelButton_clicked();


    void comboBox_changed();


signals:
    void confirmEditing(QString pattern, QString DFAR4Address, QString DBAR4Address,
                        QString LFAR4Address, QString LBAR4Address, QString PimageAddress,
                        QString MimageAddress, bool basBack, bool hasFront);
    void PatternNameChanged(QString pattern);


private:
    dataMaintainance* DM;

    API* api;
    Ui::Dialog *ui;
    int index;
    bool isEdit;
    bool PatternChanged;
    bool hasBack;
    bool hasFront;

    QString originalPatternName;

    Pattern pattern;
    QString patternName;

    QString DFAR4Address;
    QString DBAR4Address;
    QString LFAR4Address;
    QString LBAR4Address;

    QString PimageAddress;
    QString MimageAddress;


    void update();
    int fileReadyCheck();
};

#endif // DIALOG_H
