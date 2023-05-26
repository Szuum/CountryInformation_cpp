# CountryInformation_cpp
Program przyjmujący od użytkownika nazwę kraju i wypisujący podstawowe informacje na temat podanego państwa (takie jak oficjalna nazwa, stolica, waluta, itp.).

Inspiracją do zadania był program przeliczający zadaną ilość złotówek na daną walutę. Link do tego repozytorium: https://github.com/MyNameIsArko/CurrencyScraper

W programie użyto bibliotek zewnętrznych:
- nlohmann/json (obsługa danych typu json)
- cpr (pobranie danych)
- curl (pobranie i zapis flagi do pliku)
- FLTK (wraz z biblioteką do obsługi obrazów, do interfejsu graficznego)

Po uruchomieniu programu pojawi się okno, w którym należy wpisać państwo. Nazwa państwa ma być podana w języku angielskim, z dużej litery oraz ma to być nazwa nieoficjalna (np. "Poland" a nie "Republic of Poland"). W przeciwnym przypadku wyskoczy okienko informujące o błędnych danych. Po naciśnięciu przycisku na ekranie pojawią się podstawowe informacje o wskazanym kraju, a po prawej stronie zostanie wyświetlona jego flaga.
