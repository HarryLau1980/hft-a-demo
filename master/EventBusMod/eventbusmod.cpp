#include "eventbusmod.h"
#include <QMap>
#include <QList>

typedef struct EBMSlot_t
{
    EBM_SLOT slot;
    void* arg;
} EBMSlot;

typedef struct EBMEMBSubscribter_t
{
    QList<EBMSlot> mSlots;
} EBMSubscribter;

typedef QMap<int, EBMSubscribter*> EBMPublisher;

EBMPublisher gEBMPublisher;

void EBMRegisterSubscriber(int evId, EBM_SLOT slot, void *arg)
{
    if(slot == NULL)
        return;

    EBMSlot s;
    s.slot = slot;
    s.arg = arg;

    EBMPublisher::iterator itr;
    itr = gEBMPublisher.find(evId);
    EBMSubscribter* subs = NULL;
    if( itr != gEBMPublisher.end()) {
        subs = itr.value();
    } else {
        subs = new EBMSubscribter;
        gEBMPublisher[evId] = subs;
    }
    subs->mSlots.append(s);
}


void EBMUnRegisterSubscriber(EBM_SLOT slot)
{
    EBMPublisher::iterator itr;
    EBMSubscribter* subs;
    bool removed = false;

    for( itr = gEBMPublisher.begin(); itr != gEBMPublisher.end(); itr ++ ) {
        subs = itr.value();
        removed = false;
        for( int i = 0; i < subs->mSlots.size(); i ++ ) {
            if(subs->mSlots[i].slot == slot) {
                subs->mSlots.removeAt(i);
                removed = true;
                break;
            }
        }
        if(removed) break;
    }
}


void EBMSignal(int evId, const QString &msg, int iParam)
{
    EBMPublisher::iterator itr;
    itr = gEBMPublisher.find(evId);
    if(itr == gEBMPublisher.end()) {
        return;
    }
    EBMSubscribter *subs = itr.value();
    for( int i = 0; i < subs->mSlots.size(); i ++ ) {
        EBMSlot& s = subs->mSlots[i];
        if(s.slot) {
            s.slot(evId, msg, iParam, s.arg);
        }
    }
}

