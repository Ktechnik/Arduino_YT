Cześć, znajdziedz tutaj kilka informacji przydatnych do wgrania programu do ESP. Ten projetk zreolizowany jest na platformie Supla. 
Większość projektów na ESP i Supli polega na wgrywaniu do ESP innego firmware. Ta wersja oparta jest na programie napisanym
w Arduino IDE. Jest to lepsze, ponieważ to rozwiązanie jest łatwiejsze i szybsze w uzyciu oraz pozwala nam na łatwą modyfikację kodu. 

Zmiany w kodzie:
1. Nazwa i Hasło do WiFi (linie 33 i 34)
2. Numer GUIG (linia 40) jest on do wygenerowania tutaj: https://www.supla.org/arduino/get-guid 
3. Adres MAC naszego urządzenia (linia 44)
4. Jeśli chcesz obrócić sygnał (stan wysoki będzie brany jako niski, a niski jako wysoki) to możesz to zmienić (linia 45),
najprościej jest sprawdzić obydwie wersje
5. Na końcu trzeba uzupełnić adres serwera, identyfikator lokalizacji oraz Hasło. Te dane znajdują się w Supli na waszym koncie.


Ktechnik
