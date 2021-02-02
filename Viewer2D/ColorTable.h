#ifndef COLORTABLE_H
#define COLORTABLE_H
#include <QMetaType>

#include <QMap>
#include <QRgb>
#include <QHash>
#include <iostream>



/*struct rgb {
    int r;
    int g;
    int b;
    rgb(int _r=0, int _g=0, int _b=0)
        :r(_r), g(_g), b(_b){

    }
    rgb()
        :r(0), g(0), b(0){

    }
};*/
struct FloatKey {
    double id;
    bool operator<(const FloatKey & o) const {
        return  (id < o.id);
    }
};

class rgb{
public:
    int r;
    int g;
    int b;
    rgb(int _r, int _g, int _b);

    rgb() = default;
    ~rgb() = default;
    rgb(const rgb &) = default;
    rgb &operator=(const rgb &) = default;
    bool operator<(const rgb &) const;

};

class rgba {
public:
    int r;
    int g;
    int b;
    int a;
    rgba() = default;
    ~rgba() = default;
    rgba(const rgba &) = default;
    rgba &operator=(const rgba &) = default;
    rgba(int _r=0, int _g=0, int _b=0, int _a=255);

};

class ColorTable
{
public:

    ColorTable(float alpha[2], int step =256);
    void registerMap(const QString& name, const std::map<FloatKey, rgb> &colorMap);
    QStringList availableColorMaps() const;
    QVector<QRgb> colorTable(const QString& name);

private:
    std::map<QString, std::map<FloatKey, rgb>> m_colorMapList;

    std::map<QString, QVector<QRgb>> m_colorTable;
    int nStep;
    float alpha[2];
    QVector<QRgb> graysColorTable;
};


#endif // COLORTABLE_H
