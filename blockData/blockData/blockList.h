#ifndef _BLOCK_LIST_H
#define _BLOCK_LIST_H
#define NEW_BLOCK_EXPANSION_RATE 1.1

#include "block.h"
#include <vector>

template <class T>
class blockList {
public:
	typedef T value_type;
	typedef typename block<block<T> *>::iterator block_table_itr;
	typedef typename block<T>::iterator block_itr;
	class iterator {
	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef std::ptrdiff_t difference_type;
	private:
		class blockList<T> *iBlockList;
		block_table_itr blockTableItr;
		block_itr blockItr;
	public:
		iterator() : iBlockList(0), blockTableItr(0), blockItr(0) {}
		iterator(blockList<T> *_iBlockList, block_table_itr _blockTableItr, block_itr _blockItr) : iBlockList(_iBlockList), blockTableItr(_blockTableItr), blockItr(_blockItr) {}
		iterator(const iterator &itr) : iBlockList(itr.iBlockList), blockTableItr(itr.blockTableItr), blockItr(itr.blockItr) {}
		iterator& operator++() {
			if (this->blockItr == (*(this->blockTableItr))->end() - 1 && this->blockTableItr != this->iBlockList->blockTable.end() - 1) {
				this->blockTableItr++;
				this->blockItr = (*(this->blockTableItr))->begin();
			}
			else this->blockItr++;
			return (*this);
		}
		iterator operator++(int i) {
			iterator ret(*this);
			++(*this);
			return ret;
		}
		iterator& operator--() {
			if (*this == this->iBlockList->begin()) {
				this->iBlockList = 0;
				return (*this);
			}
			if (this->blockItr == (*(this->blockTableItr))->begin()) {
				this->blockTableItr--;
				this->blockItr = (*(this->blockTableItr))->end() - 1;
			}
			else this->blockItr--;
			return (*this);
		}
		iterator operator--(int i) {
			iterator ret(*this);
			--(*this);
			return ret;
		}
		iterator operator+(int i) {
			if (i < 0) return (*this - (-i));
			iterator ret(*this);
			while (i != 0) {
				if (i < (*(ret.blockTableItr))->end() - ret.blockItr || ((i == (*(ret.blockTableItr))->end() - ret.blockItr) && (ret.blockTableItr == ret.iBlockList->blockTable.end() - 1))) {
					ret.blockItr += i;
					i = 0;
				}
				else {
					i = i - ((*(ret.blockTableItr))->end() - ret.blockItr);
					ret.blockTableItr++;
					ret.blockItr = (*(ret.blockTableItr))->begin();
				}
			}
			return iterator(ret);
		}
		friend iterator operator+(int i, const iterator &itr) {
			return (itr + i);
		}
		iterator operator-(int i) {
			if (i < 0) return (*this + (-i));
			iterator ret(*this);
			while (i != 0) {
				if (i <= ret.blockItr - (*(ret.blockTableItr))->begin()) {
					ret.blockItr -= i;
					i = 0;
				}
				else {
					if (ret.blockTableItr == ret.iBlockList->blockTable.begin()) {
						ret.iBlockList = 0;
						break;
					}
					i = i - (ret.blockItr - (*(ret.blockTableItr))->begin());
					ret.blockTableItr--;
					ret.blockItr = (*(ret.blockTableItr))->end();
				}
			}
			return iterator(ret);
		}
		friend std::ptrdiff_t operator-(iterator itr1, iterator itr2) {
			int ret = 0, coef = -1;
			if (itr1 > itr2) {
				iterator box = itr1;
				itr1 = itr2;
				itr2 = box;
				coef = 1;
			}
			while (itr1.blockTableItr < itr2.blockTableItr) {
				ret += (*(itr1.blockTableItr))->end() - itr1.blockItr;
				itr1.blockTableItr++;
				itr1.blockItr = (*(itr1.blockTableItr))->begin();
			}
			ret += itr2.blockItr - itr1.blockItr;
			return ret * coef;
		}
		iterator& operator=(const iterator &itr) {
			iBlockList = itr.iBlockList;
			blockTableItr = itr.blockTableItr;
			blockItr = itr.blockItr;
			return (*this);
		}
		iterator& operator+=(int i) {
			if (i < 0) return (*this -= (-i));
			while (i != 0) {
				if (i < ((*(this->blockTableItr))->end()) - this->blockItr || ((i == (*(this->blockTableItr))->end() - this->blockItr) && (this->blockTableItr == this->iBlockList->blockTable.end() - 1))) {
					this->blockItr += i;
					i = 0;
				}
				else {
					i = i - ((*(this->blockTableItr))->end() - this->blockItr);
					this->blockTableItr++;
					this->blockItr = (*(this->blockTableItr))->begin();
				}
			}
			return (*this);
		}
		iterator& operator-=(int i) {
			if (i < 0) return (*this += (-i));
			while (i != 0) {
				if (i <= this->blockItr - (*(this->blockTableItr))->begin()) {
					this->blockItr -= i;
					i = 0;
				}
				else {
					if (this->blockTableItr == this->iBlockList->blockTable.begin()) {
						this->iBlockList = 0;
						break;
					}
					i = i - (this->blockItr - (*(this->blockTableItr))->begin());
					this->blockTableItr--;
					this->blockItr = (*(this->blockTableItr))->end();
				}
			}
			return (*this);
		}
		bool operator!=(const iterator &itr) {
			return (iBlockList != itr.iBlockList || blockTableItr != itr.blockTableItr || blockItr != itr.blockItr);
		}
		bool operator==(const iterator &itr) {
			return (iBlockList == itr.iBlockList && blockTableItr == itr.blockTableItr && blockItr == itr.blockItr);
		}
		bool operator<(const iterator &itr) {
			return (iBlockList == itr.iBlockList && (blockTableItr < itr.blockTableItr || (blockTableItr == itr.blockTableItr && blockItr < itr.blockItr)));
		}
		bool operator>(const iterator &itr) {
			return (iBlockList == itr.iBlockList && (blockTableItr > itr.blockTableItr || (blockTableItr == itr.blockTableItr && blockItr > itr.blockItr)));
		}
		bool operator<=(const iterator &itr) {
			return (iBlockList == itr.iBlockList && (blockTableItr < itr.blockTableItr || (blockTableItr == itr.blockTableItr && blockItr <= itr.blockItr)));
		}
		bool operator>=(const iterator &itr) {
			return (iBlockList == itr.iBlockList && (blockTableItr > itr.blockTableItr || (blockTableItr == itr.blockTableItr && blockItr >= itr.blockItr)));
		}
		friend T& operator*(typename blockList<T>::iterator &itr) {
			return *(itr.blockItr);
		}
		pointer operator->() { return *(*this); }
		friend class blockList;
	};
	typedef const iterator const_iterator;
private:
	int blockDefaultSize, blockCurrentSize;
	block<block<T> *> blockTable;
	iterator start, finish;
public:
	iterator begin() { return start; }
	iterator end() { return finish; }
	value_type& front() { return *start; }
	value_type& back() { return *(finish - 1); }
	const_iterator begin() const { return start; }
	const_iterator end() const { return end; }
	int size() const { return (finish - start); }
	blockList();
	blockList(int);
	blockList(int, value_type*, value_type*);
	blockList(const blockList<T> &);
	void insert(iterator, const value_type &);
	void insert(iterator, iterator, iterator);
	void insert(iterator, value_type*, value_type*);
	void erase(iterator);
	void erase(iterator, iterator);
	void push_back(const value_type &);
	void push_front(const value_type &);
	void pop_back();
	void pop_front();
	void clear();
	void compress(int _newDefaultSize = 0);
	bool empty() { return (start == finish); }
	blockList<T>& operator=(const blockList<T> &);
	value_type& operator[](int i) { return *(start + i); }
private:
	void _refresh_iterator();
	friend class iterator;
	friend class block<T>;
};

//blockList成员函数定义
template <class T>
T& operator*(const typename blockList<T>::iterator &itr) {
	return *(itr.blockItr);
}

template <class T>
blockList<T>::blockList() : blockTable(BLOCK_DEFAULT_SIZE), blockDefaultSize(BLOCK_DEFAULT_SIZE), blockCurrentSize(BLOCK_DEFAULT_SIZE), start(this, this->blockTable.begin(), 0), finish(this, this->blockTable.begin(), 0) {}

template <class T>
blockList<T>::blockList(int _blockSize) : blockTable(BLOCK_DEFAULT_SIZE), blockDefaultSize(_blockSize), blockCurrentSize(_blockSize), start(this, this->blockTable.begin(), 0), finish(this, this->blockTable.begin(), 0) {}

template <class T>
blockList<T>::blockList(int _blockSize, value_type *valStart, value_type *valFinish) : blockTable(BLOCK_DEFAULT_SIZE), blockDefaultSize(_blockSize), blockCurrentSize(_blockSize), start(this, this->blockTable.begin(), 0), finish(*this, this->blockTable.begin(), 0) {
	value_type *valCur = valStart;
	while (valCur != valFinish) {
		if (valCur + blockCurrentSize < valFinish) {
			blockTable._auto_push_back(new block<T>(blockCurrentSize, valCur, valCur + blockCurrentSize));
			valCur = valCur + blockCurrentSize;
			blockCurrentSize *= NEW_BLOCK_EXPANSION_RATE;
		}
		else {
			blockTable._auto_push_back(new block<T>(blockCurrentSize, valCur, valFinish));
			valCur = valFinish;
		}
	}
	_refresh_iterator();
}

template <class T>
blockList<T>::blockList(const blockList<T> &_blockList) : blockTable(_blockList.blockTable), blockDefaultSize(_blockList.blockDefaultSize), blockCurrentSize(_blockList.blockCurrentSize), start(this, this->blockTable.begin(), 0), finish(this, this->blockTable.begin(), 0) {
	for (block_table_itr i = blockTable.begin(); i < blockTable.end(); i++) *i = new block<T>(*i);
	_refresh_iterator();
}

template <class T>
void blockList<T>::insert(iterator itr, const value_type& val) {
	if (this->empty()) {
		blockTable._auto_push_back(new block<T>(blockCurrentSize));
		blockTable[0]->push_back(val);
		blockCurrentSize = NEW_BLOCK_EXPANSION_RATE * blockCurrentSize;
		_refresh_iterator();
		return;
	}
	switch ((*(itr.blockTableItr))->insert(itr.blockItr, val)) {
	case blockSignal::back_failed:
		itr.blockTableItr = blockTable._auto_insert(itr.blockTableItr + 1, new block<T>(blockCurrentSize, (*(itr.blockTableItr))->middle(), (*(itr.blockTableItr))->end())) - 2;
		if (itr.blockItr > (*(itr.blockTableItr))->middle()) {
			(*(itr.blockTableItr + 1))->insert((*(itr.blockTableItr + 1))->begin() + (itr.blockItr - (*(itr.blockTableItr))->middle()), val);
			(*(itr.blockTableItr))->erase((*(itr.blockTableItr))->middle(), (*(itr.blockTableItr))->end());
		}
		else {
			(*(itr.blockTableItr))->erase((*(itr.blockTableItr))->middle(), (*(itr.blockTableItr))->end());
			(*(itr.blockTableItr))->insert(itr.blockItr, val);
		}
		blockCurrentSize = NEW_BLOCK_EXPANSION_RATE * blockCurrentSize;
		break;
	case blockSignal::front_failed:
		itr.blockTableItr = blockTable._auto_insert(itr.blockTableItr, new block<T>(blockCurrentSize, (*(itr.blockTableItr))->begin(), (*(itr.blockTableItr))->middle()));
		if (itr.blockItr < (*(itr.blockTableItr))->middle()) {
			(*(itr.blockTableItr - 1))->insert((*(itr.blockTableItr - 1))->begin() + (itr.blockItr - (*(itr.blockTableItr))->begin()), val);
			(*(itr.blockTableItr))->erase((*(itr.blockTableItr))->begin(), (*(itr.blockTableItr))->middle());
		}
		else {
			(*(itr.blockTableItr))->erase((*(itr.blockTableItr))->begin(), (*(itr.blockTableItr))->middle());
			(*(itr.blockTableItr))->insert(itr.blockItr, val);
		}
		blockCurrentSize = NEW_BLOCK_EXPANSION_RATE * blockCurrentSize;
		break;
	}
	_refresh_iterator();
}

template <class T>
void blockList<T>::insert(iterator itr, iterator valStart, iterator valFinish) {
	if (valFinish == valStart) return;
	int valSize = valFinish - valStart;
	value_type* _array = new value_type[valSize];
	value_type *j = _array;
	for (iterator i = valStart; i < valFinish; i++, j++) *j = *i;
	insert(itr, _array, _array + valSize);
	delete[] _array;
}

template <class T>
void blockList<T>::insert(iterator itr, value_type* valStart, value_type* valFinish) {
	if (valStart == valFinish) return;
	int valSize = valFinish - valStart, leftSize = valSize;
	if (this->empty()) {
		while (leftSize > 0) {
			if (leftSize > blockCurrentSize) {
				blockTable._auto_push_back(new block<T>(blockCurrentSize, valFinish - leftSize, valFinish - leftSize + blockCurrentSize));
				leftSize -= blockCurrentSize;
				blockCurrentSize = NEW_BLOCK_EXPANSION_RATE * blockCurrentSize;
			}
			else {
				blockTable._auto_push_back(new block<T>(blockCurrentSize, valFinish - leftSize, valFinish));
				leftSize = 0;
				blockCurrentSize = NEW_BLOCK_EXPANSION_RATE * blockCurrentSize;
			}
		}
		_refresh_iterator();
		return;
	}
	int arraySize = (*(itr.blockTableItr))->end() - itr.blockItr, backAvailableSize = 0;
	int leftArraySize = arraySize;
	value_type* _array = new value_type[arraySize], *curPointer = valStart;
	block_table_itr curInsertTable = itr.blockTableItr;
	block_itr curInsertPosition = itr.blockItr;
	for (int i = 0; i < arraySize; i++) _array[i] = *(curInsertPosition + i);
	(*curInsertTable)->finish = itr.blockItr;
	if (curInsertTable != itr.iBlockList->blockTable.end() - 1) backAvailableSize = (*(curInsertTable + 1))->start - (*(curInsertTable + 1))->storageStart;
	while (leftSize + leftArraySize > backAvailableSize) {
		if (curInsertPosition == (*curInsertTable)->storageFinish) {
			curInsertTable = blockTable._auto_insert(curInsertTable + 1, new block<T>(blockCurrentSize)) - 1;
			curInsertPosition = (*curInsertTable)->storageStart;
			(*curInsertTable)->start = (*curInsertTable)->finish = (*curInsertTable)->storageStart;
			blockCurrentSize = NEW_BLOCK_EXPANSION_RATE * blockCurrentSize;
		}
		if (leftSize > 0) {
			*curInsertPosition = *(valFinish - leftSize);
			leftSize--;
			(*curInsertTable)->finish++;
		} else {
			*curInsertPosition = _array[arraySize - leftArraySize];
			leftArraySize--;
			(*curInsertTable)->finish++;
		}
		curInsertPosition++;
	}
	if (backAvailableSize > 0) {
		curInsertTable += 1;
		(*curInsertTable)->start = (*curInsertTable)->storageStart;
		curInsertPosition = (*curInsertTable)->start;
		while (leftSize + leftArraySize > 0) {
			if (leftSize > 0) {
				*curInsertPosition = *(valFinish - leftSize);
				leftSize--;
			}
			else {
				*curInsertPosition = _array[arraySize - leftArraySize];
				leftArraySize--;
			}
			curInsertPosition++;
		}
	}
	_refresh_iterator();
}

template <class T>
void blockList<T>::erase(iterator itr) {
	if ((*(itr.blockTableItr))->erase(itr.blockItr) == blockSignal::empty) {
		(*itr.blocTableItr)->~block();
		itr.iBlockList->blockTable.erase(itr.blockTableItr);
	}
	_refresh_iterator();
}

template <class T>
void blockList<T>::erase(iterator itr1, iterator itr2) {
	if (itr1.blockTableItr == itr2.blockTableItr) {
		if ((*(itr1.blockTableItr))->erase(itr1.blockItr, itr2.blockItr) == blockSignal::empty) {
			(*(itr1.blockTableItr))->~block();
			itr1.iBlockList->blockTable.erase(itr1.blockTableItr);
		}
	}
	else {
		if ((*(itr1.blockTableItr))->erase(itr1.blockItr, (*(itr1.blockTableItr))->end()) == blockSignal::empty) {
			(*(itr1.blockTableItr))->~block();
			switch (itr1.iBlockList->blockTable.erase(itr1.blockTableItr)) {
			case blockSignal::back_succeed:
				itr2.blockTableItr--;
				break;
			case blockSignal::front_succeed:
				itr1.blockTableItr++;
				break;
			}
		}
		if ((*(itr2.blockTableItr))->erase((*(itr2.blockTableItr))->begin(), itr2.blockItr) == blockSignal::empty) {
			(*(itr2.blockTableItr))->~block();
			switch (itr2.iBlockList->blockTable.erase(itr2.blockTableItr)) {
			case blockSignal::back_succeed:
				itr2.blockTableItr--;
				break;
			case blockSignal::front_succeed:
				itr1.blockTableItr++;
				break;
			}
		}
		for (block_table_itr _itr = itr1.blockTableItr; _itr < itr2.blockTableItr; _itr++) (*_itr)->~block();
		itr1.iBlockList->blockTable.erase(itr1.blockTableItr, itr2.blockTableItr);
	}
	_refresh_iterator();
}

template <class T>
void blockList<T>::push_back(const value_type &val) {
	insert(this->end(), val);
}

template <class T>
void blockList<T>::push_front(const value_type &val) {
	insert(this->begin(), val);
}

template <class T>
void blockList<T>::pop_back() {
	erase(this->end() - 1);
}

template <class T>
void blockList<T>::pop_front() {
	erase(this->begin());
}

template <class T>
void blockList<T>::clear() {
	for (block_table_itr i = blockTable.begin(); i < blockTable.end(); i++) (*i)->~block();
	delete[] blockTable.blockData;
	blockTable.blockData = new block<T>*[BLOCK_DEFAULT_SIZE];
	blockTable.blockSize = BLOCK_DEFAULT_SIZE;
	blockTable.storageStart = blockTable.blockData;
	blockTable.storageFinish = blockTable.blockData + blockTable.blockSize;
	blockTable.start = blockTable.finish = blockTable.storageStart + blockTable.blockSize / 2;
	blockCurrentSize = blockDefaultSize;
	_refresh_iterator();
}

template <class T>
void blockList<T>::compress(int _newDefaultSize = 0) {
	if (blockTable.empty()) return;
	if (_newDefaultSize) blockDefaultSize = _newDefaultSize;
	blockCurrentSize = blockDefaultSize;
	int _compressCount = 1 + ((finish - start - 1) / blockDefaultSize);
	block<T>* *_compressTable = new block<T>*[_compressCount];
	T* _array = new T[blockDefaultSize];
	iterator _itr = start;
	for (int i = 0; i < _compressCount; i++) {
		int _size = 0;
		for (int j = i * blockDefaultSize; j < (i + 1) * blockDefaultSize && j < finish - start; j++) {
			_array[j - i*blockDefaultSize] = *_itr;
			_itr++;
			_size++;
		}
		_compressTable[i] = new block<T>(blockDefaultSize, _array, _array + _size);
	}
	this->clear();
	for (int i = 0; i < _compressCount; i++) blockTable.push_back(_compressTable[i]);
	_refresh_iterator();
}

template <class T>
blockList<T>& blockList<T>::operator=(const blockList<T> &_blockList) {
	for (int i = 0; i < blockTable.size(); i++) (*blockTable[i]).~block();
	blockTable.clear();
	for (int i = 0; i < _blockList.blockTable.size(); i++) blockTable._auto_push_back(new block<T>(_blockList.blockTable[i]));
	blockDefaultSize = _blockList.blockDefaultSize;
	blockCurrentSize = _blockList.blockCurrentSize;
	if (blockTable.empty())
		start = finish = iterator(this, blockTable.begin(), 0); else {
		start = iterator(this, blockTable.begin(), (blockTable.front())->begin());
		finish = iterator(this, blockTable.end() - 1, (blockTable.back())->end());
	}
	return (*this);
}

template <class T>
void blockList<T>::_refresh_iterator() {
	if (blockTable.empty()) {
		start.blockTableItr = finish.blockTableItr = blockTable.begin();
		start.blockItr = finish.blockItr = 0;
		return;
	}
	start.blockTableItr = blockTable.begin();
	start.blockItr = blockTable.front()->begin();
	finish.blockTableItr = (blockTable.end() - 1);
	finish.blockItr = blockTable.back()->end();
}
#endif

