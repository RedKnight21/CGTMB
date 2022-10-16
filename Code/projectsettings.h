#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <QObject>
#include <QtWidgets>
#include "masterdata.h"
#include "license.h"

class ProjectSettings : public QDialog
{
    Q_OBJECT

public:
    ProjectSettings(MasterData *md, License *lic);
    ~ProjectSettings();

    void readSettings();
    void writeSettings();
    void setDefaults();
    void setValues();

    QSize sizeHint() const override;

signals:
    void projectSettingsUpdated(int);

private slots:
    void doClose();
    void doCancel();
    void handleProjectTitle(QString s);
    void handleDataLength(int val);
    void handleParticle(int val);
    void handleProjectType(int val);
    void handleCellComment(int val);
    void handleBodyComment(int val);
    void handleLineRuler(int val);
    void handleCellImportances(int val);
    void handleMtlComment(int val);

private:
    void buildLayout();
    void setupSlots();
    void saveValues();

    QComboBox *particle;
    QComboBox *projectType;

    QCheckBox *cellComment;
    QCheckBox *cellImportances;
    QCheckBox *bodyComment;
    QCheckBox *lineRuler;
    QCheckBox *mtlComment;

    QLabel    *particleLabel;
    QLineEdit *projectTitle;
    QSpinBox  *dataLineLength;

    QPushButton *cancelPB;
    QPushButton *okPB;

    MasterData *masterData;

    QVector<QString> projectNames;

    License *license;
    QString tmpProjectTitle;

};

#endif // PROJECTSETTINGS_H
