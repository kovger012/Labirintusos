#include "view.h"
#include "ui_view.h"

#include <QColor>
#include <QImage>
#include <QMenu>
#include <QMessageBox>
#include <QPen>
#include <QRect>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QPixmap>
//#include <QMediaPlaylist>

View::View(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);

    setWindowTitle(tr("Labirintus"));

    model = new Model;
    musicplayer = new MusicPlayer();

    //initialize variables
    melyikEmber = 0;
    melyikMap = 0;
    showMap = false;
    isLightsOn = false;
    zoomSzamlalo = 950;
    ujNezes = Right;

    //load Pixmaps
    //player(s)
    jatekosPixmap = QPixmap("pics/jatekos.png");
    //jobb
    jatekosKinezetek.push_back(QPixmap("pics/jobb1.png"));
    jatekosKinezetek.push_back(QPixmap("pics/jobb2.png"));
    jatekosKinezetek.push_back(QPixmap("pics/jobb3.png"));
    jatekosKinezetek.push_back(QPixmap("pics/jobb4.png"));
    //le
    jatekosKinezetek.push_back(QPixmap("pics/szembe1.png"));
    jatekosKinezetek.push_back(QPixmap("pics/szembe2.png"));
    jatekosKinezetek.push_back(QPixmap("pics/szembe3.png"));
    jatekosKinezetek.push_back(QPixmap("pics/szembe4.png"));
    //bal
    jatekosKinezetek.push_back(QPixmap("pics/bal1.png"));
    jatekosKinezetek.push_back(QPixmap("pics/bal2.png"));
    jatekosKinezetek.push_back(QPixmap("pics/bal3.png"));
    jatekosKinezetek.push_back(QPixmap("pics/bal4.png"));
    //fel
    jatekosKinezetek.push_back(QPixmap("pics/hat1.png"));
    jatekosKinezetek.push_back(QPixmap("pics/hat2.png"));
    jatekosKinezetek.push_back(QPixmap("pics/hat3.png"));
    jatekosKinezetek.push_back(QPixmap("pics/hat4.png"));
    //walls
    fal2 = QPixmap("pics/fal02.png");
    tavoliFal = QPixmap("pics/gigasotet.png");
    //grounds
    talaj2 = QPixmap("pics/talaj2.png");
    tavoliTalaj = QPixmap("pics/setettalaj.png");
    labnyom = QPixmap("pics/labnyom.png");
    //map
    terkepPIXMAP = QPixmap("pics/terkep.png");
    //kincsek
    kincsKinezetek.push_back(QPixmap("pics/kincs1.png"));
    kincsKinezetek.push_back(QPixmap("pics/kincs2.png"));
    kincsKinezetek.push_back(QPixmap("pics/kincs3.png"));

    //create menus
    //new game menu
    QMenu* newGameMenu = new QMenu("Új játék", this);
    newGameMenu->addAction("Jelenlegi mérettel"  , this, SLOT(ujJatek()));
    newGameMenu->addAction("10x10"  , this, SLOT(ujTizJatek()));
    newGameMenu->addAction("20x20"  , this, SLOT(ujHuszJatek()));
    newGameMenu->addAction("30x30"  , this, SLOT(ujHarmincJatek()));
    newGameMenu->addAction("40x40"  , this, SLOT(ujNegyvenJatek()));
    newGameMenu->addAction("50x50"  , this, SLOT(ujOtvenJatek()));
    newGameMenu->addAction("60x60"  , this, SLOT(ujHatvanJatek()));
    newGameMenu->addAction("70x70"  , this, SLOT(ujHetvenJatek()));
    newGameMenu->addAction("80x80"  , this, SLOT(ujNyolcvanJatek()));
    newGameMenu->addAction("90x90"  , this, SLOT(ujKilencvenJatek()));
    newGameMenu->addAction("100x100", this, SLOT(ujSzazJatek()));
    newGameMenu->addAction("110x110", this, SLOT(ujSzazTizJatek()));
    newGameMenu->addAction("120x120", this, SLOT(ujSzazHuszJatek()));
    newGameMenu->addAction("130x130", this, SLOT(ujSzazHarmincJatek()));
    newGameMenu->addAction("140x140", this, SLOT(ujSzazNegyvenJatek()));
    newGameMenu->addAction("150x150", this, SLOT(ujSzazOtvenJatek()));
    ui->menubar->addMenu(newGameMenu);
    //options menu
    QMenu* optionsMenu = new QMenu("Opciók", this);
    optionsMenu->addAction("Pálya teljes megvilágítása", this, SLOT(seeEverything()));
    optionsMenu->addAction("Látóhatár bekapcsolása", this, SLOT(seeNothing()));
    ui->menubar->addMenu(optionsMenu);
    //help menu
    QMenu* helpMenu = new QMenu("Súgó", this);
    helpMenu->addAction("Gombok használata", this, SLOT(showHelp()));
    ui->menubar->addMenu(helpMenu);
    //connect
    connect(model, SIGNAL(update()), this, SLOT(update()));
    connect(model, SIGNAL(win()), this, SLOT(win()));
    connect(model, SIGNAL(masikat()), this, SLOT(ujEmber()));
    //connect(model, SIGNAL(lebeg()), this, SLOT(ujMap()));
}

View::~View()
{
    delete ui;
}

void View::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    zoom = zoomSzamlalo/model->getN();
    int yEltolas = ui->menubar->height();

    QImage samePlayerImg = jatekosPixmap.toImage();
    samePlayerImg = samePlayerImg.scaled(zoom, zoom, Qt::IgnoreAspectRatio);
    QImage fal2Img = fal2.toImage();
    fal2Img = fal2Img.scaled(zoom, zoom, Qt::IgnoreAspectRatio);
    QImage talaj2Img = talaj2.toImage();
    talaj2Img = talaj2Img.scaled(zoom, zoom, Qt::IgnoreAspectRatio);
    QImage jatekosKinez = jatekosKinezetek[melyikEmber].toImage();
    jatekosKinez = jatekosKinez.scaled(zoom, zoom, Qt::IgnoreAspectRatio);
    QImage setatalaj = tavoliTalaj.toImage();
    setatalaj = setatalaj.scaled(zoom, zoom, Qt::IgnoreAspectRatio);
    QImage tavFal = tavoliFal.toImage();
    tavFal= tavFal.scaled(zoom, zoom, Qt::IgnoreAspectRatio);
    QImage terkepesIMAGE = terkepPIXMAP.toImage();
    terkepesIMAGE = terkepesIMAGE.scaled(zoom, zoom, Qt::IgnoreAspectRatio);
    QImage labnyomIMAGE = labnyom.toImage();
    labnyomIMAGE = labnyomIMAGE.scaled(zoom, zoom, Qt::IgnoreAspectRatio);

    if(isLightsOn)
    {
        //falak és talaj és kincsek
        for (int x=0; x< model->getPalyaMatrix().size(); ++x) {
            for (int y=0; y< model->getPalyaMatrix().size(); ++y) {
                if (model->getPalyaMatrix()[x][y] == 0){
                    painter.drawImage(QPoint(x*zoom, y*zoom + yEltolas), fal2Img);
                }else if (model->getPalyaMatrix()[x][y] == 1){
                    painter.drawImage(QPoint(x*zoom, y*zoom + yEltolas), talaj2Img);
                }else if (model->getPalyaMatrix()[x][y] == 2){
                    painter.drawImage(QPoint(x*zoom, y*zoom + yEltolas), kincsKinezetek[0].toImage().scaled(zoom, zoom, Qt::IgnoreAspectRatio));
                }else if (model->getPalyaMatrix()[x][y] == 3){
                    painter.drawImage(QPoint(x*zoom, y*zoom + yEltolas), kincsKinezetek[1].toImage().scaled(zoom, zoom, Qt::IgnoreAspectRatio));
                }else if (model->getPalyaMatrix()[x][y] == 4){
                    painter.drawImage(QPoint(x*zoom, y*zoom + yEltolas), kincsKinezetek[2].toImage().scaled(zoom, zoom, Qt::IgnoreAspectRatio));
                }
            }
        }

        //térkép
        for(int i = 0; i < model->getTerkep().size(); ++i)
        {
            painter.drawImage(QPoint(model->getTerkep()[i].x() * zoom, model->getTerkep()[i].y() * zoom + yEltolas), terkepesIMAGE);
        }
    }
    else
    {
        //falak és talaj
        painter.setPen(QPen(Qt::black, 0));
        painter.setBrush(Qt::black);
        for (int x=0; x< model->getPalyaMatrix().size(); ++x) {
            for (int y=0; y< model->getPalyaMatrix().size(); ++y) {
                if (model->getPalyaMatrix()[x][y] == 0){
                    painter.drawRect(x*zoom, y*zoom + yEltolas, 1*zoom, 1*zoom);
                }else{
                    painter.drawRect(x*zoom, y*zoom + yEltolas, 1*zoom, 1*zoom);
                }
            }
        }
        //nem látóhatár
        painter.setPen(QPen(Qt::black, 0));
        painter.setBrush(Qt::black);
        for(int i = 0; i < model->getNemLatas().size(); ++i)
        {
            painter.drawRect(model->getNemLatas()[i].x()*zoom, model->getNemLatas()[i].y()*zoom + yEltolas, 1*zoom, 1*zoom);
        }
        //látás, de fal
        for(int i = 0; i < model->getLatasDEfal().size(); ++i)
        {
            painter.drawImage(QPoint(model->getLatasDEfal()[i].x()*zoom, model->getLatasDEfal()[i].y()*zoom + yEltolas), fal2Img);
        }
        //látóhatár
        for(int i = 0; i < model->getLatas().size(); ++i)
        {
            painter.drawImage(QPoint(model->getLatas()[i].x()*zoom, model->getLatas()[i].y()*zoom + yEltolas), talaj2Img);
        }

        //távoli fal
        for(int i = 0; i < model->getTavoliFal().size(); ++i)
        {
            painter.drawImage(QPoint(model->getTavoliFal()[i].x()*zoom, model->getTavoliFal()[i].y()*zoom + yEltolas), tavFal);
        }

        //tálvoli talaj
        for(int i = 0; i < model->getTavoliTalaj().size(); ++i)
        {
            painter.drawImage(QPoint(model->getTavoliTalaj()[i].x()*zoom, model->getTavoliTalaj()[i].y()*zoom + yEltolas), setatalaj);
        }
        //látható térképek
        for(int i = 0; i < model->getVisibleMaps().size(); ++i)
        {
            painter.drawImage(QPoint(model->getVisibleMaps()[i].x() * zoom, model->getVisibleMaps()[i].y() * zoom + yEltolas), terkepesIMAGE);
        }
        //látható kincsek
        for(int i = 0; i < model->getVisibleKincs().size(); ++i)
        {
            if (model->getPalyaMatrix()[model->getVisibleKincs()[i].x()][model->getVisibleKincs()[i].y()] == 2){
                painter.drawImage(QPoint(model->getVisibleKincs()[i].x() * zoom, model->getVisibleKincs()[i].y() * zoom + yEltolas), kincsKinezetek[0].toImage().scaled(zoom, zoom, Qt::IgnoreAspectRatio));
            }else if (model->getPalyaMatrix()[model->getVisibleKincs()[i].x()][model->getVisibleKincs()[i].y()] == 3){
                painter.drawImage(QPoint(model->getVisibleKincs()[i].x() * zoom, model->getVisibleKincs()[i].y() * zoom + yEltolas), kincsKinezetek[1].toImage().scaled(zoom, zoom, Qt::IgnoreAspectRatio));
            }else if (model->getPalyaMatrix()[model->getVisibleKincs()[i].x()][model->getVisibleKincs()[i].y()] == 4){
                painter.drawImage(QPoint(model->getVisibleKincs()[i].x() * zoom, model->getVisibleKincs()[i].y() * zoom + yEltolas), kincsKinezetek[2].toImage().scaled(zoom, zoom, Qt::IgnoreAspectRatio));
            }
        }
    }

    if(showMap)
    {
        painter.setPen(QPen(Qt::black, 0));
        painter.setBrush(Qt::black);
        for (int x=0; x< model->getPalyaMatrix().size(); ++x) {
            for (int y=0; y< model->getPalyaMatrix().size(); ++y) {
                if (model->getPalyaMatrix()[x][y] == 0){
                    painter.drawRect(x*zoom, y*zoom + yEltolas, 1*zoom, 1*zoom);
                }else{
                    painter.drawRect(x*zoom, y*zoom + yEltolas, 1*zoom, 1*zoom);
                }
            }
        }

        //érintett útak
        for(int i = 0; i < model->getUtvonal().size(); ++i)
        {
            painter.drawImage(QPoint(model->getUtvonal()[i].x() * zoom, model->getUtvonal()[i].y() * zoom + yEltolas),labnyomIMAGE);
        }
    }

    //játékos
    painter.drawImage(QPoint(model->getJatekos().x()*zoom, model->getJatekos().y()*zoom + yEltolas), jatekosKinez);

    //egyebek
    ui->statusbar->showMessage(QString::number(model->getIdo()));
    setFixedSize(model->getN()*zoom, model->getN()*zoom + 2*yEltolas);
}

void View::keyPressEvent(QKeyEvent * event)
{
    switch(event->key()) {
        case Qt::Key_A:
            ujNezes = Left;
            ujEmber();
            model->move(Left);
            break;
        case Qt::Key_W:
            ujNezes = Up;
            ujEmber();
            model->move(Up);
            break;
        case Qt::Key_S:
            ujNezes = Down;
            ujEmber();
            model->move(Down);
            break;
        case Qt::Key_D:
            ujNezes = Right;
            ujEmber();
            model->move(Right);
            break;
        case Qt::Key_Left:
            ujNezes = Left;
            model->move(Left);
            break;
        case Qt::Key_Up:
            ujNezes = Up;
            ujEmber();
            model->move(Up);
            break;
        case Qt::Key_Down:
            ujNezes = Down;
            ujEmber();
            model->move(Down);
            break;
        case Qt::Key_Right:
            ujNezes = Right;
            ujEmber();
            model->move(Right);
            break;
        case Qt::Key_P:
            model->pauseAndPlay();
            break;
        case Qt::Key_M:
            openMap();
            break;
        case Qt::Key_N:
            model->newGame();
            break;
        case Qt::Key_F4:
            zoomSzamlalo += 50;
            break;
        case Qt::Key_F3:
            zoomSzamlalo -= 50;
            break;
        case Qt::Key_F1:
            musicplayer->tooLoud();
            break;
        case Qt::Key_F2:
            musicplayer->tooQuiet();
            break;
    }
}

void View::win()
{
    model->calculatescore();
    int finalscore = model->getScore();

    int terkepek = 0;
    for (int i=0; i<model->getMegVanATerkep().size(); ++i){
        if (model->getMegVanATerkep()[i]) {
            terkepek += 1;
        }
    }

    int maradekkincsek = model->getKincs().size();
    int osszeskincs = 4 * (((model->getN()*model->getN())/1600)+1) ;

    QMessageBox msg;
    msg.setText("Gratulálok, megtaláltad a kiutat!\n"
                "Megtalált térképek: " + QString::number(terkepek) + "/4\n"
                "Megtalált kincsek: " + QString::number(osszeskincs-maradekkincsek) + "/" + QString::number(osszeskincs) + "\n"
                "Az időd: " + QString::number(model->getIdo()) + "\n"
                "Összpontszámod: " + QString::number(finalscore));
    msg.exec();
    model->newGame();
}

void View::ujJatek()
{
    model->newGame();
}

void View::ujTizJatek()
{
    model->setN(10);
    model->newGame();
}

void View::ujHuszJatek()
{
    model->setN(20);
    model->newGame();
}

void View::ujHarmincJatek()
{
    model->setN(30);
    model->newGame();
}

void View::ujNegyvenJatek()
{
    model->setN(40);
    model->newGame();
}

void View::ujOtvenJatek()
{
    model->setN(50);
    model->newGame();
}

void View::ujHatvanJatek()
{
    model->setN(60);
    model->newGame();
}

void View::ujHetvenJatek()
{
    model->setN(70);
    model->newGame();
}

void View::ujNyolcvanJatek()
{
    model->setN(80);
    model->newGame();
}

void View::ujKilencvenJatek()
{
    model->setN(90);
    model->newGame();
}

void View::ujSzazJatek()
{
    model->setN(100);
    model->newGame();
}

void View::ujSzazTizJatek()
{
    model->setN(110);
    model->newGame();
}

void View::ujSzazHuszJatek()
{
    model->setN(120);
    model->newGame();
}

void View::ujSzazHarmincJatek()
{
    model->setN(130);
    model->newGame();
}

void View::ujSzazNegyvenJatek()
{
    model->setN(140);
    model->newGame();
}

void View::ujSzazOtvenJatek()
{
    model->setN(150);
    model->newGame();
}

void View::seeEverything()
{
    isLightsOn = true;
}

void View::seeNothing()
{
    isLightsOn = false;
}

void View::ujEmber()
{
    if(ujNezes == Right) melyikEmber = (melyikEmber + 1) % 4 + 0;
    else if(ujNezes == Down ) melyikEmber = (melyikEmber + 1) % 4 + 4;
    else if(ujNezes == Left ) melyikEmber = (melyikEmber + 1) % 4 + 8;
    else if(ujNezes == Up   ) melyikEmber = (melyikEmber + 1) % 4 + 12;

    update();
}

void View::ujMap()
{
    melyikMap = (melyikMap + 1) % 4;
    update();
}

void View::showHelp()
{
    QMessageBox msg;
    msg.setText("A játék használata:\n\nA mozgáshoz a nyilakat, vagy a WASD gombokat lehet használni.\n\nHa megszereztél egy térkép darabot, akkor onnantól ha a pálya azon negyedén érintettél egy pontot, akkor az M gombbal tudod megnézni.\n\nAz N gombbal új játékot lehet indítani a jelenlegi pályamérettel\n\nAz F1 és F2 billentyűkkel lehet a zene hangerejét változtatni.\n\n Az F3 és F4 billentyűkkel pedig az ablak méretét");
    msg.exec();
}

void View::openMap()
{
    showMap = !showMap;
    update();
}



