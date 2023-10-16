#include "InitiationQt.h"
InitiationQt::InitiationQt(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	socket = new QTcpSocket(this);
	QObject::connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
	QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
	QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));

	server = new QTcpServer(this);
	QObject::connect(server, SIGNAL(newConnection()), this, SLOT(onServerNewConnection()));
	server->listen(QHostAddress::AnyIPv4, 4321);
	connect(ui.pushButton, &QPushButton::clicked, this, &InitiationQt::onSendButtonClicked);
	//bdd = QSqlDatabase::addDatabase("QMYSQL");
	//bdd.setHostName("192.168.65.186");
	//bdd.setUserName("root");
	//bdd.setPassword("root");
	//bdd.setDatabaseName("basetest");
}
void InitiationQt::onDisplayMessageButtonClicked() {
	ui.label->setText("Hello word !");
}
void InitiationQt::onConnectButtonClicked() {
	QString ip = ui.IPLineEdit->text();
	QString port = ui.portLineEdit->text();


	bool ok;
	int portAsInt = port.toInt(&ok);
	if (ok)
	{
		socket->connectToHost(ip, portAsInt);
	}
}
void InitiationQt::onSocketConnected() {
	ui.connectToHost->setText("Status connexion : connecte !");
}
void InitiationQt::onSocketDisconnected() {
	ui.connectToHost->setText("Status connexion : Déconnecté !");
}
void InitiationQt::onSendMessageButtonClicked() {
	if (socket->state() == QTcpSocket::ConnectedState)
	{
		socket->write("Hello server !\n");
	}
}
void InitiationQt::onSocketReadyRead() {

	QByteArray data = socket->read(socket->bytesAvailable());
	QString str(data);
	ui.connectToHost->setText("Status connexion : Message recu = " + str);
}
void InitiationQt::onServerNewConnection() {
	ui.connectToHost->setText("Un client s'est connecte");
	QTcpSocket * client = server->nextPendingConnection();
	QObject::connect(client, SIGNAL(readyRead()), this, SLOT(onClientReadyRead()));
	QObject::connect(client, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
}
void InitiationQt::onClientDisconnected() {
	QTcpSocket *obj = qobject_cast<QTcpSocket*>(sender());
	QObject::disconnect(obj, SIGNAL(readyRead()), this, SLOT(onClientReadyRead()));
	QObject::disconnect(obj, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
	obj->deleteLater();
}
void InitiationQt::onClientReadyRead() {
	QTcpSocket *obj = qobject_cast<QTcpSocket*>(sender());
	QByteArray coucou = obj->read(obj->bytesAvailable());
	QString str(coucou);
	ui.connectToHost->setText("Status connexion : Message recu = " + str);
	obj->write(coucou);
}

void InitiationQt::onSendButtonClicked() {
	if (socket->state() == QTcpSocket::ConnectedState) {
		QString message = ui.plainTextEdit->toPlainText(); // Récupérer le texte de plainTextEdit
		socket->write(message.toUtf8()); // Envoyer le message au serveur
	}
	QString message = ui.plainTextEdit->toPlainText(); // Récupérer le texte de plainTextEdit
	QByteArray sendData = message.toUtf8(); // Convertir en QByteArray pour l'envoi
	qint64 bytesWritten = socket->write(sendData); // Envoyer le message au serveur
	if (bytesWritten == -1) {
		ui.label_5->setText("Échec de l'envoi du message !");
	}
	else {
		ui.label_5->setText("Message envoyé avec succès !");
	}
}


void InitiationQt::ClientReadyRead() {
	QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
	QByteArray data = client->readAll();
	QString str(data);
	ui.label_6->setText("Message reçu : " + str);
	// Vous pouvez faire d'autres opérations avec le message reçu ici si nécessaire
}






