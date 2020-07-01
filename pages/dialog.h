#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <vector>
#include "models/pattern.h"

class dataMaintainance;


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog( dataMaintainance* DM, Pattern* pattern, QWidget *parent = nullptr);
    ~Dialog();
    void init(Pattern* pattern);

private slots:

    void on_Pattern_lineEdit_textEdited(QString s);
    void on_Note_lineEdit_textChanged(QString s);

    void on_AR4_lineEdit_textChanged(QString s);
    void on_AR4_tool_button_clicked();

    void on_Pimage_toolButton_clicked();
    void on_Pimage_lineEdit_textChanged(QString s);

    void on_ConfirmButton_clicked();
    void on_CancelButton_clicked();

    void on_add_button_clicked();
    void on_delete_button_clicked();

    void on_tableView_clicked();

    void type_comboBox_changed(const QString & s);

signals:
    void confirmEditing(Pattern* pattern);
    void PatternNameChanged(QString pattern);


private:
    dataMaintainance* DM;

    Ui::Dialog *ui;

    int index;
    bool isEdit;

    QString originalPatternName;

    Pattern* pattern;
    AR4FileModel* fileModel;
    QString patternName;

    QString PimageAddress;
    bool hasPimages;
    QString AR4Address;

    vector<QString> AR4FileAddresses;

    void update();
    int fileReadyCheck();

    friend class AR4FileModel;
};

#endif // DIALOG_H
