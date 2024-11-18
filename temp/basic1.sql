CREATE TABLE sklep (
    id_sklep NUMBER PRIMARY KEY,
    adres VARCHAR2(50) NOT NULL,
    otwarcie_w_tyg VARCHAR2(10),
    zamkniecie_w_tyg VARCHAR2(10),
    otwarcie_w_sobote VARCHAR2(10),
    zamkniecie_w_sobote VARCHAR2(10)
);

CREATE TABLE alejka (
    id_alejka NUMBER PRIMARY KEY,
    nr_w_sklepie NUMBER,
    nazwa VARCHAR2(50),
    fk_id_sklep NUMBER NOT NULL,
    CONSTRAINT fk_alejka_sklep FOREIGN KEY (fk_id_sklep)
        REFERENCES sklep (id_sklep)
);

CREATE TABLE produkt (
    nazwa VARCHAR2(50) PRIMARY KEY
);

CREATE TABLE produkt_w_asortymencie (
	id_produkt_w_asortymencie NUMBER PRIMARY KEY,
    ilosc NUMBER NOT NULL,
    cena_100g FLOAT(5),
    cena_szt FLOAT(5),
	fk_produkt VARCHAR2(50) NOT NULL,
	fk_id_alejka NUMBER NOT NULL,
    CONSTRAINT fk_prod_asort_produkt FOREIGN KEY (fk_produkt)
        REFERENCES produkt (nazwa),
    CONSTRAINT fk_prod_asort_alejka FOREIGN KEY (fk_id_alejka)
        REFERENCES alejka (id_alejka)
);

CREATE TABLE promocja (
	id_promocja NUMBER PRIMARY KEY,
    typ VARCHAR2(50) NOT NULL,
    data_rozpoczecia DATE NOT NULL,
    data_zakonczenia DATE NOT NULL,
    procent FLOAT(5),
    fk_id_sklep NUMBER NOT NULL,
	fk_produkt VARCHAR2(50) NOT NULL,
	CONSTRAINT fk_promocja_sklep FOREIGN KEY (fk_id_sklep)
        REFERENCES sklep (id_sklep),
    CONSTRAINT fk_promocja_produkt FOREIGN KEY (fk_produkt)
        REFERENCES produkt (nazwa)
);

CREATE TABLE skladnik (
	id_skladnik NUMBER PRIMARY KEY,
	ilosc NUMBER NOT NULL,
    fk_produkt VARCHAR2(50) NOT NULL,
    fk_danie VARCHAR2(50) NOT NULL,
	CONSTRAINT fk_skladnik_produkt FOREIGN KEY (fk_produkt)
        REFERENCES produkt (nazwa),
    CONSTRAINT fk_skladnik_danie FOREIGN KEY (fk_danie)
        REFERENCES danie (nazwa)
);

CREATE TABLE danie (
    nazwa VARCHAR2(50) PRIMARY KEY,
    tresc_przepisu VARCHAR2(500)
);

CREATE TABLE element_lodowki (
	id_element_lodowki NUMBER PRIMARY KEY,
	ilosc NUMBER NOT NULL,
    fk_produkt VARCHAR2(50) NOT NULL,
    fk_klient VARCHAR2(50) NOT NULL,
    CONSTRAINT fk_lodowka_produkt FOREIGN KEY (fk_produkt)
        REFERENCES produkt (nazwa),
	CONSTRAINT fk_lodowka_klient FOREIGN KEY (fk_klient)
        REFERENCES klient (email)
);

CREATE TABLE klient (
    email VARCHAR2(50) PRIMARY KEY,
    imie VARCHAR2(50) NOT NULL,
    nazwisko VARCHAR2(50) NOT NULL,
    adres VARCHAR2(50)
);

CREATE TABLE zamowienie (
    id_zamowienie NUMBER PRIMARY KEY,
    stan VARCHAR2(50) NOT NULL,
    cena_sumaryczna FLOAT(10) NOT NULL,
    data DATE NOT NULL,
    fk_klient VARCHAR2(50) NOT NULL,
    CONSTRAINT fk_zamowienie_klient FOREIGN KEY (fk_klient)
        REFERENCES klient (email)
);

CREATE TABLE element_zamowienia (
	id_element_zamowienia NUMBER PRIMARY KEY,
	ilosc NUMBER NOT NULL,
    fk_id_zamowienie NUMBER NOT NULL,
    fk_produkt VARCHAR2(50) NOT NULL,
    CONSTRAINT fk_elem_zamow_zamow FOREIGN KEY (fk_id_zamowienie)
        REFERENCES zamowienie (id_zamowienie),
    CONSTRAINT fk_elem_zamow_produkt FOREIGN KEY (fk_produkt)
        REFERENCES produkt (nazwa)
);
