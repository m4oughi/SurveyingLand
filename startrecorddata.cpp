#include "startrecorddata.h"
#include "ui_startrecorddata.h"

#include <QFileDialog>

StartRecordData::StartRecordData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartRecordData)
{
    ui->setupUi(this);
}

StartRecordData::~StartRecordData()
{
    delete ui;
}

void StartRecordData::on_gpsSaveAsPushButton_clicked()
{
    gpsFileName = QFileDialog::getSaveFileName(this, tr("Save File"), "001.txt", tr("*.txt"));
    ui-> gpsSaveAsLineEdit-> setText(gpsFileName);
}


QString StartRecordData::fileName() const {
    return ui-> gpsSaveAsLineEdit-> text();
}





void StartRecordData::on_setRangePushButton_clicked()
{
    bool ok = false;

    QString X_Min = ui-> axisXFromLineEdit-> text();
    QString X_Max = ui-> axisXToLineEdit-> text();
    QString Y_Min = ui-> axisYFromLineEdit-> text();
    QString Y_Max = ui-> axisYToLineEdit-> text();

    XMin_d = X_Min.toDouble(&ok);
    XMax_d = X_Max.toDouble(&ok);
    YMin_d = Y_Min.toDouble(&ok);
    YMax_d = Y_Max.toDouble(&ok);

    ui-> minXLabel-> setText(X_Min);
    ui-> maxXLabel-> setText(X_Max);
    ui-> minYLabel-> setText(Y_Min);
    ui-> maxYLabel-> setText(Y_Max);
}

void StartRecordData::on_manualPlotXYRadioButton_clicked()
{
    ui-> manualXYAxisSettingGroupBox-> setEnabled(true);
    choice = "manual";
}

void StartRecordData::on_automaticPlotXYRadioButton_clicked()
{
    ui-> manualXYAxisSettingGroupBox-> setEnabled(false);
    choice = "auto";
}




double StartRecordData::XMin() const {
    return XMin_d;
}

double StartRecordData::XMax() const {
    return XMax_d;
}

double StartRecordData::YMin() const {
    return YMin_d;
}

double StartRecordData::YMax() const {
    return YMax_d;
}

QString StartRecordData::userChoice() const {
    return choice;
}

void StartRecordData::on_cancelBtn_clicked()
{
    this-> close();
}

void StartRecordData::on_reordDataBtn_clicked()
{
    this-> close();
}
