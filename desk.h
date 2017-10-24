#ifndef DESK_H
#define DESK_H

#include <QMainWindow>

namespace Ui {
class desk;
}

class desk : public QMainWindow
{
    Q_OBJECT

public:
    explicit desk(QWidget *parent = 0);
    ~desk();

private:
    Ui::desk *ui;
};

#endif // DESK_H
