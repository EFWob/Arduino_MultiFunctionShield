# Arduino_MultiFunctionShield UPDATE

## Zusammenfassung

  - Verbesserung der Performance. In der vorherigen Version dauerte die Interruptroutine zur Ansteuerung des Displays ca. 230 Mikrosekunden und verbrauchte damit ca. 5,75 Prozent der CPU-Rechenzeit. Durch dieses Update wurde die Interruptroutine auf ca. 25 Mikrosekunden verkürzt und somit die CPU-Last dafür auf nur noch 0.62 Prozent gesenkt (Gemessen an Arduino-UNO mit 16 MHz). 
  - Die Dezimalpunkte können jetzt gezielt angesteuert werden.
  - Neben Ziffern können jetzt auch die Buchstaben von a-z angezeigt werden (so gut das eben mit einer 7-Segmentanzeige geht)
  - Damit lassen sich jetzt auch Texte (max. 4 Zeichen) oder Hexadezimalzahlen darstellen.
  - Zusätzlich gibt es jetzt eine Methode, mit der LEDs der Segmente frei gesetzt werden können.
  - Ein neues Beispiel **buttons.ino** zeigt den aktuellen Status der 3 Taster des Shields als Piktogramm auf dem Display an.

## Funktionserweiterungen

Einige der neuen APIs fordern Positionsangaben. Die Segmente sind von 0 bis 3 durchnummeriert,
dabei steht die Position 0 für ganz rechts, dann nach links Position 1, 2 und 3.

### Dezimalpunkte setzen und abfragen
Aus dem Sketch heraus können jetzt auch die Dezimalpunkte gezielt angesteuert werden. Dazu gibt es mehrere Möglichkeiten:
1. Die Methode *DisplayDot(uint8_t pos, bool show=true)*

   Damit kann ein Punkt gezielt angesteuert werden. Der Parameter **pos** bestimmt die Position des Punktes (0..3). Der Parameter **show** bestimmt, ob der Punkt angezeigt werden soll (falls auf _true_ gesetzt) oder ausgeschaltet werden soll (falls *false*).  
   _MFS.DisplayDot(3, true);_ schaltet den ganz linken Dezimalpunkt ein. Die verkürzte Schreibweise wäre _MFS.setDot(3);_ und bewirkt genau das Gleiche.  
   _MFS.DisplayDot(0, false);_ schaltet den Dezimalpunkt ganz rechts aus.
   
2. Die Methode *DisplayDots(uint8_t dotMask)*

   Mit dieser Methode können mehrere Punkte auf einmal gesetzt werden. Dazu sollten folgende vordefinierte Konstanten für den Parameter **dotMask** verwendet werden:
  * **DOT_NONE** schaltet alle Punkte aus (Beispiel: *MFS.DisplayDots(DOT_NONE);*)
  * **DOT_ALL** schaltet alle Punkte ein (Beispiel: *MFS.DisplayDots(DOT_ALL);*)
  * **DOT_POS0**, **DOT_POS1**, **DOT_POS2**, **DOT_POS3** schaltet den jeweiligen Punkt ein und alle anderen aus (Beispiel: *MFS.DisplayDots(DOT_POS1);* schaltet den zweiten Punkt von rechts ein und alle anderen aus). Die **DOT_POSx**-Werte können durch bitweises ODER verknüpft werden, um eine beliebige Kombination von Punkten gleichzeitig einzuschalten (und alle anderen aus). Als Beispiel schaltet *MFS.DisplayDots(DOT\_POS0 | DOT\_POS3);* den rechten und linken Punkt ein und die beiden dazwischen aus.

Mit der Methode *getDot(uint8_t pos)* kann der Status eines Punkts (an Position 0..3) einzeln abgefragt werden.
  - ist der Punkt nicht gesetzt, wird **0** zurückgegeben.
  - ist der Punkt gesetzt, wird **128** zurückgegeben.

Die ursprüngliche Methode *Display(int16_t value, uint8_t dotMode = DOT_OFF);* hat einen zweiten, optionalen Parameter **dotMode**, der die Darstellung der Punkte reguliert. Mögliche Werte sind
  - **DOT_OFF** alle Punkte werden gelöscht (das ist auch die Standardeinstellung, falls der Parameter nicht angegeben wird)
  - **DOT_ON** alle Punkte werden eingeschaltet
  - **DOT_KEEP** alle Punkte bleiben so, wie sie vor Aufruf der *Display()*-Methode eingestellt waren
  - **DOT_POSx** mit Veroderung wie oben bei *DisplayDots()* beschrieben erlaubt das wahlfreie Setzen einzelner Punkte (und das Löschen aller anderen).

### Erweiterte Funktionen zur Darstellung

1. Mit der Methode *DisplayChar(uint8\_t pos, char c, uint8\_t dotMode=DOT\_OFF);* erlaubt die Ausgabe des Zeichens im zweiten Parameter **char c** an die Position, die durch den ersten Parameter **uint8_t pos** festgelegt wird. Position wie üblich 0..3, mögliche Zeichen sind die Buchstaben 'a' bis 'z' (Gross-/Kleinschreibung ist egal), die Ziffern '0'..'9', das Minus-Zeichen '-' und der Unterstrich '_'. Alle anderen Zeichen (auch das Leerzeichen ' ') werden als blank ausgegeben.
Der optionale Parameter **dotMode** kann auf **DOT_ON**, **DOT_OFF** oder **DOT_KEEP** gesetzt werden. Voreinstellung (falls der Parameter nicht angegeben wird) ist **DOT_OFF** (Punkt aus).

2. Die Methode *DisplayText(const char* s, uint8\_t dotMode=DOT\_OFF);* erlaubt die Ausgabe des Texts, der als erster Parameter **const char *s** übergeben wurde. Ist der Text länger als 4 Zeichen, werden nur die ersten 4 Zeichen angezeigt. Ist sie kürzer, werden links Leerzeichen eingefügt. Der optionale Parameter **dotMode** steuert die Ausgabe von Dezimalpunkten wie oben bei der Methode *Display()** beschrieben.

3. Die Methode *DisplayHexValue(uint16_t value, uint8_t dotMode = DOT\_OFF);* gibt den Wert des ersten Parameters **uint16_t value** als Hexadezimalzahl aus. Die Ausgabe erfolgt rechtsbündig, gegebenenfalls wird links mit Leerzeichen aufgefüllt. Der optionale Parameter **dotMode** steuert die Ausgabe von Dezimalpunkten wie oben bei der Methode *Display()** beschrieben.

4. Die Methode *DisplayHexValue0(uint16_t value, uint8_t dotMode = DOT\_OFF);* ist identisch mit *DisplayHexValue()*, außer dass erforderlichenfalls mit '0' statt mit Leerzeichen aufgefüllt wird.

5. Die Methode *DisplayBitmap (uint8_t pos, uint8_t bmap);* erlaubt, beliebige Muster auf der 7-Segment-Anzeige an Position **uint8_t pos** anzuzeigen. Die Segmente 'a' bis 'g' sind jeweils den Bits b0..b6 im Parameter **uint8_t bmap** zugeordnet, Bit b7 dem Dezimalpunkt. Beispiele für eine Bitmap:
  - **0** alles aus
  - **1** der obere Strich leuchtet
  - **6** eine Eins (Strich rechts) wird angezeigt
  - **7** eine Sieben wird angezeigt (witzig)
  - **127** alle 7 Segmente leuchten, der Dezimalpunkt ist aus
  - **128** alle 7 Segmente sind aus, der Dezimalpunkt leuchtet
  - **255** alle 7 Segmente und der Dezimalpunkt sind an


