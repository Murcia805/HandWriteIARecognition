#include "MACROS.h"
#include "BMPImageLoader.h"
#include "Neural_Net.h"
#include <ctime>
#include <sstream>
/*
 * Bibliografias
 * https://mattmazur.com/2015/03/17/a-step-by-step-backpropagation-example/
 */

//IGNORA ESTO HASTA EL MAIN.
string easyPathOfFolder(const string folderPath, const int folderNumber,
                        const string folder, const string prefix, const int fileNumeration, const string postfix){
    //Helps getting the path of enumerated files in a folder
    std::stringstream converter1;
    string result = folderPath;
    result += "\\";
    converter1 << folderNumber;
    result += converter1.str();
    result += folder;
    result += "\\";
    result += prefix;
    std::stringstream converter2;
    if(fileNumeration < 10) converter2 << 0;
    converter2 << fileNumeration;
    result += converter2.str();
    result += postfix;
    return std::move(result);
}

template<typename T>
struct is_pointer { static const bool value = false; };

template<typename T>
struct is_pointer<T*> { static const bool value = true; };

template<typename T>
void func(const vector<T>& v) {
    std::cout << "is it a pointer? " << is_pointer<T>::value << std::endl;
}

int main() {
    /*TODO Lee los comentarios de Neural_Net.h, Intenta No cambiar el grafo o lo de las imagenes
     *TODO Ponele 8 neuronas de salida, utiliza Topology para definir la red
     *TODO El vector de una imagen se consigue cargando la imagen y llamando la funcion patron() que retorna un vector
     */
    //srand tiene que ser llamado en el main!!!!!!!!
    std::srand((unsigned int)std::time(NULL));//RANDOM!!!!!
    rand();

    //Cargar todas las imagenes en un vector de doubles que representan cada patron
    vector<vector<double>> patronesDeImagenes;//Estos son los inputs para el entrenamiento
    vector<vector<double>> expectedVals;//Los valores esperados, cada output representa un bit. son 8 bits para asi representar letras y numeros
    //El 0 es 48 en ascii = 00110001 y se suma de a uno hasta el 9 = 00111001
    /*
     * 0 = 00110000
     * 1 = 00110001
     * 2 = 00110010
     * 3 = 00110011
     * 4 = 00110100
     * 5 = 00110101
     * 6 = 00110110
     * 7 = 00110111
     * 8 = 00111000
     * 9 = 00111001
     */
    //Creamos los 8 vectores que representan la salida
    vector<double> l0 = {0,0,1,1,0,0,0,0};
    vector<double> l1 = {0,0,1,1,0,0,0,1};
    vector<double> l2 = {0,0,1,1,0,0,1,0};
    vector<double> l3 = {0,0,1,1,0,0,1,1};
    vector<double> l4 = {0,0,1,1,0,1,0,0};
    vector<double> l5 = {0,0,1,1,0,1,0,1};
    vector<double> l6 = {0,0,1,1,0,1,1,0};
    vector<double> l7 = {0,0,1,1,0,1,1,1};
    vector<double> l8 = {0,0,1,1,1,0,0,0};
    vector<double> l9 = {0,0,1,1,1,0,0,1};
    vector<vector<double>> tempExpectedVals;
    tempExpectedVals.push_back(l0);
    tempExpectedVals.push_back(l1);
    tempExpectedVals.push_back(l2);
    tempExpectedVals.push_back(l3);
    tempExpectedVals.push_back(l4);
    tempExpectedVals.push_back(l5);
    tempExpectedVals.push_back(l6);
    tempExpectedVals.push_back(l7);
    tempExpectedVals.push_back(l8);
    tempExpectedVals.push_back(l9);

    //Primero Conseguimos los patrones de cada imagen y las guardamos enun vector: patronDeImagenes
    BMPImage<double> imageLoader;
    string pathToFolders = "C:\\Users\\Guaberx\\Desktop\\HandWriteRecognition - Copy\\Corpus\\Images";
    string nameOfFolderX = "_Training";
    string singleFileName = "_";
    string fileType = ".bmp";
    uint32_t numeroDeCarpetas = 10;
    uint32_t numeroDeArchivosPorCarpeta = 36;
    for (int i = 0; i < numeroDeCarpetas; ++i) {//Each Folder
        for (int j = 1; j <= numeroDeArchivosPorCarpeta; ++j) {//Each File from folder
            //cout << easyPathOfFolder(pathToFolders,i,nameOfFolderX,singleFileName,j,fileType) << endl;
            imageLoader.read(easyPathOfFolder(pathToFolders,i,nameOfFolderX,singleFileName,j,fileType));
            imageLoader.normalize();
            patronesDeImagenes.push_back(imageLoader.patron());//Metemos el vector normalizado de esa imagen a nuestros patrones
            expectedVals.push_back(tempExpectedVals.at(i % numeroDeCarpetas));//Agregamos los valores esperados correspondientes
        }
    }
    assert(!patronesDeImagenes.empty());

    //Red neuronal

    Topology top1(patronesDeImagenes.at(0).size(),patronesDeImagenes.at(0).size(),8);
    Net net0(top1);

    net0.train(patronesDeImagenes,expectedVals,1);
    net0.getResult(patronesDeImagenes.at(0));

    return 0;
    /*
     * Guardar imagen en vector                                                     V
     * Binarizarla:                                                                 V
     *      Coger el ((.red + .green + .blue)/3) < umbral  127 o algo asi
     * Imprimirla                                                                   V
     * Esqueletizarla (filtros de sobel o conney)                                   X
     * Guardarla en una base de datos
     * Corpus: 80% Training; 10% Pruebas, Ajustes, Tuning; 10% Evaluacion, Test
     * Metodo: Leave One Out
     * Distancia euclidiana del promedio del vector prueba de un tipo de lo que metemos para saber a cual se parece mas
     */
}