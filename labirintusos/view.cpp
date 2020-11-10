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

    //load Pixmaps
    //player(s)
    jatekosPixmap = QPixmap("pics/jatekos.png");
    jatekosKinezetek.push_back(QPixmap("pics/jobb1.png"));
    jatekosKinezetek.push_back(QPixmap("pics/jobb2.png"));
    jatekosKinezetek.push_back(QPixmap("pics/jobb3.png"));
    jatekosKinezetek.push_back(QPixmap("pics/jobb4.png"));
    //walls
    fal2 = QPixmap("pics/fal02.png");
    tavoliFal = QPixmap("pics/gigasotet.png");
    //grounds
    talaj2 = QPixmap("pics/talaj2.png");
    tavoliTalaj = QPixmap("pics/setettalaj.png");
    //map
    terkepPIXMAP = QPixmap("pics/terkep.png");

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
    connect(model, SIGNAL(lebeg()), this, SLOT(ujMap()));
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

    if(isLightsOn)
    {
        //falak
        for(int i = 0; i < model->getFal().size(); ++i)
        {
            painter.drawImage(QPoint(model->getFal()[i].x() * zoom, model->getFal()[i].y() * zoom + yEltolas), fal2Img);
        }
        //talaj
        for(int i = 0; i < model->getTalaj().size(); ++i)
        {
            painter.drawImage(QPoint(model->getTalaj()[i].x() * zoom, model->getTalaj()[i].y() * zoom + yEltolas), talaj2Img);
        }
        //térkép
        for(int i = 0; i < model->getTerkep().size(); ++i)
        {
            painter.drawImage(QPoint(model->getTerkep()[i].x() * zoom, model->getTerkep()[i].y() * zoom + yEltolas), terkepesIMAGE);
        }
    }
    else
    {
        //falak
        painter.setPen(QPen(Qt::black, 0));
        painter.setBrush(Qt::black);
        for(int i = 0; i < model->getFal().size(); ++i)
        {
            painter.drawRect(model->getFal()[i].x() * zoom, model->getFal()[i].y() * zoom + yEltolas, 1 * zoom, 1 * zoom);
        }
        //talaj
        painter.setPen(QPen(Qt::black, 0));
        painter.setBrush(Qt::black);
        for(int i = 0; i < model->getTalaj().size(); ++i)
        {
            painter.drawRect(model->getTalaj()[i].x() * zoom, model->getTalaj()[i].y() * zoom + yEltolas, 1 * zoom, 1 * zoom);
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
    }

    if(showMap)
    {
        //falak
        painter.setPen(QPen(Qt::black, 0));
        painter.setBrush(Qt::black);
        for(int i = 0; i < model->getFal().size(); ++i)
        {
            painter.drawRect(model->getFal()[i].x() * zoom, model->getFal()[i].y() * zoom + yEltolas, 1 * zoom, 1 * zoom);
        }
        //talaj
        painter.setPen(QPen(Qt::black, 0));
        painter.setBrush(Qt::black);
        for(int i = 0; i < model->getTalaj().size(); ++i)
        {
            painter.drawRect(model->getTalaj()[i].x() * zoom, model->getTalaj()[i].y() * zoom + yEltolas, 1 * zoom, 1 * zoom);
        }
        //érintett útak
        painter.setBrush(Qt::blue);
        for(int i = 0; i < model->getUtvonal().size(); ++i)
        {
            painter.drawRect(model->getUtvonal()[i].x() * zoom, model->getUtvonal()[i].y() * zoom + yEltolas, 1 * zoom, 1 * zoom);
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
            model->move(Left);
            break;
        case Qt::Key_W:
            model->move(Up);
            break;
        case Qt::Key_S:
            model->move(Down);
            break;
        case Qt::Key_D:
            model->move(Right);
            break;
        case Qt::Key_Left:
            model->move(Left);
            break;
        case Qt::Key_Up:
            model->move(Up);
            break;
        case Qt::Key_Down:
            model->move(Down);
            break;
        case Qt::Key_Right:
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
        case Qt::Key_F2:
            musicplayer->tooQuiet();
    }
}

void View::win()
{
    QMessageBox msg;
    msg.setText("Gratulálok, megtaláltad a kiutat!");
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

void View::writeIntoFile()
{
    QFile file("korabbiLabirintusok.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        // We're going to streaming text to the file
        QTextStream stream(&file);
        for(int i = 0; i < model->getN(); ++i)
        {
            QString line = "";
            for(int j = 0; j < model->getN(); ++j)
            {
                line += model->getFal().contains(QPoint(j, i)) ? "1 " : "0 ";
            }
            line += "\n";
            stream << line;
        }
        stream << "------------------------------------------------------------------------------------\n";
        file.close();
        qDebug() << "Writing finished";
    }
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
    melyikEmber = (melyikEmber + 1) % 4;
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



