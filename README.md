# REST-API-Comunication
REST API este baza pe HTTP , au fost de implementat comenzi pentru Register , Log-in , Log-out , adaugarea unui element intr-o lista , stergerea unui element .



Salut , 

Implementarea mea a avut la baza functiile auxiliare din laboratorul 10 cat si headerele din functia client.c .

In fisierul sursa "buffer.c" am mai adauga o functie auxiliara care lipea 2 stringuri (ajutor pentru formarea stringului de tip json)

In fisierul sursa "requests,c" am mai adauga 3 functii auxiliare pe langa cele 2 oferite in laborator pentru 
prelucrarea stringului "message" dupa forma dorita de fiecare exercitiu in parte. Unele aveau nevoie de Cookie , 
acolo erau de ajuns functiile din laborator , dar unde a fost nevoie de jwt a trebuit sa implementez alta functie. 
Cat si in cazul functiei "delete" este schimbat foarte putin in ea , dar am avut nevoie de ea in acel format.

Prelucrarea pe text am facut-o cu ajutorul functiei strtok , folosita intensiv. Motivul pentru care am ales-o este 
ca stiam cum functioneaza in mare si incepand tema aseara , am vrut sa ma asigur ca o termin pana astazi.
La inceput m-am chinuit putin pana sa obtin stringurile exact cum trebuie , dar in cele din urma mi-a iesit cum trebuie.

Principiul de functionare
Am ciclat un while , unde am afisat un fel de prompt(sa fie intuitiv) , si in acel while citesc un string de la tastatura . 
In cazul in care este una dintre comenzile temei : register, login ... etc. , sa se prelucreze inputul , sa fie trimis 
catre server apoi prelucrarea raspunsului pentru a afisa daca este OK sau o eroare specifica in caz de eroare.
In acelasi timp cu prelucrarea outputului se retine dupa caz Cookie'ul sau jwt'ul(Tokenul). In caz ca nu este introdusa 
nici una dintre comenzile din tema , am afisat un mesaj de genul : " Tasteaza "help" pentru mai multe informatii" , si am 
implementat si o comanda help pentru a da legenda de comenzi cu ce face fiecare , chiar daca acestea aveau deja un nume sugestiv.

Tema din punctul meu de vedere a fost de o dificultate rezonabila ,  din aceasta tema am inteles mai bine cum functioneaza 
principiile http , dar nu numai . Si prelucrarea cu ajutorul strtok'ului a fost ok.


Pentru putin mai multe detalii am comentarii in cod unde fac ceva pentru prima data.

Multumesc mult , 

Robert Mistodinis
