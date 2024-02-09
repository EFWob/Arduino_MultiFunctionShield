# Arduino_MultiFunctionShield UPDATE
##Funktionserweiterungen
Neben den Funktionserweiterungen wurde die Performance verbessert. Mehr dazu folgt unten

Einige der neuen APIs fordern Positionsangaben. Die Segmente sind von 0 bis 3 durchnummeriert,
dabei steht die Position 0 für ganz rechts, dann nach links Position 1, 2 und 3.

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

   

LED Display driver for Multi Function Shield

**Note:** Timer1 used => Pins 9 and 10 on Uno for PWM and analogWrite() are effected

![multi_function_shield](multi_function_shield.png?raw=true)

Fritzing Part: [Download Multi Function Shield](https://www.heise.de/make/downloads/76/2/4/1/3/8/7/4/Multi_Function_Shield.fzpz)

https://www.blafusel.de/index.html
