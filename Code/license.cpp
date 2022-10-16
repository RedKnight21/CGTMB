#include "license.h"

License::License()
{
    days2Expire        = 0;
    secondaries        = 0;
    moniker            = "";
    allowPtrack        = false;
    allowPnnlMats      = false;
    allowXsdir         = false;
    accepted           = false;
    doCg               = false;
    devOption1         = false;
    devOption2         = false;
    allowEmbedded      = true;
}
// ------------------------------------------------------------------------------------------------
void License::readLicenseFile()
{
    QChar sep = QDir::separator();
    QString fileName = QApplication::applicationDirPath () + sep + "cgtmb_0.6.0.lic";

    QFile LicFile(fileName);
    if (LicFile.open(QFile::ReadOnly))
    {
        QString LicString = QLatin1String(LicFile.readAll());
        LicFile.close();
        decryptLicenseString(LicString);
    }
    else
    {
        QString str1;
        //qDebug() << "License file: " << fileName << " not found";
        QMessageBox about;
        str1 += "\n\nLicense file not found.\n";
        str1 += fileName;
        about.setIcon(QMessageBox::Critical);
        about.setInformativeText(str1);
        about.setStandardButtons(QMessageBox::Ok);
        about.setDefaultButton(QMessageBox::Ok);
        about.show();
        about.exec();
        exit(0);
    }
}
// ----------------------------------------------------------------------------
void License::decryptLicenseString(QString encryptedString)
{
    QDate myDate = QDateTime::currentDateTime().date();
    int jDay1    = myDate.toJulianDay();

    SimpleCrypt crypto(Q_UINT64_C(0x6922f614bc1858a0));
    QString decrypted = crypto.decryptToString(encryptedString);

    //qDebug() << "decrypted string" << decrypted;

    fields = decrypted.split(' ', QString::SkipEmptyParts);
    int jDay2  = fields[1].toInt();
    macAddress = fields[2];

    //qDebug() << "decryptLicenseString:: fields size: " << fields.size();
    //for (int i=0; i< fields.size(); ++i)
    //{
    //    qDebug() << i << "\t field: " << fields[i];
    //}

    QString verField      = fields[0];
    QStringList verFields = verField.split('.', QString::SkipEmptyParts);
    version               = verFields[0] + "." + verFields[1] + "." + verFields[2];
    version               = version.mid(3,5);
    lType                 = verFields[3];

    moniker = fields[3];

    if (fields[4] == "MATNO")
        allowXsdir = false;
    else
        allowXsdir = true;

    if (fields[5] == "EMBNO")
        allowEmbedded = false;
    else
        allowEmbedded = true;

    if (fields[6] == "PTRNO")
        allowPtrack = false;
    else
        allowPtrack = true;

    //qDebug() << "license version: " << license.version;
    //qDebug() << "license type   : " << license.lType;
    //qDebug() << "node: " << fields[6];
    //qDebug() << "license beta   : " << license.beta << "\t fields[7] : " << fields[7];

    //qDebug() <<  "\t fields[8] : " << fields[8];

    myYear  = jDay2 / 10000;
    myMonth = (jDay2 - myYear * 10000) / 100;
    myDay   = (jDay2 - myYear * 10000) - myMonth * 100;

    expDate     = QDate(myYear, myMonth, myDay);
    jDay2       = (int) QDate(myYear, myMonth, myDay).toJulianDay();
    days2Expire = (short) (jDay2 - jDay1);

    for(int i=3; i < (int) fields.size(); ++i)
    {
        if (fields[i].startsWith("SEC"))
        {
            fields[i].replace(0, 3, "");
            secondaries = fields[i].toInt();
            break;
        }
    }
}
// ------------------------------------------------------------------------------------------------
void License::checkLicense(const QString progName, const QString checkVersion)
{
    // This function will exit the program under several conditions:
    //   1)  Incompatible license file
    //   2)  Licnese expired
    //   3)  Licnese file not found
    //
    //  Will show message box for license expiration reminder.  Otherwise, returns.
    //

    QString str1 = "TranzViz Version ";
    str1 += QApplication::applicationVersion();

    QString s2, s1;
    QString hostName = QSysInfo::machineHostName();
//    hostName.insert(0,"M");

    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    int i = (beta ==  true) ? 1:0;
    settings.setValue("defaults/beta", i);
    licenseExpire = settings.value("defaults/licenseExpire", 30).toInt();
    if (licenseExpire < 1) licenseExpire = 30;

    //    qDebug() << "Days to expire: " << days2Expire;
    //    qDebug() << "<" << QApplication::applicationVersion() << ">";
    //    qDebug() << "<" << version << ">";

    if (!version.contains(checkVersion))
    {
        QMessageBox about;
        about.setIcon(QMessageBox::Critical);
        about.setWindowTitle(progName);
        s1  = "\n\nYour license file is incompatible with this " + progName + " version. ";
        s1 += "License file is for version " + version;
        about.setInformativeText(s1);
        about.setStandardButtons(QMessageBox::Ok);
        about.setDefaultButton(QMessageBox::Ok);
        about.show();
        about.exec();
        exit(0);
    }

    //qDebug() << "licenseExpire:: " << licenseExpire;
    //qDebug() << "days2Expire  :: " << days2Expire;

    if ((days2Expire < licenseExpire) && (days2Expire > 0))
//    if ((days2Expire < 3) && (days2Expire > 0))
    {
        QMessageBox about;
        about.setIcon(QMessageBox::Warning);
        about.setWindowTitle(progName);
            //qDebug() << "about size: " << about.size();
            //about.setText(tr("\n"));
        s1 = "\n\nYour license will expire in ";
        s2.sprintf("%d", days2Expire);
        s1 += s2;
        s1 += " days.";
        about.setInformativeText(s1);
        about.setStandardButtons(QMessageBox::Ok);
        about.setDefaultButton(QMessageBox::Ok);
        about.show();
        about.exec();
    }
    else if (days2Expire < 0)
    {
        s1 = progName + " Version ";
        s1 += QApplication::applicationVersion();
        s1 += "\n\nYour license to use this product has expired.";

        QChar sep = QDir::separator();
        QString fileName = QApplication::applicationDirPath() + sep + progName.toLower() + ".lic";
        s1 += "\n\nSee: " + fileName;

        QMessageBox about;
        s1 += "\n\nYour license has expired!\n";
        about.setIcon(QMessageBox::Critical);
        about.setInformativeText(s1);
        about.setStandardButtons(QMessageBox::Ok);
        about.setDefaultButton(QMessageBox::Ok);
        about.show();
        about.exec();
        exit(0);
    }

    if (lType == "M")
    {
        if (moniker != hostName)
        {
                //qDebug() << "host names match";
            s1 = "TranzViz Version ";
            s1 += QApplication::applicationVersion();
            s1 += "\n\nYou are not authorized to use this product on this host: ";
            s1 += hostName;

            QMessageBox about;
            about.setIcon(QMessageBox::Critical);
            about.setInformativeText(s1);
            about.setStandardButtons(QMessageBox::Ok);
            about.setDefaultButton(QMessageBox::Ok);
            about.show();
            about.exec();
            exit(0);
        }
    }
}           // -- end checkLicense
