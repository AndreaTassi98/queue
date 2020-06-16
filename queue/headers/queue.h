/**
	@headerfile queue.h

    @brief Dichiarazione e definizione della classe queue

    @description
	File header con dichiarazione e definizione della classe template queue,
    di una funzione globale transformif e della ridefinizione dell'operatore
    di stream << per la classe queue.
*/

// Guardie del file header queue.h

#ifndef QUEUE_H
#define QUEUE_H

// Direttive per il pre-compilatore

#include <ostream> // std::ostream
#include <iterator> // std::forward_iterator_tag
#include <cstddef> // std::ptrdiff_t
#include "queue_exceptions.h" // empty_queue_exception,
                              // queue_iterator_out_of_bounds_exception

// Dichiarazione e definizione della classe template queue

/**
	@brief Coda FIFO
	
	@description
    Classe template che implementa una coda FIFO di elementi generici T.
	In una coda FIFO è possibile inserire e rimuovere un elemento alla volta.
    L'ordine di inserimento determina l'anzianità dell'elemento.
    L'elemento rimosso è sempre il più vecchio inserito nella coda.
    Le operazioni d'inserimento e rimozione sono eseguite in tempo costante.
	
	@tparam T tipo del valore degli elementi della coda
    @tparam E funtore di confronto di uguaglianza del valore di due elementi
*/
template <typename T, typename E>
class queue {

    // Tipo di supporto privato della coda
    typedef T value_type; ///< @brief Tipo di supporto privato per rappresentare
                          ///< il tipo generico del valore degli elementi
                          ///< della coda

    // Strutture di supporto interne private della coda

	/**
		@brief Elemento della coda
		
		@description
	    Struttura di supporto interna privata della coda
	    che implementa un elemento della coda.
	    Un elemento della coda è costituito da un valore di tipo T
	    e dal puntatore all'elemento successivo della coda.
	*/
	struct element {

        // Dati membro privati dell'elemento della coda

		value_type value; ///< @brief Valore dell'elemento
		element *next; ///< @brief Puntatore all'elemento successivo della coda

		// Metodi fondamentali dell'elemento della coda

		/**
			@brief Costruttore di default (METODO FONDAMENTALE)
			
			@description
		    Costruttore di default per istanziare un elemento vuoto.
		    I dati membro sono inizializzati tramite initialization list,
		    per migliorare l'efficienza dell'inizializzazione.
		    Il valore dell'elemento vuoto è inizializzato tramite il
		    costruttore di default del tipo T.
		    Il puntatore all'elemento successivo della coda è inizializzato
		    a nullptr.

			@note Questo è l'unico costruttore che può essere utilizzato
		          per istanziare un eventuale array di elementi.
		*/
		element() :
		    next(nullptr) {} // initialization list

		// Costruttori secondari dell'elemento della coda

		/**
			@brief Costruttore secondario
			
			@description
		    Costruttore secondario che permette di istanziare un elemento,
			specificandone il valore.
		    I dati membro sono inizializzati tramite initialization list,
		    per migliorare l'efficienza dell'inizializzazione.
		    Il valore dell'elemento è inizializzato al valore v di tipo T
		    passato come parametro al costruttore.
		    Il puntatore all'elemento successivo della coda è inizializzato
		    a nullptr.

		    @note Questo costruttore è definito explicit per evitare che venga
		          utilizzato dal compilatore per effettuare conversioni di tipo
		          implicite da T a element.

			@param v valore dell'elemento
		*/
		explicit element(const value_type &v) :
			value(v), next(nullptr) {} // initialization list
		
		/**
			@brief Costruttore secondario
			
			@description
		    Costruttore secondario che permette di istanziare un elemento,
			specificandone il valore e il puntatore all'elemento successivo
		    della coda.
		    I dati membro sono inizializzati tramite initialization list,
		    per migliorare l'efficienza dell'inizializzazione.
		    Il valore dell'elemento è inizializzato al valore v di tipo T
		    passato come primo parametro al costruttore.
		    Il puntatore all'elemento successivo della coda è inizializzato
		    al valore n passato come secondo parametro al costruttore.
			
			@param v valore dell'elemento
			@param n puntatore all'elemento successivo della coda
		*/
		element(const value_type &v, element *n) :
			value(v), next(n) {} // initialization list

		/**
			@brief Distruttore (METODO FONDAMENTALE)
			
			@description
		    Distruttore per deallocare la memoria allocata da un elemento.
		    Viene chiamato automaticamente il distruttore del valore
		    dell'elemento, corrispondente al distruttore del tipo T.
		    Al puntatore all'elemento successivo della coda viene assegnato
		    il valore di default nullptr.

		    @post La locazione di memoria allocata in precedenza dall'elemento
		          ora non è più valida.
		*/
		~element() {
			next = nullptr;
		}

        /*
            Gli altri metodi fondamentali (Copy Constructor e operatore di
            assegnamento) coincidono con quelli di default, pertanto è inutile
            e ridondante specificarli.
        */

	}; // struct element

	// Dati membro privati della coda

	element *_head; ///< @brief Puntatore all'elemento in testa alla coda
                    ///< (il primo elemento della coda)
    element *_tail; ///< @brief Puntatore all'elemento in coda alla coda
                    ///< (l'ultimo elemento della coda)

    // Tipo di supporto privato della coda
	typedef unsigned int size_type; ///< @brief Tipo di supporto privato per
                                    ///< rappresentare il numero di elementi
                                    ///< inseriti nella coda (dimensione della
                                    ///< coda)
	size_type _size; ///< @brief Numero di elementi inseriti nella coda
                     ///< (dimensione della coda)

    E _equals; ///< @brief Oggetto funtore per il confronto di uguaglianza
               ///< del valore di due elementi della coda

    // Funzioni private della coda

    /**
		@brief Rimozione dell'intero contenuto della coda

		@description
        Funzione privata che rimuove tutti gli elementi della coda.
        La rimozione viene effettuata ricorsivamente, chiamando la funzione
        helper privata ricorsiva clear_helper(element *e), a partire
        dall'elemento in testa alla coda.
        Al puntatore alla coda viene assegnato il valore di default nullptr
        (poiché, al termine dell'esecuzione della funzione helper ricorsiva,
        punta ad una locazione di memoria ora non più valida).

        @post La memoria allocata da tutti gli elementi della coda
	          viene deallocata.
	*/
    void clear() {
        clear_helper(_head);
        _tail = nullptr;
    }

    // Funzioni helper private della coda

    /**
		@brief Rimozione ricorsiva degli elementi della coda a partire
        da un elemento (compreso)

		@description
        Funzione helper privata che rimuove ricorsivamente gli elementi
        della coda a partire da un elemento specificato attraverso il puntatore
        ad element passato come parametro (compreso).
        Ogni volta che la funzione viene chiamata ricorsivamente, vengono svolte
        le seguenti operazioni:
        la funzione rimuove prima gli elementi successivi all'elemento da cui
        partire, richiamando la stessa funzione ricorsivamente;
        dopodiché, la memoria allocata dall'elemento da cui partire viene
        deallocata e al puntatore all'elemento da cui partire viene assegnato
        il valore di default nullptr (poiché l'elemento a cui puntava è stato
        rimosso, evitando, così, che il puntatore punti ad una locazione
        di memoria non più valida);
        infine, il numero di elementi inseriti nella coda viene decrementato
        di un'unità.

		@param e puntatore all'elemento da cui iniziare la rimozione
               degli elementi della coda

        @post La memoria allocata dagli elementi della coda a partire da quello
              specificato (compreso) viene deallocata.
        @post Il numero di elementi inseriti nella coda viene decrementato
              di tante unità quanti sono gli elementi della coda a partire
              da quello specificato (compreso).
        @post Il puntatore alla coda punta ad una locazione di memoria ora
              non più valida.
	*/
    void clear_helper(element *e) {
        if(e != nullptr) {
            clear_helper(e->next);
            delete e;
            e = nullptr;
            _size--;
        }
    }

public:

    // Interfaccia pubblica della coda

	// Metodi fondamentali della coda
	
	/**
		@brief Costruttore di default (METODO FONDAMENTALE)
		
		@description
	    Costruttore di default per istanziare una coda vuota.
	    I dati membro sono inizializzati tramite initialization list,
		per migliorare l'efficienza dell'inizializzazione.
		Il puntatore all'elemento in testa alla coda vuota è inizializzato
	    a nullptr (la coda vuota non ha alcun elemento alla propria testa).
	    Il puntatore all'elemento in coda alla coda vuota è inizializzato
	    a nullptr (la coda vuota non ha alcun elemento alla propria coda).
	    Il numero di elementi inseriti nella coda vuota è inizializzato a 0
	    (la coda vuota non presenta alcun elemento al suo interno).

		@note Questo è l'unico costruttore che può essere utilizzato
	          per istanziare un eventuale array di code.
	*/
	queue() :
	    _head(nullptr), _tail(nullptr), _size(0) {} // initialization list

	/**
		@brief Costruttore di copia/Copy Constructor (METODO FONDAMENTALE)
		
		@description
	    Costruttore di copia/Copy Constructor della coda, che permette
	    di istanziare una coda con i valori presi da un'altra coda.
	    Prima di effettuare la copia dei dati da una coda all'altra,
	    i dati membro della coda da istanziare (ovvero la coda corrente)
	    sono inizializzati con valori di default tramite initialization list:
	    il puntatore all'elemento in testa alla coda è inizializzato
	    a nullptr;
	    il puntatore all'elemento in coda alla coda è inizializzato
	    a nullptr;
	    il numero di elementi inseriti nella coda è inizializzato a 0.
	    Dopodiché, i valori degli elementi della coda da copiare
	    vengono inseriti singolarmente, uno alla volta, nella coda corrente,
	    mantenendo l'ordine originale (i puntatori alla testa e alla coda
	    della coda vengono aggiornati all'interno del metodo di inserimento
	    dei singoli elementi).

	    Nel caso in cui, durante l'inserimento degli elementi
	    nella coda corrente, venga lanciata un'eccezione di allocazione
	    di memoria, quest'ultima viene gestita eliminando completamente
	    la coda corrente, insieme ai suoi singoli elementi (per evitare
	    memory leak), e l'eccezione viene propagata alla funzione chiamante.
	    Pertanto, spetta anche all'utente gestirla adeguatamente
	    nella funzione chiamante.

		@param other coda da copiare per istanziare quella corrente
		
		@throw eccezione di allocazione di memoria
	*/
	queue(const queue &other) :
	    _head(nullptr), _tail(nullptr), _size(0) { // initialization list

	    element *curr = other._head;

        try {
            while(curr != nullptr) {
                enqueue(curr->value);
                curr = curr->next;
            }
        }
        catch(...) { // eccezione di allocazione di memoria
            clear();
            throw;
        }
	}

	/**
		@brief Operatore di assegnamento (METODO FONDAMENTALE)
		
		@description
	    Operatore di assegnamento della coda, che permette la copia tra code.
	    In caso di auto-assegnamento, l'operatore non compie alcuna operazione,
	    restituendo semplicemente un riferimento alla coda corrente
	    (che coincide con quella da copiare).
	    In caso contrario, cioè senza auto-assegnamento (la coda corrente e
	    quella da copiare non coincidono), viene istanziata nello stack
	    una nuova coda temporanea tmp, tramite il Copy Constructor,
	    copiando i valori degli elementi della coda da copiare,
	    passata come parametro.
	    I valori dei dati membro della coda corrente vengono scambiati
	    con quelli dei dati membro della coda temporanea: in questo modo,
	    i puntatori alla testa e alla coda della coda corrente puntano
	    ad elementi con lo stesso valore di quelli puntati dai puntatori
	    alla testa e alla coda della coda da copiare;
	    inoltre, il numero di elementi inseriti nella coda corrente è uguale
	    al numero di elementi inseriti nella coda da copiare.
	    La memoria allocata dalla coda temporanea viene deallocata
	    automaticamente a fine scope, essendo un dato automatico istanziato
	    nello stack.

	    Nel caso in cui, durante l'inserimento degli elementi
	    nella coda temporanea, venga lanciata un'eccezione di allocazione
	    di memoria, quest'ultima viene propagata alla funzione chiamante.
	    Pertanto, spetta all'utente gestirla adeguatamente
	    nella funzione chiamante.

	    @note Non è necessario gestire quest'eccezione direttamente all'interno
	          dell'operatore di assegnamento, poiché l'eventuale memoria
	          allocata dalla coda temporanea viene deallocata automaticamente
	          a fine scope, essendo un dato automatico istanziato nello stack.
		
		@param other coda come sorgente da copiare (operando di destra)
		
		@return riferimento alla coda corrente (this)

		@throw eccezione di allocazione di memoria
	*/
	queue &operator=(const queue &other) {
		// Per evitare l'auto-assegnamento (self-assignment: this = this)
		if(this != &other) {
			queue tmp(other);
			std::swap(_head, tmp._head);
            std::swap(_tail, tmp._tail);
			std::swap(_size, tmp._size);
		}
		return *this;
	}

	/**
		@brief Distruttore (METODO FONDAMENTALE)
		
		@description
	    Distruttore della coda, che dealloca la memoria allocata da una coda,
	    permettendo la deallocazione implicita della memoria alla "morte"
	    della coda.
	    Ciò avviene deallocando la memoria allocata da tutti i suoi elementi.
	    Quest'ultima viene deallocata tramite una chiamata alla funzione privata
	    clear(), che li dealloca ricorsivamente.

	    @note In questo modo, la deallocazione della memoria allocata
	          da una coda comporta anche la deallocazione della memoria allocata
	          da tutti i suoi elementi. Pertanto, non può esistere un elemento
              che non sia presente all'interno di una coda.

	    @note Il distruttore è definito virtual, in modo da rendere polimorfo
	          il metodo e, quindi, anche la classe (in caso di eventuale
	          ereditarietà).

	    @post La memoria allocata da tutti gli elementi della coda
	          viene deallocata.
	    @post Le locazioni di memoria allocate in precedenza dalla coda
	          e da tutti i suoi elementi ora non sono più valide.
	*/
	virtual ~queue() {
		clear();
	}
	
	// Fine metodi fondamentali della coda
	
	// Ulteriori metodi dell'interfaccia pubblica della coda

    /**
        @brief Numero di elementi inseriti nella coda

        @description
        Metodo dell'interfaccia pubblica che restituisce il numero di elementi
        inseriti nella coda.
        Ciò avviene restituendo il valore dell'apposito dato membro _size.

        @return numero di elementi inseriti nella coda
    */
    size_type size() const {
        return _size;
    }

	/**
		@brief Inserimento di un singolo elemento nella coda
		
		@description
	    Metodo dell'interfaccia pubblica che inserisce un singolo elemento
	    nella coda.
	    L'ordine di inserimento determina l'anzianità dell'elemento
	    (l'elemento inserito è il più recente).
	    L'operazione di inserimento è eseguita in tempo costante.
	    Viene istanziato nell'heap un nuovo elemento dinamico, inizializzandolo
	    con il valore passato come parametro.
	    Se la coda è vuota, il nuovo elemento viene inserito in testa
	    (e, pertanto, ne diventa anche la coda).
	    Altrimenti, il puntatore all'elemento successivo all'elemento in coda e
	    il puntatore all'elemento in coda vengono aggiornati, puntando entrambi
	    al nuovo elemento.
	    Infine, in entrambi i casi, il numero di elementi inseriti nella coda
	    viene incrementato di un'unità.

	    Nel caso in cui, durante l'istanziamento del nuovo elemento dinamico
	    nell'heap, venga lanciata un'eccezione di allocazione di memoria,
	    quest'ultima viene propagata alla funzione chiamante. Pertanto,
	    spetta all'utente gestirla adeguatamente nella funzione chiamante.

	    @note Non è necessario gestire quest'eccezione direttamente all'interno
	          del metodo di inserimento, poiché, in caso di eccezione
	          di allocazione di memoria per il nuovo elemento dinamico,
	          questa non viene allocata.
		
		@param value valore dell'elemento da inserire nella coda

	    @post Viene istanziato nell'heap un nuovo elemento dinamico,
	          inizializzato con il valore passato come parametro.
	    @post Il puntatore alla testa della coda viene eventualmente aggiornato,
	          puntando al nuovo elemento.
	    @post Il puntatore alla coda della coda viene aggiornato,
	          puntando al nuovo elemento.
	    @post Il numero di elementi inseriti nella coda viene incrementato
	          di un'unità.

		@throw eccezione di allocazione di memoria
	*/
	void enqueue(const value_type &value) {
		element *tmp = new element(value);

		if(_head == nullptr)
		    _head = tmp;
		else
		    _tail->next = tmp;
		_tail = tmp;

		_size++;
	}

    /**
        @brief Inserimento nella coda di un insieme di elementi presi da una
               sequenza identificata da due iteratori

        @overload enqueue

        @description
        Metodo dell'interfaccia pubblica che inserisce nella coda un insieme
        di elementi presi da una sequenza generica identificata
        da due iteratori, che puntano, rispettivamente, all'inizio e alla fine
        della sequenza.
        La sequenza generica non è necessariamente un'altra coda e gli elementi
        della sequenza generica non sono necessariamente dello stesso tipo T
        degli elementi della coda (l'eventuale conversione tra i tipi
        è delegata alla funzione template static_cast<T>).
        L'ordine di inserimento determina l'anzianità degli elementi.
        Viene istanziata nello stack una coda temporanea, in cui vengono
        inseriti, uno alla volta, gli elementi della sequenza, e la coda
        temporanea viene concatenata a quella corrente.

        Nel caso in cui, durante l'inserimento degli elementi nella coda
        temporanea, venga lanciata un'eccezione di allocazione di memoria,
        quest'ultima viene propagata alla funzione chiamante. Pertanto, spetta
        all'utente gestirla adeguatamente nella funzione chiamante.

        @note Non è necessario gestire quest'eccezione direttamente all'interno
	          del metodo, poiché, in caso di eccezione di allocazione
	          di memoria durante l'inserimento degli elementi nella coda
	          temporanea, quest'ultima, uscendo di scope, viene automaticamente
	          deallocata, essendo un dato automatico istanziato nello stack,
	          e la coda corrente rimane allo stato precedente alla chiamata
	          del metodo.

        @pre Il tipo del valore degli elementi della sequenza deve essere
             convertibile nel tipo del valore degli elementi della coda.

        @tparam IterT tipo degli iteratori che identificano la sequenza
                di elementi da inserire nella coda

        @param begin iteratore che punta all'inizio della sequenza di elementi
               da inserire nella coda
        @param end iteratore che punta alla fine della sequenza di elementi
               da inserire nella coda

        @post Il numero di elementi inseriti nella coda viene incrementato
              di tante unità quanti sono gli elementi della sequenza
              da inserire nella coda.
        @post L'iteratore passato come primo parametro, che puntava all'inizio
              della sequenza di elementi da inserire nella coda, ora punta
              alla fine della stessa sequenza.

        @throw eccezione di allocazione di memoria
    */
    template <typename IterT>
    void enqueue(IterT begin, IterT end) {
        queue tmp;
        while(begin != end) {
            tmp.enqueue(static_cast<value_type>(*begin));
            ++begin;
        }

        _tail->next = tmp._head;
        _tail = tmp._tail;
        _size += tmp._size;
        tmp._head = nullptr;
    }

    /**
        @brief Rimozione dell'elemento più vecchio della coda

        @description
        Metodo dell'interfaccia pubblica che rimuove l'elemento più vecchio
        della coda (l'elemento in testa alla coda).
        L'anzianità degli elementi è determinata dall'ordine di inserimento,
        pertanto l'elemento più vecchio si trova in testa alla coda.
        L'operazione di rimozione è eseguita in tempo costante.
        Il secondo elemento più vecchio della coda diventa l'elemento in testa
        alla coda: il puntatore all'elemento in testa alla coda viene
        aggiornato, puntando a tale elemento.
        L'elemento più vecchio della coda (che in precedenza era in testa
        alla coda) viene rimosso dalla coda e la memoria da esso allocata
        viene deallocata (poiché non può esistere un elemento che non sia
        presente all'interno di una coda): in questo modo, si evita memory leak.
        Inoltre, al puntatore temporaneo a questo elemento viene assegnato
        il valore di default nullptr (per evitare che punti ad una locazione
        di memoria non più valida).
        Infine, il numero di elementi inseriti nella coda viene decrementato
        di un'unità.

        Nel caso in cui la coda sia vuota, non è presente un elemento
        più vecchio da rimuovere e viene lanciata un'eccezione apposita.
        Pertanto, spetta all'utente gestirla adeguatamente
        nella funzione chiamante.

        @pre La coda non dev'essere vuota.

        @return valore dell'elemento rimosso

        @post Il secondo elemento più vecchio della coda diventa l'elemento
              in testa alla coda.
        @post La memoria allocata dall'elemento più vecchio della coda
              viene deallocata.
        @post Il numero di elementi inseriti nella coda viene decrementato
              di un'unità.

        @throw empty_queue_exception se la coda è vuota
    */
    value_type dequeue() {
        if(_size == 0)
            throw empty_queue_exception("Impossibile rimuovere l'elemento piu' "
                                        "vecchio: la coda e' vuota.");
        value_type removed_value = _head->value;

        element *first = _head;
        _head = _head->next;
        delete first;
        first = nullptr;
        _size--;

        return removed_value;
    }

	/**
		@brief Accesso in lettura all'elemento più recente della coda

	    @description
		Metodo dell'interfaccia pubblica che restituisce il valore dell'elemento
	    più recente della coda (l'elemento in coda).

	    @pre La coda non dev'essere vuota.

		@return valore dell'elemento più recente della coda

	    @throw empty_queue_exception se la coda è vuota (non è presente
	           un elemento più recente da leggere)
        @note L'eccezione dev'essere gestita adeguatamente nella funzione
              chiamante.
	*/
    value_type &get_tail() const {
	    if(_size == 0)
	        throw empty_queue_exception("Impossibile leggere l'elemento piu' "
                                        "recente: la coda e' vuota.");
	    return _tail->value;
	}

    /**
        @brief Accesso in scrittura all'elemento più recente della coda

        @description
        Metodo dell'interfaccia pubblica che assegna all'elemento più recente
        della coda (l'elemento in coda) il valore passato come parametro.
        La sovrascrittura di un elemento non ne cambia l'anzianità.

        @pre La coda non dev'essere vuota.

        @param value valore da assegnare all'elemento più recente della coda

        @throw empty_queue_exception se la coda è vuota (non è presente
	           un elemento più recente a cui assegnare il valore)
        @note L'eccezione dev'essere gestita adeguatamente nella funzione
              chiamante.
    */
    void set_tail(const value_type &value) {
        if(_size == 0)
            throw empty_queue_exception("Impossibile assegnare il valore "
                                        "all'elemento piu' recente: "
                                        "la coda e' vuota.");
        _tail->value = value;
    }

    /**
		@brief Accesso in lettura all'elemento più vecchio della coda

        @description
		Metodo dell'interfaccia pubblica che restituisce il valore dell'elemento
	    più vecchio della coda (l'elemento in testa alla coda).

	    @pre La coda non dev'essere vuota.

		@return valore dell'elemento più vecchio della coda

	    @throw empty_queue_exception se la coda è vuota (non è presente
	           un elemento più vecchio da leggere)
        @note L'eccezione dev'essere gestita adeguatamente nella funzione
              chiamante.
	*/
    value_type &get_head() const {
        if(_size == 0)
            throw empty_queue_exception("Impossibile leggere l'elemento piu' "
                                        "vecchio: la coda e' vuota.");
        return _head->value;
    }

    /**
        @brief Accesso in scrittura all'elemento più vecchio della coda

        @description
        Metodo dell'interfaccia pubblica che assegna all'elemento più vecchio
        della coda (l'elemento in testa alla coda) il valore passato come
        parametro.
        La sovrascrittura di un elemento non ne cambia l'anzianità.

        @pre La coda non dev'essere vuota.

        @param value valore da assegnare all'elemento più vecchio della coda

        @throw empty_queue_exception se la coda è vuota (non è presente
	           un elemento più vecchio a cui assegnare il valore)
        @note L'eccezione dev'essere gestita adeguatamente nella funzione
              chiamante.
    */
    void set_head(const value_type &value) {
        if(_size == 0)
            throw empty_queue_exception("Impossibile assegnare il valore "
                                        "all'elemento piu' vecchio: "
                                        "la coda e' vuota.");
        _head->value = value;
    }

    /**
		@brief Esistenza nella coda di almeno un elemento di un certo valore
               dato

        @description
        Metodo dell'interfaccia pubblica che determina se la coda corrente
        contiene almeno un elemento di un certo valore dato, passato come
        parametro.

		@param value valore dell'elemento di cui determinare l'esistenza
               nella coda

		@return true se la coda contiene almeno un elemento del valore dato
        @return false altrimenti
	*/
    bool contains(const value_type &value) const {
        element *curr = _head;

        while(curr != nullptr) {
            if (_equals(curr->value, value))
                return true;
            curr = curr->next;
        }

        return false;
    }

    // Supporto agli iteratori della coda

    class const_iterator; // forward declaration dell'iteratore costante

    // Iteratore della coda in lettura e scrittura

    /**
		@brief Iteratore della coda in lettura e scrittura di tipo forward

	    @description
		Iteratore della coda in lettura e scrittura di tipo forward.
		Gli elementi sono ritornati seguendo la logica FIFO.
        La sovrascrittura di un elemento non ne cambia l’anzianità.
	*/
    class iterator {

        // Dati membro privati dell'iteratore

        element *_e; ///< @brief Puntatore a un elemento della coda

    public:

        // Interfaccia pubblica dell'iteratore

        // Traits dell'iteratore

        typedef std::forward_iterator_tag iterator_category; ///< @brief
                                                             ///< Categoria
                                                             ///< dell'iteratore
        typedef T                         value_type; ///< @brief Tipo dei dati
                                                      ///< puntati
                                                      ///< dall'iteratore
        typedef ptrdiff_t                 difference_type; ///< @brief Tipo per
                                                           ///< rappresentare la
                                                           ///< differenza tra
                                                           ///< due puntatori
        typedef T*                        pointer; ///< @brief Tipo di puntatore
                                                   ///< ai dati puntati
                                                   ///< dall'iteratore
        typedef T&                        reference; ///< @brief Tipo di
                                                     ///< riferimento ai dati
                                                     ///< puntati dall'iteratore

        // Metodi fondamentali dell'iteratore

        /**
			@brief Costruttore di default (METODO FONDAMENTALE)

            @description
			Costruttore di default per istanziare un iteratore che punta
            a nullptr.
		*/
        iterator() : _e(nullptr) {} // initialization list

        /**
			@brief Costruttore di copia/Copy Constructor (METODO FONDAMENTALE)

            @description
			Costruttore di copia/Copy Constructor dell'iteratore.
			Permette di istanziare un iteratore con i valori dei dati membro
            presi da un altro iteratore.

			@param other iteratore da copiare per istanziare quello corrente
		*/
        iterator(const iterator &other) : _e(other._e) {} // initialization list

        /**
			@brief Operatore di assegnamento (METODO FONDAMENTALE)

            @description
			Operatore di assegnamento dell'iteratore.
			Permette la copia tra iteratori.

			@param other iteratore come sorgente da copiare (operando di destra)

			@return riferimento all'iteratore corrente
		*/
        iterator& operator=(const iterator &other) {
            _e = other._e;
            return *this;
        }

        /**
			@brief Distruttore (METODO FONDAMENTALE)

            @description
			Distruttore dell'iteratore.
			Dealloca la memoria allocata da un iteratore e dai suoi dati membro.
		*/
        ~iterator() {}

        // Fine metodi fondamentali dell'iteratore

        // Ulteriori metodi dell'interfaccia pubblica dell'iteratore

        /**
			@brief Operatore di dereferenziamento

            @description
			Operatore di dereferenziamento dell'iteratore.
            Permette l'accesso in lettura e scrittura al valore dell'elemento
            puntato dall'iteratore.

			@return valore dell'elemento puntato dall'iteratore
		*/
        reference operator*() const {
            return _e->value;
        }

        /**
			@brief Operatore di accesso ai dati tramite puntatore

            @description
			Operatore di accesso, tramite puntatore, ai dati puntati
            dall'iteratore.
            Permette l'accesso in lettura e scrittura, tramite puntatore,
            al valore dell'elemento puntato dall'iteratore.

			@return puntatore al valore dell'elemento puntato dall'iteratore
		*/
        pointer operator->() const {
            return &(_e->value);
        }

        /**
			@brief Operatore di iterazione pre-incremento

            @description
			Operatore di iterazione pre-incremento dell'iteratore.
            Permette di incrementare l'iteratore di un'unità, puntando
            all'elemento successivo, e di utilizzarlo allo stato successivo
            all'incremento.

            @pre L'iteratore deve puntare ad una locazione di memoria interna
                 alla coda.

			@return riferimento all'iteratore corrente incrementato

            @throw queue_iterator_out_of_bounds_exception se l'iteratore,
                   incrementando di un'unità, punta ad una locazione di memoria
                   esterna alla coda
		*/
        iterator& operator++() {
            if(_e == nullptr)
                throw queue_iterator_out_of_bounds_exception("Impossibile "
                                                             "incrementare "
                                                             "l'iteratore: "
                                                             "locazione di "
                                                             "memoria esterna "
                                                             "alla coda.");
            _e = _e->next;
            return *this;
        }

        /**
			@brief Operatore di iterazione post-incremento

            @description
			Operatore di iterazione post-incremento dell'iteratore.
            Permette di incrementare l'iteratore di un'unità, puntando
            all'elemento successivo, e di utilizzarlo allo stato precedente
            all'incremento.

            @pre L'iteratore deve puntare ad una locazione di memoria interna
                 alla coda.

			@param int place parameter per distinguere l'operatore di iterazione
                   post-incremento da quello di pre-incremento

			@return copia dell'iteratore corrente prima di essere incrementato

            @throw queue_iterator_out_of_bounds_exception se l'iteratore,
                   incrementando di un'unità, punta ad una locazione di memoria
                   esterna alla coda
		*/
        iterator operator++(int) {
            if(_e == nullptr)
                throw queue_iterator_out_of_bounds_exception("Impossibile "
                                                             "incrementare "
                                                             "l'iteratore: "
                                                             "locazione di "
                                                             "memoria esterna "
                                                             "alla coda.");
            iterator tmp(*this);
            _e = _e->next;
            return tmp;
        }

        /**
			@brief Operatore di uguaglianza

            @description
			Operatore di uguaglianza dell'iteratore.
            Permette di confrontare due iteratori, verificando se sono uguali.

            @note Due iteratori sono definiti "uguali" se puntano allo stesso
                  elemento della coda.

			@param other iteratore da confrontare con quello corrente

			@return true se i due iteratori puntano allo stesso elemento
                    della coda
            @return false altrimenti
		*/
        bool operator==(const iterator &other) const {
            return (_e == other._e);
        }

        /**
			@brief Operatore di diversità

            @description
			Operatore di diversità dell'iteratore.
            Permette di confrontare due iteratori, verificando se sono diversi.

            @note Due iteratori sono definiti "diversi" se non puntano
                  allo stesso elemento della coda.

			@param other iteratore da confrontare con quello corrente

			@return true se i due iteratori non puntano allo stesso elemento
                    della coda
            @return false altrimenti
		*/
        bool operator!=(const iterator &other) const {
            return (_e != other._e);
        }

        /*
           Dati e metodi dell'interfaccia pubblica dell'iteratore
           per la compatibilità tra iteratori in lettura e scrittura
           e iteratori costanti
        */

        friend class const_iterator; // per dare accesso ai dati privati
                                     // dell'iteratore da parte dell'iteratore
                                     // costante

        /**
			@brief Operatore di uguaglianza tra un iteratore in lettura
                   e scrittura e un iteratore costante

            @description
			Operatore di uguaglianza dell'iteratore in lettura e scrittura
            per confrontarlo con un iteratore costante.
            Permette di confrontare un iteratore in lettura e scrittura
            (operando di sinistra) con un iteratore costante
            (operando di destra), verificando se sono uguali.

            @note Due iteratori di tipi diversi (uno in lettura e scrittura
                  e uno costante) sono definiti "uguali" se puntano allo stesso
                  elemento della coda.

			@param other iteratore costante da confrontare con l'iteratore
                   in lettura e scrittura corrente

			@return true se i due iteratori puntano allo stesso elemento
                    della coda
            @return false altrimenti
		*/
        bool operator==(const const_iterator &other) const {
            return (_e == other._e);
        }

        /**
			@brief Operatore di diversità tra un iteratore in lettura
                   e scrittura e un iteratore costante

			@description
            Operatore di diversità dell'iteratore in lettura e scrittura
            per confrontarlo con un iteratore costante.
            Permette di confrontare un iteratore in lettura e scrittura
            (operando di sinistra) con un iteratore costante
            (operando di destra), verificando se sono diversi.

            @note Due iteratori di tipi diversi (uno in lettura e scrittura
                  e uno costante) sono definiti "diversi" se non puntano
                  allo stesso elemento della coda.

			@param other iteratore costante da confrontare con l'iteratore
                   in lettura e scrittura corrente

			@return true se i due iteratori non puntano allo stesso elemento
                    della coda
            @return false altrimenti
		*/
        bool operator!=(const const_iterator &other) const {
            return (_e != other._e);
        }

    private:

        // Ulteriori dati membro privati dell'iteratore

        friend class queue; // per permettere alla classe container (queue)
                            // di utilizzare il costruttore privato
                            // di inizializzazione

        // Metodi privati dell'iteratore

        /**
			@brief Costruttore privato

			@description
            Costruttore privato di inizializzazione dell'iteratore.
            Permette di istanziare un iteratore, inizializzandolo con
            il puntatore a un elemento della coda.

            @note Questo costruttore è definito explicit per evitare che venga
		          utilizzato dal compilatore per effettuare conversioni di tipo
		          implicite da puntatore a elemento a iteratore.

            @note Questo costruttore è utilizzato dalla classe container (queue)
                  nelle funzioni membro che permettono di utilizzare
                  gli iteratori: begin() ed end().

			@param e puntatore a un elemento della coda
		*/
        explicit iterator(element *e) : _e(e) {} // initialization list

    }; // class iterator

    // Funzioni membro per l'utilizzo degli iteratori in lettura e scrittura

    /**
		@brief Iteratore che punta all'inizio della coda

        @description
		Funzione membro che restituisce un iteratore che punta all'inizio
        della coda (all'elemento in testa).

		@return iteratore che punta all'inizio della coda
	*/
    iterator begin() {
        return iterator(_head);
    }

    /**
		@brief Iteratore che punta alla fine della coda

        @description
		Funzione membro che restituisce un iteratore che punta alla fine
        della coda.

		@return iteratore che punta alla fine della coda
	*/
    iterator end() {
        return iterator(nullptr);
    }

    // Iteratore costante della coda

    /**
		@brief Iteratore costante della coda di tipo forward

	    @description
		Iteratore costante (in sola lettura) della coda di tipo forward.
		Gli elementi sono ritornati seguendo la logica FIFO.
	*/
    class const_iterator {

        // Dati membro privati dell'iteratore costante

        const element *_e; ///< @brief Puntatore a un elemento costante
                           ///< della coda

    public:

        // Interfaccia pubblica dell'iteratore costante

        // Traits dell'iteratore costante

        typedef std::forward_iterator_tag iterator_category; ///< @brief
                                                             ///< Categoria
                                                             ///< dell'iteratore
                                                             ///< costante
        typedef const T                   value_type; ///< @brief Tipo dei dati
                                                      ///< puntati
                                                      ///< dall'iteratore
                                                      ///< costante
        typedef ptrdiff_t                 difference_type; ///< @brief Tipo per
                                                           ///< rappresentare la
                                                           ///< differenza tra
                                                           ///< due puntatori
        typedef const T*                  pointer; ///< @brief Tipo di puntatore
                                                   ///< ai dati puntati
                                                   ///< dall'iteratore costante
        typedef const T&                  reference; ///< @brief Tipo di
                                                     ///< riferimento ai dati
                                                     ///< puntati dall'iteratore
                                                     ///< costante

        // Metodi fondamentali dell'iteratore costante

        /**
			@brief Costruttore di default (METODO FONDAMENTALE)

            @description
			Costruttore di default per istanziare un iteratore costante
            che punta a nullptr.
		*/
        const_iterator() : _e(nullptr) {} // initialization list

        /**
			@brief Costruttore di copia/Copy Constructor (METODO FONDAMENTALE)

            @description
			Costruttore di copia/Copy Constructor dell'iteratore costante.
			Permette di istanziare un iteratore costante con i valori dei dati
            membro presi da un altro iteratore costante.

			@param other iteratore costante da copiare per istanziare quello
                   corrente
		*/
        const_iterator(const const_iterator &other) :
            _e(other._e) {} // initialization list

        /**
			@brief Operatore di assegnamento (METODO FONDAMENTALE)

            @description
			Operatore di assegnamento dell'iteratore costante.
			Permette la copia tra iteratori costanti.

			@param other iteratore costante come sorgente da copiare
                   (operando di destra)

			@return riferimento all'iteratore costante corrente
		*/
        const_iterator& operator=(const const_iterator &other) {
            _e = other._e;
            return *this;
        }

        /**
			@brief Distruttore (METODO FONDAMENTALE)

            @description
			Distruttore dell'iteratore costante.
			Dealloca la memoria allocata da un iteratore costante e dai suoi
            dati membro.
		*/
        ~const_iterator() {}

        // Fine metodi fondamentali dell'iteratore costante

        // Ulteriori metodi dell'interfaccia pubblica dell'iteratore costante

        /**
			@brief Operatore di dereferenziamento

            @description
			Operatore di dereferenziamento dell'iteratore costante.
            Permette l'accesso in sola lettura al valore costante dell'elemento
            puntato dall'iteratore costante.

			@return valore costante dell'elemento puntato dall'iteratore
                    costante
		*/
        reference operator*() const {
            return _e->value;
        }

        /**
            @brief Operatore di accesso ai dati tramite puntatore

            @description
			Operatore di accesso, tramite puntatore, ai dati puntati
            dall'iteratore costante.
            Permette l'accesso in sola lettura, tramite puntatore, al valore
            costante dell'elemento puntato dall'iteratore costante.

			@return puntatore al valore costante dell'elemento puntato
                    dall'iteratore costante
		*/
        pointer operator->() const {
            return &(_e->value);
        }

        /**
			@brief Operatore di iterazione pre-incremento

            @description
			Operatore di iterazione pre-incremento dell'iteratore costante.
            Permette di incrementare l'iteratore costante di un'unità, puntando
            all'elemento successivo, e di utilizzarlo allo stato successivo
            all'incremento.

            @pre L'iteratore costante deve puntare ad una locazione di memoria
                 interna alla coda.

			@return riferimento all'iteratore costante corrente incrementato

            @throw queue_iterator_out_of_bounds_exception se l'iteratore
                   costante, incrementando di un'unità, punta ad una locazione
                   di memoria esterna alla coda
		*/
        const_iterator& operator++() {
            if(_e == nullptr)
                throw queue_iterator_out_of_bounds_exception("Impossibile "
                                                             "incrementare "
                                                             "l'iteratore "
                                                             "costante: "
                                                             "locazione di "
                                                             "memoria esterna "
                                                             "alla coda.");
            _e = _e->next;
            return *this;
        }

        /**
			@brief Operatore di iterazione post-incremento

            @description
			Operatore di iterazione post-incremento dell'iteratore costante.
            Permette di incrementare l'iteratore costante di un'unità, puntando
            all'elemento successivo, e di utilizzarlo allo stato precedente
            all'incremento.

            @pre L'iteratore costante deve puntare ad una locazione di memoria
                 interna alla coda.

			@param int place parameter per distinguere l'operatore di iterazione
                   post-incremento da quello di pre-incremento

			@return copia dell'iteratore costante corrente prima di essere
                    incrementato

            @throw queue_iterator_out_of_bounds_exception se l'iteratore
                   costante, incrementando di un'unità, punta ad una locazione
                   di memoria esterna alla coda
		*/
        const_iterator operator++(int) {
            if(_e == nullptr)
                throw queue_iterator_out_of_bounds_exception("Impossibile "
                                                             "incrementare "
                                                             "l'iteratore "
                                                             "costante: "
                                                             "locazione di "
                                                             "memoria esterna "
                                                             "alla coda.");
            const_iterator tmp(*this);
            _e = _e->next;
            return tmp;
        }

        /**
			@brief Operatore di uguaglianza

            @description
			Operatore di uguaglianza dell'iteratore costante.
            Permette di confrontare due iteratori costanti, verificando se sono
            uguali.

            @note Due iteratori costanti sono definiti "uguali" se puntano
                  allo stesso elemento della coda.

			@param other iteratore costante da confrontare con quello corrente

			@return true se i due iteratori costanti puntano allo stesso
                    elemento della coda
            @return false altrimenti
		*/
        bool operator==(const const_iterator &other) const {
            return (_e == other._e);
        }

        /**
			@brief Operatore di diversità

            @description
			Operatore di diversità dell'iteratore costante.
            Permette di confrontare due iteratori costanti, verificando se sono
            diversi.

            @note Due iteratori costanti sono definiti "diversi" se non puntano
                  allo stesso elemento della coda.

			@param other iteratore costante da confrontare con quello corrente

			@return true se i due iteratori costanti non puntano allo stesso
                    elemento della coda
            @return false altrimenti
		*/
        bool operator!=(const const_iterator &other) const {
            return (_e != other._e);
        }

        /*
           Dati e metodi dell'interfaccia pubblica dell'iteratore costante
           per la compatibilità tra iteratori costanti e iteratori
           in lettura e scrittura
        */

        friend class iterator; // per dare accesso ai dati privati
                               // dell'iteratore costante da parte
                               // dell'iteratore in lettura e scrittura

        /**
			@brief Operatore di uguaglianza tra un iteratore costante
                   e un iteratore in lettura e scrittura

            @description
			Operatore di uguaglianza dell'iteratore costante per confrontarlo
            con un iteratore in lettura e scrittura.
            Permette di confrontare un iteratore costante (operando di sinistra)
            con un iteratore in lettura e scrittura (operando di destra),
            verificando se sono uguali.

            @note Due iteratori di tipi diversi (uno costante e uno in lettura
                  e scrittura) sono definiti "uguali" se puntano allo stesso
                  elemento della coda.

			@param other iteratore in lettura e scrittura da confrontare
                   con l'iteratore costante corrente

			@return true se i due iteratori puntano allo stesso elemento
                    della coda
            @return false altrimenti
		*/
        bool operator==(const iterator &other) const {
            return (_e == other._e);
        }

        /**
			@brief Operatore di diversità tra un iteratore costante
                   e un iteratore in lettura e scrittura

            @description
			Operatore di diversità dell'iteratore costante per confrontarlo
            con un iteratore in lettura e scrittura.
            Permette di confrontare un iteratore costante (operando di sinistra)
            con un iteratore in lettura e scrittura (operando di destra),
            verificando se sono diversi.

            @note Due iteratori di tipi diversi (uno costante e uno in lettura
                  e scrittura) sono definiti "diversi" se non puntano
                  allo stesso elemento della coda.

			@param other iteratore in lettura e scrittura da confrontare
                   con l'iteratore costante corrente

			@return true se i due iteratori non puntano allo stesso elemento
                    della coda
            @return false altrimenti
		*/
        bool operator!=(const iterator &other) const {
            return (_e != other._e);
        }

        /**
			@brief Costruttore di conversione da iteratore in lettura e
                   scrittura a iteratore costante

            @description
			Costruttore dell'iteratore costante di conversione da iteratore
            in lettura e scrittura a iteratore costante.
			Permette di convertire un iteratore in lettura e scrittura
            in un iteratore costante, istanziando quest'ultimo con i valori
            dei dati membro presi da un iteratore in lettura e scrittura.

			@param other iteratore in lettura e scrittura da copiare e
                   convertire per istanziare l'iteratore costante corrente
		*/
        const_iterator(const iterator &other) :
            _e(other._e) {} // initialization list

        /**
			@brief Operatore di assegnamento di un iteratore in lettura
                   e scrittura a un iteratore costante

            @description
			Operatore dell'iteratore costante di assegnamento di un iteratore
            in lettura e scrittura a un iteratore costante.
			Permette la copia da un iteratore in lettura e scrittura
            a un iteratore costante.

			@param other iteratore in lettura e scrittura come sorgente
                   da copiare (operando di destra)

			@return riferimento all'iteratore costante corrente
		*/
        const_iterator &operator=(const iterator &other) {
            _e = other._e;
            return *this;
        }

    private:

        // Ulteriori dati membro privati dell'iteratore costante

        friend class queue; // per permettere alla classe container (queue)
                            // di utilizzare il costruttore privato
                            // di inizializzazione

        // Metodi privati dell'iteratore costante

        /**
			@brief Costruttore privato

			@description
            Costruttore privato di inizializzazione dell'iteratore costante.
            Permette di istanziare un iteratore costante, inizializzandolo con
            il puntatore a un elemento costante della coda.

            @note Questo costruttore è definito explicit per evitare che venga
		          utilizzato dal compilatore per effettuare conversioni di tipo
		          implicite da puntatore a elemento costante a iteratore
		          costante.

            @note Questo costruttore è utilizzato dalla classe container (queue)
                  nelle funzioni membro che permettono di utilizzare
                  gli iteratori costanti: begin() e end().

			@param e puntatore a un elemento costante della coda
		*/
        explicit const_iterator(const element *e) :
            _e(e) {} // initialization list

    }; // class const_iterator

    // Funzioni membro per l'utilizzo degli iteratori costanti

    /**
		@brief Iteratore costante che punta all'inizio della coda

        @description
		Funzione membro che restituisce un iteratore costante che punta
        all'inizio della coda (all'elemento in testa).

		@return iteratore costante che punta all'inizio della coda
	*/
    const_iterator begin() const {
        return const_iterator(_head);
    }

    /**
		@brief Iteratore costante che punta alla fine della coda

        @description
		Funzione membro che restituisce un iteratore costante che punta
        alla fine della coda.

		@return iteratore costante che punta alla fine della coda
	*/
    const_iterator end() const {
        return const_iterator(nullptr);
    }

}; // class queue

// Funzioni globali

/**
	@brief Trasformazione dei valori degli elementi della coda che soddisfano
           un predicato

    @description
	Funzione globale generica che, dati una coda Q, un predicato P
    e un operatore generico F, modifica con l'operatore i valori contenuti
    nella coda che soddisfano il predicato.
    Se Qi è un elemento della coda:
    Qi = F(Qi) se P(Qi) == true
    Qi = Qi    se P(Qi) == false

    @tparam Q tipo del valore degli elementi della coda
    @tparam E funtore di confronto di uguaglianza del valore di due elementi
            della coda
    @tparam P tipo del predicato
    @tparam F tipo dell'operatore generico
	
	@param q coda di cui modificare i valori contenuti
	@param pred predicato che i valori contenuti nella coda devono soddisfare
           per essere modificati
    @param op operatore generico con cui modificare i valori contenuti
           nella coda
*/
template <typename Q, typename E, typename P, typename F>
void transformif(queue<Q, E> &q, P pred, F op) {

    /*
        La keyword "typename" è necessaria per specificare che iterator
        non è un dato membro della classe template queue, bensì un tipo
        (in questo caso, una classe) definito al suo interno.
    */
	typename queue<Q, E>::iterator i, ie;

	for(i = q.begin(), ie = q.end(); i != ie; ++i)
		if(pred(*i))
			*i = op(*i);
}

// Operatori globali

/**
	@brief Ridefinizione dell'operatore di stream <<

    @description
	Ridefinizione dell'operatore di stream << per la stampa del contenuto
	della coda.

    @tparam T tipo del valore degli elementi della coda da stampare
    @tparam E funtore di confronto di uguaglianza del valore di due elementi
            della coda

	@param os oggetto di stream di output
	@param q coda da stampare

	@return riferimento allo stream di output
*/
template <typename T, typename E>
std::ostream &operator<<(std::ostream &os, const queue<T, E> &q) {

    /*
        La keyword "typename" è necessaria per specificare che const_iterator
        non è un dato membro della classe template queue, bensì un tipo
        (in questo caso, una classe) definito al suo interno.
    */
    typename queue<T, E>::const_iterator i, ie;

    os << "[";
    for(i = q.begin(), ie = q.end(); i != ie; ++i) {
        if(i != q.begin())
            os << ", ";
        os << *i;
    }
    os << "]";

    return os;
}

#endif

// Fine file header queue.h
