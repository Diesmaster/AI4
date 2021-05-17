
//
// nnskelet.cc
// C++-programma voor neuraal netwerk (NN) met \'e\'en output-knoop
// TODO: 1,2,3,4,5,6,7
// Zie www.liacs.leidenuniv.nl/~kosterswa/AI/nnhelp.pdf
// 21 april 2021
// Compileren: g++ -Wall -O2 -o nn nnskelet.cc
// Gebruik:    ./nn <inputs> <hiddens> <epochs> <type> <seed>
// Voorbeeld:  ./nn 2 3 100000 1 1234
// Let op: de inputs komen in input[1],...,input[inputs] (tot en
// met inputs) en de verborgen knopen worden geindiceerd met
// 0,1,...,hiddens (tot en met hiddens)
//

#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
using namespace std;

const int MAX = 20;
const double ALPHA = 0.1;
const double BETA = 1.0;

// activatie-functie (sigmoid)
double sigmoid (double x) {
  return 1 / ( 1 + exp ( - BETA * x ) );
}//sigmoid

// afgeleide van sigmoid
double sigmoidprime (double x) {
  return BETA * sigmoid (x) * ( 1 - sigmoid (x) );
}//sigmoidprime

// activatie-functie (softplus)
double softplus (double x) {
  return log ( 1 + exp (x) );
}//softplus

// afgeleide van softplus
double softplusprime (double x) {
  return exp (x) / ( 1 + exp (x) );
}//softplusprime

// activatie-functie (ReLU)
double ReLU (double x) {
  if ( x >= 0 ) {
    return x;
  }//if
  return 0;
}//ReLU

// "afgeleide" van ReLU
double ReLUprime (double x) {
  if ( x >= 0 ) {
    return 1;
  }//if
  return 0;
}//ReLUprime

// activatie-functie van verschillend type
double g (double x, int type) {
  if ( type == 1 ) {
    return sigmoid (x);
  }//if
  else if ( type == 2 ) {
    return softplus (x);
  }//if
  else if ( type == 3 ) {
    return ReLU (x);
  }//if
  return 0;
}//g

// afgeleide van activatie-functie van verschillend type
double gprime (double x, int type) {
  if ( type == 1 ) {
    return sigmoidprime (x);
  }//if
  else if ( type == 2 ) {
    return softplusprime (x);
  }//if
  else if ( type == 3 ) {
    return ReLUprime (x);
  }//if
  return 0;
}//gprime

int main (int argc, char* argv[ ]) {

  double fout = 0;

  int inputs, hiddens;            // aantal invoer- en verborgen knopen
                                  // inputs is exclusief de bias-knoop!
  double input[MAX];              // de invoer is input[1]...input[inputs]
  double inputtohidden[MAX][MAX]; // gewichten van invoerknopen 0..inputs
                                  // naar verborgen knopen 1..hiddens
  double hiddentooutput[MAX];     // gewichten van verborgen knopen 0..hiddens
                                  // naar de ene uitvoerknoop
  double inhidden[MAX];           // invoer voor de verborgen knopen 1..hiddens
  double acthidden[MAX];          // en de uitvoer daarvan
  double inoutput;                // invoer voor de ene uitvoerknoop
  double netoutput;               // en de uitvoer daarvan: de net-uitvoer
  double target;                  // gewenste uitvoer
  double error;                   // verschil tussen gewenste en
                                  // geproduceerde uitvoer
  double delta;                   // de delta voor de uitvoerknoop
  double deltahidden[MAX];        // de delta's voor de verborgen
                                  // knopen 1..hiddens
  int epochs;                     // aantal trainingsvoorbeelden
  int cnt, i, j;                  // tellertjes
  int type;                       // welke activatie-functie?
  int seed;                       // voor random number generator

  if ( argc != 6 ) {
    cout << "Gebruik: " << argv[0] << " <inputs> <hiddens> <epochs> "
	 << "<type> <seed>" << endl;
    return 1;
  }//if
  inputs = atoi (argv[1]);
  if ( inputs >= MAX ) {
    cout << "Aantal inputs < " << MAX << "!" << endl;
    return 1;
  }//if
  hiddens = atoi (argv[2]);
  if ( hiddens >= MAX ) {
    cout << "Aantal verborgen knopen < " << MAX << "!" << endl;
    return 1;
  }//if
  epochs = atoi (argv[3]);
  type = atoi (argv[4]);
  seed = atoi (argv[5]);
  input[0] = -1;                  // invoer bias-knoop: altijd -1
  acthidden[0] = -1;              // verborgen bias-knoop: altijd -1
  srand (seed);

  // LATER TODO-7 lees de gehele Abalone dataset:
  // ifstream abalonedata ("leiden.abalone.data",ios::in);
  // if ( abalonedata.fail ( ) ) {
  //   cout << "Inputfile bestaat niet!" << endl;
  //   return 1;
  // }//if
  // const int SAMPLES = 4177;
  // double abalone[SAMPLES][9];
  // for ( i = 0; i < SAMPLES; i++ )
  //   for ( j = 0; j < 9; j++ )
  //     abalonedata >> abalone[i][j];
  // abalonedata.close ( );

  // TODO-1 initialiseer de gewichten random tussen -1 en 1:
  // inputtohidden en hiddentooutput
  for (int x = 0; x < inputs; x++){
    for (int y = 0; y < hiddens; y++){
      inputtohidden[x][y] = double (((rand()%200)-100))/100;
    }
  }

  for (int y = 0; y < hiddens; y++){
    hiddentooutput[y] = double (((rand()%200)-100))/100;
  }

  for ( cnt = 1; cnt <= epochs; cnt++ ) {
    for (int y = 0; y < hiddens; y++){
      inhidden[y] = 0;
      acthidden[y] = 0;
    }
    netoutput = 0;
    inoutput = 0;
    // TODO-2 lees voorbeeld in naar input/target, of genereer ter plekke:
    // als voorbeeld: de XOR-functie, waarvoor geldt dat inputs = 2
    input[0] = -1;
    for(int i = 1; i <= inputs; i++ ){
      input[i] = rand ( ) % 2;
      if( i == 2 ) {
        target = int(( input[1] + input[2] )) % 2;
      }else if (i > 2){
        target = int(( target + input[i] )) % 2;
      }
    }

    // LATER TODO-7 Abalone, met inputs = 8:
    // int mysample = rand ( ) % SAMPLES;
    // for ( j = 1; j <= 8; j++ )
    //   input[j] = abalone[mysample][j-1];
    // target = abalone[mysample][8] / 30.0;

    // TODO-3 stuur het voorbeeld door het netwerk
    // reken inhidden's uit, acthidden's, inoutput en netoutput

    inhidden[0] = -1;
    for (int x = 0; x <= inputs; x++){
      for (int y = 1; y < hiddens; y++){
        inhidden[y] = inhidden[y] + input[x]*inputtohidden[x][y];
      }
    }

    for(int y = 0; y < hiddens; y++){
      acthidden[y] = g(inhidden[y], type);

    }

    for (int y = 0; y < hiddens; y++){
      inoutput = inoutput + acthidden[y]*hiddentooutput[y];
    }

    netoutput = g(inoutput, type);

    //std::cout << "netoutput: " << netoutput << "target: " << target << std::endl;

    error = target - netoutput;
    delta = error*gprime(inoutput, type);
    for (int y = 0; y < hiddens; y++){
        deltahidden[y] = gprime(inhidden[y], type)*hiddentooutput[y]*delta;
    }

    // TODO-5 update gewichten hiddentooutput en inputtohidden
    for (int y = 0; y < hiddens; y++){
      hiddentooutput[y] = hiddentooutput[y] + ALPHA*acthidden[y]*delta;
    }

    for (int x = 0; x <= inputs; x++){
      for (int y = 0; y < hiddens; y++){
        inputtohidden[x][y] = inputtohidden[x][y] + ALPHA*input[x]*deltahidden[y];
      }
    }


    if(cnt%1000 == 0){
      std::cout << "gemiddelde fout is: " << fout << endl;
      fout = fout + ((error*-1)/1000);
      fout = 0;
    }else{
      fout = fout + error/1000;
    }
    // TODO-6 beoordeel het netwerk en rapporteer
    // bijvoorbeeld (totaal)fout bij laatste 1000 voorbeelden

  }//for

  return 0;
}//main
