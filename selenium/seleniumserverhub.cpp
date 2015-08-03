#include "seleniumserverhub.h"

SeleniumServerHub::SeleniumServerHub(QString host, int port) :
    QObject()
{ 
    m_result = "";

    buildUrl(host, port);
}

SeleniumServerHub::~SeleniumServerHub()
{
    qDebug()<<"I am destructor SeleniumServerHub!";
}

QString SeleniumServerHub::host()
{
    return m_host;
}

int SeleniumServerHub::port()
{
    return m_port;
}

//####### PROTECTED #######
void SeleniumServerHub::buildUrl(QString host, int port)
{
    m_host = host.replace("http://", "");
    m_port = port;

    m_urlHub = "http://";
    m_urlHub.append(m_host).append(":").append(QString::number(m_port)).append("/wd/hub/");
}

QJsonObject SeleniumServerHub::exec(QString url, QByteArray rawBody, int method)
{
    m_result = "";
    QJsonObject result;

    QUrl urlObject(m_urlHub + url);
    QNetworkRequest request(urlObject);

    QString contentType = "application/json";

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    if(method == GET) {

        m_reply = manager->get(request);

    } else if(method == POST) {

        if(rawBody.isEmpty()) {

            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            m_reply = manager->post(request, "");

        } else {

            QByteArray postDataSize = QByteArray::number(rawBody.size());
            request.setRawHeader("Content-Type", contentType.toLatin1());
            request.setRawHeader("Content-Length", postDataSize);

            m_reply = manager->post(request, rawBody);
        }

    } else if(method == DELETE) {

        m_reply = manager->deleteResource(request);

    } else {
        throw (new QueleniumException("Unsupported method", 99));
    }

    connect(m_reply, SIGNAL(readyRead()), SLOT(slotReplyResponse()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(slotReplyError()));
    connect(m_reply, SIGNAL(finished()), SLOT(slotFinishRequest()));

    //this loop for synchronize requests
    QEventLoop loop;
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(deleteLater()));
    connect(m_reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if(m_result.trimmed().isEmpty()) {
        WebDriverException::throwException("Empty reply from server");
    }

    result = QJsonDocument::fromJson(m_result.toUtf8()).object();

    if(result["status"].toInt() != ResponseStatusCodes::SUCCESS) {

        QString message = result["state"].toString();
        if(result["value"].isObject() && result["value"].toObject()["message"].isString()) {
            message = result["value"].toObject()["message"].toString();
        }

        WebDriverException::throwException(message, result["status"].toInt());
    }

    return result;
}

QJsonObject SeleniumServerHub::post(QString url, QJsonObject rawBody)
{
    QJsonDocument doc;
    doc.setObject(rawBody);

    return exec(url, doc.toJson(), POST);
}

QJsonObject SeleniumServerHub::post(QString url)
{
    return exec(url, "", POST);
}

QJsonObject SeleniumServerHub::get(QString url)
{
    return exec(url, "", GET);
}

QJsonObject SeleniumServerHub::deleteMethod(QString url)
{
    return exec(url, "", DELETE);
}

QString SeleniumServerHub::getValueString(QString s)
{
    QJsonObject result = get(s);

    QString value = "";

    if(result["value"].isString()) {
        value = result["value"].toString();
    } else {
        qDebug()<<"Error get string value "<<s;
    }

    return value;
}

QStringList SeleniumServerHub::getValueArrayString(QString s)
{
    QJsonObject result = get(s);

    QStringList array;

    if(result["value"].isArray()) {
        QJsonArray arr = result["value"].toArray();
        for(int i = 0; i < arr.size(); i++) {
            array << arr.at(i).toString();
        }
    } else {
        qDebug()<<"Error get array string value";
    }

    return array;
}

bool SeleniumServerHub::getValueBool(QString s)
{
    QJsonObject result = get(s);

    bool value = false;

    if(result["value"].isBool()) {
        value = result["value"].toBool();
    } else {
        qDebug()<<"Error get bool value";
    }

    return value;
}

QJsonObject SeleniumServerHub::getValueObject(QString s)
{
    QJsonObject result = get(s);

    QJsonObject value;

    if(result["value"].isObject()) {
        value = result["value"].toObject();
    } else {
        qDebug()<<"Error get object value";
    }

    return value;
}

QJsonArray SeleniumServerHub::getValueArray(QString s)
{
    QJsonObject result = get(s);

    QJsonArray array;

    if(result["value"].isArray()) {
        array = result["value"].toArray();
    } else {
        qDebug()<<"Error get array value";
    }

    return array;
}

int SeleniumServerHub::getValueInt(QString s)
{
    QJsonObject result = get(s);

    int value = 0;

    if(result["value"].isDouble()) {
        value = result["value"].toInt();
    } else {
        qDebug()<<"Error get int value";
    }

    return value;
}

void SeleniumServerHub::releaseReplyResources()
{
    disconnect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReplyResponse()));
    disconnect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,SLOT(slotReplyError()));
    disconnect(m_reply, SIGNAL(finished()), this, SLOT(slotFinishRequest()));

    m_reply->manager()->deleteLater();
    m_reply->deleteLater();
}

//####### SLOTS #######
void SeleniumServerHub::slotFinishRequest()
{
    m_reply->close();
    releaseReplyResources();

    //qDebug()<<"result: "<<m_result;
}

void SeleniumServerHub::slotReplyResponse()
{
    m_result.append(QString(m_reply->readAll()));    
}

void SeleniumServerHub::slotReplyError()
{
    releaseReplyResources();

    //qDebug()<<"Error: "<<m_reply->errorString();

    if(m_result.isEmpty()) {

        QJsonObject error;
        error["status"] = -1;
        error["state"] = m_reply->errorString();

        QJsonDocument jsonDoc;
        jsonDoc.setObject(error);

        m_result = jsonDoc.toJson();
    }

}
