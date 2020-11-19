
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#include <stdio.h>

//----------------------------------------------------------------------------
//Boje za figure - Colors for figures:
//----------------------------------------------------------------------------
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define RES    "\x1B[0m"


typedef struct {
    char kor2c, nkor2c; int kor1, nkor1;
} coordinate;

//----------------------------------------------------------------------------
//Potrebne funkcije - Required functions:
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//calculate coordinate
//----------------------------------------------------------------------------
int koordinata(char c) {
    fflush(stdin);
    int k = -1;
    switch (c) {
    case 'a': k = 2; break;
    case 'b': k = 3; break;
    case 'c': k = 4; break;
    case 'd': k = 5; break;
    case 'e': k = 6; break;
    case 'f': k = 7; break;
    case 'g': k = 8; break;
    case 'h': k = 9; break;
    }
    return k;
}
//----------------------------------------------------------------------------
//is it white figure (red)
//----------------------------------------------------------------------------
int beli(char c) {
    char b[6] = { 'r','n','b','q','k','p' };
    int i;
    for (i = 0; i < 6; i++) if (c == b[i]) return 1; //yes
    return 0; //no
}

//----------------------------------------------------------------------------
//is it black figure (green)
//----------------------------------------------------------------------------
int crni(char c) {
    char b[6] = { 'R','N','B','Q','K','P' };
    int i;
    for (i = 0; i < 6; i++) if (c == b[i]) return 1; //yes
    return 0; //no
}

//----------------------------------------------------------------------------
//print the table (in colour)
//----------------------------------------------------------------------------
void IspisTable(char a[10][12]) {
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 12; j++) {
            if (beli(a[i][j]) && (i < 9)) { if (a[i][j] == 'p')  printf(RED "%c ", a[i][j]); else  printf(RED "%c ", a[i][j] - 32); }
            else if (crni(a[i][j])) { if (a[i][j] == 'P') printf(GRN "%c ", a[i][j] + 32); else printf(GRN "%c ", a[i][j]); }
            else printf(RES "%c ", a[i][j]);
        }
        printf("\n");
    }
}

//----------------------------------------------------------------------------
//whether the selected field is correct
//example: field a2 -> proveraOkvira(2, 'a') = 0 -> yes
//----------------------------------------------------------------------------
int proveraOkvira(int k1, char k2c) {
    if (k1 < 0 || k1>7)  return 1;                       //NO
    char a[8] = { 'a','b','c','d','e','f','g','h' };
    int i;
    for (i = 0; i < 8; i++) if (k2c == a[i])  return 0;  //YES
    if (i == 8) return 1;                                //NO
    //return 0; //needlessly
}

//----------------------------------------------------------------------------
//whether the selected figures are the same type (red & red or green & green)
//----------------------------------------------------------------------------
int svojaFigura(char c1, char c2) {
    if (beli(c1) && beli(c2)) return 1;
    if (crni(c1) && crni(c2)) return 1;
    return 0;                              //NO
}

//--------------------------------------------------------------------------
//Are the rules of the game for the knight met
//inputs: char which is denoting a knight, coordinates of start position and coordinates of end position
//--------------------------------------------------------------------------
int pravilaKonj(char k, int k1, int k2, int nk1, int nk2) {
    if ((k != 'N') && (k != 'n')) return 0;                   //Skip checking if the figure is not knight
    int a, b, c;
    a = k1 - 2;
    b = k2 + 1;
    c = k2 - 1;
    if ((nk1 == a) && ((nk2 == b) || (nk2 == c))) return 0;
    a = k1 + 2;
    if ((nk1 == a) && ((nk2 == b) || (nk2 == c))) return 0;
    a = k2 - 2;
    b = k1 + 1;
    c = k1 - 1;
    if ((nk2 == a) && ((nk1 == b) || (nk1 == c))) return 0;
    a = k2 + 2;
    if ((nk2 == a) && ((nk1 == b) || (nk1 == c))) return 0;
    return 1;                                               //The rules are not satisfied!
}

//--------------------------------------------------------------------------
//Are the rules of the game for the pawn met
//inputs: char which is denoting a pawn, char which is denoting an attacked figure/field, char which is denoting coordinates of start position and coordinates of end position
//TODO: implement "en passant" - recommendation: use arrays of "coordinate" structure
//--------------------------------------------------------------------------
int pravilaPijun(char a, char b, int k1, int k2, int nk1, int nk2) {
    if ((a != 'p') && (a != 'P')) return 0;
    int y1, y2, y3, y4;
    y1 = k2 - 1;
    y2 = k2 + 1;
    y3 = k1 - nk1 - 1;
    y4 = k1 - nk1 - 2;
    if (a == 'p') {
        if ((y3 == 0) && ((nk2 == y1) || (nk2 == y2)) && (crni(b)))  return 0;
        if ((y3 == 0) && (b == 'x') && (nk2 == k2))  return 0;
        if ((y4 == 0) && (b == 'x') && (nk2 == k2) && (nk1 > 3))  return 0;
    }
    y3 = nk1 - k1 - 1;
    y4 = nk1 - k1 - 2;
    if (a == 'P') {
        if ((y3 == 0) && ((nk2 == y1) || (nk2 == y2)) && (beli(b)))  return 0;
        if ((y3 == 0) && (b == 'x') && (nk2 == k2))   return 0;
        if ((y4 == 0) && (b == 'x') && (nk2 == k2) && (nk1 < 4))   return 0;
    }
    return 1;
}

//--------------------------------------------------------------------------
//Prohibition of playing with an opponent's figure
//inputs: player indicator, figure
//--------------------------------------------------------------------------
int nemojTudjom(int i, char c) {
    if (i == 0) {
        if (crni(c)) return 1;
    }
    if (i == 1) {
        if (beli(c)) return 1;
    }
    return 0;
}

//--------------------------------------------------------------------------
//Are the rules of the game for the rook met
//inputs: char which is denoting a rook, coordinates of start position and coordinates of end position
//--------------------------------------------------------------------------
int pravilaTop(char t, int k1, int k2, int nk1, int nk2) {
    if ((t != 'R') && (t != 'r')) return 0; //skip
    if (k1 == nk1) return 0;                //rules are satisfied
    if (k2 == nk2) return 0;                //rules are satisfied
    return 1;                               //rules are not satisfied
}

//--------------------------------------------------------------------------
//Are the rules of the game for the bishop met
//inputs: char which is denoting a bishop, coordinates of start position and coordinates of end position
//--------------------------------------------------------------------------
int pravilaLovac(char  t, int k1, int k2, int nk1, int nk2) {
    if ((t != 'B') && (t != 'b')) return 0;
    int i;
    for (i = 1; i < 8; i++) {
        if ((nk1 == (k1 + i)) && (nk2 == (k2 - i))) return 0;
        if ((nk1 == (k1 + i)) && (nk2 == (k2 + i))) return 0;
        if ((nk1 == (k1 - i)) && (nk2 == (k2 - i))) return 0;
        if ((nk1 == (k1 - i)) && (nk2 == (k2 + i))) return 0;
    }
    return 1;
}

//--------------------------------------------------------------------------
//Are the rules of the game for the queen met
//inputs: char which is denoting a queen, coordinates of start position and coordinates of end position
//--------------------------------------------------------------------------
int pravilaKraljica(char  t, int k1, int k2, int nk1, int nk2) {
    if ((t != 'Q') && (t != 'q')) return 0;
    if (k1 == nk1) return 0;
    if (k2 == nk2) return 0;
    int i;
    for (i = 1; i < 8; i++) {
        if ((nk1 == (k1 + i)) && (nk2 == (k2 - i))) return 0;
        if ((nk1 == (k1 + i)) && (nk2 == (k2 + i))) return 0;
        if ((nk1 == (k1 - i)) && (nk2 == (k2 - i))) return 0;
        if ((nk1 == (k1 - i)) && (nk2 == (k2 + i))) return 0;
    }
    return 1;
}

//--------------------------------------------------------------------------
//Small castling
//C - denoting red, Z - denoting green
//s{c,z} -chess to the corresponding king
// TODO : add checking for last rule
//--------------------------------------------------------------------------
int malaRokada(char c, char n[4], char m[4], int sc, int sz, int k1, int k2, int nk1, int nk2, int pomC, int pomZ, int pomtC, int pomtZ) {

    if ((c != 'k') && (c != 'K')) return 0;                                //skip if the king is not selected figure

    if (c == 'k') {
        if (pomC != 0) return 0;                                           //skip if the king is moved
        if (pomtC != 0) return 0;                                          //skip if the rook is moved
        if ((k1 != 7) || (k2 != 6) || (nk1 != 7) || (nk2 != 8)) return 0;  //skip if the king is not moving properly
        if (sc != 0) return 0;                                             //skip if the king is in the chess
        //checking that are the appropriate figures on  appropriate fields:
        if (n[0] != 'k') return 0;
        if (n[3] != 'r') return 0;
        if (n[1] != 'x') return 0;
        if (n[2] != 'x') return 0;
        //upadate fields:
        n[0] = 'x';
        n[3] = 'x';
        n[1] = 'r';
        n[2] = 'k';
    }
    if (c == 'K') {                                                        //the same for another king
        if (pomZ != 0) return 0;
        if (pomtZ != 0) return 0;
        if (k1 != 0 || k2 != 6 || nk1 != 0 || nk2 != 8) return 0;
        if (sz != 0) return 0;
        if (m[0] != 'K') return 0;
        if (m[3] != 'R') return 0;
        if (m[1] != 'x') return 0;
        if (m[2] != 'x') return 0;

        m[0] = 'x';
        m[3] = 'x';
        m[1] = 'R';
        m[2] = 'K';
    }

    return 1;                                                              //castling is applied
}

//--------------------------------------------------------------------------
//Big castling
//C - denoting red, Z - denoting green
//s{c,z} -chess to the corresponding king
//TODO : add checking for last rule
//--------------------------------------------------------------------------
int velikaRokada(char c, char n[5], char m[5], int sc, int sz, int k1, int k2, int nk1, int nk2, int pomC, int pomZ, int pomtC, int pomtZ) {

    if ((c != 'k') && (c != 'K')) return 0;

    if (c == 'k') {
        if (pomC != 0) return 0;
        if (pomtC != 0) return 0;
        if ((k1 != 7) || (k2 != 6) || (nk1 != 7) || (nk2 != 4)) return 0;
        if (sc != 0) return 0;
        if (n[0] != 'r')   return 0;
        if (n[4] != 'k') return 0;
        if (n[1] != 'x') return 0;
        if (n[2] != 'x') return 0;
        if (n[3] != 'x') return 0;
        n[2] = 'k';
        n[3] = 'r';
        n[1] = 'x';
        n[4] = 'x';
        n[0] = 'x';
    }
    if (c == 'K') {
        if (pomZ != 0) return 0;
        if (pomtZ != 0) return 0;
        if (k1 != 0 || k2 != 6 || nk1 != 0 || nk2 != 4) return 0;
        if (sz != 0) return 0;
        if (m[0] != 'R')   return 0;
        if (m[4] != 'K') return 0;
        if (m[1] != 'x') return 0;
        if (m[2] != 'x') return 0;
        if (m[3] != 'x') return 0;
        m[2] = 'K';
        m[3] = 'R';
        m[1] = 'x';
        m[4] = 'x';
        m[0] = 'x';
    }

    return 1;                                                    //castling is applied
}

//--------------------------------------------------------------------------
//Are the rules of the game for the king met
//inputs: char which is denoting a king, coordinates of start position, coordinates of end position and indicators for small and big castling
//--------------------------------------------------------------------------
int pravilaKralj(char  t, int k1, int k2, int nk1, int nk2, int rok, int rokV) {
    if (rok == 1) return 0;
    if (rokV == 1) return 0;
    if ((t != 'K') && (t != 'k')) return 0;
    if ((nk1 == k1) && (nk2 == (k2 + 1))) return 0;
    if ((nk1 == k1) && (nk2 == (k2 - 1))) return 0;
    if ((nk2 == k2) && (nk1 == (k1 + 1))) return 0;
    if ((nk2 == k2) && (nk1 == (k1 - 1))) return 0;
    if ((nk1 == (k1 + 1)) && (nk2 == (k2 - 1))) return 0;
    if ((nk1 == (k1 + 1)) && (nk2 == (k2 + 1))) return 0;
    if ((nk1 == (k1 - 1)) && (nk2 == (k2 - 1))) return 0;
    if ((nk1 == (k1 - 1)) && (nk2 == (k2 + 1))) return 0;
    return 1;
}

//--------------------------------------------------------------------------
//If the figure needs to pass several fields in a straight line,
//we check if those fields are empty
//--------------------------------------------------------------------------
int prazanProstorPravo(char t, int k1, int k2, int nk1, int nk2, char w[6]) {

    //skip for irrelevant figures:
    if ((t == 'B') || (t == 'b')) return 0;
    if ((t == 'K') || (t == 'k')) return 0;
    if ((t == 'N') || (t == 'n')) return 0;

    int i;
    for (i = 0; i < 6; i++)
        if (w[i] != 'x') return 1;           //space is not empty!

    return 0;
}

//--------------------------------------------------------------------------
//If the figure needs to pass several fields in an oblique line,
//we check if those fields are empty
//--------------------------------------------------------------------------
int prazanProstorKoso(char t, int k1, int k2, int nk1, int nk2, char w[6]) {
    if ((t == 'R') || (t == 'r')) return 0;
    if ((t == 'P') || (t == 'p')) return 0;
    if ((t == 'K') || (t == 'k')) return 0;
    if ((t == 'N') || (t == 'n')) return 0;
    int i;
    for (i = 0; i < 6; i++)
        if (w[i] != 'x') return 1;

    return 0;
}

//--------------------------------------------------------------------------
//Find the row of the figure
//--------------------------------------------------------------------------
int pronadjiVrstuFigure(char t, char a[10][12]) {
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 12; j++) { if (a[i][j] == t) return i; }

    }
    return -1;
}

//--------------------------------------------------------------------------
//Find the column of the figure
//--------------------------------------------------------------------------
int pronadjiKolonuFigure(char t, char a[10][12]) {
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 12; j++) { if (a[i][j] == t) return j; }

    }
    return -1;
}

//--------------------------------------------------------------------------
//Does a figure attack the red king in a straight line?
//input: king's row/column
//--------------------------------------------------------------------------
int sahPravo1(char n[8]) {
    int i, k, f;
    f = -1;
    for (i = 0; i < 8; i++) {
        if (n[i] == 'k') {
            k = i;
            break;
        }
    }
    for (i = 0; i < 8; i++) {
        if ((n[i] == 'Q') || (n[i] == 'R')) {
            f = i;
            break;
        }
    }
    if (f == -1) return 0;
    if (f > k) {
        for (i = k + 1; i < f; i++) {
            if (n[i] != 'x') return 0;
        }
    }
    if (k > f) {
        for (i = f + 1; i < k; i++) {
            if (n[i] != 'x') return 0;
        }
    }
    return 1;
}

//--------------------------------------------------------------------------
//Does a figure attack the green king in a straight line?
//input: king's row/column
//Use this only for row/column which contains the king!
//--------------------------------------------------------------------------
int sahPravo2(char n[8]) {
    int i, k, f;
    f = -1;
    for (i = 0; i < 8; i++) {
        if (n[i] == 'K') {      //the king must exist
            k = i;
            break;
        }
    }
    for (i = 0; i < 8; i++) {
        if ((n[i] == 'q') || (n[i] == 'r')) {
            f = i;
            break;
        }
    }
    if (f == -1) return 0;
    if (f > k) {
        for (i = k + 1; i < f; i++) {
            if (n[i] != 'x') return 0;
        }
    }
    if (k > f) {
        for (i = f + 1; i < k; i++) {
            if (n[i] != 'x') return 0;
        }
    }
    return 1;
}

//--------------------------------------------------------------------------
//a, &gDijagonalaKralja, ak1, ok1
//  ak1 = pronadjiVrstuFigure('k', a);
//  ok1 = pronadjiKolonuFigure('k', a);
//make the diagonal
//--------------------------------------------------------------------------
void napraviDijagonalu(char m[10][12], char n[8], int a, int o) {
    int i, j;
    j = 0;
    for (i = 8; i > 0; i--) {
        if (((a - i) < 0) || ((o - i) < 2)) continue;
        n[j] = m[a - i][o - i]; j++;
    }
    n[j] = m[a][o]; j++;
    for (i = 1; i < 9; i++) {
        if (((a + i) > 7) || ((o + i) > 9)) continue;
        n[j] = m[a + i][o + i]; j++;
    }
}

//--------------------------------------------------------------------------
//a, &sDijagonalaKralja, ak1, ok1
//make the side diagonal
//--------------------------------------------------------------------------
void napravi_S_Dijagonalu(char m[10][12], char n[8], int a, int o) {
    int i, j;
    j = 0;
    for (i = 8; i > 0; i--) {                          
        if (((a + i) > 7) || ((o - i) < 2)) continue;
        n[j] = m[a + i][o - i]; j++;
    }
    n[j] = m[a][o]; j++;
    for (i = 1; i < 9; i++) {
        if (((a - i) < 0) || ((o + i) > 9)) continue;
        n[j] = m[a - i][o + i]; j++;
    }
}

//--------------------------------------------------------------------------
//Is the red king in the chess?
//cheking of one diagonal
//--------------------------------------------------------------------------
int sahKoso1(char n[8]) {
    int i, k, f;
    f = -1;
    for (i = 0; i < 8; i++) {
        if (n[i] == 'k') {
            k = i;
            break;
        }
    }
    for (i = 0; i < 8; i++) {
        if ((n[i] == 'Q') || (n[i] == 'B')) {
            f = i;
            break;
        }
    }
    if (f == -1) return 0;
    if (f > k) {
        for (i = k + 1; i < f; i++) {
            if (n[i] != 'x') return 0;
        }
    }
    if (k > f) {
        for (i = f + 1; i < k; i++) {
            if (n[i] != 'x') return 0;
        }
    }
    return 1;
}

//--------------------------------------------------------------------------
//Is the green king in the chess?
//cheking of one diagonal
//--------------------------------------------------------------------------
int sahKoso2(char n[8]) {
    int i, k, f;
    f = -1;
    for (i = 0; i < 8; i++) {
        if (n[i] == 'K') {
            k = i;
            break;
        }
    }
    for (i = 0; i < 8; i++) {
        if ((n[i] == 'q') || (n[i] == 'b')) {
            f = i;
            break;
        }
    }
    if (f == -1) return 0;
    if (f > k) {
        for (i = k + 1; i < f; i++) {
            if (n[i] != 'x') return 0;
        }
    }
    if (k > f) {
        for (i = f + 1; i < k; i++) {
            if (n[i] != 'x') return 0;
        }
    }
    return 1;
}

//--------------------------------------------------------------------------
//Is the red king in the chess from knight?
//--------------------------------------------------------------------------
int sahKonj1(char n[10][12], int ak1, int ok1) {
    int a, b, c;
    a = ak1 - 2;
    b = ok1 + 1;
    c = ok1 - 1;
    if (n[a][b] == 'N') return 1;
    if (n[a][c] == 'N')  return 1;
    a = ak1 + 2;
    if (n[a][b] == 'N') return 1;
    if (n[a][c] == 'N')  return 1;
    a = ok1 - 2;
    b = ak1 + 1;
    c = ak1 - 1;
    if (n[b][a] == 'N') return 1;
    if (n[c][a] == 'N')  return 1;
    a = ok1 + 2;
    if (n[b][a] == 'N') return 1;
    if (n[c][a] == 'N') return 1;
    return 0;
}

//--------------------------------------------------------------------------
//Is the green king in the chess from knight?
//--------------------------------------------------------------------------
int sahKonj2(char n[10][12], int ak1, int ok1) {
    int a, b, c;
    a = ak1 - 2;
    b = ok1 + 1;
    c = ok1 - 1;
    if (n[a][b] == 'n') return 1;
    if (n[a][c] == 'n')  return 1;
    a = ak1 + 2;
    if (n[a][b] == 'n') return 1;
    if (n[a][c] == 'n')  return 1;
    a = ok1 - 2;
    b = ak1 + 1;
    c = ak1 - 1;
    if (n[b][a] == 'n') return 1;
    if (n[c][a] == 'n')  return 1;
    a = ok1 + 2;
    if (n[b][a] == 'n') return 1;
    if (n[c][a] == 'n') return 1;
    return 0;
}

//--------------------------------------------------------------------------
//Can the red king move?
//NOT USED YET 
//TODO: use this function for checkmate
//--------------------------------------------------------------------------
int mozeLiBezatiC(char a[10][12], int ak1, int ok1) {     //ne koristim ovu f-ju, ona je za mat
    if ((a[ak1 + 1][ok1 + 1] == 'x') || crni(a[ak1 + 1][ok1 + 1])) return 1;
    if ((a[ak1 - 1][ok1 - 1] == 'x') || crni(a[ak1 - 1][ok1 - 1])) return 1;
    if ((a[ak1 + 1][ok1 - 1] == 'x') || crni(a[ak1 + 1][ok1 - 1])) return 1;
    if ((a[ak1 - 1][ok1 + 1] == 'x') || crni(a[ak1 - 1][ok1 + 1])) return 1;
    return 0;
}

//--------------------------------------------------------------------------
//Glavni program
//Main program
//--------------------------------------------------------------------------
int main()
{

    //--------------------------------------------------------------------------
    //Pocetni izgled table
    //The initial appearance of the board
    //--------------------------------------------------------------------------
    printf("%s", "Developed by Nikola Filipovic\n\n");
    printf("%s", "Copyright © 2018\n\n\n");
    printf("%s", "Da se predate unesite r0 - To resign enter r0 \n");
    printf("%s", "Pocetna pozicija - Start position\n\n");
    char a[10][12] = { {'8',' ','R','N','B','Q','K','B','N','R',' ','8'},{'7',' ','P','P','P','P','P','P','P','P',' ','7'},{'6',' ','x','x','x','x','x','x','x','x',' ','6'},{'5',' ','x','x','x','x','x','x','x','x',' ','5'},{'4',' ','x','x','x','x','x','x','x','x',' ','4'},{'3',' ','x','x','x','x','x','x','x','x',' ','3'},{'2',' ','p','p','p','p','p','p','p','p',' ','2'},{'1',' ','r','n','b','q','k','b','n','r',' ','1'},{' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},{' ',' ','a','b','c','d','e','f','g','h',' ',' '} };
    IspisTable(a);

    //--------------------------------------------------------------------------
    //Potrebne promenljive
    //Required variables
    //--------------------------------------------------------------------------
    int brojacPoteza = 0;                                                               //counter of moves 
    int kor1, kor2, ostatak; int mat = 0;                                               //the first and the second coordinate (int), checkmate
    coordinate redCoor[27]; coordinate greenCoor[27];                                   //arrays of moves                                        
    int ii = 0; int jj = 0;                                                             //array's index
    int rok, rokV;                                                                      //small and big castling
    char kor2c, OdabranaFigura, NapadnutaFigura, c;                                     //the second coordinate(char), selected figure, attacked figure, something
    int nkor1, nkor2, i;                                                                //new coordinates, and something                                     
    char nkor2c;                                                                        //the new second coordinate(char)
    int indikator1 = 0; int indikator2 = 0; int indikator3 = 0; int indikator4 = 0;     //indicators for chess
    int indikator1z = 0; int indikator2z = 0; int indikator3z = 0; int indikator4z = 0; //indicators for chess
    int ip1 = 0; int ik1 = 0; int ip1z = 0; int ik1z = 0;                               //indicators for chess from pawn/knight
    char nc[4] = { 'x','x','x','x' };                                                   //arrays for red/green small/big castling:
    char nz[4] = { 'x','x','x','x' };
    char vc[5] = { 'x','x','x','x','x' };
    char vz[5] = { 'x','x','x','x','x' };
    int sc = 0; int sz = 0;                                                              //chess to red, chess to green king
    int stampajLog=0;                                                                    //print the log
    int pomeranKralj_beli_crveni = 0; int pomeranLeviTopC = 0; int pomeranDesniTopC = 0; //is white king moved, is left red rook moved, is right red rook moved
    int pomeranKralj_crni_zeleni = 0; int pomeranLeviTopZ = 0; int pomeranDesniTopZ = 0; //is black king moved, is left green rook moved, is right green rook moved

    //--------------------------------------------------------------------------
    //Ciklus igranja paritje
    //The game playing cycle
    //--------------------------------------------------------------------------
    while (brojacPoteza < 54) {

        //--------------------------------------------------------------------------
        //Selekcija figure
        //Figure selection
        //--------------------------------------------------------------------------
        ostatak = brojacPoteza % 2;
        if (ostatak) printf("Vuce zeleni - Green is on the move:\n");
        else printf("Vuce crveni - Red is on the move:\n");
        printf("%s", " Izaberi figuru - Choose a figure:\n");
        scanf("%c%d", &kor2c, &kor1);
        while (getchar() != '\n');
        if (kor2c == 'r' && kor1 == 0) { 
            if (ostatak) printf("%s", " Crveni je pobedio! - Red won! \n");
            else printf("%s", " Zeleni je pobedio! - Green won! \n");
            break;
        }
        if (ostatak) { greenCoor[jj].kor2c = kor2c; greenCoor[jj].kor1 = kor1; }
        else { redCoor[ii].kor2c = kor2c; redCoor[ii].kor1 = kor1; };
        kor2 = koordinata(kor2c);
        kor1 = 8 - kor1;
        if (proveraOkvira(kor1, kor2c)) { printf("%s", "Izasli ste iz okvira table! - You stepped out of the board frame!"); continue; }
        if (nemojTudjom(ostatak, a[kor1][kor2])) { printf("%s", "Ne mozete igrati tudjom figurom! - You can't play with someone else's figure!"); continue; }
        OdabranaFigura = a[kor1][kor2];
        if (OdabranaFigura == 'x') { printf("%s", "Izabrali ste prazno polje!- You have selected an empty field!"); continue; }

        //--------------------------------------------------------------------------
        //Odabir novog mesta za figuru
        //Choosing a new place for the figure
        //--------------------------------------------------------------------------
        printf("%s", " Polje gde igrate - The field where you play:\n");
        scanf("%c%d", &nkor2c, &nkor1);
        while (getchar() != '\n');
        if (ostatak) { greenCoor[jj].nkor2c = nkor2c; greenCoor[jj].nkor1 = nkor1; }
        else { redCoor[ii].nkor2c = nkor2c; redCoor[ii].nkor1 = nkor1; };
        nkor2 = koordinata(nkor2c);
        nkor1 = 8 - nkor1;
        if (proveraOkvira(nkor1, nkor2c)) { printf("%s", "Izasli ste iz okvira table! - You stepped out of the board frame!"); continue; }
        NapadnutaFigura = a[nkor1][nkor2];
        if (svojaFigura(OdabranaFigura, NapadnutaFigura)) { printf("%s", "Izvinite, ali pojeli ste svoju! - Sorry, but you ate yours! "); continue; }
        if (pravilaKonj(OdabranaFigura, kor1, kor2, nkor1, nkor2)) { printf("%s", "Ne valja konj! - The knight is incorrect!"); continue; }
        if (pravilaPijun(OdabranaFigura, NapadnutaFigura, kor1, kor2, nkor1, nkor2)) { printf("%s", "Ne valja pijun! - The pawn is incorrect!"); continue; }
        if (pravilaTop(OdabranaFigura, kor1, kor2, nkor1, nkor2)) { printf("%s", "Ne valja top! - The rook is incorrect!"); continue; }
        if (pravilaLovac(OdabranaFigura, kor1, kor2, nkor1, nkor2)) { printf("%s", "Ne valja lovac! - The bishop is incorrect!"); continue; }
        if (pravilaKraljica(OdabranaFigura, kor1, kor2, nkor1, nkor2)) { printf("%s", "Ne valja kraljica! - The queen is incorrect!"); continue; }

        //--------------------------------------------------------------------------
        //prepare arrays for castlings:
        //--------------------------------------------------------------------------
        for (i = 0; i < 4; i++)
            nc[i] = a[7][i + 6];
        for (i = 0; i < 4; i++)
            nz[i] = a[0][i + 6];
        for (i = 0; i < 5; i++)
            vc[i] = a[7][i + 2];
        for (i = 0; i < 5; i++)
            vz[i] = a[0][i + 2];

        rok = malaRokada(a[kor1][kor2], &nc, &nz, sc, sz, kor1, kor2, nkor1, nkor2, pomeranKralj_beli_crveni, pomeranKralj_crni_zeleni, pomeranDesniTopC, pomeranDesniTopZ);
        rokV = velikaRokada(a[kor1][kor2], &vc, &vz, sc, sz, kor1, kor2, nkor1, nkor2, pomeranKralj_beli_crveni, pomeranKralj_crni_zeleni, pomeranLeviTopC, pomeranLeviTopZ);

        if (pravilaKralj(OdabranaFigura, kor1, kor2, nkor1, nkor2, rok, rokV)) { printf("%s", "Ne valja kralj! - The king is incorrect!"); continue; }
        sc = 0; sz = 0;

        //--------------------------------------------------------------------------
        //Proveravamo da li smo pregazili nedozvoljena polja - PRAVO
        //We're checking to see if we've run over illegal fields - in a straight line
        //--------------------------------------------------------------------------
        char w[6] = { 'x', 'x', 'x', 'x', 'x', 'x' };
        if (kor1 == nkor1) {
            if (nkor2 > kor2) { for (i = 0; i < (nkor2 - kor2) - 1; i++) w[i] = a[kor1][kor2 + i + 1]; }
            if (kor2 > nkor2) { for (i = 0; i < (kor2 - nkor2) - 1; i++) w[i] = a[kor1][nkor2 + i + 1]; }
        }
        if (kor2 == nkor2) {
            if (nkor1 > kor1) { for (i = 0; i < (nkor1 - kor1) - 1; i++) w[i] = a[kor1 + i + 1][kor2]; }
            if (kor1 > nkor1) { for (i = 0; i < (kor1 - nkor1) - 1; i++) w[i] = a[nkor1 + i + 1][kor2]; }
        }

        if (prazanProstorPravo(OdabranaFigura, kor1, kor2, nkor1, nkor2, w)) { printf("%s", "PREGAZIO pravo! "); continue; }

        //--------------------------------------------------------------------------
        //Proveravamo da li smo pregazili nedozvoljena polja - KOSO
        //We're checking to see if we've run over illegal fields - along an oblique line
        //--------------------------------------------------------------------------
        char v[6] = { 'x', 'x', 'x', 'x', 'x', 'x' };
        int j;
        for (i = 1; i < 8; i++) {
            if ((nkor1 == (kor1 + i)) && (nkor2 == (kor2 - i))) for (j = 0; j < (nkor1 - kor1) - 1; j++)  v[j] = a[kor1 + j + 1][kor2 - j - 1];
            if ((nkor1 == (kor1 + i)) && (nkor2 == (kor2 + i))) for (j = 0; j < (nkor1 - kor1) - 1; j++)  v[j] = a[kor1 + j + 1][kor2 + j + 1];
            if ((nkor1 == (kor1 - i)) && (nkor2 == (kor2 - i))) for (j = 0; j < (kor1 - nkor1) - 1; j++)  v[j] = a[kor1 - j - 1][kor2 - j - 1];
            if ((nkor1 == (kor1 - i)) && (nkor2 == (kor2 + i))) for (j = 0; j < (kor1 - nkor1) - 1; j++)  v[j] = a[kor1 - j - 1][kor2 + j + 1];
        }

        if (prazanProstorKoso(OdabranaFigura, kor1, kor2, nkor1, nkor2, v)) { printf("%s", "Pregazio koso! - Run over illegal fields along an oblique line! "); continue; }

        //--------------------------------------------------------------------------
        //Da li je kralj u dozvoljenoj poziciji? Promenljive
        //Is the king in the allowed position? Variables:
        //--------------------------------------------------------------------------
        int ak1, ok1;
        char vrstaKralja[8] = { 'x','x','x','x','x','x','x','x' };
        char kolonaKralja[8] = { 'x','x','x','x','x','x','x','x' };
        char gDijagonalaKralja[8] = { 'x','x','x','x','x','x','x','x' };
        char sDijagonalaKralja[8] = { 'x','x','x','x','x','x','x','x' };

        //--------------------------------------------------------------------------
        //Promena stanja na tabli
        //Change the state of the board
        //--------------------------------------------------------------------------
        if (rok || rokV) {
            if (rok) {
                if (OdabranaFigura == 'k')   for (i = 0; i < 4; i++)  a[7][i + 6] = nc[i];
                if (OdabranaFigura == 'K')   for (i = 0; i < 4; i++)  a[0][i + 6] = nz[i];
            }
            if (rokV) {
                if (OdabranaFigura == 'k')   for (i = 0; i < 5; i++)  a[7][i + 2] = vc[i];
                if (OdabranaFigura == 'K')   for (i = 0; i < 5; i++)  a[0][i + 2] = vz[i];
            }
        }
        else {
            a[kor1][kor2] = 'x';
            a[nkor1][nkor2] = OdabranaFigura;
        }
        if ((nkor1 == 0) && (a[nkor1][nkor2] == 'p')) a[nkor1][nkor2] = 'q';
        if ((nkor1 == 7) && (a[nkor1][nkor2] == 'P')) a[nkor1][nkor2] = 'Q';
        //    IspisTable(a);

        //--------------------------------------------------------------------------
        //Da li je kralj u dozvoljenoj poziciji? Provera!
        //Is the king in the allowed position? Checking!
        //--------------------------------------------------------------------------

        if (ostatak == 0) {

            //--------------------------------------------------------------------------
            //Cetiri relevantna niza za crvenog kralja - Four relevant arrays for the red king:
            //--------------------------------------------------------------------------

            for (int i = 0; i < 8; i++) {
                vrstaKralja[i] = 'x';
                kolonaKralja[i] = 'x';
                gDijagonalaKralja[i] = 'x';
                sDijagonalaKralja[i] = 'x';
            }

            ak1 = pronadjiVrstuFigure('k', a);
            ok1 = pronadjiKolonuFigure('k', a);

            for (i = 0; i < 8; i++) {
                vrstaKralja[i] = a[ak1][i + 2];
            }
            for (i = 0; i < 8; i++) {
                kolonaKralja[i] = a[i][ok1];
            }
            napraviDijagonalu(a, &gDijagonalaKralja, ak1, ok1);
            napravi_S_Dijagonalu(a, &sDijagonalaKralja, ak1, ok1);

            //--------------------------------------------------------------------------
            //Provera da li je crveni kralj u sahu - Check if the red king is in chess
            //Update indicators(flegs)
            //--------------------------------------------------------------------------
            if (sahPravo1(kolonaKralja)) {  indikator1 = 1; sc = 1; }
            else indikator1 = 0;
            if (sahPravo1(vrstaKralja)) { indikator2 = 1; sc = 1; }
            else indikator2 = 0;
            if (sahKoso1(gDijagonalaKralja)) { indikator3 = 1; sc = 1; }
            else indikator3 = 0;
            if (sahKoso1(sDijagonalaKralja)) { indikator4 = 1; sc = 1; }
            else indikator4 = 0;
            if ((a[ak1 - 1][ok1 - 1] == 'P') || (a[ak1 - 1][ok1 + 1] == 'P')) { ip1 = 1; sc = 1; }
            else ip1 = 0;

            if (sahKonj1(a, ak1, ok1)) { ik1 = 1; sc = 1; }
            else ik1 = 0;

            if ((indikator1 == 1) || (indikator2 == 1) || (indikator3 == 1) || (indikator4 == 1) || (ip1 == 1) || (ik1 == 1)) {
                printf("\n Sah crvenom, igraj opet! - Chess to red player, play again!  \n");
                a[kor1][kor2] = OdabranaFigura;
                a[nkor1][nkor2] = NapadnutaFigura;
                if (rok) { a[7][9] = 'r'; a[7][7] = 'x'; }
                if (rokV) { a[7][2] = 'r'; a[7][5] = 'x'; }
                IspisTable(a);
                continue;
            }

            //-------------------------------------------------------------------------
            // Cetiri relevantna niza za zelenog kralja - - Four relevant arrays for the green king:
            //-------------------------------------------------------------------------

            for (int i = 0; i < 8; i++) {
                vrstaKralja[i] = 'x';
                kolonaKralja[i] = 'x';
                gDijagonalaKralja[i] = 'x';
                sDijagonalaKralja[i] = 'x';
            }

            ak1 = pronadjiVrstuFigure('K', a);
            ok1 = pronadjiKolonuFigure('K', a);

            for (i = 0; i < 8; i++) {
                vrstaKralja[i] = a[ak1][i + 2];
            }
            for (i = 0; i < 8; i++) {
                kolonaKralja[i] = a[i][ok1];
            }
            napraviDijagonalu(a, &gDijagonalaKralja, ak1, ok1);
            napravi_S_Dijagonalu(a, &sDijagonalaKralja, ak1, ok1);

            //--------------------------------------------------------------------------
            //Provera da li je zeleni kralj u sahu - Check if the green king is in chess
            //Update indicators(flegs). Print the messages
            //--------------------------------------------------------------------------
            if (sahPravo2(kolonaKralja)) { printf("\n SAH zelenom! - Chess to green player! \n"); indikator1z = 1; sz = 1; }
            else indikator1z = 0;
            if (sahPravo2(vrstaKralja)) { printf("\n SAH zelenom! - Chess to green player! \n"); indikator2z = 1; sz = 1; }
            else indikator2z = 0;
            if (sahKoso2(gDijagonalaKralja)) { printf("\n SAH zelenom! - Chess to green player! \n"); indikator3z = 1; sz = 1; }
            else indikator3z = 0;
            if (sahKoso2(sDijagonalaKralja)) { printf("\n SAH zelenom! - Chess to green player! \n"); indikator4z = 1; sz = 1; }
            else indikator4z = 0;
            if ((a[ak1 + 1][ok1 - 1] == 'p') || (a[ak1 + 1][ok1 + 1] == 'p')) { printf("\n SAH zelenom od pijuna! - Chess to green player! \n"); ip1z = 1; sz = 1; }
            else ip1z = 0;

            if (sahKonj2(a, ak1, ok1)) { printf("\n SAH zelenom od konja! - Chess to green player!\n"); ik1z = 1; sz = 1; }
            else ik1z = 0;

            //--------------------------------------------------------------------------
            //TODO: Dodaj proveru za mat zelenom! - Add a checking for checkmate to the green!
            //*by using and updating mat variable
            //--------------------------------------------------------------------------
            if (mat) {
                printf("\n Mat zelenom! Crveni je pobedio! - Red won! \n");
                break;
            }

            indikator1z = 0;
            indikator2z = 0;
            indikator3z = 0;
            indikator4z = 0;
            ip1z = 0;
            ik1z = 0;
            sz = 0;

        } //if ostatak == 0

        if (ostatak == 1) {

            //--------------------------------------------------------------------------
            //Cetiri relevantna niza za zelenog kralja - Four relevant arrays for the green king:
            //--------------------------------------------------------------------------

            for (int i = 0; i < 8; i++) {
                vrstaKralja[i] = 'x';
                kolonaKralja[i] = 'x';
                gDijagonalaKralja[i] = 'x';
                sDijagonalaKralja[i] = 'x';
            }

            ak1 = pronadjiVrstuFigure('K', a);
            ok1 = pronadjiKolonuFigure('K', a);

            for (i = 0; i < 8; i++) {
                vrstaKralja[i] = a[ak1][i + 2];
            }
            for (i = 0; i < 8; i++) {
                kolonaKralja[i] = a[i][ok1];
            }
            napraviDijagonalu(a, &gDijagonalaKralja, ak1, ok1);
            napravi_S_Dijagonalu(a, &sDijagonalaKralja, ak1, ok1);

            //--------------------------------------------------------------------------
            //Provera da li je zeleni kralj u sahu - Check if the green king is in chess
            //Update indicators(flegs)
            //--------------------------------------------------------------------------
            if (sahPravo2(kolonaKralja)) { indikator1z = 1; sz = 1; }
            else indikator1z = 0;
            if (sahPravo2(vrstaKralja)) { indikator2z = 1; sz = 1; }
            else indikator2z = 0;
            if (sahKoso2(gDijagonalaKralja)) { indikator3z = 1; sz = 1; }
            else indikator3z = 0;
            if (sahKoso2(sDijagonalaKralja)) { indikator4z = 1; sz = 1; }
            else indikator4z = 0;
            if ((a[ak1 + 1][ok1 - 1] == 'p') || (a[ak1 + 1][ok1 + 1] == 'p')) { ip1z = 1; sz = 1; }
            else ip1z = 0;

            if (sahKonj2(a, ak1, ok1)) { ik1z = 1; sz = 1; }
            else ik1z = 0;

            if ((indikator1z == 1) || (indikator2z == 1) || (indikator3z == 1) || (indikator4z == 1) || (ip1z == 1) || (ik1z == 1)) {
                printf("\n SAH zelenom, igraj opet! \n");
                a[kor1][kor2] = OdabranaFigura;
                a[nkor1][nkor2] = NapadnutaFigura;
                if (rok) { a[0][9] = 'R'; a[0][7] = 'x'; }
                if (rokV) { a[0][2] = 'R'; a[0][5] = 'x'; }
                IspisTable(a);
                continue;
            }

            //--------------------------------------------------------------------------
            //Cetiri relevantna niza za crvenog kralja - Four relevant arrays for the red king:
            //--------------------------------------------------------------------------

            for (int i = 0; i < 8; i++) {
                vrstaKralja[i] = 'x';
                kolonaKralja[i] = 'x';
                gDijagonalaKralja[i] = 'x';
                sDijagonalaKralja[i] = 'x';
            }

            ak1 = pronadjiVrstuFigure('k', a);
            ok1 = pronadjiKolonuFigure('k', a);

            for (i = 0; i < 8; i++) {
                vrstaKralja[i] = a[ak1][i + 2];
            }
            for (i = 0; i < 8; i++) {
                kolonaKralja[i] = a[i][ok1];
            }
            napraviDijagonalu(a, &gDijagonalaKralja, ak1, ok1);
            napravi_S_Dijagonalu(a, &sDijagonalaKralja, ak1, ok1);

            //--------------------------------------------------------------------------
            //Provera da li je crveni kralj u sahu - Check if the red king is in chess
            //Update indicators(flegs). Print the messages
            //--------------------------------------------------------------------------
            if (sahPravo1(kolonaKralja)) { printf("\n SAH crvenom1! - Chess to red player! \n"); indikator1 = 1; sc = 1; }
            else indikator1 = 0;
            if (sahPravo1(vrstaKralja)) { printf("\n SAH crvenom2! - Chess to red player! \n"); indikator2 = 1; sc = 1; }
            else indikator2 = 0;
            if (sahKoso1(gDijagonalaKralja)) { printf("\n SAH crvenom3! - Chess to red player! \n"); indikator3 = 1; sc = 1; }
            else indikator3 = 0;
            if (sahKoso1(sDijagonalaKralja)) { printf("\n SAH crvenom4! - Chess to red player! \n"); indikator4 = 1; sc = 1; }
            else indikator4 = 0;
            if ((a[ak1 - 1][ok1 - 1] == 'P') || (a[ak1 - 1][ok1 + 1] == 'P')) { printf("\n SAH crvenom od pijuna! - Chess to red player! \n"); ip1 = 1; sc = 1; }
            else ip1 = 0;

            if (sahKonj1(a, ak1, ok1)) { printf("\n SAH crvenom od konja! - Chess to red player!\n"); ik1 = 1; sc = 1; }
            else ik1 = 0;

            //--------------------------------------------------------------------------
            //TODO: Dodaj proveru za mat crvenom! Add a checking for checkmate to the red!
            //*by using and updating mat variable
            //--------------------------------------------------------------------------
            if (mat) {
                printf("\n Mat crvenom! Zeleni je pobedio! - Green won! \n");
                break;
            }

            indikator1 = 0;
            indikator2 = 0;
            indikator3 = 0;
            indikator4 = 0;
            ip1 = 0;
            ik1 = 0;
            sc = 0;

        } //if(ostatak==1)

        //--------------------------------------------------------------------------
        //Print status at the end of one cycle
        //--------------------------------------------------------------------------
        IspisTable(a);

        if (ostatak) jj++;
        else ii++;
        brojacPoteza++;
        printf("\n Preostalo jos: %d poteza \n ", 54 - brojacPoteza);

        //--------------------------------------------------------------------------
        //Castling indicators
        //--------------------------------------------------------------------------
        if (a[7][6] != 'k') pomeranKralj_beli_crveni++;
        if (a[0][6] != 'K') pomeranKralj_crni_zeleni++;
        if (a[7][9] != 'r') pomeranDesniTopC++;
        if (a[7][2] != 'r') pomeranLeviTopC++;
        if (a[0][2] != 'R') pomeranLeviTopZ++;
        if (a[0][9] != 'R') pomeranDesniTopZ++;

    } //while

    if (brojacPoteza == 54) printf("Prekoracen dozvoljen broj poteza! Nereseno! - Permitted number of moves exceeded! Draw!");

    printf("\nDa li zelite da pogledate log partije? - Do you want to see the log of the game? 1 - YES, 0 - NO \n ");
    scanf("%d", &stampajLog);
    while (getchar() != '\n');
    if (stampajLog == 1) {
        printf("\n");
        printf("Log: \n");
        for (int i = 0,  iiPom = 0,  jjPom = 0; i < brojacPoteza; i++) {
            if (i % 2) {
                printf("%d. Green played %c%d --> %c%d \n", i + 1, greenCoor[jjPom].kor2c, greenCoor[jjPom].kor1, greenCoor[jjPom].nkor2c, greenCoor[jjPom].nkor1);
                jjPom++;
            }
            else {
                printf("%d. Red played %c%d --> %c%d \n", i + 1, redCoor[iiPom].kor2c, redCoor[iiPom].kor1, redCoor[iiPom].nkor2c, redCoor[iiPom].nkor1);
                iiPom++;
            }
        };
    }

    return 0;

} //main
