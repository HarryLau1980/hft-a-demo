#include "strategyanalysismod.h"
#include "../DataAccInterf/dataaccif.h"
#include "hftmath.h"
#include "../utils/diagnosis.h"
#include "./strategy/deltastgworker.h"
#include <QDebug>


void SAMInit()
{
   RedirectLog2Files();
}


void SAMUnInit()
{

}

void SAMStrategyIvy()
{
}

void SAMTestIndexSynchronizer()
{
    //const char* args[] = {"1a", "2b", "3c", "4dd"};
    //QString res = DAIFormatQuery("%1, %2, %3, %4", args, 4);

    quint32 codes[] = {1505, 1506};
    DeltaStgWorker* worker = new DeltaStgWorker(codes, sizeof(codes)/sizeof(int), 1);
    worker->create();
    worker->start();
}
