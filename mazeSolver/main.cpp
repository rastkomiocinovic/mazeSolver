#include <iostream>
#include <climits>
#include <fstream>

using namespace std;

class Graf {
	bool* vektor = nullptr;
	int n = 0;

	bool& dohvatiIz(bool* vekt, int n, unsigned int v, unsigned int k) const {
		unsigned int vrsta, kolona;
		if (v > k) vrsta = k, kolona = v;
		else vrsta = v, kolona = k;
		unsigned int pomeraj =
			vrsta * n + kolona - vrsta * (vrsta + 1) / 2;
		return vekt[pomeraj];
	}

public:
	void pisiGraf() {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				cout << dohvatiGranu(i, j) << " ";
			}
			cout << endl;
		}
	}
	void napraviPrazanGraf(int Size) {
		n = Size;
		try {
			vektor = new bool[(n * (n + 1) / 2)];
		}
		catch (bad_alloc) {
			cerr << "NEDOVOLJNO MEMORIJE" << endl;
			system("pause");
			return;
		}
		for (int i = 0; i < (n * (n + 1) / 2); i++) {
			vektor[i] = false;
		}
	}
	bool postoji() { return 0 != n; }
	bool& dohvatiGranu(unsigned int v, unsigned int k) const {
		return dohvatiIz(vektor, n, v, k);
	}
	void brisiGraf() {
		delete[] vektor;
		vektor = nullptr;
		n = 0;
	}

	void dodajGranu(int cvor1, int cvor2) {
		if (cvor1 >= n || cvor2 >= n) {
			cerr << "GRESKA: NEPOSTOJECI CVOR/CVOROVI" << endl;
			system("pause");
			return;
		}

		dohvatiGranu(cvor1, cvor2) = true;
	}

	void ukloniGranu(int cvor1, int cvor2) {
		if (cvor1 >= n || cvor2 >= n) {
			cerr << "GRESKA: NEPOSTOJECI CVOR/CVOROVI" << endl;
			system("pause");
			return;
		}

		dohvatiGranu(cvor1, cvor2) = false;
	}

	void dodajCvor() {
		int n2 = n + 1;
		bool* noviVektor = nullptr;
		try {
			noviVektor = new bool[(n2 * (n2 + 1) / 2)];
		}
		catch (bad_alloc) {
			cerr << "NEDOVOLJNO MEMORIJE" << endl;
			system("pause");
			return;
		}
		for (int i = 0; i < n; i++) {
			for (int j = 0; j <= i; j++) {
				dohvatiIz(noviVektor, n2, i, j) = dohvatiIz(vektor, n, i, j);
			}
			dohvatiIz(noviVektor, n2, i, n) = false;
		}
		for (int j = 0; j < n2; j++) {
			dohvatiIz(noviVektor, n2, n2 - 1, j) = false;
		}

		delete[] vektor;
		vektor = noviVektor;
		n = n2;
	}
	void izbaciCvor(int cvor) {
		if (cvor >= n) {
			cout << "GRESKA: CVOR NE POSTOJI" << endl;
			system("pause");
			return;
		}

		int n2 = n - 1;
		bool* noviVektor = nullptr;
		try {
			noviVektor = new bool[(n2 * (n2 + 1) / 2)];
		}
		catch (bad_alloc) {
			cerr << "NEDOVOLJNO MEMORIJE" << endl;
			system("pause");
			return;
		}
		bool posleCvorai = false;
		for (int i = 0; i < n; i++) {
			if (cvor != i) {
				bool posleCvoraj = false;
				for (int j = 0; j <= i; j++) {
					if (cvor != j)
						dohvatiIz(noviVektor, n2, posleCvorai ? i - 1 : i, posleCvoraj ? j - 1 : j) = dohvatiIz(vektor, n, i, j);
					else
						posleCvoraj = true;
				}
			}
			else
				posleCvorai = true;
		}

		delete[] vektor;
		vektor = noviVektor;
		n = n2;
	}

};

struct Pozicija {
	int x;
	int y;
	Pozicija(int X = 0, int Y = 0) : x(X), y(Y) {}
	friend bool operator==(const Pozicija& levi, const Pozicija& desni) { return (levi.x == desni.x && levi.y == desni.y); }
	friend bool operator!=(const Pozicija& levi, const Pozicija& desni) { return (levi.x != desni.x || levi.y != desni.y); }
	friend ostream& operator<<(ostream& os, const Pozicija& poz) { os << "(" << poz.x << "," << poz.y << ")"; return os; }
};
int dimX, dimY;
Pozicija Ulaz, Izlaz;
Graf g;


int id(Pozicija poz) {
	return dimX * poz.y + poz.x;
}
void pisiLavirint(ostream& os) {
	Pozicija currPos(0, 0);
	for (int i = 0; i < 2 * dimX + 1; i++) {
		if (Izlaz != Pozicija(i / 2, 0) && Ulaz != Pozicija(i / 2, 0))
			os << "#";
		else
			if (i % 2 == 1)
				os << " ";
			else
				os << "#";
	}
	os << endl;
	for (int j = 0; j < dimY; j++) {
		currPos.y = j;
		if (Izlaz != Pozicija(0, j) && Ulaz != Pozicija(0, j))
			os << "#";
		else
			os << " ";
		// crtanje desne granice:
		for (int i = 0; i < dimX - 1; i++) {
			currPos.x = i;
			Pozicija desni = currPos;
			desni.x++;
			if (currPos == Ulaz) {
				os << "o";
			}
			else if (currPos == Izlaz) {
				os << "x";
			}
			else {
				os << " ";
			}
			if (g.dohvatiGranu(id(currPos), id(desni))) {
				os << " ";
			}
			else
				os << "#";
		}
		currPos.x++;
		if (currPos == Izlaz)
			os << "x" << " " << endl;
		else
			os << " " << "#" << endl;

		if (j != dimY - 1) {
			for (int i = 0; i < dimX; i++) {
				os << "#";
				currPos.x = i;
				Pozicija donji = currPos;
				donji.y++;
				if (g.dohvatiGranu(id(currPos), id(donji))) {
					os << " ";
				}
				else
					os << "#";

			}
			os << "#" << endl;
		}
	}
	for (int i = 0; i < 2 * dimX + 1; i++) {
		if (Izlaz != Pozicija(i / 2, dimY - 1) && Ulaz != Pozicija(i / 2, dimY - 1))
			os << "#";
		else
			if (i % 2 == 1)
				os << " ";
			else
				os << "#";
	}
	os << endl;
}


int cvorSaNajmanjimRastojanjem(int* rastojanje, bool* nadjenoNajkraceRastojanje) {
	int min = INT_MAX;
	int minId = 0;

	for (int i = 0; i < dimX * dimY; i++) {
		if (nadjenoNajkraceRastojanje[i] == false && rastojanje[i] <= min) {
			min = rastojanje[i];
			minId = i;
		}
	}
	return minId;
}

class Stack {
	struct node {
		int id;
		node* next;
		node(int ID) { next = nullptr; id = ID; }
	};
	node* sp = nullptr;

public:
	void push(int id) {
		node* novi = new node(id);
		novi->next = sp;
		sp = novi;
	}
	int pop() {
		int id = sp->id;
		node* old = sp;
		sp = sp->next;
		delete old;
		return id;
	}
	bool empty() {
		return sp == nullptr;
	}
	~Stack() {
		while (sp != nullptr) {
			node* old = sp;
			sp = sp->next;
			delete old;
		}
	}

};

Pozicija poz(int ID) {
	return Pozicija(ID % dimX, ID / dimX);
}

void pisiPut(int* prethodni, int i) {
	Stack S;
	while (prethodni[i] != -1) {
		S.push(i);
		i = prethodni[i];
	}
	S.push(i);

	cout << poz(S.pop());
	while (!S.empty()) {
		cout << " -> " << poz(S.pop());
	}
	cout << endl;
}

void Dijkstra(const Graf& g, int idUlaza, int idIzlaza) {
	const int N = dimX * dimY;
	int* rastojanje = new int[N];
	bool* nadjenoNajkraceRastojanje = new bool[N];
	int* prethodni = new int[N];
	for (int i = 0; i < N; i++) {
		rastojanje[i] = INT_MAX;
		nadjenoNajkraceRastojanje[i] = false;
		prethodni[i] = -1;
	}
	rastojanje[idUlaza] = 0;
	for (int i = 0; i < N - 1; i++) {
		int curr = cvorSaNajmanjimRastojanjem(rastojanje, nadjenoNajkraceRastojanje);
		nadjenoNajkraceRastojanje[curr] = true;
		for (int j = 0; j < N; j++) {
			if (!nadjenoNajkraceRastojanje[j] && g.dohvatiGranu(curr, j) && rastojanje[curr] + 1 < rastojanje[j]) {
				prethodni[j] = curr;
				rastojanje[j] = rastojanje[curr] + 1;
				if (j == idIzlaza) {
					cout << "Najkraci put je: ";
					pisiPut(prethodni, idIzlaza);
					delete[] rastojanje;
					delete[] nadjenoNajkraceRastojanje;
					delete[] prethodni;
					return;
				}
			}
		}
	}
	cout << "Ne postoji put od ulaza do izlaza." << endl;
	delete[] rastojanje;
	delete[] nadjenoNajkraceRastojanje;
	delete[] prethodni;
}

void izbor1() {
	if (!g.postoji()) {
		system("cls");
		cout << "Dimenzije lavirinta su (X Y): ";
		cin >> dimX >> dimY;
		while (true) {
			system("cls");
			cout << "Koordinate ulaza su (X Y): ";
			cin >> Ulaz.x >> Ulaz.y;
			if (Ulaz.x == 0 || Ulaz.y == 0 || Ulaz.x == dimX - 1 || Ulaz.y == dimY - 1)
				break;
			else {
				cout << "GRESKA: KOORDINATE ULAZA NISU VALIDNE" << endl;
				system("pause");
			}
		}
		while (true) {
			system("cls");
			cout << "Koordinate izlaza su (X Y): ";
			cin >> Izlaz.x >> Izlaz.y;
			if (Izlaz.x == 0 || Izlaz.y == 0 || Izlaz.x == dimX - 1 || Izlaz.y == dimY - 1)
				break;
			else {
				cerr << "GRESKA: KOORDINATE ULAZA NISU VALIDNE" << endl;
				system("pause");
			}
		}
		g.napraviPrazanGraf(dimX * dimY);
		while (true) {
			system("cls");
			cout << "Napravi prolaz izmedju cvorova (X1 Y1 X2 Y2)(-1 za izlaz): ";
			int x1, x2, y1, y2;
			cin >> x1;
			if (x1 == -1)
				break;
			else {
				cin >> y1 >> x2 >> y2;
				while (x1 != x2) {
					g.dodajGranu(id(Pozicija(x1, y1)), id(Pozicija(x1 + (x1 < x2 ? 1 : -1), y1)));
					x1 += x1 < x2 ? 1 : -1;
				}
				while (y1 != y2) {
					g.dodajGranu(id(Pozicija(x1, y1)), id(Pozicija(x1, y1 + (y1 < y2 ? 1 : -1))));
					y1 += y1 < y2 ? 1 : -1;
				}
			}
		}
	}
}
void izbor2() {
	if (!g.postoji()) {
		return;
	}
	if (dimX > 80 || dimY > 50) {
		system("cls");
		cerr << "GRESKA: LAVIRINT SE NE MOZE ISPISATI NA STANDARDNI IZLAZ" << endl;
	}
	else
		pisiLavirint(cout);
}
void izbor3() {
	if (!g.postoji()) {
		return;
	}
	if (g.postoji()) {
		ofstream file("Lavirint.txt");
		pisiLavirint(file);
	}
}

int main() {

	system("cls");
	while (true) {
		system("cls");
		if (!g.postoji()) {
			cout << "[1] Napravi lavirint" << endl;
		}
		else {
			cout << "[2] Pisi lavirint na standardni izlaz" << endl;
			cout << "[3] Pisi lavirint u datoteku" << endl;
			cout << "[4] Pronadji najkraci put Dijkstrinim algoritmom" << endl;
			cout << "[5] Brisi Lavirint" << endl;
		}
		cout << "[0] Izlaz iz programa" << endl;
		cout << "Izbor: ";
		int izbor;
		cin >> izbor;

		switch (izbor)
		{
		case 1:
			izbor1();
			break;
		case 2:
			izbor2();
			system("pause");
			break;
		case 3:
			izbor3();
			break;
		case 4:
			Dijkstra(g, id(Ulaz), id(Izlaz));
			system("pause");
			break;
		case 5:
			g.brisiGraf();
			break;
		case 0:
			return 0;
		}
	}


	return 0;
}