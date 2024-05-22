#ifndef FUNCIONES_H
#define FUNCIONES_H
#include <iostream>
#include <vector>
#include <cmath>

#include <string>
#include <cstdlib>
#include <thread> // Para std::this_thread::sleep_for
#include <chrono> // Para std::chrono::seconds
#include <MinimalSocket/udp/UdpSocket.h>

using namespace std;

struct Posicion{
    int x;
    int y;
};

struct Jugador{
    int numero;
    int equipo;
    Posicion pos;
};
struct Lectura{
   string tipo;
   string porteria_der;
   string porteria_der_dist;
   string porteria_izq;
   string porteria_izq_dist;
   string pelota;
   string pelota_angle;
   vector<string> direccionamigo;
   vector<string> direccionamigo2;
   vector<string> distamigo;
   vector<string> distamigo2;
};
struct Decision{
    string direccion;
    string distancia;
};
vector<string> vectorpalabras(string const &ejercicio){
    vector<string> resultado;
    string aux=ejercicio;
    int pos=aux.find(' ');
    if(pos!=-1)
        resultado.push_back(aux.substr(0,pos));
    for(int i=0;i<ejercicio.size();i++){
        if(ejercicio.at(i)==' '){
            aux=ejercicio.substr(i+1,ejercicio.size());
            pos=aux.find(' ');
            if(pos!=-1)
                resultado.push_back(aux.substr(0,pos));
        }
    }
    resultado.push_back(aux);
    return resultado;
}

string obtenerPrimeraPalabra(string& str) {
    auto pos = str.find(' ');
    string primeraPalabra;
    if (pos != -1) {
        primeraPalabra = str.substr(0, pos);
        str = str.substr(pos + 1); // Actualiza str para que contenga la parte restante de la cadena
    } else {
        primeraPalabra = str;
        str.clear(); // Vacía str ya que no hay más palabras después de esta
    }
    return primeraPalabra;
}

vector<string> dividir_en_palabras_parentesis(string const &s) {
    vector<string> palabras;
    string palabra;
    int count_parentesis = 0;

    for (char ch : s) {
        if (ch == '(') {
            count_parentesis++;
            if (count_parentesis > 1) {
                palabra.push_back(ch);
            }
        } else if (ch == ')') {
            count_parentesis--;
            if (count_parentesis > 0) {
                palabra.push_back(ch);
            }
        } else {
            if (count_parentesis > 0) {
                palabra.push_back(ch);
            }
        }

        if (count_parentesis == 0 && !palabra.empty()) {
            palabras.push_back(palabra);
            palabra.clear();
        }
    }

    return palabras;
}


string crearMove(Posicion pos){
    string movimiento="(move "+to_string(pos.x)+" "+to_string(pos.y)+")";
    return movimiento;
}

vector<string> encontrarStringConPrefijo(const string& str, const string& prefijo) {
    size_t pos = str.find(prefijo);
    if (pos != string::npos) { // Correcto uso de std::string::npos
        return vectorpalabras(str.substr(pos));
    }
    return vectorpalabras(""); // Retorna un vector vacío si no se encuentra el prefijo
}

/*Posicion TriangularPos2(const std::vector<std::string>& lecturaFlag, const std::vector<std::string>& lecturaFlag2, const Posicion& Pos2, const Posicion& Pos1) {
    // Obtener las decisiones de dirección y distancia de cada lectura de flag
    Decision decision1 = { lecvector<string> encontrarStringConPrefijo(const string& str, const string& prefijo) {
                                                                                                                size_t pos = str.find(prefijo);
    if (pos != string::npos) { // Correcto uso de std::string::npos
        return vectorpalabras(str.substr(pos));
    }
    return vectorpalabras(""); // Retorna un vector vacío si no se encuentra el prefijo
}turaFlag[4], lecturaFlag[3]};
    Decision decision2 = { lecturaFlag2[4], lecturaFlag2[3] };

    double Da = stod(decision1.distancia), Db = stod(decision2.distancia);
    double Aa_grados = stod(decision1.direccion), Ab_grados = stod(decision2.direccion);
    double Xa = Pos1.x, Ya = Pos1.y, Xb = Pos2.x, Yb = Pos2.y;
    cout<<"Dist1: "<<Da<<" Direccion1: "<<Aa_grados<<" PosX:"<<Xa<<" PosY:"<<Ya<<endl;
            cout<<"Dist2: "<<Db<<" Direccion2: "<<Ab_grados<<" PosX:"<<Xb<<" PosY:"<<Yb<<endl;
    Posicion resultado;

    // Convertir ángulos de grados a radianes
    double Aa = Aa_grados * M_PI / 180.0;
    double Ab = Ab_grados * M_PI / 180.0;

    // Cálculo de la posición del cuerpo
    double x = (Da * std::cos(Aa) * (Xa - Pos1.x) + Db * std::cos(Ab) * (Xb - Pos2.x)) / (std::cos(Aa) + std::cos(Ab));
    double y = (Da * std::sin(Aa) * (Ya - Pos1.y) + Db * std::sin(Ab) * (Yb - Pos2.y)) / (std::sin(Aa) + std::sin(Ab));

    // Cálculo del ángulo del cuerpo
    double alpha = std::atan2(y - Ya, x - Xa);

    resultado.x = x;
    resultado.y = y;
    //resultado.angulo = alpha;

    return resultado;
}*/
struct Triangulo {
    double xa, ya, da, alpha_a;
    double xb, yb, db, alpha_b;
};

// Función para convertir grados a radianes
double degToRad(double degrees) {
    return degrees * M_PI / 180.0;
}

Posicion TriangularPos(const std::vector<std::string>& lecturaFlag, const std::vector<std::string>& lecturaFlag2, const Posicion& Pos2, const Posicion& Pos1) {
    // Obtener las decisiones de dirección y distancia de cada lectura de flag
    Decision decision1 = { lecturaFlag[4], lecturaFlag[3]};
    Decision decision2 = { lecturaFlag2[4], lecturaFlag2[3] };

    // Crear el triángulo con las posiciones y decisiones de lectura de flag
    Triangulo otro = {
        (double)Pos1.x, (double)Pos1.y, std::stod(decision1.distancia), std::stod(decision1.direccion),
        (double)Pos2.x, (double)Pos2.y, std::stod(decision2.distancia), std::stod(decision2.direccion)
    };

    std::cout << "Dist1: " << otro.da << " Direccion1: " << otro.alpha_a << " PosX:" << otro.xa << " PosY:" << otro.ya << std::endl;
    std::cout << "Dist2: " << otro.db << " Direccion2: " << otro.alpha_b << " PosX:" << otro.xb << " PosY:" << otro.yb << std::endl;

    // Calcular las coordenadas triangulares
    Posicion coordenadas;
    coordenadas.x = otro.xb + otro.db * cos(degToRad(otro.alpha_b));
    coordenadas.y = otro.yb + otro.db * sin(degToRad(otro.alpha_b));

    return coordenadas;
}

Lectura ClasificaDatos (string &tipo, vector<string>  &cadenas, vector<string> &palabras,Jugador &jugador) {
    vector<string> valor,vectoria,valor2,valor3,valor4,valor5,aux,aux2;
    int count=0;
    int i=0;
    int aux3=-1;
    Lectura lectura;
    string semiOrden;
    /*
    vector <string> Flags={"(f l b)","(f c b)","(f r b)","(f l t)","(f c t)","(f r t)","(g r)","(g l)"};
    vector <Posicion> FlagsPos={{-50,30},{0,30},{50,30},{-50,-30},{0,-30},{50,-30},{50,0},{-50,0}};
    */
    vector <string> Flags={"(f l b)","(f c b)","(f r b)","(f l t)","(f c t)","(f r t)"};
    vector <Posicion> FlagsPos={{-50,30},{0,30},{50,30},{-50,-30},{0,-30},{50,-30}};
    if(tipo=="see"){
        lectura.tipo="see";
        for(auto parentesis:cadenas){
            i=0;
            //cout<<parentesis<<endl;
            valor=encontrarStringConPrefijo(parentesis,"(b)");//Buscar en todos los parentesis el de (b)
            valor2=encontrarStringConPrefijo(parentesis,"(g r)");//Buscar en todos los parentesis el de (g r)
            valor3=encontrarStringConPrefijo(parentesis,"(g l)");//Buscar en todos los parentesis el de (g l)
            valor4 = encontrarStringConPrefijo(parentesis, "\"pOESIACA\" ");
            valor5 = encontrarStringConPrefijo(parentesis, "\"pOESIAC\" ");
            for(auto Bandera:Flags){
               aux  = encontrarStringConPrefijo(parentesis,Bandera);
                if(aux.size()>1){//Hemos visto un flag
                    if(count==0){
                        count++;
                        aux2=aux;
                        aux3=i;//Guardamos el indice del vector de posiciones
                    }else if(count ==1){
                        count++;
                        jugador.pos=TriangularPos(aux,aux2,FlagsPos[aux3],FlagsPos[i]);
                        cout<<"La posicion es: "<<"X: "<<jugador.pos.x <<"Y: "<<jugador.pos.y<<endl;
                    }
                }
                i++;
            }
            if(valor2.size()>1){
                lectura.porteria_der=(valor2.at(3));
                lectura.porteria_der_dist=(valor2.at(2));
            }
            if(valor3.size()>1){
                lectura.porteria_izq=(valor3.at(3));
                lectura.porteria_izq_dist=(valor3.at(2));
            }
            if(valor.size()>1){
                lectura.pelota=(valor.at(1));
                lectura.pelota_angle=(valor.at(2));
            }
            if(valor4.size()>1){
                lectura.direccionamigo.push_back(valor4.at(3));
                lectura.distamigo.push_back(valor4.at(2));
            }
            if(valor5.size()>1){
                lectura.direccionamigo2.push_back(valor5.at(3));
                lectura.distamigo2.push_back(valor5.at(2));
            }
        }
    }else if(tipo=="hear"){

        for (int i = 0; i < palabras.size(); ++i) {
            semiOrden=palabras.at(i);
            string parteDeseada = semiOrden.substr(0, semiOrden.size()-1);
            cout<<palabras.at(i)<<endl;

            if(palabras.at(i)=="play_on"){
                lectura.tipo="play_on";

                cout<<"empieza el partido"<<endl;
            }else if(palabras.at(i)=="kick_off_l"){
                lectura.tipo="kick_off_l";
                cout<<"Saca el equipo de la izq"<< endl;
            }else if(palabras.at(i)=="kick_off_r"){
                lectura.tipo="kick_off_r";
                cout<<"Saca el equipo de la derecha"<< endl;
            }else if(parteDeseada=="goal_l_"){
                lectura.tipo="goal";
                cout<<"GOLASOOOOOOOOOOOOOOOO"<< endl;
            }else if(parteDeseada=="goal_r_"){
                lectura.tipo="goal";
                cout<<"GOLASOOOOOOOOOOOOOOOO"<< endl;
            }
        }
    }else{
        lectura.porteria_der="";
        lectura.porteria_izq="";
        lectura.pelota="";
        lectura.pelota_angle="";
        lectura.tipo="";
        lectura.direccionamigo.clear();
        lectura.direccionamigo2.clear();
        lectura.distamigo2.clear();
        lectura.distamigo2.clear();

        lectura.porteria_der_dist="";
        lectura.porteria_izq_dist="";
    }
    return lectura;
}

void PosicionarJugador(Jugador jugador, MinimalSocket::Address server_udp,MinimalSocket::udp::Udp<true>& udp_socket) {
    vector<Posicion> posiciones={{50,0},{35,-20},{35,20},{20,-25},{18,-9},{18,5},{20,20},{2,-18},{28,-18},{35,11},{5,0}};
    for(auto &p:posiciones){
        p.x=-p.x;
    }
    switch(jugador.numero){
    case 1:
        udp_socket.sendTo(crearMove(posiciones.at(0)), server_udp);
        cout << crearMove(posiciones.at(0)) << endl;
        break;
    case 2:
        udp_socket.sendTo(crearMove(posiciones.at(1)), server_udp);
        cout << crearMove(posiciones.at(1)) << endl;
        break;
    case 3:
        udp_socket.sendTo(crearMove(posiciones.at(2)), server_udp);
        cout << crearMove(posiciones.at(2)) << endl;

        break;
    case 4:
        udp_socket.sendTo(crearMove(posiciones.at(3)), server_udp);
        cout << crearMove(posiciones.at(3)) << endl;

        break;
    case 5:
        udp_socket.sendTo(crearMove(posiciones.at(4)), server_udp);
        cout << crearMove(posiciones.at(4)) << endl;

        break;
    case 6:
        udp_socket.sendTo(crearMove(posiciones.at(5)), server_udp);
        cout << crearMove(posiciones.at(5)) << endl;
        break;
    case 7:
        udp_socket.sendTo(crearMove(posiciones.at(6)), server_udp);
        cout << crearMove(posiciones.at(6)) << endl;

        break;
    case 8:
        udp_socket.sendTo(crearMove(posiciones.at(7)), server_udp);
        cout << crearMove(posiciones.at(7)) << endl;

        break;
    case 9:
        udp_socket.sendTo(crearMove(posiciones.at(8)), server_udp);
        cout << crearMove(posiciones.at(8)) << endl;
        break;
    case 10:
        udp_socket.sendTo(crearMove(posiciones.at(9)), server_udp);
        cout << crearMove(posiciones.at(9)) << endl;
        break;
    case 11:
        udp_socket.sendTo(crearMove(posiciones.at(10)), server_udp);
        cout << crearMove(posiciones.at(10)) << endl;
        break;
    }
    if(jugador.equipo == 1){
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Espera 1 segundo
        udp_socket.sendTo("(turn 180)", server_udp);
    }
}


Decision DetectarMasCercano(vector<string> direcciones, vector<string> distancias){
    Decision MasCercano;
    double dist=1000;
    int i=0;
        for(auto elem:direcciones){
            if(stod(elem)<dist){
                MasCercano.distancia=elem;
                MasCercano.direccion=direcciones.at(i);
            }
            i++;
        }
    return MasCercano;
}

bool MasCercaBola(vector<string> direcciones, vector<string> distancias,double dist,int num){
    int i=0;
        for(auto elem:distancias){
            cout <<elem<<endl;
            if(stod(elem)-dist<dist){//Hay un jugador mas cerca que yo
                i++;
                cout << "Veo a alguien mas cerca"<<endl;
                if(stod(elem)-dist<1){//Hay un jugador muy muy cerca
                    return false;
                }
            }
        }
        if(num<i){//Numero de jugadores a detectar<=Detectados
            cout << "Vemos a mas de 2"<<endl;
            return false;
        }
    return true;
}

Lectura Accion (const Jugador &jugador,Lectura &Data, MinimalSocket::Address server_udp,MinimalSocket::udp::Udp<true>& udp_socket){
    string vectoria,valor2,valor3, porteria,valorpase,distPor;
    Decision pase;
    if(Data.tipo=="see"){
        bool bola=false;
            if(jugador.equipo==-1){
                pase=DetectarMasCercano(Data.direccionamigo2,Data.distamigo2);
                valor2=Data.porteria_der;//Buscar en todos los parentesis el de (g r)
                valorpase=pase.direccion;//Funcion para obtener el mas cercano
                valor3=Data.porteria_der_dist;//Buscar en todos los parentesis el de (g r)
            }else if(jugador.equipo==1){
                pase=DetectarMasCercano(Data.direccionamigo,Data.distamigo);
                valor2=Data.porteria_izq;//Buscar en todos los parentesis el de (g l)
                valorpase=pase.direccion;;//Funcion para obtener el mas cercano
                valor3=Data.porteria_izq_dist;//Buscar en todos los parentesis el de (g r)
            }
            if(valor2!=""){
                porteria=valor2;
                distPor=valor3;
            }
            if(Data.pelota!=""){
                bola = true;
                double variable=stod(Data.pelota);
                if(variable<0.6&&porteria!=""&&stod(distPor)<50){
                    cout<<"Patadon a la direccion:"<<porteria<<endl;
                    udp_socket.sendTo("(kick 50 "+porteria+")", server_udp);
                }else if(variable<0.6&&porteria==""){//Tengo el balon y no veo la porteria
                    udp_socket.sendTo("(kick 5 90)", server_udp);
                    this_thread::sleep_for(std::chrono::milliseconds(150));

                }else if(variable<0.6&&valorpase!=""){
                    cout<<"Pasecito a la direccion:"<<valorpase<<endl;
                    udp_socket.sendTo("(kick 50 "+valorpase+")", server_udp);
                }else if(variable<0.6&&porteria!=""){//Tengo el balon y veo la porteria
                    udp_socket.sendTo("(kick 50 "+porteria+")", server_udp);
                }
                else if(abs(stod(Data.pelota_angle))>20){
                    udp_socket.sendTo("(turn "+Data.pelota_angle+")", server_udp);
                }else if(jugador.equipo==-1&&MasCercaBola(Data.direccionamigo2,Data.distamigo2,variable,1)){
                    udp_socket.sendTo("(dash 100 "+Data.pelota_angle+")", server_udp);
                }else if(jugador.equipo==1&&MasCercaBola(Data.direccionamigo,Data.distamigo,variable,1)){
                    udp_socket.sendTo("(dash 100 "+Data.pelota_angle+")", server_udp);
                }
            }
        if(!bola){
            udp_socket.sendTo("(turn 30)", server_udp);
        }
        }else if(Data.tipo=="kick_off_l"){
            cout<<"EMPIEZA EL GAME"<<endl;

        }else if(Data.tipo=="kick_off_r"){
            cout<<"EMPIEZA EL GAME"<<endl;

        }
        else if(Data.tipo=="play_on"){
            cout<<"EMPIEZA EL GAME"<<endl;

        }
        else if(Data.tipo=="goal"){
            cout<<"Colocalos"<<endl;
            PosicionarJugador(jugador, server_udp,udp_socket);
            this_thread::sleep_for(std::chrono::seconds(5));
        }
    return Data;
}

#endif // FUNCIONES_H
