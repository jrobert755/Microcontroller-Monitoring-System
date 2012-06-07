#ifndef READTHREAD_H
#define READTHREAD_H

#include <QThread>
#include <MMS.h>
#include <MMSListeners.h>
#include <Listeners.h>

class ReadThread : public QThread
{
    Q_OBJECT
public:
    explicit ReadThread(MMSConnection* connection, QObject *parent = 0);

protected:
    void run();

private:
    MMSConnection* m_connection;
    
signals:
    
public slots:
    
};

#endif // READTHREAD_H
