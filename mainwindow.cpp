#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qTel = new QTelnet(this);
     m_size = 0;
     m_response.clear();

    connect( qTel , SIGNAL(newData(const char*,int)), this, SLOT(onResponse(const char*,int)) );
    connect( qTel, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onStateChanged(QAbstractSocket::SocketState)) );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onResponse(const char *msg, int count)
{
    m_response.clear();
    m_size = count;
    m_response = QString::fromStdString(msg);
    m_response.truncate(count);
    m_response.replace("\n","");
    ui->plainTextEdit->insertPlainText("\n");
    ui->plainTextEdit->insertPlainText(QByteArray(msg,count));
}

void MainWindow::on_pbConnect_pressed()
{
    if (qTel->isConnected()) {
        qTel->disconnectFromHost();
        ui->pbConnect->setText("Connect to Camera");

    } else {
        if(m_insightConnect()){
          ui->pbConnect->setText("Disconnect From Camera");
        }
        else {
            //TO-DO:Add Connection Failed actions
        }
    }
}

void MainWindow::onStateChanged(QAbstractSocket::SocketState s)
{
    switch( s )
    {
    case QAbstractSocket::UnconnectedState:
        //setStatusText( tr("Desconectado"), true );
        ui->pbConnect->setText( tr("Conectar") );
        break;
    case QAbstractSocket::HostLookupState:
        //setStatusText( tr("Resolviendo DNS %1").arg(telnet.peerName()), true );
        qDebug()<<QString("Resolviendo DNS %1").arg(qTel->peerName());
        ui->plainTextEdit->insertPlainText(QString("Resolviendo DNS %1").arg(qTel->peerName()));
        ui->plainTextEdit->insertPlainText("\n");
        break;
    case QAbstractSocket::ConnectingState:
        //setStatusText( tr("Conectando a %1").arg(qTel->peerInfo()), true );
        qDebug()<<QString("Resolviendo DNS %1").arg(qTel->peerInfo());
        ui->plainTextEdit->insertPlainText(QString("Resolviendo DNS %1").arg(qTel->peerInfo()));
        ui->plainTextEdit->insertPlainText("\n");
        ui->pbConnect->setText( tr("Cancelar") );
        break;
    case QAbstractSocket::ConnectedState:
        //setStatusText( tr("Conectado a %1").arg(telnet.peerInfo()), true );
        qDebug()<<QString("Resolviendo DNS %1").arg(qTel->peerInfo());
        ui->plainTextEdit->insertPlainText(QString("Resolviendo DNS %1").arg(qTel->peerInfo()));
        ui->plainTextEdit->insertPlainText("\n");
        ui->pbConnect->setText( tr("Desconectar") );
        break;
    case QAbstractSocket::BoundState:
        //setStatusText( tr("Enlazado"), true );
        qDebug()<<"Enlazado";
        ui->plainTextEdit->insertPlainText("Enlazado");
        ui->plainTextEdit->insertPlainText("\n");
        ui->pbConnect->setText( tr("- - - -") );
        break;
    case QAbstractSocket::ListeningState:
        //setStatusText( tr("Escuchando"), true );
        qDebug()<<"Escuchando";
        ui->plainTextEdit->insertPlainText("Enlazado");
        ui->plainTextEdit->insertPlainText("\n");
        ui->pbConnect->setText( tr("- - - -") );
        break;
    case QAbstractSocket::ClosingState:
        //setStatusText( tr("Cerrando"), true );
        qDebug()<<"Cerrando";
        ui->plainTextEdit->insertPlainText("Cerrando\n");
        ui->pbConnect->setText( tr("Forzar cierre") );
        break;

    }
}

bool MainWindow::m_insightConnect()
{
    //Connect To Camera Process

    //Connect to Adrress
    qTel->connectToHost(ui->txtIpAdr->text(), ui->spPortT->value() );

    connect( qTel , &QTelnet::stateChanged , [=] (const int st)
            {
        if(st == 3){
            this->sendCommand(ui->txtUser->text());
            this->sendCommand(ui->txtPsswrd->text());
        }
    } );

    return true;
}

void MainWindow::sendCommand(const QString &cmd)
{
    if(qTel->isConnected()){
        qTel->sendData(cmd.toLatin1());
        qTel->sendData("\n");
    }else {
    qDebug()<<"Not Connected";
}
}
