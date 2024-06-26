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
    double x;
    double y;
};

struct Jugador{
    int numero;
    int equipo;
    Posicion pos;
    double NeckAngle;
    string tipoJugador;
};

struct Lectura{
    string tipo;
    string centro_arriba;
    string centro_abajo;
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
    string distancia_portero_up_l;
    string distancia_portero_up_r;
    string distancia_portero_down_r;
    string distancia_portero_down_l;
    string distancia_portero_centre_r;
    string distancia_portero_centre_l;
    string distanciaAreaChica_r;
    string distanciaAreaChica_l;
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

vector<string> dividir_en_palabras_parentesis(string const &s){
    vector<string> palabras;
    string palabra;
    char token1 = '(';
    int count_parentesis = 0;
    char token2 = ')';

    for(int i = 0; i<s.length(); i++){
        if(s.at(i) == token1)
        {
            count_parentesis++;
            if(count_parentesis > 1)
                palabra.push_back(s.at(i));
        }
        else if (s.at(i) == token2)
        {
            count_parentesis--;
            if(count_parentesis > 0)
                palabra.push_back(s.at(i));
        }
        else{
            palabra.push_back(s.at(i));
        }
        if (count_parentesis == 0){
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

vector<string> encontrarStringConPrefijo(const string& str, const string& prefijo) {//va
    size_t pos = str.find(prefijo);
    if (pos != -1) {
        //cout <<"Encontrado"<<endl;
        for(auto d:vectorpalabras(str.substr(pos))){
            //cout << d<<endl;
        }
        return vectorpalabras(str.substr(pos));
    }
    return vectorpalabras(""); // Retorna una cadena vacía si no se encuentra el prefijo
}

Posicion TriangularPos2(const std::vector<std::string>& lecturaFlag, const std::vector<std::string>& lecturaFlag2, const Posicion& Pos2, const Posicion& Pos1) {
    // Obtener las decisiones de dirección y distancia de cada lectura de flag
    Decision decision1 = { lecturaFlag[4], lecturaFlag[3]};
    Decision decision2 = { lecturaFlag2[4], lecturaFlag2[3] };

    double Da = stod(decision1.distancia), Db = stod(decision2.distancia);
    double Aa_grados = stod(decision1.direccion), Ab_grados = stod(decision2.direccion);
    double Xa = Pos1.x, Ya = Pos1.y, Xb = Pos2.x, Yb = Pos2.y;
    //cout<<"Dist1: "<<Da<<" Direccion1: "<<Aa_grados<<" PosX:"<<Xa<<" PosY:"<<Ya<<endl;
    //cout<<"Dist2: "<<Db<<" Direccion2: "<<Ab_grados<<" PosX:"<<Xb<<" PosY:"<<Yb<<endl;
    Posicion resultado;

    // Convertir ángulos de grados a radianes
    double Aa = Aa_grados * M_PI / 180.0;
    double Ab = Ab_grados * M_PI / 180.0;

    // Cálculo de la posición del cuerpo
    double x = (Da * std::cos(Aa) * (Xa - Pos1.x) + Db * std::cos(Ab) * (Xb - Pos2.x)) / (std::cos(Aa) + std::cos(Ab));
    double y = (Da * std::sin(Aa) * (Ya - Pos1.y) + Db * std::sin(Ab) * (Yb - Pos2.y)) / (std::sin(Aa) + std::sin(Ab));

    // Cálculo del ángulo del cuerpo
    //double alpha = std::atan2(y - Ya, x - Xa);

    resultado.x = x;
    resultado.y = y;
    //resultado.angulo = alpha;

    return resultado;
}

struct Triangulo {
    double xa, ya, da, alpha_a;
    double xb, yb, db, alpha_b;
};

// Función para convertir grados a radianes
double degToRad(double degrees) {
    return degrees * M_PI / 180.0;
}

double distance(Posicion p1, Posicion p2) {
    return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}
Posicion circleIntersection(Posicion p1, double r1, Posicion p2, double r2) {
    double d = distance(p1, p2);
    if (d >= r1 + r2 || d <= fabs(r1 - r2)) {
        return {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
    }
    double a = (r1 * r1 - r2 * r2 + d * d) / (2 * d);
    double h = sqrt(r1 * r1 - a * a);
    double x2 = p1.x + a * (p2.x - p1.x) / d;
    double y2 = p1.y + a * (p2.y - p1.y) / d;

    double x3 = x2 + h * (p2.y - p1.y) / d;
    double y3 = y2 - h * (p2.x - p1.x) / d;

    double x4 = x2 - h * (p2.y - p1.y) / d;
    double y4 = y2 + h * (p2.x - p1.x) / d;

    // Elegir la intersección más cercana al punto (0,0)
    if (distance({0,0}, {x3, y3}) < distance({0,0}, {x4, y4}))
        return {x3, y3};
    else
        return {x4, y4};
}

Posicion TriangularPos(const std::vector<std::string>& lecturaFlag, const std::vector<std::string>& lecturaFlag2, const Posicion& Pos2, const Posicion& Pos1) {
    // Obtener las decisiones de dirección y distancia de cada lectura de flag
    Decision decision1 = { lecturaFlag[4], lecturaFlag[3]};
    Decision decision2 = { lecturaFlag2[4], lecturaFlag2[3] };

    // Crear el triángulo con las posiciones y decisiones de lectura de flag
    Triangulo otro = {
        (double)Pos1.x, (double)Pos1.y, std::stod(decision1.distancia)+4, std::stod(decision1.direccion),
        (double)Pos2.x, (double)Pos2.y, std::stod(decision2.distancia)+4, std::stod(decision2.direccion)
    };
    for (int i = 0; i < 100000; ++i) {
            Posicion coordenadas = circleIntersection(Pos1, otro.da, Pos2, otro.db);
            if (std::isnan(coordenadas.x) || std::isnan(coordenadas.y)) {
                        continue;
                    }
            // Calcular la distancia entre la intersección y los puntos dados
            double dist1 = distance(Pos1, coordenadas);
            double dist2 = distance(Pos2, coordenadas);
            // Si las distancias calculadas son cercanas a las distancias dadas, retornar la intersección
            if (std::abs(dist1 - otro.da) < 0.01 && std::abs(dist2 - otro.db) < 0.01) {
                if(coordenadas.x<53&&coordenadas.y<33&&coordenadas.x>-53&&coordenadas.y>-33){
                return coordenadas;
                }else{
                    //std::cout << "Dist1: " << otro.da << " Direccion1: " << otro.alpha_a << " PosX:" << otro.xa << " PosY:" << otro.ya << std::endl;
                    //std::cout << "Dist2: " << otro.db << " Direccion2: " << otro.alpha_b << " PosX:" << otro.xb << " PosY:" << otro.yb << std::endl;
                }
            }
            // Actualizar las distancias utilizando las distancias calculadas
            otro.da = dist1;
            otro.db = dist2;
        }
        // Si no se encuentra una solución después de un número máximo de iteraciones, retornar un punto inválido
        return {std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN()};
}




Lectura ClasificaDatos (string &tipo, vector<string>  &cadenas, vector<string> &palabras,Jugador &jugador) {
    vector<string> valor,vectoria,valor2,valor3,valor4,valor5,valor6,valor7,aux,aux2,distancia_portero_up_l,distancia_portero_up_r,distancia_portero_down,distancia_portero_centre;
    vector<string> distancia_portero_down_l, distancia_portero_down_r,distancia_portero_centre_l,distancia_portero_centre_r,distanciaAreaChica_r,distanciaAreaChica_l;
    int count=0;
    int i=0;
    int aux3=-1;
    Lectura lectura;
    string semiOrden;
    /*
    vector <string> Flags={"(f l b)","(f c b)","(f r b)","(f l t)","(f c t)","(f r t)","(g r)","(g l)"};(f c b)(f c t)
    vector <Posicion> FlagsPos={{-50,30},{0,30},{50,30},{-50,-30},{0,-30},{50,-30},{50,0},       {-50,0}};
    */

    vector <string> Flags={"(f l b)","(f c b)","(f r b)","(f l t)","(f c t)","(f r t)"};
    vector <Posicion> FlagsPos={{-53,33},{1,33},{53,33},{-53,-33},{1,-33},{53,-33}};
    if(tipo=="see"){
        lectura.tipo="see";
        for(auto parentesis:cadenas){
            i=0;
            valor=encontrarStringConPrefijo(parentesis,"(b)");//Buscar en todos los parentesis el de (b)
            valor2=encontrarStringConPrefijo(parentesis,"(g r)");//Buscar en todos los parentesis el de (g r)
            valor3=encontrarStringConPrefijo(parentesis,"(g l)");//Buscar en todos los parentesis el de (g l)
            distancia_portero_up_l=encontrarStringConPrefijo(parentesis,"(f p l t)");
            distancia_portero_up_r=encontrarStringConPrefijo(parentesis,"(f p r t)");
            distancia_portero_down_l=encontrarStringConPrefijo(parentesis,"(f p l b)");
            distancia_portero_down_r=encontrarStringConPrefijo(parentesis,"(f p r b)");
            distancia_portero_centre_l=encontrarStringConPrefijo(parentesis,"(f p l c)");
            distancia_portero_centre_r=encontrarStringConPrefijo(parentesis,"(f p r c)");
            distanciaAreaChica_r=encontrarStringConPrefijo(parentesis,"(f r c)");
            distanciaAreaChica_l=encontrarStringConPrefijo(parentesis,"(f l c)");
            valor4 = encontrarStringConPrefijo(parentesis, "\"pOESIACA\" ");
            valor5 = encontrarStringConPrefijo(parentesis, "\"pOESIAC\" ");
            valor6=encontrarStringConPrefijo(parentesis,"(f c b)");//Buscar en todos los parentesis el de (g l)
            valor7=encontrarStringConPrefijo(parentesis,"(f c t)");//Buscar en todos los parentesis el de (g l

            if(valor6.size()>1){
                lectura.centro_arriba=(valor6.at(1));
            }
            if(valor7.size()>1){
                lectura.centro_abajo=(valor7.at(1));
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
            if(distancia_portero_up_l.size()>1){
                lectura.distancia_portero_up_l=(distancia_portero_up_l.at(4));
            }
            if(distancia_portero_up_r.size()>1){
                lectura.distancia_portero_up_r=(distancia_portero_up_r.at(4));
            }
            if(distancia_portero_down_l.size()>1){
                lectura.distancia_portero_down_l=(distancia_portero_down_l.at(4));
            }
            if(distancia_portero_down_r.size()>1){
                lectura.distancia_portero_down_r=(distancia_portero_down_r.at(4));
            }
            if(distancia_portero_centre_l.size()>1){
                lectura.distancia_portero_centre_l=(distancia_portero_centre_l.at(4));
            }
            if(distancia_portero_centre_r.size()>1){
                lectura.distancia_portero_centre_r=(distancia_portero_centre_r.at(4));
            }
            if(distanciaAreaChica_l.size()>1){
                lectura.distanciaAreaChica_l=(distanciaAreaChica_l.at(4));
            }
            if(distanciaAreaChica_r.size()>1){
                lectura.distanciaAreaChica_r=(distanciaAreaChica_r.at(4));
            }

        }
    }else if(tipo=="sense_body"){
        lectura.tipo="sense_body";
        for(auto parentesis:cadenas){
            //cout<<parentesis<<endl;
        valor=encontrarStringConPrefijo(parentesis,"head_angle");
        if(valor.size()>1){
            jugador.NeckAngle=stoi(valor.at(1));
            }
        }
    }else if(tipo=="sense_body"){
        lectura.tipo="sense_body";
        for(auto parentesis:cadenas){
            //cout<<parentesis<<endl;
        valor=encontrarStringConPrefijo(parentesis,"head_angle");
        if(valor.size()>1){
            jugador.NeckAngle=stoi(valor.at(1));
            }
        }
    }else if(tipo=="hear"){

        for (int i = 0; i < palabras.size(); ++i) {
            semiOrden=palabras.at(i);
            string parteDeseada = semiOrden.substr(0, semiOrden.size()-1);
            //cout<<palabras.at(i)<<endl;
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
        lectura.centro_abajo="";
        lectura.centro_arriba="";
    }
    return lectura;
}

void PosicionarJugador(Jugador jugador, MinimalSocket::Address server_udp,MinimalSocket::udp::Udp<true>& udp_socket) {
    vector<Posicion> posiciones={{50,0},{35,-20},{35,20},{20,-25},{18,-9},{18,5},{20,20},{2,-18},{30,-3},{35,11},{5,0}};
    for(auto &p:posiciones){
        p.x=-p.x;
    }
    switch(jugador.numero){
    case 1:
        jugador.pos=posiciones.at(0);
        udp_socket.sendTo(crearMove(posiciones.at(0)), server_udp);
        cout <<"a"<< crearMove(posiciones.at(0)) << endl;
        break;
    case 2:
        jugador.pos=posiciones.at(1);
        udp_socket.sendTo(crearMove(posiciones.at(1)), server_udp);
        cout << "b"<<crearMove(posiciones.at(1)) << endl;
        break;
    case 3:
        udp_socket.sendTo(crearMove(posiciones.at(2)), server_udp);
        cout <<"c"<< crearMove(posiciones.at(2)) << endl;

        break;
    case 4:
        udp_socket.sendTo(crearMove(posiciones.at(3)), server_udp);
        cout << "d"<<crearMove(posiciones.at(3)) << endl;

        break;
    case 5:
        udp_socket.sendTo(crearMove(posiciones.at(4)), server_udp);
        cout <<"e"<< crearMove(posiciones.at(4)) << endl;

        break;
    case 6:
        udp_socket.sendTo(crearMove(posiciones.at(5)), server_udp);
        cout <<"f"<< crearMove(posiciones.at(5)) << endl;
        break;
    case 7:
        udp_socket.sendTo(crearMove(posiciones.at(6)), server_udp);
        cout << "g"<<crearMove(posiciones.at(6)) << endl;

        break;
    case 8:
        udp_socket.sendTo(crearMove(posiciones.at(7)), server_udp);
        cout << "h"<<crearMove(posiciones.at(7)) << endl;
        break;
    case 9:
        udp_socket.sendTo(crearMove(posiciones.at(8)), server_udp);
        cout <<"i"<< crearMove(posiciones.at(8)) << endl;
        break;
    case 10:
        udp_socket.sendTo(crearMove(posiciones.at(9)), server_udp);
        cout << "j"<<crearMove(posiciones.at(9)) << endl;
        break;
    case 11:
        udp_socket.sendTo(crearMove(posiciones.at(10)), server_udp);
        cout <<"k"<< crearMove(posiciones.at(10)) << endl;
        break;
    }
    if(jugador.equipo == 1){
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Espera 1 segundo
        if(jugador.tipoJugador=="Portero")
            udp_socket.sendTo("(turn 175)", server_udp);
            else{
            udp_socket.sendTo("(turn 180)", server_udp);
        }
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
            if(elem=="goalie)"){

            }else if(elem!="goalie)"){
                if(stod(elem)-dist<dist){//Hay un jugador mas cerca que yo
                    i++;
                    //cout << "Veo a alguien mas cerca"<<endl;
                    if(stod(elem)-dist<0.6){//Hay un jugador muy muy cerca
                        return false;
                    }
                }
            }
        }
        if(num<i){//Numero de jugadores a detectar<=Detectados
            return false;
        }
    return true;
}

bool EstoyEnMiCuadrante(Jugador const &jugador,double porteria,MinimalSocket::Address server_udp,MinimalSocket::udp::Udp<true>& udp_socket){
    switch(jugador.numero){
    case 1:
            if(jugador.pos.x<36*jugador.equipo&&jugador.pos.x>52*jugador.equipo&&jugador.pos.y>-20&&jugador.pos.y<20){
                return true;
            }else if(jugador.pos.x<36*jugador.equipo){
                //Acciones
               udp_socket.sendTo("(dash 100 "+to_string(porteria)+")", server_udp);
                    return false;
                }else{
                //Acciones
                udp_socket.sendTo("(dash 100 "+to_string(porteria+180)+")", server_udp);
                    return false;
            }
        break;
    case 2:
        if(jugador.pos.x>0*jugador.equipo&&jugador.pos.x<20*jugador.equipo&&jugador.pos.y>0*jugador.equipo&&jugador.pos.y<20*jugador.equipo){
            return true;
        }else{
                return false;
            }
    break;
    case 3:
        if(jugador.pos.x>0*jugador.equipo&&jugador.pos.x<20*jugador.equipo&&jugador.pos.y>0*jugador.equipo&&jugador.pos.y<20*jugador.equipo){
            return true;
        }else{
                return false;
            }
    break;
    case 4:
        if(jugador.pos.x>0*jugador.equipo&&jugador.pos.x<20*jugador.equipo&&jugador.pos.y>0*jugador.equipo&&jugador.pos.y<20*jugador.equipo){
            return true;
        }else{
                return false;
            }
    break;
    case 5:
        if(jugador.pos.x>0*jugador.equipo&&jugador.pos.x<20*jugador.equipo&&jugador.pos.y>0*jugador.equipo&&jugador.pos.y<20*jugador.equipo){
            return true;
        }else{
                return false;
            }
    break;
    case 6:
        if(jugador.pos.x>0*jugador.equipo&&jugador.pos.x<20*jugador.equipo&&jugador.pos.y>0*jugador.equipo&&jugador.pos.y<20*jugador.equipo){
            return true;
        }else{
                return false;
            }
    break;
    case 7:
        if(jugador.pos.x>0*jugador.equipo&&jugador.pos.x<20*jugador.equipo&&jugador.pos.y>0*jugador.equipo&&jugador.pos.y<20*jugador.equipo){
            return true;
        }else{
                return false;
            }
    break;
    case 8:
        if(jugador.pos.x>0*jugador.equipo&&jugador.pos.x<20*jugador.equipo&&jugador.pos.y>0*jugador.equipo&&jugador.pos.y<20*jugador.equipo){
            return true;
        }else{
                return false;
            }
    break;
    case 9:
        if(jugador.pos.x>0*jugador.equipo&&jugador.pos.x<20*jugador.equipo&&jugador.pos.y>0*jugador.equipo&&jugador.pos.y<20*jugador.equipo){
            return true;
        }else{
                return false;
            }
    break;
    case 10:
        if(jugador.pos.x>0*jugador.equipo&&jugador.pos.x<20*jugador.equipo&&jugador.pos.y>0*jugador.equipo&&jugador.pos.y<20*jugador.equipo){
            return true;
        }else{
                return false;
            }
    break;
    case 11:
        if(jugador.pos.x>0*jugador.equipo&&jugador.pos.x<20*jugador.equipo&&jugador.pos.y>0*jugador.equipo&&jugador.pos.y<20*jugador.equipo){
            return true;
        }else{
                return false;
            }
        break;
    }
    return false;
}

Posicion calcularPosicionRelativa(double Px, double Py, double d, double theta) {
    Posicion pos;
    pos.x = Px + d * cos(degToRad(theta));
    pos.y = Py + d * sin(degToRad(theta));
    return pos;
}


double radToDeg(double radians) {
    return radians * 180.0 / M_PI;
}



double calcularOrientacionAbsoluta(double Px, double Py, double x, double y) {
    return (atan2(y - Py, x - Px)); // Convertir radianes a grados
}
void Decisionamos(Jugador const &jugador,double porteria,MinimalSocket::Address server_udp,MinimalSocket::udp::Udp<true>& udp_socket,double DistPor){
    double Px = 0, Py = 50*jugador.equipo; // Coordenadas del punto visible

    Posicion pos = calcularPosicionRelativa(Px, Py, DistPor, porteria);
    double orientacion = calcularOrientacionAbsoluta(Px, Py, pos.x, pos.y);
//    cout <<"Mi orientacion es: "<<orientacion;
//    cout <<"Mi KEKORI es: "<<porteria;

 //   cout <<"Mi X es: "<<pos.x;
  //  cout <<"Mi Y es: "<<pos.y<<endl;
    //cout <<"Estoy a "<<DistPor<<" de la porteria"<<endl;
    int dist=0;
    switch(jugador.numero){
    case 1:
        dist=100;
        if(DistPor<100){//Si estoy muy cerca
        udp_socket.sendTo("(dash 100 "+to_string(porteria+180)+")", server_udp);
        }
        else if (DistPor >= dist && DistPor <= dist+5) {
            udp_socket.sendTo("(dash 0 "+to_string(porteria)+")", server_udp);
        }
        else{
            udp_socket.sendTo("(dash 100 "+to_string(porteria)+")", server_udp);
        }
        break;
    case 2:        
        dist=80;
        if(DistPor<dist){//Si estoy muy cerca
        udp_socket.sendTo("(dash 100 "+to_string(porteria+180)+")", server_udp);
        }
        else if (DistPor >= dist && DistPor <= dist+5) {
            udp_socket.sendTo("(dash 0 "+to_string(porteria)+")", server_udp);
        }
        else{
            udp_socket.sendTo("(dash 100 "+to_string(porteria)+")", server_udp);
        }
        break;
    case 3:
        dist=80;
        if(DistPor<dist){//Si estoy muy cerca
        udp_socket.sendTo("(dash 100 "+to_string(porteria+180)+")", server_udp);
        }
        else if (DistPor >= dist && DistPor <= dist+5) {
            udp_socket.sendTo("(dash 0 "+to_string(porteria)+")", server_udp);
        }
        else{
            udp_socket.sendTo("(dash 100 "+to_string(porteria)+")", server_udp);
        }
        break;
    case 4:
        dist=60;
        if(DistPor<dist){//Si estoy muy cerca
        udp_socket.sendTo("(dash 100 "+to_string(porteria+180)+")", server_udp);
        }
        else if (DistPor >= dist && DistPor <= dist+5) {
            udp_socket.sendTo("(dash 0 "+to_string(porteria)+")", server_udp);
        }
        else{
            udp_socket.sendTo("(dash 100 "+to_string(porteria)+")", server_udp);
        }
        break;
    case 5:
        dist=60;
        if(DistPor<dist){//Si estoy muy cerca
        udp_socket.sendTo("(dash 100 "+to_string(porteria+180)+")", server_udp);
        }
        else if (DistPor >= dist && DistPor <= dist+5) {
            udp_socket.sendTo("(dash 0 "+to_string(porteria)+")", server_udp);
        }
        else{
            udp_socket.sendTo("(dash 100 "+to_string(porteria)+")", server_udp);
        }
        break;
    case 6:
        dist=60;
        if(DistPor<dist){//Si estoy muy cerca
        udp_socket.sendTo("(dash 100 "+to_string(porteria+180)+")", server_udp);
        }
        else if (DistPor >= dist && DistPor <= dist+5) {
            udp_socket.sendTo("(dash 0 "+to_string(porteria)+")", server_udp);
        }
        else{
            udp_socket.sendTo("(dash 100 "+to_string(porteria)+")", server_udp);
        }
        break;
    case 7:
        dist=60;
        if(DistPor<dist){//Si estoy muy cerca
        udp_socket.sendTo("(dash 100 "+to_string(porteria+180)+")", server_udp);
        }
        else if (DistPor >= dist && DistPor <= dist+5) {
            udp_socket.sendTo("(dash 0 "+to_string(porteria)+")", server_udp);
        }
        else{
            udp_socket.sendTo("(dash 100 "+to_string(porteria)+")", server_udp);
        }
        break;
    case 8:
        dist=30;
        if(DistPor<dist){//Si estoy muy cerca
        udp_socket.sendTo("(dash 100 "+to_string(porteria+180)+")", server_udp);
        }
        else if (DistPor >= dist && DistPor <= dist+5) {
            udp_socket.sendTo("(dash 0 "+to_string(porteria)+")", server_udp);
        }
        else{
            udp_socket.sendTo("(dash 100 "+to_string(porteria)+")", server_udp);
        }
        break;

    case 9:
        dist=80;
        if(DistPor<dist){//Si estoy muy cerca
        udp_socket.sendTo("(dash 100 "+to_string(porteria+180)+")", server_udp);
        }
        else if (DistPor >= dist && DistPor <= dist+5) {
            udp_socket.sendTo("(dash 0 "+to_string(porteria)+")", server_udp);
        }
        else{
            udp_socket.sendTo("(dash 100 "+to_string(porteria)+")", server_udp);
        }
        break;
    case 10:
        dist=80;
        if(DistPor<dist){//Si estoy muy cerca
        udp_socket.sendTo("(dash 100 "+to_string(porteria+180)+")", server_udp);
        }
        else if (DistPor >= dist && DistPor <= dist+5) {
            udp_socket.sendTo("(dash 0 "+to_string(porteria)+")", server_udp);
        }
        else{
            udp_socket.sendTo("(dash 100 "+to_string(porteria)+")", server_udp);
        }
        break;
    case 11:
        dist=30;
        if(DistPor<dist){//Si estoy muy cerca
        udp_socket.sendTo("(dash 100 "+to_string(porteria+180)+")", server_udp);
        }
        else if (DistPor >= dist && DistPor <= dist+5) {
            udp_socket.sendTo("(dash 0 "+to_string(porteria)+")", server_udp);
        }
        else{
            udp_socket.sendTo("(dash 100 "+to_string(porteria)+")", server_udp);
        }
        break;
    }

}

void imprimirPosicion(Posicion pos){
    cout <<"Posicion X: "<<pos.x <<" Y:"<<pos.y<<endl;
}
Lectura Accion (const Jugador &jugador,Lectura &Data, MinimalSocket::Address server_udp,MinimalSocket::udp::Udp<true>& udp_socket){
  string vectoria,valor2,valor3, porteria,valorpase,distPor,distanciaPase,porteriaRival;
  Decision pase;
    if(jugador.equipo==1 && jugador.tipoJugador =="Portero"){
        if(Data.pelota_angle!=""  && Data.tipo=="see"){
            if(stod(Data.pelota_angle) > 0 && Data.distancia_portero_down_r!=""&&stod(Data.pelota)<30){
                        udp_socket.sendTo("(dash 100 90)", server_udp);

            }
            if(stod(Data.pelota_angle) < 0 && Data.distancia_portero_up_r!=""&&stod(Data.pelota)<30){
                        udp_socket.sendTo("(dash 100 -90)", server_udp);
            }
            if(stod(Data.pelota) < 1.3){
                udp_socket.sendTo("(catch "+ Data.pelota_angle+")", server_udp);
                this_thread::sleep_for(std::chrono::milliseconds(155));
                udp_socket.sendTo("(kick 100 17)", server_udp);
            }
        }
        if(Data.tipo=="goal"){
            //PosicionarJugador(jugador, server_udp,udp_socket);
            this_thread::sleep_for(std::chrono::seconds(5));
        }
    }else if(jugador.equipo==-1 && jugador.tipoJugador =="Portero"){
        if(Data.pelota_angle!="" && Data.tipo=="see"){
            if(stod(Data.pelota_angle) > 0 && Data.distancia_portero_up_l!=""){
                if(stod(Data.distancia_portero_up_l) > 6.5){
                    udp_socket.sendTo("(dash 100 90)", server_udp);
                }
            }
            if(stod(Data.pelota_angle) < 0 && Data.distancia_portero_down_l!="") {
                if (stod(Data.distancia_portero_down_l) > 6.5){
                    udp_socket.sendTo("(dash 100 -90)", server_udp);
                }
            }
            if(stod(Data.pelota) <1.3){
                udp_socket.sendTo("(catch "+ Data.pelota_angle+")", server_udp);
                this_thread::sleep_for(std::chrono::milliseconds(155));
                udp_socket.sendTo("(kick 100 15)", server_udp);
            }
        }
    }else if(jugador.tipoJugador =="Jugador"){
        if(Data.tipo=="see"){
                bool bola=false;
                    if(jugador.equipo==-1){
                        pase=DetectarMasCercano(Data.direccionamigo2,Data.distamigo2);
                        valor2=Data.porteria_der;
                        porteriaRival=Data.porteria_izq;
                        valorpase=pase.direccion;//Funcion para obtener el mas cercano
                        distanciaPase=pase.distancia;
                        valor3=Data.porteria_der_dist;//Buscar en todos los parentesis el de (g r)
                    }else if(jugador.equipo==1){
                        pase=DetectarMasCercano(Data.direccionamigo,Data.distamigo);
                        valor2=Data.porteria_izq;//Buscar en todos los parentesis el de (g l)
                        porteriaRival=Data.porteria_der;
                        distanciaPase=pase.distancia;
                        valorpase=pase.direccion;;//Funcion para obtener el mas cercano
                        valor3=Data.porteria_izq_dist;//Buscar en todos los parentesis el de (g r)
                    }
                    if(valor2!=""){
                        porteria=valor2;
                        distPor=valor3;
                    }
                    if(Data.pelota!=""){
                        bola = true;
                        bool pases;
                        if(jugador.equipo==1)
                        pases=(Data.centro_abajo!="");//Los de la derecha la pasan si ven la de abajo
                        else
                         pases=Data.centro_arriba!="";
                        double variable=stod(Data.pelota);
                        if(variable<0.6&&porteria!=""&&stod(distPor)<40){//Cebollon
                            int offset = (std::rand() % 2 == 0) ? 9 : -9;
                            if(stod(distPor)>22)
                                offset=(std::rand() % 2 == 0) ? 7 : -7;
                            int Palo=stod(porteria)+offset;
                            udp_socket.sendTo("(kick 100 "+to_string(Palo)+")", server_udp);
                        }
                        else if(variable<0.6&&porteria!=""&&stod(distPor)<50){//Conduccion a la porteria
                            udp_socket.sendTo("(kick 30 "+porteria+")", server_udp);
                        }else if(variable<0.6&&valorpase!=""&&porteriaRival==""&&(porteria!=""||pases)){//No miro hacia mi porteria pero veo a un amigo
                            int paseplus=stod(distanciaPase)+20;//Mas potencia para el pase
                            if(paseplus<20)
                                paseplus=30;
                            udp_socket.sendTo("(kick "+to_string(paseplus)+" "+valorpase+")", server_udp);
                        }else if(variable<0.6&&porteria==""&&(porteriaRival!=""||Data.centro_abajo!=""||Data.centro_arriba!="")){//Tengo el balon y no veo la porteria rival pero si otras flags
                            udp_socket.sendTo("(kick 15 90)", server_udp);
                            this_thread::sleep_for(std::chrono::milliseconds(150));
                        }else if(variable<0.6&&porteria!=""){//Tengo el balon y veo la porteria rival
                            udp_socket.sendTo("(kick 100 "+porteria+")", server_udp);
                        }
                        else if(porteria==""&&variable<0.6&&porteriaRival==""&&Data.centro_abajo==""&&Data.centro_arriba==""){
                                            udp_socket.sendTo("(kick 40 180)", server_udp);
                                        }
                        else if(abs(stod(Data.pelota_angle))>45){
                            udp_socket.sendTo("(turn "+Data.pelota_angle+")", server_udp);
                        }else if(jugador.equipo==-1&&MasCercaBola(Data.direccionamigo2,Data.distamigo2,variable,2)){
                            udp_socket.sendTo("(dash 100 "+Data.pelota_angle+")", server_udp);
                        }else if(jugador.equipo==1&&MasCercaBola(Data.direccionamigo,Data.distamigo,variable,2)){
                            udp_socket.sendTo("(dash 100 "+Data.pelota_angle+")", server_udp);
                        }else if(porteria!=""){//No soy el mas cercano a la bola pero en lugar de quedarme parado voy a cambiar mi X
                            Decisionamos(jugador,stod(porteria),server_udp,udp_socket,stod(distPor));
                        }
                    }
                if(!bola){
                    udp_socket.sendTo("(turn 90)", server_udp);
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
                    if(jugador.tipoJugador!="Portero")
                    PosicionarJugador(jugador, server_udp,udp_socket);
                    this_thread::sleep_for(std::chrono::seconds(5));
                }
            }
            return Data;
        }

        #endif // FUNCIONES_H
