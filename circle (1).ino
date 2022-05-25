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


int rightSpeed = 30, leftSpeed = 30, i = 0;        //on initialise trois variables différentes auxquelles on donne des valeurs entières: les vitesses des roues que l'on met à 40 , ainsi que i à 0 qui nous servira à compter
                                                   //le nombre de fois qu'un obstacle est détecté par le robot

void loop() {

  setSpeed(leftSpeed, rightSpeed);
  readSensors();                                  //met à jour les valeurs des capteurs

  if (getSensor(0) > 700 || getSensor(1) > 700) { //on regarde si du noir est détecté à gauche, c'est-à-dire quand le robot touche un bord du cercle à gauche
    rightSpeed = 10;                              //si oui, on modifie la valeur de la vitesse de la roue droite de sorte qu'elle soit inférieure à celle de la roue gauche
    leftSpeed = 30;
    setSpeed(leftSpeed, rightSpeed);              //on utilise de nouveau setSpeed avec les nouvelles valeurs afin de faire tourner le robot vers la droite
    delay(400);
    leftSpeed = 30;                               //on remet ensuite les valeurs des vitesses des roues à la valeur initiale 40
    rightSpeed = 30;
  }
  if (getSensor(4) > 700 || getSensor(3) > 700) { //on regarde si du noir est détecté à droite, c'est-à-dire quand le robot touche un bord du cercle à droite
    leftSpeed = 10;                               //si oui, on modifie la valeur de la vitesse de la roue gauche de sorte qu'elle soit inférieure à celle de la roue droite
    rightSpeed = 30;
    setSpeed(leftSpeed, rightSpeed);              //on utilise de nouveau setSpeed avec les nouvelles valeurs afin de faire tourner le robot vers la gauche
    delay(400);
    leftSpeed = 30;                               //on remet ensuite les valeurs des vitesses des roues à la valeur initiale 40
    rightSpeed = 30;
  }
  if ( getDistance() < 8) {                       //on regarde, en utilisant getDistance() qui utilise les capteurs ultrasons, si un obstacle est détecté à une distance de moins de 8cm
    i += 1;                                       //si oui, on incrémente la variable i de 1
    
    if (i == 2) {                                 //on regarde si la valeur de i est égale à 2, ce qui correspond à la deuxième détection de l'obstacle souhaitée
      leftSpeed = 0;                              //si oui, on change les valeurs des vitesses des deux roues que l'on met à 0
      rightSpeed = 0;
      setSpeed(leftSpeed, rightSpeed);            //on utilise de nouveau setSpeed avec les nouvelles valeurs pour que le robot s'arrête
      delay(500000);                              //on se sert d'un delay avec un temps très élevé pour que le robot ne reparte pas
    }

    setSpeed(50, -50);                            //dans le cas où un obstacle est détecté qu'une seule fois (i=1), on utilise setSpeed en mettant la roue gauche à une vitesse de 50 
    delay(800);                                   //et la roue droite à une vitesse de -50 pour que le robot effectue un demi-tour sur place
  }

}
