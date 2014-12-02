#ifndef NET_H
#define NET_H

#include <QNetworkAccessManager>
#include <QObject>

class Net : public QObject
{
    Q_OBJECT
public:
    static Net* getNetManager();
    bool isConnectedToNet();
signals:

public slots:

private:
    explicit Net(QObject *parent = 0);
    explicit Net(const Net&);
    Net operator =(Net&);

    QNetworkAccessManager* netAccess;
};

bool isConnected();

#endif // NET_H
