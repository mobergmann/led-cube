# Dokumentation
Dies ist die Dokumentation zu dem 3D LED Cube.
Hier wird dokumentiert, wie man den Cube zusammenbauen kann, dafür benötigte Assets, wie man das Program kompiliert und auf den Cube spielt, wie man den Editor benutzt und wie man den Cube benutzt.

## Assemble


## Main Program
Das "Main Program" ist das Program, welches auf dem Pi durchgängig läuft um
auf I/O Eingaben durch die Knöppfe zu reagieren und die aktuelle Konfiguration
durchgängig Frame für Frame anzuzeigen.

### Knöpfe funktion
Es gibt verscheidene Knöpfe.
- **Sync Button**: Der Sync Button lädt Dateien von einen eingesteckten USB Stick persistent in den internen Speicher.
  Dabei werden, wenn Dateien, den gleichen Namen haben wie Dateien, die schon auf dem Cube sind überschrieben.
  Die LED neben dem Sync-Button blinkt, wie lange der transfer statt findet (dies sollte nicht allzu lang dauern).
- **Previous-/ Next Button**: Der Previous und Next Button schaltet zwischen den Konfigurationen durch, die er gespeichert hat.
  Previous schaltet eine Konfiguration zurück und Next eine Konfiguration weiter.
- **On/ Off Button**: Schaltet die LEDs aus, aber die Frames werden weiter simuliert.
- **Reset Button**: Der Reset Button ist ein Spezial-Button, dessen funktionalität nur durch das Lange drücken - mindestens 3 Sekunden - des *Sync-Buttons* erhalten werden kann.
  Dann wird die aktuelle Konfiguration aus dem persistent Speicher gelöscht und das nächste Program wird geladen
  Es können nur vom dem Benutzer selbst hochgeladene Konfiguration gelöscht werden, vorinstallierte Programme werden nicht gelöscht.

### Installation
Um die Installation möglicht einfach zu gestalten haben wir ein paar vordefinierte Skripte in dem `cxx/install/` Ordner hinzugefütgt.
1. ein build skript
2. ein install skript

Um die Installation durchzuführen brauchst du einen Pi mit Internetzugang, da du es für das Betriebsystem und die Architektur kompilieren musst. 
Du kannst mit den passenden Tools auch auf den Pi verzichten, aber dies ist relativ aufwending und deswegen wird dies hier nicht genau erklärt. 
Möchtest du dennoch ohne Pi/ Internetzugang versuchen, dann solltest du wissen, dass du für das main Program die `libgpiod2` Abhängigkeit brauchst und zum kompilieren die `libgpiod-dev` Abhängigkeit.

## Editor


## Assets



---------------

In der Reposetory sind in dem `assets/` Ordner die für den Bau des Cubes verwendeten KiCad Dateien enthalten.

Die KiCad Datei für den **PCB** ist in dem `assets/KiCad/` Ordner.

Die für den 3D Druck verwendeten Dateien sind in dem `assets/3d-print/` Ordner.
Speziell ist der **Top-Case** als `top-case.stl` und die **Seitenumrandung** als `walls.stl` gespeichert.


