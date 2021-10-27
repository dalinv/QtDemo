#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPixmap>
#include <QMdiSubWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
//struct NodeInfos
//{
//    QPixmap icon;
//    QString name;
//};

class MdiChild;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setListWdtStyle();
    void newMdiChild(const QString &staPath,const QJsonObject &jsonObj);

signals:
    void sigSaveFile(const QString &strPath);
    void sigOpenFile(const QString &strName);

private slots:
    void slotMenuNew();
    void slotMenuSave();
    void slotMenuOpen();
    void slotExit();
    void slotNodeType();

    void on_mdiArea_subWindowActivated(QMdiSubWindow *arg1);

private:
    Ui::MainWindow *ui;
    int m_titleNum;
    QString m_strName;
    MdiChild *m_currentMdiChild;



};
#endif // MAINWINDOW_H
