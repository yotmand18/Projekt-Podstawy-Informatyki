# Projekt-Podstawy-Informatyki

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

 
