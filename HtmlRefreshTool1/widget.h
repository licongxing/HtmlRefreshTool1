#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
signals:

public slots:
    void beginRefresh();
    void stopRefresh();

private:
    Ui::Widget *ui;
    QTimer* myTimer;
    void refresh(HWND win);
};

#endif // WIDGET_H
