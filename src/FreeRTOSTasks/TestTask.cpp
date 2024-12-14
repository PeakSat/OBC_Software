#include "TestTask.hpp"
#include "task.h"
#include "EPS_Driver.hpp"

TestTask::TestTask() : Task("TestTask") {
    // LOG_INFO << "Initialised instance of TestTask";
}



void TestTask::execute() {
    vTaskDelay(pdMS_TO_TICKS(this->delayMs));
    LOG_INFO << "TestTask started";
    EPS eps;


    auto stat = eps.watchdogReset();
    stat = eps.getSystemStatus();

    if (stat) {
        if (stat.value() == EPSConfiguration::STAT::ACCEPTED||stat.value() == EPSConfiguration::STAT::NEW) {
            LOG_DEBUG << "MODE:  " << AcubeSATParameters::epsMODE.getValue();
            LOG_DEBUG << "CONF:  " << AcubeSATParameters::epsCONF.getValue();
            LOG_DEBUG << "RESET_CAUSE:  " << AcubeSATParameters::epsRESET_CAUSE.getValue();
            LOG_DEBUG << "UPTIME:  " << AcubeSATParameters::epsUPTIME.getValue();
            LOG_DEBUG << "ERROR:  " << AcubeSATParameters::epsERROR.getValue();
            LOG_DEBUG << "UNIX_TIME:  " << AcubeSATParameters::epsUNIX_TIME.getValue();
            LOG_DEBUG << "UNIX_HOUR:  " << AcubeSATParameters::epsUNIX_HOUR.getValue();
            LOG_DEBUG << "UNIX_MINUTE:  " << AcubeSATParameters::epsUNIX_MINUTE.getValue();
            LOG_DEBUG << "UNIX_SECOND:  " << AcubeSATParameters::epsUNIX_SECOND.getValue();

        }
    }


    vTaskDelay(pdMS_TO_TICKS(10));
    stat = eps.getConfigurationParameter(EPSConfiguration::READ_ONLY_CONFIG_PARAM_IDs::CH_STARTUP_ENA_USE_BF);
    LOG_DEBUG << "CH_STARTUP_ENA_USE_BF: " << AcubeSATParameters::epsCH_STARTUP_ENA_USE_BF.getValue();

    while (true) {
        auto stat = eps.watchdogReset();
        LOG_DEBUG<< "EPS STAT: " << static_cast<EPSConfiguration::STAT_t>(stat.value());
        LOG_DEBUG << "Test Task executed";
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}