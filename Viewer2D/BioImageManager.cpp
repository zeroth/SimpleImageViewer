#include "BioImageManager.h"
//#include "Anisotropy.h"
#include <QDir>
#include <QDataStream>
#include <QDebug>
#include <QUuid>
#include <QFileInfo>

#define RIMGID "aniR"
#define TIMGID "aniT"

void BioImageManager::createImage(const QString& path)
{
//    zeroth::BioImage *i = new zeroth::BioImage();
//    i->setSource(path);
//    setImage("a", i);

//    QString file_Path = path.toLocalFile();

    this->setImage(QFileInfo(path).baseName(), path);

}


void BioImageManager::saveImage(const QString& id,const QString &path)
{
    QString file;
    QUrl url(path);
    if(url.isValid() && url.scheme().compare("file", Qt::CaseInsensitive) == 0) {
        file = url.toLocalFile();
    } else{
        file = path;
    }

    m_images[id]->save(file);
}

void BioImageManager::setImage(const QString& id, zeroth::BioImage* img)
{

    m_images[id] = img;
    //    emit imageLoaded(id);
}

void BioImageManager::setImage(const QString &id, const QString &path)
{
    setImage(id, new zeroth::BioImage(path));
}

zeroth::BioImage *BioImageManager::image(const QString& id)
{
    return m_images[id];
}

QString BioImageManager::addImage(const QUrl &path) {

    QString file_Path = path.toLocalFile();
    QString uuid = QFileInfo(file_Path).baseName();
    if(m_images.contains(uuid)) {
        uuid = uuid + QString("(%1)").arg(QString::number(m_images.count()));
    }

    this->setImage(uuid, path.toLocalFile());
    return uuid;
}

float BioImageManager::intensity(const QString& id, qreal x, qreal y, qreal z)
{
    if(m_images.isEmpty()) return 0;

    return m_images[id]->intensityAt(x, y, z);
}

double BioImageManager::imgMin(const QString& id)
{
    return m_images[id]->min();
}

double BioImageManager::imgMax(const QString& id)
{
    return m_images[id]->max();
}

int BioImageManager::imgWidth(const QString &id)
{
    return m_images[id]->width();
}

int BioImageManager::imgHeight(const QString &id)
{
    return m_images[id]->height();
}

int BioImageManager::imgLength(const QString &id)
{
    return m_images[id]->length();
}

//QList<qreal> BioImageManager::hist(const QString& id)
//{

//    return m_img[id]->histogram()->hist;
//}

//QList<int> BioImageManager::bins(const QString& id)
//{
//    return m_img[id]->histogram()->bins;
//}

//int BioImageManager::range(const QString& id)
//{
//    return m_img[id]->histogram()->range;
//}


/*void BioImageManager::exeAnisotropy(const QString &parallelBackGround, const QString &perpendicularBackground,
                                    const QString &parallel, const QString &perpendicular, double subtractVal)
{
    Anisotropy* ani = new Anisotropy;
    ani->setParallelBackground(parallelBackGround);
    ani->setPerpendicularBackground(perpendicularBackground);
    ani->setParallel(parallel);
    ani->setPerpendicular(perpendicular);
    ani->setSubtractVal(subtractVal);
    ani->apply();

    this->setImage(RIMGID, ani->getImageR());
    this->setImage(TIMGID, ani->getImageT());

    emit anisotropyReady(RIMGID, TIMGID);
}

void BioImageManager::saveAnisotropy(const QUrl &path, const QString &idR, const QString &idT)
{

    QDir dir(path.toLocalFile());

    QString metaFileName = QString("metadata.ani");
    QString rFileName = QString("%1_R.tif").arg(dir.dirName());
    QString tFileName = QString("%1_T.tif").arg(dir.dirName());

    QString metaFilePath = QString("%1%2%3").arg(path.toLocalFile()).arg(QDir::separator()).arg(metaFileName);
    QString rFilePath = QString("%1%2%3").arg(path.toLocalFile()).arg(QDir::separator()).arg(rFileName);
    QString tFilePath = QString("%1%2%3").arg(path.toLocalFile()).arg(QDir::separator()).arg(tFileName);

    // save R & T
    m_img[idR]->save(rFilePath.toStdString());
    m_img[idT]->save(tFilePath.toStdString());

    // create metaFile
    QFile metaFile(metaFilePath);
    if(!metaFile.open(QIODevice::WriteOnly)) {
        // TODO: add some warnning
        return;
    }
    QDataStream out(&metaFile);
    out << metaFileName;
    out << rFileName;
    out << tFileName;

    metaFile.close();
}

void BioImageManager::openAnisotropy(const QUrl &path)
{
    QString metaFilePath = path.toLocalFile();
    QString metaFileName;
    QString rFileName;
    QString tFileName;

    // open  metaFile
    QFile metaFile(metaFilePath);
    if(!metaFile.open(QIODevice::ReadOnly)) {
        // TODO: add some warnning
        return;
    }
    QDataStream in(&metaFile);
    in >> metaFileName;
    in >> rFileName;
    in >> tFileName;
    metaFile.close();

    QFileInfo info(metaFilePath);
    QString basePath = info.dir().absolutePath();
    if(metaFileName != info.fileName()){
        qWarning() << "Looks like the metafile name has changed";
    }

//    QString metaFilePath = QString("%1%2%3").arg(path.toLocalFile()).arg(QDir::separator()).arg(metaFileName);
    QString rFilePath = QString("%1%2%3").arg(basePath).arg(QDir::separator()).arg(rFileName);
    QString tFilePath = QString("%1%2%3").arg(basePath).arg(QDir::separator()).arg(tFileName);

    this->setImage(RIMGID, rFilePath);
    this->setImage(TIMGID, tFilePath);
    emit anisotropyReady(RIMGID, TIMGID);
}*/

QStringList BioImageManager::availableColorMaps()
{
    return cTable->availableColorMaps();
}

QVector<QRgb> BioImageManager::colorTable(const QString &name)
{
    return cTable->colorTable(name);
}

bool BioImageManager::hasImage()
{
    return (!m_images.isEmpty());
}


BioImageManager::BioImageManager(QObject *parent) : QObject(parent)
{
    float Alpha[2] = {1.0, 1.0};
    cTable = new ColorTable(Alpha, 256);
    std::map<FloatKey, rgb> jetMap;
    jetMap[{0}] = rgb(0, 0, 131);
    jetMap[{0.125}] = rgb(0, 60, 170);
    jetMap[{0.375}] = rgb(5, 255, 255);
    jetMap[{0.625}] = rgb(255, 255, 0);
    jetMap[{0.875}] = rgb(250, 0, 0);
    jetMap[{1}] = rgb(128, 0, 0);
    cTable->registerMap("Jet", jetMap);

    // fire from image j
    std::map<FloatKey, rgb> fireMap;

    //    fireMap[{0.        }]=rgb(0,  0,0    );
    fireMap[{0.    }]=rgb(0,  0,61 );
    fireMap[{0.034}]=rgb(1,   0,96 );
    fireMap[{0.068}]=rgb(25,  0,130);
    fireMap[{0.103}]=rgb(49,  0,165);
    fireMap[{0.137}]=rgb(73,  0,192);
    fireMap[{0.172}]=rgb(98,  0,220);
    fireMap[{0.206}]=rgb(122, 0,227);
    fireMap[{0.241}]=rgb(146, 0,210);
    fireMap[{0.275}]=rgb(162, 0,181);
    fireMap[{0.310}]=rgb(173, 0,151);
    fireMap[{0.344}]=rgb(184, 0,122);
    fireMap[{0.379}]=rgb(195, 0,93 );
    fireMap[{0.413}]=rgb(207, 14,64);
    fireMap[{0.448}]=rgb(217, 35,35);
    fireMap[{0.482}]=rgb(229, 57,5 );
    fireMap[{0.517}]=rgb(240, 79,0 );
    fireMap[{0.551}]=rgb(252, 101,0);
    fireMap[{0.586}]=rgb(255, 117,0);
    fireMap[{0.620}]=rgb(255, 133,0);
    fireMap[{0.655}]=rgb(255, 147,0);
    fireMap[{0.689}]=rgb(255, 161,0);
    fireMap[{0.724}]=rgb(255, 175,0);
    fireMap[{0.758}]=rgb(255, 190,0);
    fireMap[{0.793}]=rgb(255, 205,0);
    fireMap[{0.827}]=rgb(255, 219,0);
    fireMap[{0.862}]=rgb(255, 234,0);
    fireMap[{0.896}]=rgb(255, 248,35 );
    fireMap[{0.931}]=rgb(255, 255,98 );
    fireMap[{0.965}]=rgb(255, 255,160);
    fireMap[{1.    }]=rgb(255,255,223);
    //    fireMap[{1.0}]=rgb(255,255,255);
    cTable->registerMap("Fire", fireMap);

    std::map<FloatKey, rgb> hsvMap;
    hsvMap[{0}] = rgb(255, 0, 0);
    hsvMap[{0.169}] = rgb(253, 255, 2);
    hsvMap[{0.173}] = rgb(247, 255, 2);
    hsvMap[{0.337}] = rgb(0, 252, 4);
    hsvMap[{0.341}] = rgb(0, 252, 10);
    hsvMap[{0.506}] = rgb(1, 249, 255);
    hsvMap[{0.671}] = rgb(2, 0, 253);
    hsvMap[{0.675}] = rgb(8, 0, 253);
    hsvMap[{0.839}] = rgb(255, 0, 251);
    hsvMap[{0.843}] = rgb(255, 0, 245);
    hsvMap[{1}] = rgb(255, 0, 6);
    cTable->registerMap("Hsv", hsvMap);

    std::map<FloatKey, rgb> hotMap;
    hotMap[{0}] = rgb(0, 0, 0);
    hotMap[{0.3}] = rgb(230, 0, 0);
    hotMap[{0.6}] = rgb(255, 210, 0);
    hotMap[{1}] = rgb(255, 255, 255);
    cTable->registerMap("Hot", hotMap);

    std::map<FloatKey, rgb> coolMap;
    coolMap[{0}] = rgb(125, 0, 179);
    coolMap[{0.13}] = rgb(116, 0, 218);
    coolMap[{0.25}] = rgb(98, 74, 237);
    coolMap[{0.38}] = rgb(68, 146, 231);
    coolMap[{0.5}] = rgb(0, 204, 197);
    coolMap[{0.63}] = rgb(0, 247, 146);
    coolMap[{0.75}] = rgb(0, 255, 88);
    coolMap[{0.88}] = rgb(40, 255, 8);
    coolMap[{1}] = rgb(147, 255, 0);
    cTable->registerMap("Cool", coolMap);

    std::map<FloatKey, rgb> springMap;
    springMap[{0}] = rgb(255, 0, 255);
    springMap[{1}] = rgb(255, 255, 0);
    cTable->registerMap("Spring", springMap);

    std::map<FloatKey, rgb> summerMap;
    summerMap[{0}] = rgb(0, 128, 102);
    summerMap[{1}] = rgb(255, 255, 102);
    cTable->registerMap("Summer", summerMap);

    std::map<FloatKey, rgb> autumnMap;
    autumnMap[{0}] = rgb(255, 0, 0);
    autumnMap[{1}] = rgb(255, 255, 0);
    cTable->registerMap("Autumn", autumnMap);

    std::map<FloatKey, rgb> winterMap;
    winterMap[{0}] = rgb(0, 0, 255);
    winterMap[{1}] = rgb(0, 255, 128);
    cTable->registerMap("Winter", winterMap);

    std::map<FloatKey, rgb> boneMap;
    boneMap[{0}] = rgb(0, 0, 0);
    boneMap[{0.376}] = rgb(84, 84, 116);
    boneMap[{0.753}] = rgb(169, 200, 200);
    boneMap[{1}] = rgb(255, 255, 255);
    cTable->registerMap("Bone", boneMap);

    std::map<FloatKey, rgb> copperMap;
    copperMap[{0}] = rgb(0, 0, 0);
    copperMap[{0.804}] = rgb(255, 160, 102);
    copperMap[{1}] = rgb(255, 199, 127);
    cTable->registerMap("Copper", copperMap);

    std::map<FloatKey, rgb> greysMap;
    greysMap[{0}] = rgb(0, 0, 0);
    greysMap[{1}] = rgb(255, 255, 255);
    cTable->registerMap("Greys", greysMap);

    std::map<FloatKey, rgb> yignbuMap;
    yignbuMap[{0}] = rgb(8, 29, 88);
    yignbuMap[{0.125}] = rgb(37, 52, 148);
    yignbuMap[{0.25}] = rgb(34, 94, 168);
    yignbuMap[{0.375}] = rgb(29, 145, 192);
    yignbuMap[{0.5}] = rgb(65, 182, 196);
    yignbuMap[{0.625}] = rgb(127, 205, 187);
    yignbuMap[{0.75}] = rgb(199, 233, 180);
    yignbuMap[{0.875}] = rgb(237, 248, 217);
    yignbuMap[{1}] = rgb(255, 255, 217);
    cTable->registerMap("Yignbu", yignbuMap);

    std::map<FloatKey, rgb> greensMap;
    greensMap[{0}] = rgb(0, 68, 27);
    greensMap[{0.125}] = rgb(0, 109, 44);
    greensMap[{0.25}] = rgb(35, 139, 69);
    greensMap[{0.375}] = rgb(65, 171, 93);
    greensMap[{0.5}] = rgb(116, 196, 118);
    greensMap[{0.625}] = rgb(161, 217, 155);
    greensMap[{0.75}] = rgb(199, 233, 192);
    greensMap[{0.875}] = rgb(229, 245, 224);
    greensMap[{1}] = rgb(247, 252, 245);
    cTable->registerMap("Greens", greensMap);

    std::map<FloatKey, rgb> yiorrdMap;
    yiorrdMap[{0}] = rgb(128, 0, 38);
    yiorrdMap[{0.125}] = rgb(189, 0, 38);
    yiorrdMap[{0.25}] = rgb(227, 26, 28);
    yiorrdMap[{0.375}] = rgb(252, 78, 42);
    yiorrdMap[{0.5}] = rgb(253, 141, 60);
    yiorrdMap[{0.625}] = rgb(254, 178, 76);
    yiorrdMap[{0.75}] = rgb(254, 217, 118);
    yiorrdMap[{0.875}] = rgb(255, 237, 160);
    yiorrdMap[{1}] = rgb(255, 255, 204);
    cTable->registerMap("Yiorrd", yiorrdMap);

    std::map<FloatKey, rgb> blueredMap;
    blueredMap[{0}] = rgb(0, 0, 255);
    blueredMap[{1}] = rgb(255, 0, 0);
    cTable->registerMap("Bluered", blueredMap);

    std::map<FloatKey, rgb> rdbuMap;
    rdbuMap[{0}] = rgb(5, 10, 172);
    rdbuMap[{0.35}] = rgb(106, 137, 247);
    rdbuMap[{0.5}] = rgb(190, 190, 190);
    rdbuMap[{0.6}] = rgb(220, 170, 132);
    rdbuMap[{0.7}] = rgb(230, 145, 90);
    rdbuMap[{1}] = rgb(178, 10, 28);
    cTable->registerMap("Rdbu", rdbuMap);

    std::map<FloatKey, rgb> picnicMap;
    picnicMap[{0}] = rgb(0, 0, 255);
    picnicMap[{0.1}] = rgb(51, 153, 255);
    picnicMap[{0.2}] = rgb(102, 204, 255);
    picnicMap[{0.3}] = rgb(153, 204, 255);
    picnicMap[{0.4}] = rgb(204, 204, 255);
    picnicMap[{0.5}] = rgb(255, 255, 255);
    picnicMap[{0.6}] = rgb(255, 204, 255);
    picnicMap[{0.7}] = rgb(255, 153, 255);
    picnicMap[{0.8}] = rgb(255, 102, 204);
    picnicMap[{0.9}] = rgb(255, 102, 102);
    picnicMap[{1}] = rgb(255, 0, 0);
    cTable->registerMap("Picnic", picnicMap);

    std::map<FloatKey, rgb> rainbowMap;
    rainbowMap[{0}] = rgb(150, 0, 90);
    rainbowMap[{0.125}] = rgb(0, 0, 200);
    rainbowMap[{0.25}] = rgb(0, 25, 255);
    rainbowMap[{0.375}] = rgb(0, 152, 255);
    rainbowMap[{0.5}] = rgb(44, 255, 150);
    rainbowMap[{0.625}] = rgb(151, 255, 0);
    rainbowMap[{0.75}] = rgb(255, 234, 0);
    rainbowMap[{0.875}] = rgb(255, 111, 0);
    rainbowMap[{1}] = rgb(255, 0, 0);
    cTable->registerMap("Rainbow", rainbowMap);

    std::map<FloatKey, rgb> portlandMap;
    portlandMap[{0}] = rgb(12, 51, 131);
    portlandMap[{0.25}] = rgb(10, 136, 186);
    portlandMap[{0.5}] = rgb(242, 211, 56);
    portlandMap[{0.75}] = rgb(242, 143, 56);
    portlandMap[{1}] = rgb(217, 30, 30);
    cTable->registerMap("Portland", portlandMap);

    std::map<FloatKey, rgb> blackbodyMap;
    blackbodyMap[{0}] = rgb(0, 0, 0);
    blackbodyMap[{0.2}] = rgb(230, 0, 0);
    blackbodyMap[{0.4}] = rgb(230, 210, 0);
    blackbodyMap[{0.7}] = rgb(255, 255, 255);
    blackbodyMap[{1}] = rgb(160, 200, 255);
    cTable->registerMap("Blackbody", blackbodyMap);

    std::map<FloatKey, rgb> earthMap;
    earthMap[{0}] = rgb(0, 0, 130);
    earthMap[{0.1}] = rgb(0, 180, 180);
    earthMap[{0.2}] = rgb(40, 210, 40);
    earthMap[{0.4}] = rgb(230, 230, 50);
    earthMap[{0.6}] = rgb(120, 70, 20);
    earthMap[{1}] = rgb(255, 255, 255);
    cTable->registerMap("Earth", earthMap);

    std::map<FloatKey, rgb> electricMap;
    electricMap[{0}] = rgb(0, 0, 0);
    electricMap[{0.15}] = rgb(30, 0, 100);
    electricMap[{0.4}] = rgb(120, 0, 100);
    electricMap[{0.6}] = rgb(160, 90, 0);
    electricMap[{0.8}] = rgb(230, 200, 0);
    electricMap[{1}] = rgb(255, 250, 220);
    cTable->registerMap("Electric", electricMap);

    std::map<FloatKey, rgb> alphaMap;
    alphaMap[{0}] = rgb(255, 255, 255);
    alphaMap[{1}] = rgb(255, 255, 255);
    cTable->registerMap("Alpha", alphaMap);

    std::map<FloatKey, rgb> viridisMap;
    viridisMap[{0}] = rgb(68, 1, 84);
    viridisMap[{0.13}] = rgb(71, 44, 122);
    viridisMap[{0.25}] = rgb(59, 81, 139);
    viridisMap[{0.38}] = rgb(44, 113, 142);
    viridisMap[{0.5}] = rgb(33, 144, 141);
    viridisMap[{0.63}] = rgb(39, 173, 129);
    viridisMap[{0.75}] = rgb(92, 200, 99);
    viridisMap[{0.88}] = rgb(170, 220, 50);
    viridisMap[{1}] = rgb(253, 231, 37);
    cTable->registerMap("Viridis", viridisMap);

    std::map<FloatKey, rgb> infernoMap;
    infernoMap[{0}] = rgb(0, 0, 4);
    infernoMap[{0.13}] = rgb(31, 12, 72);
    infernoMap[{0.25}] = rgb(85, 15, 109);
    infernoMap[{0.38}] = rgb(136, 34, 106);
    infernoMap[{0.5}] = rgb(186, 54, 85);
    infernoMap[{0.63}] = rgb(227, 89, 51);
    infernoMap[{0.75}] = rgb(249, 140, 10);
    infernoMap[{0.88}] = rgb(249, 201, 50);
    infernoMap[{1}] = rgb(252, 255, 164);
    cTable->registerMap("Inferno", infernoMap);

    std::map<FloatKey, rgb> magmaMap;
    magmaMap[{0}] = rgb(0, 0, 4);
    magmaMap[{0.13}] = rgb(28, 16, 68);
    magmaMap[{0.25}] = rgb(79, 18, 123);
    magmaMap[{0.38}] = rgb(129, 37, 129);
    magmaMap[{0.5}] = rgb(181, 54, 122);
    magmaMap[{0.63}] = rgb(229, 80, 100);
    magmaMap[{0.75}] = rgb(251, 135, 97);
    magmaMap[{0.88}] = rgb(254, 194, 135);
    magmaMap[{1}] = rgb(252, 253, 191);
    cTable->registerMap("Magma", magmaMap);

    std::map<FloatKey, rgb> plasmaMap;
    plasmaMap[{0}] = rgb(13, 8, 135);
    plasmaMap[{0.13}] = rgb(75, 3, 161);
    plasmaMap[{0.25}] = rgb(125, 3, 168);
    plasmaMap[{0.38}] = rgb(168, 34, 150);
    plasmaMap[{0.5}] = rgb(203, 70, 121);
    plasmaMap[{0.63}] = rgb(229, 107, 93);
    plasmaMap[{0.75}] = rgb(248, 148, 65);
    plasmaMap[{0.88}] = rgb(253, 195, 40);
    plasmaMap[{1}] = rgb(240, 249, 33);
    cTable->registerMap("Plasma", plasmaMap);

    std::map<FloatKey, rgb> warmMap;
    warmMap[{0}] = rgb(125, 0, 179);
    warmMap[{0.13}] = rgb(172, 0, 187);
    warmMap[{0.25}] = rgb(219, 0, 170);
    warmMap[{0.38}] = rgb(255, 0, 130);
    warmMap[{0.5}] = rgb(255, 63, 74);
    warmMap[{0.63}] = rgb(255, 123, 0);
    warmMap[{0.75}] = rgb(234, 176, 0);
    warmMap[{0.88}] = rgb(190, 228, 0);
    warmMap[{1}] = rgb(147, 255, 0);
    cTable->registerMap("Warm", warmMap);

    std::map<FloatKey, rgb> rainbow_softMap;
    rainbow_softMap[{0}] = rgb(125, 0, 179);
    rainbow_softMap[{0.1}] = rgb(199, 0, 180);
    rainbow_softMap[{0.2}] = rgb(255, 0, 121);
    rainbow_softMap[{0.3}] = rgb(255, 108, 0);
    rainbow_softMap[{0.4}] = rgb(222, 194, 0);
    rainbow_softMap[{0.5}] = rgb(150, 255, 0);
    rainbow_softMap[{0.6}] = rgb(0, 255, 55);
    rainbow_softMap[{0.7}] = rgb(0, 246, 150);
    rainbow_softMap[{0.8}] = rgb(50, 167, 222);
    rainbow_softMap[{0.9}] = rgb(103, 51, 235);
    rainbow_softMap[{1}] = rgb(124, 0, 186);
    cTable->registerMap("Rainbow-soft", rainbow_softMap);

    std::map<FloatKey, rgb> bathymetryMap;
    bathymetryMap[{0}] = rgb(40, 26, 44);
    bathymetryMap[{0.13}] = rgb(59, 49, 90);
    bathymetryMap[{0.25}] = rgb(64, 76, 139);
    bathymetryMap[{0.38}] = rgb(63, 110, 151);
    bathymetryMap[{0.5}] = rgb(72, 142, 158);
    bathymetryMap[{0.63}] = rgb(85, 174, 163);
    bathymetryMap[{0.75}] = rgb(120, 206, 163);
    bathymetryMap[{0.88}] = rgb(187, 230, 172);
    bathymetryMap[{1}] = rgb(253, 254, 204);
    cTable->registerMap("Bathymetry", bathymetryMap);

    std::map<FloatKey, rgb> cdomMap;
    cdomMap[{0}] = rgb(47, 15, 62);
    cdomMap[{0.13}] = rgb(87, 23, 86);
    cdomMap[{0.25}] = rgb(130, 28, 99);
    cdomMap[{0.38}] = rgb(171, 41, 96);
    cdomMap[{0.5}] = rgb(206, 67, 86);
    cdomMap[{0.63}] = rgb(230, 106, 84);
    cdomMap[{0.75}] = rgb(242, 149, 103);
    cdomMap[{0.88}] = rgb(249, 193, 135);
    cdomMap[{1}] = rgb(254, 237, 176);
    cTable->registerMap("Cdom", cdomMap);

    std::map<FloatKey, rgb> chlorophyllMap;
    chlorophyllMap[{0}] = rgb(18, 36, 20);
    chlorophyllMap[{0.13}] = rgb(25, 63, 41);
    chlorophyllMap[{0.25}] = rgb(24, 91, 59);
    chlorophyllMap[{0.38}] = rgb(13, 119, 72);
    chlorophyllMap[{0.5}] = rgb(18, 148, 80);
    chlorophyllMap[{0.63}] = rgb(80, 173, 89);
    chlorophyllMap[{0.75}] = rgb(132, 196, 122);
    chlorophyllMap[{0.88}] = rgb(175, 221, 162);
    chlorophyllMap[{1}] = rgb(215, 249, 208);
    cTable->registerMap("Chlorophyll", chlorophyllMap);

    std::map<FloatKey, rgb> densityMap;
    densityMap[{0}] = rgb(54, 14, 36);
    densityMap[{0.13}] = rgb(89, 23, 80);
    densityMap[{0.25}] = rgb(110, 45, 132);
    densityMap[{0.38}] = rgb(120, 77, 178);
    densityMap[{0.5}] = rgb(120, 113, 213);
    densityMap[{0.63}] = rgb(115, 151, 228);
    densityMap[{0.75}] = rgb(134, 185, 227);
    densityMap[{0.88}] = rgb(177, 214, 227);
    densityMap[{1}] = rgb(230, 241, 241);
    cTable->registerMap("Density", densityMap);

    std::map<FloatKey, rgb> freesurface_blueMap;
    freesurface_blueMap[{0}] = rgb(30, 4, 110);
    freesurface_blueMap[{0.13}] = rgb(47, 14, 176);
    freesurface_blueMap[{0.25}] = rgb(41, 45, 236);
    freesurface_blueMap[{0.38}] = rgb(25, 99, 212);
    freesurface_blueMap[{0.5}] = rgb(68, 131, 200);
    freesurface_blueMap[{0.63}] = rgb(114, 156, 197);
    freesurface_blueMap[{0.75}] = rgb(157, 181, 203);
    freesurface_blueMap[{0.88}] = rgb(200, 208, 216);
    freesurface_blueMap[{1}] = rgb(241, 237, 236);
    cTable->registerMap("Freesurface-blue", freesurface_blueMap);

    std::map<FloatKey, rgb> freesurface_redMap;
    freesurface_redMap[{0}] = rgb(60, 9, 18);
    freesurface_redMap[{0.13}] = rgb(100, 17, 27);
    freesurface_redMap[{0.25}] = rgb(142, 20, 29);
    freesurface_redMap[{0.38}] = rgb(177, 43, 27);
    freesurface_redMap[{0.5}] = rgb(192, 87, 63);
    freesurface_redMap[{0.63}] = rgb(205, 125, 105);
    freesurface_redMap[{0.75}] = rgb(216, 162, 148);
    freesurface_redMap[{0.88}] = rgb(227, 199, 193);
    freesurface_redMap[{1}] = rgb(241, 237, 236);
    cTable->registerMap("Freesurface-red", freesurface_redMap);

    std::map<FloatKey, rgb> oxygenMap;
    oxygenMap[{0}] = rgb(64, 5, 5);
    oxygenMap[{0.13}] = rgb(106, 6, 15);
    oxygenMap[{0.25}] = rgb(144, 26, 7);
    oxygenMap[{0.38}] = rgb(168, 64, 3);
    oxygenMap[{0.5}] = rgb(188, 100, 4);
    oxygenMap[{0.63}] = rgb(206, 136, 11);
    oxygenMap[{0.75}] = rgb(220, 174, 25);
    oxygenMap[{0.88}] = rgb(231, 215, 44);
    oxygenMap[{1}] = rgb(248, 254, 105);
    cTable->registerMap("Oxygen", oxygenMap);

    std::map<FloatKey, rgb> parMap;
    parMap[{0}] = rgb(51, 20, 24);
    parMap[{0.13}] = rgb(90, 32, 35);
    parMap[{0.25}] = rgb(129, 44, 34);
    parMap[{0.38}] = rgb(159, 68, 25);
    parMap[{0.5}] = rgb(182, 99, 19);
    parMap[{0.63}] = rgb(199, 134, 22);
    parMap[{0.75}] = rgb(212, 171, 35);
    parMap[{0.88}] = rgb(221, 210, 54);
    parMap[{1}] = rgb(225, 253, 75);
    cTable->registerMap("Par", parMap);

    std::map<FloatKey, rgb> phaseMap;
    phaseMap[{0}] = rgb(145, 105, 18);
    phaseMap[{0.13}] = rgb(184, 71, 38);
    phaseMap[{0.25}] = rgb(186, 58, 115);
    phaseMap[{0.38}] = rgb(160, 71, 185);
    phaseMap[{0.5}] = rgb(110, 97, 218);
    phaseMap[{0.63}] = rgb(50, 123, 164);
    phaseMap[{0.75}] = rgb(31, 131, 110);
    phaseMap[{0.88}] = rgb(77, 129, 34);
    phaseMap[{1}] = rgb(145, 105, 18);
    cTable->registerMap("Phase", phaseMap);

    std::map<FloatKey, rgb> salinityMap;
    salinityMap[{0}] = rgb(42, 24, 108);
    salinityMap[{0.13}] = rgb(33, 50, 162);
    salinityMap[{0.25}] = rgb(15, 90, 145);
    salinityMap[{0.38}] = rgb(40, 118, 137);
    salinityMap[{0.5}] = rgb(59, 146, 135);
    salinityMap[{0.63}] = rgb(79, 175, 126);
    salinityMap[{0.75}] = rgb(120, 203, 104);
    salinityMap[{0.88}] = rgb(193, 221, 100);
    salinityMap[{1}] = rgb(253, 239, 154);
    cTable->registerMap("Salinity", salinityMap);

    std::map<FloatKey, rgb> temperatureMap;
    temperatureMap[{0}] = rgb(4, 35, 51);
    temperatureMap[{0.13}] = rgb(23, 51, 122);
    temperatureMap[{0.25}] = rgb(85, 59, 157);
    temperatureMap[{0.38}] = rgb(129, 79, 143);
    temperatureMap[{0.5}] = rgb(175, 95, 130);
    temperatureMap[{0.63}] = rgb(222, 112, 101);
    temperatureMap[{0.75}] = rgb(249, 146, 66);
    temperatureMap[{0.88}] = rgb(249, 196, 65);
    temperatureMap[{1}] = rgb(232, 250, 91);
    cTable->registerMap("Temperature", temperatureMap);

    std::map<FloatKey, rgb> turbidityMap;
    turbidityMap[{0}] = rgb(34, 31, 27);
    turbidityMap[{0.13}] = rgb(65, 50, 41);
    turbidityMap[{0.25}] = rgb(98, 69, 52);
    turbidityMap[{0.38}] = rgb(131, 89, 57);
    turbidityMap[{0.5}] = rgb(161, 112, 59);
    turbidityMap[{0.63}] = rgb(185, 140, 66);
    turbidityMap[{0.75}] = rgb(202, 174, 88);
    turbidityMap[{0.88}] = rgb(216, 209, 126);
    turbidityMap[{1}] = rgb(233, 246, 171);
    cTable->registerMap("Turbidity", turbidityMap);

    std::map<FloatKey, rgb> velocity_blueMap;
    velocity_blueMap[{0}] = rgb(17, 32, 64);
    velocity_blueMap[{0.13}] = rgb(35, 52, 116);
    velocity_blueMap[{0.25}] = rgb(29, 81, 156);
    velocity_blueMap[{0.38}] = rgb(31, 113, 162);
    velocity_blueMap[{0.5}] = rgb(50, 144, 169);
    velocity_blueMap[{0.63}] = rgb(87, 173, 176);
    velocity_blueMap[{0.75}] = rgb(149, 196, 189);
    velocity_blueMap[{0.88}] = rgb(203, 221, 211);
    velocity_blueMap[{1}] = rgb(254, 251, 230);
    cTable->registerMap("Velocity-blue", velocity_blueMap);

    std::map<FloatKey, rgb> velocity_greenMap;
    velocity_greenMap[{0}] = rgb(23, 35, 19);
    velocity_greenMap[{0.13}] = rgb(24, 64, 38);
    velocity_greenMap[{0.25}] = rgb(11, 95, 45);
    velocity_greenMap[{0.38}] = rgb(39, 123, 35);
    velocity_greenMap[{0.5}] = rgb(95, 146, 12);
    velocity_greenMap[{0.63}] = rgb(152, 165, 18);
    velocity_greenMap[{0.75}] = rgb(201, 186, 69);
    velocity_greenMap[{0.88}] = rgb(233, 216, 137);
    velocity_greenMap[{1}] = rgb(255, 253, 205);
    cTable->registerMap("Velocity-green", velocity_greenMap);

    std::map<FloatKey, rgb> cubehelixMap;
    cubehelixMap[{0}] = rgb(0, 0, 0);
    cubehelixMap[{0.07}] = rgb(22, 5, 59);
    cubehelixMap[{0.13}] = rgb(60, 4, 105);
    cubehelixMap[{0.2}] = rgb(109, 1, 135);
    cubehelixMap[{0.27}] = rgb(161, 0, 147);
    cubehelixMap[{0.33}] = rgb(210, 2, 142);
    cubehelixMap[{0.4}] = rgb(251, 11, 123);
    cubehelixMap[{0.47}] = rgb(255, 29, 97);
    cubehelixMap[{0.53}] = rgb(255, 54, 69);
    cubehelixMap[{0.6}] = rgb(255, 85, 46);
    cubehelixMap[{0.67}] = rgb(255, 120, 34);
    cubehelixMap[{0.73}] = rgb(255, 157, 37);
    cubehelixMap[{0.8}] = rgb(241, 191, 57);
    cubehelixMap[{0.87}] = rgb(224, 220, 93);
    cubehelixMap[{0.93}] = rgb(218, 241, 142);
    cubehelixMap[{1}] = rgb(227, 253, 198);
    cTable->registerMap("Cubehelix", cubehelixMap);




}
