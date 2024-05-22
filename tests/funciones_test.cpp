#include <gtest/gtest.h>
#include "../funciones.h"
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
    string s = "((move 10 20) (kick 30 40))";
    vector<string> expected = {"((move 10 20)", "(kick 30 40))"};
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

