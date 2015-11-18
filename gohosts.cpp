#include "gohosts.h"
#include "ui_gohosts.h"
#include <QDesktopServices>

#define URL_GOHOSTS "http://baidut.sinaapp.com/gohosts.txt"

#if defined(Q_OS_WIN32)
#define PATH_LOCAL_HOSTS "C:\\Windows\\System32\\drivers\\etc\\hosts"
//"%SYSTEMROOT%/System32/drivers/etc/hosts"
#elif defined(Q_OS_LINUX)
#define PATH_LOCAL_HOSTS "/etc/hosts"
#endif

GoHosts::GoHosts(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GoHosts)
{
    ui->setupUi(this);

    manager =new QNetworkAccessManager(this);
    QNetworkRequest request ;
    request.setUrl(QUrl(URL_GOHOSTS));
    reply_doc = manager->get(request);
    connect(reply_doc, SIGNAL(finished()), this, SLOT(getSource()));
}

GoHosts::~GoHosts()
{
    delete ui;
}

void GoHosts::getSource(){
    QString get = QString::fromUtf8( reply_doc->readAll());
    if (get.isEmpty()) {
        QMessageBox::critical(this, "加载软件文档-失败", "请确保网络连接后重试<br>"
                              "如网络连接正常时浏览器无法访问<a href=\'" URL_GOHOSTS "\'>软件文档</a>"
                              "则说明软件已经下架，抱歉！");
    }
    else {
        ui->textBrowser->append(get);

        // 调用系统浏览器打开文档里的链接
        ui->textBrowser->setOpenLinks(false);
        connect(ui->textBrowser, SIGNAL(anchorClicked(const QUrl&)),this, SLOT(on_anchor_clicked(const QUrl&)));

        // 解析
        QString pattern("http*.txt");
        QRegExp rx(pattern);
        if (get.indexOf(rx) > -1){
            url_hosts = rx.cap(0);
            ui->statusbar->showMessage("解析hosts资源地址-成功");
        }
        else {
            QMessageBox::critical(this,"错误", "解析hosts资源地址-失败");
        }
    }
}

void GoHosts::updateHosts(){
    QString get = QString::fromUtf8( reply_hosts->readAll());
    if (get.isEmpty()) {
        QMessageBox::critical(this, "Error", "获取hosts失败,请点击重试");
        return;
    }

    // 502 Bad Gateway
    QString pattern("502 Bad Gateway");
    QRegExp rx(pattern);
    if (get.indexOf(rx) > -1){
        QMessageBox::critical(this, "Error", "服务器忙,请点击重试");
        return;
    }

    // 删除已经存在的hosts文件
    QFile hosts_old(PATH_LOCAL_HOSTS);
    if ( hosts_old.exists()) {
        if(hosts_old.setPermissions(QFile::WriteGroup)
                &&hosts_old.setPermissions(QFile::WriteOwner)
                &&hosts_old.setPermissions(QFile::WriteUser)) {
            hosts_old.remove();
        }
        else {
            QMessageBox::critical(this, "错误", "没有权限，请右键管理员身份打开本程序");
            return;
        }
    }
    // 添加新的hosts文件 创建也需要权限
    QFile hosts_new(PATH_LOCAL_HOSTS);
    if (hosts_new.open( QIODevice::WriteOnly|QFile::Text)) {
        QTextStream stream(&hosts_new);
        stream << get;
        hosts_new.close();
        QMessageBox::information(this, "完成", "更新hosts文件-成功");
    }
    else {
        QMessageBox::critical(this, "错误", "没有权限，请右键管理员身份打开本程序");
    }
}

void GoHosts::on_pushButton_clicked()
{
    QNetworkRequest request ;
    request.setUrl(QUrl(url_hosts));
    reply_hosts = manager->get(request);
    connect(reply_hosts, SIGNAL(finished()), this, SLOT(updateHosts()));
}

void GoHosts::on_anchor_clicked(const QUrl&url)
{
    QDesktopServices::openUrl(QUrl(url));
}
