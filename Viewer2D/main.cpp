#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
//#include "Bio2DImageView.h"
#include "BioImage.h"
//#include "Anisotropy.h"
#include "BioImageProvider.h"
#include "BioImageManager.h"
#include <QImage>
#include <QRgb>
#include <QApplication>
#include <QQuickView>
#include <QQmlDebuggingEnabler>
#include "ImageManager.h"
#include <Anisotropy.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QQmlDebuggingEnabler enabler;

    QApplication app(argc, argv);
    QApplication::setApplicationName("Anisotropy");
    QApplication::setOrganizationName("CellphyLab");
    QApplication::setOrganizationDomain("CellphyLab.com");

    qmlRegisterUncreatableType<zeroth::BioImage>("BioImages", 1, 0, "BioImage","You can not create BioImage in QML");
    qmlRegisterType<Anisotropy>("BioImages", 1, 0, "Anisotropy");
    //        qmlRegisterType<Bio2DImageView>("BioImages", 1,0, "BioImage2DViewer");
//    qmlRegisterType<Anisotropy>("BioImages", 1,0, "Anisotropy");
//    qmlRegisterType<zeroth::Histogram>("BioImages", 1,0, "Histogram");
//    qmlRegisterType<DataModel>("BioImages", 1, 0, "DataModel");

    QQmlApplicationEngine engine;
    engine.addImageProvider("Bi", new BioImageProvider);
    engine.rootContext()->setContextProperty("app", &app);
    engine.rootContext()->setContextProperty("Manager", &ImageManager::instance());
    engine.addImageProvider("Bi", new BioImageProvider);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

   /* QQuickView qmlView;
    qmlView.engine()->addImageProvider("Bi", new BioImageProvider);
    qmlView.engine()->rootContext()->setContextProperty("app", &app);
    qmlView.engine()->rootContext()->setContextProperty("qmlWindow", &qmlView);
//    qmlView.engine()->rootContext()->setContextProperty("Manager", &BioImageManager::instance());
        qmlView.engine()->rootContext()->setContextProperty("Manager", &ImageManager::instance());
//    qmlView.engine()->rootContext()->setContextProperty("tmpBioImage", new zeroth::BioImage());
    qmlView.setResizeMode(QQuickView::SizeRootObjectToView);
    qmlView.setSource(QUrl("qrc:/main.qml"));
    qmlView.setMinimumSize(QSize(1366, 768));
    qmlView.showMaximized();
    qmlView.show();*/

    return app.exec();

    //    img.save("C:/Code/test_tiff_2.tif");
}
