#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QObject>

class Processor : public QObject {
    Q_OBJECT

public slots:
    void doWork();
};

#endif // PROCESSOR_H
