***Komunikator sieciowy typu skype****
**Temat projektu:**
Projekt polega na implementacji serwera obsługującego komunikację między klientami poprzez protokół TCP. Serwer umożliwia klientom łączenie się, wyświetlanie listy dostępnych użytkowników oraz zestawianie połączeń do bezpośredniej komunikacji.

**Opis protokołu komunikacyjnego:**
Protokół komunikacyjny opiera się na przesyłaniu prostych wiadomości tekstowych między klientami a serwerem. Komendy obsługiwane przez serwer to:
- `refresh` – wysyła listę dostępnych klientów,
- `connect <IP>` – próba połączenia z podanym klientem,
- `ask` – wiadomość wysyłana do użytkownika z zapytaniem o zgodę na połączenie,
- `ask yes` – akceptacja połączenia przez drugiego klienta,
- `ask rejected` – odmowa połączenia, 
- `stop` – zakończenie komunikacji.

Serwer zarządza połączeniami i kontroluje dostępność klientów, zapewniając mechanizm blokowania użytkowników podczas aktywnej komunikacji.

**Opis implementacji:**
Projekt składa się z jednego pliku źródłowego `server.c`, który zawiera całą logikę serwera. 

Serwer wykorzystuje:
- **Gniazda TCP** do obsługi połączeń,
- **Wątki Pthreads** do obsługi wielu klientów jednocześnie,
- **Mutexy** do synchronizacji dostępu do współdzielonych zasobów (lista klientów, blokowanie sesji),
- **Funkcję `handle_client`**, która obsługuje pojedynczego klienta, odbiera od niego komendy i wykonuje odpowiednie operacje,
- **Funkcję `handle_connection`**, odpowiadającą za wymianę wiadomości między dwoma połączonymi klientami.

Serwer przechowuje klientów w tablicy `clients[MAX_CLIENTS]`, gdzie dla każdego klienta zapisany jest deskryptor gniazda, adres IP oraz status (czy jest zajęty). Po odebraniu komendy `connect`, serwer sprawdza, czy docelowy klient jest dostępny i wysyła do niego zapytanie o zgodę. Jeśli klient zaakceptuje, tworzona jest para wątków umożliwiających bezpośrednią komunikację.

**Sposób kompilacji i uruchomienia:**
1. **Kompilacja serwera:**
   ```sh
   gcc -o server server.c -pthread
   ```
2. **Uruchomienie serwera:**
   ```sh
   ./server
   ```
   Serwer domyślnie nasłuchuje na porcie `12347`.
3. **Obsługa klientów:**
   Klienci mogą łączyć się z serwerem i wysyłać komendy zgodnie z opisanym protokołem. Po zestawieniu połączenia mogą wymieniać wiadomości w czasie rzeczywistym.

**Pliki dodatkowe:**
- **Plik klienta (`.exe`)** – aplikacja kliencka uruchamiana przez użytkownika,
- **Plik `conf.txt`** – plik konfiguracyjny zawierający ustawienia IP hosta, portu oraz indeksu kamery.

W razie potrzeby można modyfikować kod, aby dostosować serwer do własnych wymagań.
