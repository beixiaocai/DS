#ifndef CONSTANT_H
#define CONSTANT_H

#include <QString>

/**
 * @brief HOST
 * 各位朋友请注意，在这个项目里面，我一共添加了三个业务接口，分别是如下三个：
    1 reportCrash  // 上报崩溃信息
    2 reportHeart  // 上传心跳
    3 checkVersion // 检查版本
  大家在二次开发的时候，完全可以忽略接口的存在，因为这并不会影响功能的使用。
 *
 */
const QString HOST = "http://127.0.0.1:18000";
const bool    IS_reportHeart = false;
const QString URL_DOCUMENT = "https://github.com/any12345com/DS/blob/master/README.md";// 文档
const QString URL_FEEDBACK = "https://github.com/any12345com/DS/issues"; // 反馈
const QString URL_OPENSOURCE = "https://github.com/any12345com/DS"; // 开源地址


const QString JS_QWEBCHANNEL_FILE = ":/res/js/lib/qwebchannel_release.js.txt";

const QString JS_CUSTOMTASK_FILE = ":/res/js/customtask.js.txt";
const QString JS_EXECUTE_FILE = ":/res/js/execute.js.txt";

//const QString JS_CUSTOMTASK_FILE = "D:\\DS\\static\\test\\customtask.js";
//const QString JS_EXECUTE_FILE = "D:\\DS\\static\\test\\execute.js";

const QString SETTINGS_KEY_FINGER= "settings_finger";// 唯一指纹
const QString SETTINGS_KEY_OPEN_PROXY= "settings_open_proxy";// 是否开启代理
const QString SETTINGS_KEY_PROXY_IP= "settings_proxy_ip";
const QString SETTINGS_KEY_PROXY_PORT= "settings_proxy_port";
const QString SETTINGS_KEY_PROXY_USERNAME= "settings_proxy_username";
const QString SETTINGS_KEY_PROXY_PASSWORD= "settings_proxy_password";

#endif // CONSTANT_H
