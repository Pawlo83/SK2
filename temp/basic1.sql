CREATE TABLE sklepy (
    id_sklep NUMBER PRIMARY KEY,
    adres VARCHAR2(50) NOT NULL,
    otwarcie_w_tyg VARCHAR2(10),
    zamkniecie_w_tyg VARCHAR2(10),
    otwarcie_w_sobote VARCHAR2(10),
    zamkniecie_w_sobote VARCHAR2(10)
);

CREATE TABLE alejki (
    id_alejka NUMBER PRIMARY KEY,
    nr_w_sklepie NUMBER,
    nazwa VARCHAR2(50),
    fk_id_sklep NUMBER,
    CONSTRAINT fk_alejka_sklep FOREIGN KEY (fk_id_sklep)
        REFERENCES sklepy (id_sklep),
	CONSTRAINT unique_alejka UNIQUE (fk_id_sklep, nr_w_sklepie)
);

CREATE TABLE produkty (
    nazwa VARCHAR2(50) PRIMARY KEY
);

CREATE TABLE produkty_w_ofercie (
	id_produkt_w_ofercie NUMBER PRIMARY KEY,
	fk_produkt VARCHAR2(50),
	fk_id_sklep NUMBER,
	CONSTRAINT fk_prod_ofert_produkt FOREIGN KEY (fk_produkt)
        REFERENCES produkty (nazwa),
	CONSTRAINT fk_prod_ofert_sklep FOREIGN KEY (fk_id_sklep)
        REFERENCES sklepy (id_sklep),
	CONSTRAINT unique_prod_ofert UNIQUE (fk_produkt, fk_id_sklep)
);

CREATE TABLE produkty_w_asortymencie (
	id_produkt_w_asortymencie NUMBER PRIMARY KEY,
    ilosc NUMBER NOT NULL,
    cena_100g FLOAT(5),
    cena_szt FLOAT(5),
	fk_produkt VARCHAR2(50),
	fk_id_alejka NUMBER,
    CONSTRAINT fk_prod_asort_produkt FOREIGN KEY (fk_produkt)
        REFERENCES produkty (nazwa),
    CONSTRAINT fk_prod_asort_alejka FOREIGN KEY (fk_id_alejka)
        REFERENCES alejki (id_alejka),
	CONSTRAINT unique_prod_asort UNIQUE (fk_produkt, fk_id_alejka)
);

CREATE TABLE promocje (
	id_promocja NUMBER PRIMARY KEY,
    typ VARCHAR2(50) NOT NULL,
    data_rozpoczecia DATE NOT NULL,
    data_zakonczenia DATE NOT NULL,
    procent FLOAT(5),
    fk_id_sklep NUMBER,
	fk_produkt VARCHAR2(50),
	CONSTRAINT fk_promocja_sklep FOREIGN KEY (fk_id_sklep)
        REFERENCES sklepy (id_sklep),
    CONSTRAINT fk_promocja_produkt FOREIGN KEY (fk_produkt)
        REFERENCES produkty (nazwa)
);

CREATE TABLE skladniki (
	id_skladnik NUMBER PRIMARY KEY,
	ilosc NUMBER NOT NULL,
    fk_produkt VARCHAR2(50),
    fk_danie VARCHAR2(50),
	CONSTRAINT fk_skladnik_produkt FOREIGN KEY (fk_produkt)
        REFERENCES produkty (nazwa),
    CONSTRAINT fk_skladnik_danie FOREIGN KEY (fk_danie)
        REFERENCES dania (nazwa),
	CONSTRAINT unique_skladnik UNIQUE (fk_produkt, fk_danie)
);

CREATE TABLE dania (
    nazwa VARCHAR2(50) PRIMARY KEY,
    tresc_przepisu VARCHAR2(500)
);

CREATE TABLE elementy_lodowki (
	id_element_lodowki NUMBER PRIMARY KEY,
	ilosc NUMBER NOT NULL,
    fk_produkt VARCHAR2(50),
    fk_klient VARCHAR2(50),
    CONSTRAINT fk_lodowka_produkt FOREIGN KEY (fk_produkt)
        REFERENCES produkty (nazwa),
	CONSTRAINT fk_lodowka_klient FOREIGN KEY (fk_klient)
        REFERENCES klienci (email),
	CONSTRAINT unique_lodowka UNIQUE (fk_produkt, fk_klient)
);

CREATE TABLE klienci (
    email VARCHAR2(50) PRIMARY KEY,
    imie VARCHAR2(50) NOT NULL,
    nazwisko VARCHAR2(50) NOT NULL,
    adres VARCHAR2(50)
);

CREATE TABLE zamowienia (
    id_zamowienie NUMBER PRIMARY KEY,
    stan VARCHAR2(50) NOT NULL,
    cena_sumaryczna FLOAT(10) NOT NULL,
    data DATE NOT NULL,
    fk_klient VARCHAR2(50),
    CONSTRAINT fk_zamowienie_klient FOREIGN KEY (fk_klient)
        REFERENCES klienci (email)
);

CREATE TABLE elementy_zamowienia (
	id_element_zamowienia NUMBER PRIMARY KEY,
	ilosc NUMBER NOT NULL,
    fk_id_zamowienie NUMBER,
    fk_produkt VARCHAR2(50),
    CONSTRAINT fk_elem_zamow_zamow FOREIGN KEY (fk_id_zamowienie)
        REFERENCES zamowienia (id_zamowienie),
    CONSTRAINT fk_elem_zamow_produkt FOREIGN KEY (fk_produkt)
        REFERENCES produkty (nazwa),
	CONSTRAINT unique_elem_zamow UNIQUE (fk_id_zamowienie, fk_produkt)
);
