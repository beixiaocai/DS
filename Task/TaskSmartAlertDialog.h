#ifndef TASKSMARTALERTDIALOG_H
#define TASKSMARTALERTDIALOG_H

#include <QDialog>
QT_BEGIN_NAMESPACE;
class QStackedWidget;
class QVBoxLayout;
class QLabel;
QT_END_NAMESPACE;

class MCustomTaskSmartAlertXPath;
class MCustomTaskSmartAlertClick;
class MCustomTaskSmartAlertSelectAll;
class MCustomTaskSmartAlertSuccess;
class PageCustomTaskFlowControl;
class MFlowStepData;
class TaskWebEngineView;


class TaskSmartAlertDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskSmartAlertDialog(QWidget *parent,TaskWebEngineView *webView,PageCustomTaskFlowControl *fc);

    void block();

private:
    struct MEachP2{
        MEachP2(const QString eachp2);
        MEachP2();
        QString ele_fullXpath; // xpath绝对路径
        QString ele_xpath;     //xpath相对路径
        QString ele_text;   // 任意标签的文本
        QString ele_custom; // a标签的href 或 img标签的src 或 input标签value 或 select标签的options[index].value
        QString ele_innerHtml;// 任意标签
        QString ele_outerHtml;// 任意标签
        int     sameTypeCount;//任意标签同类型元素数量
    };
    struct MEachParams{
        QString clickNodeName; // 例：A
        QString clickNodeCode; // 例：6778962007
        QString loopXpath;     // 循环xpath
        MEachP2          i_p2;
        QVector<MEachP2> i_p2V; // 二次交互获取
        QString eachp2;      // 二次交互获取 例：/html/body/div[3]/div[2]/ul[1]/li[1]/div[1]/div[1]/a[1],,QQ,,https://sj.qq.com/myapp/detail.htm?apkName=com.tencent.mobileqq,,QQ,,<a href=\"detail.htm?apkName=com.tencent.mobileqq\" target=\"_blank\" class=\"name ofh\" hidefocus=\"true\" style=\"border-color: rgb(60, 179, 113); border-style: solid; border-width: 1px;\">QQ</a>,,
    };

    QVBoxLayout * boxLayout;
    QLabel *tipLabel;
    QStackedWidget *stackedWidget;

    MCustomTaskSmartAlertXPath *xpathWidget;



    void initSuccessWidget();
    MCustomTaskSmartAlertSuccess *successWidget;

    // a标签
    void initAWidget();
    MCustomTaskSmartAlertClick *aWidget;
    void initAWidgetSelectAll();
    MCustomTaskSmartAlertSelectAll *aWidgetSelectAll;
    // img标签
    void initImgWidget();
    MCustomTaskSmartAlertClick *imgWidget;
    void initImgWidgetSelectAll();
    MCustomTaskSmartAlertSelectAll *imgWidgetSelectAll;

    // input标签（不存在选择全部，只有单标签）
    void initInputWidget();
    MCustomTaskSmartAlertClick *inputWidget;
    void initInputInputWidget();
    QWidget *inputinputWidget;

//    // 常见标签 （span,div,h1,h2,,,）
    void initCommonWidget();
    MCustomTaskSmartAlertClick *commonWidget;
    void initCommonWidgetSelectAll();
    MCustomTaskSmartAlertSelectAll *commonWidgetSelectAll;


    MEachParams m_i;// 网页直接交互过来的参数
    TaskWebEngineView *m_webView;
    PageCustomTaskFlowControl *m_fc;
private:
    void smartAlertSelectAll(MCustomTaskSmartAlertSelectAll * selectAll);
    void smartAlertClickEle();
    void smartAlertClickEleTurnPage();// 单个元素（下一页元素） 循环翻页
    void smartAlertMouse();
    void smartAlertExtract(const QString &name);
    void smartAlertInput(const QString &text);
    void smartAlertSelectAllClickEle();
    void smartAlertSelectAllExtract(const QString &name);

protected:
    void closeEvent(QCloseEvent *) override;

signals:
    void notifySmartAlertChecked(bool checked);
public slots:
    void onSmartAlertCancel(bool checked);
    void onClickSignalToQt(const QString &eachp2,const QString &nodeName,const QString &nodeCode,const QString &loopXpath);
    void onNotificationFromFlowControl(MFlowStepData *data);



};

#endif // TASKSMARTALERTDIALOG_H
