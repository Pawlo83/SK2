# Komunikator sieciowy typu skype
### Temat projektu:
Projekt polega na implementacji serwera obsługującego komunikację między klientami poprzez protokół TCP. Serwer umożliwia klientom łączenie się, wyświetlanie listy dostępnych użytkowników oraz wywołanie połączeń do bezpośredniej komunikacji.

### Opis protokołu komunikacyjnego:
Protokół komunikacyjny opiera się na przesyłaniu prostych wiadomości tekstowych między klientami a serwerem. Komendy obsługiwane przez serwer to:
- `refresh` – wysyła listę dostępnych klientów,
- `connect <IP>` – próba połączenia z podanym klientem,
- `ask <IP>` – wiadomość wysyłana do użytkownika z zapytaniem o zgodę na połączenie,
- `ask yes` – akceptacja połączenia przez drugiego klienta,
- `ask no` – odmowa połączenia, 
- `stop` – zakończenie rozmowy wideo,
- `start` – rozpoczęcie rozmowy wideo.

Ewentualne części komunikatów i zakończenie przesyłania są oddzielone znakiem nowej lini.
Serwer zarządza połączeniami i kontroluje dostępność klientów, zapewniając mechanizm blokowania użytkowników podczas aktywnej komunikacji.

### Opis implementacji:
Serwer przechowuje klientów w tablicy `clients`, gdzie dla każdego klienta zapisany jest deskryptor gniazda, adres IP oraz status (czy klient jest już w rozmowie). Po odebraniu komendy `connect`, serwer sprawdza, czy docelowy klient jest dostępny i wysyła do niego zapytanie o zgodę. Jeśli klient zaakceptuje, tworzona jest para wątków umożliwiających bezpośrednią komunikację.
Serwer wykorzystuje:
- **Gniazda TCP** do obsługi połączeń,
- **Wątki Pthreads** do obsługi wielu klientów jednocześnie,
- **Mutexy** do synchronizacji dostępu do współdzielonych zasobów takich jak lista klientów,
- **Funkcję `handle_client`**, która obsługuje pojedynczego klienta, odbiera od niego komendy i wykonuje odpowiednie operacje,
- **Funkcję `handle_connection`**, odpowiadającą za wymianę danych między dwoma połączonymi klientami w rozmowie wideo.



Klient wykorzystuje:
- **Gniazda TCP** do połączenia z serwerem,
- **Wątki** do jednoczesnej obsługi GUI i płynnego przesyłu danych w obie strony,
- **Interfejs graficzny** do komunikacji z użytkownikiem, utworzony przy pomocy biblioteki opencv,
- **Obsługę wideo** przy użyciu biblioteki opencv,
- **Obsługę audio** przy użyciu biblioteki pyaudio,
- **Eventy i kolejkę** do obsługi komunikacji między wątkami,
- **Funkcję `receive`**, do odbioru danych audio i wideo od serwera,
- **Funkcję `send_video` oraz `send_audio`**, do wysyłania audio i wideo do serwera.

### Sposób kompilacji i uruchomienia:
1. **Kompilacja serwera:**
   ```sh
   gcc -Wall serwer.c -o serwer
   ```
2. **Uruchomienie serwera:**
   ```sh
   ./serwer
   ```
   Serwer domyślnie nasłuchuje na porcie `12345`.
3. **Konfiguracja klienta (edycja pliku conf.txt, w którym zawiera się IP serwera, port oraz indeksu kamery) np.:**
   ```
   192.168.0.1
   12345
   0
   ```
4. **Uruchomienie klienta:**
   ```
   klient.exe
   ```
5. **Obsługa klientów:**
   Klienci mogą łączyć się z serwerem i wysyłać komendy zgodnie z opisanym protokołem. Po zestawieniu połączenia mogą wymieniać wiadomości w czasie rzeczywistym.
