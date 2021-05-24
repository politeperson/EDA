#pragma once
#ifndef SUFFIX_TREE_HPP_
#define SUFFIX_TREE_HPP_

#include <cstdint>
#include <unordered_map>
#include <utility>
#include <set>
#include <memory>
#include <iostream>


class SUFFIX_NODE
{
public:
	SUFFIX_NODE();

	bool checkTransition(char letter);

	std::shared_ptr<SUFFIX_NODE> setTransition(char letter, int k, int p, std::shared_ptr<SUFFIX_NODE> node = nullptr);
	
	void unsetTransition(char letter);

	std::pair<std::pair<int, int>, std::shared_ptr<SUFFIX_NODE>> getTransition(char letter);

	std::shared_ptr<SUFFIX_NODE> getSuffixLink();

	void setSuffixLink(std::shared_ptr<SUFFIX_NODE> _suffix_link);

	~SUFFIX_NODE();
private:
	// transición con un carácter como llave, dos índices y el puntero al nodo hijo
	std::unordered_map<char, std::pair<std::pair<int, int>, std::shared_ptr<SUFFIX_NODE>>> trans;
	std::shared_ptr<SUFFIX_NODE> suffix_link;
};

SUFFIX_NODE::SUFFIX_NODE()
{
	// 64 bits: [00,...,$,-,0-9,a-z]
	// first 26 bits are for lowercase english letters [a-z]: from 0 to 25 positions
	// next 10 bits are for numbers 10-base digits [0-9]: from 26 to 35 positions
	// next 1 bit is for '-' symbol: 36 position
	// next 1 bit is for '$' symbol (terminal symbol): 37 positions
	trans.clear();
	suffix_link.reset();
}

bool SUFFIX_NODE::checkTransition(char letter) 
{
	return trans.find(letter) != trans.end();
}

std::shared_ptr<SUFFIX_NODE> SUFFIX_NODE::setTransition(char letter, int k, int p, std::shared_ptr<SUFFIX_NODE> node)
{
	trans[letter] = std::make_pair(std::make_pair(k, p), node);
	return trans[letter].second; // retornamos un puntero al nuevo estado de la nueva transición creada
}

void SUFFIX_NODE::unsetTransition(char letter) 
{
	trans.erase(letter);
}

std::pair<std::pair<int, int>, std::shared_ptr<SUFFIX_NODE>> SUFFIX_NODE::getTransition(char letter) 
{
	if (trans.find(letter) != trans.end()) return trans[letter];
	// en caso de que no exista la transición retornamos una transición NULL, con índices (-1,-1)
	return std::make_pair(std::make_pair(-1, -1), nullptr);
}

std::shared_ptr<SUFFIX_NODE> SUFFIX_NODE::getSuffixLink() {
	return suffix_link;
}

void SUFFIX_NODE::setSuffixLink(std::shared_ptr<SUFFIX_NODE> _suffix_link) {
	suffix_link = _suffix_link;
}

SUFFIX_NODE::~SUFFIX_NODE()
{
	std::cout << "eliminando nodo con transiciones: " << trans.size() << std::endl;
	trans.clear();
	suffix_link.reset();
}


class SUFFIX_TREE
{
public:
	SUFFIX_TREE();

	std::pair<bool, std::shared_ptr<SUFFIX_NODE>> testAndSplit(std::shared_ptr<SUFFIX_NODE>& s, int k, int p, char t, std::string& T);

	std::shared_ptr<SUFFIX_NODE> canonize(std::shared_ptr<SUFFIX_NODE> s, int& k, int p, std::string& T);

	void update(std::shared_ptr<SUFFIX_NODE>& s, int& k, int i, std::string& T);
	
	void buildSTree(std::string& T);

	void buildSTree(std::string& T, int& i, int& k, size_t size_T);

	~SUFFIX_TREE();

private:
	std::shared_ptr<SUFFIX_NODE> aux_state;
	std::shared_ptr<SUFFIX_NODE> root_state;
	std::unordered_map<char, int> alphabet;
};

SUFFIX_TREE::SUFFIX_TREE()
{
	for (char c = 'a'; c <= 'z'; ++c) alphabet.insert(std::make_pair(c, c - 'a'));
	for (char n = '0'; n <= '9'; ++n) alphabet.insert(std::make_pair(n, 26 + n - '0'));
	alphabet.insert(std::make_pair('-', 36));
	alphabet.insert(std::make_pair('$', 37));
	
	// lo mismo que: aux_state = new SUFFIX_NODE;
	aux_state = std::make_shared<SUFFIX_NODE>();
	root_state = std::make_shared<SUFFIX_NODE>();

	// al inicio creamos transiciones desde el nodo auxiliar hacia la raíz, todos los caracteres
	// de nuestro alfabeto son relacionados cada una de las aristas tiene como índices a-transiciones
	// con (j, j) como índices
	for (auto& letter : alphabet)
		aux_state->setTransition(letter.first, letter.second, letter.second, root_state);
	// y la raíz tiene como suffix_link a aux_state
	root_state->setSuffixLink(aux_state);
}

std::shared_ptr<SUFFIX_NODE> SUFFIX_TREE::canonize(std::shared_ptr<SUFFIX_NODE> s, int& k, int p, std::string& T)
{
	if (p < k) return s; // lo mismo que return(s, k)
	// encuentra la t_k-transición g'(s, (k',p')) = s'
	std::pair<std::pair<int, int>, std::shared_ptr<SUFFIX_NODE>> sp = s->getTransition(T[k]);
	while (sp.first.second - sp.first.first <= p - k)
	{
		// k <- k + p' - k' + 1
		k = k + sp.first.second - sp.first.first + 1;
		// s <- s'
		s = sp.second;
		// if k <= p encuentra la t_k transición g'(s, (k',p')) = s' desde s
		if (k <= p) sp = s->getTransition(T[k]);
	}
	return s; // return (s, k)
}

std::pair<bool, std::shared_ptr<SUFFIX_NODE>> SUFFIX_TREE::testAndSplit(std::shared_ptr<SUFFIX_NODE>& s, int k, int p, char t, std::string& T) 
{
	if (k <= p) 
	{
		// la t_k-transición (s, (k', p')) = s'
		std::pair<std::pair<int,int>, std::shared_ptr<SUFFIX_NODE>> sp = s->getTransition(T[k]);
		// if t = T[k' + p - k + 1] entonces return(true, s)
		if (t == T[(size_t)sp.first.first + (size_t)p - (size_t)k + (size_t)1]) return std::make_pair(true, s);
		// replace t_k transition above by transitions
		// g'(s, (k', k' + p - k)) = r and g'(r, (k' + p - k + 1, p')) = s'
		// where r is a new state
		// g'(s, (k', k' + p - k)) = r
		std::shared_ptr<SUFFIX_NODE> r = s->setTransition(T[sp.first.first], sp.first.first, sp.first.first + p - k, std::make_shared<SUFFIX_NODE>());
		// g'(r, (k' + p - k + 1, p')) = s'
		r->setTransition(T[(size_t)sp.first.first + (size_t)p - (size_t)k + (size_t)1], sp.first.first + p - k + 1, sp.first.second, sp.second);
		return std::make_pair(false, s->getTransition(T[sp.first.first]).second);
	}
	// si no hay una t-transición desde s: return (false, s)
	if (!s->checkTransition(t)) return std::make_pair(false, s);
	// sino return(true, s)
	return std::make_pair(true, s);
}

void SUFFIX_TREE::update(std::shared_ptr<SUFFIX_NODE>& s, int& k, int i, std::string& T) 
{
	std::shared_ptr<SUFFIX_NODE> oldr = root_state; // oldr <- root; r <- nullptr
	// la variable testPair es lo equivalente a (end-point, r)
	std::pair<bool, std::shared_ptr<SUFFIX_NODE>> testPair = testAndSplit(s, k, i - 1, T[i], T);
	// while note end-point
	while (!testPair.first) 
	{
		// set transition to r: g'(r, (i, infinity)) = r' donde r' es un nuevo estado
		testPair.second->setTransition(T[i], i, INT_MAX, std::make_shared<SUFFIX_NODE>());
		// f'(oldr) = r
		if (oldr != root_state) oldr->setSuffixLink(testPair.second);
		// oldr <- r
		oldr = testPair.second;
		// (s, k) = canonize(f'(s), (k, i-1))
		s = canonize(s->getSuffixLink(), k, i - 1, T);// k es pasado por referencia
		// (end-point, r) = test-and-split(s, (k, p), t_i)
		testPair = testAndSplit(s, k, i - 1, T[i], T);
	}
	if (oldr != root_state) oldr->setSuffixLink(s);
	return; // return (s, k)
}

void SUFFIX_TREE::buildSTree(std::string& T) {
	int k = 0, size_T = T.size();
	std::shared_ptr<SUFFIX_NODE> s = root_state;
	for (int i = 0; i < size_T; ++i) {	
		update(s, k, i, T); // (s,k) <- update(s, (k,i))
		s = canonize(s, k, i, T); // (s,k) <- canonize(s, (k,i))
	}
}

void SUFFIX_TREE::buildSTree(std::string& T, int& i, int& k, size_t size_T) {
	std::shared_ptr<SUFFIX_NODE> s = root_state;
	for (i; i < size_T; ++i) {
		update(s, k, i, T); // (s,k) <- update(s, (k,i))
		s = canonize(s, k, i, T); // (s,k) <- canonize(s, (k,i))
	}
	// añadimos uno para que continúe con la búsqueda al recibir el siguiente string
	++i;
}


SUFFIX_TREE::~SUFFIX_TREE()
{
	aux_state.reset();
	root_state.reset();
}



#endif // !SUFFIX_TREE_HPP_

