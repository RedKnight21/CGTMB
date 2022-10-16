#ifndef LICENSE_H
#define LICENSE_H

#include <QWidget>
#include <QDate>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include "simplecrypt.h"

class License
{
  public:
    License();

    void checkLicense(const QString progName, const QString checkVersion);
    void readLicenseFile();

    QStringList fields;


    short   days2Expire;
    short   numMoniker;
    QDate   expDate;
    QString version;
    QString macAddress;
    QString moniker;
    QString lType;
    bool    allowPtrack;
    bool    allowXsdir;
    bool    allowPnnlMats;
    bool    accepted;
    bool    beta;
    bool    doCg;
    bool    allowEmbedded;
    bool    devOption1;
    bool    devOption2;
    int     licenseExpire;
    int     secondaries;

    int myMonth;
    int myYear;
    int myDay;

  private:

    void decryptLicenseString(QString encryptedString);

};

#endif // LICENSE_H
