/**
	@file queue_exceptions.cpp

	@brief Definizione delle classi di eccezioni custom relative alla classe
           queue

    @description
    File sorgente con la definizione delle classi di eccezioni custom
    empty_queue_exception e queue_iterator_out_of_bounds_exception.
*/

// Direttive per il pre-compilatore

#include "../headers/queue_exceptions.h" // empty_queue_exception,
                                       // queue_iterator_out_of_bounds_exception

// Definizione della classe eccezione custom empty_queue_exception

// Definizione del costruttore
empty_queue_exception::empty_queue_exception(const std::string &message) :
    _message(message) {} // initialization list

// Definizione del metodo what()
std::string empty_queue_exception::what() const {
    return _message;
}

/*
	Definizione della classe eccezione custom
    queue_iterator_out_of_bounds_exception
*/

// Definizione del costruttore
queue_iterator_out_of_bounds_exception::
    queue_iterator_out_of_bounds_exception(const std::string &message) :
    _message(message) {} // initialization list

// Definizione del metodo what()
std::string queue_iterator_out_of_bounds_exception::what() const {
    return _message;
}
