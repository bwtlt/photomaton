#ifndef GPIO_H
#define GPIO_H

#include <QObject>

static const int BUTTON0 = 14;
static const int BUTTON1 = 15;
static const int BUTTON2 = 18;
static const int BUTTON3 = 23;

class GPIO : public QObject
{
    Q_OBJECT
public:
    static GPIO& Instance();
    bool init();

    GPIO& operator= (const GPIO&) = delete;
    GPIO (const GPIO&) = delete;

private:
    GPIO();

    static void okInterrupt (void);
    static void cancelInterrupt (void);
    static void leftInterrupt (void);
    static void rightInterrupt (void);

signals:
    void okBtnPressed();
    void cancelBtnPressed();
    void leftBtnPressed();
    void rightBtnPressed();
};

#endif // GPIO_H
