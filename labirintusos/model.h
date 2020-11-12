#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QPoint>
#include <QTimer>
#include <QVector>

enum Direction{Left, Right, Up, Down};

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    int getN() const;

    int getIdo() const;

    QPoint getJatekos() const;

    QVector<QPoint> getFal() const;

    QVector<QPoint> getTalaj() const;

    QVector<QPoint> getUres() const;

    QVector<QPoint> getTerkep() const;

    QVector<QPoint> getKincs() const;

    QVector<QPoint> getLatas() const;

    QVector<QPoint> getNemLatas() const;

    QVector<QPoint> getLatasDEfal() const;

    QVector<QPoint> getLataster() const;

    QVector<QVector<int>> getPalyaMatrix() const;

    void Matrixcsinalo();

    void targygeneralo ();

    void alapAllapot(int n);

    void labirintusCsinalo();

    void fuggoleges(QPoint);

    void vizszintes(QPoint);

    int egyDarabFoglalt(QPoint);

    int kettoDarabFoglalt(QPoint);

    int falas(QPoint, int);

    void move(Direction dir);

    void messzeseg();

    void tester();

    void newGame();

    void setN(int value);

    void pauseAndPlay();

    void voltamMarItt();

    void refreshVisibleMaps();

    void refreshVisibleKincs();

    QVector<QPoint> getUtvonal() const;

    QVector<QPoint> getTavoliTalaj() const;

    QVector<QPoint> getTavoliFal() const;

    QVector<QPoint> getVisibleMaps() const;

    QVector<QPoint> getVisibleKincs() const;

signals:
    void update();
    void win();
    void masikat();

public slots:
    void azIdoSzall();
    void changePali();

private:
    int n;
    int gamesPlayed;
    QVector<QPoint> ures;
    QVector<QPoint> fal;
    QVector<QPoint> talaj;
    QVector<QVector<int>> palyamatrix;
    QVector<QPoint> terkep;
    QVector<QPoint> kincs;
    QVector<bool>   megVanATerkep;
    QVector<QPoint> utvonal;
    QPoint jatekos;
    bool canMove;
    QVector<QPoint> latas;
    QVector<QPoint> nemLatas;
    QTimer* timer;
    QTimer* refreshJatekos;
    QVector<QPoint> visibleMaps;
    QVector<QPoint> visibleKincs;
    int ido;
    //------------------
    QVector<QPoint> latasDEfal;
    QVector<QPoint> lataster;
    QVector<QPoint> tavoliTalaj;
    QVector<QPoint> tavoliFal;
    int terkepdarab; // gyok 4
};

#endif // MODEL_H
