#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTelnet/QTelnet.h>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
 void onResponse(const char *msg , int count);

private slots:


    void on_pbConnect_pressed();
    void onStateChanged(QAbstractSocket::SocketState s);

private:
    Ui::MainWindow *ui;
    QTelnet *qTel;

    bool m_insightConnect();
    int m_size;
    QString m_response;
    void sendCommand(const QString &cmd);



};

#endif // MAINWINDOW_H
