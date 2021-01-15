#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

ifstream drum("drum.txt");
ifstream inventar("inventar.txt");
ifstream copii_rai("copii-rai.txt");

template<typename T>
T adunare(T a, T b, T c)
{
    T d = a + c + b;
    return d;
} ///template pentru a lucra mai usor cu stringurile

class Scrisoare
{

public:

    string nume;
    string prenume;
    int varsta;
    string oras;
    string wishlist;
    string culoare;

    Scrisoare() {}

    ~Scrisoare(){}

    Scrisoare(string n, string p, int v, string o, string w, string c)
    {
        nume = n;
        prenume= p;
        varsta = v;
        oras = o;
        wishlist = w;
        culoare = c;
    }

};

class Elf : public Scrisoare
{

private:
    float buget;
    int i=0;
    string spatiu = " ";
    string nume_pren;

public:
    int cumintenie;
    float suma = 0;
    string traseu[6];
    int acadele;
    int totalacadele = 0;


    friend class Troll;

    Elf() {
        buget = 0;
    }

    ~Elf()  {}

    int getbug() ///functie pentru calcularea bugetului
    {
        if(cumintenie == 1) buget = 100;
        else buget = 10;
        return buget;
    }


    void verificare(Scrisoare s)
    {
        string aux;
        nume_pren = adunare(s.nume, s.prenume, spatiu); ///in variabila nume_pren se unesc numele cu prenumele pentru a putea compara cu lista copiilor rai
        while(getline(copii_rai,aux))
        {
            if(aux.compare(nume_pren) == 0)
            {
                cumintenie = 0;
                break;
            }
            else cumintenie = 1;
        }
        copii_rai.clear();
        copii_rai.seekg(0); ///se seteaza cursorul la inceputul fisierului
    }


    void cadouri(Scrisoare s)
    {
        string tok;
        string cadou;
        string listacad;
        int pret;
        verificare(s); ///se verifica pentru a aloca bugetul
        istringstream wish(s.wishlist); ///pentru a putea folosi orice operatie cu wishlist(string)
        getbug();   ///elfii iau bugetul fiecarui copil
        while(getline(wish, tok, ',')) ///separator pentru a imparti in tokenuri
        {
            while(inventar >> cadou && inventar >> pret) ///se citesc din fisierul inventar, in ordine, cadourile si pretul aferent
            {
                if(cadou.compare(tok) == 0)  ///se verifica daca exista in inventar cu aceasta functie
                {
                    if(buget - pret >= 0)
                    {
                        buget-=pret; ///daca nu se depaseste bugetul, se va scade cu pretul cadoului
                        listacad = adunare(listacad, tok, spatiu); ///in aceasta variabila se salveaza cadourile in ordinea in care au fost adaugate
                    }
                }
            }

            if(listacad == "") listacad = " Cadou Default";

            inventar.clear();
            inventar.seekg(0); ///se seteaza cursorul la inceputul fisierului pentru a mai citi o data

        }

        acadele = buget;
        totalacadele += acadele;
        traseu[0] = "Rovaniemi"; ///primul element al traseului este in Laponia
        traseu[++i] = s.oras; ///se salveaza traseul pentru a-l putea accesa pe elemente
        cout<<"Pentru "<<nume_pren<<" au fost impachetate urmatoarele cadouri:"<<listacad;

        if(acadele >= 20)
            cout<<" + "<<acadele<<" de acadele"<<endl;
        else cout<<" + "<<acadele<<" acadele"<<endl;
    }

    int getacadele()
    {
        return totalacadele;
    }
                                    ///getters
    string getoras(int i)
    {
        return traseu[i];
    }

};

class Troll : public Elf
{

private:

    int cadouriB = 0;
    int cadouriF = 0;
    int nrcarbuni = 0;

public:

    Troll() {}

    ~Troll() {}

    void getnrcad(Scrisoare s) ///functie pentru a determina numarul total de cadouri (baieti, fete)
    {
        if(s.culoare.compare("albastru") == 0) cadouriB++;
        else cadouriF++;
    }
    void carbuni(Scrisoare s)
    {
        verificare(s);
        if(cumintenie == 0)
        {
            nrcarbuni++;
        }
    }
    void afisare()
    {
        cout<<"Trolii au impachetat "<<cadouriF<<" cadouri pentru fete si "<<cadouriB<<" pentru baieti "<<endl;
    }
    int getnrcarbuni()
    {
        return nrcarbuni;
    }
};

class MosCraciun
{

public:

    int a[6][6];
    int suma = 0;

    void citire()
    {
        for(int i=0; i<6; i++)
            for(int j=0; j<6; j++)
                drum >> a[i][j];
    }

    int shortest(Elf e) ///Algoritmul lui Prim pentru un graf dens
    {
        citire();
        int edge;

        int visit[6];
        cout<<endl;
        visit[0] = true;
        for(int i=1; i<6; i++)
        {
            visit[i]=false;
        }

        edge = 0;

        while (edge++ < 5)
        {

            int min = INT_MAX;
            int x;
            int y;
            for (int i = 0; i < 6; i++)
            {
                if (visit[i])
                {
                    for (int j = 0; j < 6; j++)
                    {
                        if (!visit[j] && a[i][j])
                        {
                            if (min > a[i][j])
                            {
                                min = a[i][j];
                                x = i;
                                y = j;
                            }

                        }
                    }
                }
            }
            cout << e.getoras(x)  <<  " - " << e.getoras(y) << " : "<<a[x][y]<<" km"<<endl;
            suma+=a[x][y];
            visit[y] = true;
        }
    }

    friend class DnaCraciun;

};

class DnaCraciun : public Troll, public Elf
{
private:
    float suma;
public:
    DnaCraciun() {}

    int total(Elf e, Troll t)
    {
        suma += e.getacadele();
        suma += t.getnrcarbuni()/2;
        return suma;
    }

};

int main()
{
    Scrisoare a("Mihai", "Daniel", 15, "Ljubljana", "Masinuta,Papusa,Carte,Satelit", "albastru");
    Scrisoare b("Mercedes", "Vito", 72, "Maribor", "Papusa,Masinuta", "roz");
    Scrisoare c("Cernea", "Remus", 11, "Koper", "Satelit", "albastru");
    Scrisoare d("Alabama", "Aura", 6, "Kranj", "Carte", "roz");
    Scrisoare e("Marosi", "Zoltan", 17, "Novo Mesto", "Porsche", "albastru");

    Elf k;
    Troll t;
    DnaCraciun l;
    MosCraciun p;

    cout<<endl;
    k.cadouri(a);
    k.cadouri(b);
    k.cadouri(c);
    k.cadouri(d);
    k.cadouri(e);

    t.getnrcad(a);
    t.getnrcad(b);
    t.getnrcad(c);
    t.getnrcad(d);
    t.getnrcad(e);

    t.carbuni(a);
    t.carbuni(b);
    t.carbuni(c);
    t.carbuni(d);
    t.carbuni(e);

    cout<<endl<<"Au fost adaugati "<<t.getnrcarbuni()<<" carbuni in total"<<endl;
    cout<<endl<<"A ramas un buget extra de : "<<l.total(k,t)<<"$"<<endl;
    cout<<endl;
    t.afisare();
    cout<<endl<<"Drumul critic: "<<p.shortest(k)<<endl;
    cout<<endl<<"Distanta minima "<<p.suma<<" km";
    return 0;
}
