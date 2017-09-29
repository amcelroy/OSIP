#ifndef SAVINGPIPELINE_IMPL_H
#define SAVINGPIPELINE_IMPL_H

#include "savingstage.h"

using namespace OSIP;

template<class I>
SavingStage<I>::SavingStage(){

}

template<class I>
void SavingStage<I>::postStage(){
    sig_SavingFinished();
}

#endif // SAVINGPIPELINE_IMPL_H
