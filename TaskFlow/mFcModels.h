#ifndef MFCMODELS_H
#define MFCMODELS_H
#include <QObject>
QT_BEGIN_NAMESPACE;
class QString;
class QWidget;
class QVBoxLayout;
QT_END_NAMESPACE;

class RunWebEngineView;

//分割重点说明： 如某个参数类型，其对象有多个则属性使用*分割，如某个属性是一个集合，集合内是另外一个类型，则集合内的每一个对象之间使用&分割。而集合内每一个对象的属性之间仍然使用*分割
const QString SEPARATOR_1 = "*";//对象内，属性与属性之间的参数分割符号
const QString SEPARATOR_2 = "&";//对象内，某个具有循环参数的属性，该属性循环参数的分隔符号
const QString SEPARATOR_SUB_LEFT = "[";//循环窗体的左侧
const QString SEPARATOR_SUB_AMONG = ","; //循环窗体内，每个step的分割符号
const QString SEPARATOR_SUB_RIGHT = "]";//循环窗体的右侧
const QString SEPARATOR_WEBDATA_ARRAY = ",r#,"; // c++与网页交互，对象与对象之间的分隔符
const QString SEPARATOR_WEBDATA_ATTR  = "*r#*"; // c++与网页交互，对象内属性间的分隔符


const QString MCONSTANT_RADIO_SINGLE = "radio_single"; //单个元素
const QString MCONSTANT_RADIO_FIXED = "radio_fixed";//固定元素
const QString MCONSTANT_RADIO_UNFIXED = "radio_unfixed";//不固定元素
const QString MCONSTANT_RADIO_SITEADDRESS = "radio_siteaddress";//网址列表

const QString MCONSTANT_FLOW_MENU_TYPE_OpenWebBtn = "1114";// 打开网页
const QString MCONSTANT_FLOW_MENU_TYPE_ClickEleBtn ="1115";// 点击元素
const QString MCONSTANT_FLOW_MENU_TYPE_ExtractBtn = "1116";//提取数据
const QString MCONSTANT_FLOW_MENU_TYPE_InputBtn = "1117";// 输入文字
const QString MCONSTANT_FLOW_MENU_TYPE_CaptchaBtn = "1118";//识别验证码
const QString MCONSTANT_FLOW_MENU_TYPE_SwitchBtn = "1119";//切换下拉选项
const QString MCONSTANT_FLOW_MENU_TYPE_JudgeBtn = "1120";//判断条件
const QString MCONSTANT_FLOW_MENU_TYPE_LoopBtn = "1121";//循环
const QString MCONSTANT_FLOW_MENU_TYPE_MouseBtn = "1122";//移动鼠标到元素上
const QString MCONSTANT_FLOW_MENU_TYPE_EndLoopBtn ="1123";//结束循环
const QString MCONSTANT_FLOW_MENU_TYPE_EndBtn ="1124";//结束


// 拖拽流程上的窗口时，发送给control控件的信号类型
// 通知父窗口，开始拖拽
const int MCONNSTANT_NOTIFICATION_TYPE_START = 100;
// 通知父窗口，拖拽以取消的形式结束（侧边栏的菜单项使用，拖拽取消不影响任何内容显示）
const int MCONNSTANT_NOTIFICATION_TYPE_OTHER_END = 101;
// 通知父窗口，拖拽以取消的形式结束+显示属性部分 （内部窗体拖拽，即使拖拽取消，也会影响右侧内容显示）
const int MCONNSTANT_NOTIFICATION_TYPE_OTHER_END_SHOWCONTENT = 103;
// 通知父窗口，拖拽以正常完成的形式结束+显示属性部分（内部窗体拖拽，拖拽完成，同时影响右侧内容显示）
const int MCONNSTANT_NOTIFICATION_TYPE_MOVE_END_SHOWCONTENT = 104;
// 通知父窗口，当前窗口被删除
const int MCONNSTANT_NOTIFICATION_TYPE_DELETE_CURRENT_WIDGET= 110;

// 修饰流程步骤窗体正常状态下的样式 偏蓝色
const QString m_stylesheet_QWidget_flowleftwidget_blue_normal = "QWidget {background-color: rgb(99,131,188); \
        border-radius:5px; padding:2px 4px;} QWidget:click{background-color: rgb(43,113,237);}";
// 修饰流程步骤窗体按下状态下的样式 偏蓝色
const QString m_stylesheet_QWidget_flowleftwidget_blue_press = "QWidget {background-color: rgb(43,113,237); border-radius:5px; padding:2px 4px;}";

const QString m_stylesheet_QLabel_title = ".QLabel{font-family:Microsoft YaHei;font-size:12px;background-color: rgba(0,0,0,0);color:rgb(255,255,255);}";

class MControl{
public:
    MControl(QString menuType,QString menuTip,QString menuIcon,QString menuFlowWidgetName);
    ~MControl();
public:
    QString menuType; /*1114,1115,,, 必须是4位，否则在任务序列化和发序列化会出错*/
    QString menuTip;
    QString menuIcon;
    QString menuFlowWidgetName;
};


class MFlowStepParams{
public:
    MFlowStepParams();
    virtual ~MFlowStepParams();
    enum LocationType{
        Front,
        Middle,
        Last
    };
public:
    QString menuType;// 控件所属类型
    QString stepID;  // 每一个控件窗体生成时，自动产生的步骤ID
    QString stepURL; // 生成控件窗体时，产生规则的网页url地址
    QString name;

public:
    //协助执行任务流程
    MFlowStepParams *carryParent = nullptr; // 默认无父窗体
    LocationType carryStepLocation; // 位置范围
};

//! 打开网页
class MFlowStepParamsOpenWeb : public MFlowStepParams {
public:
    MFlowStepParamsOpenWeb();
    MFlowStepParamsOpenWeb(const QString &program);
public:
    QString url;//网址
    int overtime = 10;//超时 s

    int waitSleep=0;//执行前等待时间 s
    QString waitEmergintEle;//或出现元素

    bool isAutoTry = false;//自动重试
    bool isCustomCookie = false;//自定义cookie
    QString cookie;//自定义cookie值
    bool isLoop = false;// 使用循环
    bool isPreventWindow = false;//是否阻止弹出窗口
    bool isClearCacheBeforeOpenWeb = false;//打开网页前先清理浏览器缓存
    bool isRoll = false;// 滚动页面
    int rollCount = 1;//滚动次数
    int rollInterval = 0;// 滚动每次间隔 单位s
    int rollTypeIndex = 0;// 滚动方式 0：直接滚动到底部  1：向下滚动一屏

    const QString getProgram();

};

//! 点击元素
class MFlowStepParamsClickEle : public MFlowStepParams{
public:
    MFlowStepParamsClickEle();
    MFlowStepParamsClickEle(const QString &program);
public:
    QString eleText;//节点文本
    QString eleXpath;//节点xpath
    int waitSleep=0;//执行前等待时间 s
    QString waitEmergintEle;//或出现元素
    bool isAutoTry = false;//自动重试
    bool isLoop = false;// 使用循环

    bool isNewTab = false;//开新标签
    bool isAjax = false;//Ajax加载
    int ajaxOvertime = 0;//ajax超时
    bool isPageSpeed = false;//页面加速（非ajax页面加速）
    bool isAnchor = false;//定位锚点
    QString anchorId;//锚点ID

    bool isRoll = false;// 滚动页面
    int rollCount = 1;//滚动次数
    int rollInterval = 0;// 滚动每次间隔 单位s
    int rollTypeIndex = 0;// 滚动方式 0：直接滚动到底部  1：向下滚动一屏

    bool isTurnPage = false; // 是否翻页
    const QString getProgram();
};

struct MParamsField{
    public:
        MParamsField();
        MParamsField(const QString &program);
        ~MParamsField();
    public:
        QString fieldXpath;//字段xpath
        QString fieldName;//字段名称 例：字段_1
        QString fieldTag;  //字段对应的标签 例：A
        QString fieldVal;//字段值的例子  例：http://xxx.com.cn/xxxx/123
        QString fieldActionName;//字段的行为名称 例：采集该链接地址
        QString fieldActionMethod;//字段的行为获取方式，在网页上获取的方法 例：.href
        // 辅助显示的字段
        int currentRow;// 当前数据所处行 （从0开始）

        const QString getProgram();

};

//! 提取数据
class MFlowStepParamsExtract : public MFlowStepParams{
public:
    MFlowStepParamsExtract();
    MFlowStepParamsExtract(const QString &program);
    ~MFlowStepParamsExtract();
public:
    int waitSleep=0;//执行前等待时间 s
    QString waitEmergintEle;//或出现元素
    bool isLoop = false;// 使用循环
    QVector<MParamsField *> pfs;
public:
    //协助执行规则
    int carryDeep;// 主要应用于提取窗体（从任务流中划分深度，用于字段合并）
    QString carryFields;

    const QString getProgram();

};
//! 输入文字
class MFlowStepParamsInput :public MFlowStepParams{
public:
    MFlowStepParamsInput();
    MFlowStepParamsInput(const QString &program);
public:
    QString eleXpath;//节点xpath
    QString eleText;//节点文本
    int waitSleep=0;//执行前等待时间 s
    QString waitEmergintEle;//或出现元素
    bool isLoop = false;// 使用循环
    QString inputText;

    const QString getProgram();
};
//! 验证码
class MFlowStepParamsCaptcha : public MFlowStepParams{
public:
    MFlowStepParamsCaptcha();
    const QString getProgram();
};

//!  切换下拉选项
class MFlowStepParamsSwitch : public MFlowStepParams{
public:
    MFlowStepParamsSwitch();
    MFlowStepParamsSwitch(const QString &program);
public:
    QString eleXpath;//节点xpath
    QString eleText;//节点文本
    int waitSleep=0;//执行前等待时间 s
    QString waitEmergintEle;//或出现元素
    bool isLoop = false;// 使用循环
    const QString getProgram();
};
//! 判断窗体
class MFlowStepParamsIf : public MFlowStepParams{
public:
    MFlowStepParamsIf();
    MFlowStepParamsIf(const QString &program);
public:
    int waitSleep=0;//执行前等待时间 s
    QString waitEmergintEle;//或出现元素
    bool isLoop = false;// 使用循环

    const QString getProgram();

};
//! 循环窗体
class MFlowStepParamsLoop : public MFlowStepParams{
public:
    MFlowStepParamsLoop();
    MFlowStepParamsLoop(const QString &program);
public:
    int waitSleep=0;//执行前等待时间 s
    QString waitEmergintEle;//或出现元素
    QString loopTypeName;//循环方式
    QString loopTypeValue;//循环方式对应的值
    bool isEndLoopWithTimes = false; //是否选中满足循环次数则终止
    int  endLoopTimes = 0; // 满足循环则退出的次数
    QStringList loopTextList;// 循环列表的数据
    const QString getProgram();
public:
    //协助采集
    // 单个元素循环使用
    int carrySingleCurrent = 0;
    // 网址列表循环使用
    QStringList carryAddressList;
    int carryAddressCurrent=0;
    int carryAddressTotal=0;

    // 固定不固定循环使用
    RunWebEngineView *carryCurrentWebView;
    int carryTotal = 0;
    int carryCurrent = 0;

};

//! 鼠标移动到元素上
class MFlowStepParamsMouse : public MFlowStepParams{
public:
    MFlowStepParamsMouse();
    MFlowStepParamsMouse(const QString &program);
    QString eleText;//节点文本
    QString eleXpath;
    int waitSleep=0;//执行前等待时间 s
    QString waitEmergintEle;//或出现元素
    bool isLoop = false;// 使用循环

    const QString getProgram();
};
//! 结束循环
class MFlowStepParamsEndLoop : public MFlowStepParams{
public:
    MFlowStepParamsEndLoop();
    const QString getProgram();
};
//! 结束流程
class MFlowStepParamsEnd : public MFlowStepParams{
public:
    MFlowStepParamsEnd();
    const QString getProgram();
};
//! 步骤窗体的参数
class MFlowStepData{
public:
    MFlowStepData(MControl *control,MFlowStepParams *params);
    ~MFlowStepData();
public:
    MControl *control;  // 控件类型
    MFlowStepParams *stepparams;  //步骤参数（窗体被释放时，自我释放。除执行器部分统一释放内存，带有窗体的全部，自我释放）

    int notificationType;// 消息通知类型
    QWidget  *widget;    //数据所属的窗体，同时也是该对象也是在创建窗体时进行的初始化，需要这个地方
    bool     widgetIsDelete=false;//所属窗体是否被删除。默认false：未删除  true:已经删除（窗体对象已不存在）

};

#endif // MFCMODELS_H
