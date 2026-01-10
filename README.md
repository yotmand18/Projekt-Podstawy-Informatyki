# Projekt-Podstawy-Informatyki

_( nie ma tu zbyt wiele )_

## Obecnie zaimplementowane mechaniki:
* lib.h / lib.cpp
  - centralne miejsce dla wszystkich bibliotek (SFML, STD)
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
  - ładuje tekstury odpowiadające przecinikom i platformom, później oddaje je odpowiednim klasom

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
 
#### TODO
- [x] przeciwnicy
- [x] mechaniki walki
- [ ] śmierć gracza, ekran końcowy
- [ ] system punktów
- [ ] powerupy / modyfikatory w levelach
- [ ] menu główne
- [ ] interfejs
- [ ] możliwość zmiany przypisania klawiszy
- [ ] zapisywanie / ładowanie gry
- [ ] projektowanie poziomów
- [ ] dźwięk i muzyka?

 ## Grafika
 TODO:
 * tekstury różnych przeciwników i animacje chodzenia dla nich, ewentualnie też ataku
 * tekstury różnych platform
 * **Tło**
