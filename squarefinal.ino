#include <progSeq.h>
void setup() {

  Serial.begin(115200);
  initRobot();

  screen("Waveshare\nAlphabot2\nPress to calibrate"); // display things

  waitForButton(); // wait for button

  calibrate(); // turn on 180 degrees to get the values of sensors for black and white floor

  beepOn(); // make a beep
  delay(200); // wait 200ms
  beepOff(); // stop the beep

  confirmCalibration();
  screen("Go !");

}
int maxSpeed = 50, i = 0, p[30] = {0}, o, leftSpeed = 30, rightSpeed = 30, varbalance = true, t = 0, m, rightend, leftend;

void loop() {

  m = millis();  //le temps depuis le lancement du programme 
  if (i == 10 ) {
    readSensors();
    setSpeed(leftSpeed, rightSpeed);

// followline: cette partie du code permet la direction du robot s'il détecte une ligne à gauche ou à droite
    if (getSensor(3) > 600) {     
      rightSpeed = 20;
      setSpeed(leftSpeed, rightSpeed);
      delay(50);
      rightSpeed = 30;
    }
    else if (getSensor(1) > 600) {
      leftSpeed = 20;
      setSpeed(leftSpeed, rightSpeed);
      delay(50);
      leftSpeed = 30;
    }
//condition permettant l'arrêt au centre du rectangle
    if ( p[9] + ((p[2] - p[1]) / 2) < m) {      //on regarde si le temps au 9ème caractère, qui est le dernier virage avant d'aller au centre, plus le temps au milieu du dernier coté
      setSpeed(0, 0);                           //est inférieur au temps depuis le début, si oui, on fait s'arrêter le robot
                                                //il est alors au milieu du dernier coté
      delay(1000);
      setSpeed(leftend, rightend);              //on fait ensuite avancer le robot dans le carré
      delay(500);
      setSpeed(30, 30);
      delay((p[4] - p[3]) / 2);                  //cette intervalle de temps correspond au milieu du duxième coté
      setSpeed(40, -40);
      delay(3000);
      setSpeed(0, 0);                            //le robot s'arrête au centre du carré
      screen("      Finito");
      delay(60000);
    }


  }
  else {

    setSpeed(leftSpeed, rightSpeed);
    readSensors();


    if (getSensor(3) > 600) {     // followline
      rightSpeed = 20;
      setSpeed(leftSpeed, rightSpeed);
      delay(50);
      rightSpeed = 30;
    }
    if (getSensor(1) > 600) {
      leftSpeed = 20;
      setSpeed(leftSpeed, rightSpeed);
      delay(50);
      leftSpeed = 30;
    }

//virage à droite
    if (getSensor(4) > 600 && getSensor(3) > 600 ) {    //on regarde si le robot détecte à droite
      rightend = -40;
      leftend = 40;
      p[i] = millis();      //on fait correspondre la valeur i du tableau p au temps 
      i += 1;               //chaque valeur de i correspond à la position du robot, il est incrémentait à l'entrée du virage
      setSpeed(30, 30);
      delay(200);
      setSpeed(40, -40);    //le robot tourne vers la droite
      delay(500);
      setSpeed(30, 30);
      p[i] = millis();     //on prend la valeur suivante dans le tableau p que l'on fait correspondre au nouveau temps
      i += 1;              //puis on incrémente de nouveau i pour changer passer au caractère suivnat dans le tableau

      while (t == 10) {   //cette fonction while permet au robot de suivre la ligne pendant une seconde
        readSensors();    //cela permet d'éviter au robot d'effectuer de nouveau un virage s'il détecte mal quelque chose
        if (getSensor(3) > 600) {
          rightSpeed = 20;
          setSpeed(leftSpeed, rightSpeed);
          delay(100);
          rightSpeed = 30;
        }
        if (getSensor(1) > 600) {
          leftSpeed = 20;
          setSpeed(leftSpeed, rightSpeed);
          delay(100);

          leftSpeed = 30;
        }
        t += 1;
      }
      t = 0;

      readSensors();
    }
//virage à gauche
    else if ( getSensor(0) > 600 && getSensor(1) > 600 && getSensor(4) < 600 ) {    //on effectue la même chose ici mais pour le virage à gauche
      rightend = 40;
      leftend = -40;
      p[i] = millis();
      i += 1;
      setSpeed(30, 30);
      delay(200);
      setSpeed(-40, 40);
      delay(500);
      setSpeed(30, 30);
      p[i] = millis();
      i += 1;

      while (t == 10) {     //cette fonction while permet au robot de suivre la ligne pendant une seconde
        readSensors();      //cela permet d'éviter au robot d'effectuer de nouveau un virage s'il détecte mal quelque chose
        if (getSensor(3) > 600) {
          rightSpeed = 20;
          setSpeed(leftSpeed, rightSpeed);
          delay(100);
          rightSpeed = 30;
        }
        if (getSensor(1) > 600) {
          leftSpeed = 20;
          setSpeed(leftSpeed, rightSpeed);
          delay(100);
          leftSpeed = 30;
        }
        t += 1;
      }
      t = 0;
    }

    readSensors();

  }
}
