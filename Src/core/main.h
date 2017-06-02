//
// Author: Louis Meadows
// Copyright 2016 Positively Front Street, Inc
// Created January 25th 2016
//

#ifndef MAIN
#define MAIN
#include <QtGlobal>
#include <QGuiApplication>
#include <QUrl>
#include <QSysInfo>
#include <QScreen>
#include <QIcon>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QNetworkAccessManager>
#include <QDebug>
#include "proxypics.h"

//TODO - Unsed Variables, Please remove if it is unused
//ProxyPics                 * proxyPics;
//QQmlContext               * cntx;
//QSysInfo                  * systemInfo;
QQmlApplicationEngine     * engine;
QScreen * screen;
QUrl qml;
QObject *firstRootItem;
QByteArray checkdata;
QString checkString;
QIcon proxyPicsIcon;
#endif // MAIN

