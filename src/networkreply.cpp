#include "NetworkReply.h"
#include "Log4cplusWrapper.h"
#include <QDebug>
#include "ClassMemoryTracer.h"

NetworkReply::NetworkReply(bool bBatch, QObject *parent /* = nullptr */)
	: QObject(parent)
	, m_bBatch(m_bBatch)
{
	TRACE_CLASS_CONSTRUCTOR(NetworkReply);
}

NetworkReply::~NetworkReply()
{
	TRACE_CLASS_DESTRUCTOR(NetworkReply);
}

bool NetworkReply::event(QEvent *event)
{
	if (event->type() == NetworkEvent::ReplyResult)
	{
		ReplyResultEvent *e = static_cast<ReplyResultEvent *>(event);
		if (nullptr != e)
		{
			replyResult(e->request, e->bDestroyed);
		}
		return true;
	}

	return QObject::event(event);
}

void NetworkReply::replyResult(const RequestTask& request, bool bDestroy)
{
	emit requestFinished(request);
	if (bDestroy)
	{
		deleteLater();
	}
}