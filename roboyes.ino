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

  confirmCalibration(); // wait for button while displaying line position info

  screen("Go !"); // display things
}
int maxSpeed = 80, p = 0, wheelSpeed = 40;   //on initialise trois variables différentes auxquelles on donne des valeurs entières:la variable maxSpeed que l'on met à 80 qui correspond à la vitesse de référence du robot,
                                    //une variable minSpeed à 40 (la moitié de la vitesse maxSpeed) qui nous servira à modifier seulement la vitesse des roues et une variable p à 0 que l'on utilisera pour savoir si la première ligne a été détectée

void loop() {
  int wait = false;

  followLine(maxSpeed); // on utilise la fonction followLine dans laquelle on met la variable maxSpeed donc le robot suivra automatiquement la ligne noire à la vitesse maxSpeed

  readSensors(); // update sensors value

  // read the values that has just been updated, and stop at some conditions
  
  if (getSensor(0) > 500 && getSensor(4) > 500) {  //on regarde si du noir est détecté à la fois par le capteur gauche et le droit, ce qui signifie qu'une ligne est détectée
    setSpeed(0, 0);                                //si oui, on se sert de la fonction setSpeed pour mettre les valeurs des vitesses des roues pour que le robot s'arrête
    delay(1000);                                   //le robot attend pendant une seconde
    setSpeed(wheelSpeed, wheelSpeed);              //on relance le robot en mettant cette fois les vitesses des roues à la valeur de a
    delay(500);

    if (p == 1) {
      maxSpeed = 80;
      wheelSpeed = 80;
      p = 0;
    }
    else {
      maxSpeed = 40;
      p = 1;
    }
    readSensors();

  }
  if (getSensor(2) < 500 && getSensor(0) < 500 && getSensor(4) < 500 && getSensor(1) < 500 && getSensor(3) < 500) {  //on regarde si les 5 tableurs détectent du blanc, ce qui correspond à la fin de la feuille
    setSpeed(0, 0);                                                                    //si oui, on met la vitesse des roues (grâce à setSpeed) et la vitesse maximale (maxSpeed) à 0 pour que le robot s'arrête
    maxSpeed = 0;                                                                      //si le robot est arrêté, c'est la fin du programme

  }
}
