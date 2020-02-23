#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QPixmap>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      timer (new QTimer),
      seattleTime(new QTimer),
      londonTime(new QTimer),
      japanTime(new QTimer),

      httpManager(new HTTPManager)
{
    ui->setupUi(this);

    connect(timer, SIGNAL(timeout()),
            this, SLOT(changeBackground()));
    connect(seattleTime, SIGNAL(timeout()),
            this, SLOT(setSeattleTime()));
    connect(londonTime, SIGNAL(timeout()),
            this, SLOT(setLondonTime()));
    connect(japanTime, SIGNAL(timeout()),
            this, SLOT(setjapanTime()));

    imageInput();
    setSeattleTime();
    timer->start(10000);
    changeBackground();



    //map signal
    connect(httpManager, SIGNAL(ImageReady(QPixmap *)),
            this, SLOT(processImage(QPixmap *)));
    //weather signal
    connect(httpManager, SIGNAL(WeatherJsonReady(QJsonObject *)),
            this, SLOT(processWeatherJson(QJsonObject *)));
    //icon signal
    connect(httpManager, SIGNAL(IconReady(QPixmap *)),
            this, SLOT(processIcon(QPixmap *)));






}

MainWindow::~MainWindow()
{
    delete ui;
}



// getting map picture
void MainWindow::processImage(QPixmap *image)
{
    ui->mapLabel->setPixmap(*image);
}

// getting weather info
void MainWindow::processWeatherJson(QJsonObject *json)
{
      QString weather =  json->value("weather").toArray()[0].toObject()["main"].toString();
      QString getIcon = json->value("weather").toArray()[0].toObject()["icon"].toString();
      httpManager->sendIconRequest(getIcon);
      double temp = json->value("main").toObject()["temp"].toDouble();


      ui->conditionLabel->setText(weather);
      ui->tempLabel->setText(QString::number(temp) + "°F");


}

void MainWindow::processIcon(QPixmap *icon)
{
    ui->iconLabel->setPixmap(*icon);
}


//set timezone seattle
void MainWindow::setSeattleTime()
{
    QTime time = QTime::currentTime();
    QString hour = time.toString("hh");
    QString minute = time.toString("mm");
    QString second = time.toString("ss");
    ui->hourLcd->display(hour);
    ui->minuteLcd->display(minute);
    ui->secondLcd->display(second);
    if(!seattleTime->isActive()){
    seattleTime->start(1000);
    }
    if(londonTime->isActive()){
        londonTime->stop();
    }
    if(japanTime->isActive()){
        japanTime->stop();
    }
}

//set timezone london
void MainWindow::setLondonTime()
{
    QTime time = QTime::currentTime().addSecs(28800);
    QString hour = time.toString("hh");
    QString minute = time.toString("mm");
    QString second = time.toString("ss");
    ui->hourLcd->display(hour);
    ui->minuteLcd->display(minute);
    ui->secondLcd->display(second);
    if(!londonTime->isActive()){
        londonTime->start(1000);
    }
    if(seattleTime->isActive()){
        seattleTime->stop();
    }
    if(japanTime->isActive()){
        japanTime->stop();
    }

}

//set timezone jakarta
void MainWindow::setjapanTime()
{
    QTime time = QTime::currentTime().addSecs(-32400);
    QString hour = time.toString("hh");
    QString minute = time.toString("mm");
    QString second = time.toString("ss");
    ui->hourLcd->display(hour);
    ui->minuteLcd->display(minute);
    ui->secondLcd->display(second);
    if(!japanTime->isActive()){
        japanTime->start(1000);
    }
    if(seattleTime->isActive()){
        seattleTime->stop();
    }
    if(londonTime->isActive()){
        londonTime->stop();
    }
}


void MainWindow::on_seattleButton_clicked()
{
    picCount = 0;
    setSeattleTime();
    changeBackground();
    timer->stop();
    timer->start(10000);




}

void MainWindow::on_japanButton_clicked()
{
    picCount = 0;
    setjapanTime();
    changeBackground();
    timer->stop();
    timer->start(10000);

}

void MainWindow::on_londonButton_clicked()
{
    picCount = 0;
    setLondonTime();
    changeBackground();
    timer->stop();
    timer->start(10000);


}

void MainWindow::changeBackground()
{
  if(seattleTime->isActive()){
      if(picCount == 0){
          ui->backgroundLabel->setPixmap(seattle);
      } else if (picCount == 1){
         ui->backgroundLabel->setPixmap(seattle1);
      } else if (picCount == 2){
          ui->backgroundLabel->setPixmap(seattle2);
      } else if (picCount == 3){
          ui->backgroundLabel->setPixmap(seattle3);
      } else if (picCount == 4) {
          ui->backgroundLabel->setPixmap(seattle4);
      } else if (picCount == 5) {
          ui->backgroundLabel->setPixmap(seattle5);
      }
      picCount++;
      if(picCount == 6){
          picCount = 0;
      }

  }
  if (londonTime->isActive()){

      if(picCount == 0){
          ui->backgroundLabel->setPixmap(london);
      } else if (picCount == 1){
         ui->backgroundLabel->setPixmap(london1);
      } else if (picCount == 2){
          ui->backgroundLabel->setPixmap(london2);
      } else if (picCount == 3){
          ui->backgroundLabel->setPixmap(london3);
      } else if (picCount == 4) {
          ui->backgroundLabel->setPixmap(london4);
      } else if (picCount == 5) {
          ui->backgroundLabel->setPixmap(london5);
      }
      picCount++;
      if(picCount == 6){
          picCount = 0;
      }
  }
  if(japanTime->isActive()){
      if(picCount == 0){
          ui->backgroundLabel->setPixmap(japan);
      } else if (picCount == 1){
         ui->backgroundLabel->setPixmap(japan1);
      } else if (picCount == 2){
          ui->backgroundLabel->setPixmap(japan2);
      } else if (picCount == 3){
          ui->backgroundLabel->setPixmap(japan3);
      } else if (picCount == 4) {
          ui->backgroundLabel->setPixmap(japan4);
      } else if (picCount == 5) {
          ui->backgroundLabel->setPixmap(japan5);
      }
      picCount++;
      if(picCount == 6){
          picCount = 0;
      }
  }

}



void MainWindow::on_locationButton_clicked()
{
    QString zip = ui->zipCodeEdit->text();

    httpManager->mapRequest(zip);
    httpManager->sendWeatherRequest(zip);

}



void MainWindow::imageInput(){
    QString imageJapan = ":/images/japan1.jpg";
    QString imageJapan1 = ":/images/japan2.jpg";
    QString imageJapan2 = ":/images/japan3.jpeg";
    QString imageJapan3 = ":/images/japan4.jpg";
    QString imageJapan4 = ":/images/japan5.jpeg";
    QString imageJapan5 = ":/images/japan6.jpg";
    QString imageLondon = ":/images/london1.jpg";
    QString imageLondon1 = ":/images/london2.jpg";
    QString imageLondon2 = ":/images/london3.jpg";
    QString imageLondon3 = ":/images/london4.jpg";
    QString imageLondon4 = ":/images/london5.jpg";
    QString imageLondon5 = ":/images/london6.jpg";
    QString imageSeattle = ":/images/seattle1.jpg";
    QString imageSeattle1 = ":/images/seattle2.jpg";
    QString imageSeattle2 = ":/images/seattle3.jpg";
    QString imageSeattle3 = ":/images/seattle4.jpg";
    QString imageSeattle4 = ":/images/seattle5.jpg";
    QString imageSeattle5 = ":/images/seattle6.jpg";

    if(japan.load(imageJapan)){
        japan = japan.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
    }
    if(japan1.load(imageJapan1)){
        japan1 = japan1.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
    }
    if(japan2.load(imageJapan2)){
        japan2 = japan2.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
    }
    if(japan3.load(imageJapan3)){
        japan3 = japan3.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
    }
    if(japan4.load(imageJapan4)){
        japan4 = japan4.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
    }
    if(japan5.load(imageJapan5)){
        japan5 = japan5.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
    }
    if(london.load(imageLondon)){
       london = london.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
    }
    if(london1.load(imageLondon1)){
        london1 = london1.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
     }
    if(london2.load(imageLondon2)){
       london2 = london2.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
    }
    if(london3.load(imageLondon3)){
       london3 = london3.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
    }
    if(london4.load(imageLondon4)){
       london4 = london4.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
    }
    if(london5.load(imageLondon5)){
       london5 = london5.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
    }
    if (seattle.load(imageSeattle)){
        seattle = seattle.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
    }
    if (seattle1.load(imageSeattle1)){
        seattle1 = seattle1.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
    }
    if (seattle2.load(imageSeattle2)){
        seattle2 = seattle2.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
    }
    if (seattle3.load(imageSeattle3)){
        seattle3 = seattle3.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
    }
    if (seattle4.load(imageSeattle4)){
        seattle4 = seattle4.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
    }
    if (seattle5.load(imageSeattle5)){
        seattle5 = seattle5.scaled(ui->backgroundLabel->size(), Qt::KeepAspectRatioByExpanding);
    }


}











