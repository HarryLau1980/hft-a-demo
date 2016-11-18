#ifndef EVENTBUSMOD_H
#define EVENTBUSMOD_H

#include "eventbusmod_global.h"
#include <QString>

typedef int (*EBM_SLOT)(int evId, const QString& msg, int iParam, void* arg);

void EBM_EXPORT EBMRegisterSubscriber(int evId, EBM_SLOT slot, void* arg);
void EBM_EXPORT EBMUnRegisterSubscriber(EBM_SLOT slot);

void EBM_EXPORT EBMSignal(int evId, const QString& msg, int iParam);

#endif // EVENTBUSMOD_H
