#include <progSeq.h>

void setup() {

  Serial.begin(115200);
  initRobot();

  screen(" Bonjour \n Je m'appelle \n Sebastien"); // display things

  waitForButton(); // wait for button

  calibrate(); // turn on 180 degrees to get the values of sensors for black and white floor

  beepOn(); // make a beep
  screen(" Beep ");
  delay(200); // wait 200ms
  beepOff(); // stop the beep
  delay(500);
  beepOn(); // make a beep
  screen(" Boop ");
  delay(200); // wait 200ms
  beepOff(); // stop the beep
  delay(500);

  confirmCalibration();
  screen(" \n C'est parti !");

}
int leftSpeed = 30, rightSpeed = 30, i = 0, retour = false, varcomp = 0, start = true, x = 0, Verif_Tab = true, z = 0, q = 0;
byte Direction[60], action[20], recu[60];

void loop() {

// retour permet d'identifier si on se trouve dans la première ou deuxième phase

  if (retour == false) {
    Direction[0] = 0;

    if (millis() > 10000 && millis() < 15000 && retour == false) {    //IL PARLE !!!!!!!!
      screen("\n    Vous allez bien?");
    }
    else if (millis() > 30000 && millis() < 35000 && retour == false) {
      screen("\n    C'est long nan?");
    }

    setSpeed(leftSpeed, rightSpeed);
    readSensors();

// followline

    if (getSensor(3) > 600) {     // ajuste la trajectoire vers la droite
      rightSpeed = 20;
      setSpeed(leftSpeed, rightSpeed);
      delay(50);
      rightSpeed = 30;
    }
    if (getSensor(1) > 600) {     // ajuste la trajectoire vers la gauche
      leftSpeed = 20;
      setSpeed(leftSpeed, rightSpeed);
      delay(50);
      leftSpeed = 30;
    }


    readSensors(); // on rafraichît les sensors

// Gestion de la direction : Nord = 0; Est = 1 ; Sud = 2 ; Ouest = 3
// Nord est la direction dans laquelle part le robot.
// x est l'indice du tableau Direction
// On tourne en priorité à droite, ensuite on va tout droit, puis finalement à gauche

    if (getSensor(4) > 600 && getSensor(3) > 600 ) { // On repère à droite
      varcomp += 1;                                 // ici on tourne à droite, donc on incrémente varcomp ( la direction) de 1. On tourne dans le sens horaire.
      x += 1;                                       // on déplace l'indice du tableau
      if (varcomp == 4) {
        varcomp = 0; // remise à 0, on travaille en modulo 4 sur les directions
      }

      Direction[x] = varcomp; // Tableau des directions qu'a prit le robot


      // on s'éloigne du virage afin que le robot ne détécte pas autre chose directement après avoir tourné
      
      setSpeed(30, 30);
      delay(300);         
      setSpeed(40, -40);        // on fait un 90 degré vers la droite
      delay(550);               // puis on s'éloigne du virage
      setSpeed(30, 30);
      delay(200);

      // on fait avancer le robot pendant 1 secondes après avoir tourner pour le stabiliser
      
      while (i == 10) {                     // chrono de 1 secondes pour suivre la ligne avec le followline
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
        i += 1;                           // i est la variable de temps en dixièmes de seconde
      }
      i = 0; 

    }
    // Bloc gauche et tout droit
    
    else if ( getSensor(0) > 600 && getSensor(1) > 600 && getSensor(4) < 600 ) {
      setSpeed(30, 30);
      delay(300);
      readSensors();
      
      // tout droit
      
      if (getSensor(2) > 600) { // On détecte tout droit donc on va tout droit.
        x += 1;                 
        Direction[x] = varcomp;     // on ajoute à Direction la même direction que l'on suivait

        while (i == 10) {
          readSensors();
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
          i += 1;
        }
        i = 0;
      }

          // on va à gauche
      
      else if (getSensor(2) < 600) {
        varcomp -= 1;              // on décrémente de 1. On tourne dans le sens anti-horaire 
        x += 1;

        if (varcomp == -1) {
          varcomp = 3;
        }
        Direction[x] = varcomp;

        setSpeed(-40, 40);        // on fait un 90 degré vers la gauche
        delay(550);
        setSpeed(30, 30);
        delay(200);
        
        while (i == 10) {
          readSensors();
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
          i += 1;
        }
        i = 0;
      }
    }

    // Demi-tour
    
    else if (getSensor(4) < 600 && getSensor(3) < 600 && getSensor(2) < 600 && getSensor(1) < 600 && getSensor(0) < 600) {
      x += 1;
      varcomp += 2;               // on indique dans le tableau la direction opposée
      if (varcomp == 4) {
        varcomp = 0;
      }
      else if (varcomp == 5) {
        varcomp = 1;
      }
      Direction[x] = varcomp;

      setSpeed(40, -40); // on fait un 180 degré pour le demi-tour
      delay(1000);

    }

// détection de la sortie est symbolisée par un obstacle

    if ( getDistance() < 8) {
      setSpeed(0, 0);
      retour = true;            // on a atteint la sortie du labyrinthe. On sort alors de la première phase, on met donc retour à True.
      screen("\n    Terminus");
      waitForButton();          // le robot attend une pression sur le bouton avant de repartir

    }

  }
  // On sort du Bloc permettant de traiter la première phase
  // Deuxième phase : le robot doit suivre le chemin le plus court afin de sortir du labyrinthe
  
  else {

   // Traitement du tableau (cf rapport)
    if (Verif_Tab == true) {
      for (int p = 0; p <= 60; p++) {

        if ((Direction[p] == 0 && Direction[p + 1] == 2) || (Direction[p] == 2 && Direction[p + 1] == 0)) {
          Direction[p] = 5;                                                                                 // on remplace les valeurs non voulues par 5
          Direction[p + 1] = 5;
          p = p - 2 ;
        }
        else if ((Direction[p] == 1 && Direction[p + 1] == 3) || (Direction[p] == 3 && Direction[p + 1] == 1)) {
          Direction[p] = 5;
          Direction[p + 1] = 5;
          p = p - 2 ;
        }
        
        for (int y = 0; y < 60; y++) {     // On recopie le tableau dans un nouveau tableau sans les 5
          if (Direction[y] != 5) {
            recu[z] = Direction[y];
            z += 1;
          }

        }
        z = 0;

        for (int e = 0; e < 60; e++) {    // on remplace l'ancien tableau par le nouveau puis on lui fait refaire le traitement jusqu'à ce qu'il n'y ait plus de valeurs non voulue.
          Direction[e] = recu[e];
        }

      }
      Verif_Tab = false;       // une fois le tableau traité on sort de la condition
    }

//Une fois le tableau traité, on force le robot à suivre les directions 

    setSpeed(leftSpeed,rightSpeed);
   
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


readSensors();

   // On force le robot à suivre une direction indiquée par deux cases consécutives de Direction
   // Par exemple : 0 et 1 correspond à un virage à droite, 0 et 3 à un virage à gauche
   // q est un indice de Direction
   
if (getSensor(0) > 600 || getSensor(4) > 600) {

    if (Direction[q] == 0) {

      if (Direction[q + 1] == 1) {
        setSpeed(30, 30);
        delay(300);
        setSpeed(40, -40);  // tourne à droite
        delay(550);
        setSpeed(30, 30);
        delay(300);
      }
      else if (Direction[q + 1] == 3) {
        setSpeed(30, 30);
        delay(300);
        setSpeed(-40, 40);    // tourne à gauche
        delay(550);
        setSpeed(30, 30);
        delay(300);
      }
      else if (Direction[q + 1] == 0) {
        setSpeed (30, 30);  // tout droit
        delay(400);
      }

    }
    else if (Direction[q] == 1) {

      if (Direction[q + 1] == 1) {
        setSpeed (30, 30);          // tout droit
        delay(400);
      }
      else if (Direction[q + 1] == 2) {
        setSpeed(30, 30);
        delay(300);
        setSpeed(40, -40);    // tourne à droite
        delay(550);
        setSpeed(30, 30);
        delay(300);
      }
      else if (Direction[q + 1] == 0) {
        setSpeed(30, 30);
        delay(300);
        setSpeed(-40, 40);       // tourne à gauche
        delay(550);
        setSpeed(30, 30);
        delay(300);
      }

    }
    else if (Direction[q] == 2) {

      if (Direction[q + 1] == 1) {
        setSpeed(30, 30);
        delay(300);
        setSpeed(-40, 40);      // tourne à gauche
        delay(550);
        setSpeed(30, 30);
        delay(300);
      }
      else if (Direction[q + 1] == 2) {
        setSpeed (30, 30);        // tout droit
        delay(300);
      }
      else if (Direction[q + 1] == 3) {
        setSpeed(30, 30);
        delay(300);
        setSpeed(40, -40);        // tourne à droite
        delay(550);
        setSpeed(30, 30);
        delay(300);
      }

    }
    else if (Direction[q] == 3) {

      if (Direction[q + 1] == 2) {
        setSpeed(30, 30);
        delay(300);
        setSpeed(-40, 40);      // tourne à gauche
        delay(550);
        setSpeed(30, 30);
        delay(300);
      }
      else if (Direction[q + 1] == 3) {
        setSpeed (30, 30);          // tout droit
        delay(300);
      }
      else if (Direction[q + 1] == 0) {
        setSpeed(30, 30);
        delay(300);
        setSpeed(40, -40);        // tourne à droite
        delay(550);
        setSpeed(30, 30);
        delay(300);
      }

    }
q = q + 1; 
  }
  else if (getDistance() < 8) {       // sortie du labyrinthe
    setSpeed(0, 0);
    screen("\n    vrai Terminus");
    waitForButton();


  }
  }

}
