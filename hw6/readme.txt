comile: gcc -Wall -o assa assa.c -lm

./assa 3 4 a.bmp config.cfg






Das Programm wird mit folgenden Parametern aufgerufen (wobei die Namen, inklusive der Klammern, mit entsprechenden Werten zu ersetzen sind):
./assa [float:angle] [float:speed] [output_filename] {optional:config_filename}
angle: ist eine Gleitkommazahl und bestimmt den Abschusswinkel in Grad, wobei 0° einem geraden Schuss nach rechts entspricht.
speed: bestimmt die Kraft des Abschusses und entspricht in unserem Fall der Distanz in Metern, welche das Projektil in einer Sekunde zurücklegt. Ist durch eine Gleitkommazahl >= 0.0 > 0 gegeben.
output_filename: der Dateiname der fertigen Bitmap-Datei
config_filename {optional!}: ist der Dateiname einer Konfigurationsdatei, welche bestimmte Schlüsselwerte, wie Höhe/Breite des Bildes usw., verändern kann.
Programmaufrufe könnten z.B. so aussehen:
./assa 45.5 1000 image.bmp
./assa 80 10 img.bmp config.cfg
Konfigurationsdatei

Sollte keine Konfigurationsdatei angegeben sein oder die angegebene Datei nicht vorhanden bzw. nicht lesbar sein, wird die Nachricht "no config file found - using default values\n" ausgegeben und alle Werte können auf ihre Vorgabewerte gesetzt werden. Es erfolgen keine weiteren Ausgaben welche die Konfigurationsdatei betreffen.
Die Konfigurationsdatei beinhaltet folgende mögliche Einträge (je ein Eintrag pro Zeile), welche in beliebiger Reihenfolge vorkommen können (Zusätzliche Leerzeichen sollen dabei ignoriert werden). Die Ausgaben haben entsprechend der Reihenfolge des Auftretens in der Datei zu erfolgen, sollte ein Eintrag nicht vorkommen oder ungültig sein, wird er auf den Vorgabewert (dritter Wert in der Klammer) gesetzt. Alles in den eckigen Klammern [ Datentyp : Bedeutung : Vorgabewert ] soll dabei, inklusive der Klammern, durch die entsprechenden Werte ersetzt werden. Sollte ein Eintrag mehrfach vorkommen, werden alle Wiederholungen als fehlerhafte Einträge angesehen.
resolution [unsigned int:width:320] [unsigned int:height:320]
Setzt die Breite und Höhe des Bildes auf den entsprechenden Wert. Der Eintrag ist nur gültig, wenn Beide Werte vorhanden sind. Bei Erfolg wird "resolution set to [width]:[height]\n" ausgegeben.
pps [unsigned int:pps:5]
Punkte pro Sekunde (pps) wird in diesem Fall verwendet um anzugeben, wieviele Punkte für eine Sekunde Flugzeit berechnet werden. Verbindet man später diese Punkte mit geraden Linien, ergibt sich eine Annäherung an die tatsächliche Flugbahn (mehr pps -> 'rundere' Flugbahn). Ist nur gültig, wenn der Wert > 0 ist und gibt bei Erfolg "pps set to [pps]\n" aus.
gravitation [float:gravitation:9.798]
Bestimmt die Erdbeschleunigung, welche das Projektil pro Sekunde um diesen Wert richtung Boden beschleunigt. Bei Erfolg wird "gravitation set to [gravitation]m/s^2\n" ausgegeben. (2 Kommastellen)
wind [float:angle:0] [float:force:0]
Bestimmt einen Windvektor, welcher das Projektil pro sekunde mit der Angegebenen Kraft in die entsprechende Richtung beschleunigt. Sind beide Werte vorhanden wird "wind set to [angle] degrees with force [force]m/s^2\n" (2 Kommastellen)
Sollte zumindest ein Eintrag gefehlt haben oder fehlerhaft sein, wird am Schluss noch zusätzlich "[x] missing or incorrect entrie(s) - using default values\n" ausgegeben, wobei x der Anzahl der fehlenden oder fehlerhaften Einträgen entspricht.
Anmerkung: Die Vorgabewerte sind sehr klein gewählt um beim Testen am Universitätsserver keine zu großen Dateien zu erzeugen.
Mögliche Konfigurationsdatei:
wind 33.333 50
resolution 800 600
pps 2
Ausgabe:
wind set to 33.33 degrees with force 50.00m/s^2
resolution set to 800:600
pps set to 2
1 missing or incorrect entrie(s) - using default values
Berechnungen

Die Berechnungen in diesem Beispiel sind, da es um das Programmieren und nicht um die Physik dahinter geht, sehr vereinfacht, so gibt es z.B keine Masse, keinen Luftwiederstand usw., ausserdem wird als Maßstab 10 Meter : 1 Bildpunkt gewählt. Das Projektil startet in der Mitte des Bildschirms (immer abgerundet) und hat zu beginn einen Geschwindigkeitsvektor (v) welcher durch [angle] und [speed] bestimmt ist. Dabei gibt [angle] die Richtung des Vektors und [speed] die Länge des Vektors an. In jeder Zeiteinheit (t), wirkt die Erdanziehung (g) und der Wind (w) auf das Projektil (die vergangene Zeit lässt sich leicht mit den [pps] berechnen). Die daraus resultierende Formel lautet also:
position = position_start + v * t + g * t²/2 + w * t²/2
Verlässt das Projektil dabei den Bildschirm wird das Bild geschrieben und das Programm beendet.
Bitmap-Datei

Als graphische Ausgabe reicht ein gefüllter Kreis als Ursprung des Projektils und eine einfache Linie für die Flugbahn, welche dadurch entsteht, dass die berechneten Punkte mit geraden Linien verbunden werden. Dabei sind Linienstärke, Radius und Farbe frei wählbar.
Bmp-Format: http://www.digicamsoft.com/bmp/bmp.html

