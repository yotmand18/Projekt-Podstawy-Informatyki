# Projekt-Podstawy-Informatyki

_( nie ma tu zbyt wiele )_

## Struktura repozytorium

```
.
├── src
│   └── pliki .cpp i .h
├── levels
│   └── pliki tekstowe zawierające zawartość poziomów
└── textures
    ├── backgrounds
    │   └── tła poszczególnych poziomów
    ├── enemies
    │   └── stylesheets dla przeciwników
    ├── platforms
    │   └── tekstury platform
    └── player
        └── stylesheet dla gracza
```

#### Struktura posiadania bibliotek

```
project.cpp
├── Game.h - Game.cpp
    ├── Input.h
    │   └── lib.h
    ├── Player.h
    │   └── Physics.h
    │       └── lib.h
    └── Level.h
        ├── Enemy.h
        │   └── Physics.h
        │       └── lib.h
        └── Platform.h
            └── lib.h
```

        
## Obecnie zaimplementowane mechaniki
* lib.h / lib.cpp
  - wspólne includy dla całego projektu

* Input.h / Input.cpp
  - przyjmowanie inputu,
  - mapowanie klawiszy do akcji wewnątrz gry

#### Fizyka i Ruch

* Physics.h / Physics.cpp _new_
  - poruszanie się
  - skakanie
  - grawitacja

#### Gracz
 
* Player.h / Player.cpp
  - tekstura i animacje:
      - idle
      - chodzenie
      - bieg
      - atak
  - integracja z klasą fizyki
  - niezależne hitboxy
  - statystyki:
      - ilość życia
      - siła ataku
  - mechaniki walki:
      - cooldown ataku
      - cooldown otrzymywania obrażeń
      - świecenie na czerwono przy obrażeniach
      - świecenie na zielono przy zyskiwaniu życia

#### Przeciwnicy

* Enemy.h / Enemy.cpp _new_
  - różni przeciwnicy, obecnie:
      - drowner (utopiec)
      - nekker
      - ghoul
      - wraith (upiór)
      - griffin (gryf)
  - statystyki:
      - ilość życia
      - siła ataku
      - prędkość poruszania się
      - wielkość przeciwnika
      - lata czy nie
      - jest agresywny ( biegnie za graczem ) czy patrolujący ( chodzi dookoła )
  - tekstura i animacje
  - atakowanie i przyjmowanie obrażeń, cooldown
  - patrol:
      - chodzi między patrolLeft / patrolRight, aż do zbliżenia się gracza jeśli jest agresywny
  - pościg (jeśli agresywny)
      - wykrywanie gracza w określonej dla typu odległości
      - prędkość podczas pościgu zwiększona o 1.5

#### Poziomy i środowisko
 
* Platform.h / Platform.cpp
  - tworzenie platformy na podstawie danych z pliku tekstowego

* Level.h / Level.cpp
  - odczytuje pliki z ./levels/
  - tworzy wektor zawierające platformy
  - tworzy wektor zawierający przeciwników
  - ładuje tekstury odpowiadające przeciwnikom i platformom, później oddaje je odpowiednim klasom

* ./levels/
  - format:

 typ koordynat_x koordynat_y szerokość wysokość nazwa_tekstury

( jeżeli przeciwnik ):
 typ koordynat_x koordynat_y szerokość wysokość nazwa_tekstury koordynat_patrolowania_lewy koordynat_patrolowania_prawy

  - typy : platform, enemy
  - przykład:

platform 200 480 160 120 rock1
platform 600 440 320 160 rock2
enemy 900 440 160 160 drowner 900 1600
enemy 1500 440 160 160 ghoul 1500 2500

#### Główna pętla gry

* Game.h / Game.cpp
  - otwieranie okna, obecnie 800x600, 60FPS
  - tworzenie kamery (view), który porusza się wraz z graczem
  - zapisywanie wydarzeń (events)
  - ładowanie tekstur gracza
  - włączanie poszczególnych mechanik z innych klas na podstawie inputu
  - implementacja walki między graczem a przeciwnikami
  - zamknięcie gry gdy gracz umiera
  - kolizja między graczem, przeciwnikami a platformami
  - updateowanie klas
  - renderowanie tekstur
 
## TODO

#### Gameplay / Techniczne
- [x] przeciwnicy
- [x] mechaniki walki
- [x] zmiana wielkości okna
- [ ] koniec gry:
  - [ ] ekran przegranej - śmierci (restart/menu)
  - [ ] ekran zwycięstwa
  - [ ] statystyki? (czas, zabici wrogowie, punkty)
- [ ] system progresji:
  - [ ] punkty za zabijanie wrogów
  - [ ] system levelu gracza?
  - [ ] ulepszenia? (więcej życia, silniejszy atak)
- [ ] menu główne:
  - [ ] new Game
  - [ ] continue (load save)
  - [ ] options (rebind keys, volume)
  - [x] exit
- [ ] powerupy / modyfikatory w levelach
- [ ] interfejs
  - [ ] ilość punktów życia
  - [ ] ilość zdobytych punktów
  - [ ] guzik menu
- [ ] możliwość zmiany przypisania klawiszy
- [ ] dźwięk i muzyka?
- [ ] zapisywanie / ładowanie gry?

#### Grafika
- [ ] tekstury różnych przeciwników i animacje chodzenia dla nich, ewentualnie też ataku: (przykładowe, przeciwnicy do ustalenia z projektem)
  - [ ] drowner.png
  - [ ] nekker.png
  - [ ] ghoul.png
  - [ ] wraith.png
  - [ ] griffin.png
- [ ] tekstury różnych platform - w zależności od poziomów
- [ ] tła różnych poziomów - w zależności od poziomów
 
#### Projektowanie
- [ ] wymyślenie historii:
  - [ ] główny cel (boss?)
  - [ ] narracja między poziomami?
- [ ] ilość poziomów?
- [ ] czym jest poszczególny poziom - np. las, miasto itd.
- [ ] zaprojektowanie poziomów - ułożenie platform, przeciwników w odpowiednich miejscach
 
## Wersja 0.2
* Dodanie kamery podążającej za graczem
* Edycja struktury plików poziomu w ./levels/
  - obiekt zaczyna się od typu obiektu: "platform" lub "enemy"
  - dla typu "enemy" zapisuje się: koordynaty, szerokość, wysokość, nazwę tekstury / rodzaju przeciwnika, lewą granica strefy patrolowania, prawą granicę strefy patrolowania
* Dodanie klasy Enemy.h
* Edycja Level.h - uwzględnienie przeciwników
* Dodanie klasy Physics.h, przeniesienie obliczania fizyki z Player.h, zarówno przeciwnicy jak i gracz korzystają z fizyki
* Dodanie mechaniki ataku, tracenia i zyskiwania życia
* zmiana nazwy main.cpp na project.cpp

## Wersja 0.2.1
* Przeciwnicy kolidują z platformami, potrafią skakać
* Gra się wyłącza po straceniu życia przez gracza
* Dodanie możliwości zmiany wielkości okna
* Dodanie możliwości wczytania tła
* Dodanie CMakeLists.txt
* Przeniesienie plików źródłowych do ./src
