# Dokumentation
Dies ist die Dokumentation zu dem 3D LED Cube.
Hier wird dokumentiert, wie man den Cube zusammenbaut, dafür benötigte Assets, wie man das Program kompiliert und auf den Cube installiert, wie man den Editor benutzt und wie man den Cube benutzt.

## Assets
Für den Zusammenbau werden ein paar Assets benötigt, die hier zusammengefasst werden sollen.

### PCB
In dem `assets/PCB` Ordner sind die KiCad Dateien für den PCB enthalten. Diesen müsst ihr anfertigen lassen/ bestellen.

### 3D Print
In dem `assets/3D-Print` Ordner sind die STL Dateien, die ihr Drucken müsst.


## Assemble
Siehe die [`assemble.md` Datei](./assemble.md) für mehr infromationen, wie man den Cube zusammenbaut.

## Main Program
Das "Main Program" ist das Program, welches auf dem Pi durchgängig läuft um auf I/O Eingaben durch die Knöppfe zu reagieren und die aktuelle Konfiguration durchgängig Frame für Frame anzuzeigen.

### Knöpfe Funktion
Es gibt verscheidene Knöpfe.
- **Sync Button**: Der Sync Button lädt Dateien von einen eingesteckten USB Stick persistent in den internen Speicher.
  Dabei werden, wenn Dateien, den gleichen Namen haben wie Dateien, die schon auf dem Cube sind überschrieben.
  Die LED neben dem Sync-Button blinkt, wie lange der transfer statt findet (dies sollte nicht allzu lang dauern).
- **Previous-/ Next Button**: Der Previous und Next Button schaltet zwischen den Konfigurationen durch, die er gespeichert hat.
  Previous schaltet eine Konfiguration zurück und Next eine Konfiguration weiter.
- **On/ Off Button**: Schaltet die LEDs aus, aber die Frames werden weiter simuliert.
- **Reset Button**: Der Reset Button ist ein Spezial-Button, dessen funktionalität nur durch das Lange drücken - mindestens 3 Sekunden - des *Sync-Buttons* erhalten werden kann.
  Dann wird die aktuelle Konfiguration aus dem persistent Speicher gelöscht und das nächste Program wird geladen Es können nur vom dem Benutzer selbst hochgeladene Konfiguration gelöscht werden, vorinstallierte Programme werden nicht gelöscht.

### Kompilieren & Installieren
Um die Installation möglicht einfach zu gestalten haben wir ein paar Skripte in dem `cxx/install/` Ordner hinzugefütgt.
1. ein build skript
2. ein install skript

Um die Installation durchzuführen brauchst du einen Pi mit Internetzugang, da du es für das Betriebsystem und die Architektur kompilieren musst.
Du kannst mit den passenden Tools auch auf den Pi verzichten, aber dies ist relativ aufwending und wird hier nicht genauer erklärt.
Möchtest du dennoch *ohne Pi* oder *ohne Internetzugang* versuchen zu kompilieren, dann solltest du wissen, dass du für das *main program* die `libgpiod2` Abhängigkeit brauchst und *zum kompilieren* die `libgpiod-dev` Abhängigkeit.

#### Build
Benutzt du aber den Pi, dann kannst du einfach die Repo (mit den submodules) Klonen und das `cxx/build/build.sh` Skript ausführen.
Stelle sicher, dass du einen USB Stick in den kompilier-Pi eingesteckt hast und bestätige während des Skriptes, ob die Dateien auf den Stick geschrieben werden sollen mit ja.
Kann dein USB Stick nicht gefunden werden, dann kann es daran liegen, dass dieser nicht erreichbar unter dem `/dev/sda1` Pfad ist.
In diesem Fall, kannst du im Skript den Pfad von `/dev/sda1` zu deinen Pfad ändern.

#### Install
Wenn du die Programme von dem Build auf den USB Stick hast spielen lassen, dann kannst du den Stick in deinen LED-Cube-Pi stecken.
Auf dem Stick sollte sich ein `cube/` Ordner finden, in dem sich unter anderem auch eine `install.sh` Datei befindet.
Diese Datei musst du nun ausführen, damit das Program auf dem Pi installiert wird.

Die Installation umfasst auch das eintragen und aktivieren als System Service,  sodass wenn der Pi gestartet wird das Program automaitsch auch startet.


## Editor
Siehe die [`/editor/README.md` Datei](../editor/README.md) für mehr Informationen, wie man den Editor kompiliert und bedient.
