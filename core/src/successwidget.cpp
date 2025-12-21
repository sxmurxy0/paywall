#include <QRandomGenerator>
#include "successwidget.h"
#include "ui/ui_successwidget.h"

SuccessWidget::SuccessWidget(QWidget *parent) : 
    QWidget(parent),
    ui(new Ui::SuccessWidget),
    timer(new QTimer(this)),
    text("Welcome to the wall!"),
    currentIndex(0),
    counter(0)
{
    ui->setupUi(this);
    
    timer->setInterval(40);
    connect(timer, &QTimer::timeout, this, &SuccessWidget::animationStep);
}

SuccessWidget::~SuccessWidget()
{
    delete ui;
}

void SuccessWidget::startAnimation()
{
    timer->stop();
    currentIndex = 0;
    counter = 0;

    timer->start();
}

void SuccessWidget::animationStep()
{
    counter++;

    QString currentText = text.left(currentIndex);
    for (int i = currentIndex; i < text.size(); i++) {
        int randomIndex = QRandomGenerator::global()->bounded(RANDOM_CHARS.length());
        currentText += RANDOM_CHARS[randomIndex];
    }
    
    ui->label->setText(currentText);

    if (currentIndex == text.size()) {
        timer->stop();
    }

    if (counter % 3 == 0) {
        currentIndex++;
    }
}
