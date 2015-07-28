#ifndef SELENIUMSERVERHUB_H
#define SELENIUMSERVERHUB_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>
#include <QEventLoop>

#include "responsestatuscodes.h"
#include "webdriverexception.h"

class WebDriverException;
class SeleniumServerHub : public QObject
{
    Q_OBJECT
public:
    explicit SeleniumServerHub(QString host, QString port);
    ~SeleniumServerHub();

    enum HTTP_METHOD {
        GET,
        POST,
        PUT,
        DELETE
    };

    QString host();
    QString port();

protected:
    void buildUrl(QString host, QString port);

    QString     getValueString(QString s);
    QStringList getValueArrayString(QString s);
    bool        getValueBool(QString s);
    QJsonObject getValueObject(QString s);
    QJsonArray  getValueArray(QString s);
    int         getValueInt(QString s);

    QJsonObject exec(QString url, QByteArray rawBody, int method);
    QJsonObject post(QString url, QJsonObject rawBody);
    QJsonObject post(QString url);
    QJsonObject get(QString url);
    QJsonObject deleteMethod(QString url);
    void releaseReplyResources();

    QString m_host;
    QString m_port;
    QString m_urlHub;

    QNetworkReply *m_reply;

    QString m_result;
    QString m_sessionId;

signals:

public slots:
    void slotFinishRequest();
    void slotReplyResponse();
    void slotReplyError();

};

#endif // SELENIUMSERVERHUB_H
