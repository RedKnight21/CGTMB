/*******************************************************************************
 **                                                                           **
 ** Copyright (C) 2022 Stag Sceintific Solutions, Inc.                        **
 ** Contact:	Roger Martz, President & CEO                                  **
 **			PO Box 427, Wadley, GA 30477                                      **
 **                                                                           **
 ********************************************************************************/

#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "QVTKRenderWidget.h"
#include "mainwindow.h"

extern int qInitResources_icons();

int main(int argc, char** argv)
{
    QStringList sl = QStyleFactory::keys();
    for(int i=0; i < sl.size(); ++i)
        qDebug() << sl[i];

	// needed to ensure appropriate OpenGL context is created for VTK rendering.
	QSurfaceFormat::setDefaultFormat(QVTKRenderWidget::defaultFormat());

	// QT Stuff
	QApplication app(argc, argv);

#ifdef USING_WINDOWS

    QApplication::setStyle("Windows");

#else

    QApplication::setStyle("motif");

#endif

    // -----------------------
    // -- Get the style sheet.
    QFile File(":/Resources/cgtmb_win.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    File.close();

	qInitResources_icons();

    MainWindow mainWindow;
    int desktopArea = QApplication::desktop()->width() * QApplication::desktop()->height();
    int widgetArea  = mainWindow.width() * mainWindow.height();

    if (((float)widgetArea / (float)desktopArea) < 0.75f)
        mainWindow.show();
    else
        mainWindow.showMaximized();

    app.setStyleSheet(StyleSheet);

	return app.exec();
}
