#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <thread> // Para std::this_thread::sleep_for
#include <chrono> // Para std::chrono::seconds
#include "funciones.h"
#include "tictoc.h"
#include <MinimalSocket/udp/UdpSocket.h>

using namespace std;



int main(int argc, char* argv[])
{
    TicToc clock;
    Lectura Estructura;
    string argumentoString = argv[2];
    MinimalSocket::Port this_socket_port = atoi(argv[0]);
    cout << "Creating a UDP socket" << endl;
    MinimalSocket::udp::Udp<true> udp_socket(this_socket_port, MinimalSocket::AddressFamily::IP_V6);
    cout << "Socket created" << endl;
    bool success = udp_socket.open();
    if (!success)
    {
        cout << "Error opening socket" << endl;
        return 1;
    }
        // send a message to another udp
        MinimalSocket::Address other_recipient_udp = MinimalSocket::Address{"127.0.0.1", 6000};
        udp_socket.sendTo("(init "+argumentoString+"(version 19))", other_recipient_udp);
        //udp_socket.sendTo("(init pOESIAC(version 19))", other_recipient_udp);
        cout << "Message sent" << endl;
        // receive a message from another udp reaching this one
        std::size_t message_max_size = 10000;
        cout << "Waiting for a message" << endl;
        auto received_message = udp_socket.receive(message_max_size);
        // check the sender address
        MinimalSocket::Address other_sender_udp = received_message->sender;
        // access the received message
        // resized to the nunber of bytes
        // actually received
        std::string received_message_content = received_message->received_message;
        //cout << received_message_content << endl;

        Jugador jugador;
                jugador.numero = stoi(vectorpalabras(received_message_content).at(2));
                if (vectorpalabras(received_message_content).at(1) == "l"){
                    jugador.equipo = -1;
                }else{
                    jugador.equipo = 1;
                }
        vector <string>palabras=vectorpalabras(received_message_content);

        for(const auto &palabra : palabras) {
              //cout << palabra << endl;
          }
        MinimalSocket::Address server_udp = MinimalSocket::Address{"127.0.0.1", other_sender_udp.getPort()};

        PosicionarJugador(jugador, server_udp,udp_socket,argumentoString);
        //clock.tic();

        while (true)
        {
           /*while (clock.toc() < 100){
                std::this_thread::sleep_for(std::chrono::milliseconds(1));                }
            clock.tic();*/
            string resultado;
            auto received_message = udp_socket.receive(message_max_size);
            std::string received_message_content = received_message->received_message;
            string contenido = received_message_content.substr(1, received_message_content.size() - 2);
            auto tipo=obtenerPrimeraPalabra(contenido);
            vector<string> cadenas = dividir_en_palabras_parentesis(contenido);
            //cadenas.push_back(tipo);

            cadenas.insert(cadenas.begin(), tipo); // Inserta la primera palabra al principio del vector

            for (int i = 0; i < cadenas.size(); ++i) {
                resultado += cadenas.at(i);
            }
            auto pPalabras=vectorpalabras(resultado);
            Estructura = ClasificaDatos(tipo, cadenas, pPalabras);
            Estructura = Accion ( jugador,Estructura, server_udp, udp_socket);
        }
        return 0;
    }

