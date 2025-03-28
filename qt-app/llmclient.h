/**
 * @file llmclient.h
 * @brief Declaration of LLMClient class
 * 
 * @author Callum Thompson (cthom226@uwo.ca)
 * @date Mar. 4, 2025
 */

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

class LLMClient : public QObject
{
    Q_OBJECT

public:
    void sendRequest(const QString &prompt);
    void setApiKey(const QString& key);
    static LLMClient *getInstance();
    friend class Settings;

signals:
    void responseReceived(const QString &response);

private slots:
    void handleNetworkReply(QNetworkReply *reply);

private:
    explicit LLMClient();
    LLMClient(const LLMClient &) = delete;
    LLMClient &operator=(const LLMClient &) = delete;
    QNetworkAccessManager *networkManager;
    QString apiKey;
    QString userPrompt; // Stores the user prompt for the second request

    void sendLLMRequest(const QString &inputPrompt);
};

#endif // LLMCLIENT_H
