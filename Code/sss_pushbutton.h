#ifndef SSS_PushButton_H
#define SSS_PushButton_H

#include <QtWidgets>

class SSS_PushButton : public QPushButton
{
    Q_OBJECT
public:
    SSS_PushButton(QWidget *parent = 0);

    int val1() const;
    int val2() const;

public slots:
    void setVal1(int vv);
    void setVal2(int vv);
    
    void setButton(SSS_PushButton *b);

private slots:
    void onPressed();
    void onReleased();

signals:
    void released(int val1, int val2);
    void pressed(int val1, int val2, SSS_PushButton *b);
    
private:
    int v1, v2;
    SSS_PushButton *b;
};
#endif // SSS_PushButton_H
