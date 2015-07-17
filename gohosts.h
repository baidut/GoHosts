#ifndef GOHOSTS_H
#define GOHOSTS_H

#include <QMainWindow>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkrequest.h>
#include <QtNetwork/qnetworkreply.h>
#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>
#include <QMessageBox>

namespace Ui {
class GoHosts;
}

class GoHosts : public QMainWindow
{
    Q_OBJECT

public:
    explicit GoHosts(QWidget *parent = 0);
    ~GoHosts();

private slots:
    void getSource();
    void updateHosts();
    void on_pushButton_clicked();
    void on_anchor_clicked(const QUrl &);

private:
    Ui::GoHosts *ui;

    QNetworkAccessManager *manager;
    QNetworkReply *reply_hosts;
    QNetworkReply *reply_doc;
    QString url_hosts;
};

#endif // GOHOSTS_H
