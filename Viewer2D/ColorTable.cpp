#include "ColorTable.h"
#include <QColor>
#include <QtMath>

float lerp(float v0, float v1, float t) {
    float r = v0*(1-t)+v1*t;
    return r;
}

ColorTable::ColorTable(float alphaVal[2], int step)
{
    this->nStep = step;
    this->alpha[0] = alphaVal[0];
    this->alpha[1] = alphaVal[1];
    QVector<QRgb> gColorTable (nStep);
    for (int i = 0; i<nStep; ++i) {

        gColorTable[i] = qRgb(i, i, i);
    }
    graysColorTable = gColorTable;
    m_colorTable["Grays"] = gColorTable;

}

void ColorTable::registerMap(const QString &name, const std::map<FloatKey, rgb> &colorMap)
{
    m_colorMapList[name] = colorMap;
    //"jet":[{"index":0,"rgb":[0,0,131]},{"index":0.125,"rgb":[0,60,170]},{"index":0.375,"rgb":[5,255,255]},{"index":0.625,"rgb":[255,255,0]},{"index":0.875,"rgb":[250,0,0]},{"index":1,"rgb":[128,0,0]}],


    QList<float> indicies;
    std::map<FloatKey, rgb>::const_iterator i = colorMap.cbegin();
    while (i != colorMap.cend()) {
        indicies.append(i->first.id*this->nStep);
        ++i;
    }

    // making sure alpha is between 0 and 1
    this->alpha[0] = qMin<float>( qMax<float>(this->alpha[0], 0), 1);
    this->alpha[1] = qMin<float>( qMax<float>(this->alpha[1], 0), 1);

    QList<rgba> steps;
    std::map<FloatKey, rgb>::const_iterator c = colorMap.cbegin();
    while (c != colorMap.cend()) {

        int avgAlpha = (this->alpha[0] + (this->alpha[1] - this->alpha[0])*c->first.id)*255; // calse it between 0-255

        steps.append(rgba(c->second.r, c->second.g, c->second.b, avgAlpha));
        ++c;
    }

    /*
     * map increasing linear values between indicies to
     * linear steps in colorvalues
     */
    QVector<QRgb> colors;
    for (int i = 0; i < indicies.size()-1; i++) {
        int scaleRange = qRound(indicies[i+1] - indicies[i]);
        rgba fromrgba = steps[i];
        rgba torgba = steps[i+1];

        for (int j = 0; j < scaleRange; j++) {
            float amt = (float)j / (float)scaleRange;
            int r = qRound(lerp(fromrgba.r, torgba.r, amt));
            int g = qRound(lerp(fromrgba.g, torgba.g, amt));
            int b = qRound(lerp(fromrgba.b, torgba.b, amt));
            colors.append(qRgba(r,g,b,255));
        }
    }
    rgb last = colorMap.rbegin()->second;

//    QRgb lastQRgb = qRgba(last.r, last.g, last.b, this->alpha[1]);
    QRgb lastQRgb = qRgb(last.r, last.g, last.b);

//    colors[this->nStep-1]= lastQRgb;
    colors.insert(this->nStep-1, lastQRgb);

    m_colorTable[name] = colors;
}

QStringList ColorTable::availableColorMaps() const
{
    QStringList keys;
    std::map<QString, QVector<QRgb>>::const_iterator  it = m_colorTable.cbegin();
    while (it != m_colorTable.cend()) {
        keys.append(it->first);
        ++it;
    }
    return keys;
}

QVector<QRgb> ColorTable::colorTable(const QString &name)
{
//    if(m_colorTable.contains)
    std::map<QString, QVector<QRgb>>::iterator  check = m_colorTable.find(name);
    if(check != m_colorTable.end()){
        return check->second;
    } else {
        return graysColorTable;
    }
//    return m_colorTable.value(name, graysColorTable);
}

///////////////////////////////////////////////////////
bool rgb::operator<(const rgb & ot) const
{
    return  (this->r < ot.r) && (this->g < ot.g) && (this->b < ot.b);
}

rgb::rgb(int _r, int _g, int _b)
    :r(_r), g(_g), b(_b){

}

rgba::rgba(int _r, int _g, int _b, int _a)
    :r(_r), g(_g), b(_b), a(_a){

}
