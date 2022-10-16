#ifndef CUTAWAYWIDGET_H
#define CUTAWAYWIDGET_H

#include <QObject>
#include <QWidget>
#include <QSlider>
#include <QGroupBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QDialog>
#include "sss_guiutility.h"
#include "bounds.h"



class IncrementChangeDialog : public QDialog
{
    Q_OBJECT

public:
    IncrementChangeDialog();
    void setValue(double val);

signals:
    void incrementValue(double value);

private:
    QPushButton *acceptButton;
    QPushButton *cancelButton;
    QLineEdit   *incrementLineEdit;

private slots:
    void handleAcceptButton();

};


class CutAwayWidget  : public QWidget
{
    Q_OBJECT

public:
    CutAwayWidget(Bounds *inBounds);
    void resetBounds();
    void resetForProject();

signals:
  void      xCutawayChanged(double plane, int direction);
  void      yCutawayChanged(double plane, int direction);
  void      zCutawayChanged(double plane, int direction);

  void      xAxisCullDirChange(int value);
  void      yAxisCullDirChange(int value);
  void      zAxisCullDirChange(int value);

private slots:
  void handleXAxisSpinBox(double value);
  void handleYAxisSpinBox(double value);
  void handleZAxisSpinBox(double value);

  void setNoXRB();
  void setNegativeXRB();
  void setPositiveXRB();
  void setXPlaneValues(int value);
  void setXReverse(int value);

  void setNoYRB();
  void setNegativeYRB();
  void setPositiveYRB();
  void setYPlaneValues(int value);
  void setYReverse(int value);

  void setNoZRB();
  void setNegativeZRB();
  void setPositiveZRB();
  void setZPlaneValues(int value);
  void setZReverse(int value);

  void handleNewIncrement(double value);

protected:
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;


private:

  void setupModel();
  void setupSlots();
  void setXAxisSpinBox();
  void setYAxisSpinBox();
  void setZAxisSpinBox();
  void launchIncrementDialog();
  QSlider *createAxisSlider();

  QGroupBox       *cullSelectionGroupBox;
  QButtonGroup    *cullSelectionButtonGroup;
  QRadioButton    *bothFrontBack;
  QRadioButton    *frontOnly;
  QRadioButton    *backOnly;

  QGroupBox       *xAxisSelectionGroupBox;
  QButtonGroup    *xAxisSelectionButtonGroup;
  QDoubleSpinBox  *xAxisDoubleSpinBox;
  QRadioButton    *negativeX;
  QRadioButton    *noX;
  QRadioButton    *positiveX;
  QSlider         *xSlider;
  QCheckBox       *xReverse;

  QGroupBox       *yAxisSelectionGroupBox;
  QButtonGroup    *yAxisSelectionButtonGroup;
  QDoubleSpinBox  *yAxisDoubleSpinBox;
  QRadioButton    *negativeY;
  QRadioButton    *noY;
  QRadioButton    *positiveY;
  QSlider         *ySlider;
  QCheckBox       *yReverse;

  QGroupBox       *zAxisSelectionGroupBox;
  QButtonGroup    *zAxisSelectionButtonGroup;
  QDoubleSpinBox  *zAxisDoubleSpinBox;
  QRadioButton    *negativeZ;
  QRadioButton    *noZ;
  QRadioButton    *positiveZ;
  QSlider         *zSlider;
  QCheckBox       *zReverse;

  IncrementChangeDialog  *incrementDialog;

  bool    changesToApply;
  int     nInstances;
  int     currentDelRow;
  QString currentPath;

  short   xSliderValue;
  short   ySliderValue;
  short   zSliderValue;

  int       xAxisCullDirection;
  int       yAxisCullDirection;
  int       zAxisCullDirection;

  double    xPlaneLocation;
  double    yPlaneLocation;
  double    zPlaneLocation;

  double    xAxisCullFactor;
  double    yAxisCullFactor;
  double    zAxisCullFactor;
  double    spinBoxStepSize;

  Bounds *bounds;

  bool    isControlKey;

};

#endif // CUTAWAYWIDGET_H
