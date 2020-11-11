#include "model.h"
#include <QDebug>

Model::Model(QObject *parent) : QObject(parent)
{
    qsrand(time(NULL));
    n = 40;
    gamesPlayed = 0;
    if(gamesPlayed == 1) newGame();
    newGame();
    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(azIdoSzall()));

    refreshJatekos = new QTimer(this);
    refreshJatekos->start(150);
    connect(refreshJatekos, SIGNAL(timeout()), this, SLOT(changePali()));
}

int Model::getN() const
{
    return n;
}

QVector<QPoint> Model::getFal() const
{
    return fal;
}

QVector<QPoint> Model::getTalaj() const
{
    return talaj;
}

QVector<QPoint> Model::getUres() const
{
    return ures;
}

QVector<QPoint> Model::getTerkep() const
{
    return terkep;
}

QVector<QVector<int>> Model::getPalyaMatrix() const
{
    return palyamatrix;
}


void Model::alapAllapot(int n)
{
    for(int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            QPoint p = QPoint(i, j);
            ures.push_back(p);
        }
    }
    megVanATerkep.clear();
    megVanATerkep.push_back(false);
    megVanATerkep.push_back(false);
    megVanATerkep.push_back(false);
    megVanATerkep.push_back(false);
    terkep.clear();
    utvonal.clear();
    tavoliFal.clear();
    tavoliTalaj.clear();
    visibleMaps.clear();
}

void Model::labirintusCsinalo()
{
    talaj.push_back(QPoint(0, 0));
    talaj.push_back(QPoint(0, n-1));
    talaj.push_back(QPoint(n-1, 0));
    talaj.push_back(QPoint(n-1, n-1));
    ures.removeOne(QPoint(0, 0));
    ures.removeOne(QPoint(0, n-1));
    ures.removeOne(QPoint(n-1, 0));
    ures.removeOne(QPoint(n-1, n-1));
    while(ures.size() != 0)
    {
        int index = qrand() % ures.size();
        QPoint p = ures.at(index);
        int dir = qrand() % 2; //0 fel/le   |   1 jobb/bal
        dir = falas(p, dir);
        if(dir == 0)
        {// fel/le
            fuggoleges(p);
        }
        else if(dir == 1) //------------------------------------------------------------------------------------------------------
        {
            vizszintes(p);
        }
    }
    palyamatrix.resize(n);
    for ( int i = 0 ; i < n ; ++i ) {
        palyamatrix[i].resize(n);
    }
    Matrixcsinalo();
}

void Model::terkepgeneralo()
{
    terkepdarab = 2;    //gyök 4

    QVector<QPoint> negyed0;
    QVector<QPoint> negyed1;
    QVector<QPoint> negyed2;
    QVector<QPoint> negyed3;
    //bal felső sarok
    for (int i=0; i<talaj.size(); ++i)
    {
        if (talaj[i].x()<(n/terkepdarab) && talaj[i].y()<(n/terkepdarab))
        {
            negyed0.push_back(talaj[i]);
        }
    }

    int terkep0_index = qrand()%negyed0.size();
    terkep.push_back(negyed0[terkep0_index]);

    //jobb felső sarok
    for (int i=0; i<talaj.size(); ++i)
    {
        if (talaj[i].x()>(n/terkepdarab) && talaj[i].y()<(n/terkepdarab))
        {
            negyed1.push_back(talaj[i]);
        }
    }
    int terkep1_index = qrand()%negyed1.size();
    while(negyed1[terkep1_index] == QPoint(n-1, 0)) {
        terkep1_index = qrand()%negyed1.size();
    }

    terkep.push_back(negyed1[terkep1_index]);

    //bal alsó sarok
    for (int i=0; i<talaj.size(); ++i)
    {
        if (talaj[i].x()<(n/terkepdarab) && talaj[i].y()>(n/terkepdarab))
        {
            negyed2.push_back(talaj[i]);
        }
    }
    int terkep2_index = qrand()%negyed2.size();
    while(negyed2[terkep2_index] == QPoint(0, n-1)) {
        terkep2_index = qrand()%negyed2.size();
    }
    terkep.push_back(negyed2[terkep2_index]);

    //jobb alsó sarok
    for (int i=0; i<talaj.size(); ++i)
    {
        if (talaj[i].x()>(n/terkepdarab) && talaj[i].y()>(n/terkepdarab))
        {
            negyed3.push_back(talaj[i]);
        }
    }

    int terkep3_index = qrand()%negyed3.size();
    terkep.push_back(negyed3[terkep3_index]);

}

void Model::Matrixcsinalo()
{ 
    int falak = 0;        //matrixban a fal koordinátákat 0-val
    int talajok = 1;      //matrixban a talaj koordinátákat 1-el

    for (int i=0; i<fal.size(); ++i)
    {
        palyamatrix[fal[i].x()][fal[i].y()] = (falak);
    }
    for (int i=0; i<talaj.size(); ++i)
    {
        palyamatrix[talaj[i].x()][talaj[i].y()] = (talajok);
    }
}

void Model::fuggoleges(QPoint p)
{// fel/le
    int x = p.x();
    int y = p.y();
    QPoint fenti = p;
    QPoint lenti = p;
    QPoint tmp = fenti;
    while(fenti.y() != -1 && ures.contains(fenti))
    {
        tmp = fenti;
        fenti.setY(fenti.y()-1);
    }
    fenti = tmp;
    while(lenti.y() != n && ures.contains(lenti))
    {
        tmp = lenti;
        lenti.setY(lenti.y()+1);
    }
    lenti = tmp;
    //ha átjáróba futunk
    QPoint fentiFeletti = QPoint(fenti.x(), fenti.y()-1);
    QPoint lentiAlatti  = QPoint(lenti.x(), lenti.y()+1);
    if(talaj.contains(fentiFeletti)) //fenti feletti átjáró, önmaga nincs benne
    {
        if(fenti != p)
        {
            talaj.push_back(fenti);
            ures.removeOne(fenti);
        }
        for(int i = fenti.y()+1; i < y; ++i)
        {
            fal.push_back(QPoint(x, i));
            ures.removeOne(QPoint(x, i));
        }
    }
    else                             //FELfele feltöltjük fallal
    {
        for(int i = fenti.y(); i < y; ++i)
        {
            fal.push_back(QPoint(x, i));
            ures.removeOne(QPoint(x, i));
        }
    }
    if(talaj.contains(lentiAlatti))  //lenti alatti átjáró, önmaga nincs benne
    {
        if(fenti != p)
        {
            talaj.push_back(lenti);
            ures.removeOne(lenti);
        }
        for(int i = y+1; i <= lenti.y()-1; ++i)
        {
            fal.push_back(QPoint(x, i));
            ures.removeOne(QPoint(x, i));
        }
    }
    else
    {
        for(int i = y+1; i <= lenti.y(); ++i)
        {
            fal.push_back(QPoint(x, i));
            ures.removeOne(QPoint(x, i));
        }
    }
    //önmaga, és szélsőségek
    if(talaj.contains(fentiFeletti) || talaj.contains(lentiAlatti))
    {//van kék
        if(p.y() == fenti.y() && talaj.contains(fentiFeletti))
        {
            talaj.push_back(p);
            ures.removeOne(p);
        }
        else if(p.y() == lenti.y() && talaj.contains(lentiAlatti))
        {
            talaj.push_back(p);
            ures.removeOne(p);
        }
        else
        {
            fal.push_back(p);
            ures.removeOne(p);
        }
    }
    else                             //nincs kék
    {
        talaj.push_back(p);
        ures.removeOne(p);
    }
    //sarkok színezése
    QPoint balFelso  = QPoint(fenti.x()-1, fenti.y());
    QPoint jobbFelso = QPoint(fenti.x()+1, fenti.y());
    QPoint balAlso   = QPoint(lenti.x()-1, lenti.y());
    QPoint jobbAlso  = QPoint(lenti.x()+1, lenti.y());
    if(ures.contains(balFelso)/* && !ures.contains(QPoint(balFelso.x(), balFelso.y()-1))*/)
    {
        talaj.push_back(balFelso);
        ures.removeOne(balFelso);
    }
    if(ures.contains(jobbFelso)/* && !ures.contains(QPoint(jobbFelso.x(), jobbFelso.y()-1))*/)
    {
        talaj.push_back(jobbFelso);
        ures.removeOne(jobbFelso);
    }
    if(ures.contains(balAlso)/* && !ures.contains(QPoint(balAlso.x(), balAlso.y()+1))*/)
    {
        talaj.push_back(balAlso);
        ures.removeOne(balAlso);
    }
    if(ures.contains(jobbAlso)/* && !ures.contains(QPoint(jobbAlso.x(), jobbAlso.y()+1))*/)
    {
        talaj.push_back(jobbAlso);
        ures.removeOne(jobbAlso);
    }
    //kettő közötti sávot beszínezzük
    for(int i = fenti.y(); i <= lenti.y(); ++i)
    {
        QPoint ottani = QPoint(x, i);
        QPoint kettoJobbra = QPoint(ottani.x()+2, ottani.y());
        QPoint kettoBalra = QPoint(ottani.x()-2, ottani.y());
        QPoint egyJobbra = QPoint(ottani.x()+1, ottani.y());
        QPoint egyBalra = QPoint(ottani.x()-1, ottani.y());
        if(!ures.contains(kettoBalra) && ures.contains(egyBalra))
        {
            talaj.push_back(egyBalra);
            ures.removeOne(egyBalra);
        }
        if(!ures.contains(kettoJobbra) && ures.contains(egyJobbra))
        {
            talaj.push_back(egyJobbra);
            ures.removeOne(egyJobbra);
        }
    }
}

void Model::vizszintes(QPoint p)
{// jobb/bal
    int x = p.x();
    int y = p.y();
    QPoint bal = p;
    QPoint jobb = p;
    QPoint tmp = bal;
    while(bal.x() != -1 && ures.contains(bal))
    {
        tmp = bal;
        bal.setX(bal.x()-1);
    }
    bal = tmp;
    while(jobb.x() != n && ures.contains(jobb))
    {
        tmp = jobb;
        jobb.setX(jobb.x()+1);
    }
    jobb = tmp;
    //ha átjáróba futunk
    QPoint baltolBalra = QPoint(bal.x()-1, bal.y());
    QPoint jobbtolJobbra  = QPoint(jobb.x()+1, jobb.y());
    if(talaj.contains(baltolBalra))
    {//baltól balra átjáró, önmaga nincs benne
        if(bal != p)
        {
            talaj.push_back(bal);
            ures.removeOne(bal);
        }
        for(int i = bal.x()+1; i < x; ++i)
        {
            fal.push_back(QPoint(i, y));
            ures.removeOne(QPoint(i, y));
        }
    }
    else
    {//BALfele feltöltjük fallal
        for(int i = bal.x(); i < x; ++i)
        {
            fal.push_back(QPoint(i, y));
            ures.removeOne(QPoint(i, y));
        }
    }
    if(talaj.contains(jobbtolJobbra))  //jobb alatti átjáró, önmaga nincs benne
    {
        if(jobb != p)
        {
            talaj.push_back(jobb);
            ures.removeOne(jobb);
        }
        for(int i = x+1; i < jobb.x(); ++i)
        {
            fal.push_back(QPoint(i, y));
            ures.removeOne(QPoint(i, y));
        }
    }
    else
    {
        for(int i = x+1; i <= jobb.x(); ++i)
        {
            fal.push_back(QPoint(i, y));
            ures.removeOne(QPoint(i, y));
        }
    }
    //önmaga, és szélsőségek
    if(talaj.contains(baltolBalra) || talaj.contains(jobbtolJobbra))
    {//van kék
        if(p.x() == bal.x() && talaj.contains(baltolBalra))
        {
            talaj.push_back(p);
            ures.removeOne(p);
        }
        else if(p.x() == jobb.x() && talaj.contains(jobbtolJobbra))
        {
            talaj.push_back(p);
            ures.removeOne(p);
        }
        else
        {
            fal.push_back(p);
            ures.removeOne(p);
        }
    }
    else                             //nincs kék
    {
        talaj.push_back(p);
        ures.removeOne(p);
    }

    //sarkok színezése
    QPoint felsoBal  = QPoint(bal.x(), bal.y()-1);
    QPoint alsoBal = QPoint(bal.x(), bal.y()+1);
    QPoint felsoJobb   = QPoint(jobb.x(), jobb.y()-1);
    QPoint alsoJobb  = QPoint(jobb.x(), jobb.y()+1);
    if(ures.contains(felsoBal)/* && !ures.contains(QPoint(felsoBal.x()-1, felsoBal.y()))*/)
    {
        talaj.push_back(felsoBal);
        ures.removeOne(felsoBal);
    }
    if(ures.contains(alsoBal)/* && !ures.contains(QPoint(alsoBal.x()-1, alsoBal.y()))*/)
    {
        talaj.push_back(alsoBal);
        ures.removeOne(alsoBal);
    }
    if(ures.contains(felsoJobb)/* && !ures.contains(QPoint(felsoJobb.x()+1, felsoJobb.y()))*/)
    {
        talaj.push_back(felsoJobb);
        ures.removeOne(felsoJobb);
    }
    if(ures.contains(alsoJobb)/* && !ures.contains(QPoint(alsoJobb.x()+1, alsoJobb.y()))*/)
    {
        talaj.push_back(alsoJobb);
        ures.removeOne(alsoJobb);
    }
    //kettő közötti sávot beszínezzük
    for(int i = bal.x(); i <= jobb.y(); ++i)
    {
        QPoint ottani = QPoint(i, y);
        QPoint kettoJobbra = QPoint(ottani.x(), ottani.y()+2);
        QPoint kettoBalra = QPoint(ottani.x(), ottani.y()-2);
        QPoint egyJobbra = QPoint(ottani.x(), ottani.y()+1);
        QPoint egyBalra = QPoint(ottani.x(), ottani.y()-1);
        if(!ures.contains(kettoBalra) && ures.contains(egyBalra))
        {
            talaj.push_back(egyBalra);
            ures.removeOne(egyBalra);
        }
        if(!ures.contains(kettoJobbra) && ures.contains(egyJobbra))
        {
            talaj.push_back(egyJobbra);
            ures.removeOne(egyJobbra);
        }
    }

}

int Model::falas(QPoint p, int dir)
{//0 fel/le     |     1 jobb/bal
    int x = p.x();
    int y = p.y();
    int kettoEgyben = 0;
    if(y == 0 || y == n-1)
    {
        return 0;
    }
    else if(x == 0 || x == n-1)
    {
        return 1;
    }
    //vízszintesen
    for(int i = -1; i <= 1; ++i)
    {
        if(fal.contains(QPoint( x+i, y-1 ))) ++kettoEgyben;
    }
    if(kettoEgyben >= 2)
    {
        return 0;
    }
    kettoEgyben = 0;
    for(int i = -1; i <= 1; ++i)
    {
        if(fal.contains(QPoint( x+i, y+1 ))) ++kettoEgyben;
    }
    if(kettoEgyben >= 2)
    {
        return 0;
    }
    //függőleges
    kettoEgyben = 0;
    for(int i = -1; i <= 1; ++i)
    {
        if(fal.contains(QPoint( x-1, y+i ))) ++kettoEgyben;
    }
    if(kettoEgyben >= 2)
    {
        return 1;
    }
    kettoEgyben = 0;
    for(int i = -1; i <= 1; ++i)
    {
        if(fal.contains(QPoint( x+1, y+i ))) ++kettoEgyben;
    }
    if(kettoEgyben >= 2)
    {
        return 1;
    }
    return dir;
}

QPoint Model::getJatekos() const
{
    return jatekos;
}

void Model::move(Direction dir)
{
    if(canMove)
    {
        switch(dir) {
            case Left:
                if(jatekos.x() > 0 && !fal.contains(QPoint(jatekos.x()-1, jatekos.y()))) jatekos.setX(jatekos.x()-1);
                break;
            case Right:
                if(jatekos.x() < n-1 && !fal.contains(QPoint(jatekos.x()+1, jatekos.y()))) jatekos.setX(jatekos.x()+1);
                break;
            case Up:
                if(jatekos.y() > 0 && !fal.contains(QPoint(jatekos.x(), jatekos.y()-1))) jatekos.setY(jatekos.y()-1);
                break;
            case Down:
                if(jatekos.y() < n-1 && !fal.contains(QPoint(jatekos.x(), jatekos.y()+1))) jatekos.setY(jatekos.y()+1);
                break;
        }
    }
    if(jatekos.x() < n/terkepdarab && jatekos.y() < n/terkepdarab && megVanATerkep[0] )
    {
        voltamMarItt();
    }
    else if(jatekos.x() >= n/terkepdarab && jatekos.y() < n/terkepdarab && megVanATerkep[1] )
    {
        voltamMarItt();
    }
    else if(jatekos.x() < n/terkepdarab && jatekos.y() >= n/terkepdarab && megVanATerkep[2] )
    {
        voltamMarItt();
    }
    else if(jatekos.x() >= n/terkepdarab && jatekos.y() >= n/terkepdarab && megVanATerkep[3] )
    {
        voltamMarItt();
    }
    tavoliFal.clear();
    tavoliTalaj.clear();
    visibleMaps.clear();

    messzeseg();
    tester();
    refreshVisibleMaps();
    update();
}

QVector<QPoint> Model::getLatas() const
{
    return latas;
}

void Model::messzeseg()
{

    latas.clear();
    latasDEfal.clear();
    nemLatas.clear();
    lataster.clear();

    int x = jatekos.x();
    int y = jatekos.y();

    QPoint bf = QPoint(x-1, y-1); //bal     felső
    QPoint kf = QPoint(x  , y-1); //középső felső
    QPoint jf = QPoint(x+1, y-1); //jobb    felső
    QPoint bs = QPoint(x-1, y  ); //bal     semmi
    QPoint js = QPoint(x+1, y  ); //jobb    semmi
    QPoint ba = QPoint(x-1, y+1); //bal     alsó
    QPoint ka = QPoint(x  , y+1); //középső alsó
    QPoint ja = QPoint(x+1, y+1); //jobb    alsó

    //Marci
    for(int i = jatekos.x()-2; i <= jatekos.x()+2; ++i)
    {
        for(int j = jatekos.y()-2; j <= jatekos.y()+2; ++j)
        {
            lataster.push_back(QPoint(i, j));
        }
    }
    for(int i = -1; i <= 1; ++i)
    {
        if(fal.contains(QPoint(x+i, y-1))) latasDEfal.push_back(QPoint(x+i, y-1));
        else latas.push_back(QPoint(x+i, y-1));
    }
    for(int i = -1; i <= 1; ++i)
    {
        if(fal.contains(QPoint(x+i, y+1))) latasDEfal.push_back(QPoint(x+i, y+1));
        else latas.push_back(QPoint(x+i, y+1));
    }
    if(fal.contains(QPoint(x-1, y))) latasDEfal.push_back(QPoint(x-1, y));
    else latas.push_back(QPoint(x-1, y));
    if(fal.contains(QPoint(x+1, y))) latasDEfal.push_back(QPoint(x+1, y));
    else latas.push_back(QPoint(x+1, y));
    if(latasDEfal.contains(bf))
    {
        nemLatas.push_back(QPoint(bf.x()-1, bf.y()-1));
        nemLatas.push_back(QPoint(bf.x()  , bf.y()-1));
        nemLatas.push_back(QPoint(bf.x()-1, bf.y()  ));
    }
    if(latasDEfal.contains(jf))
    {
        nemLatas.push_back(QPoint(jf.x()+1, jf.y()-1));
        nemLatas.push_back(QPoint(jf.x()  , jf.y()-1));
        nemLatas.push_back(QPoint(jf.x()+1, jf.y()  ));
    }
    if(latasDEfal.contains(ba))
    {
        nemLatas.push_back(QPoint(ba.x()-1, ba.y()+1));
        nemLatas.push_back(QPoint(ba.x()  , ba.y()+1));
        nemLatas.push_back(QPoint(ba.x()-1, ba.y()  ));
    }
    if(latasDEfal.contains(ja))
    {
        nemLatas.push_back(QPoint(ja.x()+1, ja.y()+1));
        nemLatas.push_back(QPoint(ja.x()  , ja.y()+1));
        nemLatas.push_back(QPoint(ja.x()+1, ja.y()  ));
    }
    if(latasDEfal.contains(kf))
    {
        nemLatas.push_back(QPoint(kf.x(), kf.y()-1));
        if(!nemLatas.contains(QPoint(kf.x()-1, kf.y()-1))) nemLatas.push_back(QPoint(kf.x()-1, kf.y()-1));
        if(!nemLatas.contains(QPoint(kf.x()+1, kf.y()-1))) nemLatas.push_back(QPoint(kf.x()+1, kf.y()-1));
    }
    if(latasDEfal.contains(ka))
    {
        nemLatas.push_back(QPoint(ka.x(), ka.y()+1));
        if(!nemLatas.contains(QPoint(ka.x()-1, ka.y()+1))) nemLatas.push_back(QPoint(ka.x()-1, ka.y()+1));
        if(!nemLatas.contains(QPoint(ka.x()+1, ka.y()+1))) nemLatas.push_back(QPoint(ka.x()+1, ka.y()+1));
    }
    if(latasDEfal.contains(bs))
    {
        nemLatas.push_back(QPoint(bs.x()-1, bs.y()));
        if(!nemLatas.contains(QPoint(bs.x()-1, bs.y()-1))) nemLatas.push_back(QPoint(bs.x()-1, bs.y()-1));
        if(!nemLatas.contains(QPoint(bs.x()-1, bs.y()+1))) nemLatas.push_back(QPoint(bs.x()-1, bs.y()+1));
    }
    if(latasDEfal.contains(js))
    {
        nemLatas.push_back(QPoint(js.x()+1, js.y()));
        if(!nemLatas.contains(QPoint(js.x()+1, js.y()-1))) nemLatas.push_back(QPoint(js.x()+1, js.y()-1));
        if(!nemLatas.contains(QPoint(js.x()+1, js.y()+1))) nemLatas.push_back(QPoint(js.x()+1, js.y()+1));
    }
    for(int i = jatekos.x()-2; i <= jatekos.x()+2; ++i)
    {
        for(int j = jatekos.y()-2; j <= jatekos.y()+2; ++j)
        {
            if(!(latas.contains(QPoint(i, j)) || nemLatas.contains(QPoint(i, j)) || latasDEfal.contains(QPoint(i, j))))
            {
            if(fal.contains(QPoint(i, j))) tavoliFal.push_back(QPoint(i, j));
            else tavoliTalaj.push_back(QPoint(i, j));
            }
        }
    }



    //y
    if(jatekos.y() == 0)
    {
        for(int i = -2; i <= 2; ++i)
        {
            nemLatas.removeOne(QPoint(jatekos.x()+i, -2));
            nemLatas.removeOne(QPoint(jatekos.x()+i, -1));
            latas.removeOne(QPoint(jatekos.x()+i, -2));
            latas.removeOne(QPoint(jatekos.x()+i, -1));
            tavoliTalaj.removeOne(QPoint(jatekos.x()+i, -2));
            tavoliTalaj.removeOne(QPoint(jatekos.x()+i, -1));
            tavoliFal.removeOne(QPoint(jatekos.x()+i, -2));
            tavoliFal.removeOne(QPoint(jatekos.x()+i, -1));
        }
    }
    else if(jatekos.y() == 1)
    {
        for(int i = -2; i <= 2; ++i)
        {
            nemLatas.removeOne(QPoint(jatekos.x()+i, -1));
            latas.removeOne(QPoint(jatekos.x()+i, -1));
            tavoliFal.removeOne(QPoint(jatekos.x()+i, -1));
            tavoliTalaj.removeOne(QPoint(jatekos.x()+i, -1));
        }
    }
    if(jatekos.y() == n-2)
    {
        for(int i = -2; i <= 2; ++i)
        {
            nemLatas.removeOne(QPoint(jatekos.x()+i, n));
            latas.removeOne(QPoint(jatekos.x()+i, n));
            tavoliFal.removeOne(QPoint(jatekos.x()+i, n));
            tavoliTalaj.removeOne(QPoint(jatekos.x()+i, n));
        }
    }
    else if(jatekos.y() == n-1)
    {
        for(int i = -2; i <= 2; ++i)
        {
            nemLatas.removeOne(QPoint(jatekos.x()+i, n));
            nemLatas.removeOne(QPoint(jatekos.x()+i, n+1));
            latas.removeOne(QPoint(jatekos.x()+i, n));
            latas.removeOne(QPoint(jatekos.x()+i, n+1));
            tavoliTalaj.removeOne(QPoint(jatekos.x()+i, n));
            tavoliTalaj.removeOne(QPoint(jatekos.x()+i, n+1));
            tavoliFal.removeOne(QPoint(jatekos.x()+i, n));
            tavoliFal.removeOne(QPoint(jatekos.x()+i, n+1));
        }
    }
    //x
    if(jatekos.x() == 0)
    {
        for(int i = -2; i <= 2; ++i)
        {
            nemLatas.removeOne(QPoint(-1, jatekos.y()+i));
            nemLatas.removeOne(QPoint(-2, jatekos.y()+i));
            latas.removeOne(QPoint(-1, jatekos.y()+i));
            latas.removeOne(QPoint(-2, jatekos.y()+i));
            tavoliFal.removeOne(QPoint(-1, jatekos.y()+i));
            tavoliFal.removeOne(QPoint(-2, jatekos.y()+i));
            tavoliTalaj.removeOne(QPoint(-1, jatekos.y()+i));
            tavoliTalaj.removeOne(QPoint(-2, jatekos.y()+i));
        }
    }
    else if(jatekos.x() == 1)
    {
        for(int i = -2; i <= 2; ++i)
        {
            nemLatas.removeOne(QPoint(-1, jatekos.y()+i));
            latas.removeOne(QPoint(-1, jatekos.y()+i));
            tavoliTalaj.removeOne(QPoint(-1, jatekos.y()+i));
            tavoliFal.removeOne(QPoint(-1, jatekos.y()+i));
        }
    }
    if(jatekos.x() == n-3)
    {
        for(int i = -2; i <= 2; ++i)
        {
            nemLatas.removeOne(QPoint(n, jatekos.y()+i));
            nemLatas.removeOne(QPoint(n+1, jatekos.y()+i));
            latas.removeOne(QPoint(n, jatekos.y()+i));
            latas.removeOne(QPoint(n+1, jatekos.y()+i));
            tavoliTalaj.removeOne(QPoint(n, jatekos.y()+i));
            tavoliTalaj.removeOne(QPoint(n+1, jatekos.y()+i));
            tavoliFal.removeOne(QPoint(n, jatekos.y()+i));
            tavoliFal.removeOne(QPoint(n+1, jatekos.y()+i));
        }
    }
    else if(jatekos.x() == n-2)
    {
        for(int i = -2; i <= 2; ++i)
        {
            nemLatas.removeOne(QPoint(n, jatekos.y()+i));
            latas.removeOne(QPoint(n, jatekos.y()+i));
            tavoliFal.removeOne(QPoint(n, jatekos.y()+i));
            tavoliTalaj.removeOne(QPoint(n, jatekos.y()+i));
        }
    }
}

void Model::tester()
{
    if(jatekos == QPoint(n-1, 0)) win();
    if(jatekos == terkep[0]) { megVanATerkep[0] = true; voltamMarItt(); }
    else if(jatekos == terkep[1]) { megVanATerkep[1] = true; voltamMarItt(); }
    else if(jatekos == terkep[2]) { megVanATerkep[2] = true; voltamMarItt(); }
    else if(jatekos == terkep[3]) { megVanATerkep[3] = true; voltamMarItt(); }
}

void Model::newGame()
{
    jatekos = QPoint(0, n-1);
    canMove = true;
    ido = 0;
    ures.clear();
    fal.clear();
    talaj.clear();
    latas.clear();
    alapAllapot(n);
    labirintusCsinalo();
    terkepgeneralo();
    messzeseg();
    ++gamesPlayed;
    refreshVisibleMaps();
    update();
}

QVector<QPoint> Model::getNemLatas() const
{
    return nemLatas;
}

void Model::setN(int value)
{
    n = value;
}

int Model::getIdo() const
{
    return ido;
}

void Model::pauseAndPlay()
{
    if(timer->isActive())
    {
        canMove = false;
        timer->stop();
    }
    else
    {
        canMove = true;
        timer->start(1000);
    }
    if(refreshJatekos->isActive())
    {
        refreshJatekos->stop();
    }
    else
    {
        refreshJatekos->start(250);
    }
}

void Model::voltamMarItt()
{
    utvonal.push_back(jatekos);
}

void Model::refreshVisibleMaps()
{
    for(int i = 0; i < terkep.size(); ++i)
    {
        if(abs(jatekos.x() - terkep[i].x()) == 1 && abs(jatekos.y() - terkep[i].y()) == 1)
        {
            visibleMaps.push_back(terkep[i]);
        }
    }


    for(int i = 0; i < terkep.size(); ++i)
    {
        if(latasDEfal.contains(terkep[i]) || latas.contains(terkep[i]) || tavoliFal.contains(terkep[i]) || tavoliTalaj.contains(terkep[i]))
        {
            visibleMaps.push_back(terkep[i]);
        }
    }
}

void Model::azIdoSzall()
{
    ++ido;
    update();
}

void Model::changePali()
{
    masikat();
}

QVector<QPoint> Model::getVisibleMaps() const
{
    return visibleMaps;
}

QVector<QPoint> Model::getTavoliFal() const
{
    return tavoliFal;
}

QVector<QPoint> Model::getTavoliTalaj() const
{
    return tavoliTalaj;
}

QVector<QPoint> Model::getUtvonal() const
{
    return utvonal;
}

QVector<QPoint> Model::getLataster() const
{
    return lataster;
}

QVector<QPoint> Model::getLatasDEfal() const
{
    return latasDEfal;
}


