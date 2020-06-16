/**
	@file main.cpp
	
	@brief Test dei metodi fondamentali e dei metodi d'uso della classe queue

    @description
    File sorgente con la funzione main().
    Contiene la dichiarazione e la definizione di vari tipi custom e funzioni
    di test per testare i metodi fondamentali e i metodi d'uso della classe
    queue, le funzioni e gli operatori globali e le eccezioni custom ad essa
    associati.
*/

// Direttive per il pre-compilatore

#include <iostream> // std::cout, std::endl
#include <cassert> // assert
#include <string> // std::string
#include <vector> // std::vector
#include <list> // std::list
#include "./headers/queue.h" // queue, transformif,
                             // operatore di stream << per la classe queue,
                             // empty_queue_exception,
                             // queue_iterator_out_of_bounds_exception

/**
	@brief Funtore per l'uguaglianza tra interi
*/
struct equal_int {
    bool operator()(int i1, int i2) const {
        return i1 == i2;
    }
};

/**
	@brief Predicato su interi che determina se un intero è pari

    @param i intero di cui determinare la parità

	@return true se l'intero è pari
    @return false altrimenti
*/
bool even_int(int i) {
    return i % 2;
}

/**
	@brief Operatore su interi che raddoppia il valore di un intero
	
	@param i intero da raddoppiare

	@return intero raddoppiato
*/
int double_int(int i) {
    return 2 * i;
}

/**
	@brief Funtore per l'uguaglianza tra float
*/
struct equal_float {
    bool operator()(float f1, float f2) const {
        return f1 == f2;
    }
};

/**
	@brief Predicato su float che determina se un numero reale è minore
           di un altro
*/
struct less_than_float {

    float value; ///< @brief Valore reale da confrontare

    /**
		@brief Costruttore

        @description
		Costruttore che istanzia il predicato, inizializzandolo con il valore
        reale da confrontare.
	*/
    less_than_float(float v) : value(v) {} // initialization list

    /**
	    @brief Operatore su float che determina se un numero reale è minore
               di un altro

        @param other numero reale di cui determinare se è minore del valore dato

	    @return true se il numero reale è minore del valore dato
        @return false altrimenti
    */
    bool operator()(const float other) const {
        return other < value;
    }
};

/**
	@brief Operatore su float che somma due numeri reali
*/
struct add_float {

    float value; ///< @brief Valore reale da sommare

    /**
		@brief Costruttore

        @description
		Costruttore che istanzia l'operatore, inizializzandolo con il valore
        reale da sommare.
	*/
    add_float(float v) : value(v) {} // initialization list

    /**
	    @brief Operatore su float che somma due numeri reali

	    @param other addendo reale a cui sommare il valore dato

	    @return numero reale somma dell'addendo e del valore dato
    */
    float operator()(const float other) const {
        return other + value;
    }
};

/**
	@brief Funtore per l'uguaglianza tra booleani
*/
struct equal_bool {
    bool operator()(bool b1, bool b2) const {
        return b1 == b2;
    }
};

/**
	@brief Predicato su booleani che determina se un booleano ha valore true

    @param b booleano di cui determinare se ha valore true

	@return true se il booleano ha valore true
    @return false altrimenti
*/
bool true_bool(bool b) {
    return b;
}

/**
	@brief Operatore su booleani che nega un booleano

	@param b booleano da negare

	@return booleano negato
*/
bool negate_bool(bool b) {
    return !b;
}

/**
	@brief Funtore per l'uguaglianza tra stringhe
*/
struct equal_string {
    bool operator()(const std::string &s1, const std::string &s2) const {
        return s1.compare(s2) == 0;
    }
};

/**
	@brief Predicato su stringhe che determina se una stringa inizia con
           un certo carattere
*/
struct first_char_string {

    float character; ///< @brief Carattere da confrontare con il primo
                     ///< della stringa

    /**
		@brief Costruttore

        @description
		Costruttore che istanzia il predicato, inizializzandolo con il carattere
        da confrontare con il primo della stringa.
	*/
    first_char_string(char c) : character(c) {} // initialization list

    /**
	    @brief Operatore su stringhe che determina se una stringa inizia con
               un certo carattere

        @param s stringa di cui determinare se inizia con un certo carattere

	    @return true se la stringa inizia con il carattere dato
        @return false altrimenti
    */
    bool operator()(const std::string &s) const {
        return s.front() == character;
    }
};

/**
	@brief Operatore su stringhe che rimuove il primo carattere di una stringa

	@param b booleano da negare

	@return booleano negato
*/
std::string remove_first_char_string(const std::string &s) {
    return s.substr(1);
}

/**
	@brief Struttura che implementa un numero complesso
*/
struct complex {
	int real; ///< @brief Parte reale del numero complesso
	int imaginary; ///< @brief Parte immaginaria del numero complesso

	/**
		@brief Costruttore

	    @description
		Costruttore che istanzia un numero complesso, inizializzandolo con
	    una parte reale e una immaginaria.

	    @param re parte reale del numero complesso
	    @param im parte immaginaria del numero complesso
	*/
	complex(int re, int im) :
	    real(re), imaginary(im) {} // initialization list
};

/**
	@brief Ridefinizione dell'operatore di stream << per un numero complesso

    @description
	Necessaria per l'operatore di stream << della classe queue.

    @param os oggetto di stream di output
	@param c numero complesso da stampare

	@return riferimento allo stream di output
*/
std::ostream &operator<<(std::ostream &os, const complex &c) {
    std::cout << "(" << c.real << ", " << c.imaginary << ")";
    return os;
}

/**
	@brief Funtore di uguaglianza di due numeri complessi
*/
struct equal_complex {
    bool operator()(const complex &c1, const complex &c2) const {
        return (c1.real == c2.real) && (c1.imaginary == c2.imaginary);
    }
};

/**
	@brief Predicato su numeri complessi che determina se la parte reale è
           uguale alla parte immaginaria

    @param c numero complesso di cui determinare se la parte reale è uguale
           alla parte immaginaria

	@return true se la parte reale del numero complesso è uguale alla parte
            immaginaria
    @return false altrimenti
*/
bool equal_real_imaginary_complex(const complex &c) {
    return c.real == c.imaginary;
}

/**
	@brief Operatore su numeri complessi che rende nulla la parte immaginaria

	@param c numero complesso di cui rendere nulla la parte immaginaria

	@return numero complesso con la parte immaginaria nulla
*/
complex null_imaginary_complex(const complex &c) {
    return complex(c.real, 0);
}

/**
	@brief Struttura che implementa un impiegato
*/
struct employee {
	std::string name; ///< @brief Nome dell'impiegato
	std::string surname; ///< @brief Cognome dell'impiegato
	unsigned int salary; ///< @brief Salario dell'impiegato

	/**
		@brief Costruttore

	    @description
		Costruttore che istanzia un impiegato, inizializzandolo con un nome,
	    un cognome e un salario.
	*/
	employee(std::string n, std::string sur, unsigned int sal) :
		name(n), surname(sur), salary(sal) {} // initialization list
};

/**
	@brief Ridefinizione dell'operatore di stream << per un impiegato

    @description
	Necessaria per l'operatore di stream << della classe queue.

    @param os oggetto di stream di output
	@param e impiegato da stampare

	@return riferimento allo stream di output
*/
std::ostream &operator<<(std::ostream &os, const employee &e) {
    std::cout << "{" << e.name << " " << e.surname << ": " << e.salary << "}";
    return os;
}

/**
	@brief Funtore di uguaglianza di due impiegati

    @description
	Due impiegati sono definiti "uguali" se hanno lo stesso nome e lo stesso
    cognome.
*/
struct equal_employee {
    bool operator()(const employee &e1, const employee &e2) const {
        return (e1.name.compare(e2.name) == 0 &&
                e1.surname.compare(e2.surname) == 0);
    }
};

/**
	@brief Predicato su impiegati che determina se il salario è maggiore
           o uguale a una certa quota
*/
struct salary_greater_or_equal_than_employee {

    unsigned int quote; ///< @brief Quota da confrontare con il salario
                        ///< dell'impiegato

    /**
		@brief Costruttore

        @description
		Costruttore che istanzia il predicato, inizializzandolo con la quota
        da confrontare con il salario dell'impiegato.
	*/
    salary_greater_or_equal_than_employee(unsigned int q) :
        quote(q) {} // initialization list

    /**
	    @brief Operatore su impiegati che determina se il salario è maggiore
               o uguale a una certa quota

        @param e impiegato di cui determinare se il salario è maggiore o uguale
               a una certa quota

	    @return true se il salario è maggiore o uguale alla quota data
        @return false altrimenti
    */
    bool operator()(const employee &e) const {
        return e.salary >= quote;
    }
};

/**
	@brief Operatore su impiegati che ne dimezza il salario

    @note L'eventuale arrotondamento è effettuato per difetto.

	@param e impiegato di cui dimezzare il salario

	@return impiegato con il salario dimezzato
*/
employee half_salary_employee(const employee &e) {
    return employee(e.name, e.surname, e.salary / 2);
}

/**
	@brief Definizione di un tipo di dato per code di interi
*/
typedef queue<int, equal_int> queue_int;

/**
	@brief Funtore di uguaglianza di due code di interi

    @description
	Due code di interi sono definite "uguali" se tutti gli elementi hanno
    gli stessi valori, nello stesso ordine.
*/
struct equal_queue_int {
	bool operator()(const queue_int &q1, const queue_int &q2) const {
		if(q1.size() != q2.size())
			return false;
		
		queue_int::const_iterator i1, ie1, i2, ie2;
		i1 = q1.begin();
		ie1 = q1.end();
		i2 = q2.begin();
		ie2 = q2.end();
		
		while(i1 != ie1 && i2 != ie2) {
			if(*i1 != *i2)
				return false;
			++i1;
			++i2;
		}
		return true;
	}
};

/**
	@brief Predicato su code di interi che determina se una coda di interi
           contiene meno di un certo numero di elementi
*/
struct size_less_than_queue_int {

    unsigned int size; ///< @brief Intero da confrontare con il numero
                       ///< di elementi della coda di interi

    /**
		@brief Costruttore

        @description
		Costruttore che istanzia il predicato, inizializzandolo con l'intero
        da confrontare con il numero di elementi della coda di interi.
	*/
    size_less_than_queue_int(unsigned int n) : size(n) {} // initialization list

    /**
	    @brief Predicato su code di interi che determina se una coda di interi
               contiene meno di un certo numero di elementi

        @param q coda di interi di cui determinare se contiene meno di un certo
               numero di elementi

	    @return true se la coda di interi contiene meno del numero di elementi
                dato
        @return false altrimenti
    */
    bool operator()(const queue_int &q) const {
        return q.size() < size;
    }
};

/**
	@brief Operatore su code di interi che somma un certo valore a tutti
           gli elementi di un coda di interi
*/
struct add_queue_int {

    int value; ///< @brief Valore intero da sommare a tutti gli elementi
               ///< della coda di interi

    /**
		@brief Costruttore

        @description
		Costruttore che istanzia l'operatore, inizializzandolo con il valore
        intero da sommare a tutti gli elementi della coda di interi.
	*/
    add_queue_int(int v) : value(v) {} // initialization list

    /**
	    @brief Operatore su code di interi che somma un certo valore a tutti
               gli elementi di un coda di interi

	    @param q coda di interi ai cui elementi sommare un certo valore

	    @return coda di interi con tutti i valori degli elementi sommati
                al valore intero dato
    */
    queue_int operator()(const queue_int &q) const {
        queue_int tmp(q);
        queue_int::iterator i, ie;
        for(i = tmp.begin(), ie = tmp.end(); i != ie; ++i)
            *i += value;
        return tmp;
    }
};

template <typename E, typename T, typename P, typename F>
void test_queue(const std::vector<T> values, const std::list<T> enqueue_values,
        T set_tail_value, T set_head_value, P pred, F op) {

    typedef T value_type;
    typedef queue<value_type, E> queue;
    typedef unsigned int size_type;

    E equals;
	
	std::cout << "******** Test dei metodi fondamentali ********" << std::endl;
	std::cout << std::endl;
	
	std::cout << "Costruttore di default:" << std::endl;
    queue q;
	std::cout << q << std::endl;
    size_type q_size = q.size();
	std::cout << "Numero di elementi inseriti nella coda: " << q_size <<
	             std::endl;
	assert(q_size == 0);
	std::cout << std::endl;
	
	std::cout << "enqueue (non e' un metodo fondamentale, ma necessario per "
                 "testarli):" << std::endl;
	std::cout << "Inserimento dei valori: ";
	for(size_type i = 0; i < values.size(); ++i) {
		if(i != 0)
		    std::cout << ", ";
	    std::cout << values[i];

        size_type q_size_pre = q.size();
        q.enqueue(values[i]);
        assert(q.size() == q_size_pre + 1);
	}
	std::cout << std::endl;
    std::cout << q << std::endl;
    q_size = q.size();
	std::cout << "Numero di elementi inseriti nella coda: " << q_size <<
	             std::endl;
	assert(q_size == values.size());
	std::cout << std::endl;

	std::cout << "Copy Constructor:" << std::endl;
    queue q2(q);
	std::cout << q2 << std::endl;
    size_type q2_size = q2.size();
    std::cout << "Numero di elementi inseriti nella coda: " << q2_size <<
                 std::endl;
	assert(q2_size == q_size);
	std::cout << std::endl;

	std::cout << "Operatore di assegnamento (operator=):" << std::endl;
    queue q3;
	q3 = q;
    std::cout << q3 << std::endl;
    size_type q3_size = q3.size();
    std::cout << "Numero di elementi inseriti nella coda: " << q3_size <<
                 std::endl;
    assert(q3_size == q_size);
    std::cout << std::endl;

    std::cout << "Distruttore:" << std::endl;
    queue *q4 = new queue(q);
    delete q4;
    q4 = nullptr;
	
	std::cout << std::endl;
	std::cout << "******** Test dei metodi d'uso dell'interfaccia pubblica "
                 "********" << std::endl;
	std::cout << std::endl;

    std::cout << "size:" << std::endl;
    std::cout << "Gia' testato in precedenza." << std::endl;
    std::cout << std::endl;

    std::cout << "enqueue di un singolo elemento:" << std::endl;
    std::cout << "Gia' testato in precedenza." << std::endl;
    std::cout << std::endl;

    std::cout << "enqueue da coda:" << std::endl;
    size_type q_size_pre = q.size();
    q.enqueue(q2.begin(), q2.end());
    std::cout << q << std::endl;
    size_type q_size_post = q.size();
    std::cout << "Numero di elementi inseriti nella coda: " << q_size_post <<
              std::endl;
    assert(q_size_post == q_size_pre + q2.size());
    std::cout << std::endl;

    std::cout << "enqueue da lista:" << std::endl;
    q_size_pre = q.size();
    q.enqueue(enqueue_values.begin(), enqueue_values.end());
    std::cout << q << std::endl;
    q_size_post = q.size();
    std::cout << "Numero di elementi inseriti nella coda: " << q_size_post <<
              std::endl;
    assert(q_size_post == q_size_pre + enqueue_values.size());
    std::cout << std::endl;

    std::cout << "dequeue:" << std::endl;
    for(size_type i = 0; i < values.size(); ++i) {
        q_size_pre = q.size();
        value_type removed_value = q.dequeue();
        std::cout << "Valore dell'elemento rimosso: " << removed_value <<
                     std::endl;
        std::cout << q << std::endl;
        assert(equals(removed_value, values[i]));
        q_size_post = q.size();
        std::cout << "Numero di elementi inseriti nella coda: " <<
                     q_size_post << std::endl;
        assert(q_size_post == q_size_pre - 1);
    }
    std::cout << std::endl;

    std::cout << "dequeue su coda vuota:" << std::endl;
    queue empty_queue;
    try {
        empty_queue.dequeue();
    }
    catch(empty_queue_exception &e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "get_tail:" << std::endl;
    value_type tail = q.get_tail();
    std::cout << "Valore dell'elemento in coda: " << tail << std::endl;
    assert(equals(tail, enqueue_values.back()));
    std::cout << std::endl;

    std::cout << "get_tail su coda vuota:" << std::endl;
    try {
        empty_queue.get_tail();
    }
    catch(empty_queue_exception &e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "set_tail:" << std::endl;
    std::cout << "Valore da sovrascrivere in coda: " << set_tail_value <<
                 std::endl;
    q.set_tail(set_tail_value);
    std::cout << q << std::endl;
    assert(equals(q.get_tail(), set_tail_value));
    std::cout << std::endl;

    std::cout << "set_tail su coda vuota:" << std::endl;
    try {
        empty_queue.set_tail(set_tail_value);
    }
    catch(empty_queue_exception &e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "get_head:" << std::endl;
    value_type head = q.get_head();
    std::cout << "Valore dell'elemento in testa: " << head << std::endl;
    assert(values.size() > 1);
    assert(equals(head, values.front()));
    std::cout << std::endl;

    std::cout << "get_head su coda vuota:" << std::endl;
    try {
        empty_queue.get_head();
    }
    catch(empty_queue_exception &e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "set_head:" << std::endl;
    std::cout << "Valore da sovrascrivere in testa: " << set_head_value <<
              std::endl;
    q.set_head(set_head_value);
    std::cout << q << std::endl;
    assert(equals(q.get_head(), set_head_value));
    std::cout << std::endl;

    std::cout << "set_head su coda vuota:" << std::endl;
    try {
        empty_queue.set_head(set_head_value);
    }
    catch(empty_queue_exception &e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "contains:" << std::endl;
    bool q_contains = q.contains(set_head_value);
    std::cout << "Il valore " << set_head_value;
    if(!q_contains)
        std::cout << " non";
    std::cout << " e' presente nella coda." << std::endl;
    assert(q_contains);
    for(size_type i = 0; i < values.size(); ++i) {
        q_contains = q.contains(values[i]);
        std::cout << "Il valore " << values[i];
        if(!q_contains)
            std::cout << " non";
        std::cout << " e' presente nella coda." << std::endl;
    }
    typename std::list<value_type>::const_iterator lit, lite;
    for(lit = enqueue_values.begin(), lite = enqueue_values.end(); lit != lite;
        ++lit) {
        q_contains = q.contains(*lit);
        std::cout << "Il valore " << *lit;
        if(!q_contains)
            std::cout << " non";
        std::cout << " e' presente nella coda." << std::endl;
    }
    q_contains = q.contains(set_tail_value);
    std::cout << "Il valore " << set_tail_value;
    if(!q_contains)
        std::cout << " non";
    std::cout << " e' presente nella coda." << std::endl;
    assert(q_contains);

    std::cout << std::endl;
    std::cout << "******** Test degli iteratori ********" << std::endl;
    std::cout << std::endl;

    std::cout << "iterator:" << std::endl;
    std::cout << "Stampa in colonna con iteratore in lettura e scrittura:" <<
                 std::endl;
    typename queue::iterator it, ite;
    for(it = q.begin(), ite = q.end(); it != ite; ++it)
        std::cout << *it << std::endl;
    std::cout << std::endl;

    std::cout << "Sovrascrittura e stampa in colonna dei valori con iteratore "
                 "in lettura e scrittura:" << std::endl;
    queue q_write(q);
    typename queue::iterator it_write, ite_write;
    for(it_write = q_write.begin(), ite_write = q_write.end();
        it_write != ite_write; ++it_write) {
        *it_write = op(*it_write);
        std::cout << *it_write << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Pre-incremento fuori dai limiti:" <<
              std::endl;
    try {
        ++it;
    }
    catch(queue_iterator_out_of_bounds_exception &e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Post-incremento fuori dai limiti:" <<
              std::endl;
    try {
        it++;
    }
    catch(queue_iterator_out_of_bounds_exception &e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << std::endl;
	
	std::cout << "const_iterator:" << std::endl;
	std::cout << "Stampa in colonna con iteratore costante:" << std::endl;
	typename queue::const_iterator cit, cite;
	for(cit = q.begin(), cite = q.end(); cit != cite; ++cit)
		std::cout << *cit << std::endl;
	std::cout << std::endl;

	// La sovrascrittura dei valori con iteratore costante non è possibile.

    std::cout << "Pre-incremento fuori dai limiti:" <<
              std::endl;
    try {
        ++cit;
    }
    catch(queue_iterator_out_of_bounds_exception &e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Post-incremento fuori dai limiti:" <<
              std::endl;
    try {
        cit++;
    }
    catch(queue_iterator_out_of_bounds_exception &e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << std::endl;
    std::cout << "******** Test di funzioni e operatori globali ********" <<
                 std::endl;
    std::cout << std::endl;

	std::cout << "transformif:" << std::endl;
	queue q_pre(q);
	transformif(q, pred, op);
    std::cout << q << std::endl;
    typename queue::const_iterator cit_pre, cite_pre;
    cit_pre = q_pre.begin();
    cite_pre = q_pre.end();
    cit = q.begin();
    cite = q.end();
    while(cit_pre != cite_pre && cit != cite) {
        if(pred(*cit_pre))
            assert(equals(*cit, op(*cit_pre)));
        else
            assert(equals(*cit, *cit_pre));
        ++cit_pre;
        ++cit;
    }
	std::cout << std::endl;

    std::cout << "Operatore di stream <<:" << std::endl;
    std::cout << "Stampa in riga con l'operatore di stream <<:" << std::endl;
    std::cout << q << std::endl;
	
	std::cout << std::endl;
	std::cout << "******** Test su code costanti ********" << std::endl;

    std::cout << std::endl;
    std::cout << "******** Test dei metodi fondamentali ********" << std::endl;
    std::cout << std::endl;
	
	std::cout << "Costruttore di default:" << std::endl;
	const queue empty_const_queue;
	std::cout << empty_const_queue << std::endl;
	size_type empty_const_queue_size = empty_const_queue.size();
	std::cout << "Numero di elementi inseriti nella coda: " <<
	             empty_const_queue_size << std::endl;
	assert(empty_const_queue_size == 0);
	std::cout << std::endl;
	
	// enqueue non è chiamabile su una coda costante
	
	std::cout << "Copy Constructor:" << std::endl;
	const queue const_queue(q2);
	std::cout << const_queue << std::endl;
	assert(const_queue.size() == q2.size());
	std::cout << std::endl;

    // L'operatore di assegnamento non è chiamabile su una coda costante

    std::cout << "Distruttore:" << std::endl;
    const queue *const_queue2 = new queue(const_queue);
    delete const_queue2;
    const_queue2 = nullptr;

    std::cout << std::endl;
    std::cout << "******** Test dei metodi d'uso dell'interfaccia pubblica "
                 "********" << std::endl;
    std::cout << std::endl;

    // enqueue di un singolo elemento non è chiamabile su una coda costante

    // enqueue da una sequenza non è chiamabile su una coda costante

    std::cout << "size:" << std::endl;
    size_type const_queue_size = const_queue.size();
    std::cout << "Numero di elementi nella coda costante: " <<
                 const_queue_size << std::endl;
    assert(const_queue_size == values.size());
    std::cout << std::endl;

    // dequeue non è chiamabile su una coda costante

    std::cout << "get_tail:" << std::endl;
    tail = const_queue.get_tail();
    std::cout << "Valore dell'elemento in coda: " << tail << std::endl;
    assert(equals(tail, values.back()));
    std::cout << std::endl;

    std::cout << "get_tail su coda costante vuota:" << std::endl;
    try {
        empty_const_queue.get_tail();
    }
    catch(empty_queue_exception &e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << std::endl;

    // set_tail non è chiamabile su una coda costante

    std::cout << "get_head:" << std::endl;
    head = const_queue.get_head();
    std::cout << "Valore dell'elemento in testa: " << head << std::endl;
    assert(equals(head, values.front()));
    std::cout << std::endl;

    std::cout << "get_head su coda costante vuota:" << std::endl;
    try {
        empty_const_queue.get_head();
    }
    catch(empty_queue_exception &e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << std::endl;

    // set_head non è chiamabile su una coda costante

    std::cout << "contains:" << std::endl;
    q_contains = const_queue.contains(set_head_value);
    std::cout << "Il valore " << set_head_value;
    if(!q_contains)
        std::cout << " non";
    std::cout << " e' presente nella coda." << std::endl;
    assert(!q_contains);
    for(size_type i = 0; i < values.size(); ++i) {
        q_contains = const_queue.contains(values[i]);
        std::cout << "Il valore " << values[i];
        if(!q_contains)
            std::cout << " non";
        std::cout << " e' presente nella coda." << std::endl;
        assert(q_contains);
    }
    for(lit = enqueue_values.begin(), lite = enqueue_values.end(); lit != lite;
        ++lit) {
        q_contains = const_queue.contains(*lit);
        std::cout << "Il valore " << *lit;
        if(!q_contains)
            std::cout << " non";
        std::cout << " e' presente nella coda." << std::endl;
        assert(!q_contains);
    }
    q_contains = const_queue.contains(set_tail_value);
    std::cout << "Il valore " << set_tail_value;
    if(!q_contains)
        std::cout << " non";
    std::cout << " e' presente nella coda." << std::endl;
    assert(!q_contains);

    std::cout << std::endl;
    std::cout << "******** Test degli iteratori ********" << std::endl;
    std::cout << std::endl;

    // Gli iteratori in lettura e scrittura non possono essere utilizzati su
    // una coda costante.

    std::cout << "const_iterator:" << std::endl;
    std::cout << "Stampa in colonna con iteratore costante:" << std::endl;
    for(cit = const_queue.begin(), cite = const_queue.end(); cit != cite; ++cit)
        std::cout << *cit << std::endl;
    std::cout << std::endl;

    // La sovrascrittura dei valori con iteratore costante su una coda costante
    // non è possibile.

    std::cout << "Pre-incremento fuori dai limiti:" <<
              std::endl;
    try {
        ++cit;
    }
    catch(queue_iterator_out_of_bounds_exception &e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Post-incremento fuori dai limiti:" <<
              std::endl;
    try {
        cit++;
    }
    catch(queue_iterator_out_of_bounds_exception &e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << std::endl;
    std::cout << "******** Test di funzioni e operatori globali ********" <<
              std::endl;
    std::cout << std::endl;

    // transformif non è chiamabile su una coda costante

    std::cout << "Operatore di stream <<:" << std::endl;
    std::cout << "Stampa in riga con l'operatore di stream <<:" << std::endl;
    std::cout << const_queue << std::endl;
}

void test_queue_int() {
	
	std::cout << std::endl;
	std::cout << "******** Test su una coda di interi ********" << std::endl;
	std::cout << std::endl;

	std::vector<int> values;
	values.push_back(1);
    values.push_back(2);
    values.push_back(3);
    values.push_back(2);
    values.push_back(-1);

    std::list<int> enqueue_values;
    enqueue_values.push_back(0);
    enqueue_values.push_back(4);
    enqueue_values.push_back(5);
    enqueue_values.push_back(6);

    int set_tail_value = 10;
    int set_head_value = 20;
	
	test_queue<equal_int>(values, enqueue_values, set_tail_value,
	                      set_head_value, even_int, double_int);
}

void test_queue_float() {
	
	std::cout << std::endl;
	std::cout << "******** Test su una coda di float ********" << std::endl;
	std::cout << std::endl;

    std::vector<float> values;
    values.push_back(1.0f);
    values.push_back(20.2f);
    values.push_back(20.2f);
    values.push_back(34.5f);

    std::list<float> enqueue_values;
    enqueue_values.push_back(0.3f);
    enqueue_values.push_back(-4.4f);
    enqueue_values.push_back(5.5f);
    enqueue_values.push_back(6.0f);
    enqueue_values.push_back(9.34f);

    float set_tail_value = 50.3f;
    float set_head_value = 30.0f;

    test_queue<equal_float>(values, enqueue_values, set_tail_value,
                            set_head_value, less_than_float(10.0f),
                            add_float(5.5f));
}

void test_queue_bool() {
	
	std::cout << std::endl;
	std::cout << "******** Test su una coda di booleani ********" << std::endl;
	std::cout << std::endl;

    std::vector<bool> values;
    values.push_back(true);
    values.push_back(true);
    values.push_back(true);

    std::list<bool> enqueue_values;
    enqueue_values.push_back(false);
    enqueue_values.push_back(false);

    bool set_tail_value = false;
    bool set_head_value = false;

    test_queue<equal_bool>(values, enqueue_values, set_tail_value,
                           set_head_value, true_bool, negate_bool);
}

void test_queue_string() {
	
	std::cout << std::endl;
	std::cout << "******** Test su una coda di stringhe ********" << std::endl;
	std::cout << std::endl;

    std::vector<std::string> values;
    values.push_back("12345");
    values.push_back("QWERTY");
    values.push_back("C++");
    values.push_back("QWERTY");

    std::list<std::string> enqueue_values;
    enqueue_values.push_back("Quando");
    enqueue_values.push_back("questa e' una stringa");
    enqueue_values.push_back("Programmazione");

    std::string set_tail_value = "ultimo";
    std::string set_head_value = "primo";

    test_queue<equal_string>(values, enqueue_values, set_tail_value,
                             set_head_value, first_char_string('Q'),
                             remove_first_char_string);
}

void test_queue_complex() {
	
	std::cout << std::endl;
	std::cout << "******** Test su una coda di numeri complessi ********" << std::endl;
	std::cout << std::endl;

    std::vector<complex> values;
    values.push_back(complex(2, 3));
    values.push_back(complex(2, 2));
    values.push_back(complex(5, -7));

    std::list<complex> enqueue_values;
    enqueue_values.push_back(complex(4, 4));
    enqueue_values.push_back(complex(-7, 5));
    enqueue_values.push_back(complex(10, 0));

    complex set_tail_value = complex(0, -1);
    complex set_head_value = complex(9, 9);

    test_queue<equal_complex>(values, enqueue_values, set_tail_value,
                              set_head_value, equal_real_imaginary_complex,
                              null_imaginary_complex);
}

void test_queue_employee() {
	
	std::cout << std::endl;
	std::cout << "******** Test su una coda di impiegati ********" << std::endl;
	std::cout << std::endl;

    std::vector<employee> values;
    values.push_back(employee("Andrea", "Tassi", 300));
    values.push_back(employee("a", "b", 200));
    values.push_back(employee("Andrea", "Tassi", 700));

    std::list<employee> enqueue_values;
    enqueue_values.push_back(employee("a", "f", 401));
    enqueue_values.push_back(employee("b", "b", 255));

    employee set_tail_value = employee("nome", "cognome", 230);
    employee set_head_value = employee("Name", "Surname", 300);

    test_queue<equal_employee>(values, enqueue_values, set_tail_value,
                               set_head_value,
                               salary_greater_or_equal_than_employee(300),
                               half_salary_employee);
}

void test_queue_queue_int() {
	
	std::cout << std::endl;
	std::cout << "******** Test su una coda di code di interi ********" << std::endl;
	std::cout << std::endl;

	queue_int q1;
    q1.enqueue(0);
    q1.enqueue(1);
    q1.enqueue(-1);

	queue_int q2;
    q2.enqueue(3);
    q2.enqueue(5);
    q2.enqueue(5);
    q2.enqueue(7);

    std::vector<queue_int> values;
    values.push_back(q1);
    values.push_back(q2);

    queue_int q3;
    q3.enqueue(0);
    q3.enqueue(1);
    q3.enqueue(0);

    queue_int q4;
    q4.enqueue(4);
    q4.enqueue(8);
    q4.enqueue(-9);
    q4.enqueue(0);

    std::list<queue_int> enqueue_values;
    enqueue_values.push_back(q3);
    enqueue_values.push_back(q4);

    queue_int q5;
    q5.enqueue(3);

    queue_int q6;
    q6.enqueue(8);
    q6.enqueue(-11);

    queue_int set_tail_value = q5;
    queue_int set_head_value = q6;

    test_queue<equal_queue_int>(values, enqueue_values, set_tail_value,
                                set_head_value, size_less_than_queue_int(3),
                                add_queue_int(10));
}

void test_continue() {
	
	std::cout << std::endl;
	std::cout << "******** Fine test precedente ********" << std::endl;
}

void test() {
	
	std::cout << "**************** INIZIO TEST ****************" << std::endl;

	test_queue_int();

	test_continue();
	test_queue_float();

	test_continue();
	test_queue_bool();

	test_continue();
	test_queue_string();

	test_continue();
	test_queue_complex();

	test_continue();
	test_queue_employee();

	test_continue();
	test_queue_queue_int();
	
	std::cout << std::endl << "**************** FINE TEST ****************" <<
	             std::endl;
}

int main() {

	test();

	return 0;
}

// Fine file main.cpp
