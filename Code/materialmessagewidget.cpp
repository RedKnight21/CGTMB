#include "materialmessagewidget.h"

MaterialMessageWidget::MaterialMessageWidget(QWidget *parent, bool readFlag, bool saveFlag,
                                             bool clrFlag, bool clsFlag)
  : TextViewWindow(parent, readFlag, saveFlag, clrFlag, clsFlag)
{
    this->setMinimumWidth(300);
    this->setMinimumHeight(200);

}
// ----------------------------------------------------------------------------
MaterialMessageWidget::~MaterialMessageWidget()
{
    writeSettings();
}
// ----------------------------------------------------------------------------
void MaterialMessageWidget::saveFile()
{

}
// ----------------------------------------------------------------------------
QSize MaterialMessageWidget::sizeHint() const
{
    return QSize(300, 100);
}
// ----------------------------------------------------------------------------
void MaterialMessageWidget::writeSettings()
{
    //qDebug() << "\nCardImageView::writeSettings\n";
//    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
//    settings.setValue("cardImageWindow/size",     this->size());
//    settings.setValue("cardImageWindow/Geometry", this->saveGeometry());
//    qDebug() << "size: " << this->size();
//    qDebug() << "geom: " << this->saveGeometry();
}
// ----------------------------------------------------------------------------
void MaterialMessageWidget::readSettings()
{

}
void MaterialMessageWidget::clearText()
{

}
