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
int rs = 30, ls = 30, i = 0, retour = false, varcomp = 0, start = true, tourne = false, x = 0, memoire, whilemem = false, truc = true, z = 0, q = 0;
byte t[60], action[20], recu[60];

void loop() {

  if (retour == false) {
    t[0] = 0;

    /*if (millis() > 10000 && millis() < 15000 && retour == false) {    //IL PARLE !!!!!!!!
      screen("\n    Vous allez bien?");
    }
    else if (millis() > 30000 && millis() < 35000 && retour == false) {
      screen("\n    C'est long nan?");
    }*/





    setSpeed(rs, ls);
    readSensors();


    if (getSensor(3) > 600) {     // followline
      ls = 20;
      setSpeed(rs, ls);
      delay(50);
      ls = 30;
    }
    if (getSensor(1) > 600) {
      rs = 20;
      setSpeed(rs, ls);
      delay(50);
      rs = 30;
    }


    readSensors();


    if (getSensor(4) > 600 && getSensor(3) > 600 ) {
      varcomp += 1;
      x += 1;
      if (varcomp == 4) {
        varcomp = 0;
      }

      t[x] = varcomp;



      setSpeed(30, 30);
      delay(300);
      setSpeed(40, -40);
      delay(550);
      setSpeed(30, 30);
      delay(200);

      while (i == 10) {
        if (getSensor(3) > 600) {
          ls = 20;
          setSpeed(rs, ls);
          delay(100);
          ls = 30;
        }
        if (getSensor(1) > 600) {
          rs = 20;
          setSpeed(rs, ls);
          delay(100);

          rs = 30;
        }
        i += 1;
      }
      i = 0;

      readSensors();
    }
    else if ( getSensor(0) > 600 && getSensor(1) > 600 && getSensor(4) < 600 ) {
      setSpeed(30, 30);
      delay(300);
      readSensors();
      if (getSensor(2) > 600) {
        x += 1;
        t[x] = varcomp;





        while (i == 10) {
          if (getSensor(3) > 600) {
            ls = 20;
            setSpeed(rs, ls);
            delay(100);
            ls = 30;
          }
          if (getSensor(1) > 600) {
            rs = 20;
            setSpeed(rs, ls);
            delay(100);
            rs = 30;
          }
          i += 1;
        }
        i = 0;
      }
      else if (getSensor(2) < 600) {
        varcomp -= 1;
        x += 1;


        if (varcomp == -1) {
          varcomp = 3;
        }
        t[x] = varcomp;


        //gauche



        setSpeed(-40, 40);
        delay(550);
        setSpeed(30, 30);
        delay(200);
        while (i == 10) {
          if (getSensor(3) > 600) {
            ls = 20;
            setSpeed(rs, ls);
            delay(100);
            ls = 30;
          }
          if (getSensor(1) > 600) {
            rs = 20;
            setSpeed(rs, ls);
            delay(100);
            rs = 30;
          }
          i += 1;
        }
        i = 0;
      }
    }
    else if (getSensor(4) < 600 && getSensor(3) < 600 && getSensor(2) < 600 && getSensor(1) < 600 && getSensor(0) < 600) {
      x += 1;
      varcomp += 2;
      if (varcomp == 4) {
        varcomp = 0;
      }
      else if (varcomp == 5) {
        varcomp = 1;
      }
      t[x] = varcomp;

      setSpeed(40, -40);
      delay(1100);

    }






    if ( getDistance() < 8) {
      setSpeed(0, 0);
      retour = true;
      screen("\n    Terminus");
      waitForButton();

    }

  }
  else {
    
    
    if (truc == true) {
      for (int p = 0; p <= 60; p++) {

        if ((t[p] == 0 && t[p + 1] == 2) || (t[p] == 2 && t[p + 1] == 0)) {
          t[p] = 5;
          t[p + 1] = 5;
          p = p - 2 ;
        }
        else if ((t[p] == 1 && t[p + 1] == 3) || (t[p] == 3 && t[p + 1] == 1)) {
          t[p] = 5;
          t[p + 1] = 5;
          p = p - 2 ;
        }


        for (int y = 0; y < 60; y++) {
          if (t[y] != 5) {
            recu[z] = t[y];
            z += 1;
          }

        }
        z = 0;

        for (int e = 0; e < 60; e++) {
          t[e] = recu[e];
        }

      }
      truc = false;
    }



    setSpeed(rs,ls);
    
    readSensors();
    
    if (getSensor(3) > 600) {     // followline
      ls = 20;
      setSpeed(rs, ls);
      delay(50);
      ls = 30;
    }
    if (getSensor(1) > 600) {
      rs = 20;
      setSpeed(rs, ls);
      delay(50);
      rs = 30;
    }


readSensors();
    
if (getSensor(0) > 600 || getSensor(4) > 600) {

    if (t[q] == 0) {

      if (t[q + 1] == 1) {
        setSpeed(30, 30);
        delay(300);
        setSpeed(40, -40);
        delay(550);
        setSpeed(30, 30);
        delay(300);
      }
      else if (t[q + 1] == 3) {
        setSpeed(30, 30);
        delay(300);
        setSpeed(-40, 40);
        delay(550);
        setSpeed(30, 30);
        delay(300);
      }
      else if (t[q + 1] == 0) {
        setSpeed (30, 30);
        delay(400);
      }

    }
    else if (t[q] == 1) {

      if (t[q + 1] == 1) {
        setSpeed (30, 30);
        delay(400);
      }
      else if (t[q + 1] == 2) {
        setSpeed(30, 30);
        delay(300);
        setSpeed(40, -40);
        delay(550);
        setSpeed(30, 30);
        delay(300);
      }
      else if (t[q + 1] == 0) {
        setSpeed(30, 30);
        delay(300);
        setSpeed(-40, 40);
        delay(550);
        setSpeed(30, 30);
        delay(300);
      }

    }
    else if (t[q] == 2) {

      if (t[q + 1] == 1) {
        setSpeed(30, 30);
        delay(300);
        setSpeed(-40, 40);
        delay(550);
        setSpeed(30, 30);
        delay(300);
      }
      else if (t[q + 1] == 2) {
        setSpeed (30, 30);
        delay(400);
      }
      else if (t[q + 1] == 3) {
        setSpeed(30, 30);
        delay(300);
        setSpeed(40, -40);
        delay(550);
        setSpeed(30, 30);
        delay(300);
      }

    }
    else if (t[q] == 3) {

      if (t[q + 1] == 2) {
        setSpeed(30, 30);
        delay(300);
        setSpeed(-40, 40);
        delay(550);
        setSpeed(30, 30);
        delay(300);
      }
      else if (t[q + 1] == 3) {
        setSpeed (30, 30);
        delay(400);
      }
      else if (t[q + 1] == 0) {
        setSpeed(30, 30);
        delay(300);
        setSpeed(40, -40);
        delay(550);
        setSpeed(30, 30);
        delay(300);
      }

    }
q = q + 1;
  }
  else if (getDistance() < 8) {
    setSpeed(0, 0);
    screen("\n    vrai Terminus");
    waitForButton();


  }
  }

}
