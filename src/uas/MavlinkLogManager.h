/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


#ifndef MavlinkLogManager_H
#define MavlinkLogManager_H

#include <QObject>

#include "QmlObjectListModel.h"
#include "QGCLoggingCategory.h"
#include "QGCToolbox.h"

Q_DECLARE_LOGGING_CATEGORY(MavlinkLogManagerLog)

class QNetworkAccessManager;
class MavlinkLogManager;

//-----------------------------------------------------------------------------
class MavlinkLogFiles : public QObject
{
    Q_OBJECT
public:
    MavlinkLogFiles    (MavlinkLogManager* manager, const QString& filePath);

    Q_PROPERTY(QString      name        READ    name                                CONSTANT)
    Q_PROPERTY(quint32      size        READ    size                                CONSTANT)
    Q_PROPERTY(bool         selected    READ    selected    WRITE setSelected       NOTIFY selectedChanged)
    Q_PROPERTY(bool         uploading   READ    uploading                           NOTIFY uploadingChanged)
    Q_PROPERTY(qreal        progress    READ    progress                            NOTIFY progressChanged)

    QString     name                () { return _name; }
    quint32     size                () { return _size; }
    bool        selected            () { return _selected; }
    bool        uploading           () { return _uploading; }
    qreal       progress            () { return _progress; }

    void        setSelected         (bool selected);
    void        setUploading        (bool uploading);
    void        setProgress         (qreal progress);

signals:
    void        selectedChanged     ();
    void        uploadingChanged    ();
    void        progressChanged     ();

private:
    MavlinkLogManager*  _manager;
    QString             _name;
    quint32             _size;
    bool                _selected;
    bool                _uploading;
    qreal               _progress;
};

class MavlinkLogManager : public QGCTool
{
    Q_OBJECT

public:
    MavlinkLogManager    (QGCApplication* app);
    ~MavlinkLogManager   ();

    Q_PROPERTY(QString              emailAddress    READ    emailAddress    WRITE setEmailAddress   NOTIFY emailAddressChanged)
    Q_PROPERTY(QString              description     READ    description     WRITE setDescription    NOTIFY descriptionChanged)
    Q_PROPERTY(QString              uploadURL       READ    uploadURL       WRITE setUploadURL      NOTIFY uploadURLChanged)
    Q_PROPERTY(bool                 enableAutolog   READ    enableAutolog   WRITE setEnableAutolog  NOTIFY enableAutologChanged)
    Q_PROPERTY(bool                 busy            READ    busy                                    NOTIFY busyChanged)
    Q_PROPERTY(QmlObjectListModel*  logFiles        READ    logFiles                                NOTIFY logFilesChanged)

    Q_INVOKABLE void uploadLog      ();
    Q_INVOKABLE void deleteLog      ();
    Q_INVOKABLE void cancelUpload   ();

    QString     emailAddress        () { return _emailAddress; }
    QString     description         () { return _description; }
    QString     uploadURL           () { return _uploadURL; }
    bool        enableAutolog       () { return _enableAutolog; }
    bool        busy                ();

    QmlObjectListModel* logFiles    () { return &_logFiles; }

    void        setEmailAddress     (QString email);
    void        setDescription      (QString description);
    void        setUploadURL        (QString url);
    void        setEnableAutolog    (bool enable);

    // Override from QGCTool
    void        setToolbox          (QGCToolbox *toolbox);

signals:
    void emailAddressChanged        ();
    void descriptionChanged         ();
    void uploadURLChanged           ();
    void enableAutologChanged       ();
    void logFilesChanged            ();
    void selectedCountChanged       ();
    void busyChanged                ();
    void readyRead                  (QByteArray data);
    void failed                     ();
    void succeed                    ();
    void abortUpload                ();

private slots:
    void _uploadFinished            ();
    void _dataAvailable             ();
    void _uploadProgress            (qint64 bytesSent, qint64 bytesTotal);

private:
    bool _sendLog                   (const QString& logFile);
    bool _processUploadResponse     (int http_code, QByteArray &data);

private:
    QString                 _description;
    QString                 _emailAddress;
    QString                 _uploadURL;
    QString                 _logPath;
    bool                    _enableAutolog;
    QNetworkAccessManager*  _nam;
    QmlObjectListModel      _logFiles;
    MavlinkLogFiles*        _currentLogfile;
};

#endif
