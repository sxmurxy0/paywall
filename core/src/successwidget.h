#ifndef SUCCESSWIDGET_H
#define SUCCESSWIDGET_H

#include <QWidget>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
    class SuccessWidget;
}
QT_END_NAMESPACE

const QString RANDOM_CHARS = "qertyuiopasdfghjklzxcvbnm";

class SuccessWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::SuccessWidget *ui;
    QTimer *timer;
    QString text;
    int currentIndex;
    int counter;

public:
    SuccessWidget(QWidget *parent = nullptr);
    ~SuccessWidget();

    void startAnimation();

private slots:
    void animationStep();
};

#endif // SUCCESSWIDGET_H