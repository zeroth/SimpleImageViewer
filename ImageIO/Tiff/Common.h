#ifndef COMMON_H
#define COMMON_H
#include <cmath>
namespace zeroth {
typedef enum DataType {
    UINT8,
    UINT16,
    UINT32,
    INT8,
    INT16,
    INT32,
    FLOAT
}DataType;

/*extern double range(double number, double oldMin, double oldMax, double newMin, double newMax) {

//        rmin denote the minimum of the range of your measurement
//        rmax denote the maximum of the range of your measurement
//        tmin denote the minimum of the range of your desired target scaling
//        tmax denote the maximum of the range of your desired target scaling
//        m ∈ [rmin,rmax] denote your measurement to be scaled

//        Then m = ((m− rmin)/ (rmax−rmin))×(tmax−tmin)+tmin

    return ( (number-oldMin)/(oldMax-oldMin) ) * (newMax - newMin) + newMin;
}

extern double rangeTo8(double number, double oldMin, double oldMax) {
    return range(number, oldMin, oldMax, 0, 255);
}

extern double rangeTo16(double number, double oldMin, double oldMax) {
    return range(number, oldMin, oldMax, 0, 65535);
}

extern double rangeTo32(double number, double oldMin, double oldMax) {
    return range(number, oldMin, oldMax, 0, std::pow(2,32)-1);
}*/


}
#endif // COMMON_H
