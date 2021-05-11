#include <iostream>
#include <thread>
#include <time.h>
#include <random>
#include <chrono>
#include <mutex>

using namespace std;

typedef enum{
	Pensando, EnEspera, Comiendo
}status;

status *FiloEstado;
thread *Mesa;
int maximo;

struct Filosofo{
  string nombre;
  int ident;
  int izq,der;
  int Energia=50;
  int TotalComida=100;
  Filosofo(){};
  Filosofo(int ident){
    this->nombre = "Filosofo " + to_string(ident+1);
    this->ident = ident;
    izq = ((ident) !=0 ? (ident)-1 : (maximo-1));
    der = (((ident)+1)%maximo);
  }  
};


void Coger_Tenedores(Filosofo &Persona){
  FiloEstado[Persona.ident]=EnEspera;
  if(FiloEstado[Persona.izq]!=Comiendo&&FiloEstado[Persona.der]!=Comiendo&&Persona.TotalComida>0){
		FiloEstado[Persona.ident]=Comiendo;
  }
}

void Soltar_Tenedores(Filosofo &Persona){
  if(FiloEstado[Persona.ident]==Comiendo){
    cout<<"El "<<Persona.nombre<<" ha dejado los tenedores."<<endl;
    FiloEstado[Persona.ident]=Pensando;
  }
  
}

void Comer(Filosofo &Persona){
  if(FiloEstado[Persona.ident]==Comiendo && Persona.TotalComida>0){
    cout<<"El "<< Persona.nombre << " esta comiendo."<<endl;
    while(Persona.Energia<100 && Persona.TotalComida>0){
      int bocado = rand() % 20 + 10;
      Persona.Energia+=bocado;
      Persona.TotalComida-=bocado;

      int descanso = rand()%3+1;
      std::this_thread::sleep_for(chrono::seconds(descanso));   
    }
    if(Persona.Energia>100){
      Persona.Energia=100;
    }
    if(Persona.TotalComida<0){
      Persona.TotalComida=0;
    }
  }
}


void Pensar(Filosofo &Persona){ 
  cout<<"El "<<Persona.nombre<<" esta pensando."<<endl;
  FiloEstado[Persona.ident]=Pensando;

  while(Persona.Energia>0){
    int pensamiento= rand()%25+10;
    Persona.Energia-=pensamiento;
    int segundos = rand()%3+1;
    std::this_thread::sleep_for(chrono::seconds(segundos));
  }
  if(Persona.Energia<0){
    Persona.Energia=0;
  }
}

void Sentarse(Filosofo &Persona){
  while(true){
    cout<<Persona.nombre<<" "<<Persona.TotalComida<<" "<<Persona.Energia<<endl;
    if(Persona.TotalComida==0){
      cout<<"El "<<Persona.nombre<<" ha terminado la cena."<<endl;
      return;
    }
    if(Persona.Energia==0){
      cout<<"El "<<Persona.nombre<<" ha muerto de hambre."<<endl;
      return;
    }
    
    Pensar(Persona);
    Coger_Tenedores(Persona);
    Comer(Persona);
    Soltar_Tenedores(Persona);
  }
}

int main(){
  srand(time(NULL));
  int asistentes;
  cout<<"Ingrese cantidad de filosofos: ";
  cin>>asistentes;
  maximo = asistentes;

  Mesa = new thread[asistentes];
  FiloEstado = new status[asistentes];
  Filosofo Personas[asistentes];
  
  for(int i = 0; i<asistentes; i++){
    FiloEstado[i]=Pensando;
  }

  for(int i = 0; i<asistentes; i++){
    Personas[i] = Filosofo(i);
    Mesa[i]=thread(Sentarse,ref(Personas[i]));
  }

  for(int i = 0; i<asistentes; i++){
    Mesa[i].join();
  }
  cout<<"Termino la cena."<<endl;
}
