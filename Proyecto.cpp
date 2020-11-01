// Proyecto.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>
#include <sstream>
#include <cstring>
#include <stack> 
using namespace std;

//spliteamos el string
template <class Container>
void split2(const std::string& str, Container& cont, char delim = '|')
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}

//funcion para leer y reemplazar
bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}


//lista enlazada
class Node {
public:
    string ingles;
    string espal;
    bool esp;
    string descripcion;
    Node* siguiente = NULL;
};

class linked_list
{
private:
    Node* head, * tail;
public:
    linked_list()
    {
        head = NULL;
        tail = NULL;
    }

    Node* gethead()
    {
        return head;
    }

    void add_node(string ingles, string espa, string des, bool esp)
    {
        Node* tmp = new Node;
        tmp->ingles = ingles;
        tmp->espal = espa;
        tmp->descripcion = des;
        tmp->esp = esp;
        tmp->siguiente = NULL;

        if (head == NULL)
        {
            head = tmp;
            tail = tmp;
        }
        else
        {
            tail->siguiente = tmp;
            tail = tail->siguiente;
        }
    }

    static void display(Node* head, int num)
    {
        if (head == NULL)
        {
            cout << "NULL" << endl;
        }
        else
        {
            cout << num << ". " << head->ingles << endl;
            display(head->siguiente,num+1);
        }
    }

    void modificar(Node* head, int val, string ing, string esp, string des) {
        int cont = 1;
        Node* tmp = head;
        while (tmp != NULL) {
            if (val == cont) {
                tmp->ingles = ing;
                tmp->espal = esp;
                tmp->descripcion = des;
                break;
            }
            cont++;
            tmp = tmp->siguiente;
        }
    }

    void del(int num, Node* cabeza)
    {
        int val = 1;
        Node* Anterior = cabeza;
        Node* actual = cabeza;
        while (actual != NULL) {
            if (val == num) {
                Anterior->siguiente = actual->siguiente;
                delete actual;
                break;
            }
            val++;
            Anterior = actual;
            actual = actual->siguiente;
        }
    }

    string regresarTrad(string ingles, Node* cabeza) {
        Node* tmp = head;
        while (tmp != NULL) {
            if (ingles.compare(tmp->ingles) == 0) {
                return tmp->espal;
            }
            tmp = tmp->siguiente;
        }
        return "{}";
    }
};

linked_list palabras;
std::stack<string> control;

void lectura_archivo() {
    // Create a text string, which is used to output the text file
    string myText;
    ifstream MyReadFile("palabras.txt"); //archivo de palabras
    while (getline(MyReadFile, myText)) {
        // Output the text from the file
        string ing,des="";
        string esp="";
        std::vector<std::string> words;
        split2(myText, words);
        for (int i = 0; i < words.size(); i++) {
            //ingles
            if (i == 1) {
                ing = words[i];
            }

            //español
            if (i == 2) {
                esp = words[i];
            }

            //descripcion
            if (i == 0) {
                des = words[i];
            }
        }
        
        //convertimos a lower case todo
        std::for_each(ing.begin(), ing.end(), [](char& c) {
            c = ::tolower(c);
        }
	 
	 );

        std::for_each(esp.begin(), esp.end(), [](char& c) {
            c = ::tolower(c);
        });

        std::for_each(des.begin(), des.end(), [](char& c) {
            c = ::tolower(c);
        });

        palabras.add_node(ing, esp, des, words.size() > 3);
    }

    // Close the file
    MyReadFile.close();
}

void ingresarPalabra() {
    string ing, esp, des;

    cout << "Palabra en ingles" << endl;
    cin >> ing;

    cout << "Traduccion en espanol" << endl;
    cin >> esp;

    cout << "Descripcion" << endl;
    cin >> des;

    palabras.add_node(ing, esp, des, true);
}

void modificarPalabra() {
    int num;
    string ing, esp, des = "";
    palabras.display(palabras.gethead(), 1);
    cout << "Eliga la palabra que quiera modificar " << endl;
    cin >> num;

    cout << "Palabra en ingles" << endl;
    cin >> ing;

    cout << "Traduccion en espanol" << endl;
    cin >> esp;

    cout << "Descripcion" << endl;
    cin >> des;
    palabras.modificar(palabras.gethead(), num, ing, esp, des);
}

void eliminarPalabra() {
    int num;
    palabras.display(palabras.gethead(), 1);
    cout << "Eliga la palabra que quiera eliminar " << endl;
    cin >> num;
    palabras.del(num,palabras.gethead());
}

void guardarPalabra() {

}

string obtenerLlaveAbierta() {
    if (control.empty() == true) {
        return "{";
    }
    return " inicio " + palabras.regresarTrad(control.top(), palabras.gethead());
}
string obtenerLlaveCerrada() {
    if (control.empty() == true) {
        return "}";
    }
    return " fin " + palabras.regresarTrad(control.top(), palabras.gethead());
}

string traduccion(string entrada) {
    Node* actual = palabras.gethead();
    while (actual != NULL) {
        if (actual->esp) {
            if (replace(entrada, actual->ingles, actual->espal) == true) {
                if (actual->ingles.compare("if") == 0) {control.push("if"); }
                else if (actual->ingles.compare("else") == 0) { control.push("else"); }
                else if (actual->ingles.compare("do") == 0) { control.push("do"); }
                else if (actual->ingles.compare("try") == 0) { control.push("try"); }
                else if (actual->ingles.compare("catch") == 0) { control.push("catch"); }
                else if (actual->ingles.compare("while") == 0) {
                    control.push("while");
                }

                else if (actual->ingles.compare("for") == 0) { control.push("for"); }
                else if (actual->ingles.compare("switch") == 0) { control.push("for"); }
            }
        }
        actual = actual->siguiente;
    }

    replace(entrada, "{", obtenerLlaveAbierta());
    if (replace(entrada, "}", obtenerLlaveCerrada()) == true){
        if (control.empty() == true) {}
        else {control.pop();}
    }
    return entrada;
}

void traducir_texto() {
    string myText;
    ifstream MyReadFile("entrada.txt");
    string resultado = "";

    while (getline(MyReadFile, myText)) {
        resultado += traduccion(myText) + "\n";
        
    }
    // Close the file
    MyReadFile.close();
    cout << resultado;
}

int main()
{
    lectura_archivo();
    //palabras.display(palabras.gethead());
    int opcion;
    while (true) {
        cout << "Que desea hacer " << endl;
        cout << "1. Ingresar palabra " << endl;
        cout << "2. Eliminar palabra " << endl;
        cout << "3. Modificar palabra " << endl;
        cout << "4. Guardar palabras " << endl;
        cout << "5. Leer archivo" << endl;
        cout << "6. Salir" << endl;
        cin >> opcion;
        switch ((int)opcion) {
            case 1: ingresarPalabra(); break;
            case 2: eliminarPalabra(); break;
            case 3: modificarPalabra(); break;
            case 4: cout << "eligio 4"; break;
            case 5: traducir_texto(); break;
            case 6: exit; break;
            default:cout << "elija bien " << endl; break;
        }
    }
    
}
