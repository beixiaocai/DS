#include "RunHelper.h"
#include "Utils/models.h"
#include "TaskFlow/mFcModels.h"
#include "TaskFlow/mTaskDeserialize.h"
#include <QDebug>

RunHelper::RunHelper(MTask *task,QObject *parent) : QObject(parent)
{
    this->task = task;

    m_taskdeser = new MTaskDeserialize(task->program,this);

    int fieldindex = 0;

    for (int i = 0; i < m_taskdeser->steps.length(); ++i) {
        MFlowStepParams *params = m_taskdeser->steps[i];
        if(MCONSTANT_FLOW_MENU_TYPE_ExtractBtn==params->menuType){
            // 提取字段控件参数
            MFlowStepParamsExtract *pe = static_cast<MFlowStepParamsExtract *>(params);
            // 预处理提取控件的字段数组
            QStringList carryFields;
            for (int j = 0; j < pe->pfs.length(); ++j) {
                MParamsField *pf = pe->pfs[j];

                fields.append(pf->fieldName);
                fieldsH.insert(pf->fieldName,fieldindex);

                fieldindex +=1;
                carryFields.append(QString("%1%2%3%4%5").arg(pf->fieldXpath).arg(SEPARATOR_WEBDATA_ATTR).
                                        arg(pf->fieldName).arg(SEPARATOR_WEBDATA_ATTR).arg(pf->fieldActionMethod));
            }
            pe->carryFields = carryFields.join(SEPARATOR_WEBDATA_ARRAY);
            carryFields.clear();

            extracts.append(pe);

        }else if (MCONSTANT_FLOW_MENU_TYPE_LoopBtn==params->menuType) {
            loops.append(static_cast<MFlowStepParamsLoop *>(params));
        }
        m_stepsH.insert(params->stepID,params);
    }
}
RunHelper::~RunHelper(){
    for (int i = 0; i < m_taskdeser->steps.length(); ++i) {
        delete m_taskdeser->steps[i];
    }

}

MFlowStepParams * RunHelper::getNextStepParams(QString lastStepID){

    MFlowStepParams *np= nullptr;

    int stepCount = m_taskdeser->steps.length();

    // 根据上一步的stepID，获取下一步的params
    if (stepCount > 0){
        if(lastStepID==""){
            np= m_taskdeser->steps[0];
        }else {
            for (int i = 0; i < stepCount; ++i) {
                MFlowStepParams *currentParams = m_taskdeser->steps[i];
                if(lastStepID==currentParams->stepID){
                    int next = i+1;
                    if(next<stepCount){
                        np = m_taskdeser->steps[next];
                    }
                }
            }
        }
    }
    lastStepID.clear();
    return np;
}
