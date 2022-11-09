#ifndef VIEW3DCONTROL_H
#define VIEW3DCONTROL_H

#include <QObject>
#include <QWidget>
#include "cutawaywidget.h"
#include "bounds.h"
#include <abaqusReadFile.h>
#include <masterdata.h>
#include <meshuniversebuild.h>

class View3DControl : public QWidget
{
    Q_OBJECT
public:
   View3DControl(MasterData *mdIn, QVector<ReadFileAbaqus> *abqFiles, int *m3Dangles);

//   QSize sizeHint() const override;
//   QSize minimumSizeHint() const override;

   void setBounds(Bounds b);
   void resetForProject();

signals:
   void      xCutawayChanged(double plane, int direction);
   void      yCutawayChanged(double plane, int direction);
   void      zCutawayChanged(double plane, int direction);

   void      xAxisCullDirChange(int value);
   void      yAxisCullDirChange(int value);
   void      zAxisCullDirChange(int value);

public slots:
   void handleXCutAwayChanged(double plane, int direction);
   void handleYCutAwayChanged(double plane, int direction);
   void handleZCutAwayChanged(double plane, int direction);

   void handleXAxisCullDirChange(int value);
   void handleYAxisCullDirChange(int value);
   void handleZAxisCullDirChange(int value);

private:
    void  buildTab2();
    void  setupSlots();

    CutAwayWidget       *cutawayWidget;
    MasterData          *md;

    QTabWidget          *tabWidget;
    QWidget             *tab1Widget;
    QWidget             *tab2Widget;
    QWidget             *tab3Widget;

    int                 tabIndex1;
    int                 tabIndex2;
    int                 tabIndex3;

    Bounds bounds;

};

#endif // VIEW3DCONTROL_H
