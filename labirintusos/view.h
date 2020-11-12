#ifndef VIEW_H
#define VIEW_H

#include "model.h"
#include "musicplayer.h"

#include <QMainWindow>
#include <QPainter>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT

public:
    View(QWidget *parent = nullptr);
    ~View();

    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

public slots:
    void win();
    void ujJatek();
    void ujTizJatek();
    void ujHuszJatek();
    void ujHarmincJatek();
    void ujNegyvenJatek();
    void ujOtvenJatek();
    void ujHatvanJatek();
    void ujHetvenJatek();
    void ujNyolcvanJatek();
    void ujKilencvenJatek();
    void ujSzazJatek();
    void ujSzazTizJatek();
    void ujSzazHuszJatek();
    void ujSzazHarmincJatek();
    void ujSzazNegyvenJatek();
    void ujSzazOtvenJatek();
    void seeEverything();
    void seeNothing();
    void ujEmber();
    void ujMap();
    void showHelp();

private:
    Ui::View *ui;
    //variables
    Model * model;
    MusicPlayer* musicplayer;
    bool isLightsOn;
    bool showMap;
    int zoom;
    int zoomSzamlalo;
    int melyikEmber;
    int melyikMap;
    QPixmap fal2;
    QVector<QPixmap> jatekosKinezetek;
    QVector<QPixmap> kincsKinezetek;
    QPixmap tavoliTalaj;
    QPixmap tavoliFal;
    QPixmap terkepPIXMAP;
    QPixmap jatekosPixmap;
    QPixmap talaj2;
    QPixmap labnyom;
    Direction ujNezes;
    //functions
    void openMap();
};
#endif // VIEW_H
