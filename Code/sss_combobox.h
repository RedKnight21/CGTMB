#ifndef SSS_COMBOBOX_H
#define SSS_COMBOBOX_H

#include <QtWidgets>

// ----------------------------------------------------------------------------

class SSS_ComboBox : public QComboBox
{
    Q_OBJECT
public:
    SSS_ComboBox(QWidget *parent = 0);

    int val1() const;
    int val2() const;

public slots:
    void setVal1(int vv);
    void setVal2(int vv);

    void setComboBox(SSS_ComboBox *cb);

private slots:
    void valuesChanged(int val);

signals:
    void currentIndexChanged(int val, int val1, int val2);

private:
    int v1, v2;
    SSS_ComboBox *cb;
};
// ----------------------------------------------------------------------------



#endif // SSS_COMBOBOX_H
