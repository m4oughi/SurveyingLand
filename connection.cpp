#include "connection.h"
#include "ui_connection.h"
#include "mainwindow.h"


Connection::Connection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Connection)
{
    ui->setupUi(this);

    // Find available ports & set ports UI
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {

        QStringList list;
        list << info.portName();

        ui->portNameComboBox-> addItem(list.first(), list);
    }


    // Fixed point Baudrate UI
    ui->BaudrateComboBox->addItem(QStringLiteral("1200"),    QSerialPort::Baud1200    );
    ui->BaudrateComboBox->addItem(QStringLiteral("2400"),    QSerialPort::Baud2400    );
    ui->BaudrateComboBox->addItem(QStringLiteral("4800"),    QSerialPort::Baud4800    );
    ui->BaudrateComboBox->addItem(QStringLiteral("9600"),    QSerialPort::Baud9600    );
    ui->BaudrateComboBox->addItem(QStringLiteral("19200"),   QSerialPort::Baud19200   );
    ui->BaudrateComboBox->addItem(QStringLiteral("38400"),   QSerialPort::Baud38400   );
    ui->BaudrateComboBox->addItem(QStringLiteral("57600"),   QSerialPort::Baud57600   );
    ui->BaudrateComboBox->addItem(QStringLiteral("115200"),  QSerialPort::Baud115200  );
    ui->BaudrateComboBox->addItem(QStringLiteral("Unknown"), QSerialPort::UnknownBaud );
    ui->BaudrateComboBox->setCurrentIndex(7); // Set default 115200
}


Connection::~Connection()
{
    delete ui;
}


void Connection::on_connectionBtn_clicked()
{
    gpsPortName = ui-> portNameComboBox-> currentText(); // Get fixed gps PortName from user
    gpsBaudrate = ui-> BaudrateComboBox-> currentText().toInt(); // Get fixed gps Baudrate from user

    qDebug() << gpsPortName << gpsBaudrate;

    this->close();
}


QString Connection::name() const {
    return gpsPortName;
}

qint32 Connection::baud() const {
    return gpsBaudrate;
}
