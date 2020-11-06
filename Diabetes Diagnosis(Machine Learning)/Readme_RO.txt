Obiectivul acestui proiect este de a diagnostica daca o persoana are sau nu diabet.
Datele au fost preluate de la Institutul National de Diabet, Digestie si Boli de Rinichi din India.
Toti pacientii au fost femei cu varsta minima de 21 de ani.

S-au luat in considerare:
1.Numarul de sarcini
2.Glucoza
3.Presiunea Sangelui
4.Grosimea Pielii
5.Nivelul de insulina
6.Indicele de masa
7.DiabetesPedigreeFunction
8.Varsta

A fost realizat cu ajutorul unei Retele Neurale. Am incercat sa descriu pe scurt majoritatea liniilor din cod, prin comentarii.
Programul a avut o acuratete de 84.515195 pe setul de antrenament si 81.818182 pe cel de test, nefolisit in antrenamente.

mainScript este programul principal, ce antreneaza reteaua si afiseaza rezultatele pe setul de antrenament respectiv pe cel de test.
Daca aveti exemple proprii: 
1.Cu ajutorul WordPad-ului creati un fisier CSV  
2.In Octave incarcati parametrii cu functia load("weights.mat") apoi chemati functia featureNormalize pentru a aduce toate caracteristicile in aceeasi gama de valori, apoi apelati functia predict, pentru a vedea diagnosticul.

Link-ul de unde au fost preluate datele: https://www.kaggle.com/uciml/pima-indians-diabetes-database#diabetes.csv
Link download Octave(software-ul in care a fost realizat proiectul): https://ftp.gnu.org/gnu/octave/windows/
Trebuiesc descarcate toate functiile.