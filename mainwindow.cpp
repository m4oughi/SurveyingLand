#include "mainwindow.h"
#include "ui_mainwindow.h"


/*!
 * \brief MainWindow::MainWindow
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui-> connectBtn -> setEnabled(true);
    ui-> disconnectBtn -> setEnabled(false);
    ui-> startRecordBtn -> setEnabled(false);
    ui-> stopRecordBtn -> setEnabled(false);
    ui-> plotXYBtn -> setEnabled(false);

    ui-> exitBtn -> setEnabled(true);

    ui-> fixedPointConfigurationGropuBox-> setEnabled(false);
    ui-> fixedPointManualGroupBox-> setEnabled(false);
    ui-> fixedPointAutoGroupBox-> setEnabled(false);
    ui-> fixedPointFinalGroupBox->setEnabled(false);
    ui-> currentGpsDataGroupBox-> setEnabled(false);
}



/*!
 * \brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}



/*!
 * \brief MainWindow::on_connectBtn_clicked
 */
void MainWindow::on_connectBtn_clicked()
{
    ui-> connectBtn -> setEnabled(false);
    ui-> disconnectBtn -> setEnabled(true);
    ui-> startRecordBtn -> setEnabled(true);
    ui-> stopRecordBtn -> setEnabled(false);
    ui-> plotXYBtn -> setEnabled(true);

    ui-> fixedPointConfigurationGropuBox-> setEnabled(true);
    ui-> fixedPointManualGroupBox-> setEnabled(false);
    ui-> fixedPointAutoGroupBox-> setEnabled(false);
    ui-> fixedPointFinalGroupBox->setEnabled(true);
    ui-> currentGpsDataGroupBox-> setEnabled(true);


    Connection conn;
    conn.setModal(true);
    conn.exec();

    gpsSerialPort = new QSerialPort(this);

    gpsSerialPort-> setPortName(conn.name()); // Define port name
    gpsSerialPort-> setBaudRate(conn.baud()); // Define baudrate

    // Check the gps connection [3]
    if(gpsSerialPort-> open(QIODevice::ReadOnly)) {
        ui-> stausUpdatableLabel-> setText("Connected!"); // Connected status
        ui-> stausUpdatableLabel-> setStyleSheet("color: rgb(0, 255, 0)"); // Connected status color
    }
    else {
        ui-> stausUpdatableLabel-> setText("Disconnected!"); // Disconnected status
        ui-> stausUpdatableLabel-> setStyleSheet("color: rgb(255, 0, 127)"); // Disconnected status color

        QMessageBox::critical(this, tr("Error"), gpsSerialPort-> errorString()); // Warning message in order to show failure in connecting to the gps
    }

    connect(gpsSerialPort, SIGNAL(readyRead()), this, SLOT(gpsReadyRead()));
    connect(this, SIGNAL(calculateSaiSignal()), this, SLOT(calculateSai()));
    connect(this, SIGNAL(speedSignal()), this, SLOT(speed()));
    connect(this, SIGNAL(distanceSignal()), this, SLOT(distance()));
}



/*!
 * \brief MainWindow::on_disconnectBtn_clicked
 */
void MainWindow::on_disconnectBtn_clicked()
{
    ui-> connectBtn -> setEnabled(true);
    ui-> disconnectBtn -> setEnabled(false);
    ui-> startRecordBtn -> setEnabled(false);
    ui-> stopRecordBtn -> setEnabled(false);
    ui-> plotXYBtn -> setEnabled(false);

    ui-> fixedPointConfigurationGropuBox-> setEnabled(false);
    ui-> fixedPointManualGroupBox-> setEnabled(false);
    ui-> fixedPointAutoGroupBox-> setEnabled(false);
    ui-> fixedPointFinalGroupBox->setEnabled(true);
    ui-> currentGpsDataGroupBox-> setEnabled(true);

    ui-> stausUpdatableLabel-> setText("Disconnected!"); // Disconnected status
    ui-> stausUpdatableLabel-> setStyleSheet("color: rgb(255, 0, 127)"); // Disconnected status color

    gpsSerialPort->close();
}



/*!
 * \brief MainWindow::on_startRecordBtn_clicked
 */
void MainWindow::on_startRecordBtn_clicked()
{
    ui-> connectBtn -> setEnabled(false);
    ui-> disconnectBtn -> setEnabled(false);
    ui-> startRecordBtn -> setEnabled(false);
    ui-> stopRecordBtn -> setEnabled(true);
    ui-> plotXYBtn -> setEnabled(true);



    start.setModal(true);
    start.exec();



    dir = start.fileName();

    gpsStartRecord = new QFile(dir, this);
    gpsStartRecordStream = new QTextStream(gpsStartRecord);

    if(!gpsStartRecord->open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append)) {
        dir.append(" Not Opened");
        QMessageBox::warning(this, "Warning", dir);
        return;
    }
    else {
        dir.append(" Opened");
        qDebug() << dir;
    }

    gpsStartRecordStream->operator <<("GpsTime; X; Y; Heading; Latitude; Longitude; Speed; Distance") << "\n";
    connect(this, SIGNAL(writeDataSignal()), this, SLOT(writeData()));
    connect(this, SIGNAL(plotXYSignal()), this, SLOT(plotXY()));

    chart-> removeAllSeries();

    chart->createDefaultAxes();

    series = new QLineSeries();
}



/*!
 * \brief MainWindow::on_stopRecordBtn_clicked
 */
void MainWindow::on_stopRecordBtn_clicked()
{
    ui-> connectBtn -> setEnabled(false);
    ui-> disconnectBtn -> setEnabled(true);
    ui-> startRecordBtn -> setEnabled(true);
    ui-> stopRecordBtn -> setEnabled(false);
    ui-> plotXYBtn -> setEnabled(true);

    gpsStartRecord->close();
    QDesktopServices::openUrl(QUrl::fromLocalFile(dir));
    disconnect(this, SIGNAL(plotXYSignal()), this, SLOT(plotXY()));
}


/*!
 * \brief MainWindow::on_exitBtn_clicked
 */
void MainWindow::on_exitBtn_clicked()
{
    this->close();
}



/*!
 * \brief MainWindow::on_manualRadioBtn_clicked
 */
void MainWindow::on_manualRadioBtn_clicked()
{
    ui-> manualHeader-> setStyleSheet("background-color: rgb(24, 90, 219); color: rgb(245, 239, 231); font: 18pt 'K792-Geometrical-Demi';");
    ui-> automaticHeader-> setStyleSheet("background-color: rgb(0, 0, 0); color: rgb(255, 255, 255); font: 18pt 'K792-Geometrical-Demi';");

    ui-> fixedPointManualGroupBox-> setEnabled(true);
    ui-> fixedPointAutoGroupBox-> setEnabled(false);
}



/*!
 * \brief MainWindow::on_AutoRadioBtn_clicked
 */
void MainWindow::on_AutoRadioBtn_clicked()
{
    ui-> manualHeader-> setStyleSheet("background-color: rgb(0, 0, 0); color: rgb(255, 255, 255); font: 18pt 'K792-Geometrical-Demi';");

    ui-> automaticHeader-> setStyleSheet("background-color: rgb(24, 90, 219); color: rgb(245, 239, 231); font: 18pt 'K792-Geometrical-Demi';");

    ui-> fixedPointManualGroupBox-> setEnabled(false);
    ui-> fixedPointAutoGroupBox-> setEnabled(true);
}



/*!
 * \brief MainWindow::on_fixedPointManualSetBtn_clicked
 */
void MainWindow::on_fixedPointManualSetBtn_clicked()
{
    bool ok = false;
    latfix_str = ui-> fixedPointLatitudeLineEdit-> text();
    longfix_str = ui-> fixedPointLongitudeLineEdit-> text();

    latfix = latfix_str.toDouble(&ok);
    lonfix = longfix_str.toDouble(&ok);

    ui-> fixedPointLatitudeLabel-> setText(latfix_str);
    ui-> fixedPointLongitudeLabel-> setText(longfix_str);

    lat1_str = ui-> fixedPointLatitudeLabel-> text();
    lon1_str = ui-> fixedPointLongitudeLabel-> text();

    lat1 = lat1_str.toDouble(&ok);
    lon1 = lon1_str.toDouble(&ok);

    ui-> label_7->setText("N");
    ui-> label_8->setText("E");
}



/*!
 * \brief MainWindow::on_fixedPointStartGettingDataBtn_clicked
 */
void MainWindow::on_fixedPointStartGettingDataBtn_clicked()
{
    bool ok = false;

    onTime = gpsTime_double;

    QString duration = ui-> TimeLineEdit-> text();
    duration_d = duration.toDouble(&ok);
}


/*!
 * \brief MainWindow::on_fixedPointGetAverageSetBtn_clicked
 */
void MainWindow::on_fixedPointGetAverageSetBtn_clicked()
{
    sum_la = 0;
    sum_lo = 0;
    for (int i=0; i<fixLa.count(); i++) {
        sum_la = sum_la + fixLa.at(i);
    }

    for (int i=0; i<fixLo.count(); i++) {
        sum_lo = sum_lo + fixLo.at(i);
    }

    avg_la = sum_la/(fixLa.count());
    ui-> fixedPointLatitudeLabel-> setText(QString::number(avg_la, 'g', 17));

    avg_lo = sum_lo/(fixLo.count());
    ui-> fixedPointLongitudeLabel-> setText(QString::number(avg_lo, 'g', 17));

    lat1_str = ui-> fixedPointLatitudeLabel-> text();
    lon1_str = ui-> fixedPointLongitudeLabel-> text();


    bool ok = false;
    lat1 = lat1_str.toDouble(&ok);
    lon1 = lon1_str.toDouble(&ok);
}



/*!
 * \brief MainWindow::gpsReadyRead
 */
void MainWindow::gpsReadyRead() {

    while(gpsSerialPort->canReadLine()) {
        gpsReadData = gpsSerialPort->readLine();
        qDebug() << gpsReadData;

        if(gpsReadData.startsWith("$GPGGA")) {

            gpsOutput = gpsReadData.split(',');



            bool ok = false;

            gpsTime = gpsOutput.at(1);
            gpsTime_double = gpsTime.toDouble(&ok);
            ti.append(gpsTime_double);

            gpsLatitude1 = gpsOutput.at(2).mid(0, 2);
            gpsLatitude2 = gpsOutput.at(2).mid(2, 12);

            gpsLatitude1_double = gpsLatitude1.toDouble(&ok);
            gpsLatitude2_double = gpsLatitude2.toDouble(&ok);
            gpsLatitude_double = gpsLatitude1_double + (gpsLatitude2_double/60);
            la.append(gpsLatitude_double);

            gpsLatitude = QString::number(gpsLatitude_double, 'g', 17);

            gpsLatitudeOrientation = gpsOutput.at(3);

            gpsLongitude1 = gpsOutput.at(4).mid(0, 3);
            gpsLongitude2 = gpsOutput.at(4).mid(3, 13);

            gpsLongitude1_double = gpsLongitude1.toDouble(&ok);
            gpsLongitude2_double = gpsLongitude2.toDouble(&ok);
            gpsLongitude_double = gpsLongitude1_double + (gpsLongitude2_double/60);
            lo.append(gpsLongitude_double);
            gpsLongitude = QString::number(gpsLongitude_double, 'g', 17);

            gpsLongitudeOrientation = gpsOutput.at(5);

            ui-> gpsTimeUpdatableLabel-> setText(gpsTime);
            ui-> timeLabel-> setText(gpsTime);
            ui-> latitudeUpdatableLabel-> setText(gpsLatitude);
            ui-> latLabel-> setText(gpsLatitude);
            ui-> latitudeOrientationUpdatableLabel-> setText(gpsLatitudeOrientation);
            ui-> latDir-> setText(gpsLatitudeOrientation);
            ui-> longitudeUpdatableLabel-> setText(gpsLongitude);
            ui-> longLabel-> setText(gpsLongitude);
            ui-> longitudeOrientationUpdatableLabel-> setText(gpsLongitudeOrientation);
            ui-> longDir-> setText(gpsLongitudeOrientation);

            QString format = gpsOutput.at(0);
            ui-> formatLabel-> setText(format);
            QString quality = gpsOutput.at(6);
            ui-> qualityLabel-> setText(quality);
            QString sats = gpsOutput.at(7);
            ui-> satsLabel-> setText(sats);
            QString hDiluationOfPrecision = gpsOutput.at(8);
            ui-> horizentalDilutionOfPrecisionLAbel-> setText(hDiluationOfPrecision);
            QString altitude = gpsOutput.at(9);
            ui-> altitudeLabel-> setText(altitude);
            QString altitudeUnit = gpsOutput.at(10);
            ui-> altUnit-> setText(altitudeUnit);
            QString undulation = gpsOutput.at(11);
            ui-> undulationLabel-> setText(undulation);
            QString undulationUnit = gpsOutput.at(12);
            ui-> undulationUnit-> setText(undulationUnit);
            QString ageOfCorrectionData = gpsOutput.at(13);
            ui-> ageOfCorrectionFactorLabel-> setText(ageOfCorrectionData);
            QString differentialBaseStationID = gpsOutput.at(14);
            ui-> differentialBaseStationLabel-> setText(differentialBaseStationID);



            emit speedSignal();
            emit distanceSignal();
            emit writeDataSignal();
            emit calculateSaiSignal();
        }

        if (onTime<=gpsTime_double && onTime+duration_d+0.2>=gpsTime_double) {
            ui-> fixedPointStartGettingDataBtn-> setEnabled(false);
            ui-> fixedPointStartGettingDataBtn-> setStyleSheet("background-color: rgb(255, 0, 0); font: 15pt 'K792-Geometrical-Demi';");
            ui-> fixedPointGetAverageSetBtn-> setEnabled(false);
            ui-> fixedPointGetAverageSetBtn-> setStyleSheet("background-color: rgb(255, 0, 0); font: 15pt 'K792-Geometrical-Demi';");

            fixLa.append(gpsLatitude_double);
            fixLo.append(gpsLongitude_double);
        }
        else {
            ui-> fixedPointStartGettingDataBtn-> setEnabled(true);
            ui-> fixedPointStartGettingDataBtn-> setStyleSheet("background-color: rgb(33, 32, 156); font: 15pt 'K792-Geometrical-Demi';");
            ui-> fixedPointGetAverageSetBtn-> setEnabled(true);
            ui-> fixedPointGetAverageSetBtn-> setStyleSheet("background-color: rgb(33, 32, 156); font: 15pt 'K792-Geometrical-Demi';");
        }
    }
}



/*!
 * \brief MainWindow::speed
 * \param radius, dis, t, sp, speedText
 * \return "None"
 *
 * \quotation
 *
 * \endquotation
 */
void MainWindow::speed()
{
    int radius = 6371; //km

    if (ti.count()-2>=0 && la.count()-2>=0 && lo.count()-2>=0){

        double dis = (qAcos(qSin(qDegreesToRadians(la.at(la.count()-1)))*qSin(qDegreesToRadians(la.at(la.count()-2))) + qCos(qDegreesToRadians(la.at(la.count()-1)))*qCos(qDegreesToRadians(la.at(la.count()-2)))*qCos(qDegreesToRadians(lo.at(lo.count()-1))-qDegreesToRadians(lo.at(lo.count()-2))))*radius)*1000;
        double t = ti.at(ti.count()-1) - ti.at(ti.count()-2);
        double sp = (dis/t); //m/s

        speedText = QString::number(sp, 'g', 17);

        ui-> speedUpdatableLabel-> setText(speedText);
    }
}



/*!
 * \brief MainWindow::distance
 * \param
 * \param deltaPhi, deltaLanda
 * \param x, x_str, y, y_str
 * \return "None"
 *
 * \quotation
 *
 * \endquotation
 */
void MainWindow::distance()
{
    int radius = 6371; //km

    double _distance_ = (qAcos(qSin(qDegreesToRadians(la.at(la.count()-1)))*qSin(qDegreesToRadians(lat1))+ qCos(qDegreesToRadians(la.at(la.count()-1)))*qCos(qDegreesToRadians(lat1))*qCos(qDegreesToRadians(lo.at(lo.count()-1))-qDegreesToRadians(lon1)))*radius)*1000;
    distance_ = QString::number(_distance_, 'g', 17); //Convert double to string

    ui-> distanceUpdatableLabel-> setText(distance_);

    lat1_radian = qDegreesToRadians(lat1);
    lon1_radian = qDegreesToRadians(lon1);

    lat2_radian = qDegreesToRadians(la.at(la.count()-1));
    lon2_radian = qDegreesToRadians(lo.at(lo.count()-1));

    deltaPhi = lat2_radian - lat1_radian;
    deltaLanda = lon2_radian - lon1_radian;

    x = deltaPhi*6371*1000;
    x_str = QString::number(x, 'g', 17);
    ui-> xUpdatableLabel-> setText(x_str);

    y = deltaLanda*6371*1000*qCos(lat2_radian);
    y_str = QString::number(y, 'g', 17);
    ui-> yUpdatableLabel-> setText(y_str);

    emit plotXYSignal();
}



/*!
 * \brief MainWindow::writeData
 */
void MainWindow::writeData() {
    gpsStartRecordStream->operator <<(gpsTime);
    gpsStartRecordStream->operator <<("; ");
    gpsStartRecordStream->operator <<(x_str);
    gpsStartRecordStream->operator <<("; ");
    gpsStartRecordStream->operator <<(y_str);
    gpsStartRecordStream->operator <<("; ");
    gpsStartRecordStream->operator <<(QString::number(gpsLatitude_double, 'g', 17));
    gpsStartRecordStream->operator <<("; ");
    gpsStartRecordStream->operator <<(QString::number(gpsLongitude_double, 'g', 17));
    gpsStartRecordStream->operator <<("; ");
    gpsStartRecordStream->operator << (sai);
    gpsStartRecordStream->operator <<("; ");
    gpsStartRecordStream->operator << (speedText);
    gpsStartRecordStream->operator <<("; ");
    gpsStartRecordStream->operator <<(distance_) << "\n";
}



/*!
 * \brief MainWindow::XandY
 * \param xList, yList, sai
 * \return "None"
 *
 * \quotation
 *
 * \endquotation
 */
void MainWindow::calculateSai() {

    sai = qTan(x/y);
    ui-> saiLabel-> setText(QString::number(sai, 'g', 17));
}



/*!
 * \brief MainWindow::on_plotXYBtn_clicked
 */
void MainWindow::on_plotXYBtn_clicked()
{
    ui-> stackedWidget-> setCurrentIndex(1);
}



/*!
 * \brief MainWindow::on_savePlotBtn_clicked
 */
void MainWindow::on_savePlotBtn_clicked()
{
    const auto dpr = chartView-> devicePixelRatioF();
    QPixmap buffer(chartView->width()*dpr, chartView->height()*dpr);
    buffer.setDevicePixelRatio(dpr);
    buffer.fill(Qt::transparent);

    QPainter *paint = new QPainter(&buffer);
    paint-> setPen(*(new QColor(255, 34, 255, 255)));
    chartView-> render(paint);

    QString imageDir = QFileDialog::getSaveFileName(this, tr("Save File"), "image001", tr("*.png"));
    imageFile = new QFile(imageDir, this);

    imageFile->open(QIODevice::WriteOnly);
    buffer.save(imageFile, "PNG");
    imageFile->close();
}


void MainWindow::on_backToMainWindowBtn_clicked()
{
    ui-> stackedWidget-> setCurrentIndex(0);
}

void MainWindow::on_backToMainWindowBtn_2_clicked()
{
    ui-> stackedWidget-> setCurrentIndex(0);
}



void MainWindow::plotXY() {

    xList.append(x);
    yList.append(y);

    ui-> xLabelPlot-> setText(QString::number(x, 'g', 17));
    ui-> yLabelPlot-> setText(QString::number(y, 'g', 17));
    ui-> HeadingLabelPlot-> setText(QString::number(sai, 'g', 17));

    series-> append(y, x);

    chart->addSeries(series);
    chart->legend()->hide();
    chart->createDefaultAxes();

    chart->axisX()-> setTitleText("Y");
    chart->axisY()-> setTitleText("X");

    if (start.userChoice()=="manual") {

        chart-> axisX()-> setRange(start.YMin(), start.YMax());
        chart-> axisY()-> setRange(start.XMin(), start.XMax());

    } else if (start.userChoice()=="auto") {

        double minAxisX = *std::min_element(xList.begin(), xList.end());
        double maxAxisX = *std::max_element(xList.begin(), xList.end());

        double minAxisY = *std::min_element(yList.begin(), yList.end());;
        double maxAxisY = *std::max_element(yList.begin(), yList.end());;

        chart-> axisX()-> setRange(minAxisY, maxAxisY);
        chart-> axisY()-> setRange(minAxisX, maxAxisX);
    }

    chart->setTitle("Graph");

    chartView->setRenderHint(QPainter::Antialiasing);

    ui-> plotXYGridLayout->addWidget(chartView, 1, 1);
}



void MainWindow::on_moreInfoBtn_clicked()
{
    ui-> stackedWidget-> setCurrentIndex(2);
}
