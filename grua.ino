#include <Separador.h> //libreria usada para separar una cadena de caracteres
#include <Stepper.h>   //libreria usada para controlar el motor paso a paso
Separador s1,s2,s3; // inicializa objetos que ayudaran a separar una cadena de caracteres
const int stepsPerRevolution = 200; // valor del numero de pasos del motor que usa en la practica
int contador=0;
String grados,dir,velocidad; // variables para metodos de los motores
// inicializa la libreria 'stepper' en los pines 8 a 11
Stepper myStepper(stepsPerRevolution, 8,9,10,11);
int IN3 = 5; //creacion y asignacion de pin digital a variable IN3
int IN4 = 4; //creacion y asignacion de pin digital a variable IN4
int ENB = 3; //creacion y asignacion de pin digital a variable ENB 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ENB,OUTPUT); //cambiar el modo del pin asignado a ENB a salida
  pinMode(IN3,OUTPUT); //cambiar el modo del pin asignado a IN3 a salida
  pinMode(IN4,OUTPUT); //cambiar el modo del pin asignado a IN4 a salida  
}
String instrucciones[] = {"mov(2,D,10","mov(2,U,10)","mov(1,L,5)","mov(30,L,5)","mov(2,D,10)","mov(2,U,10)","mov(31,R,5)"}; //arreglo de instrucciones
void loop() { //Inicio de ciclo 
  char c; // Caracter a leer 
  while(!Serial.available()) // Esperar hasta que encuentre algo
    delay(10); // Esperar 10 ms
  while(Serial.available() > 0) // Si hay algo para leer
  { 
    c = Serial.read(); // Se lee el siguiente caracter
    if (c == 'o'){ // si el caracter es igual a o el contador vuelve al valor 0
      contador = 0; 
    } 
    serialEvent();
    delay(10); // Esperar 10 ms
  }
} //fin de ciclo

void serialEvent(){ //el metodo serial event sepera la cadena de string y asigna las variables a los metodos para mover los motores
   while(contador < 7) // este while sirve para leer las instrucciones del arreglo de strings 
   {
    String datosrecibidos = instrucciones[contador]; 
    String elemento1 = s1.separa(datosrecibidos,'(',0); 
    String elemento2 = s1.separa(datosrecibidos,'(',1);
    String dato1 = s2.separa(elemento2,')',0);
    grados = s3.separa(dato1,',',0);
    dir = s3.separa(dato1,',',1);
    velocidad = s3.separa(dato1,',',2);
    Serial.println("El elemento 1 es: " + grados);
    Serial.println("El elemento 2 es: " + dir);
    Serial.println("El elemento 3 es: " + velocidad);
    Serial.println("Valor de la direccion: "+(String)direccion(dir));
    
    char o = dir.charAt(0);
    if(o == 'R' || o == 'L'){
      Serial.println("Mover motor paso a paso");
      moverMotor(grados.toInt(),direccion(dir),velocidad.toInt());
    }
    if(o == 'U' || o == 'D'){
      Serial.println("Mover motor SERVO");
      servoMotor(grados.toInt(),direccion(dir),velocidad.toInt());
    }
    delay(1000);
    contador++;
  }
}

int direccion(String data) //determina la direccion respecto a la caracter de la instruccion
{
  switch (data.charAt(0)) 
  {
    case 'R': 
        return 1;
      break;
    case 'L':
        return -1;
      break;
    case 'U':
        return 1;
      break;
    case 'D':
        return -1;
      break;
    default:
        return 0;   
      break;
  }
}
void moverMotor(int rev,int dir, int vel) // metodo para mover el motor paso paso 
{
  myStepper.step(dir*rev); //asigna 
  myStepper.setSpeed(vel);
  delay(500);
}
void servoMotor(int tiempo,int dir, int vel)
{
  if(dir == 1){
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB,vel*10);
    delay(tiempo*900);
    digitalWrite (IN3, LOW); 
  }else{
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB,vel*10);
    delay(tiempo*900);
    digitalWrite (IN4, LOW); 
  }
}
