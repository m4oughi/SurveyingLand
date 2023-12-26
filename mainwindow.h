#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <connection.h>
#include <startrecorddata.h>

#include <QMainWindow>

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

#include <QMessageBox>
#include <QString>
#include <QtMath>
#include <QPoint>
#include <QDebug>

#include <QPlainTextEdit>
#include <QTextDocument>
#include <QTextBlock>

#include <QTimer>
#include <QDateTime>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QByteArray>
#include <QList>

#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

#include <QtMath>
#include <QDesktopServices>
#include <QUrl>

QT_CHARTS_USE_NAMESPACE


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    StartRecordData start;


private slots:
    void on_connectBtn_clicked();
    void on_disconnectBtn_clicked();
    void on_startRecordBtn_clicked();
    void on_stopRecordBtn_clicked();
    void on_plotXYBtn_clicked();
    void on_exitBtn_clicked();

    void on_manualRadioBtn_clicked();
    void on_AutoRadioBtn_clicked();

    void on_fixedPointManualSetBtn_clicked();

    void on_fixedPointStartGettingDataBtn_clicked();
    void on_fixedPointGetAverageSetBtn_clicked();

    void on_savePlotBtn_clicked();

    void gpsReadyRead();
    void writeData();
    void distance();
    void speed();

    void calculateSai();

    void on_backToMainWindowBtn_clicked();

    void plotXY();


    void on_backToMainWindowBtn_2_clicked();

    void on_moreInfoBtn_clicked();

private:
    Ui::MainWindow *ui;

    QSerialPort *gpsSerialPort;

    QString dir;
    QFile *gpsStartRecord;
    QTextStream *gpsStartRecordStream;

    QByteArray gpsReadData;
    QList<QByteArray> gpsOutput;
    QString gpsTime, gpsLatitude, gpsLatitude1, gpsLatitude2, gpsLatitudeOrientation, gpsLongitude, gpsLongitude1, gpsLongitude2, gpsLongitudeOrientation;
    double gpsTime_double, gpsLatitude_double, gpsLatitude1_double, gpsLatitude2_double, gpsLongitude_double, gpsLongitude1_double, gpsLongitude2_double;

    QList<double> ti, la, lo, fixTi, fixLa, fixLo, xList, yList;

    double lat1, lon1, lat1_radian, lon1_radian, lat2_radian, lon2_radian, deltaPhi, deltaLanda, x, y, sai, onTime, duration_d;
    double sum_la, sum_lo, avg_la, avg_lo;
    QString x_str, y_str, speedText, distance_;

    QLineSeries* series;
    QChart* chart = new QChart();
    QChartView* chartView = new QChartView(chart);

    QFile *imageFile;

    QString minX, maxX, minY, maxY, lat1_str, lon1_str;

    QString latfix_str, longfix_str;
    double latfix, lonfix;



signals:
    void distanceSignal();
    void speedSignal();
    void calculateSaiSignal();
    void writeDataSignal();
    void plotXYSignal();
};

#endif // MAINWINDOW_H
