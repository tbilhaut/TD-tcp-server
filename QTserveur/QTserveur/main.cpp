#define _CTR_NO_SECURE_WARNINGS_
#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpServer>
#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include <QCoreApplication>


class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject* parent = 0) : QTcpServer(parent) {}

protected:
    void incomingConnection(qintptr socketDescriptor)
    {
        // Cr�ez un socket pour g�rer la nouvelle connexion.
        QTcpSocket* clientSocket = new QTcpSocket(this);
        clientSocket->setSocketDescriptor(socketDescriptor);

        // Connectez le signal readyRead() pour recevoir des donn�es du client.
        connect(clientSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

        // Connectez le signal disconnected() pour g�rer la d�connexion du client.
        connect(clientSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    }

public slots:
    void readyRead()
    {
            QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());

            if (!clientSocket)
                return;

            // Lisez les donn�es envoy�es par le client.
            QByteArray requestData = clientSocket->readAll();
            QString request = QString(requestData).trimmed();

            QString response;

            // Traitez la demande du client ici en fonction du protocole de communication.
            if (request.startsWith("Td") && request.endsWith("?"))
            {
                // G�n�rez une temp�rature en degr�s Celsius al�atoire entre -20.00 et +35.00.
                double temperature = QRandomGenerator::global()->generateDouble() * (35.00 + 20.00) - 20.00;

                response = QString("Td%1,%2").arg(request.mid(2, 2)).arg(temperature, 0, 'f', 2);
            }
            else if (request.startsWith("Tf") && request.endsWith("?"))
            {
                // G�n�rez une temp�rature en degr�s Fahrenheit al�atoire entre -20.00 et +35.00.
                double celsiusTemperature = QRandomGenerator::global()->generateDouble() * (35.00 + 20.00) - 20.00;
                double fahrenheitTemperature = celsiusTemperature * 9.0 / 5.0 + 32.0;
                response = QString("Tf%1,%2").arg(request.mid(2, 2)).arg(fahrenheitTemperature, 0, 'f', 2);
            }
            else if (request.startsWith("Hr") && request.endsWith("?"))
            {
                // G�n�rez une hygrom�trie al�atoire entre 0.0 et 99.9.
                double humidity = QRandomGenerator::global()->generateDouble() * 99.9;

                response = QString("Hr%1,%2").arg(request.mid(2, 2)).arg(humidity, 0, 'f', 2);
            }

            // Envoyez la r�ponse au client.
            clientSocket->write(response.toUtf8());

            // Affichez la demande du client et la r�ponse g�n�r�e sur le serveur.
            qDebug() << "Message du client : " << request;
            qDebug() << "R�ponse du serveur : " << response;
        

    }

    void disconnected()
    {
        QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
        if (clientSocket)
        {
            clientSocket->deleteLater();
        }
    }
};

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    Server server;

    if (!server.listen(QHostAddress::Any, 12345)) // Remplacez 12345 par le port de votre choix.
    {
        qDebug() << "Le serveur n'a pas pu d�marrer.";
        return 1;
    }

    qDebug() << "Le serveur �coute sur le port 12345..."; // Remplacez par le port que vous avez choisi.

    return app.exec();
}

#include "main.moc"



