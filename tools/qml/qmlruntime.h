/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the tools applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QDECLARATIVEVIEWER_H
#define QDECLARATIVEVIEWER_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QList>

#include "loggerwidget.h"

QT_BEGIN_NAMESPACE

class QDeclarativeView;
class PreviewDeviceSkin;
class QDeclarativeTestEngine;
class QProcess;
class RecordingDialog;
class QDeclarativeTester;
class QNetworkReply;
class QNetworkCookieJar;
class NetworkAccessManagerFactory;
class QTranslator;
class QActionGroup;
class QMenuBar;

class QDeclarativeViewer
    : public QMainWindow
{
    Q_OBJECT

public:
    QDeclarativeViewer(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~QDeclarativeViewer();

    static void registerTypes();

    enum ScriptOption {
        Play = 0x00000001,
        Record = 0x00000002,
        TestImages = 0x00000004,
        TestErrorProperty = 0x00000008,
        SaveOnExit = 0x00000010,
        ExitOnComplete = 0x00000020,
        ExitOnFailure = 0x00000040,
        Snapshot = 0x00000080,
        TestSkipProperty = 0x00000100
    };
    Q_DECLARE_FLAGS(ScriptOptions, ScriptOption)
    void setScript(const QString &s) { m_script = s; }
    void setScriptOptions(ScriptOptions opt) { m_scriptOptions = opt; }
    void setRecordDither(const QString& s) { record_dither = s; }
    void setRecordRate(int fps);
    void setRecordFile(const QString&);
    void setRecordArgs(const QStringList&);
    void setRecording(bool on);
    bool isRecording() const { return recordTimer.isActive(); }
    void setAutoRecord(int from, int to);
    void setDeviceKeys(bool);
    void setNetworkCacheSize(int size);
    void addLibraryPath(const QString& lib);
    void addPluginPath(const QString& plugin);
    void setUseGL(bool use);
    void setUseNativeFileBrowser(bool);
    void setSizeToView(bool sizeToView);

    QDeclarativeView *view() const;
    LoggerWidget *warningsWidget() const;
    QString currentFile() const { return currentFileOrUrl; }

    void enableExperimentalGestures();

public slots:
    void sceneResized(QSize size);
    bool open(const QString&);
    void openFile();
    void openUrl();
    void reload();
    void takeSnapShot();
    void toggleRecording();
    void toggleRecordingWithSelection();
    void ffmpegFinished(int code);
    void showProxySettings ();
    void proxySettingsChanged ();
    void rotateOrientation();
    void statusChanged();
    void setSlowMode(bool);
    void launch(const QString &);

protected:
    virtual void keyPressEvent(QKeyEvent *);
    virtual bool event(QEvent *);
    void createMenu();

private slots:
    void appAboutToQuit();

    void autoStartRecording();
    void autoStopRecording();
    void recordFrame();
    void chooseRecordingOptions();
    void pickRecordingFile();
    void toggleFullScreen();
    void changeOrientation(QAction*);
    void orientationChanged();

    void showWarnings(bool show);
    void warningsWidgetOpened();
    void warningsWidgetClosed();

private:
    void updateSizeHints(bool initial = false);

    QString getVideoFileName();

    LoggerWidget *loggerWindow;
    QDeclarativeView *canvas;
    QSize initialSize;
    QString currentFileOrUrl;
    QTimer recordTimer;
    QString frame_fmt;
    QImage frame;
    QList<QImage*> frames;
    QProcess* frame_stream;
    QTimer autoStartTimer;
    QTimer autoStopTimer;
    QString record_dither;
    QString record_file;
    QSize record_outsize;
    QStringList record_args;
    int record_rate;
    int record_autotime;
    bool devicemode;
    QAction *recordAction;
    RecordingDialog *recdlg;

    void senseImageMagick();
    void senseFfmpeg();
    QWidget *ffmpegHelpWindow;
    bool ffmpegAvailable;
    bool convertAvailable;

    QAction *rotateAction;
    QActionGroup *orientation;
    QAction *showWarningsWindow;

    QString m_script;
    ScriptOptions m_scriptOptions;
    QDeclarativeTester *tester;

    NetworkAccessManagerFactory *namFactory;

    bool useQmlFileBrowser;

    QTranslator *translator;
    void loadTranslationFile(const QString& directory);

    void loadDummyDataFiles(const QString& directory);
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QDeclarativeViewer::ScriptOptions)

QT_END_NAMESPACE

#endif
