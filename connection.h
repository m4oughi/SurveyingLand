#ifndef CONNECTION_H
#define CONNECTION_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>


namespace Ui {
class Connection;
}

class Connection : public QDialog
{
    Q_OBJECT

public:
    explicit Connection(QWidget *parent = 0);
    ~Connection();

    QString gpsPortName;
    qint32 gpsBaudrate;

    QString name() const;
    qint32 baud() const;


private slots:

    void on_connectionBtn_clicked();

private:
    Ui::Connection *ui;

};

#endif // CONNECTION_H
