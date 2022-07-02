
#include "signalThread.h"
#include "qimage.h"

#include "msgQueue.hpp"


void signalThread::run()
{

    while (1)
    {
        std::string mqttMsg;
        FromMQTTQueue.getMsg(mqttMsg);
        // ∑¢…‰–≈∫≈
        signalMsg msg;
        msg.msg = mqttMsg.c_str();
        emit Send2UI(msg);
        printf("send success!!\n");
    }


}

