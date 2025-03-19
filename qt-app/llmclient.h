#ifndef LLMCLIENT_H
#define LLMCLIENT_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <QFile>
//#include <api.h>

class LLMClient : public QObject
{
    Q_OBJECT

public:
    explicit LLMClient(QObject *parent = nullptr);
    void sendRequest(const QString &prompt);
    QString getAPIKey();

    friend class Settings;

signals:
    void responseReceived(const QString &response);

private slots:
    void handleNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    QString apiKey;
    QString userPrompt; // Stores the user prompt for the second request

    void sendLLMRequest(const QString &inputPrompt);
};

#endif // LLMCLIENT_H
