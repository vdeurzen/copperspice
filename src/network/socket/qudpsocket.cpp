/***********************************************************************
*
* Copyright (c) 2012-2017 Barbara Geller
* Copyright (c) 2012-2017 Ansel Sermersheim
* Copyright (c) 2012-2016 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This file is part of CopperSpice.
*
* CopperSpice is free software. You can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* <http://www.gnu.org/licenses/>.
*
***********************************************************************/

//#define QUDPSOCKET_DEBUG

#include <qhostaddress.h>
#include <qnetworkinterface.h>
#include <qabstractsocket_p.h>
#include <qudpsocket.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_UDPSOCKET

#define QT_CHECK_BOUND(function, a) do { \
    if (!isValid()) { \
        qWarning(function" called on a QUdpSocket when not in QUdpSocket::BoundState"); \
        return (a); \
    } } while (0)

class QUdpSocketPrivate : public QAbstractSocketPrivate
{
   Q_DECLARE_PUBLIC(QUdpSocket)

   bool doEnsureInitialized(const QHostAddress &bindAddress, quint16 bindPort,
                            const QHostAddress &remoteAddress);
 public:
   inline bool ensureInitialized(const QHostAddress &bindAddress, quint16 bindPort) {
      return doEnsureInitialized(bindAddress, bindPort, QHostAddress());
   }

   inline bool ensureInitialized(const QHostAddress &remoteAddress) {
      return doEnsureInitialized(QHostAddress(), 0, remoteAddress);
   }
};

bool QUdpSocketPrivate::doEnsureInitialized(const QHostAddress &bindAddress, quint16 bindPort,
      const QHostAddress &remoteAddress)
{
   const QHostAddress *address = &bindAddress;
   QAbstractSocket::NetworkLayerProtocol proto = address->protocol();
   if (proto == QUdpSocket::UnknownNetworkLayerProtocol) {
      address = &remoteAddress;
      proto = address->protocol();
   }

   // now check if the socket engine is initialized and to the right type
   if (!socketEngine || !socketEngine->isValid()) {
      resolveProxy(remoteAddress.toString(), bindPort);
      if (!initSocketLayer(address->protocol())) {
         return false;
      }
   }

   return true;
}

/*!
    Creates a QUdpSocket object.

    \a parent is passed to the QObject constructor.

    \sa socketType()
*/
QUdpSocket::QUdpSocket(QObject *parent)
   : QAbstractSocket(UdpSocket, *new QUdpSocketPrivate, parent)
{
   d_func()->isBuffered = false;
}

/*!
    Destroys the socket, closing the connection if necessary.

    \sa close()
*/
QUdpSocket::~QUdpSocket()
{
}

/*!
    Binds this socket to the address \a address and the port \a port.
    When bound, the signal readyRead() is emitted whenever a UDP
    datagram arrives on the specified address and port. This function
    is useful to write UDP servers.

    On success, the functions returns true and the socket enters
    BoundState; otherwise it returns false.

    The socket is bound using the DefaultForPlatform BindMode.

    \sa readDatagram()
*/
bool QUdpSocket::bind(const QHostAddress &address, quint16 port)
{
   Q_D(QUdpSocket);
   if (!d->ensureInitialized(address, port)) {
      return false;
   }

   bool result = d_func()->socketEngine->bind(address, port);
   d->cachedSocketDescriptor = d->socketEngine->socketDescriptor();

   if (!result) {
      d->socketError = d_func()->socketEngine->error();
      setErrorString(d_func()->socketEngine->errorString());
      emit error(d_func()->socketError);
      return false;
   }

   d->state = BoundState;
   d->localAddress = d->socketEngine->localAddress();
   d->localPort = d->socketEngine->localPort();

   emit stateChanged(d_func()->state);
   d_func()->socketEngine->setReadNotificationEnabled(true);
   return true;
}

/*!
    \since 4.1
    \overload

    Binds to \a address on port \a port, using the BindMode \a mode.
*/
bool QUdpSocket::bind(const QHostAddress &address, quint16 port, BindMode mode)
{
   Q_D(QUdpSocket);
   if (!d->ensureInitialized(address, port)) {
      return false;
   }

#ifdef Q_OS_UNIX
   if ((mode & ShareAddress) || (mode & ReuseAddressHint)) {
      d->socketEngine->setOption(QAbstractSocketEngine::AddressReusable, 1);
   } else {
      d->socketEngine->setOption(QAbstractSocketEngine::AddressReusable, 0);
   }
#endif
#ifdef Q_OS_WIN
   if (mode & ReuseAddressHint) {
      d->socketEngine->setOption(QAbstractSocketEngine::AddressReusable, 1);
   } else {
      d->socketEngine->setOption(QAbstractSocketEngine::AddressReusable, 0);
   }
   if (mode & DontShareAddress) {
      d->socketEngine->setOption(QAbstractSocketEngine::BindExclusively, 1);
   } else {
      d->socketEngine->setOption(QAbstractSocketEngine::BindExclusively, 0);
   }
#endif
   bool result = d_func()->socketEngine->bind(address, port);
   d->cachedSocketDescriptor = d->socketEngine->socketDescriptor();

   if (!result) {
      d->socketError = d_func()->socketEngine->error();
      setErrorString(d_func()->socketEngine->errorString());
      emit error(d_func()->socketError);
      return false;
   }

   d->state = BoundState;
   d->localAddress = d->socketEngine->localAddress();
   d->localPort = d->socketEngine->localPort();

   emit stateChanged(d_func()->state);
   d_func()->socketEngine->setReadNotificationEnabled(true);
   return true;
}

/*! \overload

    Binds to QHostAddress:Any on port \a port.
*/
bool QUdpSocket::bind(quint16 port)
{
   return bind(QHostAddress::Any, port);
}

/*!
    \since 4.1
    \overload

    Binds to QHostAddress:Any on port \a port, using the BindMode \a mode.
*/
bool QUdpSocket::bind(quint16 port, BindMode mode)
{
   return bind(QHostAddress::Any, port, mode);
}

#ifndef QT_NO_NETWORKINTERFACE

/*!
    \since 4.8

    Joins the the multicast group specified by \a groupAddress on the default
    interface chosen by the operating system. The socket must be in BoundState,
    otherwise an error occurs.

    This function returns true if successful; otherwise it returns false
    and sets the socket error accordingly.

    \sa leaveMulticastGroup()
*/
bool QUdpSocket::joinMulticastGroup(const QHostAddress &groupAddress)
{
   return joinMulticastGroup(groupAddress, QNetworkInterface());
}

/*!
    \since 4.8
    \overload

    Joins the multicast group address \a groupAddress on the interface \a
    iface.

    \sa leaveMulticastGroup()
*/
bool QUdpSocket::joinMulticastGroup(const QHostAddress &groupAddress,
                                    const QNetworkInterface &iface)
{
   Q_D(QUdpSocket);
   QT_CHECK_BOUND("QUdpSocket::joinMulticastGroup()", false);
   return d->socketEngine->joinMulticastGroup(groupAddress, iface);
}

/*!
    \since 4.8

    Leaves the multicast group specified by \a groupAddress on the default
    interface chosen by the operating system. The socket must be in BoundState,
    otherwise an error occurs.

   This function returns true if successful; otherwise it returns false and
   sets the socket error accordingly.

   \sa joinMulticastGroup()
*/
bool QUdpSocket::leaveMulticastGroup(const QHostAddress &groupAddress)
{
   return leaveMulticastGroup(groupAddress, QNetworkInterface());
}

/*!
    \since 4.8
    \overload

    Leaves the multicast group specified by \a groupAddress on the interface \a
    iface.

    \sa joinMulticastGroup()
*/
bool QUdpSocket::leaveMulticastGroup(const QHostAddress &groupAddress,
                                     const QNetworkInterface &iface)
{
   QT_CHECK_BOUND("QUdpSocket::leaveMulticastGroup()", false);
   return d_func()->socketEngine->leaveMulticastGroup(groupAddress, iface);
}

/*!
    \since 4.8

    Returns the interface for the outgoing interface for multicast datagrams.
    This corresponds to the IP_MULTICAST_IF socket option for IPv4 sockets and
    the IPV6_MULTICAST_IF socket option for IPv6 sockets. If no interface has
    been previously set, this function returns an invalid QNetworkInterface.
    The socket must be in BoundState, otherwise an invalid QNetworkInterface is
    returned.

    \sa setMulticastInterface()
*/
QNetworkInterface QUdpSocket::multicastInterface() const
{
   Q_D(const QUdpSocket);
   QT_CHECK_BOUND("QUdpSocket::multicastInterface()", QNetworkInterface());
   return d->socketEngine->multicastInterface();
}

/*!
    \since 4.8

    Sets the outgoing interface for multicast datagrams to the interface \a
    iface. This corresponds to the IP_MULTICAST_IF socket option for IPv4
    sockets and the IPV6_MULTICAST_IF socket option for IPv6 sockets. The
    socket must be in BoundState, otherwise this function does nothing.

    \sa multicastInterface(), joinMulticastGroup(), leaveMulticastGroup()
*/
void QUdpSocket::setMulticastInterface(const QNetworkInterface &iface)
{
   Q_D(QUdpSocket);
   if (!isValid()) {
      qWarning("QUdpSocket::setMulticastInterface() called on a QUdpSocket when not in QUdpSocket::BoundState");
      return;
   }
   d->socketEngine->setMulticastInterface(iface);
}

#endif // QT_NO_NETWORKINTERFACE

/*!
    Returns true if at least one datagram is waiting to be read;
    otherwise returns false.

    \sa pendingDatagramSize(), readDatagram()
*/
bool QUdpSocket::hasPendingDatagrams() const
{
   QT_CHECK_BOUND("QUdpSocket::hasPendingDatagrams()", false);
   return d_func()->socketEngine->hasPendingDatagrams();
}

/*!
    Returns the size of the first pending UDP datagram. If there is
    no datagram available, this function returns -1.

    \sa hasPendingDatagrams(), readDatagram()
*/
qint64 QUdpSocket::pendingDatagramSize() const
{
   QT_CHECK_BOUND("QUdpSocket::pendingDatagramSize()", -1);
   return d_func()->socketEngine->pendingDatagramSize();
}

/*!
    Sends the datagram at \a data of size \a size to the host
    address \a address at port \a port. Returns the number of
    bytes sent on success; otherwise returns -1.

    Datagrams are always written as one block. The maximum size of a
    datagram is highly platform-dependent, but can be as low as 8192
    bytes. If the datagram is too large, this function will return -1
    and error() will return DatagramTooLargeError.

    Sending datagrams larger than 512 bytes is in general disadvised,
    as even if they are sent successfully, they are likely to be
    fragmented by the IP layer before arriving at their final
    destination.

    \warning In S60 5.0 and earlier versions, the writeDatagram return
    value is not reliable for large datagrams.

    \warning Calling this function on a connected UDP socket may
    result in an error and no packet being sent. If you are using a
    connected socket, use write() to send datagrams.

    \sa readDatagram(), write()
*/
qint64 QUdpSocket::writeDatagram(const char *data, qint64 size, const QHostAddress &address,
                                 quint16 port)
{
   Q_D(QUdpSocket);
#if defined QUDPSOCKET_DEBUG
   qDebug("QUdpSocket::writeDatagram(%p, %llu, \"%s\", %i)", data, size,
          address.toString().toLatin1().constData(), port);
#endif
   if (!d->doEnsureInitialized(QHostAddress::Any, 0, address)) {
      return -1;
   }
   if (state() == UnconnectedState) {
      bind();
   }

   qint64 sent = d->socketEngine->writeDatagram(data, size, address, port);
   d->cachedSocketDescriptor = d->socketEngine->socketDescriptor();

   if (sent >= 0) {
      emit bytesWritten(sent);
   } else {
      d->socketError = d->socketEngine->error();
      setErrorString(d->socketEngine->errorString());
      emit error(d->socketError);
   }
   return sent;
}

/*!
    \fn qint64 QUdpSocket::writeDatagram(const QByteArray &datagram,
                                             const QHostAddress &host, quint16 port)
    \overload

    Sends the datagram \a datagram to the host address \a host and at
    port \a port.
*/

/*!
    Receives a datagram no larger than \a maxSize bytes and stores
    it in \a data. The sender's host address and port is stored in
    *\a address and *\a port (unless the pointers are 0).

    Returns the size of the datagram on success; otherwise returns
    -1.

    If \a maxSize is too small, the rest of the datagram will be
    lost. To avoid loss of data, call pendingDatagramSize() to
    determine the size of the pending datagram before attempting to
    read it. If \a maxSize is 0, the datagram will be discarded.

    \sa writeDatagram(), hasPendingDatagrams(), pendingDatagramSize()
*/
qint64 QUdpSocket::readDatagram(char *data, qint64 maxSize, QHostAddress *address,
                                quint16 *port)
{
   Q_D(QUdpSocket);

#if defined QUDPSOCKET_DEBUG
   qDebug("QUdpSocket::readDatagram(%p, %llu, %p, %p)", data, maxSize, address, port);
#endif
   QT_CHECK_BOUND("QUdpSocket::readDatagram()", -1);
   qint64 readBytes = d->socketEngine->readDatagram(data, maxSize, address, port);
   d_func()->socketEngine->setReadNotificationEnabled(true);
   if (readBytes < 0) {
      d->socketError = d->socketEngine->error();
      setErrorString(d->socketEngine->errorString());
      emit error(d->socketError);
   }
   return readBytes;
}
#endif // QT_NO_UDPSOCKET

QT_END_NAMESPACE
