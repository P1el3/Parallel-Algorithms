Piele Mihai Teodor - 331 CB - TEMA 1

*Pentru a implementa paradigma Map Reduce, am folosit o structura, in care am inclus:
    -id-ul thread-ului
    -numarul de mapperi
    -numarul de reduceri
    -bariera
    -mutexul
    -coada de fisiere (pe care am folosit-o pentru a parsa dinamic)
    -structura pentru mapperi
    -structura pentru reduceri
    -un vector ce contine un set de puteri

*Intai, am implementat setul de puteri, mai exact, am generat toate puterile perfecte de la 1 pana la
INT_MAX, pentru fiecare exponent in parte (care este de la 2 pana la NR_REDUCERI + 2)
*Am implementat o functie care imi creaza coada in care introduc fisierele de parsat
*in functia de thread, am dat ca argument un pointer la o structura, pe care o dereferentiez
*in aceasta functie, verific daca id-ul e mai mic decar numarul de mapperi; daca da, atunci thread-ul respectiv
va face map, adica va extrage din coada un fisier, pe care il parseaza, si adauga fiecare numar, daca este o putere
perfecta, in structura pentru mapperi, altfel va trece la numarul urmator; de mentionat faptul ca extragerea din coada
reprezinta o zona critica, astfel ca am folosit un mutex, caruia i-am dat lock inainte sa verific daca coada este goala,
si i-am dat unlock fie dupa ce am dat pop din coada, fie daca coada era vida (intre timp, aceasta s-ar fi putut goli);
*dupa ce s-a executat map-ul, am folosit o bariera, pentru a astepta toate thread-urile care au facut map, si se trece la partea
de reduce, unde creez un set in care introduc toate elementele de pe pozitia i(i de la 0 la numar mapperi) si numarMapperReducer
astfel ca, numarul de elemente unice din reduceri va fi reprezentat de dimensiunea setului (caci in set se introduc doar valori unice)
*creez numele fisierului de iesire, printez inauntrul sau dimensiunea setului, si se trece la urmatorul thread
*in main aloc memorie pentru fiecare din campurile pe care le voi pasa ca pointeri structurii din crearea threadurilor,
creez threadurile si apoi le dau join.