#include <gtest/gtest.h>
#include "../funciones.h" // Asegúrate de incluir el archivo donde se encuentran las funciones que deseas probar
#include <MinimalSocket/udp/UdpSocket.h>

// Test para la función vectorpalabras
TEST(VectorPalabrasTest, BasicTest) {
    string ejercicio = "Hola mundo! Esto es una prueba.";
    vector<string> expected = {"Hola", "mundo!", "Esto", "es", "una", "prueba."};
    EXPECT_EQ(vectorpalabras(ejercicio), expected);
}

// Test para la función obtenerPrimeraPalabra
TEST(ObtenerPrimeraPalabraTest, BasicTest) {
    string str = "Hola mundo!";
    string expected = "Hola";
    EXPECT_EQ(obtenerPrimeraPalabra(str), expected);
}

// Test para la función dividir_en_palabras_parentesis
TEST(DividirEnPalabrasParentesisTest, BasicTest) {
    string s = "(move 10 20) (kick 30 40)";
    vector<string> expected = {"(move 10 20)", "(kick 30 40)"};
    EXPECT_EQ(dividir_en_palabras_parentesis(s), expected);
}

// Test para la función crearMove
TEST(CrearMoveTest, BasicTest) {
    Posicion pos = {10, 20};
    string expected = "(move 10 20)";
    EXPECT_EQ(crearMove(pos), expected);
}

// Test para la función encontrarStringConPrefijo
TEST(EncontrarStringConPrefijoTest, BasicTest) {
    string str = "(move 10 20) (kick 30 40)";
    string prefijo = "(move";
    vector<string> expected = {"(move", "10", "20)"};
    EXPECT_EQ(encontrarStringConPrefijo(str, prefijo), expected);
}

// Test para la función TriangularPos2
TEST(TriangularPos2Test, BasicTest) {
    vector<string> lecturaFlag = {"1", "2", "3", "4", "5"};
    vector<string> lecturaFlag2 = {"6", "7", "8", "9", "10"};
    Posicion Pos2 = {10, 20};
    Posicion Pos1 = {30, 40};
    Posicion expected = {25, 35}; // Resultado esperado, ajusta según lo necesario
    EXPECT_EQ(TriangularPos2(lecturaFlag, lecturaFlag2, Pos2, Pos1), expected);
}

// Test para la función TriangularPos
TEST(TriangularPosTest, BasicTest) {
    vector<string> lecturaFlag = {"1", "2", "3", "4", "5"};
    vector<string> lecturaFlag2 = {"6", "7", "8", "9", "10"};
    Posicion Pos2 = {10, 20};
    Posicion Pos1 = {30, 40};
    Posicion expected = {25, 35}; // Resultado esperado, ajusta según lo necesario
    EXPECT_EQ(TriangularPos(lecturaFlag, lecturaFlag2, Pos2, Pos1), expected);
}

// Test para la función ClasificaDatos
TEST(ClasificaDatosTest, BasicTest) {
    string tipo = "see";
    vector<string> cadenas = {"(b)", "(g r)", "(g l)"};
    vector<string> palabras = {"play_on"};
    Jugador jugador; // Ajusta según sea necesario
    Lectura expected; // Ajusta según sea necesario
    EXPECT_EQ(ClasificaDatos(tipo, cadenas, palabras, jugador), expected);
}

// Test para la función PosicionarJugador
TEST(PosicionarJugadorTest, BasicTest) {
    Jugador jugador; // Ajusta según sea necesario
    MinimalSocket::Address server_udp; // Ajusta según sea necesario
    MinimalSocket::udp::Udp<true> udp_socket; // Ajusta según sea necesario
    // Se espera que esta función no genere errores
    PosicionarJugador(jugador, server_udp, udp_socket);
}

// Test para la función DetectarMasCercano
TEST(DetectarMasCercanoTest, BasicTest) {
    vector<string> direcciones = {"10", "20", "30"};
    vector<string> distancias = {"1", "2", "3"};
    Decision expected = {"1", "10"}; // Resultado esperado, ajusta según lo necesario
    EXPECT_EQ(DetectarMasCercano(direcciones, distancias), expected);
}

// Test para la función MasCercaBola
TEST(MasCercaBolaTest, BasicTest) {
    vector<string> direcciones = {"10", "20", "30"};
    vector<string> distancias = {"1", "2", "3"};
    double dist = 0.5;
    int num = 2;
    // Ajusta según lo necesario para el resultado esperado
    bool expected = true; // true si se espera que sea verdadero, false si se espera que sea falso
    EXPECT_EQ(MasCercaBola(direcciones, distancias, dist, num), expected);
}

// Agrega más tests según sea necesario para cubrir todas las funciones y casos posibles...
