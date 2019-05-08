#include "networkrequest.h"
#include "networkdownloadrequest.h"
#include "networkuploadrequest.h"
#include "networkcommonrequest.h"
#include "networkmtdownloadrequest.h"
#include "Log4cplusWrapper.h"


NetworkRequest::NetworkRequest(QObject *parent) : QObject(parent)
, m_bAbortManual(false)
, m_pNetworkManager(nullptr)
, m_pNetworkReply(nullptr)
{
    TRACE_CLASS_CONSTRUCTOR(NetworkRequest);
}

NetworkRequest::~NetworkRequest()
{
    TRACE_CLASS_DESTRUCTOR(NetworkRequest);

    abort();
    if (m_pNetworkReply)
    {
        m_pNetworkReply->deleteLater();
        m_pNetworkReply = nullptr;
    }
    if (m_pNetworkManager)
    {
        m_pNetworkManager->deleteLater();
        m_pNetworkManager = nullptr;
    }
}

void NetworkRequest::abort()
{
    m_bAbortManual = true;
    if (m_pNetworkReply)
    {
        m_pNetworkReply->abort();
        m_pNetworkReply->deleteLater();
        m_pNetworkReply = nullptr;
    }
}

void NetworkRequest::start()
{
    m_bAbortManual = false;
}

void NetworkRequest::onError(QNetworkReply::NetworkError code)
{
    LOG_FUN("");
    Q_UNUSED(code);
    qDebug() << "[QMultiThreadNetwork] onError" << QString("Type[%1]").arg(m_request.eType) << m_pNetworkReply->errorString();
    LOG_ERROR("[url]" << m_request.url.toString().toStdWString()
        << "  [type]" << m_request.eType
        << "  [error]" << m_pNetworkReply->errorString().toStdString());
    m_strError = m_pNetworkReply->errorString();
}

void NetworkRequest::onAuthenticationRequired(QNetworkReply *r, QAuthenticator *a)
{
    LOG_FUN(__FUNCTION__ << r->readAll().toStdString());
    Q_UNUSED(a);
    qDebug() << __FUNCTION__ << r->readAll();
}


std::unique_ptr<NetworkRequest> NetworkRequestFactory::create(const RequestType& eType)
{
    std::unique_ptr<NetworkRequest> pRequest;
    switch (eType)
    {
    case eTypeDownload:
    {
        pRequest.reset(new NetworkDownloadRequest());
    }
    break;
    case eTypeMTDownload:
    {
        pRequest.reset(new NetworkMTDownloadRequest());
    }
    break;
    case eTypeUpload:
    {
        pRequest.reset(new NetworkUploadRequest());
    }
    break;
    case eTypePost:
    case eTypeGet:
    case eTypePut:
    case eTypeDelete:
    case eTypeHead:
    {
        pRequest.reset(new NetworkCommonRequest());
    }
    break;
    /*New type add to here*/
    default:
        break;
    }
    return pRequest;
}