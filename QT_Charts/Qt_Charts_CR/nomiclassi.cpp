#include "nomiclassi.h"

QString nomiClassi::ClasseLine = "line";
QString nomiClassi::ClasseCandleStick = "CandleStick";
QString nomiClassi::ClassePie = "pie";
QString nomiClassi::ClasseBar = "bar";

QString nomiClassi::getLineClass(){
    return ClasseLine;
}

QString nomiClassi::getCandleStickClass(){
    return ClasseCandleStick;
}

QString nomiClassi::getPieClass(){
    return ClassePie;
}

QString nomiClassi::getBarClass(){
    return ClasseBar;
}
