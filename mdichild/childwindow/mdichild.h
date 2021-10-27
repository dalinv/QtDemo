#ifndef MDICHILD_H
#define MDICHILD_H

#include <QWidget>

namespace Ui {
class MdiChild;
}
class QJsonObject;
class CustomGraphicsScence;
class MdiChild : public QWidget
{
    Q_OBJECT

public:
    explicit MdiChild(QWidget *parent = nullptr);
    ~MdiChild();
    void initScene(const QJsonObject &rootObj);
    void writeData(const QJsonObject &jsonObj,const QString &strPath);
    bool getSaveFlag();
    QString getSavePath();

signals:
    void signalModeChange(bool mode);
    void sigSaveFile();

public slots:
    void slotSaveAllInfo(const QString &strPath);

protected:
    void keyReleaseEvent(QKeyEvent *keyEvent) override;
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MdiChild *ui;
    bool m_bMouseMode;
    CustomGraphicsScence *m_scene;
    bool m_bSave;
    QString m_strSavePath;
};

#endif // MDICHILD_H
