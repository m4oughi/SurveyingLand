#ifndef STARTRECORDDATA_H
#define STARTRECORDDATA_H

#include <QDialog>
#include <QDebug>



namespace Ui {
class StartRecordData;
}

class StartRecordData : public QDialog
{
    Q_OBJECT

public:
    explicit StartRecordData(QWidget *parent = 0);
    ~StartRecordData();

    QString fileName() const;

    double XMin() const;
    double XMax() const;
    double YMin() const;
    double YMax() const;

    QString choice;
    QString userChoice() const;
    double XMin_d, XMax_d, YMin_d, YMax_d;




private slots:
    void on_gpsSaveAsPushButton_clicked();

    void on_setRangePushButton_clicked();

    void on_manualPlotXYRadioButton_clicked();

    void on_automaticPlotXYRadioButton_clicked();

    void on_cancelBtn_clicked();

    void on_reordDataBtn_clicked();

private:
    Ui::StartRecordData *ui;

    QString gpsFileName;
};

#endif // STARTRECORDDATA_H
